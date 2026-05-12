#include <vector>
#include <cmath>
#include <windows.h>


struct Point2D {
    double x, y;
};

// edgeType: 0 = Left, 1 = Right, 2 = Bottom, 3 = Top
std::vector<Point2D> ClipPolygonEdge(const std::vector<Point2D>& poly, int edgeType, int x_min, int y_min, int x_max, int y_max) {
    std::vector<Point2D> outPoly;
    if (poly.empty()) return outPoly;

    Point2D prev = poly.back();

    for (size_t i = 0; i < poly.size(); i++) {
        Point2D curr = poly[i];
        bool currInside = false, prevInside = false;
        
        // Determine inside/outside status based on the specific edge
        if (edgeType == 0) // Left
            currInside = (curr.x >= x_min); prevInside = (prev.x >= x_min);
        else if (edgeType == 1) // Right
            currInside = (curr.x <= x_max); prevInside = (prev.x <= x_max);
        else if (edgeType == 2) // Bottom
            currInside = (curr.y >= y_min); prevInside = (prev.y >= y_min);
        else if (edgeType == 3) // Top
            currInside = (curr.y <= y_max); prevInside = (prev.y <= y_max);
        

        // Logic for finding intersection and pushing vertices
        if (currInside) {
            if (!prevInside) { // Outside to Inside (push intersection, then current)
                Point2D intersect;
                if (edgeType == 0) { intersect.x = x_min; intersect.y = prev.y + (curr.y - prev.y) * (x_min - prev.x) / (curr.x - prev.x); }
                else if (edgeType == 1) { intersect.x = x_max; intersect.y = prev.y + (curr.y - prev.y) * (x_max - prev.x) / (curr.x - prev.x); }
                else if (edgeType == 2) { intersect.y = y_min; intersect.x = prev.x + (curr.x - prev.x) * (y_min - prev.y) / (curr.y - prev.y); }
                else if (edgeType == 3) { intersect.y = y_max; intersect.x = prev.x + (curr.x - prev.x) * (y_max - prev.y) / (curr.y - prev.y); }
                outPoly.push_back(intersect);
            }
            outPoly.push_back(curr); // Current is inside
        } else {
            if (prevInside) { // Inside to Outside (push intersection only)
                Point2D intersect;
                if (edgeType == 0) { intersect.x = x_min; intersect.y = prev.y + (curr.y - prev.y) * (x_min - prev.x) / (curr.x - prev.x); }
                else if (edgeType == 1) { intersect.x = x_max; intersect.y = prev.y + (curr.y - prev.y) * (x_max - prev.x) / (curr.x - prev.x); }
                else if (edgeType == 2) { intersect.y = y_min; intersect.x = prev.x + (curr.x - prev.x) * (y_min - prev.y) / (curr.y - prev.y); }
                else if (edgeType == 3) { intersect.y = y_max; intersect.x = prev.x + (curr.x - prev.x) * (y_max - prev.y) / (curr.y - prev.y); }
                outPoly.push_back(intersect);
            }
        }
        prev = curr;
    }
    return outPoly;
}

void ClipAndDrawPolygon(HDC hdc, std::vector<Point2D> poly, int x_min, int y_min, int x_max, int y_max, int color) {
    poly = ClipPolygonEdge(poly, 0, x_min, y_min, x_max, y_max);
    poly = ClipPolygonEdge(poly, 1, x_min, y_min, x_max, y_max);
    poly = ClipPolygonEdge(poly, 2, x_min, y_min, x_max, y_max);
    poly = ClipPolygonEdge(poly, 3, x_min, y_min, x_max, y_max);

    if (poly.empty()) return;

    for (size_t i = 0; i < poly.size(); i++) {
        Point2D p1 = poly[i];
        Point2D p2 = poly[(i + 1) % poly.size()];
        
        DrawLine(hdc, std::round(p1.x), std::round(p1.y), std::round(p2.x), std::round(p2.y), color);
    }
}