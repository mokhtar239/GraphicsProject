#pragma once
#ifndef NOMINMAX
#define NOMINMAX // Prevent Windows.h from defining min/max macros that conflict with std::min/std::max
#endif
#include <Windows.h>
#include <algorithm>

// Bonus Smiley Face Implementations
void DrawHappyFace(HDC hdc, int xc, int yc, int radius, COLORREF c);
void DrawSadFace(HDC hdc, int xc, int yc, int radius, COLORREF c);