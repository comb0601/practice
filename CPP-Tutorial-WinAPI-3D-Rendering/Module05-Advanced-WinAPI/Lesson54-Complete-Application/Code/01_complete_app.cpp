/*
 * Complete Windows Application
 * Lesson 54 - Example 1
 *
 * Advanced Windows API application demonstrating complete windows application.
 *
 * Compilation (MinGW): g++ -std=c++17 01_complete_app.cpp -o example_1.exe -lgdi32 -luser32 -lgdiplus -lole32 -loleaut32 -luuid -mwindows
 * Compilation (MSVC): cl 01_complete_app.cpp /Fe:example_1.exe user32.lib gdi32.lib gdiplus.lib ole32.lib oleaut32.lib
 */

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <gdiplus.h>
#include <string>

using namespace Gdiplus;

// Global variables
const wchar_t g_szClassName[] = L"AdvancedWindowClass";
HINSTANCE g_hInst;
ULONG_PTR g_gdiplusToken;

// Forward declarations
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// WinMain - Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    g_hInst = hInstance;
    
    // Initialize GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);
    
    // Register window class
    WNDCLASSEX wc = {0};
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = g_szClassName;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    
    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, L"Window Registration Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    
    // Create window
    HWND hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        L"Complete Windows Application - Example 1",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL
    );
    
    if (hwnd == NULL) {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    // Cleanup GDI+
    GdiplusShutdown(g_gdiplusToken);
    
    return msg.wParam;
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_CREATE:
            // Initialize window-specific data
            break;
            
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // Use GDI+ for advanced graphics
            Graphics graphics(hdc);
            
            // Draw title
            SolidBrush brush(Color(255, 0, 0, 0));
            FontFamily fontFamily(L"Arial");
            Font font(&fontFamily, 16, FontStyleBold, UnitPixel);
            PointF pointF(10.0f, 10.0f);
            graphics.DrawString(L"Complete Windows Application", -1, &font, pointF, &brush);
            
            // Draw example graphics
            Pen pen(Color(255, 0, 0, 255), 2);
            graphics.DrawRectangle(&pen, 50, 50, 200, 150);
            
            SolidBrush fillBrush(Color(128, 255, 0, 0));
            graphics.FillEllipse(&fillBrush, 300, 50, 200, 150);
            
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
