#ifndef CIRCLES_H
#define CIRCLES_H

#include <windows.h>

void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c);
void DrawCircleDirect(HDC hdc, int xc, int yc, int R, COLORREF c);
void DrawCircleIterativePolar(HDC hdc, int xc, int yc, int R, COLORREF c);
void DrawCircleMidpoint(HDC hdc, int xc, int yc, int R, COLORREF c);
void DrawCircleModifiedMidpoint(HDC hdc, int xc, int yc, int R, COLORREF c);
void DrawCirclePolar(HDC hdc, int xc, int yc, int R, COLORREF c);

#endif
