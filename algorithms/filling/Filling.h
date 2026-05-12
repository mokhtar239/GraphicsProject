#pragma once

#include <windows.h>

struct Point {
    int x, y;
};

// ===========================
// Flood Fill
// ===========================

void FloodFillRecursive(
    HDC hdc,
    int x,
    int y,
    COLORREF boundaryColor,
    COLORREF fillColor
);

void FloodFillNonRecursive(
    HDC hdc,
    int x,
    int y,
    COLORREF boundaryColor,
    COLORREF fillColor
);

// ===========================
// Convex Polygon Filling
// ===========================

void FillConvexPolygon(
    HDC hdc,
    POINT p[],
    int n,
    COLORREF color
);

// ===========================
// Non Convex Polygon Filling
// ===========================

void FillGeneralPolygon(
    HDC hdc,
    POINT* polygon,
    int n,
    COLORREF color
);

// ===========================
// Bezier Filling
// ===========================

void DrawBezier(
    HDC hdc,
    Point p0,
    Point p1,
    Point p2,
    Point p3,
    COLORREF c
);

void FillingRectangleBezierHorizontal(
    HDC hdc,
    int left,
    int top,
    int right,
    int bottom,
    COLORREF c
);