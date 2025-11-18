# Lesson 35: Menus and Accelerators

## Introduction
Master menu creation and management in Windows applications. Learn to create menu bars, popup menus, and keyboard accelerators for professional application interfaces.

**Topics Covered:**
- Creating menus with CreateMenu
- Menu commands and WM_COMMAND
- Popup/context menus with TrackPopupMenu
- Menu item states (checked, disabled, radio)
- Keyboard accelerators and shortcuts
- Dynamic menu manipulation
- Owner-drawn custom menus

**Time Required:** 4-5 hours

## Menu Basics
Menus are essential for application navigation and commands.

### Creating a Menu
```cpp
HMENU hMenu = CreateMenu();
HMENU hSubMenu = CreatePopupMenu();
AppendMenu(hSubMenu, MF_STRING, ID_FILE_NEW, L"&New");
AppendMenu(hSubMenu, MF_STRING, ID_FILE_OPEN, L"&Open");
AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, L"&File");
SetMenu(hwnd, hMenu);
```

### Handling Menu Commands
```cpp
case WM_COMMAND:
    switch (LOWORD(wParam)) {
        case ID_FILE_OPEN:
            // Handle File > Open
            break;
    }
    return 0;
```

## Popup Menus
Context menus that appear on right-click:
```cpp
case WM_RBUTTONDOWN:
    POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
    ClientToScreen(hwnd, &pt);
    TrackPopupMenu(hPopupMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
    return 0;
```

## Accelerators
Keyboard shortcuts for menu commands:
```cpp
HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));
while (GetMessage(&msg, 0, 0, 0)) {
    if (!TranslateAccelerator(hwnd, hAccel, &msg)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
```

## See Code Examples
The Code/ folder contains 8 complete programs demonstrating all menu techniques.
