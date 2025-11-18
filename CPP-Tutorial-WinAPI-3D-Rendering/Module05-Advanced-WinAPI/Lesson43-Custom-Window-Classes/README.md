# Lesson 43: Custom Window Classes

## Introduction
Learn advanced window class techniques: subclassing, superclassing, and creating custom controls.

**Topics Covered:**
- Subclassing existing windows
- Superclassing window classes
- Creating custom controls from scratch
- Window properties (SetProp/GetProp)
- Using cbWndExtra for per-window data
- Custom control notifications
- Message reflection
- Control lifecycle management

**Time Required:** 4-5 hours

## Subclassing
Intercept messages for existing window:
```cpp
WNDPROC g_oldProc;

LRESULT CALLBACK SubclassProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_KEYDOWN && wp == VK_RETURN) {
        // Custom Enter key handling
        return 0;
    }
    return CallWindowProc(g_oldProc, hwnd, msg, wp, lp);
}

// Subclass a control
g_oldProc = (WNDPROC)SetWindowLongPtr(hEdit, GWLP_WNDPROC, (LONG_PTR)SubclassProc);
```

## Superclassing
Create new class based on existing:
```cpp
WNDCLASSEX wc = {};
wc.cbSize = sizeof(wc);
GetClassInfoEx(NULL, L"BUTTON", &wc);
wc.lpszClassName = L"MyCustomButton";
wc.lpfnWndProc = CustomButtonProc;
RegisterClassEx(&wc);
```

## Custom Control
Complete custom control from scratch - see Code/ examples.

## See Code Examples
8 complete demonstrations in Code/ folder.
