/// \file Input.h
/// \author Paolo Mazzon
/// \brief Bindings for JamUtil input
#pragma once
#include <wren.h>

void vksk_RuntimeInputCheckKey(WrenVM *vm);
void vksk_RuntimeInputCheckKeyPressed(WrenVM *vm);
void vksk_RuntimeInputCheckKeyReleased(WrenVM *vm);