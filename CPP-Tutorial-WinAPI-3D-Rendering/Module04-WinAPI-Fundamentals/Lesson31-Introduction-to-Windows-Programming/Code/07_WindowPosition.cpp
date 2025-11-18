/*
 * Lesson 31, Example 07: Window Position and Size
 *
 * This program demonstrates:
 * - Setting window position and size at creation
 * - Moving windows with SetWindowPos and MoveWindow
 * - Getting window position and size
 * - Handling WM_SIZE and WM_MOVE messages
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 07_WindowPosition.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 07_WindowPosition.cpp -o 07_WindowPosition.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_KEYDOWN:
        {
            // Get current window position and size
            RECT rc;
            GetWindowRect(hwnd, &rc);

            int width = rc.right - rc.left;
            int height = rc.bottom - rc.top;
            int x = rc.left;
            int y = rc.top;

            switch (wParam)
            {
                case VK_LEFT:
                    // Move window left
                    SetWindowPos(hwnd, NULL, x - 10, y, 0, 0,
                                SWP_NOSIZE | SWP_NOZORDER);
                    break;

                case VK_RIGHT:
                    // Move window right
                    SetWindowPos(hwnd, NULL, x + 10, y, 0, 0,
                                SWP_NOSIZE | SWP_NOZORDER);
                    break;

                case VK_UP:
                    // Move window up
                    SetWindowPos(hwnd, NULL, x, y - 10, 0, 0,
                                SWP_NOSIZE | SWP_NOZORDER);
                    break;

                case VK_DOWN:
                    // Move window down
                    SetWindowPos(hwnd, NULL, x, y + 10, 0, 0,
                                SWP_NOSIZE | SWP_NOZORDER);
                    break;

                case VK_ADD:      // Numpad +
                case VK_OEM_PLUS: // Regular +
                    // Increase size
                    SetWindowPos(hwnd, NULL, 0, 0, width + 20, height + 20,
                                SWP_NOMOVE | SWP_NOZORDER);
                    break;

                case VK_SUBTRACT:  // Numpad -
                case VK_OEM_MINUS: // Regular -
                    // Decrease size
                    if (width > 100 && height > 100)
                    {
                        SetWindowPos(hwnd, NULL, 0, 0, width - 20, height - 20,
                                    SWP_NOMOVE | SWP_NOZORDER);
                    }
                    break;

                case 'C':
                    // Center window on screen
                    {
                        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
                        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
                        int centerX = (screenWidth - width) / 2;
                        int centerY = (screenHeight - height) / 2;

                        MoveWindow(hwnd, centerX, centerY, width, height, TRUE);
                    }
                    break;

                case 'R':
                    // Reset to default size
                    MoveWindow(hwnd, 100, 100, 600, 400, TRUE);
                    break;
            }
            return 0;
        }

        case WM_SIZE:
        {
            // Window has been resized
            // lParam contains new size
            int newWidth = LOWORD(lParam);   // New width
            int newHeight = HIWORD(lParam);  // New height

            // wParam indicates the type of resizing
            switch (wParam)
            {
                case SIZE_RESTORED:
                    // Window restored to normal size
                    break;
                case SIZE_MINIMIZED:
                    // Window minimized
                    break;
                case SIZE_MAXIMIZED:
                    // Window maximized
                    break;
            }

            // Force repaint to update display
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }

        case WM_MOVE:
        {
            // Window has been moved
            // lParam contains new position
            int newX = (int)(short)LOWORD(lParam);  // New X position
            int newY = (int)(short)HIWORD(lParam);  // New Y position

            // Force repaint to update display
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Display instructions
            const wchar_t* info1 = L"Arrow Keys: Move window";
            const wchar_t* info2 = L"+/- Keys: Change size";
            const wchar_t* info3 = L"C: Center on screen";
            const wchar_t* info4 = L"R: Reset position and size";

            TextOut(hdc, 10, 10, info1, (int)wcslen(info1));
            TextOut(hdc, 10, 30, info2, (int)wcslen(info2));
            TextOut(hdc, 10, 50, info3, (int)wcslen(info3));
            TextOut(hdc, 10, 70, info4, (int)wcslen(info4));

            // Get and display window position (screen coordinates)
            RECT windowRect;
            GetWindowRect(hwnd, &windowRect);

            wchar_t posText[200];
            swprintf_s(posText, 200,
                      L"Window Position (screen): X=%d, Y=%d, Width=%d, Height=%d",
                      windowRect.left, windowRect.top,
                      windowRect.right - windowRect.left,
                      windowRect.bottom - windowRect.top);
            TextOut(hdc, 10, 100, posText, (int)wcslen(posText));

            // Get and display client area size
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);

            wchar_t clientText[200];
            swprintf_s(clientText, 200,
                      L"Client Area: Width=%d, Height=%d",
                      clientRect.right, clientRect.bottom);
            TextOut(hdc, 10, 120, clientText, (int)wcslen(clientText));

            // Display screen size
            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
            int screenHeight = GetSystemMetrics(SM_CYSCREEN);

            wchar_t screenText[200];
            swprintf_s(screenText, 200,
                      L"Screen Size: Width=%d, Height=%d",
                      screenWidth, screenHeight);
            TextOut(hdc, 10, 140, screenText, (int)wcslen(screenText));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"WindowPositionClass";

    WNDCLASS wc = { };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    // Create window with specific position and size
    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME,
        L"Window Position and Size Demo",
        WS_OVERLAPPEDWINDOW,
        100, 100,    // X, Y position (screen coordinates)
        600, 300,    // Width, Height
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

/*
 * Key Concepts Demonstrated:
 *
 * 1. Creating Window with Position/Size:
 *    CreateWindowEx(..., x, y, width, height, ...)
 *    - x, y: Screen coordinates (pixels from top-left of screen)
 *    - width, height: Size in pixels
 *    - CW_USEDEFAULT: Let Windows choose
 *
 * 2. SetWindowPos:
 *    SetWindowPos(hwnd, hwndAfter, x, y, cx, cy, flags)
 *    - Most versatile function for window positioning
 *    - Flags control what changes:
 *      SWP_NOSIZE: Don't change size
 *      SWP_NOMOVE: Don't change position
 *      SWP_NOZORDER: Don't change Z-order
 *      SWP_SHOWWINDOW: Show window
 *      SWP_HIDEWINDOW: Hide window
 *
 * 3. MoveWindow:
 *    MoveWindow(hwnd, x, y, width, height, repaint)
 *    - Simpler alternative to SetWindowPos
 *    - Changes both position and size
 *    - Last parameter: TRUE = repaint, FALSE = don't repaint
 *
 * 4. Getting Window Position:
 *    - GetWindowRect: Gets window rectangle in screen coordinates
 *    - GetClientRect: Gets client area in client coordinates (always 0,0 origin)
 *
 * 5. RECT Structure:
 *    typedef struct tagRECT {
 *        LONG left;
 *        LONG top;
 *        LONG right;   // Note: right edge, not width!
 *        LONG bottom;  // Note: bottom edge, not height!
 *    } RECT;
 *
 *    Width = right - left
 *    Height = bottom - top
 *
 * 6. WM_SIZE Message:
 *    - Sent when window is resized
 *    - lParam: LOWORD = new width, HIWORD = new height
 *    - wParam: SIZE_RESTORED, SIZE_MINIMIZED, SIZE_MAXIMIZED
 *
 * 7. WM_MOVE Message:
 *    - Sent when window is moved
 *    - lParam: LOWORD = new x, HIWORD = new y
 *    - Position is of client area's top-left corner
 *
 * 8. System Metrics:
 *    GetSystemMetrics(index) returns system information
 *    - SM_CXSCREEN: Screen width
 *    - SM_CYSCREEN: Screen height
 *    - SM_CXFULLSCREEN: Full-screen window width
 *    - SM_CYFULLSCREEN: Full-screen window height
 *    - Many more available
 *
 * Coordinate Systems:
 *
 * 1. Screen Coordinates:
 *    - Origin at top-left of screen
 *    - GetWindowRect returns screen coordinates
 *
 * 2. Client Coordinates:
 *    - Origin at top-left of window's client area
 *    - GetClientRect returns client coordinates
 *    - Client area excludes title bar and borders
 *
 * Converting Coordinates:
 * - ClientToScreen: Convert client to screen coords
 * - ScreenToClient: Convert screen to client coords
 *
 * Advanced Positioning:
 *
 * Cascade windows:
 * CascadeWindows(NULL, 0, NULL, count, hwndArray);
 *
 * Tile windows:
 * TileWindows(NULL, MDITILE_HORIZONTAL, NULL, count, hwndArray);
 *
 * Adjust for client size:
 * RECT rc = { 0, 0, desiredWidth, desiredHeight };
 * AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
 * int windowWidth = rc.right - rc.left;
 * int windowHeight = rc.bottom - rc.top;
 */
