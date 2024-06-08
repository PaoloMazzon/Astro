/// \file Util.h
/// \author Paolo Mazzon
/// \brief Various utilies
#pragma once
#include <wren.h>
#include <IntermediateTypes.h>

// Grabs a file buffer from either game.pak if it finds it or the filesystem
void *vksk_GetFileBuffer(const char *filename, int *size);

// Puts a newly allocated foreign class into slot 0 and returns it
VKSK_RuntimeForeign *vksk_NewForeignClass(WrenVM *vm, const char *module, const char *name);