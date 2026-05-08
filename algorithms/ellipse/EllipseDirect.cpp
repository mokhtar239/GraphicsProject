#include "Ellipse.h"
#include <cmath>

// Direct / Cartesian: solve y from x and x from y to keep pixel density
// even as the slope changes between the two regions.
void DrawEllipseDirect(HDC hdc, int xc, int yc, int A, int B, COLORREF c) {
    // Region 1: step in x while |slope| < 1
    for (int x = 0; x <= A; ++x) {
        double y = (double)B * std::sqrt(1.0 - (double)x * x / ((double)A * A));
        Draw4Points(hdc, xc, yc, x, (int)std::round(y), c);
    }
    // Region 2: step in y while |slope| > 1
    for (int y = 0; y <= B; ++y) {
        double x = (double)A * std::sqrt(1.0 - (double)y * y / ((double)B * B));
        Draw4Points(hdc, xc, yc, (int)std::round(x), y, c);
    }
}
