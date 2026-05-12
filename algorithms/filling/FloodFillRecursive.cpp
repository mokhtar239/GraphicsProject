#include "Filling.h"

using namespace std;

void FloodFillRecursive(HDC hdc, int x, int y, COLORREF cb, COLORREF cf) {
    COLORREF c = GetPixel(hdc, x, y);
    if (c == cb || c == cf || c == CLR_INVALID) return;
    
    SetPixel(hdc, x, y, cf);
    FloodFillRecursive(hdc, x + 1, y, cb, cf);
    FloodFillRecursive(hdc, x - 1, y, cb, cf);
    FloodFillRecursive(hdc, x, y + 1, cb, cf);
    FloodFillRecursive(hdc, x, y - 1, cb, cf);
}