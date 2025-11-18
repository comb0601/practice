/*
 * Lesson 31, Example 05: Multiple Windows
 *
 * This program demonstrates:
 * - Creating multiple windows from the same class
 * - Creating windows from different classes
 * - Managing multiple windows
 * - Communication between windows
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 05_MultipleWindows.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 05_MultipleWindows.cpp -o 05_MultipleWindows.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>
#include <stdio.h>

// Window procedure for main window
LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            const wchar_t* message = L"This is the MAIN window";
            TextOut(hdc, 10, 10, message, (int)wcslen(message));

            const wchar_t* info = L"Click to send message to all windows";
            TextOut(hdc, 10, 30, info, (int)wcslen(info));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            // Broadcast a message to all top-level windows
            // WM_USER is the start of user-defined messages
            PostMessage(HWND_BROADCAST, WM_USER + 1, 0, 0);
            return 0;
        }

        case WM_USER + 1:
        {
            // Received broadcast message
            MessageBox(hwnd, L"Main window received broadcast!", L"Info", MB_OK);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Window procedure for child windows
LRESULT CALLBACK ChildWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Get window title to display
            wchar_t title[100];
            GetWindowText(hwnd, title, 100);

            wchar_t message[200];
            swprintf_s(message, 200, L"This is: %s", title);
            TextOut(hdc, 10, 10, message, (int)wcslen(message));

            const wchar_t* info = L"Each window has its own window procedure";
            TextOut(hdc, 10, 30, info, (int)wcslen(info));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_USER + 1:
        {
            // Received broadcast message
            wchar_t title[100];
            GetWindowText(hwnd, title, 100);

            wchar_t message[200];
            swprintf_s(message, 200, L"%s received broadcast!", title);
            MessageBox(hwnd, message, L"Info", MB_OK);
            return 0;
        }

        case WM_DESTROY:
            // Don't call PostQuitMessage for child windows
            // Only the main window should post WM_QUIT
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    // Register main window class
    const wchar_t MAIN_CLASS[] = L"MainWindowClass";

    WNDCLASS wcMain = { };
    wcMain.lpfnWndProc   = MainWindowProc;
    wcMain.hInstance     = hInstance;
    wcMain.lpszClassName = MAIN_CLASS;
    wcMain.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcMain.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wcMain);

    // Register child window class
    const wchar_t CHILD_CLASS[] = L"ChildWindowClass";

    WNDCLASS wcChild = { };
    wcChild.lpfnWndProc   = ChildWindowProc;
    wcChild.hInstance     = hInstance;
    wcChild.lpszClassName = CHILD_CLASS;
    wcChild.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcChild.hbrBackground = CreateSolidBrush(RGB(240, 248, 255));  // Alice blue

    RegisterClass(&wcChild);

    // Create main window
    HWND hwndMain = CreateWindowEx(
        0, MAIN_CLASS,
        L"Main Window - Click to broadcast message",
        WS_OVERLAPPEDWINDOW,
        100, 100, 500, 200,
        NULL, NULL, hInstance, NULL
    );

    // Create multiple child windows from the same class
    HWND hwndChild1 = CreateWindowEx(
        0, CHILD_CLASS,
        L"Child Window 1",
        WS_OVERLAPPEDWINDOW,
        150, 150, 400, 150,
        NULL, NULL, hInstance, NULL
    );

    HWND hwndChild2 = CreateWindowEx(
        0, CHILD_CLASS,
        L"Child Window 2",
        WS_OVERLAPPEDWINDOW,
        200, 200, 400, 150,
        NULL, NULL, hInstance, NULL
    );

    HWND hwndChild3 = CreateWindowEx(
        0, CHILD_CLASS,
        L"Child Window 3",
        WS_OVERLAPPEDWINDOW,
        250, 250, 400, 150,
        NULL, NULL, hInstance, NULL
    );

    if (!hwndMain || !hwndChild1 || !hwndChild2 || !hwndChild3)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    // Show all windows
    ShowWindow(hwndMain, nCmdShow);
    ShowWindow(hwndChild1, nCmdShow);
    ShowWindow(hwndChild2, nCmdShow);
    ShowWindow(hwndChild3, nCmdShow);

    // Message loop
    // This single message loop handles messages for ALL windows
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Clean up
    DeleteObject(wcChild.hbrBackground);

    return (int)msg.wParam;
}

/*
 * Key Concepts Demonstrated:
 *
 * 1. Multiple Window Classes:
 *    - Different window classes can have different behaviors
 *    - Each class has its own window procedure
 *    - Classes must be registered before creating windows
 *
 * 2. Multiple Windows from Same Class:
 *    - Can create many windows from one class
 *    - All share the same window procedure
 *    - Each window has unique HWND (window handle)
 *    - Window procedure can differentiate using hwnd parameter
 *
 * 3. Single Message Loop:
 *    - One message loop handles all windows
 *    - GetMessage(NULL) retrieves messages for all windows in the thread
 *    - Windows routes messages to appropriate window procedure
 *
 * 4. Window Communication:
 *    - PostMessage: Send message asynchronously (doesn't wait)
 *    - SendMessage: Send message synchronously (waits for processing)
 *    - HWND_BROADCAST: Send to all top-level windows
 *
 * 5. User-Defined Messages:
 *    - WM_USER to 0x7FFF: Available for application use
 *    - WM_APP to 0xBFFF: Recommended range
 *    - Use WM_USER + n or WM_APP + n
 *
 * 6. Window Destruction:
 *    - Only main window calls PostQuitMessage(0)
 *    - Other windows just return from WM_DESTROY
 *    - PostQuitMessage exits the message loop
 *
 * 7. Window Handles:
 *    - Each window has unique HWND
 *    - HWND is used to identify and communicate with windows
 *    - Can store HWNDs to send messages between windows
 *
 * 8. Window Hierarchy:
 *    - Despite the names, these are all top-level windows (no parent)
 *    - Could create actual child windows by passing parent HWND
 *    - Child windows are clipped to parent and move with parent
 *
 * Advanced Topics:
 *
 * PostMessage vs SendMessage:
 * - PostMessage: Puts message in queue, returns immediately
 * - SendMessage: Calls window procedure directly, waits for result
 *
 * Window Enumeration:
 * - EnumWindows: Enumerate all top-level windows
 * - EnumChildWindows: Enumerate child windows
 *
 * Finding Windows:
 * - FindWindow: Find by class name or window title
 * - FindWindowEx: More specific search
 *
 * Example of sending message to specific window:
 * PostMessage(hwndChild1, WM_USER + 2, 0, 0);
 *
 * Example of sending synchronously:
 * LRESULT result = SendMessage(hwndChild1, WM_USER + 3, 0, 0);
 */
