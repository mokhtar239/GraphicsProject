#include "Filling.h"
#include <list>
#include <algorithm>
#include <cmath>
using namespace std;
#define MAXENTRIES 2000

struct EdgeRec {
    double x;
    double minv;
    int ymax;
    
    bool operator<(const EdgeRec& r) const {
        return x < r.x;
    }
};

typedef list<EdgeRec> EdgeList;

static EdgeRec InitEdgeRec(POINT v1, POINT v2) {
    if (v1.y > v2.y) std::swap(v1, v2);
    EdgeRec rec;
    rec.x = v1.x;
    rec.ymax = v2.y;
    rec.minv = (double)(v2.x - v1.x) / (v2.y - v1.y);
    return rec;
}

void FillGeneralPolygon(HDC hdc, POINT* polygon, int n, COLORREF c) {
    if (n < 3) return;
    
    EdgeList* table = new EdgeList[MAXENTRIES];
    
    // InitEdgeTable logic directly inlined
    POINT v1 = polygon[n - 1];
    for (int i = 0; i < n; i++) {
        POINT v2 = polygon[i];
        if (v1.y != v2.y) {
            EdgeRec rec = InitEdgeRec(v1, v2);
            if (v1.y >= 0 && v1.y < MAXENTRIES) { // safely index top endpoint
                table[min(v1.y, v2.y)].push_back(rec);
            }
        }
        v1 = polygon[i];
    }
    
    int y = 0;
    while (y < MAXENTRIES && table[y].empty()) y++;
    if (y == MAXENTRIES) { delete[] table; return; }
    
    EdgeList ActiveList = table[y];
    while (!ActiveList.empty()) {
        ActiveList.sort();
        
        for (auto it = ActiveList.begin(); it != ActiveList.end(); ) {
            int x1 = (int)std::ceil(it->x);
            it++;
            if (it == ActiveList.end()) break;
            int x2 = (int)std::floor(it->x);
            
            for (int x = x1; x <= x2; x++) SetPixel(hdc, x, y, c);
            it++;
        }
        y++;
        if (y >= MAXENTRIES) break;
        
        auto it = ActiveList.begin();
        while (it != ActiveList.end()) {
            if (y == it->ymax) it = ActiveList.erase(it);
            else it++;
        }
        for (auto& edge : ActiveList) edge.x += edge.minv;
        
        ActiveList.insert(ActiveList.end(), table[y].begin(), table[y].end());
    }
    delete[] table;
}