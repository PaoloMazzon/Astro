/// \file Input.c
/// \author Paolo Mazzon

#include "src/JamUtil.h"
#include "src/Input.h"
#include "src/Validation.h"
#include "src/IntermediateTypes.h"

// Globals
extern SDL_Window *gWindow; // -- from src/Runtime.c
extern bool gMouseButtons[3]; // -- from src/Runtime.c
extern bool gMouseButtonsPrevious[3]; // -- from src/Runtime.c

void vksk_RuntimeInputCheckKey(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	wrenSetSlotBool(vm, 0, juKeyboardGetKey((int)wrenGetSlotDouble(vm, 1)));
}

void vksk_RuntimeInputCheckKeyPressed(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	wrenSetSlotBool(vm, 0, juKeyboardGetKeyPressed((int)wrenGetSlotDouble(vm, 1)));
}

void vksk_RuntimeInputCheckKeyReleased(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	wrenSetSlotBool(vm, 0, juKeyboardGetKeyReleased((int)wrenGetSlotDouble(vm, 1)));
}

void vksk_RuntimeInputGetMousePosition(WrenVM *vm) {
	vec2 vec;
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (wrenGetSlotType(vm, 1) != WREN_TYPE_NULL) {
		VALIDATE_FOREIGN_ARGS(vm, FOREIGN_CAMERA, FOREIGN_END)
		VKSK_RuntimeForeign *cam = wrenGetSlotForeign(vm, 1);
		int w, h;
		SDL_GetWindowSize(gWindow, &w, &h);
		float horizontalFactor = cam->camera.spec.w / (float)w;
		float verticalFactor = cam->camera.spec.h / (float)h;
		vec[0] = ((double)x * horizontalFactor) + cam->camera.spec.x;
		vec[1] = ((double)y * verticalFactor) + cam->camera.spec.y;
	} else {
		vec[0] = x;
		vec[1] = y;
	}

	wrenEnsureSlots(vm, 2);
	wrenSetSlotNewList(vm, 0);
	for (int i = 0; i < 2; i++) {
		wrenSetSlotDouble(vm, 1, vec[i]);
		wrenInsertInList(vm, 0, -1, 1);
	}
}

void vksk_RuntimeInputGetMouseLeftButton(WrenVM *vm) {
	wrenSetSlotBool(vm, 0, gMouseButtons[0]);
}

void vksk_RuntimeInputGetMouseLeftButtonPressed(WrenVM *vm) {
	wrenSetSlotBool(vm, 0, gMouseButtons[0] && !gMouseButtonsPrevious[0]);
}

void vksk_RuntimeInputGetMouseLeftButtonReleased(WrenVM *vm) {
	wrenSetSlotBool(vm, 0, !gMouseButtons[0] && gMouseButtonsPrevious[0]);
}

void vksk_RuntimeInputGetMouseMiddleButton(WrenVM *vm) {
	wrenSetSlotBool(vm, 0, gMouseButtons[1]);
}

void vksk_RuntimeInputGetMouseMiddleButtonPressed(WrenVM *vm) {
	wrenSetSlotBool(vm, 0, gMouseButtons[1] && !gMouseButtonsPrevious[1]);
}

void vksk_RuntimeInputGetMouseMiddleButtonReleased(WrenVM *vm) {
	wrenSetSlotBool(vm, 0, !gMouseButtons[1] && gMouseButtonsPrevious[1]);
}

void vksk_RuntimeInputGetMouseRightButton(WrenVM *vm) {
	wrenSetSlotBool(vm, 0, gMouseButtons[2]);
}

void vksk_RuntimeInputGetMouseRightButtonPressed(WrenVM *vm) {
	wrenSetSlotBool(vm, 0, gMouseButtons[2] && !gMouseButtonsPrevious[2]);
}

void vksk_RuntimeInputGetMouseRightButtonReleased(WrenVM *vm) {
	wrenSetSlotBool(vm, 0, !gMouseButtons[2] && gMouseButtonsPrevious[2]);
}
