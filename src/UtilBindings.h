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

void vksk_RuntimeUtilHitboxAllocate(WrenVM *vm);
void vksk_RuntimeUtilHitboxFinalize(void *data);
void vksk_RuntimeUtilHitboxNewCircle(WrenVM *vm);
void vksk_RuntimeUtilHitboxNewRectangle(WrenVM *vm);
void vksk_RuntimeUtilHitboxNewPolygon(WrenVM *vm);
void vksk_RuntimeUtilHitboxR(WrenVM *vm);
void vksk_RuntimeUtilHitboxW(WrenVM *vm);
void vksk_RuntimeUtilHitboxH(WrenVM *vm);
void vksk_RuntimeUtilHitboxXOffsetSetter(WrenVM *vm);
void vksk_RuntimeUtilHitboxYOffsetSetter(WrenVM *vm);
void vksk_RuntimeUtilHitboxXOffsetGetter(WrenVM *vm);
void vksk_RuntimeUtilHitboxYOffsetGetter(WrenVM *vm);
void vksk_RuntimeUtilHitboxCollision(WrenVM *vm);
void vksk_RuntimeUtilHitboxBoundingBox(WrenVM *vm);
void vksk_RuntimeUtilHitboxBbLeft(WrenVM *vm);
void vksk_RuntimeUtilHitboxBbRight(WrenVM *vm);
void vksk_RuntimeUtilHitboxBbTop(WrenVM *vm);
void vksk_RuntimeUtilHitboxBbBottom(WrenVM *vm);