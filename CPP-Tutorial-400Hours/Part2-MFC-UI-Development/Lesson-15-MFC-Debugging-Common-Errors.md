# Lesson 15: MFC Debugging and Common Errors

**Duration**: 6-8 hours
**Difficulty**: Intermediate to Advanced

## Table of Contents
1. Debugging Tools and Techniques
2. Common MFC Errors and Solutions
3. Memory Leak Detection
4. Performance Profiling
5. Best Practices
6. Troubleshooting Guide

---

## 1. Debugging Tools and Techniques

### Using Breakpoints

```cpp
void CMyDialog::OnBnClickedButtonTest()
{
    CString strName;
    GetDlgItemText(IDC_EDIT_NAME, strName);  // ← Set breakpoint here (F9)

    // Execution stops, inspect variables:
    // - Hover over strName
    // - Watch window: Add 'strName'
    // - Locals window: See all local variables

    ProcessName(strName);
}
```

**Breakpoint Types**:
- **Regular breakpoint** (F9): Stops every time
- **Conditional breakpoint**: Right-click breakpoint → Condition
  ```
  Condition: strName == "Debug"
  Hit count: Break when hit count is equal to 5
  ```

### TRACE Macro

```cpp
void CMyDialog::OnBnClickedButton()
{
    TRACE(_T("Button clicked\n"));

    CString strName;
    GetDlgItemText(IDC_EDIT_NAME, strName);

    TRACE(_T("Name: %s\n"), (LPCTSTR)strName);

    int nCount = 42;
    TRACE(_T("Count: %d\n"), nCount);
}

// View output: Debug → Windows → Output (Ctrl+Alt+O)
// Output only appears in DEBUG builds, not RELEASE
```

### ASSERT Macro

```cpp
void CMyDialog::ProcessData(CDataObject* pData)
{
    ASSERT(pData != nullptr);  // ← Breaks in debug if pData is NULL
    ASSERT_VALID(pData);       // ← Calls pData->AssertValid()

    // Use the data
    pData->DoSomething();
}

// In your class
class CDataObject : public CObject
{
public:
#ifdef _DEBUG
    virtual void AssertValid() const
    {
        CObject::AssertValid();

        ASSERT(m_nCount >= 0);
        ASSERT(!m_strName.IsEmpty());
    }
#endif
};
```

### Watch Window

```cpp
void CMyDialog::OnButtonTest()
{
    CString str1 = _T("Hello");
    CString str2 = _T("World");
    int nValue = 42;

    // Set breakpoint here
    // In Watch window, add:
    //   str1
    //   str2
    //   nValue
    //   this  (see all member variables)
}
```

### Call Stack

```
When stopped at breakpoint:
Debug → Windows → Call Stack (Alt+7)

Shows:
MyApp.exe!CMyDialog::OnBnClickedButton() Line 45
MyApp.exe!_AfxDispatchCmdMsg(...) Line 88
MyApp.exe!CCmdTarget::OnCmdMsg(...) Line 403
MyApp.exe!CWnd::OnCommand(...) Line 2896
...
```

**Use Call Stack to**:
- See how you got to current function
- Double-click to jump to calling function
- Understand execution flow

---

## 2. Common MFC Errors and Solutions

### Error: "Cannot open include file: 'afxwin.h'"

**CAUSE**: MFC not installed

**SOLUTION**:
```
1. Close Visual Studio
2. Run Visual Studio Installer
3. Modify installation
4. Select "Desktop development with C++"
5. Under Individual Components, check "MFC and ATL support"
6. Install
```

### Error: "pch.h not found" or "stdafx.h not found"

**CAUSE**: Precompiled header not included first

**WRONG**:
```cpp
#include "MyDialog.h"
#include "pch.h"  // ← Too late!
```

**CORRECT**:
```cpp
#include "pch.h"  // ← MUST be first
#include "MyDialog.h"
```

### Error: "DECLARE_MESSAGE_MAP: undeclared identifier"

**CAUSE**: Forgot to include base class header or forgot to derive from MFC class

**WRONG**:
```cpp
class CMyDialog : public CDialog  // ← Forgot to include afxdialogex.h
{
    DECLARE_MESSAGE_MAP()
};
```

**CORRECT**:
```cpp
#include "pch.h"
#include "framework.h"

class CMyDialog : public CDialogEx  // ← Proper base class
{
    DECLARE_MESSAGE_MAP()
};
```

### Error: Handler function not called when button clicked

**CAUSE**: Missing or incorrect message map entry

**CHECK**:
```cpp
// In .h file:
afx_msg void OnBnClickedButton1();  // ← Is this declared?

// In .cpp file:
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog::OnBnClickedButton1)
    // ↑ Is control ID correct?
    // ↑ Is function name correct?
END_MESSAGE_MAP()
```

### Error: Access Violation / Crash when accessing control

**CAUSE**: Accessing control before it's created

**WRONG**:
```cpp
CMyDialog::CMyDialog()
{
    // Controls don't exist yet!
    SetDlgItemText(IDC_EDIT1, _T("Hello"));  // ← CRASH
}
```

**CORRECT**:
```cpp
BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();  // ← Creates controls

    // NOW safe to access
    SetDlgItemText(IDC_EDIT1, _T("Hello"));

    return TRUE;
}
```

### Error: UpdateData(TRUE) returns FALSE

**CAUSE**: DDV validation failed

**DEBUG**:
```cpp
void CMyDialog::OnOK()
{
    if (!UpdateData(TRUE))
    {
        // Set breakpoint here
        // Check DoDataExchange for DDV failures
        return;
    }

    // ...
}

void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    DDX_Text(pDX, IDC_EDIT_AGE, m_nAge);
    DDV_MinMaxInt(pDX, m_nAge, 0, 120);  // ← Validation fails here?
}
```

### Error: Memory Leak Detected

**CAUSE**: Allocated memory not freed

**EXAMPLE**:
```cpp
void CMyDialog::OnButton()
{
    CDialog* pDlg = new CDialog();  // ← Allocated
    pDlg->DoModal();
    // ← Never deleted! MEMORY LEAK
}
```

**SOLUTION**:
```cpp
void CMyDialog::OnButton()
{
    CDialog dlg;  // ← Stack allocation (automatic cleanup)
    dlg.DoModal();

    // OR:
    CDialog* pDlg = new CDialog();
    pDlg->DoModal();
    delete pDlg;  // ← Delete!
}
```

---

## 3. Memory Leak Detection

### Enabling Memory Leak Detection

```cpp
// In application class (.cpp file)
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// This enables MFC memory leak detection automatically
```

### Reading Memory Leak Reports

**Output Window** (on program exit):
```
Detected memory leaks!
Dumping objects ->
{145} normal block at 0x00395F80, 32 bytes long.
 Data: <MyDialog        > 4D 00 79 00 44 00 69 00 61 00 6C 00 6F 00 67 00
Object dump complete.
```

**Information**:
- `{145}` - Allocation number
- `32 bytes long` - Size of leak
- `Data: <MyDialog...>` - Content preview

### Finding Memory Leaks

```cpp
// Set breakpoint on specific allocation
void CMyApp::InitInstance()
{
    #ifdef _DEBUG
    _CrtSetBreakAlloc(145);  // ← Break on allocation #145
    #endif

    // ... rest of InitInstance
}
```

### Common Memory Leak Sources

```cpp
// 1. Modeless dialogs not deleted
class CMyDialog : public CDialogEx
{
    CChildDialog* m_pChildDlg;

    ~CMyDialog()
    {
        if (m_pChildDlg != nullptr)
        {
            m_pChildDlg->DestroyWindow();
            delete m_pChildDlg;  // ← Don't forget!
        }
    }
};

// 2. GDI objects not deleted
void CMyDialog::OnPaint()
{
    CPaintDC dc(this);

    CBrush brush;
    brush.CreateSolidBrush(RGB(255, 0, 0));
    dc.SelectObject(&brush);
    dc.Rectangle(0, 0, 100, 100);

    brush.DeleteObject();  // ← Clean up!
}

// 3. Allocated strings not freed
void CMyDialog::OnButton()
{
    CString* pStr = new CString(_T("Hello"));
    // ... use pStr
    delete pStr;  // ← Don't forget!
}
```

---

## 4. Performance Profiling

### Using Visual Studio Profiler

```
1. Debug → Performance Profiler (Alt+F2)
2. Select "CPU Usage" or "Memory Usage"
3. Click Start
4. Use your application
5. Click Stop
6. Analyze results
```

### Optimizing MFC Applications

```cpp
// 1. Reduce redraws
void CMyView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    // Don't invalidate entire window
    // InvalidateRect(); ← Slow

    // Invalidate only changed area
    CRect rectChanged(10, 10, 100, 100);
    InvalidateRect(&rectChanged);  // ← Faster
}

// 2. Use precompiled headers
// Already enabled by default in MFC projects

// 3. Minimize string operations in loops
void CMyDialog::ProcessItems()
{
    CString strResult;

    // BAD - repeated allocations
    for (int i = 0; i < 1000; i++)
    {
        CString strTemp;
        strTemp.Format(_T("Item %d"), i);
        strResult += strTemp;  // ← Allocates each time
    }

    // BETTER - preallocate
    strResult.Preallocate(1000 * 20);  // Estimate size
    for (int i = 0; i < 1000; i++)
    {
        CString strTemp;
        strTemp.Format(_T("Item %d"), i);
        strResult += strTemp;
    }
}

// 4. Cache GetDlgItem results
void CMyDialog::UpdateDisplay()
{
    // BAD - repeatedly calls GetDlgItem
    for (int i = 0; i < 100; i++)
    {
        SetDlgItemInt(IDC_STATIC_COUNT, i);  // ← GetDlgItem each time
    }

    // BETTER - cache the pointer
    CWnd* pStatic = GetDlgItem(IDC_STATIC_COUNT);
    for (int i = 0; i < 100; i++)
    {
        CString str;
        str.Format(_T("%d"), i);
        pStatic->SetWindowText(str);
    }
}
```

---

## 5. Best Practices

### Error Handling

```cpp
// Always check return values
void CMyDialog::OpenFile()
{
    CFileDialog dlg(TRUE);
    if (dlg.DoModal() != IDOK)
        return;  // User cancelled

    CFile file;
    if (!file.Open(dlg.GetPathName(), CFile::modeRead))
    {
        AfxMessageBox(_T("Cannot open file!"));
        return;  // File open failed
    }

    // Process file
}

// Use VERIFY in debug, simple assignment in release
void CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    #ifdef _DEBUG
    VERIFY(SetDlgItemText(IDC_EDIT1, _T("Hello")));  // ← Asserts if fails
    #else
    SetDlgItemText(IDC_EDIT1, _T("Hello"));
    #endif
}
```

### Defensive Programming

```cpp
void CMyDialog::ProcessData(CDataObject* pData)
{
    // Check for NULL
    if (pData == nullptr)
    {
        ASSERT(FALSE);  // ← Debug: alert programmer
        return;         // ← Release: handle gracefully
    }

    // Check for valid state
    if (!pData->IsValid())
    {
        TRACE(_T("Invalid data object\n"));
        return;
    }

    // Process data
    pData->DoSomething();
}
```

---

## 6. Troubleshooting Guide

### Problem: Controls not visible

**CHECKS**:
1. Is control hidden? Check Visible property
2. Is control outside dialog bounds?
3. Is Z-order correct? (Format → Order)
4. Is control covered by another control?

### Problem: Dialog doesn't close when pressing Enter

**SOLUTION**:
```cpp
// Ensure you have a default button (IDOK)
// In Resource Editor:
//   Select OK button
//   Properties → Default Button = True
```

### Problem: Tab order is wrong

**SOLUTION**:
```
Resource Editor:
1. Format → Tab Order (Ctrl+D)
2. Click controls in desired tab order
3. Press Enter when done
```

### Problem: UpdateData crashes

**CHECKS**:
```cpp
void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    // Check control ID exists in resource
    DDX_Text(pDX, IDC_EDIT_NAME, m_strName);  // ← Does IDC_EDIT_NAME exist?

    // Check variable is declared
    // ← Is m_strName declared in .h file?
}
```

---

## Summary

### Key Takeaways

1. **Use debugging tools**: Breakpoints, watch, call stack
2. **Use TRACE and ASSERT**: Debug output and validation
3. **Check for common errors**: Missing includes, wrong base class, accessing controls too early
4. **Detect memory leaks**: Use DEBUG_NEW, check output window
5. **Profile performance**: Use Visual Studio profiler
6. **Follow best practices**: Error handling, defensive programming

### MFC Development Checklist

**Before Committing Code**:
- [ ] No compiler warnings
- [ ] All breakpoints removed
- [ ] TRACE statements cleaned up or commented
- [ ] No memory leaks detected
- [ ] Tested debug and release builds
- [ ] Error handling implemented
- [ ] Code commented where necessary

**Congratulations!** You've completed all 15 MFC lessons. You now have comprehensive knowledge of:
- MFC architecture and class hierarchy
- Dialog-based applications
- Controls and DDX/DDV
- Message maps and event handling
- Resource files
- Common dialogs
- Custom dialog classes
- Debugging and troubleshooting

### Next Steps

**Continue Learning**:
- Advanced MFC topics (Document/View, SDI/MDI applications)
- GDI and graphics programming
- Multithreading in MFC
- Database programming with MFC
- COM and ActiveX

**Practice Projects**:
1. Text editor with file operations
2. Contact management system
3. Image viewer with filters
4. Calculator with history
5. Database frontend application

---

## Course Complete!

You've mastered the fundamentals of MFC programming and are ready to maintain and develop Windows desktop applications using MFC. Keep practicing and building real-world applications to solidify your skills!
