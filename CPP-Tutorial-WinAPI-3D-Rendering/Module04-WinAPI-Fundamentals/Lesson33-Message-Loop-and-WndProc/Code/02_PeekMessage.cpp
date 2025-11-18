/*
 * Lesson 33, Example 02: PeekMessage - Non-Blocking Message Loop
 *
 * Demonstrates PeekMessage for non-blocking message processing,
 * useful for games and animations that need continuous updates.
 */

#define UNICODE
#define _UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

DWORD g_frameCount = 0;
DWORD g_lastTime = 0;
float g_fps = 0.0f;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"PeekMessageClass";
    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = CLASS_NAME;

    RegisterClassEx(&wcex);
    HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"PeekMessage Demo",
                                WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                                800, 600, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);

    // Non-blocking message loop with PeekMessage
    MSG msg = {};
    BOOL running = TRUE;

    g_lastTime = GetTickCount();

    while (running)
    {
        // PeekMessage returns immediately, doesn't block
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                running = FALSE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            // No messages - do continuous processing
            // This is where games would update and render

            g_frameCount++;

            DWORD currentTime = GetTickCount();
            if (currentTime - g_lastTime >= 1000)
            {
                g_fps = g_frameCount * 1000.0f / (currentTime - g_lastTime);
                g_frameCount = 0;
                g_lastTime = currentTime;

                // Update display
                InvalidateRect(hwnd, NULL, FALSE);
            }

            // Optional: Sleep briefly to reduce CPU usage
            // Remove this for maximum frame rate
            Sleep(1);
        }
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            wchar_t buffer[256];
            int y = 10;

            const wchar_t* title = L"PeekMessage - Non-Blocking Loop:";
            TextOut(hdc, 10, y, title, (int)wcslen(title));
            y += 30;

            swprintf_s(buffer, L"FPS (Frames Per Second): %.2f", g_fps);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 30;

            const wchar_t* info1 = L"PeekMessage vs GetMessage:";
            TextOut(hdc, 10, y, info1, (int)wcslen(info1));
            y += 25;

            const wchar_t* info2 = L"  GetMessage: BLOCKS if no messages (low CPU, standard apps)";
            TextOut(hdc, 10, y, info2, (int)wcslen(info2));
            y += 20;

            const wchar_t* info3 = L"  PeekMessage: NEVER blocks (high CPU, games/animation)";
            TextOut(hdc, 10, y, info3, (int)wcslen(info3));
            y += 30;

            const wchar_t* usage = L"This loop continuously updates, allowing for:";
            TextOut(hdc, 10, y, usage, (int)wcslen(usage));
            y += 25;

            const wchar_t* use1 = L"  - Game loops";
            TextOut(hdc, 10, y, use1, (int)wcslen(use1));
            y += 20;

            const wchar_t* use2 = L"  - Animations";
            TextOut(hdc, 10, y, use2, (int)wcslen(use2));
            y += 20;

            const wchar_t* use3 = L"  - Real-time simulations";
            TextOut(hdc, 10, y, use3, (int)wcslen(use3));
            y += 20;

            const wchar_t* use4 = L"  - Continuous background processing";
            TextOut(hdc, 10, y, use4, (int)wcslen(use4));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
