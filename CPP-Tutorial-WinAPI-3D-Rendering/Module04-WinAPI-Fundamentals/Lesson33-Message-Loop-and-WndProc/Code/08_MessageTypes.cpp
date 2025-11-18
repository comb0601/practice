/* Lesson 33, Example 08: Message Types and Categories */
#define UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
DWORD g_queuedMsg = 0, g_sentMsg = 0, g_inputMsg = 0, g_systemMsg = 0;

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int nShow) {
    WNDCLASSEX wc = {sizeof(wc), CS_HREDRAW|CS_VREDRAW, WindowProc, 0, 0,
                     hInst, 0, LoadCursor(0, IDC_ARROW), (HBRUSH)(COLOR_WINDOW+1),
                     0, L"MsgTypeClass", 0};
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindowEx(0, L"MsgTypeClass", L"Message Types Demo",
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
    // Categorize messages
    if (uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST || 
        uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST)
        g_inputMsg++;
    else if (uMsg == WM_SIZE || uMsg == WM_MOVE || uMsg == WM_ACTIVATE)
        g_systemMsg++;
    else if (uMsg == WM_PAINT || uMsg == WM_TIMER)
        g_queuedMsg++;
    else if (uMsg >= WM_USER)
        g_sentMsg++;
    
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            wchar_t buf[256];
            int y = 10;
            TextOut(hdc, 10, y, L"Message Categories:", 19); y += 30;
            wsprintf(buf, L"Input messages (mouse/keyboard): %lu", g_inputMsg);
            TextOut(hdc, 10, y, buf, (int)wcslen(buf)); y += 20;
            wsprintf(buf, L"System messages (size/move/activate): %lu", g_systemMsg);
            TextOut(hdc, 10, y, buf, (int)wcslen(buf)); y += 20;
            wsprintf(buf, L"Queued messages (paint/timer): %lu", g_queuedMsg);
            TextOut(hdc, 10, y, buf, (int)wcslen(buf)); y += 20;
            wsprintf(buf, L"User/sent messages: %lu", g_sentMsg);
            TextOut(hdc, 10, y, buf, (int)wcslen(buf));
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
