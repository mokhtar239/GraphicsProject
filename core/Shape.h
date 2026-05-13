#pragma once
#include <windows.h>
#include <vector>

union OutCode {
    unsigned int all;
    struct {
        unsigned int left : 1;
        unsigned int right : 1;
        unsigned int bottom : 1;
        unsigned int top : 1;
    };
};

enum ShapeType {
    SHAPE_LINE_DDA, SHAPE_LINE_MIDPOINT, SHAPE_LINE_PARAMETRIC,
    SHAPE_CIRCLE_DIRECT, SHAPE_CIRCLE_POLAR, SHAPE_CIRCLE_ITERATIVE_POLAR, SHAPE_CIRCLE_MIDPOINT, SHAPE_CIRCLE_MODIFIED_MIDPOINT,
    SHAPE_ELLIPSE_DIRECT, SHAPE_ELLIPSE_POLAR, SHAPE_ELLIPSE_MIDPOINT,
    SHAPE_CURVE_BEZIER, SHAPE_CURVE_HERMITE, SHAPE_CURVE_CARDINAL,
    
    SHAPE_CLIP_POINT,
    SHAPE_CLIP_LINE,
    SHAPE_CLIP_POLY,

    SHAPE_FILL_FLOOD_REC, SHAPE_FILL_FLOOD_NONREC, SHAPE_FILL_CONVEX, SHAPE_FILL_NONCONVEX, SHAPE_FILL_BEZIER, SHAPE_FILL_CIRCLE_CIRCLES, SHAPE_FILL_CIRCLE_LINES, SHAPE_FILL_SQUARE_HERMITE,
    
    SHAPE_BONUS_HAPPY, SHAPE_BONUS_SAD
};

enum ClipShape {
    CLIP_NONE,
    CLIP_RECT,
    CLIP_SQUARE,
    CLIP_CIRCLE
};

struct ClipRegion {
    ClipShape shape;
    int xmin, ymin, xmax, ymax;
    int centerX, centerY, radius;
};

struct Shape {
    ShapeType type;
    int x1, y1;
    int x2, y2;
    ClipRegion clipRegion;
    std::vector<POINT> polygonPts;
    COLORREF color;
};

void DrawShape(HDC hdc, const Shape &shape);