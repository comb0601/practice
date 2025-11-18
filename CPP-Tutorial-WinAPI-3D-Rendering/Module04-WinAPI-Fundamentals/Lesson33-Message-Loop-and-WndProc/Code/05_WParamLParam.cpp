/* Lesson 33, Example 05: wParam and lParam - Message Parameters */
#define UNICODE
#include <windows.h>
#include <windowsx.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int nShow) {
    WNDCLASSEX wc = {sizeof(wc), CS_HREDRAW|CS_VREDRAW, WindowProc, 0, 0,
                     hInst, 0, LoadCursor(0, IDC_ARROW), (HBRUSH)(COLOR_WINDOW+1),
                     0, L"ParamClass", 0};
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindowEx(0, L"ParamClass", L"wParam/lParam Demo",
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
    static wchar_t lastMsg[512] = L"Click, type, or resize to see message parameters";
    
    switch (uMsg) {
        case WM_LBUTTONDOWN: {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            UINT flags = (UINT)wParam;
            wsprintf(lastMsg, L"WM_LBUTTONDOWN: X=%d, Y=%d, Flags=0x%X (Shift=%s, Ctrl=%s)",
                     x, y, flags, (flags & MK_SHIFT) ? L"Yes" : L"No",
                     (flags & MK_CONTROL) ? L"Yes" : L"No");
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }
        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            wsprintf(lastMsg, L"WM_SIZE: Width=%d, Height=%d, Flag=%Id",
                     width, height, wParam);
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }
        case WM_KEYDOWN: {
            wsprintf(lastMsg, L"WM_KEYDOWN: VirtualKey=0x%X, RepeatCount=%d",
                     (UINT)wParam, (int)(lParam & 0xFFFF));
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            TextOut(hdc, 10, 10, L"Last Message:", 13);
            TextOut(hdc, 10, 40, lastMsg, (int)wcslen(lastMsg));
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
