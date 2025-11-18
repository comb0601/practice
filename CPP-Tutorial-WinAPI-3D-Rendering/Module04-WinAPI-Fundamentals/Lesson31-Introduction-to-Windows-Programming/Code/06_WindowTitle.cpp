/*
 * Lesson 31, Example 06: Window Title
 *
 * This program demonstrates:
 * - Setting window title at creation
 * - Changing window title dynamically
 * - Getting window title
 * - Using SetWindowText and GetWindowText
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 06_WindowTitle.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 06_WindowTitle.cpp -o 06_WindowTitle.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>
#include <stdio.h>

int g_clickCount = 0;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
        {
            // WM_CREATE is sent when the window is being created
            // Good place for initialization
            SetWindowText(hwnd, L"Window Created! Click to change title");
            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            // Change title on each click
            g_clickCount++;

            wchar_t newTitle[100];
            swprintf_s(newTitle, 100, L"You clicked %d time(s)", g_clickCount);

            // SetWindowText changes the window title
            SetWindowText(hwnd, newTitle);

            return 0;
        }

        case WM_RBUTTONDOWN:
        {
            // Right click to get and display current title
            wchar_t title[100];

            // GetWindowText retrieves the window title
            // Returns the number of characters retrieved
            int length = GetWindowText(hwnd, title, 100);

            wchar_t message[200];
            swprintf_s(message, 200,
                      L"Current title: %s\n\nLength: %d characters",
                      title, length);

            MessageBox(hwnd, message, L"Window Title Info", MB_OK | MB_ICONINFORMATION);

            return 0;
        }

        case WM_KEYDOWN:
        {
            // Change title based on key pressed
            switch (wParam)
            {
                case VK_F1:
                    SetWindowText(hwnd, L"You pressed F1");
                    break;
                case VK_F2:
                    SetWindowText(hwnd, L"You pressed F2");
                    break;
                case VK_SPACE:
                    SetWindowText(hwnd, L"You pressed SPACE");
                    break;
                case VK_RETURN:
                    SetWindowText(hwnd, L"You pressed ENTER");
                    break;
                case 'R':
                    // Reset title
                    SetWindowText(hwnd, L"Title Reset - Click or press keys");
                    g_clickCount = 0;
                    break;
            }
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            const wchar_t* info1 = L"Left Click: Change title with click count";
            const wchar_t* info2 = L"Right Click: Display current title";
            const wchar_t* info3 = L"Press F1, F2, SPACE, ENTER: Change title";
            const wchar_t* info4 = L"Press R: Reset";

            TextOut(hdc, 10, 10, info1, (int)wcslen(info1));
            TextOut(hdc, 10, 30, info2, (int)wcslen(info2));
            TextOut(hdc, 10, 50, info3, (int)wcslen(info3));
            TextOut(hdc, 10, 70, info4, (int)wcslen(info4));

            // Get and display current title
            wchar_t currentTitle[100];
            GetWindowText(hwnd, currentTitle, 100);

            wchar_t display[150];
            swprintf_s(display, 150, L"Current title: \"%s\"", currentTitle);
            TextOut(hdc, 10, 100, display, (int)wcslen(display));

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
    const wchar_t CLASS_NAME[] = L"WindowTitleClass";

    WNDCLASS wc = { };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    // The window title is set in CreateWindowEx
    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME,
        L"Initial Window Title",  // This is the initial title
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 250,
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
 * 1. Setting Window Title:
 *    - At creation: Pass title string to CreateWindowEx
 *    - Dynamically: Use SetWindowText(hwnd, newTitle)
 *
 * 2. Getting Window Title:
 *    - GetWindowText(hwnd, buffer, bufferSize)
 *    - Returns number of characters retrieved
 *    - Buffer must be large enough to hold title + null terminator
 *
 * 3. WM_CREATE Message:
 *    - Sent when window is being created
 *    - Sent before CreateWindowEx returns
 *    - Good for initialization that requires hwnd
 *
 * 4. Window Title Uses:
 *    - Display application name
 *    - Show current document/file
 *    - Indicate application state
 *    - Provide user feedback
 *
 * 5. SetWindowText vs SetWindowTextA/W:
 *    - SetWindowText: Generic macro (A or W based on UNICODE)
 *    - SetWindowTextA: ANSI version (char*)
 *    - SetWindowTextW: Unicode version (wchar_t*)
 *
 * 6. Common Patterns:
 *    - "AppName - DocumentName"
 *    - "AppName - [Modified]"
 *    - "AppName - Status Message"
 *    - "Progress: 50%"
 *
 * Advanced Usage:
 *
 * Setting title with format string:
 * wchar_t title[100];
 * swprintf_s(title, 100, L"MyApp - %s", filename);
 * SetWindowText(hwnd, title);
 *
 * Getting title length:
 * int length = GetWindowTextLength(hwnd);
 * wchar_t* title = new wchar_t[length + 1];
 * GetWindowText(hwnd, title, length + 1);
 * // Use title
 * delete[] title;
 *
 * Flashing window title (for notifications):
 * FLASHWINFO fwi = { sizeof(FLASHWINFO) };
 * fwi.hwnd = hwnd;
 * fwi.dwFlags = FLASHW_CAPTION | FLASHW_TIMERNOFG;
 * fwi.uCount = 3;
 * FlashWindowEx(&fwi);
 *
 * Best Practices:
 * - Keep titles concise and informative
 * - Update title to reflect application state
 * - Use standard format: "AppName - Document"
 * - Consider accessibility (screen readers use titles)
 * - Don't update title too frequently (performance)
 */
