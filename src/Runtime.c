/// \file Runtime.c
/// \author Paolo Mazzon
#define SDL_MAIN_HANDLED
#include <VK2D/VK2D.h>
#include <JamUtil/JamUtil.h>

#include "src/Runtime.h"
#include "src/VMConfig.h"
#include "src/RendererBindings.h"
#include "src/Validation.h"

// Globals
SDL_Window *gWindow = NULL;
WrenHandle *gCurrentLevel = NULL;
WrenHandle *gNextLevel = NULL;
bool gQuit = false;
double gFPSCap = 0;
JUClock gFPSClock = {};
bool gMouseButtons[3] = {};
bool gMouseButtonsPrevious[3] = {};
const char *gAssetsFile;

// From RendererBindings.c
void vksk_LoadVK2DConfigFromMap(WrenVM *vm, int mapSlot, const char **windowTitle, int *windowWidth, int *windowHeight, VK2DRendererConfig *config);

void vksk_Start() {
	// Compile the assets code
	gAssetsFile = vksk_CompileAssetFile();

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

	// Load assets
	wrenInterpret(vm, "__top__", "import \"Assets\" for Assets\nAssets.load_assets()\n");

	// Run starting level create function and FPS cap
	wrenEnsureSlots(vm, 1);
	wrenSetSlotHandle(vm, 0, gCurrentLevel);
	wrenCall(vm, createHandle);
	juClockStart(&gFPSClock);

	// Game loop
	while (!gQuit) {
		juUpdate();
		SDL_Event e;
		while (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				gQuit = true;

		// Deal with mouse buttons
		Uint32 buttons = SDL_GetMouseState(NULL, NULL);
		gMouseButtonsPrevious[0] = gMouseButtons[0];
		gMouseButtonsPrevious[1] = gMouseButtons[1];
		gMouseButtonsPrevious[2] = gMouseButtons[2];
		gMouseButtons[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
		gMouseButtons[1] = buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE);
		gMouseButtons[2] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

		// Update VK2D and run the level update function
		VK2DCameraSpec spec = vk2dCameraGetSpec(VK2D_DEFAULT_CAMERA);
		spec.zoom = 1;
		spec.yOnScreen = spec.y = 0;
		vk2dCameraUpdate(VK2D_DEFAULT_CAMERA, spec);
		vk2dRendererStartFrame(VK2D_BLACK);
		wrenSetSlotHandle(vm, 0, gCurrentLevel);
		wrenCall(vm, updateHandle);
		vk2dRendererEndFrame();

		// Enfore the FPS clock
		if (gFPSCap != 0)
			juClockFramerate(&gFPSClock, gFPSCap);

		// Run the level creation/destruction functions if need be
		if (gQuit || gNextLevel != NULL) {
			wrenSetSlotHandle(vm, 0, gCurrentLevel);
			wrenCall(vm, destroyHandle);
            wrenReleaseHandle(vm, gCurrentLevel);
			if (gNextLevel != NULL) {
				gCurrentLevel = gNextLevel;
				gNextLevel = NULL;
				wrenSetSlotHandle(vm, 0, gCurrentLevel);
				wrenCall(vm, createHandle);
			}
		}
	}

	// Cleanup
	vk2dRendererWait();
	wrenCollectGarbage(vm);
	wrenFreeVM(vm);
	juQuit();
	vk2dRendererQuit();
	SDL_DestroyWindow(gWindow);
}

void vksk_RuntimeSwitchLevel(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM)
	gNextLevel = wrenGetSlotHandle(vm, 1);
}

void vksk_RuntimeQuit(WrenVM *vm) {
	gQuit = true;
}

void vksk_RuntimeCapFPS(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM)
	gFPSCap = wrenGetSlotDouble(vm, 1);
	juClockStart(&gFPSClock);
}

void vksk_RuntimeDelta(WrenVM *vm) {
	wrenSetSlotDouble(vm, 0, juDelta());
}

void vksk_RuntimeTime(WrenVM *vm) {
	wrenSetSlotDouble(vm, 0, juTime());
}