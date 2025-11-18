# Lesson 16: Introduction to Document/View Architecture

**Duration**: 8 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding Document/View Architecture
2. Why Document/View Architecture?
3. The Four Core Classes
4. Communication Between Components
5. Where to Put Your Code - The Critical Question
6. Creating Your First Document/View Application
7. The Application Wizard Generated Code
8. Understanding the File Structure
9. Common Beginner Mistakes
10. Exercises

---

## 1. Understanding Document/View Architecture

The Document/View architecture is **THE** fundamental design pattern in MFC. Understanding it is absolutely critical for successful MFC development. This architecture separates your application into distinct responsibilities:

### The Core Concept

```
┌─────────────────────────────────────────────────────┐
│                   APPLICATION                       │
│              (CWinApp derived class)                │
└─────────────────────────────────────────────────────┘
                        │
                        ↓
┌─────────────────────────────────────────────────────┐
│                  FRAME WINDOW                       │
│         (CFrameWnd or CMDIFrameWnd)                 │
│  - Manages the window border                        │
│  - Contains menu bar, toolbars, status bar          │
└─────────────────────────────────────────────────────┘
                        │
        ┌───────────────┴───────────────┐
        ↓                               ↓
┌──────────────────┐         ┌──────────────────┐
│    DOCUMENT      │         │      VIEW        │
│   (CDocument)    │←───────→│    (CView)       │
│                  │         │                  │
│  - Data          │         │  - Display       │
│  - Business      │         │  - User Input    │
│    Logic         │         │  - Interaction   │
└──────────────────┘         └──────────────────┘
```

### The Philosophy

**CRITICAL UNDERSTANDING**: The Document/View architecture enforces **separation of concerns**:

- **Document (CDocument)**: "What" - The data and business logic
- **View (CView)**: "How" - The presentation and user interaction
- **Frame (CFrameWnd)**: "Where" - The container and application shell
- **Application (CWinApp)**: "When" - The application lifecycle

---

## 2. Why Document/View Architecture?

### Problem Without Document/View

Imagine a text editor without this architecture:

```cpp
// BAD: Everything mixed together
class MyWindow : public CWnd
{
    CString m_text;              // Data
    CFont m_font;                // Display
    void OnPaint();              // Display
    void OnKeyDown();            // Input
    void SaveToFile();           // Data
    void OpenFile();             // Data
    void ChangeFont();           // Display
    // CHAOS! Everything is mixed!
};
```

**Problems**:
- ❌ Can't have multiple views of same data
- ❌ Hard to save/load data
- ❌ Difficult to maintain
- ❌ Can't separate data from display

### Solution With Document/View

```cpp
// GOOD: Separation of concerns

// Document: Manages ONLY the data
class CMyDocument : public CDocument
{
    CString m_text;              // Data
    void Serialize(CArchive& ar); // Save/Load
    void SetText(const CString& text);
    CString GetText() const;
};

// View: Manages ONLY the display and input
class CMyView : public CView
{
    void OnDraw(CDC* pDC);       // Display
    void OnKeyDown();            // Input
    CMyDocument* GetDocument();  // Access data
};
```

**Benefits**:
- ✅ Multiple views can show same document
- ✅ Data management is centralized
- ✅ Easy to save/load (serialization)
- ✅ Clear separation of responsibilities

---

## 3. The Four Core Classes

### 3.1 CWinApp - Application Class

**Purpose**: Represents the entire application

**Responsibilities**:
- Application initialization
- Message loop management
- Application shutdown
- Global application state

**WHERE YOUR CODE GOES**:
```cpp
// In YourApp.h
class CMyApp : public CWinApp
{
public:
    virtual BOOL InitInstance();      // ← Initialization code here
    virtual int ExitInstance();       // ← Cleanup code here
};

// In YourApp.cpp
BOOL CMyApp::InitInstance()
{
    // Put GLOBAL initialization here:
    // - COM initialization
    // - Enable visual styles
    // - Register document templates
    // - Create main window

    CWinApp::InitInstance();

    // Initialize OLE libraries
    if (!AfxOleInit())
    {
        AfxMessageBox(_T("OLE initialization failed"));
        return FALSE;
    }

    // Enable visual styles
    AfxEnableControlContainer();

    // Standard initialization
    SetRegistryKey(_T("My Company"));

    // Register document template
    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CMyDocument),
        RUNTIME_CLASS(CMainFrame),
        RUNTIME_CLASS(CMyView));
    AddDocTemplate(pDocTemplate);

    // Create main window
    m_pMainWnd = new CMainFrame;
    if (!((CMainFrame*)m_pMainWnd)->LoadFrame(IDR_MAINFRAME))
        return FALSE;

    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    return TRUE;
}
```

**WHY**: The application object runs for the entire lifetime of your program. Code here affects the ENTIRE application.

---

### 3.2 CFrameWnd - Frame Window Class

**Purpose**: The main window that contains everything else

**Responsibilities**:
- Window border and title bar
- Menu bar
- Toolbars
- Status bar
- Contains the view

**WHERE YOUR CODE GOES**:
```cpp
// In MainFrm.h
class CMainFrame : public CFrameWnd
{
protected:
    CStatusBar  m_wndStatusBar;      // Status bar
    CToolBar    m_wndToolBar;        // Toolbar

public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);  // ← Window style setup
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs,
                                CCreateContext* pContext);  // ← Client area setup

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);  // ← Toolbar/status bar creation
    DECLARE_MESSAGE_MAP()
};

// In MainFrm.cpp
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Create toolbar HERE
    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT) ||
        !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;
    }

    // Create status bar HERE
    if (!m_wndStatusBar.Create(this))
    {
        TRACE0("Failed to create status bar\n");
        return -1;
    }
    m_wndStatusBar.SetIndicators(indicators,
                                 sizeof(indicators)/sizeof(UINT));

    // Docking setup
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar);

    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    // Modify window styles HERE
    cs.style |= WS_MAXIMIZE;  // Start maximized
    cs.cx = 800;              // Set width
    cs.cy = 600;              // Set height

    return CFrameWnd::PreCreateWindow(cs);
}
```

**WHY**: The frame window is the container. Code here affects window appearance, menus, toolbars, and status bar.

---

### 3.3 CDocument - Document Class

**Purpose**: Manages your application's data

**Responsibilities**:
- Store application data
- Load data from files
- Save data to files
- Notify views when data changes
- Implement business logic

**WHERE YOUR CODE GOES**:
```cpp
// In MyDocument.h
class CMyDocument : public CDocument
{
protected:
    // PUT YOUR DATA MEMBERS HERE (not in the view!)
    CString m_strText;
    CArray<CPoint, CPoint&> m_points;
    int m_nCounter;
    MyCustomData m_data;

public:
    CMyDocument();
    virtual ~CMyDocument();

    // Data access methods
    void SetText(const CString& text);
    CString GetText() const { return m_strText; }

    void AddPoint(CPoint pt);
    int GetPointCount() const { return (int)m_points.GetSize(); }
    CPoint GetPoint(int index) const { return m_points[index]; }

    // Business logic methods
    void ProcessData();
    void CalculateResults();

protected:
    virtual BOOL OnNewDocument();         // ← Initialize new document
    virtual void Serialize(CArchive& ar); // ← Save/Load data
    virtual void DeleteContents();        // ← Clean up data

    DECLARE_DYNCREATE(CMyDocument)
    DECLARE_MESSAGE_MAP()
};

// In MyDocument.cpp
BOOL CMyDocument::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // Initialize data for NEW document HERE
    m_strText = _T("");
    m_points.RemoveAll();
    m_nCounter = 0;

    return TRUE;
}

void CMyDocument::Serialize(CArchive& ar)
{
    // Save/Load data HERE
    if (ar.IsStoring())
    {
        // Saving
        ar << m_strText;
        ar << m_nCounter;
        m_points.Serialize(ar);
    }
    else
    {
        // Loading
        ar >> m_strText;
        ar >> m_nCounter;
        m_points.Serialize(ar);
    }
}

void CMyDocument::DeleteContents()
{
    // Clean up data HERE (called before loading new document)
    m_strText.Empty();
    m_points.RemoveAll();
    m_nCounter = 0;

    CDocument::DeleteContents();
}

void CMyDocument::SetText(const CString& text)
{
    m_strText = text;
    SetModifiedFlag();  // CRITICAL: Mark document as modified
    UpdateAllViews(NULL);  // CRITICAL: Notify all views to redraw
}
```

**WHY**: The document is the "model" in MVC pattern. ALL data belongs here, NOT in the view!

---

### 3.4 CView - View Class

**Purpose**: Displays the document and handles user input

**Responsibilities**:
- Draw the document data
- Handle mouse input
- Handle keyboard input
- Handle menu commands that affect display
- Communicate changes to document

**WHERE YOUR CODE GOES**:
```cpp
// In MyView.h
class CMyView : public CView
{
protected:
    // DISPLAY-ONLY data (NOT document data!)
    CFont m_displayFont;
    COLORREF m_textColor;
    int m_nZoomLevel;

public:
    CMyDocument* GetDocument() const;  // Access document

protected:
    virtual void OnDraw(CDC* pDC);     // ← ALL drawing code HERE
    virtual void OnInitialUpdate();    // ← View initialization
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);  // ← Refresh

    // Input handlers
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

    // Command handlers
    afx_msg void OnViewZoomIn();
    afx_msg void OnViewZoomOut();

    DECLARE_DYNCREATE(CMyView)
    DECLARE_MESSAGE_MAP()
};

// In MyView.cpp
void CMyView::OnDraw(CDC* pDC)
{
    // ALL DRAWING CODE GOES HERE
    CMyDocument* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // Draw document data
    pDC->SetTextColor(m_textColor);
    pDC->SelectObject(&m_displayFont);

    CString text = pDoc->GetText();
    pDC->TextOut(10, 10, text);

    // Draw points
    for (int i = 0; i < pDoc->GetPointCount(); i++)
    {
        CPoint pt = pDoc->GetPoint(i);
        pDC->Ellipse(pt.x-5, pt.y-5, pt.x+5, pt.y+5);
    }
}

void CMyView::OnInitialUpdate()
{
    CView::OnInitialUpdate();

    // Initialize VIEW-SPECIFIC data HERE
    m_displayFont.CreatePointFont(120, _T("Arial"));
    m_textColor = RGB(0, 0, 0);
    m_nZoomLevel = 100;
}

void CMyView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    // Called when document data changes
    Invalidate();  // Request redraw
}

void CMyView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // Handle mouse input
    CMyDocument* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    // Add point to DOCUMENT (not to view!)
    pDoc->AddPoint(point);
    // Document will call UpdateAllViews(), which calls OnUpdate()

    CView::OnLButtonDown(nFlags, point);
}

void CMyView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // Handle keyboard input
    CMyDocument* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    // Append character to document text
    CString text = pDoc->GetText();
    text += (TCHAR)nChar;
    pDoc->SetText(text);

    CView::OnChar(nChar, nRepCnt, nFlags);
}

// Non-inline version of GetDocument
CMyDocument* CMyView::GetDocument() const
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyDocument)));
    return (CMyDocument*)m_pDocument;
}
```

**WHY**: The view is the "presenter" and "controller". It ONLY handles display and input, NOT data storage!

---

## 4. Communication Between Components

### 4.1 Document → View Communication

**When**: Document data changes and views need to update

**How**: Document calls `UpdateAllViews()`

```cpp
// In Document
void CMyDocument::SetText(const CString& text)
{
    m_strText = text;
    SetModifiedFlag();        // Mark as modified (for save prompt)
    UpdateAllViews(NULL);     // Notify ALL views to update
}

// With hint parameter (more efficient)
void CMyDocument::AddPoint(CPoint pt)
{
    m_points.Add(pt);
    SetModifiedFlag();

    // Pass hint: 1 = point added
    UpdateAllViews(NULL, 1, (CObject*)&pt);
}
```

**In View**: Receive notification

```cpp
void CMyView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    if (lHint == 0)
    {
        // Full update
        Invalidate();
    }
    else if (lHint == 1)
    {
        // Just one point added - optimize redraw
        CPoint* pPoint = (CPoint*)pHint;
        CRect rect(pPoint->x-10, pPoint->y-10,
                   pPoint->x+10, pPoint->y+10);
        InvalidateRect(rect);
    }
}
```

---

### 4.2 View → Document Communication

**When**: User input needs to modify document data

**How**: View calls document methods

```cpp
// In View
void CMyView::OnLButtonDown(UINT nFlags, CPoint point)
{
    CMyDocument* pDoc = GetDocument();

    // Call document method to modify data
    pDoc->AddPoint(point);
    // Document will call UpdateAllViews()
}
```

**CRITICAL RULE**: View NEVER modifies document data directly!

```cpp
// ❌ WRONG - Don't do this!
void CMyView::OnLButtonDown(UINT nFlags, CPoint point)
{
    CMyDocument* pDoc = GetDocument();
    pDoc->m_points.Add(point);  // Direct modification - BAD!
    Invalidate();
}

// ✅ RIGHT - Do this!
void CMyView::OnLButtonDown(UINT nFlags, CPoint point)
{
    CMyDocument* pDoc = GetDocument();
    pDoc->AddPoint(point);  // Use document method - GOOD!
    // Let document notify views
}
```

---

### 4.3 View → Frame Communication

**When**: View needs to update status bar or change toolbar state

**How**: Get frame pointer

```cpp
void CMyView::OnMouseMove(UINT nFlags, CPoint point)
{
    // Update status bar with mouse coordinates
    CMainFrame* pFrame = (CMainFrame*)GetParentFrame();
    CString status;
    status.Format(_T("X: %d, Y: %d"), point.x, point.y);
    pFrame->SetMessageText(status);

    CView::OnMouseMove(nFlags, point);
}
```

---

## 5. Where to Put Your Code - The Critical Question

This is THE most important section for beginners!

### 5.1 Decision Tree

```
Is it DATA?
  YES → Put in CDocument
    - All variables that need to be saved
    - All business logic
    - All data processing

  NO → Is it about DISPLAY or INPUT?
    YES → Put in CView
      - Drawing code
      - Mouse handlers
      - Keyboard handlers
      - Display preferences

    NO → Is it about WINDOW STRUCTURE?
      YES → Put in CFrameWnd
        - Toolbars
        - Status bar
        - Menu creation
        - Window sizing

      NO → Is it GLOBAL to application?
        YES → Put in CWinApp
          - Initialization
          - Shutdown
          - Global settings
```

---

### 5.2 Common Scenarios

#### Scenario 1: Text Editor

**Question**: Where do I store the text being edited?

**Answer**: `CDocument`

```cpp
// In CMyDocument
class CMyDocument : public CDocument
{
    CString m_strText;  // ✅ Document data
};
```

**Why**: The text IS the document data. It needs to be saved/loaded.

---

#### Scenario 2: Display Font

**Question**: Where do I store the font used to display text?

**Answer**: `CView`

```cpp
// In CMyView
class CMyView : public CView
{
    CFont m_displayFont;  // ✅ Display preference
};
```

**Why**: The font is a DISPLAY preference, not document data. Different views might use different fonts.

---

#### Scenario 3: Zoom Level

**Question**: Where do I store the zoom level?

**Answer**: `CView` (usually)

```cpp
// In CMyView
class CMyView : public CView
{
    int m_nZoomLevel;  // ✅ View preference
};
```

**Why**: Zoom is typically a view preference. Different views can have different zoom levels.

**Exception**: If zoom should be saved with document, put in CDocument.

---

#### Scenario 4: Drawing Canvas

**Question**: Where do I handle mouse clicks to draw?

**Answer**: `CView` handles input, `CDocument` stores data

```cpp
// In CMyView
void CMyView::OnLButtonDown(UINT nFlags, CPoint point)
{
    CMyDocument* pDoc = GetDocument();
    pDoc->AddDrawPoint(point);  // Store in document
}

// In CMyDocument
void CMyDocument::AddDrawPoint(CPoint pt)
{
    m_drawPoints.Add(pt);  // Store data
    SetModifiedFlag();
    UpdateAllViews(NULL);
}
```

**Why**: Input handled by view, data stored in document.

---

#### Scenario 5: Status Bar Message

**Question**: Where do I update status bar text?

**Answer**: Usually in `CView`, sometimes in `CFrameWnd`

```cpp
// In CMyView
void CMyView::OnMouseMove(UINT nFlags, CPoint point)
{
    CMainFrame* pFrame = (CMainFrame*)GetParentFrame();
    CString msg;
    msg.Format(_T("Position: %d, %d"), point.x, point.y);
    pFrame->SetMessageText(msg);
}
```

**Why**: Status bar is part of frame window.

---

## 6. Creating Your First Document/View Application

### Step-by-Step with MFC Application Wizard

#### Step 1: Create New Project
1. File → New → Project
2. Select "MFC Application"
3. Name: `MyFirstDocView`
4. Click OK

#### Step 2: Application Type
- **Application type**: Single document (SDI)
- **Project style**: MFC standard
- **Visual style**: Office 2007 (Blue theme)
- Click Next

#### Step 3: Compound Document Support
- Select: None
- Click Next

#### Step 4: Document Template Strings
- **File extension**: txt
- **File type name**: MyFirstDocView Document
- **Main frame caption**: MyFirstDocView
- Click Next

#### Step 5: Database Support
- Select: None
- Click Next

#### Step 6: User Interface Features
- ✅ Thick frame
- ✅ Minimize box
- ✅ Maximize box
- ✅ System menu
- ✅ Use a menu bar and toolbar
- ✅ Use a status bar
- Click Next

#### Step 7: Advanced Features
- ✅ Printing and print preview
- ✅ ActiveX controls
- Click Next

#### Step 8: Generated Classes
Review the classes that will be generated:
- `CMyFirstDocViewApp` : CWinApp
- `CMainFrame` : CFrameWnd
- `CMyFirstDocViewDoc` : CDocument
- `CMyFirstDocViewView` : CView

Click Finish

---

## 7. The Application Wizard Generated Code

### 7.1 File Structure

```
MyFirstDocView/
├── MyFirstDocView.h          // Application class header
├── MyFirstDocView.cpp        // Application class implementation
├── MainFrm.h                 // Frame window header
├── MainFrm.cpp               // Frame window implementation
├── MyFirstDocViewDoc.h       // Document class header
├── MyFirstDocViewDoc.cpp     // Document class implementation
├── MyFirstDocViewView.h      // View class header
├── MyFirstDocViewView.cpp    // View class implementation
├── Resource.h                // Resource IDs
├── MyFirstDocView.rc         // Resources (menus, dialogs, etc.)
└── stdafx.h                  // Precompiled header
```

---

### 7.2 Understanding InitInstance()

```cpp
// In MyFirstDocView.cpp
BOOL CMyFirstDocViewApp::InitInstance()
{
    // 1. Standard initialization
    CWinApp::InitInstance();

    // 2. Initialize OLE libraries (needed for file dialogs, etc.)
    if (!AfxOleInit())
    {
        AfxMessageBox(IDP_OLE_INIT_FAILED);
        return FALSE;
    }

    // 3. Enable visual styles
    AfxEnableControlContainer();

    // 4. Initialize common controls
    InitCommonControlsEx(&InitCtrls);

    // 5. Set registry key (for settings persistence)
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));

    // 6. Load standard INI file options
    LoadStdProfileSettings(4);  // Load MRU file list

    // 7. Register document template *** CRITICAL ***
    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,                         // Resource ID
        RUNTIME_CLASS(CMyFirstDocViewDoc),     // Document class
        RUNTIME_CLASS(CMainFrame),             // Frame class
        RUNTIME_CLASS(CMyFirstDocViewView));   // View class
    AddDocTemplate(pDocTemplate);

    // 8. Parse command line (File→Open double-clicked documents)
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // 9. Process command line (creates document+view+frame)
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    // 10. Show and update window
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    return TRUE;
}
```

**Key Point**: The document template (`CSingleDocTemplate`) ties together:
- The document class
- The view class
- The frame class

This is how MFC knows which classes to instantiate!

---

## 8. Understanding the File Structure

### 8.1 Object Creation Sequence

When you run the application:

```
1. CMyFirstDocViewApp object created (global)
2. WinMain() calls InitInstance()
3. InitInstance() creates CSingleDocTemplate
4. ProcessShellCommand() creates:
   a. CMyFirstDocViewDoc (document)
   b. CMainFrame (frame window)
   c. CMyFirstDocViewView (view)
5. Frame window shown
6. View displays document
```

---

### 8.2 Object Relationships

```cpp
// Get document from view
CMyDocument* pDoc = GetDocument();

// Get view from document
POSITION pos = GetFirstViewPosition();
CView* pView = GetNextView(pos);

// Get frame from view
CFrameWnd* pFrame = GetParentFrame();

// Get active document from application
CDocument* pDoc =
    ((CFrameWnd*)AfxGetMainWnd())->GetActiveDocument();

// Get active view from frame
CView* pView =
    ((CFrameWnd*)AfxGetMainWnd())->GetActiveView();
```

---

## 9. Common Beginner Mistakes

### Mistake 1: Storing Data in View

```cpp
// ❌ WRONG
class CMyView : public CView
{
    CString m_text;  // Data in view - WRONG!
};

// ✅ RIGHT
class CMyDocument : public CDocument
{
    CString m_text;  // Data in document - RIGHT!
};
```

**Why Wrong**: Data in view won't be saved, can't have multiple views.

---

### Mistake 2: Not Calling UpdateAllViews()

```cpp
// ❌ WRONG
void CMyDocument::SetText(const CString& text)
{
    m_text = text;
    // Views won't update!
}

// ✅ RIGHT
void CMyDocument::SetText(const CString& text)
{
    m_text = text;
    SetModifiedFlag();
    UpdateAllViews(NULL);  // Notify views!
}
```

---

### Mistake 3: Not Calling SetModifiedFlag()

```cpp
// ❌ WRONG
void CMyDocument::SetText(const CString& text)
{
    m_text = text;
    UpdateAllViews(NULL);
    // No save prompt when closing!
}

// ✅ RIGHT
void CMyDocument::SetText(const CString& text)
{
    m_text = text;
    SetModifiedFlag();      // Mark as modified!
    UpdateAllViews(NULL);
}
```

---

### Mistake 4: Forgetting ASSERT_VALID

```cpp
// ❌ RISKY
void CMyView::OnDraw(CDC* pDC)
{
    CMyDocument* pDoc = GetDocument();
    CString text = pDoc->GetText();  // Might crash!
}

// ✅ SAFE
void CMyView::OnDraw(CDC* pDC)
{
    CMyDocument* pDoc = GetDocument();
    ASSERT_VALID(pDoc);  // Debug check
    if (!pDoc)
        return;          // Release check

    CString text = pDoc->GetText();
}
```

---

### Mistake 5: Wrong Serialization

```cpp
// ❌ WRONG
void CMyDocument::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // Save m_text only
        ar << m_text;
    }
    // Forgot to load!
}

// ✅ RIGHT
void CMyDocument::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        ar << m_text;
    }
    else
    {
        ar >> m_text;  // Load as well!
    }
}
```

---

## 10. Exercises

### Exercise 1: Simple Text Storage

Create a Doc/View app that stores and displays a single line of text.

**Requirements**:
1. Store text in document
2. Display text in view
3. Allow text to be saved/loaded

**Hints**:
- Add `CString m_text;` to document
- Display in `OnDraw()`
- Implement `Serialize()`

---

### Exercise 2: Click Counter

Create an app that counts mouse clicks.

**Requirements**:
1. Store click count in document
2. Display count in view
3. Increment on mouse click
4. Save/load count

**Hints**:
- Add `int m_nClickCount;` to document
- Handle `OnLButtonDown()` in view
- Use `UpdateAllViews()` after incrementing

---

### Exercise 3: Point Drawing

Create an app that stores and displays clicked points.

**Requirements**:
1. Store array of points in document
2. Draw all points in view
3. Add point on click
4. Save/load points

**Hints**:
- Use `CArray<CPoint, CPoint&> m_points;`
- Draw circles at each point in `OnDraw()`
- `CArray` has `Serialize()` method

---

### Exercise 4: Status Bar Update

Extend Exercise 3 to show point count in status bar.

**Requirements**:
1. Display "Points: N" in status bar
2. Update after each click

**Hints**:
- Get frame: `GetParentFrame()`
- Call `SetMessageText()`

---

### Exercise 5: Data Validation

Create an app with text input that validates data.

**Requirements**:
1. Store validated text in document
2. Reject invalid input (e.g., only numbers)
3. Show error message for invalid input

**Hints**:
- Validate in document method
- Return bool for success/failure
- Show `AfxMessageBox()` for errors

---

## Key Takeaways

1. ✅ **Document/View separates data from presentation**
2. ✅ **CDocument stores ALL data**
3. ✅ **CView handles display and input ONLY**
4. ✅ **CFrameWnd manages window structure**
5. ✅ **CWinApp handles application lifecycle**
6. ✅ **UpdateAllViews() notifies views of changes**
7. ✅ **SetModifiedFlag() enables save prompts**
8. ✅ **GetDocument() accesses data from view**
9. ✅ **Serialize() saves/loads document data**
10. ✅ **NEVER store document data in view!**

---

## Summary

The Document/View architecture is the foundation of MFC. Understanding WHERE to put code is critical:

- **Data** → Document
- **Display** → View
- **Window Structure** → Frame
- **Application Logic** → Application

Master this concept and the rest of MFC becomes much easier!

---

## Next Lesson Preview

In **Lesson 17**, we'll dive deeper into:
- SDI (Single Document Interface) applications
- Creating and customizing SDI apps
- Understanding the SDI lifecycle
- Building a complete SDI text editor

**Congratulations on completing Lesson 16!**

You now understand the fundamental architecture of MFC applications!
