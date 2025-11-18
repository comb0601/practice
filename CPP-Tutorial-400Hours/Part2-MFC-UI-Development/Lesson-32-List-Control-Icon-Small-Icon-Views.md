# Lesson 32: List Control - Icon and Small Icon Views

**Duration**: 7 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Icon Views
2. Creating and Managing Image Lists
3. Icon View Implementation
4. Small Icon View Implementation
5. List View Mode
6. View Switching
7. Drag and Drop Operations
8. Custom Icon Rendering
9. Icon Spacing and Arrangement
10. Complete File Browser Example

---

## 1. Introduction to Icon Views

The CListCtrl supports four view modes:
- **LVS_ICON**: Large icons with labels below
- **LVS_SMALLICON**: Small icons with labels to the right
- **LVS_LIST**: Small icons in columns
- **LVS_REPORT**: Detailed list (covered in Lesson 31)

Icon views are commonly used for:
- File explorers and managers
- Photo galleries
- Application launchers
- Document browsers
- Icon-based navigation

---

## 2. Creating and Managing Image Lists

Image lists are essential for icon views. MFC provides the `CImageList` class.

### Basic Image List Operations

```cpp
// ImageListHelper.h
#pragma once

class CImageListHelper
{
public:
    static CImageList* CreateImageList(int cx, int cy, UINT nFlags = ILC_COLOR32 | ILC_MASK)
    {
        CImageList* pImageList = new CImageList();
        if (!pImageList->Create(cx, cy, nFlags, 10, 10))
        {
            delete pImageList;
            return nullptr;
        }
        return pImageList;
    }

    static int AddIcon(CImageList* pImageList, UINT nIconID)
    {
        HICON hIcon = AfxGetApp()->LoadIcon(nIconID);
        if (!hIcon)
            return -1;

        int nIndex = pImageList->Add(hIcon);
        DestroyIcon(hIcon);
        return nIndex;
    }

    static int AddBitmap(CImageList* pImageList, UINT nBitmapID, COLORREF crMask = RGB(255, 0, 255))
    {
        CBitmap bitmap;
        if (!bitmap.LoadBitmap(nBitmapID))
            return -1;

        int nIndex = pImageList->Add(&bitmap, crMask);
        return nIndex;
    }

    static int AddImageFromFile(CImageList* pImageList, const CString& strPath)
    {
        HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, strPath, IMAGE_BITMAP,
            0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (!hBitmap)
            return -1;

        CBitmap bitmap;
        bitmap.Attach(hBitmap);

        int nIndex = pImageList->Add(&bitmap, RGB(255, 0, 255));
        return nIndex;
    }

    static HICON ExtractFileIcon(const CString& strPath)
    {
        SHFILEINFO sfi;
        ZeroMemory(&sfi, sizeof(sfi));

        SHGetFileInfo(strPath, 0, &sfi, sizeof(sfi),
            SHGFI_ICON | SHGFI_LARGEICON);

        return sfi.hIcon;
    }

    static HICON ExtractSmallIcon(const CString& strPath)
    {
        SHFILEINFO sfi;
        ZeroMemory(&sfi, sizeof(sfi));

        SHGetFileInfo(strPath, 0, &sfi, sizeof(sfi),
            SHGFI_ICON | SHGFI_SMALLICON);

        return sfi.hIcon;
    }

    static int AddFileIcon(CImageList* pImageList, const CString& strPath, BOOL bLargeIcon = TRUE)
    {
        HICON hIcon = bLargeIcon ? ExtractFileIcon(strPath) : ExtractSmallIcon(strPath);
        if (!hIcon)
            return -1;

        int nIndex = pImageList->Add(hIcon);
        DestroyIcon(hIcon);
        return nIndex;
    }
};
```

---

## 3. Icon View Implementation

### Setting Up Icon View

```cpp
// IconListCtrlDlg.h
#pragma once
#include "afxcmn.h"

class CIconListCtrlDlg : public CDialogEx
{
public:
    CIconListCtrlDlg(CWnd* pParent = nullptr);
    virtual ~CIconListCtrlDlg();

    enum { IDD = IDD_ICONLIST_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

private:
    CListCtrl m_listCtrl;
    CImageList m_imageListLarge;
    CImageList m_imageListSmall;
    CComboBox m_comboView;

    void InitializeImageLists();
    void PopulateWithIcons();
    void SetViewMode(DWORD dwView);

public:
    afx_msg void OnViewChanged();
    afx_msg void OnItemActivate(NMHDR* pNMHDR, LRESULT* pResult);

    DECLARE_MESSAGE_MAP()
};

// IconListCtrlDlg.cpp
#include "stdafx.h"
#include "IconListCtrlDlg.h"

BEGIN_MESSAGE_MAP(CIconListCtrlDlg, CDialogEx)
    ON_CBN_SELCHANGE(IDC_COMBO_VIEW, &CIconListCtrlDlg::OnViewChanged)
    ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_MAIN, &CIconListCtrlDlg::OnItemActivate)
END_MESSAGE_MAP()

CIconListCtrlDlg::CIconListCtrlDlg(CWnd* pParent)
    : CDialogEx(IDD_ICONLIST_DIALOG, pParent)
{
}

CIconListCtrlDlg::~CIconListCtrlDlg()
{
    // Image lists are automatically destroyed
}

void CIconListCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_MAIN, m_listCtrl);
    DDX_Control(pDX, IDC_COMBO_VIEW, m_comboView);
}

BOOL CIconListCtrlDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    InitializeImageLists();
    PopulateWithIcons();

    // Setup view combo
    m_comboView.AddString(_T("Large Icons"));
    m_comboView.AddString(_T("Small Icons"));
    m_comboView.AddString(_T("List"));
    m_comboView.AddString(_T("Details"));
    m_comboView.SetCurSel(0);

    return TRUE;
}

void CIconListCtrlDlg::InitializeImageLists()
{
    // Create large icon image list (48x48)
    m_imageListLarge.Create(48, 48, ILC_COLOR32 | ILC_MASK, 10, 10);

    // Create small icon image list (16x16)
    m_imageListSmall.Create(16, 16, ILC_COLOR32 | ILC_MASK, 10, 10);

    // Load icons from resources
    UINT iconIDs[] = {
        IDI_FOLDER,
        IDI_DOCUMENT,
        IDI_IMAGE,
        IDI_VIDEO,
        IDI_MUSIC,
        IDI_EXECUTABLE,
        IDI_ARCHIVE,
        IDI_CODE
    };

    for (int i = 0; i < _countof(iconIDs); i++)
    {
        // Load large icon
        HICON hIconLarge = (HICON)LoadImage(
            AfxGetInstanceHandle(),
            MAKEINTRESOURCE(iconIDs[i]),
            IMAGE_ICON,
            48, 48,
            LR_DEFAULTCOLOR);

        if (hIconLarge)
        {
            m_imageListLarge.Add(hIconLarge);
            DestroyIcon(hIconLarge);
        }

        // Load small icon
        HICON hIconSmall = (HICON)LoadImage(
            AfxGetInstanceHandle(),
            MAKEINTRESOURCE(iconIDs[i]),
            IMAGE_ICON,
            16, 16,
            LR_DEFAULTCOLOR);

        if (hIconSmall)
        {
            m_imageListSmall.Add(hIconSmall);
            DestroyIcon(hIconSmall);
        }
    }

    // Associate image lists with list control
    m_listCtrl.SetImageList(&m_imageListLarge, LVSIL_NORMAL);
    m_listCtrl.SetImageList(&m_imageListSmall, LVSIL_SMALL);
}

void CIconListCtrlDlg::PopulateWithIcons()
{
    // Add items with icons
    struct FileItem
    {
        LPCTSTR pszName;
        int nIcon;
    };

    FileItem items[] = {
        { _T("Documents"), 0 },
        { _T("MyDocument.doc"), 1 },
        { _T("Photo.jpg"), 2 },
        { _T("Video.mp4"), 3 },
        { _T("Music.mp3"), 4 },
        { _T("Program.exe"), 5 },
        { _T("Archive.zip"), 6 },
        { _T("Source.cpp"), 7 },
        { _T("Pictures"), 0 },
        { _T("Downloads"), 0 }
    };

    for (int i = 0; i < _countof(items); i++)
    {
        LVITEM lvItem;
        ZeroMemory(&lvItem, sizeof(lvItem));

        lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
        lvItem.iItem = i;
        lvItem.iSubItem = 0;
        lvItem.pszText = const_cast<LPTSTR>(items[i].pszName);
        lvItem.iImage = items[i].nIcon;

        m_listCtrl.InsertItem(&lvItem);
    }
}

void CIconListCtrlDlg::SetViewMode(DWORD dwView)
{
    // Get current style
    DWORD dwStyle = m_listCtrl.GetStyle();

    // Remove view bits
    dwStyle &= ~LVS_TYPEMASK;

    // Add new view style
    dwStyle |= dwView;

    // Apply new style
    m_listCtrl.ModifyStyle(LVS_TYPEMASK, dwView);

    // Force redraw
    m_listCtrl.Invalidate();
}

void CIconListCtrlDlg::OnViewChanged()
{
    int nSel = m_comboView.GetCurSel();

    switch (nSel)
    {
    case 0: // Large Icons
        SetViewMode(LVS_ICON);
        break;

    case 1: // Small Icons
        SetViewMode(LVS_SMALLICON);
        break;

    case 2: // List
        SetViewMode(LVS_LIST);
        break;

    case 3: // Details
        SetViewMode(LVS_REPORT);
        // Add columns for report view
        if (m_listCtrl.GetHeaderCtrl()->GetItemCount() == 0)
        {
            m_listCtrl.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 200);
            m_listCtrl.InsertColumn(1, _T("Type"), LVCFMT_LEFT, 100);
            m_listCtrl.InsertColumn(2, _T("Size"), LVCFMT_RIGHT, 100);
        }
        break;
    }
}

void CIconListCtrlDlg::OnItemActivate(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

    int nItem = pNMIA->iItem;
    if (nItem >= 0)
    {
        CString strName = m_listCtrl.GetItemText(nItem, 0);
        CString strMsg;
        strMsg.Format(_T("Activated: %s"), strName);
        AfxMessageBox(strMsg);
    }

    *pResult = 0;
}
```

---

## 4. Small Icon View Implementation

### Icon Arrangement and Spacing

```cpp
class CCustomIconListCtrl : public CListCtrl
{
public:
    CCustomIconListCtrl() {}

    void SetIconSpacing(int cx, int cy)
    {
        // Set icon spacing for icon view
        SendMessage(LVM_SETICONSPACING, 0, MAKELONG(cx, cy));
    }

    void ArrangeIcons(UINT nCode)
    {
        // Arrange icons
        // LVA_DEFAULT, LVA_ALIGNLEFT, LVA_ALIGNTOP, LVA_SNAPTOGRID
        Arrange(nCode);
    }

    void AutoArrange(BOOL bEnable)
    {
        DWORD dwStyle = GetStyle();
        if (bEnable)
            dwStyle |= LVS_AUTOARRANGE;
        else
            dwStyle &= ~LVS_AUTOARRANGE;

        ModifyStyle(bEnable ? 0 : LVS_AUTOARRANGE,
                   bEnable ? LVS_AUTOARRANGE : 0);
    }

    CSize GetIconSpacing()
    {
        DWORD dwSpacing = (DWORD)SendMessage(LVM_GETICONSPACING);
        return CSize(LOWORD(dwSpacing), HIWORD(dwSpacing));
    }

    void AlignToGrid(BOOL bEnable)
    {
        ModifyStyle(bEnable ? 0 : LVS_ALIGNLEFT,
                   bEnable ? LVS_ALIGNLEFT : 0);
    }
};

// Usage example
void CIconListCtrlDlg::OnBtnCustomizeSpacing()
{
    CCustomIconListCtrl* pList = (CCustomIconListCtrl*)&m_listCtrl;

    // Set custom icon spacing (width, height in pixels)
    pList->SetIconSpacing(100, 80);

    // Enable auto-arrange
    pList->AutoArrange(TRUE);

    // Align to grid
    pList->AlignToGrid(TRUE);

    // Arrange icons
    pList->ArrangeIcons(LVA_ALIGNTOP | LVA_ALIGNLEFT);
}
```

---

## 5. List View Mode

List view displays items in columns with small icons.

```cpp
void CIconListCtrlDlg::SetupListView()
{
    // Set list view mode
    SetViewMode(LVS_LIST);

    // List view specific settings
    DWORD dwExStyle = m_listCtrl.GetExtendedStyle();
    dwExStyle |= LVS_EX_DOUBLEBUFFER;
    m_listCtrl.SetExtendedStyle(dwExStyle);

    // Set column width for list view
    m_listCtrl.SetColumnWidth(0, LVSCW_AUTOSIZE);
}
```

---

## 6. View Switching

### Advanced View Manager

```cpp
class CViewManager
{
private:
    CListCtrl* m_pListCtrl;
    DWORD m_dwCurrentView;
    CSize m_sizeIconSpacing;
    BOOL m_bAutoArrange;

public:
    CViewManager(CListCtrl* pListCtrl)
        : m_pListCtrl(pListCtrl)
        , m_dwCurrentView(LVS_ICON)
        , m_sizeIconSpacing(80, 80)
        , m_bAutoArrange(TRUE)
    {
    }

    void SetView(DWORD dwView)
    {
        if (!m_pListCtrl)
            return;

        // Save current view
        m_dwCurrentView = dwView;

        // Remove old view style
        DWORD dwStyle = m_pListCtrl->GetStyle();
        dwStyle &= ~LVS_TYPEMASK;

        // Set new view style
        m_pListCtrl->ModifyStyle(LVS_TYPEMASK, dwView);

        // Apply view-specific settings
        switch (dwView)
        {
        case LVS_ICON:
            ApplyIconViewSettings();
            break;

        case LVS_SMALLICON:
            ApplySmallIconViewSettings();
            break;

        case LVS_LIST:
            ApplyListViewSettings();
            break;

        case LVS_REPORT:
            ApplyReportViewSettings();
            break;
        }

        m_pListCtrl->Invalidate();
    }

    DWORD GetCurrentView() const
    {
        return m_dwCurrentView;
    }

    void CycleView()
    {
        DWORD dwNextView;

        switch (m_dwCurrentView)
        {
        case LVS_ICON:
            dwNextView = LVS_SMALLICON;
            break;
        case LVS_SMALLICON:
            dwNextView = LVS_LIST;
            break;
        case LVS_LIST:
            dwNextView = LVS_REPORT;
            break;
        case LVS_REPORT:
        default:
            dwNextView = LVS_ICON;
            break;
        }

        SetView(dwNextView);
    }

private:
    void ApplyIconViewSettings()
    {
        m_pListCtrl->SendMessage(LVM_SETICONSPACING, 0,
            MAKELONG(m_sizeIconSpacing.cx, m_sizeIconSpacing.cy));

        if (m_bAutoArrange)
        {
            m_pListCtrl->ModifyStyle(0, LVS_AUTOARRANGE);
            m_pListCtrl->Arrange(LVA_DEFAULT);
        }
    }

    void ApplySmallIconViewSettings()
    {
        if (m_bAutoArrange)
        {
            m_pListCtrl->ModifyStyle(0, LVS_AUTOARRANGE);
        }
    }

    void ApplyListViewSettings()
    {
        m_pListCtrl->SetColumnWidth(0, LVSCW_AUTOSIZE);
    }

    void ApplyReportViewSettings()
    {
        DWORD dwExStyle = m_pListCtrl->GetExtendedStyle();
        dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
        m_pListCtrl->SetExtendedStyle(dwExStyle);
    }
};

// Usage
void CIconListCtrlDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_F5) // F5 to cycle views
    {
        m_viewManager.CycleView();
    }

    CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}
```

---

## 7. Drag and Drop Operations

### Implementing Drag and Drop

```cpp
class CDragDropListCtrl : public CListCtrl
{
private:
    BOOL m_bDragging;
    int m_nDragItem;
    CImageList* m_pDragImage;
    CPoint m_ptHotSpot;

public:
    CDragDropListCtrl()
        : m_bDragging(FALSE)
        , m_nDragItem(-1)
        , m_pDragImage(nullptr)
    {
    }

protected:
    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

BEGIN_MESSAGE_MAP(CDragDropListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(LVN_BEGINDRAG, &CDragDropListCtrl::OnBeginDrag)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CDragDropListCtrl::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    m_nDragItem = pNMLV->iItem;

    // Create drag image
    CPoint pt(0, 0);
    m_pDragImage = CreateDragImage(m_nDragItem, &pt);

    if (m_pDragImage)
    {
        m_ptHotSpot = pt;

        // Start dragging
        m_pDragImage->BeginDrag(0, pt);
        m_pDragImage->DragEnter(this, pNMLV->ptAction);

        m_bDragging = TRUE;
        SetCapture();
    }

    *pResult = 0;
}

void CDragDropListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bDragging && m_pDragImage)
    {
        // Move drag image
        m_pDragImage->DragMove(point);

        // Highlight drop target
        m_pDragImage->DragShowNolock(FALSE);

        UINT uFlags;
        int nDropItem = HitTest(point, &uFlags);

        if (nDropItem != -1 && nDropItem != m_nDragItem)
        {
            // Highlight drop target
            SetItemState(nDropItem, LVIS_DROPHILITED, LVIS_DROPHILITED);
        }

        m_pDragImage->DragShowNolock(TRUE);
    }

    CListCtrl::OnMouseMove(nFlags, point);
}

void CDragDropListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bDragging)
    {
        // End dragging
        m_pDragImage->DragLeave(this);
        m_pDragImage->EndDrag();
        delete m_pDragImage;
        m_pDragImage = nullptr;

        ReleaseCapture();
        m_bDragging = FALSE;

        // Get drop target
        UINT uFlags;
        int nDropItem = HitTest(point, &uFlags);

        if (nDropItem != -1 && nDropItem != m_nDragItem)
        {
            // Perform drop operation
            MoveItem(m_nDragItem, nDropItem);
        }

        // Clear all drop highlights
        int nCount = GetItemCount();
        for (int i = 0; i < nCount; i++)
        {
            SetItemState(i, 0, LVIS_DROPHILITED);
        }
    }

    CListCtrl::OnLButtonUp(nFlags, point);
}

void CDragDropListCtrl::MoveItem(int nFrom, int nTo)
{
    if (nFrom == nTo)
        return;

    // Get item data
    CString strText = GetItemText(nFrom, 0);
    int nImage = GetItemImage(nFrom, 0);
    DWORD_PTR dwData = GetItemData(nFrom);

    // Delete source item
    DeleteItem(nFrom);

    // Adjust target index if necessary
    if (nFrom < nTo)
        nTo--;

    // Insert at new position
    LVITEM lvItem;
    ZeroMemory(&lvItem, sizeof(lvItem));

    lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
    lvItem.iItem = nTo;
    lvItem.pszText = const_cast<LPTSTR>((LPCTSTR)strText);
    lvItem.iImage = nImage;
    lvItem.lParam = dwData;

    InsertItem(&lvItem);

    // Select moved item
    SetItemState(nTo, LVIS_SELECTED | LVIS_FOCUSED,
                 LVIS_SELECTED | LVIS_FOCUSED);
    EnsureVisible(nTo, FALSE);
}
```

---

## 8. Custom Icon Rendering

### Owner-Draw List Control

```cpp
class COwnerDrawListCtrl : public CListCtrl
{
protected:
    DECLARE_MESSAGE_MAP()

public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

private:
    void DrawIconViewItem(CDC* pDC, int nItem, CRect rect, BOOL bSelected);
    void DrawSmallIconItem(CDC* pDC, int nItem, CRect rect, BOOL bSelected);
};

BEGIN_MESSAGE_MAP(COwnerDrawListCtrl, CListCtrl)
END_MESSAGE_MAP()

void COwnerDrawListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
    if (lpDIS->itemID == -1)
        return;

    CDC* pDC = CDC::FromHandle(lpDIS->hDC);
    CRect rect = lpDIS->rcItem;
    int nItem = lpDIS->itemID;
    BOOL bSelected = (lpDIS->itemState & ODS_SELECTED);

    DWORD dwView = GetStyle() & LVS_TYPEMASK;

    switch (dwView)
    {
    case LVS_ICON:
        DrawIconViewItem(pDC, nItem, rect, bSelected);
        break;

    case LVS_SMALLICON:
    case LVS_LIST:
        DrawSmallIconItem(pDC, nItem, rect, bSelected);
        break;
    }
}

void COwnerDrawListCtrl::DrawIconViewItem(CDC* pDC, int nItem, CRect rect, BOOL bSelected)
{
    // Get item info
    CString strText = GetItemText(nItem, 0);
    int nImage = GetItemImage(nItem, 0);

    // Fill background
    if (bSelected)
    {
        pDC->FillSolidRect(rect, RGB(0, 120, 215));
        pDC->SetTextColor(RGB(255, 255, 255));
    }
    else
    {
        pDC->FillSolidRect(rect, RGB(255, 255, 255));
        pDC->SetTextColor(RGB(0, 0, 0));
    }

    // Draw icon
    CImageList* pImageList = GetImageList(LVSIL_NORMAL);
    if (pImageList && nImage >= 0)
    {
        int cx, cy;
        ImageList_GetIconSize(pImageList->GetSafeHandle(), &cx, &cy);

        int x = rect.left + (rect.Width() - cx) / 2;
        int y = rect.top + 5;

        pImageList->Draw(pDC, nImage, CPoint(x, y), ILD_TRANSPARENT);

        // Adjust rect for text
        rect.top = y + cy + 5;
    }

    // Draw text
    pDC->SetBkMode(TRANSPARENT);
    CFont* pOldFont = pDC->SelectObject(GetFont());

    pDC->DrawText(strText, rect,
        DT_CENTER | DT_WORDBREAK | DT_NOPREFIX | DT_END_ELLIPSIS);

    pDC->SelectObject(pOldFont);

    // Draw selection border
    if (bSelected)
    {
        CPen pen(PS_DOT, 1, RGB(255, 255, 255));
        CPen* pOldPen = pDC->SelectObject(&pen);
        pDC->SelectStockObject(NULL_BRUSH);
        pDC->Rectangle(lpDIS->rcItem);
        pDC->SelectObject(pOldPen);
    }
}

void COwnerDrawListCtrl::DrawSmallIconItem(CDC* pDC, int nItem, CRect rect, BOOL bSelected)
{
    // Get item info
    CString strText = GetItemText(nItem, 0);
    int nImage = GetItemImage(nItem, 0);

    // Fill background
    if (bSelected)
    {
        pDC->FillSolidRect(rect, RGB(0, 120, 215));
        pDC->SetTextColor(RGB(255, 255, 255));
    }
    else
    {
        pDC->FillSolidRect(rect, RGB(255, 255, 255));
        pDC->SetTextColor(RGB(0, 0, 0));
    }

    // Draw icon
    CImageList* pImageList = GetImageList(LVSIL_SMALL);
    if (pImageList && nImage >= 0)
    {
        int cx, cy;
        ImageList_GetIconSize(pImageList->GetSafeHandle(), &cx, &cy);

        int x = rect.left + 2;
        int y = rect.top + (rect.Height() - cy) / 2;

        pImageList->Draw(pDC, nImage, CPoint(x, y), ILD_TRANSPARENT);

        // Adjust rect for text
        rect.left = x + cx + 5;
    }

    // Draw text
    pDC->SetBkMode(TRANSPARENT);
    CFont* pOldFont = pDC->SelectObject(GetFont());

    pDC->DrawText(strText, rect,
        DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS);

    pDC->SelectObject(pOldFont);
}
```

---

## 9. Icon Spacing and Arrangement

### Advanced Arrangement Options

```cpp
class CIconArrangementManager
{
private:
    CListCtrl* m_pListCtrl;

public:
    CIconArrangementManager(CListCtrl* pListCtrl)
        : m_pListCtrl(pListCtrl)
    {
    }

    void SetCustomSpacing(int nHorizontal, int nVertical)
    {
        m_pListCtrl->SendMessage(LVM_SETICONSPACING, 0,
            MAKELONG(nHorizontal, nVertical));
    }

    CSize GetIconSpacing()
    {
        DWORD dwSpacing = (DWORD)m_pListCtrl->SendMessage(LVM_GETICONSPACING);
        return CSize(LOWORD(dwSpacing), HIWORD(dwSpacing));
    }

    void ArrangeDefault()
    {
        m_pListCtrl->Arrange(LVA_DEFAULT);
    }

    void ArrangeAlignLeft()
    {
        m_pListCtrl->Arrange(LVA_ALIGNLEFT);
    }

    void ArrangeAlignTop()
    {
        m_pListCtrl->Arrange(LVA_ALIGNTOP);
    }

    void ArrangeSnapToGrid()
    {
        m_pListCtrl->Arrange(LVA_SNAPTOGRID);
    }

    void EnableAutoArrange(BOOL bEnable)
    {
        DWORD dwStyle = m_pListCtrl->GetStyle();

        if (bEnable)
            m_pListCtrl->ModifyStyle(0, LVS_AUTOARRANGE);
        else
            m_pListCtrl->ModifyStyle(LVS_AUTOARRANGE, 0);
    }

    BOOL IsAutoArrange()
    {
        return (m_pListCtrl->GetStyle() & LVS_AUTOARRANGE) != 0;
    }

    CPoint GetItemPosition(int nItem)
    {
        CPoint pt;
        m_pListCtrl->GetItemPosition(nItem, &pt);
        return pt;
    }

    void SetItemPosition(int nItem, CPoint pt)
    {
        m_pListCtrl->SetItemPosition(nItem, pt);
    }

    void SortByPosition()
    {
        // Sort items by their position (left to right, top to bottom)
        int nCount = m_pListCtrl->GetItemCount();
        CArray<ItemPosition, ItemPosition&> items;

        for (int i = 0; i < nCount; i++)
        {
            ItemPosition item;
            item.nIndex = i;
            item.pt = GetItemPosition(i);
            items.Add(item);
        }

        // Simple bubble sort based on position
        for (int i = 0; i < items.GetSize() - 1; i++)
        {
            for (int j = 0; j < items.GetSize() - i - 1; j++)
            {
                if (ComparePosition(items[j].pt, items[j + 1].pt) > 0)
                {
                    ItemPosition temp = items[j];
                    items[j] = items[j + 1];
                    items[j + 1] = temp;
                }
            }
        }
    }

private:
    struct ItemPosition
    {
        int nIndex;
        CPoint pt;
    };

    int ComparePosition(CPoint pt1, CPoint pt2)
    {
        if (pt1.y < pt2.y) return -1;
        if (pt1.y > pt2.y) return 1;
        if (pt1.x < pt2.x) return -1;
        if (pt1.x > pt2.x) return 1;
        return 0;
    }
};
```

---

## 10. Complete File Browser Example

### Full-Featured File Browser

```cpp
// FileBrowserDlg.h
#pragma once

class CFileBrowserDlg : public CDialogEx
{
public:
    CFileBrowserDlg(CWnd* pParent = nullptr);

    enum { IDD = IDD_FILEBROWSER_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

private:
    CDragDropListCtrl m_listFiles;
    CImageList m_imageListLarge;
    CImageList m_imageListSmall;
    CString m_strCurrentPath;
    CComboBox m_comboPath;
    CComboBox m_comboView;

    void InitializeImageLists();
    void LoadFolder(const CString& strPath);
    void AddFileItem(const CString& strPath, const CString& strName, BOOL bIsFolder);
    int GetFileIconIndex(const CString& strPath, BOOL bLarge);
    void UpdatePathCombo();

public:
    afx_msg void OnItemActivate(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnViewChanged();
    afx_msg void OnGoUp();
    afx_msg void OnRefresh();
    afx_msg void OnPathChanged();

    DECLARE_MESSAGE_MAP()
};

// FileBrowserDlg.cpp
void CFileBrowserDlg::InitializeImageLists()
{
    m_imageListLarge.Create(48, 48, ILC_COLOR32 | ILC_MASK, 100, 50);
    m_imageListSmall.Create(16, 16, ILC_COLOR32 | ILC_MASK, 100, 50);

    m_listFiles.SetImageList(&m_imageListLarge, LVSIL_NORMAL);
    m_listFiles.SetImageList(&m_imageListSmall, LVSIL_SMALL);
}

void CFileBrowserDlg::LoadFolder(const CString& strPath)
{
    m_listFiles.DeleteAllItems();
    m_strCurrentPath = strPath;

    UpdatePathCombo();

    CFileFind finder;
    BOOL bWorking = finder.FindFile(strPath + _T("\\*.*"));

    while (bWorking)
    {
        bWorking = finder.FindNextFile();

        if (finder.IsDots())
            continue;

        CString strName = finder.GetFileName();
        CString strFullPath = finder.GetFilePath();
        BOOL bIsFolder = finder.IsDirectory();

        AddFileItem(strFullPath, strName, bIsFolder);
    }

    finder.Close();
}

void CFileBrowserDlg::AddFileItem(const CString& strPath, const CString& strName, BOOL bIsFolder)
{
    // Get or add icon
    int nLargeIcon = GetFileIconIndex(strPath, TRUE);
    int nSmallIcon = GetFileIconIndex(strPath, FALSE);

    // Add item
    LVITEM lvItem;
    ZeroMemory(&lvItem, sizeof(lvItem));

    lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
    lvItem.iItem = m_listFiles.GetItemCount();
    lvItem.pszText = const_cast<LPTSTR>((LPCTSTR)strName);
    lvItem.iImage = nLargeIcon;

    int nIndex = m_listFiles.InsertItem(&lvItem);

    // Store full path in item data
    CString* pPath = new CString(strPath);
    m_listFiles.SetItemData(nIndex, (DWORD_PTR)pPath);
}

int CFileBrowserDlg::GetFileIconIndex(const CString& strPath, BOOL bLarge)
{
    SHFILEINFO sfi;
    ZeroMemory(&sfi, sizeof(sfi));

    UINT uFlags = SHGFI_ICON | (bLarge ? SHGFI_LARGEICON : SHGFI_SMALLICON);
    SHGetFileInfo(strPath, 0, &sfi, sizeof(sfi), uFlags);

    if (sfi.hIcon)
    {
        int nIndex;
        if (bLarge)
            nIndex = m_imageListLarge.Add(sfi.hIcon);
        else
            nIndex = m_imageListSmall.Add(sfi.hIcon);

        DestroyIcon(sfi.hIcon);
        return nIndex;
    }

    return -1;
}

void CFileBrowserDlg::OnItemActivate(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

    int nItem = pNMIA->iItem;
    if (nItem < 0)
        return;

    CString* pPath = (CString*)m_listFiles.GetItemData(nItem);
    if (!pPath)
        return;

    // Check if it's a folder
    DWORD dwAttrib = GetFileAttributes(*pPath);
    if (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)
    {
        // Open folder
        LoadFolder(*pPath);
    }
    else
    {
        // Open file
        ShellExecute(GetSafeHwnd(), _T("open"), *pPath,
                    nullptr, nullptr, SW_SHOW);
    }

    *pResult = 0;
}

void CFileBrowserDlg::OnGoUp()
{
    int nPos = m_strCurrentPath.ReverseFind(_T('\\'));
    if (nPos > 0)
    {
        CString strParent = m_strCurrentPath.Left(nPos);
        LoadFolder(strParent);
    }
}

void CFileBrowserDlg::UpdatePathCombo()
{
    // Add current path to combo if not already there
    int nIndex = m_comboPath.FindStringExact(-1, m_strCurrentPath);
    if (nIndex == CB_ERR)
    {
        m_comboPath.InsertString(0, m_strCurrentPath);
        m_comboPath.SetCurSel(0);
    }
    else
    {
        m_comboPath.SetCurSel(nIndex);
    }
}
```

---

## Key Takeaways

1. Icon and Small Icon views provide visual file/item browsing
2. Image lists manage icons for list controls
3. Four view modes offer different data visualization
4. Drag and drop enhances user interaction
5. Custom drawing allows unique visual styles
6. Icon spacing and arrangement improve layout
7. System icons integrate with Windows shell

---

## Best Practices

1. Always destroy icons after adding to image list
2. Use system icons for file types
3. Implement view switching for flexibility
4. Enable double buffering to prevent flicker
5. Support drag and drop for intuitive UX
6. Cache file icons for performance
7. Free LPARAM data when deleting items

---

## Common Mistakes

### Mistake 1: Forgetting to Destroy Icons
```cpp
// Wrong
HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON));
m_imageList.Add(hIcon);

// Correct
HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON));
m_imageList.Add(hIcon);
DestroyIcon(hIcon);
```

### Mistake 2: Not Freeing Item Data
```cpp
// Wrong
m_listCtrl.DeleteAllItems();

// Correct
for (int i = 0; i < m_listCtrl.GetItemCount(); i++)
{
    CString* pData = (CString*)m_listCtrl.GetItemData(i);
    delete pData;
}
m_listCtrl.DeleteAllItems();
```

---

## Summary

In this lesson, you learned:
- Creating and managing image lists
- Implementing icon and small icon views
- View mode switching
- Drag and drop operations
- Custom icon rendering
- Icon spacing and arrangement
- Building a complete file browser

---

## Next Lesson Preview

In **Lesson 33**, we'll explore:
- Tree Control (CTreeCtrl) fundamentals
- Creating hierarchical data structures
- Node manipulation and traversal
- Custom tree item rendering
- Drag and drop in tree controls

**Great job mastering icon views!**
