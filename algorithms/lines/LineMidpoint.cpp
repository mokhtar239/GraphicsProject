#include "LineMidpoint.h"
#include <algorithm>
#include <cmath>

void DrawLineMidpoint(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    
    if (std::abs(dx) > std::abs(dy)) {
        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        dx = x2 - x1;
        dy = y2 - y1;
        int yinc = dy > 0 ? 1 : -1;
        dy = std::abs(dy);
        
        int d = 2 * dy - dx;
        int change1 = 2 * (dy - dx);
        int change2 = 2 * dy;
        int x = x1, y = y1;
        
        SetPixel(hdc, x, y, color);
        while (x < x2) {
            if (d > 0) {
                d += change1;
                y += yinc;
            } else {
                d += change2;
            }
            x++;
            SetPixel(hdc, x, y, color);
        }
    } else {
        if (y1 > y2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        dx = x2 - x1;
        dy = y2 - y1;
        int xinc = dx > 0 ? 1 : -1;
        dx = std::abs(dx);
        
        int d = 2 * dx - dy;
        int change1 = 2 * (dx - dy);
        int change2 = 2 * dx;
        int x = x1, y = y1;
        
        SetPixel(hdc, x, y, color);
        while (y < y2) {
            if (d > 0) {
                d += change1;
                x += xinc;
            } else {
                d += change2;
            }
            y++;
            SetPixel(hdc, x, y, color);
        }
    }
}
