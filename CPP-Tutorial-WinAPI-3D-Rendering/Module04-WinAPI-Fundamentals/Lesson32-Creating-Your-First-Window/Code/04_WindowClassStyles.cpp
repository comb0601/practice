/*
 * Lesson 32, Example 04: Window Class Styles
 *
 * This program demonstrates window class styles (CS_*) and their effects.
 * Class styles affect all windows created from that class.
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 04_WindowClassStyles.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 04_WindowClassStyles.cpp -o 04_WindowClassStyles.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"ClassStylesDemo";

    WNDCLASSEX wcex = { };
    wcex.cbSize = sizeof(WNDCLASSEX);

    // Try different class style combinations:

    // STYLE 1: Redraw on resize (most common)
    wcex.style = CS_HREDRAW | CS_VREDRAW;

    // STYLE 2: Enable double-clicks
    // wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

    // STYLE 3: Each window gets own device context (for OpenGL, etc.)
    // wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    // STYLE 4: All windows share one device context
    // wcex.style = CS_HREDRAW | CS_VREDRAW | CS_CLASSDC;

    // STYLE 5: Drop shadow (Windows XP+, popup windows only)
    // wcex.style = CS_DROPSHADOW;

    // STYLE 6: Disable close button
    // wcex.style = CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE;

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

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Window Class Styles Demo",
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
    static int resizeCount = 0;
    static int clickCount = 0;
    static int doubleClickCount = 0;

    switch (uMsg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Get class information
            WNDCLASSEX wc = { };
            wc.cbSize = sizeof(WNDCLASSEX);
            GetClassInfoEx(GetModuleHandle(NULL), L"ClassStylesDemo", &wc);

            wchar_t buffer[256];
            int y = 10;

            const wchar_t* title = L"Window Class Styles:";
            TextOut(hdc, 10, y, title, (int)wcslen(title));
            y += 30;

            if (wc.style & CS_HREDRAW)
            {
                const wchar_t* text = L"  CS_HREDRAW - Redraw on horizontal resize";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (wc.style & CS_VREDRAW)
            {
                const wchar_t* text = L"  CS_VREDRAW - Redraw on vertical resize";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (wc.style & CS_DBLCLKS)
            {
                const wchar_t* text = L"  CS_DBLCLKS - Enable double-click messages";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (wc.style & CS_OWNDC)
            {
                const wchar_t* text = L"  CS_OWNDC - Each window has own device context";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (wc.style & CS_CLASSDC)
            {
                const wchar_t* text = L"  CS_CLASSDC - All windows share one device context";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (wc.style & CS_DROPSHADOW)
            {
                const wchar_t* text = L"  CS_DROPSHADOW - Window has drop shadow";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (wc.style & CS_NOCLOSE)
            {
                const wchar_t* text = L"  CS_NOCLOSE - Close button disabled";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            y += 10;
            wsprintf(buffer, L"Class Style Value (Hex): 0x%08X", wc.style);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 30;

            // Show resize count (demonstrates CS_HREDRAW/CS_VREDRAW)
            const wchar_t* statTitle = L"Statistics:";
            TextOut(hdc, 10, y, statTitle, (int)wcslen(statTitle));
            y += 25;

            wsprintf(buffer, L"  Resize count: %d", resizeCount);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            wsprintf(buffer, L"  Click count: %d", clickCount);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            wsprintf(buffer, L"  Double-click count: %d", doubleClickCount);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 30;

            const wchar_t* info = L"Try resizing the window to see CS_HREDRAW/CS_VREDRAW in action!";
            TextOut(hdc, 10, y, info, (int)wcslen(info));
            y += 20;

            const wchar_t* info2 = L"Try clicking/double-clicking to test CS_DBLCLKS!";
            TextOut(hdc, 10, y, info2, (int)wcslen(info2));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_SIZE:
        {
            // Increment resize count
            resizeCount++;

            // Force repaint to show new count
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            clickCount++;
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }

        case WM_LBUTTONDBLCLK:
        {
            // This message only arrives if CS_DBLCLKS is set
            doubleClickCount++;
            InvalidateRect(hwnd, NULL, TRUE);

            MessageBox(hwnd,
                      L"Double-click detected!\n\n"
                      L"This message only arrives because CS_DBLCLKS is set in the window class.",
                      L"Double-Click",
                      MB_OK | MB_ICONINFORMATION);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/*
 * Window Class Styles Explained:
 *
 * CS_HREDRAW / CS_VREDRAW:
 *   Forces entire window to redraw when width/height changes.
 *   Without these: Only newly exposed areas are redrawn.
 *   Use when: Window content depends on size (charts, layouts).
 *
 * CS_DBLCLKS:
 *   Enables WM_LBUTTONDBLCLK, WM_RBUTTONDBLCLK, WM_MBUTTONDBLCLK.
 *   Without this: Double-clicks generate two WM_LBUTTONDOWN messages.
 *   Use when: Need to detect double-clicks.
 *
 * CS_OWNDC:
 *   Each window gets its own private device context.
 *   DC settings persist across calls.
 *   Use for: OpenGL windows, windows with customized DC.
 *   Warning: Uses more memory per window.
 *
 * CS_CLASSDC:
 *   All windows of this class share one device context.
 *   Use when: All windows are drawn identically.
 *   Warning: Can cause conflicts if windows draw differently.
 *
 * CS_DROPSHADOW:
 *   Adds drop shadow effect (Windows XP+).
 *   Only works with WS_POPUP windows.
 *   Use for: Menus, tooltips, popup windows.
 *
 * CS_NOCLOSE:
 *   Disables close button in system menu.
 *   User can't close window via X button.
 *   Use for: Windows that must not be closed by user.
 *
 * CS_SAVEBITS:
 *   Saves screen area under window to restore later.
 *   Use for: Small, temporary windows like menus.
 *   Note: Rarely needed on modern systems.
 *
 * CS_PARENTDC:
 *   Child windows use parent's device context.
 *   Use for: Optimization in complex window hierarchies.
 *
 * Class styles are set once during registration and affect
 * all windows created from that class!
 */
