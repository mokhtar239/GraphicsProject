#include "Shape.h"
#include "../algorithms/lines/LineDDA.h"
#include "../algorithms/lines/LineMidpoint.h"
#include "../algorithms/lines/LineParametric.h"

void DrawShape(HDC hdc, const Shape& shape) {
    switch (shape.type) {
        case SHAPE_LINE_DDA:
            DrawLineDDA(hdc, shape.x1, shape.y1, shape.x2, shape.y2, shape.color);
            break;
        case SHAPE_LINE_MIDPOINT:
            DrawLineMidpoint(hdc, shape.x1, shape.y1, shape.x2, shape.y2, shape.color);
            break;
        case SHAPE_LINE_PARAMETRIC:
            DrawLineParametric(hdc, shape.x1, shape.y1, shape.x2, shape.y2, shape.color);
            break;
    }
}
