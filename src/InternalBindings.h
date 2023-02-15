/// \file InternalBindings.h
/// \author Paolo Mazzon
/// \brief Bindings for internally implemented things
#pragma once
#include <wren.h>

void vksk_RuntimeINIAllocate(WrenVM *vm);
void vksk_RuntimeINIFinalize(void *data);
void vksk_RuntimeINIFlush(WrenVM *vm);
void vksk_RuntimeINIKeyExists(WrenVM *vm);
void vksk_RuntimeINIGetString(WrenVM *vm);
void vksk_RuntimeINISetString(WrenVM *vm);
void vksk_RuntimeINIGetNum(WrenVM *vm);
void vksk_RuntimeINISetNum(WrenVM *vm);
void vksk_RuntimeINIGetBool(WrenVM *vm);
void vksk_RuntimeINISetBool(WrenVM *vm);

void vksk_RuntimeFileRead(WrenVM *vm);
void vksk_RuntimeFileWrite(WrenVM *vm);
void vksk_RuntimeFileExists(WrenVM *vm);

void vksk_RuntimeTiledAllocate(WrenVM *vm);
void vksk_RuntimeTiledFinalize(void *data);
void vksk_RuntimeTiledWidth(WrenVM *vm);
void vksk_RuntimeTiledHeight(WrenVM *vm);
void vksk_RuntimeTiledCellWidth(WrenVM *vm);
void vksk_RuntimeTiledCellHeight(WrenVM *vm);
void vksk_RuntimeTiledLayerName(WrenVM *vm);
void vksk_RuntimeTiledNextLayer(WrenVM *vm);
void vksk_RuntimeTiledGetObjects(WrenVM *vm);
void vksk_RuntimeTiledGetTiles(WrenVM *vm);
void vksk_RuntimeTiledGetTilesets(WrenVM *vm);

void vksk_RuntimeBufferAllocate(WrenVM *vm);
void vksk_RuntimeBufferFinalize(void *data);
void vksk_RuntimeBufferOpen(WrenVM *vm);
void vksk_RuntimeBufferResize(WrenVM *vm);
void vksk_RuntimeBufferSize(WrenVM *vm);
void vksk_RuntimeBufferSetPointer(WrenVM *vm);
void vksk_RuntimeBufferGetPointer(WrenVM *vm);
void vksk_RuntimeBufferReadDouble(WrenVM *vm);
void vksk_RuntimeBufferWriteDouble(WrenVM *vm);
void vksk_RuntimeBufferReadFloat(WrenVM *vm);
void vksk_RuntimeBufferWriteFloat(WrenVM *vm);
void vksk_RuntimeBufferReadUint64(WrenVM *vm);
void vksk_RuntimeBufferWriteUint64(WrenVM *vm);
void vksk_RuntimeBufferReadUint32(WrenVM *vm);
void vksk_RuntimeBufferWriteUint32(WrenVM *vm);
void vksk_RuntimeBufferReadUint16(WrenVM *vm);
void vksk_RuntimeBufferWriteUint16(WrenVM *vm);
void vksk_RuntimeBufferReadUint8(WrenVM *vm);
void vksk_RuntimeBufferWriteUint8(WrenVM *vm);
void vksk_RuntimeBufferReadInt64(WrenVM *vm);
void vksk_RuntimeBufferWriteInt64(WrenVM *vm);
void vksk_RuntimeBufferReadInt32(WrenVM *vm);
void vksk_RuntimeBufferWriteInt32(WrenVM *vm);
void vksk_RuntimeBufferReadInt16(WrenVM *vm);
void vksk_RuntimeBufferWriteInt16(WrenVM *vm);
void vksk_RuntimeBufferReadInt8(WrenVM *vm);
void vksk_RuntimeBufferWriteInt8(WrenVM *vm);
void vksk_RuntimeBufferReadString(WrenVM *vm);
void vksk_RuntimeBufferWriteString(WrenVM *vm);
void vksk_RuntimeBufferReadBool(WrenVM *vm);
void vksk_RuntimeBufferWriteBool(WrenVM *vm);


void vksk_RuntimeControllerStickDeadzone(WrenVM *vm);
void vksk_RuntimeControllerStickDeadzoneSet(WrenVM *vm);
void vksk_RuntimeControllerTriggerDeadzone(WrenVM *vm);
void vksk_RuntimeControllerTriggerDeadzoneSet(WrenVM *vm);
void vksk_RuntimeControllerConnectedCount(WrenVM *vm);
void vksk_RuntimeControllerButtonPressed(WrenVM *vm);
void vksk_RuntimeControllerButtonReleased(WrenVM *vm);
void vksk_RuntimeControllerButton(WrenVM *vm);
void vksk_RuntimeControllerLeftStickX(WrenVM *vm);
void vksk_RuntimeControllerLeftStickY(WrenVM *vm);
void vksk_RuntimeControllerRightStickX(WrenVM *vm);
void vksk_RuntimeControllerRightStickY(WrenVM *vm);
void vksk_RuntimeControllerLeftTrigger(WrenVM *vm);
void vksk_RuntimeControllerRightTrigger(WrenVM *vm);
void vksk_RuntimeControllerRumble(WrenVM *vm);
void vksk_RuntimeControllerIsConnected(WrenVM *vm);
void vksk_RuntimeControllerName(WrenVM *vm);

void vksk_RuntimeFontAllocate(WrenVM *vm);
void vksk_RuntimeFontFinalize(void *data);
void vksk_RuntimeFontSize(WrenVM *vm);
void vksk_RuntimeFontFree(WrenVM *vm);