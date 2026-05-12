#pragma once

#include <windows.h>

// ===========================
// Point Clipping
// ===========================

void ClipPointSquareWindow(
    HDC hdc,
    int x,
    int y,
    int xleft,
    int ytop,
    int sideLength,
    COLORREF c
);

// ===========================
// Line Clipping
// ===========================

void ClipLineSquareWindow(
    HDC hdc,
    int xs,
    int ys,
    int xe,
    int ye,
    int xleft,
    int ytop,
    int sideLength,
    COLORREF c
);