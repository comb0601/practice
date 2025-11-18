/*
 * Lesson 31, Example 14: Unicode in Windows Programming
 *
 * This program demonstrates:
 * - Unicode vs ANSI in WinAPI
 * - Using L"" wide string literals
 * - TCHAR and TEXT macros
 * - Wide character functions (wcslen, wcscpy, etc.)
 * - Displaying international text
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 14_UnicodeWindow.cpp user32.lib gdi32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 14_UnicodeWindow.cpp -o 14_UnicodeWindow.exe -mwindows -lgdi32 -luser32
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

            /*
             * Unicode allows displaying text in any language
             * Using L"" prefix creates wide string literals (wchar_t*)
             */

            // Title
            const wchar_t* title = L"Unicode Support in Windows";
            TextOut(hdc, 10, 10, title, (int)wcslen(title));

            // English
            const wchar_t* english = L"English: Hello, World!";
            TextOut(hdc, 10, 40, english, (int)wcslen(english));

            // Spanish
            const wchar_t* spanish = L"Spanish: ¡Hola, Mundo!";
            TextOut(hdc, 10, 60, spanish, (int)wcslen(spanish));

            // French
            const wchar_t* french = L"French: Bonjour, le monde!";
            TextOut(hdc, 10, 80, french, (int)wcslen(french));

            // German
            const wchar_t* german = L"German: Hallo, Welt!";
            TextOut(hdc, 10, 100, german, (int)wcslen(german));

            // Russian
            const wchar_t* russian = L"Russian: Привет, мир!";
            TextOut(hdc, 10, 120, russian, (int)wcslen(russian));

            // Japanese
            const wchar_t* japanese = L"Japanese: こんにちは、世界！";
            TextOut(hdc, 10, 140, japanese, (int)wcslen(japanese));

            // Chinese
            const wchar_t* chinese = L"Chinese: 你好，世界！";
            TextOut(hdc, 10, 160, chinese, (int)wcslen(chinese));

            // Korean
            const wchar_t* korean = L"Korean: 안녕하세요, 세계!";
            TextOut(hdc, 10, 180, korean, (int)wcslen(korean));

            // Arabic
            const wchar_t* arabic = L"Arabic: مرحبا بالعالم!";
            TextOut(hdc, 10, 200, arabic, (int)wcslen(arabic));

            // Hebrew
            const wchar_t* hebrew = L"Hebrew: שלום, עולם!";
            TextOut(hdc, 10, 220, hebrew, (int)wcslen(hebrew));

            // Greek
            const wchar_t* greek = L"Greek: Γεια σου κόσμε!";
            TextOut(hdc, 10, 240, greek, (int)wcslen(greek));

            // Special characters and symbols
            const wchar_t* symbols = L"Symbols: © ® ™ € £ ¥ § ¶ † ‡ • ◊ ♠ ♣ ♥ ♦";
            TextOut(hdc, 10, 270, symbols, (int)wcslen(symbols));

            // Mathematical symbols
            const wchar_t* math = L"Math: ∑ ∏ √ ∞ ∫ ≈ ≠ ≤ ≥ ± × ÷";
            TextOut(hdc, 10, 290, math, (int)wcslen(math));

            // Demonstrate string manipulation with Unicode
            wchar_t buffer[100];
            swprintf_s(buffer, 100, L"Buffer demo: π = %.2f", 3.14159);
            TextOut(hdc, 10, 320, buffer, (int)wcslen(buffer));

            // Character codes
            const wchar_t* info = L"Unicode character 'A' = U+0041, 'あ' = U+3042";
            TextOut(hdc, 10, 350, info, (int)wcslen(info));

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            // Demonstrate Unicode in MessageBox
            MessageBox(hwnd,
                      L"Unicode MessageBox:\n\n"
                      L"English: Hello!\n"
                      L"Spanish: ¡Hola!\n"
                      L"Japanese: こんにちは!\n"
                      L"Chinese: 你好!\n"
                      L"Russian: Привет!\n"
                      L"Arabic: مرحبا!\n\n"
                      L"All text is Unicode (UTF-16)!",
                      L"Unicode Demo - 国際化",
                      MB_OK | MB_ICONINFORMATION);
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
    /*
     * Always use L"" for string literals in Unicode programs
     * L"text" creates a wide string (wchar_t*)
     */

    const wchar_t CLASS_NAME[] = L"UnicodeWindowClass";

    WNDCLASS wc = { };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL,
                  L"RegisterClass failed!",
                  L"Error",
                  MB_OK | MB_ICONERROR);
        return 1;
    }

    // Unicode in window title
    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME,
        L"Unicode Window - 世界 مرحبا Привет こんにちは",  // Unicode in title!
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 700, 500,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL)
    {
        MessageBox(NULL,
                  L"CreateWindowEx failed!",
                  L"Error",
                  MB_OK | MB_ICONERROR);
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
 * 1. Unicode in Windows:
 *    - Windows uses UTF-16 encoding internally
 *    - Each character is 2 bytes (wchar_t)
 *    - Can represent all languages and symbols
 *    - Recommended for all modern applications
 *
 * 2. Wide String Literals:
 *    - L"text" creates wide string (wchar_t*)
 *    - Without L prefix: narrow string (char*)
 *    - Always use L prefix with UNICODE defined
 *
 *    Examples:
 *    wchar_t* str1 = L"Hello";        // Correct
 *    wchar_t* str2 = "Hello";         // Error! Type mismatch
 *    char* str3 = "Hello";            // Narrow string (ANSI)
 *    char* str4 = L"Hello";           // Error! Type mismatch
 *
 * 3. UNICODE and _UNICODE Macros:
 *    #define UNICODE      // For WinAPI (Windows headers)
 *    #define _UNICODE     // For C runtime (CRT headers)
 *
 *    When defined:
 *    - CreateWindow -> CreateWindowW (wide version)
 *    - MessageBox -> MessageBoxW
 *    - TCHAR -> wchar_t
 *    - TEXT("x") -> L"x"
 *
 * 4. TCHAR and TEXT/L Macros:
 *
 *    Method 1: Explicit Unicode (Recommended):
 *    wchar_t str[] = L"Hello";
 *    CreateWindowW(..., L"Title", ...);
 *
 *    Method 2: Generic (TCHAR):
 *    TCHAR str[] = TEXT("Hello");
 *    CreateWindow(..., TEXT("Title"), ...);
 *
 *    TCHAR expands to:
 *    - wchar_t if UNICODE is defined
 *    - char if UNICODE is not defined
 *
 * 5. String Functions:
 *
 *    ANSI (char*):          Unicode (wchar_t*):
 *    strlen                 wcslen
 *    strcpy                 wcscpy
 *    strcat                 wcscat
 *    strcmp                 wcscmp
 *    sprintf                swprintf
 *    printf                 wprintf
 *
 *    Safe versions (prevent buffer overflows):
 *    strcpy_s               wcscpy_s
 *    strcat_s               wcscat_s
 *    sprintf_s              swprintf_s
 *
 * 6. WinAPI Function Versions:
 *
 *    Most WinAPI functions have two versions:
 *    - FunctionNameA: ANSI version (char*)
 *    - FunctionNameW: Unicode version (wchar_t*)
 *
 *    Examples:
 *    CreateWindowA / CreateWindowW
 *    MessageBoxA / MessageBoxW
 *    GetWindowTextA / GetWindowTextW
 *    SetWindowTextA / SetWindowTextW
 *
 *    The generic name (CreateWindow) is a macro:
 *    #ifdef UNICODE
 *        #define CreateWindow CreateWindowW
 *    #else
 *        #define CreateWindow CreateWindowA
 *    #endif
 *
 * 7. Character Types:
 *
 *    Type        Size    Used for
 *    ----        ----    --------
 *    char        1 byte  ANSI/ASCII characters
 *    wchar_t     2 bytes Unicode characters (UTF-16)
 *    TCHAR       varies  Generic character (char or wchar_t)
 *    WCHAR       2 bytes Unicode character (same as wchar_t)
 *    CHAR        1 byte  ANSI character (same as char)
 *
 * 8. String Types:
 *
 *    Type        Meaning
 *    ----        -------
 *    LPSTR       char* (long pointer to string)
 *    LPCSTR      const char* (long pointer to const string)
 *    LPWSTR      wchar_t* (long pointer to wide string)
 *    LPCWSTR     const wchar_t* (long pointer to const wide string)
 *    LPTSTR      TCHAR* (generic)
 *    LPCTSTR     const TCHAR* (generic)
 *
 * 9. Converting Between ANSI and Unicode:
 *
 *    ANSI to Unicode:
 *    char ansiStr[] = "Hello";
 *    wchar_t wideStr[100];
 *    MultiByteToWideChar(CP_ACP, 0, ansiStr, -1, wideStr, 100);
 *
 *    Unicode to ANSI:
 *    wchar_t wideStr[] = L"Hello";
 *    char ansiStr[100];
 *    WideCharToMultiByte(CP_ACP, 0, wideStr, -1, ansiStr, 100, NULL, NULL);
 *
 * 10. Common Pitfalls:
 *
 *     Pitfall 1: Missing L prefix
 *     wchar_t* str = "Hello";  // Error! Should be L"Hello"
 *
 *     Pitfall 2: Mixing ANSI and Unicode
 *     SetWindowTextW(hwnd, "Title");  // Error! Should be L"Title"
 *
 *     Pitfall 3: Wrong string function
 *     wchar_t str[100];
 *     strlen(str);  // Wrong! Should use wcslen(str)
 *
 *     Pitfall 4: Buffer size confusion
 *     wchar_t buffer[10];  // 10 characters = 20 bytes
 *     // Some functions need character count, others need byte count!
 *
 *     Pitfall 5: Not defining UNICODE
 *     // If UNICODE is not defined, CreateWindow -> CreateWindowA
 *     // But you're using L"" strings (wide) with ANSI functions!
 *
 * 11. Best Practices:
 *
 *     Always:
 *     - Define UNICODE and _UNICODE
 *     - Use L"" prefix for all string literals
 *     - Use wchar_t, LPWSTR, LPCWSTR explicitly
 *     - Use wide string functions (wcslen, wcscpy_s, etc.)
 *     - Use ...W versions explicitly if needed (CreateWindowW)
 *
 *     Avoid:
 *     - TCHAR (explicit is better)
 *     - TEXT() macro (use L"" instead)
 *     - Mixing ANSI and Unicode
 *     - Assuming one character = one byte
 *
 * 12. Format Strings:
 *
 *     With Unicode (swprintf_s):
 *     wchar_t buffer[100];
 *     swprintf_s(buffer, 100, L"Value: %d", 42);
 *     swprintf_s(buffer, 100, L"String: %s", L"text");  // Note: %s expects wchar_t*
 *
 *     Format specifiers:
 *     %s  - string (wchar_t* in wide version)
 *     %d  - integer
 *     %f  - float/double
 *     %x  - hexadecimal
 *     %c  - character
 *
 * 13. File I/O with Unicode:
 *
 *     CreateFileW(L"filename.txt", ...);
 *
 *     Write Unicode text to file:
 *     DWORD written;
 *     WriteFile(hFile, wideString, wcslen(wideString) * sizeof(wchar_t), &written, NULL);
 *
 * 14. Why Use Unicode?
 *
 *     Benefits:
 *     - Support all languages
 *     - Correct text display worldwide
 *     - Future-proof applications
 *     - Better performance (no conversion needed)
 *     - Required for modern Windows
 *
 *     Drawbacks:
 *     - Uses more memory (2 bytes vs 1 byte per character)
 *     - Slightly larger executable
 *     - Must use L"" everywhere
 *
 * Remember:
 * - Modern Windows applications should ALWAYS use Unicode
 * - Define UNICODE and _UNICODE at the project level
 * - Use L"" for all string literals
 * - Use wide string functions (wcslen, wcscpy_s, etc.)
 * - Explicit is better than generic (wchar_t vs TCHAR)
 */
