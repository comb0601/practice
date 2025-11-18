# Lesson 05: MFC Application Wizard - Understanding Generated Code

**Duration**: 6-8 hours
**Difficulty**: Intermediate

## Table of Contents
1. MFC Application Wizard Deep Dive
2. Every Generated File Explained
3. Precompiled Headers (pch.h / stdafx.h)
4. The Framework.h File
5. Resource Files Deep Dive
6. Application Class Generated Code
7. Dialog Class Generated Code
8. Understanding Auto-Generated Comments
9. What You Can Safely Edit
10. What You Should Never Edit
11. Customizing Wizard Settings
12. Real-World Project Walkthrough

---

## 1. MFC Application Wizard Deep Dive

### Wizard Pages Explained

When you create an MFC application, the wizard presents several configuration pages. Let's understand EVERY option:

#### Page 1: Application Type

```
Application type:
( ) Single document        - One document at a time (e.g., Notepad)
( ) Multiple documents     - Multiple documents (e.g., Excel)
(•) Dialog based          - Simple dialog window (EASIEST)
( ) MFC ActiveX Control   - COM control (advanced)
```

**WHAT EACH GENERATES**:

**Single Document (SDI)**:
```
Generated classes:
- CYourApp : public CWinApp         // Application
- CMainFrame : public CFrameWnd     // Main window frame
- CYourDoc : public CDocument       // Data/document
- CYourView : public CView          // Visual representation

Generated files: 10-15 files
Complexity: HIGH (Document/View architecture)
```

**Multiple Document (MDI)**:
```
Generated classes:
- CYourApp : public CWinApp
- CMainFrame : public CMDIFrameWnd
- CChildFrame : public CMDIChildWnd
- CYourDoc : public CDocument
- CYourView : public CView

Generated files: 12-18 files
Complexity: VERY HIGH (MDI + Document/View)
```

**Dialog Based**:
```
Generated classes:
- CYourApp : public CWinApp
- CYourDlg : public CDialogEx

Generated files: 8-12 files
Complexity: LOW (best for learning)
```

**Project Style**:
```
[•] MFC standard          - Traditional look
[ ] MFC application       - Ribbon interface (Office-style)
```

**Use Unicode Libraries**:
```
[✓] Use Unicode libraries - ALWAYS CHECK THIS
```

**WHY Unicode matters**:
```cpp
// With Unicode (recommended):
CString str = _T("Hello");       // Works everywhere
SetWindowText(_T("Hello"));      // Safe

// Without Unicode (old):
CString str = "Hello";           // Might break
SetWindowText("Hello");          // Not international
```

#### Page 2: User Interface Features (Dialog-Based)

```
Dialog title: _______________     ← Window title bar text

[✓] Minimize box                  ← Allow minimize
[✓] Maximize box                  ← Allow maximize
[ ] About box                     ← Add Help → About dialog
[ ] System menu                   ← Window control menu
[✓] Use MFC in a shared DLL      ← Smaller exe, needs MFC DLL
                                    or
[ ] Use MFC in a static library  ← Larger exe, no dependencies
```

**DLL vs Static**:

```
Shared DLL:
+ Smaller EXE (50-200 KB)
+ Updates through MFC DLL updates
- Requires MFC DLLs on target machine

Static Library:
+ No dependencies
+ Single EXE deployment
- Larger EXE (1-3 MB)
```

#### Page 3: Advanced Features

```
[✓] ActiveX controls              ← Allow ActiveX controls in dialog
[ ] Common Control Manifest       ← Use Windows visual styles
[ ] Support Restart Manager       ← Windows 7+ crash recovery
[ ] Windows Sockets               ← Network programming
```

**WHEN TO CHECK**:
- **ActiveX controls**: Always (safe to enable)
- **Common Control Manifest**: For modern look
- **Restart Manager**: Enterprise applications
- **Windows Sockets**: Network/internet apps

#### Page 4: Generated Classes

```
Class name:      CYourAppDlg            ← Your dialog class name
Header file:     YourAppDlg.h           ← .h filename
Implementation:  YourAppDlg.cpp         ← .cpp filename
Base class:      CDialogEx              ← DON'T CHANGE (use CDialogEx)
```

**BASE CLASS OPTIONS**:
```
CDialogEx   - Extended dialog (Windows 7+) - USE THIS
CDialog     - Standard dialog (legacy)
```

---

## 2. Every Generated File Explained

### Complete File List

After clicking Finish, the wizard generates these files:

```
YourProject/
│
├── Header Files/
│   ├── framework.h          ★ Common includes - READ THIS
│   ├── YourApp.h            ★ Application class - EDIT THIS
│   ├── YourAppDlg.h         ★ Main dialog class - EDIT THIS
│   ├── pch.h                ☆ Precompiled header - DON'T EDIT
│   ├── Resource.h           ★ Resource IDs - AUTO-GENERATED
│   └── targetver.h          ☆ SDK version - RARELY EDIT
│
├── Source Files/
│   ├── YourApp.cpp          ★ Application implementation - EDIT
│   ├── YourAppDlg.cpp       ★ Dialog implementation - EDIT
│   └── pch.cpp              ☆ PCH source - DON'T EDIT
│
├── Resource Files/
│   ├── YourApp.rc           ★ Resources (dialogs, menus) - EDIT
│   ├── YourApp.rc2          ☆ Non-visual resources - RARELY EDIT
│   ├── YourApp.ico          ★ Application icon - REPLACE
│   └── res/ (folder)
│       └── YourApp.rc2
│
└── ReadMe.txt               ☆ Documentation - IGNORE
```

**LEGEND**:
- ★ Files you'll frequently edit
- ☆ Files you rarely/never edit

### File Size Reference

```
framework.h         ~2 KB   (includes)
YourApp.h           ~1 KB   (application class declaration)
YourApp.cpp         ~3 KB   (application implementation)
YourAppDlg.h        ~2 KB   (dialog class declaration)
YourAppDlg.cpp      ~6 KB   (dialog implementation)
pch.h               ~500 B  (precompiled header)
pch.cpp             ~200 B  (precompiled source)
Resource.h          ~1 KB   (resource IDs)
YourApp.rc          ~15 KB  (resource script - binary/text hybrid)
targetver.h         ~500 B  (SDK version)
ReadMe.txt          ~3 KB   (auto-generated docs)
```

---

## 3. Precompiled Headers (pch.h / stdafx.h)

### What is a Precompiled Header?

**Problem**: MFC headers are HUGE (thousands of lines). Compiling them every time is slow.

**Solution**: Precompile common headers once, reuse the compiled version.

```cpp
// pch.h (or stdafx.h in older VS versions)
// ======================================
// PRECOMPILED HEADER FILE
//
// DO NOT EDIT THIS FILE unless adding new system includes
// This file is compiled ONCE and reused across all .cpp files

#ifndef PCH_H
#define PCH_H

// Add C RunTime Library files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Windows Header Files
#include <windows.h>

// MFC includes
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdialogex.h>    // MFC dialog extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>       // MFC date/time controls
#endif

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC common controls
#endif

#endif //PCH_H
```

### pch.cpp - The Precompilation Trigger

```cpp
// pch.cpp
// =======
// Source file that includes just the precompiled header
// DO NOT ADD CODE HERE

#include "pch.h"

// That's it! This file creates the precompiled header file (.pch)
```

### How It Works

```
Build Process:
1. Compiler compiles pch.cpp FIRST
   → Creates YourApp.pch (precompiled header file - large, 50+ MB)

2. Compiler compiles YourApp.cpp
   → Finds #include "pch.h"
   → Uses pre-compiled YourApp.pch instead of re-compiling headers
   → MUCH FASTER

3. Compiler compiles YourAppDlg.cpp
   → Same thing, uses YourApp.pch
   → FAST
```

**EVERY .cpp file MUST include pch.h FIRST**:

```cpp
// YourApp.cpp - CORRECT
#include "pch.h"             // ← MUST BE FIRST
#include "framework.h"
#include "YourApp.h"
#include "YourAppDlg.h"

// WRONG - won't compile
#include "framework.h"       // ← pch.h should be first
#include "pch.h"             // ← TOO LATE
```

### When to Edit pch.h

**ADD to pch.h**:
- System headers you use everywhere (`<vector>`, `<map>`, etc.)
- Third-party library headers used in many files

```cpp
// pch.h - adding std::vector
#include <afxcmn.h>

// Add frequently-used STL headers
#include <vector>
#include <map>
#include <string>

#endif //PCH_H
```

**DON'T ADD to pch.h**:
- Your own header files (.h files from your project)
- Headers used in only one .cpp file
- Frequently-changing headers (slows down compilation)

---

## 4. The Framework.h File

### What is framework.h?

New in **Visual Studio 2019+**, `framework.h` is an extra layer between pch.h and your code.

```cpp
// framework.h
// ===========
// Include file for standard system include files,
// or project-specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN      // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"    // Define minimum Windows version

// Windows Header Files
#include <afxwin.h>       // MFC core
#include <afxext.h>       // MFC extensions
#include <afxdialogex.h>  // MFC dialog extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>     // MFC date/time picker
#endif

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>       // MFC common controls
#endif

#include <afxcontrolbars.h>  // MFC ribbon and control bar support
```

### framework.h vs pch.h

**OLD (VS 2017 and earlier)**:
```
YourApp.cpp
└─ includes stdafx.h (contains all MFC includes)
```

**NEW (VS 2019+)**:
```
YourApp.cpp
└─ includes pch.h
   └─ includes framework.h (contains all MFC includes)
```

**WHY TWO FILES?**
- `pch.h` - Pure system headers (C runtime, Windows, MFC)
- `framework.h` - Project-specific framework configuration

**WHERE TO ADD INCLUDES**:

```cpp
// System headers (rarely change) → pch.h
#include <vector>

// MFC headers → framework.h
#include <afxcmn.h>

// Your project headers → individual .cpp files
#include "MyCustomClass.h"
```

---

## 5. Resource Files Deep Dive

### Resource.h - Resource ID Definitions

```cpp
// Resource.h
// ==========
// THIS FILE IS AUTO-GENERATED
// Edit with Resource Editor, not manually

//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by YourApp.rc
//

#define IDD_YOURAPP_DIALOG              102
#define IDD_ABOUTBOX                    103
#define IDR_MAINFRAME                   128
#define IDC_BUTTON_CLICK                1000
#define IDC_EDIT_INPUT                  1001
#define IDC_STATIC_RESULT               1002

// Next default values for new objects
//
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        130
#define _APS_NEXT_COMMAND_VALUE         32771
#define _APS_NEXT_CONTROL_VALUE         1003
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif
```

**UNDERSTANDING THE IDs**:

```cpp
IDD_YOURAPP_DIALOG    = 102    // Dialog resource ID
IDR_MAINFRAME         = 128    // Main icon resource
IDC_BUTTON_CLICK      = 1000   // Control IDs start at 1000
IDC_EDIT_INPUT        = 1001
IDC_STATIC_RESULT     = 1002

// Special IDs (predefined):
IDOK                  = 1      // OK button
IDCANCEL              = 2      // Cancel button
IDABORT               = 3      // Abort button
IDRETRY               = 4      // Retry button
IDIGNORE              = 5      // Ignore button
IDYES                 = 6      // Yes button
IDNO                  = 7      // No button
```

**AUTO-INCREMENT SECTION**:

```cpp
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        130  // Next IDD_xxx
#define _APS_NEXT_COMMAND_VALUE         32771 // Next ID_xxx (menu)
#define _APS_NEXT_CONTROL_VALUE         1003  // Next IDC_xxx
#define _APS_NEXT_SYMED_VALUE           101   // Next symbol
#endif
#endif
```

**WHAT THIS MEANS**: When you add a control in Resource Editor, Visual Studio:
1. Looks at `_APS_NEXT_CONTROL_VALUE` (currently 1003)
2. Assigns your new control ID = 1003
3. Increments `_APS_NEXT_CONTROL_VALUE` to 1004
4. Adds `#define IDC_YOUR_CONTROL 1003`

### YourApp.rc - The Resource Script

This file contains ALL visual resources in a semi-binary format:

```cpp
// YourApp.rc (excerpt)
// ====================
// This is a TEXT file but contains binary data
// DO NOT edit manually - use Resource Editor!

#include "resource.h"
#include "afxres.h"

/////////////////////////////////////////////////////////////////////////////
// English (United States) resources

LANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_US

/////////////////////////////////////////////////////////////////////////////
// Dialog

IDD_YOURAPP_DIALOG DIALOGEX 0, 0, 320, 200
STYLE DS_SETFONT | DS_MODALFRAME | WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU
CAPTION "My Application"
FONT 8, "MS Sans Serif"
BEGIN
    DEFPUSHBUTTON   "OK",IDOK,209,179,50,14
    PUSHBUTTON      "Cancel",IDCANCEL,263,179,50,14
    LTEXT           "TODO: Place dialog controls here.",IDC_STATIC,50,90,200,8
END

/////////////////////////////////////////////////////////////////////////////
// Icon

IDR_MAINFRAME           ICON                    "res\\YourApp.ico"

/////////////////////////////////////////////////////////////////////////////
// String Table

STRINGTABLE
BEGIN
    IDS_ABOUTBOX            "&About YourApp..."
END
```

**STRUCTURE**:
```
Dialog Definition:
    IDD_YOURAPP_DIALOG DIALOGEX 0, 0, 320, 200
                                    ↑   ↑   ↑    ↑
                                    X   Y   W    H (dialog units, not pixels!)
    BEGIN
        DEFPUSHBUTTON "OK", IDOK, 209, 179, 50, 14
                       ↑     ↑     ↑    ↑    ↑   ↑
                      Text   ID    X    Y    W   H
    END
```

**DIALOG UNITS vs PIXELS**:

Dialog units are NOT pixels - they're based on the dialog's font size:
```
1 horizontal dialog unit = 1/4 of average character width
1 vertical dialog unit   = 1/8 of character height

For 8-point MS Sans Serif:
    4 DLU ≈ 1 character width
    8 DLU ≈ 1 character height
```

### YourApp.rc2 - Non-Visual Resources

```cpp
// YourApp.rc2
// ===========
// Resources that are NOT edited by Visual Studio Resource Editor
// Add manually-edited resources here

#ifdef APSTUDIO_INVOKED
#error This file is not editable by Microsoft Visual C++
#endif //APSTUDIO_INVOKED

/////////////////////////////////////////////////////////////////////////////
// Add manually-edited resources here...

// Example: Version information
// Example: Custom binary resources
// Example: Localization strings

/////////////////////////////////////////////////////////////////////////////
```

**WHEN TO USE**: Rarely. For resources you want to edit as text, not visually.

---

## 6. Application Class Generated Code

### YourApp.h - Application Class Declaration

```cpp
// YourApp.h
#pragma once

#ifndef __AFXWIN_H__
    #error "include 'pch.h' before including this file"
#endif

#include "resource.h"  // Resource symbols

// CYourApp - The application class
class CYourApp : public CWinApp
{
public:
    CYourApp();  // Constructor

// Overrides
public:
    virtual BOOL InitInstance();  // ← WHERE YOUR APP STARTS

// Implementation
    DECLARE_MESSAGE_MAP()
};

extern CYourApp theApp;  // ← Global application object
```

**WHAT EACH PART DOES**:

```cpp
#ifndef __AFXWIN_H__
    #error "include 'pch.h' before including this file"
#endif
```
**WHY**: Ensures pch.h is included first (pch.h includes afxwin.h)

```cpp
class CYourApp : public CWinApp
```
**WHAT**: Your application class inherits from MFC's CWinApp

```cpp
virtual BOOL InitInstance();
```
**WHEN**: Called when application starts (like main())

```cpp
extern CYourApp theApp;
```
**WHAT**: Declares the ONE global application object

### YourApp.cpp - Application Implementation

```cpp
// YourApp.cpp
#include "pch.h"
#include "framework.h"
#include "YourApp.h"
#include "YourAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW  // Memory leak detection in debug builds
#endif

// The one and only CYourApp object
CYourApp theApp;  // ← GLOBAL OBJECT - created before main()

// Constructor
CYourApp::CYourApp()
{
    // TODO: Add construction code here
    // Place all significant initialization in InitInstance
}

// Message map
BEGIN_MESSAGE_MAP(CYourApp, CWinApp)
END_MESSAGE_MAP()

// CYourApp initialization
BOOL CYourApp::InitInstance()
{
    // Call base class
    CWinApp::InitInstance();

    // Change the registry key to store settings under
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));

    // Create main dialog
    CYourAppDlg dlg;
    m_pMainWnd = &dlg;  // Set as main window

    // Show dialog
    INT_PTR nResponse = dlg.DoModal();

    // Check result
    if (nResponse == IDOK)
    {
        // TODO: User clicked OK
    }
    else if (nResponse == IDCANCEL)
    {
        // TODO: User clicked Cancel
    }
    else if (nResponse == -1)
    {
        TRACE(traceAppMsg, 0, "Warning: dialog creation failed.\n");
    }

    // Return FALSE to exit app (dialog-based apps exit after main dialog)
    return FALSE;
}
```

**EXECUTION FLOW**:

```
1. Windows loads YourApp.exe
2. C++ runtime creates global object 'theApp' (before main!)
3. C++ runtime calls CYourApp constructor
4. C++ runtime calls WinMain (MFC provides this - you don't see it)
5. MFC calls theApp.InitInstance() ← YOUR CODE STARTS HERE
6. Your code creates and shows dialog
7. DoModal() runs message loop (blocks until dialog closes)
8. InitInstance() returns FALSE
9. Application exits
```

---

## 7. Dialog Class Generated Code

### YourAppDlg.h - Dialog Class Declaration

```cpp
// YourAppDlg.h
#pragma once

// CYourAppDlg dialog
class CYourAppDlg : public CDialogEx
{
// Construction
public:
    CYourAppDlg(CWnd* pParent = nullptr);  // Constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_YOURAPP_DIALOG };  // Links to resource
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV

// Implementation
protected:
    HICON m_hIcon;  // Application icon

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
};
```

**SECTIONS EXPLAINED**:

```cpp
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_YOURAPP_DIALOG };
#endif
```
**WHAT**: Links this class to the dialog resource (IDD_YOURAPP_DIALOG from Resource.h)
**WHY**: Resource Editor needs this to know which dialog to open

```cpp
virtual void DoDataExchange(CDataExchange* pDX);
```
**WHAT**: DDX (Dialog Data Exchange) - connects variables to controls
**WHEN**: Called by UpdateData()

```cpp
HICON m_hIcon;
```
**WHAT**: Stores the application icon
**WHY**: Used in OnPaint to draw icon when minimized

### YourAppDlg.cpp - Dialog Implementation

```cpp
// YourAppDlg.cpp
#include "pch.h"
#include "framework.h"
#include "YourApp.h"
#include "YourAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CYourAppDlg dialog

CYourAppDlg::CYourAppDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_YOURAPP_DIALOG, pParent)  // Pass dialog ID to base
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CYourAppDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    // TODO: Add DDX calls here
}

BEGIN_MESSAGE_MAP(CYourAppDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

// CYourAppDlg message handlers

BOOL CYourAppDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();  // ALWAYS call base first

    // Set the icon
    SetIcon(m_hIcon, TRUE);   // Big icon
    SetIcon(m_hIcon, FALSE);  // Small icon

    // TODO: Add extra initialization here

    return TRUE;  // Return TRUE unless you set focus to a control
}

void CYourAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialogEx::OnSysCommand(nID, lParam);
}

void CYourAppDlg::OnPaint()
{
    if (IsIconic())  // Dialog is minimized
    {
        CPaintDC dc(this);

        SendMessage(WM_ICONERASEBKGND,
                   reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CYourAppDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}
```

**WHY SO MUCH CODE?**

All this generated code handles standard dialog behavior:
- Setting the icon
- Drawing when minimized
- Handling system commands

**YOU ADD YOUR CODE** in OnInitDialog and in button handlers.

---

## 8. Understanding Auto-Generated Comments

### Comment Markers

```cpp
// {{AFX_MSG(CYourAppDlg)
    afx_msg void OnBnClickedButton1();
// }}AFX_MSG
```

**WHAT THESE MEAN**:
- `{{AFX_MSG` - Start of auto-generated region
- `}}AFX_MSG` - End of auto-generated region

**RULES**:
- ✗ DON'T delete these markers
- ✗ DON'T add code between the markers manually
- ✓ DO let Class Wizard manage these sections
- ✓ DO add code OUTSIDE these markers

### Other Generated Comments

```cpp
//{{AFX_VIRTUAL(CYourAppDlg)
    virtual void DoDataExchange(CDataExchange* pDX);
//}}AFX_VIRTUAL
```
**WHAT**: Virtual function overrides

```cpp
//{{AFX_DATA(CYourAppDlg)
    enum { IDD = IDD_YOURAPP_DIALOG };
    CString m_strName;
//}}AFX_DATA
```
**WHAT**: DDX variables

```cpp
//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
```
**WHAT**: In Resource.h, indicates auto-generated content

---

## 9. What You Can Safely Edit

### Safe to Edit

✓ **Add member variables** (outside auto-generated regions):
```cpp
class CYourAppDlg : public CDialogEx
{
    // ... generated code ...

public:  // ← Add here
    CString m_strCustomData;
    int m_nCounter;

protected:  // ← Or here
    void MyHelperFunction();
};
```

✓ **Add helper functions**:
```cpp
// In .h
protected:
    void CalculateResults();
    bool ValidateInput();

// In .cpp
void CYourAppDlg::CalculateResults()
{
    // Your code
}
```

✓ **Modify OnInitDialog**:
```cpp
BOOL CYourAppDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();  // Keep this

    // Add your initialization here ← SAFE
    SetDlgItemText(IDC_EDIT1, _T("Initial value"));

    return TRUE;
}
```

✓ **Add message handlers** (via Class Wizard)

✓ **Modify DoDataExchange**:
```cpp
void CYourAppDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);  // Keep this

    // Add DDX calls here ← SAFE
    DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
}
```

### Never Edit

✗ **pch.cpp** - Only triggers precompilation
✗ **Auto-generated comment regions** - Use Class Wizard
✗ **Resource.h manually** - Use Resource Editor
✗ **Message map macros** - Use Class Wizard

---

## 10. What You Should Never Edit

### Critical: Don't Break These

```cpp
// DON'T remove or modify
DECLARE_MESSAGE_MAP()

// DON'T remove or modify
BEGIN_MESSAGE_MAP(CYourAppDlg, CDialogEx)
    // Don't edit manually, use Class Wizard
END_MESSAGE_MAP()

// DON'T remove
CDialogEx::DoDataExchange(pDX);  // Base class call

// DON'T remove
CDialogEx::OnInitDialog();  // Base class call
```

### Why These Are Critical

```cpp
// If you remove DECLARE_MESSAGE_MAP():
class CYourAppDlg : public CDialogEx
{
    // DECLARE_MESSAGE_MAP()  ← REMOVED - BIG MISTAKE
};

// Compiler error:
// error C2039: 'GetThisMessageMap': is not a member of 'CYourAppDlg'
// Your handlers won't work!
```

```cpp
// If you forget base class call:
BOOL CYourAppDlg::OnInitDialog()
{
    // CDialogEx::OnInitDialog();  ← FORGOT THIS

    SetDlgItemText(IDC_EDIT1, _T("Hello"));  // CRASH! Controls not created
    return TRUE;
}
```

---

## 11. Customizing Wizard Settings

### After Project Creation

**You can change**:
- Application title (change in Resource Editor)
- Icon (replace .ico file)
- Dialog size (Resource Editor)
- Unicode/MBCS (Project Properties - rarely needed)

**You CANNOT easily change**:
- Application type (SDI ↔ MDI ↔ Dialog)
- Class names (requires manual refactoring)
- Base class (CDialog ↔ CDialogEx)

**TO CHANGE TITLE**:
```cpp
// Method 1: Resource Editor
Resource View → Dialog → IDD_YOURAPP_DIALOG
Properties → Caption: "New Title"

// Method 2: Code (dynamic)
BOOL CYourAppDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetWindowText(_T("New Title"));

    return TRUE;
}
```

**TO CHANGE ICON**:
```
1. Create/obtain a .ico file (32x32 and 16x16 sizes)
2. Resource View → Icon → IDR_MAINFRAME → Import
3. Replace existing icon
4. Save
```

---

## 12. Real-World Project Walkthrough

### Scenario: Simple Calculator

**Wizard Settings**:
```
Application type:    Dialog based
Project name:        SimpleCalculator
Dialog title:        "Simple Calculator"
Use Unicode:         YES
About box:           NO
```

**Generated Files**:
```
SimpleCalculator/
├── SimpleCalculator.h/.cpp      (Application class)
├── SimpleCalculatorDlg.h/.cpp   (Main dialog)
├── Resource.h
├── SimpleCalculator.rc
├── pch.h/pch.cpp
├── framework.h
└── targetver.h
```

**Step 1: Add Controls** (Resource Editor)
```
Edit control:   IDC_EDIT_NUM1
Edit control:   IDC_EDIT_NUM2
Button:         IDC_BUTTON_ADD
Static text:    IDC_STATIC_RESULT
```

**Step 2: Add Handler** (Class Wizard)
```
Double-click IDC_BUTTON_ADD
→ Creates OnBnClickedButtonAdd()
```

**Step 3: Add Member Variables** (SimpleCalculatorDlg.h)
```cpp
class CSimpleCalculatorDlg : public CDialogEx
{
    // ... generated code ...

protected:  // Add here
    CString m_strResult;
};
```

**Step 4: Implement Handler** (SimpleCalculatorDlg.cpp)
```cpp
void CSimpleCalculatorDlg::OnBnClickedButtonAdd()
{
    CString str1, str2;
    GetDlgItemText(IDC_EDIT_NUM1, str1);
    GetDlgItemText(IDC_EDIT_NUM2, str2);

    double num1 = _tstof(str1);
    double num2 = _tstof(str2);
    double result = num1 + num2;

    m_strResult.Format(_T("Result: %.2f"), result);
    SetDlgItemText(IDC_STATIC_RESULT, m_strResult);
}
```

**Step 5: Build and Test**
```
F7 - Build
F5 - Run
```

---

## Summary

### Key Takeaways

1. **MFC Wizard** generates 8-20 files depending on application type
2. **Precompiled headers** (pch.h) speed up compilation
3. **framework.h** contains MFC includes
4. **Resource.h** auto-generated - use Resource Editor
5. **YourApp.cpp** - InitInstance() is application entry point
6. **YourAppDlg.cpp** - OnInitDialog() is dialog initialization
7. **Auto-generated comments** (`{{AFX_MSG}}`) - don't edit manually
8. **Use Class Wizard** for adding handlers safely

### File Edit Frequency

```
FREQUENTLY:
- YourAppDlg.cpp      (your dialog code)
- YourAppDlg.h        (member variables, handlers)
- YourApp.rc          (via Resource Editor)

OCCASIONALLY:
- YourApp.cpp         (application initialization)
- framework.h         (adding includes)

RARELY:
- Resource.h          (via Resource Editor)
- pch.h               (adding common includes)

NEVER:
- pch.cpp
- ReadMe.txt
```

### Best Practices

✓ Always use Class Wizard for adding handlers
✓ Always include pch.h first in .cpp files
✓ Always call base class in OnInitDialog and DoDataExchange
✓ Add member variables outside auto-generated regions
✓ Use Resource Editor for visual changes

✗ Don't edit Resource.h manually
✗ Don't remove base class calls
✗ Don't edit between {{AFX_xxx}} markers
✗ Don't add code to pch.cpp

### Next Lesson

In **Lesson 06: Dialog-Based Applications Deep Dive**, you'll learn:
- Modal vs modeless dialogs
- Dialog lifecycle
- Passing data between dialogs
- Modifying dialog properties
- Custom dialog templates

---

**Next Lesson**: [Lesson 06: Dialog-Based Applications Deep Dive](Lesson-06-Dialog-Based-Applications-Deep-Dive.md)
