/// \file Validation.c
/// \author Paolo Mazzon

#include <stdarg.h>
#include <stdio.h>

#include "src/Validation.h"

void _vksk_ValidateForeignArgs(WrenVM *vm, ...) {
	// TODO: Something here lol
}

void vksk_Log(const char *fmt, ...) {
	va_list list;
	va_start(list, fmt);
	vprintf(fmt, list);
	printf("\n");
	fflush(stdout);
	va_end(list);
}