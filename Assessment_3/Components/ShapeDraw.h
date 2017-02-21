#pragma once

#include "Shapes.h"
#include "sfwdraw.h"

void drawCircle(const Circle &C, unsigned color = YELLOW);

void drawAABB(const Mat3 &T, const AABB &aabb, unsigned color = GREEN);

void drawBox(const Box &BOX, unsigned color = RED);

void drawPlane(const Plane &plane, unsigned color = GREEN);

void drawHull(const Hull &hull, unsigned color = GREEN);