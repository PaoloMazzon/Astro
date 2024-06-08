/// \file Validation.c
/// \author Paolo Mazzon

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

#include "src/Validation.h"
#include "src/Runtime.h"
#include "src/IntermediateTypes.h"

const uint64_t FOREIGN_END          = 0; // internal use
const uint64_t FOREIGN_STRING       = 1<<0;
const uint64_t FOREIGN_BOOL         = 1<<1;
const uint64_t FOREIGN_NUM          = 1<<2;
const uint64_t FOREIGN_LIST         = 1<<3;
const uint64_t FOREIGN_MAP          = 1<<4;
const uint64_t FOREIGN_NULL         = 1<<5;
const uint64_t FOREIGN_FOREIGN      = 1<<6;
const uint64_t FOREIGN_INI          = 1<<7;
const uint64_t FOREIGN_BITMAP_FONT  = 1<<8;
const uint64_t FOREIGN_TEXTURE      = 1<<9;
const uint64_t FOREIGN_SPRITE       = 1<<10;
const uint64_t FOREIGN_SURFACE      = 1<<11;
const uint64_t FOREIGN_CAMERA       = 1<<12;
const uint64_t FOREIGN_AUDIO_DATA   = 1<<13;
const uint64_t FOREIGN_AUDIO        = 1<<14;
const uint64_t FOREIGN_TILED_MAP    = 1<<15;
const uint64_t FOREIGN_BUFFER       = 1<<16;
const uint64_t FOREIGN_SHADER       = 1<<17;
const uint64_t FOREIGN_MODEL        = 1<<18;
const uint64_t FOREIGN_POLYGON      = 1<<19;
const uint64_t FOREIGN_POLY_HITBOX  = 1<<20;
const uint64_t FOREIGN_HITBOX       = 1<<21;
const uint64_t FOREIGN_SHADOW       = 1<<22;
const uint64_t FOREIGN_LIGHT_SOURCE = 1<<23;

extern VKSK_EngineConfig gEngineConfig;

static inline const char *getTypeString(uint64_t type, char *buf) {
	if (type & FOREIGN_STRING) {
		strcat(buf, "String");
	}
	if (type & FOREIGN_BOOL) {
		if (strlen(buf) > 0)
			strcat(buf, " or ");
		strcat(buf, "Bool");
	}
	if (type & FOREIGN_NUM) {
		if (strlen(buf) > 0)
			strcat(buf, " or ");
		strcat(buf, "Num");
	}
	if (type & FOREIGN_LIST) {
		if (strlen(buf) > 0)
			strcat(buf, " or ");
		strcat(buf, "List");
	}
	if (type & FOREIGN_MAP) {
		if (strlen(buf) > 0)
			strcat(buf, " or ");
		strcat(buf, "Map");
	}
	if (type & FOREIGN_NULL) {
		if (strlen(buf) > 0)
			strcat(buf, " or ");
		strcat(buf, "Null");
	}
	if (type & FOREIGN_FOREIGN) {
		if (strlen(buf) > 0)
			strcat(buf, " or ");
		strcat(buf, "Foreign");
	}
	if (type & FOREIGN_INI) {
		if (strlen(buf) > 0)
			strcat(buf, " or ");
		strcat(buf, "INI");
	}
	if (type & FOREIGN_BITMAP_FONT) {
		if (strlen(buf) > 0)
			strcat(buf, " or ");
		strcat(buf, "Bitmap Font");
	}
	if (type & FOREIGN_TEXTURE) {
		if (strlen(buf) > 0)
			strcat(buf, " or ");
		strcat(buf, "Texture");
	}
	if (type & FOREIGN_SPRITE) {
		if (strlen(buf) > 0)
			strcat(buf, " or ");
		strcat(buf, "Sprite");
	}
	if (type & FOREIGN_SURFACE) {
		if (strlen(buf) > 0)
			strcat(buf, " or ");
		strcat(buf, "Surface");
	}
	if (type & FOREIGN_CAMERA) {
		if (strlen(buf) > 0)
			strcat(buf, " or ");
		strcat(buf, "Camera");
	}
	if (type & FOREIGN_AUDIO_DATA) {
		if (strlen(buf) > 0)
			strcat(buf, " or ");
		strcat(buf, "Audio Data");
	}
	if (type & FOREIGN_AUDIO) {
		if (strlen(buf) > 0)
			strcat(buf, " or ");
		strcat(buf, "Audio");
	}
	if (type & FOREIGN_TILED_MAP) {
		if (strlen(buf) > 0)
			strcat(buf, " or ");
		strcat(buf, "Tiled Map");
	}
	if (type & FOREIGN_BUFFER) {
		if (strlen(buf) > 0)
			strcat(buf, " or ");
		strcat(buf, "Buffer");
	}
	if (type & FOREIGN_SHADER) {
		if (strlen(buf) > 0)
			strcat(buf, " or ");
		strcat(buf, "Shader");
	}
	if (type & FOREIGN_MODEL) {
		if (strlen(buf) > 0)
			strcat(buf, " or ");
		strcat(buf, "Model");
	}
    if (type & FOREIGN_POLYGON) {
        if (strlen(buf) > 0)
            strcat(buf, " or ");
        strcat(buf, "Polygon");
    }
    if (type & FOREIGN_POLY_HITBOX) {
        if (strlen(buf) > 0)
            strcat(buf, " or ");
        strcat(buf, "Polygon Hitbox");
    }
	return buf;
}

static inline uint64_t getSlotType(WrenVM *vm, int slot) {
	const WrenType wrenType = wrenGetSlotType(vm, slot);
	if (wrenType == WREN_TYPE_FOREIGN) {
		VKSK_RuntimeForeign *f = wrenGetSlotForeign(vm, slot);
		return f->type;
	} else {
		if (wrenType == WREN_TYPE_STRING) return FOREIGN_STRING;
		if (wrenType == WREN_TYPE_BOOL) return FOREIGN_BOOL;
		if (wrenType == WREN_TYPE_NUM) return FOREIGN_NUM;
		if (wrenType == WREN_TYPE_LIST) return FOREIGN_LIST;
		if (wrenType == WREN_TYPE_MAP) return FOREIGN_MAP;
		if (wrenType == WREN_TYPE_NULL) return FOREIGN_NULL;
	}
	return UINT64_MAX;
}

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
				// Get expected type
				uint64_t expectedType = getSlotType(vm, slot);
				char buf1[200] = {0};
				char buf2[200] = {0};
				
				vksk_Error(true, "Argument %i in foreign method is of type %s, but expected %s", slot, getTypeString(expectedType, buf2), getTypeString(type, buf1));
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

extern SDL_Window *gWindow;
void vksk_PrintStackTrace(const char *errorString);
void vksk_Error(bool fatal, const char *fmt, ...) {
	char buffer[1024];
	va_list list;
	va_start(list, fmt);
	vsnprintf(buffer, 1023, fmt, list);
	fflush(stdout);
	va_end(list);
	vksk_PrintStackTrace(buffer);
	if (fatal) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "An error has occurred. Check astroerror.txt for more information.", gWindow);
		abort();
	}
}