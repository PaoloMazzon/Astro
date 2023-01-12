/// \file Validation.h
/// \author Paolo Mazzon
/// \brief Tools for debugging
#pragma once
#include <wren.h>

#include "src/BuildOptions.h"

// The type you give to `VALIDATE_FOREIGN_ARGS` if you expect a string in that position
#define FOREIGN_STRING "String"

// The type you give to `VALIDATE_FOREIGN_ARGS` if you expect a bool in that position
#define FOREIGN_BOOL "Bool"

// The type you give to `VALIDATE_FOREIGN_ARGS` if you expect a double in that position
#define FOREIGN_NUM "Num"

// The type you give to `VALIDATE_FOREIGN_ARGS` if you expect a list in that position
#define FOREIGN_LIST "List"

// The type you give to `VALIDATE_FOREIGN_ARGS` if you expect a map in that position
#define FOREIGN_MAP "Map"

// The type you give to `VALIDATE_FOREIGN_ARGS` if you expect a NULL in that position
#define FOREIGN_NULL "Null"

// The type you give to `VALIDATE_FOREIGN_ARGS` if you expect a foreign object in that position
#define FOREIGN_FOREIGN "Foreign"

// Makes sure that the arguments provided to a foreign function are what they should be
// if debug is enabled
#define VALIDATE_FOREIGN_ARGS(vm, ...) if (VKSK_DEBUGGER_BUILD) {_vksk_ValidateForeignArgs(vm, ##__VA_ARGS__);}

// Underlying function for the `VALIDATE_FOREIGN_ARGS` macro
void _vksk_ValidateForeignArgs(WrenVM *vm, ...);