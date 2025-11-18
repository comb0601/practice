# Lesson 08: MFC Controls Part 2 (List, Combo, Check, Radio)

**Duration**: 8 hours
**Difficulty**: Intermediate

## Table of Contents
1. List Box Controls (CListBox)
2. Combo Box Controls (CComboBox)
3. List Control (CListCtrl) - Report View
4. Advanced List Box Features
5. Combo Box Styles and Techniques
6. Control Arrays and Groups
7. Complete Real-World Examples

---

## 1. List Box Controls (CListBox)

### Creating List Boxes

```
Resource Editor:
Toolbox → List Box
Properties:
    ID:              IDC_LIST_ITEMS
    Selection:       Single (only one item selectable)
                    Multiple (multiple items with Ctrl/Shift)
                    Extended (advanced multi-select)
    Sort:           True/False (auto-sort alphabetically)
    Has Strings:    True (contains text items)
```

### Basic List Box Operations

```cpp
// Add items
void CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_ITEMS);
    if (pListBox != nullptr)
    {
        pListBox->AddString(_T("Item 1"));
        pListBox->AddString(_T("Item 2"));
        pListBox->AddString(_T("Item 3"));

        // Insert at specific position
        pListBox->InsertString(1, _T("Inserted Item"));

        // Select an item
        pListBox->SetCurSel(0);  // Select first item
    }

    return TRUE;
}

// Get selected item
void CMyDialog::OnBnClickedButtonGet()
{
    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_ITEMS);
    if (pListBox != nullptr)
    {
        int nSel = pListBox->GetCurSel();
        if (nSel != LB_ERR)
        {
            CString strItem;
            pListBox->GetText(nSel, strItem);
            MessageBox(strItem);
        }
    }
}

// Remove items
void CMyDialog::OnBnClickedButtonRemove()
{
    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_ITEMS);
    if (pListBox != nullptr)
    {
        int nSel = pListBox->GetCurSel();
        if (nSel != LB_ERR)
        {
            pListBox->DeleteString(nSel);
        }

        // Clear all
        // pListBox->ResetContent();
    }
}

// Get item count
int nCount = pListBox->GetCount();

// Find item
int nIndex = pListBox->FindString(-1, _T("Search Text"));
```

### Multi-Select List Boxes

```cpp
// Get multiple selections
void CMyDialog::OnBnClickedButtonGetMultiple()
{
    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_MULTI);
    if (pListBox != nullptr)
    {
        int nSelCount = pListBox->GetSelCount();
        if (nSelCount > 0)
        {
            CArray<int> arrIndices;
            arrIndices.SetSize(nSelCount);

            pListBox->GetSelItems(nSelCount, arrIndices.GetData());

            CString strResult = _T("Selected items:\n");
            for (int i = 0; i < nSelCount; i++)
            {
                CString strItem;
                pListBox->GetText(arrIndices[i], strItem);
                strResult += strItem + _T("\n");
            }

            MessageBox(strResult);
        }
    }
}

// Select multiple items programmatically
void CMyDialog::OnBnClickedButtonSelectMultiple()
{
    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_MULTI);
    if (pListBox != nullptr)
    {
        pListBox->SetSel(0, TRUE);   // Select first
        pListBox->SetSel(2, TRUE);   // Select third
        pListBox->SetSel(4, TRUE);   // Select fifth
    }
}
```

### List Box Notifications

```cpp
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_LBN_SELCHANGE(IDC_LIST_ITEMS, &CMyDialog::OnLbnSelchangeListItems)
    ON_LBN_DBLCLK(IDC_LIST_ITEMS, &CMyDialog::OnLbnDblclkListItems)
END_MESSAGE_MAP()

void CMyDialog::OnLbnSelchangeListItems()
{
    // Selection changed
    CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_ITEMS);
    int nSel = pListBox->GetCurSel();

    if (nSel != LB_ERR)
    {
        CString strItem;
        pListBox->GetText(nSel, strItem);

        // Update display
        SetDlgItemText(IDC_STATIC_SELECTED, strItem);
    }
}

void CMyDialog::OnLbnDblclkListItems()
{
    // Double-clicked
    MessageBox(_T("Item double-clicked!"));
}
```

---

## 2. Combo Box Controls (CComboBox)

### Combo Box Styles

```cpp
CBS_SIMPLE        // Always shows list
CBS_DROPDOWN      // Dropdown with edit (can type)
CBS_DROPDOWNLIST  // Dropdown, no edit (select only) - MOST COMMON
```

### Creating Combo Boxes

```
Resource Editor:
Toolbox → Combo Box
Properties:
    ID:         IDC_COMBO_CHOICES
    Type:       Drop List (dropdown, select only)
                Dropdown (can type)
                Simple (always visible)
    Sort:       True/False
```

### Basic Combo Box Operations

```cpp
BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CHOICES);
    if (pCombo != nullptr)
    {
        // Add items
        pCombo->AddString(_T("Option 1"));
        pCombo->AddString(_T("Option 2"));
        pCombo->AddString(_T("Option 3"));

        // Select default
        pCombo->SetCurSel(0);

        // Insert at position
        pCombo->InsertString(1, _T("New Option"));
    }

    return TRUE;
}

// Get selection
void CMyDialog::OnBnClickedButtonGet()
{
    CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CHOICES);
    if (pCombo != nullptr)
    {
        int nSel = pCombo->GetCurSel();
        if (nSel != CB_ERR)
        {
            CString strItem;
            pCombo->GetLBText(nSel, strItem);
            MessageBox(strItem);
        }
    }
}

// Get typed text (for dropdown with edit)
void CMyDialog::OnBnClickedButtonGetText()
{
    CString str;
    GetDlgItemText(IDC_COMBO_CHOICES, str);
    MessageBox(str);
}

// Find and select item
int nIndex = pCombo->FindString(-1, _T("Option 2"));
if (nIndex != CB_ERR)
{
    pCombo->SetCurSel(nIndex);
}

// Clear all
pCombo->ResetContent();

// Delete specific item
pCombo->DeleteString(2);
```

### Combo Box with Data

```cpp
// Associate data with items
struct ItemData
{
    int nID;
    CString strDescription;
};

BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ITEMS);
    if (pCombo != nullptr)
    {
        // Add items with associated data
        int nIndex;

        nIndex = pCombo->AddString(_T("Item One"));
        pCombo->SetItemData(nIndex, 101);  // Associate ID 101

        nIndex = pCombo->AddString(_T("Item Two"));
        pCombo->SetItemData(nIndex, 102);

        nIndex = pCombo->AddString(_T("Item Three"));
        pCombo->SetItemData(nIndex, 103);
    }

    return TRUE;
}

// Retrieve associated data
void CMyDialog::OnCbnSelchangeComboItems()
{
    CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ITEMS);
    if (pCombo != nullptr)
    {
        int nSel = pCombo->GetCurSel();
        if (nSel != CB_ERR)
        {
            DWORD_PTR dwData = pCombo->GetItemData(nSel);
            int nID = (int)dwData;

            CString strMsg;
            strMsg.Format(_T("Item ID: %d"), nID);
            MessageBox(strMsg);
        }
    }
}
```

### Combo Box Notifications

```cpp
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_CBN_SELCHANGE(IDC_COMBO_CHOICES, &CMyDialog::OnCbnSelchangeComboChoices)
    ON_CBN_EDITCHANGE(IDC_COMBO_CHOICES, &CMyDialog::OnCbnEditchangeComboChoices)
    ON_CBN_DROPDOWN(IDC_COMBO_CHOICES, &CMyDialog::OnCbnDropdownComboChoices)
    ON_CBN_CLOSEUP(IDC_COMBO_CHOICES, &CMyDialog::OnCbnCloseupComboChoices)
END_MESSAGE_MAP()

void CMyDialog::OnCbnSelchangeComboChoices()
{
    // Selection changed
}

void CMyDialog::OnCbnEditchangeComboChoices()
{
    // Text in edit portion changed (dropdown with edit only)
}

void CMyDialog::OnCbnDropdownComboChoices()
{
    // Dropdown opened
}

void CMyDialog::OnCbnCloseupComboChoices()
{
    // Dropdown closed
}
```

---

## 3. List Control (CListCtrl) - Report View

### Creating List Controls

```
Resource Editor:
Toolbox → List Control
Properties:
    ID:         IDC_LIST_REPORT
    View:       Report (columns and rows)
                Icon/Small Icon/List
    No Column Header: False (show headers)
    Edit Labels: True (allow editing)
```

### Setting Up Columns

```cpp
BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_REPORT);
    if (pList != nullptr)
    {
        // Set extended styles
        pList->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

        // Insert columns
        pList->InsertColumn(0, _T("Name"), LVCFMT_LEFT, 150);
        pList->InsertColumn(1, _T("Age"), LVCFMT_CENTER, 50);
        pList->InsertColumn(2, _T("City"), LVCFMT_LEFT, 120);
        pList->InsertColumn(3, _T("Salary"), LVCFMT_RIGHT, 100);

        // Add items
        AddListItem(pList, _T("John Doe"), _T("30"), _T("New York"), _T("$50,000"));
        AddListItem(pList, _T("Jane Smith"), _T("25"), _T("Los Angeles"), _T("$60,000"));
        AddListItem(pList, _T("Bob Johnson"), _T("35"), _T("Chicago"), _T("$55,000"));
    }

    return TRUE;
}

void CMyDialog::AddListItem(CListCtrl* pList, LPCTSTR lpszName, LPCTSTR lpszAge,
                            LPCTSTR lpszCity, LPCTSTR lpszSalary)
{
    int nItem = pList->GetItemCount();

    // Insert main item (first column)
    pList->InsertItem(nItem, lpszName);

    // Set sub-items (other columns)
    pList->SetItemText(nItem, 1, lpszAge);
    pList->SetItemText(nItem, 2, lpszCity);
    pList->SetItemText(nItem, 3, lpszSalary);
}
```

### List Control Operations

```cpp
// Get selected item
void CMyDialog::OnBnClickedButtonGetSelected()
{
    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_REPORT);
    if (pList != nullptr)
    {
        POSITION pos = pList->GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nItem = pList->GetNextSelectedItem(pos);

            CString strName = pList->GetItemText(nItem, 0);
            CString strAge = pList->GetItemText(nItem, 1);
            CString strCity = pList->GetItemText(nItem, 2);

            CString strMsg;
            strMsg.Format(_T("Name: %s\nAge: %s\nCity: %s"),
                         strName, strAge, strCity);
            MessageBox(strMsg);
        }
    }
}

// Delete selected item
void CMyDialog::OnBnClickedButtonDelete()
{
    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_REPORT);
    if (pList != nullptr)
    {
        POSITION pos = pList->GetFirstSelectedItemPosition();
        while (pos != nullptr)
        {
            int nItem = pList->GetNextSelectedItem(pos);
            pList->DeleteItem(nItem);
            pos = pList->GetFirstSelectedItemPosition();  // Restart
        }
    }
}

// Clear all items
pList->DeleteAllItems();

// Get item count
int nCount = pList->GetItemCount();

// Find item
LVFINDINFO findInfo;
findInfo.flags = LVFI_STRING;
findInfo.psz = _T("John Doe");
int nIndex = pList->FindItem(&findInfo);
```

### Sorting List Control

```cpp
// Sort callback function
static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    CListCtrl* pList = (CListCtrl*)lParamSort;

    CString str1 = pList->GetItemText((int)lParam1, 0);
    CString str2 = pList->GetItemText((int)lParam2, 0);

    return str1.Compare(str2);
}

// Sort the list
void CMyDialog::OnBnClickedButtonSort()
{
    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_REPORT);
    if (pList != nullptr)
    {
        // Set item data to item index
        for (int i = 0; i < pList->GetItemCount(); i++)
        {
            pList->SetItemData(i, i);
        }

        // Sort
        pList->SortItems(CompareFunc, (LPARAM)pList);
    }
}
```

---

## 4. Advanced List Box Features

### Owner-Draw List Box

```cpp
class CMyDialog : public CDialogEx
{
protected:
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_WM_DRAWITEM()
    ON_WM_MEASUREITEM()
END_MESSAGE_MAP()

void CMyDialog::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    if (nIDCtl == IDC_LIST_OWNERDRAW)
    {
        lpMeasureItemStruct->itemHeight = 30;  // Custom height
    }
}

void CMyDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    if (nIDCtl == IDC_LIST_OWNERDRAW)
    {
        CDC dc;
        dc.Attach(lpDrawItemStruct->hDC);

        CRect rect = lpDrawItemStruct->rcItem;

        // Draw background
        if (lpDrawItemStruct->itemState & ODS_SELECTED)
        {
            dc.FillSolidRect(&rect, RGB(0, 120, 215));  // Blue
            dc.SetTextColor(RGB(255, 255, 255));        // White text
        }
        else
        {
            dc.FillSolidRect(&rect, RGB(255, 255, 255));  // White
            dc.SetTextColor(RGB(0, 0, 0));                // Black text
        }

        // Draw text
        CListBox* pListBox = (CListBox*)GetDlgItem(nIDCtl);
        CString strText;
        pListBox->GetText(lpDrawItemStruct->itemID, strText);

        dc.DrawText(strText, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        dc.Detach();
    }
}
```

---

## Complete Example: Contact Manager

```cpp
class CContactDialog : public CDialogEx
{
private:
    struct Contact
    {
        CString strName;
        CString strPhone;
        CString strEmail;
        CString strCity;
    };

    CArray<Contact> m_contacts;

protected:
    virtual BOOL OnInitDialog();

    afx_msg void OnBnClickedButtonAdd();
    afx_msg void OnBnClickedButtonEdit();
    afx_msg void OnBnClickedButtonDelete();
    afx_msg void OnNMDblclkListContacts(NMHDR* pNMHDR, LRESULT* pResult);

    void RefreshList();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CContactDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_ADD, &CContactDialog::OnBnClickedButtonAdd)
    ON_BN_CLICKED(IDC_BUTTON_EDIT, &CContactDialog::OnBnClickedButtonEdit)
    ON_BN_CLICKED(IDC_BUTTON_DELETE, &CContactDialog::OnBnClickedButtonDelete)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_CONTACTS, &CContactDialog::OnNMDblclkListContacts)
END_MESSAGE_MAP()

BOOL CContactDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_CONTACTS);
    if (pList != nullptr)
    {
        pList->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

        pList->InsertColumn(0, _T("Name"), LVCFMT_LEFT, 150);
        pList->InsertColumn(1, _T("Phone"), LVCFMT_LEFT, 120);
        pList->InsertColumn(2, _T("Email"), LVCFMT_LEFT, 180);
        pList->InsertColumn(3, _T("City"), LVCFMT_LEFT, 120);
    }

    RefreshList();

    return TRUE;
}

void CContactDialog::OnBnClickedButtonAdd()
{
    // Show add contact dialog
    CAddContactDialog dlg;
    if (dlg.DoModal() == IDOK)
    {
        Contact contact;
        contact.strName = dlg.m_strName;
        contact.strPhone = dlg.m_strPhone;
        contact.strEmail = dlg.m_strEmail;
        contact.strCity = dlg.m_strCity;

        m_contacts.Add(contact);
        RefreshList();
    }
}

void CContactDialog::RefreshList()
{
    CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_CONTACTS);
    if (pList != nullptr)
    {
        pList->DeleteAllItems();

        for (int i = 0; i < m_contacts.GetCount(); i++)
        {
            const Contact& contact = m_contacts[i];

            int nItem = pList->InsertItem(i, contact.strName);
            pList->SetItemText(nItem, 1, contact.strPhone);
            pList->SetItemText(nItem, 2, contact.strEmail);
            pList->SetItemText(nItem, 3, contact.strCity);
        }
    }
}
```

---

## Summary

### Key Takeaways

1. **List Box**: Simple list, single/multi-select
2. **Combo Box**: Dropdown with optional edit
3. **List Control**: Advanced grid with columns
4. **Item Data**: Associate custom data with items
5. **Notifications**: LBN_SELCHANGE, CBN_SELCHANGE, NM_CLICK
6. **Owner-Draw**: Custom rendering

### Next Lesson

In **Lesson 09: Dialog Data Exchange (DDX) and Validation (DDV)**, you'll learn:
- Automatic variable-control binding
- Data validation
- Custom DDX routines
- Error handling

---

**Next Lesson**: [Lesson 09: Dialog Data Exchange (DDX) and Validation (DDV)](Lesson-09-DDX-DDV.md)
