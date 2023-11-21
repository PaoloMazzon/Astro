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

void vksk_RuntimeUtilHitboxNewVoid(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_END)
    wrenGetVariable(vm, "lib/Util", "CHitbox", 0);
    VKSK_RuntimeForeign *hitbox = wrenSetSlotNewForeign(vm, 0, 0, sizeof(struct VKSK_RuntimeForeign));
    hitbox->type = FOREIGN_HITBOX;
    hitbox->hitbox.type = HITBOX_TYPE_VOID;
    hitbox->hitbox.bb_left = 0;
    hitbox->hitbox.bb_top = 0;
    hitbox->hitbox.bb_right = 0;
    hitbox->hitbox.bb_bottom = 0;
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

// Casts a 2D point onto a 1D line given by the slope m
static double cast1dShadow(double x, double y, double m) {
    return y - (m * x);
}

// Returns true if two ranges on a number line overlap
static bool segmentOverlap(double min1, double max1, double min2, double max2) {
    return min1 < max2 && max1 > min2;
}

// Finds the min/max value of a vertex from a polygon hitbox cast to a 1D line given by m
static void satGetPolyMinMax(double x, double y, double m, double *min, double *max, _vksk_RuntimeHitbox *hitbox) {
    *min = cast1dShadow(x + hitbox->polygon.vertices[0][0], y + hitbox->polygon.vertices[0][1], m);
    *max = cast1dShadow(x + hitbox->polygon.vertices[0][0], y + hitbox->polygon.vertices[0][1], m);
    for (int i = 1; i < hitbox->polygon.count; i++) {
        const double cast = cast1dShadow(x + hitbox->polygon.vertices[i][0], y + hitbox->polygon.vertices[i][1], m);
        if (cast < *min)
            *min = cast;
        if (cast > *max)
            *max = cast;
    }
}

// workaround for dividing by 0
static inline double nodivzero(double x) {
    if (x == 0)
        return 0.1;
    return x;
}

// Checks for a sat collision only looking at the lines from the first hitbox
static bool satCollision1Way(double x1, double y1, double x2, double y2, _vksk_RuntimeHitbox *hitbox1, _vksk_RuntimeHitbox *hitbox2) {
    // Iterate through each line segment
    for (int i = 0; i < hitbox1->polygon.count; i++) {
        double m;
        if (i == 0)
            m = (hitbox1->polygon.vertices[0][1] - hitbox1->polygon.vertices[hitbox1->polygon.count - 1][1]) / nodivzero(hitbox1->polygon.vertices[0][0] - hitbox1->polygon.vertices[hitbox1->polygon.count - 1][0]);
        else
            m = (hitbox1->polygon.vertices[i][1] - hitbox1->polygon.vertices[i - 1][1]) / nodivzero(hitbox1->polygon.vertices[i][0] - hitbox1->polygon.vertices[i - 1][0]);

        // Find maxes
        double min1, min2, max1, max2;
        satGetPolyMinMax(x1, y1, m, &min1, &max1, hitbox1);
        satGetPolyMinMax(x2, y2, m, &min2, &max2, hitbox2);

        if (!segmentOverlap(min1, max1, min2, max2))
            return false;
    }
    return true;
}

static bool satCollision(double x1, double y1, double x2, double y2, _vksk_RuntimeHitbox *hitbox1, _vksk_RuntimeHitbox *hitbox2) {
    bool aabb = (x1 + hitbox1->bb_right) > (x2 + hitbox2->bb_left) && (x1 + hitbox1->bb_left) < (x2 + hitbox2->bb_right) &&
                (y1 + hitbox1->bb_bottom) > (y2 + hitbox2->bb_top) && (y1 + hitbox1->bb_top) < (y2 + hitbox2->bb_bottom);
    return aabb && satCollision1Way(x1, y1, x2, y2, hitbox1, hitbox2) && satCollision1Way(x2, y2, x1, y1, hitbox2, hitbox1);
}

// Checks if a point is inside a convex polygon
static bool pointInConvexPolygon(double x, double y, double polyX, double polyY, _vksk_RuntimeHitbox *poly) {
    x -= polyX; // converts x/y into polygon space
    y -= polyY;
    int i, j;
    bool c = false;
    for (i = 0, j = poly->polygon.count - 1; i < poly->polygon.count; j = i++) {
        if ( ((poly->polygon.vertices[i][1] > y) != (poly->polygon.vertices[j][1] > y)) &&
             (x < (poly->polygon.vertices[j][0] - poly->polygon.vertices[i][0]) * (y - poly->polygon.vertices[i][1]) / (poly->polygon.vertices[j][1] - poly->polygon.vertices[i][1]) + poly->polygon.vertices[i][0]) )
            c = !c;
    }
    return c;
}

// Checks if a line intersects a circle given the circles center and radius and two points on the line
static bool lineIntersectsCircle(double cx, double cy, double r, double x1, double y1, double x2, double y2) {
    double a = y1 - y2;
    double b = x2 - x1;
    double c = ((x1 - x2) * y1) + ((y2 - y1) * x1);
    double d = fabs((a * cx) + (b * cy) + c) / sqrt(pow(a, 2) + pow(b, 2));
    return d < r;
}

// Checks if a point is between a line given endpoints
static bool pointBetweenLine(double px, double py, double x1, double y1, double x2, double y2) {
    double m = -(x2 - x1) / nodivzero(y2 - y1);
    double a = cast1dShadow(x1, y1, m);
    double b = cast1dShadow(x2, y2, m);
    double c = cast1dShadow(px, py, m);
    if (b > a)
        return c > a && c < b;
    else
        return c > b && c < a;
}

static bool satCircle(double px, double py, double cx, double cy, _vksk_RuntimeHitbox *poly, double radius) {
    if (pointInConvexPolygon(cx, cy, px, py, poly))
        return true;

    // Check each vertex to see if its touching the circle
    for (int i = 0; i < poly->polygon.count; i++)
        if (juPointDistance(px + poly->polygon.vertices[i][0], py + poly->polygon.vertices[i][1], cx, cy) < radius)
            return true;

    // Check every line segment for an intersection with the circle
    for (int i = 0; i < poly->polygon.count; i++) {
        double x1, y1, x2, y2;
        if (i == 0) {
            x1 = px + poly->polygon.vertices[0][0];
            y1 = py + poly->polygon.vertices[0][1];
            x2 = px + poly->polygon.vertices[poly->polygon.count - 1][0];
            y2 = py + poly->polygon.vertices[poly->polygon.count - 1][1];
        } else {
            x1 = px + poly->polygon.vertices[i][0];
            y1 = py + poly->polygon.vertices[i][1];
            x2 = px + poly->polygon.vertices[i - 1][0];
            y2 = py + poly->polygon.vertices[i - 1][1];
        }

        if (pointBetweenLine(cx, cy, x1, y1, x2, y2)) {
            // Circle is between this line segment
            if (lineIntersectsCircle(cx, cy, radius, x1, y1, x2, y2))
                return true;
        }
    }

    return false;
}

static bool aabb(r1x1, r1y1, r1x2, r1y2, r2x1, r2y1, r2x2, r2y2) {
    return r1x2 > r2x1 && r1x1 < r2x2 && r1y2 > r2y1 && r1y1 < r2y2;
}

void vksk_RuntimeUtilHitboxCollision(WrenVM *vm) {
    VALIDATE_FOREIGN_ARGS(vm, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_NUM, FOREIGN_HITBOX, FOREIGN_END)
    VKSK_RuntimeForeign *hitbox1 = wrenGetSlotForeign(vm, 0);
    VKSK_RuntimeForeign *hitbox2 = wrenGetSlotForeign(vm, 5);
    double x1 = wrenGetSlotDouble(vm, 1) - hitbox1->hitbox.xOffset;
    double y1 = wrenGetSlotDouble(vm, 2) - hitbox1->hitbox.yOffset;
    double x2 = wrenGetSlotDouble(vm, 3) - hitbox2->hitbox.xOffset;
    double y2 = wrenGetSlotDouble(vm, 4) - hitbox2->hitbox.yOffset;
    if (hitbox1->hitbox.type == HITBOX_TYPE_VOID || hitbox2->hitbox.type == HITBOX_TYPE_VOID) {
        wrenSetSlotBool(vm, 0, false);
    } else if (hitbox1->hitbox.type == HITBOX_TYPE_RECTANGLE && hitbox2->hitbox.type == HITBOX_TYPE_RECTANGLE) {
        wrenSetSlotBool(vm, 0, aabb(x1, y1, x1 + hitbox1->hitbox.rectangle.w, y1 + hitbox1->hitbox.rectangle.h, x2, y2, x2 + hitbox2->hitbox.rectangle.w, y2 + hitbox2->hitbox.rectangle.h));
    } // TODO: This
}