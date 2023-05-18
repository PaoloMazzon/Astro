/// \file Runtime.h
/// \author Paolo Mazzon
/// \brief The top-level runtime and meta functions that are binded to the VM but dont exist in VK2D/JamUtil
#pragma once
#include <wren.h>
#include "src/Packer.h"

#define ASTRO_VERSION_MAJOR 0
#define ASTRO_VERSION_MINOR 6
#define ASTRO_VERSION_PATCH 11

// Engine configuration
typedef struct VKSK_EngineConfig {
	bool enableTypeChecking;
	bool enableDebugOverlay;
	bool enableAssetsPrint;
	bool disableGamePak;
	bool gcBetweenLevels;
	int argc;
	const char **argv;
} VKSK_EngineConfig;

// Pak file that may be present for the game
extern VKSK_Pak gGamePak;

// Starts the runtime
void vksk_Start();

// Searches through the assets folder and makes a wren source file loading it all
const char *vksk_CompileAssetFile(const char *rootDir);

// Returns teh wren vm
WrenVM *vksk_GetVM();

// Switches the current level
void vksk_RuntimeSwitchLevel(WrenVM *vm);

// Ends the runtime, calling the level destruction function before it does
void vksk_RuntimeQuit(WrenVM *vm);

// Caps the FPS
void vksk_RuntimeCapFPS(WrenVM *vm);

// Returns delta
void vksk_RuntimeDelta(WrenVM *vm);

// Returns time in seconds since the program start
void vksk_RuntimeTime(WrenVM *vm);

// Returns the average FPS each seconds
void vksk_RuntimeFPS(WrenVM *vm);

// Returns a map with engine information
void vksk_RuntimeInfo(WrenVM *vm);

// Returns a specified class
void vksk_RuntimeGetClass(WrenVM *vm);

// Reports debug info to the engine
void vksk_RuntimeReportDebug(WrenVM *vm);

// Sets the timestep for the engine
void vksk_RuntimeTimestep(WrenVM *vm);

// Returns true if this is a process frame
void vksk_RuntimeProcessFrame(WrenVM *vm);

// Returns the percent we are through this timestep
void vksk_RuntimeTimeStepPercent(WrenVM *vm);

// Returns a list of the compiler options
void vksk_RuntimeArgv(WrenVM *vm);

// Returns whether or not Astro is using a game pak right now
void vksk_RuntimeUsingPak(WrenVM *vm);

// Returns true if the module would be found if an import was attempted
void vksk_RuntimeImportExists(WrenVM *vm);