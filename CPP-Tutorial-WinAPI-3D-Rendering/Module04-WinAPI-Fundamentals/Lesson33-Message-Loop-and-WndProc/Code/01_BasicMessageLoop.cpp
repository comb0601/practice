/*
 * Lesson 33, Example 01: Basic Message Loop
 *
 * This program demonstrates the standard Windows message loop structure
 * and how messages flow through GetMessage, TranslateMessage, and DispatchMessage.
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 01_BasicMessageLoop.cpp user32.lib gdi32.lib
 */

#define UNICODE
#define _UNICODE
#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Global counter for messages
DWORD g_messageCount = 0;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"MessageLoopClass";

    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = CLASS_NAME;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wcex);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"Message Loop Demo",
                                WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                                800, 600, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // THE MESSAGE LOOP - Heart of Windows application
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        /*
         * GetMessage(&msg, NULL, 0, 0):
         *   - Retrieves message from queue
         *   - BLOCKS if queue is empty (waits for message)
         *   - Returns FALSE only when WM_QUIT is received
         *   - Returns -1 on error (rare)
         *
         * Parameters:
         *   &msg: Filled with message information
         *   NULL: Get messages for all windows in this thread
         *   0, 0: No message filtering
         */

        g_messageCount++;

        // TranslateMessage: Converts virtual-key messages to character messages
        TranslateMessage(&msg);

        // DispatchMessage: Sends message to window procedure
        DispatchMessage(&msg);

        /*
         * Message Flow:
         * 1. GetMessage retrieves message
         * 2. TranslateMessage processes keyboard messages
         * 3. DispatchMessage calls WindowProc
         * 4. WindowProc processes message
         * 5. Loop continues
         */
    }

    // When GetMessage returns FALSE (WM_QUIT received), loop exits
    // msg.wParam contains exit code from PostQuitMessage
    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static DWORD paintCount = 0;
    static DWORD keyCount = 0;
    static DWORD mouseCount = 0;

    switch (uMsg)
    {
        case WM_PAINT:
        {
            paintCount++;

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            wchar_t buffer[256];
            int y = 10;

            const wchar_t* title = L"Message Loop Statistics:";
            TextOut(hdc, 10, y, title, (int)wcslen(title));
            y += 30;

            swprintf_s(buffer, L"Total messages processed: %lu", g_messageCount);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 25;

            swprintf_s(buffer, L"WM_PAINT messages: %lu", paintCount);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            swprintf_s(buffer, L"Keyboard messages: %lu", keyCount);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            swprintf_s(buffer, L"Mouse messages: %lu", mouseCount);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 35;

            const wchar_t* info = L"The message loop continuously:";
            TextOut(hdc, 10, y, info, (int)wcslen(info));
            y += 25;

            const wchar_t* info1 = L"  1. Retrieves messages with GetMessage()";
            TextOut(hdc, 10, y, info1, (int)wcslen(info1));
            y += 20;

            const wchar_t* info2 = L"  2. Translates keyboard messages with TranslateMessage()";
            TextOut(hdc, 10, y, info2, (int)wcslen(info2));
            y += 20;

            const wchar_t* info3 = L"  3. Dispatches messages with DispatchMessage()";
            TextOut(hdc, 10, y, info3, (int)wcslen(info3));
            y += 20;

            const wchar_t* info4 = L"  4. Window procedure processes messages";
            TextOut(hdc, 10, y, info4, (int)wcslen(info4));
            y += 20;

            const wchar_t* info5 = L"  5. Loop repeats until WM_QUIT";
            TextOut(hdc, 10, y, info5, (int)wcslen(info5));
            y += 30;

            const wchar_t* action = L"Try: Click, type, resize - watch counters increase!";
            TextOut(hdc, 10, y, action, (int)wcslen(action));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_CHAR:
            keyCount++;
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;

        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MOUSEMOVE:
            mouseCount++;
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;

        case WM_DESTROY:
            // PostQuitMessage posts WM_QUIT to message queue
            // This causes GetMessage to return 0, exiting the loop
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
