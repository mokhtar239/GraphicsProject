#include "Circles.h"


void DrawCircleModifiedMidpoint(HDC hdc, int xc, int yc, int R, COLORREF c) {
    int x = 0, y = R;
    int d  = 1 - R;
    int d1 = 3;            
    int d2 = 5 - 2 * R;    
    Draw8Points(hdc, xc, yc, x, y, c);
    while (x < y) {
        if (d < 0) {
            d  += d1;
            d2 += 2;
        } else {
            d  += d2;
            d2 += 4;
            y--;
        }
        d1 += 2;
        x++;
        Draw8Points(hdc, xc, yc, x, y, c);
    }
}
