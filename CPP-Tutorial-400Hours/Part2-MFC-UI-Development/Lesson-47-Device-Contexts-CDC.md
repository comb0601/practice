# Lesson 47: Device Contexts (CDC)

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding Device Contexts
2. CDC Class Hierarchy
3. Types of Device Contexts
4. CDC Class Methods and Properties
5. Working with CClientDC
6. Working with CWindowDC
7. Working with CPaintDC
8. Memory Device Contexts (CMemDC)
9. DC Attributes and State
10. Advanced DC Techniques
11. Exercises

---

## 1. Understanding Device Contexts

A **Device Context (DC)** is the fundamental abstraction in GDI that represents a drawing surface. Think of it as a "canvas" with attached drawing tools and settings.

### What is a Device Context?

```
┌──────────────────────────────────┐
│     Device Context (DC)          │
│                                  │
│  ┌────────────────────────────┐ │
│  │   Current Drawing State    │ │
│  │   - Selected Pen           │ │
│  │   - Selected Brush         │ │
│  │   - Selected Font          │ │
│  │   - Current Position       │ │
│  │   - Mapping Mode           │ │
│  │   - Clipping Region        │ │
│  │   - Text Color             │ │
│  │   - Background Mode        │ │
│  └────────────────────────────┘ │
│                                  │
│  ┌────────────────────────────┐ │
│  │   Target Device            │ │
│  │   - Screen                 │ │
│  │   - Printer                │ │
│  │   - Memory Bitmap          │ │
│  │   - Metafile               │ │
│  └────────────────────────────┘ │
└──────────────────────────────────┘
```

### Why Device Contexts?

**Device contexts provide:**
1. **Abstraction**: Same code for screen, printer, or memory
2. **State Management**: Maintains drawing settings
3. **Resource Management**: Manages GDI objects safely
4. **Coordinate Mapping**: Transforms logical to device coordinates
5. **Clipping**: Restricts drawing to specific regions

### DC Lifecycle:

```cpp
// 1. Obtain DC
CDC* pDC = GetDC();

// 2. Set up drawing environment
CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
CPen* pOldPen = pDC->SelectObject(&pen);

// 3. Perform drawing operations
pDC->MoveTo(10, 10);
pDC->LineTo(100, 100);

// 4. Clean up
pDC->SelectObject(pOldPen);

// 5. Release DC
ReleaseDC(pDC);
```

---

## 2. CDC Class Hierarchy

MFC wraps the Windows HDC in the **CDC** class hierarchy:

```
CDC (Base class - generic device context)
├── CClientDC      (Client area only)
├── CWindowDC      (Entire window including frame)
├── CPaintDC       (For WM_PAINT messages)
├── CMetaFileDC    (Metafile recording)
└── Other specialized DCs
```

### CDC Class Overview:

```cpp
class CDC : public CObject
{
public:
    HDC m_hDC;          // Windows DC handle
    HDC m_hAttribDC;    // Attribute DC handle

    // Drawing operations
    virtual void MoveTo(int x, int y);
    virtual void LineTo(int x, int y);
    virtual void Rectangle(int x1, int y1, int x2, int y2);
    virtual void Ellipse(int x1, int y1, int x2, int y2);
    virtual void TextOut(int x, int y, const CString& str);

    // Object selection
    CGdiObject* SelectObject(CGdiObject* pObject);
    CPen* SelectObject(CPen* pPen);
    CBrush* SelectObject(CBrush* pBrush);
    CFont* SelectObject(CFont* pFont);

    // Attributes
    COLORREF SetTextColor(COLORREF crColor);
    COLORREF SetBkColor(COLORREF crColor);
    int SetBkMode(int nBkMode);

    // Mapping and coordinates
    int SetMapMode(int nMapMode);
    CPoint SetViewportOrg(int x, int y);
    CSize SetViewportExt(int cx, int cy);

    // Much more...
};
```

---

## 3. Types of Device Contexts

### 3.1 CClientDC - Client Area Drawing

**Purpose**: Drawing in the client area only (excludes title bar, borders)

**When to Use**:
- Drawing in response to mouse events
- Custom controls
- Dynamic drawing outside WM_PAINT

**Example:**
```cpp
void CMyView::OnLButtonDown(UINT nFlags, CPoint point)
{
    CClientDC dc(this);  // Automatic acquire/release

    CPen pen(PS_SOLID, 3, RGB(255, 0, 0));
    CPen* pOldPen = dc.SelectObject(&pen);

    dc.MoveTo(point);
    dc.LineTo(point.x + 50, point.y + 50);

    dc.SelectObject(pOldPen);
    // DC automatically released when dc goes out of scope
}
```

**Characteristics:**
- ✅ Constructor acquires DC
- ✅ Destructor releases DC
- ✅ RAII pattern (Resource Acquisition Is Initialization)
- ✅ Client area only
- ⚠️ Not persistent (erased on WM_PAINT)

---

### 3.2 CWindowDC - Entire Window

**Purpose**: Drawing on the entire window, including non-client area

**When to Use**:
- Custom window frames
- Title bar customization
- Border effects
- Window-wide overlays

**Example:**
```cpp
void CMyView::DrawCustomFrame()
{
    CWindowDC dc(this);  // Entire window

    CRect rect;
    GetWindowRect(&rect);
    rect.OffsetRect(-rect.left, -rect.top);  // Convert to window coords

    // Draw custom border
    CPen pen(PS_SOLID, 5, RGB(0, 0, 255));
    CPen* pOldPen = dc.SelectObject(&pen);
    dc.SelectStockObject(NULL_BRUSH);

    dc.Rectangle(&rect);

    dc.SelectObject(pOldPen);
}
```

**Characteristics:**
- ✅ Includes title bar, borders, scrollbars
- ✅ Uses window coordinates (not client coordinates)
- ⚠️ Rarely used (OS usually draws frame)
- ⚠️ Can interfere with standard window appearance

---

### 3.3 CPaintDC - WM_PAINT Handler

**Purpose**: Exclusive DC for WM_PAINT message handling

**When to Use**:
- **Only** in OnPaint() or OnDraw()
- When window needs repainting
- Persistent drawing

**Example:**
```cpp
void CMyView::OnPaint()
{
    CPaintDC dc(this);  // Calls BeginPaint/EndPaint

    // Get update region (area that needs repainting)
    CRect rectUpdate;
    dc.GetClipBox(&rectUpdate);

    // Draw only in update region for efficiency
    dc.FillSolidRect(&rectUpdate, RGB(255, 255, 255));

    // Your drawing code
    CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
    CPen* pOldPen = dc.SelectObject(&pen);

    dc.Rectangle(50, 50, 200, 200);

    dc.SelectObject(pOldPen);
    // EndPaint called automatically by destructor
}
```

**Critical Rules:**
- ⚠️ **MUST** use CPaintDC in WM_PAINT handler
- ⚠️ Never return from OnPaint without creating CPaintDC
- ⚠️ Constructor calls BeginPaint() (validates update region)
- ⚠️ Destructor calls EndPaint()

**What Happens Without CPaintDC:**
```cpp
void CMyView::OnPaint()
{
    // WRONG! No CPaintDC created
    CClientDC dc(this);
    dc.Rectangle(50, 50, 200, 200);
    // Update region never validated - infinite WM_PAINT loop!
}
```

---

### 3.4 CMetaFileDC - Recording Drawing Commands

**Purpose**: Records drawing operations to a metafile

```cpp
void CreateMetafile()
{
    CMetaFileDC dcMeta;
    dcMeta.Create();

    // Draw into metafile
    CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
    CPen* pOldPen = dcMeta.SelectObject(&pen);

    dcMeta.Rectangle(10, 10, 110, 110);
    dcMeta.Ellipse(20, 20, 100, 100);

    dcMeta.SelectObject(pOldPen);

    // Close and get handle
    HMETAFILE hmf = dcMeta.Close();

    // Play back later
    CClientDC dc(pView);
    dc.PlayMetaFile(hmf);

    DeleteMetaFile(hmf);
}
```

---

## 4. CDC Class Methods and Properties

### 4.1 Drawing Methods

#### Lines and Curves:
```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Move to position (doesn't draw)
    pDC->MoveTo(50, 50);

    // Draw line from current position
    pDC->LineTo(150, 50);

    // Draw multiple connected lines
    POINT points[] = {
        {200, 50}, {250, 100}, {200, 150}, {150, 100}
    };
    pDC->Polyline(points, 4);

    // Draw Bezier curve
    POINT bezier[] = {
        {300, 50}, {350, 50}, {350, 150}, {400, 150}
    };
    pDC->PolyBezier(bezier, 4);

    // Arc (part of ellipse)
    pDC->Arc(50, 200, 150, 300,   // Bounding rect
             100, 200,             // Start point
             150, 250);            // End point
}
```

#### Shapes:
```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Rectangle
    pDC->Rectangle(50, 50, 150, 150);

    // Rounded rectangle
    pDC->RoundRect(200, 50, 300, 150, 20, 20);  // 20x20 corner radius

    // Ellipse (use square rect for circle)
    pDC->Ellipse(350, 50, 450, 150);

    // Polygon (automatically closed)
    POINT poly[] = {
        {500, 100}, {550, 50}, {600, 100}, {575, 150}, {525, 150}
    };
    pDC->Polygon(poly, 5);

    // Chord (filled arc)
    pDC->Chord(50, 200, 150, 300, 100, 200, 150, 250);

    // Pie slice
    pDC->Pie(200, 200, 300, 300, 250, 200, 300, 250);
}
```

#### Filling:
```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CRect rect(50, 50, 150, 150);

    // Fill with solid color
    pDC->FillSolidRect(&rect, RGB(255, 0, 0));

    // Fill with brush
    CBrush brush(RGB(0, 255, 0));
    pDC->FillRect(&rect, &brush);

    // Flood fill (fill area with boundary)
    pDC->FloodFill(100, 100, RGB(0, 0, 0));  // Fill until black border

    // Extended flood fill
    pDC->ExtFloodFill(100, 100, RGB(255, 255, 255), FLOODFILLSURFACE);
}
```

### 4.2 Text Methods

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Simple text output
    pDC->TextOut(50, 50, _T("Hello World"));

    // Text with CString
    CString text = _T("MFC Text");
    pDC->TextOut(50, 80, text);

    // Extended text output with options
    pDC->ExtTextOut(50, 110, ETO_OPAQUE, nullptr,
                    _T("Extended Text"), 13, nullptr);

    // DrawText with formatting
    CRect rect(50, 150, 300, 250);
    pDC->DrawText(_T("Multi-line text\nwith word wrapping"),
                  &rect,
                  DT_WORDBREAK | DT_CENTER | DT_VCENTER);

    // Get text extent
    CSize size = pDC->GetTextExtent(_T("Measure Me"));
    CString info;
    info.Format(_T("Text size: %d x %d"), size.cx, size.cy);
    pDC->TextOut(50, 270, info);

    // Tab stops
    int tabStops[] = {100, 200, 300};
    pDC->TabbedTextOut(50, 300, _T("Col1\tCol2\tCol3"),
                       3, tabStops, 0);
}
```

### 4.3 Attribute Methods

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Text color
    COLORREF oldTextColor = pDC->SetTextColor(RGB(255, 0, 0));
    pDC->TextOut(50, 50, _T("Red Text"));

    // Background color
    COLORREF oldBkColor = pDC->SetBkColor(RGB(255, 255, 0));
    pDC->TextOut(50, 80, _T("Yellow Background"));

    // Background mode (transparent vs opaque)
    int oldBkMode = pDC->SetBkMode(TRANSPARENT);
    pDC->TextOut(50, 110, _T("Transparent Background"));

    // Text alignment
    UINT oldAlign = pDC->SetTextAlign(TA_CENTER | TA_BASELINE);
    pDC->TextOut(200, 150, _T("Centered"));

    // Restore
    pDC->SetTextColor(oldTextColor);
    pDC->SetBkColor(oldBkColor);
    pDC->SetBkMode(oldBkMode);
    pDC->SetTextAlign(oldAlign);
}
```

### 4.4 Coordinate Mapping

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Set mapping mode
    int oldMapMode = pDC->SetMapMode(MM_LOMETRIC);  // 0.1mm units

    // Set viewport origin (device coordinates)
    CPoint oldViewportOrg = pDC->SetViewportOrg(100, 100);

    // Set window origin (logical coordinates)
    CPoint oldWindowOrg = pDC->SetWindowOrg(-50, -50);

    // Now (0,0) in logical = (100,100) in device
    pDC->Rectangle(0, 0, 500, -500);  // Note: Y is negative in LOMETRIC

    // Convert between coordinate systems
    CPoint logical(100, 100);
    pDC->LPtoDP(&logical);  // Logical to Device
    // logical now contains device coordinates

    CPoint device(200, 200);
    pDC->DPtoLP(&device);  // Device to Logical
    // device now contains logical coordinates

    // Restore
    pDC->SetMapMode(oldMapMode);
    pDC->SetViewportOrg(oldViewportOrg);
    pDC->SetWindowOrg(oldWindowOrg);
}
```

---

## 5. Working with CClientDC

### Complete Example:

```cpp
// MyView.h
class CMyView : public CView
{
protected:
    CPoint m_lastPoint;
    BOOL m_bDrawing;

public:
    CMyView();

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

    DECLARE_MESSAGE_MAP()
};

// MyView.cpp
BEGIN_MESSAGE_MAP(CMyView, CView)
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

CMyView::CMyView()
{
    m_bDrawing = FALSE;
}

void CMyView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bDrawing = TRUE;
    m_lastPoint = point;

    // Draw starting point
    CClientDC dc(this);
    CPen pen(PS_SOLID, 5, RGB(255, 0, 0));
    CPen* pOldPen = dc.SelectObject(&pen);

    dc.Ellipse(point.x - 3, point.y - 3,
               point.x + 3, point.y + 3);

    dc.SelectObject(pOldPen);

    CView::OnLButtonDown(nFlags, point);
}

void CMyView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bDrawing)
    {
        CClientDC dc(this);
        CPen pen(PS_SOLID, 2, RGB(0, 0, 255));
        CPen* pOldPen = dc.SelectObject(&pen);

        dc.MoveTo(m_lastPoint);
        dc.LineTo(point);

        dc.SelectObject(pOldPen);

        m_lastPoint = point;
    }

    CView::OnMouseMove(nFlags, point);
}

void CMyView::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_bDrawing = FALSE;
    CView::OnLButtonUp(nFlags, point);
}
```

### Important Notes:

**Pros of CClientDC:**
- ✅ Simple to use (RAII)
- ✅ Automatic cleanup
- ✅ Works anywhere (not just OnPaint)

**Cons of CClientDC:**
- ⚠️ Not persistent (drawing lost on repaint)
- ⚠️ Must combine with data storage for persistence
- ⚠️ Slower than CPaintDC for repainting

---

## 6. Working with CWindowDC

### Custom Title Bar Drawing:

```cpp
void CMyDialog::OnNcPaint()
{
    // Default non-client painting
    CDialog::OnNcPaint();

    // Custom additions
    CWindowDC dc(this);

    CRect rectWindow;
    GetWindowRect(&rectWindow);
    rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);

    // Draw custom indicator in title bar
    CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
    CBrush brush(RGB(255, 200, 200));

    CPen* pOldPen = dc.SelectObject(&pen);
    CBrush* pOldBrush = dc.SelectObject(&brush);

    // Draw indicator in top-right of title bar
    dc.Ellipse(rectWindow.right - 30, 5,
               rectWindow.right - 10, 25);

    dc.SelectObject(pOldPen);
    dc.SelectObject(pOldBrush);
}
```

---

## 7. Working with CPaintDC

### Proper OnPaint Implementation:

```cpp
void CMyView::OnPaint()
{
    CPaintDC dc(this);  // MUST create this

    // Get the update region
    CRect rectUpdate;
    dc.GetClipBox(&rectUpdate);

    // Only redraw what's necessary
    if (rectUpdate.Width() > 0 && rectUpdate.Height() > 0)
    {
        OnDraw(&dc);
    }
}

void CMyView::OnDraw(CDC* pDC)
{
    CMyDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    // Get update region for optimization
    CRect rectClip;
    pDC->GetClipBox(&rectClip);

    // Draw only objects that intersect update region
    for (auto& obj : pDoc->m_objects)
    {
        CRect rectObj = obj->GetBounds();
        if (rectObj.IntersectRect(&rectObj, &rectClip))
        {
            obj->Draw(pDC);
        }
    }
}
```

### Why CPaintDC is Special:

```cpp
// What CPaintDC does internally:
CPaintDC::CPaintDC(CWnd* pWnd)
{
    m_hWnd = pWnd->m_hWnd;
    m_hDC = ::BeginPaint(m_hWnd, &m_ps);  // Validates region!
}

CPaintDC::~CPaintDC()
{
    ::EndPaint(m_hWnd, &m_ps);  // Must be called!
}

// If you don't create CPaintDC:
void BadOnPaint()
{
    // No BeginPaint called!
    // Update region never validated
    // Windows keeps sending WM_PAINT
    // Application hangs in infinite paint loop!
}
```

---

## 8. Memory Device Contexts (CMemDC)

Memory DCs are crucial for **double buffering** and **off-screen rendering**.

### Basic Memory DC Usage:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    // Create memory DC
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);

    // Create bitmap
    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

    // Select bitmap into memory DC
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    // Draw into memory DC (off-screen)
    memDC.FillSolidRect(&rect, RGB(255, 255, 255));

    CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
    CPen* pOldPen = memDC.SelectObject(&pen);

    for (int i = 0; i < 100; i++)
    {
        memDC.MoveTo(i * 10, 0);
        memDC.LineTo(i * 10, rect.Height());
    }

    memDC.SelectObject(pOldPen);

    // Copy from memory DC to screen (fast!)
    pDC->BitBlt(0, 0, rect.Width(), rect.Height(),
                &memDC, 0, 0, SRCCOPY);

    // Cleanup
    memDC.SelectObject(pOldBitmap);
}
```

### Double Buffering Example:

```cpp
class CMyView : public CView
{
protected:
    CDC m_memDC;
    CBitmap m_bitmap;
    CSize m_sizeBuffer;

public:
    void CreateBuffer(CDC* pDC, CSize size);
    void OnDraw(CDC* pDC) override;
};

void CMyView::CreateBuffer(CDC* pDC, CSize size)
{
    // Only recreate if size changed
    if (size != m_sizeBuffer)
    {
        if (m_memDC.GetSafeHdc())
        {
            CBitmap* pOldBitmap = m_memDC.SelectObject((CBitmap*)nullptr);
            if (pOldBitmap)
                pOldBitmap->DeleteObject();
            m_memDC.DeleteDC();
        }

        m_memDC.CreateCompatibleDC(pDC);
        m_bitmap.CreateCompatibleBitmap(pDC, size.cx, size.cy);
        m_memDC.SelectObject(&m_bitmap);

        m_sizeBuffer = size;
    }
}

void CMyView::OnDraw(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    CreateBuffer(pDC, rect.Size());

    // Draw to memory DC
    m_memDC.FillSolidRect(&rect, RGB(255, 255, 255));

    // ... complex drawing operations ...

    // Single BitBlt to screen (no flicker!)
    pDC->BitBlt(0, 0, rect.Width(), rect.Height(),
                &m_memDC, 0, 0, SRCCOPY);
}
```

---

## 9. DC Attributes and State

### Saving and Restoring DC State:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Save entire DC state
    int nSavedDC = pDC->SaveDC();

    // Make changes
    pDC->SetTextColor(RGB(255, 0, 0));
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetMapMode(MM_LOMETRIC);

    CPen pen(PS_SOLID, 5, RGB(0, 255, 0));
    pDC->SelectObject(&pen);

    // ... drawing operations ...

    // Restore everything at once!
    pDC->RestoreDC(nSavedDC);

    // All settings restored to previous state
}
```

### What SaveDC/RestoreDC Save:

- Selected pen, brush, font, bitmap
- Text color and background color
- Background mode
- Mapping mode
- Window/viewport origins and extents
- Clipping region
- Drawing mode (ROP2)
- Text alignment

---

## 10. Advanced DC Techniques

### 10.1 Clipping Regions

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Create circular clipping region
    CRgn rgnCircle;
    rgnCircle.CreateEllipticRgn(50, 50, 250, 250);

    // Set clipping region
    pDC->SelectClipRgn(&rgnCircle);

    // This rectangle will be clipped to circle
    CBrush brush(RGB(255, 0, 0));
    pDC->FillRect(CRect(0, 0, 300, 300), &brush);

    // Remove clipping
    pDC->SelectClipRgn(nullptr);

    // This draws normally
    pDC->Rectangle(300, 50, 500, 250);
}
```

### 10.2 Coordinate Transformations

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    int nSavedDC = pDC->SaveDC();

    // Center coordinate system
    CRect rect;
    GetClientRect(&rect);
    pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);

    // Now (0,0) is at center
    CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);

    // Draw axes
    pDC->MoveTo(-rect.Width() / 2, 0);
    pDC->LineTo(rect.Width() / 2, 0);
    pDC->MoveTo(0, -rect.Height() / 2);
    pDC->LineTo(0, rect.Height() / 2);

    // Draw in centered coordinates
    pDC->Ellipse(-50, -50, 50, 50);

    pDC->SelectObject(pOldPen);
    pDC->RestoreDC(nSavedDC);
}
```

### 10.3 Drawing Modes (ROP2)

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Normal drawing
    int oldROP = pDC->SetROP2(R2_COPYPEN);

    CPen pen(PS_SOLID, 10, RGB(255, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);

    pDC->MoveTo(50, 50);
    pDC->LineTo(150, 150);

    // XOR mode (draws/erases)
    pDC->SetROP2(R2_XORPEN);

    pDC->MoveTo(50, 50);
    pDC->LineTo(150, 150);  // Erases previous line!

    // NOT mode (inverts)
    pDC->SetROP2(R2_NOT);

    pDC->MoveTo(200, 50);
    pDC->LineTo(300, 150);  // Inverts pixels

    pDC->SelectObject(pOldPen);
    pDC->SetROP2(oldROP);
}
```

---

## 11. Exercises

### Exercise 1: Simple Drawing Tool

Create a simple drawing application using CClientDC:

```cpp
// MyView.h
class CMyView : public CView
{
protected:
    CArray<CPoint> m_points;
    BOOL m_bDrawing;

public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    virtual void OnDraw(CDC* pDC);

    DECLARE_MESSAGE_MAP()
};

// Implementation
void CMyView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bDrawing = TRUE;
    m_points.RemoveAll();
    m_points.Add(point);

    CView::OnLButtonDown(nFlags, point);
}

void CMyView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bDrawing)
    {
        CClientDC dc(this);

        CPen pen(PS_SOLID, 2, RGB(0, 0, 255));
        CPen* pOldPen = dc.SelectObject(&pen);

        if (m_points.GetSize() > 0)
        {
            CPoint lastPt = m_points[m_points.GetSize() - 1];
            dc.MoveTo(lastPt);
            dc.LineTo(point);
        }

        dc.SelectObject(pOldPen);

        m_points.Add(point);
    }

    CView::OnMouseMove(nFlags, point);
}

void CMyView::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_bDrawing = FALSE;
    CView::OnLButtonUp(nFlags, point);
}

void CMyView::OnDraw(CDC* pDC)
{
    if (m_points.GetSize() > 1)
    {
        CPen pen(PS_SOLID, 2, RGB(0, 0, 255));
        CPen* pOldPen = pDC->SelectObject(&pen);

        for (int i = 1; i < m_points.GetSize(); i++)
        {
            pDC->MoveTo(m_points[i - 1]);
            pDC->LineTo(m_points[i]);
        }

        pDC->SelectObject(pOldPen);
    }
}
```

---

### Exercise 2: DC State Management

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Demonstrate SaveDC/RestoreDC
    DrawWithoutSave(pDC, 50, 50);
    DrawWithSave(pDC, 250, 50);
}

void CMyView::DrawWithoutSave(CDC* pDC, int x, int y)
{
    pDC->SetTextColor(RGB(255, 0, 0));
    pDC->TextOut(x, y, _T("Red"));

    pDC->SetTextColor(RGB(0, 255, 0));
    pDC->TextOut(x, y + 20, _T("Green"));

    // Color still green after this function
}

void CMyView::DrawWithSave(CDC* pDC, int x, int y)
{
    int nSaved = pDC->SaveDC();

    pDC->SetTextColor(RGB(255, 0, 0));
    pDC->TextOut(x, y, _T("Red"));

    pDC->SetTextColor(RGB(0, 255, 0));
    pDC->TextOut(x, y + 20, _T("Green"));

    pDC->RestoreDC(nSaved);
    // Color restored to original
}
```

---

### Exercise 3: Memory DC Practice

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    // Create memory DC
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);

    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    // Draw gradient background
    for (int y = 0; y < rect.Height(); y++)
    {
        int color = (y * 255) / rect.Height();
        CBrush brush(RGB(color, color, 255));
        CRect line(0, y, rect.Width(), y + 1);
        memDC.FillRect(&line, &brush);
    }

    // Draw some shapes
    CPen pen(PS_SOLID, 3, RGB(255, 255, 255));
    CPen* pOldPen = memDC.SelectObject(&pen);
    memDC.SelectStockObject(NULL_BRUSH);

    memDC.Ellipse(50, 50, 150, 150);
    memDC.Rectangle(200, 50, 300, 150);

    memDC.SelectObject(pOldPen);

    // Copy to screen
    pDC->BitBlt(0, 0, rect.Width(), rect.Height(),
                &memDC, 0, 0, SRCCOPY);

    memDC.SelectObject(pOldBitmap);
}
```

---

## Key Takeaways

1. ✅ CDC is the MFC wrapper for Windows HDC
2. ✅ Use CClientDC for client area drawing outside OnPaint
3. ✅ Always use CPaintDC in OnPaint handlers
4. ✅ CWindowDC includes non-client area
5. ✅ Memory DCs enable double buffering
6. ✅ SaveDC/RestoreDC manage complex state changes
7. ✅ Clipping regions restrict drawing areas
8. ✅ All DCs use RAII for automatic cleanup

---

## Common Mistakes

### ❌ Not Using CPaintDC in OnPaint:
```cpp
void CMyView::OnPaint()
{
    CClientDC dc(this);  // WRONG! Infinite paint loop
    // ...
}
```

### ❌ Forgetting to Select Old Bitmap:
```cpp
void Bad()
{
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(pDC, 100, 100);
    memDC.SelectObject(&bitmap);
    // bitmap destroyed while selected - CRASH!
}
```

---

## Summary Checklist

Before moving to Lesson 48, ensure you can:
- [ ] Explain what a Device Context is
- [ ] Use CClientDC, CWindowDC, and CPaintDC appropriately
- [ ] Create and use memory DCs
- [ ] Save and restore DC state
- [ ] Work with clipping regions
- [ ] Understand coordinate transformations
- [ ] Implement double buffering

---

## Next Lesson Preview

In **Lesson 48: Pens and Brushes**, we'll learn:
- Creating and customizing pens
- Pen styles, widths, and colors
- Brush types (solid, hatch, pattern)
- Stock pens and brushes
- Custom brush patterns

**Congratulations on completing Lesson 47!**
