# Lesson 40: Fonts and Colors

## Introduction
Master font creation, text rendering, and color management in Windows GDI.

**Topics Covered:**
- CreateFont and LOGFONT structure
- Font metrics and measurement
- Text colors (foreground and background)
- System colors
- RGB color values
- Font anti-aliasing (ClearType)
- Enumerating fonts
- Loading custom fonts

**Time Required:** 4-5 hours

## Creating Fonts

### CreateFont Function
```cpp
HFONT hFont = CreateFont(
    20,                   // Height
    0,                    // Width (0 = default)
    0,                    // Escapement
    0,                    // Orientation
    FW_NORMAL,            // Weight
    FALSE,                // Italic
    FALSE,                // Underline
    FALSE,                // Strikeout
    DEFAULT_CHARSET,      // Character set
    OUT_DEFAULT_PRECIS,   // Output precision
    CLIP_DEFAULT_PRECIS,  // Clipping precision
    DEFAULT_QUALITY,      // Quality
    DEFAULT_PITCH | FF_DONTCARE,  // Pitch and family
    L"Arial"              // Font name
);

HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
TextOut(hdc, 10, 10, L"Custom Font", 11);
SelectObject(hdc, hOldFont);
DeleteObject(hFont);
```

### Using LOGFONT
```cpp
LOGFONT lf = {};
lf.lfHeight = 24;
lf.lfWeight = FW_BOLD;
wcscpy_s(lf.lfFaceName, L"Times New Roman");
HFONT hFont = CreateFontIndirect(&lf);
```

## Font Metrics
```cpp
TEXTMETRIC tm;
GetTextMetrics(hdc, &tm);

// tm.tmHeight - Total character height
// tm.tmAscent - Ascender height
// tm.tmDescent - Descender height
// tm.tmAveCharWidth - Average character width
```

## Text Colors
```cpp
// Set text color
SetTextColor(hdc, RGB(255, 0, 0));  // Red text

// Set background color
SetBkColor(hdc, RGB(255, 255, 0));  // Yellow background

// Transparent background
SetBkMode(hdc, TRANSPARENT);
```

## System Colors
```cpp
COLORREF clr = GetSysColor(COLOR_WINDOW);        // Window background
COLORREF clr = GetSysColor(COLOR_WINDOWTEXT);    // Window text
COLORREF clr = GetSysColor(COLOR_HIGHLIGHT);     // Selection background
```

## RGB Colors
```cpp
COLORREF red = RGB(255, 0, 0);
COLORREF green = RGB(0, 255, 0);
COLORREF blue = RGB(0, 0, 255);
COLORREF custom = RGB(128, 64, 192);

// Extract components
BYTE r = GetRValue(color);
BYTE g = GetGValue(color);
BYTE b = GetBValue(color);
```

## See Code Examples
8 complete font and color demonstrations in Code/ folder.
