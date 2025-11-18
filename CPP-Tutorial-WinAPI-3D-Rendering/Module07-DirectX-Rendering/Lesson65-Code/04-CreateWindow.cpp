/*
 * DirectX 11 Tutorial - Lesson 65: DirectX Overview & Setup
 * Example 04: Create Window
 *
 * This program creates a window suitable for DirectX rendering.
 * It demonstrates the Windows window creation process required
 * for all DirectX applications.
 *
 * Concepts demonstrated:
 * - Window class registration
 * - Window creation
 * - Window styles for DirectX
 * - Basic window procedure
 */

#include <windows.h>
#include <string>

// Global window handle
HWND g_hWnd = nullptr;

// Window procedure - handles messages sent to the window
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        // Window is being created
        OutputDebugString(L"WM_CREATE: Window created\n");
        return 0;

    case WM_SIZE:
        {
            // Window has been resized
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);

            wchar_t buffer[256];
            wsprintf(buffer, L"WM_SIZE: Window resized to %dx%d\n", width, height);
            OutputDebugString(buffer);
        }
        return 0;

    case WM_PAINT:
        {
            // Window needs to be painted
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // Fill with a color
            RECT rect;
            GetClientRect(hWnd, &rect);
            HBRUSH brush = CreateSolidBrush(RGB(45, 45, 48)); // Dark gray
            FillRect(hdc, &rect, brush);
            DeleteObject(brush);

            // Draw text
            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkMode(hdc, TRANSPARENT);
            DrawText(hdc, L"DirectX Window Ready", -1, &rect,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            EndPaint(hWnd, &ps);
        }
        return 0;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            // Escape key pressed - close window
            PostQuitMessage(0);
        }
        return 0;

    case WM_DESTROY:
        // Window is being destroyed
        OutputDebugString(L"WM_DESTROY: Window destroyed\n");
        PostQuitMessage(0);
        return 0;

    case WM_CLOSE:
        // User clicked the X button
        OutputDebugString(L"WM_CLOSE: Close requested\n");
        DestroyWindow(hWnd);
        return 0;
    }

    // Default handling for other messages
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// Create and register window class
bool RegisterWindowClass(HINSTANCE hInstance, const wchar_t* className)
{
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;          // Redraw on resize
    wc.lpfnWndProc = WindowProc;                  // Window procedure
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = className;
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(nullptr, L"Window class registration failed!", L"Error", MB_ICONERROR);
        return false;
    }

    return true;
}

// Create the window
bool CreateGameWindow(HINSTANCE hInstance, const wchar_t* className,
                     const wchar_t* title, int width, int height)
{
    // Calculate window size to get desired client area size
    RECT rect = { 0, 0, width, height };
    DWORD style = WS_OVERLAPPEDWINDOW;
    AdjustWindowRect(&rect, style, FALSE);

    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    // Center window on screen
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    // Create the window
    g_hWnd = CreateWindowEx(
        0,                              // Extended style
        className,                      // Class name
        title,                          // Window title
        style,                          // Window style
        posX, posY,                     // Position
        windowWidth, windowHeight,      // Size
        nullptr,                        // Parent window
        nullptr,                        // Menu
        hInstance,                      // Instance handle
        nullptr                         // Additional data
    );

    if (!g_hWnd)
    {
        MessageBox(nullptr, L"Window creation failed!", L"Error", MB_ICONERROR);
        return false;
    }

    // Show and update the window
    ShowWindow(g_hWnd, SW_SHOW);
    UpdateWindow(g_hWnd);

    return true;
}

// Display window information
void ShowWindowInfo()
{
    RECT clientRect, windowRect;
    GetClientRect(g_hWnd, &clientRect);
    GetWindowRect(g_hWnd, &windowRect);

    int clientWidth = clientRect.right - clientRect.left;
    int clientHeight = clientRect.bottom - clientRect.top;
    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

    wchar_t buffer[512];
    wsprintf(buffer,
        L"═══════════════════════════════════\n"
        L"  WINDOW INFORMATION\n"
        L"═══════════════════════════════════\n\n"
        L"Client Area: %dx%d\n"
        L"Window Size: %dx%d\n"
        L"Position: (%d, %d)\n\n"
        L"Handle: 0x%p\n\n"
        L"Press ESC to close window",
        clientWidth, clientHeight,
        windowWidth, windowHeight,
        windowRect.left, windowRect.top,
        g_hWnd);

    MessageBox(g_hWnd, buffer, L"Window Created", MB_OK | MB_ICONINFORMATION);
}

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    const wchar_t* className = L"DirectXWindowClass";
    const wchar_t* windowTitle = L"DirectX 11 Window";
    const int windowWidth = 800;
    const int windowHeight = 600;

    // Register window class
    if (!RegisterWindowClass(hInstance, className))
    {
        return 1;
    }

    // Create window
    if (!CreateGameWindow(hInstance, className, windowTitle, windowWidth, windowHeight))
    {
        return 1;
    }

    // Show window information
    ShowWindowInfo();

    // Message loop
    MSG msg = { 0 };
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}

/*
 * COMPILATION:
 * ===========
 * cl.exe /EHsc 04-CreateWindow.cpp /link user32.lib gdi32.lib /SUBSYSTEM:WINDOWS
 *
 * UNDERSTANDING WINDOW CREATION:
 * =============================
 *
 * Window Class (WNDCLASSEX):
 * -------------------------
 * Defines the behavior and appearance of windows
 * - style: Class styles (CS_HREDRAW | CS_VREDRAW redraws on resize)
 * - lpfnWndProc: Pointer to window procedure function
 * - hInstance: Application instance handle
 * - hIcon: Window icon
 * - hCursor: Mouse cursor
 * - hbrBackground: Background brush
 * - lpszClassName: Unique class name
 *
 * Window Styles:
 * -------------
 * WS_OVERLAPPEDWINDOW includes:
 * - WS_OVERLAPPED: Base style
 * - WS_CAPTION: Title bar
 * - WS_SYSMENU: System menu (X button, minimize, etc.)
 * - WS_THICKFRAME: Resizable border
 * - WS_MINIMIZEBOX: Minimize button
 * - WS_MAXIMIZEBOX: Maximize button
 *
 * For fixed-size DirectX windows, use:
 * WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX
 *
 * Client Area vs Window Size:
 * --------------------------
 * - Client area: The drawable area inside the window
 * - Window size: Includes title bar, borders, etc.
 * - Use AdjustWindowRect() to calculate window size for desired client size
 *
 * Important Messages:
 * ------------------
 * - WM_CREATE: Window is being created
 * - WM_DESTROY: Window is being destroyed
 * - WM_CLOSE: User requested close (X button)
 * - WM_PAINT: Window needs repainting
 * - WM_SIZE: Window has been resized
 * - WM_KEYDOWN: Key pressed
 *
 * Message Loop:
 * ------------
 * GetMessage(): Retrieves message from queue (blocks if empty)
 * TranslateMessage(): Translates virtual-key messages
 * DispatchMessage(): Sends message to window procedure
 *
 * DIRECTX CONSIDERATIONS:
 * ======================
 *
 * 1. Window Size:
 *    - Client area size determines render target size
 *    - Must handle WM_SIZE to resize swap chain
 *
 * 2. Window Styles:
 *    - Fixed size prevents resolution changes during rendering
 *    - Resizable requires swap chain resizing logic
 *
 * 3. Performance:
 *    - Don't block in window procedure
 *    - Handle messages quickly
 *    - Use separate rendering thread for complex apps
 *
 * LEARNING POINTS:
 * ===============
 * 1. Window class must be registered before creating window
 * 2. AdjustWindowRect ensures correct client area size
 * 3. Window procedure handles all window messages
 * 4. Must pump message loop to keep window responsive
 * 5. PostQuitMessage(0) exits the message loop
 *
 * EXERCISES:
 * =========
 * 1. Modify to create a fullscreen window
 * 2. Add handling for Alt+Enter (fullscreen toggle)
 * 3. Implement window resize with minimum/maximum constraints
 * 4. Add a custom icon and cursor
 * 5. Create a borderless window (for custom title bars)
 * 6. Implement multi-monitor support (choose which monitor)
 * 7. Add FPS counter in window title
 * 8. Handle WM_ACTIVATE to pause when window loses focus
 */
