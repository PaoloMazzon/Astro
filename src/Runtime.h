/// \file Runtime.h
/// \author Paolo Mazzon
/// \brief The top-level runtime and meta functions that are binded to the VM but dont exist in VK2D/JamUtil
#pragma once
#include <wren.h>

// Starts the runtime
void vksk_Start();

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