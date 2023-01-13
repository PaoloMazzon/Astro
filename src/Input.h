/// \file Input.h
/// \author Paolo Mazzon
/// \brief Bindings for JamUtil input
#pragma once
#include <wren.h>

void vksk_RuntimeInputCheckKey(WrenVM *vm);
void vksk_RuntimeInputCheckKeyPressed(WrenVM *vm);
void vksk_RuntimeInputCheckKeyReleased(WrenVM *vm);
void vksk_RuntimeInputGetMousePosition(WrenVM *vm);
void vksk_RuntimeInputGetMouseLeftButton(WrenVM *vm);
void vksk_RuntimeInputGetMouseLeftButtonPressed(WrenVM *vm);
void vksk_RuntimeInputGetMouseLeftButtonReleased(WrenVM *vm);
void vksk_RuntimeInputGetMouseMiddleButton(WrenVM *vm);
void vksk_RuntimeInputGetMouseMiddleButtonPressed(WrenVM *vm);
void vksk_RuntimeInputGetMouseMiddleButtonReleased(WrenVM *vm);
void vksk_RuntimeInputGetMouseRightButton(WrenVM *vm);
void vksk_RuntimeInputGetMouseRightButtonPressed(WrenVM *vm);
void vksk_RuntimeInputGetMouseRightButtonReleased(WrenVM *vm);