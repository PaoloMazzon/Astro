/// \file Input.c
/// \author Paolo Mazzon
#include <JamUtil/JamUtil.h>

#include "src/Input.h"
#include "src/Validation.h"

void vksk_RuntimeInputCheckKey(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM)
	wrenSetSlotBool(vm, 0, juKeyboardGetKey((int)wrenGetSlotDouble(vm, 1)));
}

void vksk_RuntimeInputCheckKeyPressed(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM)
	wrenSetSlotBool(vm, 0, juKeyboardGetKeyPressed((int)wrenGetSlotDouble(vm, 1)));
}

void vksk_RuntimeInputCheckKeyReleased(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM)
	wrenSetSlotBool(vm, 0, juKeyboardGetKeyReleased((int)wrenGetSlotDouble(vm, 1)));
}
