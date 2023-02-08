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
void vksk_RuntimeJUSpriteFrom(WrenVM *vm);
void vksk_RuntimeJUSpriteFree(WrenVM *vm);
void vksk_RuntimeJUSpriteGetDelay(WrenVM *vm);
void vksk_RuntimeJUSpriteSetDelay(WrenVM *vm);
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
void vksk_RuntimeJUSpriteGetWidth(WrenVM *vm);
void vksk_RuntimeJUSpriteGetHeight(WrenVM *vm);

void vksk_RuntimeJUAudioDataAllocate(WrenVM *vm);
void vksk_RuntimeJUAudioDataFinalize(void *data);
void vksk_RuntimeJUAudioDataFree(WrenVM *vm);

void vksk_RuntimeJUAudioAllocate(WrenVM *vm);
void vksk_RuntimeJUAudioFinalize(void *data);
void vksk_RuntimeJUAudioUpdate(WrenVM *vm);
void vksk_RuntimeJUAudioStop(WrenVM *vm);
void vksk_RuntimeJUAudioPause(WrenVM *vm);
void vksk_RuntimeJUAudioUnpause(WrenVM *vm);
void vksk_RuntimeJUAudioStopAll(WrenVM *vm);