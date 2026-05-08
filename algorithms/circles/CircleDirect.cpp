#include "Circles.h"
#include <cmath>


void DrawCircleDirect(HDC hdc, int xc, int yc, int R, COLORREF c) {
    int x = 0;
    double y = R;
    Draw8Points(hdc, xc, yc, x, (int)y, c);
    while (x < y) {
        x++;
        y = std::sqrt((double)R * R - (double)x * x);
        Draw8Points(hdc, xc, yc, x, (int)std::round(y), c);
    }
}
