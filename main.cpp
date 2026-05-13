#include <windows.h>
#include <iostream>
#include <vector>
#include <cstdio>
#include "resource.h"
#include "core/Shape.h"
#include "io/FileIO.h"

// Global state
std::vector<Shape> shapes;
ShapeType currentShapeType = SHAPE_LINE_DDA;
COLORREF currentColor = RGB(0, 0, 0);
HCURSOR currentCursor = NULL;
bool usingCrossCursor = false;
HBRUSH bgBrush = NULL;

bool isDrawing = false;
int startX = 0, startY = 0;

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

            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hPrefMenu, "Preferences");
            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hLinesMenu, "Lines");

            SetMenu(hwnd, hMenu);
            break;
        }

        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case ID_FILE_CLEAR:
                    shapes.clear();
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
                case ID_LINE_DDA:
                    currentShapeType = SHAPE_LINE_DDA;
                    std::cout << "Selected DDA Line.\n";
                    break;
                case ID_LINE_MIDPOINT:
                    currentShapeType = SHAPE_LINE_MIDPOINT;
                    std::cout << "Selected Midpoint Line.\n";
                    break;
                case ID_LINE_PARAMETRIC:
                    currentShapeType = SHAPE_LINE_PARAMETRIC;
                    std::cout << "Selected Parametric Line.\n";
                    break;
            }
            break;

        case WM_LBUTTONDOWN:
            startX = LOWORD(lParam);
            startY = HIWORD(lParam);
            isDrawing = true;
            break;

        case WM_LBUTTONUP:
            if (isDrawing) {
                int endX = LOWORD(lParam);
                int endY = HIWORD(lParam);
                
                Shape s;
                s.type = currentShapeType;
                s.x1 = startX;
                s.y1 = startY;
                s.x2 = endX;
                s.y2 = endY;
                s.color = currentColor;
                
                shapes.push_back(s);
                
                HDC hdc = GetDC(hwnd);
                DrawShape(hdc, s);
                ReleaseDC(hwnd, hdc);
                
                isDrawing = false;
            }
            break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // Fill background
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, bgBrush);
            
            // Redraw shapes
            for (const auto& s : shapes) {
                DrawShape(hdc, s);
            }
            
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_ERASEBKGND:
            return 1; // Handled in WM_PAINT

        case WM_SETCURSOR:
            if (LOWORD(lParam) == HTCLIENT) {
                SetCursor(currentCursor);
                return TRUE;
            }
            return DefWindowProc(hwnd, msg, wParam, lParam);

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Setup Console
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    std::cout << "Graphics Project Console Initialized.\n";
    std::cout << "Select actions from the menu.\n";

    // Setup Window Class
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
