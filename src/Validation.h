/// \file Validation.h
/// \author Paolo Mazzon
/// \brief Tools for debugging
#pragma once
#include <wren.h>

#include "src/BuildOptions.h"

// Makes sure that the arguments provided to a foreign function are what they should be
// if debug is enabled
#define VALIDATE_FOREIGN_ARGS(vm, ...) if (VKSK_DEBUGGER_BUILD) {_vksk_ValidateForeignArgs(vm, __func__, ##__VA_ARGS__);}

// Underlying function for the `VALIDATE_FOREIGN_ARGS` macro
void _vksk_ValidateForeignArgs(WrenVM *vm, const char *function, ...);

// Log a message
void vksk_Log(const char *fmt, ...);