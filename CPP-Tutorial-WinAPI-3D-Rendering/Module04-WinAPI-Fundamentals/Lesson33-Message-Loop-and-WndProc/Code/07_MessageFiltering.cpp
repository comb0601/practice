/* Lesson 33, Example 07: Message Filtering */
#define UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
BOOL g_filterMouse = FALSE;

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int nShow) {
    WNDCLASSEX wc = {sizeof(wc), CS_HREDRAW|CS_VREDRAW, WindowProc, 0, 0,
                     hInst, 0, LoadCursor(0, IDC_ARROW), (HBRUSH)(COLOR_WINDOW+1),
                     0, L"FilterClass", 0};
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindowEx(0, L"FilterClass", L"Message Filtering Demo",
                                WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                                800, 600, 0, 0, hInst, 0);
    ShowWindow(hwnd, nShow);
    MSG msg = {};
    while (GetMessage(&msg, 0, 0, 0)) {
        // Filter mouse messages if checkbox is checked
        if (g_filterMouse && msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST)
            continue;  // Skip this message
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static DWORD mouseCount = 0;
    switch (uMsg) {
        case WM_CREATE:
            CreateWindowEx(0, L"BUTTON", L"Filter Mouse Messages", 
                          WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
                          10, 10, 200, 25, hwnd, (HMENU)1, GetModuleHandle(0), 0);
            return 0;
        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                HWND hCheck = GetDlgItem(hwnd, 1);
                g_filterMouse = SendMessage(hCheck, BM_GETCHECK, 0, 0) == BST_CHECKED;
            }
            return 0;
        case WM_MOUSEMOVE:
            mouseCount++;
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            wchar_t buf[256];
            wsprintf(buf, L"Mouse move count: %lu", mouseCount);
            TextOut(hdc, 10, 50, buf, (int)wcslen(buf));
            TextOut(hdc, 10, 80, g_filterMouse ? L"Mouse filtering: ON" : L"Mouse filtering: OFF", 23);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
