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
JUClock gFPSClock = {0};
bool gMouseButtons[3] = {0};
bool gMouseButtonsPrevious[3] = {0};
const char *gAssetsFile;
double gLastTime = 0;
double gFrames = 0;
double gFPS = 0;
extern VKSK_EngineConfig gEngineConfig;
static WrenVM *vm;
VKSK_Pak gGamePak = NULL;
bool gOutsideFrame = false; // this specifies if the user is allowed to draw or not
bool gAnyKeyPressed;        // Any key was just pressed
bool gAnyKeyReleased;       // Any key was just released
bool gAnyKey;               // Any key is pressed
int gLastKeyCode = 0;       // Most recent key code
char gLastKey[100] = {0};   // Most recent key pressed

// Local globals
static VK2DTexture gDebugFont;
static VK2DImage gDebugFontImage;
static VK2DTexture gDebugGraph;
static VK2DImage gDebugGraphImage;
static int gEntityCount;
static bool gProcessFrame; // Whether or not we call update methods this frame
static double gPreviousTimeStep = 0; // Previous time gProcessFrame was enabled
static double gTimeStep = 0; // How many frames are allowed to update each second
static double gTimeStepPercentProc = 0.95;
static double gTimeStepRecovery = 0.95;
static double gAverageTimeStep = 0;
static double gTotalTimeSteps = 0;
static double gTimeStepDistributions = 0;
static double gAverageTimeStepDistribution = 0;
static double gDeltaCap = 0;

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
	uint8_t *pixels = stbi_load_from_memory(FONT_PNG, sizeof(FONT_PNG), &x, &y, &channels, 4);
	gDebugFontImage = vk2dImageFromPixels(vk2dRendererGetDevice(), pixels, x, y, true);
	gDebugFont = vk2dTextureLoadFromImage(gDebugFontImage);
	stbi_image_free(pixels);
	pixels = stbi_load_from_memory(DISTRO_PNG, sizeof(DISTRO_PNG), &x, &y, &channels, 4);
	gDebugGraphImage = vk2dImageFromPixels(vk2dRendererGetDevice(), pixels, x, y, true);
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
				float drawX = roundf((index * 21) % 336);
				float drawY = roundf(24 * floorf((index * 21) / (336)));
				vk2dDrawTexturePart(gDebugFont, x, y, drawX, drawY, 21, 24);
				x += 21;
			}
		}
	}
}

static void _vksk_DrawDebugOverlay() {
	vk2dRendererLockCameras(VK2D_DEFAULT_CAMERA);
	_vksk_DebugPrint(0, 0, "FPS: %0.2f", gFPS);
	_vksk_DebugPrint(0, 26, "Entities: %i", gEntityCount);
	if (gTimeStep != 0) {
		_vksk_DebugPrint(0, 26 + 26, "TS:%0.2ffps TS%:%0.2f%", gAverageTimeStep, gTimeStepPercentProc);
		float x = 21 * 21;
		float lx = x + (128 * gAverageTimeStepDistribution);
		vk2dDrawTexture(gDebugGraph, x, 26 + 26);
		vk2dRendererSetColourMod(VK2D_BLACK);
		vk2dDrawRectangle(lx - 1, 26 + 26, 2, 32);
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

// This is pretty much 1:1 ripped out of Wren's source code
void vksk_PrintStackTrace(const char *errorString) {
	ObjFiber* fiber = vm->fiber;
	if (IS_STRING(fiber->error)) {
		vm->config.errorFn(vm, WREN_ERROR_RUNTIME,
						   NULL, -1, AS_CSTRING(fiber->error));
	} else {
		vm->config.errorFn(vm, WREN_ERROR_RUNTIME, NULL, -1, errorString);
	}
	for (int i = fiber->numFrames - 1; i >= 0; i--) {
		CallFrame* frame = &fiber->frames[i];
		ObjFn* fn = frame->closure->fn;
		if (fn->module == NULL) continue;
		if (fn->module->name == NULL) continue;
		int line = fn->debug->sourceLines.data[frame->ip - fn->code.data - 1];
		vm->config.errorFn(vm, WREN_ERROR_STACK_TRACE, fn->module->name->value, line, fn->debug->name);
	}
}

// From RendererBindings.c
void vksk_LoadVK2DConfigFromMap(WrenVM *vm, int mapSlot, const char **windowTitle, int *windowWidth, int *windowHeight, bool *fullscreen, VK2DRendererConfig *config);
void _vksk_RuntimeControllerRefresh(); // From InternalBindings.c
void _vksk_RuntimeControllersUpdate(); // From InternalBindings.c

bool _vk2dFileExists(const char *filename);

extern const unsigned char LOADING_SCREEN_PNG[172483];
void vksk_Start() {
	// Start by loading SDL to draw a surface as a loading screen
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *loadWindow = SDL_CreateWindow(
			"",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			800,
			450,
			SDL_WINDOW_BORDERLESS
	);
	SDL_Surface *windowSurface = SDL_GetWindowSurface(loadWindow);
	int x, y, channels;
	uint8_t *pixels = stbi_load("data/_loading.png", &x, &y, &channels, 4);
	if (pixels == NULL)
		pixels = stbi_load_from_memory(LOADING_SCREEN_PNG, sizeof(LOADING_SCREEN_PNG), &x, &y, &channels, 4);
	SDL_Surface *loadSurface = SDL_CreateRGBSurfaceFrom(pixels, x, y, 32, 4 * x, rmask, gmask, bmask, amask);
	SDL_BlitSurface(loadSurface, NULL, windowSurface, NULL);
	stbi_image_free(pixels);
	SDL_FreeSurface(loadSurface);
	SDL_UpdateWindowSurface(loadWindow);
	uint64_t windowLoadScreenStartTime = SDL_GetPerformanceCounter();

	// Load game pak
	vksk_Log("Locating game pak...");
	// Load pak file
	if (gEngineConfig.disableGamePak || !_vk2dFileExists("game.pak")) {
		gGamePak = NULL;
	} else {
		gGamePak = vksk_PakLoad("game.pak");
		vksk_Log("Game pak located and loaded.");
	}

	// Compile the assets code
	vksk_Log("Compiling assets file...");
	gAssetsFile = vksk_CompileAssetFile("data/");
	if (gEngineConfig.enableAssetsPrint)
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
	config.initialHeapSize = 1024 * 1024 * 10; // 10mb
	config.heapGrowthPercent = 25;
	vm = wrenNewVM(&config);

	// Import the initialization module
	vksk_Log("Loading init file...");
	if (!_vk2dFileExists("data/game/init.wren") && !vksk_PakFileExists(gGamePak, "data/game/init.wren")) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to locate an init.wren file.", gWindow);
		abort();
	}
	wrenInterpret(vm, "__top__", "import \"init\"");

	// Create handles for the 3 primary functions of a level
	WrenHandle *createHandle = wrenMakeCallHandle(vm, "create()");
	WrenHandle *updateHandle = wrenMakeCallHandle(vm, "update()");
	WrenHandle *preFrameHandle = wrenMakeCallHandle(vm, "pre_frame()");
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

	// Wait for the load screen
	wrenEnsureSlots(vm, 1);
	double minimumLoadTime = 3;
	if (wrenHasVariable(vm, "init", "minimum_load_time")) {
		wrenGetVariable(vm, "init", "minimum_load_time", 0);
		minimumLoadTime = wrenGetSlotDouble(vm, 0);
	}
	while (SDL_GetPerformanceCounter() - windowLoadScreenStartTime < minimumLoadTime * SDL_GetPerformanceFrequency()) {
		volatile int i = 0;
	}

	// Create VK2D and all that
	vksk_Log("Starting Vulkan2D...");
	SDL_DestroyWindow(loadWindow);
	gWindow = SDL_CreateWindow(
			windowTitle,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			windowWidth,
			windowHeight,
			SDL_WINDOW_VULKAN | (fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0)
	);

	if (gWindow == NULL) {
		vksk_Error(true, "Failed to create Vulkan window, SDL error: %s", SDL_GetError()); 
	}

	VK2DStartupOptions options = {0};
	options.enableDebug = false;
	options.stdoutLogging = true;
	options.quitOnError = true;
	options.errorFile = "astroerror.txt";
	options.vramPageSize = 1000000; // 1mb page size
	vk2dRendererInit(gWindow, rendererConfig, &options);
	juInit(gWindow, 0, 0);

	// Internal stuff
    _vksk_RendererBindingsInit((void*)FONT_PNG, sizeof(FONT_PNG)); // basically just to create the default font
	_vksk_SetWindowIcon(vm);
	_vksk_InitializeDebug();
	_vksk_RuntimeControllerRefresh();
	double maxFPS = 0;
	double minFPS = 10000;
	double totalFrameCount = 0;

	// Load assets
	vksk_Log("Loading assets...");
	wrenInterpret(vm, "__top__", "import \"Assets\" for Assets\nAssets.load_assets()\n");

	// Stop the garbage collector from deleting the assets
	wrenEnsureSlots(vm, 1);
	wrenGetVariable(vm, "Assets", "Assets", 0);
	WrenHandle *assetsHandle = wrenGetSlotHandle(vm, 0);

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
			gAnyKeyPressed = gAnyKey = gAnyKeyReleased = false;
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) {
					gQuit = true;
				} else if (e.type == SDL_CONTROLLERDEVICEADDED || e.type == SDL_CONTROLLERDEVICEREMOVED) {
					_vksk_RuntimeControllerRefresh();
				} else if (e.type == SDL_KEYDOWN == e.key.repeat == 0) {
					gAnyKey = gAnyKeyPressed = true;
					const char *temp = SDL_GetKeyName(e.key.keysym.sym);
					strncpy(gLastKey, temp, 99);
					gLastKeyCode = e.key.keysym.scancode;
				} else if (e.type == SDL_KEYDOWN == e.key.repeat != 0) {
					gAnyKey = true;
				} else if (e.type == SDL_KEYUP) {
					gAnyKeyReleased = true;
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

		// Run the pre-frame method
		wrenEnsureSlots(vm, 1);
		wrenSetSlotHandle(vm, 0, gCurrentLevel);
		gOutsideFrame = true;
		wrenCall(vm, preFrameHandle);
		gOutsideFrame = false;

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
            wrenEnsureSlots(vm, 1);
			wrenSetSlotHandle(vm, 0, gCurrentLevel);
            vk2dRendererEndFrame();
            gOutsideFrame = true;
			wrenCall(vm, destroyHandle);
			if (gEngineConfig.gcBetweenLevels)
				wrenCollectGarbage(vm);
            wrenReleaseHandle(vm, gCurrentLevel);
			if (gNextLevel != NULL) {
				gCurrentLevel = gNextLevel;
				gNextLevel = NULL;
                vk2dRendererStartFrame(VK2D_BLACK);
                gOutsideFrame = false;
                wrenEnsureSlots(vm, 1);
				wrenSetSlotHandle(vm, 0, gCurrentLevel);
				wrenCall(vm, createHandle);
			}
		}

		vk2dRendererEndFrame();

		// Calculate FPS/timestep
		gFrames += 1;
        totalFrameCount += 1;
		if (juTime() - gLastTime >= 1) {
			gFPS = gFrames / (juTime() - gLastTime);
			maxFPS = gFPS > maxFPS ? gFPS : maxFPS;
			minFPS = gFPS < minFPS ? gFPS : minFPS;
			gFrames = 0;

			// Adjust timestep
			if (gTimeStep != 0) {
				gAverageTimeStepDistribution = gTimeStepDistributions / gTotalTimeSteps;
				gTimeStepDistributions = 0;

				// Recover from bad time step
				if (gAverageTimeStepDistribution > 0.49 && gAverageTimeStepDistribution < 0.51) {
					gTimeStepRecovery = gTimeStepPercentProc;
				} else if (gAverageTimeStepDistribution > 0.6 || gAverageTimeStepDistribution < 0.4) {
					gTimeStepPercentProc = gTimeStepRecovery;
				} else {
					// Adjust drifting time step
					if (gTotalTimeSteps + 1 < gTimeStep)
						gTimeStepPercentProc = gTotalTimeSteps / gTimeStep;
					if (gAverageTimeStepDistribution < 0.48 && gTimeStepPercentProc < 1) {
						gTimeStepPercentProc += 0.01;
					} else if (gAverageTimeStepDistribution > 0.52) {
						gTimeStepPercentProc -= 0.01;
					}
				}

				gAverageTimeStep = gTotalTimeSteps / (juTime() - gLastTime);
				gTotalTimeSteps = 0;
			}

			gLastTime = juTime();
		}
	}

	// Cleanup
    double finalTime = juTime();
	vksk_Log("Cleanup...");
	vk2dRendererWait();
    _vksk_RendererBindingsQuit(vm);
	_vksk_FinalizeDebug();
	wrenReleaseHandle(vm, assetsHandle);
	wrenCollectGarbage(vm);
	wrenFreeVM(vm);
	juQuit();
	vk2dRendererQuit();
	SDL_DestroyWindow(gWindow);
    SDL_Quit();
	vksk_PakFree(gGamePak);
	vksk_Log("Cleanup complete.\n---------------FPS---------------\n   Average | Minimum | Maximum\n   %7.2f | %7.2f | %7.2f\n---------------------------------", totalFrameCount / finalTime, minFPS, maxFPS);
}
/*
 * Cleanup complete.
 * ---------------FPS---------------
 *    Average | Minimum | Maximum
 *      00.00 |   00.00 |   00.00
 * ---------------------------------
 *
 * */

WrenVM *vksk_GetVM() {
	return vm;
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
	gTimeStepPercentProc = 0.95;
	juClockStart(&gFPSClock);
}

void vksk_RuntimeDelta(WrenVM *vm) {
    double delta = juDelta();
    if (gDeltaCap != 0) {
        wrenSetSlotDouble(vm, 0, delta > gDeltaCap ? gDeltaCap : delta);
    } else {
        wrenSetSlotDouble(vm, 0, delta);
    }
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
	double val = (juTime() - gPreviousTimeStep) / (1.0 / gTimeStep);
	if (val > 1)
		val = 1;
	else if (val < 0)
		val = 0;
	wrenSetSlotDouble(vm, 0, val);
}

void vksk_RuntimeArgv(WrenVM *vm) {
	wrenEnsureSlots(vm, 2);
	wrenSetSlotNewList(vm, 0);
	for (int i = 0; i < gEngineConfig.argc; i++) {
		wrenSetSlotString(vm, 1, gEngineConfig.argv[i]);
		wrenInsertInList(vm, 0, -1, 1);
	}
}

void vksk_RuntimeUsingPak(WrenVM *vm) {
	wrenSetSlotBool(vm, 0, gGamePak != NULL);
}

void vksk_RuntimeImportExists(WrenVM *vm) {
	VALIDATE_FOREIGN_ARGS(vm, FOREIGN_STRING, FOREIGN_END)
	const char *name = wrenGetSlotString(vm, 1);
	static const char *ext = ".wren";
	char fname[200] = "data/game/";
	strncat(fname, name, 200 - 1 - 5 - 5);
	strcat(fname, ext);
	wrenSetSlotBool(vm, 0, _vk2dFileExists(fname));
}

void vksk_RuntimeSetDeltaMax(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
    gDeltaCap = wrenGetSlotDouble(vm, 1);
}