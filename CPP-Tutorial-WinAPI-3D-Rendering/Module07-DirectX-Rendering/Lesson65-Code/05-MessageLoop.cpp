/*
 * DirectX 11 Tutorial - Lesson 65
 * Example 05: Message Loop
 *
 * Demonstrates proper message loop implementation for DirectX applications.
 * This version uses PeekMessage for game-style rendering loop.
 */

#include <windows.h>
#include <string>
#include <chrono>

HWND g_hWnd = nullptr;
bool g_isRunning = true;
LARGE_INTEGER g_frequency;
LARGE_INTEGER g_startTime;
UINT g_frameCount = 0;

// Get elapsed time in seconds
double GetTime()
{
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    return static_cast<double>(currentTime.QuadPart - g_startTime.QuadPart) / g_frequency.QuadPart;
}

// Window procedure
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        g_isRunning = false;
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            g_isRunning = false;
            PostQuitMessage(0);
        }
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            RECT rect;
            GetClientRect(hWnd, &rect);

            // Dark background
            HBRUSH bgBrush = CreateSolidBrush(RGB(30, 30, 30));
            FillRect(hdc, &rect, bgBrush);
            DeleteObject(bgBrush);

            // Display frame info
            SetTextColor(hdc, RGB(0, 255, 0));
            SetBkMode(hdc, TRANSPARENT);

            wchar_t buffer[256];
            wsprintf(buffer, L"Frames: %u\nTime: %.2f s\nFPS: %.1f",
                    g_frameCount, GetTime(),
                    g_frameCount / (GetTime() + 0.001));

            DrawText(hdc, buffer, -1, &rect, DT_CENTER | DT_VCENTER);

            EndPaint(hWnd, &ps);
        }
        return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    // Initialize timer
    QueryPerformanceFrequency(&g_frequency);
    QueryPerformanceCounter(&g_startTime);

    // Register window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"DirectXMessageLoop";

    RegisterClassEx(&wc);

    // Create window
    RECT rect = { 0, 0, 640, 480 };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    g_hWnd = CreateWindowEx(0, wc.lpszClassName, L"DirectX Message Loop",
                           WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                           rect.right - rect.left, rect.bottom - rect.top,
                           nullptr, nullptr, hInstance, nullptr);

    ShowWindow(g_hWnd, SW_SHOW);

    // Game loop with PeekMessage
    MSG msg = { 0 };
    while (g_isRunning)
    {
        // Process all pending messages
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                g_isRunning = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (g_isRunning)
        {
            // Update and render (in real app, this would be DirectX rendering)
            g_frameCount++;
            InvalidateRect(g_hWnd, nullptr, FALSE);
        }
    }

    return static_cast<int>(msg.wParam);
}

/*
 * COMPILATION: cl.exe 05-MessageLoop.cpp /link user32.lib gdi32.lib /SUBSYSTEM:WINDOWS
 *
 * GetMessage vs PeekMessage:
 * -------------------------
 * GetMessage: Blocks until message arrives (good for event-driven apps)
 * PeekMessage: Returns immediately (good for games/DirectX apps)
 *
 * Game Loop Pattern:
 * - Process all pending messages
 * - Update game state
 * - Render frame
 * - Repeat
 */
