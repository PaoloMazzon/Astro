/// \file AssetCompiler.c
/// \author Paolo Mazzon
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#include "src/Runtime.h"
#include "src/Validation.h"
#include "src/ConfigFile.h"

// -------------------- NEW ASSET COMPILER -------------------- //
//
// 1. Recursive for sub-directories
// 2. Pulls asset details from a .json file
// 3. Can pull sprite details from Aseprite sprite json data
// 4. Sub-directories available in Wren through other classes, for example
//    "dir/sprites/file.png" would be `Assets.dir.sprites.spr_file`
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
//     var Assets = AssetsImpl.new().
//
// ------------------------------- Constants ------------------------------- //

const char *OUTPUT_HEADER = "import \"lib/Drawing\" for Texture, Sprite, BitmapFont, Font\nimport \"lib/Audio\" for AudioData\n"
							"\n"
							"class Assets {\n";

const char *OUTPUT_FOOTER = "\n    static [asset] {\n"
							"        return __asset_map[asset]\n"
							"    }\n"
	   						"}\n";

const char *LOAD_HEADER = "    static load_assets() {\n"
						  "        __asset_map = {}\n";
const char *LOAD_FOOTER = "    }\n\n";

const char *ASSET_FILE_HEADER = "import \"lib/Drawing\" for Texture, Sprite, BitmapFont, Font\nimport \"lib/Audio\" for AudioData\n\n";
const char *ASSET_FILE_FOOTER = "\nvar Assets = AssetsImpl.new()\n";
const char *ASSET_ASSET_CLASS_HEADER = "class AssetsImpl {\n\tconstruct new() {}\n\n";
const char *ASSET_ASSET_CLASS_FOOTER = "\t[asset] {\n\t\treturn _asset_map[asset]\n\t}\n}\n";
const char *ASSET_DIR_CLASS_HEADER = "\n";
const char *ASSET_DIR_CLASS_FOOTER = "}\n\n";

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

static void freeString(String s) {
	if (s != NULL)
		free(s->str);
	free(s);
}

// ------------------------------- JSON Parsers ------------------------------- //
typedef struct SpriteData {
	double x;
	double y;
	double origin_x;
	double origin_y;
	double w;
	double h;
	double frames;
	double delay;
} SpriteData;

typedef struct BitmapFontData {
	int ustart;
	int uend;
	double w;
	double h;
} BitmapFontData;

typedef struct TrueTypeFontData {
	int ustart;
	int uend;
	double size;
	bool aa;
} TrueTypeFontData;

typedef struct DirectoryJSON {
	// TODO: This
} *DirectoryJSON;

// Will attempt to locate a corresponding .json sprite data file exported from Aseprite and
// if it does will fill out sprite with its data. Returns true if it found one and false otherwise.
static bool jsonFindSpriteData(const char *tex_filename, SpriteData *sprite) {
	// TODO: This
	return false;
}

// Will attempt to open the assets.json file for a directory, if it doesn't find one it will return
// null. Other directory json functions won't crash on the null either.
static DirectoryJSON openDirectoryJSON(const char *directory) {
	return NULL;
}

// Attempts to find the next sprite in a directory json, returns true if it found one and filled out
// the sprite data pointer.
static bool directoryJSONGetNextSprite(DirectoryJSON json, SpriteData *sprite) {
	return false;
}

// Attempts to find the next bitmap font in a directory json, returns true if it found one and filled out
// the bitmap font data pointer.
static bool directoryJSONGetNextBitmapFont(DirectoryJSON json, BitmapFontData *sprite) {
	return false;
}

// Attempts to find the next true type font in a directory json, returns true if it found one and filled out
// the true type font data pointer.
static bool directoryJSONGetNextTrueTypeFont(DirectoryJSON json, TrueTypeFontData *sprite) {
	return false;
}

// ------------------------------- Helpers ------------------------------- //

static bool filenameIsValid(const char *s) {
	if (*s >= '0' && *s <= '9')
		return false;
	while (*s != 0) {
		if (!((*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z') || (*s >= '0' && *s <= '9') || *s == '_'))
			return false;
		s++;
	}
	return true;
}

// ------------------------------- Asset compiler ------------------------------- //

static void addFileToAssets(VKSK_Config conf, const char *file, String loaderFunction, String spriteLoader, String getterFunctions) {
	char tmpCode[1024]; // Code will be generated in here
	char tmpName[1024]; // Name will be manipulated here
	memcpy(tmpName, file, strlen(file) + 1);
	const char *ext = strrchr(tmpName, '.') + 1;
	tmpName[ext - 1 - tmpName] = 0; // grr memory tomfoolery

	// Make sure the filename is valid
	if (!filenameIsValid(tmpName)) {
		vksk_Log("Asset filename \"%s\" is invalid, file is being ignored.", file);
		return;
	}

	// Sprites textures and bitmap fonts
	if (strcmp(ext, "png") == 0 || strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0 ||
		strcmp(ext, "bmp") == 0) {
		// Check if its in the assets ini
		bool exists = vksk_ConfigHeaderExists(conf, file);

		if (exists && strcmp(vksk_ConfigGetString(conf, file, "type", ""), "sprite") == 0) {
			// Generate loader function bit first
			snprintf(tmpCode, 1024, "        __tex%s = Texture.new(\"assets/%s\")\n        __asset_map[\"tex_%s\"] = __tex%s\n", tmpName, file, tmpName, tmpName);
			appendString(loaderFunction, tmpCode);

			// Getter
			snprintf(tmpCode, 1024, "    static tex_%s { __tex%s }\n", tmpName, tmpName);
			appendString(getterFunctions, tmpCode);

			double x, y, w, h, delay, frames, origin_x, origin_y;
			x = vksk_ConfigGetDouble(conf, file, "x", 0);
			y = vksk_ConfigGetDouble(conf, file, "y", 0);
			origin_x = vksk_ConfigGetDouble(conf, file, "origin_x", 0);
			origin_y = vksk_ConfigGetDouble(conf, file, "origin_y", 0);
			w = vksk_ConfigGetDouble(conf, file, "w", 0);
			h = vksk_ConfigGetDouble(conf, file, "h", 0);
			delay = vksk_ConfigGetDouble(conf, file, "delay", 0);
			frames = vksk_ConfigGetDouble(conf, file, "frames", 1);
			// Generate loader function bit first
			snprintf(tmpCode, 1024, "        __spr%s = Sprite.from(__tex%s, %f, %f, %f, %f, %f, %i)\n        __asset_map[\"spr_%s\"] = __spr%s\n        __spr%s.origin_x = %f\n        __spr%s.origin_y = %f\n", tmpName, tmpName, x, y, w, h, delay, (int)frames, tmpName, tmpName, tmpName, origin_x, tmpName, origin_y);
			appendString(spriteLoader, tmpCode);

			// Getter
			snprintf(tmpCode, 1024, "    static spr_%s { __spr%s }\n", tmpName, tmpName);
			appendString(getterFunctions, tmpCode);
		} else if (exists && strcmp(vksk_ConfigGetString(conf, file, "type", ""), "font") == 0) {
			double ustart, uend, w, h;
			ustart = vksk_ConfigGetDouble(conf, file, "ustart", 0);
			uend = vksk_ConfigGetDouble(conf, file, "uend", 0);
			w = vksk_ConfigGetDouble(conf, file, "w", 0);
			h = vksk_ConfigGetDouble(conf, file, "h", 0);
			// Generate loader function bit first
			snprintf(tmpCode, 1024, "        __fnt%s = BitmapFont.new(\"assets/%s\", %i, %i, %f, %f)\n        __asset_map[\"fnt_%s\"] = __fnt%s\n", tmpName, file, (int)ustart, (int)uend, w, h, tmpName, tmpName);
			appendString(loaderFunction, tmpCode);

			// Getter
			snprintf(tmpCode, 1024, "    static fnt_%s { __fnt%s }\n", tmpName, tmpName);
			appendString(getterFunctions, tmpCode);
		} else {
			// Generate loader function bit first
			snprintf(tmpCode, 1024, "        __tex%s = Texture.new(\"assets/%s\")\n        __asset_map[\"tex_%s\"] = __tex%s\n", tmpName, file, tmpName, tmpName);
			appendString(loaderFunction, tmpCode);

			// Getter
			snprintf(tmpCode, 1024, "    static tex_%s { __tex%s }\n", tmpName, tmpName);
			appendString(getterFunctions, tmpCode);
		}
	} else if (strcmp(ext, "wav") == 0 || strcmp(ext, "ogg") == 0) {
		// Generate loader function bit first
		snprintf(tmpCode, 1024, "        __aud%s = AudioData.open(\"assets/%s\")\n        __asset_map[\"aud_%s\"] = __aud%s\n", tmpName, file, tmpName, tmpName);
		appendString(loaderFunction, tmpCode);

		// Getter
		snprintf(tmpCode, 1024, "    static aud_%s { __aud%s }\n", tmpName, tmpName);
		appendString(getterFunctions, tmpCode);
	} else if (strcmp(ext, "ttf") == 0 && vksk_ConfigHeaderExists(conf, file)) {
		double ustart, uend, size;
		bool aa;
		ustart = vksk_ConfigGetDouble(conf, file, "ustart", 0);
		uend = vksk_ConfigGetDouble(conf, file, "uend", 0);
		size = vksk_ConfigGetDouble(conf, file, "size", 0);
		aa = vksk_ConfigGetBool(conf, file, "aa", 0);
		// Generate loader function bit first
		snprintf(tmpCode, 1024, "        __fnt%s = Font.open(\"assets/%s\", %f, %s, %f, %f)\n        __asset_map[\"fnt_%s\"] = __fnt%s\n", tmpName, file, size, aa ? "true" : "false", ustart, uend, tmpName, tmpName);
		appendString(spriteLoader, tmpCode);

		// Getter
		snprintf(tmpCode, 1024, "    static fnt_%s { __fnt%s }\n", tmpName, tmpName);
		appendString(getterFunctions, tmpCode);
	}
}

static bool _vksk_CompileAssetsFromDir(String loadFunction, String getterFunctions, String spriteLoadFunction) {
	struct dirent *dp;
	DIR *dfd;
	VKSK_Config conf = vksk_ConfigLoad("assets/assets.ini");

	char *dir = "assets/";

	if ((dfd = opendir(dir)) == NULL) {
		vksk_Log("Can't access assets directory");
		return false;
	}

	char filename_qfd[100] ;

	dp = readdir(dfd);
	while (dp != NULL) {
		struct stat stbuf ;
		sprintf( filename_qfd , "%s/%s",dir,dp->d_name) ;
		if( stat(filename_qfd,&stbuf ) == -1 ) {
			vksk_Log("Unable to stat file: %s\n",filename_qfd) ;
			continue;
		}

		if ( ( stbuf.st_mode & S_IFMT ) != S_IFDIR ) {
			addFileToAssets(conf, dp->d_name, loadFunction, spriteLoadFunction, getterFunctions);
		}
		dp = readdir(dfd);
	}
	closedir(dfd);
	vksk_ConfigFree(conf);
	return true;
}

static bool _vksk_CompileAssetsFromPak(String loadFunction, String getterFunctions, String spriteLoadFunction) {
	const char *confString = vksk_PakGetFileString(gGamePak, "assets/assets.ini");
	VKSK_Config conf = vksk_ConfigLoadFromString(confString);
	free((void*)confString);

	const char *fname = vksk_PakBeginLoop(gGamePak, "assets");
	while (fname != NULL) {
		const char *temp = fname + 7;
		addFileToAssets(conf, temp, loadFunction, spriteLoadFunction, getterFunctions);
		fname = vksk_PakNext(gGamePak);
	}

	vksk_ConfigFree(conf);

	return true;
}

const char *vksk_CompileAssetFile() {
	String loadFunction = appendString(newString(), LOAD_HEADER);
	String getterFunctions = newString();
	String spriteLoadFunction = newString();

	if (gGamePak != NULL) {
		if (!_vksk_CompileAssetsFromPak(loadFunction, getterFunctions, spriteLoadFunction)) {
			vksk_Log("Failed to compile asset file.");
		}
	} else {
		if (!_vksk_CompileAssetsFromDir(loadFunction, getterFunctions, spriteLoadFunction)) {
			vksk_Log("Failed to compile asset file.");
		}
	}

	// Build output string
	appendString(loadFunction, spriteLoadFunction->str);
	appendString(loadFunction, LOAD_FOOTER);
	String compiledAssets = appendString(newString(), OUTPUT_HEADER);
	appendString(compiledAssets, loadFunction->str);
	appendString(compiledAssets, getterFunctions->str);
	freeString(loadFunction);
	freeString(spriteLoadFunction);
	freeString(getterFunctions);
	appendString(compiledAssets, OUTPUT_FOOTER);

	const char *out = compiledAssets->str;
	free(compiledAssets);

	return out;
}