/*
 * Lesson 32, Example 12: Unregistering Window Classes
 *
 * This program demonstrates proper cleanup by unregistering window classes
 * when they're no longer needed. Also shows class reuse and registration errors.
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 12_UnregisterClass.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 12_UnregisterClass.cpp -o 12_UnregisterClass.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HINSTANCE g_hInstance = NULL;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    g_hInstance = hInstance;

    const wchar_t CLASS_NAME[] = L"UnregisterDemoClass";

    // Register window class
    WNDCLASSEX wcex = { };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = CLASS_NAME;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    ATOM classAtom = RegisterClassEx(&wcex);
    if (!classAtom)
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error", MB_OK);
        return 1;
    }

    // Create window
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Window Class Registration Demo",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_OK);
        UnregisterClass(CLASS_NAME, hInstance);  // Clean up class
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Unregister window class when done
    if (!UnregisterClass(CLASS_NAME, hInstance))
    {
        DWORD error = GetLastError();
        wchar_t errorMsg[256];
        wsprintf(errorMsg,
                 L"UnregisterClass failed!\n\nError code: %lu\n\n"
                 L"This usually means:\n"
                 L"- Windows still exist using this class\n"
                 L"- Class was already unregistered\n"
                 L"- Invalid class name or instance",
                 error);
        MessageBox(NULL, errorMsg, L"Cleanup Error", MB_OK | MB_ICONWARNING);
    }
    else
    {
        MessageBox(NULL,
                  L"Window class successfully unregistered!\n\n"
                  L"The class is now removed from the system and\n"
                  L"cannot be used to create new windows.",
                  L"Cleanup Success",
                  MB_OK | MB_ICONINFORMATION);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
        {
            // Create button to demonstrate class registration errors
            CreateWindowEx(
                0, L"BUTTON", L"Try to Register Same Class Again",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                10, 10, 280, 30,
                hwnd, (HMENU)1, GetModuleHandle(NULL), NULL
            );

            CreateWindowEx(
                0, L"BUTTON", L"Check if Class is Registered",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                300, 10, 250, 30,
                hwnd, (HMENU)2, GetModuleHandle(NULL), NULL
            );

            return 0;
        }

        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            switch (wmId)
            {
                case 1:  // Try to register same class again
                {
                    WNDCLASSEX wcex = { };
                    wcex.cbSize = sizeof(WNDCLASSEX);
                    wcex.style = CS_HREDRAW | CS_VREDRAW;
                    wcex.lpfnWndProc = WindowProc;
                    wcex.hInstance = g_hInstance;
                    wcex.hIcon = LoadIcon(NULL, IDI_WARNING);
                    wcex.hCursor = LoadCursor(NULL, IDC_HAND);
                    wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
                    wcex.lpszClassName = L"UnregisterDemoClass";
                    wcex.hIconSm = LoadIcon(NULL, IDI_WARNING);

                    ATOM result = RegisterClassEx(&wcex);
                    if (!result)
                    {
                        DWORD error = GetLastError();
                        if (error == ERROR_CLASS_ALREADY_EXISTS)
                        {
                            MessageBox(hwnd,
                                      L"RegisterClassEx failed!\n\n"
                                      L"Error: ERROR_CLASS_ALREADY_EXISTS\n\n"
                                      L"A class with this name is already registered.\n"
                                      L"You must unregister it first or use a different name.",
                                      L"Registration Error",
                                      MB_OK | MB_ICONERROR);
                        }
                        else
                        {
                            wchar_t msg[256];
                            wsprintf(msg, L"RegisterClassEx failed with error code: %lu", error);
                            MessageBox(hwnd, msg, L"Error", MB_OK | MB_ICONERROR);
                        }
                    }
                    else
                    {
                        MessageBox(hwnd,
                                  L"This shouldn't happen!\n\n"
                                  L"The class was registered successfully even though\n"
                                  L"a class with the same name already exists.",
                                  L"Unexpected Success",
                                  MB_OK | MB_ICONWARNING);
                    }
                    break;
                }

                case 2:  // Check if class is registered
                {
                    WNDCLASSEX wcex = { };
                    wcex.cbSize = sizeof(WNDCLASSEX);

                    if (GetClassInfoEx(g_hInstance, L"UnregisterDemoClass", &wcex))
                    {
                        wchar_t msg[512];
                        wsprintf(msg,
                                L"Class IS registered!\n\n"
                                L"Class Information:\n"
                                L"  Style: 0x%08X\n"
                                L"  Extra class bytes: %d\n"
                                L"  Extra window bytes: %d\n"
                                L"  Instance: 0x%p\n"
                                L"  Cursor: 0x%p\n"
                                L"  Background: 0x%p",
                                wcex.style,
                                wcex.cbClsExtra,
                                wcex.cbWndExtra,
                                wcex.hInstance,
                                wcex.hCursor,
                                wcex.hbrBackground);
                        MessageBox(hwnd, msg, L"Class Info", MB_OK | MB_ICONINFORMATION);
                    }
                    else
                    {
                        MessageBox(hwnd,
                                  L"Class is NOT registered!\n\n"
                                  L"Either it was never registered, or it was unregistered.",
                                  L"Class Info",
                                  MB_OK | MB_ICONINFORMATION);
                    }
                    break;
                }
            }

            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            int y = 60;

            const wchar_t* title = L"Window Class Registration and Cleanup:";
            TextOut(hdc, 10, y, title, (int)wcslen(title));
            y += 30;

            const wchar_t* info1 = L"When to Unregister Classes:";
            TextOut(hdc, 10, y, info1, (int)wcslen(info1));
            y += 25;

            const wchar_t* info2 = L"  1. When application exits (optional, Windows cleans up)";
            TextOut(hdc, 10, y, info2, (int)wcslen(info2));
            y += 20;

            const wchar_t* info3 = L"  2. When dynamically loading/unloading window classes";
            TextOut(hdc, 10, y, info3, (int)wcslen(info3));
            y += 20;

            const wchar_t* info4 = L"  3. In DLLs during DLL_PROCESS_DETACH";
            TextOut(hdc, 10, y, info4, (int)wcslen(info4));
            y += 30;

            const wchar_t* require = L"Requirements for UnregisterClass:";
            TextOut(hdc, 10, y, require, (int)wcslen(require));
            y += 25;

            const wchar_t* req1 = L"  - All windows using the class must be destroyed first";
            TextOut(hdc, 10, y, req1, (int)wcslen(req1));
            y += 20;

            const wchar_t* req2 = L"  - Must use same HINSTANCE as RegisterClass";
            TextOut(hdc, 10, y, req2, (int)wcslen(req2));
            y += 20;

            const wchar_t* req3 = L"  - Class name must match exactly (case-sensitive)";
            TextOut(hdc, 10, y, req3, (int)wcslen(req3));
            y += 30;

            const wchar_t* errors = L"Common Errors:";
            TextOut(hdc, 10, y, errors, (int)wcslen(errors));
            y += 25;

            const wchar_t* err1 = L"  ERROR_CLASS_ALREADY_EXISTS: Class name in use";
            TextOut(hdc, 10, y, err1, (int)wcslen(err1));
            y += 20;

            const wchar_t* err2 = L"  ERROR_CLASS_DOES_NOT_EXIST: Class not registered";
            TextOut(hdc, 10, y, err2, (int)wcslen(err2));
            y += 20;

            const wchar_t* err3 = L"  ERROR_CLASS_HAS_WINDOWS: Windows still exist using this class";
            TextOut(hdc, 10, y, err3, (int)wcslen(err3));
            y += 30;

            const wchar_t* note = L"Note: Windows automatically unregisters classes when your";
            TextOut(hdc, 10, y, note, (int)wcslen(note));
            y += 20;

            const wchar_t* note2 = L"application exits, so manual cleanup is often optional.";
            TextOut(hdc, 10, y, note2, (int)wcslen(note2));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/*
 * Window Class Registration and Cleanup - Complete Guide:
 *
 * RegisterClass / RegisterClassEx:
 *   ATOM RegisterClass(const WNDCLASS* lpWndClass);
 *   ATOM RegisterClassEx(const WNDCLASSEX* lpWndClass);
 *
 *   Returns: ATOM (unique class identifier) on success, 0 on failure
 *
 * UnregisterClass:
 *   BOOL UnregisterClass(
 *       LPCWSTR lpClassName,   // Class name or atom
 *       HINSTANCE hInstance    // Instance handle
 *   );
 *
 *   Returns: TRUE on success, FALSE on failure
 *
 * GetClassInfo / GetClassInfoEx:
 *   Check if class is registered:
 *
 *   WNDCLASSEX wc = { };
 *   wc.cbSize = sizeof(WNDCLASSEX);
 *   if (GetClassInfoEx(hInstance, L"MyClass", &wc))
 *   {
 *       // Class is registered
 *   }
 *
 * Class Lifetime:
 *   1. Registered with RegisterClass
 *   2. Exists until UnregisterClass or process terminates
 *   3. Can be used to create multiple windows
 *   4. Automatically cleaned up when process exits
 *
 * When to Unregister:
 *   - DLLs: MUST unregister in DLL_PROCESS_DETACH
 *   - Applications: Optional (Windows cleans up automatically)
 *   - Dynamic classes: When no longer needed
 *
 * Multiple Instances:
 *   - Each process has its own class table
 *   - Same class name in different processes is OK
 *   - Same class name in same process causes ERROR_CLASS_ALREADY_EXISTS
 *
 * Global Classes (CS_GLOBALCLASS):
 *   - Available to all processes
 *   - Used by system for standard controls
 *   - Rarely needed in applications
 *
 * Best Practices:
 *   1. Check RegisterClass return value
 *   2. Use GetClassInfo to check if already registered
 *   3. Unregister in DLLs (required)
 *   4. Destroy all windows before unregistering
 *   5. Use unique class names to avoid conflicts
 *
 * Resource Cleanup:
 *   If you created resources for the class (brushes, icons, cursors),
 *   delete them AFTER unregistering the class.
 */
