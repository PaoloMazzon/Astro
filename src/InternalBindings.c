/// \file InternalBindings.c
/// \author Paolo Mazzon
#include <stdio.h>

#include "src/InternalBindings.h"
#include "src/ConfigFile.h"
#include "src/Validation.h"

void vksk_RuntimeINIAllocate(WrenVM *vm) {
	VKSK_Config *conf = wrenSetSlotNewForeign(vm, 0, 0, sizeof(struct VKSK_Config));
	*conf = vksk_ConfigLoad(wrenGetSlotString(vm, 1));
}

void vksk_RuntimeINIFinalize(void *data) {
	vksk_ConfigFree(*((VKSK_Config*)data));
}

void vksk_RuntimeINIFlush(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING)
	VKSK_Config *conf = wrenGetSlotForeign(vm, 0);
	vksk_ConfigFlush(*conf, wrenGetSlotString(vm, 1));
}

void vksk_RuntimeINIKeyExists(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_STRING)
	VKSK_Config *conf = wrenGetSlotForeign(vm, 0);
	wrenSetSlotBool(vm, 0, vksk_ConfigKeyExists(*conf, wrenGetSlotString(vm, 1), wrenGetSlotString(vm, 2)));
}

void vksk_RuntimeINIGetString(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_STRING, FOREIGN_STRING)
	VKSK_Config *conf = wrenGetSlotForeign(vm, 0);
	wrenSetSlotString(vm, 0, vksk_ConfigGetString(*conf, wrenGetSlotString(vm, 1), wrenGetSlotString(vm, 2), wrenGetSlotString(vm, 3)));
}

void vksk_RuntimeINISetString(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_STRING, FOREIGN_STRING)
	VKSK_Config *conf = wrenGetSlotForeign(vm, 0);
	vksk_ConfigSetString(*conf, wrenGetSlotString(vm, 1), wrenGetSlotString(vm, 2), wrenGetSlotString(vm, 3));
}

void vksk_RuntimeINIGetNum(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_STRING, FOREIGN_NUM)
	VKSK_Config *conf = wrenGetSlotForeign(vm, 0);
	wrenSetSlotDouble(vm, 0, vksk_ConfigGetDouble(*conf, wrenGetSlotString(vm, 1), wrenGetSlotString(vm, 2), wrenGetSlotDouble(vm, 3)));
}

void vksk_RuntimeINISetNum(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_STRING, FOREIGN_NUM)
	VKSK_Config *conf = wrenGetSlotForeign(vm, 0);
	vksk_ConfigSetDouble(*conf, wrenGetSlotString(vm, 1), wrenGetSlotString(vm, 2), wrenGetSlotDouble(vm, 3));
}

void vksk_RuntimeINIGetBool(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_STRING, FOREIGN_BOOL)
	VKSK_Config *conf = wrenGetSlotForeign(vm, 0);
	wrenSetSlotBool(vm, 0, vksk_ConfigGetBool(*conf, wrenGetSlotString(vm, 1), wrenGetSlotString(vm, 2), wrenGetSlotBool(vm, 3)));
}

void vksk_RuntimeINISetBool(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_STRING, FOREIGN_BOOL)
	VKSK_Config *conf = wrenGetSlotForeign(vm, 0);
	vksk_ConfigSetBool(*conf, wrenGetSlotString(vm, 1), wrenGetSlotString(vm, 2), wrenGetSlotBool(vm, 3));
}

const char* loadFile(const char *filename); // from VMConfig.c
bool _vk2dFileExists(const char *filename);

void vksk_RuntimeFileRead(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING)
	const char *fname = wrenGetSlotString(vm, 1);
	if (_vk2dFileExists(fname)) {
		const char *str = loadFile(fname);
		wrenSetSlotString(vm, 0, str);
		free((void*)str);
	} else {
		wrenSetSlotNull(vm, 0);
	}
}

void vksk_RuntimeFileWrite(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_STRING)
	const char *fname = wrenGetSlotString(vm, 1);
	const char *val = wrenGetSlotString(vm, 2);
	FILE *f = fopen(fname, "w");
	fprintf(f, "%s", val);
	fclose(f);
}

void vksk_RuntimeFileExists(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING)
	const char *fname = wrenGetSlotString(vm, 1);
	wrenSetSlotBool(vm, 0, _vk2dFileExists(fname));
}