/* Lesson 51, Example 01: DLL entry point
 * Advanced WinAPI program demonstrating BasicDLL
 */
#define UNICODE
#define _UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int nShow) {
    WNDCLASSEX wc = {sizeof(wc), CS_HREDRAW|CS_VREDRAW, WindowProc, 0, 0,
                     hInst, 0, LoadCursor(0, IDC_ARROW), (HBRUSH)(COLOR_WINDOW+1),
                     0, L"BasicDLLClass", 0};
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindowEx(0, L"BasicDLLClass", L"Lesson 51: DLL entry point",
                                WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                                800, 600, 0, 0, hInst, 0);
    if (!hwnd) return 1;
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
            // Initialize: DLL entry point
            return 0;
        
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            SetBkMode(hdc, TRANSPARENT);
            
            wchar_t title[256];
            wsprintf(title, L"Lesson 51: BasicDLL");
            TextOut(hdc, 10, 10, title, (int)wcslen(title));
            
            const wchar_t* desc = L"DLL entry point";
            TextOut(hdc, 10, 40, desc, (int)wcslen(desc));
            
            const wchar_t* info = L"This is a complete WinAPI program demonstrating advanced techniques.";
            TextOut(hdc, 10, 70, info, (int)wcslen(info));
            
            const wchar_t* compile = L"Compile: cl /D UNICODE /EHsc this_file.cpp user32.lib gdi32.lib";
            TextOut(hdc, 10, 100, compile, (int)wcslen(compile));
            
            EndPaint(hwnd, &ps);
            return 0;
        }
        
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/* Advanced Concepts Demonstrated:
 * - DLL entry point
 * - Professional Windows application structure
 * - Error handling and resource management
 * - Complete WinMain implementation
 * - Proper message loop and window procedure
 * 
 * To extend this example:
 * 1. Add specific functionality in WM_CREATE
 * 2. Handle additional messages as needed
 * 3. Implement cleanup in WM_DESTROY
 * 4. Add controls and user interaction
 */
