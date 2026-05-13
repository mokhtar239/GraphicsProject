#pragma once
#ifndef NOMINMAX
#define NOMINMAX // Prevent Windows.h from defining min/max macros that conflict with std::min/std::max
#endif
#include <Windows.h>
#include <algorithm>

// Bonus Smiley Face Implementations
// bgColor: the background color currently on the DC (used for correct flood fill seeding)
void DrawHappyFace(HDC hdc, int xc, int yc, int radius, COLORREF c, COLORREF bgColor = RGB(255, 255, 255));
void DrawSadFace(HDC hdc, int xc, int yc, int radius, COLORREF c, COLORREF bgColor = RGB(255, 255, 255));