/// \file Validation.c
/// \author Paolo Mazzon

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

#include "src/Validation.h"

const uint64_t FOREIGN_END = UINT64_MAX; // internal use
const uint64_t FOREIGN_STRING = 1<<0;
const uint64_t FOREIGN_BOOL = 1<<1;
const uint64_t FOREIGN_NUM = 1<<2;
const uint64_t FOREIGN_LIST = 1<<3;
const uint64_t FOREIGN_MAP = 1<<4;
const uint64_t FOREIGN_NULL = 1<<5;
const uint64_t FOREIGN_FOREIGN = 1<<6;
const uint64_t FOREIGN_INI = 1<<7;
const uint64_t FOREIGN_BITMAP_FONT = 1<<8;
const uint64_t FOREIGN_TEXTURE = 1<<9;
const uint64_t FOREIGN_SPRITE = 1<<10;
const uint64_t FOREIGN_SURFACE = 1<<11;
const uint64_t FOREIGN_CAMERA = 1<<12;
const uint64_t FOREIGN_AUDIO_DATA = 1<<13;
const uint64_t FOREIGN_AUDIO = 1<<14;

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