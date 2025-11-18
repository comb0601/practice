# Lesson 34: Keyboard and Mouse Input

## Introduction
Master keyboard and mouse input handling in Windows applications. Learn to process key presses, mouse clicks, movement, and wheel events.

**Topics Covered:**
- WM_KEYDOWN, WM_KEYUP, WM_CHAR messages
- Virtual key codes and character input
- Mouse messages: clicks, movement, wheel
- Keyboard state and modifiers
- Double-click handling
- Mouse capture and tracking

**Time Required:** 4-5 hours

## Keyboard Input

### Virtual Keys vs Characters
- **Virtual Keys** (WM_KEYDOWN/WM_KEYUP): Hardware-independent key codes (VK_A, VK_RETURN)
- **Characters** (WM_CHAR): Actual character produced (affected by Shift, Caps Lock)

### Key Messages
| Message | When Sent | Purpose |
|---------|-----------|---------|
| WM_KEYDOWN | Key pressed | Detect key press |
| WM_KEYUP | Key released | Detect key release |
| WM_CHAR | Character produced | Get actual character |
| WM_SYSKEYDOWN | Alt+Key pressed | System keys |

### Example:
```cpp
case WM_KEYDOWN:
    if (wParam == VK_ESCAPE) {
        PostQuitMessage(0);
    }
    return 0;
```

## Mouse Input

### Mouse Messages
| Message | Event | Parameters |
|---------|-------|------------|
| WM_LBUTTONDOWN | Left click | x, y in lParam |
| WM_RBUTTONDOWN | Right click | x, y in lParam |
| WM_MOUSEMOVE | Mouse moves | x, y in lParam |
| WM_MOUSEWHEEL | Wheel scrolled | Delta in wParam |

### Extract Mouse Position:
```cpp
int x = GET_X_LPARAM(lParam);
int y = GET_Y_LPARAM(lParam);
```

## Code Examples
See Code/ folder for 8 complete examples covering all input scenarios.
