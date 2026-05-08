#include "Ellipse.h"
#include <cmath>
#include <algorithm>

void DrawEllipsePolar(HDC hdc, int xc, int yc, int A, int B, COLORREF c) {
    double dtheta = 1.0 / std::max(A, B);   // step ~ 1 pixel along arc
    const double PI_2 = 1.5707963267948966;
    for (double t = 0.0; t <= PI_2; t += dtheta) {
        int x = (int)std::round(A * std::cos(t));
        int y = (int)std::round(B * std::sin(t));
        Draw4Points(hdc, xc, yc, x, y, c);
    }
    
    Draw4Points(hdc, xc, yc, A, 0, c);
    Draw4Points(hdc, xc, yc, 0, B, c);
}
