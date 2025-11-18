/*
 * Lesson 32, Example 10: Tool Window
 *
 * This program demonstrates creating a tool window with WS_EX_TOOLWINDOW.
 * Tool windows have a smaller title bar and don't appear in the taskbar.
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 10_ToolWindow.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 10_ToolWindow.cpp -o 10_ToolWindow.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"ToolWindowClass";

    WNDCLASSEX wcex = { };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wcex.lpszClassName = CLASS_NAME;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wcex);

    // Create tool window with WS_EX_TOOLWINDOW and WS_EX_TOPMOST
    HWND hwnd = CreateWindowEx(
        WS_EX_TOOLWINDOW | WS_EX_TOPMOST,     // Tool window + always on top
        CLASS_NAME,
        L"Tool Palette",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME,  // No minimize/maximize
        100, 100,
        300, 400,
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
        case WM_CREATE:
        {
            // Create some sample buttons for the tool palette
            CreateWindowEx(
                0, L"BUTTON", L"Tool 1",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                10, 10, 120, 30,
                hwnd, (HMENU)1, GetModuleHandle(NULL), NULL
            );

            CreateWindowEx(
                0, L"BUTTON", L"Tool 2",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                140, 10, 120, 30,
                hwnd, (HMENU)2, GetModuleHandle(NULL), NULL
            );

            CreateWindowEx(
                0, L"BUTTON", L"Tool 3",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                10, 50, 120, 30,
                hwnd, (HMENU)3, GetModuleHandle(NULL), NULL
            );

            CreateWindowEx(
                0, L"BUTTON", L"Tool 4",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                140, 50, 120, 30,
                hwnd, (HMENU)4, GetModuleHandle(NULL), NULL
            );

            return 0;
        }

        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            wchar_t message[256];
            wsprintf(message, L"You clicked Tool %d!", wmId);
            MessageBox(hwnd, message, L"Tool Clicked", MB_OK | MB_ICONINFORMATION);
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            int y = 100;

            const wchar_t* title = L"Tool Window Features:";
            TextOut(hdc, 10, y, title, (int)wcslen(title));
            y += 30;

            const wchar_t* feature1 = L"1. Smaller title bar";
            TextOut(hdc, 10, y, feature1, (int)wcslen(feature1));
            y += 20;

            const wchar_t* feature2 = L"2. Does not appear in taskbar";
            TextOut(hdc, 10, y, feature2, (int)wcslen(feature2));
            y += 20;

            const wchar_t* feature3 = L"3. Not shown in Alt+Tab list";
            TextOut(hdc, 10, y, feature3, (int)wcslen(feature3));
            y += 20;

            const wchar_t* feature4 = L"4. Always on top (WS_EX_TOPMOST)";
            TextOut(hdc, 10, y, feature4, (int)wcslen(feature4));
            y += 20;

            const wchar_t* feature5 = L"5. No minimize/maximize buttons";
            TextOut(hdc, 10, y, feature5, (int)wcslen(feature5));
            y += 30;

            const wchar_t* usage = L"Common Uses:";
            TextOut(hdc, 10, y, usage, (int)wcslen(usage));
            y += 25;

            const wchar_t* use1 = L"  - Tool palettes";
            TextOut(hdc, 10, y, use1, (int)wcslen(use1));
            y += 20;

            const wchar_t* use2 = L"  - Floating toolbars";
            TextOut(hdc, 10, y, use2, (int)wcslen(use2));
            y += 20;

            const wchar_t* use3 = L"  - Inspector windows";
            TextOut(hdc, 10, y, use3, (int)wcslen(use3));
            y += 20;

            const wchar_t* use4 = L"  - Property panels";
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

/*
 * WS_EX_TOOLWINDOW - Tool Window Extended Style:
 *
 * Characteristics:
 *   1. Smaller title bar (uses smaller caption font)
 *   2. Not shown in taskbar
 *   3. Not included in Alt+Tab window list
 *   4. Typically combined with WS_EX_TOPMOST
 *   5. Usually no minimize/maximize buttons
 *
 * Typical Style Combination:
 *   Extended Style: WS_EX_TOOLWINDOW | WS_EX_TOPMOST
 *   Window Style: WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME
 *
 * When to Use:
 *   - Floating tool palettes
 *   - Auxiliary windows that support main window
 *   - Inspector/property windows
 *   - Color pickers
 *   - Find/Replace dialogs
 *   - Any utility window that shouldn't clutter taskbar
 *
 * Best Practices:
 *   1. Set owner window:
 *      CreateWindowEx(..., hwndParent, ...);
 *      This makes tool window float above owner.
 *
 *   2. Hide when owner is minimized:
 *      Handle WM_SIZE in owner window.
 *
 *   3. Close when owner closes:
 *      Destroy tool windows in owner's WM_DESTROY.
 *
 *   4. Save position:
 *      Remember tool window position between sessions.
 *
 * Comparison with Regular Windows:
 *
 *   Regular Window:
 *     - Full-size title bar
 *     - Appears in taskbar
 *     - Shows in Alt+Tab
 *     - Can have min/max buttons
 *     - Independent of other windows
 *
 *   Tool Window (WS_EX_TOOLWINDOW):
 *     - Small title bar
 *     - Hidden from taskbar
 *     - Hidden from Alt+Tab
 *     - Typically no min/max buttons
 *     - Usually owned by main window
 *
 * Alternative: Popup Windows
 *   For even simpler floating windows without title bar,
 *   use WS_POPUP instead of WS_OVERLAPPED.
 */
