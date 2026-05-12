#include "Clipping.h"
#include <cmath>
#include <algorithm>

union OutCode {
    unsigned All : 4;
    struct { unsigned left : 1, top : 1, right : 1, bottom : 1; };
};

static OutCode GetOutCode(double x, double y, int xleft, int ytop, int xright, int ybottom) {
    OutCode out;
    out.All = 0;
    if (x < xleft) out.left = 1; else if (x > xright) out.right = 1;
    if (y < ytop)  out.top = 1;  else if (y > ybottom) out.bottom = 1;
    return out;
}

static void VIntersect(double xs, double ys, double xe, double ye, int x, double* xi, double* yi) {
    *xi = x;
    *yi = ys + (x - xs) * (ye - ys) / (xe - xs);
}

static void HIntersect(double xs, double ys, double xe, double ye, int y, double* xi, double* yi) {
    *yi = y;
    *xi = xs + (y - ys) * (xe - xs) / (ye - ys);
}

// Function handles Line Clipping against Square Clipping Window
void ClipLineSquareWindow(HDC hdc, int xs, int ys, int xe, int ye, 
                          int xleft, int ytop, int sideLength, COLORREF c) 
{
    // Derive matching square bounds internally
    int xright = xleft + sideLength;
    int ybottom = ytop + sideLength;

    double x1 = xs, y1 = ys, x2 = xe, y2 = ye;
    OutCode out1 = GetOutCode(x1, y1, xleft, ytop, xright, ybottom);
    OutCode out2 = GetOutCode(x2, y2, xleft, ytop, xright, ybottom);
    
    while ((out1.All || out2.All) && !(out1.All & out2.All)) {
        double xi = 0, yi = 0;
        if (out1.All) {
            if (out1.left)       VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
            else if (out1.top)   HIntersect(x1, y1, x2, y2, ytop, &xi, &yi);
            else if (out1.right) VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
            else                 HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi);
            x1 = xi; y1 = yi;
            out1 = GetOutCode(x1, y1, xleft, ytop, xright, ybottom);
        } else {
            if (out2.left)       VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
            else if (out2.top)   HIntersect(x1, y1, x2, y2, ytop, &xi, &yi);
            else if (out2.right) VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
            else                 HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi);
            x2 = xi; y2 = yi;
            out2 = GetOutCode(x2, y2, xleft, ytop, xright, ybottom);
        }
    }
    
    if (!out1.All && !out2.All) {
        HPEN hPen = CreatePen(PS_SOLID, 1, c);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        MoveToEx(hdc, (int)std::round(x1), (int)std::round(y1), NULL);
        LineTo(hdc, (int)std::round(x2), (int)std::round(y2));
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    }
}

// Point Clipping against Square Clipping Window
void ClipPointSquareWindow(HDC hdc, int x, int y, int xleft, int ytop, int sideLength, COLORREF c) {
    int xright = xleft + sideLength;
    int ybottom = ytop + sideLength;
    if (x >= xleft && x <= xright && y >= ytop && y <= ybottom) {
        SetPixel(hdc, x, y, c);
    }
}