/// \file UtilBindings.c
/// \author Paolo Mazzon
#include "src/UtilBindings.h"
#include "src/Validation.h"
#include "src/IntermediateTypes.h"
#include <math.h>

void vksk_RuntimeUtilMathRoundTo(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    double x = wrenGetSlotDouble(vm, 1);
    double y = wrenGetSlotDouble(vm, 2);
    wrenSetSlotDouble(vm, 0, round(x / y) * y);
}

void vksk_RuntimeUtilMathFloorTo(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    double x = wrenGetSlotDouble(vm, 1);
    double y = wrenGetSlotDouble(vm, 2);
    wrenSetSlotDouble(vm, 0, floor(x / y) * y);
}

void vksk_RuntimeUtilMathCeilTo(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    double x = wrenGetSlotDouble(vm, 1);
    double y = wrenGetSlotDouble(vm, 2);
    wrenSetSlotDouble(vm, 0, ceil(x / y) * y);
}

void vksk_RuntimeUtilMathPointAngle(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    double x1 = wrenGetSlotDouble(vm, 1);
    double y1 = wrenGetSlotDouble(vm, 2);
    double x2 = wrenGetSlotDouble(vm, 3);
    double y2 = wrenGetSlotDouble(vm, 4);
    wrenSetSlotDouble(vm, 0, atan2(y2 - y1, x2 - x1));// - (VK2D_PI));
}

void vksk_RuntimeUtilMathPointDistance(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    double x1 = wrenGetSlotDouble(vm, 1);
    double y1 = wrenGetSlotDouble(vm, 2);
    double x2 = wrenGetSlotDouble(vm, 3);
    double y2 = wrenGetSlotDouble(vm, 4);
    wrenSetSlotDouble(vm, 0, sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2)));
}

void vksk_RuntimeUtilMathPointInRectangle(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    double x = wrenGetSlotDouble(vm, 1);
    double y = wrenGetSlotDouble(vm, 2);
    double w = wrenGetSlotDouble(vm, 3);
    double h = wrenGetSlotDouble(vm, 4);
    double px = wrenGetSlotDouble(vm, 5);
    double py = wrenGetSlotDouble(vm, 6);
    wrenSetSlotBool(vm, 0, px >= x && px <= x + w && py >= y && py <= y + h);
}

void vksk_RuntimeUtilMathPointInCircle(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    double x = wrenGetSlotDouble(vm, 1);
    double y = wrenGetSlotDouble(vm, 2);
    double r = wrenGetSlotDouble(vm, 3);
    double px = wrenGetSlotDouble(vm, 4);
    double py = wrenGetSlotDouble(vm, 5);
    wrenSetSlotBool(vm, 0, sqrt(pow(py - y, 2) + pow(px - x, 2)) < r);
}

void vksk_RuntimeUtilMathLerp(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    double percent = wrenGetSlotDouble(vm, 1);
    double start = wrenGetSlotDouble(vm, 2);
    double stop = wrenGetSlotDouble(vm, 3);
    wrenSetSlotDouble(vm, 0, (percent * (stop - start)) + start);
}

void vksk_RuntimeUtilMathSerp(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    double percent = wrenGetSlotDouble(vm, 1);
    double start = wrenGetSlotDouble(vm, 2);
    double stop = wrenGetSlotDouble(vm, 3);
    wrenSetSlotDouble(vm, 0, (((sin((VK2D_PI * percent) - (VK2D_PI / 2)) / 2) + 0.5) * (stop - start)) + start);
}

void vksk_RuntimeUtilMathCastX(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    double length = wrenGetSlotDouble(vm, 1);
    double angle = wrenGetSlotDouble(vm, 2);
    wrenSetSlotDouble(vm, 0, length * cos(angle));
}

void vksk_RuntimeUtilMathCastY(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    double length = wrenGetSlotDouble(vm, 1);
    double angle = wrenGetSlotDouble(vm, 2);
    wrenSetSlotDouble(vm, 0, length * sin(angle));
}

void vksk_RuntimeUtilMathClamp(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    double x = wrenGetSlotDouble(vm, 1);
    double start = wrenGetSlotDouble(vm, 2);
    double stop = wrenGetSlotDouble(vm, 3);
    wrenSetSlotDouble(vm, 0, x < start ? start : (x > stop ? stop : x));
}

void vksk_RuntimeUtilHitboxAllocate(WrenVM *vm) {
    wrenSetSlotNull(vm, 0);
}

void vksk_RuntimeUtilHitboxFinalize(void *data) {

}

void vksk_RuntimeUtilHitboxNewCircle(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    // TODO: This
}

void vksk_RuntimeUtilHitboxNewRectangle(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    // TODO: This
}

void vksk_RuntimeUtilHitboxNewPolygon(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    // TODO: This
}

void vksk_RuntimeUtilHitboxR(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    // TODO: This
}

void vksk_RuntimeUtilHitboxW(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    // TODO: This
}

void vksk_RuntimeUtilHitboxH(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    // TODO: This
}

void vksk_RuntimeUtilHitboxXOffsetSetter(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    // TODO: This
}

void vksk_RuntimeUtilHitboxYOffsetSetter(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    // TODO: This
}

void vksk_RuntimeUtilHitboxXOffsetGetter(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    // TODO: This
}

void vksk_RuntimeUtilHitboxYOffsetGetter(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    // TODO: This
}

void vksk_RuntimeUtilHitboxCollision(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    // TODO: This
}

void vksk_RuntimeUtilHitboxBoundingBox(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    // TODO: This
}

void vksk_RuntimeUtilHitboxBbLeft(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    // TODO: This
}

void vksk_RuntimeUtilHitboxBbRight(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    // TODO: This
}

void vksk_RuntimeUtilHitboxBbTop(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    // TODO: This
}

void vksk_RuntimeUtilHitboxBbBottom(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    // TODO: This
}
