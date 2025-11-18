# Lesson 01: Introduction to MFC and Windows Programming

**Duration**: 8 hours
**Difficulty**: Beginner to Intermediate

## Table of Contents
1. What is MFC and Why Learn It?
2. Windows Programming Fundamentals
3. MFC vs Win32 API vs Modern Frameworks
4. Understanding the MFC "Spaghetti Code" Problem
5. MFC Architecture Overview
6. Setting Up Your Development Environment
7. Key MFC Concepts - WHERE to Look
8. Understanding Visual Studio's Role
9. Your First Look at MFC Code
10. Common Pitfalls and How to Avoid Them

---

## 1. What is MFC and Why Learn It?

### Understanding MFC

**MFC (Microsoft Foundation Classes)** is a C++ library that wraps the Windows API in an object-oriented framework. Created by Microsoft in the early 1990s, MFC has been the backbone of thousands of enterprise Windows applications.

**Why MFC Still Matters in 2025:**

1. **Legacy Applications**: Millions of lines of MFC code run in banking, manufacturing, healthcare, and government systems
2. **Maintenance Jobs**: Companies need developers who can maintain and extend existing MFC applications
3. **Understanding Windows**: MFC teaches you how Windows really works under the hood
4. **Bridge Technology**: Understanding MFC helps you transition between Win32 API and modern frameworks
5. **Real-World Skills**: Many companies still actively develop new features in MFC applications

### The Reality of MFC Code

**MFC code is complex** - and this course won't sugarcoat it. Here's what you'll face:

```cpp
// A simple button click in MFC can involve:
// 1. Resource files (.rc)
// 2. Resource.h with IDs
// 3. Header file with class declaration
// 4. Message map macros
// 5. Implementation file with handler
// 6. Generated code mixed with your code

// It looks like this:
class CMyDialog : public CDialogEx
{
    DECLARE_MESSAGE_MAP()  // Macro - where is this defined?
public:
    afx_msg void OnBnClickedButton1();  // What does afx_msg mean?
};

// In the .cpp file:
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)  // More macros!
    ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog::OnBnClickedButton1)
END_MESSAGE_MAP()

void CMyDialog::OnBnClickedButton1()
{
    // Your code here - but how did we get here?
}
```

**This course teaches you WHERE to look, WHAT matters, and HOW it all connects.**

---

## 2. Windows Programming Fundamentals

### The Windows Message Loop

Before understanding MFC, you must understand how Windows applications work. Every Windows program is **event-driven**:

```cpp
// This is what happens BEHIND MFC (Win32 API level)
// MFC hides this, but you need to understand it!

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    // 1. Register a window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;  // Your message handler
    wc.hInstance = hInstance;
    wc.lpszClassName = "MyWindowClass";
    RegisterClass(&wc);

    // 2. Create a window
    HWND hwnd = CreateWindow("MyWindowClass", "My Window",
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             500, 400,
                             NULL, NULL, hInstance, NULL);

    // 3. Show the window
    ShowWindow(hwnd, nCmdShow);

    // 4. THE MESSAGE LOOP - This is crucial!
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);  // Sends to WindowProc
    }

    return msg.wParam;
}

// 5. The Window Procedure - handles ALL messages
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg,
                            WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_LBUTTONDOWN:
        MessageBox(hwnd, "You clicked!", "Info", MB_OK);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
```

### What MFC Does

MFC **wraps** this complexity:

```cpp
// MFC hides the message loop and window creation
// You just do this:

class CMyApp : public CWinApp
{
public:
    virtual BOOL InitInstance();
};

class CMyDialog : public CDialogEx
{
public:
    CMyDialog(CWnd* pParent = nullptr);

protected:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    DECLARE_MESSAGE_MAP()
};

// MFC handles the message loop automatically!
// But WHERE is it? (Answer: in CWinApp::Run)
```

**Key Concept**: MFC doesn't eliminate complexity - it **reorganizes** it. You need to know WHERE to find things.

---

## 3. MFC vs Win32 API vs Modern Frameworks

### The Evolution of Windows Development

```
1985: Win16 API (Windows 1.0)
      Pure C, handles everywhere
      ↓
1992: MFC 1.0
      C++ wrapper around Win32 API
      ↓
1995: Win32 API mature (Windows 95)
      MFC 4.0 becomes standard
      ↓
2002: .NET Framework / WinForms
      C# takes over new development
      ↓
2012: WPF (XAML-based)
      Modern UI paradigm
      ↓
2020: WinUI 3 / .NET MAUI
      Cross-platform, modern C#
      ↓
2025: MFC still alive in enterprise!
      Legacy code needs maintenance
```

### Comparison Table

| Feature | Win32 API | MFC | WinForms (.NET) | WPF |
|---------|-----------|-----|-----------------|-----|
| Language | C | C++ | C# | C# |
| Complexity | Very High | High | Medium | Medium-High |
| Performance | Excellent | Excellent | Good | Good |
| UI Designer | Resource Editor | Resource Editor | Designer | XAML Designer |
| Learning Curve | Steep | Steep | Gentle | Medium |
| Code Volume | Very Large | Large | Small | Medium |
| Still Used? | Yes (drivers) | Yes (enterprise) | Yes | Yes |

### When to Choose MFC

**Choose MFC when:**
- Maintaining existing MFC applications (most common)
- Need maximum performance with native Windows controls
- Working with legacy C++ codebases
- Building system-level Windows applications
- Company standards require it

**Avoid MFC for:**
- New greenfield projects (use WPF, WinUI 3, or cross-platform)
- Web-based applications
- Mobile applications
- Cross-platform requirements

---

## 4. Understanding the MFC "Spaghetti Code" Problem

### Why MFC Code Feels Like Spaghetti

MFC code is notoriously difficult to navigate because:

#### Problem 1: Code Split Across Multiple Files

A single button click involves **at least 5 files**:

```
MyApp Project
│
├── Resource.h                    // Defines: #define IDC_BUTTON1 1001
├── MyApp.rc                      // Visual layout in text format
├── MyDialog.h                    // Class declaration
├── MyDialog.cpp                  // Implementation
└── ReadMe.txt                    // Auto-generated, often ignored
```

#### Problem 2: Generated Code Mixed with Your Code

```cpp
// MyDialog.h - Which parts are generated? Which can you edit?

#pragma once

// AFX_INCLUDES - Generated by wizard, DON'T EDIT THIS SECTION
// {{AFX_INCLUDES()
// }}AFX_INCLUDES

class CMyDialog : public CDialogEx
{
// Construction - You can edit this
public:
    CMyDialog(CWnd* pParent = nullptr);

// Dialog Data - Generated, but you add variables here
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MY_DIALOG };  // Generated
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV

// Implementation - You edit this
protected:
    DECLARE_MESSAGE_MAP()  // Macro - expands to code

// MORE GENERATED CODE
// {{AFX_MSG(CMyDialog)
    afx_msg void OnBnClickedButton1();  // You add this via wizard
// }}AFX_MSG
};
```

**WHERE TO LOOK**: Comments like `{{AFX_MSG(CMyDialog)}}` indicate wizard-generated regions. Be careful editing these!

#### Problem 3: Macros Hiding Real Code

```cpp
// What you see:
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog::OnBnClickedButton1)
END_MESSAGE_MAP()

// What it ACTUALLY expands to (simplified):
const AFX_MSGMAP* CMyDialog::GetMessageMap() const
{
    return GetThisMessageMap();
}

const AFX_MSGMAP* CMyDialog::GetThisMessageMap()
{
    static const AFX_MSGMAP_ENTRY _messageEntries[] =
    {
        { WM_COMMAND, BN_CLICKED, IDC_BUTTON1,
          (AFX_PMSG)(void (CMyDialog::*)())&CMyDialog::OnBnClickedButton1 },
        { 0, 0, 0, 0 }
    };

    static const AFX_MSGMAP messageMap =
    {
        &CDialogEx::GetThisMessageMap,
        &_messageEntries[0]
    };

    return &messageMap;
}
```

**Don't panic!** You don't need to understand macro expansion to use MFC, but knowing it exists helps demystify the "magic."

#### Problem 4: Hungarian Notation Everywhere

```cpp
// MFC uses Hungarian notation extensively
HWND   hwndMain;      // h = handle, wnd = window
LPCTSTR lpszText;     // lp = long pointer, sz = string zero-terminated
UINT   nFlags;        // n = number
CString strName;      // str = string (object)
int    m_nCount;      // m_ = member variable
BOOL   bSuccess;      // b = boolean
DWORD  dwValue;       // dw = double word (32-bit)
```

**WHERE TO LOOK**: The prefix tells you the type. `m_` means it's a member variable of your class.

---

## 5. MFC Architecture Overview

### The Big Picture

```
Your Application
    ↓
CWinApp (Application Object)
    ↓
CFrameWnd or CDialog (Main Window)
    ↓
CWnd (Base Window Class)
    ↓
Windows API (USER32.DLL, GDI32.DLL, etc.)
    ↓
Windows Kernel
```

### Key MFC Classes - WHERE They Fit

```cpp
// Application Level
class CMyApp : public CWinApp
{
    // ONE instance per application
    // Manages application lifecycle
    // InitInstance() - start here!
};

// Window Level
class CMainFrame : public CFrameWnd
{
    // For SDI/MDI applications
    // The main application window
};

class CMyDialog : public CDialogEx
{
    // For dialog-based applications
    // Simpler than frame windows
};

// Control Level
class CButton : public CWnd
{
    // Wrapper for Windows button control
};

class CEdit : public CWnd
{
    // Wrapper for edit control
};

// Utility Classes
class CString
{
    // String handling (better than char*)
};

class CArray<T>
{
    // Dynamic array template
};
```

### The Three Essential Files

Every MFC project has these core files:

#### 1. TheApp.h and TheApp.cpp
```cpp
// TheApp.h - APPLICATION OBJECT
class CMyApp : public CWinApp
{
public:
    virtual BOOL InitInstance();  // ← START HERE when program launches
};

extern CMyApp theApp;  // The ONE global application object
```

**WHERE TO LOOK**: `InitInstance()` is where your application starts. This is your `main()` equivalent.

#### 2. MainDlg.h and MainDlg.cpp
```cpp
// MainDlg.h - MAIN DIALOG
class CMainDlg : public CDialogEx
{
public:
    CMainDlg(CWnd* pParent = nullptr);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV
    virtual BOOL OnInitDialog();  // ← Dialog initialization

    DECLARE_MESSAGE_MAP()
};
```

**WHERE TO LOOK**: `OnInitDialog()` is where you initialize your dialog (set values, configure controls).

#### 3. Resource.h and YourApp.rc
```cpp
// Resource.h - RESOURCE IDs
#define IDD_MY_DIALOG          101
#define IDC_BUTTON1            1001
#define IDC_EDIT1              1002

// These connect your code to visual elements!
```

**WHERE TO LOOK**: Every control has an ID. Use these IDs in your code to reference controls.

---

## 6. Setting Up Your Development Environment

### Required Software

1. **Visual Studio 2019 or 2022** (Community Edition works)
   - Download from: visualstudio.microsoft.com
   - During installation, select: "Desktop development with C++"
   - Under Individual Components, ensure "MFC and ATL support" is checked

2. **Windows 10/11 SDK**
   - Usually included with Visual Studio
   - Verify in: Tools → Get Tools and Features

### Verifying MFC Installation

```cpp
// Quick test - Create new project
// File → New → Project
// Search for "MFC"
// You should see:
// - MFC Application
// - MFC ActiveX Control
// - MFC DLL

// If you DON'T see these, MFC isn't installed!
```

### Visual Studio Configuration for MFC

**Important Settings**:

1. **Show all files**: Solution Explorer → Show All Files (icon)
2. **Resource View**: View → Other Windows → Resource View (Ctrl+Shift+E)
3. **Class View**: View → Class View (Ctrl+Shift+C)
4. **Properties Window**: View → Properties Window (F4)

**WHERE TO LOOK in Visual Studio**:

```
Solution Explorer        - File structure, your code files
├── Header Files        - .h files (class declarations)
├── Source Files        - .cpp files (implementations)
├── Resource Files      - .rc, .ico, .bmp (visual resources)

Resource View           - Visual editor for dialogs, menus
├── Dialog             - Visual dialog designer
├── Menu               - Menu editor
├── Accelerator        - Keyboard shortcuts

Class View              - Class hierarchy
├── Your classes       - Quick navigation
├── Member functions   - Jump to implementations
```

---

## 7. Key MFC Concepts - WHERE to Look

### Concept 1: Message Maps

**WHAT**: Message maps connect Windows messages to your handler functions.

**WHERE TO LOOK**:
```cpp
// In .h file:
class CMyDialog : public CDialogEx
{
    DECLARE_MESSAGE_MAP()  // ← This macro declares the map
};

// In .cpp file:
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog::OnBnClickedButton1)
    ON_WM_PAINT()
    ON_WM_CLOSE()
END_MESSAGE_MAP()

// ↑ This section maps Windows messages to YOUR functions
```

**HOW TO FIND**: Search for `BEGIN_MESSAGE_MAP` in your .cpp file.

### Concept 2: Dialog Data Exchange (DDX)

**WHAT**: Automatic transfer between controls and variables.

**WHERE TO LOOK**:
```cpp
// In .h file:
class CMyDialog : public CDialogEx
{
protected:
    CString m_strName;      // ← Variable connected to edit control
    int     m_nAge;         // ← Variable connected to edit control
    BOOL    m_bAccept;      // ← Variable connected to checkbox
};

// In .cpp file:
void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    // ↓ These connect variables to control IDs
    DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
    DDX_Text(pDX, IDC_EDIT_AGE, m_nAge);
    DDX_Check(pDX, IDC_CHECK_ACCEPT, m_bAccept);
}
```

**HOW TO FIND**: Look for `DoDataExchange()` function.

### Concept 3: Resource IDs

**WHAT**: Numeric constants that identify controls and dialogs.

**WHERE TO LOOK**:
```cpp
// Resource.h file:
#define IDD_MAIN_DIALOG        100  // Dialog ID
#define IDC_BUTTON_OK          1001 // Button ID
#define IDC_EDIT_NAME          1002 // Edit control ID

// Used in code:
CWnd* pEdit = GetDlgItem(IDC_EDIT_NAME);
```

**HOW TO FIND**: Open Resource.h file in your project.

---

## 8. Understanding Visual Studio's Role

### The MFC Wizard

Visual Studio's MFC Application Wizard generates a LOT of code. Understanding what's generated vs. what you add is crucial.

**Wizard-Generated Files** (DON'T delete these):

```
MyApp/
├── MyApp.cpp           // Application class implementation
├── MyApp.h             // Application class declaration
├── MyAppDlg.cpp        // Main dialog implementation
├── MyAppDlg.h          // Main dialog declaration
├── Resource.h          // Resource ID definitions
├── MyApp.rc            // Resource script (dialogs, menus, etc.)
├── stdafx.h            // Precompiled header (older VS)
├── pch.h               // Precompiled header (newer VS)
├── framework.h         // Common includes
└── targetver.h         // SDK version targeting
```

### The Resource Editor

**WHERE TO LOOK**: Double-click the .rc file in Solution Explorer, or use Resource View.

**WHAT YOU SEE**:
- Dialog Editor: Visual designer for dialogs
- Menu Editor: Create menus
- Toolbar Editor: Design toolbars
- Icon Editor: Create/edit icons
- String Table: Localized strings

**HOW IT CONNECTS TO CODE**:
1. You design a button in Resource Editor
2. Visual Studio assigns it an ID (e.g., IDC_BUTTON1)
3. You double-click the button
4. Visual Studio generates a handler function
5. You write code in that handler

### Class Wizard

**WHERE**: Right-click your dialog class → Class Wizard (or Ctrl+Shift+X)

**WHAT IT DOES**:
- Adds message handlers
- Adds member variables
- Connects controls to variables (DDX)
- Adds virtual function overrides

**IMPORTANT**: Always use Class Wizard for adding handlers - don't manually edit message maps unless you know what you're doing!

---

## 9. Your First Look at MFC Code

Let's examine a minimal MFC dialog application:

### The Application Class

```cpp
// MyApp.h - WHERE the application is defined
#pragma once

#ifndef __AFXWIN_H__
    #error "include 'pch.h' before including this file"
#endif

#include "resource.h"  // Resource symbols

class CMyApp : public CWinApp
{
public:
    CMyApp();  // Constructor

// Overrides
public:
    virtual BOOL InitInstance();  // ← MOST IMPORTANT function!

    DECLARE_MESSAGE_MAP()
};

extern CMyApp theApp;  // ← The ONE global app object
```

```cpp
// MyApp.cpp - WHERE the application starts
#include "pch.h"
#include "framework.h"
#include "MyApp.h"
#include "MyAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The ONE and ONLY application object
CMyApp theApp;

// Constructor
CMyApp::CMyApp()
{
    // TODO: add construction code here
    // All significant initialization happens in InitInstance
}

// Message map - even the app object has one
BEGIN_MESSAGE_MAP(CMyApp, CWinApp)
END_MESSAGE_MAP()

// THE KEY FUNCTION - This is where execution starts
BOOL CMyApp::InitInstance()
{
    // Standard MFC initialization
    // If you want standard Windows features, don't remove these
    CWinApp::InitInstance();

    // Create and show the main dialog
    CMyAppDlg dlg;           // ← Create dialog object
    m_pMainWnd = &dlg;       // ← Set as main window
    INT_PTR nResponse = dlg.DoModal();  // ← Show dialog

    // DoModal() blocks here until dialog closes

    if (nResponse == IDOK)
    {
        // User clicked OK
    }
    else if (nResponse == IDCANCEL)
    {
        // User clicked Cancel or closed dialog
    }

    // Return FALSE to exit application
    // (Dialog-based apps exit when main dialog closes)
    return FALSE;
}
```

**WHERE TO LOOK**:
- `theApp` is the global application object
- `InitInstance()` is where your program starts (like `main()`)
- `DoModal()` shows the dialog and waits for user input

### The Dialog Class

```cpp
// MyAppDlg.h - The main dialog
#pragma once

class CMyAppDlg : public CDialogEx
{
// Construction
public:
    CMyAppDlg(CWnd* pParent = nullptr);  // Constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MYAPP_DIALOG };  // ← Links to resource ID
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV

// Implementation
protected:
    HICON m_hIcon;  // Member variable for icon

    // Generated message map functions
    virtual BOOL OnInitDialog();  // ← Dialog initialization
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
};
```

```cpp
// MyAppDlg.cpp - Dialog implementation
#include "pch.h"
#include "framework.h"
#include "MyApp.h"
#include "MyAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Constructor
CMyAppDlg::CMyAppDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MYAPP_DIALOG, pParent)  // ← Pass dialog ID to base
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

// DDX - Data exchange between controls and variables
void CMyAppDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    // ← Add DDX_xxx calls here to connect variables to controls
}

// Message Map - connects Windows messages to your functions
BEGIN_MESSAGE_MAP(CMyAppDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

// OnInitDialog - runs ONCE when dialog first appears
BOOL CMyAppDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();  // ← Always call base class first!

    // Set the icon for this dialog
    SetIcon(m_hIcon, TRUE);   // Set big icon
    SetIcon(m_hIcon, FALSE);  // Set small icon

    // TODO: Add extra initialization here
    // This is where YOU set initial values, configure controls, etc.

    return TRUE;  // Return TRUE unless you set focus to a control
}

// OnPaint - handles painting the dialog
void CMyAppDlg::OnPaint()
{
    if (IsIconic())  // If minimized, draw the icon
    {
        CPaintDC dc(this);  // Device context for painting

        SendMessage(WM_ICONERASEBKGND,
                   reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon
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
        CDialogEx::OnPaint();  // Let base class handle it
    }
}

// OnQueryDragIcon - returns cursor to display while dragging
HCURSOR CMyAppDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}
```

**WHERE TO LOOK**:
- `IDD_MYAPP_DIALOG` connects this class to a visual dialog resource
- `OnInitDialog()` is where YOU initialize (like a constructor for the UI)
- `DoDataExchange()` is where control↔variable binding happens
- Message map connects Windows messages to handler functions

---

## 10. Common Pitfalls and How to Avoid Them

### Pitfall 1: Editing Generated Code Regions

**WRONG**:
```cpp
// DON'T manually edit between {{AFX_xxx markers!
// {{AFX_MSG(CMyDialog)
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();  // ← Added manually
// }}AFX_MSG
```

**RIGHT**:
```cpp
// Use Class Wizard to add handlers
// Or add OUTSIDE the markers:
// {{AFX_MSG(CMyDialog)
    afx_msg void OnBnClickedButton1();
// }}AFX_MSG
    // Put your custom handlers here:
    afx_msg void OnBnClickedButton2();
```

### Pitfall 2: Forgetting to Call Base Class

**WRONG**:
```cpp
BOOL CMyDialog::OnInitDialog()
{
    // CDialogEx::OnInitDialog();  ← FORGOT THIS!

    SetDlgItemText(IDC_EDIT1, _T("Hello"));
    return TRUE;
}
```

**RIGHT**:
```cpp
BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();  // ← ALWAYS call base class first!

    SetDlgItemText(IDC_EDIT1, _T("Hello"));
    return TRUE;
}
```

### Pitfall 3: Wrong String Type

**WRONG**:
```cpp
CString str = "Hello";  // Might not compile in Unicode builds
SetWindowText("Text");  // Might not compile
```

**RIGHT**:
```cpp
CString str = _T("Hello");        // Unicode-safe
SetWindowText(_T("Text"));        // Unicode-safe
// Or:
CString str = L"Hello";           // Explicitly Unicode
SetWindowTextW(L"Text");          // Explicitly Unicode API
```

### Pitfall 4: Memory Leaks with Dialogs

**WRONG**:
```cpp
void CMyDialog::OnButton()
{
    CDialog* pDlg = new CDialog();  // ← Memory leak!
    pDlg->DoModal();
    // Never deleted!
}
```

**RIGHT**:
```cpp
void CMyDialog::OnButton()
{
    CDialog dlg;        // Stack allocation
    dlg.DoModal();      // Automatically cleaned up
}

// OR if you must use heap:
void CMyDialog::OnButton()
{
    CDialog* pDlg = new CDialog();
    pDlg->DoModal();
    delete pDlg;        // ← Don't forget!
}
```

### Pitfall 5: Accessing Controls Before Dialog Creation

**WRONG**:
```cpp
CMyDialog::CMyDialog()
{
    // Controls don't exist yet!
    SetDlgItemText(IDC_EDIT1, _T("Hello"));  // ← CRASH!
}
```

**RIGHT**:
```cpp
BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // NOW controls exist
    SetDlgItemText(IDC_EDIT1, _T("Hello"));  // ← Works!
    return TRUE;
}
```

---

## Summary - Your MFC Roadmap

### What You Learned

1. **MFC is a wrapper** around the Windows API, making it easier but still complex
2. **Code is split across multiple files** - Resource.h, .rc, .h, .cpp
3. **Visual Studio generates a lot of code** - learn to distinguish generated from user code
4. **Message maps are key** - they connect Windows messages to your functions
5. **WHERE to look** - specific files and functions matter

### Key Locations Reference Card

```
WHERE TO START:
  theApp.InitInstance()           - Application starts here

WHERE TO INITIALIZE UI:
  CYourDialog::OnInitDialog()     - Set initial values here

WHERE TO HANDLE EVENTS:
  Message map in .cpp file        - ON_BN_CLICKED, etc.
  Handler functions               - OnBnClickedButton1(), etc.

WHERE TO FIND CONTROL IDs:
  Resource.h                      - #define IDC_BUTTON1 1001

WHERE TO DESIGN UI:
  Resource View → Dialog          - Visual designer

WHERE TO ADD HANDLERS:
  Class Wizard (Ctrl+Shift+X)     - Adds handlers correctly

WHERE TO CONNECT VARIABLES:
  DoDataExchange()                - DDX_Text, DDX_Check, etc.
```

### Next Steps

In **Lesson 02**, you'll create your first MFC dialog-based application step-by-step, seeing exactly WHERE each piece of code goes and HOW it all connects.

You'll learn:
- Creating a project with MFC Application Wizard
- Understanding every file that gets generated
- Adding a button and handling its click
- Displaying and reading text from edit controls
- Using the debugger to trace message flow

### Essential Questions to Review

1. What is the difference between Win32 API and MFC?
2. Where does an MFC application start executing?
3. What are message maps and where are they declared?
4. What does the DoDataExchange function do?
5. Why should you use _T() or L"" for string literals?
6. Where do you initialize dialog controls?
7. What's the purpose of Resource.h?
8. How do you add an event handler safely?

---

**Remember**: MFC is complex, but it's **organized complexity**. Once you know WHERE to look, WHAT to edit, and HOW things connect, it becomes manageable. This course will guide you through every step.

**Time to Practice**: Before moving to Lesson 02, install Visual Studio with MFC support and explore the IDE. Familiarize yourself with Solution Explorer, Resource View, and Class View.

---

**Next Lesson**: [Lesson 02: Your First MFC Application (Dialog-based)](Lesson-02-First-MFC-Application-Dialog.md)
