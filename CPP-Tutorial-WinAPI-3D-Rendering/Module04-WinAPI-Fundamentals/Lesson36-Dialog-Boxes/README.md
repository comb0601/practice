# Lesson 36: Dialog Boxes

## Introduction
Learn to create and manage dialog boxes - essential UI elements for user interaction.

**Topics Covered:**
- Modal dialogs with DialogBox
- Modeless dialogs with CreateDialog
- Common dialogs (Open, Save, Color, Font)
- Dialog procedures and message handling
- Custom dialog templates
- Dialog control manipulation
- Tab order and keyboard navigation

**Time Required:** 4-5 hours

## Modal vs Modeless Dialogs

### Modal Dialog
Blocks parent window until closed:
```cpp
INT_PTR result = DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1),
                            hwndParent, DialogProc);
```

### Modeless Dialog
Allows interaction with parent:
```cpp
HWND hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG2),
                          hwndParent, DialogProc);
ShowWindow(hDlg, SW_SHOW);
```

## Dialog Procedure
```cpp
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_INITDIALOG:
            return TRUE;
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            break;
    }
    return FALSE;
}
```

## Common Dialogs

### Open File Dialog
```cpp
OPENFILENAME ofn = {sizeof(ofn)};
wchar_t szFile[260] = {0};
ofn.hwndOwner = hwnd;
ofn.lpstrFile = szFile;
ofn.nMaxFile = sizeof(szFile)/sizeof(wchar_t);
ofn.lpstrFilter = L"Text Files\0*.TXT\0All Files\0*.*\0";
ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

if (GetOpenFileName(&ofn)) {
    // File selected in szFile
}
```

## See Code Examples
8 complete dialog demonstrations in Code/ folder.
