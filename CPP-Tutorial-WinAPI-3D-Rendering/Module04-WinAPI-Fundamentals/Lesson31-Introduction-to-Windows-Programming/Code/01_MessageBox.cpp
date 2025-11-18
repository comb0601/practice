/*
 * Lesson 31, Example 01: MessageBox
 *
 * This is the simplest possible Windows program. It demonstrates:
 * - WinMain entry point
 * - Displaying a message box
 * - Different message box styles and icons
 * - Return values from MessageBox
 *
 * Compile: cl /D "UNICODE" /D "_UNICODE" 01_MessageBox.cpp user32.lib
 * Or with g++: g++ -D UNICODE -D _UNICODE 01_MessageBox.cpp -o 01_MessageBox.exe -mwindows -luser32
 */

#define UNICODE
#define _UNICODE
#include <windows.h>

/*
 * WinMain - Entry point for Windows GUI applications
 *
 * Parameters:
 *   hInstance     - Handle to the current instance of the application
 *   hPrevInstance - Always NULL (legacy from 16-bit Windows)
 *   lpCmdLine     - Command line arguments as a string
 *   nCmdShow      - How the window should be shown (SW_SHOW, SW_HIDE, etc.)
 */
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR lpCmdLine, int nCmdShow)
{
    // Display a simple message box
    // MessageBox returns the button clicked by the user

    MessageBox(
        NULL,                           // hwndParent: No parent window
        L"Welcome to Windows Programming!\n\n"
        L"This is your first WinAPI program.",  // Text to display
        L"Hello Windows",               // Title bar text
        MB_OK | MB_ICONINFORMATION     // OK button with info icon
    );

    // Demonstrate different message box types
    int result = MessageBox(
        NULL,
        L"Do you want to continue?",
        L"Question",
        MB_YESNO | MB_ICONQUESTION    // Yes/No buttons with question icon
    );

    // Check which button was clicked
    if (result == IDYES) {
        MessageBox(NULL, L"You clicked YES!", L"Result", MB_OK);
    } else {
        MessageBox(NULL, L"You clicked NO!", L"Result", MB_OK);
    }

    // Demonstrate more message box styles
    MessageBox(NULL,
               L"This is an error message!",
               L"Error Example",
               MB_OK | MB_ICONERROR);       // Error icon (red X)

    MessageBox(NULL,
               L"This is a warning message!",
               L"Warning Example",
               MB_OK | MB_ICONWARNING);     // Warning icon (yellow !)

    // MessageBox with multiple buttons
    result = MessageBox(NULL,
                       L"Would you like to save?",
                       L"Save Changes",
                       MB_YESNOCANCEL | MB_ICONQUESTION);

    // Check the result
    switch (result) {
        case IDYES:
            MessageBox(NULL, L"Saving...", L"Info", MB_OK);
            break;
        case IDNO:
            MessageBox(NULL, L"Not saving.", L"Info", MB_OK);
            break;
        case IDCANCEL:
            MessageBox(NULL, L"Cancelled.", L"Info", MB_OK);
            break;
    }

    // Final goodbye message
    MessageBox(NULL,
               L"Program completed!\n\n"
               L"MessageBox is useful for:\n"
               L"  - Displaying information\n"
               L"  - Asking questions\n"
               L"  - Showing errors\n"
               L"  - Debugging output",
               L"Goodbye",
               MB_OK | MB_ICONINFORMATION);

    // Return 0 to indicate success
    return 0;
}

/*
 * Key Concepts Demonstrated:
 *
 * 1. WinMain - The entry point for Windows GUI applications
 *    - Must use WINAPI calling convention
 *    - Takes 4 parameters (hInstance, hPrevInstance, lpCmdLine, nCmdShow)
 *
 * 2. MessageBox - Simple way to display information
 *    - First parameter: Parent window handle (NULL = no parent)
 *    - Second parameter: Message text
 *    - Third parameter: Title text
 *    - Fourth parameter: Style flags (buttons and icon)
 *
 * 3. MessageBox return values:
 *    - IDOK     = OK button clicked
 *    - IDCANCEL = Cancel button clicked
 *    - IDYES    = Yes button clicked
 *    - IDNO     = No button clicked
 *    - IDRETRY  = Retry button clicked
 *    - IDABORT  = Abort button clicked
 *
 * 4. MessageBox button styles:
 *    - MB_OK            = Single OK button
 *    - MB_OKCANCEL      = OK and Cancel buttons
 *    - MB_YESNO         = Yes and No buttons
 *    - MB_YESNOCANCEL   = Yes, No, and Cancel buttons
 *    - MB_RETRYCANCEL   = Retry and Cancel buttons
 *    - MB_ABORTRETRYIGNORE = Abort, Retry, and Ignore buttons
 *
 * 5. MessageBox icon styles:
 *    - MB_ICONERROR       = Error icon (red X)
 *    - MB_ICONWARNING     = Warning icon (yellow !)
 *    - MB_ICONINFORMATION = Information icon (blue i)
 *    - MB_ICONQUESTION    = Question icon (?)
 *
 * 6. Unicode:
 *    - L"text" creates a wide string (wchar_t)
 *    - Required when UNICODE is defined
 *    - Modern Windows applications should always use Unicode
 *
 * Usage:
 * This program doesn't create a window; it only shows message boxes.
 * Each MessageBox blocks until the user responds.
 * Useful for simple notifications or debugging.
 */
