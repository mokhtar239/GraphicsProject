#include "Filling.h"
#include <cmath>
#include <utility>
using namespace std;

void DrawBezier(HDC hdc,
                 Point p0,
                 Point p1,
                 Point p2,
                 Point p3,
                 COLORREF c){
    for(double t = 0; t < 1; t += 0.001)
    {
        double a = pow(1 - t, 3);
        double b = 3 * t * pow(1 - t, 2);
        double d = 3 * t * t * (1 - t);
        double e = t * t * t;

        int x = a*p0.x + b*p1.x + d*p2.x + e*p3.x;
        int y = a*p0.y + b*p1.y + d*p2.y + e*p3.y;

        SetPixel(hdc, x, y, c);
    }
}


void FillingRectangleBezierHorizontal(
    HDC hdc,
    int left,
    int top,
    int right,
    int bottom,
    COLORREF c)
{
    // Ensure that top is less than bottom and left is less than right
    if (top > bottom) swap(top, bottom);
    if (left > right) swap(left, right);
    
   
    for(int y = top; y <= bottom; y += 5) // Adjust the step for smoother or coarser filling
    {
        Point p0 = {left, y};

        Point p1 = {left + (right-left)/3, y + 20};

        Point p2 = {left + 2*(right-left)/3, y - 20};

        Point p3 = {right, y};
        DrawBezier(hdc, p0, p1, p2, p3, c);
    }
}