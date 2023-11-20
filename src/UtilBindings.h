/// \file UtilBindings.h
/// \author Paolo Mazzon
/// \brief C side of the game util stuff for wren
#pragma once
#include "wren.h"

void vksk_RuntimeUtilMathRoundTo(WrenVM *vm);
void vksk_RuntimeUtilMathFloorTo(WrenVM *vm);
void vksk_RuntimeUtilMathCeilTo(WrenVM *vm);
void vksk_RuntimeUtilMathPointAngle(WrenVM *vm);
void vksk_RuntimeUtilMathPointDistance(WrenVM *vm);
void vksk_RuntimeUtilMathPointInRectangle(WrenVM *vm);
void vksk_RuntimeUtilMathPointInCircle(WrenVM *vm);
void vksk_RuntimeUtilMathLerp(WrenVM *vm);
void vksk_RuntimeUtilMathSerp(WrenVM *vm);
void vksk_RuntimeUtilMathCastX(WrenVM *vm);
void vksk_RuntimeUtilMathCastY(WrenVM *vm);
void vksk_RuntimeUtilMathClamp(WrenVM *vm);