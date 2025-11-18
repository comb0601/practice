/*
 * Lesson 32, Example 01: Basic Window Creation
 *
 * This program demonstrates the complete window creation process:
 * - Defining WNDCLASSEX structure
 * - Registering the window class
 * - Creating a window with CreateWindowEx
 * - Showing and updating the window
 * - Running the message loop
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 01_BasicWindow.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 01_BasicWindow.cpp -o 01_BasicWindow.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>

// Window procedure forward declaration
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/*
 * WinMain - Application entry point
 */
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    /*
     * STEP 1: Register Window Class
     *
     * A window class defines shared properties for all windows
     * of that class: window procedure, icon, cursor, etc.
     */

    const wchar_t CLASS_NAME[] = L"BasicWindowClass";

    // Fill WNDCLASSEX structure
    WNDCLASSEX wcex = { };
    wcex.cbSize = sizeof(WNDCLASSEX);                    // REQUIRED: Structure size
    wcex.style = CS_HREDRAW | CS_VREDRAW;                // Redraw on resize
    wcex.lpfnWndProc = WindowProc;                       // Window procedure
    wcex.cbClsExtra = 0;                                 // No extra class memory
    wcex.cbWndExtra = 0;                                 // No extra window memory
    wcex.hInstance = hInstance;                          // Application instance
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);        // Large icon
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);          // Cursor
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);     // Background brush
    wcex.lpszMenuName = NULL;                            // No menu
    wcex.lpszClassName = CLASS_NAME;                     // Class name
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);      // Small icon

    // Register the window class
    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
                  L"Window class registration failed!\n\nThis usually means:\n"
                  L"  - Invalid structure fields\n"
                  L"  - Class name already registered\n"
                  L"  - System resource exhaustion",
                  L"Registration Error",
                  MB_ICONERROR | MB_OK);
        return 1;
    }

    /*
     * STEP 2: Create Window
     *
     * CreateWindowEx creates an instance of the registered class.
     * This is the actual window that appears on screen.
     */

    HWND hwnd = CreateWindowEx(
        0,                              // dwExStyle: Extended window styles
        CLASS_NAME,                     // lpClassName: Window class name
        L"Basic Window - Lesson 32",    // lpWindowName: Window title
        WS_OVERLAPPEDWINDOW,           // dwStyle: Window style
        CW_USEDEFAULT,                 // x: X position (let Windows decide)
        CW_USEDEFAULT,                 // y: Y position (let Windows decide)
        800,                            // nWidth: Window width
        600,                            // nHeight: Window height
        NULL,                           // hWndParent: No parent window
        NULL,                           // hMenu: No menu
        hInstance,                      // hInstance: Application instance
        NULL                            // lpParam: No additional data
    );

    // Check if window creation succeeded
    if (hwnd == NULL)
    {
        DWORD error = GetLastError();

        wchar_t errorMsg[256];
        swprintf_s(errorMsg, 256,
                  L"Window creation failed!\n\nError code: %lu (0x%08X)\n\n"
                  L"Possible causes:\n"
                  L"  - Invalid class name\n"
                  L"  - System resources exhausted\n"
                  L"  - Invalid parameters",
                  error, error);

        MessageBox(NULL, errorMsg, L"Creation Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    /*
     * STEP 3: Show Window
     *
     * The window is created but hidden by default.
     * ShowWindow makes it visible.
     */

    ShowWindow(hwnd, nCmdShow);  // nCmdShow from WinMain parameter

    /*
     * STEP 4: Update Window
     *
     * UpdateWindow sends WM_PAINT immediately if needed.
     * This ensures the window content is drawn right away.
     */

    UpdateWindow(hwnd);

    /*
     * STEP 5: Message Loop
     *
     * The message loop retrieves and dispatches messages.
     * This is where the application spends most of its time.
     */

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);  // Translate keyboard messages
        DispatchMessage(&msg);    // Dispatch to window procedure
    }

    /*
     * Application exits when WM_QUIT is received.
     * msg.wParam contains the exit code.
     */
    return (int)msg.wParam;
}

/*
 * WindowProc - Window Procedure
 *
 * Handles messages sent to the window.
 */
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
        {
            /*
             * WM_CREATE is sent when the window is being created.
             * This happens during CreateWindowEx, before it returns.
             *
             * Good place for initialization:
             * - Create child windows
             * - Initialize data structures
             * - Allocate resources
             */

            MessageBox(hwnd,
                      L"WM_CREATE received!\n\n"
                      L"The window is being created.\n"
                      L"This message is sent during CreateWindowEx.",
                      L"Window Creation",
                      MB_ICONINFORMATION | MB_OK);

            return 0;  // 0 = success, -1 = fail (destroys window)
        }

        case WM_PAINT:
        {
            /*
             * WM_PAINT is sent when window content needs repainting.
             */

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Display information about window creation
            const wchar_t* title = L"Window Successfully Created!";
            const wchar_t* info1 = L"This window was created with the following steps:";
            const wchar_t* info2 = L"  1. Defined WNDCLASSEX structure";
            const wchar_t* info3 = L"  2. Registered the window class with RegisterClassEx";
            const wchar_t* info4 = L"  3. Created window instance with CreateWindowEx";
            const wchar_t* info5 = L"  4. Showed window with ShowWindow";
            const wchar_t* info6 = L"  5. Updated window with UpdateWindow";
            const wchar_t* info7 = L"  6. Started message loop with GetMessage";

            int y = 10;
            TextOut(hdc, 10, y, title, (int)wcslen(title)); y += 30;
            TextOut(hdc, 10, y, info1, (int)wcslen(info1)); y += 25;
            TextOut(hdc, 10, y, info2, (int)wcslen(info2)); y += 20;
            TextOut(hdc, 10, y, info3, (int)wcslen(info3)); y += 20;
            TextOut(hdc, 10, y, info4, (int)wcslen(info4)); y += 20;
            TextOut(hdc, 10, y, info5, (int)wcslen(info5)); y += 20;
            TextOut(hdc, 10, y, info6, (int)wcslen(info6)); y += 20;
            TextOut(hdc, 10, y, info7, (int)wcslen(info7)); y += 30;

            const wchar_t* footer = L"This is the foundation for all Windows applications!";
            TextOut(hdc, 10, y, footer, (int)wcslen(footer));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
        {
            /*
             * WM_DESTROY is sent when window is being destroyed.
             * Call PostQuitMessage to exit the application.
             */

            PostQuitMessage(0);  // Exit code 0
            return 0;
        }

        case WM_CLOSE:
        {
            /*
             * WM_CLOSE is sent when user clicks the X button.
             * Default behavior (DefWindowProc) calls DestroyWindow.
             */

            int result = MessageBox(hwnd,
                                   L"Are you sure you want to close?",
                                   L"Confirm Exit",
                                   MB_YESNO | MB_ICONQUESTION);

            if (result == IDYES)
            {
                DestroyWindow(hwnd);  // This will trigger WM_DESTROY
            }

            return 0;  // We handled it (don't call DefWindowProc)
        }
    }

    /*
     * DefWindowProc provides default handling for all messages.
     * ALWAYS call this for messages you don't handle.
     */
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/*
 * Key Concepts:
 *
 * 1. Window Class Registration:
 *    - Define window class properties in WNDCLASSEX
 *    - Register with RegisterClassEx
 *    - Registration fails if class name already exists
 *
 * 2. Window Creation:
 *    - Use CreateWindowEx to create window instance
 *    - Returns HWND (window handle) on success
 *    - Returns NULL on failure
 *
 * 3. Window Visibility:
 *    - Windows are created hidden by default
 *    - Use ShowWindow to make visible
 *    - Use UpdateWindow to force immediate paint
 *
 * 4. Message Loop:
 *    - GetMessage retrieves messages from queue
 *    - TranslateMessage processes keyboard input
 *    - DispatchMessage sends to window procedure
 *
 * 5. Window Procedure:
 *    - Callback function that handles messages
 *    - Must call DefWindowProc for unhandled messages
 *    - Returns LRESULT (meaning depends on message)
 *
 * 6. Important Messages:
 *    - WM_CREATE: Window being created
 *    - WM_PAINT: Window needs repainting
 *    - WM_CLOSE: User wants to close
 *    - WM_DESTROY: Window being destroyed
 *
 * 7. Error Handling:
 *    - Always check RegisterClassEx return value
 *    - Always check CreateWindowEx return value
 *    - Use GetLastError() for error details
 *
 * This is the template for ALL Windows applications!
 */
