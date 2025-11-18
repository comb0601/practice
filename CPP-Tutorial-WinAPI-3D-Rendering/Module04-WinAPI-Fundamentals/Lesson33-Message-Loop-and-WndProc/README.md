# Lesson 33: Message Loop and Window Procedure

## Table of Contents
1. [Introduction](#introduction)
2. [The Windows Message System](#the-windows-message-system)
3. [The Message Loop](#the-message-loop)
4. [GetMessage vs PeekMessage](#getmessage-vs-peekmessage)
5. [The MSG Structure](#the-msg-structure)
6. [TranslateMessage Function](#translatemessage-function)
7. [DispatchMessage Function](#dispatchmessage-function)
8. [The Window Procedure](#the-window-procedure)
9. [Message Parameters: wParam and lParam](#message-parameters-wparam-and-lparam)
10. [DefWindowProc Function](#defwindowproc-function)
11. [Common Windows Messages](#common-windows-messages)
12. [Message Flow and Routing](#message-flow-and-routing)
13. [Sending vs Posting Messages](#sending-vs-posting-messages)
14. [Message Queues](#message-queues)
15. [Message Filtering](#message-filtering)
16. [Best Practices](#best-practices)
17. [Summary](#summary)

## Introduction

The message loop and window procedure are the heart of every Windows application. Understanding how messages flow through your application is essential for creating responsive, well-behaved Windows programs.

**Learning Objectives:**
- Understand the Windows message-driven architecture
- Master the message loop implementation
- Learn to process messages in the window procedure
- Understand message routing and queuing
- Handle both queued and non-queued messages
- Implement efficient message processing

**Time Required:** 4-5 hours

## The Windows Message System

Windows is a **message-driven** operating system. Everything that happens in a Windows application is driven by messages.

### What is a Message?

A message is a notification of an event. Examples:
- User clicks a button → `WM_LBUTTONDOWN` message
- Window needs repainting → `WM_PAINT` message
- User presses a key → `WM_KEYDOWN` message
- Window is being closed → `WM_CLOSE` message

### Message-Driven Architecture

```
 User Action          Windows          Your Application
     |                   |                    |
     |  Click mouse      |                    |
     |------------------>|                    |
     |                   |  WM_LBUTTONDOWN    |
     |                   |------------------->|
     |                   |                    |  Handle click
     |                   |                    |  in WndProc
     |                   |      Return        |
     |                   |<-------------------|
```

### Message Categories

**1. Queued Messages**
- Placed in message queue
- Retrieved by GetMessage/PeekMessage
- Examples: Mouse clicks, key presses, WM_PAINT, WM_TIMER

**2. Non-Queued Messages**
- Sent directly to window procedure
- Bypass message queue
- Examples: WM_SIZE, WM_CREATE, WM_DESTROY, WM_ACTIVATE

**3. Posted Messages**
- Asynchronously placed in queue
- Posted with PostMessage
- Don't wait for processing

**4. Sent Messages**
- Synchronously delivered
- Sent with SendMessage
- Wait for processing to complete

## The Message Loop

The message loop is the core of your application. It continuously retrieves and dispatches messages.

### Basic Message Loop

```cpp
MSG msg = { };
while (GetMessage(&msg, NULL, 0, 0))
{
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}

return (int)msg.wParam;
```

### Message Loop Flow

```
1. GetMessage(&msg, ...)
   ↓ Retrieve message from queue (blocks if empty)
2. TranslateMessage(&msg)
   ↓ Translate virtual-key messages to character messages
3. DispatchMessage(&msg)
   ↓ Send message to window procedure
4. Window Procedure processes message
   ↓ Returns result
5. Loop continues
```

### GetMessage Function

```cpp
BOOL GetMessage(
    LPMSG lpMsg,         // Pointer to MSG structure
    HWND  hWnd,          // Handle to window (NULL = all windows)
    UINT  wMsgFilterMin, // Minimum message value (0 = no filter)
    UINT  wMsgFilterMax  // Maximum message value (0 = no filter)
);
```

**Return Values:**
- Non-zero: Message retrieved (continue loop)
- Zero: `WM_QUIT` received (exit loop)
- -1: Error occurred (rare, check with `GetLastError()`)

**Behavior:**
- Blocks (waits) if message queue is empty
- Removes message from queue
- Returns when message is available

**Parameters:**
- `lpMsg`: Filled with message information
- `hWnd`: Filter to specific window (NULL for all windows in thread)
- `wMsgFilterMin/Max`: Message value range filter (0, 0 for no filter)

### Message Loop Variants

**Standard Loop (Blocking):**
```cpp
while (GetMessage(&msg, NULL, 0, 0))
{
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}
```

**Non-Blocking Loop with PeekMessage:**
```cpp
while (TRUE)
{
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            break;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    else
    {
        // No messages, do background work
        DoBackgroundProcessing();
    }
}
```

**Loop with Message Filtering:**
```cpp
while (GetMessage(&msg, NULL, 0, 0))
{
    // Filter out mouse messages during initialization
    if (msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST)
        continue;  // Skip mouse messages

    TranslateMessage(&msg);
    DispatchMessage(&msg);
}
```

## GetMessage vs PeekMessage

### GetMessage

```cpp
BOOL GetMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
```

**Characteristics:**
- **Blocks** if no messages in queue
- **Removes** message from queue
- Returns FALSE only for WM_QUIT
- Most common for standard applications

**Use when:**
- Standard desktop application
- Want to minimize CPU usage
- Messages drive all activity

### PeekMessage

```cpp
BOOL PeekMessage(
    LPMSG lpMsg,
    HWND  hWnd,
    UINT  wMsgFilterMin,
    UINT  wMsgFilterMax,
    UINT  wRemoveMsg       // PM_REMOVE or PM_NOREMOVE
);
```

**Characteristics:**
- **Never blocks** (returns immediately)
- Can peek without removing (PM_NOREMOVE)
- Can remove like GetMessage (PM_REMOVE)
- Returns TRUE if message available, FALSE otherwise

**Use when:**
- Games and animations
- Need continuous processing
- Background tasks while waiting for messages

**Flags:**
- `PM_REMOVE`: Remove message (like GetMessage)
- `PM_NOREMOVE`: Leave message in queue
- `PM_NOYIELD`: Don't yield to other threads (rarely used)

### Comparison

| Feature | GetMessage | PeekMessage |
|---------|-----------|-------------|
| Blocking | Yes | No |
| CPU Usage | Low (waits) | High (continuous polling) |
| Best For | Standard apps | Games, animations |
| Remove Message | Always | Optional (PM_REMOVE) |
| Check for Messages | Returns when available | Returns immediately |

## The MSG Structure

The MSG structure contains message information:

```cpp
typedef struct tagMSG {
    HWND   hwnd;      // Window handle
    UINT   message;   // Message identifier
    WPARAM wParam;    // Additional message info
    LPARAM lParam;    // Additional message info
    DWORD  time;      // Message post time
    POINT  pt;        // Cursor position (screen coords)
} MSG;
```

### Field Descriptions

**hwnd:**
- Handle to window that receives the message
- NULL for thread messages (not window-specific)

**message:**
- Message identifier (e.g., WM_PAINT, WM_KEYDOWN)
- Unsigned integer value
- Predefined in winuser.h

**wParam and lParam:**
- Additional message-specific information
- Meaning depends on message type
- Often contain multiple values packed together

**time:**
- Time message was posted
- In milliseconds since system started
- Get with `GetTickCount()`

**pt:**
- Cursor position when message was posted
- In screen coordinates
- Convert to client coords with `ScreenToClient()`

## TranslateMessage Function

```cpp
BOOL TranslateMessage(const MSG* lpMsg);
```

**Purpose:**
Translates virtual-key messages into character messages.

**What It Does:**
- Processes `WM_KEYDOWN` and `WM_KEYUP` messages
- Generates `WM_CHAR` messages for character keys
- Handles dead keys (accents, combining characters)
- Manages keyboard state

**Example:**

```
User presses 'A' key:
1. WM_KEYDOWN (virtual key VK_A)
   ↓ TranslateMessage called
2. WM_CHAR (character 'A' or 'a' depending on Shift)
3. WM_KEYUP (virtual key VK_A)
```

**When to Skip:**
```cpp
// Skip for dialogs (use IsDialogMessage instead)
if (!IsDialogMessage(hDlg, &msg))
{
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}
```

**Virtual Keys vs Characters:**
- Virtual Keys: VK_A, VK_RETURN, VK_SHIFT (hardware-independent)
- Characters: 'a', 'A', '\n' (actual character to input)

## DispatchMessage Function

```cpp
LRESULT DispatchMessage(const MSG* lpMsg);
```

**Purpose:**
Sends the message to the window procedure.

**What It Does:**
1. Looks up window procedure for `msg.hwnd`
2. Calls window procedure with message parameters
3. Waits for window procedure to return
4. Returns the result from window procedure

**Flow:**

```cpp
// In message loop:
DispatchMessage(&msg);

// Windows calls:
result = WindowProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);

// DispatchMessage returns result
```

**Return Value:**
- Return value from window procedure
- Usually ignored in message loop
- Meaning depends on specific message

## The Window Procedure

The window procedure is a callback function that processes messages.

### Function Signature

```cpp
LRESULT CALLBACK WindowProc(
    HWND   hwnd,    // Window handle
    UINT   uMsg,    // Message identifier
    WPARAM wParam,  // First message parameter
    LPARAM lParam   // Second message parameter
);
```

### Basic Structure

```cpp
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
            // Window being created
            return 0;

        case WM_PAINT:
            // Window needs painting
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                // Draw content
                EndPaint(hwnd, &ps);
            }
            return 0;

        case WM_DESTROY:
            // Window being destroyed
            PostQuitMessage(0);
            return 0;

        default:
            // Pass unhandled messages to default handler
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}
```

### Message Processing Rules

1. **Handle messages you care about**
2. **Call DefWindowProc for all others**
3. **Return appropriate value** (depends on message)
4. **Don't call DefWindowProc** for handled messages (usually)

### Return Values

Different messages expect different return values:

| Message | Return Value |
|---------|-------------|
| WM_CREATE | 0 = success, -1 = fail (destroy window) |
| WM_DESTROY | 0 (ignored) |
| WM_PAINT | 0 (ignored) |
| WM_CLOSE | 0 if handled (don't call DefWindowProc) |
| WM_SIZE | 0 (ignored) |
| WM_SETCURSOR | TRUE if handled, FALSE to pass to DefWindowProc |
| Most messages | Return from DefWindowProc |

## Message Parameters: wParam and lParam

The meaning of wParam and lParam depends on the message.

### Common Patterns

**WM_COMMAND (from menu/button):**
```cpp
case WM_COMMAND:
{
    WORD wmId = LOWORD(wParam);      // Menu/control ID
    WORD wmEvent = HIWORD(wParam);   // Notification code
    HWND hControl = (HWND)lParam;    // Control handle

    switch (wmId)
    {
        case ID_FILE_OPEN:
            // Handle menu command
            break;
    }
    return 0;
}
```

**WM_SIZE (window resized):**
```cpp
case WM_SIZE:
{
    int width = LOWORD(lParam);   // New width
    int height = HIWORD(lParam);  // New height
    UINT flag = wParam;           // SIZE_MINIMIZED, SIZE_MAXIMIZED, etc.

    // Resize child windows, etc.
    return 0;
}
```

**WM_LBUTTONDOWN (left mouse button):**
```cpp
case WM_LBUTTONDOWN:
{
    int xPos = GET_X_LPARAM(lParam);  // X position
    int yPos = GET_Y_LPARAM(lParam);  // Y position
    UINT flags = wParam;              // MK_SHIFT, MK_CONTROL, etc.

    // Handle mouse click
    return 0;
}
```

**WM_KEYDOWN (key pressed):**
```cpp
case WM_KEYDOWN:
{
    UINT vkCode = wParam;         // Virtual key code
    UINT flags = lParam;          // Repeat count, scan code, etc.

    if (vkCode == VK_ESCAPE)
    {
        // Handle Escape key
    }
    return 0;
}
```

### Extracting Values

**LOWORD / HIWORD:**
```cpp
WORD low = LOWORD(lParam);   // Lower 16 bits
WORD high = HIWORD(lParam);  // Upper 16 bits
```

**Mouse Position:**
```cpp
int x = GET_X_LPARAM(lParam);  // Signed X coordinate
int y = GET_Y_LPARAM(lParam);  // Signed Y coordinate

// Or:
POINT pt;
pt.x = LOWORD(lParam);  // Unsigned, cast to signed if needed
pt.y = HIWORD(lParam);
```

## DefWindowProc Function

```cpp
LRESULT DefWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
```

**Purpose:**
Provides default processing for messages you don't handle.

**Critical Rule:**
**Always call DefWindowProc for messages you don't handle!**

**Example:**

```cpp
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;  // We handled it, don't call DefWindowProc

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            // Paint
            EndPaint(hwnd, &ps);
            return 0;  // We handled it
        }

        // DON'T NEED TO LIST EVERY MESSAGE
        // Just pass unhandled messages to DefWindowProc
    }

    // CRITICAL: Pass unhandled messages here
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
```

**What DefWindowProc Does:**
- Handles system messages (minimize, maximize, close, etc.)
- Implements default window behavior
- Processes non-client area messages
- Manages window frame, title bar, etc.

## Common Windows Messages

### Window Lifecycle Messages

| Message | When Sent | Purpose |
|---------|-----------|---------|
| `WM_CREATE` | During CreateWindowEx | Initialize window |
| `WM_DESTROY` | During DestroyWindow | Clean up resources |
| `WM_CLOSE` | User clicks X or Alt+F4 | Confirm/cancel close |
| `WM_QUIT` | PostQuitMessage called | Exit message loop |

### Input Messages

| Message | When Sent | Purpose |
|---------|-----------|---------|
| `WM_KEYDOWN` | Key pressed | Process key press |
| `WM_KEYUP` | Key released | Process key release |
| `WM_CHAR` | Character input | Get character |
| `WM_LBUTTONDOWN` | Left mouse button down | Handle click |
| `WM_MOUSEMOVE` | Mouse moves | Track mouse |
| `WM_MOUSEWHEEL` | Mouse wheel scrolled | Handle scrolling |

### Painting Messages

| Message | When Sent | Purpose |
|---------|-----------|---------|
| `WM_PAINT` | Window needs repainting | Draw content |
| `WM_ERASEBKGND` | Background needs erasing | Erase background |
| `WM_NCPAINT` | Non-client area needs paint | Draw borders/title |

### Window State Messages

| Message | When Sent | Purpose |
|---------|-----------|---------|
| `WM_SIZE` | Window resized | Adjust layout |
| `WM_MOVE` | Window moved | Update position-dependent code |
| `WM_ACTIVATE` | Window activated/deactivated | Manage focus |
| `WM_SETFOCUS` | Window receives focus | Start accepting input |
| `WM_KILLFOCUS` | Window loses focus | Stop accepting input |

### System Messages

| Message | When Sent | Purpose |
|---------|-----------|---------|
| `WM_TIMER` | Timer fires | Periodic processing |
| `WM_COMMAND` | Menu/button clicked | Handle commands |
| `WM_NOTIFY` | Control notification | Handle control events |

## Message Flow and Routing

### Message Sources

Messages come from multiple sources:

1. **System-Generated:**
   - Window creation/destruction
   - Window state changes
   - Paint requests

2. **User Input:**
   - Keyboard
   - Mouse
   - Touch

3. **Application-Generated:**
   - PostMessage
   - SendMessage
   - PostThreadMessage

### Message Routing

```
Message Origin
    ↓
System Message Queue
    ↓
Thread Message Queue  ← GetMessage/PeekMessage retrieves from here
    ↓
DispatchMessage
    ↓
Window Procedure
    ↓
DefWindowProc (if not handled)
```

### Queue vs Non-Queue

**Queued Messages:**
- Go through message queue
- Retrieved by GetMessage
- Can be filtered/removed
- Examples: WM_KEYDOWN, WM_LBUTTONDOWN, WM_TIMER

**Non-Queued Messages:**
- Sent directly to window procedure
- Bypass queue
- Cannot be filtered
- Examples: WM_SIZE, WM_CREATE, WM_GETMINMAXINFO

## Sending vs Posting Messages

### SendMessage (Synchronous)

```cpp
LRESULT result = SendMessage(hwnd, WM_MYMESSAGE, wParam, lParam);
```

**Characteristics:**
- **Synchronous**: Waits for window procedure to process
- **Direct call**: Calls window procedure directly
- **Return value**: Gets return value from window procedure
- **Blocking**: Caller blocked until processing completes

**Use when:**
- Need immediate processing
- Need return value
- Calling from same thread

### PostMessage (Asynchronous)

```cpp
BOOL success = PostMessage(hwnd, WM_MYMESSAGE, wParam, lParam);
```

**Characteristics:**
- **Asynchronous**: Returns immediately
- **Queued**: Places message in queue
- **No return value**: Can't get processing result
- **Non-blocking**: Caller continues immediately

**Use when:**
- Don't need immediate processing
- Don't need return value
- Cross-thread communication

### PostQuitMessage

```cpp
PostQuitMessage(int nExitCode);
```

**Purpose:**
Posts WM_QUIT to message queue, causing GetMessage to return 0.

**Typical Use:**
```cpp
case WM_DESTROY:
    PostQuitMessage(0);  // Exit code 0
    return 0;
```

**Flow:**
1. PostQuitMessage(0) called
2. WM_QUIT placed in queue
3. GetMessage retrieves WM_QUIT and returns 0
4. Message loop exits
5. Application terminates

## Message Queues

Each thread has its own message queue.

### Queue Structure

```
Thread Message Queue:
┌────────────────────┐
│  WM_KEYDOWN        │
│  WM_PAINT          │
│  WM_TIMER          │
│  WM_LBUTTONDOWN    │
│  ...               │
└────────────────────┘
```

### Queue Operations

**Add to Queue:**
- PostMessage (specific window)
- PostThreadMessage (thread, no window)
- System-generated messages

**Remove from Queue:**
- GetMessage (waits, removes)
- PeekMessage with PM_REMOVE (doesn't wait, removes)

**Inspect Queue:**
- PeekMessage with PM_NOREMOVE (leaves message)

### Queue Priority

Messages have priorities:

1. **Sent messages** (highest priority, bypass queue)
2. **Posted messages** (in order received)
3. **Input messages** (keyboard, mouse)
4. **WM_PAINT** (lowest priority, coalesced)
5. **WM_TIMER** (lowest priority)

WM_PAINT and WM_TIMER are retrieved only when queue is otherwise empty.

## Message Filtering

You can filter messages in the message loop or with message-specific APIs.

### Loop Filtering

```cpp
while (GetMessage(&msg, NULL, 0, 0))
{
    // Ignore all mouse messages
    if (msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST)
        continue;

    TranslateMessage(&msg);
    DispatchMessage(&msg);
}
```

### Window Filtering

```cpp
// Get messages only for specific window
while (GetMessage(&msg, hwndMain, 0, 0))
{
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}
```

### Message Range Filtering

```cpp
// Get only keyboard messages
while (GetMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST))
{
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}
```

## Best Practices

1. **Always call DefWindowProc for unhandled messages**
2. **Return correct values from window procedure**
3. **Don't perform lengthy operations in window procedure**
4. **Use PostMessage for async communication**
5. **Use SendMessage for immediate results**
6. **Handle WM_DESTROY and call PostQuitMessage**
7. **Check GetMessage return value**
8. **Don't call DefWindowProc for handled messages**
9. **Keep window procedure efficient**
10. **Use PeekMessage only when necessary (CPU intensive)**

## Summary

The message loop and window procedure work together:

1. **Message Loop** retrieves messages from queue
2. **TranslateMessage** converts virtual keys to characters
3. **DispatchMessage** sends message to window procedure
4. **Window Procedure** processes the message
5. **DefWindowProc** handles unprocessed messages

This architecture makes Windows applications event-driven and responsive.

**Key Takeaways:**
- GetMessage blocks, PeekMessage doesn't
- Always call DefWindowProc for unhandled messages
- Use SendMessage for synchronous, PostMessage for asynchronous
- Each thread has its own message queue
- Window procedure must be efficient

---

**Next Lesson:** [Lesson 34: Keyboard and Mouse Input](#)
**Previous Lesson:** [Lesson 32: Creating Your First Window](#)
