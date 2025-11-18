# Lesson 19: Understanding CView Class

**Duration**: 8 hours
**Difficulty**: Intermediate

## Table of Contents
1. The CView Class - Deep Dive
2. View Types and Choosing the Right One
3. The OnDraw() Method - Core Rendering
4. Input Handling in Views
5. View Initialization and Lifetime
6. Scrolling Views
7. View Preferences vs Document Data
8. Multiple View Types for Same Document
9. View Communication Patterns
10. Complete Real-World Examples
11. Exercises

---

## 1. The CView Class - Deep Dive

### What is CView?

`CView` is the **VIEW and CONTROLLER** in the Model-View-Controller (MVC) pattern. It handles:
- **Display** (rendering document data)
- **Input** (mouse, keyboard)
- **User interaction** (commands, gestures)

```cpp
class CView : public CWnd
{
public:
    // Get associated document
    CDocument* GetDocument() const;

    // Drawing
    virtual void OnDraw(CDC* pDC) = 0;  // PURE VIRTUAL - must override!
    virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

    // Updating
    virtual void OnInitialUpdate();
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

    // Activation
    virtual void OnActivateView(BOOL bActivate,
                               CView* pActivateView,
                               CView* pDeactiveView);

protected:
    CDocument* m_pDocument;  // Associated document
};
```

---

## 2. View Types and Choosing the Right One

### 2.1 CView - Basic View

**Use When**: You need complete control over rendering.

```cpp
class CMyView : public CView
{
protected:
    virtual void OnDraw(CDC* pDC)
    {
        // You draw EVERYTHING
        CMyDocument* pDoc = GetDocument();
        // ... custom drawing code ...
    }
};
```

**Advantages**:
- ✅ Complete control
- ✅ Maximum flexibility
- ✅ Optimal performance

**Disadvantages**:
- ❌ More code to write
- ❌ Must handle everything yourself

**Examples**: Custom graphics, games, specialized displays

---

### 2.2 CScrollView - Scrolling View

**Use When**: Content is larger than window.

```cpp
class CMyScrollView : public CScrollView
{
protected:
    virtual void OnInitialUpdate()
    {
        CScrollView::OnInitialUpdate();

        // Set scroll sizes
        CSize sizeTotal(800, 600);      // Total scrollable area
        CSize sizePage(100, 100);        // Page scroll amount
        CSize sizeLine(10, 10);          // Line scroll amount

        SetScrollSizes(MM_TEXT, sizeTotal, sizePage, sizeLine);
    }

    virtual void OnDraw(CDC* pDC)
    {
        // Draw using logical coordinates
        // Scrolling handled automatically
    }
};
```

**Advantages**:
- ✅ Automatic scrollbars
- ✅ Automatic coordinate transformation
- ✅ Page up/down support

**Disadvantages**:
- ❌ Fixed-size scrolling only (can't dynamically resize easily)

**Examples**: Large documents, drawings, maps

---

### 2.3 CEditView - Text Editor View

**Use When**: Building a text editor.

```cpp
class CMyEditView : public CEditView
{
    // Built-in edit control!
    // Automatically handles:
    // - Text input
    // - Scrolling
    // - Cut/Copy/Paste
    // - Find/Replace
    // - Undo/Redo
};
```

**Advantages**:
- ✅ Instant text editor with zero code
- ✅ All editing features built-in
- ✅ Supports large files

**Disadvantages**:
- ❌ Plain text only (no formatting)
- ❌ Limited customization

**Examples**: Notepad clone, log viewers, code editors

---

### 2.4 CRichEditView - Rich Text Editor View

**Use When**: Building a word processor with formatting.

```cpp
class CMyRichEditView : public CRichEditView
{
    // Built-in rich edit control!
    // Automatically handles:
    // - Bold, italic, underline
    // - Fonts, colors, sizes
    // - Paragraph formatting
    // - RTF format
};
```

**Advantages**:
- ✅ Full formatting support
- ✅ All editing features
- ✅ RTF and plain text

**Disadvantages**:
- ❌ More complex than CEditView
- ❌ Larger memory footprint

**Examples**: WordPad clone, rich text editors

---

### 2.5 CFormView - Dialog-Based View

**Use When**: View is primarily controls (like a dialog).

```cpp
class CMyFormView : public CFormView
{
public:
    enum { IDD = IDD_MY_FORM };  // Dialog resource ID

protected:
    CEdit m_editName;
    CComboBox m_comboType;
    CButton m_btnSave;

    virtual void DoDataExchange(CDataExchange* pDX)
    {
        CFormView::DoDataExchange(pDX);
        DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
        DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
        DDX_Control(pDX, IDC_BTN_SAVE, m_btnSave);
    }
};
```

**Advantages**:
- ✅ Visual dialog editor
- ✅ Easy control layout
- ✅ Data exchange (DDX/DDV)

**Disadvantages**:
- ❌ Fixed layout (no automatic resizing)
- ❌ Not for custom graphics

**Examples**: Database forms, configuration screens, data entry

---

### 2.6 CListView - List Control View

**Use When**: Displaying lists or tables.

```cpp
class CMyListView : public CListView
{
protected:
    virtual void OnInitialUpdate()
    {
        CListView::OnInitialUpdate();

        CListCtrl& list = GetListCtrl();

        // Set view style
        list.ModifyStyle(0, LVS_REPORT);

        // Add columns
        list.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 200);
        list.InsertColumn(1, _T("Type"), LVCFMT_LEFT, 100);
        list.InsertColumn(2, _T("Size"), LVCFMT_RIGHT, 100);

        // Populate from document
        CMyDocument* pDoc = GetDocument();
        for (int i = 0; i < pDoc->GetItemCount(); i++)
        {
            CMyItem* pItem = pDoc->GetItem(i);
            int idx = list.InsertItem(i, pItem->GetName());
            list.SetItemText(idx, 1, pItem->GetType());
            list.SetItemText(idx, 2, pItem->GetSize());
        }
    }
};
```

**Advantages**:
- ✅ Built-in list control
- ✅ Sorting, grouping
- ✅ Icons, details, list views

**Disadvantages**:
- ❌ Limited customization
- ❌ Performance issues with huge lists

**Examples**: File managers, database browsers, lists

---

### 2.7 CTreeView - Tree Control View

**Use When**: Displaying hierarchical data.

```cpp
class CMyTreeView : public CTreeView
{
protected:
    virtual void OnInitialUpdate()
    {
        CTreeView::OnInitialUpdate();

        CTreeCtrl& tree = GetTreeCtrl();

        // Populate tree from document
        CMyDocument* pDoc = GetDocument();
        HTREEITEM hRoot = tree.InsertItem(_T("Root"));

        PopulateTree(hRoot, pDoc->GetRootNode());
        tree.Expand(hRoot, TVE_EXPAND);
    }

    void PopulateTree(HTREEITEM hParent, CNode* pNode)
    {
        CTreeCtrl& tree = GetTreeCtrl();

        for (int i = 0; i < pNode->GetChildCount(); i++)
        {
            CNode* pChild = pNode->GetChild(i);
            HTREEITEM hItem = tree.InsertItem(pChild->GetName(), hParent);

            if (pChild->HasChildren())
                PopulateTree(hItem, pChild);
        }
    }
};
```

**Advantages**:
- ✅ Built-in tree control
- ✅ Hierarchical display
- ✅ Expand/collapse

**Disadvantages**:
- ❌ Limited customization

**Examples**: File explorers, class browsers, XML viewers

---

### 2.8 CHtmlView - HTML Display View

**Use When**: Displaying HTML content.

```cpp
class CMyHtmlView : public CHtmlView
{
protected:
    virtual void OnInitialUpdate()
    {
        CHtmlView::OnInitialUpdate();

        // Navigate to URL
        Navigate2(_T("http://www.example.com"));

        // Or load HTML from string
        CString html = _T("<html><body><h1>Hello</h1></body></html>");
        LoadFromString(html);
    }
};
```

**Advantages**:
- ✅ Full HTML rendering
- ✅ JavaScript support
- ✅ Web browsing

**Disadvantages**:
- ❌ Heavy (Internet Explorer control)
- ❌ Deprecated technology

**Examples**: Help viewers, web browsers, HTML reports

---

## 3. The OnDraw() Method - Core Rendering

### 3.1 Understanding OnDraw()

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // CRITICAL RULES:
    // 1. Get document data
    // 2. Draw ONLY - don't modify data!
    // 3. Use pDC for all drawing
    // 4. Check document validity
    // 5. Be efficient - called frequently

    CMyDocument* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // Now draw...
}
```

**WHEN CALLED**:
- Window needs repainting
- After `Invalidate()` / `InvalidateRect()`
- After scrolling (in CScrollView)
- After window resize
- After switching applications
- When printer DC is provided (printing)

---

### 3.2 Device Context (CDC) Basics

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CMyDocument* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // --- Drawing Text ---

    // Simple text
    pDC->TextOut(10, 10, _T("Hello World"));

    // Formatted text
    CString text = pDoc->GetText();
    CRect rect;
    GetClientRect(&rect);
    pDC->DrawText(text, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK);

    // --- Drawing Shapes ---

    // Line
    pDC->MoveTo(0, 0);
    pDC->LineTo(100, 100);

    // Rectangle
    pDC->Rectangle(10, 10, 110, 60);

    // Ellipse
    pDC->Ellipse(50, 50, 150, 150);

    // Polygon
    CPoint points[] = {
        CPoint(100, 100),
        CPoint(150, 50),
        CPoint(200, 100),
        CPoint(150, 150)
    };
    pDC->Polygon(points, 4);

    // --- Colors and Pens ---

    // Create pen
    CPen redPen(PS_SOLID, 2, RGB(255, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&redPen);

    pDC->Rectangle(10, 10, 110, 60);  // Red outline

    pDC->SelectObject(pOldPen);  // IMPORTANT: Restore!

    // --- Brushes ---

    CBrush blueBrush(RGB(0, 0, 255));
    CBrush* pOldBrush = pDC->SelectObject(&blueBrush);

    pDC->Rectangle(120, 10, 220, 60);  // Blue fill

    pDC->SelectObject(pOldBrush);  // IMPORTANT: Restore!

    // --- Fonts ---

    CFont font;
    font.CreatePointFont(200, _T("Arial"));
    CFont* pOldFont = pDC->SelectObject(&font);

    pDC->TextOut(10, 100, _T("Large Arial"));

    pDC->SelectObject(pOldFont);  // IMPORTANT: Restore!

    // --- Text Color ---

    COLORREF oldColor = pDC->SetTextColor(RGB(255, 0, 0));
    pDC->TextOut(10, 150, _T("Red Text"));
    pDC->SetTextColor(oldColor);  // Restore

    // --- Background Mode ---

    int oldMode = pDC->SetBkMode(TRANSPARENT);
    pDC->TextOut(10, 200, _T("Transparent background"));
    pDC->SetBkMode(oldMode);  // Restore
}
```

---

### 3.3 Advanced OnDraw() Patterns

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CMyDocument* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // Save DC state
    int savedDC = pDC->SaveDC();

    // Set up drawing environment
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(0, 0, 0));

    // Create and select font
    CFont font;
    font.CreatePointFont(100, _T("Arial"));
    pDC->SelectObject(&font);

    // Get drawing area
    CRect rectClient;
    GetClientRect(&rectClient);

    // Draw background
    CBrush bgBrush(RGB(255, 255, 255));
    pDC->FillRect(&rectClient, &bgBrush);

    // Draw document content
    DrawContent(pDC, pDoc);

    // Draw selection
    if (m_bHasSelection)
    {
        DrawSelection(pDC);
    }

    // Draw grid (if enabled)
    if (m_bShowGrid)
    {
        DrawGrid(pDC);
    }

    // Restore DC state (automatic cleanup!)
    pDC->RestoreDC(savedDC);
}

void CMyView::DrawContent(CDC* pDC, CMyDocument* pDoc)
{
    // Draw all objects
    for (int i = 0; i < pDoc->GetObjectCount(); i++)
    {
        CDrawObject* pObj = pDoc->GetObject(i);
        pObj->Draw(pDC);
    }
}

void CMyView::DrawSelection(CDC* pDC)
{
    CPen pen(PS_DOT, 1, RGB(0, 0, 255));
    CPen* pOldPen = pDC->SelectObject(&pen);
    CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

    pDC->Rectangle(&m_rectSelection);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}

void CMyView::DrawGrid(CDC* pDC)
{
    CPen pen(PS_DOT, 1, RGB(200, 200, 200));
    CPen* pOldPen = pDC->SelectObject(&pen);

    CRect rect;
    GetClientRect(&rect);

    // Vertical lines
    for (int x = 0; x < rect.right; x += m_nGridSize)
    {
        pDC->MoveTo(x, 0);
        pDC->LineTo(x, rect.bottom);
    }

    // Horizontal lines
    for (int y = 0; y < rect.bottom; y += m_nGridSize)
    {
        pDC->MoveTo(0, y);
        pDC->LineTo(rect.right, y);
    }

    pDC->SelectObject(pOldPen);
}
```

---

## 4. Input Handling in Views

### 4.1 Mouse Input

```cpp
class CMyView : public CView
{
protected:
    CPoint m_ptMouseDown;
    CPoint m_ptMouseCurrent;
    BOOL m_bMouseDown;
    BOOL m_bMouseTracking;

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnMouseHover(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMyView, CView)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_RBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_MOUSEWHEEL()
    ON_WM_MOUSEHOVER()
    ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

void CMyView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // Capture mouse
    SetCapture();

    m_ptMouseDown = point;
    m_ptMouseCurrent = point;
    m_bMouseDown = TRUE;

    // Check modifiers
    BOOL bShift = (nFlags & MK_SHIFT) != 0;
    BOOL bCtrl = (nFlags & MK_CONTROL) != 0;

    if (bCtrl)
    {
        // Add to selection
        AddToSelection(point);
    }
    else
    {
        // Start new selection
        StartSelection(point);
    }

    CView::OnLButtonDown(nFlags, point);
}

void CMyView::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bMouseDown)
    {
        // Release mouse capture
        ReleaseCapture();

        m_bMouseDown = FALSE;

        // Finalize selection
        FinalizeSelection(point);
    }

    CView::OnLButtonUp(nFlags, point);
}

void CMyView::OnMouseMove(UINT nFlags, CPoint point)
{
    // Update status bar
    CMainFrame* pFrame = (CMainFrame*)GetParentFrame();
    CString status;
    status.Format(_T("X: %d, Y: %d"), point.x, point.y);
    pFrame->SetMessageText(status);

    if (m_bMouseDown)
    {
        // Dragging
        m_ptMouseCurrent = point;

        // Update rubber-band rectangle
        UpdateRubberBand(m_ptMouseDown, m_ptMouseCurrent);

        Invalidate();
    }
    else
    {
        // Hovering - update cursor
        CMyDocument* pDoc = GetDocument();
        CDrawObject* pObj = HitTest(point);

        if (pObj)
        {
            SetCursor(LoadCursor(NULL, IDC_HAND));
        }
        else
        {
            SetCursor(LoadCursor(NULL, IDC_ARROW));
        }
    }

    // Enable hover tracking
    if (!m_bMouseTracking)
    {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_HOVER | TME_LEAVE;
        tme.hwndTrack = m_hWnd;
        tme.dwHoverTime = HOVER_DEFAULT;
        TrackMouseEvent(&tme);
        m_bMouseTracking = TRUE;
    }

    CView::OnMouseMove(nFlags, point);
}

void CMyView::OnRButtonDown(UINT nFlags, CPoint point)
{
    // Show context menu
    CMenu menu;
    menu.LoadMenu(IDR_CONTEXT_MENU);
    CMenu* pPopup = menu.GetSubMenu(0);

    ClientToScreen(&point);
    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                          point.x, point.y, this);

    CView::OnRButtonDown(nFlags, point);
}

void CMyView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // Double-click - edit object
    CMyDocument* pDoc = GetDocument();
    CDrawObject* pObj = HitTest(point);

    if (pObj)
    {
        EditObject(pObj);
    }

    CView::OnLButtonDblClk(nFlags, point);
}

BOOL CMyView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // Zoom in/out
    if (nFlags & MK_CONTROL)
    {
        if (zDelta > 0)
            ZoomIn();
        else
            ZoomOut();

        return TRUE;
    }

    return CView::OnMouseWheel(nFlags, zDelta, pt);
}
```

---

### 4.2 Keyboard Input

```cpp
class CMyView : public CView
{
protected:
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMyView, CView)
    ON_WM_KEYDOWN()
    ON_WM_KEYUP()
    ON_WM_CHAR()
    ON_WM_SYSKEYDOWN()
END_MESSAGE_MAP()

void CMyView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CMyDocument* pDoc = GetDocument();

    switch (nChar)
    {
    case VK_DELETE:
        DeleteSelection();
        break;

    case VK_LEFT:
        MoveSelection(-1, 0);
        break;

    case VK_RIGHT:
        MoveSelection(1, 0);
        break;

    case VK_UP:
        MoveSelection(0, -1);
        break;

    case VK_DOWN:
        MoveSelection(0, 1);
        break;

    case VK_ESCAPE:
        CancelOperation();
        break;

    case VK_F5:
        Refresh();
        break;

    case 'A':
        if (GetKeyState(VK_CONTROL) & 0x8000)
        {
            SelectAll();
        }
        break;

    case 'C':
        if (GetKeyState(VK_CONTROL) & 0x8000)
        {
            Copy();
        }
        break;

    case 'V':
        if (GetKeyState(VK_CONTROL) & 0x8000)
        {
            Paste();
        }
        break;

    case 'Z':
        if (GetKeyState(VK_CONTROL) & 0x8000)
        {
            pDoc->Undo();
        }
        break;
    }

    CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMyView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar >= 32 && nChar < 127)
    {
        // Printable character
        HandleTextInput((TCHAR)nChar);
    }

    CView::OnChar(nChar, nRepCnt, nFlags);
}
```

---

## 5. View Initialization and Lifetime

### 5.1 View Lifecycle

```cpp
class CMyView : public CView
{
public:
    CMyView();
    virtual ~CMyView();

protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
    virtual void OnInitialUpdate();
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
    virtual void OnActivateView(BOOL bActivate,
                               CView* pActivateView,
                               CView* pDeactiveView);
    virtual void OnDestroy();
};

CMyView::CMyView()
{
    // Constructor - initialize member variables
    m_bShowGrid = FALSE;
    m_nGridSize = 20;
    m_nZoomLevel = 100;
    m_textColor = RGB(0, 0, 0);
}

CMyView::~CMyView()
{
    // Destructor - cleanup
    // GDI objects cleanup
    if (m_font.GetSafeHandle())
        m_font.DeleteObject();

    // Note: Don't delete document data here!
}

BOOL CMyView::PreCreateWindow(CREATESTRUCT& cs)
{
    // Modify window class before creation
    cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

    // Register custom window class
    cs.lpszClass = AfxRegisterWndClass(
        CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
        ::LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)::GetStockObject(WHITE_BRUSH),
        NULL);

    return CView::PreCreateWindow(cs);
}

int CMyView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Create GDI objects
    m_font.CreatePointFont(100, _T("Arial"));

    // Create child controls if needed
    // ...

    return 0;
}

void CMyView::OnInitialUpdate()
{
    CView::OnInitialUpdate();

    // Called ONCE when view is first created
    // Initialize view-specific data

    CMyDocument* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    // Set initial zoom
    m_nZoomLevel = 100;

    // Set initial scroll size (if CScrollView)
    // SetScrollSizes(...);

    // Request initial draw
    Invalidate();
}

void CMyView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    // Called when document changes

    if (lHint == HINT_FULL_UPDATE)
    {
        // Full update
        Invalidate();
    }
    else if (lHint == HINT_OBJECT_ADDED)
    {
        // Partial update
        CDrawObject* pObj = (CDrawObject*)pHint;
        CRect rect = pObj->GetBounds();
        InvalidateRect(&rect);
    }
    else
    {
        // Unknown hint - full update
        Invalidate();
    }
}

void CMyView::OnActivateView(BOOL bActivate,
                            CView* pActivateView,
                            CView* pDeactiveView)
{
    // Called when view becomes active/inactive

    if (bActivate)
    {
        // View is being activated
        SetFocus();
        // Update menus/toolbars for this view
    }
    else
    {
        // View is being deactivated
        // Save view state if needed
    }

    CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CMyView::OnDestroy()
{
    // Save view preferences
    SaveViewSettings();

    CView::OnDestroy();
}
```

---

## 6. Scrolling Views

### 6.1 CScrollView Basics

```cpp
class CMyScrollView : public CScrollView
{
protected:
    virtual void OnInitialUpdate()
    {
        CScrollView::OnInitialUpdate();

        // Set scroll sizes
        CSize sizeTotal(1000, 800);    // Total area
        CSize sizePage(100, 100);       // Page scroll
        CSize sizeLine(10, 10);         // Line scroll

        SetScrollSizes(MM_TEXT, sizeTotal, sizePage, sizeLine);
    }

    virtual void OnDraw(CDC* pDC)
    {
        CMyDocument* pDoc = GetDocument();
        ASSERT_VALID(pDoc);

        // Draw using LOGICAL coordinates
        // Scrolling handled automatically!

        pDC->TextOut(0, 0, _T("Top-Left"));
        pDC->TextOut(900, 700, _T("Bottom-Right"));
    }
};
```

---

### 6.2 Dynamic Scroll Sizes

```cpp
void CMyScrollView::UpdateScrollSizes()
{
    CMyDocument* pDoc = GetDocument();

    // Calculate total size from document
    CRect bounds = pDoc->GetBounds();

    CSize sizeTotal(bounds.Width() + 100, bounds.Height() + 100);

    SetScrollSizes(MM_TEXT, sizeTotal);

    Invalidate();
}

// Call when document bounds change
void CMyScrollView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    if (lHint == HINT_BOUNDS_CHANGED)
    {
        UpdateScrollSizes();
    }
    else
    {
        Invalidate();
    }
}
```

---

### 6.3 Scroll to Position

```cpp
void CMyScrollView::ScrollToObject(CDrawObject* pObj)
{
    CRect rect = pObj->GetBounds();
    CPoint pt = rect.TopLeft();

    // Scroll to make object visible
    ScrollToPosition(pt);

    // Or center on object
    CRect rectClient;
    GetClientRect(&rectClient);

    CPoint ptCenter(
        pt.x - rectClient.Width() / 2,
        pt.y - rectClient.Height() / 2);

    ScrollToPosition(ptCenter);
}
```

---

## 7. View Preferences vs Document Data

### CRITICAL DISTINCTION

```cpp
// ❌ WRONG - Display preferences in Document
class CMyDocument : public CDocument
{
    CFont m_displayFont;       // NO! This is view preference!
    COLORREF m_textColor;      // NO! This is view preference!
    int m_nZoomLevel;          // NO! This is view preference!
};

// ✅ RIGHT - Display preferences in View
class CMyView : public CView
{
    CFont m_displayFont;       // YES! View preference
    COLORREF m_textColor;      // YES! View preference
    int m_nZoomLevel;          // YES! View preference
};

// ✅ ALSO RIGHT - If saved with document
class CMyDocument : public CDocument
{
    COLORREF m_dataColor;      // YES! Part of document data
    CString m_fontName;        // YES! If it's part of the data
};
```

**Rule**: Ask yourself: "If I have two views, should they have different values?"
- **YES** → Put in CView
- **NO** → Put in CDocument

---

## 8. Multiple View Types for Same Document

### 8.1 Creating Multiple View Types

```cpp
// TextView.h - Text view of document
class CTextView : public CEditView
{
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
    {
        CMyDocument* pDoc = GetDocument();
        SetWindowText(pDoc->GetText());
    }
};

// GraphView.h - Graph view of same document
class CGraphView : public CView
{
    virtual void OnDraw(CDC* pDC)
    {
        CMyDocument* pDoc = GetDocument();
        DrawGraph(pDC, pDoc->GetData());
    }
};

// In App::InitInstance() - MDI only
CSingleDocTemplate* pDocTemplate;
pDocTemplate = new CMultiDocTemplate(
    IDR_MYTYPE,
    RUNTIME_CLASS(CMyDocument),
    RUNTIME_CLASS(CMDIChildWnd),
    RUNTIME_CLASS(CTextView));  // Default view
AddDocTemplate(pDocTemplate);

// Add second view type
CMultiDocTemplate* pGraphTemplate;
pGraphTemplate = new CMultiDocTemplate(
    IDR_GRAPH,
    RUNTIME_CLASS(CMyDocument),
    RUNTIME_CLASS(CMDIChildWnd),
    RUNTIME_CLASS(CGraphView));
AddDocTemplate(pGraphTemplate);
```

---

## 9. View Communication Patterns

### 9.1 View → Document → View

```cpp
// In View 1 (Text View)
void CTextView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // User types character

    CMyDocument* pDoc = GetDocument();

    // Modify document
    CString text = pDoc->GetText();
    text += (TCHAR)nChar;
    pDoc->SetText(text);  // This calls UpdateAllViews()

    // Document notifies ALL views (including graph view)

    CEditView::OnChar(nChar, nRepCnt, nFlags);
}

// In View 2 (Graph View)
void CGraphView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    // Receives notification from document
    // Redraw graph with new data
    Invalidate();
}
```

---

## 10. Complete Real-World Examples

### Example: Drawing Application View

```cpp
// DrawView.h
class CDrawView : public CScrollView
{
protected:
    // View preferences (NOT document data)
    BOOL m_bShowGrid;
    int m_nGridSize;
    int m_nZoomLevel;
    CPoint m_ptMouseDown;
    BOOL m_bDragging;
    CDrawObject* m_pSelectedObject;

public:
    CDrawView();
    virtual ~CDrawView();

    CDrawDoc* GetDocument() const;

protected:
    virtual void OnDraw(CDC* pDC);
    virtual void OnInitialUpdate();
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnViewGrid();
    afx_msg void OnUpdateViewGrid(CCmdUI* pCmdUI);
    afx_msg void OnViewZoomIn();
    afx_msg void OnViewZoomOut();

    void DrawGrid(CDC* pDC);
    CDrawObject* HitTest(CPoint point);

    DECLARE_DYNCREATE(CDrawView)
    DECLARE_MESSAGE_MAP()
};

// DrawView.cpp
IMPLEMENT_DYNCREATE(CDrawView, CScrollView)

BEGIN_MESSAGE_MAP(CDrawView, CScrollView)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_COMMAND(ID_VIEW_GRID, &CDrawView::OnViewGrid)
    ON_UPDATE_COMMAND_UI(ID_VIEW_GRID, &CDrawView::OnUpdateViewGrid)
    ON_COMMAND(ID_VIEW_ZOOMIN, &CDrawView::OnViewZoomIn)
    ON_COMMAND(ID_VIEW_ZOOMOUT, &CDrawView::OnViewZoomOut)
END_MESSAGE_MAP()

CDrawView::CDrawView()
    : m_bShowGrid(TRUE)
    , m_nGridSize(20)
    , m_nZoomLevel(100)
    , m_bDragging(FALSE)
    , m_pSelectedObject(NULL)
{
}

void CDrawView::OnInitialUpdate()
{
    CScrollView::OnInitialUpdate();

    CSize sizeTotal(2000, 2000);
    SetScrollSizes(MM_TEXT, sizeTotal);
}

void CDrawView::OnDraw(CDC* pDC)
{
    CDrawDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // Draw grid
    if (m_bShowGrid)
    {
        DrawGrid(pDC);
    }

    // Draw all objects
    for (int i = 0; i < pDoc->GetObjectCount(); i++)
    {
        CDrawObject* pObj = pDoc->GetObject(i);
        pObj->Draw(pDC);

        // Highlight selected
        if (pObj == m_pSelectedObject)
        {
            pObj->DrawSelection(pDC);
        }
    }
}

void CDrawView::DrawGrid(CDC* pDC)
{
    CPen pen(PS_DOT, 1, RGB(200, 200, 200));
    CPen* pOldPen = pDC->SelectObject(&pen);

    CRect rectClient;
    GetClientRect(&rectClient);

    CPoint ptOrg = GetScrollPosition();
    rectClient.OffsetRect(ptOrg);

    for (int x = 0; x < rectClient.right; x += m_nGridSize)
    {
        pDC->MoveTo(x, rectClient.top);
        pDC->LineTo(x, rectClient.bottom);
    }

    for (int y = 0; y < rectClient.bottom; y += m_nGridSize)
    {
        pDC->MoveTo(rectClient.left, y);
        pDC->LineTo(rectClient.right, y);
    }

    pDC->SelectObject(pOldPen);
}

void CDrawView::OnLButtonDown(UINT nFlags, CPoint point)
{
    CDrawDoc* pDoc = GetDocument();

    // Convert to logical coordinates
    CClientDC dc(this);
    OnPrepareDC(&dc);
    dc.DPtoLP(&point);

    // Hit test
    m_pSelectedObject = HitTest(point);

    if (m_pSelectedObject)
    {
        // Start dragging
        SetCapture();
        m_bDragging = TRUE;
        m_ptMouseDown = point;
    }
    else
    {
        // Create new object
        CDrawObject* pNewObj = new CDrawRectangle(point);
        pDoc->AddObject(pNewObj);
        m_pSelectedObject = pNewObj;
    }

    Invalidate();

    CScrollView::OnLButtonDown(nFlags, point);
}

void CDrawView::OnMouseMove(UINT nFlags, CPoint point)
{
    // Convert to logical coordinates
    CClientDC dc(this);
    OnPrepareDC(&dc);
    dc.DPtoLP(&point);

    // Update status bar
    CMainFrame* pFrame = (CMainFrame*)GetParentFrame();
    CString status;
    status.Format(_T("X: %d, Y: %d"), point.x, point.y);
    pFrame->SetMessageText(status);

    if (m_bDragging && m_pSelectedObject)
    {
        // Drag object
        CPoint offset = point - m_ptMouseDown;
        m_pSelectedObject->Move(offset);
        m_ptMouseDown = point;

        CDrawDoc* pDoc = GetDocument();
        pDoc->SetModifiedFlag();
        pDoc->UpdateAllViews(this);
    }

    CScrollView::OnMouseMove(nFlags, point);
}

void CDrawView::OnViewGrid()
{
    m_bShowGrid = !m_bShowGrid;
    Invalidate();
}

void CDrawView::OnUpdateViewGrid(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_bShowGrid);
}

void CDrawView::OnViewZoomIn()
{
    if (m_nZoomLevel < 400)
    {
        m_nZoomLevel += 10;
        // Implement zoom
        Invalidate();
    }
}

void CDrawView::OnViewZoomOut()
{
    if (m_nZoomLevel > 10)
    {
        m_nZoomLevel -= 10;
        // Implement zoom
        Invalidate();
    }
}

CDrawObject* CDrawView::HitTest(CPoint point)
{
    CDrawDoc* pDoc = GetDocument();

    // Test in reverse order (topmost first)
    for (int i = pDoc->GetObjectCount() - 1; i >= 0; i--)
    {
        CDrawObject* pObj = pDoc->GetObject(i);
        if (pObj->HitTest(point))
        {
            return pObj;
        }
    }

    return NULL;
}
```

---

## 11. Exercises

### Exercise 1: Custom View

Create a custom view that displays document data as a bar chart.

---

### Exercise 2: Grid View

Implement a grid view that displays data in a table format with:
- Column headers
- Sorting by column
- Row selection

---

### Exercise 3: Zoom Feature

Add zoom in/out to a view:
- Mouse wheel zoom
- Toolbar buttons
- Zoom percentage display

---

### Exercise 4: Minimap

Add a minimap (overview) window that shows the entire document with a viewport indicator.

---

### Exercise 5: Multiple Cursors

Implement different cursors based on:
- Hovering over objects
- Current tool selected
- Keyboard modifiers

---

## Key Takeaways

1. ✅ CView handles display and input
2. ✅ OnDraw() is the core rendering method
3. ✅ View preferences stay in CView
4. ✅ Document data stays in CDocument
5. ✅ Use OnUpdate() to refresh when data changes
6. ✅ Handle mouse/keyboard in view
7. ✅ Multiple views can show same document
8. ✅ Choose the right view base class
9. ✅ CScrollView for large content
10. ✅ Never modify document data in OnDraw()

---

## Summary

The CView class is your window into the document. Master drawing, input handling, and the view/document separation to build powerful MFC applications!

---

## Next Lesson Preview

In **Lesson 20**, we'll explore:
- Serialization basics
- Reading and writing files
- The CArchive class
- File formats and versioning

**Congratulations on completing Lesson 19!**
