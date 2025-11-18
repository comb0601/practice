# Lesson 46: Introduction to GDI (Graphics Device Interface)

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. What is GDI?
2. GDI Architecture and Components
3. Understanding Device Independence
4. GDI Objects and Handles
5. Basic Drawing Concepts
6. Creating Your First GDI Application
7. GDI vs GDI+
8. Common GDI Functions Overview
9. Exercises
10. Best Practices

---

## 1. What is GDI?

**GDI (Graphics Device Interface)** is the core Windows API for rendering graphics, text, and images on display devices and printers. It provides device-independent graphics output, meaning the same code works across different display devices without modification.

### Key Characteristics:

- **Device Independence**: Code works on any device (screen, printer, etc.)
- **2D Graphics**: Optimized for 2D rendering
- **Vector Graphics**: Uses mathematical descriptions for shapes
- **Raster Graphics**: Supports bitmap images
- **Text Rendering**: Sophisticated font and text handling
- **Printing Support**: Unified API for screen and printer output

### History:

- **Windows 1.0 (1985)**: Original GDI introduced
- **Windows 95/NT**: Enhanced GDI with 32-bit support
- **Windows XP**: GDI+ introduced (C++ wrapper)
- **Windows Vista+**: Desktop Window Manager uses DirectX, but GDI still widely used

### Why Learn GDI?

1. **MFC Foundation**: Essential for MFC UI development
2. **Windows Legacy**: Billions of lines of existing code
3. **Printing**: Best API for Windows printing
4. **Simple 2D Graphics**: Easier than DirectX for basic needs
5. **Business Applications**: Standard for forms and reports

---

## 2. GDI Architecture and Components

### GDI Architecture:

```
┌─────────────────────────────────────────┐
│      Application (Your Code)            │
├─────────────────────────────────────────┤
│      MFC Classes (CDC, CPen, etc.)      │
├─────────────────────────────────────────┤
│      Windows GDI API (gdi32.dll)        │
├─────────────────────────────────────────┤
│      Device Drivers                      │
├─────────────────────────────────────────┤
│   Hardware (Display, Printer, etc.)     │
└─────────────────────────────────────────┘
```

### Core GDI Components:

#### 1. **Device Context (DC)**
The primary interface for all GDI operations:
```cpp
// Device Context is the "canvas" you draw on
HDC hdc = GetDC(hwnd);           // Get DC
// ... draw operations ...
ReleaseDC(hwnd, hdc);            // Release DC
```

#### 2. **GDI Objects**
Tools used for drawing:
- **Pens**: Draw lines and shape outlines
- **Brushes**: Fill shapes and backgrounds
- **Fonts**: Render text
- **Bitmaps**: Store images
- **Regions**: Define complex areas
- **Palettes**: Manage colors

#### 3. **Drawing Functions**
Operations that produce output:
```cpp
LineTo()        // Draw lines
Rectangle()     // Draw rectangles
Ellipse()       // Draw circles/ellipses
TextOut()       // Draw text
BitBlt()        // Copy bitmaps
```

### GDI Object Hierarchy:

```
CGdiObject (Base Class)
├── CPen            (Drawing lines)
├── CBrush          (Filling areas)
├── CFont           (Text rendering)
├── CBitmap         (Raster images)
├── CPalette        (Color management)
└── CRgn            (Regions)
```

---

## 3. Understanding Device Independence

### The Problem GDI Solves:

Different devices have different capabilities:
- **Screen**: 96 DPI, RGB color
- **Laser Printer**: 600 DPI, CMYK color
- **Plotter**: Vector output
- **PDF**: Virtual device

### How GDI Achieves Device Independence:

#### 1. **Logical Units**
You specify coordinates in logical units, GDI converts to device units:

```cpp
// You think in logical coordinates
pDC->MoveTo(100, 100);
pDC->LineTo(200, 200);

// GDI translates to actual device pixels
// Screen: might be 100x100 pixels
// 600 DPI Printer: might be 2500x2500 dots
```

#### 2. **Mapping Modes**
Define how logical units map to device units:

```cpp
pDC->SetMapMode(MM_TEXT);        // 1 unit = 1 pixel
pDC->SetMapMode(MM_LOMETRIC);    // 1 unit = 0.1 mm
pDC->SetMapMode(MM_HIMETRIC);    // 1 unit = 0.01 mm
pDC->SetMapMode(MM_LOENGLISH);   // 1 unit = 0.01 inch
pDC->SetMapMode(MM_HIENGLISH);   // 1 unit = 0.001 inch
```

#### 3. **Device Capabilities**
Query device capabilities at runtime:

```cpp
int width = GetDeviceCaps(hdc, HORZRES);    // Horizontal resolution
int height = GetDeviceCaps(hdc, VERTRES);   // Vertical resolution
int bpp = GetDeviceCaps(hdc, BITSPIXEL);    // Bits per pixel
int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);  // DPI horizontal
```

---

## 4. GDI Objects and Handles

### Understanding Handles:

In Windows, a **handle** is an opaque identifier to a resource:

```cpp
HWND  hWnd;     // Window handle
HDC   hdc;      // Device Context handle
HPEN  hPen;     // Pen handle
HBRUSH hBrush;  // Brush handle
HFONT hFont;    // Font handle
```

### GDI Object Lifecycle:

#### 1. **Create Object**
```cpp
HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));  // Red pen, 2 pixels wide
```

#### 2. **Select into DC**
```cpp
HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);  // Save old, select new
```

#### 3. **Use Object**
```cpp
MoveToEx(hdc, 10, 10, NULL);
LineTo(hdc, 100, 100);  // Draws with red pen
```

#### 4. **Restore and Delete**
```cpp
SelectObject(hdc, hOldPen);  // Restore original
DeleteObject(hPen);          // Free memory
```

### Important Rules:

**⚠️ Critical GDI Rules:**
1. **Always save the old object** when selecting new one
2. **Always restore the old object** before deleting new one
3. **Always delete GDI objects** you create (except stock objects)
4. **Never delete an object that's selected** into a DC

### Stock Objects:

Windows provides pre-created objects:

```cpp
HPEN hPen = (HPEN)GetStockObject(BLACK_PEN);
HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
HFONT hFont = (HFONT)GetStockObject(SYSTEM_FONT);

// Stock objects don't need to be deleted
```

---

## 5. Basic Drawing Concepts

### Coordinate System:

```
(0,0) ─────────────────> X
 │
 │
 │
 │
 ↓
 Y

Default: Origin at top-left, Y increases downward
```

### Drawing State:

The DC maintains current state:
- **Current Position**: Where next operation starts
- **Selected Pen**: Current line drawing tool
- **Selected Brush**: Current fill tool
- **Selected Font**: Current text rendering tool
- **Text Color**: Current text color
- **Background Color**: Current background color
- **Drawing Mode**: How colors combine (ROP2)

### Example - Drawing State:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Set current position
    pDC->MoveTo(50, 50);

    // Draw from current position to (150, 50)
    pDC->LineTo(150, 50);
    // Current position now at (150, 50)

    // Draw from (150, 50) to (150, 150)
    pDC->LineTo(150, 150);

    // Draw from (150, 150) to (50, 150)
    pDC->LineTo(50, 150);

    // Close the rectangle back to start
    pDC->LineTo(50, 50);
}
```

---

## 6. Creating Your First GDI Application

### Complete MFC Application Example:

#### Step 1: Create MFC Application

1. Visual Studio → New Project
2. MFC Application
3. Name: **GDIIntro**
4. Application Type: **Single document**
5. Finish

#### Step 2: Modify the View Class

**GDIIntroView.h:**
```cpp
#pragma once

class CGDIIntroView : public CView
{
protected:
    CGDIIntroView() noexcept;
    DECLARE_DYNCREATE(CGDIIntroView)

public:
    virtual void OnDraw(CDC* pDC);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
    virtual ~CGDIIntroView();

    DECLARE_MESSAGE_MAP()
};
```

**GDIIntroView.cpp:**
```cpp
#include "pch.h"
#include "framework.h"
#include "GDIIntro.h"
#include "GDIIntroView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CGDIIntroView, CView)

BEGIN_MESSAGE_MAP(CGDIIntroView, CView)
END_MESSAGE_MAP()

CGDIIntroView::CGDIIntroView() noexcept
{
}

CGDIIntroView::~CGDIIntroView()
{
}

BOOL CGDIIntroView::PreCreateWindow(CREATESTRUCT& cs)
{
    return CView::PreCreateWindow(cs);
}

void CGDIIntroView::OnDraw(CDC* pDC)
{
    CDocument* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // Get client rectangle
    CRect rect;
    GetClientRect(&rect);

    // Draw background
    pDC->FillSolidRect(rect, RGB(240, 240, 240));

    // Draw title
    CFont titleFont;
    titleFont.CreatePointFont(240, _T("Arial"));
    CFont* pOldFont = pDC->SelectObject(&titleFont);
    pDC->SetTextColor(RGB(0, 0, 128));
    pDC->SetBkMode(TRANSPARENT);
    pDC->TextOut(50, 20, _T("GDI Graphics Demo"));
    pDC->SelectObject(pOldFont);

    // Draw a red rectangle
    CPen redPen(PS_SOLID, 3, RGB(255, 0, 0));
    CBrush blueBrush(RGB(100, 100, 255));

    CPen* pOldPen = pDC->SelectObject(&redPen);
    CBrush* pOldBrush = pDC->SelectObject(&blueBrush);

    pDC->Rectangle(50, 80, 250, 200);

    // Draw a green circle
    CPen greenPen(PS_SOLID, 3, RGB(0, 200, 0));
    CBrush yellowBrush(RGB(255, 255, 0));

    pDC->SelectObject(&greenPen);
    pDC->SelectObject(&yellowBrush);

    pDC->Ellipse(300, 80, 500, 280);

    // Draw lines
    CPen blackPen(PS_SOLID, 2, RGB(0, 0, 0));
    pDC->SelectObject(&blackPen);
    pDC->SelectObject(pOldBrush);  // Use null brush

    pDC->MoveTo(50, 320);
    pDC->LineTo(500, 320);
    pDC->LineTo(275, 450);
    pDC->LineTo(50, 320);

    // Restore original objects
    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);

    // Draw text information
    CString info;
    info.Format(_T("Client Area: %d x %d pixels"), rect.Width(), rect.Height());
    pDC->TextOut(50, rect.bottom - 30, info);
}
```

### What This Code Does:

1. **Fills background** with light gray
2. **Draws title** in large blue Arial font
3. **Draws red rectangle** with blue fill
4. **Draws green circle** with yellow fill
5. **Draws triangle** with black outline
6. **Displays client area dimensions**

---

## 7. GDI vs GDI+

### Comparison:

| Feature | GDI | GDI+ |
|---------|-----|------|
| **Language** | C API | C++ Classes |
| **Antialiasing** | No | Yes |
| **Alpha Blending** | Limited | Full support |
| **Image Formats** | BMP only | PNG, JPEG, GIF, TIFF |
| **Transformations** | Limited | Matrix-based |
| **Performance** | Faster for simple ops | Slower but prettier |
| **Gradients** | No | Yes |
| **Learning Curve** | Steeper | Easier |

### When to Use Each:

**Use GDI when:**
- Maximum performance needed
- Printing (GDI is more reliable)
- Legacy code maintenance
- Simple shapes and text

**Use GDI+ when:**
- Image manipulation needed
- Transparency required
- Antialiased graphics wanted
- Modern look needed

### Example Comparison:

**GDI Code:**
```cpp
// GDI - More verbose
CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
CPen* pOldPen = pDC->SelectObject(&pen);
pDC->MoveTo(10, 10);
pDC->LineTo(100, 100);
pDC->SelectObject(pOldPen);
```

**GDI+ Code:**
```cpp
// GDI+ - Cleaner syntax
Graphics graphics(pDC->m_hDC);
Pen pen(Color(255, 255, 0, 0), 2.0f);
graphics.DrawLine(&pen, 10, 10, 100, 100);
// Auto cleanup
```

---

## 8. Common GDI Functions Overview

### Drawing Functions:

```cpp
// Lines
MoveTo(x, y)              // Set current position
LineTo(x, y)              // Draw line from current position
Polyline(points, count)   // Draw connected lines

// Shapes
Rectangle(x1, y1, x2, y2)           // Rectangle
Ellipse(x1, y1, x2, y2)             // Ellipse/Circle
RoundRect(x1, y1, x2, y2, w, h)     // Rounded rectangle
Polygon(points, count)              // Filled polygon
Arc(x1, y1, x2, y2, x3, y3, x4, y4) // Arc segment

// Text
TextOut(x, y, text)                 // Simple text output
DrawText(text, rect, format)        // Formatted text
ExtTextOut(...)                     // Extended text with options

// Bitmaps
BitBlt(...)                         // Copy bitmap
StretchBlt(...)                     // Copy and scale bitmap
```

### GDI Object Creation:

```cpp
// Pens
CreatePen(style, width, color)
CreatePenIndirect(&logpen)

// Brushes
CreateSolidBrush(color)
CreateHatchBrush(hatch, color)
CreatePatternBrush(hBitmap)

// Fonts
CreateFont(height, width, ...)
CreateFontIndirect(&logfont)

// Bitmaps
CreateBitmap(width, height, planes, bpp, bits)
CreateCompatibleBitmap(hdc, width, height)
```

---

## 9. Exercises

### Exercise 1: Simple Shapes

Create an application that draws:
- A red square (100x100) at (50, 50)
- A blue circle (diameter 100) at (200, 50)
- A green triangle at (350, 50)

**Solution:**
```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Red square
    CPen redPen(PS_SOLID, 2, RGB(255, 0, 0));
    CBrush redBrush(RGB(255, 200, 200));
    CPen* pOldPen = pDC->SelectObject(&redPen);
    CBrush* pOldBrush = pDC->SelectObject(&redBrush);
    pDC->Rectangle(50, 50, 150, 150);

    // Blue circle
    CPen bluePen(PS_SOLID, 2, RGB(0, 0, 255));
    CBrush blueBrush(RGB(200, 200, 255));
    pDC->SelectObject(&bluePen);
    pDC->SelectObject(&blueBrush);
    pDC->Ellipse(200, 50, 300, 150);

    // Green triangle
    CPen greenPen(PS_SOLID, 2, RGB(0, 255, 0));
    CBrush greenBrush(RGB(200, 255, 200));
    pDC->SelectObject(&greenPen);
    pDC->SelectObject(&greenBrush);

    POINT triangle[3] = {
        {400, 150},  // Bottom
        {350, 50},   // Top left
        {450, 50}    // Top right
    };
    pDC->Polygon(triangle, 3);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}
```

---

### Exercise 2: Device Information Display

Create an app that displays device capabilities:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CString info;
    int y = 20;

    info.Format(_T("Horizontal Resolution: %d pixels"),
                GetDeviceCaps(pDC->m_hDC, HORZRES));
    pDC->TextOut(20, y, info);
    y += 30;

    info.Format(_T("Vertical Resolution: %d pixels"),
                GetDeviceCaps(pDC->m_hDC, VERTRES));
    pDC->TextOut(20, y, info);
    y += 30;

    info.Format(_T("Bits Per Pixel: %d"),
                GetDeviceCaps(pDC->m_hDC, BITSPIXEL));
    pDC->TextOut(20, y, info);
    y += 30;

    info.Format(_T("Logical Pixels X: %d DPI"),
                GetDeviceCaps(pDC->m_hDC, LOGPIXELSX));
    pDC->TextOut(20, y, info);
    y += 30;

    info.Format(_T("Logical Pixels Y: %d DPI"),
                GetDeviceCaps(pDC->m_hDC, LOGPIXELSY));
    pDC->TextOut(20, y, info);
    y += 30;

    info.Format(_T("Color Planes: %d"),
                GetDeviceCaps(pDC->m_hDC, PLANES));
    pDC->TextOut(20, y, info);
}
```

---

### Exercise 3: Stock Object Demonstration

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // BLACK_PEN example
    CPen* pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
    CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(WHITE_BRUSH);
    pDC->Rectangle(20, 20, 120, 120);
    pDC->TextOut(30, 130, _T("BLACK_PEN"));

    // GRAY_BRUSH
    pDC->SelectStockObject(GRAY_BRUSH);
    pDC->Rectangle(140, 20, 240, 120);
    pDC->TextOut(150, 130, _T("GRAY_BRUSH"));

    // LTGRAY_BRUSH
    pDC->SelectStockObject(LTGRAY_BRUSH);
    pDC->Rectangle(260, 20, 360, 120);
    pDC->TextOut(270, 130, _T("LTGRAY_BRUSH"));

    // Restore
    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}
```

---

### Exercise 4: Grid Drawing

Draw a 10x10 grid:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CPen gridPen(PS_SOLID, 1, RGB(200, 200, 200));
    CPen* pOldPen = pDC->SelectObject(&gridPen);

    int gridSize = 50;
    int gridCount = 10;

    // Vertical lines
    for (int i = 0; i <= gridCount; i++)
    {
        int x = i * gridSize;
        pDC->MoveTo(x, 0);
        pDC->LineTo(x, gridCount * gridSize);
    }

    // Horizontal lines
    for (int i = 0; i <= gridCount; i++)
    {
        int y = i * gridSize;
        pDC->MoveTo(0, y);
        pDC->LineTo(gridCount * gridSize, y);
    }

    pDC->SelectObject(pOldPen);
}
```

---

### Exercise 5: Color Gradient Simulation

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    int steps = 256;
    int barHeight = rect.Height() / steps;

    for (int i = 0; i < steps; i++)
    {
        int color = (i * 255) / steps;
        CBrush brush(RGB(color, 0, 255 - color));

        CRect bar(0, i * barHeight, rect.Width(), (i + 1) * barHeight);
        pDC->FillRect(&bar, &brush);
    }
}
```

---

## 10. Best Practices

### Memory Management:

```cpp
// ✅ GOOD - Proper cleanup
void DrawGood(CDC* pDC)
{
    CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);

    pDC->MoveTo(10, 10);
    pDC->LineTo(100, 100);

    pDC->SelectObject(pOldPen);  // Restore before destruction
}

// ❌ BAD - Leaked GDI object
void DrawBad(CDC* pDC)
{
    CPen* pPen = new CPen(PS_SOLID, 2, RGB(255, 0, 0));
    pDC->SelectObject(pPen);

    pDC->MoveTo(10, 10);
    pDC->LineTo(100, 100);

    // Never restored, never deleted - MEMORY LEAK!
}
```

### Performance Tips:

```cpp
// ✅ GOOD - Create objects once
void CMyView::OnDraw(CDC* pDC)
{
    static CPen pen(PS_SOLID, 2, RGB(255, 0, 0));  // Created once
    CPen* pOldPen = pDC->SelectObject(&pen);

    // Draw many lines...
    for (int i = 0; i < 1000; i++)
    {
        pDC->MoveTo(i, 0);
        pDC->LineTo(i, 100);
    }

    pDC->SelectObject(pOldPen);
}

// ❌ BAD - Creating objects in loop
void CMyView::OnDraw(CDC* pDC)
{
    for (int i = 0; i < 1000; i++)
    {
        CPen pen(PS_SOLID, 2, RGB(255, 0, 0));  // Created 1000 times!
        CPen* pOldPen = pDC->SelectObject(&pen);

        pDC->MoveTo(i, 0);
        pDC->LineTo(i, 100);

        pDC->SelectObject(pOldPen);
    }
}
```

### Error Handling:

```cpp
void DrawSafe(CDC* pDC)
{
    CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
    if (pen.m_hObject == NULL)
    {
        AfxMessageBox(_T("Failed to create pen!"));
        return;
    }

    CPen* pOldPen = pDC->SelectObject(&pen);
    if (pOldPen == NULL)
    {
        AfxMessageBox(_T("Failed to select pen!"));
        return;
    }

    // Draw operations...

    pDC->SelectObject(pOldPen);
}
```

---

## Key Takeaways

1. ✅ GDI is the foundation of Windows graphics programming
2. ✅ Device Context (DC) is the primary drawing interface
3. ✅ Always save, restore, and delete GDI objects properly
4. ✅ GDI provides device-independent graphics
5. ✅ Understanding handles and object lifecycle is crucial
6. ✅ Stock objects don't need deletion
7. ✅ Never delete an object selected into a DC
8. ✅ Create expensive objects once, reuse them

---

## Common Mistakes to Avoid

### ❌ Forgetting to Restore Objects:
```cpp
void Bad(CDC* pDC)
{
    CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
    pDC->SelectObject(&pen);
    // pen goes out of scope while selected - CRASH!
}
```

### ❌ Deleting Selected Objects:
```cpp
void Bad(CDC* pDC)
{
    CPen* pPen = new CPen(PS_SOLID, 2, RGB(255, 0, 0));
    pDC->SelectObject(pPen);
    delete pPen;  // CRASH! Object still selected
}
```

### ❌ Leaking GDI Objects:
```cpp
void Bad()
{
    for (int i = 0; i < 10000; i++)
    {
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
        // Never deleted - will exhaust GDI resources!
    }
}
```

---

## Summary Checklist

Before moving to Lesson 47, ensure you can:
- [ ] Explain what GDI is and its purpose
- [ ] Understand device independence
- [ ] Create and manage GDI objects properly
- [ ] Use handles correctly
- [ ] Draw basic shapes (rectangle, ellipse, lines)
- [ ] Select objects into DC and restore them
- [ ] Understand the importance of cleanup
- [ ] Query device capabilities
- [ ] Explain GDI vs GDI+ differences

---

## Next Lesson Preview

In **Lesson 47: Device Contexts (CDC)**, we'll dive deep into:
- Different types of Device Contexts
- CDC class and its methods
- Client DC vs Window DC vs Paint DC
- Memory DC for off-screen drawing
- DC attributes and state management

---

## Additional Resources

- [Microsoft GDI Documentation](https://docs.microsoft.com/windows/win32/gdi/windows-gdi)
- [Charles Petzold's Programming Windows](https://www.charlespetzold.com/)
- [GDI Objects Reference](https://docs.microsoft.com/windows/win32/gdi/gdi-objects)

**Congratulations on completing Lesson 46!**

You now have a solid foundation in GDI concepts. Time to master Device Contexts!
