# Lesson 07: MFC Controls Part 1 (Buttons, Edit, Static)

**Duration**: 8 hours
**Difficulty**: Intermediate

## Table of Contents
1. Button Controls (CButton)
2. Edit Controls (CEdit)
3. Static Controls (CStatic)
4. Control Common Properties
5. Control Notifications
6. Dynamic Control Creation
7. Subclassing Controls
8. Complete Examples

---

## 1. Button Controls (CButton)

### Button Types and Styles

```cpp
// Standard push button
BS_PUSHBUTTON         // Normal button
BS_DEFPUSHBUTTON      // Default button (bold outline)

// Check boxes
BS_CHECKBOX           // Standard checkbox
BS_AUTOCHECKBOX       // Auto-toggles when clicked (most common)
BS_3STATE             // Three states: checked, unchecked, indeterminate
BS_AUTO3STATE         // Auto-toggles through 3 states

// Radio buttons
BS_RADIOBUTTON        // Manual radio button
BS_AUTORADIOBUTTON    // Auto radio (most common)

// Group box
BS_GROUPBOX           // Visual grouping (non-clickable)

// Owner-drawn
BS_OWNERDRAW          // Custom drawing
```

### Creating Buttons in Resource Editor

```
1. Toolbox → Button
2. Click on dialog to place
3. Properties:
    ID:         IDC_BUTTON_SAVE
    Caption:    &Save
    Style:      Push Button (or Auto Check Box, etc.)
    Default:    True (for default button)
```

### Button Properties

```cpp
// In Resource Editor Properties:
ID:              IDC_BUTTON1          // Control ID
Caption:         &Click Me            // & creates Alt+C shortcut
Default button:  True/False           // Bold outline, Enter key
Tab stop:        True                 // Can tab to it
Visible:         True                 // Initially visible
Disabled:        False                // Initially enabled
```

### Accessing Buttons in Code

```cpp
// Method 1: Direct access via ID
void CMyDialog::OnBnClickedButtonTest()
{
    // Enable/disable
    GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);  // Disable
    GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);   // Enable

    // Show/hide
    GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);  // Hide
    GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);  // Show

    // Change text
    SetDlgItemText(IDC_BUTTON1, _T("New Caption"));
}

// Method 2: CButton pointer
void CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CButton* pBtn = (CButton*)GetDlgItem(IDC_BUTTON1);
    if (pBtn != nullptr)
    {
        pBtn->SetWindowText(_T("Click Me"));
        pBtn->EnableWindow(TRUE);
    }

    return TRUE;
}
```

### Check Box Operations

```cpp
// In dialog class (.h)
protected:
    BOOL m_bAcceptTerms;  // TRUE = checked, FALSE = unchecked

// In DoDataExchange
void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_CHECK_ACCEPT, m_bAcceptTerms);
}

// Using directly
void CMyDialog::OnButtonSubmit()
{
    CButton* pCheck = (CButton*)GetDlgItem(IDC_CHECK_ACCEPT);

    // Get state
    int state = pCheck->GetCheck();
    // BST_UNCHECKED (0), BST_CHECKED (1), BST_INDETERMINATE (2)

    if (state == BST_CHECKED)
    {
        MessageBox(_T("Terms accepted"));
    }
    else
    {
        MessageBox(_T("Please accept terms"));
    }

    // Set state
    pCheck->SetCheck(BST_CHECKED);    // Check it
    pCheck->SetCheck(BST_UNCHECKED);  // Uncheck it
}
```

### Radio Buttons

```cpp
// Radio buttons must be in a GROUP
// In Resource Editor:
// Radio 1: Group = True, Auto Radio Button
// Radio 2: Group = False, Auto Radio Button
// Radio 3: Group = False, Auto Radio Button
// (Next control): Group = True  ← Ends the radio group

// In code - using DDX
protected:
    int m_nColorChoice;  // 0 = first radio, 1 = second, 2 = third

void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Radio(pDX, IDC_RADIO_RED, m_nColorChoice);
    //             ↑ ID of FIRST radio in group
}

// Manually checking/unchecking
void CMyDialog::OnButtonSelectBlue()
{
    // Check specific radio button
    CheckRadioButton(IDC_RADIO_RED,    // First in group
                    IDC_RADIO_BLUE,    // Last in group
                    IDC_RADIO_BLUE);   // The one to check
}

// Getting which radio is selected
int CMyDialog::GetSelectedColor()
{
    if (IsDlgButtonChecked(IDC_RADIO_RED))
        return 0;  // Red
    else if (IsDlgButtonChecked(IDC_RADIO_GREEN))
        return 1;  // Green
    else if (IsDlgButtonChecked(IDC_RADIO_BLUE))
        return 2;  // Blue

    return -1;  // None selected
}
```

### Button Notifications

```cpp
// Message map
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog::OnBnClickedButton1)
    ON_BN_DOUBLECLICKED(IDC_BUTTON1, &CMyDialog::OnBnDoubleclickedButton1)
    ON_BN_SETFOCUS(IDC_BUTTON1, &CMyDialog::OnBnSetfocusButton1)
    ON_BN_KILLFOCUS(IDC_BUTTON1, &CMyDialog::OnBnKillfocusButton1)
END_MESSAGE_MAP()

void CMyDialog::OnBnClickedButton1()
{
    MessageBox(_T("Button clicked"));
}

void CMyDialog::OnBnDoubleclickedButton1()
{
    MessageBox(_T("Button double-clicked"));
}
```

---

## 2. Edit Controls (CEdit)

### Edit Control Styles

```cpp
// Single-line edit
ES_LEFT               // Left-aligned text
ES_CENTER             // Center-aligned
ES_RIGHT              // Right-aligned
ES_AUTOHSCROLL        // Auto-scroll horizontally (common)
ES_PASSWORD           // Password field (*** display)
ES_READONLY           // Read-only
ES_UPPERCASE          // Force uppercase
ES_LOWERCASE          // Force lowercase
ES_NUMBER             // Numbers only

// Multi-line edit
ES_MULTILINE          // Multiple lines
ES_AUTOVSCROLL        // Auto-scroll vertically
ES_WANTRETURN         // Enter key creates new line (vs closing dialog)
```

### Creating Edit Controls

```
Resource Editor:
1. Toolbox → Edit Control
2. Properties:
    ID:              IDC_EDIT_NAME
    Multiline:       False (single line) or True (multiple lines)
    Auto HScroll:    True (allows horizontal scrolling)
    Want return:     True (for multiline - Enter creates new line)
    Read-only:       False
    Password:        False
```

### Working with Edit Controls

```cpp
// Getting text
CString str;
GetDlgItemText(IDC_EDIT_NAME, str);

// Setting text
SetDlgItemText(IDC_EDIT_NAME, _T("Hello World"));

// Using CEdit pointer
CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NAME);
if (pEdit != nullptr)
{
    // Get text
    CString str;
    pEdit->GetWindowText(str);

    // Set text
    pEdit->SetWindowText(_T("New text"));

    // Select all text
    pEdit->SetSel(0, -1);

    // Replace selection
    pEdit->ReplaceSel(_T("Replacement"));

    // Get line count (multiline)
    int nLines = pEdit->GetLineCount();

    // Get specific line
    CString strLine;
    int nLength = pEdit->LineLength(0);  // Line 0
    pEdit->GetLine(0, strLine.GetBuffer(nLength), nLength);
    strLine.ReleaseBuffer(nLength);

    // Set read-only
    pEdit->SetReadOnly(TRUE);

    // Set password character
    pEdit->SetPasswordChar('*');

    // Limit text length
    pEdit->SetLimitText(50);  // Max 50 characters

    // Scroll to end
    pEdit->LineScroll(pEdit->GetLineCount());
}
```

### Edit Control with DDX

```cpp
// In .h file
protected:
    CString m_strName;
    int m_nAge;
    double m_dSalary;

// In DoDataExchange
void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
    DDX_Text(pDX, IDC_EDIT_AGE, m_nAge);
    DDX_Text(pDX, IDC_EDIT_SALARY, m_dSalary);

    // With validation (DDV)
    DDV_MinMaxInt(pDX, m_nAge, 0, 120);
    DDV_MinMaxDouble(pDX, m_dSalary, 0.0, 1000000.0);
}

// Using the variables
void CMyDialog::OnBnClickedButtonSave()
{
    if (!UpdateData(TRUE))  // Control → Variables
    {
        return;  // Validation failed
    }

    // Now use m_strName, m_nAge, m_dSalary
    SaveToDatabase(m_strName, m_nAge, m_dSalary);
}
```

### Multiline Edit Control

```cpp
BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Configure multiline edit
    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MULTILINE);
    if (pEdit != nullptr)
    {
        // Load text from file
        CString strText = LoadTextFile(_T("notes.txt"));
        pEdit->SetWindowText(strText);

        // Scroll to top
        pEdit->LineScroll(-pEdit->GetFirstVisibleLine());
    }

    return TRUE;
}

void CMyDialog::OnBnClickedButtonSave()
{
    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MULTILINE);
    if (pEdit != nullptr)
    {
        CString strText;
        pEdit->GetWindowText(strText);

        // Save to file
        SaveTextFile(_T("notes.txt"), strText);
    }
}

// Helper functions
CString CMyDialog::LoadTextFile(LPCTSTR lpszPath)
{
    CStdioFile file;
    if (file.Open(lpszPath, CFile::modeRead | CFile::typeText))
    {
        CString strLine, strAll;
        while (file.ReadString(strLine))
        {
            strAll += strLine + _T("\r\n");
        }
        file.Close();
        return strAll;
    }
    return _T("");
}

void CMyDialog::SaveTextFile(LPCTSTR lpszPath, const CString& strText)
{
    CStdioFile file;
    if (file.Open(lpszPath, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
    {
        file.WriteString(strText);
        file.Close();
    }
}
```

### Edit Control Notifications

```cpp
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_EN_CHANGE(IDC_EDIT_NAME, &CMyDialog::OnEnChangeEditName)
    ON_EN_UPDATE(IDC_EDIT_NAME, &CMyDialog::OnEnUpdateEditName)
    ON_EN_SETFOCUS(IDC_EDIT_NAME, &CMyDialog::OnEnSetfocusEditName)
    ON_EN_KILLFOCUS(IDC_EDIT_NAME, &CMyDialog::OnEnKillfocusEditName)
    ON_EN_MAXTEXT(IDC_EDIT_NAME, &CMyDialog::OnEnMaxtextEditName)
END_MESSAGE_MAP()

void CMyDialog::OnEnChangeEditName()
{
    // Text changed
    CString str;
    GetDlgItemText(IDC_EDIT_NAME, str);

    // Update character count
    CString strCount;
    strCount.Format(_T("Characters: %d"), str.GetLength());
    SetDlgItemText(IDC_STATIC_COUNT, strCount);
}

void CMyDialog::OnEnKillfocusEditName()
{
    // Edit control lost focus - validate
    CString str;
    GetDlgItemText(IDC_EDIT_NAME, str);

    if (str.IsEmpty())
    {
        MessageBox(_T("Name cannot be empty"));
        GetDlgItem(IDC_EDIT_NAME)->SetFocus();
    }
}

void CMyDialog::OnEnMaxtextEditName()
{
    // User tried to type beyond limit
    MessageBox(_T("Maximum length reached!"));
}
```

---

## 3. Static Controls (CStatic)

### Static Control Types

```cpp
SS_LEFT               // Left-aligned text
SS_CENTER             // Center-aligned text
SS_RIGHT              // Right-aligned text
SS_ICON               // Display icon
SS_BITMAP             // Display bitmap
SS_OWNERDRAW          // Custom drawing
SS_SIMPLE             // Simple text (faster)
SS_LEFTNOWORDWRAP     // No word wrap
SS_NOTIFY             // Sends click notifications
SS_SUNKEN             // Sunken border
```

### Creating Static Controls

```
Resource Editor:
1. Toolbox → Static Text
2. Properties:
    ID:         IDC_STATIC_RESULT  (change from IDC_STATIC!)
    Caption:    "Result:"
    Align:      Left/Center/Right
    Border:     None/Simple/Sunken
    Notify:     True (if you want click events)
```

**IMPORTANT**: Default static controls have ID `IDC_STATIC` (-1). To access in code, you MUST change the ID!

### Displaying Text

```cpp
// Set static text
SetDlgItemText(IDC_STATIC_RESULT, _T("Hello"));

// Format numbers
CString str;
str.Format(_T("Count: %d"), m_nCount);
SetDlgItemText(IDC_STATIC_COUNT, str);

// Using CStatic pointer
CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_RESULT);
if (pStatic != nullptr)
{
    pStatic->SetWindowText(_T("Result: Success"));

    // Change color (requires subclassing)
    // See Subclassing section
}
```

### Displaying Images

```cpp
// Load and display icon
BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Load icon from resources
    HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);

    // Display in static control
    CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_ICON);
    if (pStatic != nullptr)
    {
        pStatic->SetIcon(hIcon);
    }

    return TRUE;
}

// Load and display bitmap
BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Load bitmap
    HBITMAP hBitmap = (HBITMAP)LoadImage(
        AfxGetInstanceHandle(),
        MAKEINTRESOURCE(IDB_BITMAP1),
        IMAGE_BITMAP,
        0, 0,
        LR_DEFAULTCOLOR);

    // Display in static control
    CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_IMAGE);
    if (pStatic != nullptr)
    {
        pStatic->SetBitmap(hBitmap);
    }

    return TRUE;
}
```

### Group Boxes

```cpp
// Group boxes visually group controls
// They don't DO anything functionally

// Create in Resource Editor:
// Toolbox → Group Box
// Properties:
//    Caption: "Options"
//    Style: Group Box

// Place other controls inside the group box visually
// (But they're not hierarchically "inside" - just positioned there)
```

---

## 4. Control Common Properties

### Enabling/Disabling Controls

```cpp
// Disable control
GetDlgItem(IDC_EDIT_NAME)->EnableWindow(FALSE);

// Enable control
GetDlgItem(IDC_EDIT_NAME)->EnableWindow(TRUE);

// Check if enabled
BOOL bEnabled = GetDlgItem(IDC_EDIT_NAME)->IsWindowEnabled();
```

### Showing/Hiding Controls

```cpp
// Hide control
GetDlgItem(IDC_BUTTON_ADVANCED)->ShowWindow(SW_HIDE);

// Show control
GetDlgItem(IDC_BUTTON_ADVANCED)->ShowWindow(SW_SHOW);

// Check if visible
BOOL bVisible = GetDlgItem(IDC_BUTTON_ADVANCED)->IsWindowVisible();
```

### Moving and Resizing Controls

```cpp
void CMyDialog::OnButtonResize()
{
    CWnd* pEdit = GetDlgItem(IDC_EDIT1);
    if (pEdit != nullptr)
    {
        // Method 1: SetWindowPos
        pEdit->SetWindowPos(nullptr,
                           10, 10,      // X, Y position
                           300, 100,    // Width, Height
                           SWP_NOZORDER);  // Don't change Z-order

        // Method 2: MoveWindow
        pEdit->MoveWindow(10, 10, 300, 100);

        // Get current position
        CRect rect;
        pEdit->GetWindowRect(&rect);  // Screen coordinates
        ScreenToClient(&rect);         // Convert to dialog coordinates

        // Resize proportionally
        rect.right = rect.left + (rect.Width() * 2);
        pEdit->MoveWindow(&rect);
    }
}
```

### Setting Focus

```cpp
void CMyDialog::OnButtonFocusName()
{
    CWnd* pEdit = GetDlgItem(IDC_EDIT_NAME);
    if (pEdit != nullptr)
    {
        pEdit->SetFocus();

        // Also select all text
        ((CEdit*)pEdit)->SetSel(0, -1);
    }
}

// In OnInitDialog - set initial focus
BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Your initialization...

    GetDlgItem(IDC_EDIT_NAME)->SetFocus();

    return FALSE;  // We set focus, so return FALSE
}
```

---

## 5. Control Notifications

### How Notifications Work

```
User clicks button
    ↓
Button sends WM_COMMAND to parent dialog
    ↓
Message map checks for ON_BN_CLICKED entry
    ↓
Calls your handler function
```

### Adding Notification Handlers

**Method 1: Class Wizard (Recommended)**
```
1. Double-click control in Resource Editor
2. Select notification (e.g., BN_CLICKED)
3. Click Add Handler
```

**Method 2: Manually**
```cpp
// In .h
protected:
    afx_msg void OnBnClickedButton1();
    DECLARE_MESSAGE_MAP()

// In .cpp
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog::OnBnClickedButton1)
END_MESSAGE_MAP()

void CMyDialog::OnBnClickedButton1()
{
    // Handler code
}
```

---

## 6. Dynamic Control Creation

### Creating Controls at Runtime

```cpp
class CMyDialog : public CDialogEx
{
protected:
    CButton m_btnDynamic;    // Member variable for control
    CEdit m_editDynamic;
    CStatic m_staticDynamic;
};

BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Create button dynamically
    m_btnDynamic.Create(_T("Dynamic Button"),
                       WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                       CRect(10, 10, 120, 40),
                       this,
                       IDC_BUTTON_DYNAMIC);

    // Create edit control
    m_editDynamic.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                        CRect(10, 50, 200, 70),
                        this,
                        IDC_EDIT_DYNAMIC);

    // Create static text
    m_staticDynamic.Create(_T("Dynamic Label"),
                          WS_CHILD | WS_VISIBLE | SS_LEFT,
                          CRect(10, 80, 200, 100),
                          this,
                          IDC_STATIC_DYNAMIC);

    return TRUE;
}
```

### Dynamic Control Arrays

```cpp
class CMyDialog : public CDialogEx
{
protected:
    CButton m_buttons[10];   // Array of buttons
};

BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Create 10 buttons in a grid
    for (int i = 0; i < 10; i++)
    {
        int x = (i % 5) * 60 + 10;   // 5 columns
        int y = (i / 5) * 30 + 10;   // 2 rows

        CString strCaption;
        strCaption.Format(_T("Button %d"), i + 1);

        m_buttons[i].Create(strCaption,
                           WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                           CRect(x, y, x + 50, y + 25),
                           this,
                           IDC_BUTTON_START + i);
    }

    return TRUE;
}
```

---

## 7. Subclassing Controls

### Why Subclass?

Change control behavior:
- Custom drawing
- Additional validation
- Special key handling
- Color changes

### Example: Custom Edit Control

```cpp
// CustomEdit.h
class CCustomEdit : public CEdit
{
public:
    void SetTextColor(COLORREF color);
    void SetBackgroundColor(COLORREF color);

protected:
    COLORREF m_clrText;
    COLORREF m_clrBackground;
    CBrush m_brushBackground;

    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

    DECLARE_MESSAGE_MAP()
};

// CustomEdit.cpp
BEGIN_MESSAGE_MAP(CCustomEdit, CEdit)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_CHAR()
END_MESSAGE_MAP()

CCustomEdit::CCustomEdit()
    : m_clrText(RGB(0, 0, 0))
    , m_clrBackground(RGB(255, 255, 255))
{
    m_brushBackground.CreateSolidBrush(m_clrBackground);
}

HBRUSH CCustomEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
    pDC->SetTextColor(m_clrText);
    pDC->SetBkColor(m_clrBackground);
    return m_brushBackground;
}

void CCustomEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // Only allow digits and backspace
    if (!isdigit(nChar) && nChar != VK_BACK)
    {
        MessageBeep(0);
        return;  // Don't call base class - character ignored
    }

    CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CCustomEdit::SetTextColor(COLORREF color)
{
    m_clrText = color;
    Invalidate();
}

void CCustomEdit::SetBackgroundColor(COLORREF color)
{
    m_clrBackground = color;
    m_brushBackground.DeleteObject();
    m_brushBackground.CreateSolidBrush(color);
    Invalidate();
}

// Using in dialog
class CMyDialog : public CDialogEx
{
protected:
    CCustomEdit m_editCustom;
};

BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Subclass existing control
    m_editCustom.SubclassDlgItem(IDC_EDIT_CUSTOM, this);

    // Set colors
    m_editCustom.SetTextColor(RGB(0, 0, 255));  // Blue text
    m_editCustom.SetBackgroundColor(RGB(255, 255, 0));  // Yellow background

    return TRUE;
}
```

---

## 8. Complete Examples

### Example: Login Form

```cpp
class CLoginDialog : public CDialogEx
{
protected:
    CString m_strUsername;
    CString m_strPassword;

    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK();

    afx_msg void OnEnChangeEditUsername();
    afx_msg void OnEnChangeEditPassword();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CLoginDialog, CDialogEx)
    ON_EN_CHANGE(IDC_EDIT_USERNAME, &CLoginDialog::OnEnChangeEditUsername)
    ON_EN_CHANGE(IDC_EDIT_PASSWORD, &CLoginDialog::OnEnChangeEditPassword)
END_MESSAGE_MAP()

void CLoginDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUsername);
    DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
}

BOOL CLoginDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Set password character
    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PASSWORD);
    if (pEdit != nullptr)
    {
        pEdit->SetPasswordChar('*');
    }

    // Disable OK button initially
    GetDlgItem(IDOK)->EnableWindow(FALSE);

    return TRUE;
}

void CLoginDialog::OnEnChangeEditUsername()
{
    ValidateForm();
}

void CLoginDialog::OnEnChangeEditPassword()
{
    ValidateForm();
}

void CLoginDialog::ValidateForm()
{
    CString strUser, strPass;
    GetDlgItemText(IDC_EDIT_USERNAME, strUser);
    GetDlgItemText(IDC_EDIT_PASSWORD, strPass);

    // Enable OK only if both filled
    BOOL bEnable = !strUser.IsEmpty() && !strPass.IsEmpty();
    GetDlgItem(IDOK)->EnableWindow(bEnable);
}

void CLoginDialog::OnOK()
{
    UpdateData(TRUE);

    if (Authenticate(m_strUsername, m_strPassword))
    {
        CDialogEx::OnOK();
    }
    else
    {
        MessageBox(_T("Invalid credentials!"));
        SetDlgItemText(IDC_EDIT_PASSWORD, _T(""));
        GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
    }
}
```

---

## Summary

### Key Takeaways

1. **Buttons**: Push button, checkbox, radio button, group box
2. **Edit controls**: Single/multiline, password, numeric, validation
3. **Static controls**: Text, images, group boxes
4. **Notifications**: ON_BN_CLICKED, ON_EN_CHANGE, etc.
5. **DDX**: Automatic variable ↔ control synchronization
6. **Dynamic creation**: Create controls at runtime
7. **Subclassing**: Customize control behavior

### Next Steps

In **Lesson 08: MFC Controls Part 2**, you'll learn:
- List boxes and combo boxes
- List controls (report view)
- Tree controls
- Spin buttons and sliders
- Date/time pickers

---

**Next Lesson**: [Lesson 08: MFC Controls Part 2 (List, Combo, Check, Radio)](Lesson-08-MFC-Controls-Part2-List-Combo.md)
