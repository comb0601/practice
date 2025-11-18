# Lesson 31: List Control (CListCtrl) - Report View

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to List Controls
2. Creating a List Control in Report View
3. Adding Columns and Items
4. Item Data and LPARAM
5. Sorting List Items
6. Selecting and Highlighting Items
7. Custom Colors and Fonts
8. Context Menus and Item Operations
9. Virtual List Controls
10. Advanced Features and Best Practices

---

## 1. Introduction to List Controls

The `CListCtrl` class is one of the most versatile and powerful controls in MFC. It provides four different views:
- **Report View** (detailed list with columns)
- Icon View (large icons)
- Small Icon View (small icons)
- List View (single column list)

In this lesson, we focus on Report View, which is commonly used for displaying tabular data similar to Windows Explorer's Details view.

### Key Features of Report View:
- Multiple columns with headers
- Sortable columns
- Item selection (single or multiple)
- Custom drawing support
- Virtual mode for large datasets
- Subitem editing
- Grid lines and full row selection

### Common Applications:
- File explorers and browsers
- Database record viewers
- Log file viewers
- Shopping carts
- Task lists and to-do applications

---

## 2. Creating a List Control in Report View

### Basic List Control Setup

Let's start with a dialog-based application that demonstrates list control fundamentals.

**Resource.h:**
```cpp
#define IDD_LISTCTRL_DIALOG    1000
#define IDC_LIST_MAIN          1001
#define IDC_BTN_ADD            1002
#define IDC_BTN_DELETE         1003
#define IDC_BTN_CLEAR          1004
#define IDC_EDIT_NAME          1005
#define IDC_EDIT_EMAIL         1006
#define IDC_EDIT_PHONE         1007
```

**ListCtrlDlg.h:**
```cpp
#pragma once
#include "afxcmn.h"

class CListCtrlDlg : public CDialogEx
{
public:
    CListCtrlDlg(CWnd* pParent = nullptr);
    virtual ~CListCtrlDlg();

    enum { IDD = IDD_LISTCTRL_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()

private:
    CListCtrl m_listCtrl;
    CEdit m_editName;
    CEdit m_editEmail;
    CEdit m_editPhone;

    void InitializeListControl();
    void AddSampleData();

public:
    afx_msg void OnBtnAdd();
    afx_msg void OnBtnDelete();
    afx_msg void OnBtnClear();
    afx_msg void OnListItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnListColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
};
```

**ListCtrlDlg.cpp:**
```cpp
#include "stdafx.h"
#include "ListCtrlDlg.h"

BEGIN_MESSAGE_MAP(CListCtrlDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_ADD, &CListCtrlDlg::OnBtnAdd)
    ON_BN_CLICKED(IDC_BTN_DELETE, &CListCtrlDlg::OnBtnDelete)
    ON_BN_CLICKED(IDC_BTN_CLEAR, &CListCtrlDlg::OnBtnClear)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_MAIN, &CListCtrlDlg::OnListItemChanged)
    ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_MAIN, &CListCtrlDlg::OnListColumnClick)
END_MESSAGE_MAP()

CListCtrlDlg::CListCtrlDlg(CWnd* pParent)
    : CDialogEx(IDD_LISTCTRL_DIALOG, pParent)
{
}

CListCtrlDlg::~CListCtrlDlg()
{
}

void CListCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_MAIN, m_listCtrl);
    DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
    DDX_Control(pDX, IDC_EDIT_EMAIL, m_editEmail);
    DDX_Control(pDX, IDC_EDIT_PHONE, m_editPhone);
}

BOOL CListCtrlDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    InitializeListControl();
    AddSampleData();

    return TRUE;
}

void CListCtrlDlg::InitializeListControl()
{
    // Set extended styles for modern appearance
    DWORD dwStyle = m_listCtrl.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;     // Select entire row
    dwStyle |= LVS_EX_GRIDLINES;         // Show grid lines
    dwStyle |= LVS_EX_HEADERDRAGDROP;    // Allow column reordering
    dwStyle |= LVS_EX_DOUBLEBUFFER;      // Eliminate flicker
    m_listCtrl.SetExtendedStyle(dwStyle);

    // Insert columns
    m_listCtrl.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 150);
    m_listCtrl.InsertColumn(1, _T("Email"), LVCFMT_LEFT, 200);
    m_listCtrl.InsertColumn(2, _T("Phone"), LVCFMT_LEFT, 120);
    m_listCtrl.InsertColumn(3, _T("Status"), LVCFMT_CENTER, 80);
}
```

---

## 3. Adding Columns and Items

### Dynamic Column Management

```cpp
// Add a column programmatically
void CListCtrlDlg::AddColumn(const CString& strHeader, int nFormat, int nWidth)
{
    int nColumnCount = m_listCtrl.GetHeaderCtrl()->GetItemCount();

    LVCOLUMN lvColumn;
    lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvColumn.fmt = nFormat;
    lvColumn.cx = nWidth;
    lvColumn.pszText = const_cast<LPTSTR>((LPCTSTR)strHeader);
    lvColumn.iSubItem = nColumnCount;

    m_listCtrl.InsertColumn(nColumnCount, &lvColumn);
}

// Resize columns to fit content
void CListCtrlDlg::AutoSizeColumns()
{
    int nColumnCount = m_listCtrl.GetHeaderCtrl()->GetItemCount();

    for (int i = 0; i < nColumnCount; i++)
    {
        m_listCtrl.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
    }
}

// Get column width
int CListCtrlDlg::GetColumnWidth(int nColumn)
{
    return m_listCtrl.GetColumnWidth(nColumn);
}

// Set column width
void CListCtrlDlg::SetColumnWidth(int nColumn, int nWidth)
{
    m_listCtrl.SetColumnWidth(nColumn, nWidth);
}
```

### Adding Items and Subitems

```cpp
void CListCtrlDlg::AddSampleData()
{
    // Method 1: Using InsertItem and SetItemText
    int nIndex = m_listCtrl.InsertItem(0, _T("John Smith"));
    m_listCtrl.SetItemText(nIndex, 1, _T("john.smith@email.com"));
    m_listCtrl.SetItemText(nIndex, 2, _T("555-0101"));
    m_listCtrl.SetItemText(nIndex, 3, _T("Active"));

    // Method 2: Using LVITEM structure
    LVITEM lvItem;
    ZeroMemory(&lvItem, sizeof(lvItem));

    lvItem.mask = LVIF_TEXT | LVIF_PARAM;
    lvItem.iItem = m_listCtrl.GetItemCount();
    lvItem.iSubItem = 0;
    lvItem.pszText = _T("Jane Doe");
    lvItem.lParam = (LPARAM)1001; // Custom data associated with this item

    nIndex = m_listCtrl.InsertItem(&lvItem);
    m_listCtrl.SetItemText(nIndex, 1, _T("jane.doe@email.com"));
    m_listCtrl.SetItemText(nIndex, 2, _T("555-0102"));
    m_listCtrl.SetItemText(nIndex, 3, _T("Active"));

    // Method 3: Insert at specific position
    nIndex = m_listCtrl.InsertItem(1, _T("Bob Johnson"));
    m_listCtrl.SetItemText(nIndex, 1, _T("bob.j@email.com"));
    m_listCtrl.SetItemText(nIndex, 2, _T("555-0103"));
    m_listCtrl.SetItemText(nIndex, 3, _T("Inactive"));
}

// Add item button handler
void CListCtrlDlg::OnBtnAdd()
{
    CString strName, strEmail, strPhone;
    m_editName.GetWindowText(strName);
    m_editEmail.GetWindowText(strEmail);
    m_editPhone.GetWindowText(strPhone);

    if (strName.IsEmpty())
    {
        AfxMessageBox(_T("Please enter a name"));
        return;
    }

    // Insert new item
    int nIndex = m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), strName);
    m_listCtrl.SetItemText(nIndex, 1, strEmail);
    m_listCtrl.SetItemText(nIndex, 2, strPhone);
    m_listCtrl.SetItemText(nIndex, 3, _T("Active"));

    // Clear input fields
    m_editName.SetWindowText(_T(""));
    m_editEmail.SetWindowText(_T(""));
    m_editPhone.SetWindowText(_T(""));

    // Select the new item
    m_listCtrl.SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED,
                            LVIS_SELECTED | LVIS_FOCUSED);
    m_listCtrl.EnsureVisible(nIndex, FALSE);
}
```

---

## 4. Item Data and LPARAM

Each list item can store a custom LPARAM value, which is useful for associating data with items.

### Using LPARAM for Custom Data

```cpp
// Structure to hold item data
struct ContactInfo
{
    int nID;
    CString strName;
    CString strEmail;
    CString strPhone;
    BOOL bActive;
    SYSTEMTIME createdTime;

    ContactInfo() : nID(0), bActive(TRUE)
    {
        GetLocalTime(&createdTime);
    }
};

class CContactListCtrl : public CListCtrl
{
private:
    CArray<ContactInfo*, ContactInfo*> m_arrContacts;
    int m_nNextID;

public:
    CContactListCtrl() : m_nNextID(1) {}

    virtual ~CContactListCtrl()
    {
        ClearAllContacts();
    }

    int AddContact(const CString& strName, const CString& strEmail,
                   const CString& strPhone)
    {
        // Create new contact info
        ContactInfo* pContact = new ContactInfo();
        pContact->nID = m_nNextID++;
        pContact->strName = strName;
        pContact->strEmail = strEmail;
        pContact->strPhone = strPhone;
        pContact->bActive = TRUE;

        m_arrContacts.Add(pContact);

        // Add to list control
        int nIndex = InsertItem(GetItemCount(), strName);
        SetItemText(nIndex, 1, strEmail);
        SetItemText(nIndex, 2, strPhone);
        SetItemText(nIndex, 3, _T("Active"));

        // Store pointer in LPARAM
        SetItemData(nIndex, (DWORD_PTR)pContact);

        return nIndex;
    }

    ContactInfo* GetContactInfo(int nItem)
    {
        if (nItem < 0 || nItem >= GetItemCount())
            return nullptr;

        return (ContactInfo*)GetItemData(nItem);
    }

    void UpdateContactStatus(int nItem, BOOL bActive)
    {
        ContactInfo* pContact = GetContactInfo(nItem);
        if (pContact)
        {
            pContact->bActive = bActive;
            SetItemText(nItem, 3, bActive ? _T("Active") : _T("Inactive"));
        }
    }

    void DeleteContact(int nItem)
    {
        ContactInfo* pContact = GetContactInfo(nItem);
        if (pContact)
        {
            // Remove from array
            for (int i = 0; i < m_arrContacts.GetSize(); i++)
            {
                if (m_arrContacts[i] == pContact)
                {
                    delete pContact;
                    m_arrContacts.RemoveAt(i);
                    break;
                }
            }

            // Remove from list
            DeleteItem(nItem);
        }
    }

    void ClearAllContacts()
    {
        // Delete all contact data
        for (int i = 0; i < m_arrContacts.GetSize(); i++)
        {
            delete m_arrContacts[i];
        }
        m_arrContacts.RemoveAll();

        // Clear list
        DeleteAllItems();
    }

    CString GetContactDetails(int nItem)
    {
        ContactInfo* pContact = GetContactInfo(nItem);
        if (!pContact)
            return _T("");

        CString strDetails;
        strDetails.Format(_T("ID: %d\nName: %s\nEmail: %s\nPhone: %s\nStatus: %s\nCreated: %02d/%02d/%04d %02d:%02d"),
            pContact->nID,
            pContact->strName,
            pContact->strEmail,
            pContact->strPhone,
            pContact->bActive ? _T("Active") : _T("Inactive"),
            pContact->createdTime.wMonth,
            pContact->createdTime.wDay,
            pContact->createdTime.wYear,
            pContact->createdTime.wHour,
            pContact->createdTime.wMinute);

        return strDetails;
    }
};
```

---

## 5. Sorting List Items

### Implementing Column Sorting

```cpp
// Global or class member variable to track sort order
static int g_nSortColumn = 0;
static BOOL g_bSortAscending = TRUE;

// Comparison callback function
int CALLBACK CompareListItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    ContactInfo* pContact1 = (ContactInfo*)lParam1;
    ContactInfo* pContact2 = (ContactInfo*)lParam2;

    if (!pContact1 || !pContact2)
        return 0;

    int nResult = 0;
    int nColumn = (int)lParamSort;

    switch (nColumn)
    {
    case 0: // Name
        nResult = pContact1->strName.CompareNoCase(pContact2->strName);
        break;

    case 1: // Email
        nResult = pContact1->strEmail.CompareNoCase(pContact2->strEmail);
        break;

    case 2: // Phone
        nResult = pContact1->strPhone.Compare(pContact2->strPhone);
        break;

    case 3: // Status
        nResult = pContact1->bActive - pContact2->bActive;
        break;

    default:
        nResult = 0;
    }

    // Apply sort direction
    if (!g_bSortAscending)
        nResult = -nResult;

    return nResult;
}

// Column click handler
void CListCtrlDlg::OnListColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    int nColumn = pNMLV->iSubItem;

    // If clicking the same column, reverse sort order
    if (nColumn == g_nSortColumn)
    {
        g_bSortAscending = !g_bSortAscending;
    }
    else
    {
        g_nSortColumn = nColumn;
        g_bSortAscending = TRUE;
    }

    // Sort the list
    m_listCtrl.SortItems(CompareListItems, (LPARAM)g_nSortColumn);

    *pResult = 0;
}

// Update header to show sort indicator
void CListCtrlDlg::UpdateSortIndicator()
{
    CHeaderCtrl* pHeader = m_listCtrl.GetHeaderCtrl();
    if (!pHeader)
        return;

    int nColumnCount = pHeader->GetItemCount();

    for (int i = 0; i < nColumnCount; i++)
    {
        HDITEM hdItem;
        hdItem.mask = HDI_FORMAT;
        pHeader->GetItem(i, &hdItem);

        // Remove sort indicators
        hdItem.fmt &= ~(HDF_SORTUP | HDF_SORTDOWN);

        // Add indicator for current sort column
        if (i == g_nSortColumn)
        {
            hdItem.fmt |= g_bSortAscending ? HDF_SORTUP : HDF_SORTDOWN;
        }

        pHeader->SetItem(i, &hdItem);
    }
}
```

---

## 6. Selecting and Highlighting Items

### Selection Management

```cpp
// Get selected item index
int CListCtrlDlg::GetSelectedItem()
{
    return m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
}

// Get all selected items
void CListCtrlDlg::GetSelectedItems(CArray<int, int>& arrSelected)
{
    arrSelected.RemoveAll();

    POSITION pos = m_listCtrl.GetFirstSelectedItemPosition();
    while (pos)
    {
        int nItem = m_listCtrl.GetNextSelectedItem(pos);
        arrSelected.Add(nItem);
    }
}

// Select an item
void CListCtrlDlg::SelectItem(int nItem, BOOL bSelect)
{
    UINT uState = bSelect ? (LVIS_SELECTED | LVIS_FOCUSED) : 0;
    m_listCtrl.SetItemState(nItem, uState, LVIS_SELECTED | LVIS_FOCUSED);
}

// Select all items
void CListCtrlDlg::SelectAll()
{
    int nCount = m_listCtrl.GetItemCount();
    for (int i = 0; i < nCount; i++)
    {
        m_listCtrl.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
    }
}

// Deselect all items
void CListCtrlDlg::DeselectAll()
{
    int nCount = m_listCtrl.GetItemCount();
    for (int i = 0; i < nCount; i++)
    {
        m_listCtrl.SetItemState(i, 0, LVIS_SELECTED);
    }
}

// Delete selected items
void CListCtrlDlg::OnBtnDelete()
{
    CArray<int, int> arrSelected;
    GetSelectedItems(arrSelected);

    if (arrSelected.GetSize() == 0)
    {
        AfxMessageBox(_T("No items selected"));
        return;
    }

    CString strMsg;
    strMsg.Format(_T("Delete %d item(s)?"), arrSelected.GetSize());

    if (AfxMessageBox(strMsg, MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        // Delete from bottom to top to maintain indices
        for (int i = arrSelected.GetSize() - 1; i >= 0; i--)
        {
            ContactInfo* pContact = (ContactInfo*)m_listCtrl.GetItemData(arrSelected[i]);
            delete pContact;
            m_listCtrl.DeleteItem(arrSelected[i]);
        }
    }
}

// Item changed notification
void CListCtrlDlg::OnListItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    // Check if selection changed
    if ((pNMLV->uChanged & LVIF_STATE) &&
        (pNMLV->uNewState & LVIS_SELECTED))
    {
        int nItem = pNMLV->iItem;

        // Get item text
        CString strName = m_listCtrl.GetItemText(nItem, 0);
        CString strEmail = m_listCtrl.GetItemText(nItem, 1);
        CString strPhone = m_listCtrl.GetItemText(nItem, 2);

        // Update edit controls
        m_editName.SetWindowText(strName);
        m_editEmail.SetWindowText(strEmail);
        m_editPhone.SetWindowText(strPhone);
    }

    *pResult = 0;
}
```

---

## 7. Custom Colors and Fonts

### Custom Drawing

```cpp
class CColorListCtrl : public CListCtrl
{
public:
    CColorListCtrl()
    {
        m_clrTextNormal = RGB(0, 0, 0);
        m_clrTextHighlight = RGB(255, 255, 255);
        m_clrBackground = RGB(255, 255, 255);
        m_clrBackgroundHighlight = RGB(0, 120, 215);
        m_clrAlternate = RGB(240, 240, 240);
    }

    void SetColors(COLORREF clrText, COLORREF clrBackground, COLORREF clrAlternate)
    {
        m_clrTextNormal = clrText;
        m_clrBackground = clrBackground;
        m_clrAlternate = clrAlternate;
    }

protected:
    COLORREF m_clrTextNormal;
    COLORREF m_clrTextHighlight;
    COLORREF m_clrBackground;
    COLORREF m_clrBackgroundHighlight;
    COLORREF m_clrAlternate;

    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
};

BEGIN_MESSAGE_MAP(CColorListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CColorListCtrl::OnCustomDraw)
END_MESSAGE_MAP()

void CColorListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLVCUSTOMDRAW pLVCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

    *pResult = CDRF_DODEFAULT;

    switch (pLVCD->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
        *pResult = CDRF_NOTIFYITEMDRAW;
        break;

    case CDDS_ITEMPREPAINT:
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
        break;

    case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
        {
            int nItem = (int)pLVCD->nmcd.dwItemSpec;
            int nSubItem = pLVCD->iSubItem;

            // Check if item is selected
            BOOL bSelected = (GetItemState(nItem, LVIS_SELECTED) & LVIS_SELECTED);

            if (bSelected)
            {
                pLVCD->clrText = m_clrTextHighlight;
                pLVCD->clrTextBk = m_clrBackgroundHighlight;
            }
            else
            {
                // Alternate row colors
                if (nItem % 2 == 0)
                    pLVCD->clrTextBk = m_clrBackground;
                else
                    pLVCD->clrTextBk = m_clrAlternate;

                // Custom color based on status column
                CString strStatus = GetItemText(nItem, 3);
                if (strStatus == _T("Active"))
                    pLVCD->clrText = RGB(0, 128, 0); // Green
                else if (strStatus == _T("Inactive"))
                    pLVCD->clrText = RGB(128, 128, 128); // Gray
                else
                    pLVCD->clrText = m_clrTextNormal;
            }

            *pResult = CDRF_DODEFAULT;
        }
        break;
    }
}
```

---

## 8. Context Menus and Item Operations

### Implementing Context Menu

```cpp
// Add to header
public:
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnEditItem();
    afx_msg void OnToggleStatus();
    afx_msg void OnViewDetails();

// Add to message map
ON_WM_CONTEXTMENU()
ON_COMMAND(ID_EDIT_ITEM, &CListCtrlDlg::OnEditItem)
ON_COMMAND(ID_TOGGLE_STATUS, &CListCtrlDlg::OnToggleStatus)
ON_COMMAND(ID_VIEW_DETAILS, &CListCtrlDlg::OnViewDetails)

// Implementation
void CListCtrlDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
    if (pWnd->GetSafeHwnd() != m_listCtrl.GetSafeHwnd())
        return;

    int nSelectedCount = m_listCtrl.GetSelectedCount();
    if (nSelectedCount == 0)
        return;

    CMenu menu;
    menu.CreatePopupMenu();

    if (nSelectedCount == 1)
    {
        menu.AppendMenu(MF_STRING, ID_EDIT_ITEM, _T("&Edit"));
        menu.AppendMenu(MF_STRING, ID_VIEW_DETAILS, _T("&View Details"));
        menu.AppendMenu(MF_SEPARATOR);
        menu.AppendMenu(MF_STRING, ID_TOGGLE_STATUS, _T("Toggle &Status"));
        menu.AppendMenu(MF_SEPARATOR);
    }

    menu.AppendMenu(MF_STRING, IDC_BTN_DELETE, _T("&Delete"));

    menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                        point.x, point.y, this);
}

void CListCtrlDlg::OnEditItem()
{
    int nItem = GetSelectedItem();
    if (nItem < 0)
        return;

    // Create inline edit control
    CRect rect;
    m_listCtrl.GetSubItemRect(nItem, 0, LVIR_LABEL, rect);

    CEdit* pEdit = new CEdit();
    pEdit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                  rect, &m_listCtrl, 5000);

    CString strText = m_listCtrl.GetItemText(nItem, 0);
    pEdit->SetWindowText(strText);
    pEdit->SetFocus();
    pEdit->SetSel(0, -1);
}

void CListCtrlDlg::OnToggleStatus()
{
    int nItem = GetSelectedItem();
    if (nItem < 0)
        return;

    CString strStatus = m_listCtrl.GetItemText(nItem, 3);

    if (strStatus == _T("Active"))
        m_listCtrl.SetItemText(nItem, 3, _T("Inactive"));
    else
        m_listCtrl.SetItemText(nItem, 3, _T("Active"));

    m_listCtrl.RedrawItems(nItem, nItem);
}

void CListCtrlDlg::OnViewDetails()
{
    int nItem = GetSelectedItem();
    if (nItem < 0)
        return;

    ContactInfo* pContact = (ContactInfo*)m_listCtrl.GetItemData(nItem);
    if (!pContact)
        return;

    CString strDetails;
    strDetails.Format(_T("Contact Details\n\n")
                     _T("ID: %d\n")
                     _T("Name: %s\n")
                     _T("Email: %s\n")
                     _T("Phone: %s\n")
                     _T("Status: %s"),
                     pContact->nID,
                     pContact->strName,
                     pContact->strEmail,
                     pContact->strPhone,
                     pContact->bActive ? _T("Active") : _T("Inactive"));

    AfxMessageBox(strDetails, MB_OK | MB_ICONINFORMATION);
}
```

---

## 9. Virtual List Controls

Virtual list controls are perfect for handling large datasets efficiently.

```cpp
class CVirtualListCtrl : public CListCtrl
{
private:
    CArray<ContactInfo*, ContactInfo*> m_arrData;

public:
    CVirtualListCtrl() {}

    virtual ~CVirtualListCtrl()
    {
        ClearData();
    }

    void InitVirtualList(int nItemCount)
    {
        // Set virtual mode
        ModifyStyle(0, LVS_OWNERDATA);
        SetItemCount(nItemCount);
    }

    void SetData(CArray<ContactInfo*, ContactInfo*>& arrData)
    {
        m_arrData.RemoveAll();
        m_arrData.Append(arrData);
        SetItemCount((int)m_arrData.GetSize());
    }

    void ClearData()
    {
        for (int i = 0; i < m_arrData.GetSize(); i++)
            delete m_arrData[i];
        m_arrData.RemoveAll();
        SetItemCount(0);
    }

protected:
    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnOdCacheHint(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnOdFindItem(NMHDR* pNMHDR, LRESULT* pResult);
};

BEGIN_MESSAGE_MAP(CVirtualListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(LVN_GETDISPINFO, &CVirtualListCtrl::OnGetDispInfo)
    ON_NOTIFY_REFLECT(LVN_ODCACHEHINT, &CVirtualListCtrl::OnOdCacheHint)
    ON_NOTIFY_REFLECT(LVN_ODFINDITEM, &CVirtualListCtrl::OnOdFindItem)
END_MESSAGE_MAP()

void CVirtualListCtrl::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

    int nItem = pDispInfo->item.iItem;

    if (nItem < 0 || nItem >= m_arrData.GetSize())
    {
        *pResult = 0;
        return;
    }

    ContactInfo* pContact = m_arrData[nItem];

    if (pDispInfo->item.mask & LVIF_TEXT)
    {
        switch (pDispInfo->item.iSubItem)
        {
        case 0:
            _tcscpy_s(pDispInfo->item.pszText, pDispInfo->item.cchTextMax,
                     pContact->strName);
            break;
        case 1:
            _tcscpy_s(pDispInfo->item.pszText, pDispInfo->item.cchTextMax,
                     pContact->strEmail);
            break;
        case 2:
            _tcscpy_s(pDispInfo->item.pszText, pDispInfo->item.cchTextMax,
                     pContact->strPhone);
            break;
        case 3:
            _tcscpy_s(pDispInfo->item.pszText, pDispInfo->item.cchTextMax,
                     pContact->bActive ? _T("Active") : _T("Inactive"));
            break;
        }
    }

    *pResult = 0;
}

void CVirtualListCtrl::OnOdCacheHint(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLVCACHEHINT pCacheHint = reinterpret_cast<LPNMLVCACHEHINT>(pNMHDR);

    // Preload data for items from pCacheHint->iFrom to pCacheHint->iTo
    // This is useful for optimizing data loading from databases

    *pResult = 0;
}

void CVirtualListCtrl::OnOdFindItem(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLVFINDITEM pFindInfo = reinterpret_cast<LPNMLVFINDITEM>(pNMHDR);

    // Implement search functionality
    *pResult = -1; // Not found
}
```

---

## 10. Advanced Features and Best Practices

### Complete Example: Contact Manager Application

```cpp
// ContactManagerDlg.h
#pragma once

class CContactManagerDlg : public CDialogEx
{
public:
    CContactManagerDlg(CWnd* pParent = nullptr);

    enum { IDD = IDD_CONTACTMANAGER_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

private:
    CColorListCtrl m_listContacts;
    CEdit m_editSearch;
    CButton m_btnAdd;
    CButton m_btnExport;

    void LoadContacts();
    void SaveContacts();
    void SearchContacts(const CString& strSearch);
    void ExportToCSV();

public:
    afx_msg void OnBtnAdd();
    afx_msg void OnBtnExport();
    afx_msg void OnSearchChanged();
    afx_msg void OnDestroy();

    DECLARE_MESSAGE_MAP()
};

// Implementation highlights
void CContactManagerDlg::LoadContacts()
{
    // Load from file or database
    CStdioFile file;
    if (file.Open(_T("contacts.dat"), CFile::modeRead))
    {
        CString strLine;
        while (file.ReadString(strLine))
        {
            // Parse and add to list
            int nPos = 0;
            CString strName = strLine.Tokenize(_T("|"), nPos);
            CString strEmail = strLine.Tokenize(_T("|"), nPos);
            CString strPhone = strLine.Tokenize(_T("|"), nPos);

            int nIndex = m_listContacts.InsertItem(
                m_listContacts.GetItemCount(), strName);
            m_listContacts.SetItemText(nIndex, 1, strEmail);
            m_listContacts.SetItemText(nIndex, 2, strPhone);
        }
        file.Close();
    }
}

void CContactManagerDlg::SaveContacts()
{
    CStdioFile file;
    if (file.Open(_T("contacts.dat"), CFile::modeCreate | CFile::modeWrite))
    {
        int nCount = m_listContacts.GetItemCount();
        for (int i = 0; i < nCount; i++)
        {
            CString strName = m_listContacts.GetItemText(i, 0);
            CString strEmail = m_listContacts.GetItemText(i, 1);
            CString strPhone = m_listContacts.GetItemText(i, 2);

            CString strLine;
            strLine.Format(_T("%s|%s|%s\n"), strName, strEmail, strPhone);
            file.WriteString(strLine);
        }
        file.Close();
    }
}

void CContactManagerDlg::ExportToCSV()
{
    CFileDialog dlg(FALSE, _T("csv"), _T("contacts.csv"),
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    _T("CSV Files (*.csv)|*.csv||"));

    if (dlg.DoModal() == IDOK)
    {
        CStdioFile file;
        if (file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
        {
            // Write header
            file.WriteString(_T("Name,Email,Phone,Status\n"));

            // Write data
            int nCount = m_listContacts.GetItemCount();
            for (int i = 0; i < nCount; i++)
            {
                CString strLine;
                strLine.Format(_T("%s,%s,%s,%s\n"),
                    m_listContacts.GetItemText(i, 0),
                    m_listContacts.GetItemText(i, 1),
                    m_listContacts.GetItemText(i, 2),
                    m_listContacts.GetItemText(i, 3));
                file.WriteString(strLine);
            }
            file.Close();

            AfxMessageBox(_T("Export completed successfully!"));
        }
    }
}
```

---

## Key Takeaways

1. CListCtrl in Report View provides powerful tabular data display
2. Extended styles enhance appearance and functionality
3. LPARAM allows associating custom data with each item
4. Custom drawing enables flexible visual customization
5. Virtual lists handle large datasets efficiently
6. Column sorting improves user experience
7. Context menus provide intuitive item operations
8. Proper cleanup prevents memory leaks

---

## Best Practices

1. Always use LVS_EX_DOUBLEBUFFER to prevent flicker
2. Store complex data using LPARAM pointers
3. Delete items from bottom to top to maintain indices
4. Use virtual lists for datasets over 10,000 items
5. Implement custom drawing for visual feedback
6. Provide keyboard shortcuts for common operations
7. Save column widths and order in user preferences
8. Handle selection changes efficiently

---

## Common Mistakes

### Mistake 1: Not Deleting LPARAM Data
```cpp
// Wrong
m_listCtrl.DeleteAllItems();

// Correct
for (int i = 0; i < m_listCtrl.GetItemCount(); i++)
{
    ContactInfo* pContact = (ContactInfo*)m_listCtrl.GetItemData(i);
    delete pContact;
}
m_listCtrl.DeleteAllItems();
```

### Mistake 2: Incorrect Column Index
```cpp
// Wrong - SetItemText needs item index first
m_listCtrl.SetItemText(1, nItem, _T("Value"));

// Correct
m_listCtrl.SetItemText(nItem, 1, _T("Value"));
```

---

## Summary

In this lesson, you learned:
- Creating and configuring list controls in Report View
- Adding columns and items dynamically
- Using LPARAM for custom data storage
- Implementing sortable columns
- Managing item selection
- Custom drawing for colors and fonts
- Context menus and operations
- Virtual lists for large datasets

---

## Next Lesson Preview

In **Lesson 32**, we'll explore:
- List Control Icon and Small Icon Views
- Image lists and icon management
- Drag and drop operations
- Custom icon rendering
- View switching

**Excellent work mastering CListCtrl Report View!**
