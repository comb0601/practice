/*
 * Lesson 31, Example 09: PeekMessage
 *
 * This program demonstrates:
 * - Using PeekMessage for non-blocking message retrieval
 * - Difference between GetMessage and PeekMessage
 * - Implementing an idle-time processing loop
 * - Creating animations and games with PeekMessage
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 09_PeekMessage.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 09_PeekMessage.cpp -o 09_PeekMessage.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>
#include <stdio.h>

// Global counter for idle processing
LONGLONG g_idleCounter = 0;
int g_ballX = 50;
int g_ballY = 50;
int g_ballDX = 2;
int g_ballDY = 2;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Fill background
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            // Display information
            const wchar_t* info1 = L"PeekMessage Demo - Non-Blocking Message Loop";
            const wchar_t* info2 = L"";
            const wchar_t* info3 = L"This program uses PeekMessage instead of GetMessage";
            const wchar_t* info4 = L"PeekMessage never blocks, allowing continuous processing";
            const wchar_t* info5 = L"";
            const wchar_t* info6 = L"Watch the ball animate continuously!";
            const wchar_t* info7 = L"The idle counter shows background processing";

            int y = 10;
            TextOut(hdc, 10, y, info1, (int)wcslen(info1)); y += 20;
            TextOut(hdc, 10, y, info2, (int)wcslen(info2)); y += 20;
            TextOut(hdc, 10, y, info3, (int)wcslen(info3)); y += 20;
            TextOut(hdc, 10, y, info4, (int)wcslen(info4)); y += 20;
            TextOut(hdc, 10, y, info5, (int)wcslen(info5)); y += 20;
            TextOut(hdc, 10, y, info6, (int)wcslen(info6)); y += 20;
            TextOut(hdc, 10, y, info7, (int)wcslen(info7)); y += 20;

            // Display idle counter
            wchar_t counterText[100];
            swprintf_s(counterText, 100, L"Idle iterations: %I64d", g_idleCounter);
            TextOut(hdc, 10, y + 20, counterText, (int)wcslen(counterText));

            // Draw bouncing ball
            HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

            Ellipse(hdc, g_ballX - 15, g_ballY - 15, g_ballX + 15, g_ballY + 15);

            SelectObject(hdc, hOldBrush);
            DeleteObject(hBrush);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_SIZE:
            // Force repaint when window is resized
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"PeekMessageClass";

    WNDCLASS wc = { };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME,
        L"PeekMessage - Non-Blocking Message Loop",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 700, 350,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);

    /*
     * PeekMessage Message Loop
     *
     * Unlike GetMessage which blocks when no messages are available,
     * PeekMessage returns immediately whether a message is available or not.
     *
     * This allows the application to perform idle-time processing,
     * animations, game logic, etc.
     */

    MSG msg = { };
    BOOL done = FALSE;

    while (!done)
    {
        /*
         * PeekMessage syntax:
         * BOOL PeekMessage(
         *     LPMSG lpMsg,        // Pointer to MSG structure
         *     HWND  hWnd,         // Handle to window (NULL = all windows)
         *     UINT  wMsgFilterMin,// Minimum message value
         *     UINT  wMsgFilterMax,// Maximum message value
         *     UINT  wRemoveMsg    // How to handle the message
         * );
         *
         * Last parameter options:
         * PM_NOREMOVE - Leave message in queue
         * PM_REMOVE   - Remove message from queue (like GetMessage)
         * PM_NOYIELD  - Don't yield to other threads
         */

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // Message is available

            if (msg.message == WM_QUIT)
            {
                done = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /*
             * No message available - this is idle time
             * Perfect for animations, game logic, background processing, etc.
             *
             * With GetMessage, we would never reach this point because
             * GetMessage blocks until a message arrives.
             */

            // Increment idle counter
            g_idleCounter++;

            // Animate the ball
            // Get window size
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);

            // Update ball position
            g_ballX += g_ballDX;
            g_ballY += g_ballDY;

            // Bounce off edges
            if (g_ballX <= 15 || g_ballX >= clientRect.right - 15)
                g_ballDX = -g_ballDX;

            if (g_ballY <= 15 || g_ballY >= clientRect.bottom - 15)
                g_ballDY = -g_ballDY;

            // Force repaint
            InvalidateRect(hwnd, NULL, FALSE);

            // Small sleep to control animation speed and reduce CPU usage
            // Without this, the loop would consume 100% CPU
            Sleep(10);  // 10ms delay = approximately 100 FPS
        }
    }

    return (int)msg.wParam;
}

/*
 * Key Concepts Demonstrated:
 *
 * 1. PeekMessage Signature:
 *    BOOL PeekMessage(LPMSG lpMsg, HWND hWnd, UINT min, UINT max, UINT wRemoveMsg)
 *
 * 2. Return Values:
 *    - Nonzero: Message is available (retrieved into lpMsg)
 *    - Zero: No message available
 *
 * 3. PM_REMOVE vs PM_NOREMOVE:
 *    - PM_REMOVE: Remove message from queue (normal usage)
 *    - PM_NOREMOVE: Peek at message without removing it
 *
 * 4. GetMessage vs PeekMessage:
 *
 *    GetMessage:
 *    - Blocks if no messages
 *    - Waits for user input
 *    - Efficient for event-driven apps
 *    - Low CPU usage
 *
 *    PeekMessage:
 *    - Never blocks
 *    - Returns immediately
 *    - Required for games/animations
 *    - Can consume high CPU if not careful
 *
 * 5. Idle-Time Processing:
 *    When PeekMessage returns FALSE (no messages), you can:
 *    - Update animations
 *    - Process game logic
 *    - Perform background calculations
 *    - Update continuous simulations
 *
 * 6. CPU Usage Control:
 *    - Without Sleep(), PeekMessage loop consumes 100% CPU
 *    - Use Sleep() to limit frame rate and reduce CPU usage
 *    - Sleep(10) = approximately 100 FPS maximum
 *    - Sleep(16) = approximately 60 FPS maximum
 *
 * 7. When to Use Each:
 *
 *    Use GetMessage for:
 *    - Standard business applications
 *    - Dialog-based applications
 *    - Applications that only respond to user input
 *    - Low CPU usage is important
 *
 *    Use PeekMessage for:
 *    - Games
 *    - Animations
 *    - Real-time simulations
 *    - Applications with continuous updates
 *    - Media players
 *
 * Message Loop Patterns:
 *
 * 1. Standard Event-Driven (GetMessage):
 *    while (GetMessage(&msg, NULL, 0, 0)) {
 *        TranslateMessage(&msg);
 *        DispatchMessage(&msg);
 *    }
 *
 * 2. Game Loop (PeekMessage):
 *    while (!done) {
 *        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
 *            if (msg.message == WM_QUIT) done = TRUE;
 *            else {
 *                TranslateMessage(&msg);
 *                DispatchMessage(&msg);
 *            }
 *        } else {
 *            UpdateGame();
 *            RenderFrame();
 *        }
 *    }
 *
 * 3. Hybrid Approach (MsgWaitForMultipleObjects):
 *    - Waits for messages or other events
 *    - More efficient than PeekMessage for some applications
 *    - Advanced topic for later lessons
 *
 * Performance Considerations:
 *
 * 1. PeekMessage with PM_NOREMOVE:
 *    - Useful for checking if messages are pending
 *    - Don't use in tight loop (performance hit)
 *
 * 2. Sleep() duration:
 *    - Too short: High CPU usage
 *    - Too long: Choppy animation
 *    - Balance based on needs
 *
 * 3. Invalidate efficiently:
 *    - InvalidateRect(hwnd, NULL, FALSE) - don't erase background
 *    - Or invalidate only changed regions
 *
 * Advanced: High-Resolution Timing
 * For precise timing in games, use:
 * - QueryPerformanceCounter / QueryPerformanceFrequency
 * - Will be covered in animation lessons
 */
