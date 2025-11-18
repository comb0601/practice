/*
 * Lesson 32, Example 05: WM_GETMINMAXINFO - Controlling Window Size
 *
 * This program demonstrates how to control minimum and maximum window size
 * using the WM_GETMINMAXINFO message.
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 05_MinMaxInfo.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 05_MinMaxInfo.cpp -o 05_MinMaxInfo.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"MinMaxInfoClass";

    WNDCLASSEX wcex = { };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = CLASS_NAME;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wcex);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Window Size Constraints (WM_GETMINMAXINFO)",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_OK);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_GETMINMAXINFO:
        {
            /*
             * WM_GETMINMAXINFO is sent when window size is being queried.
             * This happens during:
             * - Window creation
             * - Window resizing
             * - Maximizing/minimizing
             *
             * lParam points to MINMAXINFO structure.
             */

            MINMAXINFO* pInfo = (MINMAXINFO*)lParam;

            // Set minimum window size (400x300)
            pInfo->ptMinTrackSize.x = 400;
            pInfo->ptMinTrackSize.y = 300;

            // Set maximum window size (1200x900)
            pInfo->ptMaxTrackSize.x = 1200;
            pInfo->ptMaxTrackSize.y = 900;

            // Set maximized window size (same as max track size)
            pInfo->ptMaxSize.x = 1200;
            pInfo->ptMaxSize.y = 900;

            // Set maximized window position (top-left)
            pInfo->ptMaxPosition.x = 0;
            pInfo->ptMaxPosition.y = 0;

            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Get current window size
            RECT rc;
            GetClientRect(hwnd, &rc);

            wchar_t buffer[256];
            int y = 10;

            const wchar_t* title = L"Window Size Constraints with WM_GETMINMAXINFO";
            TextOut(hdc, 10, y, title, (int)wcslen(title));
            y += 30;

            const wchar_t* info1 = L"This window has the following size constraints:";
            TextOut(hdc, 10, y, info1, (int)wcslen(info1));
            y += 25;

            const wchar_t* info2 = L"  Minimum Size: 400 x 300 pixels";
            TextOut(hdc, 10, y, info2, (int)wcslen(info2));
            y += 20;

            const wchar_t* info3 = L"  Maximum Size: 1200 x 900 pixels";
            TextOut(hdc, 10, y, info3, (int)wcslen(info3));
            y += 30;

            const wchar_t* current = L"Current Client Area Size:";
            TextOut(hdc, 10, y, current, (int)wcslen(current));
            y += 25;

            wsprintf(buffer, L"  Width:  %d pixels", rc.right - rc.left);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            wsprintf(buffer, L"  Height: %d pixels", rc.bottom - rc.top);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 30;

            const wchar_t* try1 = L"Try to resize the window:";
            TextOut(hdc, 10, y, try1, (int)wcslen(try1));
            y += 25;

            const wchar_t* try2 = L"  - You cannot make it smaller than 400x300";
            TextOut(hdc, 10, y, try2, (int)wcslen(try2));
            y += 20;

            const wchar_t* try3 = L"  - You cannot make it larger than 1200x900";
            TextOut(hdc, 10, y, try3, (int)wcslen(try3));
            y += 20;

            const wchar_t* try4 = L"  - Maximizing will resize to 1200x900";
            TextOut(hdc, 10, y, try4, (int)wcslen(try4));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_SIZE:
        {
            // Repaint to show new size
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/*
 * MINMAXINFO Structure:
 *
 * typedef struct tagMINMAXINFO {
 *     POINT ptReserved;        // Reserved, do not use
 *     POINT ptMaxSize;         // Maximized size
 *     POINT ptMaxPosition;     // Maximized position
 *     POINT ptMinTrackSize;    // Minimum tracking size
 *     POINT ptMaxTrackSize;    // Maximum tracking size
 * } MINMAXINFO;
 *
 * ptMaxSize:
 *   Size when window is maximized.
 *   Usually set to screen size or custom limit.
 *
 * ptMaxPosition:
 *   Position when window is maximized.
 *   Usually (0, 0) for top-left of screen.
 *
 * ptMinTrackSize:
 *   Minimum size user can resize window to.
 *   Default is GetSystemMetrics(SM_CXMINTRACK/SM_CYMINTRACK).
 *
 * ptMaxTrackSize:
 *   Maximum size user can resize window to.
 *   Default is screen size.
 *
 * Common Use Cases:
 *
 * 1. Enforce minimum size (prevent too small):
 *    pInfo->ptMinTrackSize.x = 640;
 *    pInfo->ptMinTrackSize.y = 480;
 *
 * 2. Enforce maximum size (prevent too large):
 *    pInfo->ptMaxTrackSize.x = 1920;
 *    pInfo->ptMaxTrackSize.y = 1080;
 *
 * 3. Fixed-size window (non-resizable alternative):
 *    pInfo->ptMinTrackSize.x = 800;
 *    pInfo->ptMinTrackSize.y = 600;
 *    pInfo->ptMaxTrackSize.x = 800;
 *    pInfo->ptMaxTrackSize.y = 600;
 *
 * 4. Enforce aspect ratio (more complex):
 *    Calculate based on current dimensions to maintain ratio.
 *
 * 5. Custom maximized behavior:
 *    pInfo->ptMaxSize.x = desiredWidth;
 *    pInfo->ptMaxSize.y = desiredHeight;
 *    pInfo->ptMaxPosition.x = xPos;
 *    pInfo->ptMaxPosition.y = yPos;
 *
 * This message gives you complete control over window sizing behavior!
 */
