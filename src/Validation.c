/// \file Validation.c
/// \author Paolo Mazzon

#include <stdarg.h>
#include <stdio.h>

#include "src/Validation.h"

void _vksk_ValidateForeignArgs(WrenVM *vm, const char *function, ...) {
	// TODO: Possibly something?
}

void vksk_Log(const char *fmt, ...) {
	va_list list;
	va_start(list, fmt);
	vprintf(fmt, list);
	printf("\n");
	fflush(stdout);
	va_end(list);
}