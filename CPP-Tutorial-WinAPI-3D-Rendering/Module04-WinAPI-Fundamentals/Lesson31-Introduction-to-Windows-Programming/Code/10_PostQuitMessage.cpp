/*
 * Lesson 31, Example 10: PostQuitMessage and Application Termination
 *
 * This program demonstrates:
 * - PostQuitMessage function
 * - Different ways to exit a Windows application
 * - Exit codes
 * - Proper shutdown sequence
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 10_PostQuitMessage.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 10_PostQuitMessage.cpp -o 10_PostQuitMessage.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
        {
            /*
             * WM_DESTROY is sent when the window is being destroyed.
             * This is the proper place to call PostQuitMessage.
             *
             * PostQuitMessage posts WM_QUIT to the message queue,
             * causing GetMessage to return 0, which exits the message loop.
             */

            MessageBox(hwnd,
                      L"WM_DESTROY received.\n\n"
                      L"Calling PostQuitMessage(0) to exit application.",
                      L"Shutdown",
                      MB_OK | MB_ICONINFORMATION);

            /*
             * PostQuitMessage posts WM_QUIT with specified exit code
             * Exit code can be retrieved as msg.wParam after GetMessage returns 0
             */
            PostQuitMessage(0);  // 0 = success exit code
            return 0;
        }

        case WM_CLOSE:
        {
            /*
             * WM_CLOSE is sent when user clicks the X button.
             * Default behavior (DefWindowProc) calls DestroyWindow.
             *
             * You can intercept WM_CLOSE to:
             * - Ask for confirmation
             * - Save unsaved data
             * - Prevent closing under certain conditions
             */

            int result = MessageBox(hwnd,
                                   L"Are you sure you want to exit?",
                                   L"Confirm Exit",
                                   MB_YESNO | MB_ICONQUESTION);

            if (result == IDYES)
            {
                /*
                 * User confirmed exit - destroy the window
                 * This will trigger WM_DESTROY
                 */
                DestroyWindow(hwnd);
            }
            // else: User clicked No, don't destroy window

            return 0;
        }

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    // ESC key - exit with code 1
                    MessageBox(hwnd, L"ESC pressed - exiting with code 1", L"Exit", MB_OK);
                    PostQuitMessage(1);
                    break;

                case VK_F4:
                    if (GetKeyState(VK_MENU) & 0x8000)
                    {
                        // Alt+F4 - standard Windows close
                        DestroyWindow(hwnd);
                    }
                    break;
            }
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            const wchar_t* info1 = L"Application Termination Methods:";
            const wchar_t* info2 = L"";
            const wchar_t* info3 = L"1. Click X button:";
            const wchar_t* info4 = L"   - Sends WM_CLOSE";
            const wchar_t* info5 = L"   - Shows confirmation dialog";
            const wchar_t* info6 = L"   - If confirmed, calls DestroyWindow";
            const wchar_t* info7 = L"";
            const wchar_t* info8 = L"2. Press ESC:";
            const wchar_t* info9 = L"   - Directly calls PostQuitMessage(1)";
            const wchar_t* info10 = L"   - Exits with code 1";
            const wchar_t* info11 = L"";
            const wchar_t* info12 = L"3. Alt+F4:";
            const wchar_t* info13 = L"   - Standard Windows shortcut";
            const wchar_t* info14 = L"   - Calls DestroyWindow -> WM_DESTROY -> PostQuitMessage(0)";

            int y = 10;
            TextOut(hdc, 10, y, info1, (int)wcslen(info1)); y += 25;
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
            TextOut(hdc, 10, y, info14, (int)wcslen(info14)); y += 20;

            EndPaint(hwnd, &ps);
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"PostQuitMessageClass";

    WNDCLASS wc = { };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME,
        L"PostQuitMessage and Application Exit",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 700, 400,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    /*
     * When we reach here, GetMessage has returned 0 (WM_QUIT received)
     * msg.wParam contains the exit code passed to PostQuitMessage
     */

    wchar_t exitMsg[100];
    swprintf_s(exitMsg, 100, L"Application exiting with code: %d", (int)msg.wParam);
    MessageBox(NULL, exitMsg, L"Exit Code", MB_OK | MB_ICONINFORMATION);

    return (int)msg.wParam;
}

/*
 * Key Concepts Demonstrated:
 *
 * 1. PostQuitMessage:
 *    void PostQuitMessage(int nExitCode)
 *    - Posts WM_QUIT to message queue
 *    - Causes GetMessage to return 0
 *    - Exit code is stored in msg.wParam
 *    - Only call from main window's WM_DESTROY
 *
 * 2. Application Termination Sequence:
 *
 *    User clicks X:
 *    → WM_CLOSE sent
 *    → Your code handles WM_CLOSE
 *    → Call DestroyWindow if OK to close
 *    → WM_DESTROY sent
 *    → Call PostQuitMessage
 *    → WM_QUIT posted
 *    → GetMessage returns 0
 *    → Message loop exits
 *    → WinMain returns
 *
 * 3. WM_CLOSE vs WM_DESTROY:
 *
 *    WM_CLOSE:
 *    - Request to close window
 *    - Can be rejected
 *    - Good place for "Save changes?" dialogs
 *    - Default handler calls DestroyWindow
 *
 *    WM_DESTROY:
 *    - Window is being destroyed
 *    - Cannot be stopped
 *    - Must call PostQuitMessage here
 *    - Clean up resources
 *
 * 4. Exit Codes:
 *    - 0: Normal/successful exit
 *    - Non-zero: Error or abnormal exit
 *    - Returned from WinMain
 *    - Can be checked by batch files or parent processes
 *
 * 5. DestroyWindow:
 *    - Destroys a window
 *    - Sends WM_DESTROY message
 *    - For main window, leads to PostQuitMessage
 *    - For child windows, just destroys the window
 *
 * Common Patterns:
 *
 * 1. Simple exit (no confirmation):
 *    case WM_DESTROY:
 *        PostQuitMessage(0);
 *        return 0;
 *
 * 2. Exit with confirmation:
 *    case WM_CLOSE:
 *        if (MessageBox(hwnd, L"Exit?", L"Confirm", MB_YESNO) == IDYES)
 *            DestroyWindow(hwnd);
 *        return 0;
 *    case WM_DESTROY:
 *        PostQuitMessage(0);
 *        return 0;
 *
 * 3. Save before exit:
 *    case WM_CLOSE:
 *        if (hasUnsavedChanges) {
 *            int result = MessageBox(hwnd, L"Save?", L"Unsaved", MB_YESNOCANCEL);
 *            if (result == IDCANCEL) return 0;
 *            if (result == IDYES) SaveData();
 *        }
 *        DestroyWindow(hwnd);
 *        return 0;
 *
 * 4. Prevent closing:
 *    case WM_CLOSE:
 *        if (!canClose) {
 *            MessageBox(hwnd, L"Cannot close now", L"Info", MB_OK);
 *            return 0;  // Don't call DestroyWindow
 *        }
 *        DestroyWindow(hwnd);
 *        return 0;
 *
 * Multiple Windows:
 *
 * When application has multiple windows:
 * - Only main window calls PostQuitMessage
 * - Other windows just handle WM_DESTROY without PostQuitMessage
 * - Or use window counting to call PostQuitMessage when last window closes
 *
 * Example:
 * static int windowCount = 0;
 *
 * case WM_CREATE:
 *     windowCount++;
 *     return 0;
 *
 * case WM_DESTROY:
 *     windowCount--;
 *     if (windowCount == 0)
 *         PostQuitMessage(0);
 *     return 0;
 *
 * ExitProcess vs PostQuitMessage:
 *
 * PostQuitMessage:
 * - Graceful shutdown
 * - Allows cleanup
 * - Recommended approach
 *
 * ExitProcess:
 * - Immediate termination
 * - No cleanup
 * - Only for emergency situations
 * - Leaves resources unreleased
 *
 * Best Practices:
 * - Always handle WM_CLOSE if you need cleanup or confirmation
 * - Always call PostQuitMessage in WM_DESTROY
 * - Return meaningful exit codes
 * - Clean up resources before calling PostQuitMessage
 * - Don't call PostQuitMessage from non-main windows
 */
