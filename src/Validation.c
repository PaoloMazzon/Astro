/// \file Validation.c
/// \author Paolo Mazzon

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

#include "src/Validation.h"
#include "src/Runtime.h"
#include "src/IntermediateTypes.h"

const uint64_t FOREIGN_END = 0; // internal use
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
const uint64_t FOREIGN_TILED_MAP = 1<<15;
const uint64_t FOREIGN_BUFFER = 1<<16;
const uint64_t FOREIGN_SHADER = 1<<17;

extern VKSK_EngineConfig gEngineConfig;

static inline bool checkType(WrenVM *vm, int slot, uint64_t type) {
	WrenType wrenType = wrenGetSlotType(vm, slot);
	if (wrenType != WREN_TYPE_FOREIGN) { // wren types
		if (wrenType == WREN_TYPE_STRING && type & FOREIGN_STRING) return true;
		if (wrenType == WREN_TYPE_BOOL && type & FOREIGN_BOOL) return true;
		if (wrenType == WREN_TYPE_NUM && type & FOREIGN_NUM) return true;
		if (wrenType == WREN_TYPE_LIST && type & FOREIGN_LIST) return true;
		if (wrenType == WREN_TYPE_MAP && type & FOREIGN_MAP) return true;
		if (wrenType == WREN_TYPE_NULL && type & FOREIGN_NULL) return true;
		return false;
	} else { // foreign types
		uint64_t actualType = ((VKSK_RuntimeForeign*)wrenGetSlotForeign(vm, slot))->type;
		return (actualType & type) == actualType;
	}
}

void _vksk_ValidateForeignArgs(WrenVM *vm, const char *function, ...) {
	if (gEngineConfig.enableTypeChecking) {
		va_list list;
		va_start(list, function);

		uint64_t type = va_arg(list, uint64_t);
		int slot = 1;
		while (type != FOREIGN_END) {
			if (!checkType(vm, slot, type)) {
				vksk_Log("Argument %i in foreign function \"%s\" is of incorrect type %#010x", slot, function, type);
				abort();
			}
			slot += 1;

			type = va_arg(list, uint64_t);
		}

		va_end(list);
	}
}

void vksk_Log(const char *fmt, ...) {
	va_list list;
	va_start(list, fmt);
	vprintf(fmt, list);
	printf("\n");
	fflush(stdout);
	va_end(list);
}