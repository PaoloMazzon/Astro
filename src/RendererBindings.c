/// \file RendererBindings.c
/// \author Paolo Mazzon
#define SDL_MAIN_HANDLED
#include <VK2D/VK2D.h>
#include <JamUtil/JamUtil.h>
#include "wren.h"

#include "src/RendererBindings.h"

// Globals
SDL_Window *gWindow = NULL;

void vksk_RuntimeRendererInit(WrenVM *vm) {
	// TODO: This
}

void vksk_RuntimeRendererUpdate(WrenVM *vm) {
	// TODO: This
	wrenSetSlotBool(vm, 0, true);
}

void vksk_RendererCleanup() {
	if (gWindow != NULL) {
		vk2dRendererWait();
		juQuit();
		vk2dRendererQuit();
		SDL_DestroyWindow(gWindow);
	}
}