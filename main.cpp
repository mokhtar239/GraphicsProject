#include <windows.h>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include "resource.h"
#include "core/Shape.h"
#include "io/FileIO.h"

enum AppMode {
    MODE_DRAW_SHAPE,
    MODE_SET_CLIP_RECT,
    MODE_SET_CLIP_SQUARE,
    MODE_SET_CLIP_CIRCLE
};

AppMode currentMode = MODE_DRAW_SHAPE;
ClipRegion globalClipRegion = {CLIP_NONE, 100, 100, 300, 300, 200, 200, 100};
std::vector<POINT> currentPolygonPts;

HDC memDC = NULL;
HBITMAP memBitmap = NULL;
int clientWidth = 0, clientHeight = 0;
int currentX = 0, currentY = 0;

std::vector<Shape> shapes;
ShapeType currentShapeType = SHAPE_LINE_DDA;
COLORREF currentColor = RGB(0, 0, 0);
HCURSOR currentCursor = NULL;
bool usingCrossCursor = false;
HBRUSH bgBrush = NULL;

bool isDrawing = false;
int startX = 0, startY = 0;

void RedrawAllToMemDC() {
    if (!memDC) return;
    RECT rect = {0, 0, clientWidth, clientHeight};
    FillRect(memDC, &rect, bgBrush);
    for (const auto& s : shapes) {
        DrawShape(memDC, s);
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE: {
            currentCursor = LoadCursor(NULL, IDC_ARROW);
            bgBrush = (HBRUSH)(COLOR_WINDOW + 1);

            HMENU hMenu = CreateMenu();
            
            HMENU hFileMenu = CreatePopupMenu();
            AppendMenu(hFileMenu, MF_STRING, ID_FILE_CLEAR, "Clear Screen");
            AppendMenu(hFileMenu, MF_STRING, ID_FILE_SAVE, "Save");
            AppendMenu(hFileMenu, MF_STRING, ID_FILE_LOAD, "Load");
            
            HMENU hPrefMenu = CreatePopupMenu();
            AppendMenu(hPrefMenu, MF_STRING, ID_PREF_BG_WHITE, "White Background");
            AppendMenu(hPrefMenu, MF_STRING, ID_PREF_CURSOR, "Change Cursor");
            AppendMenu(hPrefMenu, MF_STRING, ID_PREF_COLOR, "Choose Color");

            HMENU hLinesMenu = CreatePopupMenu();
            AppendMenu(hLinesMenu, MF_STRING, ID_LINE_DDA, "DDA");
            AppendMenu(hLinesMenu, MF_STRING, ID_LINE_MIDPOINT, "Midpoint");
            AppendMenu(hLinesMenu, MF_STRING, ID_LINE_PARAMETRIC, "Parametric");

            HMENU hCirclesMenu = CreatePopupMenu();
            AppendMenu(hCirclesMenu, MF_STRING, ID_CIRCLE_DIRECT, "Direct");
            AppendMenu(hCirclesMenu, MF_STRING, ID_CIRCLE_POLAR, "Polar");
            AppendMenu(hCirclesMenu, MF_STRING, ID_CIRCLE_ITERATIVE_POLAR, "Iterative Polar");
            AppendMenu(hCirclesMenu, MF_STRING, ID_CIRCLE_MIDPOINT, "Midpoint");
            AppendMenu(hCirclesMenu, MF_STRING, ID_CIRCLE_MODIFIED_MIDPOINT, "Modified Midpoint");

            HMENU hEllipseMenu = CreatePopupMenu();
            AppendMenu(hEllipseMenu, MF_STRING, ID_ELLIPSE_DIRECT, "Direct");
            AppendMenu(hEllipseMenu, MF_STRING, ID_ELLIPSE_POLAR, "Polar");
            AppendMenu(hEllipseMenu, MF_STRING, ID_ELLIPSE_MIDPOINT, "Midpoint");

            HMENU hCurvesMenu = CreatePopupMenu();
            AppendMenu(hCurvesMenu, MF_STRING, ID_CURVE_HERMITE, "Hermite");
            AppendMenu(hCurvesMenu, MF_STRING, ID_CURVE_CARDINAL, "Cardinal");

            HMENU hClippingMenu = CreatePopupMenu();
            AppendMenu(hClippingMenu, MF_STRING, ID_SET_CLIP_RECT, "1. Set Window: Rectangle");
            AppendMenu(hClippingMenu, MF_STRING, ID_SET_CLIP_SQUARE, "1. Set Window: Square");
            AppendMenu(hClippingMenu, MF_STRING, ID_SET_CLIP_CIRCLE, "1. Set Window: Circle");
            AppendMenu(hClippingMenu, MF_SEPARATOR, 0, NULL);
            AppendMenu(hClippingMenu, MF_STRING, ID_CLIP_DRAW_POINT, "2. Draw Clipped Point");
            AppendMenu(hClippingMenu, MF_STRING, ID_CLIP_DRAW_LINE, "2. Draw Clipped Line");
            AppendMenu(hClippingMenu, MF_STRING, ID_CLIP_DRAW_POLY, "2. Draw Clipped Polygon");

            HMENU hFillingMenu = CreatePopupMenu();
            AppendMenu(hFillingMenu, MF_STRING, ID_FILL_FLOOD_REC, "Flood Recursive");
            AppendMenu(hFillingMenu, MF_STRING, ID_FILL_FLOOD_NONREC, "Flood Non-Recursive");
            AppendMenu(hFillingMenu, MF_STRING, ID_FILL_CONVEX, "Convex");
            AppendMenu(hFillingMenu, MF_STRING, ID_FILL_NONCONVEX, "Non-Convex");
            AppendMenu(hFillingMenu, MF_STRING, ID_FILL_BEZIER, "Bezier Filling");
            AppendMenu(hFillingMenu, MF_STRING, ID_FILL_CIRCLE_CIRCLES, "Circle with Circles");
            AppendMenu(hFillingMenu, MF_STRING, ID_FILL_CIRCLE_LINES, "Circle with Lines");
            AppendMenu(hFillingMenu, MF_STRING, ID_FILL_SQUARE_HERMITE, "Square Hermite");

            HMENU hBonusMenu = CreatePopupMenu();
            AppendMenu(hBonusMenu, MF_STRING, ID_BONUS_HAPPY, "Happy Face");
            AppendMenu(hBonusMenu, MF_STRING, ID_BONUS_SAD, "Sad Face");

            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hPrefMenu, "Preferences");
            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hLinesMenu, "Lines");
            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hCirclesMenu, "Circles");
            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hEllipseMenu, "Ellipses");
            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hCurvesMenu, "Curves");
            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hClippingMenu, "Clipping");
            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFillingMenu, "Filling");
            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hBonusMenu, "Bonus");

            SetMenu(hwnd, hMenu);
            break;
        }

        case WM_SIZE: {
            clientWidth = LOWORD(lParam);
            clientHeight = HIWORD(lParam);

            HDC hdc = GetDC(hwnd);
            if (memDC) DeleteDC(memDC);
            if (memBitmap) DeleteObject(memBitmap);

            memDC = CreateCompatibleDC(hdc);
            memBitmap = CreateCompatibleBitmap(hdc, clientWidth, clientHeight);
            SelectObject(memDC, memBitmap);

            RedrawAllToMemDC();
            ReleaseDC(hwnd, hdc);
            break;
        }

        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case ID_FILE_CLEAR:
                    shapes.clear();
                    currentPolygonPts.clear();
                    RedrawAllToMemDC();
                    InvalidateRect(hwnd, NULL, TRUE);
                    std::cout << "Screen cleared.\n";
                    break;
                case ID_FILE_SAVE:
                    if (SaveShapes(shapes, "shapes.txt")) {
                        std::cout << "Saved shapes to shapes.txt.\n";
                    } else {
                        std::cout << "Failed to save shapes.\n";
                    }
                    break;
                case ID_FILE_LOAD:
                    if (LoadShapes(shapes, "shapes.txt")) {
                        RedrawAllToMemDC();
                        InvalidateRect(hwnd, NULL, TRUE);
                        std::cout << "Loaded shapes from shapes.txt.\n";
                    } else {
                        std::cout << "Failed to load shapes.\n";
                    }
                    break;
                case ID_PREF_BG_WHITE:
                    if (bgBrush && bgBrush != (HBRUSH)(COLOR_WINDOW + 1)) {
                        DeleteObject(bgBrush);
                    }
                    bgBrush = CreateSolidBrush(RGB(255, 255, 255));
                    RedrawAllToMemDC();
                    InvalidateRect(hwnd, NULL, TRUE);
                    std::cout << "Background changed to white.\n";
                    break;
                case ID_PREF_CURSOR:
                    if (!usingCrossCursor) {
                        currentCursor = LoadCursor(NULL, IDC_CROSS);
                        usingCrossCursor = true;
                        std::cout << "Cursor changed to cross.\n";
                    } else {
                        currentCursor = LoadCursor(NULL, IDC_ARROW);
                        usingCrossCursor = false;
                        std::cout << "Cursor changed to arrow.\n";
                    }
                    SetCursor(currentCursor);
                    break;
                case ID_PREF_COLOR: {
                    std::cout << "Enter RGB values separated by spaces (e.g. 255 0 0 for red): ";
                    int r, g, b;
                    if (std::cin >> r >> g >> b) {
                        currentColor = RGB(r, g, b);
                        std::cout << "Color updated.\n";
                    } else {
                        std::cin.clear();
                        std::cin.ignore(10000, '\n');
                        std::cout << "Invalid input.\n";
                    }
                    break;
                }

                case ID_LINE_DDA: currentShapeType = SHAPE_LINE_DDA; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected DDA Line.\n"; break;
                case ID_LINE_MIDPOINT: currentShapeType = SHAPE_LINE_MIDPOINT; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Midpoint Line.\n"; break;
                case ID_LINE_PARAMETRIC: currentShapeType = SHAPE_LINE_PARAMETRIC; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Parametric Line.\n"; break;

                case ID_CIRCLE_DIRECT: currentShapeType = SHAPE_CIRCLE_DIRECT; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Circle Direct.\n"; break;
                case ID_CIRCLE_POLAR: currentShapeType = SHAPE_CIRCLE_POLAR; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Circle Polar.\n"; break;
                case ID_CIRCLE_ITERATIVE_POLAR: currentShapeType = SHAPE_CIRCLE_ITERATIVE_POLAR; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Circle Iterative Polar.\n"; break;
                case ID_CIRCLE_MIDPOINT: currentShapeType = SHAPE_CIRCLE_MIDPOINT; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Circle Midpoint.\n"; break;
                case ID_CIRCLE_MODIFIED_MIDPOINT: currentShapeType = SHAPE_CIRCLE_MODIFIED_MIDPOINT; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Circle Modified Midpoint.\n"; break;

                case ID_ELLIPSE_DIRECT: currentShapeType = SHAPE_ELLIPSE_DIRECT; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Ellipse Direct.\n"; break;
                case ID_ELLIPSE_POLAR: currentShapeType = SHAPE_ELLIPSE_POLAR; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Ellipse Polar.\n"; break;
                case ID_ELLIPSE_MIDPOINT: currentShapeType = SHAPE_ELLIPSE_MIDPOINT; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Ellipse Midpoint.\n"; break;

                case ID_CURVE_BEZIER: currentShapeType = SHAPE_CURVE_BEZIER; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Bezier Curve.\n"; break;
                case ID_CURVE_HERMITE: currentShapeType = SHAPE_CURVE_HERMITE; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Hermite Curve.\n"; break;
                case ID_CURVE_CARDINAL: currentShapeType = SHAPE_CURVE_CARDINAL; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Cardinal Curve.\n"; break;

                case ID_SET_CLIP_RECT: currentMode = MODE_SET_CLIP_RECT; std::cout << "Drag to set RECTANGLE clipping window.\n"; break;
                case ID_SET_CLIP_SQUARE: currentMode = MODE_SET_CLIP_SQUARE; std::cout << "Drag to set SQUARE clipping window.\n"; break;
                case ID_SET_CLIP_CIRCLE: currentMode = MODE_SET_CLIP_CIRCLE; std::cout << "Drag to set CIRCLE clipping window.\n"; break;

                case ID_CLIP_DRAW_POINT: currentMode = MODE_DRAW_SHAPE; currentShapeType = SHAPE_CLIP_POINT; std::cout << "Draw Clipped Point.\n"; break;
                case ID_CLIP_DRAW_LINE: currentMode = MODE_DRAW_SHAPE; currentShapeType = SHAPE_CLIP_LINE; std::cout << "Draw Clipped Line.\n"; break;
                case ID_CLIP_DRAW_POLY:
                    if (globalClipRegion.shape == CLIP_CIRCLE) {
                        std::cout << "[ERROR] Polygon clipping requires Rect or Square!\n";
                    } else {
                        currentMode = MODE_DRAW_SHAPE;
                        currentShapeType = SHAPE_CLIP_POLY;
                        currentPolygonPts.clear();
                        std::cout << "Draw Clipped Polygon.\n";
                    }
                    break;

                case ID_FILL_FLOOD_REC: currentShapeType = SHAPE_FILL_FLOOD_REC; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Flood Recursive.\n"; break;
                case ID_FILL_FLOOD_NONREC: currentShapeType = SHAPE_FILL_FLOOD_NONREC; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Flood Non-Recursive.\n"; break;
                case ID_FILL_CONVEX: currentShapeType = SHAPE_FILL_CONVEX; currentMode = MODE_DRAW_SHAPE; currentPolygonPts.clear(); std::cout << "Selected Convex Fill.\n"; break;
                case ID_FILL_NONCONVEX: currentShapeType = SHAPE_FILL_NONCONVEX; currentMode = MODE_DRAW_SHAPE; currentPolygonPts.clear(); std::cout << "Selected Non-Convex Fill.\n"; break;
                case ID_FILL_BEZIER: currentShapeType = SHAPE_FILL_BEZIER; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Bezier Fill.\n"; break;
                case ID_FILL_CIRCLE_CIRCLES: currentShapeType = SHAPE_FILL_CIRCLE_CIRCLES; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Circle with Circles Fill.\n"; break;
                case ID_FILL_CIRCLE_LINES: currentShapeType = SHAPE_FILL_CIRCLE_LINES; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Circle with Lines Fill.\n"; break;
                case ID_FILL_SQUARE_HERMITE: currentShapeType = SHAPE_FILL_SQUARE_HERMITE; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Square Hermite Fill.\n"; break;

                case ID_BONUS_HAPPY: currentShapeType = SHAPE_BONUS_HAPPY; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Happy Face.\n"; break;
                case ID_BONUS_SAD: currentShapeType = SHAPE_BONUS_SAD; currentMode = MODE_DRAW_SHAPE; std::cout << "Selected Sad Face.\n"; break;
            }
            break;

        case WM_LBUTTONDOWN:
            startX = LOWORD(lParam);
            startY = HIWORD(lParam);
            currentX = startX;
            currentY = startY;

            if (currentMode != MODE_DRAW_SHAPE) {
                isDrawing = true;
            } else if (currentShapeType == SHAPE_CLIP_POLY || currentShapeType == SHAPE_FILL_CONVEX || currentShapeType == SHAPE_FILL_NONCONVEX) {
                POINT p = {startX, startY};
                currentPolygonPts.push_back(p);
                InvalidateRect(hwnd, NULL, TRUE); 
            } else {
                isDrawing = true;
            }
            break;

        case WM_RBUTTONDOWN:
            if (currentMode == MODE_DRAW_SHAPE && (currentShapeType == SHAPE_CLIP_POLY || currentShapeType == SHAPE_FILL_CONVEX || currentShapeType == SHAPE_FILL_NONCONVEX)) {
                if (currentPolygonPts.size() > 1) {
                    Shape s;
                    s.type = currentShapeType;
                    s.color = currentColor;
                    s.clipRegion = globalClipRegion;
                    s.polygonPts = currentPolygonPts;

                    shapes.push_back(s);
                    DrawShape(memDC, s);
                    InvalidateRect(hwnd, NULL, TRUE);
                }
                currentPolygonPts.clear();
            }
            break;

        case WM_LBUTTONUP:
            if (isDrawing) {
                int endX = LOWORD(lParam);
                int endY = HIWORD(lParam);
                
                if (currentMode == MODE_SET_CLIP_RECT) {
                    globalClipRegion.shape = CLIP_RECT;
                    globalClipRegion.xmin = std::min(startX, endX);
                    globalClipRegion.ymin = std::min(startY, endY);
                    globalClipRegion.xmax = std::max(startX, endX);
                    globalClipRegion.ymax = std::max(startY, endY);
                } else if (currentMode == MODE_SET_CLIP_SQUARE) {
                    globalClipRegion.shape = CLIP_SQUARE;
                    int side = std::max(std::abs(endX - startX), std::abs(endY - startY));
                    int signX = (endX > startX) ? 1 : -1;
                    int signY = (endY > startY) ? 1 : -1;
                    globalClipRegion.xmin = std::min(startX, startX + signX * side);
                    globalClipRegion.ymin = std::min(startY, startY + signY * side);
                    globalClipRegion.xmax = std::max(startX, startX + signX * side);
                    globalClipRegion.ymax = std::max(startY, startY + signY * side);
                } else if (currentMode == MODE_SET_CLIP_CIRCLE) {
                    globalClipRegion.shape = CLIP_CIRCLE;
                    globalClipRegion.centerX = startX;
                    globalClipRegion.centerY = startY;
                    globalClipRegion.radius = (int)std::round(std::sqrt(std::pow(endX - startX, 2) + std::pow(endY - startY, 2)));
                } else {
                    Shape s;
                    s.type = currentShapeType;
                    s.x1 = startX;
                    s.y1 = startY;
                    s.x2 = endX;
                    s.y2 = endY;
                    s.color = currentColor;
                    s.clipRegion = globalClipRegion;
                    
                    shapes.push_back(s);
                    DrawShape(memDC, s);
                }
                
                isDrawing = false;
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;

        case WM_MOUSEMOVE:
            currentX = LOWORD(lParam);
            currentY = HIWORD(lParam);
            if (isDrawing || (!currentPolygonPts.empty() && currentMode == MODE_DRAW_SHAPE)) {
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            BitBlt(hdc, 0, 0, clientWidth, clientHeight, memDC, 0, 0, SRCCOPY);

            HPEN hPen = CreatePen(PS_DASH, 1, RGB(100, 100, 100));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
            HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush);

            if (globalClipRegion.shape == CLIP_RECT || globalClipRegion.shape == CLIP_SQUARE) {
                Rectangle(hdc, globalClipRegion.xmin, globalClipRegion.ymin, globalClipRegion.xmax, globalClipRegion.ymax);
            } else if (globalClipRegion.shape == CLIP_CIRCLE) {
                Ellipse(hdc, globalClipRegion.centerX - globalClipRegion.radius,
                             globalClipRegion.centerY - globalClipRegion.radius,
                             globalClipRegion.centerX + globalClipRegion.radius,
                             globalClipRegion.centerY + globalClipRegion.radius);
            }
            
            SelectObject(hdc, hOldPen);
            SelectObject(hdc, hOldBrush);
            DeleteObject(hPen);

            if (currentMode == MODE_DRAW_SHAPE && !currentPolygonPts.empty() && 
               (currentShapeType == SHAPE_CLIP_POLY || currentShapeType == SHAPE_FILL_CONVEX || currentShapeType == SHAPE_FILL_NONCONVEX)) {
                HPEN hp = CreatePen(PS_SOLID, 1, currentColor);
                HPEN oldHp = (HPEN)SelectObject(hdc, hp);
                MoveToEx(hdc, currentPolygonPts[0].x, currentPolygonPts[0].y, NULL);
                for (size_t i = 1; i < currentPolygonPts.size(); ++i) {
                    LineTo(hdc, currentPolygonPts[i].x, currentPolygonPts[i].y);
                }
                LineTo(hdc, currentX, currentY);
                SelectObject(hdc, oldHp);
                DeleteObject(hp);
            }

            if (isDrawing) {
                if (currentMode == MODE_SET_CLIP_RECT) {
                    HPEN rb = CreatePen(PS_DOT, 1, RGB(0,0,0)); HPEN o_rb = (HPEN)SelectObject(hdc, rb);
                    SelectObject(hdc, GetStockObject(NULL_BRUSH));
                    Rectangle(hdc, std::min(startX, currentX), std::min(startY, currentY), std::max(startX, currentX), std::max(startY, currentY));
                    SelectObject(hdc, o_rb); DeleteObject(rb);
                } else if (currentMode == MODE_SET_CLIP_SQUARE) {
                    HPEN rb = CreatePen(PS_DOT, 1, RGB(0,0,0)); HPEN o_rb = (HPEN)SelectObject(hdc, rb);
                    SelectObject(hdc, GetStockObject(NULL_BRUSH));
                    int side = std::max(std::abs(currentX - startX), std::abs(currentY - startY));
                    int signX = (currentX > startX) ? 1 : -1; int signY = (currentY > startY) ? 1 : -1;
                    Rectangle(hdc, startX, startY, startX + signX * side, startY + signY * side);
                    SelectObject(hdc, o_rb); DeleteObject(rb);
                } else if (currentMode == MODE_SET_CLIP_CIRCLE) {
                    HPEN rb = CreatePen(PS_DOT, 1, RGB(0,0,0)); HPEN o_rb = (HPEN)SelectObject(hdc, rb);
                    SelectObject(hdc, GetStockObject(NULL_BRUSH));
                    int r = (int)std::round(std::sqrt(std::pow(currentX - startX, 2) + std::pow(currentY - startY, 2)));
                    Ellipse(hdc, startX - r, startY - r, startX + r, startY + r);
                    SelectObject(hdc, o_rb); DeleteObject(rb);
                } else if (currentMode == MODE_DRAW_SHAPE && currentShapeType != SHAPE_CLIP_POLY && currentShapeType != SHAPE_FILL_CONVEX && currentShapeType != SHAPE_FILL_NONCONVEX) {
                    Shape temp;
                    temp.type = currentShapeType;
                    temp.x1 = startX; temp.y1 = startY;
                    temp.x2 = currentX; temp.y2 = currentY;
                    temp.color = currentColor;
                    temp.clipRegion = globalClipRegion;
                    DrawShape(hdc, temp);
                }
            }
            
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_ERASEBKGND:
            return 1;

        case WM_SETCURSOR:
            if (LOWORD(lParam) == HTCLIENT) {
                SetCursor(currentCursor);
                return TRUE;
            }
            return DefWindowProc(hwnd, msg, wParam, lParam);

        case WM_DESTROY:
            if (memDC) DeleteDC(memDC);
            if (memBitmap) DeleteObject(memBitmap);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    std::cout << "Graphics Project Console Initialized.\n";
    std::cout << "Select actions from the menu.\n";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "GraphicsProjectClass";
    wc.lpszMenuName = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindow("GraphicsProjectClass", "2D Drawing Package", 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, 
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}