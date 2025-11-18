# Lesson 06: Dialog-Based Applications Deep Dive

**Duration**: 8-10 hours
**Difficulty**: Intermediate

## Table of Contents
1. Modal vs Modeless Dialogs
2. Dialog Lifecycle
3. Dialog Templates
4. Passing Data Between Dialogs
5. Dialog Return Values
6. Common Dialog Properties
7. Dynamic Dialog Creation
8. Subclassing Dialogs
9. Dialog Bars and Control Bars
10. Real-World Dialog Patterns

---

## 1. Modal vs Modeless Dialogs

### Modal Dialogs (DoModal)

**Definition**: Blocks parent window until dialog closes

```cpp
// Modal Dialog - Synchronous
void CParentDialog::OnButtonShowModal()
{
    CChildDialog dlg;
    dlg.m_strData = _T("Initial data");  // Set data before showing

    INT_PTR result = dlg.DoModal();  // ← BLOCKS HERE until dialog closes

    if (result == IDOK)
    {
        // User clicked OK
        CString strResult = dlg.m_strData;  // Retrieve data after close
        MessageBox(strResult);
    }
    else if (result == IDCANCEL)
    {
        // User clicked Cancel or closed dialog
    }
}
```

**Key Points**:
- Stack allocation is OK (`CChildDialog dlg;`)
- Blocks parent until closed
- Easy data exchange (set before, retrieve after)
- Auto-cleanup when function returns

**Message Loop**:
```
DoModal() called
    ↓
Creates own message loop
    ↓
Processes messages for THIS dialog only
    ↓
Parent window disabled (greyed out)
    ↓
User closes dialog (IDOK, IDCANCEL, etc.)
    ↓
DoModal() returns
    ↓
Dialog destroyed automatically
    ↓
Execution continues
```

### Modeless Dialogs (Create)

**Definition**: Doesn't block parent, both windows active simultaneously

```cpp
// In MyDialog.h
class CMyDialog : public CDialogEx
{
public:
    CSettingsDialog* m_pModelessDlg;  // ← Must be member variable

    CMyDialog();
    ~CMyDialog();
};

// In MyDialog.cpp
CMyDialog::CMyDialog()
    : m_pModelessDlg(nullptr)
{
}

CMyDialog::~CMyDialog()
{
    // Clean up modeless dialog
    if (m_pModelessDlg != nullptr)
    {
        m_pModelessDlg->DestroyWindow();
        delete m_pModelessDlg;
        m_pModelessDlg = nullptr;
    }
}

void CMyDialog::OnButtonShowModeless()
{
    if (m_pModelessDlg == nullptr)
    {
        // Create dialog
        m_pModelessDlg = new CSettingsDialog();
        m_pModelessDlg->Create(IDD_SETTINGS_DIALOG, this);
        //                     ↑ Dialog ID        ↑ Parent

        m_pModelessDlg->ShowWindow(SW_SHOW);
    }
    else
    {
        // Already exists, just bring to front
        m_pModelessDlg->SetFocus();
    }
}
```

**Key Points**:
- **MUST** use heap allocation (`new`)
- **MUST** be member variable (survives function return)
- **MUST** manually destroy in parent's destructor
- Parent remains active
- No automatic cleanup

### Comparison Table

| Feature | Modal (DoModal) | Modeless (Create) |
|---------|----------------|-------------------|
| Blocks parent? | YES | NO |
| Memory allocation | Stack OK | Must use heap |
| Lifetime management | Automatic | Manual |
| Data exchange | Easy (before/after) | Complex (callbacks) |
| Use case | Settings, input, confirmation | Tool windows, palettes |
| Cleanup | Automatic | Must destroy manually |

---

## 2. Dialog Lifecycle

### Complete Lifecycle Flow

```
1. Construction Phase
   CMyDialog dlg;  (or new CMyDialog)
   ↓
   Constructor called
   Member variables initialized
   NO WINDOW YET

2. Creation Phase (DoModal or Create)
   dlg.DoModal();
   ↓
   Windows creates the dialog window (HWND)
   Controls created
   WM_INITDIALOG sent
   ↓
   OnInitDialog() called ← YOUR INITIALIZATION CODE HERE

3. Active Phase
   Dialog visible and running
   User interacts with controls
   Message handlers fire (OnButtonClick, etc.)

4. Closing Phase
   User clicks OK/Cancel/X
   ↓
   OnOK() or OnCancel() called
   ↓
   UpdateData(TRUE) called (in OnOK)
   ↓
   EndDialog(IDOK or IDCANCEL) called
   ↓
   DoModal() returns

5. Destruction Phase
   Dialog window destroyed
   ↓
   ~CMyDialog() called
   Cleanup
```

### Critical Functions in Lifecycle

#### Constructor

```cpp
CMyDialog::CMyDialog(CWnd* pParent)
    : CDialogEx(IDD_MY_DIALOG, pParent)
    , m_nValue(0)           // Initialize member variables
    , m_strName(_T(""))
{
    // NO WINDOW YET - Can't access controls!
    // Don't call GetDlgItem, SetDlgItemText, etc.
}
```

**DO**:
- Initialize member variables
- Set default values
- Initialize non-window resources

**DON'T**:
- Access controls (they don't exist yet)
- Call Windows API functions

#### OnInitDialog

```cpp
BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();  // ← MUST call base class FIRST

    // NOW controls exist - safe to access

    // Set initial control values
    SetDlgItemText(IDC_EDIT_NAME, _T("Default name"));

    // Configure controls
    CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
    if (pSlider != nullptr)
    {
        pSlider->SetRange(0, 100);
        pSlider->SetPos(50);
    }

    // Set initial selection
    CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO1);
    if (pCombo != nullptr)
    {
        pCombo->AddString(_T("Option 1"));
        pCombo->AddString(_T("Option 2"));
        pCombo->SetCurSel(0);
    }

    // Load saved settings
    LoadSettings();

    return TRUE;  // Return TRUE unless you set focus to a control
                  // Return FALSE if you call SetFocus()
}
```

**DO**:
- Call base class first
- Initialize control values
- Configure control properties
- Load saved settings
- Set focus if needed

**RETURN VALUE**:
- `TRUE` - System sets focus to first control
- `FALSE` - You set focus manually

#### OnOK

```cpp
void CMyDialog::OnOK()
{
    // Validate data before closing
    UpdateData(TRUE);  // Transfer control → variables

    if (m_strName.IsEmpty())
    {
        MessageBox(_T("Please enter a name!"));
        GetDlgItem(IDC_EDIT_NAME)->SetFocus();
        return;  // Don't close dialog
    }

    if (m_nAge < 0 || m_nAge > 120)
    {
        MessageBox(_T("Invalid age!"));
        return;  // Don't close dialog
    }

    // Save settings
    SaveSettings();

    // Close dialog with IDOK
    CDialogEx::OnOK();  // ← Calls EndDialog(IDOK)
}
```

**DO**:
- Validate user input
- Save settings
- Call base class if validation passes

**DON'T**:
- Call base class if validation fails
- Forget UpdateData(TRUE)

#### OnCancel

```cpp
void CMyDialog::OnCancel()
{
    // Confirm cancellation if there are unsaved changes
    if (m_bDataModified)
    {
        int result = MessageBox(_T("Discard changes?"),
                               _T("Confirm"),
                               MB_YESNO | MB_ICONQUESTION);
        if (result != IDYES)
        {
            return;  // Don't close
        }
    }

    // Close dialog with IDCANCEL
    CDialogEx::OnCancel();  // ← Calls EndDialog(IDCANCEL)
}
```

#### Destructor

```cpp
CMyDialog::~CMyDialog()
{
    // Clean up resources
    if (m_pModelessChild != nullptr)
    {
        m_pModelessChild->DestroyWindow();
        delete m_pModelessChild;
    }

    // Clean up other resources (files, memory, etc.)
}
```

---

## 3. Dialog Templates

### What is a Dialog Template?

A dialog template defines:
- Dialog size and position
- Style flags (border, title bar, etc.)
- Font
- Controls and their properties

**WHERE**: Stored in the .rc file

```cpp
// In YourApp.rc (text format)
IDD_MY_DIALOG DIALOGEX 0, 0, 320, 200
STYLE DS_SETFONT | DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "My Dialog"
FONT 8, "MS Sans Serif", 0, 0, 0x0
BEGIN
    DEFPUSHBUTTON   "OK",IDOK,209,179,50,14
    PUSHBUTTON      "Cancel",IDCANCEL,263,179,50,14
    EDITTEXT        IDC_EDIT1,50,50,200,14,ES_AUTOHSCROLL
    LTEXT           "Name:",IDC_STATIC,20,52,25,8
END
```

### Common Dialog Styles

```cpp
// Basic styles (choose one)
WS_POPUP                    // Popup window (most dialogs)
WS_CHILD                    // Child window (rare for dialogs)

// Title bar and border
WS_CAPTION                  // Title bar
WS_SYSMENU                  // System menu (icon, minimize, close)
WS_THICKFRAME               // Resizable border
DS_MODALFRAME               // Fixed modal frame (common for dialogs)

// Buttons on title bar
WS_MINIMIZEBOX              // Minimize button
WS_MAXIMIZEBOX              // Maximize button

// Position
DS_CENTER                   // Center on screen
DS_CENTERMOUSE              // Center on mouse

// Other
WS_VISIBLE                  // Initially visible
DS_SETFONT                  // Uses specified font
```

### Setting Dialog Properties

#### In Resource Editor

1. Open dialog in Resource Editor
2. Click empty area (not on a control)
3. Properties window shows dialog properties:

```
Properties:
    ID:             IDD_MY_DIALOG
    Caption:        "My Dialog"
    Font name:      MS Sans Serif
    Font size:      8
    Border:         Dialog Frame
    System Menu:    True
    Minimize Box:   False
    Maximize Box:   False
    Center:         True
```

#### In Code (Runtime)

```cpp
BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Change title
    SetWindowText(_T("New Title"));

    // Center on parent
    CenterWindow();

    // Disable close button
    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
    }

    // Make resizable
    ModifyStyle(0, WS_THICKFRAME);

    return TRUE;
}
```

---

## 4. Passing Data Between Dialogs

### Method 1: Public Member Variables (Simplest)

```cpp
// ChildDialog.h
class CChildDialog : public CDialogEx
{
public:
    CString m_strInput;   // Public - parent can access directly
    int m_nChoice;
};

// ParentDialog.cpp
void CParentDialog::OnButtonShowChild()
{
    CChildDialog dlg;

    // Pass data TO child
    dlg.m_strInput = m_strData;
    dlg.m_nChoice = m_nCurrentChoice;

    if (dlg.DoModal() == IDOK)
    {
        // Retrieve data FROM child
        m_strData = dlg.m_strInput;
        m_nCurrentChoice = dlg.m_nChoice;
    }
}
```

**Pros**: Simple, direct
**Cons**: Breaks encapsulation

### Method 2: Constructor Parameters

```cpp
// ChildDialog.h
class CChildDialog : public CDialogEx
{
private:
    CString m_strInput;
    int m_nChoice;

public:
    // Constructor with parameters
    CChildDialog(const CString& strInput, int nChoice, CWnd* pParent = nullptr);

    // Getters
    CString GetInput() const { return m_strInput; }
    int GetChoice() const { return m_nChoice; }
};

// ChildDialog.cpp
CChildDialog::CChildDialog(const CString& strInput, int nChoice, CWnd* pParent)
    : CDialogEx(IDD_CHILD_DIALOG, pParent)
    , m_strInput(strInput)
    , m_nChoice(nChoice)
{
}

// ParentDialog.cpp
void CParentDialog::OnButtonShowChild()
{
    CChildDialog dlg(m_strData, m_nCurrentChoice);

    if (dlg.DoModal() == IDOK)
    {
        m_strData = dlg.GetInput();
        m_nCurrentChoice = dlg.GetChoice();
    }
}
```

**Pros**: Better encapsulation
**Cons**: More code

### Method 3: SetData/GetData Methods

```cpp
// ChildDialog.h
class CChildDialog : public CDialogEx
{
private:
    CString m_strInput;
    int m_nChoice;

public:
    void SetData(const CString& str, int choice);
    void GetData(CString& str, int& choice);
};

// ChildDialog.cpp
void CChildDialog::SetData(const CString& str, int choice)
{
    m_strInput = str;
    m_nChoice = choice;
}

void CChildDialog::GetData(CString& str, int& choice)
{
    str = m_strInput;
    choice = m_nChoice;
}

// ParentDialog.cpp
void CParentDialog::OnButtonShowChild()
{
    CChildDialog dlg;
    dlg.SetData(m_strData, m_nCurrentChoice);

    if (dlg.DoModal() == IDOK)
    {
        dlg.GetData(m_strData, m_nCurrentChoice);
    }
}
```

**Pros**: Clean interface
**Cons**: Most code

### Method 4: Parent Pointer (For Modeless)

```cpp
// ChildDialog.h
class CChildDialog : public CDialogEx
{
private:
    CParentDialog* m_pParent;

public:
    void SetParent(CParentDialog* pParent) { m_pParent = pParent; }

protected:
    afx_msg void OnBnClickedButtonApply();
};

// ChildDialog.cpp
void CChildDialog::OnBnClickedButtonApply()
{
    // Call method on parent
    if (m_pParent != nullptr)
    {
        CString str;
        GetDlgItemText(IDC_EDIT1, str);
        m_pParent->UpdateData(str);  // Parent must have this method
    }
}

// ParentDialog.cpp
void CParentDialog::OnButtonShowModeless()
{
    if (m_pChildDlg == nullptr)
    {
        m_pChildDlg = new CChildDialog();
        m_pChildDlg->SetParent(this);
        m_pChildDlg->Create(IDD_CHILD_DIALOG, this);
        m_pChildDlg->ShowWindow(SW_SHOW);
    }
}

void CParentDialog::UpdateData(const CString& str)
{
    // Called from child
    m_strData = str;
    SetDlgItemText(IDC_STATIC_RESULT, str);
}
```

---

## 5. Dialog Return Values

### Standard Return Values

```cpp
IDOK          = 1      // OK button
IDCANCEL      = 2      // Cancel button
IDABORT       = 3      // Abort button
IDRETRY       = 4      // Retry button
IDIGNORE      = 5      // Ignore button
IDYES         = 6      // Yes button
IDNO          = 7      // No button
```

### Setting Return Value

```cpp
void CMyDialog::OnBnClickedButtonSave()
{
    // Close dialog with custom return value
    EndDialog(IDOK);
}

void CMyDialog::OnBnClickedButtonSaveAs()
{
    // Custom return value
    EndDialog(100);  // Any value you want
}
```

### Using Custom Return Values

```cpp
// Define custom values
#define ID_SAVE_AS      100
#define ID_SAVE_COPY    101
#define ID_EXPORT       102

// In dialog
void CMyDialog::OnBnClickedButtonSaveAs()
{
    EndDialog(ID_SAVE_AS);
}

// In parent
void CParentDialog::OnButtonShowDialog()
{
    CMyDialog dlg;
    INT_PTR result = dlg.DoModal();

    switch (result)
    {
    case IDOK:
        // Save normally
        break;

    case ID_SAVE_AS:
        // Show Save As dialog
        break;

    case ID_SAVE_COPY:
        // Save copy
        break;

    case IDCANCEL:
        // User cancelled
        break;
    }
}
```

---

## 6. Common Dialog Properties

### Making Dialogs Resizable

```cpp
BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Add thick frame (resizable border)
    ModifyStyle(0, WS_THICKFRAME);

    // Store initial size
    GetClientRect(&m_rectInitial);

    return TRUE;
}

void CMyDialog::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    // Reposition controls when dialog resizes
    if (m_rectInitial.Width() > 0)  // Check if initialized
    {
        // Resize edit control to fill dialog
        CWnd* pEdit = GetDlgItem(IDC_EDIT1);
        if (pEdit != nullptr && pEdit->GetSafeHwnd() != nullptr)
        {
            pEdit->SetWindowPos(nullptr,
                               10, 10,
                               cx - 20, cy - 50,
                               SWP_NOZORDER);
        }
    }
}
```

### Always on Top

```cpp
BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Make dialog always on top
    SetWindowPos(&wndTopMost, 0, 0, 0, 0,
                SWP_NOMOVE | SWP_NOSIZE);

    return TRUE;
}
```

### Transparent Background

```cpp
BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Set layered window attribute for transparency
    ModifyStyleEx(0, WS_EX_LAYERED);
    SetLayeredWindowAttributes(RGB(255, 0, 255), 0, LWA_COLORKEY);
    // All magenta pixels become transparent

    return TRUE;
}
```

---

## 7. Dynamic Dialog Creation

### Creating Dialogs from Memory Template

```cpp
// Define dialog template in code
class CDynamicDialog : public CDialog
{
public:
    CDynamicDialog(CWnd* pParent = nullptr);

protected:
    virtual BOOL OnInitDialog();
};

BOOL CDynamicDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add controls dynamically
    CButton* pBtn = new CButton();
    pBtn->Create(_T("Click Me"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                CRect(10, 10, 100, 40), this, 100);

    CEdit* pEdit = new CEdit();
    pEdit->Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
                 CRect(10, 50, 200, 70), this, 101);

    return TRUE;
}
```

---

## 8. Subclassing Dialogs

### Why Subclass?

Change behavior of existing dialog template without modifying resources.

```cpp
// Subclass common file dialog
class CMyFileDialog : public CFileDialog
{
public:
    CMyFileDialog(BOOL bOpenFileDialog);

protected:
    virtual void OnInitDone();
    virtual void OnFileNameChange();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMyFileDialog, CFileDialog)
END_MESSAGE_MAP()

CMyFileDialog::CMyFileDialog(BOOL bOpenFileDialog)
    : CFileDialog(bOpenFileDialog)
{
}

void CMyFileDialog::OnInitDone()
{
    // Add custom controls or behavior
    CFileDialog::OnInitDone();
}

void CMyFileDialog::OnFileNameChange()
{
    // React to selection changes
    CString strPath = GetPathName();
    TRACE(_T("Selected: %s\n"), strPath);

    CFileDialog::OnFileNameChange();
}
```

---

## 9. Real-World Dialog Patterns

### Login Dialog

```cpp
class CLoginDialog : public CDialogEx
{
private:
    CString m_strUsername;
    CString m_strPassword;
    int m_nAttempts;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK();

public:
    CString GetUsername() const { return m_strUsername; }
};

void CLoginDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUsername);
    DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
}

BOOL CLoginDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_nAttempts = 0;

    // Set password character
    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PASSWORD);
    if (pEdit != nullptr)
    {
        pEdit->SetPasswordChar('*');
    }

    return TRUE;
}

void CLoginDialog::OnOK()
{
    UpdateData(TRUE);

    if (ValidateCredentials(m_strUsername, m_strPassword))
    {
        CDialogEx::OnOK();  // Close with IDOK
    }
    else
    {
        m_nAttempts++;
        if (m_nAttempts >= 3)
        {
            MessageBox(_T("Too many failed attempts!"));
            CDialogEx::OnCancel();  // Close with IDCANCEL
        }
        else
        {
            MessageBox(_T("Invalid credentials!"));
            GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowText(_T(""));
            GetDlgItem(IDC_EDIT_USERNAME)->SetFocus();
        }
    }
}
```

### Progress Dialog

```cpp
class CProgressDialog : public CDialogEx
{
private:
    CProgressCtrl m_progressCtrl;
    int m_nCurrent;
    int m_nTotal;
    BOOL m_bCancelled;

public:
    void SetRange(int nTotal);
    void SetProgress(int nCurrent);
    BOOL IsCancelled() const { return m_bCancelled; }

protected:
    virtual void OnCancel();
};

void CProgressDialog::SetRange(int nTotal)
{
    m_nTotal = nTotal;
    m_progressCtrl.SetRange(0, m_nTotal);
}

void CProgressDialog::SetProgress(int nCurrent)
{
    m_nCurrent = nCurrent;
    m_progressCtrl.SetPos(m_nCurrent);

    CString str;
    str.Format(_T("Processing %d of %d..."), m_nCurrent, m_nTotal);
    SetDlgItemText(IDC_STATIC_STATUS, str);
}

void CProgressDialog::OnCancel()
{
    m_bCancelled = TRUE;
    // Don't close dialog - let worker thread finish
}
```

---

## Summary

### Key Takeaways

1. **Modal dialogs** (DoModal) block parent, modeless dialogs (Create) don't
2. **Dialog lifecycle**: Constructor → OnInitDialog → Active → OnOK/OnCancel → Destructor
3. **OnInitDialog**: Initialize controls (controls exist now)
4. **OnOK**: Validate before closing
5. **Data passing**: Public members, constructors, or methods
6. **Modeless dialogs**: Heap allocation, manual lifetime management
7. **Return values**: IDOK, IDCANCEL, or custom values

### Next Steps

In **Lesson 07: MFC Controls Part 1 (Buttons, Edit, Static)**, you'll learn:
- Button control types and styles
- Edit control features
- Static text and images
- Control notifications
- Grouping controls

---

**Next Lesson**: [Lesson 07: MFC Controls Part 1 (Buttons, Edit, Static)](Lesson-07-MFC-Controls-Part1-Buttons-Edit-Static.md)
