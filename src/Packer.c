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

struct VKSK_PakDir {
	VKSK_Pak root;
	int fileIndex;
	int dirsDeep;
	char currentDir[1024];
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

// Returns true if string starts with the entire contents of root
static bool _vksk_StartsWith(const char *root, const char *string) {
	int rl = strlen(root);
	int sl = strlen(string);
	if (rl > sl)
		return false;
	for (int i = 0; i < rl; i++)
		if (root[i] != string[i])
			return false;
	return true;
}

// Returns how many /'s are in a path
static int _vksk_DirectoriesDeep(const char *path) {
	int deep = 0;
	while (*path != 0) {
		if (*path == '/')
			deep += 1;
		path++;
	}
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
	FILE *f = fopen(filename, "rb");
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

static void _vksk_AddHeaderInfo(VKSK_Pak pak, const char *file) {
	pak->header.files = realloc(pak->header.files, sizeof(VKSK_PakFileInfo) * (pak->header.fileCount + 1));
	VKSK_PakFileInfo *info = &pak->header.files[pak->header.fileCount];
	pak->header.fileCount += 1;
	info->filename = _vksk_CopyString(file);
	info->pointer = 0;
	info->size = _vksk_FileSize(file);
}

static void _vksk_IterateDirectory(VKSK_Pak pak, const char *dir) {
	struct dirent *dp;
	DIR *dfd;

	if ((dfd = opendir(dir)) == NULL) {
		return;
	}

	char filename_qfd[1000] ;

	dp = readdir(dfd);
	while (dp != NULL) {
		struct stat stbuf ;
		sprintf( filename_qfd , "%s/%s",dir,dp->d_name) ;
		if( stat(filename_qfd,&stbuf ) == -1 ) {
			continue;
		}

		if ((stbuf.st_mode & S_IFMT) != S_IFDIR) {
			_vksk_AddHeaderInfo(pak, filename_qfd);
		} else if (( stbuf.st_mode & S_IFMT ) == S_IFDIR) {
			if (filename_qfd[strlen(filename_qfd) - 1] != '.') {
				_vksk_IterateDirectory(pak, filename_qfd);
			}
		}
		dp = readdir(dfd);
	}
}

static bool _vksk_IsFromDirectory(const char *fname, const char *dir) {
	if (strstr(fname, dir) == fname || dir[0] == 0)
		return true;
	return false;
}

static int _vksk_SwapEndian(int valEnd, int val) {
	if (valEnd != SDL_BYTEORDER)
		return SDL_Swap32(val);
	else
		return val;
}

VKSK_Pak vksk_PakLoad(const char *filename) {
	VKSK_Pak pak = _vksk_PakMakeEmpty(PAK_TYPE_READ);
	pak->header.files = NULL;
	pak->filename = _vksk_CopyString(filename);
	FILE *f = fopen(filename, "rb");
	if (f != NULL) {
		int endian;
		fread(&endian, 4, 1, f);
		fread(&pak->header.fileCount, 4, 1, f);
		pak->header.fileCount = _vksk_SwapEndian(endian, pak->header.fileCount);

		// Create header
		pak->header.files = malloc(sizeof(struct VKSK_PakFileInfo) * pak->header.fileCount);
		int filesProcessed = 0;
		for (int i = 0; i < pak->header.fileCount && !feof(f); i++) {
			VKSK_PakFileInfo *fileInfo = &pak->header.files[i];

			// String size
			int stringSize;
			fread(&stringSize, 4, 1, f);
			stringSize = _vksk_SwapEndian(endian, stringSize);

			// File size
			fread(&fileInfo->size, 4, 1, f);
			fileInfo->size = _vksk_SwapEndian(endian, fileInfo->size);

			// Offset
			fread(&fileInfo->pointer, 4, 1, f);
			fileInfo->pointer = _vksk_SwapEndian(endian, fileInfo->pointer);

			// Filename
			char *fname = malloc(stringSize + 1);
			fname[stringSize] = 0;
			fread(fname, 1, stringSize, f);
			fileInfo->filename = fname;
			filesProcessed += 1;
		}
		fclose(f);

		if (filesProcessed != pak->header.fileCount) {
			for (int i = 0; i < filesProcessed; i++)
				free((void *) pak->header.files[i].filename);
			free(pak->header.files);
			free((void *) pak->filename);
			free(pak);
			return NULL;
		}
	}

	return pak;
}

bool vksk_PakFileExists(VKSK_Pak pak, const char *filename) {
	if (pak != NULL && pak->type == PAK_TYPE_READ)
		for (int i = 0; i < pak->header.fileCount; i++)
			if (strcmp(pak->header.files[i].filename, filename) == 0)
				return true;
	return false;
}

uint8_t *vksk_PakGetFile(VKSK_Pak pak, const char *filename, int *size) {
	*size = -1;
	uint8_t *out = NULL;
	if (pak != NULL && pak->type == PAK_TYPE_READ) {
		// Find the file
		VKSK_PakFileInfo *found = NULL;
		for (int i = 0; i < pak->header.fileCount && found == NULL; i++) {
			if (strcmp(pak->header.files[i].filename, filename) == 0) {
				found = &pak->header.files[i];
			}
		}

		// Create a piece of memory for it
		if (found != NULL) {
			FILE *f = fopen(pak->filename, "rb");
			out = malloc(found->size);
			*size = found->size;
			fseek(f, found->pointer, SEEK_SET);
			fread(out, 1, *size, f);
			fclose(f);
		}
	}

	return out;
}

const char *vksk_PakGetFileString(VKSK_Pak pak, const char *filename) {
	int size;
	char *buffer = (void*)vksk_PakGetFile(pak, filename, &size);
	if (buffer != NULL) {
		buffer = realloc(buffer, size + 1);
		buffer[size] = 0;
	}
	return buffer;
}

void vksk_PakPrintContents(VKSK_Pak pak) {
	for (int i = 0; i < pak->header.fileCount; i++) {
		printf("%s\n  Size: %i\n  Offset: %i\n", pak->header.files[i].filename, pak->header.files[i].size, pak->header.files[i].pointer);
	}
}

const char *vksk_PakBeginLoop(VKSK_Pak pak, VKSK_PakDir *pakdir, const char *dir) {
	// Make sure the directory is either blank or ends with a slash if not blank
	if (strcmp("./", dir) != 0 && strcmp("../", dir) != 0 && strcmp(".", dir) != 0 && strcmp("..", dir) != 0) {
		if (dir[strlen(dir) - 1] == '/')
			strncpy(pakdir->currentDir, dir, 1024);
		else
			snprintf(pakdir->currentDir, 1024, "%s/", dir);

		// Count how many dirs deep
		pakdir->dirsDeep = _vksk_DirectoriesDeep(pakdir->currentDir);
	} else {
		pakdir->currentDir[0] = 0;
		pakdir->dirsDeep = 0;
	}
	pakdir->root = pak;
	pakdir->fileIndex = 0;
	return vksk_PakNext(pakdir);
}

const char *vksk_PakNext(VKSK_PakDir *pakdir) {
	return NULL; // TODO: This
}

VKSK_Pak vksk_PakCreate() {
	VKSK_Pak pak = _vksk_PakMakeEmpty(PAK_TYPE_WRITE);
	return pak;
}

void vksk_PakAddDirectory(VKSK_Pak pak, const char *directory) {
	if (pak->type == PAK_TYPE_WRITE)
		_vksk_IterateDirectory(pak, directory);
}

void vksk_PakSave(VKSK_Pak pak, const char *file) {
	if (pak->type == PAK_TYPE_WRITE) {
		int pointer = _vksk_CalculateHeaderSize(pak);

		// Calculate pointers
		for (int i = 0; i < pak->header.fileCount; i++) {
			pak->header.files[i].pointer = pointer;
			pointer += pak->header.files[i].size;
		}

		FILE *f = fopen(file, "wb");
		fwrite(&pak->header.endian, 4, 1, f);
		fwrite(&pak->header.fileCount, 4, 1, f);

		// Output header
		for (int i = 0; i < pak->header.fileCount; i++) {
			VKSK_PakFileInfo *pakFile = &pak->header.files[i];
			int strsize = strlen(pakFile->filename);
			fwrite(&strsize, 4, 1, f);
			fwrite(&pakFile->size, 4, 1, f);
			fwrite(&pakFile->pointer, 4, 1, f);
			fwrite(pakFile->filename, 1, strsize, f);
		}

		// Copy all files into the pak
		for (int i = 0; i < pak->header.fileCount; i++) {
			int fileSize = 0;
			void *data = _vksk_LoadFileRaw(pak->header.files[i].filename, &fileSize);
			fwrite(data, 1, fileSize, f);
		}

		fclose(f);
	}
}

void vksk_PakFree(VKSK_Pak pak) {
	if (pak != NULL) {
		for (int i = 0; i < pak->header.fileCount; i++)
			free((void*)pak->header.files[i].filename);
		free(pak->header.files);
		free((void*)pak->filename);
		free(pak);
	}
}