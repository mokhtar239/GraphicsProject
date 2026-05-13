#pragma once
#include <windows.h>

enum ShapeType {
    SHAPE_LINE_DDA,
    SHAPE_LINE_MIDPOINT,
    SHAPE_LINE_PARAMETRIC
};

struct Shape {
    ShapeType type;
    int x1, y1;
    int x2, y2;
    COLORREF color;
};

void DrawShape(HDC hdc, const Shape& shape);
