/* Lesson 33, Example 03: SendMessage - Synchronous Communication
 * Demonstrates SendMessage for immediate message processing */
#define UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int nShow) {
    WNDCLASSEX wc = {sizeof(wc), CS_HREDRAW|CS_VREDRAW, WindowProc, 0, 0, 
                     hInst, 0, LoadCursor(0, IDC_ARROW), (HBRUSH)(COLOR_WINDOW+1), 
                     0, L"SendMsgClass", 0};
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindowEx(0, L"SendMsgClass", L"SendMessage Demo",
                                WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                                800, 600, 0, 0, hInst, 0);
    ShowWindow(hwnd, nShow);
    MSG msg = {};
    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            CreateWindowEx(0, L"BUTTON", L"Send Message", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                          10, 10, 150, 30, hwnd, (HMENU)1, GetModuleHandle(0), 0);
            return 0;
        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                // SendMessage: Synchronous, waits for result
                LRESULT result = SendMessage(hwnd, WM_USER + 100, 42, 0);
                wchar_t buf[256];
                wsprintf(buf, L"SendMessage returned: %Id", result);
                MessageBox(hwnd, buf, L"Result", MB_OK);
            }
            return 0;
        case WM_USER + 100:
            // Custom message handler
            MessageBox(hwnd, L"Processing custom message...", L"Info", MB_OK);
            return wParam * 2;  // Return value received by SendMessage
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
