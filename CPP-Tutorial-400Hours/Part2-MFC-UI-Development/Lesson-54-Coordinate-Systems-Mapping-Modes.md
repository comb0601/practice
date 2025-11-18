# Lesson 54: Coordinate Systems and Mapping Modes

**Duration**: 5 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding Coordinate Systems
2. Device vs Logical Coordinates
3. Mapping Modes Overview
4. MM_TEXT Mode
5. MM_LOMETRIC and MM_HIMETRIC
6. MM_LOENGLISH and MM_HIENGLISH
7. MM_ISOTROPIC and MM_ANISOTROPIC
8. Viewport and Window Origins
9. Coordinate Transformations
10. Practical Examples

---

## 1. Understanding Coordinate Systems

GDI uses two types of coordinates:
- **Device Coordinates**: Physical screen/printer pixels
- **Logical Coordinates**: Application-defined units

### Why Two Coordinate Systems?

✅ **Device Independence**: Same code works on different devices
✅ **Flexible Units**: Use meaningful units (inches, millimeters)
✅ **Easy Scaling**: Zoom in/out without changing drawing code
✅ **Professional Output**: Print exactly as displayed

---

## 2. Device vs Logical Coordinates

### Coordinate Conversion:

```
Logical Coordinates (your code)
         ↓
    [Mapping Mode]
         ↓
Device Coordinates (screen pixels)
```

### Example:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Default: MM_TEXT mode (1 logical unit = 1 pixel)
    CPoint logical(100, 100);

    // Draw at logical coordinates
    pDC->TextOut(logical.x, logical.y, _T("Logical (100, 100)"));

    // Convert to device coordinates
    pDC->LPtoDP(&logical);

    CString info;
    info.Format(_T("Device: (%d, %d)"), logical.x, logical.y);
    pDC->TextOut(10, 10, info);
}
```

---

## 3. Mapping Modes Overview

### Available Mapping Modes:

```cpp
MM_TEXT         // 1 unit = 1 pixel, Y increases downward
MM_LOMETRIC     // 1 unit = 0.1 millimeter, Y increases upward
MM_HIMETRIC     // 1 unit = 0.01 millimeter, Y increases upward
MM_LOENGLISH    // 1 unit = 0.01 inch, Y increases upward
MM_HIENGLISH    // 1 unit = 0.001 inch, Y increases upward
MM_TWIPS        // 1 unit = 1/1440 inch, Y increases upward
MM_ISOTROPIC    // Custom, X and Y scale equally
MM_ANISOTROPIC  // Custom, X and Y scale independently
```

### Setting Mapping Mode:

```cpp
int SetMapMode(int nMapMode);
```

---

## 4. MM_TEXT Mode

**Default mode**: 1 logical unit = 1 pixel, origin at top-left, Y increases downward.

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    pDC->SetMapMode(MM_TEXT);  // Default

    // Origin at (0, 0) top-left
    // X increases right, Y increases down

    pDC->Rectangle(50, 50, 150, 150);
    // 100x100 pixel rectangle

    pDC->TextOut(10, 10, _T("MM_TEXT mode"));
}
```

### Characteristics:

✅ Simple and intuitive
✅ Direct pixel control
✅ Fast performance
⚠️ Device-dependent (different DPI = different size)
⚠️ Y axis downward (unconventional for math)

---

## 5. MM_LOMETRIC and MM_HIMETRIC

### MM_LOMETRIC: 1 unit = 0.1 mm

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    pDC->SetMapMode(MM_LOMETRIC);

    // Origin at bottom-left
    // X increases right, Y increases UP
    // 1 unit = 0.1 mm

    // Draw 50mm x 50mm rectangle
    pDC->Rectangle(100, 100, 600, 600);
    // 500 units = 50 mm

    pDC->TextOut(100, -50, _T("MM_LOMETRIC"));
    // Note: Y is negative (above origin)
}
```

### MM_HIMETRIC: 1 unit = 0.01 mm

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    pDC->SetMapMode(MM_HIMETRIC);

    // 1 unit = 0.01 mm
    // 100 units = 1 mm

    // Draw 50mm x 50mm rectangle
    pDC->Rectangle(1000, 1000, 6000, 6000);
    // 5000 units = 50 mm

    pDC->TextOut(1000, -500, _T("MM_HIMETRIC"));
}
```

### Metric Mode Example:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    pDC->SetMapMode(MM_LOMETRIC);

    // Set viewport origin to center of screen
    CRect rect;
    GetClientRect(&rect);
    pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);

    // Now (0,0) is at center
    // Draw axes
    CPen axisPen(PS_SOLID, 1, RGB(200, 200, 200));
    CPen* pOldPen = pDC->SelectObject(&axisPen);

    pDC->MoveTo(-1000, 0);
    pDC->LineTo(1000, 0);
    pDC->MoveTo(0, -1000);
    pDC->LineTo(0, 1000);

    pDC->SelectObject(pOldPen);

    // Draw square 20mm x 20mm
    CPen pen(PS_SOLID, 20, RGB(255, 0, 0));  // 2mm thick
    pDC->SelectObject(&pen);
    pDC->SelectStockObject(NULL_BRUSH);

    pDC->Rectangle(-100, -100, 100, 100);
    // 200 units = 20mm

    pDC->SelectObject(pOldPen);
}
```

---

## 6. MM_LOENGLISH and MM_HIENGLISH

### MM_LOENGLISH: 1 unit = 0.01 inch

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    pDC->SetMapMode(MM_LOENGLISH);

    // 1 unit = 0.01 inch
    // 100 units = 1 inch

    // Draw 2 inch x 2 inch square
    pDC->Rectangle(100, 100, 300, 300);
    // 200 units = 2 inches

    pDC->TextOut(100, -50, _T("MM_LOENGLISH - 2\" square"));
}
```

### MM_HIENGLISH: 1 unit = 0.001 inch

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    pDC->SetMapMode(MM_HIENGLISH);

    // 1 unit = 0.001 inch
    // 1000 units = 1 inch

    // Draw 2 inch x 2 inch square
    pDC->Rectangle(1000, 1000, 3000, 3000);
    // 2000 units = 2 inches

    pDC->TextOut(1000, -500, _T("MM_HIENGLISH - 2\" square"));
}
```

### Ruler Example:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    pDC->SetMapMode(MM_LOENGLISH);

    // Draw ruler (12 inches)
    CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);

    int y = -200;

    for (int i = 0; i <= 12; i++)
    {
        int x = 100 + i * 100;  // 100 units = 1 inch

        // Major tick
        pDC->MoveTo(x, y);
        pDC->LineTo(x, y - 50);

        // Label
        CString label;
        label.Format(_T("%d\""), i);
        pDC->TextOut(x - 20, y - 80, label);

        // Minor ticks (1/8 inch)
        if (i < 12)
        {
            for (int j = 1; j < 8; j++)
            {
                int minorX = x + j * 12;  // 12.5 units = 1/8 inch
                int tickLen = (j == 4) ? 30 : 20;

                pDC->MoveTo(minorX, y);
                pDC->LineTo(minorX, y - tickLen);
            }
        }
    }

    pDC->SelectObject(pOldPen);
}
```

---

## 7. MM_ISOTROPIC and MM_ANISOTROPIC

### MM_ISOTROPIC: Equal X and Y Scaling

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    pDC->SetMapMode(MM_ISOTROPIC);

    CRect rect;
    GetClientRect(&rect);

    // Set logical window (coordinate space)
    pDC->SetWindowExt(1000, 1000);

    // Set physical viewport (device space)
    pDC->SetViewportExt(rect.Width(), rect.Height());

    // System adjusts to maintain aspect ratio
    // Circles stay circular when window resized

    // Draw coordinate system
    CPen axisPen(PS_SOLID, 1, RGB(200, 200, 200));
    CPen* pOldPen = pDC->SelectObject(&axisPen);

    pDC->MoveTo(0, 500);
    pDC->LineTo(1000, 500);
    pDC->MoveTo(500, 0);
    pDC->LineTo(500, 1000);

    pDC->SelectObject(pOldPen);

    // Draw circle
    CPen pen(PS_SOLID, 10, RGB(255, 0, 0));
    pDC->SelectObject(&pen);
    pDC->SelectStockObject(NULL_BRUSH);

    pDC->Ellipse(300, 300, 700, 700);
    // Always circular!

    pDC->SelectObject(pOldPen);
}
```

### MM_ANISOTROPIC: Independent X and Y Scaling

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    pDC->SetMapMode(MM_ANISOTROPIC);

    CRect rect;
    GetClientRect(&rect);

    // Logical window
    pDC->SetWindowExt(1000, 1000);

    // Physical viewport - fills entire window
    pDC->SetViewportExt(rect.Width(), rect.Height());

    // X and Y scale independently
    // Circles become ellipses when window resized

    pDC->Ellipse(250, 250, 750, 750);
    // May not be circular depending on window size
}
```

---

## 8. Viewport and Window Origins

### Understanding Origins:

```
Window Origin (Logical)  →  Viewport Origin (Device)
    (0, 0)                       (0, 0)
       ↓                            ↓
  [Mapping Mode]
```

### SetViewportOrg: Device Coordinates

```cpp
CPoint SetViewportOrg(int x, int y);
```

**Example:**
```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    // Move origin to center of window
    pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);

    // Now (0,0) logical = center of window
    pDC->Ellipse(-50, -50, 50, 50);
    // Circle centered on screen
}
```

### SetWindowOrg: Logical Coordinates

```cpp
CPoint SetWindowOrg(int x, int y);
```

**Example:**
```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Set logical origin
    pDC->SetWindowOrg(-500, -500);

    // Now point (500, 500) maps to device (0, 0)
    pDC->Rectangle(500, 500, 600, 600);
    // Draws at top-left corner
}
```

---

## 9. Coordinate Transformations

### LPtoDP: Logical to Device

```cpp
void LPtoDP(LPPOINT lpPoints, int nCount = 1);
void LPtoDP(LPRECT lpRect);
void LPtoDP(LPSIZE lpSize);
```

**Example:**
```cpp
void CMyView::OnDraw(CDC* pDC)
{
    pDC->SetMapMode(MM_LOMETRIC);

    CPoint logical(1000, 1000);  // 100mm x 100mm
    CString info;
    info.Format(_T("Logical: (%d, %d)"), logical.x, logical.y);
    pDC->TextOut(100, -100, info);

    // Convert to device coordinates
    pDC->LPtoDP(&logical);

    info.Format(_T("Device: (%d, %d)"), logical.x, logical.y);
    pDC->TextOut(100, -200, info);
}
```

### DPtoLP: Device to Logical

```cpp
void DPtoLP(LPPOINT lpPoints, int nCount = 1);
void DPtoLP(LPRECT lpRect);
void DPtoLP(LPSIZE lpSize);
```

**Example:**
```cpp
void CMyView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // point is in device coordinates

    CClientDC dc(this);
    dc.SetMapMode(MM_LOMETRIC);

    // Convert to logical
    dc.DPtoLP(&point);

    CString info;
    info.Format(_T("Clicked at (%d, %d) logical"), point.x, point.y);
    AfxMessageBox(info);
}
```

---

## 10. Practical Examples

### Zoom Viewer:

```cpp
class CZoomView : public CView
{
protected:
    double m_zoom;
    CPoint m_center;

public:
    CZoomView() : m_zoom(1.0), m_center(0, 0) {}

    void OnDraw(CDC* pDC) override
    {
        pDC->SetMapMode(MM_ANISOTROPIC);

        CRect rect;
        GetClientRect(&rect);

        // Calculate extents based on zoom
        int logicalWidth = (int)(1000 / m_zoom);
        int logicalHeight = (int)(1000 / m_zoom);

        pDC->SetWindowExt(logicalWidth, logicalHeight);
        pDC->SetViewportExt(rect.Width(), rect.Height());

        // Center on m_center point
        pDC->SetWindowOrg(m_center.x - logicalWidth / 2,
                         m_center.y - logicalHeight / 2);

        // Draw content
        DrawContent(pDC);

        // Show zoom level
        pDC->SetMapMode(MM_TEXT);  // Reset for text
        CString info;
        info.Format(_T("Zoom: %.0f%%"), m_zoom * 100);
        pDC->TextOut(10, 10, info);
    }

    void DrawContent(CDC* pDC)
    {
        // Draw in logical coordinates (0-1000)
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                CRect rect(i * 100, j * 100,
                          (i + 1) * 100, (j + 1) * 100);

                COLORREF color = RGB((i * 25), (j * 25), 128);
                CBrush brush(color);
                CBrush* pOldBrush = pDC->SelectObject(&brush);

                pDC->Rectangle(&rect);
                pDC->SelectObject(pOldBrush);
            }
        }
    }

    void ZoomIn()
    {
        m_zoom *= 1.5;
        Invalidate();
    }

    void ZoomOut()
    {
        m_zoom /= 1.5;
        if (m_zoom < 0.1) m_zoom = 0.1;
        Invalidate();
    }

    void Pan(int dx, int dy)
    {
        CClientDC dc(this);
        dc.SetMapMode(MM_ANISOTROPIC);

        CPoint delta(dx, dy);
        dc.DPtoLP(&delta);

        m_center.x += delta.x;
        m_center.y += delta.y;

        Invalidate();
    }
};
```

### Print Preview:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    BOOL bPrinting = pDC->IsPrinting();

    if (bPrinting)
    {
        // Printer: use MM_LOENGLISH for precise sizing
        pDC->SetMapMode(MM_LOENGLISH);

        // 8.5" x 11" page
        int pageWidth = 850;   // 8.5 inches
        int pageHeight = 1100; // 11 inches

        // 0.5" margins
        int margin = 50;

        CRect printRect(margin, -margin,
                       pageWidth - margin,
                       -(pageHeight - margin));

        DrawDocument(pDC, printRect);
    }
    else
    {
        // Screen: use MM_TEXT
        pDC->SetMapMode(MM_TEXT);

        CRect rect;
        GetClientRect(&rect);

        DrawDocument(pDC, rect);
    }
}

void CMyView::DrawDocument(CDC* pDC, CRect rect)
{
    // Draw content that works for both screen and printer
    pDC->Rectangle(&rect);

    // Draw title
    CFont font;
    if (pDC->IsPrinting())
        font.CreatePointFont(120, _T("Arial"), pDC);
    else
        font.CreatePointFont(120, _T("Arial"));

    CFont* pOldFont = pDC->SelectObject(&font);
    pDC->DrawText(_T("Document Title"), &rect,
                  DT_CENTER | DT_TOP | DT_SINGLELINE);
    pDC->SelectObject(pOldFont);
}
```

---

## Key Takeaways

1. ✅ MM_TEXT for pixel-based graphics
2. ✅ MM_LOMETRIC/MM_HIMETRIC for metric units
3. ✅ MM_LOENGLISH/MM_HIENGLISH for imperial units
4. ✅ MM_ISOTROPIC maintains aspect ratio
5. ✅ MM_ANISOTROPIC allows independent scaling
6. ✅ LPtoDP converts logical to device coords
7. ✅ DPtoLP converts device to logical coords
8. ✅ Viewport/Window origins control positioning

---

## Next Lesson Preview

In **Lesson 55: Printing Basics**, we'll learn:
- Print document architecture
- Starting print jobs
- Basic printing operations
- Page setup

**Congratulations on completing Lesson 54!**
