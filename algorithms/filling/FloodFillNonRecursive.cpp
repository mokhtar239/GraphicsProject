#include "Filling.h"
#include <stack>

using namespace std;

void FloodFillNonRecursive(HDC hdc, int x, int y, COLORREF cb, COLORREF cf) {
    stack<POINT> s;
    s.push({x, y});
    
    while (!s.empty()) {
        POINT p = s.top();
        s.pop();
        
        COLORREF c = GetPixel(hdc, p.x, p.y);
        if (c == cb || c == cf || c == CLR_INVALID) continue;
        
        SetPixel(hdc, p.x, p.y, cf);
        s.push({p.x + 1, p.y});
        s.push({p.x - 1, p.y});
        s.push({p.x, p.y + 1});
        s.push({p.x, p.y - 1});
    }
}