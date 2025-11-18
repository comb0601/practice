/*
 * Lesson 31, Example 03: Window With Paint
 *
 * This program demonstrates:
 * - Handling WM_PAINT message
 * - Using BeginPaint and EndPaint
 * - Drawing text in a window
 * - Understanding the PAINTSTRUCT
 * - Invalidating window regions
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 03_WindowWithPaint.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 03_WindowWithPaint.cpp -o 03_WindowWithPaint.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>
#include <stdio.h>

// Global variable to count paint operations
int g_paintCount = 0;

/*
 * WindowProc - Window procedure that handles painting
 */
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
        {
            /*
             * WM_PAINT is sent when the window needs to be redrawn.
             * This happens when:
             * - Window is first shown
             * - Window is uncovered
             * - Window is resized
             * - InvalidateRect is called
             */

            // PAINTSTRUCT contains information about the painting operation
            PAINTSTRUCT ps;

            // BeginPaint prepares the window for painting
            // It fills the PAINTSTRUCT and returns a device context (HDC)
            HDC hdc = BeginPaint(hwnd, &ps);

            // Increment paint counter
            g_paintCount++;

            /*
             * ps.rcPaint contains the rectangle that needs to be repainted
             * Only draw within this area for efficiency
             * For simple programs, you can ignore this and redraw everything
             */

            // Fill the background
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            // Draw some text
            const wchar_t* message = L"Hello, Windows Graphics!";
            TextOut(hdc, 10, 10, message, (int)wcslen(message));

            // Display paint count
            wchar_t countText[100];
            swprintf_s(countText, 100, L"Paint count: %d", g_paintCount);
            TextOut(hdc, 10, 30, countText, (int)wcslen(countText));

            // Display the invalid rectangle
            wchar_t rectText[200];
            swprintf_s(rectText, 200,
                      L"Invalid rect: left=%d, top=%d, right=%d, bottom=%d",
                      ps.rcPaint.left, ps.rcPaint.top,
                      ps.rcPaint.right, ps.rcPaint.bottom);
            TextOut(hdc, 10, 50, rectText, (int)wcslen(rectText));

            // Draw instructions
            const wchar_t* instructions = L"Click anywhere to trigger repaint";
            TextOut(hdc, 10, 80, instructions, (int)wcslen(instructions));

            /*
             * EndPaint must be called to release the device context
             * It also validates the update region (marks it as painted)
             */
            EndPaint(hwnd, &ps);

            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            /*
             * When user clicks, invalidate the window to force a repaint
             * This will cause Windows to send a WM_PAINT message
             */

            // InvalidateRect marks a region as needing to be redrawn
            InvalidateRect(hwnd, NULL, TRUE);
            // NULL = invalidate entire window
            // TRUE = erase background before painting

            // Alternative: Invalidate only part of the window
            // RECT rc = { 100, 100, 200, 200 };
            // InvalidateRect(hwnd, &rc, TRUE);

            return 0;
        }

        case WM_RBUTTONDOWN:
        {
            // Right click to force immediate repaint using UpdateWindow
            InvalidateRect(hwnd, NULL, TRUE);

            // UpdateWindow immediately sends WM_PAINT (doesn't wait for message queue)
            UpdateWindow(hwnd);

            MessageBox(hwnd, L"Window updated immediately!", L"Info", MB_OK);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_CLOSE:
            if (MessageBox(hwnd, L"Really quit?", L"Confirmation", MB_OKCANCEL) == IDOK)
            {
                DestroyWindow(hwnd);
            }
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"PaintWindowClass";

    // Register window class
    WNDCLASS wc = { };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    // Create window
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Window With Paint - Click to Repaint",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 300,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);  // Send initial WM_PAINT

    // Message loop
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
 * 1. WM_PAINT Message:
 *    - Sent when window needs repainting
 *    - Must be handled to draw window contents
 *    - Triggered by: window show, resize, uncover, or InvalidateRect
 *
 * 2. BeginPaint / EndPaint:
 *    - BeginPaint must be called at the start of WM_PAINT handling
 *    - Returns HDC (device context) for drawing
 *    - Fills PAINTSTRUCT with paint information
 *    - EndPaint must be called to release DC and validate region
 *    - Always pair BeginPaint with EndPaint
 *
 * 3. PAINTSTRUCT Structure:
 *    - hdc: Device context for drawing
 *    - rcPaint: Rectangle that needs repainting (invalid region)
 *    - fErase: Whether background was erased
 *    - fRestore, fIncUpdate: Reserved
 *
 * 4. Drawing Functions:
 *    - TextOut: Draws text at specified position
 *    - FillRect: Fills rectangle with specified brush
 *    - Many more: Rectangle, Ellipse, LineTo, etc.
 *
 * 5. InvalidateRect:
 *    - Marks a region as needing repaint
 *    - Causes WM_PAINT to be sent
 *    - NULL = invalidate entire window
 *    - Last parameter: TRUE = erase background, FALSE = don't erase
 *
 * 6. UpdateWindow:
 *    - Immediately sends WM_PAINT if update region is not empty
 *    - Bypasses message queue
 *    - Use for immediate visual feedback
 *
 * 7. Device Context (HDC):
 *    - Represents a drawing surface
 *    - Contains drawing attributes: color, font, pen, brush
 *    - Required for all drawing operations
 *    - Must be obtained via BeginPaint or GetDC
 *    - Must be released via EndPaint or ReleaseDC
 *
 * 8. Common Painting Mistakes:
 *    - Forgetting to call EndPaint (causes resource leak)
 *    - Drawing outside WM_PAINT without GetDC/ReleaseDC
 *    - Not handling WM_PAINT (window won't redraw correctly)
 *    - Using UpdateWindow unnecessarily (performance issue)
 *
 * Best Practices:
 * - Always handle WM_PAINT
 * - Always pair BeginPaint with EndPaint
 * - Draw only what's needed (check ps.rcPaint for efficiency)
 * - Don't perform lengthy operations in WM_PAINT
 * - Use InvalidateRect to trigger repaints
 */
