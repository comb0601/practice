/*
 * Lesson 31, Example 15: System Metrics and Information
 *
 * This program demonstrates:
 * - Using GetSystemMetrics to retrieve system information
 * - Getting screen dimensions
 * - Querying system capabilities
 * - Detecting system features
 * - Using GetSystemInfo for detailed hardware info
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 15_SystemMetrics.cpp user32.lib gdi32.lib kernel32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 15_SystemMetrics.cpp -o 15_SystemMetrics.exe -mwindows -lgdi32 -luser32 -lkernel32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            int y = 10;
            wchar_t buffer[200];

            // Title
            const wchar_t* title = L"System Metrics and Information";
            TextOut(hdc, 10, y, title, (int)wcslen(title));
            y += 30;

            // Screen metrics
            const wchar_t* screenTitle = L"Screen Information:";
            TextOut(hdc, 10, y, screenTitle, (int)wcslen(screenTitle));
            y += 20;

            swprintf_s(buffer, 200, L"  Screen Width: %d pixels", GetSystemMetrics(SM_CXSCREEN));
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            swprintf_s(buffer, 200, L"  Screen Height: %d pixels", GetSystemMetrics(SM_CYSCREEN));
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            swprintf_s(buffer, 200, L"  Primary Monitor Width: %d", GetSystemMetrics(SM_CXPRIMARYMONITOR));
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            swprintf_s(buffer, 200, L"  Primary Monitor Height: %d", GetSystemMetrics(SM_CYPRIMARYMONITOR));
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            swprintf_s(buffer, 200, L"  Number of Monitors: %d", GetSystemMetrics(SM_CMONITORS));
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 25;

            // Window metrics
            const wchar_t* windowTitle = L"Window Information:";
            TextOut(hdc, 10, y, windowTitle, (int)wcslen(windowTitle));
            y += 20;

            swprintf_s(buffer, 200, L"  Title Bar Height: %d pixels", GetSystemMetrics(SM_CYCAPTION));
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            swprintf_s(buffer, 200, L"  Window Border Width: %d pixels", GetSystemMetrics(SM_CXBORDER));
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            swprintf_s(buffer, 200, L"  Window Border Height: %d pixels", GetSystemMetrics(SM_CYBORDER));
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            swprintf_s(buffer, 200, L"  Scrollbar Width: %d pixels", GetSystemMetrics(SM_CXVSCROLL));
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            swprintf_s(buffer, 200, L"  Menu Bar Height: %d pixels", GetSystemMetrics(SM_CYMENU));
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 25;

            // Mouse information
            const wchar_t* mouseTitle = L"Mouse Information:";
            TextOut(hdc, 10, y, mouseTitle, (int)wcslen(mouseTitle));
            y += 20;

            swprintf_s(buffer, 200, L"  Mouse Present: %s",
                      GetSystemMetrics(SM_MOUSEPRESENT) ? L"Yes" : L"No");
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            swprintf_s(buffer, 200, L"  Mouse Buttons: %d", GetSystemMetrics(SM_CMOUSEBUTTONS));
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            swprintf_s(buffer, 200, L"  Mouse Wheel Present: %s",
                      GetSystemMetrics(SM_MOUSEWHEELPRESENT) ? L"Yes" : L"No");
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            swprintf_s(buffer, 200, L"  Swap Mouse Buttons: %s",
                      GetSystemMetrics(SM_SWAPBUTTON) ? L"Yes" : L"No");
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 25;

            // System capabilities
            const wchar_t* capsTitle = L"System Capabilities:";
            TextOut(hdc, 10, y, capsTitle, (int)wcslen(capsTitle));
            y += 20;

            swprintf_s(buffer, 200, L"  Network Present: %s",
                      GetSystemMetrics(SM_NETWORK) ? L"Yes" : L"No");
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            swprintf_s(buffer, 200, L"  Remote Session: %s",
                      GetSystemMetrics(SM_REMOTESESSION) ? L"Yes" : L"No");
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            swprintf_s(buffer, 200, L"  Tablet PC: %s",
                      GetSystemMetrics(SM_TABLETPC) ? L"Yes" : L"No");
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            swprintf_s(buffer, 200, L"  Media Center: %s",
                      GetSystemMetrics(SM_MEDIACENTER) ? L"Yes" : L"No");
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 25;

            // Detailed system information
            SYSTEM_INFO sysInfo;
            GetSystemInfo(&sysInfo);

            const wchar_t* sysTitle = L"Detailed System Information:";
            TextOut(hdc, 10, y, sysTitle, (int)wcslen(sysTitle));
            y += 20;

            swprintf_s(buffer, 200, L"  Number of Processors: %d", sysInfo.dwNumberOfProcessors);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            swprintf_s(buffer, 200, L"  Page Size: %d bytes", sysInfo.dwPageSize);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 20;

            const wchar_t* archStr = L"Unknown";
            switch (sysInfo.wProcessorArchitecture)
            {
                case PROCESSOR_ARCHITECTURE_AMD64:
                    archStr = L"x64 (AMD64/Intel64)";
                    break;
                case PROCESSOR_ARCHITECTURE_ARM:
                    archStr = L"ARM";
                    break;
                case PROCESSOR_ARCHITECTURE_ARM64:
                    archStr = L"ARM64";
                    break;
                case PROCESSOR_ARCHITECTURE_IA64:
                    archStr = L"Intel Itanium";
                    break;
                case PROCESSOR_ARCHITECTURE_INTEL:
                    archStr = L"x86 (Intel 32-bit)";
                    break;
            }

            swprintf_s(buffer, 200, L"  Processor Architecture: %s", archStr);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 25;

            // Version information
            const wchar_t* versionTitle = L"Windows Version Information:";
            TextOut(hdc, 10, y, versionTitle, (int)wcslen(versionTitle));
            y += 20;

            // Note: GetVersion is deprecated but still works
            DWORD version = GetVersion();
            DWORD majorVersion = (DWORD)(LOBYTE(LOWORD(version)));
            DWORD minorVersion = (DWORD)(HIBYTE(LOWORD(version)));

            swprintf_s(buffer, 200, L"  Windows Version: %d.%d", majorVersion, minorVersion);
            TextOut(hdc, 10, y, buffer, (int)wcslen(buffer));
            y += 25;

            // Instructions
            const wchar_t* info = L"Click anywhere to refresh information";
            TextOut(hdc, 10, y, info, (int)wcslen(info));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_LBUTTONDOWN:
            // Refresh the display
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;

        case WM_RBUTTONDOWN:
        {
            // Show additional info in a message box
            wchar_t msg[1000];
            swprintf_s(msg, 1000,
                      L"Additional System Metrics:\n\n"
                      L"Full-Screen Window:\n"
                      L"  Width: %d, Height: %d\n\n"
                      L"Maximized Window:\n"
                      L"  Width: %d, Height: %d\n\n"
                      L"Icon Size:\n"
                      L"  Small: %d x %d\n"
                      L"  Large: %d x %d\n\n"
                      L"Cursor Size: %d x %d\n\n"
                      L"Double-Click Time: %d ms\n\n"
                      L"Keyboard Delay: %d",
                      GetSystemMetrics(SM_CXFULLSCREEN),
                      GetSystemMetrics(SM_CYFULLSCREEN),
                      GetSystemMetrics(SM_CXMAXIMIZED),
                      GetSystemMetrics(SM_CYMAXIMIZED),
                      GetSystemMetrics(SM_CXSMICON),
                      GetSystemMetrics(SM_CYSMICON),
                      GetSystemMetrics(SM_CXICON),
                      GetSystemMetrics(SM_CYICON),
                      GetSystemMetrics(SM_CXCURSOR),
                      GetSystemMetrics(SM_CYCURSOR),
                      GetDoubleClickTime(),
                      GetKeyboardType(1));

            MessageBox(hwnd, msg, L"Additional Metrics", MB_OK | MB_ICONINFORMATION);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"SystemMetricsClass";

    WNDCLASS wc = { };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    // Get screen size for centering
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int windowWidth = 700;
    int windowHeight = 800;

    // Center the window
    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME,
        L"System Metrics and Information",
        WS_OVERLAPPEDWINDOW,
        x, y, windowWidth, windowHeight,
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

    return (int)msg.wParam;
}

/*
 * Key Concepts Demonstrated:
 *
 * 1. GetSystemMetrics():
 *    int GetSystemMetrics(int nIndex)
 *    - Returns various system measurements and configuration settings
 *    - Takes SM_* constant as parameter
 *    - Returns integer value
 *
 * 2. Common System Metrics:
 *
 *    Screen Metrics:
 *    SM_CXSCREEN           - Screen width in pixels
 *    SM_CYSCREEN           - Screen height in pixels
 *    SM_CXFULLSCREEN       - Full-screen window width
 *    SM_CYFULLSCREEN       - Full-screen window height
 *    SM_CXPRIMARYMONITOR   - Primary monitor width
 *    SM_CYPRIMARYMONITOR   - Primary monitor height
 *    SM_CMONITORS          - Number of monitors
 *
 *    Window Metrics:
 *    SM_CYCAPTION          - Title bar height
 *    SM_CXBORDER           - Window border width
 *    SM_CYBORDER           - Window border height
 *    SM_CXFRAME            - Window frame width
 *    SM_CYFRAME            - Window frame height
 *    SM_CXMIN              - Minimum window width
 *    SM_CYMIN              - Minimum window height
 *
 *    Control Metrics:
 *    SM_CXVSCROLL          - Vertical scrollbar width
 *    SM_CYHSCROLL          - Horizontal scrollbar height
 *    SM_CYMENU             - Menu bar height
 *    SM_CXICON             - Large icon width
 *    SM_CYICON             - Large icon height
 *    SM_CXSMICON           - Small icon width
 *    SM_CYSMICON           - Small icon height
 *
 *    Input Device Info:
 *    SM_MOUSEPRESENT       - Mouse installed (1=yes, 0=no)
 *    SM_CMOUSEBUTTONS      - Number of mouse buttons
 *    SM_MOUSEWHEELPRESENT  - Mouse wheel present
 *    SM_SWAPBUTTON         - Left/right buttons swapped
 *
 *    System Features:
 *    SM_NETWORK            - Network present
 *    SM_REMOTESESSION      - Running in remote session (RDP)
 *    SM_TABLETPC           - Tablet PC
 *    SM_MEDIACENTER        - Windows Media Center
 *    SM_STARTER            - Starter Edition
 *
 * 3. GetSystemInfo():
 *    void GetSystemInfo(LPSYSTEM_INFO lpSystemInfo)
 *    - Fills SYSTEM_INFO structure with detailed information
 *
 *    SYSTEM_INFO fields:
 *    - wProcessorArchitecture  - CPU architecture
 *    - dwPageSize              - Memory page size
 *    - lpMinimumApplicationAddress - Lowest memory address
 *    - lpMaximumApplicationAddress - Highest memory address
 *    - dwActiveProcessorMask   - Active processor mask
 *    - dwNumberOfProcessors    - Number of logical processors
 *    - dwProcessorType         - Processor type (legacy)
 *    - dwAllocationGranularity - Memory allocation granularity
 *    - wProcessorLevel         - Processor level
 *    - wProcessorRevision      - Processor revision
 *
 * 4. Processor Architectures:
 *    PROCESSOR_ARCHITECTURE_INTEL  - x86 (32-bit Intel)
 *    PROCESSOR_ARCHITECTURE_AMD64  - x64 (64-bit AMD/Intel)
 *    PROCESSOR_ARCHITECTURE_ARM    - ARM (32-bit)
 *    PROCESSOR_ARCHITECTURE_ARM64  - ARM64 (64-bit)
 *    PROCESSOR_ARCHITECTURE_IA64   - Intel Itanium
 *
 * 5. Other System Information Functions:
 *
 *    GetVersion():
 *    - Returns Windows version (deprecated but still works)
 *    - Use VerifyVersionInfo or IsWindows* helpers instead
 *
 *    GetVersionEx():
 *    - Returns detailed version info (deprecated)
 *
 *    GetComputerName():
 *    - Returns computer name
 *
 *    GetUserName():
 *    - Returns current user name
 *
 *    GetDoubleClickTime():
 *    - Returns double-click time in milliseconds
 *
 *    GetKeyboardType():
 *    - Returns keyboard type information
 *
 *    SystemParametersInfo():
 *    - Gets/sets system-wide parameters
 *    - Very versatile function
 *
 * 6. Using Metrics for Layout:
 *
 *    Center window on screen:
 *    int screenW = GetSystemMetrics(SM_CXSCREEN);
 *    int screenH = GetSystemMetrics(SM_CYSCREEN);
 *    int x = (screenW - windowWidth) / 2;
 *    int y = (screenH - windowHeight) / 2;
 *
 *    Calculate client size from window size:
 *    RECT rc = { 0, 0, clientWidth, clientHeight };
 *    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
 *    int windowWidth = rc.right - rc.left;
 *    int windowHeight = rc.bottom - rc.top;
 *
 * 7. SystemParametersInfo():
 *    BOOL SystemParametersInfo(
 *        UINT  uiAction,   // Action to perform
 *        UINT  uiParam,    // Action-specific parameter
 *        PVOID pvParam,    // Action-specific parameter
 *        UINT  fWinIni     // Update flags
 *    )
 *
 *    Examples:
 *    Get work area (screen minus taskbar):
 *    RECT workArea;
 *    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
 *
 *    Get screen saver timeout:
 *    int timeout;
 *    SystemParametersInfo(SPI_GETSCREENSAVETIMEOUT, 0, &timeout, 0);
 *
 *    Get animation effects:
 *    BOOL animation;
 *    SystemParametersInfo(SPI_GETCLIENTAREAANIMATION, 0, &animation, 0);
 *
 * 8. Monitor Information:
 *
 *    For multi-monitor setups, use:
 *    - EnumDisplayMonitors: Enumerate monitors
 *    - GetMonitorInfo: Get info about specific monitor
 *    - MonitorFromWindow: Get monitor containing window
 *    - MonitorFromPoint: Get monitor containing point
 *
 *    Example:
 *    HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
 *    MONITORINFO mi = { sizeof(MONITORINFO) };
 *    GetMonitorInfo(hMonitor, &mi);
 *    // mi.rcMonitor contains monitor rectangle
 *    // mi.rcWork contains work area (minus taskbar)
 *
 * 9. DPI Awareness:
 *
 *    Modern Windows supports different DPI settings.
 *    Use these for DPI-aware applications:
 *
 *    Get DPI:
 *    HDC hdc = GetDC(NULL);
 *    int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
 *    int dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
 *    ReleaseDC(NULL, hdc);
 *
 *    Or use GetDpiForWindow (Windows 10 1607+):
 *    UINT dpi = GetDpiForWindow(hwnd);
 *
 * 10. Common Use Cases:
 *
 *     Centering window:
 *     int x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
 *
 *     Maximizing to work area:
 *     RECT workArea;
 *     SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
 *     SetWindowPos(hwnd, NULL,
 *                  workArea.left, workArea.top,
 *                  workArea.right - workArea.left,
 *                  workArea.bottom - workArea.top,
 *                  SWP_NOZORDER);
 *
 *     Checking if running in RDP:
 *     if (GetSystemMetrics(SM_REMOTESESSION)) {
 *         // Disable fancy graphics
 *     }
 *
 *     Detecting touch support:
 *     if (GetSystemMetrics(SM_DIGITIZER) & NID_READY) {
 *         // Touch is available
 *     }
 *
 * Best Practices:
 * - Don't hardcode screen dimensions
 * - Use GetSystemMetrics for UI layout
 * - Check capabilities before using features
 * - Handle multiple monitors properly
 * - Be DPI-aware for modern Windows
 * - Cache metrics if used frequently
 * - Handle WM_SETTINGCHANGE to detect metric changes
 */
