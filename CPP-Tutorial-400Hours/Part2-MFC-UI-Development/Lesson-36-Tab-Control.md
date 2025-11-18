# Lesson 36: Tab Control

**Duration**: 7 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to Tab Controls
2. Creating Tab Controls (CTabCtrl)
3. Adding and Managing Tabs
4. Tab Item Images
5. Switching Between Tabs
6. Tabbed Dialog Implementation
7. Dynamic Tab Creation
8. Tab Styles and Customization
9. Tab Events and Notifications
10. Complete Tabbed Application Example

---

## 1. Introduction to Tab Controls

Tab controls organize multiple pages of content in a single window, commonly used for:
- Settings dialogs
- Multi-view applications
- Wizards
- Property sheets (alternative)
- Document browsers

### Tab Control Features:
- Multiple tabs with labels
- Icons on tabs
- Tooltips for tabs
- Focus indication
- Hot tracking
- Multiple row support

---

## 2. Creating Tab Controls (CTabCtrl)

### Basic Tab Control Setup

```cpp
// TabCtrlDlg.h
#pragma once
#include "afxcmn.h"

class CTabCtrlDlg : public CDialogEx
{
public:
    CTabCtrlDlg(CWnd* pParent = nullptr);

    enum { IDD = IDD_TABCTRL_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

private:
    CTabCtrl m_tabCtrl;
    CDialog* m_pCurrentPage;

    void InitializeTabControl();
    void CreateTabPages();

public:
    afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);

    DECLARE_MESSAGE_MAP()
};

// TabCtrlDlg.cpp
#include "stdafx.h"
#include "TabCtrlDlg.h"

BEGIN_MESSAGE_MAP(CTabCtrlDlg, CDialogEx)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CTabCtrlDlg::OnSelChange)
END_MESSAGE_MAP()

CTabCtrlDlg::CTabCtrlDlg(CWnd* pParent)
    : CDialogEx(IDD_TABCTRL_DIALOG, pParent)
    , m_pCurrentPage(nullptr)
{
}

void CTabCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB_MAIN, m_tabCtrl);
}

BOOL CTabCtrlDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    InitializeTabControl();
    CreateTabPages();

    return TRUE;
}

void CTabCtrlDlg::InitializeTabControl()
{
    // Set tab control styles
    DWORD dwStyle = m_tabCtrl.GetStyle();
    dwStyle |= TCS_HOTTRACK;    // Hot tracking
    dwStyle |= TCS_TOOLTIPS;    // Enable tooltips

    // Insert tabs
    m_tabCtrl.InsertItem(0, _T("General"));
    m_tabCtrl.InsertItem(1, _T("Advanced"));
    m_tabCtrl.InsertItem(2, _T("Network"));
    m_tabCtrl.InsertItem(3, _T("Security"));
}
```

---

## 3. Adding and Managing Tabs

### Tab Management Class

```cpp
class CTabManager
{
private:
    CTabCtrl* m_pTabCtrl;

public:
    CTabManager(CTabCtrl* pTabCtrl) : m_pTabCtrl(pTabCtrl) {}

    // Insert tab
    int InsertTab(int nItem, const CString& strText, int nImage = -1)
    {
        TCITEM tcItem;
        tcItem.mask = TCIF_TEXT;
        tcItem.pszText = const_cast<LPTSTR>((LPCTSTR)strText);

        if (nImage >= 0)
        {
            tcItem.mask |= TCIF_IMAGE;
            tcItem.iImage = nImage;
        }

        return m_pTabCtrl->InsertItem(nItem, &tcItem);
    }

    // Delete tab
    BOOL DeleteTab(int nItem)
    {
        return m_pTabCtrl->DeleteItem(nItem);
    }

    // Delete all tabs
    BOOL DeleteAllTabs()
    {
        return m_pTabCtrl->DeleteAllItems();
    }

    // Get tab count
    int GetTabCount()
    {
        return m_pTabCtrl->GetItemCount();
    }

    // Get/Set current selection
    int GetCurrentTab()
    {
        return m_pTabCtrl->GetCurSel();
    }

    void SetCurrentTab(int nItem)
    {
        m_pTabCtrl.SetCurSel(nItem);
    }

    // Get tab text
    CString GetTabText(int nItem)
    {
        TCITEM tcItem;
        TCHAR szText[256];

        tcItem.mask = TCIF_TEXT;
        tcItem.pszText = szText;
        tcItem.cchTextMax = 256;

        if (m_pTabCtrl->GetItem(nItem, &tcItem))
            return CString(szText);

        return _T("");
    }

    // Set tab text
    BOOL SetTabText(int nItem, const CString& strText)
    {
        TCITEM tcItem;
        tcItem.mask = TCIF_TEXT;
        tcItem.pszText = const_cast<LPTSTR>((LPCTSTR)strText);

        return m_pTabCtrl->SetItem(nItem, &tcItem);
    }

    // Get/Set tab data
    DWORD_PTR GetTabData(int nItem)
    {
        TCITEM tcItem;
        tcItem.mask = TCIF_PARAM;

        if (m_pTabCtrl->GetItem(nItem, &tcItem))
            return tcItem.lParam;

        return 0;
    }

    BOOL SetTabData(int nItem, DWORD_PTR dwData)
    {
        TCITEM tcItem;
        tcItem.mask = TCIF_PARAM;
        tcItem.lParam = dwData;

        return m_pTabCtrl->SetItem(nItem, &tcItem);
    }

    // Get display area
    CRect GetDisplayArea()
    {
        CRect rect;
        m_pTabCtrl->GetClientRect(&rect);
        m_pTabCtrl->AdjustRect(FALSE, &rect);
        return rect;
    }

    // Find tab by text
    int FindTab(const CString& strText)
    {
        int nCount = GetTabCount();

        for (int i = 0; i < nCount; i++)
        {
            if (GetTabText(i).CompareNoCase(strText) == 0)
                return i;
        }

        return -1;
    }
};
```

---

## 4. Tab Item Images

### Adding Icons to Tabs

```cpp
class CTabWithImages : public CDialogEx
{
private:
    CTabCtrl m_tabCtrl;
    CImageList m_imageList;

public:
    BOOL OnInitDialog()
    {
        CDialogEx::OnInitDialog();

        // Create image list
        m_imageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 4, 4);

        // Load icons
        HICON hIcon1 = AfxGetApp()->LoadIcon(IDI_GENERAL);
        HICON hIcon2 = AfxGetApp()->LoadIcon(IDI_ADVANCED);
        HICON hIcon3 = AfxGetApp()->LoadIcon(IDI_NETWORK);
        HICON hIcon4 = AfxGetApp()->LoadIcon(IDI_SECURITY);

        m_imageList.Add(hIcon1);
        m_imageList.Add(hIcon2);
        m_imageList.Add(hIcon3);
        m_imageList.Add(hIcon4);

        DestroyIcon(hIcon1);
        DestroyIcon(hIcon2);
        DestroyIcon(hIcon3);
        DestroyIcon(hIcon4);

        // Set image list
        m_tabCtrl.SetImageList(&m_imageList);

        // Insert tabs with icons
        TCITEM tcItem;
        tcItem.mask = TCIF_TEXT | TCIF_IMAGE;

        tcItem.pszText = _T("General");
        tcItem.iImage = 0;
        m_tabCtrl.InsertItem(0, &tcItem);

        tcItem.pszText = _T("Advanced");
        tcItem.iImage = 1;
        m_tabCtrl.InsertItem(1, &tcItem);

        tcItem.pszText = _T("Network");
        tcItem.iImage = 2;
        m_tabCtrl.InsertItem(2, &tcItem);

        tcItem.pszText = _T("Security");
        tcItem.iImage = 3;
        m_tabCtrl.InsertItem(3, &tcItem);

        return TRUE;
    }
};
```

---

## 5. Switching Between Tabs

### Page Management

```cpp
class CTabbedDialogBase : public CDialogEx
{
protected:
    CTabCtrl m_tabCtrl;
    CDialog* m_arrPages[10];
    int m_nPageCount;
    int m_nCurrentPage;

public:
    CTabbedDialogBase() : m_nPageCount(0), m_nCurrentPage(-1)
    {
        ZeroMemory(m_arrPages, sizeof(m_arrPages));
    }

    virtual ~CTabbedDialogBase()
    {
        // Delete all pages
        for (int i = 0; i < m_nPageCount; i++)
        {
            if (m_arrPages[i])
            {
                m_arrPages[i]->DestroyWindow();
                delete m_arrPages[i];
            }
        }
    }

protected:
    // Add page
    int AddPage(CDialog* pPage, const CString& strTitle, int nImage = -1)
    {
        if (m_nPageCount >= 10)
            return -1;

        m_arrPages[m_nPageCount] = pPage;

        // Insert tab
        TCITEM tcItem;
        tcItem.mask = TCIF_TEXT;
        tcItem.pszText = const_cast<LPTSTR>((LPCTSTR)strTitle);

        if (nImage >= 0)
        {
            tcItem.mask |= TCIF_IMAGE;
            tcItem.iImage = nImage;
        }

        int nIndex = m_tabCtrl.InsertItem(m_nPageCount, &tcItem);
        m_nPageCount++;

        return nIndex;
    }

    // Show page
    void ShowPage(int nPage)
    {
        if (nPage < 0 || nPage >= m_nPageCount)
            return;

        // Hide current page
        if (m_nCurrentPage >= 0 && m_arrPages[m_nCurrentPage])
        {
            m_arrPages[m_nCurrentPage]->ShowWindow(SW_HIDE);
        }

        // Show new page
        CDialog* pPage = m_arrPages[nPage];

        if (!pPage->GetSafeHwnd())
        {
            // Create page if not created
            CRect rect;
            m_tabCtrl.GetClientRect(&rect);
            m_tabCtrl.AdjustRect(FALSE, &rect);

            pPage->Create(pPage->IDD, &m_tabCtrl);
            pPage->MoveWindow(&rect);
        }

        pPage->ShowWindow(SW_SHOW);
        m_nCurrentPage = nPage;
    }

    // Selection change handler
    void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult)
    {
        int nPage = m_tabCtrl.GetCurSel();
        ShowPage(nPage);
        *pResult = 0;
    }
};
```

---

## 6. Tabbed Dialog Implementation

### Complete Tabbed Settings Dialog

```cpp
// Page dialogs
class CGeneralPage : public CDialogEx
{
public:
    enum { IDD = IDD_PAGE_GENERAL };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

private:
    CEdit m_editName;
    CComboBox m_comboLanguage;
    CButton m_chkAutoSave;

    DECLARE_MESSAGE_MAP()
};

class CAdvancedPage : public CDialogEx
{
public:
    enum { IDD = IDD_PAGE_ADVANCED };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

private:
    CSliderCtrl m_sliderQuality;
    CEdit m_editCacheSize;
    CButton m_chkDebugMode;

    DECLARE_MESSAGE_MAP()
};

class CNetworkPage : public CDialogEx
{
public:
    enum { IDD = IDD_PAGE_NETWORK };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

private:
    CIPAddressCtrl m_ipProxy;
    CEdit m_editPort;
    CButton m_chkUseProxy;

    DECLARE_MESSAGE_MAP()
};

// Main tabbed dialog
class CSettingsDlg : public CTabbedDialogBase
{
public:
    CSettingsDlg(CWnd* pParent = nullptr);

    enum { IDD = IDD_SETTINGS_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

private:
    CGeneralPage m_pageGeneral;
    CAdvancedPage m_pageAdvanced;
    CNetworkPage m_pageNetwork;

public:
    afx_msg void OnBtnOK();
    afx_msg void OnBtnApply();

    DECLARE_MESSAGE_MAP()
};

// Implementation
BEGIN_MESSAGE_MAP(CSettingsDlg, CTabbedDialogBase)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CTabbedDialogBase::OnSelChange)
    ON_BN_CLICKED(IDOK, &CSettingsDlg::OnBtnOK)
    ON_BN_CLICKED(IDC_BTN_APPLY, &CSettingsDlg::OnBtnApply)
END_MESSAGE_MAP()

CSettingsDlg::CSettingsDlg(CWnd* pParent)
    : CTabbedDialogBase(CSettingsDlg::IDD, pParent)
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
    CTabbedDialogBase::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB_MAIN, m_tabCtrl);
}

BOOL CSettingsDlg::OnInitDialog()
{
    CTabbedDialogBase::OnInitDialog();

    // Add pages
    AddPage(&m_pageGeneral, _T("General"), 0);
    AddPage(&m_pageAdvanced, _T("Advanced"), 1);
    AddPage(&m_pageNetwork, _T("Network"), 2);

    // Show first page
    m_tabCtrl.SetCurSel(0);
    ShowPage(0);

    return TRUE;
}

void CSettingsDlg::OnBtnOK()
{
    // Apply settings from all pages
    OnBtnApply();

    // Close dialog
    CDialogEx::OnOK();
}

void CSettingsDlg::OnBtnApply()
{
    // Update data from all pages
    for (int i = 0; i < m_nPageCount; i++)
    {
        if (m_arrPages[i] && m_arrPages[i]->GetSafeHwnd())
        {
            m_arrPages[i]->UpdateData(TRUE);
        }
    }

    // Save settings
    SaveSettings();

    AfxMessageBox(_T("Settings applied successfully!"));
}
```

---

## 7. Dynamic Tab Creation

### Runtime Tab Management

```cpp
class CDynamicTabDlg : public CDialogEx
{
private:
    CTabCtrl m_tabCtrl;
    CArray<CDialog*, CDialog*> m_arrPages;

public:
    // Add new tab at runtime
    int AddNewTab(const CString& strTitle, UINT nDialogID)
    {
        // Create new page
        CDialog* pPage = new CDialog(nDialogID, this);

        CRect rect;
        m_tabCtrl.GetClientRect(&rect);
        m_tabCtrl.AdjustRect(FALSE, &rect);

        if (!pPage->Create(nDialogID, &m_tabCtrl))
        {
            delete pPage;
            return -1;
        }

        pPage->MoveWindow(&rect);
        pPage->ShowWindow(SW_HIDE);

        // Add to array
        int nIndex = (int)m_arrPages.Add(pPage);

        // Insert tab
        TCITEM tcItem;
        tcItem.mask = TCIF_TEXT | TCIF_PARAM;
        tcItem.pszText = const_cast<LPTSTR>((LPCTSTR)strTitle);
        tcItem.lParam = (LPARAM)pPage;

        m_tabCtrl.InsertItem(nIndex, &tcItem);

        return nIndex;
    }

    // Remove tab
    void RemoveTab(int nIndex)
    {
        if (nIndex < 0 || nIndex >= m_arrPages.GetSize())
            return;

        // Get page
        CDialog* pPage = m_arrPages[nIndex];

        // Destroy page
        if (pPage)
        {
            pPage->DestroyWindow();
            delete pPage;
        }

        // Remove from array
        m_arrPages.RemoveAt(nIndex);

        // Delete tab
        m_tabCtrl.DeleteItem(nIndex);

        // Show another page if current was removed
        int nCurrent = m_tabCtrl.GetCurSel();
        if (nCurrent >= 0 && nCurrent < m_arrPages.GetSize())
        {
            m_arrPages[nCurrent]->ShowWindow(SW_SHOW);
        }
    }

    // Event handlers
    afx_msg void OnBtnAddTab();
    afx_msg void OnBtnRemoveTab();
};

void CDynamicTabDlg::OnBtnAddTab()
{
    CString strTitle;
    strTitle.Format(_T("Tab %d"), m_arrPages.GetSize() + 1);

    int nIndex = AddNewTab(strTitle, IDD_CUSTOM_PAGE);

    // Select new tab
    m_tabCtrl.SetCurSel(nIndex);
    ShowPage(nIndex);
}

void CDynamicTabDlg::OnBtnRemoveTab()
{
    int nCurrent = m_tabCtrl.GetCurSel();

    if (nCurrent >= 0)
    {
        if (AfxMessageBox(_T("Remove current tab?"),
            MB_YESNO | MB_ICONQUESTION) == IDYES)
        {
            RemoveTab(nCurrent);
        }
    }
}
```

---

## 8. Tab Styles and Customization

### Tab Control Styles

```cpp
class CTabStyleManager
{
private:
    CTabCtrl* m_pTabCtrl;

public:
    CTabStyleManager(CTabCtrl* pTabCtrl) : m_pTabCtrl(pTabCtrl) {}

    // Set button style
    void SetButtonStyle(BOOL bEnable)
    {
        if (bEnable)
            m_pTabCtrl->ModifyStyle(0, TCS_BUTTONS);
        else
            m_pTabCtrl->ModifyStyle(TCS_BUTTONS, 0);
    }

    // Set flat buttons
    void SetFlatButtons(BOOL bEnable)
    {
        if (bEnable)
            m_pTabCtrl->ModifyStyle(0, TCS_FLATBUTTONS);
        else
            m_pTabCtrl->ModifyStyle(TCS_FLATBUTTONS, 0);
    }

    // Set multi-line
    void SetMultiline(BOOL bEnable)
    {
        if (bEnable)
            m_pTabCtrl->ModifyStyle(TCS_SINGLELINE, TCS_MULTILINE);
        else
            m_pTabCtrl->ModifyStyle(TCS_MULTILINE, TCS_SINGLELINE);
    }

    // Set bottom alignment
    void SetBottom(BOOL bEnable)
    {
        if (bEnable)
            m_pTabCtrl->ModifyStyle(0, TCS_BOTTOM);
        else
            m_pTabCtrl->ModifyStyle(TCS_BOTTOM, 0);
    }

    // Set right alignment (vertical tabs)
    void SetRight(BOOL bEnable)
    {
        if (bEnable)
            m_pTabCtrl->ModifyStyle(0, TCS_VERTICAL | TCS_RIGHT);
        else
            m_pTabCtrl->ModifyStyle(TCS_VERTICAL | TCS_RIGHT, 0);
    }

    // Set hot tracking
    void SetHotTracking(BOOL bEnable)
    {
        if (bEnable)
            m_pTabCtrl->ModifyStyle(0, TCS_HOTTRACK);
        else
            m_pTabCtrl->ModifyStyle(TCS_HOTTRACK, 0);
    }

    // Set focus on tab select
    void SetFocusOnButtonDown(BOOL bEnable)
    {
        if (bEnable)
            m_pTabCtrl->ModifyStyle(0, TCS_FOCUSONBUTTONDOWN);
        else
            m_pTabCtrl->ModifyStyle(TCS_FOCUSONBUTTONDOWN, 0);
    }

    // Set fixed width
    void SetFixedWidth(BOOL bEnable)
    {
        if (bEnable)
            m_pTabCtrl->ModifyStyle(0, TCS_FIXEDWIDTH);
        else
            m_pTabCtrl->ModifyStyle(TCS_FIXEDWIDTH, 0);
    }

    // Set item size
    void SetItemSize(CSize size)
    {
        m_pTabCtrl->SetItemSize(size);
    }

    // Set padding
    void SetPadding(CSize size)
    {
        m_pTabCtrl->SetPadding(size);
    }

    // Set minimum tab width
    void SetMinTabWidth(int nWidth)
    {
        m_pTabCtrl->SetMinTabWidth(nWidth);
    }
};
```

---

## 9. Tab Events and Notifications

### Handling Tab Events

```cpp
class CTabEventHandler
{
public:
    // Selection changing (can cancel)
    static void OnSelChanging(CTabCtrl* pTabCtrl, NMHDR* pNMHDR, LRESULT* pResult)
    {
        int nCurrentTab = pTabCtrl->GetCurSel();

        // Validate current page before switching
        if (!ValidatePage(nCurrentTab))
        {
            *pResult = TRUE; // Cancel selection change
            return;
        }

        *pResult = FALSE; // Allow selection change
    }

    // Selection changed
    static void OnSelChange(CTabCtrl* pTabCtrl, NMHDR* pNMHDR, LRESULT* pResult)
    {
        int nNewTab = pTabCtrl->GetCurSel();

        // Update UI for new tab
        UpdatePage(nNewTab);

        *pResult = 0;
    }

    // Right click
    static void OnRClick(CTabCtrl* pTabCtrl, NMHDR* pNMHDR, LRESULT* pResult)
    {
        TCHITTESTINFO hti;
        GetCursorPos(&hti.pt);
        pTabCtrl->ScreenToClient(&hti.pt);

        int nTab = pTabCtrl->HitTest(&hti);

        if (nTab >= 0)
        {
            // Show context menu for tab
            ShowTabContextMenu(pTabCtrl, nTab);
        }

        *pResult = 0;
    }

    // Key down
    static void OnKeyDown(CTabCtrl* pTabCtrl, NMHDR* pNMHDR, LRESULT* pResult)
    {
        LPNMTCKEYDOWN pTCKeyDown = reinterpret_cast<LPNMTCKEYDOWN>(pNMHDR);

        switch (pTCKeyDown->wVKey)
        {
        case VK_DELETE:
            // Handle tab deletion
            break;

        case VK_INSERT:
            // Handle tab insertion
            break;
        }

        *pResult = 0;
    }

private:
    static BOOL ValidatePage(int nPage)
    {
        // Validate page data
        return TRUE;
    }

    static void UpdatePage(int nPage)
    {
        // Update page display
    }

    static void ShowTabContextMenu(CTabCtrl* pTabCtrl, int nTab)
    {
        CMenu menu;
        menu.CreatePopupMenu();

        menu.AppendMenu(MF_STRING, ID_TAB_RENAME, _T("&Rename"));
        menu.AppendMenu(MF_STRING, ID_TAB_CLOSE, _T("&Close"));
        menu.AppendMenu(MF_SEPARATOR);
        menu.AppendMenu(MF_STRING, ID_TAB_CLOSEALL, _T("Close &All"));

        CPoint pt;
        GetCursorPos(&pt);

        menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,
                           pTabCtrl->GetParent());
    }
};
```

---

## 10. Complete Tabbed Application Example

### Full-Featured Tabbed Editor

```cpp
class CTabbedEditorDlg : public CDialogEx
{
private:
    CTabCtrl m_tabCtrl;
    CImageList m_imageList;

    struct EditorPage
    {
        CRichEditCtrl* pEditor;
        CString strFileName;
        BOOL bModified;
    };

    CArray<EditorPage, EditorPage&> m_arrPages;

public:
    BOOL OnInitDialog()
    {
        CDialogEx::OnInitDialog();

        // Setup tab control
        m_imageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 2, 2);

        HICON hIconNew = AfxGetApp()->LoadIcon(IDI_NEW);
        HICON hIconModified = AfxGetApp()->LoadIcon(IDI_MODIFIED);

        m_imageList.Add(hIconNew);
        m_imageList.Add(hIconModified);

        DestroyIcon(hIconNew);
        DestroyIcon(hIconModified);

        m_tabCtrl.SetImageList(&m_imageList);

        // Add initial tab
        AddNewDocument();

        return TRUE;
    }

    void AddNewDocument()
    {
        EditorPage page;

        // Create editor
        page.pEditor = new CRichEditCtrl();

        CRect rect;
        m_tabCtrl.GetClientRect(&rect);
        m_tabCtrl.AdjustRect(FALSE, &rect);

        page.pEditor->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE |
                            ES_AUTOVSCROLL | WS_VSCROLL,
                            rect, &m_tabCtrl, 5000 + (UINT)m_arrPages.GetSize());

        page.strFileName = _T("");
        page.bModified = FALSE;

        m_arrPages.Add(page);

        // Add tab
        CString strTitle;
        strTitle.Format(_T("Untitled %d"), m_arrPages.GetSize());

        TCITEM tcItem;
        tcItem.mask = TCIF_TEXT | TCIF_IMAGE;
        tcItem.pszText = const_cast<LPTSTR>((LPCTSTR)strTitle);
        tcItem.iImage = 0; // New document icon

        int nIndex = m_tabCtrl.InsertItem((int)m_arrPages.GetSize() - 1, &tcItem);

        // Select new tab
        m_tabCtrl.SetCurSel(nIndex);
        ShowPage(nIndex);
    }

    void OpenDocument()
    {
        CFileDialog dlg(TRUE, _T("txt"), nullptr,
            OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
            _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

        if (dlg.DoModal() == IDOK)
        {
            AddNewDocument();

            int nIndex = (int)m_arrPages.GetSize() - 1;
            EditorPage& page = m_arrPages[nIndex];

            // Load file
            CFile file;
            if (file.Open(dlg.GetPathName(), CFile::modeRead))
            {
                CString strContent;
                UINT nLength = (UINT)file.GetLength();
                file.Read(strContent.GetBuffer(nLength), nLength);
                strContent.ReleaseBuffer(nLength);
                file.Close();

                page.pEditor->SetWindowText(strContent);
                page.strFileName = dlg.GetPathName();

                // Update tab title
                CString strTitle = GetFileName(dlg.GetPathName());
                SetTabText(nIndex, strTitle);
            }
        }
    }

    void SaveDocument(int nIndex)
    {
        if (nIndex < 0 || nIndex >= m_arrPages.GetSize())
            return;

        EditorPage& page = m_arrPages[nIndex];

        if (page.strFileName.IsEmpty())
        {
            SaveDocumentAs(nIndex);
            return;
        }

        // Save file
        CString strContent;
        page.pEditor->GetWindowText(strContent);

        CFile file;
        if (file.Open(page.strFileName, CFile::modeCreate | CFile::modeWrite))
        {
            file.Write((LPCTSTR)strContent, strContent.GetLength());
            file.Close();

            page.bModified = FALSE;
            UpdateTabIcon(nIndex);
        }
    }

    void CloseDocument(int nIndex)
    {
        if (nIndex < 0 || nIndex >= m_arrPages.GetSize())
            return;

        EditorPage& page = m_arrPages[nIndex];

        if (page.bModified)
        {
            CString strMsg;
            strMsg.Format(_T("Save changes to %s?"),
                GetTabText(nIndex));

            int nResult = AfxMessageBox(strMsg, MB_YESNOCANCEL | MB_ICONQUESTION);

            if (nResult == IDCANCEL)
                return;

            if (nResult == IDYES)
                SaveDocument(nIndex);
        }

        // Delete editor
        if (page.pEditor)
        {
            page.pEditor->DestroyWindow();
            delete page.pEditor;
        }

        // Remove from array
        m_arrPages.RemoveAt(nIndex);

        // Delete tab
        m_tabCtrl.DeleteItem(nIndex);

        // Show another page
        if (m_arrPages.GetSize() > 0)
        {
            int nNewIndex = min(nIndex, (int)m_arrPages.GetSize() - 1);
            m_tabCtrl.SetCurSel(nNewIndex);
            ShowPage(nNewIndex);
        }
    }

private:
    void UpdateTabIcon(int nIndex)
    {
        EditorPage& page = m_arrPages[nIndex];

        TCITEM tcItem;
        tcItem.mask = TCIF_IMAGE;
        tcItem.iImage = page.bModified ? 1 : 0;

        m_tabCtrl.SetItem(nIndex, &tcItem);
    }
};
```

---

## Key Takeaways

1. Tab controls organize multiple pages efficiently
2. Each tab can contain any dialog or control
3. Image lists provide visual cues
4. Dynamic tab management enables flexibility
5. Event notifications enable validation
6. Proper page management prevents memory leaks
7. Tab styles customize appearance

---

## Best Practices

1. Always validate data before tab switching
2. Free page resources when closing tabs
3. Provide visual feedback for modified pages
4. Implement keyboard shortcuts
5. Save tab state for session restore
6. Use tooltips for long tab titles
7. Limit number of tabs for usability

---

## Common Mistakes

### Mistake 1: Not Adjusting Display Rect
```cpp
// Wrong - Page covers tabs
CRect rect;
m_tabCtrl.GetClientRect(&rect);
pPage->MoveWindow(&rect);

// Correct - Adjust for tab area
CRect rect;
m_tabCtrl.GetClientRect(&rect);
m_tabCtrl.AdjustRect(FALSE, &rect);
pPage->MoveWindow(&rect);
```

### Mistake 2: Memory Leaks
```cpp
// Wrong - Pages never deleted
m_tabCtrl.DeleteAllItems();

// Correct - Delete pages first
for (int i = 0; i < m_arrPages.GetSize(); i++)
{
    delete m_arrPages[i];
}
m_arrPages.RemoveAll();
m_tabCtrl.DeleteAllItems();
```

---

## Summary

In this lesson, you learned:
- Creating and managing tab controls
- Adding tabs with icons
- Page switching and management
- Dynamic tab creation
- Tab styles and customization
- Event handling
- Building tabbed applications

---

## Next Lesson Preview

In **Lesson 37**, we'll explore:
- Date Time Picker Control
- Month Calendar Control
- Date/time formatting
- Custom date ranges
- Date validation

**Excellent work with tab controls!**
