# Lesson 49: Drawing Lines and Shapes

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Line Drawing Fundamentals
2. MoveTo and LineTo
3. Polylines and Polygons
4. Drawing Rectangles
5. Drawing Ellipses and Circles
6. Arcs, Chords, and Pies
7. Bezier Curves and Splines
8. Round Rectangles
9. Complex Shape Construction
10. Practical Examples
11. Exercises

---

## 1. Line Drawing Fundamentals

Lines are the most basic drawing primitive in GDI. Understanding line drawing is essential for all graphics programming.

### Current Position Concept:

Every Device Context maintains a **current position** - the starting point for the next line operation.

```
┌───────────────────────────────┐
│  Device Context               │
│                               │
│  Current Position: (100, 50)  │
│        ↓                      │
│        •─────────────> LineTo │
│      (100,50)      (200,50)   │
└───────────────────────────────┘
```

### Basic Line Drawing:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Set current position (doesn't draw anything)
    pDC->MoveTo(50, 50);

    // Draw line from current position to (200, 50)
    pDC->LineTo(200, 50);
    // Current position now at (200, 50)

    // Draw another line from (200, 50) to (200, 200)
    pDC->LineTo(200, 200);
    // Current position now at (200, 200)

    // Draw back to start
    pDC->LineTo(50, 200);
    pDC->LineTo(50, 50);
}
```

---

## 2. MoveTo and LineTo

### MoveTo - Set Current Position:

```cpp
CPoint MoveTo(int x, int y);
CPoint MoveTo(POINT point);
```

**Returns**: Previous current position

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);

    // Get return value (previous position)
    CPoint oldPos = pDC->MoveTo(100, 100);

    // oldPos contains previous current position
    CString info;
    info.Format(_T("Previous position: (%d, %d)"), oldPos.x, oldPos.y);
    pDC->TextOut(10, 10, info);

    pDC->LineTo(300, 100);

    pDC->SelectObject(pOldPen);
}
```

### LineTo - Draw Line:

```cpp
BOOL LineTo(int x, int y);
BOOL LineTo(POINT point);
```

**Behavior**:
- Draws from current position to specified point
- Updates current position to end point
- Uses currently selected pen

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 3, RGB(0, 0, 255));
    CPen* pOldPen = pDC->SelectObject(&pen);

    // Draw connected lines
    pDC->MoveTo(50, 50);
    pDC->LineTo(150, 50);   // Horizontal
    pDC->LineTo(200, 100);  // Diagonal
    pDC->LineTo(150, 150);  // Diagonal
    pDC->LineTo(50, 150);   // Horizontal
    pDC->LineTo(50, 50);    // Close the shape

    pDC->SelectObject(pOldPen);
}
```

### Drawing Patterns:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);

    // Draw grid
    for (int i = 0; i <= 10; i++)
    {
        // Vertical lines
        pDC->MoveTo(i * 50, 0);
        pDC->LineTo(i * 50, 500);

        // Horizontal lines
        pDC->MoveTo(0, i * 50);
        pDC->LineTo(500, i * 50);
    }

    // Draw diagonal pattern
    CPen redPen(PS_SOLID, 2, RGB(255, 0, 0));
    pDC->SelectObject(&redPen);

    for (int i = 0; i <= 10; i++)
    {
        pDC->MoveTo(i * 50, 0);
        pDC->LineTo(500, i * 50);
    }

    pDC->SelectObject(pOldPen);
}
```

---

## 3. Polylines and Polygons

### Polyline - Multiple Connected Lines:

```cpp
BOOL Polyline(const POINT* lpPoints, int nCount);
```

**Features**:
- Draws multiple connected line segments
- More efficient than multiple LineTo calls
- Not filled (even if closed)

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 2, RGB(0, 0, 255));
    CPen* pOldPen = pDC->SelectObject(&pen);

    // Define points
    POINT points[] = {
        {100, 50},
        {150, 100},
        {200, 50},
        {250, 100},
        {200, 150},
        {150, 100},
        {100, 150}
    };

    // Draw polyline
    pDC->Polyline(points, 7);

    pDC->SelectObject(pOldPen);

    // Draw point markers
    CPen dotPen(PS_SOLID, 5, RGB(255, 0, 0));
    pDC->SelectObject(&dotPen);

    for (int i = 0; i < 7; i++)
    {
        pDC->SetPixel(points[i].x, points[i].y, RGB(255, 0, 0));
    }

    pDC->SelectObject(pOldPen);
}
```

### Polygon - Closed and Filled Shape:

```cpp
BOOL Polygon(const POINT* lpPoints, int nCount);
```

**Features**:
- Automatically closes the shape
- Fills interior with current brush
- Outlines with current pen

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 2, RGB(0, 0, 128));
    CBrush brush(RGB(200, 200, 255));

    CPen* pOldPen = pDC->SelectObject(&pen);
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    // Draw pentagon
    POINT pentagon[5];
    double angleStep = 2 * 3.14159 / 5;
    int centerX = 150, centerY = 150;
    int radius = 80;

    for (int i = 0; i < 5; i++)
    {
        double angle = i * angleStep - 3.14159 / 2;  // Start at top
        pentagon[i].x = centerX + (int)(radius * cos(angle));
        pentagon[i].y = centerY + (int)(radius * sin(angle));
    }

    pDC->Polygon(pentagon, 5);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);

    pDC->SetBkMode(TRANSPARENT);
    pDC->TextOut(110, 250, _T("Regular Pentagon"));
}
```

### PolyPolyline - Multiple Polylines:

```cpp
BOOL PolyPolyline(const POINT* lpPoints,
                  const DWORD* lpPolyPoints,
                  int nCount);
```

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);

    // Define multiple polylines
    POINT points[] = {
        // First polyline (triangle)
        {50, 50}, {100, 100}, {150, 50},
        // Second polyline (square)
        {200, 50}, {250, 50}, {250, 100}, {200, 100}
    };

    DWORD polyCounts[] = {3, 4};  // 3 points, then 4 points

    pDC->PolyPolyline(points, polyCounts, 2);

    pDC->SelectObject(pOldPen);
}
```

---

## 4. Drawing Rectangles

### Rectangle - Basic Rectangle:

```cpp
BOOL Rectangle(int x1, int y1, int x2, int y2);
BOOL Rectangle(LPCRECT lpRect);
```

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
    CBrush brush(RGB(255, 200, 200));

    CPen* pOldPen = pDC->SelectObject(&pen);
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    // Method 1: Coordinates
    pDC->Rectangle(50, 50, 200, 150);

    // Method 2: CRect
    CRect rect(250, 50, 400, 150);
    pDC->Rectangle(&rect);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}
```

### FillRect - Fill Without Border:

```cpp
int FillRect(LPCRECT lpRect, CBrush* pBrush);
```

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CBrush brush1(RGB(255, 200, 200));
    CBrush brush2(RGB(200, 255, 200));
    CBrush brush3(RGB(200, 200, 255));

    CRect rect1(50, 50, 150, 150);
    CRect rect2(170, 50, 270, 150);
    CRect rect3(290, 50, 390, 150);

    pDC->FillRect(&rect1, &brush1);
    pDC->FillRect(&rect2, &brush2);
    pDC->FillRect(&rect3, &brush3);

    // No border drawn
}
```

### FillSolidRect - Quick Fill:

```cpp
void FillSolidRect(LPCRECT lpRect, COLORREF clr);
void FillSolidRect(int x, int y, int cx, int cy, COLORREF clr);
```

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Very efficient for solid colors
    pDC->FillSolidRect(50, 50, 100, 100, RGB(255, 0, 0));
    pDC->FillSolidRect(170, 50, 100, 100, RGB(0, 255, 0));
    pDC->FillSolidRect(290, 50, 100, 100, RGB(0, 0, 255));
}
```

### FrameRect - Border Only:

```cpp
void FrameRect(LPCRECT lpRect, CBrush* pBrush);
```

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CBrush brush(RGB(255, 0, 0));

    CRect rect1(50, 50, 150, 150);
    CRect rect2(170, 50, 270, 150);

    // Draw only the border (1 pixel wide)
    pDC->FrameRect(&rect1, &brush);

    // For thicker borders, draw multiple times
    for (int i = 0; i < 5; i++)
    {
        CRect temp = rect2;
        temp.InflateRect(-i, -i);
        pDC->FrameRect(&temp, &brush);
    }
}
```

### InvertRect - Invert Colors:

```cpp
BOOL InvertRect(LPCRECT lpRect);
```

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Draw something
    pDC->FillSolidRect(50, 50, 200, 200, RGB(255, 200, 200));
    pDC->TextOut(100, 120, _T("Hello World"));

    // Invert a region (creates selection effect)
    CRect invertRect(80, 110, 180, 140);
    pDC->InvertRect(&invertRect);
}
```

---

## 5. Drawing Ellipses and Circles

### Ellipse - Draw Ellipse/Circle:

```cpp
BOOL Ellipse(int x1, int y1, int x2, int y2);
BOOL Ellipse(LPCRECT lpRect);
```

**Note**: Ellipse is drawn inside bounding rectangle

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
    CBrush brush(RGB(255, 255, 200));

    CPen* pOldPen = pDC->SelectObject(&pen);
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    // Draw circle (square bounding rect)
    pDC->Ellipse(50, 50, 150, 150);

    // Draw ellipse (rectangular bounding rect)
    pDC->Ellipse(200, 50, 400, 150);

    // Show bounding rectangles with dotted lines
    CPen dotPen(PS_DOT, 1, RGB(200, 200, 200));
    pDC->SelectObject(&dotPen);
    pDC->SelectStockObject(NULL_BRUSH);

    pDC->Rectangle(50, 50, 150, 150);
    pDC->Rectangle(200, 50, 400, 150);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}
```

### Drawing Perfect Circles:

```cpp
void CMyView::DrawCircle(CDC* pDC, int centerX, int centerY, int radius)
{
    pDC->Ellipse(centerX - radius, centerY - radius,
                 centerX + radius, centerY + radius);
}

void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 2, RGB(0, 0, 255));
    CBrush brush(RGB(200, 200, 255));

    CPen* pOldPen = pDC->SelectObject(&pen);
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    // Draw concentric circles
    for (int r = 20; r <= 100; r += 20)
    {
        DrawCircle(pDC, 200, 200, r);
    }

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}
```

### Filled vs Outlined:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 3, RGB(255, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);

    // Filled circle
    CBrush brush(RGB(255, 200, 200));
    CBrush* pOldBrush = pDC->SelectObject(&brush);
    pDC->Ellipse(50, 50, 150, 150);

    // Outlined only (no fill)
    pDC->SelectStockObject(NULL_BRUSH);
    pDC->Ellipse(200, 50, 300, 150);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);

    pDC->SetBkMode(TRANSPARENT);
    pDC->TextOut(70, 160, _T("Filled"));
    pDC->TextOut(205, 160, _T("Outlined"));
}
```

---

## 6. Arcs, Chords, and Pies

### Arc - Elliptical Arc:

```cpp
BOOL Arc(int x1, int y1, int x2, int y2,
         int x3, int y3, int x4, int y4);
```

**Parameters**:
- (x1, y1, x2, y2): Bounding rectangle
- (x3, y3): Start point (radial direction)
- (x4, y4): End point (radial direction)

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 3, RGB(255, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);

    // Draw arc (counterclockwise from start to end)
    pDC->Arc(50, 50, 250, 250,   // Bounding rect
             250, 150,             // Start (right)
             150, 50);             // End (top)

    // Show bounding rectangle
    CPen dotPen(PS_DOT, 1, RGB(200, 200, 200));
    pDC->SelectObject(&dotPen);
    pDC->Rectangle(50, 50, 250, 250);

    // Show start and end points
    CPen bluePen(PS_SOLID, 5, RGB(0, 0, 255));
    pDC->SelectObject(&bluePen);
    pDC->SetPixel(250, 150, RGB(0, 0, 255));  // Start
    pDC->SetPixel(150, 50, RGB(0, 255, 0));   // End

    pDC->SelectObject(pOldPen);

    pDC->SetBkMode(TRANSPARENT);
    pDC->TextOut(255, 145, _T("Start"));
    pDC->TextOut(155, 35, _T("End"));
}
```

### Chord - Filled Arc with Straight Line:

```cpp
BOOL Chord(int x1, int y1, int x2, int y2,
           int x3, int y3, int x4, int y4);
```

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 2, RGB(0, 0, 128));
    CBrush brush(RGB(200, 200, 255));

    CPen* pOldPen = pDC->SelectObject(&pen);
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    // Draw chord (arc with straight line connecting endpoints)
    pDC->Chord(50, 50, 250, 250,
               250, 150,   // Start
               150, 50);   // End

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}
```

### Pie - Filled Pie Slice:

```cpp
BOOL Pie(int x1, int y1, int x2, int y2,
         int x3, int y3, int x4, int y4);
```

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Draw pie chart
    struct PieData {
        double percent;
        COLORREF color;
        LPCTSTR label;
    };

    PieData data[] = {
        {0.35, RGB(255, 100, 100), _T("35%")},
        {0.25, RGB(100, 255, 100), _T("25%")},
        {0.20, RGB(100, 100, 255), _T("20%")},
        {0.20, RGB(255, 255, 100), _T("20%")}
    };

    int centerX = 200, centerY = 200;
    int radius = 120;
    CRect rect(centerX - radius, centerY - radius,
               centerX + radius, centerY + radius);

    double currentAngle = 0;

    for (int i = 0; i < 4; i++)
    {
        CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
        CBrush brush(data[i].color);

        CPen* pOldPen = pDC->SelectObject(&pen);
        CBrush* pOldBrush = pDC->SelectObject(&brush);

        double startAngle = currentAngle;
        double endAngle = currentAngle + data[i].percent * 2 * 3.14159;

        int x1 = centerX + (int)(radius * 1.5 * cos(startAngle));
        int y1 = centerY - (int)(radius * 1.5 * sin(startAngle));
        int x2 = centerX + (int)(radius * 1.5 * cos(endAngle));
        int y2 = centerY - (int)(radius * 1.5 * sin(endAngle));

        pDC->Pie(&rect, x1, y1, x2, y2);

        pDC->SelectObject(pOldPen);
        pDC->SelectObject(pOldBrush);

        currentAngle = endAngle;
    }

    // Draw title
    pDC->SetBkMode(TRANSPARENT);
    CFont font;
    font.CreatePointFont(120, _T("Arial"));
    CFont* pOldFont = pDC->SelectObject(&font);
    pDC->TextOut(centerX - 50, 30, _T("Sales Data"));
    pDC->SelectObject(pOldFont);
}
```

---

## 7. Bezier Curves and Splines

### PolyBezier - Cubic Bezier Curves:

```cpp
BOOL PolyBezier(const POINT* lpPoints, int nCount);
```

**Note**: Requires 4 points per curve (start, control1, control2, end)

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);

    // Define Bezier curve points
    POINT points[] = {
        {50, 200},    // Start point
        {100, 50},    // Control point 1
        {200, 50},    // Control point 2
        {250, 200}    // End point
    };

    // Draw the curve
    pDC->PolyBezier(points, 4);

    // Show control points
    CPen dotPen(PS_DOT, 1, RGB(100, 100, 100));
    pDC->SelectObject(&dotPen);

    // Lines to control points
    pDC->MoveTo(points[0]);
    pDC->LineTo(points[1]);
    pDC->MoveTo(points[3]);
    pDC->LineTo(points[2]);

    // Draw control point markers
    CBrush brush(RGB(0, 0, 255));
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    for (int i = 0; i < 4; i++)
    {
        pDC->Ellipse(points[i].x - 3, points[i].y - 3,
                     points[i].x + 3, points[i].y + 3);
    }

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}
```

### Multiple Bezier Curves:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 3, RGB(0, 0, 255));
    CPen* pOldPen = pDC->SelectObject(&pen);

    // S-curve with multiple Bezier segments
    POINT points[] = {
        // First curve
        {50, 100}, {100, 50}, {150, 50}, {200, 100},
        // Second curve (shares end point of first)
        {250, 150}, {300, 150}, {350, 100}
    };

    pDC->PolyBezier(points, 7);

    pDC->SelectObject(pOldPen);
}
```

### PolyBezierTo:

```cpp
BOOL PolyBezierTo(const POINT* lpPoints, int nCount);
```

**Note**: Uses current position as start point

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);

    // Set start position
    pDC->MoveTo(50, 200);

    // Only need 3 points (control1, control2, end)
    POINT points[] = {
        {100, 50},    // Control 1
        {200, 50},    // Control 2
        {250, 200}    // End
    };

    pDC->PolyBezierTo(points, 3);

    pDC->SelectObject(pOldPen);
}
```

---

## 8. Round Rectangles

### RoundRect - Rectangle with Rounded Corners:

```cpp
BOOL RoundRect(int x1, int y1, int x2, int y2,
               int x3, int y3);
```

**Parameters**:
- (x1, y1, x2, y2): Bounding rectangle
- (x3, y3): Width and height of corner ellipse

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
    CBrush brush(RGB(200, 255, 200));

    CPen* pOldPen = pDC->SelectObject(&pen);
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    // Various corner radii
    pDC->RoundRect(50, 50, 150, 150, 10, 10);   // Small radius
    pDC->TextOut(60, 160, _T("10x10"));

    pDC->RoundRect(200, 50, 300, 150, 30, 30);  // Medium radius
    pDC->TextOut(205, 160, _T("30x30"));

    pDC->RoundRect(350, 50, 450, 150, 50, 50);  // Large radius
    pDC->TextOut(355, 160, _T("50x50"));

    // Elliptical corners
    pDC->RoundRect(50, 200, 150, 300, 20, 40);
    pDC->TextOut(60, 310, _T("20x40"));

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}
```

### Creating Buttons with RoundRect:

```cpp
void CMyView::DrawButton(CDC* pDC, CRect rect,
                         LPCTSTR text, BOOL pressed)
{
    CPen pen(PS_SOLID, 2, RGB(0, 0, 128));
    CBrush brush(pressed ? RGB(200, 200, 255) : RGB(220, 220, 255));

    CPen* pOldPen = pDC->SelectObject(&pen);
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    if (pressed)
        rect.OffsetRect(2, 2);

    pDC->RoundRect(&rect, CPoint(10, 10));

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);

    // Draw text
    pDC->SetBkMode(TRANSPARENT);
    pDC->DrawText(text, &rect,
                  DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CMyView::OnDraw(CDC* pDC)
{
    DrawButton(pDC, CRect(50, 50, 150, 90), _T("Normal"), FALSE);
    DrawButton(pDC, CRect(170, 50, 270, 90), _T("Pressed"), TRUE);
}
```

---

## 9. Complex Shape Construction

### Combining Shapes:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
    CBrush brush(RGB(255, 255, 200));

    CPen* pOldPen = pDC->SelectObject(&pen);
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    // Draw house
    // Base
    pDC->Rectangle(100, 200, 300, 350);

    // Roof (triangle)
    POINT roof[] = {
        {100, 200}, {200, 100}, {300, 200}
    };
    pDC->Polygon(roof, 3);

    // Door
    CBrush brownBrush(RGB(139, 69, 19));
    pDC->SelectObject(&brownBrush);
    pDC->Rectangle(170, 270, 230, 350);

    // Windows
    CBrush blueBrush(RGB(173, 216, 230));
    pDC->SelectObject(&blueBrush);
    pDC->Rectangle(120, 220, 160, 260);
    pDC->Rectangle(240, 220, 280, 260);

    // Window panes
    CPen thinPen(PS_SOLID, 1, RGB(0, 0, 0));
    pDC->SelectObject(&thinPen);

    pDC->MoveTo(140, 220);
    pDC->LineTo(140, 260);
    pDC->MoveTo(120, 240);
    pDC->LineTo(160, 240);

    pDC->MoveTo(260, 220);
    pDC->LineTo(260, 260);
    pDC->MoveTo(240, 240);
    pDC->LineTo(280, 240);

    // Doorknob
    pDC->SelectObject(&pen);
    pDC->SelectStockObject(BLACK_BRUSH);
    pDC->Ellipse(215, 310, 220, 315);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}
```

### Star Shape:

```cpp
void CMyView::DrawStar(CDC* pDC, int centerX, int centerY,
                       int outerRadius, int innerRadius)
{
    POINT points[10];
    double angleStep = 3.14159 / 5;  // 36 degrees

    for (int i = 0; i < 10; i++)
    {
        double angle = i * angleStep - 3.14159 / 2;
        int radius = (i % 2 == 0) ? outerRadius : innerRadius;

        points[i].x = centerX + (int)(radius * cos(angle));
        points[i].y = centerY + (int)(radius * sin(angle));
    }

    pDC->Polygon(points, 10);
}

void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 2, RGB(255, 215, 0));
    CBrush brush(RGB(255, 255, 0));

    CPen* pOldPen = pDC->SelectObject(&pen);
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    DrawStar(pDC, 200, 200, 80, 35);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}
```

---

## 10. Practical Examples

### Graph Plotting:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    // Draw axes
    CPen axisPen(PS_SOLID, 2, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&axisPen);

    int marginLeft = 50;
    int marginBottom = 50;
    int marginTop = 30;
    int marginRight = 30;

    // Y axis
    pDC->MoveTo(marginLeft, marginTop);
    pDC->LineTo(marginLeft, rect.Height() - marginBottom);

    // X axis
    pDC->MoveTo(marginLeft, rect.Height() - marginBottom);
    pDC->LineTo(rect.Width() - marginRight, rect.Height() - marginBottom);

    // Plot sine wave
    CPen graphPen(PS_SOLID, 2, RGB(255, 0, 0));
    pDC->SelectObject(&graphPen);

    int graphWidth = rect.Width() - marginLeft - marginRight;
    int graphHeight = rect.Height() - marginTop - marginBottom;

    BOOL first = TRUE;

    for (int x = 0; x < graphWidth; x++)
    {
        double angle = (x / (double)graphWidth) * 4 * 3.14159;
        double sinValue = sin(angle);

        int screenX = marginLeft + x;
        int screenY = marginTop + graphHeight / 2 -
                      (int)(sinValue * graphHeight / 2.5);

        if (first)
        {
            pDC->MoveTo(screenX, screenY);
            first = FALSE;
        }
        else
        {
            pDC->LineTo(screenX, screenY);
        }
    }

    pDC->SelectObject(pOldPen);

    // Labels
    pDC->SetBkMode(TRANSPARENT);
    pDC->TextOut(10, marginTop + graphHeight / 2, _T("0"));
    pDC->TextOut(marginLeft + graphWidth / 2, rect.Height() - 30,
                 _T("x"));
    pDC->TextOut(10, 10, _T("sin(x)"));
}
```

---

## 11. Exercises

### Exercise 1: Shape Gallery

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    int x = 50, y = 50;
    int size = 80;
    int spacing = 100;

    CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);

    // Square
    CBrush brush1(RGB(255, 200, 200));
    CBrush* pOldBrush = pDC->SelectObject(&brush1);
    pDC->Rectangle(x, y, x + size, y + size);
    pDC->TextOut(x + 10, y + size + 5, _T("Square"));

    // Circle
    x += spacing;
    CBrush brush2(RGB(200, 255, 200));
    pDC->SelectObject(&brush2);
    pDC->Ellipse(x, y, x + size, y + size);
    pDC->TextOut(x + 10, y + size + 5, _T("Circle"));

    // Triangle
    x += spacing;
    CBrush brush3(RGB(200, 200, 255));
    pDC->SelectObject(&brush3);
    POINT tri[] = {
        {x + size / 2, y},
        {x, y + size},
        {x + size, y + size}
    };
    pDC->Polygon(tri, 3);
    pDC->TextOut(x + 5, y + size + 5, _T("Triangle"));

    // Pentagon
    x += spacing;
    CBrush brush4(RGB(255, 255, 200));
    pDC->SelectObject(&brush4);
    DrawRegularPolygon(pDC, x + size / 2, y + size / 2, size / 2, 5);
    pDC->TextOut(x, y + size + 5, _T("Pentagon"));

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}

void CMyView::DrawRegularPolygon(CDC* pDC, int cx, int cy,
                                  int radius, int sides)
{
    POINT* points = new POINT[sides];
    double angleStep = 2 * 3.14159 / sides;

    for (int i = 0; i < sides; i++)
    {
        double angle = i * angleStep - 3.14159 / 2;
        points[i].x = cx + (int)(radius * cos(angle));
        points[i].y = cy + (int)(radius * sin(angle));
    }

    pDC->Polygon(points, sides);
    delete[] points;
}
```

---

## Key Takeaways

1. ✅ MoveTo sets position, LineTo draws line
2. ✅ Polyline for multiple connected lines
3. ✅ Polygon automatically closes and fills
4. ✅ Rectangle and Ellipse for basic shapes
5. ✅ Arc, Chord, Pie for circular segments
6. ✅ PolyBezier for smooth curves
7. ✅ RoundRect for rounded corners
8. ✅ Combine shapes for complex drawings

---

## Next Lesson Preview

In **Lesson 50: Text Drawing and Fonts**, we'll learn:
- TextOut and DrawText methods
- Creating and selecting fonts
- Font attributes and metrics
- Text alignment and formatting
- Measuring text

**Congratulations on completing Lesson 49!**
