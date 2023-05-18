/// \file AssetCompiler.c
/// \author Paolo Mazzon
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <VK2D/Util.h>

#include "src/cJSON.h"
#include "src/Runtime.h"
#include "src/Validation.h"
#include "src/ConfigFile.h"

// -------------------- NEW ASSET COMPILER -------------------- //
//
// 1. Recursive for sub-directories
// 2. Pulls asset details from a .json file per directory
// 3. Can pull sprite details from Aseprite sprite json data
// 4. Directories/files checked against an exclude list from directory's json file
// 5. Sub-directories available in Wren through other classes, for example
//    "dir/sprites/file.png" would be `Assets.dir.sprites.spr_file`
// 6. Can specify arbitrary files to load a buffers as well
//
// Example Assets.wren given the directory structure of assets/
//
// | dir/
// |    | sprites/
// |    |        | player.png
// |    |        | player_run.png
// |    |        | assets.json
// |    | jump.ogg
// | banner.jpg
//
//
//     import "lib/Drawing" for Texture, Sprite, BitmapFont, Font
//     import "lib/Audio" for AudioData
//
//     class dirspritesImpl {
//         tex_player { _tex_player }
//         spr_player { _spr_player }
//         tex_player_run { _tex_player_run }
//         spr_player_run { _spr_player_run }
//
//         construct new(asset_map) {
//             _tex_player = Texture.new("assets/dir/sprites/player.png")
//             asset_map["dir.sprites.tex_player"] = _tex_player
//             _spr_player = Sprite.from(_tex_player, ...)
//             asset_map["dir.sprites.spr_player"] = _spr_player
//             _tex_player_run = Texture.new("assets/dir/sprites/player_run.png")
//             asset_map["dir.sprites.tex_player_run"] = _tex_player_run
//             _spr_player_run = Sprite.from(_tex_player_run, ...)
//             asset_map["dir.sprites.spr_player_run"] = _spr_player_run
//         }
//     }
//
//     class dirImpl {
//         sprites { _sprites }
//         aud_jump { _aud_jump }
//
//         construct new(asset_map) {
//             _sprites = dirspritesImpl.new(asset_map)
//             _aud_jump = AudioData.open("assets/dir/jump.ogg")
//             asset_map["dir.aud_jump"] = _aud_jump
//         }
//     }
//
//     class AssetsImpl {
//         dir { _dir }
//         tex_banner { _tex_banner }
//
//         construct new() {}
//
//         load_assets() {
//             _asset_map = {}
//             var asset_map = _asset_map
//             _dir = dirImpl.new(asset_map)
//             _tex_banner = Texture.new("assets/banner.jpg")
//             asset_map["tex_banner"] = _tex_banner
//         }
//
//         [asset] {
//             return _asset_map[asset]
//         }
//     }
//
//     var Assets = AssetsImpl.new()
//
// ------------------------------- Constants ------------------------------- //

#define STRING_BUFFER_SIZE (200)
#define HAS_REQUIRED_DATA(file, name) (cJSON_IsString(file) && cJSON_IsString(name))
#define GET_JSON_DOUBLE(item, def) (cJSON_IsNumber(item) ? cJSON_GetNumberValue(item) : def)
#define GET_JSON_STRING(item, def) (cJSON_IsString(item) ? cJSON_GetStringValue(item) : def)
#define GET_JSON_BOOL(item, def) (cJSON_IsBool(item) ? cJSON_IsTrue(item) : def)

const char *OUTPUT_HEADER = "import \"lib/Drawing\" for Texture, Sprite, BitmapFont, Font\nimport \"lib/Audio\" for AudioData"
							"\n"
							"class Assets {\n";

const char *OUTPUT_FOOTER = "\n    static [asset] {\n"
							"        return __asset_map[asset]\n"
							"    }\n"
	   						"}\n";

const char *LOAD_HEADER = "    static load_assets() {\n"
						  "        __asset_map = {}\n";
const char *LOAD_FOOTER = "    }\n\n";

const char *ASSET_FILE_HEADER = "import \"lib/Drawing\" for Texture, Sprite, BitmapFont, Font\nimport \"lib/Audio\" for AudioData\nimport \"File\" for File\n\n";
const char *ASSET_FILE_FOOTER = "\nvar Assets = AssetsImpl.new()\n";
const char *ASSET_ASSET_CLASS_HEADER = "class AssetsImpl {\n\tconstruct new() {}\n\n";
const char *ASSET_ASSET_CLASS_FOOTER = "\n\t[asset] {\n\t\treturn _asset_map[asset]\n\t}\n}\n";
const char *ASSET_DIR_CLASS_HEADER = "\n";
const char *ASSET_DIR_CLASS_FOOTER = "}\n\n";

const char* loadFile(const char *filename);

// ------------------------------- Strings ------------------------------- //
typedef struct String {
	char *str;
	int len;
} *String;

static String newString() {
	String s = malloc(sizeof (struct String));
	s->len = 0;
	s->str = NULL;
	return s;
}

// Returns the same string just for convenience
static String appendString(String s, const char *new) {
	if (new != NULL) {
		int len = strlen(new);
		s->str = realloc(s->str, s->len + len + 1);
		s->str[s->len] = 0;
		strcat(s->str, new);
		s->len += len;
	}
	return s;
}

// Returns the same string just for convenience, frees the passed string
static String appendStringAndFree(String s, const char *new) {
	if (new != NULL) {
		int len = strlen(new);
		s->str = realloc(s->str, s->len + len + 1);
		s->str[s->len] = 0;
		strcat(s->str, new);
		s->len += len;
		free((void*)new);
	}
	return s;
}

static void freeString(String s) {
	if (s != NULL)
		free(s->str);
	free(s);
}

// Only frees the surrounding stuff returning the internal string (which will need to be freed later)
static const char *popString(String s) {
	const char *ss = s->str;
	free(s);
	return ss;
}

// ------------------------------- JSON Parsers ------------------------------- //
typedef struct SpriteData {
	const char *filename;
	const char *name; // dont pull this directly, use jsonGetSpriteName
	double x;
	double y;
	double originX;
	double originY;
	double w;
	double h;
	double frames;
	double delay;
} SpriteData;

typedef struct BitmapFontData {
	const char *filename;
	const char *name;
	int ustart;
	int uend;
	double w;
	double h;
} BitmapFontData;

typedef struct TrueTypeFontData {
	const char *filename;
	const char *name;
	int ustart;
	int uend;
	double size;
	bool aa;
} TrueTypeFontData;

typedef struct BufferData {
	const char *filename;
	const char *name;
} BufferData;

typedef struct StringData {
	const char *filename;
	const char *name;
} StringData;

typedef struct DirectoryJSON {
	cJSON *root; // json file for the directory's asset.json
	cJSON *excludeList; // json array of exclusions
	cJSON *spritesPointer; // json for the sprite list
	cJSON *fontsPointer; // json for the fonts list
	cJSON *bitmapFontsPointer; // json for the bitmap fonts list
	cJSON *buffersPointer; // json for the buffers list
	cJSON *stringsPointer; // json for the strings list
} *DirectoryJSON;

// Either copies the asset's name into the buffer or forms a name from the filename using
// the filename without an extension or path
static const char *jsonGetAssetName(const char *filename, const char *name, char *buffer, int bufferSize) {
	if (name != NULL) {
		strncpy(buffer, name, bufferSize);
		buffer[strlen(name) >= bufferSize ? bufferSize - 1 : strlen(name)] = 0;
	} else {
		const char *baseName = strrchr(filename, '/');
		baseName = baseName == NULL ? filename : baseName + 1;
		strncpy(buffer, baseName, bufferSize);
		char *ext = strrchr(buffer, '.');
		*ext = 0;
	}

	return buffer;
}

// Returns true if it finds the same string in a cjson list
static bool jsonItemInExcludeList(DirectoryJSON json, const char *string) {
	if (json == NULL)
		return false;
	cJSON *list = json->excludeList;

	// Start from the first child
	if (list == NULL || !cJSON_IsArray(list))
		return false;
	list = list->child;

	// Loop through each child
	while (list != NULL) {
		if (cJSON_IsString(list) && strcmp(cJSON_GetStringValue(list), string) == 0)
			return true;
		list = list->next;
	}
	return false;
}

// Will attempt to locate a corresponding .json sprite data file exported from Aseprite and
// if it does will fill out sprite with its data. Returns true if it found one and false otherwise.
static bool jsonFindAsepriteData(const char *tex_filename, SpriteData *sprite) {
	char jsonFilename[STRING_BUFFER_SIZE];
	if (strlen(tex_filename) < STRING_BUFFER_SIZE - 5) {
		// Create the .json extension version of the filename
		char *ext = strrchr(tex_filename, '.');
		strncpy(jsonFilename, tex_filename, ext - tex_filename);
		strcpy(jsonFilename + (ext - tex_filename), ".json");
		jsonFilename[(ext - tex_filename) + 5] = 0;
		const char *file = loadFile(jsonFilename);
		cJSON *json = cJSON_Parse(file);
		free((void*)file);

		// We have a json to work with
		if (json != NULL) {
			int frameCount = 0;
			cJSON *frames = cJSON_GetObjectItem(json, "frames");

			if (frames != NULL && frames->child != NULL) {
				cJSON *frame;
				frames = frames->child;
				while (frames != NULL) {
					frameCount += 1;
					frame = frames;
					frames = frames->next;
				}

				// We now have a frame and frame count
				sprite->frames = frameCount;
				sprite->filename = tex_filename;
				sprite->name = NULL;

				// Extract basic data
				cJSON *duration = cJSON_GetObjectItem(frame, "duration");
				cJSON *wh = cJSON_GetObjectItem(frame, "sourceSize");
				sprite->delay = GET_JSON_DOUBLE(duration, 0) / 1000;
				sprite->x = 0;
				sprite->y = 0;
				sprite->w = 0;
				sprite->h = 0;
				sprite->originY = 0;
				sprite->originY = 0;

				// Extract width/height from "sourceSize"
				if (wh != NULL) {
					cJSON *w = cJSON_GetObjectItem(wh, "w");
					cJSON *h = cJSON_GetObjectItem(wh, "h");
					sprite->w = GET_JSON_DOUBLE(w, 0);
					sprite->h = GET_JSON_DOUBLE(h, 0);
				}

				cJSON_Delete(json);
				return true;
			}

			cJSON_Delete(json);
		}
	}
	return false;
}

// Will attempt to open the assets.json file for a directory, if it doesn't find one it will return
// null. Other directory json functions won't crash on the null either.
static DirectoryJSON openDirectoryJSON(const char *directory) {
	char assetFile[STRING_BUFFER_SIZE];
	snprintf(assetFile, STRING_BUFFER_SIZE, "%sassets.json", directory);
	DirectoryJSON dir = NULL;

	if (_vk2dFileExists(assetFile)) {
		dir = malloc(sizeof(struct DirectoryJSON));
		const char *file = loadFile(assetFile);
		cJSON *json = cJSON_Parse(file);

		if (json != NULL && dir != NULL) {
			dir->root = json;
			dir->excludeList = cJSON_GetObjectItem(json, "exclude");
			dir->spritesPointer = cJSON_GetObjectItem(json, "sprites");
			dir->fontsPointer = cJSON_GetObjectItem(json, "fonts");
			dir->bitmapFontsPointer = cJSON_GetObjectItem(json, "bitmap_fonts");
			dir->buffersPointer = cJSON_GetObjectItem(json, "buffers");
			dir->stringsPointer = cJSON_GetObjectItem(json, "strings");
			dir->spritesPointer = dir->spritesPointer != NULL && cJSON_IsArray(dir->spritesPointer) ? dir->spritesPointer->child : NULL;
			dir->fontsPointer = dir->fontsPointer != NULL && cJSON_IsArray(dir->fontsPointer) ? dir->fontsPointer->child : NULL;
			dir->bitmapFontsPointer = dir->bitmapFontsPointer != NULL && cJSON_IsArray(dir->bitmapFontsPointer) ? dir->bitmapFontsPointer->child : NULL;
			dir->buffersPointer = dir->buffersPointer != NULL && cJSON_IsArray(dir->buffersPointer) ? dir->buffersPointer->child : NULL;
			dir->spritesPointer = dir->spritesPointer != NULL && cJSON_IsArray(dir->spritesPointer) ? dir->spritesPointer->child : NULL;
		} else {
			free(dir);
			dir = NULL;
		}
		free((void*)file);
	}

	return dir;
}

static void closeDirectoryJSON(DirectoryJSON json) {
	if (json != NULL)
		cJSON_Delete(json->root);
	free(json);
}

// Attempts to find the next sprite in a directory json, returns true if it found one and filled out
// the sprite data pointer.
static bool directoryJSONGetNextSprite(DirectoryJSON json, SpriteData *sprite) {
	if (json != NULL && json->spritesPointer != NULL) {
		cJSON *file = cJSON_GetObjectItem(json->spritesPointer, "file");
		cJSON *name = cJSON_GetObjectItem(json->spritesPointer, "name");
		cJSON *jsonX = cJSON_GetObjectItem(json->spritesPointer, "x");
		cJSON *jsonY = cJSON_GetObjectItem(json->spritesPointer, "y");
		cJSON *jsonOriginX = cJSON_GetObjectItem(json->spritesPointer, "origin_x");
		cJSON *jsonOriginY = cJSON_GetObjectItem(json->spritesPointer, "origin_y");
		cJSON *jsonW = cJSON_GetObjectItem(json->spritesPointer, "w");
		cJSON *jsonH = cJSON_GetObjectItem(json->spritesPointer, "h");
		cJSON *jsonFrames = cJSON_GetObjectItem(json->spritesPointer, "frames");
		cJSON *jsonDelay = cJSON_GetObjectItem(json->spritesPointer, "delay");
		if (HAS_REQUIRED_DATA(file, name)) {
			sprite->filename = GET_JSON_STRING(file, "");
			sprite->name = GET_JSON_STRING(name, "");
			sprite->x = GET_JSON_DOUBLE(jsonX, 0);
			sprite->y = GET_JSON_DOUBLE(jsonY, 0);
			sprite->originX = GET_JSON_DOUBLE(jsonOriginX, 0);
			sprite->originY = GET_JSON_DOUBLE(jsonOriginY, 0);
			sprite->w = GET_JSON_DOUBLE(jsonW, 0);
			sprite->h = GET_JSON_DOUBLE(jsonH, 0);
			sprite->frames = GET_JSON_DOUBLE(jsonFrames, 1);
			sprite->delay = GET_JSON_DOUBLE(jsonDelay, 0);
			json->spritesPointer = json->spritesPointer->next;
			return true;
		}
		json->spritesPointer = json->spritesPointer->next;
	}
	return false;
}

// Attempts to find the next bitmap font in a directory json, returns true if it found one and filled out
// the bitmap font data pointer.
static bool directoryJSONGetNextBitmapFont(DirectoryJSON json, BitmapFontData *bmpfont) {
	if (json != NULL && json->bitmapFontsPointer != NULL) {
		cJSON *file = cJSON_GetObjectItem(json->bitmapFontsPointer, "file");
		cJSON *name = cJSON_GetObjectItem(json->bitmapFontsPointer, "name");
		cJSON *ustart = cJSON_GetObjectItem(json->bitmapFontsPointer, "ustart");
		cJSON *uend = cJSON_GetObjectItem(json->bitmapFontsPointer, "uend");
		cJSON *w = cJSON_GetObjectItem(json->bitmapFontsPointer, "w");
		cJSON *h = cJSON_GetObjectItem(json->bitmapFontsPointer, "h");
		if (HAS_REQUIRED_DATA(file, name)) {
			bmpfont->filename = GET_JSON_STRING(file, "");
			bmpfont->name = GET_JSON_STRING(name, "");
			bmpfont->ustart = GET_JSON_DOUBLE(ustart, 32);
			bmpfont->uend = GET_JSON_DOUBLE(uend, 128);
			bmpfont->w = GET_JSON_DOUBLE(w, 0);
			bmpfont->h = GET_JSON_DOUBLE(h, 0);
			json->bitmapFontsPointer = json->bitmapFontsPointer->next;
			return true;
		}
		json->bitmapFontsPointer = json->bitmapFontsPointer->next;
	}
	return false;
}

// Attempts to find the next true type font in a directory json, returns true if it found one and filled out
// the true type font data pointer.
static bool directoryJSONGetNextTrueTypeFont(DirectoryJSON json, TrueTypeFontData *ttf) {
	if (json != NULL && json->fontsPointer != NULL) {
		cJSON *file = cJSON_GetObjectItem(json->fontsPointer, "file");
		cJSON *name = cJSON_GetObjectItem(json->fontsPointer, "name");
		cJSON *ustart = cJSON_GetObjectItem(json->fontsPointer, "ustart");
		cJSON *uend = cJSON_GetObjectItem(json->fontsPointer, "uend");
		cJSON *size = cJSON_GetObjectItem(json->fontsPointer, "size");
		cJSON *aa = cJSON_GetObjectItem(json->fontsPointer, "aa");
		if (HAS_REQUIRED_DATA(file, name)) {
			ttf->filename = GET_JSON_STRING(file, "");
			ttf->name = GET_JSON_STRING(name, "");
			ttf->ustart = GET_JSON_DOUBLE(ustart, 32);
			ttf->uend = GET_JSON_DOUBLE(uend, 128);
			ttf->size = GET_JSON_DOUBLE(size, 16);
			ttf->aa = GET_JSON_BOOL(aa, true);
			json->fontsPointer = json->fontsPointer->next;
			return true;
		}
		json->fontsPointer = json->fontsPointer->next;
	}
	return false;
}

// Attempts to find the next buffer in a directory json, returns true if it found one and filled out
// the buffer data pointer.
static bool directoryJSONGetNextBuffer(DirectoryJSON json, BufferData *buffer) {
	if (json != NULL && json->buffersPointer != NULL) {
		cJSON *file = cJSON_GetObjectItem(json->buffersPointer, "file");
		cJSON *name = cJSON_GetObjectItem(json->buffersPointer, "name");
		if (HAS_REQUIRED_DATA(file, name)) {
			buffer->filename = GET_JSON_STRING(file, "");
			buffer->name = GET_JSON_STRING(name, "");
			json->buffersPointer = json->buffersPointer->next;
			return true;
		}
		json->buffersPointer = json->buffersPointer->next;
	}
	return false;
}

// Attempts to find the next string in a directory json, returns true if it found one and filled out
// the string data pointer.
static bool directoryJSONGetNextString(DirectoryJSON json, StringData *string) {
	if (json != NULL && json->stringsPointer != NULL) {
		cJSON *file = cJSON_GetObjectItem(json->stringsPointer, "file");
		cJSON *name = cJSON_GetObjectItem(json->stringsPointer, "name");
		if (HAS_REQUIRED_DATA(file, name)) {
			string->filename = GET_JSON_STRING(file, "");
			string->name = GET_JSON_STRING(name, "");
			json->stringsPointer = json->stringsPointer->next;
			return true;
		}
		json->stringsPointer = json->stringsPointer->next;
	}
	return false;
}

// ------------------------------- Helpers ------------------------------- //

// Returns true if the string is a valid Wren variable name
static bool variableNameIsValid(const char *s) {
	if (*s >= '0' && *s <= '9')
		return false;
	while (*s != 0) {
		if (!((*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z') || (*s >= '0' && *s <= '9') || *s == '_'))
			return false;
		s++;
	}
	return true;
}

// Returns the directory or filename, eg "assets/thing/" would return "thing/" and "assets/thing/text.txt"
// would return "text.txt". Does not allocate memory, just returns a pointer to the string given.
// Excludes the root directory and will return "" if its the root directory
static const char *getDirOrFile(const char *fullname) {
	int len = strlen(fullname);
	if (fullname[len - 1] == '/') {
		const char *p = NULL;
		for (int i = len - 2; i > 0; i--) {
			if (fullname[i] == '/')
				p = &fullname[i + 1];
		}
		return p == NULL ? "" : p;
	} else {
		return strrchr(fullname, '/') + 1;
	}
}

// ------------------------------- Asset compiler ------------------------------- //

// TODO: A function that adds a sprite from spritedata to the getter and class strings, same for other types
static void _vksk_CompileAssetFromSpriteData(String getter, String method, SpriteData *sprite) {
	// TODO: This
}

static void _vksk_CompileAssetFromBitmapFontData(String getter, String method, BitmapFontData *sprite) {
	// TODO: This
}

static void _vksk_CompileAssetFromTrueTypeData(String getter, String method, TrueTypeFontData *sprite) {
	// TODO: This
}

static void _vksk_CompileAssetFromBufferData(String getter, String method, BufferData *sprite) {
	// TODO: This
}

static void _vksk_CompileAssetFromStringData(String getter, String method, StringData *sprite) {
	// TODO: This
}


static void _vksk_CompileAssetFromFilename(String getter, String method, const char *path, const char *filename) {
	const char *extension = strrchr(filename, '.');
	char nameBuffer[STRING_BUFFER_SIZE];
	char output[STRING_BUFFER_SIZE];
	const char *pathNoRoot;
	const char *getterPrefix = NULL;

	if (extension != NULL) {
		pathNoRoot = strchr(path, '/') + 1;

		if (strcmp(extension, ".bmp") == 0 || strcmp(extension, ".png") == 0 ||
			strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0) {
			jsonGetAssetName(filename, NULL, nameBuffer, STRING_BUFFER_SIZE);
			snprintf(
					output,
					STRING_BUFFER_SIZE,
					"\t\t_%s = Texture.new(\"%s\")\n\t\tasset_map[\"%s\"] = _%s\n",
					nameBuffer,
					path,
					pathNoRoot,
					nameBuffer
			);
			appendString(method, output);

			// Check for a sprite json
			SpriteData sprite;
			if (jsonFindAsepriteData(path, &sprite)) {
				_vksk_CompileAssetFromSpriteData(getter, method, &sprite);
			}
			getterPrefix = "spr_";
		} else if (strcmp(extension, ".ogg") == 0 || strcmp(extension, ".wav") == 0) {
			jsonGetAssetName(filename, NULL, nameBuffer, STRING_BUFFER_SIZE);
			snprintf(
					output,
					STRING_BUFFER_SIZE,
					"\t\t_%s = AudioData.open(\"%s\")\n\t\tasset_map[\"%s\"] = _%s\n",
					nameBuffer,
					path,
					pathNoRoot,
					nameBuffer
			);
			appendString(method, output);
			getterPrefix = "aud_";
		} else if (strcmp(extension, ".txt") == 0) {
			jsonGetAssetName(filename, NULL, nameBuffer, STRING_BUFFER_SIZE);
			snprintf(
					output,
					STRING_BUFFER_SIZE,
					"\t\t_str_%s = File.read(\"%s\")\n\t\tasset_map[\"%s\"] = _str_%s\n",
					nameBuffer,
					path,
					pathNoRoot,
					nameBuffer
			);
			appendString(method, output);
			getterPrefix = "str_";
		}

		if (getterPrefix != NULL) {
			snprintf(
					output,
					STRING_BUFFER_SIZE,
					"\t%s%s { _%s%s }\n",
					getterPrefix,
					nameBuffer,
					getterPrefix,
					nameBuffer
			);
			appendString(getter, output);
		}
	}
}

// Compiles the root directory, returning the code for that directory's class
static String _vksk_CompileAssetsFromDirectory(const char *directory, const char *topOfClassString, const char *loadMethodString, const char *footerString) {
	struct dirent *dp;
	DIR *dfd;
	DirectoryJSON json = openDirectoryJSON(directory);
	String topOfClass = appendString(newString(), topOfClassString); // for getters
	String loadMethod = appendString(newString(), loadMethodString); // for the actual load method

	if ((dfd = opendir(directory)) == NULL) {
		vksk_Log("Can't access assets directory");
		return NULL;
	}

	char filedir[STRING_BUFFER_SIZE] ;

	dp = readdir(dfd);
	while (dp != NULL) {
		struct stat stbuf ;
		snprintf(filedir, STRING_BUFFER_SIZE, "%s%s", directory, dp->d_name);
		if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0 || stat(filedir, &stbuf) == -1) {
			dp = readdir(dfd);
			continue;
		}

		// Put an / at the end of a directory
		if ((stbuf.st_mode & S_IFMT) == S_IFDIR) {
			int len = strlen(filedir);
			if (len < STRING_BUFFER_SIZE - 2) {
				filedir[len] = '/';
				filedir[len + 1] = 0;
			}
		}
		char *filedirnolead = (void*)getDirOrFile(filedir);

		if (!jsonItemInExcludeList(json, filedirnolead)) {
			if ((stbuf.st_mode & S_IFMT) != S_IFDIR) {
				_vksk_CompileAssetFromFilename(topOfClass, loadMethod, filedir, filedirnolead);
			} else {
				// Add the class bit to the loader
				filedirnolead[strlen(filedirnolead) - 1] = 0;
				char directoryAssetName[STRING_BUFFER_SIZE];
				snprintf(directoryAssetName, STRING_BUFFER_SIZE, "\t\t_%s = %sImpl.new(asset_map)\n", filedirnolead, filedirnolead);
				appendString(loadMethod, directoryAssetName);

				// Append this class to the new class
				snprintf(directoryAssetName, STRING_BUFFER_SIZE, "class %sImpl {\n", filedirnolead);
				filedirnolead[strlen(filedirnolead)] = '/';
				topOfClass = appendStringAndFree(_vksk_CompileAssetsFromDirectory(filedir, directoryAssetName, "\n\tconstruct new(asset_map) {\n", ASSET_DIR_CLASS_FOOTER), popString(topOfClass));

				// Now the getter
				filedirnolead[strlen(filedirnolead) - 1] = 0;
				snprintf(directoryAssetName, STRING_BUFFER_SIZE, "\t%s { _%s }\n", filedirnolead, filedirnolead);
				appendString(topOfClass, directoryAssetName);
			}
		}
		dp = readdir(dfd);
	}

	// TODO: Load assets from json

	closedir(dfd);
	closeDirectoryJSON(json);

	// Build output string
	appendStringAndFree(topOfClass, popString(appendString(loadMethod, "\t}\n")));
	return appendString(topOfClass, footerString);
}

static bool _vksk_CompileAssetsFromPak(String loadFunction, String getterFunctions, String spriteLoadFunction) {
	return false; // TODO: This
}

const char *vksk_CompileAssetFile(const char *rootDir) {
	String string = appendString(newString(), ASSET_FILE_HEADER);
	appendString(string, popString(_vksk_CompileAssetsFromDirectory(rootDir, ASSET_ASSET_CLASS_HEADER, "\n\tload_assets() {\n\t\t_asset_map = {}\n\t\tvar asset_map = _asset_map\n", ASSET_ASSET_CLASS_FOOTER)));
	appendString(string, ASSET_FILE_FOOTER);
	printf("%s", string->str);
	fflush(stdout);
	return popString(string);
}