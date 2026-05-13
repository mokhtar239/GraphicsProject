#include "Curves.h"

void DrawCardinalSpline(HDC hdc, const POINT* P, int n, double tension, COLORREF c) {
    if (n < 2) return;

    double s = (1.0 - tension) / 2.0;

    auto pt = [&](int i) -> POINT {
        if (i < 0) return P[0];
        if (i >= n) return P[n - 1];
        return P[i];
    };

    for (int i = 0; i < n - 1; i++) {
        POINT P0 = pt(i);
        POINT P1 = pt(i + 1);

        POINT Pm1 = pt(i - 1);
        POINT P2  = pt(i + 2);

        POINT T0;
        T0.x = (LONG)(s * (P1.x - Pm1.x));
        T0.y = (LONG)(s * (P1.y - Pm1.y));

        POINT T1;
        T1.x = (LONG)(s * (P2.x - P0.x));
        T1.y = (LONG)(s * (P2.y - P0.y));

        DrawHermiteCurve(hdc, P0, T0, P1, T1, c);
    }
}