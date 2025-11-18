# Lesson 37: Basic Controls

## Introduction
Master the fundamental Windows controls: buttons, edit boxes, list boxes, and more.

**Topics Covered:**
- Button controls (push, check, radio)
- Edit controls (single and multi-line)
- Static controls for labels
- List boxes
- Combo boxes
- Scroll bars
- Group boxes
- Control notifications

**Time Required:** 4-5 hours

## Button Controls

### Creating Buttons
```cpp
CreateWindowEx(0, L"BUTTON", L"Click Me",
               WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
               10, 10, 100, 30, hwndParent, (HMENU)ID_BUTTON, hInstance, 0);
```

### Button Styles
- BS_PUSHBUTTON: Standard button
- BS_CHECKBOX: Checkbox
- BS_AUTORADIOBUTTON: Radio button
- BS_GROUPBOX: Group box

## Edit Controls

### Single-line Edit
```cpp
HWND hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
                             WS_CHILD | WS_VISIBLE | ES_LEFT,
                             10, 10, 200, 25, hwndParent, (HMENU)ID_EDIT, hInstance, 0);
```

### Getting Edit Text
```cpp
wchar_t buffer[256];
GetWindowText(hEdit, buffer, 256);
```

## List Box

### Creating List Box
```cpp
HWND hList = CreateWindowEx(0, L"LISTBOX", L"",
                             WS_CHILD | WS_VISIBLE | LBS_STANDARD,
                             10, 10, 200, 150, hwndParent, (HMENU)ID_LIST, hInstance, 0);

// Add items
SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)L"Item 1");
SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)L"Item 2");
```

## Control Notifications
Controls send WM_COMMAND notifications:
```cpp
case WM_COMMAND:
    if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == ID_BUTTON) {
        // Button clicked
    }
    if (HIWORD(wParam) == EN_CHANGE && LOWORD(wParam) == ID_EDIT) {
        // Edit text changed
    }
    return 0;
```

## See Code Examples
8 complete control demonstrations in Code/ folder.
