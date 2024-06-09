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
void vksk_RuntimeVK2DCameraGetCentre(WrenVM *vm);
void vksk_RuntimeVK2DCameraSetCentre(WrenVM *vm);
void vksk_RuntimeVK2DCameraGetUp(WrenVM *vm);
void vksk_RuntimeVK2DCameraSetUp(WrenVM *vm);
void vksk_RuntimeVK2DCameraGetFov(WrenVM *vm);
void vksk_RuntimeVK2DCameraSetFov(WrenVM *vm);
void vksk_RuntimeVK2DCameraUpdate(WrenVM *vm);

void vksk_RuntimeVK2DShaderAllocate(WrenVM *vm);
void vksk_RuntimeVK2DShaderFinalize(void *data);
void vksk_RuntimeVK2DShaderSetData(WrenVM *vm);

void vksk_RuntimeVK2DPolygonAllocate(WrenVM *vm);
void vksk_RuntimeVK2DPolygonCreate(WrenVM *vm);
void vksk_RuntimeVK2DPolygonFinalize(void *data);
void vksk_RuntimeVK2DPolygonFree(WrenVM *vm);

void vksk_RuntimeVK2DModelAllocate(WrenVM *vm);
void vksk_RuntimeVK2DModelLoad(WrenVM *vm);
void vksk_RuntimeVK2DModelFinalize(void *data);
void vksk_RuntimeVK2DModelFree(WrenVM *vm);

void vksk_RuntimeLightingAddShadow(WrenVM *vm);
void vksk_RuntimeLightingAddLight(WrenVM *vm);
void vksk_RuntimeLightingReset(WrenVM *vm);
void vksk_RuntimeLightingFlushVBO(WrenVM *vm);

void vksk_RuntimeLightSourcePositionSet(WrenVM *vm);
void vksk_RuntimeLightSourcePositionGet(WrenVM *vm);
void vksk_RuntimeLightSourceXGet(WrenVM *vm);
void vksk_RuntimeLightSourceXSet(WrenVM *vm);
void vksk_RuntimeLightSourceYGet(WrenVM *vm);
void vksk_RuntimeLightSourceYSet(WrenVM *vm);
void vksk_RuntimeLightSourceRotationSet(WrenVM *vm);
void vksk_RuntimeLightSourceRotationGet(WrenVM *vm);
void vksk_RuntimeLightSourceOriginXSet(WrenVM *vm);
void vksk_RuntimeLightSourceOriginXGet(WrenVM *vm);
void vksk_RuntimeLightSourceOriginYSet(WrenVM *vm);
void vksk_RuntimeLightSourceOriginYGet(WrenVM *vm);
void vksk_RuntimeLightSourceTextureSet(WrenVM *vm);
void vksk_RuntimeLightSourceDelete(WrenVM *vm);

void vksk_RuntimeShadowPositionSet(WrenVM *vm);
void vksk_RuntimeShadowUpdate(WrenVM *vm);
void vksk_RuntimeShadowEnabledSet(WrenVM *vm);
void vksk_RuntimeShadowEnabledGet(WrenVM *vm);
