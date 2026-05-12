#include "Face.h"
#include <cmath>
// Standalone boundary renderer for outer face and eyes
static void PlotCirclePoints(HDC hdc, int xc, int yc, int x, int y, COLORREF c) {
    SetPixel(hdc, xc + x, yc + y, c); SetPixel(hdc, xc - x, yc + y, c);
    SetPixel(hdc, xc + x, yc - y, c); SetPixel(hdc, xc - x, yc - y, c);
    SetPixel(hdc, xc + y, yc + x, c); SetPixel(hdc, xc - y, yc + x, c);
    SetPixel(hdc, xc + y, yc - x, c); SetPixel(hdc, xc - y, yc - x, c);
}

static void SimpleMidpointCircle(HDC hdc, int xc, int yc, int R, COLORREF c) {
    int x = 0, y = R;
    int d = 1 - R;
    PlotCirclePoints(hdc, xc, yc, x, y, c);
    while (x < y) {
        if (d < 0) d += 2 * x + 3; // direct shift adaptation
        else { d += 2 * (x - y) + 5; y--; }
        x++;
        PlotCirclePoints(hdc, xc, yc, x, y, c);
    }
}

// Renders nose segment natively via parametric tracking
static void DrawNoseLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
    double dt = 1.0 / (std::max(std::abs(x2 - x1), std::abs(y2 - y1)) + 1);
    for (double t = 0.0; t <= 1.0; t += dt) {
        double x = x1 + t * (x2 - x1);
        double y = y1 + t * (y2 - y1);
        SetPixel(hdc, (int)std::round(x), (int)std::round(y), c);
    }
}

// Uses Hermite curve formulation to flex arc explicitly
static void DrawMouthArc(HDC hdc, POINT P0, POINT P1, POINT T0, POINT T1, COLORREF c) {
    for (double t = 0.0; t <= 1.0; t += 0.005) {
        double t2 = t * t;
        double t3 = t2 * t;
        double h0 =  2*t3 - 3*t2 + 1;
        double h1 =      t3 - 2*t2 + t;
        double h2 = -2*t3 + 3*t2;
        double h3 =      t3 -   t2;
        double x = h0*P0.x + h1*T0.x + h2*P1.x + h3*T1.x;
        double y = h0*P0.y + h1*T0.y + h2*P1.y + h3*T1.y;
        SetPixel(hdc, (int)std::round(x), (int)std::round(y), c);
    }
}

void DrawHappyFace(HDC hdc, int xc, int yc, int radius, COLORREF c) {
    // 1. Face boundary
    SimpleMidpointCircle(hdc, xc, yc, radius, c);
    
    // 2. Eyes
    int eyeOffset = radius / 3;
    int eyeRadius = radius / 10;
    SimpleMidpointCircle(hdc, xc - eyeOffset, yc - eyeOffset, eyeRadius, c);
    SimpleMidpointCircle(hdc, xc + eyeOffset, yc - eyeOffset, eyeRadius, c);
    
    // 3. Nose (Vertical segment down center)
    DrawNoseLine(hdc, xc, yc - eyeRadius, xc, yc + eyeOffset, c);
    
    // 4. Mouth (Happy Arc pulling downward at center via tangent setup)
    POINT mStart = { xc - eyeOffset, yc + eyeOffset };
    POINT mEnd   = { xc + eyeOffset, yc + eyeOffset };
    // Downward pointing Y tangents drag the interpolating arc down cleanly
    POINT T0     = { radius / 2,  radius }; 
    POINT T1     = { radius / 2, -radius };
    DrawMouthArc(hdc, mStart, mEnd, T0, T1, c);
}

void DrawSadFace(HDC hdc, int xc, int yc, int radius, COLORREF c) {
    // 1. Face boundary
    SimpleMidpointCircle(hdc, xc, yc, radius, c);
    
    // 2. Eyes
    int eyeOffset = radius / 3;
    int eyeRadius = radius / 10;
    SimpleMidpointCircle(hdc, xc - eyeOffset, yc - eyeOffset, eyeRadius, c);
    SimpleMidpointCircle(hdc, xc + eyeOffset, yc - eyeOffset, eyeRadius, c);
    
    // 3. Nose
    DrawNoseLine(hdc, xc, yc - eyeRadius, xc, yc + eyeOffset, c);
    
    // 4. Mouth (Sad Arc pushing upward at center)
    POINT mStart = { xc - eyeOffset, yc + (int)(1.5 * eyeOffset) };
    POINT mEnd   = { xc + eyeOffset, yc + (int)(1.5 * eyeOffset) };
    // Upward pointing Y tangents pull the interpolating arc up securely
    POINT T0     = { radius / 2, -radius }; 
    POINT T1     = { radius / 2,  radius };
    DrawMouthArc(hdc, mStart, mEnd, T0, T1, c);
}