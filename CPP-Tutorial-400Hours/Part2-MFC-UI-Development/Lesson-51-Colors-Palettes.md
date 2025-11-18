# Lesson 51: Colors and Palettes

**Duration**: 5 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding Colors in GDI
2. RGB Color Model
3. COLORREF Structure
4. Creating and Using Colors
5. Color Manipulation
6. System Colors
7. Color Palettes
8. Working with Palettes
9. Color Conversion
10. Practical Examples
11. Exercises

---

## 1. Understanding Colors in GDI

Colors in GDI are represented using the RGB (Red, Green, Blue) color model. Each component ranges from 0 to 255, allowing for approximately 16.7 million possible colors.

### Color Representation:

```
┌──────────────────────────────┐
│     COLORREF (32-bit)        │
├──────────────────────────────┤
│  0x00BBGGRR                  │
│                              │
│  RR = Red   (0-255)          │
│  GG = Green (0-255)          │
│  BB = Blue  (0-255)          │
└──────────────────────────────┘
```

---

## 2. RGB Color Model

### RGB Macro:

```cpp
COLORREF RGB(BYTE red, BYTE green, BYTE blue);
```

### Basic Colors:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    int x = 50, y = 50;
    int size = 80;
    int spacing = 100;

    // Primary colors
    pDC->FillSolidRect(x, y, size, size, RGB(255, 0, 0));  // Red
    pDC->TextOut(x, y + size + 5, _T("Red"));

    x += spacing;
    pDC->FillSolidRect(x, y, size, size, RGB(0, 255, 0));  // Green
    pDC->TextOut(x, y + size + 5, _T("Green"));

    x += spacing;
    pDC->FillSolidRect(x, y, size, size, RGB(0, 0, 255));  // Blue
    pDC->TextOut(x, y + size + 5, _T("Blue"));

    // Secondary colors
    x = 50;
    y += 130;

    pDC->FillSolidRect(x, y, size, size, RGB(255, 255, 0));  // Yellow
    pDC->TextOut(x, y + size + 5, _T("Yellow"));

    x += spacing;
    pDC->FillSolidRect(x, y, size, size, RGB(0, 255, 255));  // Cyan
    pDC->TextOut(x, y + size + 5, _T("Cyan"));

    x += spacing;
    pDC->FillSolidRect(x, y, size, size, RGB(255, 0, 255));  // Magenta
    pDC->TextOut(x, y + size + 5, _T("Magenta"));

    // Grayscale
    x = 50;
    y += 130;

    pDC->FillSolidRect(x, y, size, size, RGB(0, 0, 0));  // Black
    pDC->TextOut(x, y + size + 5, _T("Black"));

    x += spacing;
    pDC->FillSolidRect(x, y, size, size, RGB(128, 128, 128));  // Gray
    pDC->TextOut(x, y + size + 5, _T("Gray"));

    x += spacing;
    pDC->FillSolidRect(x, y, size, size, RGB(255, 255, 255));  // White
    pDC->TextOut(x, y + size + 5, _T("White"));
}
```

### RGB Color Cube:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Draw RGB color cube visualization
    int startX = 50, startY = 50;
    int cellSize = 20;

    for (int r = 0; r < 6; r++)
    {
        for (int g = 0; g < 6; g++)
        {
            for (int b = 0; b < 6; b++)
            {
                int red = r * 51;    // 0, 51, 102, 153, 204, 255
                int green = g * 51;
                int blue = b * 51;

                COLORREF color = RGB(red, green, blue);

                int x = startX + b * cellSize + r * cellSize * 7;
                int y = startY + g * cellSize;

                pDC->FillSolidRect(x, y, cellSize, cellSize, color);
            }
        }
    }
}
```

---

## 3. COLORREF Structure

### Extracting Color Components:

```cpp
// Macros to extract components
BYTE GetRValue(COLORREF rgb);
BYTE GetGValue(COLORREF rgb);
BYTE GetBValue(COLORREF rgb);
```

### Example:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    COLORREF color = RGB(123, 234, 56);

    BYTE red = GetRValue(color);      // 123
    BYTE green = GetGValue(color);    // 234
    BYTE blue = GetBValue(color);     // 56

    CString info;
    info.Format(_T("Color RGB(%d, %d, %d)"), red, green, blue);
    pDC->TextOut(50, 50, info);

    // Display the color
    pDC->FillSolidRect(50, 80, 100, 100, color);
}
```

---

## 4. Creating and Using Colors

### Predefined Constants:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Windows doesn't define these, but you can
    #define CLR_RED     RGB(255, 0, 0)
    #define CLR_GREEN   RGB(0, 255, 0)
    #define CLR_BLUE    RGB(0, 0, 255)
    #define CLR_YELLOW  RGB(255, 255, 0)
    #define CLR_CYAN    RGB(0, 255, 255)
    #define CLR_MAGENTA RGB(255, 0, 255)
    #define CLR_BLACK   RGB(0, 0, 0)
    #define CLR_WHITE   RGB(255, 255, 255)
    #define CLR_GRAY    RGB(128, 128, 128)

    int x = 50, y = 50, size = 60, spacing = 80;

    pDC->FillSolidRect(x, y, size, size, CLR_RED);
    x += spacing;
    pDC->FillSolidRect(x, y, size, size, CLR_GREEN);
    x += spacing;
    pDC->FillSolidRect(x, y, size, size, CLR_BLUE);
}
```

### Setting Colors:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Text color
    COLORREF oldTextColor = pDC->SetTextColor(RGB(255, 0, 0));
    pDC->TextOut(50, 50, _T("Red Text"));

    // Background color
    COLORREF oldBkColor = pDC->SetBkColor(RGB(255, 255, 0));
    pDC->TextOut(50, 80, _T("Yellow Background"));

    // Pixel color
    pDC->SetPixel(200, 100, RGB(0, 0, 255));

    // Restore
    pDC->SetTextColor(oldTextColor);
    pDC->SetBkColor(oldBkColor);
}
```

---

## 5. Color Manipulation

### Lighten/Darken Colors:

```cpp
COLORREF LightenColor(COLORREF color, double factor)
{
    BYTE r = GetRValue(color);
    BYTE g = GetGValue(color);
    BYTE b = GetBValue(color);

    r = min(255, (int)(r + (255 - r) * factor));
    g = min(255, (int)(g + (255 - g) * factor));
    b = min(255, (int)(b + (255 - b) * factor));

    return RGB(r, g, b);
}

COLORREF DarkenColor(COLORREF color, double factor)
{
    BYTE r = GetRValue(color);
    BYTE g = GetGValue(color);
    BYTE b = GetBValue(color);

    r = (BYTE)(r * (1.0 - factor));
    g = (BYTE)(g * (1.0 - factor));
    b = (BYTE)(b * (1.0 - factor));

    return RGB(r, g, b);
}

void CMyView::OnDraw(CDC* pDC)
{
    COLORREF baseColor = RGB(0, 128, 255);

    int y = 50;
    int size = 50;

    // Original
    pDC->FillSolidRect(50, y, 100, size, baseColor);
    pDC->TextOut(160, y + 15, _T("Original"));
    y += size + 20;

    // Lightened
    for (double factor = 0.25; factor <= 0.75; factor += 0.25)
    {
        COLORREF lighter = LightenColor(baseColor, factor);
        pDC->FillSolidRect(50, y, 100, size, lighter);

        CString text;
        text.Format(_T("Lighten %.0f%%"), factor * 100);
        pDC->TextOut(160, y + 15, text);
        y += size + 20;
    }

    // Darkened
    for (double factor = 0.25; factor <= 0.75; factor += 0.25)
    {
        COLORREF darker = DarkenColor(baseColor, factor);
        pDC->FillSolidRect(50, y, 100, size, darker);

        CString text;
        text.Format(_T("Darken %.0f%%"), factor * 100);
        pDC->TextOut(160, y + 15, text);
        y += size + 20;
    }
}
```

### Color Blending:

```cpp
COLORREF BlendColors(COLORREF color1, COLORREF color2, double ratio)
{
    BYTE r1 = GetRValue(color1);
    BYTE g1 = GetGValue(color1);
    BYTE b1 = GetBValue(color1);

    BYTE r2 = GetRValue(color2);
    BYTE g2 = GetGValue(color2);
    BYTE b2 = GetBValue(color2);

    BYTE r = (BYTE)(r1 * (1.0 - ratio) + r2 * ratio);
    BYTE g = (BYTE)(g1 * (1.0 - ratio) + g2 * ratio);
    BYTE b = (BYTE)(b1 * (1.0 - ratio) + b2 * ratio);

    return RGB(r, g, b);
}

void CMyView::OnDraw(CDC* pDC)
{
    COLORREF color1 = RGB(255, 0, 0);    // Red
    COLORREF color2 = RGB(0, 0, 255);    // Blue

    int steps = 10;
    int barWidth = 40;
    int x = 50;

    for (int i = 0; i <= steps; i++)
    {
        double ratio = i / (double)steps;
        COLORREF blended = BlendColors(color1, color2, ratio);

        pDC->FillSolidRect(x, 50, barWidth, 200, blended);
        x += barWidth;
    }

    pDC->SetBkMode(TRANSPARENT);
    pDC->TextOut(50, 260, _T("Red"));
    pDC->TextOut(400, 260, _T("Blue"));
}
```

### Grayscale Conversion:

```cpp
COLORREF ToGrayscale(COLORREF color)
{
    BYTE r = GetRValue(color);
    BYTE g = GetGValue(color);
    BYTE b = GetBValue(color);

    // Weighted average (closer to human perception)
    BYTE gray = (BYTE)(0.299 * r + 0.587 * g + 0.114 * b);

    return RGB(gray, gray, gray);
}

void CMyView::OnDraw(CDC* pDC)
{
    COLORREF colors[] = {
        RGB(255, 0, 0),    // Red
        RGB(0, 255, 0),    // Green
        RGB(0, 0, 255),    // Blue
        RGB(255, 255, 0),  // Yellow
        RGB(255, 128, 0)   // Orange
    };

    int x = 50;
    int y = 50;
    int size = 60;

    for (int i = 0; i < 5; i++)
    {
        // Original color
        pDC->FillSolidRect(x, y, size, size, colors[i]);

        // Grayscale version
        COLORREF gray = ToGrayscale(colors[i]);
        pDC->FillSolidRect(x, y + size + 10, size, size, gray);

        x += size + 10;
    }

    pDC->TextOut(50, y - 20, _T("Original Colors"));
    pDC->TextOut(50, y + size + size + 20, _T("Grayscale"));
}
```

---

## 6. System Colors

### GetSysColor Function:

```cpp
COLORREF GetSysColor(int nIndex);
```

### Common System Colors:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    struct SysColorInfo {
        int index;
        LPCTSTR name;
    };

    SysColorInfo colors[] = {
        {COLOR_WINDOW, _T("COLOR_WINDOW")},
        {COLOR_WINDOWTEXT, _T("COLOR_WINDOWTEXT")},
        {COLOR_BTNFACE, _T("COLOR_BTNFACE")},
        {COLOR_BTNSHADOW, _T("COLOR_BTNSHADOW")},
        {COLOR_BTNHIGHLIGHT, _T("COLOR_BTNHIGHLIGHT")},
        {COLOR_HIGHLIGHT, _T("COLOR_HIGHLIGHT")},
        {COLOR_HIGHLIGHTTEXT, _T("COLOR_HIGHLIGHTTEXT")},
        {COLOR_GRAYTEXT, _T("COLOR_GRAYTEXT")},
        {COLOR_ACTIVECAPTION, _T("COLOR_ACTIVECAPTION")},
        {COLOR_INACTIVECAPTION, _T("COLOR_INACTIVECAPTION")}
    };

    int y = 30;
    for (int i = 0; i < 10; i++)
    {
        COLORREF color = GetSysColor(colors[i].index);

        // Draw color swatch
        pDC->FillSolidRect(50, y, 60, 25, color);
        pDC->Rectangle(50, y, 110, y + 25);

        // Draw color name
        pDC->SetBkMode(TRANSPARENT);
        pDC->TextOut(120, y + 5, colors[i].name);

        // Draw RGB values
        CString rgb;
        rgb.Format(_T("RGB(%d, %d, %d)"),
                   GetRValue(color),
                   GetGValue(color),
                   GetBValue(color));
        pDC->TextOut(350, y + 5, rgb);

        y += 35;
    }
}
```

---

## 7. Color Palettes

### What are Palettes?

Palettes are used in 256-color (8-bit) display modes to map palette indices to actual RGB colors. Modern systems typically use 24-bit or 32-bit color (True Color), making palettes less common, but understanding them is still valuable for working with images and legacy systems.

### CPalette Class:

```cpp
class CPalette : public CGdiObject
{
public:
    CPalette();

    BOOL CreatePalette(LPLOGPALETTE lpLogPalette);
    BOOL CreateHalftonePalette(CDC* pDC);

    UINT GetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
                          LPPALETTEENTRY lpPaletteColors) const;
    UINT SetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
                          LPPALETTEENTRY lpPaletteColors);

    UINT GetNearestPaletteIndex(COLORREF crColor) const;
    operator HPALETTE() const;
};
```

---

## 8. Working with Palettes

### Creating a Simple Palette:

```cpp
void CMyView::CreateSimplePalette()
{
    // Define palette with 256 entries
    LOGPALETTE* pLogPal = (LOGPALETTE*)malloc(
        sizeof(LOGPALETTE) + 256 * sizeof(PALETTEENTRY));

    pLogPal->palVersion = 0x300;
    pLogPal->palNumEntries = 256;

    // Fill with grayscale
    for (int i = 0; i < 256; i++)
    {
        pLogPal->palPalEntry[i].peRed = i;
        pLogPal->palPalEntry[i].peGreen = i;
        pLogPal->palPalEntry[i].peBlue = i;
        pLogPal->palPalEntry[i].peFlags = PC_NOCOLLAPSE;
    }

    m_palette.CreatePalette(pLogPal);
    free(pLogPal);
}
```

### Using Halftone Palette:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Create halftone palette
    CPalette palette;
    palette.CreateHalftonePalette(pDC);

    // Select and realize palette
    CPalette* pOldPalette = pDC->SelectPalette(&palette, FALSE);
    pDC->RealizePalette();

    // Draw with palette
    for (int i = 0; i < 256; i++)
    {
        COLORREF color = PALETTERGB(i, 0, 255 - i);
        pDC->FillSolidRect(i * 2, 50, 2, 100, color);
    }

    // Restore
    pDC->SelectPalette(pOldPalette, FALSE);
}
```

---

## 9. Color Conversion

### RGB to HSL:

```cpp
void RGBtoHSL(COLORREF rgb, double& h, double& s, double& l)
{
    double r = GetRValue(rgb) / 255.0;
    double g = GetGValue(rgb) / 255.0;
    double b = GetBValue(rgb) / 255.0;

    double maxVal = max(max(r, g), b);
    double minVal = min(min(r, g), b);
    double delta = maxVal - minVal;

    // Lightness
    l = (maxVal + minVal) / 2.0;

    if (delta == 0)
    {
        // Achromatic
        h = 0;
        s = 0;
    }
    else
    {
        // Saturation
        if (l < 0.5)
            s = delta / (maxVal + minVal);
        else
            s = delta / (2.0 - maxVal - minVal);

        // Hue
        if (r == maxVal)
            h = (g - b) / delta;
        else if (g == maxVal)
            h = 2.0 + (b - r) / delta;
        else
            h = 4.0 + (r - g) / delta;

        h *= 60.0;
        if (h < 0)
            h += 360.0;
    }
}

COLORREF HSLtoRGB(double h, double s, double l)
{
    double r, g, b;

    if (s == 0)
    {
        // Achromatic
        r = g = b = l;
    }
    else
    {
        auto hue2rgb = [](double p, double q, double t)
        {
            if (t < 0) t += 1;
            if (t > 1) t -= 1;
            if (t < 1.0/6) return p + (q - p) * 6 * t;
            if (t < 1.0/2) return q;
            if (t < 2.0/3) return p + (q - p) * (2.0/3 - t) * 6;
            return p;
        };

        double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        double p = 2 * l - q;

        double h_normalized = h / 360.0;

        r = hue2rgb(p, q, h_normalized + 1.0/3);
        g = hue2rgb(p, q, h_normalized);
        b = hue2rgb(p, q, h_normalized - 1.0/3);
    }

    return RGB((BYTE)(r * 255), (BYTE)(g * 255), (BYTE)(b * 255));
}
```

### Color Wheel:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    int centerX = 200, centerY = 200;
    int radius = 150;

    // Draw color wheel
    for (int angle = 0; angle < 360; angle++)
    {
        COLORREF color = HSLtoRGB(angle, 1.0, 0.5);

        CPen pen(PS_SOLID, 3, color);
        CPen* pOldPen = pDC->SelectObject(&pen);

        double rad = angle * 3.14159 / 180.0;
        int x1 = centerX;
        int y1 = centerY;
        int x2 = centerX + (int)(radius * cos(rad));
        int y2 = centerY + (int)(radius * sin(rad));

        pDC->MoveTo(x1, y1);
        pDC->LineTo(x2, y2);

        pDC->SelectObject(pOldPen);
    }
}
```

---

## 10. Practical Examples

### Color Picker:

```cpp
class CColorPickerView : public CView
{
protected:
    COLORREF m_selectedColor;
    CRect m_colorRect;

public:
    CColorPickerView() : m_selectedColor(RGB(255, 0, 0)) {}

    void OnDraw(CDC* pDC) override
    {
        m_colorRect.SetRect(50, 50, 300, 100);

        // Draw color bars
        int barWidth = 25;
        int x = 50;
        int y = 150;

        for (int r = 0; r < 10; r++)
        {
            x = 50;
            for (int g = 0; g < 10; g++)
            {
                COLORREF color = RGB(r * 25, g * 25,
                                   GetBValue(m_selectedColor));
                pDC->FillSolidRect(x, y, barWidth, barWidth, color);
                x += barWidth;
            }
            y += barWidth;
        }

        // Display selected color
        pDC->FillSolidRect(&m_colorRect, m_selectedColor);
        pDC->Rectangle(&m_colorRect);

        CString info;
        info.Format(_T("RGB(%d, %d, %d)"),
                   GetRValue(m_selectedColor),
                   GetGValue(m_selectedColor),
                   GetBValue(m_selectedColor));
        pDC->TextOut(50, 20, info);
    }

    void OnLButtonDown(UINT nFlags, CPoint point) override
    {
        // Update color based on click position
        if (point.y >= 150 && point.y < 400 &&
            point.x >= 50 && point.x < 300)
        {
            int col = (point.x - 50) / 25;
            int row = (point.y - 150) / 25;

            if (col < 10 && row < 10)
            {
                m_selectedColor = RGB(row * 25, col * 25,
                                    GetBValue(m_selectedColor));
                Invalidate();
            }
        }
    }

    DECLARE_MESSAGE_MAP()
};
```

---

## 11. Exercises

### Exercise 1: RGB Gradient Generator

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    int width = rect.Width();
    int height = rect.Height() / 3;

    // Red gradient
    for (int x = 0; x < width; x++)
    {
        int value = (x * 255) / width;
        pDC->FillSolidRect(x, 0, 1, height, RGB(value, 0, 0));
    }

    // Green gradient
    for (int x = 0; x < width; x++)
    {
        int value = (x * 255) / width;
        pDC->FillSolidRect(x, height, 1, height, RGB(0, value, 0));
    }

    // Blue gradient
    for (int x = 0; x < width; x++)
    {
        int value = (x * 255) / width;
        pDC->FillSolidRect(x, height * 2, 1, height, RGB(0, 0, value));
    }
}
```

### Exercise 2: Color Mixer

```cpp
class CColorMixerView : public CView
{
protected:
    int m_red, m_green, m_blue;

public:
    CColorMixerView() : m_red(128), m_green(128), m_blue(128) {}

    void OnDraw(CDC* pDC) override
    {
        // Draw color sliders
        DrawSlider(pDC, 50, 50, m_red, _T("Red"));
        DrawSlider(pDC, 50, 120, m_green, _T("Green"));
        DrawSlider(pDC, 50, 190, m_blue, _T("Blue"));

        // Draw mixed color
        COLORREF mixed = RGB(m_red, m_green, m_blue);
        pDC->FillSolidRect(50, 280, 300, 100, mixed);
        pDC->Rectangle(50, 280, 350, 380);

        CString info;
        info.Format(_T("RGB(%d, %d, %d)"), m_red, m_green, m_blue);
        pDC->TextOut(130, 400, info);
    }

    void DrawSlider(CDC* pDC, int x, int y, int value, LPCTSTR label)
    {
        pDC->TextOut(x, y - 20, label);

        // Draw slider track
        pDC->Rectangle(x, y, x + 255, y + 20);

        // Draw slider thumb
        CBrush brush(RGB(100, 100, 255));
        CBrush* pOldBrush = pDC->SelectObject(&brush);
        pDC->Rectangle(x + value - 5, y - 5, x + value + 5, y + 25);
        pDC->SelectObject(pOldBrush);

        // Draw value
        CString val;
        val.Format(_T("%d"), value);
        pDC->TextOut(x + 270, y, val);
    }
};
```

---

## Key Takeaways

1. ✅ Colors use RGB model (0-255 per channel)
2. ✅ COLORREF stores colors as 0x00BBGGRR
3. ✅ Use RGB() macro to create colors
4. ✅ GetRValue/GetGValue/GetBValue extract components
5. ✅ System colors adapt to theme
6. ✅ Color blending creates gradients
7. ✅ HSL conversion useful for color manipulation
8. ✅ Palettes mainly for legacy compatibility

---

## Next Lesson Preview

In **Lesson 52: Bitmaps and Images**, we'll learn:
- Loading and displaying bitmaps
- CBitmap class
- BitBlt operations
- Bitmap manipulation
- Image formats

**Congratulations on completing Lesson 51!**
