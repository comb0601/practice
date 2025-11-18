/*
 * Lesson 32, Example 11: Layered Window (Transparency)
 *
 * This program demonstrates creating transparent/translucent windows using
 * the WS_EX_LAYERED extended style and SetLayeredWindowAttributes.
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 11_LayeredWindow.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 11_LayeredWindow.cpp -o 11_LayeredWindow.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Transparency level (0 = fully transparent, 255 = fully opaque)
int g_alphaLevel = 192;  // 75% opaque

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"LayeredWindowClass";

    WNDCLASSEX wcex = { };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(100, 150, 200));  // Blue background
    wcex.lpszClassName = CLASS_NAME;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wcex);

    // Create layered window
    HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED,  // Layered window enables transparency
        CLASS_NAME,
        L"Layered Window - Transparency Demo",
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

    // Set transparency level
    // Parameters:
    //   hwnd: Window handle
    //   crKey: Transparent color key (not used here, set to 0)
    //   bAlpha: Alpha value (0-255, 0=transparent, 255=opaque)
    //   dwFlags: LWA_ALPHA to use alpha, LWA_COLORKEY for color key
    SetLayeredWindowAttributes(hwnd, 0, g_alphaLevel, LWA_ALPHA);

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
            // Create controls to adjust transparency
            CreateWindowEx(
                0, L"STATIC", L"Transparency Level:",
                WS_CHILD | WS_VISIBLE | SS_LEFT,
                10, 10, 150, 20,
                hwnd, NULL, GetModuleHandle(NULL), NULL
            );

            // Trackbar for transparency control
            CreateWindowEx(
                0, TRACKBAR_CLASS, NULL,
                WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS,
                10, 35, 300, 30,
                hwnd, (HMENU)1, GetModuleHandle(NULL), NULL
            );

            // Set trackbar range and position
            HWND hTrack = GetDlgItem(hwnd, 1);
            SendMessage(hTrack, TBM_SETRANGE, TRUE, MAKELONG(0, 255));
            SendMessage(hTrack, TBM_SETPOS, TRUE, g_alphaLevel);
            SendMessage(hTrack, TBM_SETTICFREQ, 32, 0);

            // Buttons
            CreateWindowEx(
                0, L"BUTTON", L"Fully Opaque (255)",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                10, 75, 140, 30,
                hwnd, (HMENU)2, GetModuleHandle(NULL), NULL
            );

            CreateWindowEx(
                0, L"BUTTON", L"50% Transparent (128)",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                160, 75, 160, 30,
                hwnd, (HMENU)3, GetModuleHandle(NULL), NULL
            );

            CreateWindowEx(
                0, L"BUTTON", L"Mostly Transparent (64)",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                330, 75, 170, 30,
                hwnd, (HMENU)4, GetModuleHandle(NULL), NULL
            );

            return 0;
        }

        case WM_HSCROLL:
        {
            // Handle trackbar changes
            if ((HWND)lParam == GetDlgItem(hwnd, 1))
            {
                HWND hTrack = GetDlgItem(hwnd, 1);
                g_alphaLevel = (int)SendMessage(hTrack, TBM_GETPOS, 0, 0);

                // Update window transparency
                SetLayeredWindowAttributes(hwnd, 0, g_alphaLevel, LWA_ALPHA);

                // Force repaint to update display
                InvalidateRect(hwnd, NULL, TRUE);
            }
            return 0;
        }

        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            switch (wmId)
            {
                case 2:  // Fully Opaque
                    g_alphaLevel = 255;
                    break;

                case 3:  // 50% Transparent
                    g_alphaLevel = 128;
                    break;

                case 4:  // Mostly Transparent
                    g_alphaLevel = 64;
                    break;

                default:
                    return DefWindowProc(hwnd, uMsg, wParam, lParam);
            }

            // Update trackbar and window
            HWND hTrack = GetDlgItem(hwnd, 1);
            SendMessage(hTrack, TBM_SETPOS, TRUE, g_alphaLevel);
            SetLayeredWindowAttributes(hwnd, 0, g_alphaLevel, LWA_ALPHA);
            InvalidateRect(hwnd, NULL, TRUE);

            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(255, 255, 255));  // White text

            wchar_t buffer[256];
            int y = 120;

            const wchar_t* title = L"Layered Window (WS_EX_LAYERED)";
            TextOut(hdc, 10, y, title, (int)wcslen(title));
            y += 30;

            wsprintf(buffer, L"Current Transparency Level: %d / 255 (%d%% opaque)",
                     g_alphaLevel, (g_alphaLevel * 100) / 255);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 30;

            const wchar_t* info1 = L"Layered Window Features:";
            TextOut(hdc, 10, y, info1, (int)wcslen(info1));
            y += 25;

            const wchar_t* info2 = L"  - Alpha transparency (0-255)";
            TextOut(hdc, 10, y, info2, (int)wcslen(info2));
            y += 20;

            const wchar_t* info3 = L"  - Color key transparency (make specific color transparent)";
            TextOut(hdc, 10, y, info3, (int)wcslen(info3));
            y += 20;

            const wchar_t* info4 = L"  - Can be combined with UpdateLayeredWindow for per-pixel alpha";
            TextOut(hdc, 10, y, info4, (int)wcslen(info4));
            y += 30;

            const wchar_t* usage = L"Common Uses:";
            TextOut(hdc, 10, y, usage, (int)wcslen(usage));
            y += 25;

            const wchar_t* use1 = L"  - Splash screens with transparency";
            TextOut(hdc, 10, y, use1, (int)wcslen(use1));
            y += 20;

            const wchar_t* use2 = L"  - Fading windows in/out";
            TextOut(hdc, 10, y, use2, (int)wcslen(use2));
            y += 20;

            const wchar_t* use3 = L"  - On-screen displays (OSD)";
            TextOut(hdc, 10, y, use3, (int)wcslen(use3));
            y += 20;

            const wchar_t* use4 = L"  - Custom-shaped windows (with color key)";
            TextOut(hdc, 10, y, use4, (int)wcslen(use4));
            y += 20;

            const wchar_t* use5 = L"  - Overlay windows";
            TextOut(hdc, 10, y, use5, (int)wcslen(use5));

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
 * Layered Windows (WS_EX_LAYERED) - Detailed Explanation:
 *
 * What is a Layered Window?
 *   A window that supports transparency and alpha blending.
 *   Introduced in Windows 2000.
 *
 * SetLayeredWindowAttributes:
 *   BOOL SetLayeredWindowAttributes(
 *       HWND hwnd,          // Window handle
 *       COLORREF crKey,     // Transparent color key
 *       BYTE bAlpha,        // Alpha value (0-255)
 *       DWORD dwFlags       // Flags
 *   );
 *
 *   dwFlags:
 *     LWA_ALPHA: Use bAlpha for transparency
 *     LWA_COLORKEY: Make crKey color transparent
 *     LWA_ALPHA | LWA_COLORKEY: Use both
 *
 * Alpha Transparency (LWA_ALPHA):
 *   - 0: Fully transparent (invisible)
 *   - 128: 50% transparent
 *   - 255: Fully opaque (normal)
 *
 *   Example:
 *   SetLayeredWindowAttributes(hwnd, 0, 192, LWA_ALPHA);  // 75% opaque
 *
 * Color Key Transparency (LWA_COLORKEY):
 *   Makes specific color fully transparent.
 *
 *   Example - Make magenta transparent:
 *   SetLayeredWindowAttributes(hwnd, RGB(255, 0, 255), 0, LWA_COLORKEY);
 *
 *   Use for: Custom-shaped windows (irregular shapes).
 *
 * UpdateLayeredWindow:
 *   For advanced per-pixel alpha blending.
 *   Allows complex effects and custom shapes with smooth edges.
 *   More complex but more powerful.
 *
 * Fade In/Out Effect:
 *   for (int alpha = 0; alpha <= 255; alpha += 5)
 *   {
 *       SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
 *       Sleep(10);  // Delay for smooth effect
 *   }
 *
 * Performance Considerations:
 *   - Layered windows use more resources
 *   - Desktop composition required for best performance
 *   - May be slower on older systems
 *   - Avoid frequent updates if possible
 *
 * Limitations:
 *   - Cannot have child windows with WS_CHILD style
 *   - Some GDI operations may not work as expected
 *   - Window must have WS_EX_LAYERED style
 *
 * Common Use Cases:
 *   1. Splash screens with transparency
 *   2. Fading windows in/out
 *   3. Semi-transparent overlay windows
 *   4. Custom-shaped windows (using color key)
 *   5. On-screen displays (OSD)
 *   6. Notification windows
 *
 * Best Practices:
 *   - Set WS_EX_LAYERED before creating window
 *   - Call SetLayeredWindowAttributes after creation
 *   - Use reasonable alpha values (too transparent = hard to see)
 *   - Consider accessibility (some users can't see transparent windows well)
 *   - Provide option to disable transparency
 */
