#include "LineDDA.h"
#include <cmath>
#include <algorithm>

void DrawLineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    SetPixel(hdc, x1, y1, color);
    
    if (std::abs(dx) >= std::abs(dy)) {
        int x = x1, xinc = dx > 0 ? 1 : -1;
        double y = y1, yinc = (double)dy / dx * xinc;
        while (x != x2) {
            x += xinc;
            y += yinc;
            SetPixel(hdc, x, std::round(y), color);
        }
    } else {
        int y = y1, yinc = dy > 0 ? 1 : -1;
        double x = x1, xinc = (double)dx / dy * yinc;
        while (y != y2) {
            x += xinc;
            y += yinc;
            SetPixel(hdc, std::round(x), y, color);
        }
    }
}
