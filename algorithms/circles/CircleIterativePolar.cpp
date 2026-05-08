#include "Circles.h"
#include <cmath>


void DrawCircleIterativePolar(HDC hdc, int xc, int yc, int R, COLORREF c) {
    double dtheta = 1.0 / R;
    double cs = std::cos(dtheta);
    double sn = std::sin(dtheta);
    double x = R, y = 0;
    Draw8Points(hdc, xc, yc, (int)std::round(x), (int)std::round(y), c);
    while (x > y) {
        double xn = x * cs - y * sn;
        y         = x * sn + y * cs;
        x         = xn;
        Draw8Points(hdc, xc, yc, (int)std::round(x), (int)std::round(y), c);
    }
}
