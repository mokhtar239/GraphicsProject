#include "Circles.h"
#include <cmath>

void DrawCirclePolar(HDC hdc, int xc, int yc, int R, COLORREF c) {
    double theta = 0.0;
    double dtheta = 1.0 / R;          
    int x = R, y = 0;
    Draw8Points(hdc, xc, yc, x, y, c);
    while (x > y) {
        theta += dtheta;
        x = (int)std::round(R * std::cos(theta));
        y = (int)std::round(R * std::sin(theta));
        Draw8Points(hdc, xc, yc, x, y, c);
    }
}
