/*
 * Lesson 31, Example 08: GetMessage Deep Dive
 *
 * This program demonstrates:
 * - GetMessage return values
 * - Filtering messages by window and message range
 * - Understanding message retrieval
 * - Difference between GetMessage and PeekMessage
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 08_GetMessage.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 08_GetMessage.cpp -o 08_GetMessage.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>
#include <stdio.h>

int g_messageCount = 0;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_LBUTTONDOWN:
        {
            g_messageCount++;

            wchar_t msg[100];
            swprintf_s(msg, 100, L"Message #%d: WM_LBUTTONDOWN received", g_messageCount);
            MessageBox(hwnd, msg, L"Message Info", MB_OK);

            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            const wchar_t* info1 = L"GetMessage Demo - Understanding Message Retrieval";
            const wchar_t* info2 = L"";
            const wchar_t* info3 = L"GetMessage(MSG* msg, HWND hwnd, UINT min, UINT max)";
            const wchar_t* info4 = L"  - msg: Pointer to MSG structure to fill";
            const wchar_t* info5 = L"  - hwnd: Window to get messages for (NULL = all windows)";
            const wchar_t* info6 = L"  - min/max: Message range filter (0,0 = all messages)";
            const wchar_t* info7 = L"";
            const wchar_t* info8 = L"Return Values:";
            const wchar_t* info9 = L"  - Nonzero: Message retrieved successfully";
            const wchar_t* info10 = L"  - Zero: WM_QUIT received (time to exit)";
            const wchar_t* info11 = L"  - -1: Error occurred";
            const wchar_t* info12 = L"";
            const wchar_t* info13 = L"Click anywhere to generate messages";

            int y = 10;
            TextOut(hdc, 10, y, info1, (int)wcslen(info1)); y += 20;
            TextOut(hdc, 10, y, info2, (int)wcslen(info2)); y += 20;
            TextOut(hdc, 10, y, info3, (int)wcslen(info3)); y += 20;
            TextOut(hdc, 10, y, info4, (int)wcslen(info4)); y += 20;
            TextOut(hdc, 10, y, info5, (int)wcslen(info5)); y += 20;
            TextOut(hdc, 10, y, info6, (int)wcslen(info6)); y += 20;
            TextOut(hdc, 10, y, info7, (int)wcslen(info7)); y += 20;
            TextOut(hdc, 10, y, info8, (int)wcslen(info8)); y += 20;
            TextOut(hdc, 10, y, info9, (int)wcslen(info9)); y += 20;
            TextOut(hdc, 10, y, info10, (int)wcslen(info10)); y += 20;
            TextOut(hdc, 10, y, info11, (int)wcslen(info11)); y += 20;
            TextOut(hdc, 10, y, info12, (int)wcslen(info12)); y += 20;
            TextOut(hdc, 10, y, info13, (int)wcslen(info13)); y += 20;

            wchar_t countText[100];
            swprintf_s(countText, 100, L"Messages processed: %d", g_messageCount);
            TextOut(hdc, 10, y, countText, (int)wcslen(countText));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);  // This posts WM_QUIT to message queue
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"GetMessageClass";

    WNDCLASS wc = { };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME,
        L"GetMessage Demonstration",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 700, 450,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);

    /*
     * Message Loop with detailed explanation:
     *
     * GetMessage syntax:
     * BOOL GetMessage(
     *     LPMSG lpMsg,        // Pointer to MSG structure
     *     HWND  hWnd,         // Handle to window (NULL = all windows)
     *     UINT  wMsgFilterMin,// Minimum message value (0 = no filter)
     *     UINT  wMsgFilterMax // Maximum message value (0 = no filter)
     * );
     */

    MSG msg = { };

    /*
     * Standard message loop - retrieves all messages for all windows in this thread
     */
    BOOL bRet;
    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (bRet == -1)
        {
            // Error occurred
            MessageBox(NULL, L"GetMessage error!", L"Error", MB_OK | MB_ICONERROR);
            break;
        }
        else
        {
            /*
             * Message retrieved successfully
             * msg structure now contains:
             *   msg.hwnd    - Window that receives the message
             *   msg.message - Message identifier (WM_PAINT, WM_LBUTTONDOWN, etc.)
             *   msg.wParam  - Additional message info
             *   msg.lParam  - Additional message info
             *   msg.time    - Time message was posted
             *   msg.pt      - Cursor position when message was posted
             */

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    /*
     * When GetMessage returns 0, WM_QUIT was received
     * msg.wParam contains the exit code from PostQuitMessage
     */
    return (int)msg.wParam;
}

/*
 * Key Concepts Demonstrated:
 *
 * 1. GetMessage Signature:
 *    BOOL GetMessage(LPMSG lpMsg, HWND hWnd, UINT min, UINT max)
 *
 * 2. Return Values:
 *    - Nonzero (TRUE): Message retrieved successfully, continue loop
 *    - Zero (FALSE): WM_QUIT received, exit loop
 *    - -1: Error occurred
 *
 * 3. Parameters:
 *    a) lpMsg: Pointer to MSG structure to receive message info
 *    b) hWnd: Filter by window handle
 *       - NULL: Retrieve messages for all windows in this thread
 *       - hwnd: Retrieve messages only for this window
 *    c) min/max: Filter by message ID range
 *       - 0, 0: No filter, retrieve all messages
 *       - WM_KEYFIRST, WM_KEYLAST: Only keyboard messages
 *       - WM_MOUSEFIRST, WM_MOUSELAST: Only mouse messages
 *
 * 4. Blocking Behavior:
 *    - GetMessage blocks if message queue is empty
 *    - Thread sleeps until message arrives
 *    - Yields CPU to other processes
 *    - This is why Windows apps are responsive and efficient
 *
 * 5. Message Queue:
 *    - Each GUI thread has its own message queue
 *    - Queue holds posted messages (PostMessage, mouse clicks, etc.)
 *    - Some messages bypass queue (SendMessage)
 *
 * 6. WM_QUIT:
 *    - Special message that doesn't go to window procedure
 *    - GetMessage returns 0 when WM_QUIT is retrieved
 *    - Posted by PostQuitMessage(exitCode)
 *    - Signals application should terminate
 *
 * Examples of Message Filtering:
 *
 * 1. Get all messages (standard):
 *    GetMessage(&msg, NULL, 0, 0)
 *
 * 2. Get messages only for specific window:
 *    GetMessage(&msg, hwnd, 0, 0)
 *
 * 3. Get only keyboard messages:
 *    GetMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST)
 *
 * 4. Get only mouse messages:
 *    GetMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST)
 *
 * GetMessage vs PeekMessage:
 *
 * GetMessage:
 * - Blocks if queue is empty
 * - Waits for message
 * - Standard for event-driven apps
 *
 * PeekMessage:
 * - Never blocks
 * - Returns immediately even if no message
 * - Used for applications that need to do processing (games, animations)
 * - Will be demonstrated in next example
 *
 * Common Pattern - Robust Message Loop:
 *
 * MSG msg;
 * BOOL bRet;
 * while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
 * {
 *     if (bRet == -1)
 *     {
 *         // Handle error
 *         return 1;
 *     }
 *     else
 *     {
 *         TranslateMessage(&msg);
 *         DispatchMessage(&msg);
 *     }
 * }
 * return (int)msg.wParam;
 *
 * Message Flow:
 * 1. User action (click, keypress) or system event
 * 2. Windows posts message to application's queue
 * 3. GetMessage retrieves message from queue
 * 4. TranslateMessage processes keyboard messages
 * 5. DispatchMessage sends to window procedure
 * 6. Window procedure processes message
 * 7. Loop repeats until WM_QUIT
 */
