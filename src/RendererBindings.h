/// \file RendererBindings.h
/// \author Paolo Mazzon
/// \brief Bindings for the renderer and sdl stuff
#pragma once
#include "wren.h"

void vksk_RuntimeRendererDrawCircle(WrenVM *vm);

void vksk_RuntimeRendererDrawTextureExt(WrenVM *vm);

void vksk_RuntimeRendererDrawTexture(WrenVM *vm);

void vksk_RuntimeRendererDrawTexturePartExt(WrenVM *vm);

void vksk_RuntimeRendererDrawTexturePart(WrenVM *vm);