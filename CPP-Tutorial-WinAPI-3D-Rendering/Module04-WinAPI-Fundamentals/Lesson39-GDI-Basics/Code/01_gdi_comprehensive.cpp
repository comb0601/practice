/*
 * Lesson 39: GDI Basics - Comprehensive Graphics Example
 *
 * This example demonstrates fundamental GDI operations including:
 * - Drawing shapes (lines, rectangles, ellipses, polygons)
 * - Using pens and brushes
 * - Drawing text with different fonts
 * - Working with colors
 *
 * Compilation (MinGW): g++ -std=c++17 01_gdi_comprehensive.cpp -o gdi_demo.exe -lgdi32 -luser32 -mwindows
 * Compilation (MSVC): cl 01_gdi_comprehensive.cpp /Fe:gdi_demo.exe user32.lib gdi32.lib
 */

#include <windows.h>
#include <string>
#include <cmath>

// Constants
const char g_szClassName[] = "GDIComprehensiveClass";
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 700;

// Forward declarations
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void DrawShapes(HDC hdc);
void DrawLines(HDC hdc, int x, int y);
void DrawRectangles(HDC hdc, int x, int y);
void DrawEllipses(HDC hdc, int x, int y);
void DrawPolygons(HDC hdc, int x, int y);
void DrawText(HDC hdc, int x, int y);
void DrawGradient(HDC hdc, int x, int y, int width, int height);

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc = {0};
    HWND hwnd;
    MSG msg;

    // Register window class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpszClassName = g_szClassName;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "GDI Basics - Comprehensive Graphics Demo",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Draw all graphics
            DrawShapes(hdc);

            EndPaint(hwnd, &ps);
            break;
        }

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Main drawing function
void DrawShapes(HDC hdc)
{
    // Set background mode for text
    SetBkMode(hdc, TRANSPARENT);

    // Draw title
    HFONT hTitleFont = CreateFont(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                   DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                                   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                   DEFAULT_PITCH | FF_SWISS, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    SetTextColor(hdc, RGB(0, 0, 128));
    TextOut(hdc, 20, 10, "GDI Graphics Demonstration", 27);
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);

    // Draw different shapes in sections
    DrawLines(hdc, 20, 60);
    DrawRectangles(hdc, 20, 180);
    DrawEllipses(hdc, 20, 340);
    DrawPolygons(hdc, 350, 60);
    DrawText(hdc, 350, 340);
    DrawGradient(hdc, 700, 60, 250, 150);
}

// Draw various lines
void DrawLines(HDC hdc, int x, int y)
{
    // Section title
    HFONT hFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                             DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                             DEFAULT_PITCH | FF_SWISS, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    TextOut(hdc, x, y, "Lines", 5);
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);

    y += 25;

    // Solid line
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    MoveToEx(hdc, x, y, NULL);
    LineTo(hdc, x + 200, y);
    TextOut(hdc, x + 210, y - 5, "Solid", 5);

    // Dashed line
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
    hPen = CreatePen(PS_DASH, 1, RGB(0, 255, 0));
    SelectObject(hdc, hPen);
    MoveToEx(hdc, x, y + 20, NULL);
    LineTo(hdc, x + 200, y + 20);
    TextOut(hdc, x + 210, y + 15, "Dashed", 6);

    // Dotted line
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
    hPen = CreatePen(PS_DOT, 1, RGB(0, 0, 255));
    SelectObject(hdc, hPen);
    MoveToEx(hdc, x, y + 40, NULL);
    LineTo(hdc, x + 200, y + 40);
    TextOut(hdc, x + 210, y + 35, "Dotted", 6);

    // Thick line
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
    hPen = CreatePen(PS_SOLID, 5, RGB(255, 128, 0));
    SelectObject(hdc, hPen);
    MoveToEx(hdc, x, y + 60, NULL);
    LineTo(hdc, x + 200, y + 60);
    TextOut(hdc, x + 210, y + 55, "Thick (5px)", 11);

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

// Draw various rectangles
void DrawRectangles(HDC hdc, int x, int y)
{
    // Section title
    HFONT hFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                             DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                             DEFAULT_PITCH | FF_SWISS, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    TextOut(hdc, x, y, "Rectangles", 10);
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);

    y += 25;

    // Outlined rectangle
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, x, y, x + 100, y + 60);
    TextOut(hdc, x, y + 65, "Outlined", 8);

    // Filled rectangle
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0));
    SelectObject(hdc, hBrush);
    Rectangle(hdc, x + 120, y, x + 220, y + 60);
    TextOut(hdc, x + 120, y + 65, "Filled", 6);
    DeleteObject(hBrush);

    // Rounded rectangle
    hBrush = CreateSolidBrush(RGB(0, 128, 255));
    SelectObject(hdc, hBrush);
    RoundRect(hdc, x + 240, y, x + 340, y + 60, 20, 20);
    TextOut(hdc, x + 240, y + 65, "Rounded", 7);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}

// Draw various ellipses and circles
void DrawEllipses(HDC hdc, int x, int y)
{
    // Section title
    HFONT hFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                             DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                             DEFAULT_PITCH | FF_SWISS, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    TextOut(hdc, x, y, "Ellipses & Circles", 18);
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);

    y += 25;

    // Circle
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 255));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 200, 200));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Ellipse(hdc, x, y, x + 80, y + 80);
    TextOut(hdc, x + 10, y + 85, "Circle", 6);

    // Ellipse
    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
    hBrush = CreateSolidBrush(RGB(200, 255, 200));
    SelectObject(hdc, hBrush);
    Ellipse(hdc, x + 120, y, x + 240, y + 80);
    TextOut(hdc, x + 150, y + 85, "Ellipse", 7);

    // Pie
    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
    hBrush = CreateSolidBrush(RGB(200, 200, 255));
    SelectObject(hdc, hBrush);
    Pie(hdc, x + 270, y, x + 350, y + 80, x + 310, y, x + 310, y + 80);
    TextOut(hdc, x + 280, y + 85, "Pie", 3);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}

// Draw polygons and complex shapes
void DrawPolygons(HDC hdc, int x, int y)
{
    // Section title
    HFONT hFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                             DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                             DEFAULT_PITCH | FF_SWISS, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    TextOut(hdc, x, y, "Polygons", 8);
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);

    y += 25;

    // Triangle
    POINT triangle[] = {{x + 50, y}, {x, y + 80}, {x + 100, y + 80}};
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(128, 0, 128));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 200));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Polygon(hdc, triangle, 3);
    TextOut(hdc, x + 20, y + 85, "Triangle", 8);

    // Pentagon
    POINT pentagon[5];
    int centerX = x + 180;
    int centerY = y + 40;
    int radius = 40;
    for (int i = 0; i < 5; ++i) {
        double angle = (i * 2 * 3.14159 / 5) - (3.14159 / 2);
        pentagon[i].x = centerX + (int)(radius * cos(angle));
        pentagon[i].y = centerY + (int)(radius * sin(angle));
    }
    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
    hBrush = CreateSolidBrush(RGB(200, 255, 255));
    SelectObject(hdc, hBrush);
    Polygon(hdc, pentagon, 5);
    TextOut(hdc, x + 140, y + 85, "Pentagon", 8);

    // Star
    POINT star[10];
    centerX = x + 280;
    centerY = y + 40;
    int outerRadius = 40;
    int innerRadius = 15;
    for (int i = 0; i < 10; ++i) {
        double angle = (i * 3.14159 / 5) - (3.14159 / 2);
        int r = (i % 2 == 0) ? outerRadius : innerRadius;
        star[i].x = centerX + (int)(r * cos(angle));
        star[i].y = centerY + (int)(r * sin(angle));
    }
    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
    hBrush = CreateSolidBrush(RGB(255, 255, 0));
    SelectObject(hdc, hBrush);
    Polygon(hdc, star, 10);
    TextOut(hdc, x + 260, y + 85, "Star", 4);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}

// Draw text with different fonts and styles
void DrawText(HDC hdc, int x, int y)
{
    // Section title
    HFONT hFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                             DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                             DEFAULT_PITCH | FF_SWISS, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    TextOut(hdc, x, y, "Text Rendering", 14);
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);

    y += 30;

    // Different fonts and styles
    const char* texts[] = {"Normal Text", "Bold Text", "Italic Text", "Large Text"};
    int weights[] = {FW_NORMAL, FW_BOLD, FW_NORMAL, FW_NORMAL};
    BOOL italics[] = {FALSE, FALSE, TRUE, FALSE};
    int sizes[] = {16, 16, 16, 24};
    COLORREF colors[] = {RGB(0, 0, 0), RGB(255, 0, 0), RGB(0, 128, 0), RGB(0, 0, 255)};

    for (int i = 0; i < 4; ++i) {
        hFont = CreateFont(sizes[i], 0, 0, 0, weights[i], italics[i], FALSE, FALSE,
                          DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                          CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                          DEFAULT_PITCH | FF_SWISS, "Arial");
        SelectObject(hdc, hFont);
        SetTextColor(hdc, colors[i]);
        TextOut(hdc, x, y, texts[i], strlen(texts[i]));
        y += sizes[i] + 10;
        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);
    }
}

// Draw a simple gradient
void DrawGradient(HDC hdc, int x, int y, int width, int height)
{
    // Section title
    HFONT hFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                             DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                             DEFAULT_PITCH | FF_SWISS, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    TextOut(hdc, x, y, "Gradient", 8);
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);

    y += 25;

    // Draw vertical gradient
    for (int i = 0; i < height; ++i) {
        int r = (255 * i) / height;
        int b = 255 - r;
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(r, 0, b));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        MoveToEx(hdc, x, y + i, NULL);
        LineTo(hdc, x + width, y + i);
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    }

    TextOut(hdc, x + 50, y + height + 10, "Vertical Gradient", 17);
}
