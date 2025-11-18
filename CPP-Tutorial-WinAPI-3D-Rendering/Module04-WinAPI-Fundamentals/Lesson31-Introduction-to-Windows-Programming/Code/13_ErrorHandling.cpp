/*
 * Lesson 31, Example 13: Error Handling in WinAPI
 *
 * This program demonstrates:
 * - Checking return values from WinAPI functions
 * - Using GetLastError to retrieve error codes
 * - Using FormatMessage to get error descriptions
 * - Proper error handling patterns
 * - Debugging WinAPI failures
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 13_ErrorHandling.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 13_ErrorHandling.cpp -o 13_ErrorHandling.exe -mwindows -lgdi32 -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>
#include <stdio.h>

/*
 * Helper function to display error messages
 */
void ShowError(const wchar_t* context)
{
    // Get the error code from the last function call
    DWORD error = GetLastError();

    if (error == 0)
    {
        MessageBox(NULL, L"No error information available", context, MB_OK);
        return;
    }

    // Format the error message
    LPWSTR errorText = NULL;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |  // Allocate buffer for us
        FORMAT_MESSAGE_FROM_SYSTEM |      // Get message from system
        FORMAT_MESSAGE_IGNORE_INSERTS,    // No insertion parameters
        NULL,                             // No message source
        error,                            // Error code
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  // Default language
        (LPWSTR)&errorText,              // Output buffer
        0,                                // Minimum size (0 = auto)
        NULL                              // No arguments
    );

    if (errorText != NULL)
    {
        // Create full error message
        wchar_t fullMessage[1000];
        swprintf_s(fullMessage, 1000,
                  L"%s\n\nError Code: %lu (0x%08X)\n\nDescription:\n%s",
                  context, error, error, errorText);

        MessageBox(NULL, fullMessage, L"Error", MB_OK | MB_ICONERROR);

        // Free the buffer allocated by FormatMessage
        LocalFree(errorText);
    }
    else
    {
        // FormatMessage failed, show basic info
        wchar_t basicMessage[200];
        swprintf_s(basicMessage, 200,
                  L"%s\n\nError Code: %lu (0x%08X)\n\n"
                  L"Unable to retrieve error description.",
                  context, error, error);

        MessageBox(NULL, basicMessage, L"Error", MB_OK | MB_ICONERROR);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_LBUTTONDOWN:
        {
            // Demonstrate error handling with various scenarios

            // Example 1: File operation that might fail
            HANDLE hFile = CreateFile(
                L"C:\\NonexistentDirectory\\test.txt",
                GENERIC_READ,
                0,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );

            if (hFile == INVALID_HANDLE_VALUE)
            {
                ShowError(L"Failed to open file:\nC:\\NonexistentDirectory\\test.txt");
            }
            else
            {
                MessageBox(hwnd, L"File opened successfully!", L"Success", MB_OK);
                CloseHandle(hFile);
            }

            return 0;
        }

        case WM_RBUTTONDOWN:
        {
            // Example 2: Window creation failure (invalid class name)
            HWND hwndNew = CreateWindow(
                L"NonexistentWindowClass",  // This class doesn't exist
                L"Test Window",
                WS_OVERLAPPEDWINDOW,
                0, 0, 300, 200,
                hwnd, NULL, GetModuleHandle(NULL), NULL
            );

            if (hwndNew == NULL)
            {
                ShowError(L"Failed to create window with nonexistent class");
            }
            else
            {
                ShowWindow(hwndNew, SW_SHOW);
            }

            return 0;
        }

        case WM_KEYDOWN:
        {
            if (wParam == VK_SPACE)
            {
                // Example 3: Invalid window handle
                BOOL result = SetWindowText(
                    (HWND)0x12345678,  // Invalid handle
                    L"New Title"
                );

                if (!result)
                {
                    ShowError(L"Failed to set window text (invalid handle)");
                }
            }
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            const wchar_t* info1 = L"WinAPI Error Handling Demonstration";
            const wchar_t* info2 = L"";
            const wchar_t* info3 = L"Left Click: Try to open nonexistent file";
            const wchar_t* info4 = L"  - Demonstrates GetLastError and FormatMessage";
            const wchar_t* info5 = L"";
            const wchar_t* info6 = L"Right Click: Try to create window with invalid class";
            const wchar_t* info7 = L"  - Shows error handling for window creation";
            const wchar_t* info8 = L"";
            const wchar_t* info9 = L"Press SPACE: Try to use invalid window handle";
            const wchar_t* info10 = L"  - Demonstrates API call failure";
            const wchar_t* info11 = L"";
            const wchar_t* info12 = L"Each error will show:";
            const wchar_t* info13 = L"  - Context of the error";
            const wchar_t* info14 = L"  - Error code (decimal and hex)";
            const wchar_t* info15 = L"  - System error description";

            int y = 10;
            TextOut(hdc, 10, y, info1, (int)wcslen(info1)); y += 25;
            TextOut(hdc, 10, y, info2, (int)wcslen(info2)); y += 20;
            TextOut(hdc, 10, y, info3, (int)wcslen(info3)); y += 20;
            TextOut(hdc, 10, y, info4, (int)wcslen(info4)); y += 20;
            TextOut(hdc, 10, y, info5, (int)wcslen(info5)); y += 20;
            TextOut(hdc, 10, y, info6, (int)wcslen(info6)); y += 20;
            TextOut(hdc, 10, y, info7, (int)wcslen(info7)); y += 20;
            TextOut(hdc, 10, y, info8, (int)wcslen(info8)); y += 20;
            TextOut(hdc, 10, y, info9, (int)wcslen(info9)); y += 20;
            TextOut(hdc, 10, y, info10, (int)wcslen(info10)); y += 20;
            TextOut(hdc, 10, y, info11, (int)wcslen(info11)); y += 20;
            TextOut(hdc, 10, y, info12, (int)wcslen(info12)); y += 20;
            TextOut(hdc, 10, y, info13, (int)wcslen(info13)); y += 20;
            TextOut(hdc, 10, y, info14, (int)wcslen(info14)); y += 20;
            TextOut(hdc, 10, y, info15, (int)wcslen(info15)); y += 20;

            EndPaint(hwnd, &ps);
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
    const wchar_t CLASS_NAME[] = L"ErrorHandlingClass";

    WNDCLASS wc = { };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    // Example: Proper error checking for RegisterClass
    if (!RegisterClass(&wc))
    {
        ShowError(L"Failed to register window class");
        return 1;
    }

    // Example: Proper error checking for CreateWindowEx
    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME,
        L"Error Handling in WinAPI",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 700, 450,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL)
    {
        ShowError(L"Failed to create main window");
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
 * 1. GetLastError():
 *    - Returns error code from last failed API call
 *    - Must be called immediately after failure
 *    - Returns DWORD (0 = success, non-zero = error)
 *    - Error codes defined in winerror.h
 *
 * 2. Common Error Codes:
 *    ERROR_SUCCESS             0     - Success (no error)
 *    ERROR_FILE_NOT_FOUND      2     - File not found
 *    ERROR_PATH_NOT_FOUND      3     - Path not found
 *    ERROR_ACCESS_DENIED       5     - Access denied
 *    ERROR_INVALID_HANDLE      6     - Invalid handle
 *    ERROR_NOT_ENOUGH_MEMORY   8     - Out of memory
 *    ERROR_INVALID_PARAMETER   87    - Invalid parameter
 *    ERROR_CALL_NOT_IMPLEMENTED 120  - Not implemented
 *
 * 3. FormatMessage():
 *    - Converts error code to human-readable string
 *    - Can allocate buffer automatically (FORMAT_MESSAGE_ALLOCATE_BUFFER)
 *    - Buffer must be freed with LocalFree()
 *    - Returns error description from system
 *
 * 4. Error Handling Patterns:
 *
 *    Pattern 1: Check and Display
 *    HWND hwnd = CreateWindow(...);
 *    if (hwnd == NULL) {
 *        ShowError(L"CreateWindow failed");
 *        return 1;
 *    }
 *
 *    Pattern 2: Check Multiple Return Types
 *    // NULL for handles
 *    if (hwnd == NULL) { ... }
 *
 *    // INVALID_HANDLE_VALUE for file handles
 *    if (hFile == INVALID_HANDLE_VALUE) { ... }
 *
 *    // FALSE/0 for BOOL functions
 *    if (!SetWindowText(hwnd, text)) { ... }
 *
 *    // 0 for functions returning counts
 *    int count = GetWindowText(hwnd, buffer, size);
 *    if (count == 0 && GetLastError() != ERROR_SUCCESS) { ... }
 *
 * 5. When to Check Errors:
 *    - After every API call that can fail
 *    - Especially during initialization
 *    - File operations
 *    - Memory allocations
 *    - Window/GDI object creation
 *
 * 6. Return Values to Check:
 *
 *    Returns NULL on failure:
 *    - CreateWindow, CreateWindowEx
 *    - GetDC, BeginPaint (for drawing)
 *    - LoadIcon, LoadCursor, LoadBitmap
 *    - CreateFont, CreatePen, CreateBrush
 *
 *    Returns INVALID_HANDLE_VALUE:
 *    - CreateFile
 *    - FindFirstFile
 *
 *    Returns FALSE/0 on failure:
 *    - RegisterClass
 *    - ShowWindow
 *    - UpdateWindow
 *    - SetWindowText
 *    - DestroyWindow
 *
 *    Returns -1 on failure:
 *    - GetMessage (though -1 is rare, usually just 0 or nonzero)
 *
 * 7. SetLastError:
 *    - Some functions let you clear the error:
 *      SetLastError(ERROR_SUCCESS);
 *    - Useful before calling functions that may not set errors
 *
 * 8. Debugging Tips:
 *
 *    Enable error breaks in Visual Studio:
 *    - Debug -> Exceptions -> Win32 Exceptions -> Check all
 *
 *    Use OutputDebugString for logging:
 *    wchar_t msg[100];
 *    swprintf_s(msg, 100, L"Error: %lu\n", GetLastError());
 *    OutputDebugString(msg);
 *
 *    Check error in debugger:
 *    - Add watch: @err,hr (shows last error)
 *
 * 9. Advanced Error Handling:
 *
 *    Structured Exception Handling (SEH):
 *    __try {
 *        // Code that might fail
 *    }
 *    __except (EXCEPTION_EXECUTE_HANDLER) {
 *        // Handle exception
 *    }
 *
 *    Custom error handler:
 *    LONG WINAPI UnhandledExceptionFilter(EXCEPTION_POINTERS* pExceptionInfo) {
 *        // Log error, show dialog, etc.
 *        return EXCEPTION_EXECUTE_HANDLER;
 *    }
 *    SetUnhandledExceptionFilter(UnhandledExceptionFilter);
 *
 * 10. Common Mistakes:
 *
 *     Mistake 1: Not checking return values
 *     HWND hwnd = CreateWindow(...);  // Might be NULL!
 *     ShowWindow(hwnd, SW_SHOW);      // Crash if hwnd is NULL
 *
 *     Mistake 2: Calling GetLastError too late
 *     CreateFile(...);
 *     DoSomethingElse();              // Might change error code
 *     DWORD error = GetLastError();   // Wrong error!
 *
 *     Mistake 3: Not freeing FormatMessage buffer
 *     LPWSTR errorText = NULL;
 *     FormatMessage(..., &errorText, ...);
 *     // Use errorText
 *     // Missing: LocalFree(errorText);
 *
 *     Mistake 4: Assuming GetLastError is always valid
 *     // Some functions don't set error code on success
 *     // Always check return value first!
 *
 * Best Practices:
 * - Always check return values
 * - Call GetLastError immediately after failure
 * - Use FormatMessage for user-friendly errors
 * - Free allocated error strings
 * - Log errors for debugging
 * - Provide context in error messages
 * - Handle errors gracefully (don't just crash)
 * - Test error paths (how does app behave when things fail?)
 */
