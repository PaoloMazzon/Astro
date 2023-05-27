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

const char *ASSET_FILE_HEADER = "import \"lib/Drawing\" for Texture, Sprite, BitmapFont, Font\nimport \"lib/Audio\" for AudioData\nimport \"lib/File\" for File\nimport \"lib/Util\" for Buffer\n\n";
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
			dir->stringsPointer = dir->stringsPointer != NULL && cJSON_IsArray(dir->stringsPointer) ? dir->stringsPointer->child : NULL;
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

static bool variableNameIsValid(const char *);
// TODO: Searching for assets in the json lists shouldn't stop if one of the assets is wrong
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
			if (!variableNameIsValid(sprite->name))
				return false;
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
			if (!variableNameIsValid(bmpfont->name))
				return false;
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
			if (!variableNameIsValid(ttf->name))
				return false;
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
			if (!variableNameIsValid(buffer->name))
				return false;
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
			if (!variableNameIsValid(string->name))
				return false;
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

// Converts a folder into a class name by replacing / with _ into a buffer, returning it
static const char *folderToClass(const char *folder, char *buffer, int bufferSize) {
	int i;
	for (i = 0; i < bufferSize - 1 && i < strlen(folder) - 1; i++) {
		if (folder[i] == '/')
			buffer[i] = '_';
		else
			buffer[i] = folder[i];
	}
	buffer[i] = 0;
	return buffer;
}

// ------------------------------- Asset compiler ------------------------------- //

// TODO: A function that adds a sprite from spritedata to the getter and class strings, same for other types
static void _vksk_CompileAssetFromSpriteData(const char *basePath, String getter, String method, SpriteData *sprite) {
	char nameBuffer[STRING_BUFFER_SIZE];
	char output[STRING_BUFFER_SIZE];
	char fileBuffer[STRING_BUFFER_SIZE];
	char *finalPath = strrchr(sprite->filename, '/');
	if (finalPath != NULL)
		strncpy(fileBuffer, finalPath + 1, STRING_BUFFER_SIZE);
	else
		strncpy(fileBuffer, sprite->filename, STRING_BUFFER_SIZE);
	char *ext = strrchr(fileBuffer, '.');
	if (ext != NULL) {
		*ext = 0;
	}

	jsonGetAssetName(sprite->filename, sprite->name, nameBuffer, STRING_BUFFER_SIZE);
	snprintf(
			output,
			STRING_BUFFER_SIZE,
			"\t\t_spr_%s = Sprite.from(_tex_%s, %.2f, %.2f, %.2f, %.2f, %.2f, %i)\n",
			nameBuffer,
			fileBuffer,
			sprite->x,
			sprite->y,
			sprite->w,
			sprite->h,
			sprite->delay,
			(int)sprite->frames
	);
	appendString(method, output);

	snprintf(
			output,
			STRING_BUFFER_SIZE,
			"\tspr_%s { _spr_%s }\n",
			nameBuffer,
			nameBuffer
	);
	appendString(getter, output);
}

static void _vksk_CompileAssetFromBitmapFontData(const char *basePath, String getter, String method, BitmapFontData *bmp) {
	char output[STRING_BUFFER_SIZE];

	snprintf(
			output,
			STRING_BUFFER_SIZE,
			"\t\t_fnt_%s = BitmapFont.new(\"%s%s\", %i, %i, %.2f, %.2f)\n",
			bmp->name,
			basePath,
			bmp->filename,
			(int)bmp->ustart,
			(int)bmp->uend,
			bmp->w,
			bmp->h
	);
	appendString(method, output);

	snprintf(
			output,
			STRING_BUFFER_SIZE,
			"\tfnt_%s { _fnt_%s }\n",
			bmp->name,
			bmp->name
	);
	appendString(getter, output);
}

static void _vksk_CompileAssetFromTrueTypeData(const char *basePath, String getter, String method, TrueTypeFontData *ttf) {
	char output[STRING_BUFFER_SIZE];

	snprintf(
			output,
			STRING_BUFFER_SIZE,
			"\t\t_fnt_%s = Font.open(\"%s%s\", %.2f, %s, %i, %i)\n\t\tasset_map[\"%s%s\"] = _fnt_%s\n",
			ttf->name,
			basePath,
			ttf->filename,
			ttf->size,
			ttf->aa ? "true" : "false",
			ttf->ustart,
			ttf->uend,
			basePath,
			ttf->filename,
			ttf->name
	);
	appendString(method, output);

	snprintf(
			output,
			STRING_BUFFER_SIZE,
			"\tfnt_%s { _fnt_%s }\n",
			ttf->name,
			ttf->name
	);
	appendString(getter, output);
}

static void _vksk_CompileAssetFromBufferData(const char *basePath, String getter, String method, BufferData *buffer) {
	char output[STRING_BUFFER_SIZE];

	snprintf(
			output,
			STRING_BUFFER_SIZE,
			"\t\t_buf_%s = Buffer.open(\"%s%s\")\n\t\tasset_map[\"%s%s\"] = _buf_%s\n",
			buffer->name,
			basePath,
			buffer->filename,
			basePath,
			buffer->filename,
			buffer->name
	);
	appendString(method, output);

	snprintf(
			output,
			STRING_BUFFER_SIZE,
			"\tbuf_%s { _buf_%s }\n",
			buffer->name,
			buffer->name
	);
	appendString(getter, output);
}

static void _vksk_CompileAssetFromStringData(const char *basePath, String getter, String method, StringData *text) {
	char output[STRING_BUFFER_SIZE];

	snprintf(
			output,
			STRING_BUFFER_SIZE,
			"\t\t_txt_%s = Buffer.open(\"%s%s\")\n\t\tasset_map[\"%s%s\"] = _txt_%s\n",
			text->name,
			basePath,
			text->filename,
			basePath,
			text->filename,
			text->name
	);
	appendString(method, output);

	snprintf(
			output,
			STRING_BUFFER_SIZE,
			"\ttxt_%s { _txt_%s }\n",
			text->name,
			text->name
	);
	appendString(getter, output);
}


static void _vksk_CompileAssetFromFilename(String getter, String method, const char *directory, const char *path, const char *filename) {
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
			if (!variableNameIsValid(nameBuffer))
				return;
			snprintf(
					output,
					STRING_BUFFER_SIZE,
					"\t\t_tex_%s = Texture.new(\"%s\")\n\t\tasset_map[\"%s\"] = _tex_%s\n",
					nameBuffer,
					path,
					pathNoRoot,
					nameBuffer
			);
			appendString(method, output);

			// Check for a sprite json
			SpriteData sprite;
			if (jsonFindAsepriteData(path, &sprite)) {
				_vksk_CompileAssetFromSpriteData(directory, getter, method, &sprite);
			}
			getterPrefix = "tex_";
		} else if (strcmp(extension, ".ogg") == 0 || strcmp(extension, ".wav") == 0) {
			jsonGetAssetName(filename, NULL, nameBuffer, STRING_BUFFER_SIZE);
			if (!variableNameIsValid(nameBuffer))
				return;
			snprintf(
					output,
					STRING_BUFFER_SIZE,
					"\t\t_aud_%s = AudioData.open(\"%s\")\n\t\tasset_map[\"%s\"] = _aud_%s\n",
					nameBuffer,
					path,
					pathNoRoot,
					nameBuffer
			);
			appendString(method, output);
			getterPrefix = "aud_";
		} else if (strcmp(extension, ".txt") == 0) {
			jsonGetAssetName(filename, NULL, nameBuffer, STRING_BUFFER_SIZE);
			if (!variableNameIsValid(nameBuffer))
				return;
			snprintf(
					output,
					STRING_BUFFER_SIZE,
					"\t\t_txt_%s = File.read(\"%s\")\n\t\tasset_map[\"%s\"] = _txt_%s\n",
					nameBuffer,
					path,
					pathNoRoot,
					nameBuffer
			);
			appendString(method, output);
			getterPrefix = "txt_";
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
				_vksk_CompileAssetFromFilename(topOfClass, loadMethod, directory, filedir, filedirnolead);
			} else {
				char classname[STRING_BUFFER_SIZE];
				folderToClass(filedirnolead, classname, STRING_BUFFER_SIZE);
				if (!variableNameIsValid(classname)) {
					dp = readdir(dfd);
					continue;
				}

				// Add the class bit to the loader
				char directoryAssetName[STRING_BUFFER_SIZE];
				snprintf(directoryAssetName, STRING_BUFFER_SIZE, "\t\t_%s = C%sImpl.new(asset_map)\n", classname, classname);
				appendString(loadMethod, directoryAssetName);

				// Append this class to the new class
				snprintf(directoryAssetName, STRING_BUFFER_SIZE, "class C%sImpl {\n", classname);
				topOfClass = appendStringAndFree(_vksk_CompileAssetsFromDirectory(filedir, directoryAssetName, "\n\tconstruct new(asset_map) {\n", ASSET_DIR_CLASS_FOOTER), popString(topOfClass));

				// Now the getter
				snprintf(directoryAssetName, STRING_BUFFER_SIZE, "\t%s { _%s }\n", classname, classname);
				appendString(topOfClass, directoryAssetName);
			}
		}
		dp = readdir(dfd);
	}

	SpriteData sprite;
	while (directoryJSONGetNextSprite(json, &sprite)) {
		_vksk_CompileAssetFromSpriteData(directory, topOfClass, loadMethod, &sprite);
	}
	TrueTypeFontData ttf;
	while (directoryJSONGetNextTrueTypeFont(json, &ttf)) {
		_vksk_CompileAssetFromTrueTypeData(directory, topOfClass, loadMethod, &ttf);
	}
	BitmapFontData bmf;
	while (directoryJSONGetNextBitmapFont(json, &bmf)) {
		_vksk_CompileAssetFromBitmapFontData(directory, topOfClass, loadMethod, &bmf);
	}
	BufferData buffer;
	while (directoryJSONGetNextBuffer(json, &buffer)) {
		_vksk_CompileAssetFromBufferData(directory, topOfClass, loadMethod, &buffer);
	}
	StringData text;
	while (directoryJSONGetNextString(json, &text)) {
		_vksk_CompileAssetFromStringData(directory, topOfClass, loadMethod, &text);
	}

	closedir(dfd);
	closeDirectoryJSON(json);

	// Build output string
	appendStringAndFree(topOfClass, popString(appendString(loadMethod, "\t}\n")));
	return appendString(topOfClass, footerString);
}

// Basically the same as above but uses pak methods instead of dirent
static String _vksk_CompileAssetsFromPak(const char *directory, const char *topOfClassString, const char *loadMethodString, const char *footerString) {
	DirectoryJSON json = openDirectoryJSON(directory);
	String topOfClass = appendString(newString(), topOfClassString); // for getters
	String loadMethod = appendString(newString(), loadMethodString); // for the actual load method
	VKSK_PakDir dir;

	const char *f = vksk_PakBeginLoop(gGamePak, &dir, directory);
	if (f == NULL) {
		vksk_Log("Can't access assets directory");
		return NULL;
	}

	char filedir[STRING_BUFFER_SIZE] ;

	while (f != NULL) {
		snprintf(filedir, STRING_BUFFER_SIZE, "%s", f);

		char *filedirnolead = (void*)getDirOrFile(filedir);

		if (!jsonItemInExcludeList(json, filedirnolead)) {
			if (f[strlen(f) - 1] != '/') {
				_vksk_CompileAssetFromFilename(topOfClass, loadMethod, directory, filedir, filedirnolead);
			} else {
				char classname[STRING_BUFFER_SIZE];
				folderToClass(filedirnolead, classname, STRING_BUFFER_SIZE);
				if (!variableNameIsValid(classname)) {
					f = vksk_PakNext(&dir);
					continue;
				}

				// Add the class bit to the loader
				char directoryAssetName[STRING_BUFFER_SIZE];
				snprintf(directoryAssetName, STRING_BUFFER_SIZE, "\t\t_%s = C%sImpl.new(asset_map)\n", classname, classname);
				appendString(loadMethod, directoryAssetName);

				// Append this class to the new class
				snprintf(directoryAssetName, STRING_BUFFER_SIZE, "class C%sImpl {\n", classname);
				topOfClass = appendStringAndFree(_vksk_CompileAssetsFromPak(filedir, directoryAssetName, "\n\tconstruct new(asset_map) {\n", ASSET_DIR_CLASS_FOOTER), popString(topOfClass));

				// Now the getter
				snprintf(directoryAssetName, STRING_BUFFER_SIZE, "\t%s { _%s }\n", classname, classname);
				appendString(topOfClass, directoryAssetName);
			}
		}
		f = vksk_PakNext(&dir);
	}

	SpriteData sprite;
	while (directoryJSONGetNextSprite(json, &sprite)) {
		_vksk_CompileAssetFromSpriteData(directory, topOfClass, loadMethod, &sprite);
	}
	TrueTypeFontData ttf;
	while (directoryJSONGetNextTrueTypeFont(json, &ttf)) {
		_vksk_CompileAssetFromTrueTypeData(directory, topOfClass, loadMethod, &ttf);
	}
	BitmapFontData bmf;
	while (directoryJSONGetNextBitmapFont(json, &bmf)) {
		_vksk_CompileAssetFromBitmapFontData(directory, topOfClass, loadMethod, &bmf);
	}
	BufferData buffer;
	while (directoryJSONGetNextBuffer(json, &buffer)) {
		_vksk_CompileAssetFromBufferData(directory, topOfClass, loadMethod, &buffer);
	}
	StringData text;
	while (directoryJSONGetNextString(json, &text)) {
		_vksk_CompileAssetFromStringData(directory, topOfClass, loadMethod, &text);
	}

	closeDirectoryJSON(json);

	// Build output string
	appendStringAndFree(topOfClass, popString(appendString(loadMethod, "\t}\n")));
	return appendString(topOfClass, footerString);
}

const char *vksk_CompileAssetFile(const char *rootDir) {
	String assets = NULL;

	if (gGamePak != NULL)
		assets = _vksk_CompileAssetsFromPak(rootDir, ASSET_ASSET_CLASS_HEADER, "\n\tload_assets() {\n\t\t_asset_map = {}\n\t\tvar asset_map = _asset_map\n", ASSET_ASSET_CLASS_FOOTER);
	else
		assets = _vksk_CompileAssetsFromDirectory(rootDir, ASSET_ASSET_CLASS_HEADER, "\n\tload_assets() {\n\t\t_asset_map = {}\n\t\tvar asset_map = _asset_map\n", ASSET_ASSET_CLASS_FOOTER);

	if (assets != NULL) {
		String string = appendString(newString(), ASSET_FILE_HEADER);
		appendString(string, popString(assets));
		appendString(string, ASSET_FILE_FOOTER);
		return popString(string);
	}
	return NULL;
}