#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <windows.h>

void Draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c);
void DrawEllipseDirect(HDC hdc, int xc, int yc, int A, int B, COLORREF c);
void DrawEllipseMidpoint(HDC hdc, int xc, int yc, int A, int B, COLORREF c);
void DrawEllipsePolar(HDC hdc, int xc, int yc, int A, int B, COLORREF c);

#endif
