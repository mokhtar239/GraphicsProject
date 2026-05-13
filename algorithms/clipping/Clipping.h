#pragma once  
#include <windows.h>  
#include <vector>  
void ClipPointSquareWindow(HDC hdc, int x, int y, int xleft, int ytop, int sideLength, COLORREF c);  
void ClipLineSquareWindow(HDC hdc, int xs, int ys, int xe, int ye, int xleft, int ytop, int sideLength, COLORREF c);  
void CircleClippingLine(HDC hdc, int x1, int y1, int x2, int y2, int xc, int yc, int R, int color);  
void ClipRectangleLine(HDC hdc, int x1, int y1, int x2, int y2, int x_min, int y_min, int x_max, int y_max, int color);  
void ClipRectanglePoint(HDC hdc, int x, int y, int x_min, int y_min, int x_max, int y_max, int color);  
struct Point2D { double x, y; };  
void ClipAndDrawPolygon(HDC hdc, std::vector<Point2D> poly, int x_min, int y_min, int x_max, int y_max, int color); 
