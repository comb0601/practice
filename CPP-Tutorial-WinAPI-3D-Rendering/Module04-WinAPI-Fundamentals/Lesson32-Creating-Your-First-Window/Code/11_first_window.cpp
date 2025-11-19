/*
 * Creating Your First Window
 * Lesson 32 - Example 11
 *
 * This is a complete Windows API application demonstrating creating your first window.
 *
 * Compilation (MinGW): g++ -std=c++17 11_first_window.cpp -o example_11.exe -lgdi32 -luser32 -mwindows
 * Compilation (MSVC): cl 11_first_window.cpp /Fe:example_11.exe user32.lib gdi32.lib
 */

#include <windows.h>
#include <string>

// Global variables
const char g_szClassName[] = "MyWindowClass";
HINSTANCE g_hInst;

// Forward declarations
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// WinMain - Entry point for Windows applications
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    g_hInst = hInstance;
    
    // Step 1: Register the window class
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
        MessageBox(NULL, "Window Registration Failed!", "Error",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    
    // Step 2: Create the window
    HWND hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "Creating Your First Window - Example 11",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
        NULL, NULL, hInstance, NULL
    );
    
    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    // Step 3: The message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return msg.wParam;
}

// Window procedure - Handles messages for the window
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_CREATE:
            // Window created
            break;
            
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // Draw text
            const char* text = "Creating Your First Window - Example 11";
            TextOut(hdc, 10, 10, text, strlen(text));
            
            const char* info = "This demonstrates Creating Your First Window";
            TextOut(hdc, 10, 30, info, strlen(info));
            
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
