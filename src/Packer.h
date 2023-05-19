/// \file Packer.h
/// \author Paolo Mazzon
/// \brief Saves and loads .pak files
#pragma once
#include <stdint.h>
#include <stdbool.h>

// Abstraction for packs
typedef struct VKSK_Pak *VKSK_Pak;
typedef struct VKSK_PakDir VKSK_PakDir;

struct VKSK_PakDir {
	VKSK_Pak root;
	int fileIndex;
	int dirsDeep;
	char currentDir[1024];
	char returnDir[1024];
};

// Loads a packs header into memory but not the contents -- you cannot modify these paks
VKSK_Pak vksk_PakLoad(const char *filename);

// Returns true if a file exists in a pak
bool vksk_PakFileExists(VKSK_Pak pak, const char *filename);

// Returns a binary buffer of the file, placing the size of the file into `size` (must free memory manually)
uint8_t *vksk_PakGetFile(VKSK_Pak pak, const char *filename, int *size);

// Same as vksk_PakGetFile but ensures trailing 0
const char *vksk_PakGetFileString(VKSK_Pak, const char *filename);

// Prints a list of the pak's contents to stdout
void vksk_PakPrintContents(VKSK_Pak);

// Begins iterating through the contents of a pak starting at a given root directory, returns NULL if there are none
// This will return either a file or a folder (folders will always end in '/'). If its a folder you will need to
// perform another vksk_PakBeginLoop on another VKSK_PakDir to access that directory's contents.
const char *vksk_PakBeginLoop(VKSK_Pak pak, VKSK_PakDir *pakdir, const char *dir);

// Gets the next filename from vksk_PakBeginLoop, returns NULL if there are no more
// This will return either a file or a folder (folders will always end in '/'). If its a folder you will need to
// perform another vksk_PakBeginLoop on another VKSK_PakDir to access that directory's contents.
const char *vksk_PakNext(VKSK_PakDir *pakdir);

// Creates an empty pak
VKSK_Pak vksk_PakCreate();

// Places the contents of a directory into an existing pak, returns non-zero value on failure
void vksk_PakAddDirectory(VKSK_Pak pak, const char *directory);

// Saves a pak created with vksk_PakCreate
void vksk_PakSave(VKSK_Pak pak, const char *file);

// Frees a pak from memory
void vksk_PakFree(VKSK_Pak pak);