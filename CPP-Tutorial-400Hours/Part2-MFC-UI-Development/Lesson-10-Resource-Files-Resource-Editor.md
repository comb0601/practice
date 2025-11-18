# Lesson 10: Resource Files and Resource Editor

**Duration**: 6-8 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding Resource Files
2. Resource.h - ID Definitions
3. The .rc File Format
4. Dialog Resources
5. Menu Resources
6. String Tables
7. Icons, Bitmaps, and Cursors
8. Accelerators (Keyboard Shortcuts)
9. Version Information
10. Resource Editor Tools

---

## 1. Understanding Resource Files

### What Are Resources?

Resources are **read-only data** compiled into your executable:
- Dialogs (visual layouts)
- Menus
- Toolbars
- Icons, bitmaps, cursors
- Strings (for localization)
- Accelerators (keyboard shortcuts)
- Version information

**WHY Resources?**:
- Separate UI from code
- Easy localization
- Visual editing
- Efficient loading

### Resource File Types

```
YourApp.rc          Main resource script (text + binary)
YourApp.rc2         Non-visual resources (text only)
Resource.h          Resource ID definitions (#define IDC_xxx)
YourApp.ico         Application icon
res/ folder         Additional resource files (bitmaps, etc.)
```

---

## 2. Resource.h - ID Definitions

### Structure

```cpp
// Resource.h
//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by YourApp.rc

#define IDD_YOURAPP_DIALOG              102
#define IDD_ABOUTBOX                    103
#define IDR_MAINFRAME                   128
#define IDC_BUTTON_SAVE                 1000
#define IDC_EDIT_NAME                   1001
#define IDC_STATIC_RESULT               1002

// Next default values for new objects
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        130
#define _APS_NEXT_COMMAND_VALUE         32771
#define _APS_NEXT_CONTROL_VALUE         1003
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif
```

### ID Ranges

```cpp
// Dialog IDs
IDD_xxx           100-199    (Dialogs)

// Icon/Bitmap resources
IDR_xxx           100-199    (Resources)

// Command IDs (menus, toolbars)
ID_xxx            32768+     (Commands)

// Control IDs
IDC_xxx           1000+      (Controls)

// String IDs
IDS_xxx           varies     (Strings)

// Standard IDs (predefined)
IDOK              1
IDCANCEL          2
IDABORT           3
IDRETRY           4
IDIGNORE          5
IDYES             6
IDNO              7
```

### Adding Custom IDs

```cpp
// Add manually to Resource.h
#define IDC_BUTTON_CUSTOM     1100
#define IDS_WELCOME_MESSAGE   5000
#define ID_FILE_CUSTOM        32800

// Or use Resource Editor (automatic)
```

---

## 3. The .rc File Format

### Format Overview

```cpp
// YourApp.rc

#include "resource.h"
#include "afxres.h"

/////////////////////////////////////////////////////////////////////////////
// Dialog

IDD_YOURAPP_DIALOG DIALOGEX 0, 0, 320, 200
STYLE DS_SETFONT | DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "My Application"
FONT 8, "MS Sans Serif"
BEGIN
    DEFPUSHBUTTON   "OK",IDOK,209,179,50,14
    PUSHBUTTON      "Cancel",IDCANCEL,263,179,50,14
    EDITTEXT        IDC_EDIT_NAME,50,50,200,14,ES_AUTOHSCROLL
    LTEXT           "Name:",IDC_STATIC,20,52,25,8
END

/////////////////////////////////////////////////////////////////////////////
// Icon

IDR_MAINFRAME          ICON                    "res\\YourApp.ico"

/////////////////////////////////////////////////////////////////////////////
// String Table

STRINGTABLE
BEGIN
    IDS_ABOUTBOX            "&About..."
    IDS_WELCOME             "Welcome to My Application"
END
```

### Editing .rc Files

**DON'T** edit manually unless necessary:
- Use Resource Editor for dialogs, menus
- Binary sections will corrupt if edited as text

**CAN** edit manually:
- String tables
- Simple text resources
- Version information

---

## 4. Dialog Resources

### Dialog Template Format

```
IDD_MYDIALOG DIALOGEX x, y, width, height
STYLE style_flags
CAPTION "Dialog Title"
FONT size, "Font Name"
BEGIN
    control_type "Caption", ID, x, y, width, height, style
    ...
END
```

### Example Dialog Resource

```
IDD_LOGIN_DIALOG DIALOGEX 0, 0, 250, 150
STYLE DS_SETFONT | DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "Login"
FONT 8, "MS Sans Serif"
BEGIN
    LTEXT           "Username:",IDC_STATIC,20,20,50,8
    EDITTEXT        IDC_EDIT_USERNAME,80,18,150,14,ES_AUTOHSCROLL
    LTEXT           "Password:",IDC_STATIC,20,40,50,8
    EDITTEXT        IDC_EDIT_PASSWORD,80,38,150,14,ES_PASSWORD | ES_AUTOHSCROLL
    DEFPUSHBUTTON   "OK",IDOK,85,120,50,14
    PUSHBUTTON      "Cancel",IDCANCEL,145,120,50,14
END
```

### Dialog Units vs Pixels

Dialog units are font-relative:
```
Horizontal: 1 DLU = 1/4 average character width
Vertical:   1 DLU = 1/8 character height

For 8-point MS Sans Serif:
    4 DLU ≈ 1 character wide
    8 DLU ≈ 1 character tall
```

**Converting**:
```cpp
CRect rcDlg(0, 0, 100, 50);  // 100x50 dialog units
MapDialogRect(&rcDlg);        // Convert to pixels
// rcDlg now contains pixel dimensions
```

---

## 5. Menu Resources

### Creating Menus

```
// In .rc file
IDR_MAINFRAME MENU
BEGIN
    POPUP "&File"
    BEGIN
        MENUITEM "&New\tCtrl+N",               ID_FILE_NEW
        MENUITEM "&Open...\tCtrl+O",           ID_FILE_OPEN
        MENUITEM "&Save\tCtrl+S",              ID_FILE_SAVE
        MENUITEM "Save &As...",                 ID_FILE_SAVE_AS
        MENUITEM SEPARATOR
        MENUITEM "E&xit",                       ID_APP_EXIT
    END
    POPUP "&Edit"
    BEGIN
        MENUITEM "&Undo\tCtrl+Z",              ID_EDIT_UNDO
        MENUITEM SEPARATOR
        MENUITEM "Cu&t\tCtrl+X",               ID_EDIT_CUT
        MENUITEM "&Copy\tCtrl+C",              ID_EDIT_COPY
        MENUITEM "&Paste\tCtrl+V",             ID_EDIT_PASTE
    END
    POPUP "&Help"
    BEGIN
        MENUITEM "&About...",                   ID_APP_ABOUT
    END
END
```

### Using Menus

```cpp
// In frame window
BOOL CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Load menu
    CMenu menu;
    menu.LoadMenu(IDR_MAINFRAME);
    SetMenu(&menu);
    menu.Detach();  // Menu now owned by window

    return 0;
}

// Menu command handlers
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_COMMAND(ID_FILE_NEW, &CMainFrame::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
    ON_COMMAND(ID_APP_ABOUT, &CMainFrame::OnAppAbout)
END_MESSAGE_MAP()

void CMainFrame::OnFileNew()
{
    // Handle File → New
}
```

---

## 6. String Tables

### Creating String Tables

```
// In .rc file
STRINGTABLE
BEGIN
    IDS_APP_TITLE           "My Application"
    IDS_WELCOME             "Welcome to the application!"
    IDS_ERROR_FILE_NOT_FOUND "File not found"
    IDS_CONFIRM_DELETE      "Are you sure you want to delete?"
END
```

### Loading Strings

```cpp
// Load string from resources
CString strWelcome;
strWelcome.LoadString(IDS_WELCOME);
MessageBox(strWelcome);

// Or use AfxFormatString
CString strMessage;
AfxFormatString1(strMessage, IDS_FILE_OPENED, _T("document.txt"));
// IDS_FILE_OPENED = "File %1 opened successfully"
// Result: "File document.txt opened successfully"

// Multiple parameters
CString str;
AfxFormatString2(str, IDS_COPIED, _T("5"), _T("file.txt"));
// IDS_COPIED = "Copied %1 bytes from %2"
// Result: "Copied 5 bytes from file.txt"
```

### Benefits of String Tables

1. **Localization**: Easy to translate
2. **Centralized**: All strings in one place
3. **Compile-time checking**: IDs verified
4. **Reusability**: Use same string in multiple places

---

## 7. Icons, Bitmaps, and Cursors

### Icons

```cpp
// In .rc file
IDR_MAINFRAME          ICON    "res\\YourApp.ico"
IDI_ICON_SUCCESS       ICON    "res\\success.ico"
IDI_ICON_ERROR         ICON    "res\\error.ico"

// Loading icon
HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

// Setting dialog icon
BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetIcon(hIcon, TRUE);   // Large icon
    SetIcon(hIcon, FALSE);  // Small icon

    return TRUE;
}

// Displaying in static control
CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_ICON);
pStatic->SetIcon(hIcon);
```

### Bitmaps

```cpp
// In .rc file
IDB_BITMAP1            BITMAP  "res\\image.bmp"
IDB_BACKGROUND         BITMAP  "res\\background.bmp"

// Loading bitmap
CBitmap bitmap;
bitmap.LoadBitmap(IDB_BITMAP1);

// Displaying in static control
CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_IMAGE);
HBITMAP hBitmap = (HBITMAP)::LoadImage(
    AfxGetResourceHandle(),
    MAKEINTRESOURCE(IDB_BITMAP1),
    IMAGE_BITMAP,
    0, 0,
    LR_DEFAULTCOLOR);
pStatic->SetBitmap(hBitmap);
```

### Cursors

```cpp
// In .rc file
IDC_CURSOR_CUSTOM      CURSOR  "res\\custom.cur"

// Loading and setting cursor
HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR_CUSTOM);
SetCursor(hCursor);

// Setting as window cursor
BOOL CMyWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (nHitTest == HTCLIENT)
    {
        SetCursor(hCursor);
        return TRUE;
    }

    return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
```

---

## 8. Accelerators (Keyboard Shortcuts)

### Creating Accelerators

```
// In .rc file
IDR_MAINFRAME ACCELERATORS
BEGIN
    "N",            ID_FILE_NEW,            VIRTKEY, CONTROL
    "O",            ID_FILE_OPEN,           VIRTKEY, CONTROL
    "S",            ID_FILE_SAVE,           VIRTKEY, CONTROL
    VK_F1,          ID_HELP,                VIRTKEY
    VK_DELETE,      ID_EDIT_DELETE,         VIRTKEY
    VK_INSERT,      ID_EDIT_INSERT,         VIRTKEY, CONTROL
END
```

### Using Accelerators

```cpp
// In application class
BOOL CMyApp::InitInstance()
{
    // ...

    // Load accelerators
    m_hAccelTable = LoadAccelerators(IDR_MAINFRAME);

    // ...
}

// Accelerators automatically processed by MFC message loop
// Commands routed to ON_COMMAND handlers
```

---

## 9. Version Information

### Version Resource

```
// In .rc file
VS_VERSION_INFO VERSIONINFO
 FILEVERSION 1,0,0,1
 PRODUCTVERSION 1,0,0,1
 FILEFLAGSMASK 0x3fL
#ifdef _DEBUG
 FILEFLAGS 0x1L
#else
 FILEFLAGS 0x0L
#endif
 FILEOS 0x40004L
 FILETYPE 0x1L
 FILESUBTYPE 0x0L
BEGIN
    BLOCK "StringFileInfo"
    BEGIN
        BLOCK "040904b0"
        BEGIN
            VALUE "CompanyName", "My Company"
            VALUE "FileDescription", "My Application"
            VALUE "FileVersion", "1.0.0.1"
            VALUE "InternalName", "MyApp"
            VALUE "LegalCopyright", "Copyright (C) 2025"
            VALUE "OriginalFilename", "MyApp.exe"
            VALUE "ProductName", "My Application"
            VALUE "ProductVersion", "1.0.0.1"
        END
    END
    BLOCK "VarFileInfo"
    BEGIN
        VALUE "Translation", 0x409, 1200
    END
END
```

### Reading Version Info

```cpp
CString GetFileVersion(LPCTSTR lpszFilePath)
{
    DWORD dwHandle;
    DWORD dwSize = GetFileVersionInfoSize(lpszFilePath, &dwHandle);

    if (dwSize == 0)
        return _T("");

    BYTE* pVersionInfo = new BYTE[dwSize];
    if (!GetFileVersionInfo(lpszFilePath, dwHandle, dwSize, pVersionInfo))
    {
        delete[] pVersionInfo;
        return _T("");
    }

    VS_FIXEDFILEINFO* pFileInfo;
    UINT nLen;
    if (VerQueryValue(pVersionInfo, _T("\\"),
                     (void**)&pFileInfo, &nLen))
    {
        CString strVersion;
        strVersion.Format(_T("%d.%d.%d.%d"),
            HIWORD(pFileInfo->dwFileVersionMS),
            LOWORD(pFileInfo->dwFileVersionMS),
            HIWORD(pFileInfo->dwFileVersionLS),
            LOWORD(pFileInfo->dwFileVersionLS));

        delete[] pVersionInfo;
        return strVersion;
    }

    delete[] pVersionInfo;
    return _T("");
}
```

---

## 10. Resource Editor Tools

### Dialog Editor

**Opening**:
- View → Resource View (Ctrl+Shift+E)
- Expand .rc → Dialog → Double-click dialog

**Tools**:
- Toolbox: Drag controls to dialog
- Properties: Edit control properties (F4)
- Guide lines: Align controls
- Tab order: Ctrl+D (set tab order)

**Alignment**:
- Format → Align → Left/Right/Top/Bottom
- Format → Make Same Size
- Format → Space Evenly
- Format → Center in Dialog

### Testing Dialogs

```
Test Dialog (Ctrl+T):
    - Shows dialog as it will appear
    - Can interact with controls
    - No code runs (just visual test)
```

### Icon/Bitmap Editor

**Opening**:
- Resource View → Icon/Bitmap → Double-click

**Tools**:
- Pencil, line, rectangle, circle
- Fill tool
- Color picker
- Zoom

### String Table Editor

**Opening**:
- Resource View → String Table → Double-click

**Adding Strings**:
1. Click in empty row
2. Enter ID (e.g., IDS_WELCOME)
3. Enter Value
4. Enter Caption (optional comment)

---

## Summary

### Key Takeaways

1. **Resource.h**: Defines all resource IDs
2. **.rc file**: Contains visual resources (dialogs, menus, etc.)
3. **Dialog templates**: Layout defined in .rc file
4. **String tables**: Centralize strings for localization
5. **Icons/Bitmaps**: Binary resources compiled into exe
6. **Accelerators**: Keyboard shortcuts
7. **Resource Editor**: Visual tools for editing resources

### Best Practices

- Use Resource Editor for visual resources
- Keep Resource.h auto-generated (don't edit manually)
- Use string tables for all user-visible strings
- Version your resources
- Organize resources in res/ folder

### Next Steps

In **Lesson 11: Event Handling and Message Routing**, you'll learn:
- Command routing in SDI/MDI
- Update UI handlers
- Reflected messages
- Custom message handling

---

**Next Lesson**: [Lesson 11: Event Handling and Message Routing](Lesson-11-Event-Handling-Message-Routing.md)
