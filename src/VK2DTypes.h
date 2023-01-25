/// \file VK2DTypes.h
/// \author Paolo Mazzon
/// \brief Bindings for foreign classes in VK2D
#pragma once
#include <wren.h>

// For VK2D textures
void vksk_RuntimeVK2DTextureAllocate(WrenVM *);
void vksk_RuntimeVK2DTextureFinalize(void *data);
void vksk_RuntimeVK2DTextureFree(WrenVM *vm);
void vksk_RuntimeVK2DTextureWidth(WrenVM *vm);
void vksk_RuntimeVK2DTextureHeight(WrenVM *vm);

// For VK2D textures as render targets
void vksk_RuntimeVK2DSurfaceAllocate(WrenVM *);
void vksk_RuntimeVK2DSurfaceFinalize(void *data);
void vksk_RuntimeVK2DSurfaceFree(WrenVM *vm);
void vksk_RuntimeVK2DSurfaceWidth(WrenVM *vm);
void vksk_RuntimeVK2DSurfaceHeight(WrenVM *vm);

// For VK2D cameras kinda
void vksk_RuntimeVK2DCameraAllocate(WrenVM *vm);
void vksk_RuntimeVK2DCameraFinalize(void *data);
void vksk_RuntimeVK2DCameraGetType(WrenVM *vm);
void vksk_RuntimeVK2DCameraSetType(WrenVM *vm);
void vksk_RuntimeVK2DCameraGetX(WrenVM *vm);
void vksk_RuntimeVK2DCameraSetX(WrenVM *vm);
void vksk_RuntimeVK2DCameraGetY(WrenVM *vm);
void vksk_RuntimeVK2DCameraSetY(WrenVM *vm);
void vksk_RuntimeVK2DCameraGetW(WrenVM *vm);
void vksk_RuntimeVK2DCameraSetW(WrenVM *vm);
void vksk_RuntimeVK2DCameraGetH(WrenVM *vm);
void vksk_RuntimeVK2DCameraSetH(WrenVM *vm);
void vksk_RuntimeVK2DCameraGetZoom(WrenVM *vm);
void vksk_RuntimeVK2DCameraSetZoom(WrenVM *vm);
void vksk_RuntimeVK2DCameraGetRotation(WrenVM *vm);
void vksk_RuntimeVK2DCameraSetRotation(WrenVM *vm);
void vksk_RuntimeVK2DCameraGetXOnScreen(WrenVM *vm);
void vksk_RuntimeVK2DCameraSetXOnScreen(WrenVM *vm);
void vksk_RuntimeVK2DCameraGetYOnScreen(WrenVM *vm);
void vksk_RuntimeVK2DCameraSetYOnScreen(WrenVM *vm);
void vksk_RuntimeVK2DCameraGetWOnScreen(WrenVM *vm);
void vksk_RuntimeVK2DCameraSetWOnScreen(WrenVM *vm);
void vksk_RuntimeVK2DCameraGetHOnScreen(WrenVM *vm);
void vksk_RuntimeVK2DCameraSetHOnScreen(WrenVM *vm);
void vksk_RuntimeVK2DCameraGetEyes(WrenVM *vm);
void vksk_RuntimeVK2DCameraSetEyes(WrenVM *vm);
void vksk_RuntimeVK2DCameraUpdate(WrenVM *vm);