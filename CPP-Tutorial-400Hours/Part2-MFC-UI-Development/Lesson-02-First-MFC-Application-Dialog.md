# Lesson 02: Your First MFC Application (Dialog-based)

**Duration**: 8-10 hours
**Difficulty**: Beginner

## Table of Contents
1. Creating a Dialog-Based MFC Project
2. Understanding the Generated Files - WHERE Everything Is
3. Anatomy of the Resource File
4. Your First Button Click Handler
5. Working with Edit Controls
6. Complete Calculator Example
7. Debugging Your First MFC Application
8. Common Errors and Solutions
9. Step-by-Step Video Walkthrough Guide
10. Exercises

---

## 1. Creating a Dialog-Based MFC Project

### Step-by-Step: Creating the Project

#### Step 1: Launch Visual Studio

1. Open **Visual Studio 2019** or **2022**
2. Click **"Create a new project"**
3. In the search box, type **"MFC"**
4. Select **"MFC Application"** (NOT MFC ActiveX or MFC DLL)
5. Click **Next**

**WHAT YOU SEE**: If MFC Application doesn't appear, MFC is not installed. Go back to Lesson 01 for installation instructions.

#### Step 2: Configure Your Project

```
Project name:     MyFirstMFCApp
Location:         C:\MFCProjects\
Solution name:    MyFirstMFCApp
```

**IMPORTANT**:
- NO spaces in project name (use MyFirstMFCApp, not "My First MFC App")
- Avoid special characters
- Short path names work best with MFC

Click **Create**

#### Step 3: MFC Application Wizard

**Page 1: Application Type**

```
Application type:        Dialog based      ← SELECT THIS
Project style:          MFC Standard
Use Unicode libraries:   YES (checked)     ← Keep this checked
```

**WHERE TO LOOK**: "Dialog based" means your main window is a dialog (simpler than SDI/MDI).

Click **Next**

**Page 2: User Interface Features**

```
Dialog title:           My First MFC Application
[✓] Minimize box
[✓] Maximize box
[✓] About box           ← Adds Help → About menu
[ ] System menu
```

**WHAT THIS MEANS**:
- Minimize/Maximize: Users can minimize/maximize window
- About box: Generates a second dialog for Help → About

Click **Next**

**Page 3: Advanced Features**

```
[✓] ActiveX controls     ← Leave checked
[ ] Common Control Manifest
[ ] Support Restart Manager
[ ] Windows Sockets
```

**FOR NOW**: Just leave defaults and click **Next**

**Page 4: Generated Classes**

```
Class name:         CMyFirstMFCAppDlg
Header file:        MyFirstMFCAppDlg.h
Implementation:     MyFirstMFCAppDlg.cpp
Base class:         CDialogEx            ← This is correct
```

**WHERE TO LOOK**: This creates your main dialog class. The "C" prefix and "Dlg" suffix are MFC conventions.

Click **Finish**

#### Step 4: Wait for Code Generation

Visual Studio generates approximately **15-20 files**. This takes 5-10 seconds.

**WHAT YOU SEE**: Solution Explorer fills with files. DON'T PANIC - we'll examine each one.

---

## 2. Understanding the Generated Files - WHERE Everything Is

### The Complete File Breakdown

After project creation, you'll see these files in **Solution Explorer**:

```
MyFirstMFCApp/
│
├── Header Files/
│   ├── framework.h              ← Common includes (MFC headers)
│   ├── MyFirstMFCApp.h          ← Application class DECLARATION
│   ├── MyFirstMFCAppDlg.h       ← Main dialog class DECLARATION
│   ├── pch.h                    ← Precompiled header
│   ├── Resource.h               ← Resource IDs (#define IDC_BUTTON1...)
│   └── targetver.h              ← Windows version targeting
│
├── Source Files/
│   ├── MyFirstMFCApp.cpp        ← Application class IMPLEMENTATION
│   ├── MyFirstMFCAppDlg.cpp     ← Main dialog IMPLEMENTATION
│   └── pch.cpp                  ← Precompiled header source
│
├── Resource Files/
│   ├── MyFirstMFCApp.rc         ← Resource script (dialogs, menus)
│   ├── MyFirstMFCApp.rc2        ← Resources not edited by VS
│   └── MyFirstMFCApp.ico        ← Application icon
│
└── ReadMe.txt                   ← Auto-generated, usually ignored
```

### The 5 Files You'll Actually Edit

Out of ~20 files, you'll primarily work with these **5**:

#### 1. Resource.h - The ID Definitions

```cpp
// Resource.h - WHERE ALL IDs ARE DEFINED
// This file is MOSTLY auto-generated

#define IDR_MAINFRAME                   128
#define IDD_MYFIRSTMFCAPP_DIALOG        102
#define IDD_ABOUTBOX                    103
#define IDC_BUTTON_CLICK                1000
#define IDC_EDIT_INPUT                  1001
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

**WHERE TO LOOK**: When you add a control in Resource Editor, Visual Studio adds its ID here.

**WHAT YOU EDIT**: Usually nothing directly - the wizard adds IDs automatically.

#### 2. MyFirstMFCApp.rc - The Visual Resources

**WHERE TO ACCESS**:
- Solution Explorer → Resource Files → MyFirstMFCApp.rc → Double-click
- Or: View → Other Windows → Resource View (Ctrl+Shift+E)

**WHAT YOU SEE**: A tree view:
```
MyFirstMFCApp.rc
├── Dialog
│   ├── IDD_ABOUTBOX              ← About dialog
│   └── IDD_MYFIRSTMFCAPP_DIALOG  ← Your main dialog
├── Icon
│   └── IDR_MAINFRAME             ← Application icon
├── String Table
│   └── (String resources)
└── Version
    └── VS_VERSION_INFO
```

**WHAT YOU EDIT**: Double-click `IDD_MYFIRSTMFCAPP_DIALOG` to open the visual dialog editor.

#### 3. MyFirstMFCAppDlg.h - Dialog Class Declaration

```cpp
// MyFirstMFCAppDlg.h
#pragma once

// WHERE YOUR CLASS IS DECLARED
class CMyFirstMFCAppDlg : public CDialogEx
{
// Construction
public:
    CMyFirstMFCAppDlg(CWnd* pParent = nullptr);

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MYFIRSTMFCAPP_DIALOG };  // ← Links to .rc file
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV support

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

    // ↓ YOUR HANDLERS GO HERE (added by Class Wizard)
};
```

**WHERE TO LOOK**:
- Member variables go after `protected:` or `public:`
- Handler function declarations go at the bottom (after `DECLARE_MESSAGE_MAP()`)

**WHAT YOU EDIT**: You add member variables and handler declarations (usually via Class Wizard).

#### 4. MyFirstMFCAppDlg.cpp - Dialog Implementation

```cpp
// MyFirstMFCAppDlg.cpp
#include "pch.h"
#include "framework.h"
#include "MyFirstMFCApp.h"
#include "MyFirstMFCAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// WHERE YOUR IMPLEMENTATION GOES

CMyFirstMFCAppDlg::CMyFirstMFCAppDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MYFIRSTMFCAPP_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyFirstMFCAppDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    // ↓ DDX variables go here
}

BEGIN_MESSAGE_MAP(CMyFirstMFCAppDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    // ↓ YOUR HANDLERS GO HERE (added by Class Wizard)
END_MESSAGE_MAP()

// ↓ FUNCTION IMPLEMENTATIONS GO HERE

BOOL CMyFirstMFCAppDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO: Add extra initialization here

    return TRUE;
}
```

**WHERE TO LOOK**:
- `DoDataExchange()` - Add DDX calls here
- `BEGIN_MESSAGE_MAP` - Handler entries go here
- Below message map - Handler function implementations go here

**WHAT YOU EDIT**: Most of your code goes in this file.

#### 5. MyFirstMFCApp.cpp - Application Class

```cpp
// MyFirstMFCApp.cpp - Application entry point
#include "pch.h"
#include "framework.h"
#include "MyFirstMFCApp.h"
#include "MyFirstMFCAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object
CMyFirstMFCAppApp theApp;

// Application class constructor
CMyFirstMFCAppApp::CMyFirstMFCAppApp()
{
    // TODO: add construction code here
}

BEGIN_MESSAGE_MAP(CMyFirstMFCAppApp, CWinApp)
END_MESSAGE_MAP()

// Application initialization - STARTS HERE
BOOL CMyFirstMFCAppApp::InitInstance()
{
    CWinApp::InitInstance();

    // Create the dialog
    CMyFirstMFCAppDlg dlg;
    m_pMainWnd = &dlg;

    // Show the dialog
    INT_PTR nResponse = dlg.DoModal();

    // Return FALSE to exit
    return FALSE;
}
```

**WHERE TO LOOK**: `InitInstance()` is where execution starts.

**WHAT YOU EDIT**: Rarely - only for application-level initialization.

---

## 3. Anatomy of the Resource File

### Opening the Dialog Designer

1. **View → Other Windows → Resource View** (Ctrl+Shift+E)
2. Expand **MyFirstMFCApp.rc**
3. Expand **Dialog**
4. **Double-click** `IDD_MYFIRSTMFCAPP_DIALOG`

**WHAT YOU SEE**: A visual dialog window with:
- OK button (bottom right)
- Cancel button (bottom right)
- Static text "TODO: Place dialog controls here."
- Your application icon (top left)

### Understanding the Dialog Editor

**WHAT'S ON SCREEN**:

```
┌─────────────────────────────────────────┐
│  [Icon]  My First MFC Application       │  ← Title bar
├─────────────────────────────────────────┤
│                                         │
│  TODO: Place dialog controls here.     │  ← Static text (IDC_STATIC)
│                                         │
│                                         │
│                     [  OK  ] [Cancel]   │  ← Buttons (IDOK, IDCANCEL)
└─────────────────────────────────────────┘
```

**WHERE TO LOOK**:

1. **Toolbox** (View → Toolbox or Ctrl+Alt+X): Shows available controls
   - Button
   - Edit Control
   - Static Text
   - Check Box
   - Radio Button
   - List Box
   - Combo Box
   - etc.

2. **Properties Window** (View → Properties Window or F4): Shows selected control properties
   - ID (e.g., IDC_BUTTON1)
   - Caption (visible text)
   - Position (X, Y)
   - Size (Width, Height)

### Adding Your First Control - A Button

**Step-by-Step**:

1. **Toolbox → Click "Button"**
2. **Click on the dialog** where you want the button (above OK/Cancel)
3. The button appears with default text "Button1"

**Configuring the Button**:

4. **Right-click the button → Properties** (or press F4)
5. In Properties window:
   ```
   ID:        IDC_BUTTON_SAYHELLO    ← Change from IDC_BUTTON1
   Caption:   Say Hello!             ← Change from "Button1"
   ```

6. **Resize the button**: Drag the corners to make it larger

**WHAT JUST HAPPENED**:
- Visual Studio added `#define IDC_BUTTON_SAYHELLO 1000` to Resource.h
- The button is now in the .rc file (in text format)
- The button doesn't DO anything yet - we need a handler

### Adding an Edit Control for Input

**Step-by-Step**:

1. **Toolbox → Click "Edit Control"**
2. **Click on the dialog** to place it (above your button)
3. **Right-click the edit control → Properties**:
   ```
   ID:        IDC_EDIT_NAME
   ```

### Adding Static Text for Output

**Step-by-Step**:

1. **Toolbox → Click "Static Text"**
2. **Click on the dialog** to place it (below your button)
3. **Right-click static text → Properties**:
   ```
   ID:        IDC_STATIC_RESULT
   Caption:   (leave blank)
   ```

**IMPORTANT**: By default, static text has ID `IDC_STATIC` (-1). To reference it in code, you MUST change the ID!

### Your Dialog Should Now Look Like This:

```
┌─────────────────────────────────────────┐
│  [Icon]  My First MFC Application       │
├─────────────────────────────────────────┤
│  Enter your name:                       │
│  ┌────────────────────────────────────┐ │  ← Edit: IDC_EDIT_NAME
│  │                                    │ │
│  └────────────────────────────────────┘ │
│                                         │
│         [ Say Hello! ]                  │  ← Button: IDC_BUTTON_SAYHELLO
│                                         │
│  (result appears here)                  │  ← Static: IDC_STATIC_RESULT
│                                         │
│                     [  OK  ] [Cancel]   │
└─────────────────────────────────────────┘
```

**Save the resource file**: Ctrl+S or File → Save

---

## 4. Your First Button Click Handler

### Adding the Handler Function

Now we need to make the button DO something when clicked.

**Step-by-Step**:

1. **In Dialog Editor**, ensure your dialog is open
2. **Double-click the "Say Hello!" button**

**WHAT HAPPENS**: Visual Studio:
- Opens MyFirstMFCAppDlg.h
- Adds this line at the bottom:
  ```cpp
  afx_msg void OnBnClickedButtonSayhello();
  ```

- Opens MyFirstMFCAppDlg.cpp
- Adds to message map:
  ```cpp
  BEGIN_MESSAGE_MAP(CMyFirstMFCAppDlg, CDialogEx)
      // ... existing entries
      ON_BN_CLICKED(IDC_BUTTON_SAYHELLO, &CMyFirstMFCAppDlg::OnBnClickedButtonSayhello)
  END_MESSAGE_MAP()
  ```

- Creates empty handler function:
  ```cpp
  void CMyFirstMFCAppDlg::OnBnClickedButtonSayhello()
  {
      // TODO: Add your control notification handler code here
  }
  ```

**WHERE YOU ARE**: The cursor is inside the empty handler function. **This is where you write your code!**

### Writing the Handler Code

Replace the TODO comment with this code:

```cpp
void CMyFirstMFCAppDlg::OnBnClickedButtonSayhello()
{
    // Get the text from the edit control
    CString strName;
    GetDlgItemText(IDC_EDIT_NAME, strName);

    // Check if name is empty
    if (strName.IsEmpty())
    {
        MessageBox(_T("Please enter your name!"), _T("Error"), MB_OK | MB_ICONWARNING);
        return;
    }

    // Build the greeting message
    CString strMessage;
    strMessage.Format(_T("Hello, %s! Welcome to MFC!"), strName);

    // Display in the static text control
    SetDlgItemText(IDC_STATIC_RESULT, strMessage);
}
```

### Understanding Every Line

```cpp
void CMyFirstMFCAppDlg::OnBnClickedButtonSayhello()
// ↑ This function runs when button is clicked
{
    // CString is MFC's string class (better than char*)
    CString strName;

    // GetDlgItemText retrieves text from a control
    // Syntax: GetDlgItemText(CONTROL_ID, string_variable)
    GetDlgItemText(IDC_EDIT_NAME, strName);
    // ↑ Reads whatever user typed into IDC_EDIT_NAME

    // Check if empty
    if (strName.IsEmpty())  // CString method
    {
        // MessageBox shows a popup
        // Syntax: MessageBox(message, title, flags)
        MessageBox(_T("Please enter your name!"),    // Message
                   _T("Error"),                      // Title
                   MB_OK | MB_ICONWARNING);          // Flags: OK button + warning icon
        return;  // Exit function early
    }

    // Format a string (like sprintf)
    CString strMessage;
    strMessage.Format(_T("Hello, %s! Welcome to MFC!"), strName);
    // ↑ %s gets replaced with strName

    // SetDlgItemText sets text of a control
    // Syntax: SetDlgItemText(CONTROL_ID, text)
    SetDlgItemText(IDC_STATIC_RESULT, strMessage);
    // ↑ Displays message in the static text control
}
```

**KEY POINTS**:
- `GetDlgItemText()` - **reads** from control
- `SetDlgItemText()` - **writes** to control
- `_T("text")` - Unicode-safe string literal
- `IDC_EDIT_NAME` - the ID from Resource.h

### Testing Your Application

1. **Press F5** (or Debug → Start Debugging)
2. **Wait for compilation** (first time takes 30-60 seconds)
3. **Your dialog appears!**

**Try it**:
- Type your name in the edit box
- Click "Say Hello!"
- See the greeting appear below

**Try edge cases**:
- Click "Say Hello!" without typing anything → Warning message
- Type different names → See personalized greetings

**Stop debugging**: Click X on the dialog, or Shift+F5 in Visual Studio

---

## 5. Working with Edit Controls

### Getting and Setting Text

We've seen basic text operations. Let's explore more:

```cpp
// METHOD 1: GetDlgItemText / SetDlgItemText (what we used)
CString str;
GetDlgItemText(IDC_EDIT1, str);        // Read
SetDlgItemText(IDC_EDIT1, _T("Hello")); // Write

// METHOD 2: GetDlgItem + SetWindowText
CWnd* pEdit = GetDlgItem(IDC_EDIT1);
if (pEdit != nullptr)
{
    pEdit->SetWindowText(_T("Hello"));  // Write

    CString str;
    pEdit->GetWindowText(str);          // Read
}

// METHOD 3: Control variable (DDX - covered in Lesson 09)
// In .h file: CString m_strEdit1;
// In DoDataExchange(): DDX_Text(pDX, IDC_EDIT1, m_strEdit1);
UpdateData(TRUE);   // Control → Variable
// Use m_strEdit1
UpdateData(FALSE);  // Variable → Control
```

**WHICH TO USE**?
- **Simple operations**: `GetDlgItemText` / `SetDlgItemText` (easiest)
- **Multiple operations on same control**: `GetDlgItem` (more efficient)
- **Form with many fields**: DDX (automatic, covered later)

### Working with Numbers

Edit controls store **text**, not numbers. You must convert:

```cpp
// Reading a number from edit control
void CMyDlg::OnButtonCalculate()
{
    CString strNum;
    GetDlgItemText(IDC_EDIT_NUMBER, strNum);

    // Convert string to integer
    int nValue = _ttoi(strNum);  // Text TO Integer
    // Or for double: double dValue = _tstof(strNum);

    // Do something with the number
    nValue = nValue * 2;

    // Convert back to string and display
    CString strResult;
    strResult.Format(_T("%d"), nValue);  // Integer to string
    SetDlgItemText(IDC_STATIC_RESULT, strResult);
}
```

**KEY FUNCTIONS**:
- `_ttoi(str)` - String to integer
- `_tstof(str)` - String to floating-point
- `_ttol(str)` - String to long
- `Format(_T("%d"), num)` - Integer to string
- `Format(_T("%.2f"), num)` - Float to string (2 decimals)

**WHY _ttoi and not atoi?**
- `_ttoi` works with both ANSI and Unicode
- `atoi` only works with ANSI (char*)
- Always use the `_t` versions in MFC

---

## 6. Complete Calculator Example

Let's build a simple calculator to solidify these concepts.

### Step 1: Design the Dialog

**Add these controls** (use Toolbox):

```
┌─────────────────────────────────────────┐
│  [Icon]  Simple Calculator              │
├─────────────────────────────────────────┤
│  First Number:                          │
│  ┌────────────────┐                     │  ← IDC_EDIT_NUM1
│  │                │                     │
│  └────────────────┘                     │
│                                         │
│  Second Number:                         │
│  ┌────────────────┐                     │  ← IDC_EDIT_NUM2
│  │                │                     │
│  └────────────────┘                     │
│                                         │
│  [Add] [Subtract] [Multiply] [Divide]  │  ← 4 buttons
│                                         │
│  Result: __________                     │  ← IDC_STATIC_RESULT
│                                         │
│                     [  OK  ] [Cancel]   │
└─────────────────────────────────────────┘
```

**Control IDs**:
```
IDC_EDIT_NUM1          (Edit Control)
IDC_EDIT_NUM2          (Edit Control)
IDC_BUTTON_ADD         (Button, Caption: "Add")
IDC_BUTTON_SUBTRACT    (Button, Caption: "Subtract")
IDC_BUTTON_MULTIPLY    (Button, Caption: "Multiply")
IDC_BUTTON_DIVIDE      (Button, Caption: "Divide")
IDC_STATIC_RESULT      (Static Text)
```

### Step 2: Add Handler Functions

**For each button**, double-click it to create a handler:
- `OnBnClickedButtonAdd()`
- `OnBnClickedButtonSubtract()`
- `OnBnClickedButtonMultiply()`
- `OnBnClickedButtonDivide()`

### Step 3: Implement the Handlers

```cpp
// MyFirstMFCAppDlg.cpp

// Helper function to get numbers from edit controls
bool CMyFirstMFCAppDlg::GetNumbers(double& num1, double& num2)
{
    // Get text from first edit control
    CString strNum1;
    GetDlgItemText(IDC_EDIT_NUM1, strNum1);

    // Get text from second edit control
    CString strNum2;
    GetDlgItemText(IDC_EDIT_NUM2, strNum2);

    // Check if empty
    if (strNum1.IsEmpty() || strNum2.IsEmpty())
    {
        MessageBox(_T("Please enter both numbers!"),
                   _T("Error"),
                   MB_OK | MB_ICONWARNING);
        return false;
    }

    // Convert to double
    num1 = _tstof(strNum1);
    num2 = _tstof(strNum2);

    return true;
}

// Helper function to display result
void CMyFirstMFCAppDlg::ShowResult(double result)
{
    CString strResult;
    strResult.Format(_T("Result: %.2f"), result);
    SetDlgItemText(IDC_STATIC_RESULT, strResult);
}

// Add button handler
void CMyFirstMFCAppDlg::OnBnClickedButtonAdd()
{
    double num1, num2;
    if (GetNumbers(num1, num2))
    {
        ShowResult(num1 + num2);
    }
}

// Subtract button handler
void CMyFirstMFCAppDlg::OnBnClickedButtonSubtract()
{
    double num1, num2;
    if (GetNumbers(num1, num2))
    {
        ShowResult(num1 - num2);
    }
}

// Multiply button handler
void CMyFirstMFCAppDlg::OnBnClickedButtonMultiply()
{
    double num1, num2;
    if (GetNumbers(num1, num2))
    {
        ShowResult(num1 * num2);
    }
}

// Divide button handler
void CMyFirstMFCAppDlg::OnBnClickedButtonDivide()
{
    double num1, num2;
    if (GetNumbers(num1, num2))
    {
        // Check for division by zero
        if (num2 == 0.0)
        {
            MessageBox(_T("Cannot divide by zero!"),
                       _T("Error"),
                       MB_OK | MB_ICONERROR);
            return;
        }

        ShowResult(num1 / num2);
    }
}
```

**Don't forget to add helper function declarations to the .h file**:

```cpp
// MyFirstMFCAppDlg.h
class CMyFirstMFCAppDlg : public CDialogEx
{
    // ... existing code ...

protected:
    // Helper functions
    bool GetNumbers(double& num1, double& num2);
    void ShowResult(double result);

    // Handler functions
    afx_msg void OnBnClickedButtonAdd();
    afx_msg void OnBnClickedButtonSubtract();
    afx_msg void OnBnClickedButtonMultiply();
    afx_msg void OnBnClickedButtonDivide();

    DECLARE_MESSAGE_MAP()
};
```

### Step 4: Test the Calculator

1. **Press F5**
2. **Enter** 10 in first box, 5 in second box
3. **Click Add** → Should show "Result: 15.00"
4. **Click Subtract** → Should show "Result: 5.00"
5. **Click Multiply** → Should show "Result: 50.00"
6. **Click Divide** → Should show "Result: 2.00"

**Test edge cases**:
- Enter 0 in second box, click Divide → Error message
- Leave boxes empty, click any button → Error message
- Enter decimals (3.14, 2.5) → Works correctly

---

## 7. Debugging Your First MFC Application

### Setting Breakpoints

1. **Open MyFirstMFCAppDlg.cpp**
2. **Find** `OnBnClickedButtonAdd()`
3. **Click in the gray margin** to the left of the first line
4. **A red dot appears** - this is a breakpoint

```cpp
void CMyFirstMFCAppDlg::OnBnClickedButtonAdd()
{
●→  double num1, num2;  // ← Breakpoint here (red dot)
    if (GetNumbers(num1, num2))
    {
        ShowResult(num1 + num2);
    }
}
```

### Running with Debugger

1. **Press F5** (Start Debugging)
2. **Enter numbers** in the edit controls
3. **Click Add button**
4. **Execution stops** at the breakpoint (yellow arrow appears)

**WHAT YOU SEE**:
- Yellow arrow pointing to the line about to execute
- Variables shown in Locals window (View → Locals)
- Call stack shown in Call Stack window

### Debugger Commands

**F10** - Step Over (execute current line, don't step into functions)
**F11** - Step Into (if current line calls a function, step into it)
**F5** - Continue (run until next breakpoint or end)
**Shift+F5** - Stop Debugging

**Try this**:
1. With breakpoint at `double num1, num2;`, press **F5** to start
2. Click Add
3. Press **F10** - moves to next line
4. Press **F10** - calls `GetNumbers()` (steps over it)
5. **Hover over `num1`** - see its value (should be your first number)
6. Press **F5** - continues to end

### Watching Variables

1. **Right-click a variable** (e.g., `num1`)
2. **Select "Add Watch"**
3. **Watch window** shows the variable's value as you step through code

### Common Debugging Scenarios

**Problem**: Button click doesn't work

**Solution**:
1. Set breakpoint in handler function
2. Click button
3. **If breakpoint doesn't trigger**: Message map is wrong
   - Check `BEGIN_MESSAGE_MAP` has correct entry
   - Check control ID matches

**Problem**: Wrong value displayed

**Solution**:
1. Set breakpoint before `SetDlgItemText`
2. **Watch** the variable being displayed
3. Verify its value is correct
4. If wrong, step backward through logic to find where it went wrong

---

## 8. Common Errors and Solutions

### Error 1: "Cannot open include file: 'afxwin.h'"

**CAUSE**: MFC not installed

**SOLUTION**:
1. Close Visual Studio
2. Run Visual Studio Installer
3. Modify installation
4. Check "Desktop development with C++"
5. Check "MFC and ATL support" in Individual Components
6. Install

### Error 2: "OnBnClickedButton1: function does not take 0 arguments"

**CAUSE**: Wrong signature for handler

**WRONG**:
```cpp
void CMyDlg::OnBnClickedButton1(int param)  // ← Extra parameter!
{
}
```

**CORRECT**:
```cpp
void CMyDlg::OnBnClickedButton1()  // ← No parameters
{
}
```

**SOLUTION**: Always let Class Wizard generate handlers.

### Error 3: GetDlgItemText returns empty string

**CAUSE**: Wrong control ID, or control doesn't exist yet

**DEBUGGING**:
```cpp
CString str;
GetDlgItemText(IDC_EDIT1, str);

if (str.IsEmpty())
{
    // Check: Does IDC_EDIT1 exist in Resource.h?
    // Check: Is control actually on the dialog?
    // Check: Are you calling this in OnInitDialog AFTER base class call?
}
```

### Error 4: Crash when clicking button

**CAUSE**: Handler not in message map

**SOLUTION**: Check message map has entry:
```cpp
BEGIN_MESSAGE_MAP(CMyDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CMyDlg::OnBnClickedButton1)  // ← Must have this
END_MESSAGE_MAP()
```

### Error 5: "_T: undeclared identifier"

**CAUSE**: Missing includes

**SOLUTION**: Add to top of .cpp file:
```cpp
#include "pch.h"        // Must be FIRST
#include "framework.h"  // Contains MFC definitions
```

### Error 6: Unicode string errors

**WRONG**:
```cpp
SetDlgItemText(IDC_STATIC1, "Hello");  // Might not compile
```

**CORRECT**:
```cpp
SetDlgItemText(IDC_STATIC1, _T("Hello"));   // Unicode-safe
// Or:
SetDlgItemText(IDC_STATIC1, L"Hello");      // Explicitly Unicode
```

---

## 9. Step-by-Step Video Walkthrough Guide

**If you were watching a video, here's what you'd see**:

### Part 1: Project Creation (0:00 - 3:00)
- Launch Visual Studio
- Create new MFC Application
- Configure as Dialog-based
- Quick tour of generated files

### Part 2: Dialog Design (3:00 - 7:00)
- Open Resource View
- Add Edit Control
- Add Button
- Add Static Text
- Set control IDs and captions
- Resize and position controls

### Part 3: Adding Code (7:00 - 12:00)
- Double-click button to create handler
- Explain where code appears (.h and .cpp)
- Write GetDlgItemText code
- Write SetDlgItemText code
- Add error checking

### Part 4: Testing (12:00 - 15:00)
- Press F5 to run
- Test with valid input
- Test with empty input
- Test edge cases

### Part 5: Debugging (15:00 - 20:00)
- Set breakpoint
- Step through code with F10
- Watch variables
- Explain call stack

### Part 6: Calculator Example (20:00 - 35:00)
- Design calculator UI
- Add all buttons
- Implement each operation
- Test all functions
- Handle division by zero

---

## 10. Exercises

### Exercise 1: Temperature Converter

**Create an application** that converts Celsius to Fahrenheit and vice versa.

**Requirements**:
- Two edit controls (temperature input, result output)
- Two buttons: "C → F" and "F → C"
- Display result in static text

**Formulas**:
- F = C × 9/5 + 32
- C = (F - 32) × 5/9

**Code hint**:
```cpp
void CMyDlg::OnBnClickedButtonCtoF()
{
    CString strCelsius;
    GetDlgItemText(IDC_EDIT_INPUT, strCelsius);

    double celsius = _tstof(strCelsius);
    double fahrenheit = celsius * 9.0 / 5.0 + 32.0;

    CString strResult;
    strResult.Format(_T("%.2f°F"), fahrenheit);
    SetDlgItemText(IDC_STATIC_RESULT, strResult);
}
```

### Exercise 2: String Manipulator

**Create an application** with these functions:

**UI**:
- Edit control for input
- Buttons: "UPPERCASE", "lowercase", "Reverse", "Count"
- Static text for output

**Functions**:
- UPPERCASE: Convert to upper case
- lowercase: Convert to lower case
- Reverse: Reverse the string
- Count: Count characters

**Code hints**:
```cpp
// Uppercase
strText.MakeUpper();

// Lowercase
strText.MakeLower();

// Reverse
strText.MakeReverse();

// Count
int len = strText.GetLength();
```

### Exercise 3: Grade Calculator

**Create an application** that calculates letter grade from numeric score.

**Requirements**:
- Edit control for score (0-100)
- Button "Calculate Grade"
- Static text for result

**Grading scale**:
- 90-100: A
- 80-89: B
- 70-79: C
- 60-69: D
- Below 60: F

**Code hint**:
```cpp
void CMyDlg::OnBnClickedButtonCalculate()
{
    CString strScore;
    GetDlgItemText(IDC_EDIT_SCORE, strScore);

    int score = _ttoi(strScore);
    CString grade;

    if (score >= 90) grade = _T("A");
    else if (score >= 80) grade = _T("B");
    else if (score >= 70) grade = _T("C");
    else if (score >= 60) grade = _T("D");
    else grade = _T("F");

    CString strResult;
    strResult.Format(_T("Grade: %s"), grade);
    SetDlgItemText(IDC_STATIC_RESULT, strResult);
}
```

---

## Summary

### What You Learned

1. **Creating MFC Dialog-Based Projects** - Step-by-step with MFC Wizard
2. **Understanding Generated Files** - WHERE each type of code goes
3. **Using Dialog Editor** - Adding controls visually
4. **Adding Event Handlers** - Double-clicking controls to create handlers
5. **Getting/Setting Control Text** - GetDlgItemText, SetDlgItemText
6. **Working with Numbers** - Converting strings to numbers and back
7. **Debugging MFC Applications** - Breakpoints, stepping, watching variables
8. **Common Errors** - Recognition and solutions

### Key Takeaways

**WHERE TO PUT CODE**:
- **Handlers**: In .cpp file, below message map
- **Helper functions**: Add declaration to .h, implementation to .cpp
- **Member variables**: In .h file, protected or public section

**WORKFLOW**:
1. Design UI in Resource Editor
2. Double-click controls to generate handlers
3. Write code in handler functions
4. Test with F5
5. Debug with breakpoints

**BEST PRACTICES**:
- Always use `_T()` or `L""` for string literals
- Check for empty strings before converting to numbers
- Add error checking (empty inputs, division by zero)
- Use helper functions for repeated code
- Let Class Wizard generate handlers

### Next Lesson Preview

In **Lesson 03: Understanding MFC Architecture and Class Hierarchy**, you'll learn:
- How CWnd, CDialog, and CWinApp relate
- The complete MFC class hierarchy
- Virtual functions and overriding
- When to call base class functions
- Understanding the document/view architecture

**Assignment**: Complete all three exercises before moving to Lesson 03. Each exercise reinforces the core skills you learned in this lesson.

---

**Next Lesson**: [Lesson 03: Understanding MFC Architecture and Class Hierarchy](Lesson-03-MFC-Architecture-Class-Hierarchy.md)
