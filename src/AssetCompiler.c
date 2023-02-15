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

static void addFileToAssets(VKSK_Config conf, const char *file, String loaderFunction, String spriteLoader, String getterFunctions) {
	char tmpCode[1024]; // Code will be generated in here
	char tmpName[1024]; // Name will be manipulated here
	memcpy(tmpName, file, strlen(file) + 1);
	const char *ext = strrchr(tmpName, '.') + 1;
	tmpName[ext - 1 - tmpName] = 0; // grr memory tomfoolery

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

			double x, y, w, h, delay, frames;
			x = vksk_ConfigGetDouble(conf, file, "x", 0);
			y = vksk_ConfigGetDouble(conf, file, "y", 0);
			w = vksk_ConfigGetDouble(conf, file, "w", 0);
			h = vksk_ConfigGetDouble(conf, file, "h", 0);
			delay = vksk_ConfigGetDouble(conf, file, "delay", 0);
			frames = vksk_ConfigGetDouble(conf, file, "frames", 1);
			// Generate loader function bit first
			snprintf(tmpCode, 1024, "        __spr%s = Sprite.from(__tex%s, %f, %f, %f, %f, %f, %i)\n        __asset_map[\"spr_%s\"] = __spr%s\n", tmpName, tmpName, x, y, w, h, delay, (int)frames, tmpName, tmpName);
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

const char *vksk_CompileAssetFile() {
	struct dirent *dp;
	String loadFunction = appendString(newString(), LOAD_HEADER);
	String getterFunctions = newString();
	String spriteLoadFunction = newString();
	DIR *dfd;
	VKSK_Config conf = vksk_ConfigLoad("assets/assets.ini");

	char *dir = "assets/";

	if ((dfd = opendir(dir)) == NULL) {
		vksk_Log("Can't access assets directory");
		return NULL;
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
	vksk_ConfigFree(conf);

	return out;
}