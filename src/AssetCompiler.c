/// \file AssetCompiler.c
/// \author Paolo Mazzon
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#include "src/Runtime.h"
#include "src/Validation.h"

const char *OUTPUT_HEADER = "import \"lib/Texture\" for Texture\n"
							"import \"lib/Sprite\" for Sprite\n"
							"\n"
							"class Assets {\n";

const char *OUTPUT_FOOTER = "}\n";

const char *LOAD_HEADER = "    static load_assets() {\n";
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
	int len = strlen(new);
	s->str = realloc(s->str, s->len + len + 1);
	s->str[s->len] = 0;
	strcat(s->str, new);
	s->len += len;
	return s;
}

static void freeString(String s) {
	if (s != NULL)
		free(s->str);
	free(s);
}

static void addFileToAssets(const char *file, String loaderFunction, String getterFunctions) {
	char tmpCode[1024]; // Code will be generated in here
	char tmpName[1024]; // Name will be manipulated here
	memcpy(tmpName, file, strlen(file) + 1);
	const char *ext = strrchr(tmpName, '.');
	tmpName[ext - tmpName] = 0; // grr memory tomfoolery

	if (strcmp(ext, "png")) {
		// Generate loader function bit first
		snprintf(tmpCode, 1024, "        __%s = Texture.new(\"assets/%s\")\n", tmpName, file);
		appendString(loaderFunction, tmpCode);

		// Getter
		snprintf(tmpCode, 1024, "    static %s() { __%s }\n", tmpName, tmpName);
		appendString(getterFunctions, tmpCode);
	}
}

const char *vksk_CompileAssetFile() {
	struct dirent *dp;
	String loadFunction = appendString(newString(), LOAD_HEADER);
	String getterFunctions = newString();
	DIR *dfd;

	char *dir = "assets/";

	if ((dfd = opendir(dir)) == NULL) {
		vksk_Log("Can't assets directory");
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
			addFileToAssets(dp->d_name, loadFunction, getterFunctions);
		}
		dp = readdir(dfd);
	}

	// Build output string
	appendString(loadFunction, LOAD_FOOTER);
	String compiledAssets = appendString(newString(), OUTPUT_HEADER);
	appendString(compiledAssets, loadFunction->str);
	appendString(compiledAssets, getterFunctions->str);
	freeString(loadFunction);
	freeString(getterFunctions);
	appendString(compiledAssets, OUTPUT_FOOTER);

	const char *out = compiledAssets->str;
	free(compiledAssets);

	return out;
}