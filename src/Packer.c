/// \file Packer.c
/// \author Paolo Mazzon
#include <SDL2/SDL.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#include "src/Packer.h"

/*
 * .pak file specification
 *
 * The top of a .pak file is the header, which contains the following information
 *  + 4 bytes for the endianness of the file (SDL macros)
 *  + 4 bytes for the file count
 *  + a list of details for each file in the pak, each entry containing the following
 *    + 4 bytes for the length of the filename (call it x)
 *    + 4 bytes for the size of the file
 *    + 4 bytes for the offset of the file in the .pak file (used for fseek)
 *    + x bytes for the file's name (no terminating 0, loader does that)
 *  + After the header, the contents of each is placed at its specified offset
 */

// Paks are not r/w compatible, one or the other
typedef enum {
	PAK_TYPE_READ = 1,
	PAK_TYPE_WRITE = 2,
} VKSK_PakType;

typedef struct VKSK_PakFileInfo {
	const char *filename;
	int size;
	int pointer;
} VKSK_PakFileInfo;

typedef struct VKSK_PakHeader {
	VKSK_PakFileInfo *files;
	int fileCount;
	int endian;
} VKSK_PakHeader;

struct VKSK_Pak {
	VKSK_PakType type;
	VKSK_PakHeader header;
	const char *filename;
};

static VKSK_Pak _vksk_PakMakeEmpty(VKSK_PakType type) {
	VKSK_Pak pak = calloc(1, sizeof(struct VKSK_Pak));
	pak->header.endian = SDL_BYTEORDER;
	pak->type = type;
	return pak;
}

static const char *_vksk_CopyString(const char *string) {
	int size = strlen(string);
	char *s = malloc(size + 1);
	memcpy(s, string, size);
	s[size] = 0;
	return s;
}

static unsigned char* _vksk_LoadFileRaw(const char *filename, int *size) {
	FILE* file = fopen(filename, "rb");
	unsigned char *buffer = NULL;
	*size = 0;

	if (file != NULL) {
		// Find file size
		fseek(file, 0, SEEK_END);
		*size = ftell(file);
		rewind(file);

		buffer = malloc(*size);

		if (buffer != NULL) {
			// Fill the buffer
			fread(buffer, 1, *size, file);
		}
		fclose(file);
	}

	return (unsigned char*)buffer;
}

static int _vksk_FileSize(const char *filename) {
	int size = 0;
	FILE *f = fopen(filename, "r");
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fclose(f);
	return size;
}

static int _vksk_CalculateHeaderSize(VKSK_Pak pak) {
	int size = 0;
	size += 4; // endianness
	size += 4; // file count
	for (int i = 0; i < pak->header.fileCount; i++) {
		size += 12; // filename length, file size, file offset
		size += strlen(pak->header.files[i].filename);
	}
	return size;
}

VKSK_Pak vksk_PakLoad(const char *filename) {

}

bool vksk_PakFileExists(VKSK_Pak pak, const char *filename) {

}

uint8_t *vksk_PakGetFile(VKSK_Pak pak, const char *filename, int *size) {

}

void vksk_PakExtract(VKSK_Pak pak, const char *outDir) {

}

VKSK_Pak vksk_PakCreate() {
	VKSK_Pak pak = _vksk_PakMakeEmpty(PAK_TYPE_WRITE);
	return pak;
}

int vksk_PakAddDirectory(VKSK_Pak pak, const char *directory) {
	struct dirent *dp;
	DIR *dfd;

	char *dir = "assets/";

	if ((dfd = opendir(dir)) == NULL) {
		return -1;
	}

	char filename_qfd[1000] ;

	dp = readdir(dfd);
	while (dp != NULL) {
		struct stat stbuf ;
		sprintf( filename_qfd , "%s/%s",dir,dp->d_name) ;
		if( stat(filename_qfd,&stbuf ) == -1 ) {
			continue;
		}

		if ( ( stbuf.st_mode & S_IFMT ) != S_IFDIR ) {
			// Load file into pak
		}
		dp = readdir(dfd);
	}
	return 0;
}

void vksk_PakSave(VKSK_Pak pak, const char *file) {

}

void vksk_PakFree(VKSK_Pak pak) {

}