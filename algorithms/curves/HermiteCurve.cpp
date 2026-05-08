#include "Curves.h"
#include <cmath>


void DrawHermiteCurve(HDC hdc, POINT P0, POINT T0, POINT P1, POINT T1, COLORREF c) {
    for (double t = 0.0; t <= 1.0; t += 0.001) {
        double t2 = t * t;
        double t3 = t2 * t;
        double h0 =  2*t3 - 3*t2 + 1;
        double h1 =      t3 - 2*t2 + t;
        double h2 = -2*t3 + 3*t2;
        double h3 =      t3 -   t2;
        double x = h0*P0.x + h1*T0.x + h2*P1.x + h3*T1.x;
        double y = h0*P0.y + h1*T0.y + h2*P1.y + h3*T1.y;
        SetPixel(hdc, (int)std::round(x), (int)std::round(y), c);
    }
}
