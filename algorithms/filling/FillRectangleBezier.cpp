#include "Filling.h"
#include <cmath>
#include <utility>
using namespace std;

void DrawBezier(HDC hdc, Point p0, Point p1, Point p2, Point p3, COLORREF c) {
    // Sweeping t densely ensures a solid line segment
    for (double t = 0; t <= 1.0; t += 0.001) {
        double a = pow(1 - t, 3);
        double b = 3 * t * pow(1 - t, 2);
        double d = 3 * t * t * (1 - t);
        double e = t * t * t;

        int x = (int)round(a * p0.x + b * p1.x + d * p2.x + e * p3.x);
        int y = (int)round(a * p0.y + b * p1.y + d * p2.y + e * p3.y);

        SetPixel(hdc, x, y, c);
    }
}

void FillingRectangleBezierHorizontal(
    HDC hdc,
    int left,
    int top,
    int right,
    int bottom,
    COLORREF c)
{
    // Ensure correct orientation regardless of how the mouse was dragged
    if (top > bottom) swap(top, bottom);
    if (left > right) swap(left, right);

	// Draw horizontal scanlines across the rectangle, using Bezier curves for smoothness
    for (int y = top; y <= bottom; y++)
    {
        Point p0 = { left, y };
        Point p1 = { left + (right - left) / 3, y };
        Point p2 = { left + 2 * (right - left) / 3, y };
        Point p3 = { right, y };

        DrawBezier(hdc, p0, p1, p2, p3, c);
    }
}