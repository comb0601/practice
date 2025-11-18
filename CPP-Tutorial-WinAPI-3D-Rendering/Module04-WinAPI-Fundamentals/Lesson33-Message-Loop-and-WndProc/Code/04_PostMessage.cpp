/* Lesson 33, Example 04: PostMessage - Asynchronous Communication */
#define UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
DWORD g_postedMsgCount = 0;

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int nShow) {
    WNDCLASSEX wc = {sizeof(wc), CS_HREDRAW|CS_VREDRAW, WindowProc, 0, 0,
                     hInst, 0, LoadCursor(0, IDC_ARROW), (HBRUSH)(COLOR_WINDOW+1),
                     0, L"PostMsgClass", 0};
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindowEx(0, L"PostMsgClass", L"PostMessage Demo",
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
            CreateWindowEx(0, L"BUTTON", L"Post 10 Messages", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                          10, 10, 150, 30, hwnd, (HMENU)1, GetModuleHandle(0), 0);
            return 0;
        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                // PostMessage: Asynchronous, returns immediately
                for (int i = 0; i < 10; i++)
                    PostMessage(hwnd, WM_USER + 200, i, 0);
                MessageBox(hwnd, L"Posted 10 messages! They'll be processed async.", L"Posted", MB_OK);
            }
            return 0;
        case WM_USER + 200:
            g_postedMsgCount++;
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            wchar_t buf[256];
            wsprintf(buf, L"Posted messages received: %lu", g_postedMsgCount);
            TextOut(hdc, 10, 60, buf, (int)wcslen(buf));
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
