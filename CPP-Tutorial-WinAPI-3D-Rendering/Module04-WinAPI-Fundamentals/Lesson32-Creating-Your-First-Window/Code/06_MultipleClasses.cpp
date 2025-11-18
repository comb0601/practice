/*
 * Lesson 32, Example 06: Multiple Window Classes
 *
 * This program demonstrates creating and using multiple window classes
 * within the same application. Each class can have different properties.
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 06_MultipleClasses.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 06_MultipleClasses.cpp -o 06_MultipleClasses.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ToolWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    // Register first window class: Main Window
    const wchar_t MAIN_CLASS[] = L"MainWindowClass";
    WNDCLASSEX wcMain = { };
    wcMain.cbSize = sizeof(WNDCLASSEX);
    wcMain.style = CS_HREDRAW | CS_VREDRAW;
    wcMain.lpfnWndProc = MainWindowProc;
    wcMain.hInstance = hInstance;
    wcMain.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcMain.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcMain.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcMain.lpszClassName = MAIN_CLASS;
    wcMain.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcMain))
    {
        MessageBox(NULL, L"Main Window Registration Failed!", L"Error", MB_OK);
        return 1;
    }

    // Register second window class: Tool Window
    const wchar_t TOOL_CLASS[] = L"ToolWindowClass";
    WNDCLASSEX wcTool = { };
    wcTool.cbSize = sizeof(WNDCLASSEX);
    wcTool.style = CS_HREDRAW | CS_VREDRAW;
    wcTool.lpfnWndProc = ToolWindowProc;
    wcTool.hInstance = hInstance;
    wcTool.hIcon = LoadIcon(NULL, IDI_INFORMATION);
    wcTool.hCursor = LoadCursor(NULL, IDC_HAND);
    wcTool.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wcTool.lpszClassName = TOOL_CLASS;
    wcTool.hIconSm = LoadIcon(NULL, IDI_INFORMATION);

    if (!RegisterClassEx(&wcTool))
    {
        MessageBox(NULL, L"Tool Window Registration Failed!", L"Error", MB_OK);
        return 1;
    }

    // Register third window class: Child Control
    const wchar_t CHILD_CLASS[] = L"ChildWindowClass";
    WNDCLASSEX wcChild = { };
    wcChild.cbSize = sizeof(WNDCLASSEX);
    wcChild.style = CS_HREDRAW | CS_VREDRAW;
    wcChild.lpfnWndProc = ChildWindowProc;
    wcChild.hInstance = hInstance;
    wcChild.hCursor = LoadCursor(NULL, IDC_IBEAM);
    wcChild.hbrBackground = CreateSolidBrush(RGB(240, 248, 255));  // Light blue
    wcChild.lpszClassName = CHILD_CLASS;

    if (!RegisterClassEx(&wcChild))
    {
        MessageBox(NULL, L"Child Window Registration Failed!", L"Error", MB_OK);
        return 1;
    }

    // Create main window
    HWND hwndMain = CreateWindowEx(
        0,
        MAIN_CLASS,
        L"Main Window - Multiple Classes Demo",
        WS_OVERLAPPEDWINDOW,
        100, 100,
        800, 600,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwndMain == NULL)
    {
        MessageBox(NULL, L"Main Window Creation Failed!", L"Error", MB_OK);
        return 1;
    }

    // Create tool window (floating palette)
    HWND hwndTool = CreateWindowEx(
        WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
        TOOL_CLASS,
        L"Tool Palette",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME,
        920, 100,
        300, 400,
        hwndMain,  // Parent window
        NULL,
        hInstance,
        NULL
    );

    // Create child window inside main window
    HWND hwndChild = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        CHILD_CLASS,
        NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        10, 10,
        400, 200,
        hwndMain,  // Parent
        (HMENU)1001,  // Child ID
        hInstance,
        NULL
    );

    ShowWindow(hwndMain, nCmdShow);
    UpdateWindow(hwndMain);

    ShowWindow(hwndTool, SW_SHOW);
    UpdateWindow(hwndTool);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            int y = 220;
            const wchar_t* title = L"Main Window Class";
            TextOut(hdc, 10, y, title, (int)wcslen(title));
            y += 25;

            const wchar_t* info1 = L"This window uses the 'MainWindowClass'.";
            TextOut(hdc, 10, y, info1, (int)wcslen(info1));
            y += 20;

            const wchar_t* info2 = L"It has:";
            TextOut(hdc, 10, y, info2, (int)wcslen(info2));
            y += 20;

            const wchar_t* info3 = L"  - Standard arrow cursor";
            TextOut(hdc, 10, y, info3, (int)wcslen(info3));
            y += 20;

            const wchar_t* info4 = L"  - Default window background";
            TextOut(hdc, 10, y, info4, (int)wcslen(info4));
            y += 20;

            const wchar_t* info5 = L"  - Application icon";
            TextOut(hdc, 10, y, info5, (int)wcslen(info5));
            y += 30;

            const wchar_t* info6 = L"The blue area above is a child window with a different class.";
            TextOut(hdc, 10, y, info6, (int)wcslen(info6));
            y += 20;

            const wchar_t* info7 = L"The tool palette is another window with yet another class.";
            TextOut(hdc, 10, y, info7, (int)wcslen(info7));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK ToolWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            int y = 10;
            const wchar_t* title = L"Tool Window Class";
            TextOut(hdc, 10, y, title, (int)wcslen(title));
            y += 25;

            const wchar_t* info1 = L"This window uses";
            TextOut(hdc, 10, y, info1, (int)wcslen(info1));
            y += 15;

            const wchar_t* info2 = L"'ToolWindowClass'.";
            TextOut(hdc, 10, y, info2, (int)wcslen(info2));
            y += 25;

            const wchar_t* info3 = L"It has:";
            TextOut(hdc, 10, y, info3, (int)wcslen(info3));
            y += 20;

            const wchar_t* info4 = L"  - Hand cursor";
            TextOut(hdc, 10, y, info4, (int)wcslen(info4));
            y += 20;

            const wchar_t* info5 = L"  - Button face color";
            TextOut(hdc, 10, y, info5, (int)wcslen(info5));
            y += 20;

            const wchar_t* info6 = L"  - Info icon";
            TextOut(hdc, 10, y, info6, (int)wcslen(info6));
            y += 20;

            const wchar_t* info7 = L"  - WS_EX_TOOLWINDOW";
            TextOut(hdc, 10, y, info7, (int)wcslen(info7));
            y += 20;

            const wchar_t* info8 = L"  - WS_EX_TOPMOST";
            TextOut(hdc, 10, y, info8, (int)wcslen(info8));
            y += 30;

            const wchar_t* info9 = L"It's always on top!";
            TextOut(hdc, 10, y, info9, (int)wcslen(info9));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_CLOSE:
            // Don't destroy, just hide
            ShowWindow(hwnd, SW_HIDE);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK ChildWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            int y = 10;
            const wchar_t* title = L"Child Window Class";
            TextOut(hdc, 10, y, title, (int)wcslen(title));
            y += 25;

            const wchar_t* info1 = L"This is a child window using 'ChildWindowClass'.";
            TextOut(hdc, 10, y, info1, (int)wcslen(info1));
            y += 20;

            const wchar_t* info2 = L"It has:";
            TextOut(hdc, 10, y, info2, (int)wcslen(info2));
            y += 20;

            const wchar_t* info3 = L"  - I-beam cursor (text cursor)";
            TextOut(hdc, 10, y, info3, (int)wcslen(info3));
            y += 20;

            const wchar_t* info4 = L"  - Light blue background";
            TextOut(hdc, 10, y, info4, (int)wcslen(info4));
            y += 20;

            const wchar_t* info5 = L"  - WS_CHILD style";
            TextOut(hdc, 10, y, info5, (int)wcslen(info5));
            y += 20;

            const wchar_t* info6 = L"  - WS_EX_CLIENTEDGE (sunken border)";
            TextOut(hdc, 10, y, info6, (int)wcslen(info6));
            y += 30;

            const wchar_t* info7 = L"Move your mouse over different windows to see";
            TextOut(hdc, 10, y, info7, (int)wcslen(info7));
            y += 15;

            const wchar_t* info8 = L"the different cursors defined in each class!";
            TextOut(hdc, 10, y, info8, (int)wcslen(info8));

            EndPaint(hwnd, &ps);
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/*
 * Multiple Window Classes - Key Concepts:
 *
 * 1. Each window class has unique properties:
 *    - Window procedure
 *    - Icon and cursor
 *    - Background brush
 *    - Class styles
 *    - Extra bytes
 *
 * 2. Why use multiple classes?
 *    - Different window types need different behavior
 *    - Different appearance (cursor, background, icon)
 *    - Different message handling
 *    - Organizational clarity
 *
 * 3. Common scenarios:
 *    - Main application window
 *    - Tool palettes/floating windows
 *    - Custom controls
 *    - Child windows with special behavior
 *
 * 4. Class registration:
 *    - Each class must have unique name
 *    - Register all classes before creating windows
 *    - Classes remain registered until app exits or UnregisterClass
 *
 * 5. Window creation:
 *    - Specify class name in CreateWindowEx
 *    - Windows of same class share class properties
 *    - Windows can still have individual properties (position, size, etc.)
 *
 * This example shows three different window classes working together!
 */
