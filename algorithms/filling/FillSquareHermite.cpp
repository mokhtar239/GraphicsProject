#include "../curves/HermiteCurve.h"
#include <windows.h>

void FillSquareHermite(HDC hdc, int x_min, int y_min, int x_max, int y_max, int color) 
{
    // Iterate over the width (x-axis)
    for (int x = x_min; x <= x_max; x++) 
    {
        int dx = 0;
        int dy = y_max - y_min;
        
        DrawHermiteCurve(hdc, x, y_min, x, y_max, dx, dy, dx, dy, color);
    }
}