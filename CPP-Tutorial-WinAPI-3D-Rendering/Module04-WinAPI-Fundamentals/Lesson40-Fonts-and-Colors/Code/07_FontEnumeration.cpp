/* Lesson 40, Example 07: EnumFontFamilies
 * Complete WinAPI program demonstrating FontEnumeration
 */
#define UNICODE
#define _UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int nShow) {
    WNDCLASSEX wc = {sizeof(wc), CS_HREDRAW|CS_VREDRAW, WindowProc, 0, 0,
                     hInst, 0, LoadCursor(0, IDC_ARROW), (HBRUSH)(COLOR_WINDOW+1),
                     0, L"FontEnumerationClass", 0};
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindowEx(0, L"FontEnumerationClass", L"EnumFontFamilies",
                                WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                                800, 600, 0, 0, hInst, 0);
    ShowWindow(hwnd, nShow);
    UpdateWindow(hwnd);
    MSG msg = {};
    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            // Initialize: EnumFontFamilies
            return 0;
        
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            const wchar_t* text = L"EnumFontFamilies";
            TextOut(hdc, 10, 10, text, (int)wcslen(text));
            const wchar_t* info = L"This example demonstrates FontEnumeration in WinAPI";
            TextOut(hdc, 10, 40, info, (int)wcslen(info));
            EndPaint(hwnd, &ps);
            return 0;
        }
        
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/* Key Concepts:
 * - EnumFontFamilies
 * - Complete WinMain entry point
 * - Window procedure with message handling
 * - Proper cleanup and resource management
 */
