#include "Ellipse.h"

void DrawEllipseMidpoint(HDC hdc, int xc, int yc, int A, int B, COLORREF c) {
    long A2 = (long)A * A;
    long B2 = (long)B * B;
    long twoA2 = 2 * A2;
    long twoB2 = 2 * B2;

    int x = 0;
    int y = B;
    long dx = 0;
    long dy = twoA2 * y;

    // ---- Region 1: slope > -1 ----
    long d1 = (long)(B2 - A2 * B + 0.25 * A2);
    while (dx < dy) {
        Draw4Points(hdc, xc, yc, x, y, c);
        if (d1 < 0) {
            x++;
            dx += twoB2;
            d1 += dx + B2;
        } else {
            x++;
            y--;
            dx += twoB2;
            dy -= twoA2;
            d1 += dx - dy + B2;
        }
    }

    // ---- Region 2: slope < -1 ----
    long d2 = (long)(B2 * (x + 0.5) * (x + 0.5)
                   + A2 * (y - 1) * (y - 1)
                   - A2 * B2);
    while (y >= 0) {
        Draw4Points(hdc, xc, yc, x, y, c);
        if (d2 > 0) {
            y--;
            dy -= twoA2;
            d2 += A2 - dy;
        } else {
            x++;
            y--;
            dx += twoB2;
            dy -= twoA2;
            d2 += dx - dy + A2;
        }
    }
}
