#include "Filling.h"
#include <algorithm>
#include <cmath>

#define MAXENTRIES 2000 // Safely handles high screen resolutions

struct Entry {
    int xmin, xmax;
};

static void ScanEdge(POINT v1, POINT v2, Entry table[]) {
    if (v1.y == v2.y) return;
    if (v1.y > v2.y) std::swap(v1, v2);
    
    double minv = (double)(v2.x - v1.x) / (v2.y - v1.y);
    double x = v1.x;
    int y = v1.y;
    
    while (y < v2.y) {
        if (y >= 0 && y < MAXENTRIES) {
            if (x < table[y].xmin) table[y].xmin = (int)std::ceil(x);
            if (x > table[y].xmax) table[y].xmax = (int)std::floor(x);
        }
        y++;
        x += minv;
    }
}

void FillConvexPolygon(HDC hdc, POINT p[], int n, COLORREF color) {
    if (n < 3) return;
    
    Entry* table = new Entry[MAXENTRIES];
    for (int i = 0; i < MAXENTRIES; i++) {
        table[i].xmin = 999999;  // Effectively MAXINT
        table[i].xmax = -999999; // Effectively -MAXINT
    }
    
    POINT v1 = p[n - 1];
    for (int i = 0; i < n; i++) {
        POINT v2 = p[i];
        ScanEdge(v1, v2, table);
        v1 = p[i];
    }
    
    for (int y = 0; y < MAXENTRIES; y++) {
        if (table[y].xmin < table[y].xmax) {
            for (int x = table[y].xmin; x <= table[y].xmax; x++) {
                SetPixel(hdc, x, y, color);
            }
        }
    }
    delete[] table;
}