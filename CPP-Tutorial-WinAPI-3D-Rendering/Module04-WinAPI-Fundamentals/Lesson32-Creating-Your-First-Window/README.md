# Lesson 32: Creating Your First Window

## Table of Contents
1. [Introduction](#introduction)
2. [The WNDCLASS Structure](#the-wndclass-structure)
3. [RegisterClass Function](#registerclass-function)
4. [CreateWindowEx Function](#createwindowex-function)
5. [Window Styles Deep Dive](#window-styles-deep-dive)
6. [Extended Window Styles](#extended-window-styles)
7. [Window Class Styles](#window-class-styles)
8. [ShowWindow and Window Visibility](#showwindow-and-window-visibility)
9. [UpdateWindow Function](#updatewindow-function)
10. [Window Creation Messages](#window-creation-messages)
11. [Complete Window Creation Example](#complete-window-creation-example)
12. [Multiple Window Classes](#multiple-window-classes)
13. [Unregistering Window Classes](#unregistering-window-classes)
14. [Common Pitfalls](#common-pitfalls)
15. [Practical Examples](#practical-examples)
16. [Best Practices](#best-practices)
17. [Summary](#summary)

## Introduction

Creating windows is the fundamental skill in Windows programming. Every visible element in a Windows application—from the main application window to dialog boxes, buttons, and custom controls—is a window. This lesson provides an in-depth exploration of window creation, covering every aspect from registering window classes to displaying windows on screen.

**Learning Objectives:**
- Master the WNDCLASS structure and all its fields
- Understand the complete window creation process
- Learn all window styles and their combinations
- Handle window creation messages
- Create different types of windows
- Avoid common window creation mistakes

**Time Required:** 4-5 hours

## The WNDCLASS Structure

The `WNDCLASS` structure (or `WNDCLASSEX` for extended version) defines the attributes shared by all windows of the same class.

### WNDCLASS Structure Definition

```cpp
typedef struct tagWNDCLASS {
    UINT      style;          // Class styles (CS_*)
    WNDPROC   lpfnWndProc;    // Pointer to window procedure
    int       cbClsExtra;     // Extra bytes for class
    int       cbWndExtra;     // Extra bytes for window instance
    HINSTANCE hInstance;      // Instance handle
    HICON     hIcon;          // Icon handle
    HCURSOR   hCursor;        // Cursor handle
    HBRUSH    hbrBackground;  // Background brush
    LPCWSTR   lpszMenuName;   // Menu resource name
    LPCWSTR   lpszClassName;  // Class name
} WNDCLASS;
```

### WNDCLASSEX Structure (Extended)

```cpp
typedef struct tagWNDCLASSEX {
    UINT      cbSize;         // Size of this structure
    UINT      style;          // Class styles
    WNDPROC   lpfnWndProc;    // Window procedure
    int       cbClsExtra;     // Extra class bytes
    int       cbWndExtra;     // Extra window bytes
    HINSTANCE hInstance;      // Instance handle
    HICON     hIcon;          // Large icon
    HCURSOR   hCursor;        // Cursor
    HBRUSH    hbrBackground;  // Background brush
    LPCWSTR   lpszMenuName;   // Menu name
    LPCWSTR   lpszClassName;  // Class name
    HICON     hIconSm;        // Small icon (16x16)
} WNDCLASSEX;
```

**Difference between WNDCLASS and WNDCLASSEX:**
- WNDCLASSEX includes `cbSize` (must be set to `sizeof(WNDCLASSEX)`)
- WNDCLASSEX includes `hIconSm` for small icon (used in title bar and taskbar)
- WNDCLASSEX is recommended for modern applications

### Field-by-Field Explanation

**1. style (Class Styles)**

Defines class-wide behavior. Common values:

| Style | Description |
|-------|-------------|
| `CS_HREDRAW` | Redraw entire window if width changes |
| `CS_VREDRAW` | Redraw entire window if height changes |
| `CS_DBLCLKS` | Enable double-click messages (WM_LBUTTONDBLCLK) |
| `CS_OWNDC` | Each window gets unique device context |
| `CS_CLASSDC` | All windows share one device context |
| `CS_PARENTDC` | Child window uses parent's DC |
| `CS_NOCLOSE` | Disable close menu item |
| `CS_SAVEBITS` | Save screen area behind window |
| `CS_BYTEALIGNCLIENT` | Align client area on byte boundary |
| `CS_BYTEALIGNWINDOW` | Align window on byte boundary |
| `CS_GLOBALCLASS` | Window class available to all processes |
| `CS_DROPSHADOW` | Enable drop shadow (Windows XP+) |

**Example:**
```cpp
wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
```

**2. lpfnWndProc (Window Procedure)**

Pointer to the window procedure function that handles messages for this class.

```cpp
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

wc.lpfnWndProc = WindowProc;
```

**3. cbClsExtra (Class Extra Bytes)**

Number of extra bytes to allocate for the class structure. Usually 0.

**Use case:** Store class-wide data.

```cpp
wc.cbClsExtra = 0;  // Usually not needed
```

**Access via:**
- `SetClassLongPtr(hwnd, index, value)`
- `GetClassLongPtr(hwnd, index)`

**4. cbWndExtra (Window Extra Bytes)**

Number of extra bytes to allocate for each window instance. Usually 0.

**Use case:** Store window-specific data (alternative to window properties).

```cpp
wc.cbWndExtra = sizeof(void*);  // Store pointer per window
```

**Access via:**
- `SetWindowLongPtr(hwnd, index, value)`
- `GetWindowLongPtr(hwnd, index)`

**5. hInstance (Instance Handle)**

Handle to the application instance that owns this class.

```cpp
wc.hInstance = hInstance;  // From WinMain parameter
```

**6. hIcon (Icon Handle)**

Large icon (32x32) displayed when window is minimized and in Alt+Tab.

```cpp
// Load from resources
wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON));

// Use standard system icon
wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

// No icon
wc.hIcon = NULL;
```

**System icons:**
- `IDI_APPLICATION`: Default application icon
- `IDI_ERROR`: Error icon
- `IDI_WARNING`: Warning icon
- `IDI_INFORMATION`: Information icon
- `IDI_QUESTION`: Question icon
- `IDI_WINLOGO`: Windows logo

**7. hCursor (Cursor Handle)**

Cursor displayed when mouse is over the window's client area.

```cpp
// Load system cursor
wc.hCursor = LoadCursor(NULL, IDC_ARROW);

// Load custom cursor from resources
wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_MYCURSOR));

// No cursor (must handle WM_SETCURSOR)
wc.hCursor = NULL;
```

**System cursors:**
- `IDC_ARROW`: Standard arrow
- `IDC_HAND`: Hand pointer
- `IDC_IBEAM`: I-beam (text selection)
- `IDC_CROSS`: Crosshair
- `IDC_WAIT`: Hourglass/spinner
- `IDC_SIZEALL`: Four-pointed arrow
- `IDC_SIZENWSE`: NW-SE arrow
- `IDC_SIZENESW`: NE-SW arrow
- `IDC_SIZEWE`: East-west arrow
- `IDC_SIZENS`: North-south arrow
- `IDC_NO`: Slashed circle

**8. hbrBackground (Background Brush)**

Brush used to paint the window's background.

```cpp
// System color
wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

// Custom color
wc.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));

// Stock object
wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

// No automatic background (handle WM_ERASEBKGND)
wc.hbrBackground = NULL;
```

**System colors:**
- `COLOR_WINDOW`: Window background
- `COLOR_WINDOWTEXT`: Window text
- `COLOR_3DFACE`: 3D face color
- `COLOR_BTNFACE`: Button face
- `COLOR_DESKTOP`: Desktop color

**9. lpszMenuName (Menu Resource Name)**

Name of menu resource to attach to windows of this class.

```cpp
// Menu from resources
wc.lpszMenuName = MAKEINTRESOURCE(IDR_MAINMENU);

// No menu
wc.lpszMenuName = NULL;
```

**10. lpszClassName (Class Name)**

Unique name identifying this window class.

```cpp
wc.lpszClassName = L"MyWindowClass";
```

**Requirements:**
- Must be unique within the application
- Maximum 256 characters
- Case-sensitive
- Used in CreateWindow to specify which class to instantiate

**11. hIconSm (Small Icon - WNDCLASSEX only)**

Small icon (16x16) displayed in title bar and taskbar.

```cpp
wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
wcex.hIconSm = NULL;  // Windows generates from large icon
```

## RegisterClass Function

After filling the `WNDCLASS` structure, register it with Windows using `RegisterClass` or `RegisterClassEx`.

### RegisterClass Syntax

```cpp
ATOM RegisterClass(const WNDCLASS* lpWndClass);
ATOM RegisterClassEx(const WNDCLASSEX* lpWndClass);
```

**Return Value:**
- On success: Returns an ATOM (unique identifier for the class)
- On failure: Returns 0 (use `GetLastError()` for details)

### Registration Example

```cpp
WNDCLASSEX wcex = { };
wcex.cbSize = sizeof(WNDCLASSEX);
wcex.style = CS_HREDRAW | CS_VREDRAW;
wcex.lpfnWndProc = WindowProc;
wcex.hInstance = hInstance;
wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
wcex.lpszClassName = L"MyWindowClass";
wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

ATOM classAtom = RegisterClassEx(&wcex);
if (classAtom == 0)
{
    DWORD error = GetLastError();
    // Handle error
    return FALSE;
}
```

### Common Registration Errors

| Error Code | Description | Solution |
|------------|-------------|----------|
| ERROR_CLASS_ALREADY_EXISTS | Class name already registered | Use different name or unregister old class |
| ERROR_INVALID_PARAMETER | Invalid field in structure | Check all required fields |
| ERROR_CALL_NOT_IMPLEMENTED | Unsupported on this Windows version | Use compatible flags |

### Checking if Class Exists

```cpp
WNDCLASS wc;
if (GetClassInfo(hInstance, L"MyWindowClass", &wc))
{
    // Class already registered
}
else
{
    // Class not registered, register it
    RegisterClass(&newWc);
}
```

## CreateWindowEx Function

After registering the window class, create window instances using `CreateWindowEx`.

### CreateWindowEx Syntax

```cpp
HWND CreateWindowEx(
    DWORD     dwExStyle,     // Extended window styles
    LPCWSTR   lpClassName,   // Window class name
    LPCWSTR   lpWindowName,  // Window title
    DWORD     dwStyle,       // Window styles
    int       x,             // X position
    int       y,             // Y position
    int       nWidth,        // Width
    int       nHeight,       // Height
    HWND      hWndParent,    // Parent window handle
    HMENU     hMenu,         // Menu handle or child ID
    HINSTANCE hInstance,     // Instance handle
    LPVOID    lpParam        // Additional data
);
```

### Parameter Details

**dwExStyle (Extended Styles):**
See [Extended Window Styles](#extended-window-styles) section.

**lpClassName:**
- Name of registered window class
- Can be atom returned by RegisterClass
- Must match exactly (case-sensitive)

**lpWindowName:**
- Text displayed in title bar
- Can be changed later with SetWindowText
- NULL for no title

**dwStyle:**
- Window styles (WS_*)
- See [Window Styles Deep Dive](#window-styles-deep-dive)

**x, y (Position):**
- Screen coordinates in pixels
- `CW_USEDEFAULT`: Let Windows choose
- Relative to parent if WS_CHILD style is used

**nWidth, nHeight (Size):**
- Window size in pixels
- `CW_USEDEFAULT`: Let Windows choose default size
- Includes non-client area (title bar, borders)

**hWndParent:**
- Handle to parent window
- NULL for top-level windows
- Required for WS_CHILD windows
- Establishes parent-child relationship

**hMenu:**
- For top-level windows: Menu handle
- For child windows: Control ID (cast from integer)
- NULL for no menu

**hInstance:**
- Application instance handle
- Same as used in RegisterClass

**lpParam:**
- Pointer to custom data
- Passed to window procedure in WM_CREATE message
- Cast to CREATESTRUCT* in WM_CREATE handler

### Return Value

- On success: Handle to created window (HWND)
- On failure: NULL (use GetLastError() for error code)

### Creation Example

```cpp
HWND hwnd = CreateWindowEx(
    WS_EX_CLIENTEDGE,              // Extended style
    L"MyWindowClass",               // Class name
    L"My Application",              // Window title
    WS_OVERLAPPEDWINDOW,           // Style
    CW_USEDEFAULT, CW_USEDEFAULT,  // Position
    800, 600,                       // Size
    NULL,                           // No parent
    NULL,                           // No menu
    hInstance,                      // Instance
    NULL                            // No additional data
);

if (hwnd == NULL)
{
    DWORD error = GetLastError();
    MessageBox(NULL, L"Window creation failed!", L"Error", MB_OK);
    return FALSE;
}
```

## Window Styles Deep Dive

Window styles control the appearance and behavior of windows.

### Primary Window Types

| Style | Description |
|-------|-------------|
| `WS_OVERLAPPED` | Overlapped window (has title bar and border) |
| `WS_POPUP` | Popup window |
| `WS_CHILD` | Child window (must have parent) |

**Note:** A window must be one of these three types.

### Common Style Combinations

**WS_OVERLAPPEDWINDOW:**
```cpp
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | \
                              WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
```
Standard application window with all features.

**WS_POPUPWINDOW:**
```cpp
#define WS_POPUPWINDOW (WS_POPUP | WS_BORDER | WS_SYSMENU)
```
Popup window with border and system menu.

### Style Flags

**Caption and Border:**
- `WS_CAPTION`: Title bar (implies WS_BORDER)
- `WS_BORDER`: Thin border
- `WS_DLGFRAME`: Dialog frame (non-sizable border)
- `WS_THICKFRAME` / `WS_SIZEBOX`: Resizable border

**System Elements:**
- `WS_SYSMENU`: System menu (icon and close button in title bar)
- `WS_MINIMIZEBOX`: Minimize button
- `WS_MAXIMIZEBOX`: Maximize button

**Scrollbars:**
- `WS_HSCROLL`: Horizontal scrollbar
- `WS_VSCROLL`: Vertical scrollbar

**Visibility and State:**
- `WS_VISIBLE`: Initially visible
- `WS_DISABLED`: Initially disabled
- `WS_MINIMIZE`: Initially minimized
- `WS_MAXIMIZE`: Initially maximized

**Clipping:**
- `WS_CLIPCHILDREN`: Excludes child windows when drawing
- `WS_CLIPSIBLINGS`: Clips overlapping sibling windows

**Other:**
- `WS_GROUP`: First control in a group (for dialogs)
- `WS_TABSTOP`: Can receive keyboard focus via Tab key

### Style Combinations Examples

**Standard Application Window:**
```cpp
DWORD style = WS_OVERLAPPEDWINDOW;
```

**Non-Resizable Window:**
```cpp
DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
              WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
```

**Dialog-Style Window:**
```cpp
DWORD style = WS_POPUP | WS_CAPTION | WS_SYSMENU;
```

**Tool Window:**
```cpp
DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;
DWORD exStyle = WS_EX_TOOLWINDOW;
```

**Borderless Window:**
```cpp
DWORD style = WS_POPUP;
```

**Child Window:**
```cpp
DWORD style = WS_CHILD | WS_VISIBLE | WS_BORDER;
```

## Extended Window Styles

Extended styles provide additional window features.

### Common Extended Styles

| Extended Style | Description |
|----------------|-------------|
| `WS_EX_TOPMOST` | Window stays on top of non-topmost windows |
| `WS_EX_TOOLWINDOW` | Tool window (small title bar, not in taskbar) |
| `WS_EX_CLIENTEDGE` | Sunken 3D border |
| `WS_EX_WINDOWEDGE` | Raised edge border |
| `WS_EX_STATICEDGE` | 3D border (not sunken or raised) |
| `WS_EX_LAYERED` | Supports transparency (SetLayeredWindowAttributes) |
| `WS_EX_TRANSPARENT` | Transparent to hit-testing |
| `WS_EX_ACCEPTFILES` | Accepts drag-and-drop files |
| `WS_EX_NOACTIVATE` | Top-level window doesn't activate when clicked |
| `WS_EX_CONTEXTHELP` | Context help button in title bar |
| `WS_EX_APPWINDOW` | Forces window to appear in taskbar |
| `WS_EX_CONTROLPARENT` | Allows Tab key navigation among child windows |
| `WS_EX_NOPARENTNOTIFY` | Child window doesn't send WM_PARENTNOTIFY |

### Extended Style Examples

**Always-On-Top Window:**
```cpp
HWND hwnd = CreateWindowEx(
    WS_EX_TOPMOST,
    className, title,
    WS_OVERLAPPEDWINDOW,
    x, y, width, height,
    NULL, NULL, hInstance, NULL
);
```

**Transparent Window (50% opacity):**
```cpp
HWND hwnd = CreateWindowEx(
    WS_EX_LAYERED,
    className, title,
    WS_OVERLAPPEDWINDOW,
    x, y, width, height,
    NULL, NULL, hInstance, NULL
);

SetLayeredWindowAttributes(hwnd, 0, 128, LWA_ALPHA);
```

**Tool Palette:**
```cpp
HWND hwnd = CreateWindowEx(
    WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
    className, title,
    WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
    x, y, width, height,
    NULL, NULL, hInstance, NULL
);
```

## Window Class Styles

Class styles (CS_*) affect all windows of a class.

### CS_HREDRAW and CS_VREDRAW

```cpp
wc.style = CS_HREDRAW | CS_VREDRAW;
```

**Effect:**
- `CS_HREDRAW`: Redraw entire window when width changes
- `CS_VREDRAW`: Redraw entire window when height changes

**Without these:** Only newly exposed areas are redrawn.

**Use when:** Window content depends on size (charts, graphics, layouts).

### CS_DBLCLKS

```cpp
wc.style = CS_DBLCLKS;
```

**Effect:** Enables double-click messages (WM_LBUTTONDBLCLK, etc.).

**Without this:** Double-clicks generate two WM_LBUTTONDOWN messages.

### CS_OWNDC

```cpp
wc.style = CS_OWNDC;
```

**Effect:** Each window gets its own private device context.

**Use when:**
- Need to customize DC settings that persist
- Performance is less critical than convenience

**Note:** More memory usage per window.

### CS_CLASSDC

```cpp
wc.style = CS_CLASSDC;
```

**Effect:** All windows of this class share one device context.

**Use when:** All windows are identical and drawn the same way.

**Warning:** Can cause drawing conflicts between windows.

### CS_DROPSHADOW

```cpp
wc.style = CS_DROPSHADOW;
```

**Effect:** Window has drop shadow (Windows XP+).

**Use for:** Menus, tooltips, popup windows.

**Note:** Only works for WS_POPUP windows.

## ShowWindow and Window Visibility

After creating a window, make it visible with `ShowWindow`.

### ShowWindow Syntax

```cpp
BOOL ShowWindow(HWND hWnd, int nCmdShow);
```

### nCmdShow Values

| Value | Description |
|-------|-------------|
| `SW_HIDE` | Hides window |
| `SW_SHOW` | Activates and displays window |
| `SW_SHOWNORMAL` / `SW_RESTORE` | Restores to normal size and position |
| `SW_SHOWMINIMIZED` | Displays window minimized |
| `SW_SHOWMAXIMIZED` | Displays window maximized |
| `SW_SHOWNOACTIVATE` | Shows window without activating |
| `SW_SHOWNA` | Shows window in current state without activating |
| `SW_SHOWMINNOACTIVE` | Shows minimized without activating |

### Common Usage

```cpp
ShowWindow(hwnd, nCmdShow);  // Use parameter from WinMain

ShowWindow(hwnd, SW_SHOW);  // Show normally
ShowWindow(hwnd, SW_HIDE);  // Hide window
ShowWindow(hwnd, SW_SHOWMAXIMIZED);  // Show maximized
```

### IsWindowVisible

```cpp
if (IsWindowVisible(hwnd))
{
    // Window is visible
}
```

## UpdateWindow Function

`UpdateWindow` sends `WM_PAINT` if the update region is not empty.

### Syntax

```cpp
BOOL UpdateWindow(HWND hWnd);
```

### Usage

```cpp
ShowWindow(hwnd, nCmdShow);
UpdateWindow(hwnd);  // Force immediate paint
```

**Effect:**
- Immediately sends WM_PAINT (bypasses message queue)
- Only if update region is non-empty
- Returns after WM_PAINT is processed

**When to use:**
- After ShowWindow to ensure window is drawn immediately
- After invalidating regions during initialization

## Window Creation Messages

Windows sends several messages during window creation.

### Message Sequence

1. **WM_GETMINMAXINFO**: Queries minimum/maximum size
2. **WM_NCCREATE**: Non-client area is being created
3. **WM_NCCALCSIZE**: Calculate client area size
4. **WM_CREATE**: Window is being created
5. **WM_SIZE**: Window has been sized
6. **WM_MOVE**: Window has been moved
7. **WM_SHOWWINDOW**: Window is being shown/hidden

### WM_CREATE

Most important message for initialization.

```cpp
case WM_CREATE:
{
    // lpParam points to CREATESTRUCT
    CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;

    // Access creation parameters
    void* pUserData = pCreate->lpCreateParams;

    // Initialize window-specific data
    // Create child windows
    // Allocate resources

    return 0;  // Success
    // return -1;  // Failure (window won't be created)
}
```

### WM_GETMINMAXINFO

Control minimum and maximum window size.

```cpp
case WM_GETMINMAXINFO:
{
    MINMAXINFO* pInfo = (MINMAXINFO*)lParam;

    // Set minimum size
    pInfo->ptMinTrackSize.x = 400;
    pInfo->ptMinTrackSize.y = 300;

    // Set maximum size
    pInfo->ptMaxTrackSize.x = 1920;
    pInfo->ptMaxTrackSize.y = 1080;

    return 0;
}
```

## Complete Window Creation Example

Here's a complete, production-quality example:

```cpp
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    // Register window class
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASSEX wcex = { };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = CLASS_NAME;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create window
    HWND hwnd = CreateWindowEx(
        0,                              // Extended styles
        CLASS_NAME,                     // Class name
        L"My Application Window",       // Window title
        WS_OVERLAPPEDWINDOW,           // Window style
        CW_USEDEFAULT, CW_USEDEFAULT,  // Position
        800, 600,                       // Size
        NULL,                           // Parent
        NULL,                           // Menu
        hInstance,                      // Instance
        NULL                            // Additional data
    );

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
            // Initialization code here
            return 0;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW+1));
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
```

## Practical Examples

The Code/ subdirectory contains 15 complete examples demonstrating various aspects of window creation. Each example is a fully functional, compilable program with detailed comments.

## Best Practices

1. **Always check return values**
2. **Use WNDCLASSEX over WNDCLASS**
3. **Define meaningful class names**
4. **Set appropriate icons and cursors**
5. **Handle WM_CREATE for initialization**
6. **Use CW_USEDEFAULT for initial positioning**
7. **Call UpdateWindow after ShowWindow**
8. **Clean up resources in WM_DESTROY**

## Summary

Window creation is a multi-step process:
1. Define and fill WNDCLASS/WNDCLASSEX structure
2. Register the window class
3. Create window instances with CreateWindowEx
4. Show and update the window
5. Handle window creation messages

In the next lesson, we'll explore the message loop and window procedure in greater detail.

---

**Next Lesson:** [Lesson 33: Message Loop & WndProc](#)
