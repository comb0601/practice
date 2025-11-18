/*
 * Lesson 31, Example 12: Window Styles
 *
 * This program demonstrates:
 * - Different window styles (WS_*)
 * - Extended window styles (WS_EX_*)
 * - Creating different types of windows
 * - Combining style flags
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 12_WindowStyles.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 12_WindowStyles.cpp -o 12_WindowStyles.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            wchar_t title[100];
            GetWindowText(hwnd, title, 100);

            TextOut(hdc, 10, 10, title, (int)wcslen(title));

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
    const wchar_t CLASS_NAME[] = L"WindowStylesClass";

    WNDCLASS wc = { };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    /*
     * WS_OVERLAPPEDWINDOW - Standard window with title bar, system menu, and borders
     * This is actually a combination of several styles:
     * WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME |
     * WS_MINIMIZEBOX | WS_MAXIMIZEBOX
     */
    HWND hwnd1 = CreateWindowEx(
        0, CLASS_NAME,
        L"1. WS_OVERLAPPEDWINDOW (Standard Window)",
        WS_OVERLAPPEDWINDOW,
        50, 50, 400, 150,
        NULL, NULL, hInstance, NULL
    );

    /*
     * WS_POPUP - Popup window with no frame
     * Often used with WS_CAPTION for custom-styled windows
     */
    HWND hwnd2 = CreateWindowEx(
        0, CLASS_NAME,
        L"2. WS_POPUP (No frame)",
        WS_POPUP | WS_CAPTION,
        450, 50, 400, 150,
        NULL, NULL, hInstance, NULL
    );

    /*
     * WS_CHILD - Child window (must have parent)
     * Child windows are clipped to parent and move with parent
     */
    // Note: Child window needs a parent, so we'll use hwnd1
    HWND hwndChild = CreateWindowEx(
        0, CLASS_NAME,
        L"3. WS_CHILD (Child Window)",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        10, 40, 200, 80,
        hwnd1, NULL, hInstance, NULL  // hwnd1 is the parent
    );

    /*
     * Window without WS_MINIMIZEBOX - Can't be minimized
     */
    HWND hwnd3 = CreateWindowEx(
        0, CLASS_NAME,
        L"4. No Minimize Box",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX,
        50, 220, 400, 150,
        NULL, NULL, hInstance, NULL
    );

    /*
     * Window without WS_MAXIMIZEBOX - Can't be maximized
     */
    HWND hwnd4 = CreateWindowEx(
        0, CLASS_NAME,
        L"5. No Maximize Box",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,
        450, 220, 400, 150,
        NULL, NULL, hInstance, NULL
    );

    /*
     * Window without WS_THICKFRAME - Can't be resized
     */
    HWND hwnd5 = CreateWindowEx(
        0, CLASS_NAME,
        L"6. Not Resizable (No WS_THICKFRAME)",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
        50, 390, 400, 150,
        NULL, NULL, hInstance, NULL
    );

    /*
     * WS_EX_TOPMOST - Always on top window (extended style)
     * Extended styles are passed to the first parameter of CreateWindowEx
     */
    HWND hwnd6 = CreateWindowEx(
        WS_EX_TOPMOST,  // Extended style
        CLASS_NAME,
        L"7. WS_EX_TOPMOST (Always On Top)",
        WS_OVERLAPPEDWINDOW,
        450, 390, 400, 150,
        NULL, NULL, hInstance, NULL
    );

    /*
     * WS_EX_TOOLWINDOW - Tool window (small title bar)
     * Doesn't appear in taskbar
     */
    HWND hwnd7 = CreateWindowEx(
        WS_EX_TOOLWINDOW,
        CLASS_NAME,
        L"8. WS_EX_TOOLWINDOW (Tool Window)",
        WS_OVERLAPPEDWINDOW,
        50, 560, 400, 150,
        NULL, NULL, hInstance, NULL
    );

    /*
     * WS_EX_CLIENTEDGE - Sunken 3D border
     */
    HWND hwnd8 = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        CLASS_NAME,
        L"9. WS_EX_CLIENTEDGE (3D Sunken)",
        WS_OVERLAPPEDWINDOW,
        450, 560, 400, 150,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd1 || !hwnd2 || !hwnd3 || !hwnd4 || !hwnd5 ||
        !hwnd6 || !hwnd7 || !hwnd8)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    // Show all windows (except child, which is already visible)
    ShowWindow(hwnd1, nCmdShow);
    ShowWindow(hwnd2, nCmdShow);
    ShowWindow(hwnd3, nCmdShow);
    ShowWindow(hwnd4, nCmdShow);
    ShowWindow(hwnd5, nCmdShow);
    ShowWindow(hwnd6, nCmdShow);
    ShowWindow(hwnd7, nCmdShow);
    ShowWindow(hwnd8, nCmdShow);

    MessageBox(NULL,
              L"Multiple window styles demonstrated:\n\n"
              L"1. Standard window (WS_OVERLAPPEDWINDOW)\n"
              L"2. Popup window (WS_POPUP)\n"
              L"3. Child window (WS_CHILD) - inside window 1\n"
              L"4. No minimize button\n"
              L"5. No maximize button\n"
              L"6. Not resizable\n"
              L"7. Always on top (WS_EX_TOPMOST)\n"
              L"8. Tool window (WS_EX_TOOLWINDOW)\n"
              L"9. 3D sunken border (WS_EX_CLIENTEDGE)\n\n"
              L"Close the main window to exit.",
              L"Window Styles Demo",
              MB_OK | MB_ICONINFORMATION);

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
 * 1. Window Styles (WS_*):
 *    Passed to dwStyle parameter of CreateWindowEx
 *
 *    Common styles:
 *    - WS_OVERLAPPEDWINDOW: Standard application window
 *    - WS_POPUP: Popup window
 *    - WS_CHILD: Child window (requires parent)
 *    - WS_VISIBLE: Initially visible
 *    - WS_CAPTION: Has title bar
 *    - WS_SYSMENU: Has system menu (icon in title bar)
 *    - WS_THICKFRAME: Resizable border
 *    - WS_MINIMIZEBOX: Minimize button
 *    - WS_MAXIMIZEBOX: Maximize button
 *    - WS_HSCROLL: Horizontal scrollbar
 *    - WS_VSCROLL: Vertical scrollbar
 *    - WS_BORDER: Thin border
 *
 * 2. Extended Styles (WS_EX_*):
 *    Passed to dwExStyle parameter (first parameter) of CreateWindowEx
 *
 *    Common extended styles:
 *    - WS_EX_TOPMOST: Always on top
 *    - WS_EX_TOOLWINDOW: Tool window (small title, no taskbar)
 *    - WS_EX_CLIENTEDGE: Sunken 3D border
 *    - WS_EX_STATICEDGE: 3D border (not sunken)
 *    - WS_EX_WINDOWEDGE: Raised edge
 *    - WS_EX_LAYERED: Supports transparency/alpha
 *    - WS_EX_TRANSPARENT: Transparent for hit-testing
 *    - WS_EX_ACCEPTFILES: Accepts drag-and-drop files
 *    - WS_EX_NOACTIVATE: Doesn't activate when clicked
 *
 * 3. WS_OVERLAPPEDWINDOW Breakdown:
 *    #define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | \
 *                                  WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
 *
 *    Components:
 *    - WS_OVERLAPPED: Overlapped window
 *    - WS_CAPTION: Title bar
 *    - WS_SYSMENU: System menu
 *    - WS_THICKFRAME: Resizable border
 *    - WS_MINIMIZEBOX: Minimize button
 *    - WS_MAXIMIZEBOX: Maximize button
 *
 * 4. WS_POPUPWINDOW Breakdown:
 *    #define WS_POPUPWINDOW (WS_POPUP | WS_BORDER | WS_SYSMENU)
 *
 * 5. Combining Styles:
 *    Use bitwise OR (|) to combine styles:
 *    WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
 *
 * 6. Child Windows:
 *    - Must have WS_CHILD style
 *    - Must specify parent window handle
 *    - Clipped to parent's client area
 *    - Move and hide with parent
 *    - Coordinates relative to parent's client area
 *    - Usually include WS_VISIBLE (not shown by default)
 *
 * 7. Dialog-Style Windows:
 *    For non-resizable windows like dialogs:
 *    WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX
 *
 * 8. Borderless Windows:
 *    For borderless/frameless windows:
 *    WS_POPUP (with or without WS_VISIBLE)
 *
 * 9. Changing Styles After Creation:
 *    Use SetWindowLong/SetWindowLongPtr:
 *
 *    LONG style = GetWindowLong(hwnd, GWL_STYLE);
 *    style &= ~WS_MAXIMIZEBOX;  // Remove maximize box
 *    SetWindowLong(hwnd, GWL_STYLE, style);
 *    SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
 *                 SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
 *
 * 10. Common Window Type Recipes:
 *
 *     Standard Application:
 *     CreateWindowEx(0, ..., WS_OVERLAPPEDWINDOW, ...)
 *
 *     Fixed-Size Dialog:
 *     CreateWindowEx(0, ..., WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, ...)
 *
 *     Splash Screen:
 *     CreateWindowEx(0, ..., WS_POPUP | WS_BORDER, ...)
 *
 *     Tool Palette:
 *     CreateWindowEx(WS_EX_TOOLWINDOW, ..., WS_OVERLAPPEDWINDOW, ...)
 *
 *     Always-on-Top Window:
 *     CreateWindowEx(WS_EX_TOPMOST, ..., WS_OVERLAPPEDWINDOW, ...)
 *
 *     Layered/Transparent Window:
 *     CreateWindowEx(WS_EX_LAYERED, ..., WS_OVERLAPPEDWINDOW, ...)
 *     SetLayeredWindowAttributes(hwnd, 0, 128, LWA_ALPHA);  // 50% transparent
 *
 * 11. Style Flags Reference:
 *
 *     Window Appearance:
 *     - WS_CAPTION: Title bar
 *     - WS_BORDER: Thin border
 *     - WS_THICKFRAME: Resizable border
 *     - WS_DLGFRAME: Dialog frame (no size border)
 *
 *     Window Functionality:
 *     - WS_MINIMIZEBOX: Minimize button enabled
 *     - WS_MAXIMIZEBOX: Maximize button enabled
 *     - WS_SYSMENU: System menu (icon, close button)
 *     - WS_HSCROLL: Horizontal scrollbar
 *     - WS_VSCROLL: Vertical scrollbar
 *
 *     Window Type:
 *     - WS_OVERLAPPED: Overlapped window
 *     - WS_POPUP: Popup window
 *     - WS_CHILD: Child window
 *
 *     Window State:
 *     - WS_VISIBLE: Initially visible
 *     - WS_DISABLED: Initially disabled
 *     - WS_MINIMIZE: Initially minimized
 *     - WS_MAXIMIZE: Initially maximized
 *
 * Best Practices:
 * - Use WS_OVERLAPPEDWINDOW for standard windows
 * - Add WS_VISIBLE for child windows
 * - Remove WS_THICKFRAME for non-resizable windows
 * - Use WS_EX_TOPMOST sparingly (annoying if overused)
 * - Test window styles on different Windows versions
 */
