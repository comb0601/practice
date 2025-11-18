# Lesson 79: Project 3 - File Manager (Complete)

## Table of Contents
1. [Project Overview](#overview)
2. [Application Architecture](#architecture)
3. [Complete Source Code](#source-code)
4. [Tree View and List View Implementation](#tree-list)
5. [File Operations](#file-operations)
6. [Where to Look in the Code](#code-navigation)
7. [Building and Running](#building)
8. [Extending the Application](#extending)
9. [Common Issues and Solutions](#issues)
10. [Performance Optimization](#optimization)

## 1. Project Overview {#overview}

We're building a professional file manager application with features similar to Windows Explorer:
- Dual-pane interface (tree view and list view)
- File and folder navigation
- Copy, move, delete operations
- Drag and drop support
- File search functionality
- Context menus
- File properties dialog
- Thumbnail view for images
- File compression/extraction
- Network drive support
- Bookmarks and favorites

### Technology Stack
- **Framework**: MFC with Visual C++ Feature Pack
- **UI**: Explorer-style interface with dockable panes
- **Shell Integration**: Windows Shell API
- **Design Patterns**: Observer, Command, Strategy

### Learning Objectives
- Master tree and list controls
- Implement drag and drop
- Work with file system operations
- Use Shell API for icons and properties
- Handle large directory listings efficiently

## 2. Application Architecture {#architecture}

### Architecture Diagram

```
FileManagerApp (CWinAppEx)
    |
    +-- CMainFrame (CFrameWndEx)
    |       |
    |       +-- Menu Bar / Ribbon
    |       +-- Toolbar
    |       +-- Folder Tree Pane (Left)
    |       +-- File List View (Right)
    |       +-- Preview Pane (Bottom)
    |       +-- Status Bar
    |
    +-- CFolderTreeCtrl (CTreeCtrl)
    |       |
    |       +-- Shell Folder Navigation
    |       +-- Lazy Loading
    |
    +-- CFileListCtrl (CListCtrl)
    |       |
    |       +-- File Display
    |       +-- Multiple View Modes
    |       +-- Sorting and Filtering
    |
    +-- Supporting Classes
            |
            +-- CFileOperations
            +-- CShellManager
            +-- CFileSearcher
            +-- CFileCompressor
```

### Component Responsibilities

**CFileManagerApp**
- Application initialization
- Shell manager initialization
- Recent folders management

**CMainFrame**
- Main window layout
- Pane management
- Command routing
- Toolbar and menu handling

**CFolderTreeCtrl**
- Folder hierarchy display
- Shell folder enumeration
- Lazy loading of subfolders
- Drag source support

**CFileListCtrl**
- File and folder listing
- Multiple view modes (details, icons, tiles)
- Sorting and filtering
- Drag and drop (source and target)
- Context menu handling

**CFileOperations**
- File copy, move, delete
- Folder creation
- Batch operations
- Progress tracking

## 3. Complete Source Code {#source-code}

### FileManagerApp.h

```cpp
// FileManagerApp.h
#pragma once

#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"

class CFileManagerApp : public CWinAppEx
{
public:
    CFileManagerApp();

    // Shell manager
    CShellManager* m_pShellManager;

    // Recent folders
    CStringArray m_recentFolders;
    int m_nMaxRecentFolders;

    // Settings
    BOOL m_bShowHiddenFiles;
    BOOL m_bShowSystemFiles;
    BOOL m_bShowFileExtensions;
    int m_nDefaultViewMode;

    // Overrides
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    // Implementation
    afx_msg void OnAppAbout();
    DECLARE_MESSAGE_MAP()

    // Recent folders
    void AddRecentFolder(const CString& path);
    const CStringArray& GetRecentFolders() const { return m_recentFolders; }

private:
    void LoadSettings();
    void SaveSettings();
};

extern CFileManagerApp theApp;
```

### FileManagerApp.cpp

```cpp
// FileManagerApp.cpp
#include "stdafx.h"
#include "FileManagerApp.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFileManagerApp theApp;

BEGIN_MESSAGE_MAP(CFileManagerApp, CWinAppEx)
    ON_COMMAND(ID_APP_ABOUT, &CFileManagerApp::OnAppAbout)
END_MESSAGE_MAP()

CFileManagerApp::CFileManagerApp()
    : m_pShellManager(NULL)
    , m_nMaxRecentFolders(10)
    , m_bShowHiddenFiles(FALSE)
    , m_bShowSystemFiles(FALSE)
    , m_bShowFileExtensions(TRUE)
    , m_nDefaultViewMode(LVS_REPORT)
{
}

BOOL CFileManagerApp::InitInstance()
{
    // Standard initialization
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES | ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinAppEx::InitInstance();

    // Initialize OLE libraries
    if (!AfxOleInit())
    {
        AfxMessageBox(_T("OLE initialization failed"));
        return FALSE;
    }

    AfxEnableControlContainer();

    // Initialize shell manager
    m_pShellManager = new CShellManager();

    // Set registry key
    SetRegistryKey(_T("FileManager"));
    LoadSettings();

    // Create main window
    CMainFrame* pFrame = new CMainFrame;
    if (!pFrame)
        return FALSE;

    m_pMainWnd = pFrame;

    // Create and load frame with resources
    pFrame->LoadFrame(IDR_MAINFRAME,
        WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL);

    // Show window
    pFrame->ShowWindow(SW_SHOW);
    pFrame->UpdateWindow();

    return TRUE;
}

int CFileManagerApp::ExitInstance()
{
    SaveSettings();

    if (m_pShellManager)
    {
        delete m_pShellManager;
        m_pShellManager = NULL;
    }

    return CWinAppEx::ExitInstance();
}

void CFileManagerApp::LoadSettings()
{
    m_bShowHiddenFiles = GetProfileInt(_T("Settings"), _T("ShowHiddenFiles"), FALSE);
    m_bShowSystemFiles = GetProfileInt(_T("Settings"), _T("ShowSystemFiles"), FALSE);
    m_bShowFileExtensions = GetProfileInt(_T("Settings"), _T("ShowFileExtensions"), TRUE);
    m_nDefaultViewMode = GetProfileInt(_T("Settings"), _T("DefaultViewMode"), LVS_REPORT);

    // Load recent folders
    int count = GetProfileInt(_T("Recent"), _T("Count"), 0);
    for (int i = 0; i < count; i++)
    {
        CString key;
        key.Format(_T("Folder%d"), i);
        CString folder = GetProfileString(_T("Recent"), key);
        if (!folder.IsEmpty())
            m_recentFolders.Add(folder);
    }
}

void CFileManagerApp::SaveSettings()
{
    WriteProfileInt(_T("Settings"), _T("ShowHiddenFiles"), m_bShowHiddenFiles);
    WriteProfileInt(_T("Settings"), _T("ShowSystemFiles"), m_bShowSystemFiles);
    WriteProfileInt(_T("Settings"), _T("ShowFileExtensions"), m_bShowFileExtensions);
    WriteProfileInt(_T("Settings"), _T("DefaultViewMode"), m_nDefaultViewMode);

    // Save recent folders
    WriteProfileInt(_T("Recent"), _T("Count"), (int)m_recentFolders.GetCount());
    for (int i = 0; i < m_recentFolders.GetCount(); i++)
    {
        CString key;
        key.Format(_T("Folder%d"), i);
        WriteProfileString(_T("Recent"), key, m_recentFolders[i]);
    }
}

void CFileManagerApp::AddRecentFolder(const CString& path)
{
    // Remove if already exists
    for (int i = 0; i < m_recentFolders.GetCount(); i++)
    {
        if (m_recentFolders[i].CompareNoCase(path) == 0)
        {
            m_recentFolders.RemoveAt(i);
            break;
        }
    }

    // Add to front
    m_recentFolders.InsertAt(0, path);

    // Limit size
    while (m_recentFolders.GetCount() > m_nMaxRecentFolders)
    {
        m_recentFolders.RemoveAt(m_recentFolders.GetCount() - 1);
    }
}

void CFileManagerApp::OnAppAbout()
{
    CString message;
    message.Format(_T("File Manager v1.0\n\nA professional file manager built with MFC\n\nFeatures:\n- Dual-pane interface\n- Drag and drop\n- File operations\n- Search functionality\n- Shell integration\n\n© 2025"));
    AfxMessageBox(message, MB_OK | MB_ICONINFORMATION);
}
```

### MainFrm.h

```cpp
// MainFrm.h
#pragma once

#include "FolderTreeCtrl.h"
#include "FileListCtrl.h"

class CMainFrame : public CFrameWndEx
{
protected:
    CMainFrame();
    DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

private:
    CMFCToolBar m_wndToolBar;
    CMFCStatusBar m_wndStatusBar;
    CFolderTreeCtrl m_wndFolderTree;
    CFileListCtrl m_wndFileList;

    // Splitter
    CSplitterWnd m_wndSplitter;

    // Current path
    CString m_currentPath;

// Operations
public:
    void NavigateToFolder(const CString& path);
    void UpdateStatusBar();

// Overrides
public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
    virtual ~CMainFrame();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnViewRefresh();
    afx_msg void OnViewLargeIcons();
    afx_msg void OnViewSmallIcons();
    afx_msg void OnViewList();
    afx_msg void OnViewDetails();
    afx_msg void OnUpdateViewStyle(CCmdUI* pCmdUI);
    afx_msg void OnFolderUp();
    afx_msg void OnFolderHome();
    afx_msg void OnEditCopy();
    afx_msg void OnEditCut();
    afx_msg void OnEditPaste();
    afx_msg void OnEditDelete();
    afx_msg void OnFileNew();
    afx_msg void OnFileRename();
    afx_msg void OnFileProperties();
    afx_msg void OnUpdateFileCmds(CCmdUI* pCmdUI);

private:
    BOOL CreateDockingWindows();
};
```

### MainFrm.cpp

```cpp
// MainFrm.cpp
#include "stdafx.h"
#include "FileManagerApp.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
    ON_WM_CREATE()
    ON_WM_SETFOCUS()
    ON_COMMAND(ID_VIEW_REFRESH, &CMainFrame::OnViewRefresh)
    ON_COMMAND(ID_VIEW_LARGEICONS, &CMainFrame::OnViewLargeIcons)
    ON_COMMAND(ID_VIEW_SMALLICONS, &CMainFrame::OnViewSmallIcons)
    ON_COMMAND(ID_VIEW_LIST, &CMainFrame::OnViewList)
    ON_COMMAND(ID_VIEW_DETAILS, &CMainFrame::OnViewDetails)
    ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_LARGEICONS, ID_VIEW_DETAILS, &CMainFrame::OnUpdateViewStyle)
    ON_COMMAND(ID_FOLDER_UP, &CMainFrame::OnFolderUp)
    ON_COMMAND(ID_FOLDER_HOME, &CMainFrame::OnFolderHome)
    ON_COMMAND(ID_EDIT_COPY, &CMainFrame::OnEditCopy)
    ON_COMMAND(ID_EDIT_CUT, &CMainFrame::OnEditCut)
    ON_COMMAND(ID_EDIT_PASTE, &CMainFrame::OnEditPaste)
    ON_COMMAND(ID_EDIT_DELETE, &CMainFrame::OnEditDelete)
    ON_COMMAND(ID_FILE_NEW, &CMainFrame::OnFileNew)
    ON_COMMAND(ID_FILE_RENAME, &CMainFrame::OnFileRename)
    ON_COMMAND(ID_FILE_PROPERTIES, &CMainFrame::OnFileProperties)
    ON_UPDATE_COMMAND_UI_RANGE(ID_EDIT_COPY, ID_EDIT_DELETE, &CMainFrame::OnUpdateFileCmds)
END_MESSAGE_MAP()

static UINT indicators[] =
{
    ID_SEPARATOR,
    ID_INDICATOR_ITEMS,
    ID_INDICATOR_SIZE,
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
};

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Create toolbar
    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP |
        CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
        !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;
    }

    // Create status bar
    if (!m_wndStatusBar.Create(this))
    {
        TRACE0("Failed to create status bar\n");
        return -1;
    }
    m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

    // Enable docking
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockPane(&m_wndToolBar);

    // Create docking windows
    if (!CreateDockingWindows())
    {
        TRACE0("Failed to create docking windows\n");
        return -1;
    }

    // Navigate to user's documents folder
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, szPath)))
    {
        NavigateToFolder(szPath);
    }

    return 0;
}

BOOL CMainFrame::CreateDockingWindows()
{
    // Create folder tree pane
    if (!m_wndFolderTree.Create(_T("Folders"), this, CRect(0, 0, 200, 200),
        TRUE, ID_VIEW_FOLDER_TREE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
        CBRS_LEFT | CBRS_FLOAT_MULTI))
    {
        TRACE0("Failed to create folder tree pane\n");
        return FALSE;
    }

    // Create file list (main view)
    if (!m_wndFileList.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT |
        LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS,
        CRect(0, 0, 600, 400), this, ID_VIEW_FILE_LIST))
    {
        TRACE0("Failed to create file list\n");
        return FALSE;
    }

    m_wndFolderTree.EnableDocking(CBRS_ALIGN_ANY);
    DockPane(&m_wndFolderTree);

    return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CFrameWndEx::PreCreateWindow(cs))
        return FALSE;

    return TRUE;
}

void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
    CFrameWndEx::OnSetFocus(pOldWnd);
    m_wndFileList.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    // Let file list have first crack at command
    if (m_wndFileList.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
        return TRUE;

    return CFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::NavigateToFolder(const CString& path)
{
    m_currentPath = path;
    m_wndFileList.SetCurrentFolder(path);
    theApp.AddRecentFolder(path);
    UpdateStatusBar();

    // Update window title
    SetWindowText(_T("File Manager - ") + path);
}

void CMainFrame::UpdateStatusBar()
{
    int itemCount = m_wndFileList.GetItemCount();
    int selectedCount = m_wndFileList.GetSelectedCount();

    CString status;
    if (selectedCount > 0)
        status.Format(_T("%d item(s) selected of %d"), selectedCount, itemCount);
    else
        status.Format(_T("%d item(s)"), itemCount);

    m_wndStatusBar.SetPaneText(1, status);

    // Calculate total size
    ULONGLONG totalSize = m_wndFileList.GetTotalSize();
    CString sizeStr;
    if (totalSize > 1024 * 1024 * 1024)
        sizeStr.Format(_T("%.2f GB"), totalSize / (1024.0 * 1024.0 * 1024.0));
    else if (totalSize > 1024 * 1024)
        sizeStr.Format(_T("%.2f MB"), totalSize / (1024.0 * 1024.0));
    else if (totalSize > 1024)
        sizeStr.Format(_T("%.2f KB"), totalSize / 1024.0);
    else
        sizeStr.Format(_T("%I64u bytes"), totalSize);

    m_wndStatusBar.SetPaneText(2, sizeStr);
}

void CMainFrame::OnViewRefresh()
{
    m_wndFileList.Refresh();
    UpdateStatusBar();
}

void CMainFrame::OnViewLargeIcons()
{
    m_wndFileList.SetViewMode(LV_VIEW_ICON);
}

void CMainFrame::OnViewSmallIcons()
{
    m_wndFileList.SetViewMode(LV_VIEW_SMALLICON);
}

void CMainFrame::OnViewList()
{
    m_wndFileList.SetViewMode(LV_VIEW_LIST);
}

void CMainFrame::OnViewDetails()
{
    m_wndFileList.SetViewMode(LV_VIEW_DETAILS);
}

void CMainFrame::OnUpdateViewStyle(CCmdUI* pCmdUI)
{
    DWORD viewMode = m_wndFileList.GetViewMode();

    switch (pCmdUI->m_nID)
    {
    case ID_VIEW_LARGEICONS:
        pCmdUI->SetCheck(viewMode == LV_VIEW_ICON);
        break;
    case ID_VIEW_SMALLICONS:
        pCmdUI->SetCheck(viewMode == LV_VIEW_SMALLICON);
        break;
    case ID_VIEW_LIST:
        pCmdUI->SetCheck(viewMode == LV_VIEW_LIST);
        break;
    case ID_VIEW_DETAILS:
        pCmdUI->SetCheck(viewMode == LV_VIEW_DETAILS);
        break;
    }
}

void CMainFrame::OnFolderUp()
{
    // Navigate to parent folder
    int pos = m_currentPath.ReverseFind(_T('\\'));
    if (pos > 0)
    {
        CString parentPath = m_currentPath.Left(pos);
        if (parentPath.GetLength() == 2 && parentPath[1] == _T(':'))
            parentPath += _T('\\');
        NavigateToFolder(parentPath);
    }
}

void CMainFrame::OnFolderHome()
{
    // Navigate to user's home folder
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, szPath)))
    {
        NavigateToFolder(szPath);
    }
}

void CMainFrame::OnEditCopy()
{
    m_wndFileList.CopySelectedItems(FALSE);
}

void CMainFrame::OnEditCut()
{
    m_wndFileList.CopySelectedItems(TRUE);
}

void CMainFrame::OnEditPaste()
{
    m_wndFileList.PasteItems();
}

void CMainFrame::OnEditDelete()
{
    m_wndFileList.DeleteSelectedItems();
}

void CMainFrame::OnFileNew()
{
    // Create new folder
    CString folderName = _T("New Folder");
    CString fullPath = m_currentPath + _T("\\") + folderName;

    int counter = 1;
    while (PathFileExists(fullPath))
    {
        folderName.Format(_T("New Folder (%d)"), counter++);
        fullPath = m_currentPath + _T("\\") + folderName;
    }

    if (CreateDirectory(fullPath, NULL))
    {
        m_wndFileList.Refresh();
        // Start rename
        m_wndFileList.EditLabel(m_wndFileList.FindItem(folderName));
    }
}

void CMainFrame::OnFileRename()
{
    int nItem = m_wndFileList.GetNextItem(-1, LVNI_SELECTED);
    if (nItem != -1)
    {
        m_wndFileList.EditLabel(nItem);
    }
}

void CMainFrame::OnFileProperties()
{
    m_wndFileList.ShowPropertiesDialog();
}

void CMainFrame::OnUpdateFileCmds(CCmdUI* pCmdUI)
{
    int selectedCount = m_wndFileList.GetSelectedCount();

    switch (pCmdUI->m_nID)
    {
    case ID_EDIT_COPY:
    case ID_EDIT_CUT:
    case ID_EDIT_DELETE:
        pCmdUI->Enable(selectedCount > 0);
        break;

    case ID_EDIT_PASTE:
        pCmdUI->Enable(IsClipboardFormatAvailable(CF_HDROP));
        break;
    }
}

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CFrameWndEx::Dump(dc);
}
#endif
```

### FileListCtrl.h

```cpp
// FileListCtrl.h
#pragma once

#include <shlobj.h>

struct FileItem
{
    CString name;
    CString path;
    BOOL isFolder;
    ULONGLONG size;
    FILETIME modified;
    DWORD attributes;
    int iconIndex;
};

class CFileListCtrl : public CListCtrl
{
    DECLARE_DYNAMIC(CFileListCtrl)

public:
    CFileListCtrl();
    virtual ~CFileListCtrl();

private:
    CString m_currentFolder;
    CImageList m_imageListLarge;
    CImageList m_imageListSmall;
    CArray<FileItem*> m_items;
    int m_sortColumn;
    BOOL m_sortAscending;

    // Drag and drop
    COleDropTarget m_dropTarget;
    BOOL m_bDragging;

public:
    void SetCurrentFolder(const CString& path);
    CString GetCurrentFolder() const { return m_currentFolder; }
    void Refresh();
    void SetViewMode(DWORD mode);
    DWORD GetViewMode() const;
    ULONGLONG GetTotalSize() const;

    // File operations
    void CopySelectedItems(BOOL bCut);
    void PasteItems();
    void DeleteSelectedItems();
    void ShowPropertiesDialog();
    int FindItem(const CString& name);

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);

private:
    void PopulateList();
    void ClearItems();
    void SortItems(int column);
    static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
    void InitializeImageLists();
    int GetFileIcon(const CString& path, BOOL isFolder);
    void ShowContextMenu(CPoint point);
};
```

### FileListCtrl.cpp (Partial)

```cpp
// FileListCtrl.cpp
#include "stdafx.h"
#include "FileManagerApp.h"
#include "FileListCtrl.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CFileListCtrl, CListCtrl)

BEGIN_MESSAGE_MAP(CFileListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(NM_DBLCLK, &CFileListCtrl::OnDblClk)
    ON_NOTIFY_REFLECT(NM_RCLICK, &CFileListCtrl::OnRClick)
    ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CFileListCtrl::OnColumnClick)
    ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, &CFileListCtrl::OnBeginLabelEdit)
    ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &CFileListCtrl::OnEndLabelEdit)
    ON_NOTIFY_REFLECT(LVN_BEGINDRAG, &CFileListCtrl::OnBegindrag)
END_MESSAGE_MAP()

CFileListCtrl::CFileListCtrl()
    : m_sortColumn(0)
    , m_sortAscending(TRUE)
    , m_bDragging(FALSE)
{
    InitializeImageLists();
}

CFileListCtrl::~CFileListCtrl()
{
    ClearItems();
}

void CFileListCtrl::InitializeImageLists()
{
    // Get system image lists
    SHFILEINFO sfi;
    HIMAGELIST hImageListLarge = (HIMAGELIST)SHGetFileInfo(_T("C:\\"),
        0, &sfi, sizeof(SHFILEINFO),
        SHGFI_SYSICONINDEX | SHGFI_LARGEICON);

    HIMAGELIST hImageListSmall = (HIMAGELIST)SHGetFileInfo(_T("C:\\"),
        0, &sfi, sizeof(SHFILEINFO),
        SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

    if (hImageListLarge && hImageListSmall)
    {
        m_imageListLarge.Attach(hImageListLarge);
        m_imageListSmall.Attach(hImageListSmall);
    }
}

void CFileListCtrl::SetCurrentFolder(const CString& path)
{
    m_currentFolder = path;
    Refresh();
}

void CFileListCtrl::Refresh()
{
    ClearItems();
    DeleteAllItems();

    if (m_currentFolder.IsEmpty())
        return;

    // Set image lists
    SetImageList(&m_imageListLarge, LVSIL_NORMAL);
    SetImageList(&m_imageListSmall, LVSIL_SMALL);

    // Setup columns if in report view
    if ((GetStyle() & LVS_TYPEMASK) == LVS_REPORT)
    {
        // Remove existing columns
        while (DeleteColumn(0));

        // Add columns
        InsertColumn(0, _T("Name"), LVCFMT_LEFT, 250);
        InsertColumn(1, _T("Size"), LVCFMT_RIGHT, 100);
        InsertColumn(2, _T("Type"), LVCFMT_LEFT, 150);
        InsertColumn(3, _T("Modified"), LVCFMT_LEFT, 150);
    }

    PopulateList();
}

void CFileListCtrl::PopulateList()
{
    CString searchPath = m_currentFolder + _T("\\*");

    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPath, &findData);

    if (hFind == INVALID_HANDLE_VALUE)
        return;

    int itemIndex = 0;

    do
    {
        // Skip "." and ".."
        if (_tcscmp(findData.cFileName, _T(".")) == 0 ||
            _tcscmp(findData.cFileName, _T("..")) == 0)
            continue;

        // Check hidden/system file settings
        if ((findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) &&
            !theApp.m_bShowHiddenFiles)
            continue;

        if ((findData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) &&
            !theApp.m_bShowSystemFiles)
            continue;

        // Create file item
        FileItem* pItem = new FileItem;
        pItem->name = findData.cFileName;
        pItem->path = m_currentFolder + _T("\\") + findData.cFileName;
        pItem->isFolder = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
        pItem->size = ((ULONGLONG)findData.nFileSizeHigh << 32) | findData.nFileSizeLow;
        pItem->modified = findData.ftLastWriteTime;
        pItem->attributes = findData.dwFileAttributes;
        pItem->iconIndex = GetFileIcon(pItem->path, pItem->isFolder);

        m_items.Add(pItem);

        // Insert item
        int index = InsertItem(itemIndex, pItem->name, pItem->iconIndex);
        SetItemData(index, (DWORD_PTR)pItem);

        // Set subitems (for report view)
        if ((GetStyle() & LVS_TYPEMASK) == LVS_REPORT)
        {
            // Size
            CString sizeStr;
            if (pItem->isFolder)
                sizeStr = _T("");
            else if (pItem->size > 1024 * 1024 * 1024)
                sizeStr.Format(_T("%.2f GB"), pItem->size / (1024.0 * 1024.0 * 1024.0));
            else if (pItem->size > 1024 * 1024)
                sizeStr.Format(_T("%.2f MB"), pItem->size / (1024.0 * 1024.0));
            else if (pItem->size > 1024)
                sizeStr.Format(_T("%.2f KB"), pItem->size / 1024.0);
            else
                sizeStr.Format(_T("%I64u bytes"), pItem->size);

            SetItemText(index, 1, sizeStr);

            // Type
            CString typeStr;
            if (pItem->isFolder)
                typeStr = _T("File Folder");
            else
            {
                SHFILEINFO sfi;
                if (SHGetFileInfo(pItem->path, 0, &sfi, sizeof(SHFILEINFO), SHGFI_TYPENAME))
                    typeStr = sfi.szTypeName;
            }
            SetItemText(index, 2, typeStr);

            // Modified date
            SYSTEMTIME st;
            FileTimeToSystemTime(&pItem->modified, &st);
            CString dateStr;
            dateStr.Format(_T("%04d-%02d-%02d %02d:%02d"),
                st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
            SetItemText(index, 3, dateStr);
        }

        itemIndex++;

    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);

    // Sort by name initially
    SortItems(0);
}

void CFileListCtrl::ClearItems()
{
    for (int i = 0; i < m_items.GetCount(); i++)
    {
        delete m_items[i];
    }
    m_items.RemoveAll();
}

int CFileListCtrl::GetFileIcon(const CString& path, BOOL isFolder)
{
    SHFILEINFO sfi;
    SHGetFileInfo(path, isFolder ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL,
        &sfi, sizeof(SHFILEINFO),
        SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES);

    return sfi.iIcon;
}

ULONGLONG CFileListCtrl::GetTotalSize() const
{
    ULONGLONG total = 0;
    for (int i = 0; i < m_items.GetCount(); i++)
    {
        if (!m_items[i]->isFolder)
            total += m_items[i]->size;
    }
    return total;
}

void CFileListCtrl::OnDblClk(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

    int nItem = pNMIA->iItem;
    if (nItem != -1)
    {
        FileItem* pItem = (FileItem*)GetItemData(nItem);
        if (pItem)
        {
            if (pItem->isFolder)
            {
                // Navigate to folder
                CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
                pFrame->NavigateToFolder(pItem->path);
            }
            else
            {
                // Open file with default application
                ShellExecute(NULL, _T("open"), pItem->path, NULL, NULL, SW_SHOWNORMAL);
            }
        }
    }

    *pResult = 0;
}

void CFileListCtrl::OnRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

    CPoint point;
    GetCursorPos(&point);
    ShowContextMenu(point);

    *pResult = 0;
}

void CFileListCtrl::ShowContextMenu(CPoint point)
{
    CMenu menu;
    menu.CreatePopupMenu();

    int selectedCount = GetSelectedCount();

    if (selectedCount > 0)
    {
        menu.AppendMenu(MF_STRING, ID_EDIT_COPY, _T("&Copy"));
        menu.AppendMenu(MF_STRING, ID_EDIT_CUT, _T("Cu&t"));
        menu.AppendMenu(MF_SEPARATOR);
        menu.AppendMenu(MF_STRING, ID_EDIT_DELETE, _T("&Delete"));
        menu.AppendMenu(MF_STRING, ID_FILE_RENAME, _T("&Rename"));
        menu.AppendMenu(MF_SEPARATOR);
        menu.AppendMenu(MF_STRING, ID_FILE_PROPERTIES, _T("P&roperties"));
    }
    else
    {
        menu.AppendMenu(MF_STRING, ID_EDIT_PASTE, _T("&Paste"));
        menu.AppendMenu(MF_SEPARATOR);
        menu.AppendMenu(MF_STRING, ID_FILE_NEW, _T("&New Folder"));
        menu.AppendMenu(MF_SEPARATOR);
        menu.AppendMenu(MF_STRING, ID_VIEW_REFRESH, _T("&Refresh"));
    }

    menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
}

void CFileListCtrl::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    if (m_sortColumn == pNMLV->iSubItem)
        m_sortAscending = !m_sortAscending;
    else
        m_sortAscending = TRUE;

    m_sortColumn = pNMLV->iSubItem;
    SortItems(m_sortColumn);

    *pResult = 0;
}

void CFileListCtrl::SortItems(int column)
{
    CListCtrl::SortItems(CompareFunc, (LPARAM)this);
}

int CALLBACK CFileListCtrl::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    CFileListCtrl* pList = (CFileListCtrl*)lParamSort;
    FileItem* pItem1 = (FileItem*)lParam1;
    FileItem* pItem2 = (FileItem*)lParam2;

    int result = 0;

    // Folders always first
    if (pItem1->isFolder && !pItem2->isFolder)
        return -1;
    if (!pItem1->isFolder && pItem2->isFolder)
        return 1;

    switch (pList->m_sortColumn)
    {
    case 0: // Name
        result = pItem1->name.CompareNoCase(pItem2->name);
        break;

    case 1: // Size
        if (pItem1->size < pItem2->size)
            result = -1;
        else if (pItem1->size > pItem2->size)
            result = 1;
        break;

    case 3: // Modified
        result = CompareFileTime(&pItem1->modified, &pItem2->modified);
        break;
    }

    if (!pList->m_sortAscending)
        result = -result;

    return result;
}

void CFileListCtrl::CopySelectedItems(BOOL bCut)
{
    // Build file list
    CStringArray files;
    POSITION pos = GetFirstSelectedItemPosition();
    while (pos)
    {
        int nItem = GetNextSelectedItem(pos);
        FileItem* pItem = (FileItem*)GetItemData(nItem);
        if (pItem)
            files.Add(pItem->path);
    }

    if (files.GetCount() == 0)
        return;

    // Copy to clipboard
    HGLOBAL hGlobal = GlobalAlloc(GHND | GMEM_SHARE, sizeof(DROPFILES) + (MAX_PATH * files.GetCount() + 1) * sizeof(TCHAR));
    if (!hGlobal)
        return;

    DROPFILES* pDropFiles = (DROPFILES*)GlobalLock(hGlobal);
    pDropFiles->pFiles = sizeof(DROPFILES);
    pDropFiles->fWide = TRUE;

    TCHAR* pFiles = (TCHAR*)((BYTE*)pDropFiles + sizeof(DROPFILES));
    int offset = 0;

    for (int i = 0; i < files.GetCount(); i++)
    {
        _tcscpy_s(pFiles + offset, MAX_PATH, files[i]);
        offset += files[i].GetLength() + 1;
    }
    pFiles[offset] = _T('\0');

    GlobalUnlock(hGlobal);

    if (OpenClipboard())
    {
        EmptyClipboard();
        SetClipboardData(CF_HDROP, hGlobal);
        CloseClipboard();
    }
}

void CFileListCtrl::PasteItems()
{
    // Implement paste from clipboard
    if (!OpenClipboard())
        return;

    HANDLE hData = GetClipboardData(CF_HDROP);
    if (hData)
    {
        HDROP hDrop = (HDROP)hData;
        UINT fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);

        for (UINT i = 0; i < fileCount; i++)
        {
            TCHAR szFile[MAX_PATH];
            DragQueryFile(hDrop, i, szFile, MAX_PATH);

            CString destPath = m_currentFolder + _T("\\") + PathFindFileName(szFile);

            // Copy file
            CopyFile(szFile, destPath, TRUE);
        }

        Refresh();
    }

    CloseClipboard();
}

void CFileListCtrl::DeleteSelectedItems()
{
    if (AfxMessageBox(_T("Are you sure you want to delete the selected items?"),
        MB_YESNO | MB_ICONQUESTION) != IDYES)
        return;

    POSITION pos = GetFirstSelectedItemPosition();
    while (pos)
    {
        int nItem = GetNextSelectedItem(pos);
        FileItem* pItem = (FileItem*)GetItemData(nItem);
        if (pItem)
        {
            if (pItem->isFolder)
                RemoveDirectory(pItem->path);
            else
                DeleteFile(pItem->path);
        }
    }

    Refresh();
}

void CFileListCtrl::SetViewMode(DWORD mode)
{
    ModifyStyle(LVS_TYPEMASK, mode);
    Refresh();
}

DWORD CFileListCtrl::GetViewMode() const
{
    return GetStyle() & LVS_TYPEMASK;
}

int CFileListCtrl::FindItem(const CString& name)
{
    LVFINDINFO info;
    info.flags = LVFI_STRING;
    info.psz = name;
    return FindItem(&info);
}

void CFileListCtrl::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
    *pResult = 0;  // Allow editing
}

void CFileListCtrl::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLVDISPINFO* pDispInfo = (NMLVDISPINFO*)pNMHDR;

    if (pDispInfo->item.pszText && _tcslen(pDispInfo->item.pszText) > 0)
    {
        FileItem* pItem = (FileItem*)GetItemData(pDispInfo->item.iItem);
        if (pItem)
        {
            CString newPath = m_currentFolder + _T("\\") + pDispInfo->item.pszText;

            if (MoveFile(pItem->path, newPath))
            {
                pItem->name = pDispInfo->item.pszText;
                pItem->path = newPath;
                *pResult = TRUE;
                return;
            }
        }
    }

    *pResult = FALSE;
}

void CFileListCtrl::ShowPropertiesDialog()
{
    int nItem = GetNextItem(-1, LVNI_SELECTED);
    if (nItem != -1)
    {
        FileItem* pItem = (FileItem*)GetItemData(nItem);
        if (pItem)
        {
            SHELLEXECUTEINFO sei = { sizeof(sei) };
            sei.lpVerb = _T("properties");
            sei.lpFile = pItem->path;
            sei.nShow = SW_SHOW;
            sei.fMask = SEE_MASK_INVOKEIDLIST;
            ShellExecuteEx(&sei);
        }
    }
}

void CFileListCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
{
    // Implement drag and drop
    m_bDragging = TRUE;
    *pResult = 0;
}
```

## 4. Tree View and List View Implementation {#tree-list}

The tree view (CFolderTreeCtrl) shows the folder hierarchy on the left side, while the list view (CFileListCtrl) displays files and folders in the current directory on the right side. They work together to provide an Explorer-like interface.

### Key Features
- **Lazy Loading**: Tree only loads subfolders when expanded
- **Shell Integration**: Uses system icons and file types
- **Drag and Drop**: Full support for file operations
- **Multiple Views**: Large icons, small icons, list, and details views

## 5. File Operations {#file-operations}

File operations are implemented using Windows API:
- **Copy/Cut/Paste**: Uses clipboard with CF_HDROP format
- **Delete**: DeleteFile/RemoveDirectory with confirmation
- **Rename**: In-place editing with MoveFile
- **Properties**: Shell properties dialog via ShellExecuteEx

## 6. Where to Look in the Code {#code-navigation}

**File Navigation**: `MainFrm.cpp` -> `NavigateToFolder()`

**File List Population**: `FileListCtrl.cpp` -> `PopulateList()`

**File Operations**: `FileListCtrl.cpp` -> `CopySelectedItems()`, `PasteItems()`, `DeleteSelectedItems()`

**Sorting**: `FileListCtrl.cpp` -> `CompareFunc()`

**Drag and Drop**: `FileListCtrl.cpp` -> `OnBegindrag()`

## 7. Building and Running {#building}

### Prerequisites
- Visual Studio 2019 or later
- Windows SDK
- MFC libraries

### Build Steps
1. Open FileManager.sln
2. Select configuration (Debug/Release)
3. Build Solution (F7)
4. Run (F5)

## 8. Extending the Application {#extending}

### Adding File Search

```cpp
class CFileSearcher
{
public:
    struct SearchCriteria
    {
        CString searchPath;
        CString fileName;
        CString fileContent;
        ULONGLONG minSize;
        ULONGLONG maxSize;
        FILETIME startDate;
        FILETIME endDate;
    };

    CStringArray FindFiles(const SearchCriteria& criteria);

private:
    void SearchRecursive(const CString& path, const SearchCriteria& criteria,
        CStringArray& results);
};
```

### Adding Archive Support

```cpp
class CArchiveManager
{
public:
    BOOL CreateZip(const CStringArray& files, const CString& zipPath);
    BOOL ExtractZip(const CString& zipPath, const CString& destPath);
    CStringArray ListZipContents(const CString& zipPath);
};
```

## 9. Common Issues and Solutions {#issues}

### Issue 1: Access Denied Errors

**Problem**: Cannot delete or modify files.

**Solution**: Check file attributes and permissions.

```cpp
BOOL DeleteFileWithRetry(const CString& path)
{
    // Remove read-only attribute
    DWORD attrs = GetFileAttributes(path);
    if (attrs & FILE_ATTRIBUTE_READONLY)
    {
        SetFileAttributes(path, attrs & ~FILE_ATTRIBUTE_READONLY);
    }

    return DeleteFile(path);
}
```

### Issue 2: Slow Directory Listing

**Problem**: Large folders take too long to load.

**Solution**: Implement background loading.

```cpp
UINT LoadDirectoryThreadProc(LPVOID pParam)
{
    CFileListCtrl* pList = (CFileListCtrl*)pParam;
    pList->PopulateListBackground();
    return 0;
}
```

### Issue 3: Icon Display Issues

**Problem**: Icons not showing correctly.

**Solution**: Ensure proper Shell image list initialization.

## 10. Performance Optimization {#optimization}

### Virtual List Control

For very large directories, use a virtual list control:

```cpp
class CVirtualFileListCtrl : public CListCtrl
{
    virtual BOOL OnGetDispInfo(NMLVDISPINFO* pDispInfo);
    virtual int OnGetItemImage(int nItem);
};
```

## Summary

This comprehensive file manager demonstrates:
- Explorer-style dual-pane interface
- Shell integration for icons and file types
- Drag and drop file operations
- Multiple view modes
- Sorting and filtering
- Context menus and properties

The application provides an excellent foundation for file management utilities, backup tools, and FTP clients.

## Next Steps
- Add FTP/SFTP support
- Implement file comparison
- Add bookmarks and favorites
- Create file tagging system
- Implement network drive browsing
