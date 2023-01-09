/// \file VK2DTypes.h
/// \author Paolo Mazzon
/// \brief Bindings for foreign classes in VK2D
#pragma once
#include <wren.h>

// For VK2D textures
void vksk_RuntimeVK2DTextureAllocate(WrenVM *);
void vksk_RuntimeVK2DTextureFinalize(void *data);
void vksk_RuntimeVK2DTextureWidth(WrenVM *vm);
void vksk_RuntimeVK2DTextureHeight(WrenVM *vm);

// For VK2D textures as render targets
void vksk_RuntimeVK2DSurfaceAllocate(WrenVM *);
void vksk_RuntimeVK2DSurfaceFinalize(void *data);
void vksk_RuntimeVK2DSurfaceWidth(WrenVM *vm);
void vksk_RuntimeVK2DSurfaceHeight(WrenVM *vm);