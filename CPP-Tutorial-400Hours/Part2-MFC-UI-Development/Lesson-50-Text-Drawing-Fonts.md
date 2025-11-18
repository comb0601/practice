# Lesson 50: Text Drawing and Fonts

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Text Drawing Fundamentals
2. TextOut Method
3. DrawText Method
4. ExtTextOut for Advanced Text
5. Understanding Fonts
6. CFont Class
7. Creating and Selecting Fonts
8. Font Metrics
9. Text Measurement
10. Text Alignment and Formatting
11. Advanced Text Effects
12. Exercises

---

## 1. Text Drawing Fundamentals

Text rendering in GDI involves fonts, colors, alignment, and formatting options. Understanding these elements is crucial for creating professional-looking applications.

### Basic Text Output:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Simple text output
    pDC->TextOut(50, 50, _T("Hello, World!"));

    // With CString
    CString text = _T("MFC Text Rendering");
    pDC->TextOut(50, 80, text);

    // Setting text color
    pDC->SetTextColor(RGB(255, 0, 0));
    pDC->TextOut(50, 110, _T("Red Text"));

    // Setting background color
    pDC->SetBkColor(RGB(255, 255, 0));
    pDC->TextOut(50, 140, _T("Yellow Background"));

    // Transparent background
    pDC->SetBkMode(TRANSPARENT);
    pDC->TextOut(50, 170, _T("Transparent Background"));
}
```

### Text Drawing Properties:

```
┌──────────────────────────────┐
│   Text Drawing State         │
├──────────────────────────────┤
│  • Selected Font             │
│  • Text Color                │
│  • Background Color          │
│  • Background Mode           │
│  • Text Alignment            │
│  • Character Spacing         │
└──────────────────────────────┘
```

---

## 2. TextOut Method

### TextOut Signature:

```cpp
BOOL TextOut(int x, int y, LPCTSTR lpszString, int nCount);
BOOL TextOut(int x, int y, const CString& str);
```

### Basic Usage:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Method 1: LPCTSTR with length
    LPCTSTR text = _T("Hello");
    pDC->TextOut(50, 50, text, 5);

    // Method 2: LPCTSTR (null-terminated)
    pDC->TextOut(50, 80, _T("World"));

    // Method 3: CString (most common)
    CString str = _T("MFC Programming");
    pDC->TextOut(50, 110, str);

    // Method 4: Formatted text
    CString formatted;
    formatted.Format(_T("Value: %d"), 42);
    pDC->TextOut(50, 140, formatted);
}
```

### TextOut Characteristics:

**✅ Advantages:**
- Simple and fast
- Direct coordinate positioning
- Good for single-line text
- No formatting overhead

**⚠️ Limitations:**
- No automatic word wrapping
- No multi-line support
- No text clipping to rectangle
- No formatting options

---

## 3. DrawText Method

### DrawText Signature:

```cpp
int DrawText(LPCTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat);
int DrawText(const CString& str, LPRECT lpRect, UINT nFormat);
```

### Format Flags:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CString text = _T("This is a long text that will be wrapped automatically within the rectangle bounds.");

    // Left-aligned, top
    CRect rect1(50, 50, 250, 150);
    pDC->DrawText(text, &rect1, DT_LEFT | DT_TOP | DT_WORDBREAK);
    pDC->Rectangle(&rect1);

    // Center-aligned, middle
    CRect rect2(270, 50, 470, 150);
    pDC->DrawText(_T("Centered Text"), &rect2,
                  DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    pDC->Rectangle(&rect2);

    // Right-aligned, bottom
    CRect rect3(490, 50, 690, 150);
    pDC->DrawText(_T("Right-aligned\nBottom"), &rect3,
                  DT_RIGHT | DT_BOTTOM);
    pDC->Rectangle(&rect3);
}
```

### Common Format Flags:

```cpp
// Horizontal Alignment
DT_LEFT         // Left-align text
DT_CENTER       // Center text horizontally
DT_RIGHT        // Right-align text

// Vertical Alignment
DT_TOP          // Top-align text
DT_VCENTER      // Center text vertically
DT_BOTTOM       // Bottom-align text

// Line Breaking
DT_SINGLELINE   // No line breaks
DT_WORDBREAK    // Break at word boundaries
DT_NOCLIP       // Don't clip to rectangle

// Special Effects
DT_EXPANDTABS   // Expand tab characters
DT_TABSTOP      // Custom tab stops
DT_NOPREFIX     // Don't process & for underlines
DT_END_ELLIPSIS // Add ... if text doesn't fit
DT_PATH_ELLIPSIS // Add ... in middle of path

// Calculation
DT_CALCRECT     // Calculate required rectangle size
```

### Comprehensive Example:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    int y = 20;
    int rectHeight = 80;
    int spacing = 100;

    // Example 1: Word wrap
    CRect rect1(20, y, 220, y + rectHeight);
    pDC->Rectangle(&rect1);
    pDC->DrawText(_T("This text will wrap automatically at word boundaries."),
                  &rect1, DT_WORDBREAK);
    y += spacing;

    // Example 2: Center alignment
    CRect rect2(20, y, 220, y + rectHeight);
    pDC->Rectangle(&rect2);
    pDC->DrawText(_T("Centered\nMultiple Lines"),
                  &rect2, DT_CENTER | DT_VCENTER);
    y += spacing;

    // Example 3: End ellipsis
    CRect rect3(20, y, 220, y + 30);
    pDC->Rectangle(&rect3);
    pDC->DrawText(_T("This is a very long text that won't fit"),
                  &rect3, DT_SINGLELINE | DT_END_ELLIPSIS);
    y += 50;

    // Example 4: Tab stops
    CRect rect4(20, y, 400, y + 30);
    pDC->Rectangle(&rect4);
    pDC->DrawText(_T("Name:\tJohn\tAge:\t25"),
                  &rect4, DT_EXPANDTABS);
}
```

### Calculating Text Height:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CString text = _T("This is multi-line text that we want to measure.");
    CRect rect(50, 50, 300, 50);  // Height = 0 initially

    // Calculate required height
    pDC->DrawText(text, &rect, DT_CALCRECT | DT_WORDBREAK);

    // Now rect contains the required size
    CString info;
    info.Format(_T("Required height: %d pixels"), rect.Height());
    pDC->TextOut(50, 20, info);

    // Draw the actual text
    pDC->Rectangle(&rect);
    pDC->DrawText(text, &rect, DT_WORDBREAK);
}
```

---

## 4. ExtTextOut for Advanced Text

### ExtTextOut Signature:

```cpp
BOOL ExtTextOut(int x, int y, UINT nOptions,
                LPCRECT lpRect, LPCTSTR lpszString,
                UINT nCount, LPINT lpDxWidths);
```

### Options:

```cpp
ETO_CLIPPED    // Clip text to rectangle
ETO_OPAQUE     // Fill rectangle with background color
ETO_NUMERICSLATIN // Use Latin digits
ETO_IGNORELANGUAGE // Ignore language
```

### Example Usage:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CRect rect(50, 50, 300, 100);

    // Draw with opaque background
    pDC->SetBkColor(RGB(255, 255, 200));
    pDC->ExtTextOut(60, 60, ETO_OPAQUE, &rect,
                    _T("Text with background"), 21, NULL);

    pDC->Rectangle(&rect);

    // Custom character spacing
    rect.SetRect(50, 120, 300, 170);
    int spacing[] = {20, 20, 20, 20, 20, 30, 20, 20, 20, 20, 20};
    pDC->ExtTextOut(60, 130, ETO_CLIPPED, &rect,
                    _T("Custom Space"), 12, spacing);

    pDC->Rectangle(&rect);
}
```

---

## 5. Understanding Fonts

### Font Anatomy:

```
    ┌─── Ascent ──┐
    │             │
    │    ╔═══╗   │  ← Cap Height
    │    ║   ║   │
────┼────╠═══╣───┼── Baseline
    │    ║   ║   │
    │    ╚═══╝   │
    └─── Descent ┘
```

### Font Terminology:

- **Typeface**: Font family (Arial, Times New Roman)
- **Font**: Specific instance (Arial Bold 12pt)
- **Point Size**: Height in points (1 pt = 1/72 inch)
- **Weight**: Thickness (Normal, Bold, etc.)
- **Style**: Italic, Underline, Strikeout
- **Pitch**: Fixed (monospace) or Variable
- **Family**: Serif, Sans-serif, Monospace, etc.

---

## 6. CFont Class

### CFont Class Overview:

```cpp
class CFont : public CGdiObject
{
public:
    // Constructors
    CFont();

    // Creation methods
    BOOL CreateFont(int nHeight, int nWidth,
                   int nEscapement, int nOrientation,
                   int nWeight, BYTE bItalic, BYTE bUnderline,
                   BYTE cStrikeOut, BYTE nCharSet,
                   BYTE nOutPrecision, BYTE nClipPrecision,
                   BYTE nQuality, BYTE nPitchAndFamily,
                   LPCTSTR lpszFacename);

    BOOL CreatePointFont(int nPointSize, LPCTSTR lpszFaceName,
                        CDC* pDC = NULL);

    BOOL CreateFontIndirect(const LOGFONT* lpLogFont);

    // Operators
    operator HFONT() const;

    // Attributes
    int GetLogFont(LOGFONT* pLogFont);
};
```

---

## 7. Creating and Selecting Fonts

### Method 1: CreateFont (Complete Control):

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CFont font;
    font.CreateFont(
        30,                        // Height
        0,                         // Width (0 = default)
        0,                         // Escapement (rotation)
        0,                         // Orientation
        FW_BOLD,                   // Weight
        FALSE,                     // Italic
        FALSE,                     // Underline
        FALSE,                     // StrikeOut
        DEFAULT_CHARSET,           // Character set
        OUT_DEFAULT_PRECIS,        // Output precision
        CLIP_DEFAULT_PRECIS,       // Clipping precision
        DEFAULT_QUALITY,           // Quality
        DEFAULT_PITCH | FF_SWISS,  // Pitch and family
        _T("Arial"));              // Facename

    CFont* pOldFont = pDC->SelectObject(&font);
    pDC->TextOut(50, 50, _T("Bold Arial 30"));
    pDC->SelectObject(pOldFont);
}
```

### Method 2: CreatePointFont (Easier):

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Point size * 10
    CFont font;
    font.CreatePointFont(120, _T("Arial"));  // 12 point

    CFont* pOldFont = pDC->SelectObject(&font);
    pDC->TextOut(50, 50, _T("12 Point Arial"));
    pDC->SelectObject(pOldFont);
}
```

### Method 3: CreateFontIndirect (Using LOGFONT):

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    LOGFONT lf;
    memset(&lf, 0, sizeof(LOGFONT));

    lf.lfHeight = 40;
    lf.lfWeight = FW_BOLD;
    lf.lfItalic = TRUE;
    _tcscpy_s(lf.lfFaceName, LF_FACESIZE, _T("Times New Roman"));

    CFont font;
    font.CreateFontIndirect(&lf);

    CFont* pOldFont = pDC->SelectObject(&font);
    pDC->TextOut(50, 50, _T("Bold Italic Times"));
    pDC->SelectObject(pOldFont);
}
```

### Font Weights:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    int weights[] = {
        FW_THIN,        // 100
        FW_EXTRALIGHT,  // 200
        FW_LIGHT,       // 300
        FW_NORMAL,      // 400
        FW_MEDIUM,      // 500
        FW_SEMIBOLD,    // 600
        FW_BOLD,        // 700
        FW_EXTRABOLD,   // 800
        FW_HEAVY        // 900
    };

    LPCTSTR names[] = {
        _T("Thin"), _T("ExtraLight"), _T("Light"),
        _T("Normal"), _T("Medium"), _T("SemiBold"),
        _T("Bold"), _T("ExtraBold"), _T("Heavy")
    };

    int y = 30;
    for (int i = 0; i < 9; i++)
    {
        CFont font;
        font.CreateFont(24, 0, 0, 0, weights[i],
                       FALSE, FALSE, FALSE,
                       DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                       CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                       DEFAULT_PITCH | FF_SWISS, _T("Arial"));

        CFont* pOldFont = pDC->SelectObject(&font);

        CString text;
        text.Format(_T("%s (%d)"), names[i], weights[i]);
        pDC->TextOut(50, y, text);

        pDC->SelectObject(pOldFont);
        y += 35;
    }
}
```

### Font Styles:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    int y = 50;

    // Normal
    CFont font1;
    font1.CreateFont(30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                    DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    CFont* pOldFont = pDC->SelectObject(&font1);
    pDC->TextOut(50, y, _T("Normal Text"));
    y += 40;

    // Italic
    CFont font2;
    font2.CreateFont(30, 0, 0, 0, FW_NORMAL, TRUE, FALSE, FALSE,
                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                    DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    pDC->SelectObject(&font2);
    pDC->TextOut(50, y, _T("Italic Text"));
    y += 40;

    // Underline
    CFont font3;
    font3.CreateFont(30, 0, 0, 0, FW_NORMAL, FALSE, TRUE, FALSE,
                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                    DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    pDC->SelectObject(&font3);
    pDC->TextOut(50, y, _T("Underlined Text"));
    y += 40;

    // Strikeout
    CFont font4;
    font4.CreateFont(30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, TRUE,
                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                    DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    pDC->SelectObject(&font4);
    pDC->TextOut(50, y, _T("Strikeout Text"));

    pDC->SelectObject(pOldFont);
}
```

### Rotated Text:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    int centerX = 300, centerY = 200;

    // Draw text at various angles
    for (int angle = 0; angle < 360; angle += 30)
    {
        CFont font;
        font.CreateFont(20, 0, angle * 10, 0, FW_NORMAL,
                       FALSE, FALSE, FALSE,
                       DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                       CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                       DEFAULT_PITCH | FF_SWISS, _T("Arial"));

        CFont* pOldFont = pDC->SelectObject(&font);
        pDC->SetBkMode(TRANSPARENT);

        CString text;
        text.Format(_T("%d°"), angle);
        pDC->TextOut(centerX, centerY, text);

        pDC->SelectObject(pOldFont);
    }

    // Draw center point
    CPen pen(PS_SOLID, 5, RGB(255, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->SetPixel(centerX, centerY, RGB(255, 0, 0));
    pDC->SelectObject(pOldPen);
}
```

---

## 8. Font Metrics

### TEXTMETRIC Structure:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CFont font;
    font.CreatePointFont(120, _T("Arial"));
    CFont* pOldFont = pDC->SelectObject(&font);

    TEXTMETRIC tm;
    pDC->GetTextMetrics(&tm);

    int y = 50;
    CString info;

    info.Format(_T("Height: %d"), tm.tmHeight);
    pDC->TextOut(50, y, info);
    y += 25;

    info.Format(_T("Ascent: %d"), tm.tmAscent);
    pDC->TextOut(50, y, info);
    y += 25;

    info.Format(_T("Descent: %d"), tm.tmDescent);
    pDC->TextOut(50, y, info);
    y += 25;

    info.Format(_T("Internal Leading: %d"), tm.tmInternalLeading);
    pDC->TextOut(50, y, info);
    y += 25;

    info.Format(_T("External Leading: %d"), tm.tmExternalLeading);
    pDC->TextOut(50, y, info);
    y += 25;

    info.Format(_T("Average Width: %d"), tm.tmAveCharWidth);
    pDC->TextOut(50, y, info);
    y += 25;

    info.Format(_T("Maximum Width: %d"), tm.tmMaxCharWidth);
    pDC->TextOut(50, y, info);
    y += 25;

    info.Format(_T("Weight: %d"), tm.tmWeight);
    pDC->TextOut(50, y, info);

    pDC->SelectObject(pOldFont);
}
```

### Visualizing Font Metrics:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CFont font;
    font.CreatePointFont(400, _T("Arial"));  // 40 point
    CFont* pOldFont = pDC->SelectObject(&font);

    TEXTMETRIC tm;
    pDC->GetTextMetrics(&tm);

    int x = 100;
    int baseline = 200;

    // Draw text
    pDC->SetBkMode(TRANSPARENT);
    pDC->TextOut(x, baseline - tm.tmAscent, _T("Agj"));

    // Draw metric lines
    CPen redPen(PS_SOLID, 1, RGB(255, 0, 0));
    CPen bluePen(PS_SOLID, 1, RGB(0, 0, 255));
    CPen greenPen(PS_SOLID, 1, RGB(0, 255, 0));

    // Baseline (red)
    CPen* pOldPen = pDC->SelectObject(&redPen);
    pDC->MoveTo(x - 20, baseline);
    pDC->LineTo(x + 200, baseline);
    pDC->TextOut(x + 210, baseline - 10, _T("Baseline"));

    // Top (blue)
    pDC->SelectObject(&bluePen);
    int top = baseline - tm.tmAscent;
    pDC->MoveTo(x - 20, top);
    pDC->LineTo(x + 200, top);
    pDC->TextOut(x + 210, top - 10, _T("Top"));

    // Bottom (green)
    pDC->SelectObject(&greenPen);
    int bottom = baseline + tm.tmDescent;
    pDC->MoveTo(x - 20, bottom);
    pDC->LineTo(x + 200, bottom);
    pDC->TextOut(x + 210, bottom - 10, _T("Bottom"));

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldFont);
}
```

---

## 9. Text Measurement

### GetTextExtent - Measure Single Line:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CFont font;
    font.CreatePointFont(120, _T("Arial"));
    CFont* pOldFont = pDC->SelectObject(&font);

    CString text = _T("Measure This Text");

    // Get text dimensions
    CSize size = pDC->GetTextExtent(text);

    // Draw text
    int x = 50, y = 50;
    pDC->TextOut(x, y, text);

    // Draw bounding rectangle
    CPen pen(PS_DOT, 1, RGB(255, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->SelectStockObject(NULL_BRUSH);
    pDC->Rectangle(x, y, x + size.cx, y + size.cy);
    pDC->SelectObject(pOldPen);

    // Display dimensions
    CString info;
    info.Format(_T("Width: %d, Height: %d"), size.cx, size.cy);
    pDC->TextOut(50, y + size.cy + 20, info);

    pDC->SelectObject(pOldFont);
}
```

### GetTabbedTextExtent:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CString text = _T("Name:\tJohn\tAge:\t25");

    int tabStops[] = {100, 200, 300};

    CSize size = pDC->GetTabbedTextExtent(text,
                                          3, tabStops);

    pDC->TextOut(50, 50, _T("Tabbed text:"));

    // Draw with custom tab stops
    pDC->TabbedTextOut(50, 80, text, 3, tabStops, 0);

    CString info;
    info.Format(_T("Total width: %d pixels"), size.cx);
    pDC->TextOut(50, 110, info);
}
```

---

## 10. Text Alignment and Formatting

### SetTextAlign:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    int centerX = 300, centerY = 200;

    // Draw center point
    CPen pen(PS_SOLID, 5, RGB(255, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->SetPixel(centerX, centerY, RGB(255, 0, 0));
    pDC->SelectObject(pOldPen);

    // Default (top-left)
    UINT oldAlign = pDC->SetTextAlign(TA_LEFT | TA_TOP);
    pDC->TextOut(centerX, centerY, _T("Top-Left"));

    // Center-center
    pDC->SetTextAlign(TA_CENTER | TA_BASELINE);
    pDC->TextOut(centerX, centerY + 30, _T("Center-Baseline"));

    // Right-bottom
    pDC->SetTextAlign(TA_RIGHT | TA_BOTTOM);
    pDC->TextOut(centerX, centerY + 60, _T("Right-Bottom"));

    // Restore
    pDC->SetTextAlign(oldAlign);
}
```

### Multi-Column Text Layout:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CString data[][3] = {
        {_T("Name"), _T("Age"), _T("City")},
        {_T("John"), _T("25"), _T("New York")},
        {_T("Mary"), _T("30"), _T("Boston")},
        {_T("Bob"), _T("35"), _T("Chicago")}
    };

    int colWidths[] = {150, 80, 150};
    int x = 50, y = 50;
    int rowHeight = 30;

    for (int row = 0; row < 4; row++)
    {
        int colX = x;

        for (int col = 0; col < 3; col++)
        {
            CRect rect(colX, y, colX + colWidths[col], y + rowHeight);

            if (row == 0)
            {
                // Header - bold
                CFont font;
                font.CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                               DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                               CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                               DEFAULT_PITCH | FF_SWISS, _T("Arial"));
                CFont* pOldFont = pDC->SelectObject(&font);
                pDC->DrawText(data[row][col], &rect, DT_LEFT | DT_VCENTER);
                pDC->SelectObject(pOldFont);
            }
            else
            {
                pDC->DrawText(data[row][col], &rect, DT_LEFT | DT_VCENTER);
            }

            // Draw cell border
            pDC->Rectangle(&rect);

            colX += colWidths[col];
        }

        y += rowHeight;
    }
}
```

---

## 11. Advanced Text Effects

### Shadow Text:

```cpp
void CMyView::DrawShadowText(CDC* pDC, int x, int y,
                             LPCTSTR text, COLORREF textColor,
                             COLORREF shadowColor)
{
    CFont font;
    font.CreatePointFont(240, _T("Arial Bold"));
    CFont* pOldFont = pDC->SelectObject(&font);
    pDC->SetBkMode(TRANSPARENT);

    // Draw shadow
    pDC->SetTextColor(shadowColor);
    pDC->TextOut(x + 3, y + 3, text);

    // Draw main text
    pDC->SetTextColor(textColor);
    pDC->TextOut(x, y, text);

    pDC->SelectObject(pOldFont);
}

void CMyView::OnDraw(CDC* pDC)
{
    DrawShadowText(pDC, 50, 50, _T("Shadow Text"),
                   RGB(0, 0, 255), RGB(200, 200, 200));
}
```

### Outlined Text:

```cpp
void CMyView::DrawOutlinedText(CDC* pDC, int x, int y,
                               LPCTSTR text)
{
    CFont font;
    font.CreatePointFont(300, _T("Arial Black"));
    CFont* pOldFont = pDC->SelectObject(&font);
    pDC->SetBkMode(TRANSPARENT);

    // Draw outline
    pDC->SetTextColor(RGB(0, 0, 0));
    for (int dx = -2; dx <= 2; dx++)
    {
        for (int dy = -2; dy <= 2; dy++)
        {
            if (dx != 0 || dy != 0)
                pDC->TextOut(x + dx, y + dy, text);
        }
    }

    // Draw fill
    pDC->SetTextColor(RGB(255, 255, 0));
    pDC->TextOut(x, y, text);

    pDC->SelectObject(pOldFont);
}

void CMyView::OnDraw(CDC* pDC)
{
    DrawOutlinedText(pDC, 50, 100, _T("OUTLINED"));
}
```

### Gradient Text (Using Multiple Colors):

```cpp
void CMyView::DrawGradientText(CDC* pDC, int x, int y,
                               LPCTSTR text)
{
    CFont font;
    font.CreatePointFont(300, _T("Arial Bold"));
    CFont* pOldFont = pDC->SelectObject(&font);
    pDC->SetBkMode(TRANSPARENT);

    int len = _tcslen(text);
    CSize size = pDC->GetTextExtent(text, len);

    for (int i = 0; i < len; i++)
    {
        // Calculate color (red to blue gradient)
        int red = 255 - (i * 255 / (len - 1));
        int blue = i * 255 / (len - 1);

        pDC->SetTextColor(RGB(red, 0, blue));

        // Draw single character
        CString ch(text[i]);
        CSize charSize = pDC->GetTextExtent(ch);
        pDC->TextOut(x, y, ch);

        x += charSize.cx;
    }

    pDC->SelectObject(pOldFont);
}

void CMyView::OnDraw(CDC* pDC)
{
    DrawGradientText(pDC, 50, 150, _T("GRADIENT"));
}
```

---

## 12. Exercises

### Exercise 1: Font Gallery

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    LPCTSTR fonts[] = {
        _T("Arial"), _T("Times New Roman"), _T("Courier New"),
        _T("Verdana"), _T("Georgia"), _T("Comic Sans MS")
    };

    int y = 30;
    for (int i = 0; i < 6; i++)
    {
        CFont font;
        font.CreatePointFont(120, fonts[i]);
        CFont* pOldFont = pDC->SelectObject(&font);

        CString text;
        text.Format(_T("%s - The quick brown fox jumps"), fonts[i]);
        pDC->TextOut(50, y, text);

        pDC->SelectObject(pOldFont);
        y += 40;
    }
}
```

### Exercise 2: Text Formatting Demo

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CString longText = _T("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");

    int y = 20;

    // Left-aligned with word wrap
    CRect rect1(20, y, 250, y + 100);
    pDC->Rectangle(&rect1);
    pDC->DrawText(longText, &rect1, DT_LEFT | DT_WORDBREAK);
    y += 120;

    // Center-aligned
    CRect rect2(20, y, 250, y + 100);
    pDC->Rectangle(&rect2);
    pDC->DrawText(longText, &rect2, DT_CENTER | DT_WORDBREAK);
    y += 120;

    // Right-aligned
    CRect rect3(20, y, 250, y + 100);
    pDC->Rectangle(&rect3);
    pDC->DrawText(longText, &rect3, DT_RIGHT | DT_WORDBREAK);
}
```

---

## Key Takeaways

1. ✅ TextOut for simple text, DrawText for formatted text
2. ✅ CreatePointFont is easiest for font creation
3. ✅ Always restore previous font after use
4. ✅ Use GetTextExtent to measure text
5. ✅ TEXTMETRIC provides detailed font metrics
6. ✅ SetBkMode(TRANSPARENT) for transparent text
7. ✅ DrawText supports word wrapping and alignment
8. ✅ Font rotation uses escapement parameter

---

## Next Lesson Preview

In **Lesson 51: Colors and Palettes**, we'll learn:
- RGB color model
- COLORREF structure
- Color manipulation
- System colors
- Palette management

**Congratulations on completing Lesson 50!**
