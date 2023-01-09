/// \file Runtime.c
/// \author Paolo Mazzon
#define SDL_MAIN_HANDLED
#include <VK2D/VK2D.h>
#include <JamUtil/JamUtil.h>

#include "src/Runtime.h"
#include "src/VMConfig.h"
#include "src/RendererBindings.h"

// Globals
SDL_Window *gWindow = NULL;
WrenHandle *gCurrentLevel = NULL;
WrenHandle *gNextLevel = NULL;
bool gQuit = false;

// From RendererBindings.c
void vksk_LoadVK2DConfigFromMap(WrenVM *vm, int mapSlot, const char **windowTitle, int *windowWidth, int *windowHeight, VK2DRendererConfig *config);

void vksk_Start() {
	// Wren config and VM initialization
	WrenConfiguration config;
	wrenInitConfiguration(&config);
	config.writeFn = &vksk_WrenWriteFn;
	config.errorFn = &vksk_WrenErrorFn;
	config.loadModuleFn = &vksk_WrenLoadModule;
	config.bindForeignMethodFn = &vksk_WrenBindForeignMethod;
	config.bindForeignClassFn = &vksk_WrenBindForeignClass;
	WrenVM *vm = wrenNewVM(&config);

	// Import the initialization module
	wrenInterpret(vm, "__top__", "import \"init\"");

	// Create handles for the 3 primary functions of a level
	WrenHandle *createHandle = wrenMakeCallHandle(vm, "create()");
	WrenHandle *updateHandle = wrenMakeCallHandle(vm, "update()");
	WrenHandle *destroyHandle = wrenMakeCallHandle(vm, "destroy()");

	// Load the first level into gCurrentLevel
	wrenEnsureSlots(vm, 1);
	wrenGetVariable(vm, "init", "start_level", 0);
	gCurrentLevel = wrenGetSlotHandle(vm, 0);

	// Load VK2D settings
	const char *windowTitle;
	int windowWidth, windowHeight;
	VK2DRendererConfig rendererConfig;
	wrenGetVariable(vm, "init", "renderer_config", 0);
	vksk_LoadVK2DConfigFromMap(vm, 0, &windowTitle, &windowWidth, &windowHeight, &rendererConfig);

	// Create VK2D and all that
	gWindow = SDL_CreateWindow(
			windowTitle,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			windowWidth,
			windowHeight,
			SDL_WINDOW_VULKAN
			);
	vk2dRendererInit(gWindow, rendererConfig);
	juInit(gWindow, 0, 0);



	// Cleanup
	vk2dRendererWait();
	juQuit();
	vk2dRendererQuit();
	SDL_DestroyWindow(gWindow);
	wrenFreeVM(vm);
}

void vksk_RuntimeSwitchLevel(WrenVM *vm) {
	gNextLevel = wrenGetSlotHandle(vm, 1);
}

void vksk_RuntimeQuit(WrenVM *vm) {
	gQuit = true;
}