# Lesson 33: Tree Control (CTreeCtrl)

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Tree Controls
2. Creating and Initializing Tree Controls
3. Adding and Manipulating Tree Items
4. Tree Item Images and Image Lists
5. Tree Navigation and Selection
6. Expanding and Collapsing Nodes
7. Drag and Drop in Trees
8. Custom Drawing and Styling
9. Tree Search and Filtering
10. Complete File System Browser

---

## 1. Introduction to Tree Controls

The `CTreeCtrl` class provides a hierarchical view of items, commonly used for:
- File system navigation
- Organizational charts
- XML/JSON viewers
- Class hierarchies
- Menu structures
- Category browsers

### Key Features:
- Parent-child relationships
- Expandable/collapsible nodes
- Icons for each item
- Single or multiple selection
- Checkboxes support
- Drag and drop
- Custom drawing

---

## 2. Creating and Initializing Tree Controls

### Basic Tree Control Setup

```cpp
// TreeCtrlDlg.h
#pragma once
#include "afxcmn.h"

class CTreeCtrlDlg : public CDialogEx
{
public:
    CTreeCtrlDlg(CWnd* pParent = nullptr);
    virtual ~CTreeCtrlDlg();

    enum { IDD = IDD_TREECTRL_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

private:
    CTreeCtrl m_treeCtrl;
    CImageList m_imageList;

    void InitializeTreeControl();
    void CreateSampleTree();

public:
    afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnRightClick(NMHDR* pNMHDR, LRESULT* pResult);

    DECLARE_MESSAGE_MAP()
};

// TreeCtrlDlg.cpp
#include "stdafx.h"
#include "TreeCtrlDlg.h"

BEGIN_MESSAGE_MAP(CTreeCtrlDlg, CDialogEx)
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_MAIN, &CTreeCtrlDlg::OnSelChanged)
    ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE_MAIN, &CTreeCtrlDlg::OnItemExpanding)
    ON_NOTIFY(NM_RCLICK, IDC_TREE_MAIN, &CTreeCtrlDlg::OnRightClick)
END_MESSAGE_MAP()

CTreeCtrlDlg::CTreeCtrlDlg(CWnd* pParent)
    : CDialogEx(IDD_TREECTRL_DIALOG, pParent)
{
}

CTreeCtrlDlg::~CTreeCtrlDlg()
{
}

void CTreeCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE_MAIN, m_treeCtrl);
}

BOOL CTreeCtrlDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    InitializeTreeControl();
    CreateSampleTree();

    return TRUE;
}

void CTreeCtrlDlg::InitializeTreeControl()
{
    // Set tree control styles
    DWORD dwStyle = m_treeCtrl.GetStyle();
    dwStyle |= TVS_HASBUTTONS;      // Show +/- buttons
    dwStyle |= TVS_HASLINES;        // Show lines
    dwStyle |= TVS_LINESATROOT;     // Show lines at root
    dwStyle |= TVS_SHOWSELALWAYS;   // Keep selection visible

    // Create image list
    m_imageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 10, 10);

    // Load icons
    HICON hIconFolder = AfxGetApp()->LoadIcon(IDI_FOLDER);
    HICON hIconFile = AfxGetApp()->LoadIcon(IDI_FILE);
    HICON hIconOpen = AfxGetApp()->LoadIcon(IDI_FOLDER_OPEN);

    m_imageList.Add(hIconFolder);
    m_imageList.Add(hIconFile);
    m_imageList.Add(hIconOpen);

    DestroyIcon(hIconFolder);
    DestroyIcon(hIconFile);
    DestroyIcon(hIconOpen);

    // Associate image list with tree
    m_treeCtrl.SetImageList(&m_imageList, TVSIL_NORMAL);
}
```

---

## 3. Adding and Manipulating Tree Items

### Inserting Tree Items

```cpp
void CTreeCtrlDlg::CreateSampleTree()
{
    // Insert root item
    HTREEITEM hRoot = m_treeCtrl.InsertItem(_T("My Computer"), 0, 0, TVI_ROOT);

    // Insert child items
    HTREEITEM hDriveC = m_treeCtrl.InsertItem(_T("C:"), 0, 2, hRoot);
    HTREEITEM hDriveD = m_treeCtrl.InsertItem(_T("D:"), 0, 2, hRoot);

    // Insert nested items
    HTREEITEM hWindows = m_treeCtrl.InsertItem(_T("Windows"), 0, 2, hDriveC);
    HTREEITEM hProgram = m_treeCtrl.InsertItem(_T("Program Files"), 0, 2, hDriveC);
    HTREEITEM hUsers = m_treeCtrl.InsertItem(_T("Users"), 0, 2, hDriveC);

    // Insert files
    m_treeCtrl.InsertItem(_T("system32"), 0, 2, hWindows);
    m_treeCtrl.InsertItem(_T("notepad.exe"), 1, 1, hWindows);

    // Expand root
    m_treeCtrl.Expand(hRoot, TVE_EXPAND);
    m_treeCtrl.Expand(hDriveC, TVE_EXPAND);
}

// Advanced insertion with TVINSERTSTRUCT
HTREEITEM CTreeCtrlDlg::InsertItemEx(const CString& strText, int nImage,
    int nSelectedImage, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
    TVINSERTSTRUCT tvInsert;
    tvInsert.hParent = hParent;
    tvInsert.hInsertAfter = hInsertAfter;
    tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    tvInsert.item.pszText = const_cast<LPTSTR>((LPCTSTR)strText);
    tvInsert.item.iImage = nImage;
    tvInsert.item.iSelectedImage = nSelectedImage;

    return m_treeCtrl.InsertItem(&tvInsert);
}

// Insert with custom data
HTREEITEM CTreeCtrlDlg::InsertItemWithData(const CString& strText,
    DWORD_PTR dwData, HTREEITEM hParent)
{
    TVINSERTSTRUCT tvInsert;
    tvInsert.hParent = hParent;
    tvInsert.hInsertAfter = TVI_LAST;
    tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
    tvInsert.item.pszText = const_cast<LPTSTR>((LPCTSTR)strText);
    tvInsert.item.iImage = 0;
    tvInsert.item.iSelectedImage = 0;
    tvInsert.item.lParam = dwData;

    return m_treeCtrl.InsertItem(&tvInsert);
}

// Delete item
void CTreeCtrlDlg::DeleteTreeItem(HTREEITEM hItem)
{
    if (hItem)
    {
        m_treeCtrl.DeleteItem(hItem);
    }
}

// Delete all items
void CTreeCtrlDlg::ClearTree()
{
    m_treeCtrl.DeleteAllItems();
}

// Get item text
CString CTreeCtrlDlg::GetItemText(HTREEITEM hItem)
{
    return m_treeCtrl.GetItemText(hItem);
}

// Set item text
void CTreeCtrlDlg::SetItemText(HTREEITEM hItem, const CString& strText)
{
    m_treeCtrl.SetItemText(hItem, strText);
}

// Get item data
DWORD_PTR CTreeCtrlDlg::GetItemData(HTREEITEM hItem)
{
    return m_treeCtrl.GetItemData(hItem);
}

// Set item data
void CTreeCtrlDlg::SetItemData(HTREEITEM hItem, DWORD_PTR dwData)
{
    m_treeCtrl.SetItemData(hItem, dwData);
}
```

---

## 4. Tree Item Images and Image Lists

### Managing Item Images

```cpp
class CTreeImageManager
{
private:
    CImageList m_imageList;
    CMap<CString, LPCTSTR, int, int> m_mapExtToImage;

public:
    CTreeImageManager()
    {
        InitializeImageList();
    }

    void InitializeImageList()
    {
        m_imageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 20, 10);

        // Load standard icons
        AddIcon(IDI_FOLDER_CLOSED, _T("folder"));
        AddIcon(IDI_FOLDER_OPEN, _T("folder_open"));
        AddIcon(IDI_FILE_GENERIC, _T("file"));

        // Map file extensions to icons
        m_mapExtToImage.SetAt(_T(".txt"), 3);
        m_mapExtToImage.SetAt(_T(".doc"), 4);
        m_mapExtToImage.SetAt(_T(".cpp"), 5);
        m_mapExtToImage.SetAt(_T(".h"), 6);
        m_mapExtToImage.SetAt(_T(".exe"), 7);
    }

    int AddIcon(UINT nIconID, const CString& strKey)
    {
        HICON hIcon = AfxGetApp()->LoadIcon(nIconID);
        if (!hIcon)
            return -1;

        int nIndex = m_imageList.Add(hIcon);
        DestroyIcon(hIcon);

        return nIndex;
    }

    int GetImageIndexForFile(const CString& strFileName)
    {
        int nDot = strFileName.ReverseFind(_T('.'));
        if (nDot < 0)
            return 2; // Generic file icon

        CString strExt = strFileName.Mid(nDot);
        strExt.MakeLower();

        int nImage = 2;
        m_mapExtToImage.Lookup(strExt, nImage);

        return nImage;
    }

    int GetFolderIcon(BOOL bOpen)
    {
        return bOpen ? 1 : 0;
    }

    CImageList* GetImageList()
    {
        return &m_imageList;
    }

    void SetTreeImageList(CTreeCtrl* pTreeCtrl)
    {
        pTreeCtrl->SetImageList(&m_imageList, TVSIL_NORMAL);
    }
};

// Usage
void CTreeCtrlDlg::SetItemIcon(HTREEITEM hItem, const CString& strFileName, BOOL bIsFolder)
{
    int nImage;

    if (bIsFolder)
    {
        nImage = 0; // Closed folder
        m_treeCtrl.SetItemImage(hItem, nImage, 2); // Open folder when expanded
    }
    else
    {
        nImage = m_imageManager.GetImageIndexForFile(strFileName);
        m_treeCtrl.SetItemImage(hItem, nImage, nImage);
    }
}
```

---

## 5. Tree Navigation and Selection

### Navigation Functions

```cpp
class CTreeNavigator
{
private:
    CTreeCtrl* m_pTreeCtrl;

public:
    CTreeNavigator(CTreeCtrl* pTreeCtrl) : m_pTreeCtrl(pTreeCtrl) {}

    // Get root item
    HTREEITEM GetRootItem()
    {
        return m_pTreeCtrl->GetRootItem();
    }

    // Get parent item
    HTREEITEM GetParentItem(HTREEITEM hItem)
    {
        return m_pTreeCtrl->GetParentItem(hItem);
    }

    // Get first child
    HTREEITEM GetChildItem(HTREEITEM hItem)
    {
        return m_pTreeCtrl->GetChildItem(hItem);
    }

    // Get next sibling
    HTREEITEM GetNextSiblingItem(HTREEITEM hItem)
    {
        return m_pTreeCtrl->GetNextSiblingItem(hItem);
    }

    // Get previous sibling
    HTREEITEM GetPrevSiblingItem(HTREEITEM hItem)
    {
        return m_pTreeCtrl->GetPrevSiblingItem(hItem);
    }

    // Get selected item
    HTREEITEM GetSelectedItem()
    {
        return m_pTreeCtrl->GetSelectedItem();
    }

    // Select item
    BOOL SelectItem(HTREEITEM hItem)
    {
        return m_pTreeCtrl->SelectItem(hItem);
    }

    // Get item path (from root to item)
    CString GetItemPath(HTREEITEM hItem, const CString& strSeparator = _T("\\"))
    {
        CString strPath;
        HTREEITEM hCurrent = hItem;

        while (hCurrent)
        {
            CString strText = m_pTreeCtrl->GetItemText(hCurrent);

            if (strPath.IsEmpty())
                strPath = strText;
            else
                strPath = strText + strSeparator + strPath;

            hCurrent = m_pTreeCtrl->GetParentItem(hCurrent);
        }

        return strPath;
    }

    // Find item by text
    HTREEITEM FindItem(HTREEITEM hStart, const CString& strText, BOOL bRecursive = TRUE)
    {
        if (!hStart)
            hStart = GetRootItem();

        while (hStart)
        {
            if (m_pTreeCtrl->GetItemText(hStart).CompareNoCase(strText) == 0)
                return hStart;

            if (bRecursive)
            {
                HTREEITEM hChild = GetChildItem(hStart);
                if (hChild)
                {
                    HTREEITEM hFound = FindItem(hChild, strText, TRUE);
                    if (hFound)
                        return hFound;
                }
            }

            hStart = GetNextSiblingItem(hStart);
        }

        return nullptr;
    }

    // Count children
    int GetChildCount(HTREEITEM hItem)
    {
        int nCount = 0;
        HTREEITEM hChild = GetChildItem(hItem);

        while (hChild)
        {
            nCount++;
            hChild = GetNextSiblingItem(hChild);
        }

        return nCount;
    }

    // Get level (depth) of item
    int GetItemLevel(HTREEITEM hItem)
    {
        int nLevel = 0;
        HTREEITEM hParent = m_pTreeCtrl->GetParentItem(hItem);

        while (hParent)
        {
            nLevel++;
            hParent = m_pTreeCtrl->GetParentItem(hParent);
        }

        return nLevel;
    }

    // Traverse entire tree
    void TraverseTree(HTREEITEM hItem, CArray<HTREEITEM, HTREEITEM>& arrItems)
    {
        if (!hItem)
            hItem = GetRootItem();

        while (hItem)
        {
            arrItems.Add(hItem);

            // Recursively traverse children
            HTREEITEM hChild = GetChildItem(hItem);
            if (hChild)
                TraverseTree(hChild, arrItems);

            hItem = GetNextSiblingItem(hItem);
        }
    }
};

// Selection notification
void CTreeCtrlDlg::OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

    HTREEITEM hNewItem = pNMTreeView->itemNew.hItem;

    if (hNewItem)
    {
        CString strText = m_treeCtrl.GetItemText(hNewItem);
        CString strPath = m_navigator.GetItemPath(hNewItem);

        // Display selection info
        CString strMsg;
        strMsg.Format(_T("Selected: %s\nPath: %s"), strText, strPath);
        SetDlgItemText(IDC_STATIC_INFO, strMsg);
    }

    *pResult = 0;
}
```

---

## 6. Expanding and Collapsing Nodes

### Expansion Management

```cpp
class CTreeExpander
{
private:
    CTreeCtrl* m_pTreeCtrl;

public:
    CTreeExpander(CTreeCtrl* pTreeCtrl) : m_pTreeCtrl(pTreeCtrl) {}

    // Expand item
    BOOL Expand(HTREEITEM hItem, UINT nCode = TVE_EXPAND)
    {
        // nCode: TVE_EXPAND, TVE_COLLAPSE, TVE_TOGGLE
        return m_pTreeCtrl->Expand(hItem, nCode);
    }

    // Expand all children
    void ExpandAll(HTREEITEM hItem)
    {
        if (!hItem)
            hItem = m_pTreeCtrl->GetRootItem();

        while (hItem)
        {
            m_pTreeCtrl->Expand(hItem, TVE_EXPAND);

            // Expand children
            HTREEITEM hChild = m_pTreeCtrl->GetChildItem(hItem);
            if (hChild)
                ExpandAll(hChild);

            hItem = m_pTreeCtrl->GetNextSiblingItem(hItem);
        }
    }

    // Collapse all
    void CollapseAll(HTREEITEM hItem)
    {
        if (!hItem)
            hItem = m_pTreeCtrl->GetRootItem();

        while (hItem)
        {
            // Collapse children first
            HTREEITEM hChild = m_pTreeCtrl->GetChildItem(hItem);
            if (hChild)
                CollapseAll(hChild);

            m_pTreeCtrl->Expand(hItem, TVE_COLLAPSE);

            hItem = m_pTreeCtrl->GetNextSiblingItem(hItem);
        }
    }

    // Toggle expansion
    void Toggle(HTREEITEM hItem)
    {
        m_pTreeCtrl->Expand(hItem, TVE_TOGGLE);
    }

    // Check if item is expanded
    BOOL IsExpanded(HTREEITEM hItem)
    {
        UINT nState = m_pTreeCtrl->GetItemState(hItem, TVIS_EXPANDED);
        return (nState & TVIS_EXPANDED) != 0;
    }

    // Expand to level
    void ExpandToLevel(int nLevel, HTREEITEM hItem = nullptr, int nCurrentLevel = 0)
    {
        if (!hItem)
            hItem = m_pTreeCtrl->GetRootItem();

        while (hItem)
        {
            if (nCurrentLevel < nLevel)
            {
                m_pTreeCtrl->Expand(hItem, TVE_EXPAND);

                HTREEITEM hChild = m_pTreeCtrl->GetChildItem(hItem);
                if (hChild)
                    ExpandToLevel(nLevel, hChild, nCurrentLevel + 1);
            }
            else
            {
                m_pTreeCtrl->Expand(hItem, TVE_COLLAPSE);
            }

            hItem = m_pTreeCtrl->GetNextSiblingItem(hItem);
        }
    }
};

// Item expanding notification
void CTreeCtrlDlg::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

    HTREEITEM hItem = pNMTreeView->itemNew.hItem;

    if (pNMTreeView->action == TVE_EXPAND)
    {
        // Item is being expanded
        // Update icon to "open folder"
        int nImage, nSelectedImage;
        m_treeCtrl.GetItemImage(hItem, nImage, nSelectedImage);

        if (nImage == 0) // Closed folder icon
        {
            m_treeCtrl.SetItemImage(hItem, 2, 2); // Open folder icon
        }

        // Lazy load children if needed
        LoadChildItems(hItem);
    }
    else if (pNMTreeView->action == TVE_COLLAPSE)
    {
        // Item is being collapsed
        // Update icon to "closed folder"
        m_treeCtrl.SetItemImage(hItem, 0, 0);
    }

    *pResult = 0;
}

void CTreeCtrlDlg::LoadChildItems(HTREEITEM hItem)
{
    // Check if children already loaded
    if (m_treeCtrl.GetChildItem(hItem))
        return;

    // Load children (e.g., from database or file system)
    // This is called "lazy loading" or "on-demand loading"

    CString strPath = GetItemPath(hItem);

    // Example: Load subdirectories
    CFileFind finder;
    BOOL bWorking = finder.FindFile(strPath + _T("\\*.*"));

    while (bWorking)
    {
        bWorking = finder.FindNextFile();

        if (finder.IsDirectory() && !finder.IsDots())
        {
            CString strName = finder.GetFileName();
            m_treeCtrl.InsertItem(strName, 0, 0, hItem);
        }
    }
}
```

---

## 7. Drag and Drop in Trees

### Tree Drag and Drop Implementation

```cpp
class CDragDropTreeCtrl : public CTreeCtrl
{
private:
    BOOL m_bDragging;
    HTREEITEM m_hDragItem;
    HTREEITEM m_hDropItem;
    CImageList* m_pDragImage;

public:
    CDragDropTreeCtrl()
        : m_bDragging(FALSE)
        , m_hDragItem(nullptr)
        , m_hDropItem(nullptr)
        , m_pDragImage(nullptr)
    {
    }

protected:
    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
    BOOL CanDropOn(HTREEITEM hDropItem, HTREEITEM hDragItem);
    void MoveTreeItem(HTREEITEM hDragItem, HTREEITEM hDropItem);
    HTREEITEM CopyTreeItem(HTREEITEM hItem, HTREEITEM hParent, HTREEITEM hInsertAfter);
};

BEGIN_MESSAGE_MAP(CDragDropTreeCtrl, CTreeCtrl)
    ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CDragDropTreeCtrl::OnBeginDrag)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_TIMER()
END_MESSAGE_MAP()

void CDragDropTreeCtrl::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

    m_hDragItem = pNMTreeView->itemNew.hItem;

    // Create drag image
    m_pDragImage = CreateDragImage(m_hDragItem);

    if (m_pDragImage)
    {
        m_pDragImage->BeginDrag(0, CPoint(0, 0));
        m_pDragImage->DragEnter(this, pNMTreeView->ptDrag);

        m_bDragging = TRUE;
        SetCapture();

        // Start auto-scroll timer
        SetTimer(1, 50, nullptr);
    }

    *pResult = 0;
}

void CDragDropTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bDragging && m_pDragImage)
    {
        // Move drag image
        m_pDragImage->DragMove(point);

        // Highlight drop target
        m_pDragImage->DragShowNolock(FALSE);

        UINT uFlags;
        HTREEITEM hDropItem = HitTest(point, &uFlags);

        if (hDropItem && CanDropOn(hDropItem, m_hDragItem))
        {
            SelectDropTarget(hDropItem);
            m_hDropItem = hDropItem;

            // Expand item if hovering over it
            SetTimer(2, 800, nullptr);
        }
        else
        {
            SelectDropTarget(nullptr);
            m_hDropItem = nullptr;
        }

        m_pDragImage->DragShowNolock(TRUE);
    }

    CTreeCtrl::OnMouseMove(nFlags, point);
}

void CDragDropTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bDragging)
    {
        m_pDragImage->DragLeave(this);
        m_pDragImage->EndDrag();
        delete m_pDragImage;
        m_pDragImage = nullptr;

        ReleaseCapture();
        m_bDragging = FALSE;

        KillTimer(1);
        KillTimer(2);

        // Perform drop operation
        if (m_hDropItem && m_hDragItem &&
            CanDropOn(m_hDropItem, m_hDragItem))
        {
            MoveTreeItem(m_hDragItem, m_hDropItem);
        }

        SelectDropTarget(nullptr);
        m_hDragItem = nullptr;
        m_hDropItem = nullptr;
    }

    CTreeCtrl::OnLButtonUp(nFlags, point);
}

void CDragDropTreeCtrl::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1) // Auto-scroll timer
    {
        CPoint pt;
        GetCursorPos(&pt);
        ScreenToClient(&pt);

        CRect rect;
        GetClientRect(&rect);

        // Scroll if near edges
        if (pt.y < rect.top + 10)
        {
            SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0));
        }
        else if (pt.y > rect.bottom - 10)
        {
            SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0));
        }
    }
    else if (nIDEvent == 2) // Expand timer
    {
        if (m_hDropItem)
        {
            Expand(m_hDropItem, TVE_EXPAND);
        }
        KillTimer(2);
    }

    CTreeCtrl::OnTimer(nIDEvent);
}

BOOL CDragDropTreeCtrl::CanDropOn(HTREEITEM hDropItem, HTREEITEM hDragItem)
{
    // Cannot drop on itself
    if (hDropItem == hDragItem)
        return FALSE;

    // Cannot drop on descendants
    HTREEITEM hParent = hDropItem;
    while (hParent)
    {
        if (hParent == hDragItem)
            return FALSE;
        hParent = GetParentItem(hParent);
    }

    return TRUE;
}

void CDragDropTreeCtrl::MoveTreeItem(HTREEITEM hDragItem, HTREEITEM hDropItem)
{
    // Copy item and all children to new location
    HTREEITEM hNewItem = CopyTreeItem(hDragItem, hDropItem, TVI_LAST);

    // Delete original item
    DeleteItem(hDragItem);

    // Select new item
    SelectItem(hNewItem);
    EnsureVisible(hNewItem);
}

HTREEITEM CDragDropTreeCtrl::CopyTreeItem(HTREEITEM hItem,
    HTREEITEM hParent, HTREEITEM hInsertAfter)
{
    // Get item data
    TVITEM tvItem;
    tvItem.hItem = hItem;
    tvItem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

    CString strText = GetItemText(hItem);
    tvItem.pszText = strText.GetBuffer();
    tvItem.cchTextMax = strText.GetLength();

    GetItem(&tvItem);
    strText.ReleaseBuffer();

    // Insert copy
    TVINSERTSTRUCT tvInsert;
    tvInsert.hParent = hParent;
    tvInsert.hInsertAfter = hInsertAfter;
    tvInsert.item = tvItem;
    tvInsert.item.pszText = strText.GetBuffer();

    HTREEITEM hNewItem = InsertItem(&tvInsert);
    strText.ReleaseBuffer();

    // Copy children recursively
    HTREEITEM hChild = GetChildItem(hItem);
    while (hChild)
    {
        CopyTreeItem(hChild, hNewItem, TVI_LAST);
        hChild = GetNextSiblingItem(hChild);
    }

    return hNewItem;
}
```

---

## 8. Custom Drawing and Styling

### Custom Tree Item Drawing

```cpp
class CCustomDrawTreeCtrl : public CTreeCtrl
{
protected:
    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);

private:
    void DrawTreeItem(CDC* pDC, HTREEITEM hItem, CRect rect, BOOL bSelected);
};

BEGIN_MESSAGE_MAP(CCustomDrawTreeCtrl, CTreeCtrl)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CCustomDrawTreeCtrl::OnCustomDraw)
END_MESSAGE_MAP()

void CCustomDrawTreeCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMTVCUSTOMDRAW pTVCD = reinterpret_cast<LPNMTVCUSTOMDRAW>(pNMHDR);

    *pResult = CDRF_DODEFAULT;

    switch (pTVCD->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
        *pResult = CDRF_NOTIFYITEMDRAW;
        break;

    case CDDS_ITEMPREPAINT:
        {
            HTREEITEM hItem = (HTREEITEM)pTVCD->nmcd.dwItemSpec;
            BOOL bSelected = (pTVCD->nmcd.uItemState & CDIS_SELECTED);

            // Get item level for custom coloring
            int nLevel = 0;
            HTREEITEM hParent = GetParentItem(hItem);
            while (hParent)
            {
                nLevel++;
                hParent = GetParentItem(hParent);
            }

            // Custom colors based on level
            if (bSelected)
            {
                pTVCD->clrText = RGB(255, 255, 255);
                pTVCD->clrTextBk = RGB(0, 120, 215);
            }
            else
            {
                switch (nLevel)
                {
                case 0:
                    pTVCD->clrText = RGB(0, 0, 128);    // Dark blue
                    pTVCD->clrTextBk = RGB(240, 248, 255); // Alice blue
                    break;
                case 1:
                    pTVCD->clrText = RGB(0, 128, 0);    // Dark green
                    pTVCD->clrTextBk = RGB(240, 255, 240); // Honeydew
                    break;
                default:
                    pTVCD->clrText = RGB(0, 0, 0);      // Black
                    pTVCD->clrTextBk = RGB(255, 255, 255); // White
                    break;
                }
            }

            // Make root items bold
            if (nLevel == 0)
            {
                SelectObject(pTVCD->nmcd.hdc, GetStockObject(DEFAULT_GUI_FONT));
                LOGFONT lf;
                CFont* pFont = GetFont();
                pFont->GetLogFont(&lf);
                lf.lfWeight = FW_BOLD;
                CFont fontBold;
                fontBold.CreateFontIndirect(&lf);
                SelectObject(pTVCD->nmcd.hdc, fontBold.GetSafeHandle());
            }

            *pResult = CDRF_NEWFONT;
        }
        break;
    }
}
```

---

## 9. Tree Search and Filtering

### Tree Search Implementation

```cpp
class CTreeSearcher
{
private:
    CTreeCtrl* m_pTreeCtrl;
    CArray<HTREEITEM, HTREEITEM> m_arrResults;
    int m_nCurrentResult;

public:
    CTreeSearcher(CTreeCtrl* pTreeCtrl)
        : m_pTreeCtrl(pTreeCtrl)
        , m_nCurrentResult(-1)
    {
    }

    int Search(const CString& strSearch, BOOL bCaseSensitive = FALSE)
    {
        m_arrResults.RemoveAll();
        m_nCurrentResult = -1;

        SearchRecursive(m_pTreeCtrl->GetRootItem(), strSearch, bCaseSensitive);

        return (int)m_arrResults.GetSize();
    }

    HTREEITEM GetNextResult()
    {
        if (m_arrResults.GetSize() == 0)
            return nullptr;

        m_nCurrentResult++;
        if (m_nCurrentResult >= m_arrResults.GetSize())
            m_nCurrentResult = 0;

        return m_arrResults[m_nCurrentResult];
    }

    HTREEITEM GetPrevResult()
    {
        if (m_arrResults.GetSize() == 0)
            return nullptr;

        m_nCurrentResult--;
        if (m_nCurrentResult < 0)
            m_nCurrentResult = (int)m_arrResults.GetSize() - 1;

        return m_arrResults[m_nCurrentResult];
    }

    void HighlightResults()
    {
        // Clear all highlights
        HTREEITEM hItem = m_pTreeCtrl->GetRootItem();
        ClearHighlights(hItem);

        // Highlight results
        for (int i = 0; i < m_arrResults.GetSize(); i++)
        {
            m_pTreeCtrl->SetItemState(m_arrResults[i],
                TVIS_BOLD, TVIS_BOLD);
        }
    }

    void ClearHighlights(HTREEITEM hItem = nullptr)
    {
        if (!hItem)
            hItem = m_pTreeCtrl->GetRootItem();

        while (hItem)
        {
            m_pTreeCtrl->SetItemState(hItem, 0, TVIS_BOLD);

            HTREEITEM hChild = m_pTreeCtrl->GetChildItem(hItem);
            if (hChild)
                ClearHighlights(hChild);

            hItem = m_pTreeCtrl->GetNextSiblingItem(hItem);
        }
    }

private:
    void SearchRecursive(HTREEITEM hItem, const CString& strSearch,
                        BOOL bCaseSensitive)
    {
        while (hItem)
        {
            CString strText = m_pTreeCtrl->GetItemText(hItem);

            BOOL bMatch;
            if (bCaseSensitive)
                bMatch = (strText.Find(strSearch) >= 0);
            else
                bMatch = (strText.MakeUpper().Find(strSearch.MakeUpper()) >= 0);

            if (bMatch)
                m_arrResults.Add(hItem);

            // Search children
            HTREEITEM hChild = m_pTreeCtrl->GetChildItem(hItem);
            if (hChild)
                SearchRecursive(hChild, strSearch, bCaseSensitive);

            hItem = m_pTreeCtrl->GetNextSiblingItem(hItem);
        }
    }
};

// Usage in dialog
void CTreeCtrlDlg::OnBtnSearch()
{
    CString strSearch;
    m_editSearch.GetWindowText(strSearch);

    if (strSearch.IsEmpty())
    {
        m_searcher.ClearHighlights();
        return;
    }

    int nResults = m_searcher.Search(strSearch);

    if (nResults > 0)
    {
        m_searcher.HighlightResults();

        // Go to first result
        HTREEITEM hFirst = m_searcher.GetNextResult();
        if (hFirst)
        {
            m_treeCtrl.SelectItem(hFirst);
            m_treeCtrl.EnsureVisible(hFirst);
        }

        CString strMsg;
        strMsg.Format(_T("Found %d match(es)"), nResults);
        SetDlgItemText(IDC_STATIC_RESULTS, strMsg);
    }
    else
    {
        AfxMessageBox(_T("No matches found"));
    }
}

void CTreeCtrlDlg::OnBtnNext()
{
    HTREEITEM hNext = m_searcher.GetNextResult();
    if (hNext)
    {
        m_treeCtrl.SelectItem(hNext);
        m_treeCtrl.EnsureVisible(hNext);
    }
}
```

---

## 10. Complete File System Browser

### Full Implementation

```cpp
// FileSystemTreeDlg.h
class CFileSystemTreeDlg : public CDialogEx
{
private:
    CDragDropTreeCtrl m_treeFiles;
    CImageList m_imageList;
    CTreeSearcher m_searcher;
    CEdit m_editSearch;

    void PopulateTree();
    void AddDrive(const CString& strDrive);
    void LoadSubfolders(HTREEITEM hParent, const CString& strPath);
    HTREEITEM AddFolder(HTREEITEM hParent, const CString& strPath);

public:
    afx_msg void OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnRightClick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnOpenFolder();
    afx_msg void OnOpenCommandPrompt();
    afx_msg void OnRefresh();
};

void CFileSystemTreeDlg::PopulateTree()
{
    m_treeFiles.DeleteAllItems();

    // Get all drives
    DWORD dwDrives = GetLogicalDrives();

    for (int i = 0; i < 26; i++)
    {
        if (dwDrives & (1 << i))
        {
            CString strDrive;
            strDrive.Format(_T("%c:\\"), _T('A') + i);
            AddDrive(strDrive);
        }
    }
}

void CFileSystemTreeDlg::AddDrive(const CString& strDrive)
{
    CString strLabel = strDrive;

    // Get volume information
    TCHAR szVolumeName[MAX_PATH];
    if (GetVolumeInformation(strDrive, szVolumeName, MAX_PATH,
        nullptr, nullptr, nullptr, nullptr, 0))
    {
        if (_tcslen(szVolumeName) > 0)
        {
            strLabel.Format(_T("%s (%s)"), strDrive, szVolumeName);
        }
    }

    HTREEITEM hDrive = m_treeFiles.InsertItem(strLabel, 0, 0, TVI_ROOT);
    m_treeFiles.SetItemData(hDrive, (DWORD_PTR)new CString(strDrive));

    // Add placeholder for lazy loading
    m_treeFiles.InsertItem(_T(""), 0, 0, hDrive);
}

void CFileSystemTreeDlg::LoadSubfolders(HTREEITEM hParent, const CString& strPath)
{
    // Remove placeholder
    HTREEITEM hChild = m_treeFiles.GetChildItem(hParent);
    if (hChild)
    {
        CString strText = m_treeFiles.GetItemText(hChild);
        if (strText.IsEmpty())
        {
            m_treeFiles.DeleteItem(hChild);
        }
        else
        {
            return; // Already loaded
        }
    }

    // Load subfolders
    CFileFind finder;
    BOOL bWorking = finder.FindFile(strPath + _T("\\*.*"));

    while (bWorking)
    {
        bWorking = finder.FindNextFile();

        if (finder.IsDirectory() && !finder.IsDots() && !finder.IsHidden())
        {
            CString strSubPath = finder.GetFilePath();
            AddFolder(hParent, strSubPath);
        }
    }
}

HTREEITEM CFileSystemTreeDlg::AddFolder(HTREEITEM hParent, const CString& strPath)
{
    int nPos = strPath.ReverseFind(_T('\\'));
    CString strName = (nPos >= 0) ? strPath.Mid(nPos + 1) : strPath;

    HTREEITEM hFolder = m_treeFiles.InsertItem(strName, 0, 0, hParent);
    m_treeFiles.SetItemData(hFolder, (DWORD_PTR)new CString(strPath));

    // Add placeholder for lazy loading
    m_treeFiles.InsertItem(_T(""), 0, 0, hFolder);

    return hFolder;
}

void CFileSystemTreeDlg::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

    if (pNMTreeView->action == TVE_EXPAND)
    {
        HTREEITEM hItem = pNMTreeView->itemNew.hItem;
        CString* pPath = (CString*)m_treeFiles.GetItemData(hItem);

        if (pPath)
        {
            LoadSubfolders(hItem, *pPath);
        }
    }

    *pResult = 0;
}

void CFileSystemTreeDlg::OnRightClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    CPoint pt;
    GetCursorPos(&pt);

    CPoint ptClient = pt;
    m_treeFiles.ScreenToClient(&ptClient);

    UINT uFlags;
    HTREEITEM hItem = m_treeFiles.HitTest(ptClient, &uFlags);

    if (hItem)
    {
        m_treeFiles.SelectItem(hItem);

        CMenu menu;
        menu.CreatePopupMenu();
        menu.AppendMenu(MF_STRING, ID_OPEN_FOLDER, _T("&Open in Explorer"));
        menu.AppendMenu(MF_STRING, ID_COMMAND_PROMPT, _T("Open Command &Prompt"));
        menu.AppendMenu(MF_SEPARATOR);
        menu.AppendMenu(MF_STRING, ID_REFRESH, _T("&Refresh"));

        menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
    }

    *pResult = 0;
}

void CFileSystemTreeDlg::OnOpenFolder()
{
    HTREEITEM hItem = m_treeFiles.GetSelectedItem();
    if (!hItem)
        return;

    CString* pPath = (CString*)m_treeFiles.GetItemData(hItem);
    if (pPath)
    {
        ShellExecute(GetSafeHwnd(), _T("explore"), *pPath,
                    nullptr, nullptr, SW_SHOW);
    }
}

void CFileSystemTreeDlg::OnOpenCommandPrompt()
{
    HTREEITEM hItem = m_treeFiles.GetSelectedItem();
    if (!hItem)
        return;

    CString* pPath = (CString*)m_treeFiles.GetItemData(hItem);
    if (pPath)
    {
        ShellExecute(GetSafeHwnd(), _T("open"), _T("cmd.exe"),
                    nullptr, *pPath, SW_SHOW);
    }
}
```

---

## Key Takeaways

1. CTreeCtrl provides hierarchical data visualization
2. Lazy loading improves performance with large trees
3. Drag and drop enables intuitive reorganization
4. Custom drawing allows unique visual styles
5. Search functionality enhances usability
6. Image lists provide visual cues
7. Proper memory management prevents leaks

---

## Best Practices

1. Use lazy loading for file system trees
2. Implement drag and drop validation
3. Free LPARAM data when deleting items
4. Provide visual feedback during operations
5. Enable keyboard navigation
6. Cache frequently accessed data
7. Use custom drawing sparingly for performance

---

## Common Mistakes

### Mistake 1: Not Freeing Item Data
```cpp
// Wrong
m_treeCtrl.DeleteItem(hItem);

// Correct
CString* pData = (CString*)m_treeCtrl.GetItemData(hItem);
delete pData;
m_treeCtrl.DeleteItem(hItem);
```

### Mistake 2: Infinite Recursion
```cpp
// Wrong - Can cause stack overflow
void ExpandAll(HTREEITEM hItem)
{
    m_treeCtrl.Expand(hItem, TVE_EXPAND);
    HTREEITEM hChild = m_treeCtrl.GetChildItem(hItem);
    ExpandAll(hChild); // Missing null check!
}

// Correct
void ExpandAll(HTREEITEM hItem)
{
    if (!hItem) return;
    m_treeCtrl.Expand(hItem, TVE_EXPAND);
    HTREEITEM hChild = m_treeCtrl.GetChildItem(hItem);
    if (hChild) ExpandAll(hChild);
}
```

---

## Summary

In this lesson, you learned:
- Creating and initializing tree controls
- Adding and manipulating tree items
- Managing tree item images
- Navigation and selection
- Expanding and collapsing nodes
- Drag and drop operations
- Custom drawing
- Search functionality
- Building file system browsers

---

## Next Lesson Preview

In **Lesson 34**, we'll explore:
- Rich Edit Control fundamentals
- Text formatting and styling
- RTF document handling
- Find and replace operations
- Spell checking integration

**Excellent work with tree controls!**
