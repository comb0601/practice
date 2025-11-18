# Lesson 44: Owner-Drawn Controls

## Introduction
Create completely custom-drawn controls with full visual control.

**Topics Covered:**
- WM_DRAWITEM for owner-drawn controls
- Owner-drawn buttons
- Owner-drawn list boxes and combo boxes
- Owner-drawn menus
- WM_MEASUREITEM for sizing
- Using visual styles
- State management (hover, pressed, disabled)
- Custom rendering techniques

**Time Required:** 4-5 hours

## Owner-Drawn Button
```cpp
// Create owner-drawn button
CreateWindowEx(0, L"BUTTON", L"Custom",
               WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
               10, 10, 100, 30, hwnd, (HMENU)ID_BUTTON, hInstance, 0);

// Handle drawing
case WM_DRAWITEM: {
    DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*)lParam;
    if (dis->CtlID == ID_BUTTON) {
        // Custom drawing code
        FillRect(dis->hDC, &dis->rcItem, 
                (dis->itemState & ODS_SELECTED) ? hBrushPressed : hBrushNormal);
        DrawText(dis->hDC, L"Custom", -1, &dis->rcItem, 
                DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    return TRUE;
}
```

## Owner-Drawn List Box
```cpp
CreateWindowEx(0, L"LISTBOX", L"",
               WS_CHILD | WS_VISIBLE | LBS_OWNERDRAWFIXED,
               10, 10, 200, 150, hwnd, (HMENU)ID_LIST, hInstance, 0);

case WM_MEASUREITEM: {
    MEASUREITEMSTRUCT* mis = (MEASUREITEMSTRUCT*)lParam;
    mis->itemHeight = 30;  // Custom height
    return TRUE;
}

case WM_DRAWITEM: {
    DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*)lParam;
    // Draw list item
    return TRUE;
}
```

## See Code Examples
8 complete owner-drawn demonstrations in Code/ folder.
