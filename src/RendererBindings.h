/// \file RendererBindings.h
/// \author Paolo Mazzon
/// \brief Bindings for the renderer and sdl stuff
#pragma once
#include "wren.h"

void vksk_RuntimeRendererInit(WrenVM *vm);

void vksk_RuntimeRendererUpdate(WrenVM *vm);

void vksk_RendererCleanup();