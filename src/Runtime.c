/// \file Runtime.c
/// \author Paolo Mazzon
#define SDL_MAIN_HANDLED
#include <VK2D/VK2D.h>
#include <JamUtil/JamUtil.h>
#include <VK2D/stb_image.h>

#include "src/Runtime.h"
#include "src/VMConfig.h"
#include "src/RendererBindings.h"
#include "src/Validation.h"

extern Uint32 rmask, gmask, bmask, amask;

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
double gLastTime = 0;
double gFrames = 0;
double gFPS = 0;

static void _vksk_SetWindowIcon(WrenVM *vm) {
	if (wrenHasVariable(vm, "init", "window_icon")) {
		// Get filename and load pixels
		wrenEnsureSlots(vm, 1);
		wrenGetVariable(vm, "init", "window_icon", 0);
		const char *filename = wrenGetSlotString(vm, 0);
		int x, y, channels;
		uint8_t *pixels = stbi_load(filename, &x, &y, &channels, 4);

		if (pixels != NULL) {
			// Convert to SDL surface & set window icon
			SDL_Surface *icon = SDL_CreateRGBSurfaceFrom(pixels, x, y, channels * 8, x * channels, rmask, gmask, bmask, amask);
			SDL_SetWindowIcon(gWindow, icon);
			SDL_FreeSurface(icon);
		}

		stbi_image_free(pixels);
	}
}

// From RendererBindings.c
void vksk_LoadVK2DConfigFromMap(WrenVM *vm, int mapSlot, const char **windowTitle, int *windowWidth, int *windowHeight, bool *fullscreen, VK2DRendererConfig *config);

void vksk_Start() {
	// Compile the assets code
	gAssetsFile = vksk_CompileAssetFile();
	vksk_Log("---------------------Compiled assets file---------------------\n%s\n---------------------Compiled assets file---------------------", gAssetsFile);

	// Wren config and VM initialization
	vksk_Log("Starting VM...");
	WrenConfiguration config;
	wrenInitConfiguration(&config);
	config.writeFn = &vksk_WrenWriteFn;
	config.errorFn = &vksk_WrenErrorFn;
	config.loadModuleFn = &vksk_WrenLoadModule;
	config.bindForeignMethodFn = &vksk_WrenBindForeignMethod;
	config.bindForeignClassFn = &vksk_WrenBindForeignClass;
	WrenVM *vm = wrenNewVM(&config);

	// Import the initialization module
	vksk_Log("Loading init file...");
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
	bool fullscreen;
	VK2DRendererConfig rendererConfig;
	wrenGetVariable(vm, "init", "renderer_config", 0);
	vksk_LoadVK2DConfigFromMap(vm, 0, &windowTitle, &windowWidth, &windowHeight, &fullscreen, &rendererConfig);

	// Create VK2D and all that
	vksk_Log("Starting Vulkan2D...");
	gWindow = SDL_CreateWindow(
			windowTitle,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			windowWidth,
			windowHeight,
			SDL_WINDOW_VULKAN | (fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0)
			);
	vk2dRendererInit(gWindow, rendererConfig);
	juInit(gWindow, 0, 0);

	// Load window icon if one was selected
	_vksk_SetWindowIcon(vm);

	// Load assets
	vksk_Log("Loading assets...");
	wrenInterpret(vm, "__top__", "import \"Assets\" for Assets\nAssets.load_assets()\n");

	// Run starting level create function and FPS cap
	vksk_Log("Running first level create function...");
	wrenEnsureSlots(vm, 1);
	wrenSetSlotHandle(vm, 0, gCurrentLevel);
	wrenCall(vm, createHandle);
	juClockStart(&gFPSClock);

	// Game loop
	vksk_Log("Beginning game loop...");
	gLastTime = juTime();
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

		// Calculate FPS
		gFrames += 1;
		if (juTime() - gLastTime >= 1) {
			gFPS = gFrames / (juTime() - gLastTime);
			gLastTime = juTime();
			gFrames = 0;
		}
	}

	// Cleanup
	vksk_Log("Cleanup...");
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

void vksk_RuntimeFPS(WrenVM *vm) {
	double r = round(gFPS * 100) / 100;
	wrenSetSlotDouble(vm, 0, r);
}