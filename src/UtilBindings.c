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
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
    wrenGetVariable(vm, "lib/Util", "CHitbox", 0);
    VKSK_RuntimeForeign *hitbox = wrenSetSlotNewForeign(vm, 0, 0, sizeof(struct VKSK_RuntimeForeign));
    hitbox->type = FOREIGN_HITBOX;
    hitbox->hitbox.type = HITBOX_TYPE_CIRCLE;
    hitbox->hitbox.circle.r = wrenGetSlotDouble(vm, 1);
    hitbox->hitbox.bb_left = -hitbox->hitbox.circle.r;
    hitbox->hitbox.bb_top = -hitbox->hitbox.circle.r;
    hitbox->hitbox.bb_right = hitbox->hitbox.circle.r;
    hitbox->hitbox.bb_bottom = hitbox->hitbox.circle.r;
    hitbox->hitbox.xOffset = 0;
    hitbox->hitbox.yOffset = 0;
}

void vksk_RuntimeUtilHitboxNewRectangle(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    wrenGetVariable(vm, "lib/Util", "CHitbox", 0);
    VKSK_RuntimeForeign *hitbox = wrenSetSlotNewForeign(vm, 0, 0, sizeof(struct VKSK_RuntimeForeign));
    hitbox->type = FOREIGN_HITBOX;
    hitbox->hitbox.type = HITBOX_TYPE_RECTANGLE;
    hitbox->hitbox.rectangle.w = wrenGetSlotDouble(vm, 1);
    hitbox->hitbox.rectangle.h = wrenGetSlotDouble(vm, 2);
    hitbox->hitbox.bb_left = 0;
    hitbox->hitbox.bb_top = 0;
    hitbox->hitbox.bb_right = hitbox->hitbox.rectangle.w;
    hitbox->hitbox.bb_bottom = hitbox->hitbox.rectangle.h;
    hitbox->hitbox.xOffset = 0;
    hitbox->hitbox.yOffset = 0;
}

void vksk_RuntimeUtilHitboxNewPolygon(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_LIST, FOREIGN_END)
    wrenGetVariable(vm, "lib/Util", "CHitbox", 0);
    VKSK_RuntimeForeign *hitbox = wrenSetSlotNewForeign(vm, 0, 0, sizeof(struct VKSK_RuntimeForeign));
    hitbox->type = FOREIGN_HITBOX;
    hitbox->hitbox.type = HITBOX_TYPE_POLYGON;
    hitbox->hitbox.polygon.count = wrenGetListCount(vm, 1);
    hitbox->hitbox.polygon.vertices = malloc(sizeof(vec2) * hitbox->hitbox.polygon.count);
    if (hitbox->hitbox.polygon.count >= 3 && hitbox->hitbox.polygon.vertices != NULL) {
        wrenEnsureSlots(vm, 4);
        const int vertexPosSlot = 2;
        const int vertexPosElementSlot = 3;
        double minX = 100000000;
        double minY = 100000000;
        double maxX = -100000000;
        double maxY = -100000000;
        for (int i = 0; i < hitbox->hitbox.polygon.count; i++) {
            wrenGetListElement(vm, 1, i, vertexPosSlot);
            wrenGetListElement(vm, vertexPosSlot, 0, vertexPosElementSlot);
            hitbox->hitbox.polygon.vertices[i][0] = wrenGetSlotDouble(vm, vertexPosElementSlot);
            wrenGetListElement(vm, vertexPosSlot, 1, vertexPosElementSlot);
            hitbox->hitbox.polygon.vertices[i][1] = wrenGetSlotDouble(vm, vertexPosElementSlot);

            if (hitbox->hitbox.polygon.vertices[i][0] < minX)
                minX = hitbox->hitbox.polygon.vertices[i][0];
            if (hitbox->hitbox.polygon.vertices[i][1] < minY)
                minY = hitbox->hitbox.polygon.vertices[i][1];
            if (hitbox->hitbox.polygon.vertices[i][0] > maxX)
                maxX = hitbox->hitbox.polygon.vertices[i][0];
            if (hitbox->hitbox.polygon.vertices[i][1] > maxX)
                maxY = hitbox->hitbox.polygon.vertices[i][1];
        }

        hitbox->hitbox.bb_left = minX;
        hitbox->hitbox.bb_top = minY;
        hitbox->hitbox.bb_right = maxX;
        hitbox->hitbox.bb_bottom = maxY;
        hitbox->hitbox.xOffset = 0;
        hitbox->hitbox.yOffset = 0;
    } else {
        if (hitbox->hitbox.polygon.vertices == NULL)
            vksk_Error(true, "Out of memory creating a hitbox with %i vertices.", hitbox->hitbox.polygon.count);
        if (hitbox->hitbox.polygon.count < 3)
            vksk_Error(true, "Cannot create a polygon hitbox with %i vertices.", hitbox->hitbox.polygon.count);
    }
}

void vksk_RuntimeUtilHitboxR(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    VKSK_RuntimeForeign *hitbox = wrenGetSlotForeign(vm, 0);
    if (hitbox->hitbox.type == HITBOX_TYPE_CIRCLE) {
        wrenSetSlotDouble(vm, 0, hitbox->hitbox.circle.r);
    } else {
        wrenSetSlotNull(vm, 0);
    }
}

void vksk_RuntimeUtilHitboxW(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    VKSK_RuntimeForeign *hitbox = wrenGetSlotForeign(vm, 0);
    if (hitbox->hitbox.type == HITBOX_TYPE_RECTANGLE) {
        wrenSetSlotDouble(vm, 0, hitbox->hitbox.rectangle.w);
    } else {
        wrenSetSlotNull(vm, 0);
    }
}

void vksk_RuntimeUtilHitboxH(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    VKSK_RuntimeForeign *hitbox = wrenGetSlotForeign(vm, 0);
    if (hitbox->hitbox.type == HITBOX_TYPE_RECTANGLE) {
        wrenSetSlotDouble(vm, 0, hitbox->hitbox.rectangle.h);
    } else {
        wrenSetSlotNull(vm, 0);
    }
}

void vksk_RuntimeUtilHitboxXOffsetSetter(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
    VKSK_RuntimeForeign *hitbox = wrenGetSlotForeign(vm, 0);
    hitbox->hitbox.xOffset = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeUtilHitboxYOffsetSetter(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_END)
    VKSK_RuntimeForeign *hitbox = wrenGetSlotForeign(vm, 0);
    hitbox->hitbox.yOffset = wrenGetSlotDouble(vm, 1);
}

void vksk_RuntimeUtilHitboxXOffsetGetter(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    VKSK_RuntimeForeign *hitbox = wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, hitbox->hitbox.xOffset);
}

void vksk_RuntimeUtilHitboxYOffsetGetter(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    VKSK_RuntimeForeign *hitbox = wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, hitbox->hitbox.yOffset);
}

void vksk_RuntimeUtilHitboxCollision(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    VKSK_RuntimeForeign *hitbox = wrenGetSlotForeign(vm, 0);
    // TODO: This
}

void vksk_RuntimeUtilHitboxBoundingBox(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    VKSK_RuntimeForeign *hitbox = wrenGetSlotForeign(vm, 0);
    const double x = wrenGetSlotDouble(vm, 1) - hitbox->hitbox.xOffset;
    const double y = wrenGetSlotDouble(vm, 2) - hitbox->hitbox.yOffset;
    wrenEnsureSlots(vm, 2);
    const int listSlot = 0;
    const int elementSlot = 0;
    wrenSetSlotNewList(vm, 0);
    wrenSetSlotDouble(vm, elementSlot, hitbox->hitbox.bb_left + x);
    wrenSetListElement(vm, listSlot, -1, elementSlot);
    wrenSetSlotDouble(vm, elementSlot, hitbox->hitbox.bb_top + y);
    wrenSetListElement(vm, listSlot, -1, elementSlot);
    wrenSetSlotDouble(vm, elementSlot, hitbox->hitbox.bb_right + x);
    wrenSetListElement(vm, listSlot, -1, elementSlot);
    wrenSetSlotDouble(vm, elementSlot, hitbox->hitbox.bb_bottom + y);
    wrenSetListElement(vm, listSlot, -1, elementSlot);
}

void vksk_RuntimeUtilHitboxBbLeft(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    VKSK_RuntimeForeign *hitbox = wrenGetSlotForeign(vm, 0);
    const double x = wrenGetSlotDouble(vm, 1) - hitbox->hitbox.xOffset;
    wrenSetSlotDouble(vm, 0, hitbox->hitbox.bb_left + x);
}

void vksk_RuntimeUtilHitboxBbRight(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    VKSK_RuntimeForeign *hitbox = wrenGetSlotForeign(vm, 0);
    const double x = wrenGetSlotDouble(vm, 1) - hitbox->hitbox.xOffset;
    wrenSetSlotDouble(vm, 0, hitbox->hitbox.bb_right + x);
}

void vksk_RuntimeUtilHitboxBbTop(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    VKSK_RuntimeForeign *hitbox = wrenGetSlotForeign(vm, 0);
    const double y = wrenGetSlotDouble(vm, 2) - hitbox->hitbox.yOffset;
    wrenSetSlotDouble(vm, 0, hitbox->hitbox.bb_top + y);
}

void vksk_RuntimeUtilHitboxBbBottom(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_END)
    VKSK_RuntimeForeign *hitbox = wrenGetSlotForeign(vm, 0);
    const double y = wrenGetSlotDouble(vm, 2) - hitbox->hitbox.yOffset;
    wrenSetSlotDouble(vm, 0, hitbox->hitbox.bb_bottom + y);
}
