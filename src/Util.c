/// \file Util.c
/// \author Paolo Mazzon
#include "src/Util.h"
#include "src/Runtime.h"
#include "src/Packer.h"

unsigned char* loadFileRaw(const char *filename, int *size);
void *vksk_GetFileBuffer(const char *filename, int *size) {
	if (vksk_PakFileExists(gGamePak, filename)) {
		return vksk_PakGetFile(gGamePak, filename, size);
	}
	return loadFileRaw(filename, size);
}

VKSK_RuntimeForeign *vksk_NewForeignClass(WrenVM *vm, const char *module, const char *name) {
    wrenGetVariable(vm, module, name, 0);
    return wrenSetSlotNewForeign(vm, 0, 0, sizeof(struct VKSK_RuntimeForeign));
}