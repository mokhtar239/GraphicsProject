#include <cmath>
#include <windows.h>
#include "../lines/LineDDA.h"

const int INSIDE = 0; // 0000
const int LEFT   = 1; // 0001
const int RIGHT  = 2; // 0010
const int BOTTOM = 4; // 0100
const int TOP    = 8; // 1000

// Computes the region code for a point (x, y)
int ComputeOutCode(int x, int y, int x_min, int y_min, int x_max, int y_max) {
    int code = INSIDE;
    
    if (x < x_min)       
        code |= LEFT;
    else if (x > x_max)  
        code |= RIGHT;
    
    if (y < y_min)       
        code |= BOTTOM;
    else if (y > y_max)  
        code |= TOP;
    
    return code;
}

void ClipRectangleLine(HDC hdc, int x1, int y1, int x2, int y2, int x_min, int y_min, int x_max, int y_max, int color) {
    int code1 = ComputeOutCode(x1, y1, x_min, y_min, x_max, y_max);
    int code2 = ComputeOutCode(x2, y2, x_min, y_min, x_max, y_max);
    bool accept = false;

    while (true) {
        if ((code1 == 0) && (code2 == 0)) {
            // Trivial accept: both endpoints are inside the window
            accept = true;
            break;
        } else if (code1 & code2) {
            // Trivial reject: both endpoints share an outside region
            break;
        } else {
            // Calculate the line intersection with a window edge
            int code_out;
            double x, y;
            
            // Pick the endpoint that is outside the window
            if (code1 != 0) code_out = code1;
            else code_out = code2;

            // Find the intersection point using the slope equation
            if (code_out & TOP) {
                x = x1 + (x2 - x1) * (y_max - y1) / (double)(y2 - y1);
                y = y_max;
            } else if (code_out & BOTTOM) {
                x = x1 + (x2 - x1) * (y_min - y1) / (double)(y2 - y1);
                y = y_min;
            } else if (code_out & RIGHT) {
                y = y1 + (y2 - y1) * (x_max - x1) / (double)(x2 - x1);
                x = x_max;
            } else if (code_out & LEFT) {
                y = y1 + (y2 - y1) * (x_min - x1) / (double)(x2 - x1);
                x = x_min;
            }

            // Replace the outside point with the intersection point and update code
            if (code_out == code1) {
                x1 = x; y1 = y;
                code1 = ComputeOutCode(x1, y1, x_min, y_min, x_max, y_max);
            } else {
                x2 = x; y2 = y;
                code2 = ComputeOutCode(x2, y2, x_min, y_min, x_max, y_max);
            }
        }
    }
    
    // Draw the final clipped line segment if it was accepted
    if (accept) {
        DrawLineDDA(hdc, std::round(x1), std::round(y1), std::round(x2), std::round(y2), color);
    }
}