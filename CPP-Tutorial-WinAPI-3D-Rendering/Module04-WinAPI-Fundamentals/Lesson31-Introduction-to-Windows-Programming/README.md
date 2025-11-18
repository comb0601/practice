# Lesson 31: Introduction to Windows Programming

## Table of Contents
1. [Introduction](#introduction)
2. [Windows Architecture Overview](#windows-architecture-overview)
3. [Message-Driven Programming Model](#message-driven-programming-model)
4. [The Windows API (WinAPI)](#the-windows-api-winapi)
5. [Key Concepts and Terminology](#key-concepts-and-terminology)
6. [WinMain: The Entry Point](#winmain-the-entry-point)
7. [Handles: Windows' Object Management](#handles-windows-object-management)
8. [Unicode vs ANSI](#unicode-vs-ansi)
9. [Common Data Types](#common-data-types)
10. [Error Handling in Windows](#error-handling-in-windows)
11. [Development Environment Setup](#development-environment-setup)
12. [Your First Windows Program](#your-first-windows-program)
13. [Practical Examples](#practical-examples)
14. [Best Practices](#best-practices)
15. [Summary](#summary)

## Introduction

Welcome to Windows programming! This lesson marks your transition from console-based C++ programming to the world of graphical user interface (GUI) development using the Windows Application Programming Interface (WinAPI).

Windows programming is fundamentally different from console programming. Instead of a linear execution flow with sequential input/output, Windows applications operate in an event-driven paradigm where the application responds to messages from the operating system. This lesson will introduce you to the core concepts, architecture, and mindset required for Windows application development.

**Learning Objectives:**
- Understand the Windows operating system architecture and how applications interact with it
- Learn the message-driven programming model that powers all Windows applications
- Master key WinAPI concepts including handles, window procedures, and the message loop
- Understand the difference between ANSI and Unicode programming
- Create simple Windows applications that respond to system messages
- Develop a solid foundation for advanced Windows programming topics

**Time Required:** 4-5 hours

## Windows Architecture Overview

### The Layered Architecture

Windows uses a layered architecture that separates user-mode applications from kernel-mode system components:

```
┌─────────────────────────────────────────────┐
│        User Applications (Your Code)        │
├─────────────────────────────────────────────┤
│         Windows API (WinAPI/Win32)          │
├─────────────────────────────────────────────┤
│    Subsystem DLLs (USER32, GDI32, etc.)    │
├─────────────────────────────────────────────┤
│         Executive Services (Kernel)         │
├─────────────────────────────────────────────┤
│      Hardware Abstraction Layer (HAL)       │
├─────────────────────────────────────────────┤
│               Hardware                       │
└─────────────────────────────────────────────┘
```

**Key Layers Explained:**

1. **Hardware Layer:** The physical components (CPU, memory, devices)
2. **HAL (Hardware Abstraction Layer):** Provides a consistent interface to hardware, abstracting platform-specific details
3. **Windows Kernel (Executive):** The core of the operating system that manages memory, processes, threads, security, and I/O
4. **Subsystem DLLs:** System libraries that provide Windows API functionality:
   - **USER32.DLL:** Window management, messages, input
   - **GDI32.DLL:** Graphics Device Interface for drawing
   - **KERNEL32.DLL:** Memory management, file operations, threading
   - **ADVAPI32.DLL:** Advanced services (registry, security)
5. **Windows API:** The programming interface exposed to developers
6. **User Applications:** Your programs that use the WinAPI

### User Mode vs Kernel Mode

Windows separates execution into two privilege levels:

**User Mode:**
- Where your application runs
- Limited access to system resources
- Cannot directly access hardware or kernel memory
- Protected from other applications
- Crashes in user mode don't crash the entire system

**Kernel Mode:**
- Where the Windows kernel and device drivers run
- Full access to all system resources and hardware
- Can execute privileged instructions
- Crashes in kernel mode cause system-wide failures (Blue Screen of Death)

Your Windows applications run entirely in user mode and interact with kernel mode through system calls via the WinAPI.

### The Window Manager

The Window Manager is a crucial component of Windows that:
- Creates and manages windows (visual rectangles on screen)
- Routes input events (mouse, keyboard) to appropriate windows
- Handles window painting and rendering
- Manages the Z-order (which window is on top)
- Implements the message queue system

Every Windows application interacts heavily with the Window Manager through WinAPI functions.

## Message-Driven Programming Model

### The Event-Driven Paradigm

Unlike console applications that follow a linear flow, Windows applications are **event-driven**. Your program doesn't control the flow of execution; instead, it responds to events (messages) sent by the operating system.

**Console Application Flow:**
```cpp
int main() {
    // Linear execution
    cout << "Enter name: ";
    cin >> name;
    cout << "Hello, " << name;
    return 0;
}
```

**Windows Application Flow:**
```cpp
int WINAPI WinMain(...) {
    // Create window
    // Enter message loop
    while (GetMessage(...)) {
        // Process messages
        DispatchMessage(...);
    }
    // Application ends when message loop exits
}
```

### What Are Messages?

A **message** is a data structure that describes an event or action. Messages are represented by the `MSG` structure:

```cpp
typedef struct tagMSG {
    HWND   hwnd;      // Window that receives the message
    UINT   message;   // Message identifier (WM_PAINT, WM_CLOSE, etc.)
    WPARAM wParam;    // Additional message info (16/32/64-bit)
    LPARAM lParam;    // Additional message info (32/64-bit)
    DWORD  time;      // Time when message was posted
    POINT  pt;        // Mouse cursor position
} MSG;
```

### Common Message Types

Windows defines hundreds of message types. Here are some of the most important:

| Message | Meaning | When Sent |
|---------|---------|-----------|
| `WM_CREATE` | Window is being created | During CreateWindow() call |
| `WM_DESTROY` | Window is being destroyed | When window is closed |
| `WM_PAINT` | Window needs repainting | When window content needs updating |
| `WM_SIZE` | Window has been resized | After user resizes window |
| `WM_CLOSE` | User requests to close window | When user clicks X button |
| `WM_COMMAND` | Menu or control action | When button clicked, menu selected |
| `WM_KEYDOWN` | Key pressed | When user presses a key |
| `WM_LBUTTONDOWN` | Left mouse button pressed | When user clicks left button |
| `WM_MOUSEMOVE` | Mouse moved | When user moves mouse |
| `WM_TIMER` | Timer elapsed | At regular intervals if timer set |

### Message Queues

Windows maintains two types of message queues:

**1. System Message Queue (Global):**
- Maintained by the operating system
- Receives all input events (keyboard, mouse)
- Windows routes messages to appropriate application queues

**2. Application Message Queue (Per-Thread):**
- Each GUI thread has its own message queue
- Receives messages from the system queue
- Your application retrieves messages from this queue

### The Message Loop

The message loop is the heart of every Windows application:

```cpp
MSG msg;
// Keep retrieving messages until WM_QUIT is received
while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);  // Translates virtual-key messages
    DispatchMessage(&msg);    // Sends message to window procedure
}
```

**How It Works:**

1. **GetMessage()**: Retrieves a message from the application queue
   - Blocks if no messages are available (yields CPU to other apps)
   - Returns FALSE when WM_QUIT is received, ending the loop
   - Returns TRUE for all other messages

2. **TranslateMessage()**: Translates virtual-key messages
   - Converts WM_KEYDOWN/WM_KEYUP into WM_CHAR messages
   - Essential for text input

3. **DispatchMessage()**: Sends the message to the window procedure
   - Windows calls your window procedure with the message
   - Your code handles the message and returns

### The Window Procedure (WndProc)

Every window has a **window procedure**—a callback function that processes messages:

```cpp
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            // Handle painting
            return 0;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
        default:
            // Let Windows handle unprocessed messages
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}
```

**Key Points:**
- The window procedure is called by Windows (not by your code directly)
- You only handle messages you care about
- Call `DefWindowProc()` for all unhandled messages
- Must return a value (usually 0 for handled messages)

### Message Flow Diagram

```
┌──────────────┐
│ User Action  │ (Click, Type, Move Mouse)
└──────┬───────┘
       │
       ▼
┌──────────────────┐
│ System Queue     │
└──────┬───────────┘
       │
       ▼
┌──────────────────┐
│ Application      │
│ Message Queue    │
└──────┬───────────┘
       │
       ▼
┌──────────────────┐
│ GetMessage()     │
└──────┬───────────┘
       │
       ▼
┌──────────────────┐
│ TranslateMessage │
└──────┬───────────┘
       │
       ▼
┌──────────────────┐
│ DispatchMessage  │
└──────┬───────────┘
       │
       ▼
┌──────────────────┐
│ Window Procedure │ (Your Code)
└──────────────────┘
```

## The Windows API (WinAPI)

### What Is WinAPI?

The Windows Application Programming Interface (WinAPI), also called Win32 API, is a collection of C functions and data structures that allows programs to interact with the Windows operating system.

**Key Characteristics:**
- C-based API (not C++ objects)
- Provides access to operating system features
- Thousands of functions across multiple DLLs
- Consistent naming conventions
- Platform-specific (Windows only)

### WinAPI Organization

The WinAPI is organized into several major categories:

**1. Window Management (USER32.DLL):**
```cpp
CreateWindow()     // Creates a window
ShowWindow()       // Shows/hides a window
DestroyWindow()    // Destroys a window
GetMessage()       // Retrieves a message
SendMessage()      // Sends a message to a window
```

**2. Graphics Device Interface (GDI32.DLL):**
```cpp
BeginPaint()       // Begins painting
TextOut()          // Draws text
LineTo()           // Draws a line
Rectangle()        // Draws a rectangle
SelectObject()     // Selects a GDI object
```

**3. Kernel Services (KERNEL32.DLL):**
```cpp
CreateFile()       // Opens/creates a file
ReadFile()         // Reads from a file
VirtualAlloc()     // Allocates memory
CreateThread()     // Creates a thread
Sleep()            // Suspends execution
```

**4. Common Controls (COMCTL32.DLL):**
```cpp
CreateStatusBar()  // Creates a status bar
ListView_*()       // List view functions
TreeView_*()       // Tree view functions
```

### Naming Conventions

WinAPI follows consistent naming patterns:

**Hungarian Notation:**
- `h` prefix: Handles (hWnd, hDC, hInstance)
- `p` prefix: Pointers (pData, pBuffer)
- `lp` prefix: Long pointers (lpszClassName)
- `dw` prefix: DWORD (32-bit unsigned) (dwStyle)
- `n` prefix: Integer (nWidth, nHeight)
- `sz` prefix: Null-terminated string (szTitle)

**Function Naming:**
- Pascal case: `CreateWindow`, `GetMessage`
- Verb-noun structure: `SetWindowText`, `GetClientRect`
- Sometimes suffixed with A (ANSI) or W (Unicode)

### API Documentation

Microsoft provides comprehensive documentation for the WinAPI:
- MSDN (Microsoft Developer Network)
- Windows SDK documentation
- Platform SDK headers

Each function is documented with:
- Syntax and parameters
- Return values
- Remarks and usage notes
- Example code
- Version requirements

## Key Concepts and Terminology

### Windows and Window Classes

**Window:**
- A rectangular area on screen
- Has properties: position, size, style, parent
- Can be visible or invisible
- Receives messages from the system
- Has an associated window procedure

**Window Class:**
- A template for creating windows
- Defines common properties and behavior
- Must be registered before creating windows
- Specifies the window procedure

```cpp
WNDCLASS wc = { };
wc.lpfnWndProc = WindowProc;    // Window procedure
wc.hInstance = hInstance;        // Application instance
wc.lpszClassName = L"MyClass";   // Class name
RegisterClass(&wc);
```

### Instance Handles (HINSTANCE)

An `HINSTANCE` is a handle to your application's instance:
- Uniquely identifies your running application
- Passed to WinMain as a parameter
- Required when creating windows and loading resources
- Used for module identification

```cpp
int WINAPI WinMain(HINSTANCE hInstance, ...) {
    // hInstance identifies your application
}
```

### Window Handles (HWND)

An `HWND` is a handle to a window:
- Uniquely identifies a window
- Returned by CreateWindow()
- Used in nearly all window-related functions
- Essential for message routing

```cpp
HWND hwnd = CreateWindow(...);  // Returns window handle
ShowWindow(hwnd, SW_SHOW);       // Use handle to show window
```

### Device Context (HDC)

A Device Context is a structure that defines a set of graphic objects and their associated attributes:
- Represents a drawing surface (window, printer, memory)
- Required for all drawing operations
- Obtained via GetDC() or BeginPaint()
- Must be released after use

```cpp
HDC hdc = GetDC(hwnd);           // Get device context
TextOut(hdc, 10, 10, L"Hello", 5); // Draw text
ReleaseDC(hwnd, hdc);            // Release context
```

### Resources

Resources are data embedded in your executable:
- Icons, cursors, bitmaps
- Menus, dialogs, accelerators
- String tables, version info
- Defined in .rc resource files
- Loaded at runtime with LoadIcon(), LoadMenu(), etc.

## WinMain: The Entry Point

### The WinMain Function

Windows GUI applications use `WinMain` instead of `main`:

```cpp
int WINAPI WinMain(
    HINSTANCE hInstance,      // Handle to current instance
    HINSTANCE hPrevInstance,  // Always NULL (legacy)
    LPSTR lpCmdLine,          // Command-line arguments
    int nCmdShow              // Window show state
)
{
    // Your application code
    return 0;
}
```

**Parameters Explained:**

1. **hInstance**: Handle to your application instance
   - Identifies your running application
   - Used when creating windows and loading resources
   - Store this globally for later use

2. **hPrevInstance**: Always NULL
   - Legacy parameter from 16-bit Windows
   - Ignored in modern Windows
   - Keep parameter for compatibility

3. **lpCmdLine**: Command-line arguments as a string
   - Similar to argv in console applications
   - Use GetCommandLine() for more control
   - May be empty if no arguments provided

4. **nCmdShow**: How to show the main window
   - SW_SHOW: Show normally
   - SW_HIDE: Hide window
   - SW_MAXIMIZE: Show maximized
   - SW_MINIMIZE: Show minimized

### WINAPI Calling Convention

`WINAPI` is defined as `__stdcall`:
- Specifies how parameters are passed and who cleans up the stack
- Required for Windows API callback functions
- Parameters pushed right-to-left on stack
- Callee cleans up the stack

### Unicode vs ANSI Entry Points

WinMain has two versions:
- **WinMain**: ANSI version (8-bit characters)
- **wWinMain**: Unicode version (16-bit characters)

```cpp
// ANSI version
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)

// Unicode version
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPWSTR lpCmdLine, int nCmdShow)
```

Modern applications should use Unicode (wWinMain) for international support.

## Handles: Windows' Object Management

### What Are Handles?

A handle is an opaque reference to an operating system resource:
- Acts like a pointer but is not a direct memory address
- Provides abstraction and security
- Managed by the operating system
- Must be explicitly closed when no longer needed

**Think of handles like:**
- A ticket stub for a coat check
- A reference number for a bank account
- An ID card that represents you

### Common Handle Types

```cpp
HANDLE      // Generic handle
HWND        // Window handle
HDC         // Device context handle
HINSTANCE   // Instance handle
HICON       // Icon handle
HCURSOR     // Cursor handle
HBRUSH      // Brush handle
HPEN        // Pen handle
HFONT       // Font handle
HBITMAP     // Bitmap handle
HMENU       // Menu handle
```

### Handle Lifetime Management

**Creating Handles:**
```cpp
HWND hwnd = CreateWindow(...);           // Window
HDC hdc = GetDC(hwnd);                   // Device context
HFONT hFont = CreateFont(...);           // Font
HANDLE hFile = CreateFile(...);          // File
```

**Destroying Handles:**
```cpp
DestroyWindow(hwnd);                     // Window
ReleaseDC(hwnd, hdc);                    // Device context
DeleteObject(hFont);                     // GDI object
CloseHandle(hFile);                      // Generic handle
```

**Key Rules:**
- Always release/destroy handles when done
- Don't use a handle after destroying it
- Don't destroy a handle you didn't create
- Some handles (like HINSTANCE) are managed by Windows

### NULL and INVALID_HANDLE_VALUE

Handle creation can fail:

```cpp
HWND hwnd = CreateWindow(...);
if (hwnd == NULL) {
    // Window creation failed
}

HANDLE hFile = CreateFile(...);
if (hFile == INVALID_HANDLE_VALUE) {
    // File creation failed
}
```

Note: Most functions return NULL on failure, but some (like CreateFile) return `INVALID_HANDLE_VALUE` ((HANDLE)-1).

## Unicode vs ANSI

### Character Encodings

Windows supports two character encodings:

**ANSI (8-bit):**
- 1 byte per character
- Limited to 256 characters
- Code page dependent
- Legacy support

**Unicode (UTF-16, 16-bit):**
- 2 bytes per character
- Supports all languages
- Required for modern applications
- Native Windows format

### TCHAR and Text Macros

Windows provides macros that work with both ANSI and Unicode:

```cpp
TCHAR szText[100];              // char or wchar_t depending on build
_T("Hello")                     // "Hello" or L"Hello"
TEXT("Hello")                   // Same as _T()
```

**Compilation Modes:**
- If `UNICODE` is defined: TCHAR = wchar_t, strings are L"..."
- If `UNICODE` is not defined: TCHAR = char, strings are "..."

### String Functions

Many WinAPI functions have A (ANSI) and W (Unicode) versions:

```cpp
// Explicit versions
CreateWindowA(...)    // ANSI version
CreateWindowW(...)    // Unicode version

// Generic version (macro)
CreateWindow(...)     // Expands to A or W based on UNICODE
```

**Behind the scenes:**
```cpp
#ifdef UNICODE
    #define CreateWindow CreateWindowW
#else
    #define CreateWindow CreateWindowA
#endif
```

### Best Practice: Use Unicode

**Always define UNICODE and _UNICODE:**

```cpp
#define UNICODE
#define _UNICODE
#include <windows.h>

// Use TEXT() macro for string literals
MessageBox(NULL, TEXT("Hello"), TEXT("Title"), MB_OK);

// Use wchar_t for character strings
wchar_t szBuffer[100];
wcscpy_s(szBuffer, 100, L"Unicode text");
```

**Benefits:**
- Support for all languages
- Better performance (no conversion)
- Future-proof code
- Consistent with modern Windows

## Common Data Types

### Windows-Defined Types

Windows defines its own type system for portability:

```cpp
// Basic types
BYTE        // unsigned char (8-bit)
WORD        // unsigned short (16-bit)
DWORD       // unsigned long (32-bit)
QWORD       // unsigned __int64 (64-bit)

// Signed integers
INT         // int
LONG        // long (32-bit)
LONGLONG    // __int64 (64-bit)

// Boolean
BOOL        // int (TRUE = 1, FALSE = 0)
            // Note: Not the same as C++ bool

// Pointers
LPVOID      // void* (long pointer to void)
LPCSTR      // const char* (long pointer to const string)
LPWSTR      // wchar_t* (long pointer to wide string)
LPCWSTR     // const wchar_t* (long pointer to const wide string)

// Handles
HANDLE      // Generic handle
HWND        // Window handle
HDC         // Device context handle
```

### Important Structures

**POINT: A 2D point**
```cpp
typedef struct tagPOINT {
    LONG x;
    LONG y;
} POINT;
```

**RECT: A rectangle**
```cpp
typedef struct tagRECT {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT;
```

**SIZE: Dimensions**
```cpp
typedef struct tagSIZE {
    LONG cx;  // width
    LONG cy;  // height
} SIZE;
```

**MSG: A message**
```cpp
typedef struct tagMSG {
    HWND   hwnd;
    UINT   message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD  time;
    POINT  pt;
} MSG;
```

### LPARAM and WPARAM

These are message parameters that carry additional information:

**Historical Context:**
- In 16-bit Windows: WPARAM was 16-bit, LPARAM was 32-bit
- In 32-bit Windows: Both are 32-bit
- In 64-bit Windows: Both are 64-bit

**Usage:**
- Meaning depends on the message type
- Often packed with multiple values
- Use macros to extract values:

```cpp
// For WM_COMMAND
WORD wmId = LOWORD(wParam);      // Control ID
WORD wmEvent = HIWORD(wParam);   // Notification code

// For WM_MOUSEMOVE
int xPos = GET_X_LPARAM(lParam);
int yPos = GET_Y_LPARAM(lParam);
```

## Error Handling in Windows

### GetLastError Function

Many WinAPI functions set an error code that can be retrieved:

```cpp
HANDLE hFile = CreateFile(...);
if (hFile == INVALID_HANDLE_VALUE) {
    DWORD error = GetLastError();
    // Handle error
}
```

**Common Error Codes:**
```cpp
ERROR_SUCCESS           // 0 - Success
ERROR_FILE_NOT_FOUND    // 2
ERROR_ACCESS_DENIED     // 5
ERROR_INVALID_HANDLE    // 6
ERROR_NOT_ENOUGH_MEMORY // 8
```

### FormatMessage Function

Convert error code to human-readable string:

```cpp
void DisplayError(DWORD errorCode) {
    LPWSTR errorText = NULL;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        errorCode,
        0,
        (LPWSTR)&errorText,
        0,
        NULL
    );

    if (errorText != NULL) {
        MessageBox(NULL, errorText, L"Error", MB_OK | MB_ICONERROR);
        LocalFree(errorText);
    }
}
```

### MessageBox for Debugging

The simplest way to display errors:

```cpp
MessageBox(
    NULL,                           // Parent window
    L"An error occurred!",          // Message text
    L"Error",                       // Title
    MB_OK | MB_ICONERROR           // Buttons and icon
);
```

**Common Flags:**
```cpp
// Buttons
MB_OK                    // OK button
MB_OKCANCEL             // OK and Cancel
MB_YESNO                // Yes and No
MB_RETRYCANCEL          // Retry and Cancel

// Icons
MB_ICONERROR            // Error icon (red X)
MB_ICONWARNING          // Warning icon (exclamation)
MB_ICONINFORMATION      // Information icon (blue i)
MB_ICONQUESTION         // Question icon (?)
```

## Development Environment Setup

### Required Components

To develop Windows applications, you need:

1. **Compiler:**
   - Visual Studio (recommended)
   - MinGW-w64
   - Clang for Windows

2. **Windows SDK:**
   - Included with Visual Studio
   - Contains headers and libraries
   - Provides documentation

3. **Headers:**
   - windows.h (main header)
   - Additional headers for specific features

### Compiling with Visual Studio

**Using IDE:**
1. Create new Win32 Project
2. Select "Windows Application"
3. Write code in provided template
4. Build (Ctrl+Shift+B)
5. Run (F5)

**Using Command Line (cl.exe):**
```cmd
cl /D "UNICODE" /D "_UNICODE" myapp.cpp user32.lib gdi32.lib
```

### Compiling with MinGW-w64

```bash
g++ -D UNICODE -D _UNICODE myapp.cpp -o myapp.exe -mwindows -lgdi32 -luser32
```

**Flags Explained:**
- `-D UNICODE`: Define UNICODE macro
- `-D _UNICODE`: Define _UNICODE macro
- `-mwindows`: Create Windows GUI app (not console)
- `-lgdi32`: Link GDI32.DLL
- `-luser32`: Link USER32.DLL

### Common Linker Errors

**Unresolved external symbol:**
```
error LNK2019: unresolved external symbol CreateWindowExW
```
**Solution:** Link required library (user32.lib)

**Entry point not found:**
```
error LNK2019: unresolved external symbol WinMain
```
**Solution:** Make sure you have WinMain defined, or use correct subsystem

## Your First Windows Program

### Minimal Windows Application

Here's the absolute minimum code for a Windows application:

```cpp
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    // Register window class
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create window
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        L"Learn Windows Programming",   // Window text
        WS_OVERLAPPEDWINDOW,           // Window style
        CW_USEDEFAULT, CW_USEDEFAULT,  // Position
        CW_USEDEFAULT, CW_USEDEFAULT,  // Size
        NULL,                           // Parent window
        NULL,                           // Menu
        hInstance,                      // Instance handle
        NULL                            // Additional data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Message loop
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW+1));
            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
```

### Program Flow Breakdown

**1. Register Window Class:**
```cpp
WNDCLASS wc = { };
wc.lpfnWndProc = WindowProc;     // Set window procedure
wc.hInstance = hInstance;         // Set instance handle
wc.lpszClassName = CLASS_NAME;    // Set class name
RegisterClass(&wc);               // Register with Windows
```

**2. Create Window:**
```cpp
HWND hwnd = CreateWindowEx(...);
```
- Creates a window instance
- Returns window handle (HWND)
- Window is created but not visible yet

**3. Show Window:**
```cpp
ShowWindow(hwnd, nCmdShow);
```
- Makes window visible
- Sends WM_SHOWWINDOW and WM_SIZE messages

**4. Message Loop:**
```cpp
while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}
```
- Retrieves messages from queue
- Dispatches to window procedure
- Exits when WM_QUIT received

**5. Window Procedure:**
```cpp
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
```
- Handles messages
- Must handle WM_DESTROY
- Calls DefWindowProc for unhandled messages

## Practical Examples

The Code/ subdirectory contains 15 complete, executable examples:

1. **01_MessageBox.cpp** - Displays a simple message box
2. **02_MinimalWindow.cpp** - Creates the simplest possible window
3. **03_WindowWithPaint.cpp** - Window that handles WM_PAINT
4. **04_ColoredWindow.cpp** - Window with custom background color
5. **05_MultipleWindows.cpp** - Creates multiple independent windows
6. **06_WindowTitle.cpp** - Demonstrates setting and changing window title
7. **07_WindowPosition.cpp** - Controls window position and size
8. **08_GetMessage.cpp** - Demonstrates GetMessage return values
9. **09_PeekMessage.cpp** - Non-blocking message retrieval
10. **10_PostQuitMessage.cpp** - Different ways to exit application
11. **11_DefWindowProc.cpp** - Understanding default window procedure
12. **12_WindowStyles.cpp** - Different window styles (popup, child, etc.)
13. **13_ErrorHandling.cpp** - Proper error checking and reporting
14. **14_UnicodeWindow.cpp** - Demonstrates Unicode string handling
15. **15_SystemMetrics.cpp** - Retrieving system information

Each example is fully commented and demonstrates specific concepts covered in this lesson.

## Best Practices

### Code Organization

**1. Separate WinMain and WindowProc:**
```cpp
// Forward declarations
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(...) {
    // Initialization
}

LRESULT CALLBACK WindowProc(...) {
    // Message handling
}
```

**2. Use Constants:**
```cpp
const wchar_t CLASS_NAME[] = L"MyWindowClass";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
```

**3. Error Checking:**
```cpp
HWND hwnd = CreateWindow(...);
if (hwnd == NULL) {
    MessageBox(NULL, L"Window creation failed!", L"Error", MB_ICONERROR);
    return 1;
}
```

### Memory Management

**1. Release Resources:**
```cpp
HDC hdc = GetDC(hwnd);
// Use hdc
ReleaseDC(hwnd, hdc);  // Always release
```

**2. Delete GDI Objects:**
```cpp
HFONT hFont = CreateFont(...);
// Use font
DeleteObject(hFont);  // Always delete
```

### Performance

**1. Handle WM_PAINT Efficiently:**
```cpp
case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    // Only draw within ps.rcPaint
    EndPaint(hwnd, &ps);
    return 0;
}
```

**2. Avoid Blocking in Window Procedure:**
```cpp
// BAD - blocks message processing
case WM_COMMAND:
    Sleep(5000);  // Window becomes unresponsive
    return 0;

// GOOD - use timer or thread for long operations
case WM_COMMAND:
    CreateThread(...);  // Process in background
    return 0;
```

### Security

**1. Use Safe String Functions:**
```cpp
// BAD
wcscpy(buffer, source);

// GOOD
wcscpy_s(buffer, bufferSize, source);
```

**2. Validate Input:**
```cpp
case WM_COMMAND:
    if (LOWORD(wParam) < ID_MIN || LOWORD(wParam) > ID_MAX) {
        return 0;  // Invalid command ID
    }
    break;
```

## Summary

In this lesson, you've learned the fundamental concepts of Windows programming:

**Key Takeaways:**

1. **Architecture**: Windows uses a layered architecture with user mode and kernel mode separation

2. **Message-Driven Programming**: Windows applications respond to messages rather than following linear execution

3. **Core Components**:
   - WinMain: Entry point for Windows applications
   - Window Class: Template for creating windows
   - Window Procedure: Function that handles messages
   - Message Loop: Retrieves and dispatches messages

4. **Essential Concepts**:
   - Handles: Opaque references to system resources
   - Unicode: Modern Windows uses UTF-16 encoding
   - Device Context: Required for drawing operations
   - Error Handling: Use GetLastError and FormatMessage

5. **Program Structure**:
   ```
   WinMain
   ├── Register Window Class
   ├── Create Window
   ├── Show Window
   ├── Message Loop
   │   ├── GetMessage
   │   ├── TranslateMessage
   │   └── DispatchMessage → WindowProc
   └── Return exit code
   ```

6. **Best Practices**:
   - Always use Unicode (UNICODE and _UNICODE)
   - Check return values and handle errors
   - Release resources properly
   - Keep window procedure responsive
   - Use safe string functions

**What's Next:**

In Lesson 32, we'll dive deeper into window creation, exploring:
- Extended window styles
- Window classes in detail
- Creating windows with custom properties
- Parent-child window relationships
- More complex window procedures

**Practice Exercises:**

1. Compile and run all 15 example programs
2. Modify examples to change window properties
3. Add MessageBox calls to observe message flow
4. Create a window with a custom size and position
5. Experiment with different window styles

**Additional Resources:**

- Microsoft Docs: Windows Desktop Applications
- Charles Petzold's "Programming Windows"
- MSDN Magazine articles on WinAPI
- Windows SDK samples

Congratulations on completing your introduction to Windows programming! You now have the foundation to build sophisticated Windows applications.

---

**Estimated Reading Time:** 4-5 hours
**Hands-On Practice Time:** 2-3 hours
**Total Lesson Time:** 6-8 hours

**Next Lesson:** [Lesson 32: Creating Your First Window](#)
