#include "../curves/HermiteCurve.h"
#include <windows.h>

void FillSquareHermite(HDC hdc, int x_min, int y_min, int x_max, int y_max, int color)
{
    // Iterate over the width (x-axis)
    for (int x = x_min; x <= x_max; x++)
    {
        POINT P0 = {x, y_min};
        POINT T0 = {0, y_max - y_min};
        POINT P1 = {x, y_max};
        POINT T1 = {0, y_max - y_min};

        DrawHermiteCurve(hdc, P0, T0, P1, T1, color);
    }
}