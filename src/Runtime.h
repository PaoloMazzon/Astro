/// \file Runtime.h
/// \author Paolo Mazzon
/// \brief The top-level runtime and meta functions that are binded to the VM but dont exist in VK2D/JamUtil
#pragma once
#include <wren.h>

#define ASTRO_VERSION_MAJOR 0
#define ASTRO_VERSION_MINOR 6
#define ASTRO_VERSION_PATCH 1

// Engine configuration
typedef struct VKSK_EngineConfig {
	bool enableTypeChecking;
} VKSK_EngineConfig;

// Starts the runtime
void vksk_Start();

// Searches through the assets folder and makes a wren source file loading it all
const char *vksk_CompileAssetFile();

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