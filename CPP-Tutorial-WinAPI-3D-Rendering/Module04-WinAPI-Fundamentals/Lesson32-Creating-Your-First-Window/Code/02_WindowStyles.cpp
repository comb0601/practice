/*
 * Lesson 32, Example 02: Window Styles
 *
 * This program demonstrates different window styles and combinations.
 * You can uncomment different style configurations to see the effects.
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 02_WindowStyles.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 02_WindowStyles.cpp -o 02_WindowStyles.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"WindowStylesClass";

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

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error", MB_OK);
        return 1;
    }

    // Try different window style combinations

    // STYLE 1: Standard overlapped window (default)
    DWORD style1 = WS_OVERLAPPEDWINDOW;
    const wchar_t* title1 = L"Standard Window (WS_OVERLAPPEDWINDOW)";

    // STYLE 2: Non-resizable window
    // DWORD style1 = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    // const wchar_t* title1 = L"Non-Resizable Window (No WS_THICKFRAME)";

    // STYLE 3: Dialog-style window
    // DWORD style1 = WS_POPUP | WS_CAPTION | WS_SYSMENU;
    // const wchar_t* title1 = L"Dialog-Style Window (WS_POPUP | WS_CAPTION | WS_SYSMENU)";

    // STYLE 4: Borderless window
    // DWORD style1 = WS_POPUP;
    // const wchar_t* title1 = L"Borderless Window (WS_POPUP only)";

    // STYLE 5: Window with scrollbars
    // DWORD style1 = WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL;
    // const wchar_t* title1 = L"Window with Scrollbars";

    // STYLE 6: Initially maximized
    // DWORD style1 = WS_OVERLAPPEDWINDOW | WS_MAXIMIZE;
    // const wchar_t* title1 = L"Initially Maximized Window";

    // STYLE 7: Initially minimized
    // DWORD style1 = WS_OVERLAPPEDWINDOW | WS_MINIMIZE;
    // const wchar_t* title1 = L"Initially Minimized Window";

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        title1,
        style1,
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
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Get current window style
            LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);

            // Display current styles
            wchar_t buffer[1024];
            int y = 10;

            const wchar_t* title = L"Current Window Styles:";
            TextOut(hdc, 10, y, title, (int)wcslen(title));
            y += 30;

            if (style & WS_OVERLAPPED)
            {
                const wchar_t* text = L"  WS_OVERLAPPED - Overlapped window";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (style & WS_POPUP)
            {
                const wchar_t* text = L"  WS_POPUP - Popup window";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (style & WS_CAPTION)
            {
                const wchar_t* text = L"  WS_CAPTION - Has title bar";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (style & WS_SYSMENU)
            {
                const wchar_t* text = L"  WS_SYSMENU - Has system menu";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (style & WS_THICKFRAME)
            {
                const wchar_t* text = L"  WS_THICKFRAME - Resizable border";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (style & WS_MINIMIZEBOX)
            {
                const wchar_t* text = L"  WS_MINIMIZEBOX - Has minimize button";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (style & WS_MAXIMIZEBOX)
            {
                const wchar_t* text = L"  WS_MAXIMIZEBOX - Has maximize button";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (style & WS_HSCROLL)
            {
                const wchar_t* text = L"  WS_HSCROLL - Has horizontal scrollbar";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (style & WS_VSCROLL)
            {
                const wchar_t* text = L"  WS_VSCROLL - Has vertical scrollbar";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            y += 10;
            wsprintf(buffer, L"Style Value (Hex): 0x%08X", (DWORD)style);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 25;

            const wchar_t* info = L"Edit the source code and uncomment different style combinations to experiment!";
            TextOut(hdc, 10, y, info, (int)wcslen(info));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/*
 * Window Style Combinations Explained:
 *
 * WS_OVERLAPPEDWINDOW:
 *   = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME |
 *     WS_MINIMIZEBOX | WS_MAXIMIZEBOX
 *   Standard application window with all features.
 *
 * Non-Resizable Window:
 *   Remove WS_THICKFRAME to prevent resizing.
 *   Keep other elements for standard appearance.
 *
 * Dialog-Style Window:
 *   WS_POPUP creates a window without default borders.
 *   Add WS_CAPTION and WS_SYSMENU for title bar and close button.
 *   Typically used for dialog boxes.
 *
 * Borderless Window:
 *   WS_POPUP only - no border, no title bar, no system menu.
 *   Useful for splash screens, custom-shaped windows.
 *
 * Tool Window:
 *   WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME
 *   Combined with WS_EX_TOOLWINDOW extended style.
 *
 * Experiment with different combinations to understand how
 * window styles affect appearance and behavior!
 */
