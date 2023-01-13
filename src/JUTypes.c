/// \file JUTypes.c
/// \author Paolo Mazzon
#include <JamUtil/JamUtil.h>

#include "src/JUTypes.h"

void vksk_RuntimeJUBitmapFontAllocate(WrenVM *vm) {
	JUFont *font = wrenSetSlotNewForeign(vm, 0, 0, sizeof(JUFont));
	*font = juFontLoadFromImage(
			wrenGetSlotString(vm, 1),
			(int)wrenGetSlotDouble(vm, 2),
			(int)wrenGetSlotDouble(vm, 3),
			(int)wrenGetSlotDouble(vm, 4),
			(int)wrenGetSlotDouble(vm, 5)
			);

}

void vksk_RuntimeJUBitmapFontFinalize(void *data) {
	JUFont *font = data;
	juFontFree(*font);
}

void vksk_RuntimeJUBitmapFontFree(WrenVM *vm) {
	JUFont *font = wrenGetSlotForeign(vm, 0);
	juFontFree(*font);
	*font = NULL;
}