#pragma once  
#include <windows.h>  
struct Point { int x, y; };  
void FloodFillRecursive(HDC hdc, int x, int y, COLORREF boundaryColor, COLORREF fillColor);  
void FloodFillNonRecursive(HDC hdc, int x, int y, COLORREF boundaryColor, COLORREF fillColor);  
void FillConvexPolygon(HDC hdc, POINT p[], int n, COLORREF color);  
void FillGeneralPolygon(HDC hdc, POINT* polygon, int n, COLORREF color);  
void FillCircleWithCircles(HDC hdc, int xc, int yc, int R, int quarter, int color);  
void FillCircleWithLines(HDC hdc, int xc, int yc, int R, int quarter, int color);  
void FillSquareHermite(HDC hdc, int x_min, int y_min, int x_max, int y_max, int color); 
void DrawBezier(HDC hdc, Point p0, Point p1, Point p2, Point p3, COLORREF c);  
void FillingRectangleBezierHorizontal(HDC hdc, int left, int top, int right, int bottom, COLORREF c); 
