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
