# Lesson 14: Custom Dialog Classes

**Duration**: 5-6 hours
**Difficulty**: Intermediate to Advanced

## Table of Contents
1. Creating Reusable Dialog Classes
2. Dialog Base Classes
3. Template Dialogs
4. Dialog Inheritance Hierarchies
5. Best Practices

---

## 1. Creating Reusable Dialog Classes

### Generic Input Dialog

```cpp
class CInputDialog : public CDialogEx
{
private:
    CString m_strPrompt;
    CString m_strValue;

public:
    CInputDialog(const CString& strPrompt, const CString& strInitialValue = _T(""),
                CWnd* pParent = nullptr);

    CString GetValue() const { return m_strValue; }

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()
};

CInputDialog::CInputDialog(const CString& strPrompt, const CString& strInitialValue, CWnd* pParent)
    : CDialogEx(IDD_INPUT_DIALOG, pParent)
    , m_strPrompt(strPrompt)
    , m_strValue(strInitialValue)
{
}

BOOL CInputDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetDlgItemText(IDC_STATIC_PROMPT, m_strPrompt);
    SetDlgItemText(IDC_EDIT_VALUE, m_strValue);

    return TRUE;
}

void CInputDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_VALUE, m_strValue);
}

// Usage
void CMyDialog::OnButtonGetName()
{
    CInputDialog dlg(_T("Enter your name:"), _T("John Doe"));
    if (dlg.DoModal() == IDOK)
    {
        CString strName = dlg.GetValue();
        MessageBox(strName);
    }
}
```

---

## 2. Dialog Base Classes

### Creating a Base Dialog for Common Functionality

```cpp
class CBaseDialog : public CDialogEx
{
protected:
    virtual BOOL OnInitDialog();

    // Common helpers
    void ShowError(LPCTSTR lpszMessage);
    void ShowInfo(LPCTSTR lpszMessage);
    BOOL ValidateNotEmpty(int nControlID, LPCTSTR lpszMessage);

    DECLARE_MESSAGE_MAP()
};

BOOL CBaseDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Center dialog
    CenterWindow();

    // Set icon
    HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    SetIcon(hIcon, TRUE);
    SetIcon(hIcon, FALSE);

    return TRUE;
}

void CBaseDialog::ShowError(LPCTSTR lpszMessage)
{
    MessageBox(lpszMessage, _T("Error"), MB_OK | MB_ICONERROR);
}

void CBaseDialog::ShowInfo(LPCTSTR lpszMessage)
{
    MessageBox(lpszMessage, _T("Information"), MB_OK | MB_ICONINFORMATION);
}

BOOL CBaseDialog::ValidateNotEmpty(int nControlID, LPCTSTR lpszMessage)
{
    CString str;
    GetDlgItemText(nControlID, str);

    if (str.IsEmpty())
    {
        ShowError(lpszMessage);
        GetDlgItem(nControlID)->SetFocus();
        return FALSE;
    }

    return TRUE;
}

// Derived dialogs
class CMyDialog : public CBaseDialog
{
protected:
    virtual void OnOK();
};

void CMyDialog::OnOK()
{
    if (!ValidateNotEmpty(IDC_EDIT_NAME, _T("Please enter a name")))
        return;

    CBaseDialog::OnOK();
}
```

---

## 3. Template Dialogs

### Generic List Selection Dialog

```cpp
template<typename T>
class CListSelectionDialog : public CDialogEx
{
private:
    CArray<T>& m_items;
    int m_nSelectedIndex;
    CString m_strTitle;

public:
    CListSelectionDialog(CArray<T>& items, const CString& strTitle,
                        CWnd* pParent = nullptr);

    int GetSelectedIndex() const { return m_nSelectedIndex; }
    T GetSelectedItem() const;

protected:
    virtual BOOL OnInitDialog();
    virtual void OnOK();

    DECLARE_MESSAGE_MAP()
};

template<typename T>
CListSelectionDialog<T>::CListSelectionDialog(CArray<T>& items,
                                               const CString& strTitle,
                                               CWnd* pParent)
    : CDialogEx(IDD_LIST_SELECTION, pParent)
    , m_items(items)
    , m_strTitle(strTitle)
    , m_nSelectedIndex(-1)
{
}

template<typename T>
BOOL CListSelectionDialog<T>::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetWindowText(m_strTitle);

    CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST);
    if (pList != nullptr)
    {
        for (int i = 0; i < m_items.GetCount(); i++)
        {
            CString str;
            str.Format(_T("%s"), (LPCTSTR)m_items[i].ToString());
            pList->AddString(str);
        }
    }

    return TRUE;
}

template<typename T>
void CListSelectionDialog<T>::OnOK()
{
    CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST);
    m_nSelectedIndex = pList->GetCurSel();

    if (m_nSelectedIndex == LB_ERR)
    {
        MessageBox(_T("Please select an item"));
        return;
    }

    CDialogEx::OnOK();
}

template<typename T>
T CListSelectionDialog<T>::GetSelectedItem() const
{
    if (m_nSelectedIndex >= 0 && m_nSelectedIndex < m_items.GetCount())
        return m_items[m_nSelectedIndex];

    return T();
}

// Usage
CArray<CString> names;
names.Add(_T("Alice"));
names.Add(_T("Bob"));
names.Add(_T("Charlie"));

CListSelectionDialog<CString> dlg(names, _T("Select a name"));
if (dlg.DoModal() == IDOK)
{
    CString strSelected = dlg.GetSelectedItem();
    MessageBox(strSelected);
}
```

---

## 4. Dialog Inheritance Hierarchies

### Three-Level Hierarchy Example

```cpp
// Level 1: Base dialog with common functionality
class CBaseDialog : public CDialogEx
{
protected:
    virtual BOOL OnInitDialog();
    void ShowError(LPCTSTR lpszMessage);
};

// Level 2: Form dialog with validation
class CFormDialog : public CBaseDialog
{
protected:
    virtual BOOL ValidateForm() = 0;  // Pure virtual
    virtual void OnOK();
};

void CFormDialog::OnOK()
{
    if (!ValidateForm())
        return;

    CBaseDialog::OnOK();
}

// Level 3: Specific dialogs
class CPersonDialog : public CFormDialog
{
protected:
    CString m_strName;
    int m_nAge;

    virtual BOOL ValidateForm();
    virtual void DoDataExchange(CDataExchange* pDX);
};

BOOL CPersonDialog::ValidateForm()
{
    if (m_strName.IsEmpty())
    {
        ShowError(_T("Name is required"));
        return FALSE;
    }

    if (m_nAge < 0 || m_nAge > 120)
    {
        ShowError(_T("Age must be 0-120"));
        return FALSE;
    }

    return TRUE;
}

void CPersonDialog::DoDataExchange(CDataExchange* pDX)
{
    CFormDialog::DoDataExchange(pDX);

    DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
    DDX_Text(pDX, IDC_EDIT_AGE, m_nAge);
}
```

---

## 5. Best Practices

### Best Practices for Custom Dialogs

1. **Create base classes** for common functionality
2. **Use templates** for generic dialogs
3. **Keep dialogs focused** - one purpose per dialog
4. **Provide clear interfaces** - GetData/SetData methods
5. **Handle cleanup** properly in destructors
6. **Document** constructor parameters and return values

### Example: Well-Designed Custom Dialog

```cpp
class CProductDialog : public CDialogEx
{
private:
    struct ProductData
    {
        CString strName;
        CString strDescription;
        double dPrice;
        int nQuantity;
    };

    ProductData m_data;
    BOOL m_bEditMode;  // TRUE = edit existing, FALSE = create new

public:
    // Constructor for creating new product
    CProductDialog(CWnd* pParent = nullptr);

    // Constructor for editing existing product
    CProductDialog(const ProductData& data, CWnd* pParent = nullptr);

    // Get product data after dialog closes
    ProductData GetProductData() const { return m_data; }

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual void OnOK();

    BOOL ValidateProductData();
};

// Usage - Create new product
void CMyView::OnProductAdd()
{
    CProductDialog dlg;
    if (dlg.DoModal() == IDOK)
    {
        CProductDialog::ProductData data = dlg.GetProductData();
        AddProduct(data);
    }
}

// Usage - Edit existing product
void CMyView::OnProductEdit()
{
    CProductDialog::ProductData data = GetSelectedProduct();

    CProductDialog dlg(data);
    if (dlg.DoModal() == IDOK)
    {
        CProductDialog::ProductData updatedData = dlg.GetProductData();
        UpdateProduct(updatedData);
    }
}
```

---

## Summary

### Key Takeaways

1. **Reusable dialogs**: Create generic dialogs for common tasks
2. **Base classes**: Share functionality across dialogs
3. **Templates**: Type-safe generic dialogs
4. **Inheritance**: Multi-level hierarchies for complex apps
5. **Best practices**: Clear interfaces, proper cleanup

### Next Steps

In **Lesson 15: MFC Debugging and Common Errors**, you'll learn:
- Debugging techniques
- Common MFC errors
- Memory leak detection
- Performance profiling

---

**Next Lesson**: [Lesson 15: MFC Debugging and Common Errors](Lesson-15-MFC-Debugging-Common-Errors.md)
