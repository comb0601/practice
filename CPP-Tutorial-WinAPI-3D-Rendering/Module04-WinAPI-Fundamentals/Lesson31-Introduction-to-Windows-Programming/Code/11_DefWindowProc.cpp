/*
 * Lesson 31, Example 11: DefWindowProc - Default Window Procedure
 *
 * This program demonstrates:
 * - The role of DefWindowProc
 * - What happens when you don't call DefWindowProc
 * - Default message handling
 * - Which messages you must handle vs which are optional
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 11_DefWindowProc.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 11_DefWindowProc.cpp -o 11_DefWindowProc.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>

// Toggle to demonstrate difference
BOOL g_useDefWindowProc = TRUE;

/*
 * Window procedure that properly uses DefWindowProc
 */
LRESULT CALLBACK GoodWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            const wchar_t* msg = L"This window CALLS DefWindowProc for unhandled messages";
            TextOut(hdc, 10, 10, msg, (int)wcslen(msg));

            const wchar_t* info1 = L"Try these actions:";
            const wchar_t* info2 = L"- Resize the window (works correctly)";
            const wchar_t* info3 = L"- Move the window (works correctly)";
            const wchar_t* info4 = L"- Minimize/Maximize (works correctly)";
            const wchar_t* info5 = L"- Right-click title bar for system menu (works)";
            const wchar_t* info6 = L"- Press Alt+F4 (closes window)";

            TextOut(hdc, 10, 40, info1, (int)wcslen(info1));
            TextOut(hdc, 10, 60, info2, (int)wcslen(info2));
            TextOut(hdc, 10, 80, info3, (int)wcslen(info3));
            TextOut(hdc, 10, 100, info4, (int)wcslen(info4));
            TextOut(hdc, 10, 120, info5, (int)wcslen(info5));
            TextOut(hdc, 10, 140, info6, (int)wcslen(info6));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_LBUTTONDOWN:
        {
            MessageBox(hwnd, L"Left mouse button clicked!", L"Custom Handler", MB_OK);
            return 0;
        }

        /*
         * IMPORTANT: For all unhandled messages, call DefWindowProc
         * This provides default behavior for:
         * - Window movement
         * - Window resizing
         * - System menu
         * - Minimize/Maximize
         * - And hundreds of other standard behaviors
         */
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

/*
 * Window procedure that DOESN'T use DefWindowProc (BAD!)
 * This demonstrates what happens when you don't call it
 */
LRESULT CALLBACK BadWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            const wchar_t* msg = L"This window DOES NOT call DefWindowProc (BAD!)";
            TextOut(hdc, 10, 10, msg, (int)wcslen(msg));

            const wchar_t* info1 = L"Notice what doesn't work:";
            const wchar_t* info2 = L"- Window won't resize properly";
            const wchar_t* info3 = L"- System menu doesn't work";
            const wchar_t* info4 = L"- Minimize/Maximize buttons don't work";
            const wchar_t* info5 = L"- Alt+F4 doesn't work";
            const wchar_t* info6 = L"";
            const wchar_t* info7 = L"You can still close via WM_DESTROY";

            TextOut(hdc, 10, 40, info1, (int)wcslen(info1));
            TextOut(hdc, 10, 60, info2, (int)wcslen(info2));
            TextOut(hdc, 10, 80, info3, (int)wcslen(info3));
            TextOut(hdc, 10, 100, info4, (int)wcslen(info4));
            TextOut(hdc, 10, 120, info5, (int)wcslen(info5));
            TextOut(hdc, 10, 140, info6, (int)wcslen(info6));
            TextOut(hdc, 10, 160, info7, (int)wcslen(info7));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_CLOSE:
            // At least handle WM_CLOSE so window can be closed!
            DestroyWindow(hwnd);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_LBUTTONDOWN:
        {
            MessageBox(hwnd, L"Left mouse button clicked!", L"Custom Handler", MB_OK);
            return 0;
        }

        /*
         * BAD: Return 0 for all unhandled messages
         * This breaks standard window behavior!
         */
        default:
            return 0;  // DON'T DO THIS!
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    // Create the GOOD window (uses DefWindowProc)
    const wchar_t GOOD_CLASS[] = L"GoodWindowClass";

    WNDCLASS wcGood = { };
    wcGood.lpfnWndProc   = GoodWindowProc;
    wcGood.hInstance     = hInstance;
    wcGood.lpszClassName = GOOD_CLASS;
    wcGood.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcGood.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wcGood);

    HWND hwndGood = CreateWindowEx(
        0, GOOD_CLASS,
        L"GOOD: Uses DefWindowProc",
        WS_OVERLAPPEDWINDOW,
        100, 100, 600, 300,
        NULL, NULL, hInstance, NULL
    );

    // Create the BAD window (doesn't use DefWindowProc)
    const wchar_t BAD_CLASS[] = L"BadWindowClass";

    WNDCLASS wcBad = { };
    wcBad.lpfnWndProc   = BadWindowProc;
    wcBad.hInstance     = hInstance;
    wcBad.lpszClassName = BAD_CLASS;
    wcBad.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcBad.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wcBad);

    HWND hwndBad = CreateWindowEx(
        0, BAD_CLASS,
        L"BAD: Doesn't use DefWindowProc",
        WS_OVERLAPPEDWINDOW,
        150, 150, 600, 300,
        NULL, NULL, hInstance, NULL
    );

    if (!hwndGood || !hwndBad)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    ShowWindow(hwndGood, nCmdShow);
    ShowWindow(hwndBad, nCmdShow);

    MessageBox(NULL,
              L"Two windows created:\n\n"
              L"1. GOOD window - Uses DefWindowProc\n"
              L"   Try resizing, minimizing, system menu\n\n"
              L"2. BAD window - Doesn't use DefWindowProc\n"
              L"   Notice what doesn't work!\n\n"
              L"Click OK to continue...",
              L"DefWindowProc Demonstration",
              MB_OK | MB_ICONINFORMATION);

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
 * 1. DefWindowProc:
 *    LRESULT DefWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
 *    - Provides default processing for all messages
 *    - Must be called for all unhandled messages
 *    - Returns appropriate default value for each message type
 *
 * 2. What DefWindowProc Does:
 *    - Handles window movement (WM_MOVE)
 *    - Handles window resizing (WM_SIZE, WM_SIZING)
 *    - Implements system menu (WM_SYSCOMMAND)
 *    - Handles minimize/maximize buttons
 *    - Processes keyboard shortcuts (Alt+F4, Alt+Space, etc.)
 *    - Handles mouse double-clicks on title bar
 *    - Implements window dragging
 *    - And much more!
 *
 * 3. Messages That Break Without DefWindowProc:
 *    - WM_NCCALCSIZE: Window layout calculation
 *    - WM_NCHITTEST: Hit testing for window areas
 *    - WM_NCACTIVATE: Non-client area activation
 *    - WM_NCPAINT: Non-client area painting
 *    - WM_SYSCOMMAND: System commands (minimize, maximize, close)
 *    - WM_SETCURSOR: Cursor changes
 *    - And hundreds more
 *
 * 4. Proper Pattern:
 *    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
 *    {
 *        switch (uMsg)
 *        {
 *            case WM_PAINT:
 *                // Handle paint
 *                return 0;
 *
 *            case WM_DESTROY:
 *                PostQuitMessage(0);
 *                return 0;
 *
 *            // Handle other messages you care about
 *
 *            default:
 *                return DefWindowProc(hwnd, uMsg, wParam, lParam);  // IMPORTANT!
 *        }
 *    }
 *
 * 5. When to NOT Call DefWindowProc:
 *    - When you've completely handled the message
 *    - Return 0 or other appropriate value
 *    - Examples: WM_PAINT (after BeginPaint/EndPaint)
 *                WM_DESTROY (after PostQuitMessage)
 *                WM_LBUTTONDOWN (if you handled it)
 *
 * 6. Return Values:
 *    Different messages expect different return values:
 *    - WM_PAINT: Return 0
 *    - WM_CREATE: Return 0 (success) or -1 (fail)
 *    - WM_SETCURSOR: Return TRUE if handled, FALSE otherwise
 *    - DefWindowProc returns the correct default value
 *
 * 7. Common Mistakes:
 *
 *    Mistake #1: Forgetting DefWindowProc
 *    default:
 *        return 0;  // BAD! Window won't work properly
 *
 *    Mistake #2: Calling DefWindowProc for handled messages
 *    case WM_PAINT:
 *        // ... handle paint ...
 *        return DefWindowProc(hwnd, uMsg, wParam, lParam);  // BAD!
 *
 *    Mistake #3: Not returning DefWindowProc's result
 *    default:
 *        DefWindowProc(hwnd, uMsg, wParam, lParam);
 *        return 0;  // BAD! Should return DefWindowProc's value
 *
 * 8. Intercepting Messages:
 *    You can intercept a message, do custom processing, then call DefWindowProc:
 *
 *    case WM_SYSCOMMAND:
 *        if (wParam == SC_MINIMIZE)
 *            MessageBox(hwnd, L"Minimizing!", L"Info", MB_OK);
 *        // Still call default handler to actually minimize
 *        return DefWindowProc(hwnd, uMsg, wParam, lParam);
 *
 * Messages You MUST Handle:
 * - WM_DESTROY: Must call PostQuitMessage (for main window)
 * - WM_PAINT: Must call BeginPaint/EndPaint
 *
 * Messages That Are Optional:
 * - Everything else can be handled by DefWindowProc
 * - Only handle what you need to customize
 *
 * Advanced: CallWindowProc
 * - Used for subclassing
 * - Calls original window procedure
 * - Similar to DefWindowProc but for custom chains
 * - Will be covered in advanced lessons
 *
 * Remember:
 * - When in doubt, call DefWindowProc
 * - Return DefWindowProc's return value
 * - Only skip DefWindowProc when you've completely handled the message
 */
