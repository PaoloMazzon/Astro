/// \file JUTypes.h
/// \author Paolo Mazzon
/// \brief JamUtil types
#pragma once
#include <wren.h>

void vksk_RuntimeJUBitmapFontAllocate(WrenVM *vm);
void vksk_RuntimeJUBitmapFontFinalize(void *data);
void vksk_RuntimeJUBitmapFontFree(WrenVM *vm);