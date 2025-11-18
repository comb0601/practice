/*
 * Lesson 32, Example 07: Window Positioning and Sizing
 *
 * This program demonstrates different ways to position and size windows,
 * including centering on screen, specific positions, and working area.
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 07_WindowPositioning.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 07_WindowPositioning.cpp -o 07_WindowPositioning.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void CenterWindow(HWND hwnd);
void PositionWindow(HWND hwnd, int position);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"WindowPositioningClass";

    WNDCLASSEX wcex = { };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = CLASS_NAME;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wcex);

    // Create window with default position
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Window Positioning Demo",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,  // Let Windows decide initial position
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

    // Center the window on screen
    CenterWindow(hwnd);

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

void CenterWindow(HWND hwnd)
{
    // Get window rectangle
    RECT rcWindow;
    GetWindowRect(hwnd, &rcWindow);

    int windowWidth = rcWindow.right - rcWindow.left;
    int windowHeight = rcWindow.bottom - rcWindow.top;

    // Get screen dimensions
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Calculate centered position
    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;

    // Move window to center
    SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void PositionWindow(HWND hwnd, int position)
{
    RECT rcWindow;
    GetWindowRect(hwnd, &rcWindow);
    int windowWidth = rcWindow.right - rcWindow.left;
    int windowHeight = rcWindow.bottom - rcWindow.top;

    // Get working area (screen minus taskbar)
    RECT rcWork;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0);
    int workWidth = rcWork.right - rcWork.left;
    int workHeight = rcWork.bottom - rcWork.top;

    int x, y;

    switch (position)
    {
        case 0:  // Top-Left
            x = rcWork.left;
            y = rcWork.top;
            break;

        case 1:  // Top-Right
            x = rcWork.right - windowWidth;
            y = rcWork.top;
            break;

        case 2:  // Bottom-Left
            x = rcWork.left;
            y = rcWork.bottom - windowHeight;
            break;

        case 3:  // Bottom-Right
            x = rcWork.right - windowWidth;
            y = rcWork.bottom - windowHeight;
            break;

        case 4:  // Center
            x = rcWork.left + (workWidth - windowWidth) / 2;
            y = rcWork.top + (workHeight - windowHeight) / 2;
            break;

        default:
            return;
    }

    SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
        {
            // Create buttons for positioning
            CreateWindowEx(
                0, L"BUTTON", L"Top-Left",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                10, 10, 120, 30,
                hwnd, (HMENU)1, GetModuleHandle(NULL), NULL
            );

            CreateWindowEx(
                0, L"BUTTON", L"Top-Right",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                140, 10, 120, 30,
                hwnd, (HMENU)2, GetModuleHandle(NULL), NULL
            );

            CreateWindowEx(
                0, L"BUTTON", L"Bottom-Left",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                270, 10, 120, 30,
                hwnd, (HMENU)3, GetModuleHandle(NULL), NULL
            );

            CreateWindowEx(
                0, L"BUTTON", L"Bottom-Right",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                400, 10, 120, 30,
                hwnd, (HMENU)4, GetModuleHandle(NULL), NULL
            );

            CreateWindowEx(
                0, L"BUTTON", L"Center",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                530, 10, 120, 30,
                hwnd, (HMENU)5, GetModuleHandle(NULL), NULL
            );

            return 0;
        }

        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            switch (wmId)
            {
                case 1:  // Top-Left
                    PositionWindow(hwnd, 0);
                    break;

                case 2:  // Top-Right
                    PositionWindow(hwnd, 1);
                    break;

                case 3:  // Bottom-Left
                    PositionWindow(hwnd, 2);
                    break;

                case 4:  // Bottom-Right
                    PositionWindow(hwnd, 3);
                    break;

                case 5:  // Center
                    PositionWindow(hwnd, 4);
                    break;
            }

            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Get current window position
            RECT rcWindow;
            GetWindowRect(hwnd, &rcWindow);

            wchar_t buffer[256];
            int y = 60;

            const wchar_t* title = L"Window Positioning Information:";
            TextOut(hdc, 10, y, title, (int)wcslen(title));
            y += 30;

            wsprintf(buffer, L"Window Position: X=%d, Y=%d", rcWindow.left, rcWindow.top);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            wsprintf(buffer, L"Window Size: %d x %d",
                     rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 30;

            // Screen metrics
            const wchar_t* screen = L"Screen Metrics:";
            TextOut(hdc, 10, y, screen, (int)wcslen(screen));
            y += 25;

            wsprintf(buffer, L"Screen Size: %d x %d",
                     GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            // Working area
            RECT rcWork;
            SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0);
            wsprintf(buffer, L"Working Area: %d x %d (excludes taskbar)",
                     rcWork.right - rcWork.left, rcWork.bottom - rcWork.top);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 30;

            const wchar_t* info1 = L"Click the buttons above to position this window on screen.";
            TextOut(hdc, 10, y, info1, (int)wcslen(info1));
            y += 20;

            const wchar_t* info2 = L"The window was initially centered using CenterWindow() function.";
            TextOut(hdc, 10, y, info2, (int)wcslen(info2));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_MOVE:
        {
            // Repaint when window moves
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
 * Window Positioning Techniques:
 *
 * 1. CW_USEDEFAULT:
 *    CreateWindowEx(..., CW_USEDEFAULT, CW_USEDEFAULT, ...);
 *    Lets Windows choose position (cascading from top-left).
 *
 * 2. Specific Position:
 *    CreateWindowEx(..., x, y, width, height, ...);
 *    Place window at exact screen coordinates.
 *
 * 3. Center on Screen:
 *    Calculate: x = (screenWidth - windowWidth) / 2
 *              y = (screenHeight - windowHeight) / 2
 *
 * 4. Position in Working Area:
 *    Use SystemParametersInfo(SPI_GETWORKAREA, ...) to get
 *    screen area excluding taskbar.
 *
 * 5. SetWindowPos:
 *    SetWindowPos(hwnd, NULL, x, y, width, height, flags);
 *    Move and/or resize window after creation.
 *    Flags:
 *      SWP_NOSIZE - Don't change size
 *      SWP_NOMOVE - Don't change position
 *      SWP_NOZORDER - Don't change Z-order
 *
 * 6. Multi-Monitor Support:
 *    Use MonitorFromWindow and GetMonitorInfo for
 *    multi-monitor setups.
 *
 * 7. System Metrics:
 *    SM_CXSCREEN / SM_CYSCREEN - Primary screen size
 *    SM_XVIRTUALSCREEN / SM_YVIRTUALSCREEN - Virtual screen (all monitors)
 *    SM_CXVIRTUALSCREEN / SM_CYVIRTUALSCREEN - Virtual screen size
 *
 * 8. Remember Position:
 *    Save position in WM_DESTROY, restore in WM_CREATE.
 *    Store in registry or config file.
 */
