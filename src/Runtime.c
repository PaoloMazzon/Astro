/// \file Runtime.c
/// \author Paolo Mazzon
#define SDL_MAIN_HANDLED
#include <VK2D/VK2D.h>
#include <VK2D/stb_image.h>
#include <wren/src/vm/wren_debug.h>
#include "src/JamUtil.h"
#include "src/Runtime.h"
#include "src/VMConfig.h"
#include "src/RendererBindings.h"
#include "src/Validation.h"
#include "src/IntermediateTypes.h"
#include "src/Blobs.h"

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
extern VKSK_EngineConfig gEngineConfig;
static WrenVM *vm;
VKSK_Pak gGamePak = NULL;

// Local globals
static VK2DTexture gDebugFont;
static VK2DImage gDebugFontImage;
static VK2DTexture gDebugGraph;
static VK2DImage gDebugGraphImage;
static int gEntityCount;
static bool gProcessFrame; // Whether or not we call update methods this frame
static double gPreviousTimeStep; // Previous time gProcessFrame was enabled
static double gTimeStep; // How many frames are allowed to update each second
static double gTimeStepPercentProc = 0.95;
static double gAverageTimeStep = 0;
static double gTotalTimeSteps = 0;
static double gTimeStepDistributions = 0;
static double gAverageTimeStepDistribution = 0;

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

static void _vksk_InitializeDebug() {
	int x, y, channels;
	uint8_t *pixels = stbi_load_from_memory(DEBUG_FONT_PNG, sizeof(DEBUG_FONT_PNG), &x, &y, &channels, 4);
	gDebugFontImage = vk2dImageFromPixels(vk2dRendererGetDevice(), pixels, x, y);
	gDebugFont = vk2dTextureLoadFromImage(gDebugFontImage);
	stbi_image_free(pixels);
	pixels = stbi_load_from_memory(DEBUG_DISTRITUBTION_GRAPH_PNG, sizeof(DEBUG_DISTRITUBTION_GRAPH_PNG), &x, &y, &channels, 4);
	gDebugGraphImage = vk2dImageFromPixels(vk2dRendererGetDevice(), pixels, x, y);
	gDebugGraph = vk2dTextureLoadFromImage(gDebugGraphImage);
	stbi_image_free(pixels);
}

static void _vksk_DebugPrint(float x, float y, const char *fmt, ...) {
	if (gEngineConfig.enableDebugOverlay) {
		char buffer[1024];
		va_list list;
		va_start(list, fmt);
		vsnprintf(buffer, 1024, fmt, list);
		va_end(list);
		for (int i = 0; i < strlen(buffer); i++) {
			int index = buffer[i] - 32;

			if (index >= 0 && index < 96) {
				float drawX = roundf((index * 16) % 256);
				float drawY = roundf(32 * floorf((index * 16) / (256)));
				vk2dDrawTexturePart(gDebugFont, x, y, drawX, drawY, 16, 32);
				x += 16;
			}
		}
	}
}

static void _vksk_DrawDebugOverlay() {
	vk2dRendererLockCameras(VK2D_DEFAULT_CAMERA);
	_vksk_DebugPrint(2, 0, "FPS: %0.2f", gFPS);
	_vksk_DebugPrint(2, 34, "Entities: %i", gEntityCount);
	if (gTimeStep != 0) {
		_vksk_DebugPrint(2, 34 + 34, "Time Step: %0.2ffps | TS%: %0.2f% | ", gAverageTimeStep, gTimeStepPercentProc);
		float x = 35 * 16;
		float lx = x + (128 * gAverageTimeStepDistribution);
		vk2dDrawTexture(gDebugGraph, x, 34 + 34);
		vk2dRendererSetColourMod(VK2D_BLACK);
		vk2dDrawRectangle(lx - 1, 34 + 34, 2, 32);
		vk2dRendererSetColourMod(VK2D_DEFAULT_COLOUR_MOD);
	}
	vk2dRendererUnlockCameras();
}

static void _vksk_FinalizeDebug() {
	vk2dTextureFree(gDebugFont);
	vk2dImageFree(gDebugFontImage);
	vk2dTextureFree(gDebugGraph);
	vk2dImageFree(gDebugGraphImage);
}

void vksk_PrintStackTrace(const char *errorString) {
	// Bail if the host doesn't enable printing errors.
	if (vm->config.errorFn == NULL) return;

	ObjFiber* fiber = vm->fiber;
	if (IS_STRING(fiber->error))
	{
		vm->config.errorFn(vm, WREN_ERROR_RUNTIME,
						   NULL, -1, AS_CSTRING(fiber->error));
	}
	else
	{
		// TODO: Print something a little useful here. Maybe the name of the error's
		// class?
		vm->config.errorFn(vm, WREN_ERROR_RUNTIME,
						   NULL, -1, errorString);
	}

	for (int i = fiber->numFrames - 1; i >= 0; i--)
	{
		CallFrame* frame = &fiber->frames[i];
		ObjFn* fn = frame->closure->fn;

		// Skip over stub functions for calling methods from the C API.
		if (fn->module == NULL) continue;

		// The built-in core module has no name. We explicitly omit it from stack
		// traces since we don't want to highlight to a user the implementation
		// detail of what part of the core module is written in C and what is Wren.
		if (fn->module->name == NULL) continue;

		// -1 because IP has advanced past the instruction that it just executed.
		int line = fn->debug->sourceLines.data[frame->ip - fn->code.data - 1];
		vm->config.errorFn(vm, WREN_ERROR_STACK_TRACE,
						   fn->module->name->value, line,
						   fn->debug->name);
	}
}

// From RendererBindings.c
void vksk_LoadVK2DConfigFromMap(WrenVM *vm, int mapSlot, const char **windowTitle, int *windowWidth, int *windowHeight, bool *fullscreen, VK2DRendererConfig *config);
void _vksk_RuntimeControllerRefresh(); // From InternalBindings.c
void _vksk_RuntimeControllersUpdate(); // From InternalBindings.c

void vksk_Start() {
	// Compile the assets code
	gAssetsFile = vksk_CompileAssetFile();
	if (gEngineConfig.enableAssetsPrint)
		vksk_Log("---------------------Compiled assets file---------------------\n%s\n---------------------Compiled assets file---------------------", gAssetsFile);

	// Load pak file
	if (gEngineConfig.disableGamePak)
		gGamePak = NULL;
	else
		gGamePak = vksk_PakLoad("game.pak");

	// Wren config and VM initialization
	vksk_Log("Starting VM...");
	WrenConfiguration config;
	wrenInitConfiguration(&config);
	config.writeFn = &vksk_WrenWriteFn;
	config.errorFn = &vksk_WrenErrorFn;
	config.loadModuleFn = &vksk_WrenLoadModule;
	config.bindForeignMethodFn = &vksk_WrenBindForeignMethod;
	config.bindForeignClassFn = &vksk_WrenBindForeignClass;
	config.initialHeapSize = 1024 * 1024 * 100; // 100mb
	vm = wrenNewVM(&config);

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
	SDL_Init(SDL_INIT_EVERYTHING);
	gWindow = SDL_CreateWindow(
			windowTitle,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			windowWidth,
			windowHeight,
			SDL_WINDOW_VULKAN | (fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0)
			);
	VK2DStartupOptions options;
	options.enableDebug = false;
	options.loadCustomShaders = false;
	options.stdoutLogging = false;
	options.quitOnError = true;
	options.errorFile = "astroerror.txt";
	vk2dRendererInit(gWindow, rendererConfig, &options);
	juInit(gWindow, 0, 0);

	// Internal stuff
	_vksk_SetWindowIcon(vm);
	_vksk_InitializeDebug();
	_vksk_RuntimeControllerRefresh();

	// Load assets
	vksk_Log("Loading assets...");
	wrenInterpret(vm, "__top__", "import \"Assets\" for Assets\nAssets.load_assets()\n");

	// Setup camera
	VK2DCameraSpec spec = vk2dCameraGetSpec(VK2D_DEFAULT_CAMERA);
	spec.zoom = 1;
	spec.yOnScreen = spec.y = 0;
	vk2dCameraUpdate(VK2D_DEFAULT_CAMERA, spec);

	// Run starting level create function and FPS cap
	vksk_Log("Running first level create function...");
	vk2dRendererStartFrame(VK2D_BLACK);
	wrenEnsureSlots(vm, 1);
	wrenSetSlotHandle(vm, 0, gCurrentLevel);
	wrenCall(vm, createHandle);
	vk2dRendererEndFrame();
	juClockStart(&gFPSClock);

	// Game loop
	vksk_Log("Beginning game loop...");
	gLastTime = juTime();
	while (!gQuit) {
		// Calculate timestep
		if (gTimeStep != 0) {
			gProcessFrame = false;
			if (((gFPSCap <= gTimeStep && gFPSCap != 0) || (juTime() - gPreviousTimeStep) / (1.0 / gTimeStep) > gTimeStepPercentProc) && gTotalTimeSteps < gTimeStep) {
				gProcessFrame = true;
				gPreviousTimeStep = juTime();
				gTotalTimeSteps += 1;
				gTimeStepDistributions += (juTime() - gLastTime);
			}
		} else {
			gProcessFrame = true;
		}

		if (gProcessFrame) {
			juUpdate();
			SDL_Event e;
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) {
					gQuit = true;
				} else if (e.type == SDL_CONTROLLERDEVICEADDED || e.type == SDL_CONTROLLERDEVICEREMOVED) {
					_vksk_RuntimeControllerRefresh();
				}
			}
			_vksk_RuntimeControllersUpdate();

			// Deal with mouse buttons
			Uint32 buttons = SDL_GetMouseState(NULL, NULL);
			gMouseButtonsPrevious[0] = gMouseButtons[0];
			gMouseButtonsPrevious[1] = gMouseButtons[1];
			gMouseButtonsPrevious[2] = gMouseButtons[2];
			gMouseButtons[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
			gMouseButtons[1] = buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE);
			gMouseButtons[2] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
		}

		// Start the frame and run update
		vk2dRendererStartFrame(VK2D_BLACK);
		wrenSetSlotHandle(vm, 0, gCurrentLevel);
		wrenCall(vm, updateHandle);

		// Debug overlay
		_vksk_DrawDebugOverlay();

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

		vk2dRendererEndFrame();

		// Calculate FPS/timestep
		gFrames += 1;
		if (juTime() - gLastTime >= 1) {
			gFPS = gFrames / (juTime() - gLastTime);
			gFrames = 0;

			// Adjust timestep
			if (gTimeStep != 0) {
				gAverageTimeStepDistribution = gTimeStepDistributions / gTotalTimeSteps;
				gTimeStepDistributions = 0;
				if (gTotalTimeSteps + 1 < gTimeStep)
					gTimeStepPercentProc = gTotalTimeSteps / gTimeStep;
				if (gAverageTimeStepDistribution < 0.45)
					gTimeStepPercentProc += 0.05;
				gAverageTimeStep = gTotalTimeSteps / (juTime() - gLastTime);
				gTotalTimeSteps = 0;
			}

			gLastTime = juTime();
		}
	}

	// Cleanup
	vksk_Log("Cleanup...");
	vk2dRendererWait();
	_vksk_FinalizeDebug();
	wrenCollectGarbage(vm);
	wrenFreeVM(vm);
	juQuit();
	vk2dRendererQuit();
	SDL_DestroyWindow(gWindow);
	vksk_PakFree(gGamePak);
}

void vksk_RuntimeSwitchLevel(WrenVM *vm) {
	gNextLevel = wrenGetSlotHandle(vm, 1);
}

void vksk_RuntimeQuit(WrenVM *vm) {
	gQuit = true;
}

void vksk_RuntimeCapFPS(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
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

void vksk_RuntimeInfo(WrenVM *vm) {
	wrenEnsureSlots(vm, 4);
	wrenSetSlotNewMap(vm, 0);
	int mapKeySlot = 1;
	int mapValSlot = 2;
	int listValSlot = 3;
	wrenSetSlotString(vm, mapKeySlot, "name");
	wrenSetSlotString(vm, mapValSlot, "Astro Engine (c) Paolo Mazzon");
	wrenSetMapValue(vm, 0, mapKeySlot, mapValSlot);
	wrenSetSlotString(vm, mapKeySlot, "astro-version");
	wrenSetSlotNewList(vm, mapValSlot);
	wrenSetSlotDouble(vm, listValSlot, ASTRO_VERSION_MAJOR);
	wrenInsertInList(vm, mapValSlot, -1, listValSlot);
	wrenSetSlotDouble(vm, listValSlot, ASTRO_VERSION_MINOR);
	wrenInsertInList(vm, mapValSlot, -1, listValSlot);
	wrenSetSlotDouble(vm, listValSlot, ASTRO_VERSION_PATCH);
	wrenInsertInList(vm, mapValSlot, -1, listValSlot);
	wrenSetMapValue(vm, 0, mapKeySlot, mapValSlot);
	wrenSetSlotString(vm, mapKeySlot, "wren-version");
	wrenSetSlotDouble(vm, mapValSlot, wrenGetVersionNumber());
	wrenSetMapValue(vm, 0, mapKeySlot, mapValSlot);
	wrenSetSlotString(vm, mapKeySlot, "build-date");
	wrenSetSlotString(vm, mapValSlot, __DATE__);
	wrenSetMapValue(vm, 0, mapKeySlot, mapValSlot);
}

void vksk_RuntimeGetClass(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	const char *fullName = wrenGetSlotString(vm, 1);
	char module[200];
	char *temp = strstr(fullName, "::");

	if (temp != NULL) {
		strncpy(module, fullName, temp - fullName);
		module[temp - fullName] = 0;
		temp += 2;
		if (wrenHasModule(vm, module) && wrenHasVariable(vm, module, temp)) {
			wrenGetVariable(vm, module, temp, 0);
		} else {
			wrenSetSlotNull(vm, 0);
		}
	} else {
		wrenSetSlotNull(vm, 0);
	}
}

void vksk_RuntimeReportDebug(WrenVM *vm) {
	gEntityCount = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeTimestep(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
	gTimeStep = wrenGetSlotDouble(vm, 1);
	gPreviousTimeStep = juTime();
}

void vksk_RuntimeProcessFrame(WrenVM *vm) {
	wrenSetSlotBool(vm, 0, gProcessFrame);
}

void vksk_RuntimeTimeStepPercent(WrenVM *vm) {
	wrenSetSlotDouble(vm, 0, (juTime() - gPreviousTimeStep) / (1.0 / gTimeStep));
}

void vksk_RuntimeArgv(WrenVM *vm) {
	wrenEnsureSlots(vm, 2);
	wrenSetSlotNewList(vm, 0);
	for (int i = 0; i < gEngineConfig.argc; i++) {
		wrenSetSlotString(vm, 1, gEngineConfig.argv[i]);
		wrenInsertInList(vm, 0, -1, 1);
	}
}