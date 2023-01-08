/// \file RendererBindings.c
/// \author Paolo Mazzon
#define SDL_MAIN_HANDLED
#include <VK2D/VK2D.h>
#include <JamUtil/JamUtil.h>
#include "wren.h"

#include "src/RendererBindings.h"

// Globals
SDL_Window *gWindow = NULL;

inline static VK2DMSAA convertToMSAAEnum(double wrenVal) {
	if (wrenVal == 2)
		return msaa_2x;
	if (wrenVal == 4)
		return msaa_4x;
	if (wrenVal == 8)
		return msaa_8x;
	if (wrenVal == 16)
		return msaa_16x;
	if (wrenVal == 32)
		return msaa_32x;
	return msaa_1x;
}

inline static VK2DFilterType convertToFilterTypeEnum(double wrenVal) {
	if (wrenVal == 1)
		return ft_Linear;
	return ft_Nearest;
}

inline static VK2DScreenMode convertToScreenModeEnum(double wrenVal) {
	if (wrenVal == 1)
		return sm_VSync;
	if (wrenVal == 2)
		return sm_TripleBuffer;
	return sm_Immediate;
}

void vksk_RuntimeRendererInit(WrenVM *vm) {
	// Create the window
	gWindow = SDL_CreateWindow(
			wrenGetSlotString(vm, 1),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			wrenGetSlotDouble(vm, 2),
			wrenGetSlotDouble(vm, 3),
			SDL_WINDOW_VULKAN
			);

	// Create the config and the renderer
	VK2DRendererConfig conf = {
			convertToMSAAEnum(wrenGetSlotDouble(vm, 4)),
			convertToScreenModeEnum(wrenGetSlotDouble(vm, 5)),
			convertToFilterTypeEnum(wrenGetSlotDouble(vm, 6)),
	};
	vk2dRendererInit(gWindow, conf);

	// And finally jamutil
	juInit(gWindow, 0, 0);
}

void vksk_RuntimeRendererUpdate(WrenVM *vm) {
	// End the previous frame, update the SDL window/jam util and start the next frame
	bool ret = true;
	vk2dRendererEndFrame();

	juUpdate();
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			ret = false;
		}
	}

	vk2dRendererStartFrame(VK2D_BLACK);
	wrenSetSlotBool(vm, 0, ret);
}

void vksk_RendererCleanup() {
	if (gWindow != NULL) {
		vk2dRendererEndFrame();
		vk2dRendererWait();
		juQuit();
		vk2dRendererQuit();
		SDL_DestroyWindow(gWindow);
	}
}