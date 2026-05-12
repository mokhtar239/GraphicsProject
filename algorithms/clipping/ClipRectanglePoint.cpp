#include <windows.h>

void ClipRectanglePoint(HDC hdc, int x, int y, int x_min, int y_min, int x_max, int y_max, int color) 
{
    // Check if the point lies strictly within the clipping window boundaries
    if (x >= x_min && x <= x_max && y >= y_min && y <= y_max) {
        SetPixel(hdc, x, y, color);
    }
}