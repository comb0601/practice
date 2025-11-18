# Lesson 25: Frame Windows (CFrameWnd, CMDIFrameWnd)

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding Frame Windows
2. CFrameWnd - Single Document Frame
3. CMDIFrameWnd - Multiple Document Frame  
4. Frame Window Customization
5. Creating and Managing Child Windows
6. Frame Window Styles
7. Toolbars in Frame Windows
8. Status Bars in Frame Windows
9. Client Area Management
10. Complete Examples
11. Exercises

## 1. Understanding Frame Windows

Frame windows are the top-level windows that contain views, toolbars, status bars, and menus. They are the "shell" of your application.

**Key Responsibilities**:
- Window border and title bar
- Menu bar
- Toolbars  
- Status bar
- Contains views
- Manages client area

---

## 2. CFrameWnd - Single Document Frame

```cpp
class CMainFrame : public CFrameWnd
{
protected:
    CToolBar    m_wndToolBar;
    CStatusBar  m_wndStatusBar;

public:
    CMainFrame();
    
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    
    DECLARE_DYNCREATE(CMainFrame)
    DECLARE_MESSAGE_MAP()
};
```

### PreCreateWindow - Window Style Setup

```cpp
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CFrameWnd::PreCreateWindow(cs))
        return FALSE;

    // Modify window styles
    cs.style |= WS_MAXIMIZE;    // Start maximized
    cs.cx = 1024;               // Width
    cs.cy = 768;                // Height
    
    // Center on screen
    cs.x = (GetSystemMetrics(SM_CXSCREEN) - cs.cx) / 2;
    cs.y = (GetSystemMetrics(SM_CYSCREEN) - cs.cy) / 2;

    return TRUE;
}
```

### OnCreate - Creating Child Controls

```cpp
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Create toolbar
    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT) ||
        !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
    {
        return -1;
    }

    // Create status bar
    if (!m_wndStatusBar.Create(this))
        return -1;

    static UINT indicators[] = {
        ID_SEPARATOR,
        ID_INDICATOR_CAPS,
        ID_INDICATOR_NUM,
    };
    m_wndStatusBar.SetIndicators(indicators, 3);

    // Enable docking
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar);

    return 0;
}
```

---

## 3. CMDIFrameWnd - Multiple Document Frame

```cpp
class CMainFrame : public CMDIFrameWnd
{
protected:
    CToolBar    m_wndToolBar;
    CStatusBar  m_wndStatusBar;

public:
    CMainFrame();
    
    // MDI-specific methods
    CMDIChildWnd* MDIGetActive(BOOL* pbMaximized = NULL) const;
    void MDICascade();
    void MDITile(int nType = MDITILE_HORIZONTAL);
    void MDIActivate(CWnd* pWndActivate);

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnWindowCascade();
    afx_msg void OnWindowTileHorz();
    
    DECLARE_DYNAMIC(CMainFrame)
    DECLARE_MESSAGE_MAP()
};
```

---

## 4. Frame Window Customization

### Custom Window Class

```cpp
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CFrameWnd::PreCreateWindow(cs))
        return FALSE;

    // Register custom window class
    cs.lpszClass = AfxRegisterWndClass(
        CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
        ::LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)::GetStockObject(WHITE_BRUSH),
        AfxGetApp()->LoadIcon(IDR_MAINFRAME));

    return TRUE;
}
```

### Window Position Persistence

```cpp
void CMainFrame::SaveWindowPosition()
{
    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    
    if (GetWindowPlacement(&wp))
    {
        AfxGetApp()->WriteProfileInt(_T("Window"), _T("ShowCmd"), wp.showCmd);
        AfxGetApp()->WriteProfileInt(_T("Window"), _T("MinX"), wp.ptMinPosition.x);
        AfxGetApp()->WriteProfileInt(_T("Window"), _T("MinY"), wp.ptMinPosition.y);
        AfxGetApp()->WriteProfileInt(_T("Window"), _T("MaxX"), wp.ptMaxPosition.x);
        AfxGetApp()->WriteProfileInt(_T("Window"), _T("MaxY"), wp.ptMaxPosition.y);
        AfxGetApp()->WriteProfileInt(_T("Window"), _T("Left"), wp.rcNormalPosition.left);
        AfxGetApp()->WriteProfileInt(_T("Window"), _T("Top"), wp.rcNormalPosition.top);
        AfxGetApp()->WriteProfileInt(_T("Window"), _T("Right"), wp.rcNormalPosition.right);
        AfxGetApp()->WriteProfileInt(_T("Window"), _T("Bottom"), wp.rcNormalPosition.bottom);
    }
}

void CMainFrame::LoadWindowPosition()
{
    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    
    wp.showCmd = AfxGetApp()->GetProfileInt(_T("Window"), _T("ShowCmd"), SW_SHOWNORMAL);
    wp.ptMinPosition.x = AfxGetApp()->GetProfileInt(_T("Window"), _T("MinX"), 0);
    wp.ptMinPosition.y = AfxGetApp()->GetProfileInt(_T("Window"), _T("MinY"), 0);
    wp.ptMaxPosition.x = AfxGetApp()->GetProfileInt(_T("Window"), _T("MaxX"), 0);
    wp.ptMaxPosition.y = AfxGetApp()->GetProfileInt(_T("Window"), _T("MaxY"), 0);
    wp.rcNormalPosition.left = AfxGetApp()->GetProfileInt(_T("Window"), _T("Left"), 100);
    wp.rcNormalPosition.top = AfxGetApp()->GetProfileInt(_T("Window"), _T("Top"), 100);
    wp.rcNormalPosition.right = AfxGetApp()->GetProfileInt(_T("Window"), _T("Right"), 900);
    wp.rcNormalPosition.bottom = AfxGetApp()->GetProfileInt(_T("Window"), _T("Bottom"), 700);
    
    SetWindowPlacement(&wp);
}
```

---

## 5. Creating and Managing Child Windows

### OnCreateClient - Custom Client Area

```cpp
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
    // Create splitter window
    if (!m_wndSplitter.CreateStatic(this, 2, 1))
        return FALSE;

    if (!m_wndSplitter.CreateView(0, 0, 
        RUNTIME_CLASS(CMyView1), CSize(0, 200), pContext))
        return FALSE;

    if (!m_wndSplitter.CreateView(1, 0,
        RUNTIME_CLASS(CMyView2), CSize(0, 0), pContext))
        return FALSE;

    return TRUE;
}
```

---

## Key Takeaways

1. ✅ Frame windows contain views and UI elements
2. ✅ CFrameWnd for SDI, CMDIFrameWnd for MDI
3. ✅ PreCreateWindow customizes window creation
4. ✅ OnCreate creates toolbars/status bars
5. ✅ OnCreateClient customizes client area
6. ✅ Save/restore window position for UX
7. ✅ EnableDocking allows toolbar docking
8. ✅ RecalcLayout updates layout
9. ✅ GetActiveView accesses current view
10. ✅ Frame manages application shell

---

## Next Lesson Preview

In **Lesson 26**, we'll explore:
- Command routing in MFC
- Update UI handlers
- Command enable/disable
- Menu and toolbar synchronization

**Congratulations on completing Lesson 25!**
