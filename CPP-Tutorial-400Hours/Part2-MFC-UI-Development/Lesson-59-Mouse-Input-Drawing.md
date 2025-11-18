# Lesson 59: Mouse Input and Drawing

**Duration**: 5 hours
**Difficulty**: Advanced

## Table of Contents
1. Mouse Event Handling
2. Mouse Messages
3. Drawing with Mouse
4. Drag and Drop Drawing
5. Selection and Manipulation
6. Hit Testing
7. Rubber Banding
8. Complete Examples

---

## 1. Mouse Event Handling

### Mouse Message Handlers:

```cpp
class CMyView : public CView
{
protected:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMyView, CView)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_RBUTTONDOWN()
    ON_WM_MBUTTONDOWN()
    ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()
```

### Mouse State Flags:

```cpp
void CMyView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (nFlags & MK_LBUTTON)    // Left button down
    {
    }

    if (nFlags & MK_RBUTTON)    // Right button down
    {
    }

    if (nFlags & MK_MBUTTON)    // Middle button down
    {
    }

    if (nFlags & MK_CONTROL)    // Ctrl key pressed
    {
    }

    if (nFlags & MK_SHIFT)      // Shift key pressed
    {
    }
}
```

---

## 2. Mouse Messages

### Click vs Double-Click:

```cpp
class CMyView : public CView
{
protected:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMyView, CView)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()  // Must enable CS_DBLCLKS style
END_MESSAGE_MAP()

void CMyView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // Single click
    AfxMessageBox(_T("Single Click"));
}

void CMyView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // Double click
    AfxMessageBox(_T("Double Click"));
}
```

### Mouse Capture:

```cpp
void CMyView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // Capture mouse (receive all mouse messages)
    SetCapture();

    m_bDrawing = TRUE;
    m_startPoint = point;
}

void CMyView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bDrawing)
    {
        // Draw even if mouse outside window
    }
}

void CMyView::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bDrawing)
    {
        // Release mouse capture
        ReleaseCapture();
        m_bDrawing = FALSE;
    }
}
```

---

## 3. Drawing with Mouse

### Freehand Drawing:

```cpp
class CMyView : public CView
{
protected:
    BOOL m_bDrawing;
    CPoint m_lastPoint;
    CArray<CPoint> m_currentStroke;

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

    DECLARE_MESSAGE_MAP()
};

void CMyView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bDrawing = TRUE;
    m_lastPoint = point;
    m_currentStroke.RemoveAll();
    m_currentStroke.Add(point);

    SetCapture();
}

void CMyView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bDrawing)
    {
        CClientDC dc(this);

        CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
        CPen* pOldPen = dc.SelectObject(&pen);

        dc.MoveTo(m_lastPoint);
        dc.LineTo(point);

        dc.SelectObject(pOldPen);

        m_lastPoint = point;
        m_currentStroke.Add(point);
    }
}

void CMyView::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bDrawing)
    {
        ReleaseCapture();
        m_bDrawing = FALSE;

        // Save stroke to document
        CMyDoc* pDoc = GetDocument();
        pDoc->AddStroke(m_currentStroke);
    }
}
```

---

## 4. Drag and Drop Drawing

### Drawing Shapes by Dragging:

```cpp
class CMyView : public CView
{
protected:
    enum ShapeType { RECTANGLE, ELLIPSE, LINE };

    ShapeType m_currentTool;
    BOOL m_bDrawing;
    CPoint m_startPoint;
    CPoint m_endPoint;

    void DrawRubberBand(CDC* pDC);

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

    DECLARE_MESSAGE_MAP()
};

void CMyView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bDrawing = TRUE;
    m_startPoint = point;
    m_endPoint = point;

    SetCapture();
}

void CMyView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bDrawing)
    {
        CClientDC dc(this);

        // Erase old rubber band
        DrawRubberBand(&dc);

        // Update end point
        m_endPoint = point;

        // Draw new rubber band
        DrawRubberBand(&dc);
    }
}

void CMyView::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bDrawing)
    {
        ReleaseCapture();

        m_endPoint = point;

        // Create permanent shape
        CMyDoc* pDoc = GetDocument();

        CRect rect(m_startPoint, m_endPoint);
        rect.NormalizeRect();

        switch (m_currentTool)
        {
        case RECTANGLE:
            pDoc->AddRectangle(rect);
            break;

        case ELLIPSE:
            pDoc->AddEllipse(rect);
            break;

        case LINE:
            pDoc->AddLine(m_startPoint, m_endPoint);
            break;
        }

        m_bDrawing = FALSE;
        Invalidate();
    }
}

void CMyView::DrawRubberBand(CDC* pDC)
{
    int oldROP = pDC->SetROP2(R2_NOT);  // XOR mode

    CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->SelectStockObject(NULL_BRUSH);

    CRect rect(m_startPoint, m_endPoint);
    rect.NormalizeRect();

    switch (m_currentTool)
    {
    case RECTANGLE:
        pDC->Rectangle(&rect);
        break;

    case ELLIPSE:
        pDC->Ellipse(&rect);
        break;

    case LINE:
        pDC->MoveTo(m_startPoint);
        pDC->LineTo(m_endPoint);
        break;
    }

    pDC->SelectObject(pOldPen);
    pDC->SetROP2(oldROP);
}
```

---

## 5. Selection and Manipulation

### Hit Testing:

```cpp
class CDrawObject
{
public:
    virtual BOOL HitTest(CPoint point) = 0;
    virtual CRect GetBounds() = 0;
    virtual void Draw(CDC* pDC) = 0;
};

class CRectangleObject : public CDrawObject
{
protected:
    CRect m_rect;

public:
    BOOL HitTest(CPoint point) override
    {
        return m_rect.PtInRect(point);
    }

    CRect GetBounds() override
    {
        return m_rect;
    }

    void Draw(CDC* pDC) override
    {
        pDC->Rectangle(&m_rect);
    }
};

class CMyView : public CView
{
protected:
    CDrawObject* m_selectedObject;

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

    CDrawObject* FindObjectAt(CPoint point);
};

void CMyView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // Deselect current
    m_selectedObject = NULL;

    // Find object at click point
    CMyDoc* pDoc = GetDocument();

    for (int i = pDoc->GetObjectCount() - 1; i >= 0; i--)
    {
        CDrawObject* pObj = pDoc->GetObject(i);

        if (pObj->HitTest(point))
        {
            m_selectedObject = pObj;
            break;
        }
    }

    if (m_selectedObject)
    {
        // Object selected
        Invalidate();
    }
}
```

### Drag to Move:

```cpp
class CMyView : public CView
{
protected:
    CDrawObject* m_dragObject;
    CPoint m_dragStart;
    CPoint m_dragOffset;

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

void CMyView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_dragObject = FindObjectAt(point);

    if (m_dragObject)
    {
        m_dragStart = point;

        CRect bounds = m_dragObject->GetBounds();
        m_dragOffset = point - bounds.TopLeft();

        SetCapture();
    }
}

void CMyView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_dragObject && (nFlags & MK_LBUTTON))
    {
        // Calculate new position
        CPoint newPos = point - m_dragOffset;

        // Move object
        m_dragObject->MoveTo(newPos);

        // Redraw
        Invalidate(FALSE);
    }
}

void CMyView::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_dragObject)
    {
        ReleaseCapture();
        m_dragObject = NULL;

        // Update document
        GetDocument()->SetModifiedFlag();
    }
}
```

---

## 6. Hit Testing

### Precise Hit Testing:

```cpp
class CEllipseObject : public CDrawObject
{
protected:
    CRect m_rect;

public:
    BOOL HitTest(CPoint point) override
    {
        // Check bounding box first (fast)
        if (!m_rect.PtInRect(point))
            return FALSE;

        // Check if inside ellipse (precise)
        int centerX = (m_rect.left + m_rect.right) / 2;
        int centerY = (m_rect.top + m_rect.bottom) / 2;

        int radiusX = m_rect.Width() / 2;
        int radiusY = m_rect.Height() / 2;

        double dx = (point.x - centerX) / (double)radiusX;
        double dy = (point.y - centerY) / (double)radiusY;

        return (dx * dx + dy * dy) <= 1.0;
    }
};
```

### Selection Handles:

```cpp
class CDrawObject
{
protected:
    enum HandlePos
    {
        HANDLE_TOPLEFT, HANDLE_TOP, HANDLE_TOPRIGHT,
        HANDLE_LEFT, HANDLE_RIGHT,
        HANDLE_BOTTOMLEFT, HANDLE_BOTTOM, HANDLE_BOTTOMRIGHT
    };

public:
    CRect GetHandleRect(HandlePos pos)
    {
        CRect bounds = GetBounds();
        CRect handle;

        int size = 6;  // Handle size

        switch (pos)
        {
        case HANDLE_TOPLEFT:
            handle.SetRect(bounds.left - size, bounds.top - size,
                          bounds.left + size, bounds.top + size);
            break;

        case HANDLE_TOP:
            handle.SetRect((bounds.left + bounds.right) / 2 - size,
                          bounds.top - size,
                          (bounds.left + bounds.right) / 2 + size,
                          bounds.top + size);
            break;

        // ... other handles ...
        }

        return handle;
    }

    HandlePos HitTestHandle(CPoint point)
    {
        for (int i = 0; i < 8; i++)
        {
            CRect handle = GetHandleRect((HandlePos)i);

            if (handle.PtInRect(point))
                return (HandlePos)i;
        }

        return -1;  // No handle hit
    }

    void DrawHandles(CDC* pDC)
    {
        CBrush brush(RGB(0, 0, 255));
        CBrush* pOldBrush = pDC->SelectObject(&brush);

        for (int i = 0; i < 8; i++)
        {
            CRect handle = GetHandleRect((HandlePos)i);
            pDC->Rectangle(&handle);
        }

        pDC->SelectObject(pOldBrush);
    }
};
```

---

## 7. Rubber Banding

### Rubber Band Rectangle:

```cpp
class CMyView : public CView
{
protected:
    BOOL m_bSelecting;
    CPoint m_selectStart;
    CPoint m_selectEnd;

    void DrawSelectionRect(CDC* pDC, CRect rect);

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

void CMyView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bSelecting = TRUE;
    m_selectStart = point;
    m_selectEnd = point;

    SetCapture();
}

void CMyView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bSelecting)
    {
        CClientDC dc(this);

        // Erase old selection
        CRect oldRect(m_selectStart, m_selectEnd);
        oldRect.NormalizeRect();
        DrawSelectionRect(&dc, oldRect);

        // Update and draw new selection
        m_selectEnd = point;

        CRect newRect(m_selectStart, m_selectEnd);
        newRect.NormalizeRect();
        DrawSelectionRect(&dc, newRect);
    }
}

void CMyView::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bSelecting)
    {
        ReleaseCapture();
        m_bSelecting = FALSE;

        // Erase rubber band
        CClientDC dc(this);
        CRect rect(m_selectStart, m_selectEnd);
        rect.NormalizeRect();
        DrawSelectionRect(&dc, rect);

        // Select objects in rectangle
        SelectObjectsInRect(rect);

        Invalidate();
    }
}

void CMyView::DrawSelectionRect(CDC* pDC, CRect rect)
{
    int oldROP = pDC->SetROP2(R2_NOT);

    CPen pen(PS_DOT, 1, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->SelectStockObject(NULL_BRUSH);

    pDC->Rectangle(&rect);

    pDC->SelectObject(pOldPen);
    pDC->SetROP2(oldROP);
}
```

---

## 8. Complete Examples

### Full Drawing Application:

```cpp
// MyView.h
class CMyView : public CView
{
protected:
    enum Tool { TOOL_SELECT, TOOL_RECT, TOOL_ELLIPSE, TOOL_LINE };

    Tool m_currentTool;
    BOOL m_bDrawing;
    CPoint m_startPoint;
    CPoint m_endPoint;
    CDrawObject* m_selectedObject;

public:
    void SetTool(Tool tool) { m_currentTool = tool; }

    virtual void OnDraw(CDC* pDC);

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

    DECLARE_MESSAGE_MAP()
};

// MyView.cpp
void CMyView::OnDraw(CDC* pDC)
{
    CMyDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    // Draw all objects
    for (int i = 0; i < pDoc->GetObjectCount(); i++)
    {
        CDrawObject* pObj = pDoc->GetObject(i);
        pObj->Draw(pDC);

        // Draw selection handles
        if (pObj == m_selectedObject)
        {
            pObj->DrawHandles(pDC);
        }
    }
}

void CMyView::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (m_currentTool == TOOL_SELECT)
    {
        // Selection mode
        m_selectedObject = FindObjectAt(point);
        Invalidate();
    }
    else
    {
        // Drawing mode
        m_bDrawing = TRUE;
        m_startPoint = point;
        m_endPoint = point;
        SetCapture();
    }
}

void CMyView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bDrawing)
    {
        CClientDC dc(this);

        // Draw rubber band
        DrawRubberBand(&dc);
        m_endPoint = point;
        DrawRubberBand(&dc);
    }
}

void CMyView::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bDrawing)
    {
        ReleaseCapture();
        m_bDrawing = FALSE;

        // Create object
        CMyDoc* pDoc = GetDocument();
        CRect rect(m_startPoint, m_endPoint);
        rect.NormalizeRect();

        switch (m_currentTool)
        {
        case TOOL_RECT:
            pDoc->AddRectangle(rect);
            break;

        case TOOL_ELLIPSE:
            pDoc->AddEllipse(rect);
            break;

        case TOOL_LINE:
            pDoc->AddLine(m_startPoint, m_endPoint);
            break;
        }

        pDoc->SetModifiedFlag();
        Invalidate();
    }
}

BOOL CMyView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (nHitTest == HTCLIENT)
    {
        switch (m_currentTool)
        {
        case TOOL_SELECT:
            SetCursor(LoadCursor(NULL, IDC_ARROW));
            return TRUE;

        case TOOL_RECT:
        case TOOL_ELLIPSE:
        case TOOL_LINE:
            SetCursor(LoadCursor(NULL, IDC_CROSS));
            return TRUE;
        }
    }

    return CView::OnSetCursor(pWnd, nHitTest, message);
}
```

---

## Key Takeaways

1. ✅ Handle mouse messages for interactivity
2. ✅ Use SetCapture for drag operations
3. ✅ Implement hit testing for selection
4. ✅ Rubber banding with XOR mode
5. ✅ Selection handles for manipulation
6. ✅ Different tools/modes
7. ✅ Custom cursors for feedback
8. ✅ Update document on modifications

---

## Next Lesson Preview

In **Lesson 60**, we'll create a complete drawing application integrating all concepts!

**Congratulations on completing Lesson 59!**
