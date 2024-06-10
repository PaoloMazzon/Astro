/// \file RendererBindings.h
/// \author Paolo Mazzon
/// \brief Bindings for the renderer and sdl stuff
#pragma once
#include "wren.h"

void _vksk_RendererBindingsInit(void *textureData, int size);
void _vksk_RendererBindingsQuit(WrenVM *vm);
void vksk_RuntimeRendererGetWindowWidth(WrenVM *vm);
void vksk_RuntimeRendererGetWindowHeight(WrenVM *vm);
void vksk_RuntimeRendererSetWindowSize(WrenVM *vm);
void vksk_RuntimeRendererGetWindowFullscreen(WrenVM *vm);
void vksk_RuntimeRendererSetWindowFullscreen(WrenVM *vm);
void vksk_RuntimeRendererDrawCircle(WrenVM *vm);
void vksk_RuntimeRendererDrawTextureExt(WrenVM *vm);
void vksk_RuntimeRendererDrawTexture(WrenVM *vm);
void vksk_RuntimeRendererDrawTexturePartExt(WrenVM *vm);
void vksk_RuntimeRendererDrawTexturePart(WrenVM *vm);
void vksk_RuntimeRendererGetConfig(WrenVM *vm);
void vksk_RuntimeRendererSetConfig(WrenVM *vm);
void vksk_RuntimeRendererSetTarget(WrenVM *vm);
void vksk_RuntimeRendererSetBlendMode(WrenVM *vm);
void vksk_RuntimeRendererGetBlendMode(WrenVM *vm);
void vksk_RuntimeRendererSetColourMod(WrenVM *vm);
void vksk_RuntimeRendererGetColourMod(WrenVM *vm);
void vksk_RuntimeRendererSetShader(WrenVM *vm);
void vksk_RuntimeRendererSetTextureCamera(WrenVM *vm);
void vksk_RuntimeRendererGetAverageFrameTime(WrenVM *vm);
void vksk_RuntimeRendererLockCameras(WrenVM *vm);
void vksk_RuntimeRendererUnlockCameras(WrenVM *vm);
void vksk_RuntimeRendererSetViewport(WrenVM *vm);
void vksk_RuntimeRendererGetViewport(WrenVM *vm);
void vksk_RuntimeRendererClear(WrenVM *vm);
void vksk_RuntimeRendererClearBlank(WrenVM *vm);
void vksk_RuntimeRendererDrawRectangle(WrenVM *vm);
void vksk_RuntimeRendererDrawRectangleOutline(WrenVM *vm);
void vksk_RuntimeRendererDrawCircleOutline(WrenVM *vm);
void vksk_RuntimeRendererDrawLine(WrenVM *vm);
void vksk_RuntimeRendererDrawFont(WrenVM *vm);
void vksk_RuntimeRendererDrawFontWrapped(WrenVM *vm);
void vksk_RuntimeRendererDrawFontExt(WrenVM *vm);
void vksk_RuntimeRendererDrawFontExtWrapped(WrenVM *vm);
void vksk_RuntimeRendererDrawSpritePos(WrenVM *vm);
void vksk_RuntimeRendererDrawSpriteFrame(WrenVM *vm);
void vksk_RuntimeRendererDrawModel(WrenVM *vm);
void vksk_RuntimeRendererDrawModelExt(WrenVM *vm);
void vksk_RuntimeRendererDrawPolygon(WrenVM *vm);
void vksk_RuntimeRendererDrawPolygonExt(WrenVM *vm);
void vksk_RuntimeRendererSetupLighting(WrenVM *vm);
void vksk_RuntimeRendererDrawLighting(WrenVM *vm);
void vksk_RuntimeRendererDrawFOV(WrenVM *vm);