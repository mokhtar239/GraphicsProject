#ifndef CURVES_H
#define CURVES_H

#include <windows.h>

void DrawCardinalSpline(HDC hdc, const POINT *P, int n, double c, COLORREF color);
void DrawHermiteCurve(HDC hdc, POINT P0, POINT T0, POINT P1, POINT T1, COLORREF color);

#endif
