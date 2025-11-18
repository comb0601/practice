# Lesson 38: Advanced Controls

## Introduction
Explore advanced Windows common controls for rich user interfaces.

**Topics Covered:**
- List View control (report, icon views)
- Tree View control (hierarchical data)
- Tab control (tabbed interfaces)
- Progress bars
- Trackbars (sliders)
- Up-down (spinner) controls
- Rich Edit control
- Toolbar control

**Time Required:** 4-5 hours

## List View Control

### Creating List View
```cpp
#include <commctrl.h>
InitCommonControlsEx(...);

HWND hList = CreateWindowEx(0, WC_LISTVIEW, L"",
                             WS_CHILD | WS_VISIBLE | LVS_REPORT,
                             0, 0, 400, 300, hwndParent, (HMENU)ID_LISTVIEW, hInstance, 0);

// Add columns
LVCOLUMN lvc = {LVCF_TEXT | LVCF_WIDTH, 0, 100, L"Column 1"};
ListView_InsertColumn(hList, 0, &lvc);
```

## Tree View Control

### Creating Tree View
```cpp
HWND hTree = CreateWindowEx(0, WC_TREEVIEW, L"",
                             WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_HASBUTTONS,
                             0, 0, 200, 400, hwndParent, (HMENU)ID_TREEVIEW, hInstance, 0);

// Add items
TVINSERTSTRUCT tvis = {TVI_ROOT, TVI_LAST};
tvis.item.mask = TVIF_TEXT;
tvis.item.pszText = L"Root Item";
HTREEITEM hRoot = TreeView_InsertItem(hTree, &tvis);
```

## Tab Control

### Creating Tabs
```cpp
HWND hTab = CreateWindowEx(0, WC_TABCONTROL, L"",
                            WS_CHILD | WS_VISIBLE,
                            0, 0, 500, 400, hwndParent, (HMENU)ID_TAB, hInstance, 0);

TCITEM tie = {TCIF_TEXT, 0, 0, L"Tab 1"};
TabCtrl_InsertItem(hTab, 0, &tie);
```

## Rich Edit Control
Supports RTF formatting:
```cpp
LoadLibrary(L"Riched20.dll");
HWND hRichEdit = CreateWindowEx(0, RICHEDIT_CLASS, L"",
                                 WS_CHILD | WS_VISIBLE | ES_MULTILINE,
                                 0, 0, 400, 300, hwndParent, (HMENU)ID_RICHEDIT, hInstance, 0);
```

## See Code Examples
8 complete advanced control demonstrations in Code/ folder.
