#include "LineParametric.h"
#include <cmath>
#include <algorithm>

void DrawLineParametric(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = std::max(std::abs(dx), std::abs(dy));
    
    if (steps == 0) {
        SetPixel(hdc, x1, y1, color);
        return;
    }
    
    for (int i = 0; i <= steps; i++) {
        double t = (double)i / steps;
        int x = std::round(x1 + t * (x2 - x1));
        int y = std::round(y1 + t * (y2 - y1));
        SetPixel(hdc, x, y, color);
    }
}
