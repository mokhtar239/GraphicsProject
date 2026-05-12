#include <cmath>
#include <windows.h>

void FillCircleWithCircles(HDC hdc, int xc, int yc, int R, int quarter, int color) 
{
    // Determine angles based on the quarter (1 to 4)
    double startAngle = (quarter - 1) * 90.0;
    double endAngle = quarter * 90.0;

    double dtheta = 1.0 / R;
    
    for (int r = 1; r <= R; r++)
    {
        for (double theta = startAngle; theta <= endAngle; theta += dtheta) 
        { 
            double rad = theta * 3.14159 / 180.0;
            int x = xc + r * std::cos(rad);
            int y = yc + r * std::sin(rad);
            SetPixel(hdc, x, y, color);
        }
    }
}