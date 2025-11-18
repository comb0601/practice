# Lesson 13: Multiple Dialogs and Navigation

**Duration**: 5-6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Creating Child Dialogs
2. Passing Data Between Dialogs
3. Dialog Navigation Patterns
4. Wizard-Style Dialogs
5. Modeless Dialog Management

---

## 1. Creating Child Dialogs

### Modal Child Dialog

```cpp
void CParentDialog::OnBnClickedButtonShowChild()
{
    CChildDialog dlg;
    dlg.m_strData = _T("Data from parent");

    if (dlg.DoModal() == IDOK)
    {
        // Get data back
        m_strResult = dlg.m_strResult;
        UpdateData(FALSE);
    }
}
```

### Modeless Child Dialog

```cpp
class CParentDialog : public CDialogEx
{
private:
    CChildDialog* m_pChildDlg;

public:
    CParentDialog();
    ~CParentDialog();

protected:
    afx_msg void OnBnClickedButtonShow();
    DECLARE_MESSAGE_MAP()
};

CParentDialog::CParentDialog()
    : m_pChildDlg(nullptr)
{
}

CParentDialog::~CParentDialog()
{
    if (m_pChildDlg != nullptr)
    {
        m_pChildDlg->DestroyWindow();
        delete m_pChildDlg;
    }
}

void CParentDialog::OnBnClickedButtonShow()
{
    if (m_pChildDlg == nullptr)
    {
        m_pChildDlg = new CChildDialog();
        m_pChildDlg->Create(IDD_CHILD_DIALOG, this);
        m_pChildDlg->ShowWindow(SW_SHOW);
    }
    else
    {
        m_pChildDlg->SetFocus();
    }
}
```

---

## 2. Passing Data Between Dialogs

### Method 1: Public Members

```cpp
class CChildDialog : public CDialogEx
{
public:
    CString m_strInput;
    CString m_strOutput;
};

// Usage
void CParentDialog::OnButtonShow()
{
    CChildDialog dlg;
    dlg.m_strInput = _T("Hello");

    if (dlg.DoModal() == IDOK)
    {
        MessageBox(dlg.m_strOutput);
    }
}
```

### Method 2: Constructor Parameters

```cpp
class CChildDialog : public CDialogEx
{
private:
    CString m_strData;

public:
    CChildDialog(const CString& strData, CWnd* pParent = nullptr);
    CString GetResult() const { return m_strResult; }
};

CChildDialog::CChildDialog(const CString& strData, CWnd* pParent)
    : CDialogEx(IDD_CHILD_DIALOG, pParent)
    , m_strData(strData)
{
}

// Usage
void CParentDialog::OnButtonShow()
{
    CChildDialog dlg(_T("Initial Data"));
    if (dlg.DoModal() == IDOK)
    {
        CString strResult = dlg.GetResult();
    }
}
```

---

## 3. Dialog Navigation Patterns

### Pattern: Settings Dialog with Categories

```cpp
class CSettingsDialog : public CDialogEx
{
private:
    CListBox m_listCategories;
    CRect m_rectPanel;

protected:
    afx_msg void OnLbnSelchangeListCategories();
    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CSettingsDialog, CDialogEx)
    ON_LBN_SELCHANGE(IDC_LIST_CATEGORIES, &CSettingsDialog::OnLbnSelchangeListCategories)
END_MESSAGE_MAP()

BOOL CSettingsDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Add categories
    m_listCategories.AddString(_T("General"));
    m_listCategories.AddString(_T("Display"));
    m_listCategories.AddString(_T("Advanced"));
    m_listCategories.SetCurSel(0);

    // Get panel area
    GetDlgItem(IDC_STATIC_PANEL)->GetWindowRect(&m_rectPanel);
    ScreenToClient(&m_rectPanel);

    // Show first panel
    OnLbnSelchangeListCategories();

    return TRUE;
}

void CSettingsDialog::OnLbnSelchangeListCategories()
{
    int nSel = m_listCategories.GetCurSel();

    // Hide all panels
    GetDlgItem(IDC_PANEL_GENERAL)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_PANEL_DISPLAY)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_PANEL_ADVANCED)->ShowWindow(SW_HIDE);

    // Show selected panel
    switch (nSel)
    {
    case 0: GetDlgItem(IDC_PANEL_GENERAL)->ShowWindow(SW_SHOW); break;
    case 1: GetDlgItem(IDC_PANEL_DISPLAY)->ShowWindow(SW_SHOW); break;
    case 2: GetDlgItem(IDC_PANEL_ADVANCED)->ShowWindow(SW_SHOW); break;
    }
}
```

---

## 4. Wizard-Style Dialogs

### Wizard with Multiple Pages

```cpp
class CWizardPage1 : public CDialogEx
{
public:
    CString m_strName;
};

class CWizardPage2 : public CDialogEx
{
public:
    CString m_strEmail;
};

class CWizardDialog : public CDialogEx
{
private:
    int m_nCurrentPage;
    CWizardPage1* m_pPage1;
    CWizardPage2* m_pPage2;

protected:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButtonNext();
    afx_msg void OnBnClickedButtonBack();
    afx_msg void OnBnClickedButtonFinish();

    void ShowPage(int nPage);

    DECLARE_MESSAGE_MAP()
};

BOOL CWizardDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_pPage1 = new CWizardPage1();
    m_pPage1->Create(IDD_WIZARD_PAGE1, this);

    m_pPage2 = new CWizardPage2();
    m_pPage2->Create(IDD_WIZARD_PAGE2, this);

    // Position pages
    CRect rect;
    GetDlgItem(IDC_STATIC_PAGE_AREA)->GetWindowRect(&rect);
    ScreenToClient(&rect);

    m_pPage1->SetWindowPos(nullptr, rect.left, rect.top, 0, 0,
                          SWP_NOSIZE | SWP_NOZORDER);
    m_pPage2->SetWindowPos(nullptr, rect.left, rect.top, 0, 0,
                          SWP_NOSIZE | SWP_NOZORDER);

    m_nCurrentPage = 0;
    ShowPage(0);

    return TRUE;
}

void CWizardDialog::ShowPage(int nPage)
{
    m_pPage1->ShowWindow(nPage == 0 ? SW_SHOW : SW_HIDE);
    m_pPage2->ShowWindow(nPage == 1 ? SW_SHOW : SW_HIDE);

    GetDlgItem(IDC_BUTTON_BACK)->EnableWindow(nPage > 0);
    GetDlgItem(IDC_BUTTON_NEXT)->ShowWindow(nPage < 1 ? SW_SHOW : SW_HIDE);
    GetDlgItem(IDC_BUTTON_FINISH)->ShowWindow(nPage == 1 ? SW_SHOW : SW_HIDE);
}

void CWizardDialog::OnBnClickedButtonNext()
{
    if (m_nCurrentPage < 1)
    {
        m_nCurrentPage++;
        ShowPage(m_nCurrentPage);
    }
}

void CWizardDialog::OnBnClickedButtonBack()
{
    if (m_nCurrentPage > 0)
    {
        m_nCurrentPage--;
        ShowPage(m_nCurrentPage);
    }
}

void CWizardDialog::OnBnClickedButtonFinish()
{
    // Gather all data
    CString strName = m_pPage1->m_strName;
    CString strEmail = m_pPage2->m_strEmail;

    // Process wizard completion
    OnOK();
}
```

---

## Summary

### Key Takeaways

1. **Modal dialogs**: Simple, blocks parent
2. **Modeless dialogs**: Complex, manual lifetime management
3. **Data passing**: Public members, constructors, or methods
4. **Navigation**: Show/hide panels or separate dialogs
5. **Wizards**: Multi-step dialogs with Next/Back

### Next Steps

In **Lesson 14: Custom Dialog Classes**, you'll learn:
- Creating reusable dialog classes
- Template dialogs
- Dialog inheritance hierarchies

---

**Next Lesson**: [Lesson 14: Custom Dialog Classes](Lesson-14-Custom-Dialog-Classes.md)
