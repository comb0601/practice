# Lesson 24: MDI Applications (Multiple Document Interface)

**Duration**: 7 hours
**Difficulty**: Intermediate-Advanced

## Table of Contents
1. Understanding MDI Architecture
2. MDI vs SDI Comparison
3. Creating MDI Applications
4. MDI Frame Window
5. MDI Child Windows
6. Window Management
7. Document Templates in MDI
8. Window Menu
9. MDI-Specific Features
10. Complete MDI Application
11. Exercises

---

## 1. Understanding MDI Architecture

### What is MDI?

**MDI (Multiple Document Interface)** allows multiple documents to be open simultaneously within a single parent window.

```
┌──────────────────────────────────────────────┐
│  Main Frame (CMDIFrameWnd)                   │
│  ┌────────────────────────────────────────┐  │
│  │ Menu Bar                                │  │
│  ├────────────────────────────────────────┤  │
│  │ Toolbar                                 │  │
│  ├────────────────────────────────────────┤  │
│  │                                         │  │
│  │  ┌──────────┐  ┌──────────┐           │  │
│  │  │Document 1│  │Document 2│           │  │
│  │  │ (Child)  │  │ (Child)  │           │  │
│  │  └──────────┘  └──────────┘           │  │
│  │                                         │  │
│  ├────────────────────────────────────────┤  │
│  │ Status Bar                              │  │
│  └────────────────────────────────────────┘  │
└──────────────────────────────────────────────┘
```

---

### MDI Class Hierarchy

```
CWinApp (Application)
    └─→ CMDIFrameWnd (Main Frame)
            ├─→ CMDIChildWnd (Child 1)
            │       └─→ CView (View 1)
            │               └─→ CDocument (Doc 1)
            │
            └─→ CMDIChildWnd (Child 2)
                    └─→ CView (View 2)
                            └─→ CDocument (Doc 2)
```

---

## 2. MDI vs SDI Comparison

### When to Use MDI

**Use MDI When**:
- Users work with multiple related documents
- Comparing documents is important
- Single application instance preferred
- Documents share resources

**Examples**:
- Visual Studio (multiple files)
- Old Microsoft Word/Excel
- AutoCAD
- Notepad++

---

### When to Use SDI

**Use SDI When**:
- Users typically work with one document
- Simpler interface desired
- Each document independent
- Modern UI trends

**Examples**:
- Modern Word/Excel (tabbed interface)
- Notepad
- Paint
- Calculator

---

## 3. Creating MDI Applications

### Using MFC Application Wizard

1. File → New → Project → MFC Application
2. **Application Type**: Multiple documents (MDI)
3. Follow wizard steps
4. Click Finish

**Generated Classes**:
- `CYourApp` : CWinApp
- `CMainFrame` : CMDIFrameWnd
- `CChildFrame` : CMDIChildWnd
- `CYourDoc` : CDocument
- `CYourView` : CView

---

## 4. MDI Frame Window

### CMDIFrameWnd - The Main Window

```cpp
// MainFrm.h
class CMainFrame : public CMDIFrameWnd
{
    DECLARE_DYNAMIC(CMainFrame)

protected:
    CToolBar    m_wndToolBar;
    CStatusBar  m_wndStatusBar;

public:
    CMainFrame();
    virtual ~CMainFrame();

    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnWindowManager();

    DECLARE_MESSAGE_MAP()
};

// MainFrm.cpp
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
    ON_WM_CREATE()
    ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
END_MESSAGE_MAP()

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Create toolbar
    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT) ||
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

    static UINT indicators[] =
    {
        ID_SEPARATOR,
        ID_INDICATOR_CAPS,
        ID_INDICATOR_NUM,
        ID_INDICATOR_SCRL,
    };
    m_wndStatusBar.SetIndicators(indicators,
        sizeof(indicators)/sizeof(UINT));

    // Enable docking
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar);

    return 0;
}
```

---

## 5. MDI Child Windows

### CMDIChildWnd - Document Windows

```cpp
// ChildFrm.h
class CChildFrame : public CMDIChildWnd
{
    DECLARE_DYNCREATE(CChildFrame)

public:
    CChildFrame();
    virtual ~CChildFrame();

    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
    afx_msg void OnClose();

    DECLARE_MESSAGE_MAP()
};

// ChildFrm.cpp
IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
    ON_WM_CLOSE()
END_MESSAGE_MAP()

CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CMDIChildWnd::PreCreateWindow(cs))
        return FALSE;

    // Customize child window
    cs.style &= ~WS_MAXIMIZEBOX;  // Remove maximize
    cs.cx = 600;  // Initial width
    cs.cy = 400;  // Initial height

    return TRUE;
}

void CChildFrame::OnClose()
{
    // Custom cleanup before closing
    CMDIChildWnd::OnClose();
}
```

---

## 6. Window Management

### Window Menu Commands

```cpp
// Automatically added by MFC:
// ID_WINDOW_NEW        - New window for same document
// ID_WINDOW_CASCADE    - Cascade windows
// ID_WINDOW_TILE_HORZ  - Tile horizontally
// ID_WINDOW_TILE_VERT  - Tile vertically
// ID_WINDOW_ARRANGE    - Arrange icons

// In MainFrm.cpp - message handlers already connected
void CMainFrame::OnWindowCascade()
{
    MDICascade();
}

void CMainFrame::OnWindowTileHorz()
{
    MDITile(MDITILE_HORIZONTAL);
}

void CMainFrame::OnWindowTileVert()
{
    MDITile(MDITILE_VERTICAL);
}
```

---

### Managing Child Windows

```cpp
// Get active child window
CMDIChildWnd* pChild = MDIGetActive();

// Get active view
CView* pView = GetActiveView();

// Get active document
CDocument* pDoc = GetActiveDocument();

// Iterate all child windows
CMDIChildWnd* pChild = (CMDIChildWnd*)GetWindow(GW_CHILD);
while (pChild)
{
    if (pChild->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
    {
        // Process child window
        CDocument* pDoc = pChild->GetActiveDocument();
        // ...
    }
    pChild = (CMDIChildWnd*)pChild->GetWindow(GW_HWNDNEXT);
}

// Activate specific child
MDIActivate(pChildWnd);

// Maximize/Restore child
MDIMaximize(pChildWnd);
MDIRestore(pChildWnd);
```

---

## 7. Document Templates in MDI

### Single Document Type

```cpp
// App.cpp - InitInstance()
BOOL CMyApp::InitInstance()
{
    // ... initialization ...

    CMultiDocTemplate* pDocTemplate;
    pDocTemplate = new CMultiDocTemplate(
        IDR_MYTYPE,
        RUNTIME_CLASS(CMyDoc),
        RUNTIME_CLASS(CChildFrame),
        RUNTIME_CLASS(CMyView));
    AddDocTemplate(pDocTemplate);

    // Create main window
    CMainFrame* pMainFrame = new CMainFrame;
    if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
        return FALSE;
    m_pMainWnd = pMainFrame;

    // Process command line
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    pMainFrame->ShowWindow(m_nCmdShow);
    pMainFrame->UpdateWindow();

    return TRUE;
}
```

---

### Multiple Document Types

```cpp
BOOL CMyApp::InitInstance()
{
    // ... initialization ...

    // Template 1: Text documents
    CMultiDocTemplate* pTextTemplate;
    pTextTemplate = new CMultiDocTemplate(
        IDR_TEXT_TYPE,
        RUNTIME_CLASS(CTextDoc),
        RUNTIME_CLASS(CChildFrame),
        RUNTIME_CLASS(CTextView));
    AddDocTemplate(pTextTemplate);

    // Template 2: Drawing documents
    CMultiDocTemplate* pDrawTemplate;
    pDrawTemplate = new CMultiDocTemplate(
        IDR_DRAW_TYPE,
        RUNTIME_CLASS(CDrawDoc),
        RUNTIME_CLASS(CChildFrame),
        RUNTIME_CLASS(CDrawView));
    AddDocTemplate(pDrawTemplate);

    // Now File → New shows choice dialog
    // File → Open accepts both file types

    // ... rest of initialization ...
}
```

---

## 8. Window Menu

### Automatic Window List

MFC automatically maintains list of open windows in Window menu:

```
Window
├── New Window
├── Cascade
├── Tile Horizontally
├── Tile Vertically
├── Arrange Icons
├──────────────────
├── ✓ 1 Document1.txt     ← Active
├──   2 Document2.txt
├──   3 Drawing1.drw
└──   4 Untitled
```

---

### Customizing Window Menu

```cpp
// In MainFrm resource file (.rc)
// Add to Window menu:

MENUITEM "Close All", ID_WINDOW_CLOSE_ALL
MENUITEM "Save All", ID_WINDOW_SAVE_ALL

// In MainFrm.cpp
BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
    ON_COMMAND(ID_WINDOW_CLOSE_ALL, &CMainFrame::OnWindowCloseAll)
    ON_COMMAND(ID_WINDOW_SAVE_ALL, &CMainFrame::OnWindowSaveAll)
END_MESSAGE_MAP()

void CMainFrame::OnWindowCloseAll()
{
    // Close all child windows
    CMDIChildWnd* pChild = MDIGetActive();
    while (pChild)
    {
        CMDIChildWnd* pNext =
            (CMDIChildWnd*)pChild->GetWindow(GW_HWNDNEXT);

        pChild->SendMessage(WM_CLOSE);

        pChild = pNext;
    }
}

void CMainFrame::OnWindowSaveAll()
{
    // Save all documents
    POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();
    while (pos)
    {
        CDocTemplate* pTemplate =
            AfxGetApp()->GetNextDocTemplate(pos);

        POSITION docPos = pTemplate->GetFirstDocPosition();
        while (docPos)
        {
            CDocument* pDoc = pTemplate->GetNextDoc(docPos);

            if (pDoc->IsModified())
            {
                pDoc->DoFileSave();
            }
        }
    }
}
```

---

## 9. MDI-Specific Features

### Creating New Windows for Same Document

```cpp
// Enable ID_WINDOW_NEW command
// User can create multiple views of same document

// In ChildFrm.cpp
void CChildFrame::OnWindowNew()
{
    CMDIFrameWnd* pFrame = GetMDIFrame();
    ASSERT_VALID(pFrame);

    CDocument* pDoc = GetActiveDocument();
    ASSERT_VALID(pDoc);

    CFrameWnd* pNewFrame =
        pDoc->GetDocTemplate()->CreateNewFrame(pDoc, this);

    if (pNewFrame == NULL)
        return;

    pDoc->GetDocTemplate()->InitialUpdateFrame(pNewFrame, pDoc);
}
```

---

### Custom Child Frame per Document Type

```cpp
// TextChildFrm.h - Special child frame for text docs
class CTextChildFrame : public CMDIChildWnd
{
    DECLARE_DYNCREATE(CTextChildFrame)

protected:
    CToolBar m_wndTextToolBar;  // Document-specific toolbar

public:
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs,
                               CCreateContext* pContext);

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    DECLARE_MESSAGE_MAP()
};

int CTextChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Create text-specific toolbar
    if (!m_wndTextToolBar.CreateEx(this, TBSTYLE_FLAT) ||
        !m_wndTextToolBar.LoadToolBar(IDR_TEXT_TOOLBAR))
    {
        return -1;
    }

    m_wndTextToolBar.EnableDocking(CBRS_ALIGN_TOP);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndTextToolBar);

    return 0;
}

// In App.cpp - use custom child frame
CMultiDocTemplate* pTextTemplate;
pTextTemplate = new CMultiDocTemplate(
    IDR_TEXT_TYPE,
    RUNTIME_CLASS(CTextDoc),
    RUNTIME_CLASS(CTextChildFrame),  // ← Custom child frame
    RUNTIME_CLASS(CTextView));
```

---

## 10. Complete MDI Application

### Example: Multi-Document Text Editor

```cpp
// App.h
class CMyApp : public CWinApp
{
public:
    virtual BOOL InitInstance();
};

// App.cpp
BOOL CMyApp::InitInstance()
{
    CWinApp::InitInstance();

    AfxEnableControlContainer();

    SetRegistryKey(_T("MyCompany"));

    LoadStdProfileSettings(10);  // 10 MRU files

    // Create document template
    CMultiDocTemplate* pDocTemplate;
    pDocTemplate = new CMultiDocTemplate(
        IDR_MYTYPE,
        RUNTIME_CLASS(CTextDoc),
        RUNTIME_CLASS(CChildFrame),
        RUNTIME_CLASS(CTextView));
    AddDocTemplate(pDocTemplate);

    // Create main MDI Frame
    CMainFrame* pMainFrame = new CMainFrame;
    if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
        return FALSE;
    m_pMainWnd = pMainFrame;

    // Enable drag/drop
    m_pMainWnd->DragAcceptFiles();

    // Enable DDE Execute open
    EnableShellOpen();
    RegisterShellFileTypes(TRUE);

    // Parse command line
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // Dispatch commands
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    // Show main window
    pMainFrame->ShowWindow(m_nCmdShow);
    pMainFrame->UpdateWindow();

    return TRUE;
}

// MainFrm.h
class CMainFrame : public CMDIFrameWnd
{
    DECLARE_DYNAMIC(CMainFrame)

protected:
    CToolBar    m_wndToolBar;
    CStatusBar  m_wndStatusBar;
    CReBar      m_wndReBar;

public:
    CMainFrame();
    virtual ~CMainFrame();

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnUpdateFrameTitle(BOOL bAddToTitle);

    DECLARE_MESSAGE_MAP()
};

// MainFrm.cpp
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Create toolbar
    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT,
        WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER |
        CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
    {
        return -1;
    }

    if (!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
        return -1;

    // Create status bar
    if (!m_wndStatusBar.Create(this))
        return -1;

    static UINT indicators[] =
    {
        ID_SEPARATOR,
        ID_INDICATOR_CAPS,
        ID_INDICATOR_NUM,
        ID_INDICATOR_SCRL,
    };
    m_wndStatusBar.SetIndicators(indicators,
        sizeof(indicators)/sizeof(UINT));

    // Enable docking
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar);

    return 0;
}

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
    if (bAddToTitle)
    {
        // Get active document
        CDocument* pDoc = GetActiveDocument();

        if (pDoc)
        {
            CString title;
            title.Format(_T("%s - My MDI App"),
                        pDoc->GetTitle());
            SetWindowText(title);
        }
        else
        {
            SetWindowText(_T("My MDI App"));
        }
    }
}
```

---

## 11. Exercises

### Exercise 1: Basic MDI App

Create an MDI application with basic text editing capability.

---

### Exercise 2: Multiple Document Types

Create an MDI app supporting both .txt and .rtf files with different views.

---

### Exercise 3: Window Management

Add "Close All" and "Save All" commands to Window menu.

---

### Exercise 4: Custom Child Frames

Create different child frame styles for different document types.

---

### Exercise 5: Recent File List

Implement a working MRU (Most Recently Used) file list in File menu.

---

## Key Takeaways

1. ✅ MDI = multiple documents in one window
2. ✅ CMDIFrameWnd is main window
3. ✅ CMDIChildWnd is document window
4. ✅ CMultiDocTemplate creates MDI apps
5. ✅ Window menu automatically managed
6. ✅ Multiple document types supported
7. ✅ Child windows can be customized
8. ✅ MDI commands handle window arrangement
9. ✅ Each document has own undo/redo
10. ✅ Modern apps prefer tabs over MDI

---

## Summary

MDI applications enable sophisticated multi-document workflows. While less common in modern UIs, understanding MDI is valuable for maintaining legacy apps and understanding MFC architecture!

---

## Next Lesson Preview

In **Lesson 25**, we'll explore:
- Frame windows in detail
- CFrameWnd vs CMDIFrameWnd
- Frame customization
- Status bars and toolbars in frames
- Frame window management

**Congratulations on completing Lesson 24!**
