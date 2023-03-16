/// \file Packer.c
/// \author Paolo Mazzon
#include <SDL2/SDL.h>

#include "src/Packer.h"

// Paks are not r/w compatible, one or the other
typedef enum {
	PAK_TYPE_READ = 1,
	PAK_TYPE_WRITE = 2,
} VKSK_PakType;

typedef struct VKSK_PakHeader {
	const char **filenames;
	uint64_t *fileSizes;
	int fileCount;
	int endian;
} VKSK_PakHeader;

struct VKSK_Pak {
	VKSK_PakType type;
	VKSK_PakHeader header;
	const char *filename;
};

VKSK_Pak vksk_PakLoad(const char *filename) {

}

bool vksk_PakFileExists(VKSK_Pak pak, const char *filename) {

}

uint8_t *vksk_PakGetFile(VKSK_Pak pak, const char *filename, int *size) {

}

void vksk_PakExtract(VKSK_Pak pak, const char *outDir) {

}

VKSK_Pak vksk_PakCreate() {

}

void vksk_PakAddDirectory(VKSK_Pak pak, const char *directory) {

}

void vksk_PakSave(VKSK_Pak pak, const char *file) {

}

void vksk_PakFree(VKSK_Pak pak) {

}