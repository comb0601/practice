# Lesson 03: Understanding MFC Architecture and Class Hierarchy

**Duration**: 8-10 hours
**Difficulty**: Intermediate

## Table of Contents
1. The Big Picture: MFC Architecture Overview
2. CObject - The Root of All MFC Classes
3. The CWnd Family - Window Classes
4. The CWinApp Class - Your Application
5. CDialog and CDialogEx - Dialog Classes
6. Understanding Inheritance in MFC
7. Virtual Functions and Overriding
8. When to Call Base Class Functions
9. The Document/View Architecture (Overview)
10. Class Hierarchy Quick Reference
11. Practical Examples
12. Common Pitfalls

---

## 1. The Big Picture: MFC Architecture Overview

### The MFC Class Hierarchy Pyramid

```
                    CObject
                      │
         ┌────────────┼────────────┐
         │            │            │
     CCmdTarget    CFile      CException
         │            │
     CWinThread   CArchive
         │
     CWinApp
         │
    Your App Class
```

```
                    CObject
                      │
                 CCmdTarget
                      │
                    CWnd ────────────────────────┐
                      │                          │
         ┌────────────┼────────────┐            │
         │            │            │            │
    CFrameWnd    CDialog      CView       CControlBar
         │            │            │            │
 CMainFrame    CDialogEx    CFormView    CToolBar
         │            │
  Your Frame   Your Dialog
```

**WHERE TO LOOK**: Every MFC class you use inherits from these base classes.

### The Three Pillars of MFC Architecture

#### Pillar 1: Application Object (CWinApp)

```cpp
// There is ONE and ONLY ONE application object
class CMyApp : public CWinApp
{
public:
    virtual BOOL InitInstance();  // Where your app starts
};

CMyApp theApp;  // Global instance - automatically created before main()
```

**WHAT IT DOES**:
- Initializes the application
- Manages the message loop (you don't see it, but it's there)
- Cleans up when application exits

**WHERE IT LIVES**: YourAppName.h and YourAppName.cpp

#### Pillar 2: Window Objects (CWnd-derived)

```cpp
// Windows are objects
class CMainDlg : public CDialogEx  // CDialogEx derives from CWnd
{
    // This represents a window on screen
};
```

**WHAT IT DOES**:
- Wraps a Windows HWND (handle to a window)
- Processes Windows messages (clicks, paint, etc.)
- Manages child controls

**WHERE IT LIVES**: YourDialog.h and YourDialog.cpp

#### Pillar 3: Utility Objects

```cpp
CString str;           // String class
CRect rect;            // Rectangle
CPoint pt;             // Point (x, y)
CFile file;            // File operations
CArray<int> arr;       // Dynamic array
```

**WHAT THEY DO**: Provide useful functionality without being windows

---

## 2. CObject - The Root of All MFC Classes

### What is CObject?

`CObject` is the **base class** for most MFC classes. Think of it as the "mother of all MFC objects."

```cpp
// Simplified CObject definition
class CObject
{
public:
    virtual ~CObject();  // Virtual destructor

    // Runtime type information
    virtual CRuntimeClass* GetRuntimeClass() const;

    // Serialization support
    virtual void Serialize(CArchive& ar);

    // Diagnostics
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // ...more members
};
```

### Why CObject Matters

**Benefits of inheriting from CObject**:

1. **Runtime Type Information (RTTI)**
   ```cpp
   CObject* pObj = new CMyDialog();

   // Can query type at runtime
   if (pObj->IsKindOf(RUNTIME_CLASS(CDialog)))
   {
       CDialog* pDlg = (CDialog*)pObj;
       // Use as dialog
   }
   ```

2. **Serialization** (save/load objects)
   ```cpp
   class CMyClass : public CObject
   {
       DECLARE_SERIAL(CMyClass)

   public:
       virtual void Serialize(CArchive& ar);
   };
   ```

3. **Debugging Support**
   ```cpp
   #ifdef _DEBUG
   void CMyClass::AssertValid() const
   {
       CObject::AssertValid();
       ASSERT(m_nValue >= 0);  // Custom validation
   }
   #endif
   ```

### The DECLARE/IMPLEMENT Macros

MFC uses macros extensively for RTTI:

```cpp
// In .h file
class CMyClass : public CObject
{
    DECLARE_DYNAMIC(CMyClass)  // ← Declares RTTI support
    // Or: DECLARE_DYNCREATE
    // Or: DECLARE_SERIAL
};

// In .cpp file
IMPLEMENT_DYNAMIC(CMyClass, CObject)  // ← Implements RTTI support
```

**WHERE TO LOOK**: Every MFC class has these macros. Don't edit them manually - they're part of the framework.

---

## 3. The CWnd Family - Window Classes

### CWnd: The Mother of All Windows

Every visible element in MFC is a `CWnd` or derived from it.

```cpp
class CWnd : public CCmdTarget
{
protected:
    HWND m_hWnd;  // ← The actual Windows handle (WHERE Windows API connects)

public:
    // Window creation
    virtual BOOL Create(...);
    virtual BOOL CreateEx(...);

    // Window manipulation
    void ShowWindow(int nCmdShow);
    void SetWindowText(LPCTSTR lpszString);
    void MoveWindow(int x, int y, int cx, int cy);

    // Getting information
    void GetWindowText(CString& rString);
    CWnd* GetParent();
    CWnd* GetDlgItem(int nID);

    // Message handling
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

    // Hundreds more functions...
};
```

### The CWnd Hierarchy

```
CWnd
├── CFrameWnd          (Main windows in SDI/MDI apps)
│   ├── CMDIFrameWnd   (MDI parent window)
│   └── CMDIChildWnd   (MDI child window)
│
├── CView              (Document/View architecture)
│   ├── CScrollView
│   ├── CFormView      (Form-based view)
│   └── CEditView      (Text editor view)
│
├── CDialog            (Modal/modeless dialogs)
│   └── CDialogEx      (Extended dialog - this is what you usually use)
│       └── Your Dialog Classes
│
├── CControlBar        (Toolbars, status bars)
│   ├── CToolBar
│   └── CStatusBar
│
└── CStatic, CButton, CEdit, CListBox, etc. (Controls)
```

**WHERE YOUR CODE FITS**:
```cpp
class CMyDialog : public CDialogEx  // ← You inherit from CDialogEx
{
    // CDialogEx inherits from CDialog
    // CDialog inherits from CWnd
    // CWnd inherits from CCmdTarget
    // CCmdTarget inherits from CObject

    // So your dialog IS-A CObject, CCmdTarget, CWnd, CDialog, AND CDialogEx
};
```

### Understanding HWND vs CWnd

```cpp
// Win32 API level - just a handle (integer)
HWND hwnd = CreateWindow(...);  // Returns a handle
SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)"Hello");

// MFC level - object-oriented wrapper
CWnd* pWnd = new CWnd();
pWnd->Create(...);              // Creates the window
pWnd->SetWindowText(_T("Hello")); // Object-oriented

// Converting between them
HWND hwnd = pWnd->GetSafeHwnd();  // CWnd → HWND
CWnd* pWnd = CWnd::FromHandle(hwnd);  // HWND → CWnd*
```

**KEY CONCEPT**: Every `CWnd` wraps an `HWND`. The `m_hWnd` member holds the actual Windows handle.

---

## 4. The CWinApp Class - Your Application

### Application Class Structure

```cpp
// YourApp.h
class CMyApp : public CWinApp
{
public:
    CMyApp();  // Constructor

    // Overrides
    virtual BOOL InitInstance();        // ← MOST IMPORTANT
    virtual int ExitInstance();         // Cleanup before exit
    virtual BOOL OnIdle(LONG lCount);   // Called when idle

    DECLARE_MESSAGE_MAP()
};
```

### Understanding InitInstance

This is **WHERE your application starts**:

```cpp
BOOL CMyApp::InitInstance()
{
    // 1. Call base class first (ALWAYS!)
    CWinApp::InitInstance();

    // 2. Initialize COM, GDI+, etc. (if needed)
    // AfxOleInit();
    // AfxInitRichEdit2();

    // 3. Set registry key (for saving settings)
    SetRegistryKey(_T("MyCompany"));

    // 4. Create your main window
    CMyDialog dlg;
    m_pMainWnd = &dlg;  // ← Set main window pointer

    // 5. Show the window
    INT_PTR nResponse = dlg.DoModal();

    // 6. Return FALSE for dialog-based apps (exits after dialog closes)
    return FALSE;

    // For SDI/MDI apps, return TRUE to keep app running
}
```

**WHAT HAPPENS**:

```
Windows launches your .exe
    ↓
C Runtime calls WinMain (MFC provides this - you don't see it)
    ↓
MFC creates 'theApp' object (your CWinApp-derived class)
    ↓
MFC calls theApp.InitInstance()  ← YOU IMPLEMENT THIS
    ↓
Your code creates main window
    ↓
MFC runs message loop (in CWinApp::Run - you don't see it)
    ↓
Window receives messages (clicks, paint, etc.)
    ↓
Message loop continues until WM_QUIT
    ↓
MFC calls theApp.ExitInstance()
    ↓
Application terminates
```

### The Hidden Message Loop

You never see this code, but it's running:

```cpp
// Inside CWinApp::Run() - MFC code, not yours
int CWinApp::Run()
{
    // This is the message loop
    for (;;)
    {
        // Idle processing
        while (!::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE))
        {
            if (!OnIdle(m_nIdleCount++))
                break;
        }

        // Get the next message
        if (!PumpMessage())
            return ExitInstance();
    }
}
```

**WHERE IT RUNS**: Automatically after `InitInstance()` returns TRUE (for SDI/MDI) or after `DoModal()` (for dialog-based).

---

## 5. CDialog and CDialogEx - Dialog Classes

### CDialog vs CDialogEx

```cpp
// CDialog - Original MFC dialog class
class CDialog : public CWnd
{
public:
    virtual INT_PTR DoModal();        // Show as modal dialog
    virtual BOOL Create(...);         // Create modeless dialog

protected:
    virtual void DoDataExchange(CDataExchange* pDX);  // DDX
    virtual BOOL OnInitDialog();      // Initialization
    virtual void OnOK();              // OK button handler
    virtual void OnCancel();          // Cancel button handler
};

// CDialogEx - Extended dialog (Windows 7+, better theming)
class CDialogEx : public CDialog
{
    // Same interface, better visual styles
    // USE THIS in new applications
};
```

**WHEN TO USE WHICH**:
- **CDialogEx**: New applications (Windows 7+)
- **CDialog**: Legacy code or Windows XP support

### Modal vs Modeless Dialogs

```cpp
// MODAL - Blocks until user closes dialog
void CMyView::OnShowDialog()
{
    CMyDialog dlg;
    INT_PTR result = dlg.DoModal();  // ← Blocks here

    if (result == IDOK)
    {
        // User clicked OK
    }
    else
    {
        // User clicked Cancel or closed dialog
    }
    // dlg is automatically destroyed here (goes out of scope)
}

// MODELESS - Doesn't block, dialog stays open
class CParentDlg : public CDialogEx
{
    CMyDialog* m_pModelessDlg;  // ← Must be pointer/member variable

    void OnShowModeless()
    {
        if (m_pModelessDlg == nullptr)
        {
            m_pModelessDlg = new CMyDialog();
            m_pModelessDlg->Create(IDD_MYDIALOG, this);  // ← Create, don't DoModal
            m_pModelessDlg->ShowWindow(SW_SHOW);
        }
        else
        {
            m_pModelessDlg->SetFocus();  // Already open, bring to front
        }
    }

    ~CParentDlg()
    {
        if (m_pModelessDlg != nullptr)
        {
            m_pModelessDlg->DestroyWindow();
            delete m_pModelessDlg;
        }
    }
};
```

**KEY DIFFERENCE**:
- **Modal**: Use stack allocation, DoModal() blocks
- **Modeless**: Use heap allocation, Create() doesn't block, YOU manage lifetime

---

## 6. Understanding Inheritance in MFC

### Your Dialog's Full Inheritance Chain

```cpp
class CMyDialog : public CDialogEx
{
    // Your custom code
};
```

**What you actually get**:

```
CMyDialog           ← Your code (handlers, variables, logic)
    ↓ inherits
CDialogEx           ← Better theming (Windows 7+)
    ↓ inherits
CDialog             ← Dialog-specific (DoModal, DDX, OnOK)
    ↓ inherits
CWnd                ← Window functionality (SetWindowText, GetDlgItem)
    ↓ inherits
CCmdTarget          ← Message routing, command handling
    ↓ inherits
CObject             ← Runtime type info, serialization
```

**WHAT THIS MEANS**:

Your `CMyDialog` has access to ALL functions from ALL these classes:

```cpp
void CMyDialog::OnButtonClick()
{
    // From CWnd:
    SetWindowText(_T("New Title"));
    GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);

    // From CDialog:
    UpdateData(TRUE);
    EndDialog(IDOK);

    // From CCmdTarget:
    // Message routing (handled automatically)

    // From CObject:
    #ifdef _DEBUG
    AssertValid();
    #endif
}
```

### Method Resolution Order

When you call a function, C++ searches in this order:

```cpp
CMyDialog dlg;
dlg.OnInitDialog();  // Which OnInitDialog() gets called?
```

**Search order**:
1. ✓ **CMyDialog::OnInitDialog()** - If you defined it, this runs
2. CDialogEx::OnInitDialog() - Not defined (passes through)
3. CDialog::OnInitDialog() - Defined, but CMyDialog overrides it
4. CWnd::OnInitDialog() - Not applicable

**If you DIDN'T override OnInitDialog()**:
1. CMyDialog::OnInitDialog() - Not defined
2. CDialogEx::OnInitDialog() - Not defined
3. ✓ **CDialog::OnInitDialog()** - First definition found, this runs

---

## 7. Virtual Functions and Overriding

### Understanding Virtual Functions

```cpp
// Base class
class CDialog : public CWnd
{
public:
    virtual BOOL OnInitDialog()  // ← VIRTUAL keyword
    {
        // Base implementation
        return TRUE;
    }

    virtual void OnOK()
    {
        UpdateData(TRUE);
        EndDialog(IDOK);
    }
};

// Your derived class
class CMyDialog : public CDialogEx  // CDialogEx inherits from CDialog
{
public:
    virtual BOOL OnInitDialog()  // ← Override
    {
        // MUST call base class first (usually)
        CDialogEx::OnInitDialog();

        // Your initialization code
        SetDlgItemText(IDC_STATIC1, _T("Hello!"));

        return TRUE;
    }

    virtual void OnOK()  // ← Override
    {
        // Custom validation before closing
        CString str;
        GetDlgItemText(IDC_EDIT1, str);

        if (str.IsEmpty())
        {
            MessageBox(_T("Please enter a value!"));
            return;  // ← Don't call base class = dialog stays open
        }

        // Call base class to close dialog
        CDialogEx::OnOK();
    }
};
```

### Common Virtual Functions You'll Override

#### In CWinApp:

```cpp
class CMyApp : public CWinApp
{
public:
    // Application startup
    virtual BOOL InitInstance()
    {
        CWinApp::InitInstance();  // Call base
        // Your code
        return TRUE;
    }

    // Application shutdown
    virtual int ExitInstance()
    {
        // Your cleanup code
        return CWinApp::ExitInstance();  // Call base
    }

    // Idle processing (called when no messages)
    virtual BOOL OnIdle(LONG lCount)
    {
        // Update UI, background tasks
        return CWinApp::OnIdle(lCount);
    }
};
```

#### In CDialog/CDialogEx:

```cpp
class CMyDialog : public CDialogEx
{
public:
    // Dialog initialization
    virtual BOOL OnInitDialog()
    {
        CDialogEx::OnInitDialog();  // ALWAYS call base first
        // Initialize controls, set initial values
        return TRUE;
    }

    // Data exchange
    virtual void DoDataExchange(CDataExchange* pDX)
    {
        CDialogEx::DoDataExchange(pDX);  // ALWAYS call base
        // Add DDX_xxx calls
    }

    // OK button handling
    virtual void OnOK()
    {
        // Validate data
        if (ValidateData())
        {
            CDialogEx::OnOK();  // Close dialog with IDOK
        }
    }

    // Cancel button handling
    virtual void OnCancel()
    {
        // Confirm cancellation if needed
        if (AskToSave())
        {
            CDialogEx::OnCancel();  // Close dialog with IDCANCEL
        }
    }
};
```

#### In CWnd:

```cpp
class CMyControl : public CWnd
{
public:
    // Window procedure (rare to override directly)
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        if (message == WM_LBUTTONDOWN)
        {
            // Custom handling
        }
        return CWnd::WindowProc(message, wParam, lParam);
    }

    // Pre-create window (modify window class)
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs)
    {
        cs.style |= WS_BORDER;  // Add border style
        return CWnd::PreCreateWindow(cs);
    }
};
```

---

## 8. When to Call Base Class Functions

### The Golden Rules

#### Rule 1: ALWAYS Call Base in OnInitDialog (BEFORE your code)

```cpp
// CORRECT
BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();  // ← FIRST

    // Now controls exist, safe to access
    SetDlgItemText(IDC_EDIT1, _T("Hello"));

    return TRUE;
}

// WRONG - Controls don't exist yet
BOOL CMyDialog::OnInitDialog()
{
    SetDlgItemText(IDC_EDIT1, _T("Hello"));  // ← CRASH! Controls not created

    CDialogEx::OnInitDialog();  // Too late
    return TRUE;
}
```

**WHY**: Base class creates the window and controls. You can't access them before that.

#### Rule 2: ALWAYS Call Base in DoDataExchange (BEFORE your code)

```cpp
// CORRECT
void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);  // ← FIRST

    DDX_Text(pDX, IDC_EDIT1, m_strValue);
}

// WRONG
void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
    DDX_Text(pDX, IDC_EDIT1, m_strValue);  // Might work but incorrect order

    CDialogEx::DoDataExchange(pDX);  // Should be first
}
```

#### Rule 3: Call Base in OnOK/OnCancel ONLY If You Want Default Behavior

```cpp
// Standard behavior - close dialog
void CMyDialog::OnOK()
{
    CDialogEx::OnOK();  // ← Calls UpdateData(TRUE) and EndDialog(IDOK)
}

// Custom validation - close only if valid
void CMyDialog::OnOK()
{
    if (!ValidateInput())
    {
        MessageBox(_T("Invalid input!"));
        return;  // ← Don't call base = dialog stays open
    }

    CDialogEx::OnOK();  // ← Valid input, close dialog
}

// Prevent closing (rare)
void CMyDialog::OnCancel()
{
    // Don't call base class = user can't close dialog!
    // Usually bad UX, but sometimes needed
}
```

#### Rule 4: InitInstance - Call Base FIRST

```cpp
BOOL CMyApp::InitInstance()
{
    CWinApp::InitInstance();  // ← Initialize MFC

    // Now safe to use MFC features
    SetRegistryKey(_T("MyCompany"));

    return TRUE;
}
```

#### Rule 5: Message Handlers - Usually Don't Call Base

```cpp
void CMyDialog::OnBnClickedButton1()
{
    // Handle the button click
    MessageBox(_T("Button clicked!"));

    // NO base class call needed - there's nothing to call!
    // CDialogEx doesn't have OnBnClickedButton1()
}
```

### Quick Reference Table

| Function | Call Base? | When? | Why? |
|----------|-----------|-------|------|
| OnInitDialog | YES | FIRST | Creates window/controls |
| DoDataExchange | YES | FIRST | Sets up DDX infrastructure |
| OnOK | MAYBE | LAST | Only if you want to close dialog |
| OnCancel | MAYBE | LAST | Only if you want to close dialog |
| InitInstance | YES | FIRST | Initializes MFC |
| ExitInstance | YES | LAST | Cleanup MFC |
| Message handlers (OnBnClicked, etc.) | NO | N/A | No base implementation |
| WindowProc | YES | LAST | Default message processing |

---

## 9. The Document/View Architecture (Overview)

### What is Document/View?

For **SDI (Single Document Interface)** and **MDI (Multiple Document Interface)** applications, MFC uses Document/View architecture:

```
CWinApp (Application)
    ↓ creates
CDocTemplate (Template linking Document and View)
    ↓ creates
CFrameWnd (Main Window Frame)
    ↓ contains
CDocument (Data) ←→ CView (Display)
```

**Example**:
```cpp
// Document - holds the data
class CMyDoc : public CDocument
{
protected:
    CString m_strData;  // The actual data

public:
    virtual void Serialize(CArchive& ar);  // Save/Load
};

// View - displays the data
class CMyView : public CView
{
protected:
    virtual void OnDraw(CDC* pDC);  // Draw the document

    CMyDoc* GetDocument()
    {
        return (CMyDoc*)m_pDocument;  // Access the document
    }
};

// In OnDraw:
void CMyView::OnDraw(CDC* pDC)
{
    CMyDoc* pDoc = GetDocument();
    pDC->TextOut(10, 10, pDoc->m_strData);  // Display document data
}
```

**For Dialog-Based Apps**: You don't use Document/View - dialogs manage their own data.

**WHERE YOU'LL SEE IT**: When creating SDI/MDI applications with File→Open/Save menus.

---

## 10. Class Hierarchy Quick Reference

### Complete MFC Class Map

```
CObject
├── CCmdTarget (command routing)
│   ├── CWinThread (threads)
│   │   └── CWinApp (application)
│   │       └── Your App Class
│   │
│   └── CWnd (windows)
│       ├── CFrameWnd (frame windows)
│       │   ├── CMDIFrameWnd
│       │   └── CMDIChildWnd
│       │
│       ├── CView (views)
│       │   ├── CScrollView
│       │   ├── CFormView
│       │   └── CEditView
│       │
│       ├── CDialog (dialogs)
│       │   └── CDialogEx
│       │       └── Your Dialog Classes
│       │
│       ├── CControlBar (bars)
│       │   ├── CToolBar
│       │   ├── CStatusBar
│       │   └── CReBar
│       │
│       └── Controls
│           ├── CStatic
│           ├── CButton
│           ├── CEdit
│           ├── CListBox
│           ├── CComboBox
│           ├── CListCtrl
│           └── CTreeCtrl
│
├── CFile (file I/O)
│   ├── CStdioFile
│   └── CMemFile
│
├── CException (exceptions)
│   ├── CFileException
│   ├── CMemoryException
│   └── CUserException
│
└── Utility Classes
    ├── CString (strings)
    ├── CRect (rectangles)
    ├── CPoint (points)
    ├── CSize (sizes)
    └── CTime (date/time)
```

### Key Classes Reference

```cpp
// Application
CWinApp                 - Base application class

// Windows
CWnd                    - Base window class
CFrameWnd              - SDI/MDI frame window
CDialog                - Dialog window
CDialogEx              - Extended dialog (use this)
CView                  - Document view

// Controls
CButton                - Button control
CEdit                  - Edit control (text box)
CStatic                - Static text/image
CListBox               - List box
CComboBox              - Combo box (dropdown)
CListCtrl              - List control (detailed)
CTreeCtrl              - Tree control
CProgressCtrl          - Progress bar
CSliderCtrl            - Slider
CSpinButtonCtrl        - Spin button

// Utility
CString                - String class
CRect                  - Rectangle (left, top, right, bottom)
CPoint                 - Point (x, y)
CSize                  - Size (cx, cy)
CArray<T>              - Dynamic array
CList<T>               - Linked list
CMap<K, V>             - Map/dictionary
CTime                  - Date/time
CFile                  - File I/O
CDC                    - Device context (drawing)
```

---

## 11. Practical Examples

### Example 1: Custom Dialog with Multiple Inheritance Levels

```cpp
// 1. Create a base dialog class for common functionality
class CBaseDialog : public CDialogEx
{
protected:
    // Common functionality all dialogs need
    virtual BOOL OnInitDialog()
    {
        CDialogEx::OnInitDialog();

        // Center the dialog
        CenterWindow();

        // Set icon
        HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
        SetIcon(hIcon, TRUE);
        SetIcon(hIcon, FALSE);

        return TRUE;
    }

    // Helper function all dialogs can use
    void ShowError(LPCTSTR lpszMessage)
    {
        MessageBox(lpszMessage, _T("Error"), MB_OK | MB_ICONERROR);
    }

    void ShowInfo(LPCTSTR lpszMessage)
    {
        MessageBox(lpszMessage, _T("Information"), MB_OK | MB_ICONINFORMATION);
    }
};

// 2. Derive your specific dialogs from CBaseDialog
class CLoginDialog : public CBaseDialog
{
protected:
    CString m_strUsername;
    CString m_strPassword;

    virtual BOOL OnInitDialog()
    {
        CBaseDialog::OnInitDialog();  // ← Gets centering and icon from base

        // Login-specific initialization
        GetDlgItem(IDC_EDIT_USERNAME)->SetFocus();

        return FALSE;  // We set focus manually
    }

    virtual void OnOK()
    {
        UpdateData(TRUE);

        if (m_strUsername.IsEmpty() || m_strPassword.IsEmpty())
        {
            ShowError(_T("Please enter both username and password!"));
            return;  // Don't close
        }

        if (ValidateCredentials(m_strUsername, m_strPassword))
        {
            CBaseDialog::OnOK();  // Close with IDOK
        }
        else
        {
            ShowError(_T("Invalid credentials!"));
        }
    }

    bool ValidateCredentials(const CString& user, const CString& pass)
    {
        // Your validation logic
        return (user == _T("admin") && pass == _T("password"));
    }
};

// 3. Another dialog using the same base
class CSettingsDialog : public CBaseDialog
{
protected:
    BOOL m_bEnableSound;
    int m_nVolume;

    virtual BOOL OnInitDialog()
    {
        CBaseDialog::OnInitDialog();  // ← Gets centering and icon

        // Settings-specific initialization
        LoadSettings();

        return TRUE;
    }

    void LoadSettings()
    {
        // Load from registry or file
    }
};
```

**WHAT THIS SHOWS**:
- You can create your own base classes
- Inheritance chain: CSettingsDialog → CBaseDialog → CDialogEx → CDialog → CWnd
- Common functionality in base class, specific functionality in derived classes

### Example 2: Understanding Message Routing

```cpp
class CMyDialog : public CDialogEx
{
protected:
    virtual BOOL OnInitDialog()
    {
        CDialogEx::OnInitDialog();

        // Let's trace the call chain
        TRACE("CMyDialog::OnInitDialog called\n");
        // This runs AFTER CDialogEx::OnInitDialog()
        // Which runs AFTER CDialog::OnInitDialog()
        // Which runs AFTER CWnd::OnInitDialog()

        return TRUE;
    }

    afx_msg void OnBnClickedButton1()
    {
        // Message routing:
        // 1. Windows sends WM_COMMAND message
        // 2. MFC's message loop receives it
        // 3. Message map directs to this function
        // 4. This function executes

        MessageBox(_T("Button clicked!"));

        // No base class call - there's no CDialogEx::OnBnClickedButton1()
    }

    afx_msg void OnPaint()
    {
        if (IsIconic())
        {
            // Custom painting for minimized state
        }
        else
        {
            CDialogEx::OnPaint();  // Let base class handle normal painting
        }
    }

    DECLARE_MESSAGE_MAP()
};
```

### Example 3: Accessing Controls Through the Hierarchy

```cpp
void CMyDialog::OnButtonClick()
{
    // Method 1: Using CWnd::GetDlgItem (inherited)
    CWnd* pEdit = GetDlgItem(IDC_EDIT1);
    // GetDlgItem is defined in CWnd
    // You inherit it through: CMyDialog → CDialogEx → CDialog → CWnd

    // Method 2: Using CWnd::GetDlgItemText (inherited)
    CString str;
    GetDlgItemText(IDC_EDIT1, str);
    // Also from CWnd

    // Method 3: Using CDialog::UpdateData (inherited)
    UpdateData(TRUE);
    // From CDialog

    // All these functions are available because of inheritance!
}
```

---

## 12. Common Pitfalls

### Pitfall 1: Forgetting Virtual Keyword

```cpp
// WRONG - not virtual, doesn't override base class
class CMyDialog : public CDialogEx
{
    BOOL OnInitDialog()  // ← Missing 'virtual'
    {
        // This creates a NEW function, doesn't override CDialog::OnInitDialog()
        // Base class OnInitDialog() still runs, yours doesn't!
    }
};

// CORRECT
class CMyDialog : public CDialogEx
{
    virtual BOOL OnInitDialog()  // ← Virtual keyword
    {
        // Properly overrides base class
    }
};

// EVEN BETTER (C++11+)
class CMyDialog : public CDialogEx
{
    virtual BOOL OnInitDialog() override  // ← Compiler checks override
    {
        // 'override' keyword ensures you're actually overriding something
    }
};
```

### Pitfall 2: Wrong Base Class Call

```cpp
// WRONG - calling wrong base class
class CMyDialog : public CDialogEx
{
    virtual BOOL OnInitDialog()
    {
        CDialog::OnInitDialog();  // ← Should be CDialogEx!
        // Skips CDialogEx's initialization
        return TRUE;
    }
};

// CORRECT
class CMyDialog : public CDialogEx
{
    virtual BOOL OnInitDialog()
    {
        CDialogEx::OnInitDialog();  // ← Direct base class
        return TRUE;
    }
};
```

### Pitfall 3: Accessing Controls Before Creation

```cpp
// WRONG
class CMyDialog : public CDialogEx
{
    CMyDialog()
    {
        // Constructor runs BEFORE dialog is created
        SetDlgItemText(IDC_EDIT1, _T("Hello"));  // ← CRASH! No window yet
    }
};

// CORRECT
class CMyDialog : public CDialogEx
{
    virtual BOOL OnInitDialog()
    {
        CDialogEx::OnInitDialog();  // Creates controls

        // NOW controls exist
        SetDlgItemText(IDC_EDIT1, _T("Hello"));  // ← Works!

        return TRUE;
    }
};
```

### Pitfall 4: Not Calling Base Class When Required

```cpp
// WRONG - forgot base class call
void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
    // CDialogEx::DoDataExchange(pDX);  ← FORGOT THIS!

    DDX_Text(pDX, IDC_EDIT1, m_strValue);  // ← Might crash or fail
}

// CORRECT
void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);  // ← ALWAYS call base first

    DDX_Text(pDX, IDC_EDIT1, m_strValue);
}
```

### Pitfall 5: Misunderstanding IS-A Relationship

```cpp
// This works because of inheritance
void SomeFunction(CWnd* pWnd)
{
    // Can accept ANY CWnd-derived class
    pWnd->SetWindowText(_T("Hello"));
}

// Usage:
CMyDialog dlg;
SomeFunction(&dlg);  // ← Works! CMyDialog IS-A CWnd (through inheritance chain)

CButton btn;
SomeFunction(&btn);  // ← Works! CButton IS-A CWnd

CString str;
SomeFunction(&str);  // ← ERROR! CString is NOT-A CWnd
```

---

## Summary

### Key Takeaways

1. **CObject** is the root of most MFC classes, providing RTTI and serialization
2. **CWnd** is the base for all window classes, wrapping HWND
3. **CWinApp** manages your application, InitInstance() is the entry point
4. **CDialog/CDialogEx** are used for dialog windows (modal/modeless)
5. **Inheritance** gives you access to all base class functions
6. **Virtual functions** allow you to override base class behavior
7. **ALWAYS call base class** in OnInitDialog and DoDataExchange (FIRST)
8. **CONDITIONALLY call base class** in OnOK/OnCancel (only if you want default behavior)

### Critical Functions Reference

```cpp
// Application
CWinApp::InitInstance()        - Start here
CWinApp::ExitInstance()        - Cleanup

// Dialog
CDialog::OnInitDialog()        - Initialize dialog (call base FIRST)
CDialog::DoDataExchange()      - DDX/DDV (call base FIRST)
CDialog::OnOK()                - OK button (call base LAST, or not at all)
CDialog::OnCancel()            - Cancel button (call base LAST, or not at all)
CDialog::UpdateData()          - Transfer data (TRUE = control→var, FALSE = var→control)

// Window
CWnd::GetDlgItem()            - Get control by ID
CWnd::GetDlgItemText()        - Read control text
CWnd::SetDlgItemText()        - Write control text
CWnd::SetWindowText()         - Set window/control text
CWnd::ShowWindow()            - Show/hide window
```

### Next Steps

In **Lesson 04: Message Maps - The Heart of MFC**, you'll learn:
- How message maps actually work
- Understanding ON_MESSAGE, ON_COMMAND, ON_BN_CLICKED
- Adding custom message handlers
- Message routing and propagation
- Reflected messages
- When to use message maps vs. virtual functions

### Practice Exercise

**Create a three-level dialog hierarchy**:

1. **CBaseFormDialog** (base class for all forms)
   - Centers dialog in OnInitDialog
   - Provides ShowError/ShowInfo helpers
   - Validates data in OnOK (checks all fields non-empty)

2. **CPersonInfoDialog** (inherits from CBaseFormDialog)
   - Fields: Name, Age, Email
   - Validates email format in OnOK
   - Calls base class validation

3. **CEmployeeInfoDialog** (inherits from CPersonInfoDialog)
   - Adds fields: Employee ID, Department
   - Validates Employee ID is numeric
   - Calls base class validation

**Goal**: See how inheritance allows you to build up functionality progressively.

---

**Next Lesson**: [Lesson 04: Message Maps - The Heart of MFC](Lesson-04-Message-Maps-Heart-MFC.md)
