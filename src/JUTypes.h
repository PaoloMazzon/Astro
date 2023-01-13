/// \file JUTypes.h
/// \author Paolo Mazzon
/// \brief JamUtil types
#pragma once
#include <wren.h>

void vksk_RuntimeJUBitmapFontAllocate(WrenVM *vm);
void vksk_RuntimeJUBitmapFontFinalize(void *data);
void vksk_RuntimeJUBitmapFontFree(WrenVM *vm);
void vksk_RuntimeJUSpriteAllocate(WrenVM *vm);
void vksk_RuntimeJUSpriteFinalize(void *data);
void vksk_RuntimeJUSpriteCopy(WrenVM *vm);
void vksk_RuntimeJUSpriteFree(WrenVM *vm);
void vksk_RuntimeJUSpriteSetPos(WrenVM *vm);
void vksk_RuntimeJUSpriteGetDelay(WrenVM *vm);
void vksk_RuntimeJUSpriteSetDelay(WrenVM *vm);
void vksk_RuntimeJUSpriteGetX(WrenVM *vm);
void vksk_RuntimeJUSpriteGetY(WrenVM *vm);
void vksk_RuntimeJUSpriteGetOriginX(WrenVM *vm);
void vksk_RuntimeJUSpriteSetOriginX(WrenVM *vm);
void vksk_RuntimeJUSpriteGetOriginY(WrenVM *vm);
void vksk_RuntimeJUSpriteSetOriginY(WrenVM *vm);
void vksk_RuntimeJUSpriteGetScaleX(WrenVM *vm);
void vksk_RuntimeJUSpriteSetScaleX(WrenVM *vm);
void vksk_RuntimeJUSpriteGetScaleY(WrenVM *vm);
void vksk_RuntimeJUSpriteSetScaleY(WrenVM *vm);
void vksk_RuntimeJUSpriteGetRotation(WrenVM *vm);
void vksk_RuntimeJUSpriteSetRotation(WrenVM *vm);