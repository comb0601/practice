# Lesson 58: Custom Drawing in Views

**Duration**: 5 hours
**Difficulty**: Advanced

## Table of Contents
1. CView Drawing Architecture
2. OnDraw vs OnPaint
3. Invalidation and Updates
4. Optimizing Redraws
5. Clipping Regions
6. View Scrolling
7. Custom View Classes
8. Complete Examples

---

## 1. CView Drawing Architecture

### CView Drawing Methods:

```cpp
class CMyView : public CView
{
protected:
    // Override one of these:
    virtual void OnDraw(CDC* pDC);           // Most common
    virtual void OnPaint();                   // Advanced control
    virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
};
```

### When Each is Called:

```
WM_PAINT message
     ↓
OnPaint() - creates CPaintDC
     ↓
OnPrepareDC() - sets up DC
     ↓
OnDraw() - your drawing code
```

---

## 2. OnDraw vs OnPaint

### Use OnDraw (Recommended):

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CMyDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    // Draw document content
    // Works for both screen and printing
}
```

### Use OnPaint (Advanced):

```cpp
void CMyView::OnPaint()
{
    CPaintDC dc(this);  // Must create CPaintDC

    // Custom DC setup
    CMemDC memDC(&dc);  // Double buffering

    // Call OnDraw or draw directly
    OnDraw(&memDC.GetDC());
}
```

---

## 3. Invalidation and Updates

### Invalidate Methods:

```cpp
// Invalidate entire client area
Invalidate();
Invalidate(FALSE);  // Don't erase background

// Invalidate rectangle
CRect rect(10, 10, 100, 100);
InvalidateRect(&rect);
InvalidateRect(&rect, FALSE);

// Invalidate region
CRgn rgn;
rgn.CreateEllipticRgn(50, 50, 150, 150);
InvalidateRgn(&rgn);
```

### Update Immediately:

```cpp
// Queue invalidation (processed later)
Invalidate();

// Force immediate repaint
UpdateWindow();

// Or combine
Invalidate();
UpdateWindow();  // Repaints NOW
```

### Smart Invalidation:

```cpp
class CMyView : public CView
{
protected:
    CRect m_dirtyRect;
    BOOL m_bDirty;

public:
    void MarkDirty(const CRect& rect)
    {
        if (!m_bDirty)
        {
            m_dirtyRect = rect;
            m_bDirty = TRUE;
        }
        else
        {
            m_dirtyRect.UnionRect(&m_dirtyRect, &rect);
        }

        InvalidateRect(&m_dirtyRect, FALSE);
    }

    void OnDraw(CDC* pDC) override
    {
        CRect clipRect;
        pDC->GetClipBox(&clipRect);

        // Only draw what's visible
        DrawVisibleItems(pDC, clipRect);

        m_bDirty = FALSE;
    }
};
```

---

## 4. Optimizing Redraws

### Check Clip Box:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CRect clipBox;
    pDC->GetClipBox(&clipBox);

    CMyDoc* pDoc = GetDocument();

    // Only draw objects that intersect clip box
    for (auto& obj : pDoc->m_objects)
    {
        CRect objRect = obj->GetBounds();

        if (objRect.IntersectRect(&objRect, &clipBox))
        {
            obj->Draw(pDC);  // Visible, draw it
        }
    }
}
```

### Spatial Indexing:

```cpp
class CMyView : public CView
{
protected:
    struct QuadTreeNode
    {
        CRect bounds;
        CArray<CDrawObject*> objects;
        QuadTreeNode* children[4];
    };

    QuadTreeNode* m_quadTree;

    void OnDraw(CDC* pDC) override
    {
        CRect clipBox;
        pDC->GetClipBox(&clipBox);

        // Query quad tree for visible objects
        CArray<CDrawObject*> visible;
        QueryQuadTree(m_quadTree, clipBox, visible);

        // Draw only visible objects
        for (int i = 0; i < visible.GetCount(); i++)
        {
            visible[i]->Draw(pDC);
        }
    }
};
```

---

## 5. Clipping Regions

### Using Clipping Regions:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Save DC state
    int nSavedDC = pDC->SaveDC();

    // Create clipping region
    CRgn rgn;
    rgn.CreateRectRgn(50, 50, 350, 350);

    // Set clipping region
    pDC->SelectClipRgn(&rgn);

    // Everything draws only inside region
    for (int i = 0; i < 10; i++)
    {
        pDC->TextOut(i * 40, i * 40, _T("Clipped"));
    }

    // Restore (removes clipping)
    pDC->RestoreDC(nSavedDC);

    // Now draws everywhere
    pDC->TextOut(400, 400, _T("Not Clipped"));
}
```

### Complex Clipping:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Create complex region
    CRgn rgn1, rgn2, rgnCombined;

    rgn1.CreateEllipticRgn(50, 50, 250, 250);
    rgn2.CreateRectRgn(150, 150, 350, 350);

    // Combine regions (intersection)
    rgnCombined.CreateRectRgn(0, 0, 1, 1);
    rgnCombined.CombineRgn(&rgn1, &rgn2, RGN_AND);

    pDC->SelectClipRgn(&rgnCombined);

    // Draw (only visible in intersection)
    pDC->FillSolidRect(0, 0, 400, 400, RGB(255, 0, 0));

    pDC->SelectClipRgn(NULL);
}
```

---

## 6. View Scrolling

### Implementing Scrolling:

```cpp
class CScrollingView : public CScrollView
{
protected:
    virtual void OnDraw(CDC* pDC);
    virtual void OnInitialUpdate();

    DECLARE_MESSAGE_MAP()
};

void CScrollingView::OnInitialUpdate()
{
    CScrollView::OnInitialUpdate();

    // Set scroll sizes
    CSize sizeTotal;
    sizeTotal.cx = 2000;  // Total width
    sizeTotal.cy = 2000;  // Total height

    CSize sizePage;
    sizePage.cx = 200;    // Page scroll amount
    sizePage.cy = 200;

    CSize sizeLine;
    sizeLine.cx = 20;     // Line scroll amount
    sizeLine.cy = 20;

    SetScrollSizes(MM_TEXT, sizeTotal, sizePage, sizeLine);
}

void CScrollingView::OnDraw(CDC* pDC)
{
    // Current scroll position already applied
    // Draw at logical coordinates

    for (int x = 0; x < 2000; x += 100)
    {
        for (int y = 0; y < 2000; y += 100)
        {
            CRect rect(x, y, x + 90, y + 90);
            pDC->Rectangle(&rect);

            CString text;
            text.Format(_T("%d,%d"), x, y);
            pDC->DrawText(text, &rect,
                         DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }
}
```

### Manual Scrolling:

```cpp
class CMyView : public CView
{
protected:
    CPoint m_scrollPos;

    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

    DECLARE_MESSAGE_MAP()
};

void CMyView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    int oldPos = m_scrollPos.x;

    switch (nSBCode)
    {
    case SB_LINELEFT:
        m_scrollPos.x -= 20;
        break;

    case SB_LINERIGHT:
        m_scrollPos.x += 20;
        break;

    case SB_PAGELEFT:
        m_scrollPos.x -= 200;
        break;

    case SB_PAGERIGHT:
        m_scrollPos.x += 200;
        break;

    case SB_THUMBTRACK:
        m_scrollPos.x = nPos;
        break;
    }

    // Clamp to valid range
    m_scrollPos.x = max(0, min(m_scrollPos.x, 1000));

    // Update scrollbar
    SetScrollPos(SB_HORZ, m_scrollPos.x);

    // Scroll window
    int dx = oldPos - m_scrollPos.x;
    ScrollWindow(dx, 0);
}
```

---

## 7. Custom View Classes

### Custom View Base Class:

```cpp
class CCustomView : public CView
{
protected:
    CDC m_memDC;
    CBitmap m_bitmap;
    CSize m_bufferSize;
    BOOL m_bBufferValid;

public:
    CCustomView();

    virtual void InvalidateBuffer();
    virtual void EnsureBuffer(CDC* pDC);
    virtual void DrawToBuffer(CDC* pDC) = 0;  // Pure virtual

    void OnDraw(CDC* pDC) override;
    BOOL OnEraseBkgnd(CDC* pDC) override;

    DECLARE_MESSAGE_MAP()
};

CCustomView::CCustomView()
{
    m_bBufferValid = FALSE;
    m_bufferSize = CSize(0, 0);
}

void CCustomView::InvalidateBuffer()
{
    m_bBufferValid = FALSE;
    Invalidate(FALSE);
}

void CCustomView::EnsureBuffer(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    if (!m_bBufferValid ||
        rect.Width() != m_bufferSize.cx ||
        rect.Height() != m_bufferSize.cy)
    {
        // Recreate buffer
        if (m_memDC.GetSafeHdc())
        {
            m_memDC.SelectObject((CBitmap*)NULL);
            m_bitmap.DeleteObject();
            m_memDC.DeleteDC();
        }

        m_memDC.CreateCompatibleDC(pDC);
        m_bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
        m_memDC.SelectObject(&m_bitmap);

        m_bufferSize = rect.Size();

        // Draw to buffer
        DrawToBuffer(&m_memDC);

        m_bBufferValid = TRUE;
    }
}

void CCustomView::OnDraw(CDC* pDC)
{
    EnsureBuffer(pDC);

    // Copy buffer to screen
    pDC->BitBlt(0, 0, m_bufferSize.cx, m_bufferSize.cy,
                &m_memDC, 0, 0, SRCCOPY);
}

BOOL CCustomView::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;  // Prevent flicker
}
```

### Using Custom View:

```cpp
class CMyGraphView : public CCustomView
{
protected:
    CArray<CPoint> m_points;

public:
    void AddPoint(CPoint pt)
    {
        m_points.Add(pt);
        InvalidateBuffer();
    }

    void DrawToBuffer(CDC* pDC) override
    {
        CRect rect;
        GetClientRect(&rect);

        // Clear background
        pDC->FillSolidRect(&rect, RGB(255, 255, 255));

        // Draw grid
        CPen gridPen(PS_DOT, 1, RGB(200, 200, 200));
        CPen* pOldPen = pDC->SelectObject(&gridPen);

        for (int x = 0; x < rect.Width(); x += 50)
        {
            pDC->MoveTo(x, 0);
            pDC->LineTo(x, rect.Height());
        }

        for (int y = 0; y < rect.Height(); y += 50)
        {
            pDC->MoveTo(0, y);
            pDC->LineTo(rect.Width(), y);
        }

        // Draw points
        CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
        pDC->SelectObject(&pen);

        if (m_points.GetCount() > 0)
        {
            pDC->MoveTo(m_points[0]);

            for (int i = 1; i < m_points.GetCount(); i++)
            {
                pDC->LineTo(m_points[i]);
            }
        }

        pDC->SelectObject(pOldPen);
    }
};
```

---

## 8. Complete Examples

### Performance-Optimized View:

```cpp
class COptimizedView : public CView
{
protected:
    struct DrawObject
    {
        CRect bounds;
        COLORREF color;
        int type;
    };

    CArray<DrawObject> m_objects;
    CDC m_memDC;
    CBitmap m_bitmap;
    BOOL m_bBufferValid;
    CRect m_dirtyRect;

public:
    void AddObject(const DrawObject& obj)
    {
        m_objects.Add(obj);

        // Mark area dirty
        if (m_dirtyRect.IsRectEmpty())
            m_dirtyRect = obj.bounds;
        else
            m_dirtyRect.UnionRect(&m_dirtyRect, &obj.bounds);

        InvalidateRect(&m_dirtyRect, FALSE);
    }

    void OnDraw(CDC* pDC) override
    {
        CRect rect;
        GetClientRect(&rect);

        // Ensure buffer exists
        if (!m_memDC.GetSafeHdc())
        {
            m_memDC.CreateCompatibleDC(pDC);
            m_bitmap.CreateCompatibleBitmap(pDC,
                                           rect.Width(), rect.Height());
            m_memDC.SelectObject(&m_bitmap);
            m_bBufferValid = FALSE;
        }

        // Redraw only dirty area
        if (!m_bBufferValid || !m_dirtyRect.IsRectEmpty())
        {
            RedrawBuffer(m_dirtyRect);
            m_dirtyRect.SetRectEmpty();
            m_bBufferValid = TRUE;
        }

        // Copy to screen
        pDC->BitBlt(0, 0, rect.Width(), rect.Height(),
                    &m_memDC, 0, 0, SRCCOPY);
    }

    void RedrawBuffer(const CRect& dirtyRect)
    {
        // Clear dirty area
        m_memDC.FillSolidRect(&dirtyRect, RGB(255, 255, 255));

        // Draw objects that intersect dirty area
        for (int i = 0; i < m_objects.GetCount(); i++)
        {
            if (m_objects[i].bounds.IntersectRect(&m_objects[i].bounds,
                                                  &dirtyRect))
            {
                DrawObject(&m_memDC, m_objects[i]);
            }
        }
    }

    void DrawObject(CDC* pDC, const DrawObject& obj)
    {
        CBrush brush(obj.color);
        CBrush* pOldBrush = pDC->SelectObject(&brush);

        switch (obj.type)
        {
        case 0:  // Rectangle
            pDC->Rectangle(&obj.bounds);
            break;

        case 1:  // Ellipse
            pDC->Ellipse(&obj.bounds);
            break;
        }

        pDC->SelectObject(pOldBrush);
    }
};
```

---

## Key Takeaways

1. ✅ Override OnDraw for most cases
2. ✅ Use InvalidateRect for partial updates
3. ✅ Check clip box for optimization
4. ✅ Implement double buffering for smooth rendering
5. ✅ Use clipping regions for complex layouts
6. ✅ CScrollView for scrollable content
7. ✅ Create custom view base classes
8. ✅ Optimize with dirty rectangles

---

## Next Lesson Preview

In **Lesson 59: Mouse Input and Drawing**, we'll learn:
- Mouse event handling
- Interactive drawing
- Drag and drop
- Selection

**Congratulations on completing Lesson 58!**
