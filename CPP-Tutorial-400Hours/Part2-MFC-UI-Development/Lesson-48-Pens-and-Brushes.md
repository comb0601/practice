# Lesson 48: Pens and Brushes

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding Pens
2. CPen Class Deep Dive
3. Pen Styles and Patterns
4. Understanding Brushes
5. CBrush Class Deep Dive
6. Brush Types and Patterns
7. Stock Pens and Brushes
8. Custom Patterns
9. Advanced Techniques
10. Performance Considerations
11. Exercises

---

## 1. Understanding Pens

A **Pen** is a GDI object used for drawing lines and shape outlines. Think of it as the tool that controls line appearance.

### Pen Attributes:

```
┌─────────────────────────────┐
│          PEN                │
├─────────────────────────────┤
│  Style:   _____ or •••••    │
│  Width:   1, 2, 3... pixels │
│  Color:   RGB(r, g, b)      │
│  EndCap:  □ ● ■             │
│  Join:    ∟ ⌐ ⌙             │
└─────────────────────────────┘
```

### Basic Pen Concepts:

```cpp
// Create a pen
CPen pen(PS_SOLID,          // Style
         2,                 // Width (pixels)
         RGB(255, 0, 0));   // Color (red)

// Use the pen
CDC* pDC = GetDC();
CPen* pOldPen = pDC->SelectObject(&pen);

// Draw
pDC->MoveTo(10, 10);
pDC->LineTo(100, 100);

// Restore
pDC->SelectObject(pOldPen);
ReleaseDC(pDC);
```

### What Pens Draw:

- ✅ Lines (MoveTo/LineTo)
- ✅ Shape outlines (Rectangle, Ellipse, etc.)
- ✅ Polylines and polygons
- ✅ Arcs and curves
- ❌ NOT used for filling shapes (that's brushes)

---

## 2. CPen Class Deep Dive

### CPen Class Structure:

```cpp
class CPen : public CGdiObject
{
public:
    // Constructors
    CPen();
    CPen(int nPenStyle, int nWidth, COLORREF crColor);
    CPen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,
         int nStyleCount = 0, const DWORD* lpStyle = NULL);

    // Creation methods
    BOOL CreatePen(int nPenStyle, int nWidth, COLORREF crColor);
    BOOL CreatePenIndirect(LPLOGPEN lpLogPen);

    // Operators
    operator HPEN() const;

    // Attributes
    int GetLogPen(LOGPEN* pLogPen);
    int GetExtLogPen(EXTLOGPEN* pLogPen);
};
```

### Constructor Usage:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Method 1: Constructor with parameters
    CPen pen1(PS_SOLID, 2, RGB(255, 0, 0));

    // Method 2: Default constructor + CreatePen
    CPen pen2;
    pen2.CreatePen(PS_DASH, 1, RGB(0, 255, 0));

    // Method 3: Using LOGPEN structure
    LOGPEN logPen;
    logPen.lopnStyle = PS_DOT;
    logPen.lopnWidth.x = 1;
    logPen.lopnWidth.y = 0;
    logPen.lopnColor = RGB(0, 0, 255);

    CPen pen3;
    pen3.CreatePenIndirect(&logPen);

    // Use pens
    CPen* pOldPen = pDC->SelectObject(&pen1);
    pDC->MoveTo(10, 10);
    pDC->LineTo(200, 10);

    pDC->SelectObject(&pen2);
    pDC->MoveTo(10, 30);
    pDC->LineTo(200, 30);

    pDC->SelectObject(&pen3);
    pDC->MoveTo(10, 50);
    pDC->LineTo(200, 50);

    pDC->SelectObject(pOldPen);
}
```

---

## 3. Pen Styles and Patterns

### Standard Pen Styles:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    int y = 20;
    int spacing = 30;

    // PS_SOLID - solid line
    CPen penSolid(PS_SOLID, 2, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&penSolid);
    pDC->MoveTo(50, y);
    pDC->LineTo(400, y);
    pDC->TextOut(420, y - 5, _T("PS_SOLID"));
    y += spacing;

    // PS_DASH - dashed line
    CPen penDash(PS_DASH, 1, RGB(0, 0, 0));
    pDC->SelectObject(&penDash);
    pDC->MoveTo(50, y);
    pDC->LineTo(400, y);
    pDC->TextOut(420, y - 5, _T("PS_DASH"));
    y += spacing;

    // PS_DOT - dotted line
    CPen penDot(PS_DOT, 1, RGB(0, 0, 0));
    pDC->SelectObject(&penDot);
    pDC->MoveTo(50, y);
    pDC->LineTo(400, y);
    pDC->TextOut(420, y - 5, _T("PS_DOT"));
    y += spacing;

    // PS_DASHDOT - dash-dot pattern
    CPen penDashDot(PS_DASHDOT, 1, RGB(0, 0, 0));
    pDC->SelectObject(&penDashDot);
    pDC->MoveTo(50, y);
    pDC->LineTo(400, y);
    pDC->TextOut(420, y - 5, _T("PS_DASHDOT"));
    y += spacing;

    // PS_DASHDOTDOT - dash-dot-dot pattern
    CPen penDashDotDot(PS_DASHDOTDOT, 1, RGB(0, 0, 0));
    pDC->SelectObject(&penDashDotDot);
    pDC->MoveTo(50, y);
    pDC->LineTo(400, y);
    pDC->TextOut(420, y - 5, _T("PS_DASHDOTDOT"));
    y += spacing;

    // PS_NULL - invisible pen (no drawing)
    CPen penNull(PS_NULL, 0, RGB(0, 0, 0));
    pDC->SelectObject(&penNull);
    pDC->MoveTo(50, y);
    pDC->LineTo(400, y);  // Nothing drawn
    pDC->TextOut(420, y - 5, _T("PS_NULL (invisible)"));
    y += spacing;

    // PS_INSIDEFRAME - stays inside shape bounds
    CPen penInside(PS_INSIDEFRAME, 10, RGB(255, 0, 0));
    pDC->SelectObject(&penInside);
    pDC->Rectangle(50, y, 150, y + 50);
    pDC->TextOut(420, y + 15, _T("PS_INSIDEFRAME"));

    pDC->SelectObject(pOldPen);
}
```

### Important Notes on Styles:

**Width Restrictions:**
- ⚠️ PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT only work with width = 1
- ⚠️ Wider pens automatically become PS_SOLID
- ✅ Use PS_GEOMETRIC for styled wide pens

```cpp
// This won't work as expected
CPen badPen(PS_DASH, 5, RGB(0, 0, 0));  // Becomes solid!

// Use PS_GEOMETRIC instead
LOGBRUSH lb;
lb.lbStyle = BS_SOLID;
lb.lbColor = RGB(0, 0, 0);
CPen goodPen(PS_GEOMETRIC | PS_DASH, 5, &lb);
```

### Geometric Pens (Wide Styled Pens):

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Create geometric pen for wide dashed line
    LOGBRUSH lb;
    lb.lbStyle = BS_SOLID;
    lb.lbColor = RGB(255, 0, 0);
    lb.lbHatch = 0;

    CPen pen(PS_GEOMETRIC | PS_DASH | PS_ENDCAP_ROUND | PS_JOIN_ROUND,
             5,  // 5 pixels wide
             &lb);

    CPen* pOldPen = pDC->SelectObject(&pen);

    pDC->MoveTo(50, 50);
    pDC->LineTo(400, 50);

    pDC->SelectObject(pOldPen);
}
```

### Custom Dash Patterns:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Custom dash pattern: 10 on, 2 off, 2 on, 2 off
    DWORD dashPattern[] = {10, 2, 2, 2};

    LOGBRUSH lb;
    lb.lbStyle = BS_SOLID;
    lb.lbColor = RGB(0, 0, 255);
    lb.lbHatch = 0;

    CPen pen(PS_GEOMETRIC | PS_USERSTYLE,
             3,
             &lb,
             4,              // Number of pattern elements
             dashPattern);   // Pattern array

    CPen* pOldPen = pDC->SelectObject(&pen);

    pDC->MoveTo(50, 100);
    pDC->LineTo(400, 100);

    pDC->SelectObject(pOldPen);
}
```

### End Cap Styles:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    LOGBRUSH lb;
    lb.lbStyle = BS_SOLID;
    lb.lbColor = RGB(0, 0, 0);

    int y = 50;

    // Flat end caps (default)
    CPen penFlat(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT, 20, &lb);
    CPen* pOldPen = pDC->SelectObject(&penFlat);
    pDC->MoveTo(100, y);
    pDC->LineTo(300, y);
    pDC->TextOut(320, y - 10, _T("PS_ENDCAP_FLAT"));
    y += 50;

    // Round end caps
    CPen penRound(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_ROUND, 20, &lb);
    pDC->SelectObject(&penRound);
    pDC->MoveTo(100, y);
    pDC->LineTo(300, y);
    pDC->TextOut(320, y - 10, _T("PS_ENDCAP_ROUND"));
    y += 50;

    // Square end caps (extends half pen width)
    CPen penSquare(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE, 20, &lb);
    pDC->SelectObject(&penSquare);
    pDC->MoveTo(100, y);
    pDC->LineTo(300, y);
    pDC->TextOut(320, y - 10, _T("PS_ENDCAP_SQUARE"));

    pDC->SelectObject(pOldPen);
}
```

### Join Styles:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    LOGBRUSH lb;
    lb.lbStyle = BS_SOLID;
    lb.lbColor = RGB(255, 0, 0);

    POINT points[3];

    // Bevel join
    CPen penBevel(PS_GEOMETRIC | PS_SOLID | PS_JOIN_BEVEL, 15, &lb);
    CPen* pOldPen = pDC->SelectObject(&penBevel);

    points[0] = {50, 150};
    points[1] = {150, 50};
    points[2] = {250, 150};
    pDC->Polyline(points, 3);
    pDC->TextOut(50, 160, _T("PS_JOIN_BEVEL"));

    // Miter join (pointed)
    CPen penMiter(PS_GEOMETRIC | PS_SOLID | PS_JOIN_MITER, 15, &lb);
    pDC->SelectObject(&penMiter);

    points[0] = {50, 250};
    points[1] = {150, 150};
    points[2] = {250, 250};
    pDC->Polyline(points, 3);
    pDC->TextOut(50, 260, _T("PS_JOIN_MITER"));

    // Round join
    CPen penRound(PS_GEOMETRIC | PS_SOLID | PS_JOIN_ROUND, 15, &lb);
    pDC->SelectObject(&penRound);

    points[0] = {50, 350};
    points[1] = {150, 250};
    points[2] = {250, 350};
    pDC->Polyline(points, 3);
    pDC->TextOut(50, 360, _T("PS_JOIN_ROUND"));

    pDC->SelectObject(pOldPen);
}
```

---

## 4. Understanding Brushes

A **Brush** is a GDI object used for filling closed shapes and backgrounds. It defines the pattern and color used to fill areas.

### Brush Concepts:

```
┌─────────────────────────────┐
│         BRUSH               │
├─────────────────────────────┤
│  Type:    Solid / Hatch /   │
│           Pattern / Gradient│
│  Color:   RGB(r, g, b)      │
│  Pattern: Bitmap / Hatch    │
└─────────────────────────────┘
```

### What Brushes Fill:

- ✅ Interior of closed shapes (Rectangle, Ellipse, Polygon)
- ✅ Background of text (when not transparent)
- ✅ Window backgrounds
- ✅ FloodFill operations
- ❌ NOT used for drawing lines (that's pens)

---

## 5. CBrush Class Deep Dive

### CBrush Class Structure:

```cpp
class CBrush : public CGdiObject
{
public:
    // Constructors
    CBrush();
    CBrush(COLORREF crColor);
    CBrush(int nIndex, COLORREF crColor);
    CBrush(CBitmap* pBitmap);

    // Creation methods
    BOOL CreateSolidBrush(COLORREF crColor);
    BOOL CreateHatchBrush(int nIndex, COLORREF crColor);
    BOOL CreatePatternBrush(CBitmap* pBitmap);
    BOOL CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage);
    BOOL CreateBrushIndirect(const LOGBRUSH* lpLogBrush);

    // Operators
    operator HBRUSH() const;

    // Attributes
    int GetLogBrush(LOGBRUSH* pLogBrush);
};
```

### Basic Brush Usage:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Solid brush
    CBrush brushRed(RGB(255, 0, 0));

    CPen* pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
    CBrush* pOldBrush = pDC->SelectObject(&brushRed);

    pDC->Rectangle(50, 50, 150, 150);

    // Change brush
    CBrush brushBlue(RGB(0, 0, 255));
    pDC->SelectObject(&brushBlue);

    pDC->Ellipse(200, 50, 300, 150);

    // Restore
    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}
```

---

## 6. Brush Types and Patterns

### 6.1 Solid Brushes

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Method 1: Constructor
    CBrush brush1(RGB(255, 200, 200));

    // Method 2: CreateSolidBrush
    CBrush brush2;
    brush2.CreateSolidBrush(RGB(200, 255, 200));

    // Use brushes
    CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);
    CBrush* pOldBrush = pDC->SelectObject(&brush1);

    pDC->Rectangle(50, 50, 150, 150);

    pDC->SelectObject(&brush2);
    pDC->Rectangle(200, 50, 300, 150);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}
```

### 6.2 Hatch Brushes

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    int x = 50, y = 50;
    int size = 80;
    int spacing = 100;

    CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);

    // HS_HORIZONTAL
    CBrush brushHoriz(HS_HORIZONTAL, RGB(255, 0, 0));
    CBrush* pOldBrush = pDC->SelectObject(&brushHoriz);
    pDC->Rectangle(x, y, x + size, y + size);
    pDC->TextOut(x, y + size + 5, _T("HORIZONTAL"));

    // HS_VERTICAL
    x += spacing;
    CBrush brushVert(HS_VERTICAL, RGB(0, 255, 0));
    pDC->SelectObject(&brushVert);
    pDC->Rectangle(x, y, x + size, y + size);
    pDC->TextOut(x, y + size + 5, _T("VERTICAL"));

    // HS_FDIAGONAL (45 degrees)
    x += spacing;
    CBrush brushFDiag(HS_FDIAGONAL, RGB(0, 0, 255));
    pDC->SelectObject(&brushFDiag);
    pDC->Rectangle(x, y, x + size, y + size);
    pDC->TextOut(x, y + size + 5, _T("FDIAGONAL"));

    // HS_BDIAGONAL (135 degrees)
    x += spacing;
    CBrush brushBDiag(HS_BDIAGONAL, RGB(255, 0, 255));
    pDC->SelectObject(&brushBDiag);
    pDC->Rectangle(x, y, x + size, y + size);
    pDC->TextOut(x, y + size + 5, _T("BDIAGONAL"));

    // Row 2
    x = 50;
    y += 120;

    // HS_CROSS
    CBrush brushCross(HS_CROSS, RGB(255, 128, 0));
    pDC->SelectObject(&brushCross);
    pDC->Rectangle(x, y, x + size, y + size);
    pDC->TextOut(x, y + size + 5, _T("CROSS"));

    // HS_DIAGCROSS
    x += spacing;
    CBrush brushDiagCross(HS_DIAGCROSS, RGB(128, 0, 255));
    pDC->SelectObject(&brushDiagCross);
    pDC->Rectangle(x, y, x + size, y + size);
    pDC->TextOut(x, y + size + 5, _T("DIAGCROSS"));

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}
```

### 6.3 Pattern Brushes (Bitmap Brushes)

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Create a small bitmap for pattern
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);

    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(pDC, 8, 8);
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    // Draw pattern (8x8 checkerboard)
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if ((x + y) % 2 == 0)
                memDC.SetPixel(x, y, RGB(255, 0, 0));
            else
                memDC.SetPixel(x, y, RGB(255, 255, 0));
        }
    }

    memDC.SelectObject(pOldBitmap);

    // Create pattern brush
    CBrush patternBrush(&bitmap);

    // Use pattern brush
    CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);
    CBrush* pOldBrush = pDC->SelectObject(&patternBrush);

    pDC->Rectangle(50, 50, 250, 250);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}
```

### 6.4 NULL Brush (Hollow Brush)

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 3, RGB(255, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);

    // Select NULL brush (no fill)
    CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

    // Outline only, no fill
    pDC->Rectangle(50, 50, 200, 200);
    pDC->Ellipse(250, 50, 400, 200);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}
```

---

## 7. Stock Pens and Brushes

Windows provides pre-created GDI objects called **stock objects** that don't need to be deleted.

### Stock Pens:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    int y = 20;

    // BLACK_PEN
    CPen* pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
    pDC->MoveTo(50, y);
    pDC->LineTo(200, y);
    pDC->TextOut(220, y - 5, _T("BLACK_PEN"));
    y += 30;

    // WHITE_PEN
    pDC->SelectStockObject(WHITE_PEN);
    pDC->MoveTo(50, y);
    pDC->LineTo(200, y);
    pDC->TextOut(220, y - 5, _T("WHITE_PEN (on white bg)"));
    y += 30;

    // NULL_PEN (invisible)
    pDC->SelectStockObject(NULL_PEN);
    pDC->MoveTo(50, y);
    pDC->LineTo(200, y);  // Nothing drawn
    pDC->TextOut(220, y - 5, _T("NULL_PEN (invisible)"));
    y += 30;

    pDC->SelectObject(pOldPen);

    // Using stock pen with shapes
    pDC->SelectStockObject(BLACK_PEN);
    CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(GRAY_BRUSH);

    pDC->Rectangle(50, y, 150, y + 80);

    pDC->SelectObject(pOldBrush);
}
```

### Stock Brushes:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);

    int x = 50, y = 50;
    int size = 70;
    int spacing = 90;

    // WHITE_BRUSH
    CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(WHITE_BRUSH);
    pDC->Rectangle(x, y, x + size, y + size);
    pDC->TextOut(x, y + size + 5, _T("WHITE"));

    // LTGRAY_BRUSH
    x += spacing;
    pDC->SelectStockObject(LTGRAY_BRUSH);
    pDC->Rectangle(x, y, x + size, y + size);
    pDC->TextOut(x, y + size + 5, _T("LTGRAY"));

    // GRAY_BRUSH
    x += spacing;
    pDC->SelectStockObject(GRAY_BRUSH);
    pDC->Rectangle(x, y, x + size, y + size);
    pDC->TextOut(x, y + size + 5, _T("GRAY"));

    // DKGRAY_BRUSH
    x += spacing;
    pDC->SelectStockObject(DKGRAY_BRUSH);
    pDC->Rectangle(x, y, x + size, y + size);
    pDC->SetTextColor(RGB(255, 255, 255));
    pDC->TextOut(x, y + size + 5, _T("DKGRAY"));

    // BLACK_BRUSH
    x += spacing;
    pDC->SelectStockObject(BLACK_BRUSH);
    pDC->Rectangle(x, y, x + size, y + size);
    pDC->TextOut(x, y + size + 5, _T("BLACK"));

    // NULL_BRUSH (hollow)
    x = 50;
    y += 120;
    pDC->SetTextColor(RGB(0, 0, 0));
    pDC->SelectStockObject(NULL_BRUSH);
    pDC->Rectangle(x, y, x + size, y + size);
    pDC->TextOut(x, y + size + 5, _T("NULL (hollow)"));

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}
```

### Advantages of Stock Objects:

- ✅ Always available
- ✅ No creation needed
- ✅ No deletion needed
- ✅ Slight performance benefit
- ⚠️ Limited selection (only basic colors)

---

## 8. Custom Patterns

### Creating Complex Patterns:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Create pattern bitmap (16x16)
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);

    CBitmap pattern;
    pattern.CreateCompatibleBitmap(pDC, 16, 16);
    CBitmap* pOldBitmap = memDC.SelectObject(&pattern);

    // Fill with background
    memDC.FillSolidRect(0, 0, 16, 16, RGB(255, 255, 200));

    // Draw pattern elements
    CPen pen(PS_SOLID, 1, RGB(200, 0, 0));
    CPen* pOldPen = memDC.SelectObject(&pen);

    // Draw some lines
    memDC.MoveTo(0, 0);
    memDC.LineTo(16, 16);
    memDC.MoveTo(0, 16);
    memDC.LineTo(16, 0);

    // Draw dots
    for (int i = 0; i < 16; i += 4)
    {
        memDC.SetPixel(i, 8, RGB(0, 0, 200));
        memDC.SetPixel(8, i, RGB(0, 0, 200));
    }

    memDC.SelectObject(pOldPen);
    memDC.SelectObject(pOldBitmap);

    // Create and use pattern brush
    CBrush patternBrush(&pattern);
    CBrush* pOldBrush = pDC->SelectObject(&patternBrush);

    pDC->SelectStockObject(BLACK_PEN);
    pDC->Ellipse(50, 50, 300, 300);

    pDC->SelectObject(pOldBrush);
}
```

### Gradient Simulation with Brushes:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    int steps = 100;
    int barHeight = rect.Height() / steps;

    for (int i = 0; i < steps; i++)
    {
        int red = (i * 255) / steps;
        int blue = 255 - red;

        CBrush brush(RGB(red, 100, blue));
        CRect bar(0, i * barHeight, rect.Width(), (i + 1) * barHeight);
        pDC->FillRect(&bar, &brush);
    }

    // Note: This is inefficient. Use GDI+ for real gradients.
}
```

---

## 9. Advanced Techniques

### 9.1 Pen and Brush Alignment

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Thick pen demonstration
    CPen thickPen(PS_SOLID, 20, RGB(255, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&thickPen);
    CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

    // Rectangle with thick border
    // Note: Border drawn half inside, half outside logical rectangle
    pDC->Rectangle(50, 50, 200, 200);

    // Use PS_INSIDEFRAME to keep border inside
    CPen insidePen(PS_INSIDEFRAME, 20, RGB(0, 0, 255));
    pDC->SelectObject(&insidePen);
    pDC->Rectangle(250, 50, 400, 200);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);

    // Draw labels
    pDC->SetBkMode(TRANSPARENT);
    pDC->TextOut(50, 210, _T("PS_SOLID (centered)"));
    pDC->TextOut(250, 210, _T("PS_INSIDEFRAME"));
}
```

### 9.2 Brush Origin

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Create hatch brush
    CBrush brush(HS_DIAGCROSS, RGB(255, 0, 0));

    CPen* pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    // Default brush origin
    pDC->Rectangle(50, 50, 150, 150);

    // Set brush origin
    pDC->SetBrushOrg(4, 4);
    pDC->Rectangle(200, 50, 300, 150);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);

    // Labels
    pDC->SetBkMode(TRANSPARENT);
    pDC->TextOut(50, 160, _T("Default origin"));
    pDC->TextOut(200, 160, _T("Offset origin"));
}
```

### 9.3 Combining Pens and Brushes

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Create coordinated pen and brush
    COLORREF color = RGB(100, 150, 255);

    CPen pen(PS_SOLID, 3, color);
    CBrush brush(HS_CROSS, color);

    CPen* pOldPen = pDC->SelectObject(&pen);
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    // Draw coordinated shapes
    pDC->Rectangle(50, 50, 200, 200);
    pDC->Ellipse(250, 50, 400, 200);

    POINT poly[] = {
        {450, 50}, {550, 50}, {600, 200}, {400, 200}
    };
    pDC->Polygon(poly, 4);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}
```

---

## 10. Performance Considerations

### Efficient Pen/Brush Usage:

```cpp
// ✅ GOOD - Create once, reuse
class CMyView : public CView
{
protected:
    CPen m_pen;
    CBrush m_brush;
    BOOL m_bInitialized;

public:
    CMyView() : m_bInitialized(FALSE) {}

    void OnDraw(CDC* pDC) override
    {
        if (!m_bInitialized)
        {
            m_pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
            m_brush.CreateSolidBrush(RGB(200, 200, 255));
            m_bInitialized = TRUE;
        }

        CPen* pOldPen = pDC->SelectObject(&m_pen);
        CBrush* pOldBrush = pDC->SelectObject(&m_brush);

        // Draw many shapes with same pen/brush
        for (int i = 0; i < 100; i++)
        {
            pDC->Rectangle(i * 10, i * 10, i * 10 + 50, i * 10 + 50);
        }

        pDC->SelectObject(pOldPen);
        pDC->SelectObject(pOldBrush);
    }
};

// ❌ BAD - Create in loop
void BadDraw(CDC* pDC)
{
    for (int i = 0; i < 100; i++)
    {
        CPen pen(PS_SOLID, 2, RGB(0, 0, 0));  // Created 100 times!
        CBrush brush(RGB(200, 200, 255));     // Created 100 times!

        CPen* pOldPen = pDC->SelectObject(&pen);
        CBrush* pOldBrush = pDC->SelectObject(&brush);

        pDC->Rectangle(i * 10, i * 10, i * 10 + 50, i * 10 + 50);

        pDC->SelectObject(pOldPen);
        pDC->SelectObject(pOldBrush);
    }
}
```

### Using Stock Objects When Possible:

```cpp
// ✅ GOOD - Use stock objects for common cases
void FastDraw(CDC* pDC)
{
    CPen* pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
    CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(WHITE_BRUSH);

    for (int i = 0; i < 1000; i++)
    {
        pDC->Rectangle(i, i, i + 10, i + 10);
    }

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}
```

---

## 11. Exercises

### Exercise 1: Pen Style Viewer

Create an application that displays all pen styles:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    struct PenStyle {
        int style;
        LPCTSTR name;
    };

    PenStyle styles[] = {
        {PS_SOLID, _T("PS_SOLID")},
        {PS_DASH, _T("PS_DASH")},
        {PS_DOT, _T("PS_DOT")},
        {PS_DASHDOT, _T("PS_DASHDOT")},
        {PS_DASHDOTDOT, _T("PS_DASHDOTDOT")},
        {PS_NULL, _T("PS_NULL")},
    };

    int y = 30;
    for (int i = 0; i < _countof(styles); i++)
    {
        CPen pen(styles[i].style, 2, RGB(0, 0, 0));
        CPen* pOldPen = pDC->SelectObject(&pen);

        pDC->MoveTo(50, y);
        pDC->LineTo(300, y);

        pDC->SelectObject(pOldPen);

        pDC->TextOut(320, y - 7, styles[i].name);

        y += 40;
    }
}
```

### Exercise 2: Brush Pattern Gallery

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    struct HatchStyle {
        int style;
        LPCTSTR name;
    };

    HatchStyle hatches[] = {
        {HS_HORIZONTAL, _T("HORIZONTAL")},
        {HS_VERTICAL, _T("VERTICAL")},
        {HS_FDIAGONAL, _T("FDIAGONAL")},
        {HS_BDIAGONAL, _T("BDIAGONAL")},
        {HS_CROSS, _T("CROSS")},
        {HS_DIAGCROSS, _T("DIAGCROSS")},
    };

    int x = 50, y = 50;
    int size = 100;
    int cols = 3;

    for (int i = 0; i < _countof(hatches); i++)
    {
        CBrush brush(hatches[i].style, RGB(0, 100, 200));
        CBrush* pOldBrush = pDC->SelectObject(&brush);
        CPen* pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);

        pDC->Rectangle(x, y, x + size, y + size);

        pDC->SelectObject(pOldPrush);
        pDC->SelectObject(pOldPen);

        pDC->TextOut(x, y + size + 5, hatches[i].name);

        x += size + 20;
        if ((i + 1) % cols == 0)
        {
            x = 50;
            y += size + 40;
        }
    }
}
```

### Exercise 3: Custom Pattern Creator

```cpp
CBitmap CreateDotPattern(int spacing, COLORREF dotColor, COLORREF bgColor)
{
    CDC memDC;
    CClientDC dc(AfxGetMainWnd());
    memDC.CreateCompatibleDC(&dc);

    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(&dc, spacing, spacing);
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    // Fill background
    memDC.FillSolidRect(0, 0, spacing, spacing, bgColor);

    // Draw dot in center
    int center = spacing / 2;
    memDC.SetPixel(center, center, dotColor);

    memDC.SelectObject(pOldBitmap);

    return bitmap;
}

void CMyView::OnDraw(CDC* pDC)
{
    CBitmap pattern = CreateDotPattern(8, RGB(255, 0, 0), RGB(255, 255, 200));
    CBrush brush(&pattern);

    CBrush* pOldBrush = pDC->SelectObject(&brush);
    CPen* pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);

    pDC->Ellipse(50, 50, 300, 300);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}
```

---

## Key Takeaways

1. ✅ Pens draw lines and shape outlines
2. ✅ Brushes fill closed shapes
3. ✅ Stock objects don't need deletion
4. ✅ Geometric pens allow wide styled lines
5. ✅ Pattern brushes use bitmaps for custom fills
6. ✅ PS_INSIDEFRAME keeps borders inside bounds
7. ✅ Always restore original objects
8. ✅ Create objects once and reuse for performance

---

## Common Mistakes

### ❌ Using Dashed Pen with Width > 1:
```cpp
CPen pen(PS_DASH, 5, RGB(0, 0, 0));  // Becomes solid!
```

### ❌ Forgetting to Restore:
```cpp
void Bad(CDC* pDC)
{
    CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
    pDC->SelectObject(&pen);
    // pen destroyed while selected - CRASH!
}
```

### ❌ Deleting Stock Objects:
```cpp
CPen* pPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
delete pPen;  // WRONG! Stock objects are not deleted
```

---

## Summary Checklist

Before moving to Lesson 49, ensure you can:
- [ ] Create and use pens with different styles
- [ ] Create and use brushes (solid, hatch, pattern)
- [ ] Work with stock pens and brushes
- [ ] Create custom patterns
- [ ] Understand pen and brush attributes
- [ ] Use geometric pens for wide styled lines
- [ ] Combine pens and brushes effectively

---

## Next Lesson Preview

In **Lesson 49: Drawing Lines and Shapes**, we'll learn:
- MoveTo/LineTo for line drawing
- Rectangle, Ellipse, Polygon
- Polylines and Bezier curves
- Arcs, chords, and pie slices
- Complex shape construction

**Congratulations on completing Lesson 48!**
