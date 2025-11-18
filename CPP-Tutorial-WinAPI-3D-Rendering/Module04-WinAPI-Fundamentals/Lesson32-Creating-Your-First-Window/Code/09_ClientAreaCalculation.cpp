/*
 * Lesson 32, Example 09: Client Area Calculation
 *
 * This program demonstrates the difference between window size and client area,
 * and how to calculate desired client area size including non-client elements.
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 09_ClientAreaCalculation.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 09_ClientAreaCalculation.cpp -o 09_ClientAreaCalculation.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND CreateWindowWithClientSize(HINSTANCE hInstance, const wchar_t* className,
                                  const wchar_t* title, DWORD style, DWORD exStyle,
                                  int x, int y, int clientWidth, int clientHeight);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"ClientAreaClass";

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

    // Create window with EXACT client area of 800x600
    // (window size will be larger to accommodate borders, title bar, etc.)
    HWND hwnd = CreateWindowWithClientSize(
        hInstance,
        CLASS_NAME,
        L"Client Area Calculation - Exact 800x600 Client Area",
        WS_OVERLAPPEDWINDOW,
        0,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600  // Desired CLIENT area size
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

HWND CreateWindowWithClientSize(HINSTANCE hInstance, const wchar_t* className,
                                  const wchar_t* title, DWORD style, DWORD exStyle,
                                  int x, int y, int clientWidth, int clientHeight)
{
    /*
     * This function creates a window with an exact client area size.
     * It calculates the required window size including non-client area.
     */

    // Start with desired client area
    RECT rc = { 0, 0, clientWidth, clientHeight };

    // Adjust rectangle to include non-client area (borders, title bar, etc.)
    AdjustWindowRectEx(&rc, style, FALSE, exStyle);

    // Calculate total window size
    int windowWidth = rc.right - rc.left;
    int windowHeight = rc.bottom - rc.top;

    // Create window with calculated size
    return CreateWindowEx(
        exStyle,
        className,
        title,
        style,
        x, y,
        windowWidth, windowHeight,
        NULL,
        NULL,
        hInstance,
        NULL
    );
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Get window and client rectangles
            RECT rcWindow, rcClient;
            GetWindowRect(hwnd, &rcWindow);
            GetClientRect(hwnd, &rcClient);

            // Calculate sizes
            int windowWidth = rcWindow.right - rcWindow.left;
            int windowHeight = rcWindow.bottom - rcWindow.top;
            int clientWidth = rcClient.right - rcClient.left;
            int clientHeight = rcClient.bottom - rcClient.top;

            // Calculate non-client area sizes
            int borderWidth = (windowWidth - clientWidth) / 2;
            int titleBarHeight = windowHeight - clientHeight - borderWidth * 2;

            wchar_t buffer[256];
            int y = 10;

            const wchar_t* title = L"Window Size vs Client Area Size:";
            TextOut(hdc, 10, y, title, (int)wcslen(title));
            y += 30;

            // Window dimensions
            const wchar_t* winTitle = L"Total Window Size (includes borders, title bar):";
            TextOut(hdc, 10, y, winTitle, (int)wcslen(winTitle));
            y += 25;

            wsprintf(buffer, L"  Width:  %d pixels", windowWidth);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            wsprintf(buffer, L"  Height: %d pixels", windowHeight);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 30;

            // Client area dimensions
            const wchar_t* clientTitle = L"Client Area Size (drawable area):";
            TextOut(hdc, 10, y, clientTitle, (int)wcslen(clientTitle));
            y += 25;

            wsprintf(buffer, L"  Width:  %d pixels", clientWidth);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            wsprintf(buffer, L"  Height: %d pixels", clientHeight);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 30;

            // Non-client area breakdown
            const wchar_t* ncTitle = L"Non-Client Area (borders, title bar):";
            TextOut(hdc, 10, y, ncTitle, (int)wcslen(ncTitle));
            y += 25;

            wsprintf(buffer, L"  Border Width: %d pixels", borderWidth);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            wsprintf(buffer, L"  Title Bar Height: %d pixels", titleBarHeight);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            wsprintf(buffer, L"  Total Width Overhead: %d pixels", windowWidth - clientWidth);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            wsprintf(buffer, L"  Total Height Overhead: %d pixels", windowHeight - clientHeight);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 35;

            const wchar_t* info1 = L"This window was created using CreateWindowWithClientSize()";
            TextOut(hdc, 10, y, info1, (int)wcslen(info1));
            y += 20;

            const wchar_t* info2 = L"which uses AdjustWindowRectEx() to calculate the correct";
            TextOut(hdc, 10, y, info2, (int)wcslen(info2));
            y += 20;

            const wchar_t* info3 = L"window size for an exact 800x600 client area.";
            TextOut(hdc, 10, y, info3, (int)wcslen(info3));
            y += 30;

            const wchar_t* note = L"Note: Client area is what you can draw on. Window size includes";
            TextOut(hdc, 10, y, note, (int)wcslen(note));
            y += 20;

            const wchar_t* note2 = L"title bar, borders, and other non-client elements.";
            TextOut(hdc, 10, y, note2, (int)wcslen(note2));

            // Draw a border around client area to visualize it
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

            Rectangle(hdc, 0, 0, clientWidth, clientHeight);

            SelectObject(hdc, hOldPen);
            SelectObject(hdc, hOldBrush);
            DeleteObject(hPen);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_SIZE:
        {
            // Repaint when resized
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
 * Client Area vs Window Size - Detailed Explanation:
 *
 * Window Components:
 *   ┌─────────────────────────────────┐
 *   │ Title Bar                       │ ← Non-client area
 *   ├─────────────────────────────────┤
 *   │ ┌─────────────────────────────┐ │
 *   │ │                             │ │
 *   │ │    Client Area              │ │ ← Drawable area
 *   │ │    (your content)           │ │
 *   │ │                             │ │
 *   │ └─────────────────────────────┘ │
 *   └─────────────────────────────────┘
 *   ↑ Border (non-client area)
 *
 * GetWindowRect:
 *   Returns window rectangle in screen coordinates.
 *   Includes all non-client area (title bar, borders, menu).
 *
 * GetClientRect:
 *   Returns client rectangle in window coordinates.
 *   Always starts at (0, 0).
 *   This is the drawable area.
 *
 * AdjustWindowRect / AdjustWindowRectEx:
 *   Calculates required window size for desired client area.
 *
 *   RECT rc = { 0, 0, desiredWidth, desiredHeight };
 *   AdjustWindowRectEx(&rc, style, hasMenu, exStyle);
 *
 *   Result:
 *   - rc.left, rc.top: Usually negative (border extends outward)
 *   - rc.right, rc.bottom: Width + border + title bar
 *   - Window size = rc.right - rc.left, rc.bottom - rc.top
 *
 * System Metrics:
 *   SM_CXBORDER / SM_CYBORDER: Window border size
 *   SM_CXFRAME / SM_CYFRAME: Resizable window border
 *   SM_CYCAPTION: Title bar height
 *   SM_CYMENU: Menu bar height
 *
 * Why This Matters:
 *   1. If you want 800x600 drawable area, window must be larger
 *   2. Different styles have different border sizes
 *   3. Important for precise layout and design
 *   4. Critical when positioning child windows
 *
 * Common Mistakes:
 *   - Creating 800x600 window and expecting 800x600 drawable area
 *   - Forgetting to account for title bar and borders
 *   - Not using AdjustWindowRect for exact client sizes
 *
 * Best Practice:
 *   Always use AdjustWindowRectEx when you need exact client dimensions!
 */
