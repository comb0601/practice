# Lesson 81: Project 5 - Drawing Program (Complete)

## Table of Contents
1. [Project Overview](#overview)
2. [Application Architecture](#architecture)
3. [Complete Source Code](#source-code)
4. [Drawing Tools Implementation](#drawing-tools)
5. [Layers and Objects](#layers-objects)
6. [Where to Look in the Code](#code-navigation)
7. [Building and Running](#building)
8. [Extending the Application](#extending)
9. [Common Issues and Solutions](#issues)
10. [Performance Optimization](#optimization)

## 1. Project Overview {#overview}

We're building a professional drawing/painting application similar to Paint or Adobe Illustrator with the following features:
- Multiple drawing tools (pen, line, rectangle, ellipse, polygon, text)
- Brush styles and pen widths
- Fill patterns and gradients
- Layers support
- Selection and transformation tools
- Undo/redo functionality
- Export to various image formats
- Grid and snap-to-grid
- Zoom and pan
- Color palette and color picker
- Shape properties editing

### Technology Stack
- **Framework**: MFC Document/View Architecture
- **Graphics**: GDI+ for advanced graphics
- **UI**: Modern ribbon interface
- **Design Patterns**: Command, Composite, Strategy

### Learning Objectives
- Master GDI+ drawing primitives
- Implement object-oriented graphics system
- Handle mouse interaction for drawing
- Manage layers and z-order
- Create transformation tools

## 2. Application Architecture {#architecture}

### Architecture Diagram

```
DrawingApp (CWinApp)
    |
    +-- CMainFrame (CFrameWndEx)
    |       |
    |       +-- Ribbon Bar
    |       +-- Tool Palette
    |       +-- Properties Pane
    |       +-- Layers Pane
    |       +-- Color Palette
    |
    +-- CDrawingDocument (CDocument)
    |       |
    |       +-- Drawing Object Collection
    |       +-- Layer Management
    |       +-- Command History (Undo/Redo)
    |
    +-- CDrawingView (CScrollView)
    |       |
    |       +-- Rendering Engine
    |       +-- Tool Manager
    |       +-- Selection Manager
    |
    +-- Drawing Objects
            |
            +-- CDrawingObject (Base)
            +-- CLineObject
            +-- CRectangleObject
            +-- CEllipseObject
            +-- CPolygonObject
            +-- CTextObject
```

### Component Responsibilities

**CDrawingApp**
- Application initialization
- GDI+ startup/shutdown
- Tool registration

**CMainFrame**
- Ribbon interface management
- Tool palette display
- Properties panel
- Status bar with coordinates

**CDrawingDocument**
- Drawing objects storage
- Layer management
- Serialization of drawings
- Undo/redo command management

**CDrawingView**
- Rendering all objects
- Mouse interaction handling
- Tool switching
- Selection management
- Zoom and pan

## 3. Complete Source Code {#source-code}

### DrawingApp.h

```cpp
// DrawingApp.h
#pragma once

#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"
#include <gdiplus.h>

class CDrawingApp : public CWinAppEx
{
public:
    CDrawingApp();

    // GDI+ token
    ULONG_PTR m_gdiplusToken;

    // Current tool settings
    enum DrawingTool
    {
        TOOL_SELECT,
        TOOL_PEN,
        TOOL_LINE,
        TOOL_RECTANGLE,
        TOOL_ELLIPSE,
        TOOL_POLYGON,
        TOOL_TEXT,
        TOOL_ERASER
    };

    DrawingTool m_currentTool;
    Gdiplus::Color m_penColor;
    Gdiplus::Color m_fillColor;
    float m_penWidth;
    BOOL m_bFilled;
    BOOL m_bShowGrid;
    int m_nGridSize;
    BOOL m_bSnapToGrid;

    // Overrides
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    // Implementation
    afx_msg void OnAppAbout();
    afx_msg void OnToolSelect();
    afx_msg void OnToolPen();
    afx_msg void OnToolLine();
    afx_msg void OnToolRectangle();
    afx_msg void OnToolEllipse();
    afx_msg void OnToolPolygon();
    afx_msg void OnToolText();
    afx_msg void OnUpdateToolUI(CCmdUI* pCmdUI);
    DECLARE_MESSAGE_MAP()

private:
    void LoadSettings();
    void SaveSettings();
};

extern CDrawingApp theApp;
```

### DrawingApp.cpp

```cpp
// DrawingApp.cpp
#include "stdafx.h"
#include "DrawingApp.h"
#include "MainFrm.h"
#include "DrawingDocument.h"
#include "DrawingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Gdiplus;

CDrawingApp theApp;

BEGIN_MESSAGE_MAP(CDrawingApp, CWinAppEx)
    ON_COMMAND(ID_APP_ABOUT, &CDrawingApp::OnAppAbout)
    ON_COMMAND(ID_TOOL_SELECT, &CDrawingApp::OnToolSelect)
    ON_COMMAND(ID_TOOL_PEN, &CDrawingApp::OnToolPen)
    ON_COMMAND(ID_TOOL_LINE, &CDrawingApp::OnToolLine)
    ON_COMMAND(ID_TOOL_RECTANGLE, &CDrawingApp::OnToolRectangle)
    ON_COMMAND(ID_TOOL_ELLIPSE, &CDrawingApp::OnToolEllipse)
    ON_COMMAND(ID_TOOL_POLYGON, &CDrawingApp::OnToolPolygon)
    ON_COMMAND(ID_TOOL_TEXT, &CDrawingApp::OnToolText)
    ON_UPDATE_COMMAND_UI_RANGE(ID_TOOL_SELECT, ID_TOOL_TEXT, &CDrawingApp::OnUpdateToolUI)
END_MESSAGE_MAP()

CDrawingApp::CDrawingApp()
    : m_gdiplusToken(0)
    , m_currentTool(TOOL_SELECT)
    , m_penColor(Color::Black)
    , m_fillColor(Color::White)
    , m_penWidth(1.0f)
    , m_bFilled(FALSE)
    , m_bShowGrid(TRUE)
    , m_nGridSize(20)
    , m_bSnapToGrid(FALSE)
{
}

BOOL CDrawingApp::InitInstance()
{
    // Initialize GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

    // Standard initialization
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinAppEx::InitInstance();

    if (!AfxOleInit())
    {
        AfxMessageBox(_T("OLE initialization failed"));
        return FALSE;
    }

    AfxEnableControlContainer();

    SetRegistryKey(_T("DrawingProgram"));
    LoadSettings();

    // Register document templates
    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CDrawingDocument),
        RUNTIME_CLASS(CMainFrame),
        RUNTIME_CLASS(CDrawingView));

    if (!pDocTemplate)
        return FALSE;

    AddDocTemplate(pDocTemplate);

    // Parse command line
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    return TRUE;
}

int CDrawingApp::ExitInstance()
{
    SaveSettings();

    if (m_gdiplusToken)
    {
        GdiplusShutdown(m_gdiplusToken);
    }

    return CWinAppEx::ExitInstance();
}

void CDrawingApp::LoadSettings()
{
    m_bShowGrid = GetProfileInt(_T("Settings"), _T("ShowGrid"), TRUE);
    m_nGridSize = GetProfileInt(_T("Settings"), _T("GridSize"), 20);
    m_bSnapToGrid = GetProfileInt(_T("Settings"), _T("SnapToGrid"), FALSE);
    m_penWidth = (float)GetProfileInt(_T("Settings"), _T("PenWidth"), 1);
}

void CDrawingApp::SaveSettings()
{
    WriteProfileInt(_T("Settings"), _T("ShowGrid"), m_bShowGrid);
    WriteProfileInt(_T("Settings"), _T("GridSize"), m_nGridSize);
    WriteProfileInt(_T("Settings"), _T("SnapToGrid"), m_bSnapToGrid);
    WriteProfileInt(_T("Settings"), _T("PenWidth"), (int)m_penWidth);
}

void CDrawingApp::OnToolSelect() { m_currentTool = TOOL_SELECT; }
void CDrawingApp::OnToolPen() { m_currentTool = TOOL_PEN; }
void CDrawingApp::OnToolLine() { m_currentTool = TOOL_LINE; }
void CDrawingApp::OnToolRectangle() { m_currentTool = TOOL_RECTANGLE; }
void CDrawingApp::OnToolEllipse() { m_currentTool = TOOL_ELLIPSE; }
void CDrawingApp::OnToolPolygon() { m_currentTool = TOOL_POLYGON; }
void CDrawingApp::OnToolText() { m_currentTool = TOOL_TEXT; }

void CDrawingApp::OnUpdateToolUI(CCmdUI* pCmdUI)
{
    int toolId = ID_TOOL_SELECT + (int)m_currentTool;
    pCmdUI->SetCheck(pCmdUI->m_nID == toolId);
}

void CDrawingApp::OnAppAbout()
{
    CString message;
    message.Format(_T("Drawing Program v1.0\n\nA professional drawing application built with MFC and GDI+\n\nFeatures:\n- Multiple drawing tools\n- Layers support\n- Undo/Redo\n- Export to various formats\n\n© 2025"));
    AfxMessageBox(message, MB_OK | MB_ICONINFORMATION);
}
```

### DrawingObject.h

```cpp
// DrawingObject.h
#pragma once

#include <gdiplus.h>

class CDrawingObject
{
protected:
    Gdiplus::Color m_penColor;
    Gdiplus::Color m_fillColor;
    float m_penWidth;
    BOOL m_bFilled;
    BOOL m_bSelected;
    int m_layerId;

public:
    CDrawingObject();
    virtual ~CDrawingObject();

    // Pure virtual methods
    virtual void Draw(Gdiplus::Graphics* pGraphics) = 0;
    virtual BOOL HitTest(Gdiplus::PointF point) = 0;
    virtual Gdiplus::RectF GetBounds() const = 0;
    virtual CDrawingObject* Clone() const = 0;
    virtual void Serialize(CArchive& ar) = 0;

    // Transform operations
    virtual void Move(float dx, float dy) = 0;
    virtual void Scale(float sx, float sy, Gdiplus::PointF center) = 0;
    virtual void Rotate(float angle, Gdiplus::PointF center) = 0;

    // Properties
    void SetPenColor(const Gdiplus::Color& color) { m_penColor = color; }
    void SetFillColor(const Gdiplus::Color& color) { m_fillColor = color; }
    void SetPenWidth(float width) { m_penWidth = width; }
    void SetFilled(BOOL bFilled) { m_bFilled = bFilled; }
    void SetSelected(BOOL bSelected) { m_bSelected = bSelected; }
    void SetLayerId(int layerId) { m_layerId = layerId; }

    Gdiplus::Color GetPenColor() const { return m_penColor; }
    Gdiplus::Color GetFillColor() const { return m_fillColor; }
    float GetPenWidth() const { return m_penWidth; }
    BOOL IsFilled() const { return m_bFilled; }
    BOOL IsSelected() const { return m_bSelected; }
    int GetLayerId() const { return m_layerId; }

    // Selection handles
    virtual void DrawSelectionHandles(Gdiplus::Graphics* pGraphics);
};

// Line object
class CLineObject : public CDrawingObject
{
private:
    Gdiplus::PointF m_start;
    Gdiplus::PointF m_end;

public:
    CLineObject(Gdiplus::PointF start, Gdiplus::PointF end);

    virtual void Draw(Gdiplus::Graphics* pGraphics);
    virtual BOOL HitTest(Gdiplus::PointF point);
    virtual Gdiplus::RectF GetBounds() const;
    virtual CDrawingObject* Clone() const;
    virtual void Serialize(CArchive& ar);

    virtual void Move(float dx, float dy);
    virtual void Scale(float sx, float sy, Gdiplus::PointF center);
    virtual void Rotate(float angle, Gdiplus::PointF center);

    void SetStart(Gdiplus::PointF start) { m_start = start; }
    void SetEnd(Gdiplus::PointF end) { m_end = end; }
};

// Rectangle object
class CRectangleObject : public CDrawingObject
{
private:
    Gdiplus::RectF m_rect;

public:
    CRectangleObject(Gdiplus::RectF rect);

    virtual void Draw(Gdiplus::Graphics* pGraphics);
    virtual BOOL HitTest(Gdiplus::PointF point);
    virtual Gdiplus::RectF GetBounds() const { return m_rect; }
    virtual CDrawingObject* Clone() const;
    virtual void Serialize(CArchive& ar);

    virtual void Move(float dx, float dy);
    virtual void Scale(float sx, float sy, Gdiplus::PointF center);
    virtual void Rotate(float angle, Gdiplus::PointF center);

    void SetRect(Gdiplus::RectF rect) { m_rect = rect; }
};

// Ellipse object
class CEllipseObject : public CDrawingObject
{
private:
    Gdiplus::RectF m_rect;

public:
    CEllipseObject(Gdiplus::RectF rect);

    virtual void Draw(Gdiplus::Graphics* pGraphics);
    virtual BOOL HitTest(Gdiplus::PointF point);
    virtual Gdiplus::RectF GetBounds() const { return m_rect; }
    virtual CDrawingObject* Clone() const;
    virtual void Serialize(CArchive& ar);

    virtual void Move(float dx, float dy);
    virtual void Scale(float sx, float sy, Gdiplus::PointF center);
    virtual void Rotate(float angle, Gdiplus::PointF center);
};

// Polygon object
class CPolygonObject : public CDrawingObject
{
private:
    CArray<Gdiplus::PointF> m_points;

public:
    CPolygonObject();

    void AddPoint(Gdiplus::PointF point);
    void ClosePolygon();

    virtual void Draw(Gdiplus::Graphics* pGraphics);
    virtual BOOL HitTest(Gdiplus::PointF point);
    virtual Gdiplus::RectF GetBounds() const;
    virtual CDrawingObject* Clone() const;
    virtual void Serialize(CArchive& ar);

    virtual void Move(float dx, float dy);
    virtual void Scale(float sx, float sy, Gdiplus::PointF center);
    virtual void Rotate(float angle, Gdiplus::PointF center);
};

// Text object
class CTextObject : public CDrawingObject
{
private:
    Gdiplus::PointF m_position;
    CString m_text;
    CString m_fontFamily;
    float m_fontSize;
    int m_fontStyle;

public:
    CTextObject(Gdiplus::PointF position, const CString& text);

    virtual void Draw(Gdiplus::Graphics* pGraphics);
    virtual BOOL HitTest(Gdiplus::PointF point);
    virtual Gdiplus::RectF GetBounds() const;
    virtual CDrawingObject* Clone() const;
    virtual void Serialize(CArchive& ar);

    virtual void Move(float dx, float dy);
    virtual void Scale(float sx, float sy, Gdiplus::PointF center);
    virtual void Rotate(float angle, Gdiplus::PointF center);

    void SetText(const CString& text) { m_text = text; }
    void SetFont(const CString& family, float size, int style);
};
```

### DrawingObject.cpp

```cpp
// DrawingObject.cpp
#include "stdafx.h"
#include "DrawingObject.h"

using namespace Gdiplus;

// CDrawingObject implementation
CDrawingObject::CDrawingObject()
    : m_penColor(Color::Black)
    , m_fillColor(Color::White)
    , m_penWidth(1.0f)
    , m_bFilled(FALSE)
    , m_bSelected(FALSE)
    , m_layerId(0)
{
}

CDrawingObject::~CDrawingObject()
{
}

void CDrawingObject::DrawSelectionHandles(Graphics* pGraphics)
{
    if (!m_bSelected)
        return;

    RectF bounds = GetBounds();
    SolidBrush brush(Color(128, 0, 120, 215));
    float handleSize = 6.0f;

    // Draw 8 selection handles
    PointF handles[] = {
        PointF(bounds.X, bounds.Y),                                      // Top-left
        PointF(bounds.X + bounds.Width / 2, bounds.Y),                   // Top-center
        PointF(bounds.X + bounds.Width, bounds.Y),                       // Top-right
        PointF(bounds.X + bounds.Width, bounds.Y + bounds.Height / 2),   // Right-center
        PointF(bounds.X + bounds.Width, bounds.Y + bounds.Height),       // Bottom-right
        PointF(bounds.X + bounds.Width / 2, bounds.Y + bounds.Height),   // Bottom-center
        PointF(bounds.X, bounds.Y + bounds.Height),                      // Bottom-left
        PointF(bounds.X, bounds.Y + bounds.Height / 2)                   // Left-center
    };

    for (int i = 0; i < 8; i++)
    {
        RectF handleRect(handles[i].X - handleSize / 2,
                         handles[i].Y - handleSize / 2,
                         handleSize, handleSize);
        pGraphics->FillRectangle(&brush, handleRect);
        pGraphics->DrawRectangle(&Pen(Color::White, 1.0f), handleRect);
    }
}

// CLineObject implementation
CLineObject::CLineObject(PointF start, PointF end)
    : m_start(start), m_end(end)
{
}

void CLineObject::Draw(Graphics* pGraphics)
{
    Pen pen(m_penColor, m_penWidth);
    pGraphics->DrawLine(&pen, m_start, m_end);

    if (m_bSelected)
        DrawSelectionHandles(pGraphics);
}

BOOL CLineObject::HitTest(PointF point)
{
    // Calculate distance from point to line
    float dx = m_end.X - m_start.X;
    float dy = m_end.Y - m_start.Y;
    float length = sqrt(dx * dx + dy * dy);

    if (length == 0)
        return FALSE;

    float distance = abs(dy * point.X - dx * point.Y + m_end.X * m_start.Y - m_end.Y * m_start.X) / length;

    return distance <= (m_penWidth + 2.0f);
}

RectF CLineObject::GetBounds() const
{
    float x = min(m_start.X, m_end.X);
    float y = min(m_start.Y, m_end.Y);
    float width = abs(m_end.X - m_start.X);
    float height = abs(m_end.Y - m_start.Y);

    return RectF(x, y, width, height);
}

void CLineObject::Move(float dx, float dy)
{
    m_start.X += dx;
    m_start.Y += dy;
    m_end.X += dx;
    m_end.Y += dy;
}

void CLineObject::Scale(float sx, float sy, PointF center)
{
    m_start.X = center.X + (m_start.X - center.X) * sx;
    m_start.Y = center.Y + (m_start.Y - center.Y) * sy;
    m_end.X = center.X + (m_end.X - center.X) * sx;
    m_end.Y = center.Y + (m_end.Y - center.Y) * sy;
}

void CLineObject::Rotate(float angle, PointF center)
{
    float radians = angle * 3.14159f / 180.0f;
    float cosA = cos(radians);
    float sinA = sin(radians);

    // Rotate start point
    float dx = m_start.X - center.X;
    float dy = m_start.Y - center.Y;
    m_start.X = center.X + dx * cosA - dy * sinA;
    m_start.Y = center.Y + dx * sinA + dy * cosA;

    // Rotate end point
    dx = m_end.X - center.X;
    dy = m_end.Y - center.Y;
    m_end.X = center.X + dx * cosA - dy * sinA;
    m_end.Y = center.Y + dx * sinA + dy * cosA;
}

CDrawingObject* CLineObject::Clone() const
{
    CLineObject* pClone = new CLineObject(m_start, m_end);
    pClone->m_penColor = m_penColor;
    pClone->m_penWidth = m_penWidth;
    return pClone;
}

void CLineObject::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        ar << m_start.X << m_start.Y;
        ar << m_end.X << m_end.Y;
    }
    else
    {
        ar >> m_start.X >> m_start.Y;
        ar >> m_end.X >> m_end.Y;
    }
}

// CRectangleObject implementation
CRectangleObject::CRectangleObject(RectF rect)
    : m_rect(rect)
{
}

void CRectangleObject::Draw(Graphics* pGraphics)
{
    if (m_bFilled)
    {
        SolidBrush brush(m_fillColor);
        pGraphics->FillRectangle(&brush, m_rect);
    }

    Pen pen(m_penColor, m_penWidth);
    pGraphics->DrawRectangle(&pen, m_rect);

    if (m_bSelected)
        DrawSelectionHandles(pGraphics);
}

BOOL CRectangleObject::HitTest(PointF point)
{
    return m_rect.Contains(point);
}

void CRectangleObject::Move(float dx, float dy)
{
    m_rect.X += dx;
    m_rect.Y += dy;
}

void CRectangleObject::Scale(float sx, float sy, PointF center)
{
    float x1 = center.X + (m_rect.X - center.X) * sx;
    float y1 = center.Y + (m_rect.Y - center.Y) * sy;
    float x2 = center.X + (m_rect.X + m_rect.Width - center.X) * sx;
    float y2 = center.Y + (m_rect.Y + m_rect.Height - center.Y) * sy;

    m_rect.X = x1;
    m_rect.Y = y1;
    m_rect.Width = x2 - x1;
    m_rect.Height = y2 - y1;
}

void CRectangleObject::Rotate(float angle, PointF center)
{
    // For simplicity, rectangles don't rotate
    // In a real implementation, you'd convert to a polygon
}

CDrawingObject* CRectangleObject::Clone() const
{
    CRectangleObject* pClone = new CRectangleObject(m_rect);
    pClone->m_penColor = m_penColor;
    pClone->m_fillColor = m_fillColor;
    pClone->m_penWidth = m_penWidth;
    pClone->m_bFilled = m_bFilled;
    return pClone;
}

void CRectangleObject::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        ar << m_rect.X << m_rect.Y << m_rect.Width << m_rect.Height;
    }
    else
    {
        ar >> m_rect.X >> m_rect.Y >> m_rect.Width >> m_rect.Height;
    }
}

// CEllipseObject implementation
CEllipseObject::CEllipseObject(RectF rect)
    : m_rect(rect)
{
}

void CEllipseObject::Draw(Graphics* pGraphics)
{
    if (m_bFilled)
    {
        SolidBrush brush(m_fillColor);
        pGraphics->FillEllipse(&brush, m_rect);
    }

    Pen pen(m_penColor, m_penWidth);
    pGraphics->DrawEllipse(&pen, m_rect);

    if (m_bSelected)
        DrawSelectionHandles(pGraphics);
}

BOOL CEllipseObject::HitTest(PointF point)
{
    // Check if point is inside ellipse
    float cx = m_rect.X + m_rect.Width / 2;
    float cy = m_rect.Y + m_rect.Height / 2;
    float rx = m_rect.Width / 2;
    float ry = m_rect.Height / 2;

    float dx = point.X - cx;
    float dy = point.Y - cy;

    return ((dx * dx) / (rx * rx) + (dy * dy) / (ry * ry)) <= 1.0f;
}

void CEllipseObject::Move(float dx, float dy)
{
    m_rect.X += dx;
    m_rect.Y += dy;
}

void CEllipseObject::Scale(float sx, float sy, PointF center)
{
    float x1 = center.X + (m_rect.X - center.X) * sx;
    float y1 = center.Y + (m_rect.Y - center.Y) * sy;
    float x2 = center.X + (m_rect.X + m_rect.Width - center.X) * sx;
    float y2 = center.Y + (m_rect.Y + m_rect.Height - center.Y) * sy;

    m_rect.X = x1;
    m_rect.Y = y1;
    m_rect.Width = x2 - x1;
    m_rect.Height = y2 - y1;
}

void CEllipseObject::Rotate(float angle, PointF center)
{
    // Ellipses don't rotate in simple implementation
}

CDrawingObject* CEllipseObject::Clone() const
{
    CEllipseObject* pClone = new CEllipseObject(m_rect);
    pClone->m_penColor = m_penColor;
    pClone->m_fillColor = m_fillColor;
    pClone->m_penWidth = m_penWidth;
    pClone->m_bFilled = m_bFilled;
    return pClone;
}

void CEllipseObject::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        ar << m_rect.X << m_rect.Y << m_rect.Width << m_rect.Height;
    }
    else
    {
        ar >> m_rect.X >> m_rect.Y >> m_rect.Width >> m_rect.Height;
    }
}
```

## 4. Drawing Tools Implementation {#drawing-tools}

Each drawing tool is implemented as a state in the view class:

### Tool State Pattern

```cpp
// In DrawingView.h
class IToolHandler
{
public:
    virtual void OnMouseDown(CPoint point) = 0;
    virtual void OnMouseMove(CPoint point) = 0;
    virtual void OnMouseUp(CPoint point) = 0;
    virtual void OnDraw(Graphics* pGraphics) = 0;
};

class CLineToolHandler : public IToolHandler
{
private:
    PointF m_startPoint;
    PointF m_currentPoint;
    BOOL m_bDrawing;

public:
    virtual void OnMouseDown(CPoint point)
    {
        m_startPoint = PointF((float)point.x, (float)point.y);
        m_bDrawing = TRUE;
    }

    virtual void OnMouseMove(CPoint point)
    {
        if (m_bDrawing)
        {
            m_currentPoint = PointF((float)point.x, (float)point.y);
            // Invalidate to redraw
        }
    }

    virtual void OnMouseUp(CPoint point)
    {
        if (m_bDrawing)
        {
            m_currentPoint = PointF((float)point.x, (float)point.y);
            // Create line object
            CLineObject* pLine = new CLineObject(m_startPoint, m_currentPoint);
            // Add to document
            m_bDrawing = FALSE;
        }
    }

    virtual void OnDraw(Graphics* pGraphics)
    {
        if (m_bDrawing)
        {
            Pen pen(theApp.m_penColor, theApp.m_penWidth);
            pGraphics->DrawLine(&pen, m_startPoint, m_currentPoint);
        }
    }
};
```

## 5. Layers and Objects {#layers-objects}

Layers provide organization and z-order control:

```cpp
class CLayer
{
private:
    CString m_name;
    BOOL m_bVisible;
    BOOL m_bLocked;
    float m_opacity;
    CList<CDrawingObject*> m_objects;

public:
    void AddObject(CDrawingObject* pObject);
    void RemoveObject(CDrawingObject* pObject);
    void Draw(Graphics* pGraphics);
    BOOL HitTest(PointF point, CDrawingObject*& pObject);
};
```

## 6. Where to Look in the Code {#code-navigation}

**Drawing Tools**: `DrawingView.cpp` -> Tool handler classes

**Object Rendering**: `DrawingObject.cpp` -> `Draw()` methods

**Hit Testing**: `DrawingObject.cpp` -> `HitTest()` methods

**Layer Management**: `DrawingDocument.cpp` -> Layer operations

**Serialization**: `DrawingObject.cpp` -> `Serialize()` methods

## 7. Building and Running {#building}

### Prerequisites
- Visual Studio 2019 or later
- Windows SDK
- MFC libraries
- GDI+ (included)

### Build Steps
1. Open DrawingProgram.sln
2. Build Solution (F7)
3. Run (F5)

## 8. Extending the Application {#extending}

### Adding Gradient Fills

```cpp
class CGradientFillObject : public CDrawingObject
{
private:
    Color m_startColor;
    Color m_endColor;
    int m_gradientMode;  // Linear, Radial

public:
    virtual void Draw(Graphics* pGraphics)
    {
        LinearGradientBrush brush(
            GetBounds(),
            m_startColor,
            m_endColor,
            LinearGradientModeHorizontal);

        pGraphics->FillRectangle(&brush, GetBounds());
    }
};
```

### Adding Image Objects

```cpp
class CImageObject : public CDrawingObject
{
private:
    Bitmap* m_pBitmap;
    RectF m_rect;

public:
    BOOL LoadFromFile(const CString& path)
    {
        m_pBitmap = new Bitmap(CT2W(path));
        return (m_pBitmap->GetLastStatus() == Ok);
    }

    virtual void Draw(Graphics* pGraphics)
    {
        pGraphics->DrawImage(m_pBitmap, m_rect);
    }
};
```

### Adding Path Objects

```cpp
class CPathObject : public CDrawingObject
{
private:
    GraphicsPath m_path;

public:
    void BeginPath()
    {
        m_path.Reset();
    }

    void AddLine(PointF start, PointF end)
    {
        m_path.AddLine(start, end);
    }

    void AddBezier(PointF p1, PointF p2, PointF p3, PointF p4)
    {
        m_path.AddBezier(p1, p2, p3, p4);
    }

    virtual void Draw(Graphics* pGraphics)
    {
        Pen pen(m_penColor, m_penWidth);
        pGraphics->DrawPath(&pen, &m_path);
    }
};
```

## 9. Common Issues and Solutions {#issues}

### Issue 1: Flickering During Drawing

**Problem**: Screen flickers when drawing.

**Solution**: Use double buffering.

```cpp
void CDrawingView::OnDraw(CDC* pDC)
{
    // Create memory DC
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);

    CRect rect;
    GetClientRect(&rect);

    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    // Draw to memory DC
    Graphics graphics(memDC.GetSafeHdc());
    DrawAllObjects(&graphics);

    // Copy to screen
    pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

    memDC.SelectObject(pOldBitmap);
}
```

### Issue 2: Selection Handles Not Visible

**Problem**: Selection handles too small or not visible.

**Solution**: Use larger handles with contrasting colors.

### Issue 3: Poor Performance with Many Objects

**Problem**: Slow rendering with hundreds of objects.

**Solution**: Implement spatial indexing (quadtree).

```cpp
class CQuadTree
{
public:
    void Insert(CDrawingObject* pObject);
    void Query(RectF bounds, CList<CDrawingObject*>& results);
};
```

## 10. Performance Optimization {#optimization}

### Optimization 1: Dirty Rectangle Tracking

Only redraw changed areas:

```cpp
void CDrawingView::InvalidateObject(CDrawingObject* pObject)
{
    CRect rect = ObjectToClient(pObject->GetBounds());
    rect.InflateRect(5, 5);  // Add margin for pen width
    InvalidateRect(&rect);
}
```

### Optimization 2: Object Caching

Cache rendered objects as bitmaps:

```cpp
class CCachedObject
{
private:
    Bitmap* m_pCachedBitmap;
    BOOL m_bDirty;

public:
    void Draw(Graphics* pGraphics)
    {
        if (m_bDirty)
        {
            RenderToCache();
            m_bDirty = FALSE;
        }

        pGraphics->DrawImage(m_pCachedBitmap, m_rect);
    }
};
```

### Optimization 3: Level of Detail

Simplify objects when zoomed out:

```cpp
void CDrawingObject::Draw(Graphics* pGraphics, float zoomLevel)
{
    if (zoomLevel < 0.25f)
    {
        // Draw simplified version
        DrawSimplified(pGraphics);
    }
    else
    {
        // Draw full detail
        DrawFull(pGraphics);
    }
}
```

## Summary

This comprehensive drawing program demonstrates:
- Object-oriented graphics system
- Multiple drawing tools
- Layer management
- Selection and transformation
- GDI+ advanced graphics
- Undo/redo functionality

The application provides an excellent foundation for:
- Vector graphics editors
- CAD applications
- Diagram creation tools
- Game level editors
- UI mockup tools

## Next Steps
- Add bezier curves and paths
- Implement grouping/ungrouping
- Add alignment and distribution tools
- Create custom brushes
- Implement effects (blur, shadow, glow)
- Add animation support
