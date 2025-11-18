/*
 * Lesson 31, Example 02: Minimal Window
 *
 * This program creates the simplest possible window. It demonstrates:
 * - Window class registration
 * - Window creation
 * - Message loop
 * - Basic window procedure
 *
 * This is the foundation for all Windows GUI applications.
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 02_MinimalWindow.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 02_MinimalWindow.cpp -o 02_MinimalWindow.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>

/*
 * WindowProc - The window procedure (callback function)
 *
 * This function is called by Windows whenever a message is sent to the window.
 * It processes messages and returns a result.
 *
 * Parameters:
 *   hwnd   - Handle to the window receiving the message
 *   uMsg   - Message identifier (WM_CREATE, WM_PAINT, etc.)
 *   wParam - Additional message information (depends on message type)
 *   lParam - Additional message information (depends on message type)
 *
 * Return:
 *   Result depends on the message type
 */
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
            // Window is being destroyed - post quit message to exit application
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
        {
            // Window needs to be painted
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Fill the client area with the default window color
            // (COLOR_WINDOW+1) is a system color
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_CLOSE:
            // User clicked the X button
            // Ask for confirmation
            if (MessageBox(hwnd, L"Really quit?", L"Confirmation", MB_OKCANCEL) == IDOK)
            {
                DestroyWindow(hwnd);
            }
            return 0;
    }

    // For all other messages, call the default window procedure
    // DefWindowProc provides default processing for all messages
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/*
 * WinMain - Entry point for Windows GUI applications
 */
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    // Step 1: Register the window class
    // A window class defines common properties for all windows of that class

    const wchar_t CLASS_NAME[] = L"MinimalWindowClass";

    WNDCLASS wc = { };  // Initialize to zero

    // Set window class properties
    wc.lpfnWndProc   = WindowProc;      // Pointer to window procedure
    wc.hInstance     = hInstance;        // Instance handle
    wc.lpszClassName = CLASS_NAME;       // Class name (must be unique)

    // Optional: Set additional properties
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);  // Default arrow cursor

    // Register the window class with Windows
    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error",
                   MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    // Step 2: Create the window
    // CreateWindowEx creates an instance of the window class

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles (extended)
        CLASS_NAME,                     // Window class name
        L"Minimal Window - Lesson 31",  // Window title
        WS_OVERLAPPEDWINDOW,           // Window style (standard window with title bar, borders, etc.)

        // Position and size
        CW_USEDEFAULT, CW_USEDEFAULT,  // X, Y position (let Windows decide)
        CW_USEDEFAULT, CW_USEDEFAULT,  // Width, Height (let Windows decide)

        NULL,       // Parent window (NULL = no parent, this is a top-level window)
        NULL,       // Menu (NULL = no menu)
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    // Check if window creation succeeded
    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error",
                   MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    // Step 3: Show the window
    // The window is created but hidden by default
    ShowWindow(hwnd, nCmdShow);  // nCmdShow specifies how to show the window

    // UpdateWindow sends a WM_PAINT message to the window
    // This ensures the window is drawn immediately
    UpdateWindow(hwnd);

    // Step 4: Run the message loop
    // This is the heart of the Windows application
    // It retrieves messages from the application's message queue and dispatches them

    MSG msg = { };  // Initialize message structure

    // GetMessage retrieves a message from the queue
    // Returns TRUE for all messages except WM_QUIT
    // Returns FALSE when WM_QUIT is received (time to exit)
    while (GetMessage(&msg, NULL, 0, 0))
    {
        // TranslateMessage translates virtual-key messages into character messages
        // This is important for text input
        TranslateMessage(&msg);

        // DispatchMessage sends the message to the window procedure
        // Windows will call our WindowProc function
        DispatchMessage(&msg);
    }

    // When GetMessage returns FALSE, the message loop exits
    // msg.wParam contains the exit code from PostQuitMessage
    return (int)msg.wParam;
}

/*
 * Key Concepts Demonstrated:
 *
 * 1. Window Class Registration:
 *    - Every window must belong to a window class
 *    - Window class defines shared properties: window procedure, cursor, icon, etc.
 *    - Must register class before creating windows of that class
 *    - WNDCLASS structure holds window class properties
 *
 * 2. Window Creation:
 *    - CreateWindowEx creates a window instance
 *    - Returns HWND (window handle) - a unique identifier for the window
 *    - Window is created hidden by default
 *    - WS_OVERLAPPEDWINDOW = standard window with title bar, borders, min/max buttons
 *
 * 3. Message Loop:
 *    - GetMessage() - Retrieves message from queue, blocks if queue is empty
 *    - TranslateMessage() - Converts key messages to character messages
 *    - DispatchMessage() - Sends message to window procedure
 *    - Loop continues until GetMessage() returns FALSE (WM_QUIT received)
 *
 * 4. Window Procedure:
 *    - Callback function called by Windows for each message
 *    - Uses switch statement to handle different message types
 *    - Must handle WM_DESTROY (call PostQuitMessage)
 *    - Should handle WM_PAINT (draw window contents)
 *    - Call DefWindowProc for unhandled messages
 *
 * 5. Important Messages:
 *    - WM_DESTROY: Window is being destroyed, time to clean up
 *    - WM_PAINT: Window needs to be redrawn
 *    - WM_CLOSE: User wants to close the window (clicked X)
 *
 * 6. Program Flow:
 *    WinMain
 *      → Register window class
 *      → Create window
 *      → Show window
 *      → Message loop
 *         → GetMessage
 *         → TranslateMessage
 *         → DispatchMessage → WindowProc
 *      → Return when WM_QUIT received
 *
 * 7. Error Handling:
 *    - Check return values (RegisterClass, CreateWindowEx)
 *    - Display error messages using MessageBox
 *    - Return non-zero from WinMain to indicate error
 *
 * This is the template for all Windows applications. Every Windows program
 * follows this basic structure, with variations in the window procedure
 * to handle specific application needs.
 */
