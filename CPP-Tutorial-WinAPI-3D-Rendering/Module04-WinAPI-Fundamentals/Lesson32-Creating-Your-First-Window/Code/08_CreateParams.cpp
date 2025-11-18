/*
 * Lesson 32, Example 08: CreateWindowEx lpParam - Passing Custom Data
 *
 * This program demonstrates how to pass custom data to window creation
 * using the lpParam parameter of CreateWindowEx.
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 08_CreateParams.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 08_CreateParams.cpp -o 08_CreateParams.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>

// Custom structure to pass to window
struct WindowData
{
    const wchar_t* title;
    COLORREF backgroundColor;
    int counter;
    BOOL showDetails;
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"CreateParamsClass";

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

    // Prepare custom data to pass to window
    WindowData data1;
    data1.title = L"Window with Custom Data";
    data1.backgroundColor = RGB(240, 248, 255);  // Alice blue
    data1.counter = 100;
    data1.showDetails = TRUE;

    // Create window and pass pointer to custom data via lpParam
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"CreateWindowEx lpParam Demo",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL,
        NULL,
        hInstance,
        &data1  // Pass pointer to custom data
    );

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_OK);
        return 1;
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
    // Store WindowData pointer in window's user data area
    static WindowData* pData = nullptr;

    switch (uMsg)
    {
        case WM_NCCREATE:
        {
            /*
             * WM_NCCREATE is sent BEFORE WM_CREATE.
             * This is the earliest point where we can access lpParam.
             *
             * lParam points to CREATESTRUCT, which contains lpCreateParams.
             */

            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;

            // Extract our custom data pointer
            pData = (WindowData*)pCreate->lpCreateParams;

            // Store pointer in window for later access
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pData);

            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }

        case WM_CREATE:
        {
            /*
             * WM_CREATE is sent during window creation.
             * We can also access custom data here.
             */

            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            WindowData* pCreateData = (WindowData*)pCreate->lpCreateParams;

            if (pCreateData)
            {
                wchar_t message[512];
                wsprintf(message,
                         L"WM_CREATE received!\n\n"
                         L"Custom Data:\n"
                         L"  Title: %s\n"
                         L"  Background Color: RGB(%d, %d, %d)\n"
                         L"  Counter: %d\n"
                         L"  Show Details: %s",
                         pCreateData->title,
                         GetRValue(pCreateData->backgroundColor),
                         GetGValue(pCreateData->backgroundColor),
                         GetBValue(pCreateData->backgroundColor),
                         pCreateData->counter,
                         pCreateData->showDetails ? L"Yes" : L"No");

                MessageBox(hwnd, message, L"Window Creation Data", MB_OK | MB_ICONINFORMATION);
            }

            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Retrieve stored data pointer
            WindowData* pWndData = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

            if (pWndData)
            {
                // Use the custom data to customize drawing
                HBRUSH hBrush = CreateSolidBrush(pWndData->backgroundColor);
                FillRect(hdc, &ps.rcPaint, hBrush);
                DeleteObject(hBrush);

                SetBkMode(hdc, TRANSPARENT);

                wchar_t buffer[256];
                int y = 10;

                const wchar_t* title = L"Custom Data Passed via CreateWindowEx lpParam:";
                TextOut(hdc, 10, y, title, (int)wcslen(title));
                y += 30;

                wsprintf(buffer, L"Title: %s", pWndData->title);
                TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
                y += 25;

                wsprintf(buffer, L"Background Color: RGB(%d, %d, %d)",
                         GetRValue(pWndData->backgroundColor),
                         GetGValue(pWndData->backgroundColor),
                         GetBValue(pWndData->backgroundColor));
                TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
                y += 25;

                wsprintf(buffer, L"Counter: %d", pWndData->counter);
                TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
                y += 25;

                wsprintf(buffer, L"Show Details: %s", pWndData->showDetails ? L"Yes" : L"No");
                TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
                y += 35;

                if (pWndData->showDetails)
                {
                    const wchar_t* details = L"How This Works:";
                    TextOut(hdc, 10, y, details, (int)wcslen(details));
                    y += 25;

                    const wchar_t* info1 = L"1. Create custom data structure";
                    TextOut(hdc, 10, y, info1, (int)wcslen(info1));
                    y += 20;

                    const wchar_t* info2 = L"2. Pass pointer via CreateWindowEx lpParam";
                    TextOut(hdc, 10, y, info2, (int)wcslen(info2));
                    y += 20;

                    const wchar_t* info3 = L"3. Retrieve in WM_NCCREATE or WM_CREATE from CREATESTRUCT";
                    TextOut(hdc, 10, y, info3, (int)wcslen(info3));
                    y += 20;

                    const wchar_t* info4 = L"4. Store pointer using SetWindowLongPtr(GWLP_USERDATA)";
                    TextOut(hdc, 10, y, info4, (int)wcslen(info4));
                    y += 20;

                    const wchar_t* info5 = L"5. Access later using GetWindowLongPtr(GWLP_USERDATA)";
                    TextOut(hdc, 10, y, info5, (int)wcslen(info5));
                }
            }
            else
            {
                const wchar_t* error = L"No custom data available!";
                TextOut(hdc, 10, 10, error, (int)wcslen(error));
            }

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
        {
            // Clean up if needed
            // Note: In this example, WindowData is on stack in WinMain,
            // so we don't need to free it here.

            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/*
 * Passing Custom Data to Windows - Detailed Explanation:
 *
 * CreateWindowEx lpParam:
 *   The last parameter of CreateWindowEx is lpCreateParams.
 *   This pointer is passed to the window during creation.
 *
 * How to Access:
 *   1. In WM_NCCREATE or WM_CREATE:
 *      CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
 *      MyData* pData = (MyData*)pCreate->lpCreateParams;
 *
 *   2. CREATESTRUCT members:
 *      - lpCreateParams: Your custom data
 *      - hInstance: Application instance
 *      - hMenu: Menu handle
 *      - hwndParent: Parent window
 *      - cy, cx: Height and width
 *      - y, x: Position
 *      - style: Window style
 *      - lpszName: Window title
 *      - lpszClass: Class name
 *      - dwExStyle: Extended style
 *
 * Storing for Later Use:
 *   Use SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pData)
 *   to store pointer in window's user data area.
 *
 *   Retrieve with: GetWindowLongPtr(hwnd, GWLP_USERDATA)
 *
 * Message Order:
 *   1. WM_GETMINMAXINFO (before window is created)
 *   2. WM_NCCREATE (non-client area creation)
 *   3. WM_NCCALCSIZE (calculate client area)
 *   4. WM_CREATE (window creation)
 *   5. WM_SIZE, WM_MOVE, etc.
 *
 * WM_NCCREATE vs WM_CREATE:
 *   - WM_NCCREATE: Earliest message where lpParam is available
 *   - WM_CREATE: More commonly used
 *   - Both receive CREATESTRUCT in lParam
 *
 * Use Cases:
 *   - Pass initialization data
 *   - Create window wrapper classes (C++)
 *   - Initialize window-specific state
 *   - Avoid global variables
 *
 * Memory Management:
 *   - Data must remain valid during window's lifetime
 *   - Allocate on heap or use static/global storage
 *   - Free in WM_DESTROY if heap-allocated
 *
 * This technique is essential for object-oriented window wrappers!
 */
