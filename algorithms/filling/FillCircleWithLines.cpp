#include <cmath>
#include <windows.h>

void FillCircleWithLines(HDC hdc, int xc, int yc, int R, int quarter, int color) 
{
    // Determine angles based on the quarter (1 to 4)
    double startAngle = (quarter - 1) * 90.0;
    double endAngle = quarter * 90.0;

    double dtheta = 1.0 / R;
    
    for (double theta = startAngle; theta <= endAngle; theta += dtheta) 
    {
        double rad = theta * 3.14159 / 180.0;
        int x = xc + R * std::cos(rad);
        int y = yc + R * std::sin(rad);
        
        DrawLine(hdc, xc, yc, x, y, color);
    }
}