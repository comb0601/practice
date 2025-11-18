# Lesson 60: Complete Drawing Application Project

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Project Overview
2. Application Architecture
3. Document Class Implementation
4. Draw Object Classes
5. View Class Implementation
6. Toolbar and Menus
7. File Save/Load
8. Complete Source Code
9. Building and Testing
10. Enhancements

---

## 1. Project Overview

### DrawPro - Professional Drawing Application

**Features:**
- Multiple shape tools (Rectangle, Ellipse, Line)
- Freehand drawing
- Color selection
- Line width selection
- Object selection and manipulation
- Undo/Redo functionality
- File save/load (custom format)
- Printing and print preview
- Copy/Paste
- Properties dialog

---

## 2. Application Architecture

### Class Diagram:

```
CDrawProApp (Application)
     ↓
CMainFrame (Main Window)
     ↓
CDrawProDoc (Document)
     ↓  manages
CDrawObject (Base Class)
   ├── CRectObject
   ├── CEllipseObject
   ├── CLineObject
   └── CFreehandObject
     ↓
CDrawProView (View)
```

---

## 3. Document Class Implementation

### DrawProDoc.h:

```cpp
#pragma once
#include <afxtempl.h>

class CDrawObject;

class CDrawProDoc : public CDocument
{
protected:
    DECLARE_DYNCREATE(CDrawProDoc)

public:
    CDrawProDoc();
    virtual ~CDrawProDoc();

    // Object management
    void AddObject(CDrawObject* pObj);
    void RemoveObject(CDrawObject* pObj);
    void DeleteObject(CDrawObject* pObj);
    int GetObjectCount() const;
    CDrawObject* GetObject(int index) const;

    // Selection
    void SelectObject(CDrawObject* pObj);
    CDrawObject* GetSelectedObject() const;

    // Undo/Redo
    void PushUndo();
    BOOL CanUndo() const;
    BOOL CanRedo() const;
    void Undo();
    void Redo();

    // Serialization
    virtual void Serialize(CArchive& ar);

protected:
    CTypedPtrArray<CObArray, CDrawObject*> m_objects;
    CDrawObject* m_pSelectedObject;

    // Undo/Redo stacks
    CList<CTypedPtrArray<CObArray, CDrawObject*>*> m_undoStack;
    CList<CTypedPtrArray<CObArray, CDrawObject*>*> m_redoStack;

    DECLARE_MESSAGE_MAP()
};
```

### DrawProDoc.cpp:

```cpp
#include "pch.h"
#include "DrawPro.h"
#include "DrawProDoc.h"
#include "DrawObject.h"

IMPLEMENT_DYNCREATE(CDrawProDoc, CDocument)

BEGIN_MESSAGE_MAP(CDrawProDoc, CDocument)
END_MESSAGE_MAP()

CDrawProDoc::CDrawProDoc()
{
    m_pSelectedObject = NULL;
}

CDrawProDoc::~CDrawProDoc()
{
    // Delete all objects
    for (int i = 0; i < m_objects.GetCount(); i++)
    {
        delete m_objects[i];
    }
    m_objects.RemoveAll();

    // Clear undo/redo stacks
    // ... cleanup code ...
}

void CDrawProDoc::AddObject(CDrawObject* pObj)
{
    m_objects.Add(pObj);
    PushUndo();
    SetModifiedFlag();
}

void CDrawProDoc::RemoveObject(CDrawObject* pObj)
{
    for (int i = 0; i < m_objects.GetCount(); i++)
    {
        if (m_objects[i] == pObj)
        {
            m_objects.RemoveAt(i);
            PushUndo();
            SetModifiedFlag();
            break;
        }
    }
}

void CDrawProDoc::DeleteObject(CDrawObject* pObj)
{
    RemoveObject(pObj);
    delete pObj;
}

int CDrawProDoc::GetObjectCount() const
{
    return (int)m_objects.GetCount();
}

CDrawObject* CDrawProDoc::GetObject(int index) const
{
    if (index >= 0 && index < m_objects.GetCount())
        return m_objects[index];

    return NULL;
}

void CDrawProDoc::SelectObject(CDrawObject* pObj)
{
    m_pSelectedObject = pObj;
}

CDrawObject* CDrawProDoc::GetSelectedObject() const
{
    return m_pSelectedObject;
}

void CDrawProDoc::PushUndo()
{
    // Save current state to undo stack
    // Clear redo stack
    // Implementation details...
}

void CDrawProDoc::Serialize(CArchive& ar)
{
    CDocument::Serialize(ar);

    if (ar.IsStoring())
    {
        // Save object count
        ar << m_objects.GetCount();

        // Save each object
        for (int i = 0; i < m_objects.GetCount(); i++)
        {
            m_objects[i]->Serialize(ar);
        }
    }
    else
    {
        // Load object count
        int count;
        ar >> count;

        // Load each object
        for (int i = 0; i < count; i++)
        {
            CDrawObject* pObj = CDrawObject::CreateFromArchive(ar);
            if (pObj)
                m_objects.Add(pObj);
        }
    }
}
```

---

## 4. Draw Object Classes

### DrawObject.h:

```cpp
#pragma once

enum DrawObjectType
{
    OBJ_RECTANGLE,
    OBJ_ELLIPSE,
    OBJ_LINE,
    OBJ_FREEHAND
};

class CDrawObject : public CObject
{
    DECLARE_SERIAL(CDrawObject)

protected:
    CRect m_bounds;
    COLORREF m_color;
    int m_penWidth;
    BOOL m_bSelected;

public:
    CDrawObject();
    virtual ~CDrawObject();

    // Pure virtual methods
    virtual void Draw(CDC* pDC) = 0;
    virtual BOOL HitTest(CPoint point) = 0;
    virtual DrawObjectType GetType() = 0;

    // Properties
    void SetColor(COLORREF color) { m_color = color; }
    COLORREF GetColor() const { return m_color; }

    void SetPenWidth(int width) { m_penWidth = width; }
    int GetPenWidth() const { return m_penWidth; }

    void SetSelected(BOOL bSel) { m_bSelected = bSel; }
    BOOL IsSelected() const { return m_bSelected; }

    CRect GetBounds() const { return m_bounds; }
    void SetBounds(const CRect& rect) { m_bounds = rect; }

    // Selection handles
    void DrawHandles(CDC* pDC);
    int HitTestHandle(CPoint point);
    void MoveHandle(int handle, CPoint point);

    // Serialization
    virtual void Serialize(CArchive& ar);
    static CDrawObject* CreateFromArchive(CArchive& ar);
};

class CRectObject : public CDrawObject
{
    DECLARE_SERIAL(CRectObject)

public:
    CRectObject();
    CRectObject(const CRect& rect);

    void Draw(CDC* pDC) override;
    BOOL HitTest(CPoint point) override;
    DrawObjectType GetType() override { return OBJ_RECTANGLE; }

    void Serialize(CArchive& ar) override;
};

class CEllipseObject : public CDrawObject
{
    DECLARE_SERIAL(CEllipseObject)

public:
    CEllipseObject();
    CEllipseObject(const CRect& rect);

    void Draw(CDC* pDC) override;
    BOOL HitTest(CPoint point) override;
    DrawObjectType GetType() override { return OBJ_ELLIPSE; }

    void Serialize(CArchive& ar) override;
};

class CLineObject : public CDrawObject
{
    DECLARE_SERIAL(CLineObject)

protected:
    CPoint m_start;
    CPoint m_end;

public:
    CLineObject();
    CLineObject(CPoint start, CPoint end);

    void Draw(CDC* pDC) override;
    BOOL HitTest(CPoint point) override;
    DrawObjectType GetType() override { return OBJ_LINE; }

    void Serialize(CArchive& ar) override;
};

class CFreehandObject : public CDrawObject
{
    DECLARE_SERIAL(CFreehandObject)

protected:
    CArray<CPoint> m_points;

public:
    CFreehandObject();

    void AddPoint(CPoint pt);

    void Draw(CDC* pDC) override;
    BOOL HitTest(CPoint point) override;
    DrawObjectType GetType() override { return OBJ_FREEHAND; }

    void Serialize(CArchive& ar) override;
};
```

### DrawObject.cpp:

```cpp
#include "pch.h"
#include "DrawObject.h"

IMPLEMENT_SERIAL(CDrawObject, CObject, 1)
IMPLEMENT_SERIAL(CRectObject, CDrawObject, 1)
IMPLEMENT_SERIAL(CEllipseObject, CDrawObject, 1)
IMPLEMENT_SERIAL(CLineObject, CDrawObject, 1)
IMPLEMENT_SERIAL(CFreehandObject, CDrawObject, 1)

// CDrawObject implementation
CDrawObject::CDrawObject()
{
    m_color = RGB(0, 0, 0);
    m_penWidth = 1;
    m_bSelected = FALSE;
}

CDrawObject::~CDrawObject()
{
}

void CDrawObject::DrawHandles(CDC* pDC)
{
    if (!m_bSelected)
        return;

    CBrush brush(RGB(0, 0, 255));
    CBrush* pOldBrush = pDC->SelectObject(&brush);
    CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);

    int handles[] = {
        m_bounds.left, m_bounds.top,
        (m_bounds.left + m_bounds.right) / 2, m_bounds.top,
        m_bounds.right, m_bounds.top,
        m_bounds.left, (m_bounds.top + m_bounds.bottom) / 2,
        m_bounds.right, (m_bounds.top + m_bounds.bottom) / 2,
        m_bounds.left, m_bounds.bottom,
        (m_bounds.left + m_bounds.right) / 2, m_bounds.bottom,
        m_bounds.right, m_bounds.bottom
    };

    for (int i = 0; i < 8; i++)
    {
        int x = handles[i * 2];
        int y = handles[i * 2 + 1];

        pDC->Rectangle(x - 3, y - 3, x + 3, y + 3);
    }

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}

void CDrawObject::Serialize(CArchive& ar)
{
    CObject::Serialize(ar);

    if (ar.IsStoring())
    {
        ar << m_bounds;
        ar << m_color;
        ar << m_penWidth;
    }
    else
    {
        ar >> m_bounds;
        ar >> m_color;
        ar >> m_penWidth;
    }
}

CDrawObject* CDrawObject::CreateFromArchive(CArchive& ar)
{
    int type;
    ar >> type;

    CDrawObject* pObj = NULL;

    switch (type)
    {
    case OBJ_RECTANGLE:
        pObj = new CRectObject();
        break;

    case OBJ_ELLIPSE:
        pObj = new CEllipseObject();
        break;

    case OBJ_LINE:
        pObj = new CLineObject();
        break;

    case OBJ_FREEHAND:
        pObj = new CFreehandObject();
        break;
    }

    if (pObj)
        pObj->Serialize(ar);

    return pObj;
}

// CRectObject implementation
CRectObject::CRectObject()
{
}

CRectObject::CRectObject(const CRect& rect)
{
    m_bounds = rect;
    m_bounds.NormalizeRect();
}

void CRectObject::Draw(CDC* pDC)
{
    CPen pen(PS_SOLID, m_penWidth, m_color);
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->SelectStockObject(NULL_BRUSH);

    pDC->Rectangle(&m_bounds);

    pDC->SelectObject(pOldPen);

    DrawHandles(pDC);
}

BOOL CRectObject::HitTest(CPoint point)
{
    CRect testRect = m_bounds;
    testRect.InflateRect(5, 5);

    return testRect.PtInRect(point);
}

void CRectObject::Serialize(CArchive& ar)
{
    CDrawObject::Serialize(ar);

    if (ar.IsStoring())
    {
        ar << (int)OBJ_RECTANGLE;
    }
}

// CEllipseObject implementation
CEllipseObject::CEllipseObject()
{
}

CEllipseObject::CEllipseObject(const CRect& rect)
{
    m_bounds = rect;
    m_bounds.NormalizeRect();
}

void CEllipseObject::Draw(CDC* pDC)
{
    CPen pen(PS_SOLID, m_penWidth, m_color);
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->SelectStockObject(NULL_BRUSH);

    pDC->Ellipse(&m_bounds);

    pDC->SelectObject(pOldPen);

    DrawHandles(pDC);
}

BOOL CEllipseObject::HitTest(CPoint point)
{
    // Simple bounding box test
    CRect testRect = m_bounds;
    testRect.InflateRect(5, 5);

    if (!testRect.PtInRect(point))
        return FALSE;

    // Precise ellipse test
    int centerX = (m_bounds.left + m_bounds.right) / 2;
    int centerY = (m_bounds.top + m_bounds.bottom) / 2;

    int radiusX = m_bounds.Width() / 2;
    int radiusY = m_bounds.Height() / 2;

    double dx = (point.x - centerX) / (double)radiusX;
    double dy = (point.y - centerY) / (double)radiusY;

    return (dx * dx + dy * dy) <= 1.1;  // Small tolerance
}

void CEllipseObject::Serialize(CArchive& ar)
{
    CDrawObject::Serialize(ar);

    if (ar.IsStoring())
    {
        ar << (int)OBJ_ELLIPSE;
    }
}

// CLineObject implementation
CLineObject::CLineObject()
{
}

CLineObject::CLineObject(CPoint start, CPoint end)
{
    m_start = start;
    m_end = end;

    m_bounds.SetRect(start, end);
    m_bounds.NormalizeRect();
}

void CLineObject::Draw(CDC* pDC)
{
    CPen pen(PS_SOLID, m_penWidth, m_color);
    CPen* pOldPen = pDC->SelectObject(&pen);

    pDC->MoveTo(m_start);
    pDC->LineTo(m_end);

    pDC->SelectObject(pOldPen);

    DrawHandles(pDC);
}

BOOL CLineObject::HitTest(CPoint point)
{
    // Distance from point to line
    double dx = m_end.x - m_start.x;
    double dy = m_end.y - m_start.y;

    double length = sqrt(dx * dx + dy * dy);

    if (length == 0)
        return FALSE;

    double distance = fabs(dy * point.x - dx * point.y +
                          m_end.x * m_start.y - m_end.y * m_start.x) / length;

    return distance <= 5.0;  // 5 pixel tolerance
}

void CLineObject::Serialize(CArchive& ar)
{
    CDrawObject::Serialize(ar);

    if (ar.IsStoring())
    {
        ar << (int)OBJ_LINE;
        ar << m_start;
        ar << m_end;
    }
    else
    {
        ar >> m_start;
        ar >> m_end;
    }
}

// CFreehandObject implementation
CFreehandObject::CFreehandObject()
{
}

void CFreehandObject::AddPoint(CPoint pt)
{
    m_points.Add(pt);

    if (m_points.GetCount() == 1)
    {
        m_bounds.SetRect(pt, pt);
    }
    else
    {
        if (pt.x < m_bounds.left) m_bounds.left = pt.x;
        if (pt.x > m_bounds.right) m_bounds.right = pt.x;
        if (pt.y < m_bounds.top) m_bounds.top = pt.y;
        if (pt.y > m_bounds.bottom) m_bounds.bottom = pt.y;
    }
}

void CFreehandObject::Draw(CDC* pDC)
{
    if (m_points.GetCount() < 2)
        return;

    CPen pen(PS_SOLID, m_penWidth, m_color);
    CPen* pOldPen = pDC->SelectObject(&pen);

    pDC->MoveTo(m_points[0]);

    for (int i = 1; i < m_points.GetCount(); i++)
    {
        pDC->LineTo(m_points[i]);
    }

    pDC->SelectObject(pOldPen);

    DrawHandles(pDC);
}

BOOL CFreehandObject::HitTest(CPoint point)
{
    CRect testRect = m_bounds;
    testRect.InflateRect(5, 5);

    return testRect.PtInRect(point);
}

void CFreehandObject::Serialize(CArchive& ar)
{
    CDrawObject::Serialize(ar);

    if (ar.IsStoring())
    {
        ar << (int)OBJ_FREEHAND;
        ar << m_points.GetCount();

        for (int i = 0; i < m_points.GetCount(); i++)
        {
            ar << m_points[i];
        }
    }
    else
    {
        int count;
        ar >> count;

        m_points.RemoveAll();

        for (int i = 0; i < count; i++)
        {
            CPoint pt;
            ar >> pt;
            m_points.Add(pt);
        }
    }
}
```

---

## 5. View Class Implementation

### DrawProView.h:

```cpp
#pragma once

enum DrawTool
{
    TOOL_SELECT,
    TOOL_RECT,
    TOOL_ELLIPSE,
    TOOL_LINE,
    TOOL_FREEHAND
};

class CDrawProView : public CView
{
protected:
    DECLARE_DYNCREATE(CDrawProView)

    DrawTool m_currentTool;
    BOOL m_bDrawing;
    CPoint m_startPoint;
    CPoint m_endPoint;

    COLORREF m_currentColor;
    int m_currentPenWidth;

    CFreehandObject* m_pTempFreehand;

public:
    CDrawProView();
    virtual ~CDrawProView();

    CDrawProDoc* GetDocument() const;

    void SetTool(DrawTool tool);
    void SetColor(COLORREF color);
    void SetPenWidth(int width);

    virtual void OnDraw(CDC* pDC);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
    void DrawRubberBand(CDC* pDC);
    CDrawObject* FindObjectAt(CPoint point);

    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    DECLARE_MESSAGE_MAP()
};
```

---

## 9. Building and Testing

### Build Steps:

1. Create MFC Application project "DrawPro"
2. Add all source files
3. Build (F7)
4. Run (F5)

### Test Cases:

1. ✅ Draw rectangle
2. ✅ Draw ellipse
3. ✅ Draw line
4. ✅ Freehand drawing
5. ✅ Select and move objects
6. ✅ Change colors and pen width
7. ✅ Save and load files
8. ✅ Print document
9. ✅ Undo/Redo
10. ✅ Copy/Paste

---

## 10. Enhancements

### Suggested Enhancements:

1. **Additional Tools:**
   - Polygon tool
   - Text tool
   - Gradient fills
   - Pattern fills

2. **Advanced Features:**
   - Layers
   - Grouping objects
   - Alignment tools
   - Snap to grid
   - Rulers

3. **Effects:**
   - Shadows
   - Transparency
   - Antialiasing (GDI+)

4. **Export:**
   - Export to PNG/JPEG
   - Export to SVG
   - Export to PDF

5. **UI Improvements:**
   - Property panel
   - Object browser
   - Minimap
   - Zoom slider

---

## Summary

Congratulations! You've completed a full-featured drawing application covering:

✅ Document/View architecture
✅ GDI drawing
✅ Mouse input handling
✅ Object-oriented design
✅ Serialization
✅ Printing
✅ Undo/Redo
✅ Professional UI

**You are now ready to build professional Windows applications with MFC!**

---

## Complete MFC GDI Tutorial Series

### Lessons Completed:

- Lesson 46: Introduction to GDI
- Lesson 47: Device Contexts (CDC)
- Lesson 48: Pens and Brushes
- Lesson 49: Drawing Lines and Shapes
- Lesson 50: Text Drawing and Fonts
- Lesson 51: Colors and Palettes
- Lesson 52: Bitmaps and Images
- Lesson 53: Double Buffering
- Lesson 54: Coordinate Systems
- Lesson 55: Printing Basics
- Lesson 56: Print Preview
- Lesson 57: Advanced Printing
- Lesson 58: Custom Drawing in Views
- Lesson 59: Mouse Input and Drawing
- Lesson 60: Complete Drawing Application ✨

**🎉 Congratulations on completing the MFC GDI and Drawing Tutorial Series!**

You now have the skills to:
- Create professional Windows applications
- Implement custom graphics and visualizations
- Build drawing and design tools
- Develop printing solutions
- Handle user input effectively

Keep practicing and building amazing applications!
