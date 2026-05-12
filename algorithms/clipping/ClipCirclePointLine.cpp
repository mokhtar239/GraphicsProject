#include <cmath>
#include <windows.h>


void CircleClippingLine(HDC hdc, int x1, int y1, int x2, int y2, int xc, int yc, int R, int color) 
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    
    // Coefficients of the quadratic equation A*t^2 + B*t + C = 0
    double A = dx * dx + dy * dy;
    double B = 2 * (dx * (x1 - xc) + dy * (y1 - yc));
    double C = (x1 - xc) * (x1 - xc) + (y1 - yc) * (y1 - yc) - R * R;

    double discriminant = B * B - 4 * A * C;

    if (discriminant < 0) {
        // No intersection. If C <= 0, the first point is inside the circle, 
        // meaning the whole line is inside since there are no intersection points to exit from.
        if (C <= 0) {
            DrawLine(hdc, x1, y1, x2, y2, color); 
        }
        return;
    }

    // Calculate t values
    double t1 = (-B - std::sqrt(discriminant)) / (2 * A);
    double t2 = (-B + std::sqrt(discriminant)) / (2 * A);

    // If t values are outside [0, 1], the line segment is entirely outside the circle
    if (t1 > 1.0 || t2 < 0.0) {
        if (C <= 0) {
            DrawLine(hdc, x1, y1, x2, y2, color);
        }
        return;
    }

    // Clamp t values within the actual line segment [0, 1]
    t1 = std::max(0.0, t1);
    t2 = std::min(1.0, t2);

    // Calculate final intersection points and draw the remaining line
    int nx1 = x1 + t1 * dx;
    int ny1 = y1 + t1 * dy;
    int nx2 = x1 + t2 * dx;
    int ny2 = y1 + t2 * dy;

    DrawLine(hdc, nx1, ny1, nx2, ny2, color);
}