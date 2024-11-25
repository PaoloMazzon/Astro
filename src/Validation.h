/// \file Validation.h
/// \author Paolo Mazzon
/// \brief Tools for debugging
#pragma once
#include <wren.h>
#include "src/BuildOptions.h"

#ifdef __cplusplus
extern "C" {
#endif

// Makes sure that the arguments provided to a foreign function are what they should be
// if debug is enabled
#define VALIDATE_FOREIGN_ARGS(vm, ...) if (VKSK_DEBUGGER_BUILD) {_vksk_ValidateForeignArgs(vm, __func__, ##__VA_ARGS__);}

// For SDL functions that can return null
#define VALIDATE_SDL(val) if (val == NULL) {vksk_Log("SDL failure in \"%s\" on line %i, SDL error: %s", __func__, __LINE__, SDL_GetError());}

// Underlying function for the `VALIDATE_FOREIGN_ARGS` macro
void _vksk_ValidateForeignArgs(WrenVM *vm, const char *function, ...);

// Log a message
void vksk_Log(const char *fmt, ...);

// Throw an error
void vksk_Error(bool fatal, const char *fmt, ...);

#ifdef __cplusplus
}
#endif