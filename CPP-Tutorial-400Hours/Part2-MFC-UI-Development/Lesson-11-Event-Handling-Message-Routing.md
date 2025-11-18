# Lesson 11: Event Handling and Message Routing

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding Message Routing
2. Command Routing Architecture
3. Update UI Mechanism
4. Handling Control Notifications
5. Custom Message Handling
6. Message Reflection
7. Message Filtering

---

## 1. Understanding Message Routing

### Message Types

```cpp
// Windows Messages (WM_xxx)
WM_CREATE, WM_PAINT, WM_LBUTTONDOWN, etc.
→ Handled by ON_WM_xxx macros

// Command Messages (from menus/toolbars)
ID_FILE_OPEN, ID_EDIT_COPY, etc.
→ Handled by ON_COMMAND

// Control Notifications (from child controls)
BN_CLICKED, EN_CHANGE, LBN_SELCHANGE, etc.
→ Handled by ON_BN_CLICKED, ON_EN_CHANGE, etc.

// Custom Messages (user-defined)
WM_USER + xxx, WM_APP + xxx
→ Handled by ON_MESSAGE
```

### Routing Order (Dialog-Based Apps)

```
User clicks button
    ↓
Button sends WM_COMMAND to parent dialog
    ↓
Dialog message map checked
    ↓
Found → Handler called
Not found → Default processing
```

### Routing Order (SDI/MDI Apps)

```
User selects menu item or toolbar button (ID_FILE_OPEN)
    ↓
1. Active view (CView-derived)
   ON_COMMAND(ID_FILE_OPEN, ...)? → Call handler
    ↓ Not found
2. Document (CDocument-derived)
   ON_COMMAND(ID_FILE_OPEN, ...)? → Call handler
    ↓ Not found
3. Frame window (CFrameWnd-derived)
   ON_COMMAND(ID_FILE_OPEN, ...)? → Call handler
    ↓ Not found
4. Application (CWinApp-derived)
   ON_COMMAND(ID_FILE_OPEN, ...)? → Call handler
    ↓ Not found
5. Default processing
```

**Example**:
```cpp
// In CMyView
BEGIN_MESSAGE_MAP(CMyView, CView)
    ON_COMMAND(ID_FILE_SAVE, &CMyView::OnFileSave)
END_MESSAGE_MAP()

void CMyView::OnFileSave()
{
    // This runs when File → Save clicked
    // Even though menu is in frame window!
}
```

---

## 2. Command Routing Architecture

### ON_COMMAND

```cpp
// Basic command handling
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

### ON_COMMAND_RANGE

```cpp
// Handle range of commands
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_COMMAND_RANGE(ID_COLOR_RED, ID_COLOR_BLUE, &CMainFrame::OnColorCommand)
END_MESSAGE_MAP()

void CMainFrame::OnColorCommand(UINT nID)
{
    switch (nID)
    {
    case ID_COLOR_RED:
        SetColor(RGB(255, 0, 0));
        break;
    case ID_COLOR_GREEN:
        SetColor(RGB(0, 255, 0));
        break;
    case ID_COLOR_BLUE:
        SetColor(RGB(0, 0, 255));
        break;
    }
}
```

---

## 3. Update UI Mechanism

### What is Update UI?

Update UI handlers run automatically to:
- Enable/disable menu items
- Check/uncheck menu items
- Update toolbar button states

**When they run**:
- Before displaying a menu
- Periodically for toolbars (when idle)

### ON_UPDATE_COMMAND_UI

```cpp
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_COMMAND(ID_EDIT_PASTE, &CMainFrame::OnEditPaste)
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CMainFrame::OnUpdateEditPaste)
END_MESSAGE_MAP()

void CMainFrame::OnEditPaste()
{
    // Paste data
}

void CMainFrame::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
    // Enable Paste only if clipboard has data
    BOOL bHasData = IsClipboardFormatAvailable(CF_TEXT);
    pCmdUI->Enable(bHasData);
}
```

### CCmdUI Methods

```cpp
void OnUpdateCommand(CCmdUI* pCmdUI)
{
    // Enable/disable
    pCmdUI->Enable(TRUE);   // Enable
    pCmdUI->Enable(FALSE);  // Disable (gray out)

    // Check/uncheck
    pCmdUI->SetCheck(TRUE);   // Check
    pCmdUI->SetCheck(FALSE);  // Uncheck
    pCmdUI->SetCheck(2);      // Indeterminate

    // Set text
    pCmdUI->SetText(_T("New Text"));

    // Set radio button
    pCmdUI->SetRadio(TRUE);   // Filled dot
    pCmdUI->SetRadio(FALSE);  // Empty circle
}
```

### Complete Example

```cpp
class CMainFrame : public CFrameWnd
{
private:
    BOOL m_bBold;
    BOOL m_bItalic;
    BOOL m_bUnderline;

protected:
    afx_msg void OnFormatBold();
    afx_msg void OnFormatItalic();
    afx_msg void OnFormatUnderline();
    afx_msg void OnUpdateFormatBold(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFormatItalic(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFormatUnderline(CCmdUI* pCmdUI);

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_COMMAND(ID_FORMAT_BOLD, &CMainFrame::OnFormatBold)
    ON_COMMAND(ID_FORMAT_ITALIC, &CMainFrame::OnFormatItalic)
    ON_COMMAND(ID_FORMAT_UNDERLINE, &CMainFrame::OnFormatUnderline)
    ON_UPDATE_COMMAND_UI(ID_FORMAT_BOLD, &CMainFrame::OnUpdateFormatBold)
    ON_UPDATE_COMMAND_UI(ID_FORMAT_ITALIC, &CMainFrame::OnUpdateFormatItalic)
    ON_UPDATE_COMMAND_UI(ID_FORMAT_UNDERLINE, &CMainFrame::OnUpdateFormatUnderline)
END_MESSAGE_MAP()

void CMainFrame::OnFormatBold()
{
    m_bBold = !m_bBold;
    UpdateFormatting();
}

void CMainFrame::OnUpdateFormatBold(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_bBold);  // Show check mark if bold active
}

void CMainFrame::OnFormatItalic()
{
    m_bItalic = !m_bItalic;
    UpdateFormatting();
}

void CMainFrame::OnUpdateFormatItalic(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_bItalic);
}
```

---

## 4. Handling Control Notifications

### Button Notifications

```cpp
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog::OnBnClickedButton1)
    ON_BN_DOUBLECLICKED(IDC_BUTTON1, &CMyDialog::OnBnDoubleclickedButton1)
END_MESSAGE_MAP()

void CMyDialog::OnBnClickedButton1()
{
    MessageBox(_T("Button clicked"));
}
```

### Edit Control Notifications

```cpp
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_EN_CHANGE(IDC_EDIT_NAME, &CMyDialog::OnEnChangeEditName)
    ON_EN_SETFOCUS(IDC_EDIT_NAME, &CMyDialog::OnEnSetfocusEditName)
    ON_EN_KILLFOCUS(IDC_EDIT_NAME, &CMyDialog::OnEnKillfocusEditName)
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
```

---

## 5. Custom Message Handling

### Defining Custom Messages

```cpp
// In header file
#define WM_MY_CUSTOM_MESSAGE (WM_USER + 100)
// or
#define WM_MY_APP_MESSAGE (WM_APP + 1)
```

### Handling Custom Messages

```cpp
// In .h
class CMyDialog : public CDialogEx
{
protected:
    afx_msg LRESULT OnMyCustomMessage(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};

// In .cpp
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_MESSAGE(WM_MY_CUSTOM_MESSAGE, &CMyDialog::OnMyCustomMessage)
END_MESSAGE_MAP()

LRESULT CMyDialog::OnMyCustomMessage(WPARAM wParam, LPARAM lParam)
{
    int value = (int)wParam;
    CString* pStr = (CString*)lParam;

    // Process message
    MessageBox(*pStr);

    delete pStr;  // Clean up if allocated

    return 0;
}
```

### Sending Custom Messages

```cpp
// From same thread
SendMessage(WM_MY_CUSTOM_MESSAGE, 100, (LPARAM)_T("Hello"));

// From different thread (async)
PostMessage(WM_MY_CUSTOM_MESSAGE, 100, (LPARAM)new CString(_T("Hello")));
```

---

## Summary

### Key Takeaways

1. **Message routing**: View → Document → Frame → App
2. **ON_COMMAND**: Handle menu/toolbar commands
3. **ON_UPDATE_COMMAND_UI**: Enable/disable, check/uncheck
4. **Control notifications**: ON_BN_CLICKED, ON_EN_CHANGE
5. **Custom messages**: WM_USER + xxx or WM_APP + xxx

### Next Steps

In **Lesson 12: Common Dialogs**, you'll learn:
- File Open/Save dialogs
- Color picker dialog
- Font picker dialog
- Print dialogs

---

**Next Lesson**: [Lesson 12: Common Dialogs (File, Color, Font)](Lesson-12-Common-Dialogs.md)
