# Lesson 17: SDI Applications (Single Document Interface)

**Duration**: 7 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding SDI Architecture
2. SDI vs MDI - When to Use Each
3. The SDI Application Lifecycle
4. Creating an SDI Application Step-by-Step
5. Customizing Your SDI Application
6. File Operations in SDI
7. The Document Template
8. Command Line Processing
9. Building a Complete SDI Text Editor
10. Exercises

---

## 1. Understanding SDI Architecture

### What is SDI?

**SDI (Single Document Interface)** means your application can work with **only ONE document at a time**.

```
┌─────────────────────────────────────────┐
│  Application Window (Frame)             │
│  ┌───────────────────────────────────┐  │
│  │         Menu Bar                  │  │
│  ├───────────────────────────────────┤  │
│  │         Toolbar                   │  │
│  ├───────────────────────────────────┤  │
│  │                                   │  │
│  │         View                      │  │
│  │    (Displays ONE Document)        │  │
│  │                                   │  │
│  │                                   │  │
│  ├───────────────────────────────────┤  │
│  │         Status Bar                │  │
│  └───────────────────────────────────┘  │
└─────────────────────────────────────────┘
```

### Real-World SDI Examples

- **Notepad** - One text file at a time
- **Paint** - One image at a time
- **Calculator** - One calculation at a time
- **Windows Explorer** - One folder view at a time (per window)
- **Adobe Acrobat Reader** - One PDF per window

---

## 2. SDI vs MDI - When to Use Each

### SDI (Single Document Interface)

**Use When**:
- ✅ Users typically work with one document at a time
- ✅ Simpler user interface is desired
- ✅ Documents are small and quick to switch
- ✅ You want multiple windows visible simultaneously (each window = one document)

**Advantages**:
- Simpler architecture
- Each document has its own window
- Easy to manage with taskbar
- Less memory overhead
- Cleaner code

**Disadvantages**:
- Can't compare documents side-by-side within same window
- More windows to manage
- Each window has its own menu bar (uses more screen space)

**Examples**:
```cpp
// Notepad - Simple text editor
// Paint - Simple image editor
// Calculator - Single calculation
```

---

### MDI (Multiple Document Interface)

**Use When**:
- Users need to work with multiple documents simultaneously
- Comparing documents is important
- Single window with multiple child windows is preferred
- Documents share common resources

**Advantages**:
- Multiple documents in one window
- Easy to compare documents
- Single menu bar for all documents
- Better for complex applications

**Disadvantages**:
- More complex architecture
- Can be confusing for users
- Less popular in modern UI design
- More code to manage

**Examples**:
```cpp
// Visual Studio - Multiple files open
// Excel - Multiple spreadsheets
// Word (classic) - Multiple documents
```

---

### Modern Trend

**Tabbed Interface** (Like Chrome, Firefox, Visual Studio Code):
- Each tab = one document
- One window, multiple tabs
- Best of both worlds
- MFC doesn't have built-in tab support (requires custom implementation)

---

## 3. The SDI Application Lifecycle

### Application Startup Sequence

```cpp
1. WinMain() Entry Point
   ↓
2. Global CWinApp Object Construction
   ↓
3. CWinApp::InitInstance() Called
   ↓
4. Register Document Templates
   ↓
5. Parse Command Line
   ↓
6. ProcessShellCommand()
   ↓
   ├─→ Create CDocument
   ├─→ Create CFrameWnd
   └─→ Create CView
   ↓
7. Show Main Window
   ↓
8. Message Loop Runs
```

### Detailed InitInstance() Flow

```cpp
BOOL CMyApp::InitInstance()
{
    // 1. Base class initialization
    CWinApp::InitInstance();

    // 2. Enable visual styles
    AfxEnableControlContainer();

    // 3. Initialize common controls
    InitCommonControls();

    // 4. Set registry key for settings
    SetRegistryKey(_T("MyCompany"));

    // 5. Load MRU (Most Recently Used) file list
    LoadStdProfileSettings(4);  // 4 = number of MRU files

    // 6. *** CREATE DOCUMENT TEMPLATE ***
    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,                    // Resource ID
        RUNTIME_CLASS(CMyDoc),            // Document class
        RUNTIME_CLASS(CMainFrame),        // Frame class
        RUNTIME_CLASS(CMyView));          // View class

    // 7. *** REGISTER TEMPLATE ***
    AddDocTemplate(pDocTemplate);

    // 8. Enable DDE (Dynamic Data Exchange) for File→Open
    EnableShellOpen();
    RegisterShellFileTypes(TRUE);

    // 9. Parse command line
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // 10. *** PROCESS COMMAND (Creates Doc/View/Frame) ***
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    // 11. Show window
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    // 12. Enable drag-drop for files
    m_pMainWnd->DragAcceptFiles();

    return TRUE;
}
```

---

## 4. Creating an SDI Application Step-by-Step

### Using MFC Application Wizard

#### Step 1: Create New Project

```
File → New → Project
Templates → Visual C++ → MFC → MFC Application
Name: MySDIApp
Location: C:\Projects
Click OK
```

#### Step 2: Application Type

```
Application type:
  ● Single document                    ← SELECT THIS!
  ○ Multiple documents
  ○ Dialog based

Project style:
  ● MFC standard
  ○ Office

Visual style and colors:
  Office 2007 (Blue theme)             ← Modern look

Click Next
```

**WHY Single Document**: This creates SDI architecture with one document at a time.

---

#### Step 3: Compound Document Support

```
Compound document support:
  ● None                                ← SELECT THIS!
  ○ Container
  ○ Mini-server
  ○ Full-server

Support for compound files:
  ☐ Yes, please                         ← UNCHECKED for simple apps

Click Next
```

**WHY None**: Unless you need OLE embedding (like embedding Excel in your app), select None.

---

#### Step 4: Document Template Properties

```
File extension: txt                     ← YOUR file extension
File type ID: MySDIApp.Document.1      ← Internal ID
Main frame caption: MySDIApp            ← Window title
File type name: MySDIApp Document       ← Description

Filter name: MySDIApp Files (*.txt)     ← File dialog filter
File new short name: Document           ← "New Document" name

Click Next
```

**IMPORTANT**: The file extension determines what files your app can open!

---

#### Step 5: Database Support

```
Database support:
  ● None                                ← SELECT THIS!
  ○ Header files only
  ○ Database view without file support
  ○ Database view with file support

Click Next
```

**WHY None**: Most SDI apps don't need database support.

---

#### Step 6: User Interface Features

```
Main frame styles:
  ☑ Thick frame                         ← Resizable window
  ☑ Minimize box
  ☑ Maximize box
  ☑ System menu

☑ Use a menu bar and toolbar
☑ Use a status bar

Toolbars:
  ☑ Main toolbar
  ☐ Dialog toolbar

Menu bar type:
  ● Classic                             ← Traditional menus
  ○ Office style

Project style:
  ☐ Docking toolbar                     ← Allow toolbar docking
  ☐ Docking pane                        ← Allow pane docking

Click Next
```

---

#### Step 7: Advanced Features

```
Advanced frame window styles:
  ☑ Split window                        ← Allow splitting view

Advanced document features:
  ☑ Printing and print preview
  ☑ ActiveX controls
  ☐ ActiveX document container
  ☐ Document container support

Restart Manager:
  ☐ Support Restart Manager

Click Next
```

---

#### Step 8: Generated Classes

```
Generated classes:
  CMySDIAppApp : public CWinApp
  CMainFrame : public CFrameWnd
  CMySDIAppDoc : public CDocument
  CMySDIAppView : public CView

Base class for CMySDIAppView:
  ▼ CView                               ← Can change to CEditView, etc.

Click Finish
```

**View Base Class Options**:
- `CView` - Basic view (you draw everything)
- `CScrollView` - Automatic scrolling support
- `CEditView` - Built-in text editing
- `CFormView` - Dialog-based view
- `CListView` - List control view
- `CTreeView` - Tree control view
- `CRichEditView` - Rich text editing
- `CHTMLView` - HTML display

---

## 5. Customizing Your SDI Application

### 5.1 Customizing the Frame Window

```cpp
// In MainFrm.h
class CMainFrame : public CFrameWnd
{
public:
    CMainFrame();
    virtual ~CMainFrame();

    // Customize window creation
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()
};

// In MainFrm.cpp
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    // Modify window styles BEFORE creation
    if (!CFrameWnd::PreCreateWindow(cs))
        return FALSE;

    // Set initial size
    cs.cx = 800;                    // Width
    cs.cy = 600;                    // Height

    // Center on screen
    cs.x = (GetSystemMetrics(SM_CXSCREEN) - cs.cx) / 2;
    cs.y = (GetSystemMetrics(SM_CYSCREEN) - cs.cy) / 2;

    // Modify window style
    cs.style |= WS_MAXIMIZE;        // Start maximized
    // cs.style &= ~WS_MAXIMIZEBOX;  // Remove maximize button
    // cs.style &= ~WS_MINIMIZEBOX;  // Remove minimize button
    // cs.style &= ~WS_THICKFRAME;   // Make non-resizable

    // Set window class name (for custom icon)
    cs.lpszClass = AfxRegisterWndClass(
        CS_DBLCLKS,                 // Double-click support
        ::LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)::GetStockObject(WHITE_BRUSH),
        AfxGetApp()->LoadIcon(IDR_MAINFRAME));

    return TRUE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Create toolbar
    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT,
        WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER |
        CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;
    }

    if (!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
    {
        TRACE0("Failed to load toolbar\n");
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
        ID_SEPARATOR,           // Status line indicator
        ID_INDICATOR_CAPS,      // Caps Lock
        ID_INDICATOR_NUM,       // Num Lock
        ID_INDICATOR_SCRL,      // Scroll Lock
    };

    m_wndStatusBar.SetIndicators(indicators,
        sizeof(indicators)/sizeof(UINT));

    // Enable toolbar docking
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar);

    return 0;
}
```

---

### 5.2 Customizing the Document

```cpp
// In MySDIAppDoc.h
class CMySDIAppDoc : public CDocument
{
protected:
    // YOUR DATA HERE
    CString m_strContent;
    CTime m_timeCreated;
    CTime m_timeModified;

public:
    CMySDIAppDoc();
    virtual ~CMySDIAppDoc();

    // Data access
    void SetContent(const CString& content);
    CString GetContent() const { return m_strContent; }

    CTime GetCreatedTime() const { return m_timeCreated; }
    CTime GetModifiedTime() const { return m_timeModified; }

protected:
    virtual BOOL OnNewDocument();
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();
    virtual void Serialize(CArchive& ar);

    DECLARE_DYNCREATE(CMySDIAppDoc)
    DECLARE_MESSAGE_MAP()
};

// In MySDIAppDoc.cpp
BOOL CMySDIAppDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // Initialize new document
    m_strContent.Empty();
    m_timeCreated = CTime::GetCurrentTime();
    m_timeModified = m_timeCreated;

    return TRUE;
}

BOOL CMySDIAppDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    // Called BEFORE file is loaded
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;

    // Custom processing after load
    m_timeModified = CTime::GetCurrentTime();

    return TRUE;
}

BOOL CMySDIAppDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    // Custom processing before save
    m_timeModified = CTime::GetCurrentTime();

    if (!CDocument::OnSaveDocument(lpszPathName))
        return FALSE;

    // Custom processing after save
    return TRUE;
}

void CMySDIAppDoc::DeleteContents()
{
    // Clean up document data
    m_strContent.Empty();

    CDocument::DeleteContents();
}

void CMySDIAppDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // Saving
        ar << m_strContent;
        ar << m_timeCreated;
        ar << m_timeModified;
    }
    else
    {
        // Loading
        ar >> m_strContent;
        ar >> m_timeCreated;
        ar >> m_timeModified;
    }
}

void CMySDIAppDoc::SetContent(const CString& content)
{
    m_strContent = content;
    m_timeModified = CTime::GetCurrentTime();
    SetModifiedFlag();          // Enable save prompt
    UpdateAllViews(NULL);       // Refresh all views
}
```

---

### 5.3 Customizing the View

```cpp
// In MySDIAppView.h
class CMySDIAppView : public CView
{
protected:
    // Display preferences (NOT document data!)
    CFont m_font;
    COLORREF m_textColor;
    COLORREF m_bgColor;

public:
    CMySDIAppView();
    virtual ~CMySDIAppView();

    CMySDIAppDoc* GetDocument() const;

protected:
    virtual void OnDraw(CDC* pDC);
    virtual void OnInitialUpdate();
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    // Input handlers
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

    DECLARE_DYNCREATE(CMySDIAppView)
    DECLARE_MESSAGE_MAP()
};

// In MySDIAppView.cpp
CMySDIAppView::CMySDIAppView()
    : m_textColor(RGB(0, 0, 0))
    , m_bgColor(RGB(255, 255, 255))
{
}

void CMySDIAppView::OnInitialUpdate()
{
    CView::OnInitialUpdate();

    // Initialize view-specific data
    m_font.CreatePointFont(100, _T("Courier New"));

    // Set background color
    SetClassLongPtr(m_hWnd, GCLP_HBRBACKGROUND,
        (LONG_PTR)CreateSolidBrush(m_bgColor));
}

void CMySDIAppView::OnDraw(CDC* pDC)
{
    CMySDIAppDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // Set up drawing
    CFont* pOldFont = pDC->SelectObject(&m_font);
    pDC->SetTextColor(m_textColor);
    pDC->SetBkColor(m_bgColor);

    // Get document content
    CString content = pDoc->GetContent();

    // Draw content
    CRect rect;
    GetClientRect(&rect);
    pDC->DrawText(content, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK);

    // Restore DC
    pDC->SelectObject(pOldFont);
}

void CMySDIAppView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    // Document changed - redraw
    Invalidate();
}

void CMySDIAppView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CMySDIAppDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    if (nChar == VK_BACK)
    {
        // Backspace
        CString content = pDoc->GetContent();
        if (!content.IsEmpty())
        {
            content.Delete(content.GetLength() - 1);
            pDoc->SetContent(content);
        }
    }
    else if (nChar >= 32)  // Printable character
    {
        // Append character
        CString content = pDoc->GetContent();
        content += (TCHAR)nChar;
        pDoc->SetContent(content);
    }

    CView::OnChar(nChar, nRepCnt, nFlags);
}
```

---

## 6. File Operations in SDI

### 6.1 File→New

```cpp
// Handled automatically by framework
// Sequence:
1. Check if current document is modified
2. If modified, prompt "Save changes?"
3. Delete current document contents (DeleteContents())
4. Create new document (OnNewDocument())
5. Update views
```

**You Override**:
```cpp
BOOL CMyDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // Initialize your data here
    m_data.Clear();

    return TRUE;
}
```

---

### 6.2 File→Open

```cpp
// Handled automatically by framework
// Sequence:
1. Show File Open dialog
2. User selects file
3. Check if current document is modified
4. If modified, prompt "Save changes?"
5. Delete current document contents (DeleteContents())
6. Call OnOpenDocument()
7. Call Serialize() to load data
8. Update views
```

**You Override**:
```cpp
BOOL CMyDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;

    // Custom post-load processing
    ProcessLoadedData();

    return TRUE;
}

void CMyDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // Saving (not used during Open)
    }
    else
    {
        // Loading - READ YOUR DATA HERE
        ar >> m_data;
    }
}
```

---

### 6.3 File→Save / Save As

```cpp
// File→Save sequence:
1. If document has never been saved, call File→Save As
2. Otherwise, call OnSaveDocument(existing path)
3. Call Serialize() to save data
4. Clear modified flag

// File→Save As sequence:
1. Show File Save dialog
2. User enters filename
3. Call OnSaveDocument(new path)
4. Call Serialize() to save data
5. Update document title
6. Clear modified flag
```

**You Override**:
```cpp
BOOL CMyDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    // Custom pre-save processing
    PrepareDataForSaving();

    if (!CDocument::OnSaveDocument(lpszPathName))
        return FALSE;

    // Custom post-save processing
    return TRUE;
}

void CMyDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // Saving - WRITE YOUR DATA HERE
        ar << m_data;
    }
    else
    {
        // Loading (not used during Save)
    }
}
```

---

### 6.4 Custom File Formats

```cpp
// Override to support multiple file formats
BOOL CMyDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    // Don't call base class - handle ourselves
    DeleteContents();

    CString path = lpszPathName;
    CString ext = path.Right(4);
    ext.MakeLower();

    if (ext == _T(".txt"))
    {
        return LoadTextFormat(path);
    }
    else if (ext == _T(".xml"))
    {
        return LoadXMLFormat(path);
    }
    else if (ext == _T(".bin"))
    {
        return LoadBinaryFormat(path);
    }

    AfxMessageBox(_T("Unsupported file format"));
    return FALSE;
}

BOOL CMyDoc::LoadTextFormat(LPCTSTR lpszPathName)
{
    CStdioFile file;
    if (!file.Open(lpszPathName, CFile::modeRead | CFile::typeText))
        return FALSE;

    CString line;
    m_strContent.Empty();

    while (file.ReadString(line))
    {
        m_strContent += line;
        m_strContent += _T("\r\n");
    }

    file.Close();
    SetModifiedFlag(FALSE);
    return TRUE;
}
```

---

## 7. The Document Template

### What is a Document Template?

A **document template** tells MFC how to create the document/view/frame trio.

```cpp
CSingleDocTemplate* pDocTemplate;
pDocTemplate = new CSingleDocTemplate(
    IDR_MAINFRAME,              // Resource ID (menu, icon, strings)
    RUNTIME_CLASS(CMyDoc),      // Document class
    RUNTIME_CLASS(CMainFrame),  // Frame class
    RUNTIME_CLASS(CMyView));    // View class

AddDocTemplate(pDocTemplate);
```

---

### Document Template Resources

**IDR_MAINFRAME** in Resource.h:
```cpp
#define IDR_MAINFRAME  128
```

**In .rc file**, this ID is used for:
- Menu (`IDR_MAINFRAME MENU`)
- Icon (`IDR_MAINFRAME ICON`)
- Accelerators (`IDR_MAINFRAME ACCELERATORS`)
- String table entry

**String Table Entry**:
```
IDR_MAINFRAME = "\nMyApp\nMyApp\nMyApp Files (*.txt)\n.txt\nMyApp.Document\nMyApp Document"
```

**Format** (separated by `\n`):
1. Window title prefix
2. Root for default document name
3. Document type name
4. Filter for File dialog
5. File extension
6. Registry file type ID
7. Registry file type name

---

### Multiple Document Types in SDI?

SDI typically supports ONE document type, but you can:

```cpp
// Add multiple filters to file dialog
virtual void OnFileOpen()
{
    CFileDialog dlg(TRUE, _T("txt"), NULL,
        OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
        _T("Text Files (*.txt)|*.txt|")
        _T("XML Files (*.xml)|*.xml|")
        _T("All Files (*.*)|*.*||"),
        AfxGetMainWnd());

    if (dlg.DoModal() == IDOK)
    {
        AfxGetApp()->OpenDocumentFile(dlg.GetPathName());
    }
}
```

---

## 8. Command Line Processing

### Default Command Line Support

```cpp
// In InitInstance()
CCommandLineInfo cmdInfo;
ParseCommandLine(cmdInfo);

if (!ProcessShellCommand(cmdInfo))
    return FALSE;
```

**Supports**:
- `MyApp.exe` - Create new document
- `MyApp.exe /p file.txt` - Print file
- `MyApp.exe /pt file.txt printer driver port` - Print to
- `MyApp.exe file.txt` - Open file
- Double-clicking .txt file in Explorer - Open file

---

### Custom Command Line

```cpp
class CMyCommandLineInfo : public CCommandLineInfo
{
public:
    BOOL m_bReadOnly;
    CString m_strCustomParam;

    CMyCommandLineInfo()
        : m_bReadOnly(FALSE)
    {
    }

    virtual void ParseParam(const TCHAR* pszParam,
                          BOOL bFlag, BOOL bLast)
    {
        if (bFlag && _tcsicmp(pszParam, _T("readonly")) == 0)
        {
            m_bReadOnly = TRUE;
        }
        else
        {
            CCommandLineInfo::ParseParam(pszParam, bFlag, bLast);
        }
    }
};

// In InitInstance()
CMyCommandLineInfo cmdInfo;
ParseCommandLine(cmdInfo);

if (cmdInfo.m_bReadOnly)
{
    // Open in read-only mode
    // Store this setting somewhere accessible
}

if (!ProcessShellCommand(cmdInfo))
    return FALSE;
```

**Usage**:
```
MyApp.exe /readonly file.txt
```

---

## 9. Building a Complete SDI Text Editor

### Complete Example - Simple Text Editor

#### MyTextDoc.h
```cpp
class CMyTextDoc : public CDocument
{
protected:
    CString m_strText;
    CFont m_font;

public:
    CMyTextDoc();
    virtual ~CMyTextDoc();

    void SetText(const CString& text);
    CString GetText() const { return m_strText; }

    void AppendText(const CString& text);
    void ClearText();

protected:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
    virtual void DeleteContents();

    DECLARE_DYNCREATE(CMyTextDoc)
    DECLARE_MESSAGE_MAP()
};
```

#### MyTextDoc.cpp
```cpp
IMPLEMENT_DYNCREATE(CMyTextDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyTextDoc, CDocument)
END_MESSAGE_MAP()

CMyTextDoc::CMyTextDoc()
{
}

CMyTextDoc::~CMyTextDoc()
{
}

BOOL CMyTextDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    m_strText.Empty();
    return TRUE;
}

void CMyTextDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        ar.WriteString(m_strText);
    }
    else
    {
        CString line;
        m_strText.Empty();

        while (ar.ReadString(line))
        {
            m_strText += line;
            m_strText += _T("\r\n");
        }
    }
}

void CMyTextDoc::DeleteContents()
{
    m_strText.Empty();
    CDocument::DeleteContents();
}

void CMyTextDoc::SetText(const CString& text)
{
    m_strText = text;
    SetModifiedFlag();
    UpdateAllViews(NULL);
}

void CMyTextDoc::AppendText(const CString& text)
{
    m_strText += text;
    SetModifiedFlag();
    UpdateAllViews(NULL);
}

void CMyTextDoc::ClearText()
{
    m_strText.Empty();
    SetModifiedFlag();
    UpdateAllViews(NULL);
}
```

#### MyTextView.h
```cpp
class CMyTextView : public CView
{
protected:
    CFont m_font;
    CPoint m_ptCursor;

public:
    CMyTextView();
    virtual ~CMyTextView();

    CMyTextDoc* GetDocument() const;

protected:
    virtual void OnDraw(CDC* pDC);
    virtual void OnInitialUpdate();
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);

    DECLARE_DYNCREATE(CMyTextView)
    DECLARE_MESSAGE_MAP()
};
```

#### MyTextView.cpp
```cpp
IMPLEMENT_DYNCREATE(CMyTextView, CView)

BEGIN_MESSAGE_MAP(CMyTextView, CView)
    ON_WM_CHAR()
    ON_WM_KEYDOWN()
    ON_WM_CREATE()
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

CMyTextView::CMyTextView()
    : m_ptCursor(0, 0)
{
}

int CMyTextView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_font.CreatePointFont(100, _T("Courier New"));

    return 0;
}

void CMyTextView::OnInitialUpdate()
{
    CView::OnInitialUpdate();

    // Set focus to receive keyboard input
    SetFocus();
}

void CMyTextView::OnDraw(CDC* pDC)
{
    CMyTextDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    CFont* pOldFont = pDC->SelectObject(&m_font);

    CString text = pDoc->GetText();

    CRect rect;
    GetClientRect(&rect);
    rect.DeflateRect(5, 5);

    pDC->DrawText(text, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK);

    pDC->SelectObject(pOldFont);
}

void CMyTextView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    Invalidate();
}

void CMyTextView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CMyTextDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    CString text = pDoc->GetText();

    if (nChar == VK_BACK)
    {
        if (!text.IsEmpty())
        {
            text.Delete(text.GetLength() - 1);
            pDoc->SetText(text);
        }
    }
    else if (nChar == VK_RETURN)
    {
        text += _T("\r\n");
        pDoc->SetText(text);
    }
    else if (nChar >= 32)
    {
        text += (TCHAR)nChar;
        pDoc->SetText(text);
    }

    CView::OnChar(nChar, nRepCnt, nFlags);
}

void CMyTextView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CMyTextDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    if (nChar == VK_DELETE)
    {
        if (GetKeyState(VK_CONTROL) & 0x8000)
        {
            // Ctrl+Delete - Clear all
            pDoc->ClearText();
        }
    }

    CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMyTextView::OnSetFocus(CWnd* pOldWnd)
{
    CView::OnSetFocus(pOldWnd);
    CreateSolidCaret(2, 15);
    ShowCaret();
}

void CMyTextView::OnKillFocus(CWnd* pNewWnd)
{
    HideCaret();
    CView::OnKillFocus(pNewWnd);
}
```

---

## 10. Exercises

### Exercise 1: Customized SDI App

Create an SDI application with:
- Custom window size (1024x768)
- Centered on screen
- Custom title bar text
- Disabled maximize button

---

### Exercise 2: Read-Only Mode

Add read-only mode:
- Command line: `/readonly`
- Disable text editing in read-only mode
- Show "Read-Only" in status bar

---

### Exercise 3: Auto-Save

Implement auto-save:
- Save every 5 minutes if modified
- Use a timer
- Show last save time in status bar

---

### Exercise 4: Word Count

Add word count feature:
- Count words in document
- Display in status bar
- Update on each change

---

### Exercise 5: Multiple File Formats

Support multiple formats:
- .txt (text)
- .rtf (rich text)
- .html (HTML)
- Auto-detect format on load
- Save in appropriate format

---

## Key Takeaways

1. ✅ SDI = One document per window
2. ✅ CSingleDocTemplate ties Doc/View/Frame together
3. ✅ InitInstance() creates the document template
4. ✅ ProcessShellCommand() creates Doc/View/Frame
5. ✅ File operations handled automatically
6. ✅ Override OnNewDocument(), OnOpenDocument(), OnSaveDocument()
7. ✅ Serialize() handles save/load
8. ✅ Command line support is built-in
9. ✅ Custom file formats require custom loading
10. ✅ View handles input, Document stores data

---

## Summary

SDI applications are perfect for:
- Simple, focused applications
- One task at a time
- Clean, uncluttered interface

Understanding SDI is essential before moving to MDI or more complex architectures.

---

## Next Lesson Preview

In **Lesson 18**, we'll deep-dive into:
- The CDocument class in detail
- Document data management
- Modified flag handling
- Document templates
- Best practices for document design

**Congratulations on completing Lesson 17!**
