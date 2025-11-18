/* Lesson 33, Example 06: DefWindowProc - Default Message Handling */
#define UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
DWORD g_handledCount = 0, g_defWndProcCount = 0;

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int nShow) {
    WNDCLASSEX wc = {sizeof(wc), CS_HREDRAW|CS_VREDRAW, WindowProc, 0, 0,
                     hInst, 0, LoadCursor(0, IDC_ARROW), (HBRUSH)(COLOR_WINDOW+1),
                     0, L"DefProcClass", 0};
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindowEx(0, L"DefProcClass", L"DefWindowProc Demo",
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
        case WM_PAINT: {
            g_handledCount++;
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            wchar_t buf[256];
            wsprintf(buf, L"Messages we handled: %lu", g_handledCount);
            TextOut(hdc, 10, 10, buf, (int)wcslen(buf));
            wsprintf(buf, L"Messages passed to DefWindowProc: %lu", g_defWndProcCount);
            TextOut(hdc, 10, 40, buf, (int)wcslen(buf));
            const wchar_t* info = L"DefWindowProc handles: minimize, maximize, resize, move, etc.";
            TextOut(hdc, 10, 80, info, (int)wcslen(info));
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_LBUTTONDOWN:
        case WM_KEYDOWN:
            g_handledCount++;
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            // CRITICAL: Always call DefWindowProc for unhandled messages!
            g_defWndProcCount++;
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}
