/*
 * Lesson 32, Example 03: Extended Window Styles
 *
 * This program demonstrates extended window styles (WS_EX_*).
 * Extended styles provide additional appearance and behavior options.
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 03_ExtendedStyles.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 03_ExtendedStyles.cpp -o 03_ExtendedStyles.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"ExtendedStylesClass";

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

    // Try different extended styles

    // EXTENDED STYLE 1: Sunken client edge (3D border)
    DWORD exStyle = WS_EX_CLIENTEDGE;
    const wchar_t* title = L"Sunken Border (WS_EX_CLIENTEDGE)";

    // EXTENDED STYLE 2: Always on top
    // DWORD exStyle = WS_EX_TOPMOST;
    // const wchar_t* title = L"Always On Top (WS_EX_TOPMOST)";

    // EXTENDED STYLE 3: Tool window (small title bar, not in taskbar)
    // DWORD exStyle = WS_EX_TOOLWINDOW;
    // const wchar_t* title = L"Tool Window (WS_EX_TOOLWINDOW)";

    // EXTENDED STYLE 4: Accepts drag-and-drop files
    // DWORD exStyle = WS_EX_ACCEPTFILES;
    // const wchar_t* title = L"Accepts Drag-Drop Files (WS_EX_ACCEPTFILES)";

    // EXTENDED STYLE 5: Transparent window (layered)
    // DWORD exStyle = WS_EX_LAYERED;
    // const wchar_t* title = L"Transparent Window (WS_EX_LAYERED)";

    // EXTENDED STYLE 6: Raised window edge
    // DWORD exStyle = WS_EX_WINDOWEDGE;
    // const wchar_t* title = L"Raised Border (WS_EX_WINDOWEDGE)";

    // EXTENDED STYLE 7: Context help button
    // DWORD exStyle = WS_EX_CONTEXTHELP;
    // const wchar_t* title = L"Context Help Button (WS_EX_CONTEXTHELP)";

    HWND hwnd = CreateWindowEx(
        exStyle,                        // Extended styles
        CLASS_NAME,
        title,
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

    // If using WS_EX_LAYERED, set transparency
    LONG_PTR exStyleCheck = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    if (exStyleCheck & WS_EX_LAYERED)
    {
        // Make window 70% opaque (179 out of 255)
        SetLayeredWindowAttributes(hwnd, 0, 179, LWA_ALPHA);
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

            LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);

            wchar_t buffer[256];
            int y = 10;

            const wchar_t* title = L"Current Extended Window Styles:";
            TextOut(hdc, 10, y, title, (int)wcslen(title));
            y += 30;

            if (exStyle & WS_EX_TOPMOST)
            {
                const wchar_t* text = L"  WS_EX_TOPMOST - Window stays on top";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (exStyle & WS_EX_TOOLWINDOW)
            {
                const wchar_t* text = L"  WS_EX_TOOLWINDOW - Tool window (not in taskbar)";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (exStyle & WS_EX_CLIENTEDGE)
            {
                const wchar_t* text = L"  WS_EX_CLIENTEDGE - Sunken 3D border";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (exStyle & WS_EX_WINDOWEDGE)
            {
                const wchar_t* text = L"  WS_EX_WINDOWEDGE - Raised border";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (exStyle & WS_EX_LAYERED)
            {
                const wchar_t* text = L"  WS_EX_LAYERED - Supports transparency";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (exStyle & WS_EX_ACCEPTFILES)
            {
                const wchar_t* text = L"  WS_EX_ACCEPTFILES - Accepts drag-drop files";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (exStyle & WS_EX_CONTEXTHELP)
            {
                const wchar_t* text = L"  WS_EX_CONTEXTHELP - Context help button in title bar";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            if (exStyle & WS_EX_APPWINDOW)
            {
                const wchar_t* text = L"  WS_EX_APPWINDOW - Forces taskbar button";
                TextOut(hdc, 10, y, text, (int)wcslen(text));
                y += 20;
            }

            y += 10;
            wsprintf(buffer, L"Extended Style Value (Hex): 0x%08X", (DWORD)exStyle);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 25;

            const wchar_t* info = L"Extended styles provide additional window appearance options.";
            TextOut(hdc, 10, y, info, (int)wcslen(info));
            y += 20;

            const wchar_t* info2 = L"Edit source code to try different extended style combinations!";
            TextOut(hdc, 10, y, info2, (int)wcslen(info2));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DROPFILES:
        {
            // Handle drag-and-drop files (if WS_EX_ACCEPTFILES is set)
            HDROP hDrop = (HDROP)wParam;

            UINT fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);

            wchar_t message[512];
            wsprintf(message, L"Files dropped: %u\n\nThis demonstrates WS_EX_ACCEPTFILES.", fileCount);
            MessageBox(hwnd, message, L"Drag and Drop", MB_OK | MB_ICONINFORMATION);

            DragFinish(hDrop);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/*
 * Extended Window Styles Explained:
 *
 * WS_EX_TOPMOST:
 *   Window stays above all non-topmost windows.
 *   Useful for tool palettes, notifications.
 *
 * WS_EX_TOOLWINDOW:
 *   Creates a tool window with:
 *   - Smaller title bar
 *   - Not shown in taskbar
 *   - Not included in Alt+Tab
 *
 * WS_EX_CLIENTEDGE:
 *   Sunken 3D border around client area.
 *   Common for edit controls and list boxes.
 *
 * WS_EX_WINDOWEDGE:
 *   Raised border edge.
 *   Creates a 3D "raised" effect.
 *
 * WS_EX_LAYERED:
 *   Enables transparency and alpha blending.
 *   Use SetLayeredWindowAttributes to set opacity.
 *
 * WS_EX_ACCEPTFILES:
 *   Window accepts drag-and-drop files.
 *   Receives WM_DROPFILES messages.
 *
 * WS_EX_CONTEXTHELP:
 *   Adds "?" button in title bar.
 *   Cannot be used with WS_MINIMIZEBOX or WS_MAXIMIZEBOX.
 *
 * WS_EX_APPWINDOW:
 *   Forces a top-level window to appear in taskbar.
 *   Overrides default behavior for child windows.
 *
 * WS_EX_NOACTIVATE:
 *   Window doesn't become active when clicked.
 *   Useful for tooltips and palettes.
 */
