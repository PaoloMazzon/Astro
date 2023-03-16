/// \file Packer.h
/// \author Paolo Mazzon
/// \brief Saves and loads .pak files
#pragma once
#include <stdint.h>
#include <stdbool.h>

// Abstraction for packs
typedef struct VKSK_Pak *VKSK_Pak;

// Loads a packs header into memory but not the contents -- you cannot modify these paks
VKSK_Pak vksk_PakLoad(const char *filename);

// Returns true if a file exists in a pak
bool vksk_PakFileExists(VKSK_Pak pak, const char *filename);

// Returns a binary buffer of the file, placing the size of the file into `size`
uint8_t *vksk_PakGetFile(VKSK_Pak pak, const char *filename, int *size);

// Extracts the contents of a pak into a directory
void vksk_PakExtract(VKSK_Pak pak, const char *outDir);

// Creates an empty pak
VKSK_Pak vksk_PakCreate();

// Places the contents of a directory into an existing pak, returns non-zero value on failure
void vksk_PakAddDirectory(VKSK_Pak pak, const char *directory);

// Saves a pak created with vksk_PakCreate
void vksk_PakSave(VKSK_Pak pak, const char *file);

// Frees a pak from memory
void vksk_PakFree(VKSK_Pak pak);