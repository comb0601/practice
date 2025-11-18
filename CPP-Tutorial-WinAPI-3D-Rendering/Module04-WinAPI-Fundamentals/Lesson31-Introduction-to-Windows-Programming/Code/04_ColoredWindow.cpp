/*
 * Lesson 31, Example 04: Colored Window
 *
 * This program demonstrates:
 * - Setting window background color using hbrBackground
 * - Creating custom brushes
 * - System colors
 * - Creating colored windows
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 04_ColoredWindow.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 04_ColoredWindow.cpp -o 04_ColoredWindow.exe -mwindows -lgdi32 -luser32
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

            // The background color is already set by hbrBackground in WNDCLASS
            // We don't need to fill it manually, but we'll draw some text

            const wchar_t* message = L"This window has a custom background color!";
            TextOut(hdc, 10, 10, message, (int)wcslen(message));

            const wchar_t* info = L"The color is set using hbrBackground in WNDCLASS";
            TextOut(hdc, 10, 30, info, (int)wcslen(info));

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
    /*
     * Method 1: Using a system color
     * System colors are predefined colors that match the user's theme
     */

    const wchar_t CLASS_NAME1[] = L"SystemColorWindowClass";

    WNDCLASS wc1 = { };
    wc1.lpfnWndProc   = WindowProc;
    wc1.hInstance     = hInstance;
    wc1.lpszClassName = CLASS_NAME1;
    wc1.hCursor       = LoadCursor(NULL, IDC_ARROW);

    /*
     * hbrBackground sets the background brush
     * (HBRUSH)(COLOR_XXX + 1) uses a system color
     *
     * Common system colors:
     * COLOR_WINDOW      - Standard window background (usually white)
     * COLOR_WINDOWTEXT  - Standard window text color
     * COLOR_MENU        - Menu background
     * COLOR_MENUTEXT    - Menu text
     * COLOR_BTNFACE     - Button face color
     * COLOR_3DFACE      - 3D face color (same as BTNFACE)
     * COLOR_DESKTOP     - Desktop color
     * COLOR_HIGHLIGHT   - Highlight color
     */
    wc1.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);  // Standard window color

    RegisterClass(&wc1);

    /*
     * Method 2: Using a custom solid color brush
     * Create a brush with a specific RGB color
     */

    const wchar_t CLASS_NAME2[] = L"CustomColorWindowClass";

    WNDCLASS wc2 = { };
    wc2.lpfnWndProc   = WindowProc;
    wc2.hInstance     = hInstance;
    wc2.lpszClassName = CLASS_NAME2;
    wc2.hCursor       = LoadCursor(NULL, IDC_ARROW);

    /*
     * CreateSolidBrush creates a brush with a specific color
     * RGB(r, g, b) macro creates a COLORREF value
     * r, g, b range from 0-255
     */
    wc2.hbrBackground = CreateSolidBrush(RGB(173, 216, 230));  // Light blue

    RegisterClass(&wc2);

    /*
     * Method 3: Using stock objects
     * Windows provides some predefined GDI objects
     */

    const wchar_t CLASS_NAME3[] = L"StockBrushWindowClass";

    WNDCLASS wc3 = { };
    wc3.lpfnWndProc   = WindowProc;
    wc3.hInstance     = hInstance;
    wc3.lpszClassName = CLASS_NAME3;
    wc3.hCursor       = LoadCursor(NULL, IDC_ARROW);

    /*
     * GetStockObject returns a handle to a predefined GDI object
     *
     * Stock brushes:
     * WHITE_BRUSH   - White brush
     * LTGRAY_BRUSH  - Light gray brush
     * GRAY_BRUSH    - Gray brush
     * DKGRAY_BRUSH  - Dark gray brush
     * BLACK_BRUSH   - Black brush
     * NULL_BRUSH    - Transparent brush (no background)
     */
    wc3.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);  // Light gray

    RegisterClass(&wc3);

    // Create three windows with different colors

    // Window 1: System color (white)
    HWND hwnd1 = CreateWindowEx(
        0, CLASS_NAME1,
        L"System Color (COLOR_WINDOW)",
        WS_OVERLAPPEDWINDOW,
        100, 100, 400, 200,
        NULL, NULL, hInstance, NULL
    );

    // Window 2: Custom color (light blue)
    HWND hwnd2 = CreateWindowEx(
        0, CLASS_NAME2,
        L"Custom Color (Light Blue)",
        WS_OVERLAPPEDWINDOW,
        150, 150, 400, 200,
        NULL, NULL, hInstance, NULL
    );

    // Window 3: Stock object (light gray)
    HWND hwnd3 = CreateWindowEx(
        0, CLASS_NAME3,
        L"Stock Brush (Light Gray)",
        WS_OVERLAPPEDWINDOW,
        200, 200, 400, 200,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd1 || !hwnd2 || !hwnd3)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    ShowWindow(hwnd1, nCmdShow);
    ShowWindow(hwnd2, nCmdShow);
    ShowWindow(hwnd3, nCmdShow);

    // Message loop
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Clean up: Delete custom brush created with CreateSolidBrush
    // Note: System color brushes and stock objects should NOT be deleted
    DeleteObject(wc2.hbrBackground);

    return (int)msg.wParam;
}

/*
 * Key Concepts Demonstrated:
 *
 * 1. WNDCLASS.hbrBackground:
 *    - Sets the background brush for the window
 *    - Windows uses this to paint the background automatically
 *    - Can be system color, custom brush, or stock object
 *
 * 2. System Colors:
 *    - (HBRUSH)(COLOR_XXX + 1) syntax
 *    - Adapts to user's theme
 *    - Recommended for standard UI elements
 *    - The +1 is required for historical reasons
 *
 * 3. Custom Brushes:
 *    - CreateSolidBrush(RGB(r, g, b))
 *    - Allows any color
 *    - Must be deleted with DeleteObject when done
 *    - RGB macro: RGB(red, green, blue) where each is 0-255
 *
 * 4. Stock Objects:
 *    - GetStockObject(OBJECT_ID)
 *    - Predefined GDI objects
 *    - Do NOT delete stock objects
 *    - Always available, no creation needed
 *
 * 5. Color Values (COLORREF):
 *    - 32-bit value: 0x00BBGGRR (note: BGR, not RGB!)
 *    - RGB(r, g, b) macro handles the conversion
 *    - Can extract components:
 *      - GetRValue(color) - Red component
 *      - GetGValue(color) - Green component
 *      - GetBValue(color) - Blue component
 *
 * 6. NULL_BRUSH:
 *    - Special brush that doesn't paint
 *    - Useful when you want to handle all painting manually in WM_PAINT
 *
 * 7. Memory Management:
 *    - System color brushes: Don't delete
 *    - Stock objects: Don't delete
 *    - Custom brushes (CreateSolidBrush, CreatePatternBrush, etc.): Must delete
 *
 * Common Background Colors:
 * - White:      RGB(255, 255, 255) or (COLOR_WINDOW + 1)
 * - Black:      RGB(0, 0, 0)
 * - Red:        RGB(255, 0, 0)
 * - Green:      RGB(0, 255, 0)
 * - Blue:       RGB(0, 0, 255)
 * - Yellow:     RGB(255, 255, 0)
 * - Cyan:       RGB(0, 255, 255)
 * - Magenta:    RGB(255, 0, 255)
 * - Light Gray: RGB(192, 192, 192)
 * - Dark Gray:  RGB(128, 128, 128)
 *
 * Advanced: Pattern Brushes
 * You can also create brushes with patterns using:
 * - CreateHatchBrush: Hatched patterns
 * - CreatePatternBrush: Bitmap patterns
 */
