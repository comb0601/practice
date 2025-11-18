# Lesson 39: GDI Basics - Graphics Device Interface

## Introduction
Learn the fundamentals of GDI (Graphics Device Interface) for drawing in Windows applications.

**Topics Covered:**
- Device contexts (DC)
- Drawing lines and curves
- Drawing shapes (rectangle, ellipse, polygon)
- Text output
- Pens for line drawing
- Brushes for filling
- Clipping regions
- Coordinate systems

**Time Required:** 4-5 hours

## Device Context (DC)

### Getting DC
```cpp
case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    // Drawing code here
    EndPaint(hwnd, &ps);
    return 0;
}

// Or outside WM_PAINT:
HDC hdc = GetDC(hwnd);
// Drawing code
ReleaseDC(hwnd, hdc);
```

## Drawing Lines
```cpp
MoveToEx(hdc, 10, 10, NULL);
LineTo(hdc, 100, 100);

// Multiple connected lines
POINT pts[] = {{10,10}, {50,100}, {100,50}};
Polyline(hdc, pts, 3);
```

## Drawing Shapes
```cpp
// Rectangle
Rectangle(hdc, 10, 10, 100, 100);

// Ellipse
Ellipse(hdc, 10, 10, 100, 100);

// Polygon
POINT polygon[] = {{50,10}, {100,50}, {75,100}, {25,100}, {0,50}};
Polygon(hdc, polygon, 5);
```

## Text Output
```cpp
// Simple text
TextOut(hdc, 10, 10, L"Hello World", 11);

// Text in rectangle
RECT rc = {10, 10, 200, 100};
DrawText(hdc, L"Multi-line text", -1, &rc, DT_LEFT | DT_WORDBREAK);
```

## Pens and Brushes

### Creating Pen
```cpp
HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));  // Red, 2 pixels wide
HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
// Draw with pen
SelectObject(hdc, hOldPen);
DeleteObject(hPen);
```

### Creating Brush
```cpp
HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0));  // Green
HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
Rectangle(hdc, 10, 10, 100, 100);  // Filled with green
SelectObject(hdc, hOldBrush);
DeleteObject(hBrush);
```

## See Code Examples
8 complete GDI demonstrations in Code/ folder.
