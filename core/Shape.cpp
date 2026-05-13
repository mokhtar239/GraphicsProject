#include "Shape.h"
#include <cmath>
#include <algorithm>
#include "../algorithms/lines/LineDDA.h"
#include "../algorithms/lines/LineMidpoint.h"
#include "../algorithms/lines/LineParametric.h"
#include "../algorithms/circles/Circles.h"
#include "../algorithms/ellipse/Ellipse.h"
#include "../algorithms/curves/Curves.h"
#include "../algorithms/curves/HermiteCurve.h"
#include "../algorithms/clipping/Clipping.h"
#include "../algorithms/filling/Filling.h"
#include "../bonus/Face.h"

void DrawShape(HDC hdc, const Shape &shape)
{
    int x1 = shape.x1, y1 = shape.y1;
    int x2 = shape.x2, y2 = shape.y2;
    int R = (int)std::round(std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
    int A = std::abs(x2 - x1);
    int B = std::abs(y2 - y1);

    switch (shape.type)
    {
    case SHAPE_LINE_DDA:
        DrawLineDDA(hdc, x1, y1, x2, y2, shape.color);
        break;
    case SHAPE_LINE_MIDPOINT:
        DrawLineMidpoint(hdc, x1, y1, x2, y2, shape.color);
        break;
    case SHAPE_LINE_PARAMETRIC:
        DrawLineParametric(hdc, x1, y1, x2, y2, shape.color);
        break;

    case SHAPE_CIRCLE_DIRECT:
        DrawCircleDirect(hdc, x1, y1, R, shape.color);
        break;
    case SHAPE_CIRCLE_POLAR:
        DrawCirclePolar(hdc, x1, y1, R, shape.color);
        break;
    case SHAPE_CIRCLE_ITERATIVE_POLAR:
        DrawCircleIterativePolar(hdc, x1, y1, R, shape.color);
        break;
    case SHAPE_CIRCLE_MIDPOINT:
        DrawCircleMidpoint(hdc, x1, y1, R, shape.color);
        break;
    case SHAPE_CIRCLE_MODIFIED_MIDPOINT:
        DrawCircleModifiedMidpoint(hdc, x1, y1, R, shape.color);
        break;

    case SHAPE_ELLIPSE_DIRECT:
        DrawEllipseDirect(hdc, x1, y1, A, B, shape.color);
        break;
    case SHAPE_ELLIPSE_POLAR:
        DrawEllipsePolar(hdc, x1, y1, A, B, shape.color);
        break;
    case SHAPE_ELLIPSE_MIDPOINT:
        DrawEllipseMidpoint(hdc, x1, y1, A, B, shape.color);
        break;

    case SHAPE_CURVE_HERMITE:
    {
        POINT p0 = {x1, y1};
        POINT t0 = {0, y2 - y1};
        POINT p1 = {x2, y2};
        POINT t1 = {0, y1 - y2};
        DrawHermiteCurve(hdc, p0, t0, p1, t1, shape.color);
        break;
    }
    case SHAPE_CURVE_CARDINAL:
    {
        if (shape.polygonPts.size() >= 2) {
            DrawCardinalSpline(hdc, shape.polygonPts.data(), shape.polygonPts.size(), 0.0, shape.color);
        }
        break;
    }

    case SHAPE_CLIP_POINT:
        if (shape.clipRegion.shape == CLIP_RECT)
        {
            ClipRectanglePoint(hdc, x1, y1, shape.clipRegion.xmin, shape.clipRegion.ymin, shape.clipRegion.xmax, shape.clipRegion.ymax, shape.color);
        }
        else if (shape.clipRegion.shape == CLIP_SQUARE)
        {
            int side = shape.clipRegion.xmax - shape.clipRegion.xmin;
            ClipPointSquareWindow(hdc, x1, y1, shape.clipRegion.xmin, shape.clipRegion.ymin, side, shape.color);
        }
        else if (shape.clipRegion.shape == CLIP_CIRCLE)
        {
            int dx = x1 - shape.clipRegion.centerX;
            int dy = y1 - shape.clipRegion.centerY;
            if ((dx * dx + dy * dy) <= (shape.clipRegion.radius * shape.clipRegion.radius))
            {
                SetPixel(hdc, x1, y1, shape.color);
            }
        }
        break;

    case SHAPE_CLIP_LINE:
        if (shape.clipRegion.shape == CLIP_RECT)
        {
            ClipRectangleLine(hdc, x1, y1, x2, y2, shape.clipRegion.xmin, shape.clipRegion.ymin, shape.clipRegion.xmax, shape.clipRegion.ymax, shape.color);
        }
        else if (shape.clipRegion.shape == CLIP_SQUARE)
        {
            int side = shape.clipRegion.xmax - shape.clipRegion.xmin;
            ClipLineSquareWindow(hdc, x1, y1, x2, y2, shape.clipRegion.xmin, shape.clipRegion.ymin, side, shape.color);
        }
        else if (shape.clipRegion.shape == CLIP_CIRCLE)
        {
            CircleClippingLine(hdc, x1, y1, x2, y2, shape.clipRegion.centerX, shape.clipRegion.centerY, shape.clipRegion.radius, shape.color);
        }
        break;

    case SHAPE_CLIP_POLY:
        if (shape.clipRegion.shape == CLIP_RECT || shape.clipRegion.shape == CLIP_SQUARE)
        {
            std::vector<Point2D> pts2d;
            for (const auto &p : shape.polygonPts)
            {
                Point2D pt;
                pt.x = (double)p.x;
                pt.y = (double)p.y;
                pts2d.push_back(pt);
            }
            ClipAndDrawPolygon(hdc, pts2d, shape.clipRegion.xmin, shape.clipRegion.ymin, shape.clipRegion.xmax, shape.clipRegion.ymax, shape.color);
        }
        break;

    case SHAPE_FILL_FLOOD_REC:
        FloodFillRecursive(hdc, x1, y1, RGB(0, 0, 0), shape.color);
        break;
    case SHAPE_FILL_FLOOD_NONREC:
        FloodFillNonRecursive(hdc, x1, y1, RGB(0, 0, 0), shape.color);
        break;
    case SHAPE_FILL_CONVEX:
        if (shape.polygonPts.size() > 2)
            FillConvexPolygon(hdc, (POINT *)shape.polygonPts.data(), shape.polygonPts.size(), shape.color);
        break;
    case SHAPE_FILL_NONCONVEX:
        if (shape.polygonPts.size() > 2)
            FillGeneralPolygon(hdc, (POINT *)shape.polygonPts.data(), shape.polygonPts.size(), shape.color);
        break;
    case SHAPE_FILL_BEZIER:
        FillingRectangleBezierHorizontal(hdc, x1, y1, x2, y2, shape.color);
        break;
    case SHAPE_FILL_CIRCLE_CIRCLES:
    {
        int quarter = 1;
        if (x2 > x1 && y2 > y1) quarter = 1;
        else if (x2 < x1 && y2 > y1) quarter = 2;
        else if (x2 < x1 && y2 < y1) quarter = 3;
        else if (x2 > x1 && y2 < y1) quarter = 4;
        FillCircleWithCircles(hdc, x1, y1, R, quarter, shape.color);
        break;
    }
    case SHAPE_FILL_CIRCLE_LINES:
    {
        int quarter = 1;
        if (x2 > x1 && y2 > y1) quarter = 1;
        else if (x2 < x1 && y2 > y1) quarter = 2;
        else if (x2 < x1 && y2 < y1) quarter = 3;
        else if (x2 > x1 && y2 < y1) quarter = 4;
        FillCircleWithLines(hdc, x1, y1, R, quarter, shape.color);
        break;
    }
    case SHAPE_FILL_SQUARE_HERMITE:
    {
        int side = std::max(A, B);
        int xm = std::min(x1, x2);
        int ym = std::min(y1, y2);
        FillSquareHermite(hdc, xm, ym, xm + side, ym + side, shape.color);
        break;
    }

    case SHAPE_BONUS_HAPPY:
        DrawHappyFace(hdc, x1, y1, R, shape.color);
        break;
    case SHAPE_BONUS_SAD:
        DrawSadFace(hdc, x1, y1, R, shape.color);
        break;
    }
}