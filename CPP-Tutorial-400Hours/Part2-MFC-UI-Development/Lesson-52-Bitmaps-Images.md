# Lesson 52: Bitmaps and Images

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding Bitmaps
2. CBitmap Class
3. Loading Bitmaps from Resources
4. Loading Bitmaps from Files
5. Displaying Bitmaps with BitBlt
6. StretchBlt for Scaling
7. Transparent Bitmaps
8. Creating Bitmaps Dynamically
9. Bitmap Information and Manipulation
10. Practical Examples
11. Exercises

---

## 1. Understanding Bitmaps

A **bitmap** is a raster graphics image format that stores pixel data. Each pixel contains color information, making bitmaps ideal for photographs and complex images.

### Bitmap Structure:

```
┌─────────────────────────────┐
│    Bitmap File (.bmp)       │
├─────────────────────────────┤
│  File Header                │
│  Info Header                │
│  Color Palette (optional)   │
│  Pixel Data                 │
└─────────────────────────────┘
```

### Key Concepts:

- **Width × Height**: Image dimensions in pixels
- **Bits Per Pixel (BPP)**: Color depth (1, 4, 8, 16, 24, 32)
- **Device-Dependent Bitmap (DDB)**: System-specific format
- **Device-Independent Bitmap (DIB)**: Portable format

---

## 2. CBitmap Class

### CBitmap Overview:

```cpp
class CBitmap : public CGdiObject
{
public:
    CBitmap();

    // Loading methods
    BOOL LoadBitmap(UINT nIDResource);
    BOOL LoadBitmap(LPCTSTR lpszResourceName);
    BOOL LoadOEMBitmap(UINT nIDBitmap);

    // Creation methods
    BOOL CreateBitmap(int nWidth, int nHeight,
                     UINT nPlanes, UINT nBitcount,
                     const void* lpBits);
    BOOL CreateCompatibleBitmap(CDC* pDC, int nWidth, int nHeight);

    // Information
    int GetBitmap(BITMAP* pBitMap);

    // Operations
    DWORD SetBitmapBits(DWORD dwCount, const void* lpBits);
    DWORD GetBitmapBits(DWORD dwCount, LPVOID lpBits) const;

    operator HBITMAP() const;
};
```

---

## 3. Loading Bitmaps from Resources

### Step 1: Add Bitmap to Resources

1. In Visual Studio, open Resource View
2. Right-click on project → Add → Resource
3. Select Bitmap → Import
4. Choose bitmap file (IDB_MYBITMAP assigned)

### Step 2: Load and Display:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Load bitmap from resources
    CBitmap bitmap;
    bitmap.LoadBitmap(IDB_MYBITMAP);

    // Create memory DC
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);

    // Select bitmap into memory DC
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    // Get bitmap dimensions
    BITMAP bm;
    bitmap.GetBitmap(&bm);

    // Copy to screen
    pDC->BitBlt(50, 50,                    // Destination
                bm.bmWidth, bm.bmHeight,   // Size
                &memDC,                    // Source DC
                0, 0,                      // Source position
                SRCCOPY);                  // Raster operation

    // Cleanup
    memDC.SelectObject(pOldBitmap);
}
```

### Complete Example with Error Handling:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CBitmap bitmap;
    if (!bitmap.LoadBitmap(IDB_MYBITMAP))
    {
        AfxMessageBox(_T("Failed to load bitmap!"));
        return;
    }

    CDC memDC;
    if (!memDC.CreateCompatibleDC(pDC))
    {
        AfxMessageBox(_T("Failed to create memory DC!"));
        return;
    }

    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
    if (!pOldBitmap)
    {
        AfxMessageBox(_T("Failed to select bitmap!"));
        return;
    }

    BITMAP bm;
    bitmap.GetBitmap(&bm);

    pDC->BitBlt(50, 50, bm.bmWidth, bm.bmHeight,
                &memDC, 0, 0, SRCCOPY);

    memDC.SelectObject(pOldBitmap);
}
```

---

## 4. Loading Bitmaps from Files

### Using LoadImage API:

```cpp
HBITMAP LoadBitmapFromFile(LPCTSTR filename)
{
    return (HBITMAP)::LoadImage(
        NULL,                       // hInstance (NULL for file)
        filename,                   // Filename
        IMAGE_BITMAP,               // Type
        0, 0,                       // Width, Height (0 = original)
        LR_LOADFROMFILE |          // Load from file
        LR_CREATEDIBSECTION);      // Create DIB section
}

void CMyView::OnDraw(CDC* pDC)
{
    HBITMAP hBitmap = LoadBitmapFromFile(_T("C:\\Images\\photo.bmp"));

    if (hBitmap)
    {
        CBitmap bitmap;
        bitmap.Attach(hBitmap);  // Attach handle to CBitmap

        CDC memDC;
        memDC.CreateCompatibleDC(pDC);
        CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

        BITMAP bm;
        bitmap.GetBitmap(&bm);

        pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight,
                    &memDC, 0, 0, SRCCOPY);

        memDC.SelectObject(pOldBitmap);
        // bitmap.Detach(); handled by destructor
    }
}
```

### Using CImage (Easier Method):

```cpp
#include <atlimage.h>

void CMyView::OnDraw(CDC* pDC)
{
    CImage image;

    if (SUCCEEDED(image.Load(_T("C:\\Images\\photo.bmp"))))
    {
        // Draw image
        image.Draw(pDC->m_hDC, 50, 50);

        // Or draw with size
        // image.Draw(pDC->m_hDC, 50, 50, 200, 150);
    }
    else
    {
        AfxMessageBox(_T("Failed to load image!"));
    }
}
```

---

## 5. Displaying Bitmaps with BitBlt

### BitBlt Function:

```cpp
BOOL BitBlt(int x, int y, int nWidth, int nHeight,
            CDC* pSrcDC, int xSrc, int ySrc, DWORD dwRop);
```

### Raster Operations (ROP):

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CBitmap bitmap;
    bitmap.LoadBitmap(IDB_MYBITMAP);

    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    BITMAP bm;
    bitmap.GetBitmap(&bm);

    int y = 50;
    int spacing = bm.bmHeight + 30;

    // SRCCOPY - Copy source to destination
    pDC->BitBlt(50, y, bm.bmWidth, bm.bmHeight,
                &memDC, 0, 0, SRCCOPY);
    pDC->TextOut(50, y - 20, _T("SRCCOPY"));
    y += spacing;

    // SRCPAINT - OR source and destination
    pDC->BitBlt(50, y, bm.bmWidth, bm.bmHeight,
                &memDC, 0, 0, SRCPAINT);
    pDC->TextOut(50, y - 20, _T("SRCPAINT (OR)"));
    y += spacing;

    // SRCAND - AND source and destination
    pDC->BitBlt(50, y, bm.bmWidth, bm.bmHeight,
                &memDC, 0, 0, SRCAND);
    pDC->TextOut(50, y - 20, _T("SRCAND (AND)"));
    y += spacing;

    // SRCINVERT - XOR source and destination
    pDC->BitBlt(50, y, bm.bmWidth, bm.bmHeight,
                &memDC, 0, 0, SRCINVERT);
    pDC->TextOut(50, y - 20, _T("SRCINVERT (XOR)"));
    y += spacing;

    // NOTSRCCOPY - Copy inverted source
    pDC->BitBlt(50, y, bm.bmWidth, bm.bmHeight,
                &memDC, 0, 0, NOTSRCCOPY);
    pDC->TextOut(50, y - 20, _T("NOTSRCCOPY (Inverted)"));

    memDC.SelectObject(pOldBitmap);
}
```

### Partial Bitmap Display:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CBitmap bitmap;
    bitmap.LoadBitmap(IDB_LARGEBITMAP);

    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    // Display top-left 100x100 portion at (50, 50)
    pDC->BitBlt(50, 50, 100, 100,
                &memDC, 0, 0, SRCCOPY);

    // Display center 100x100 portion at (200, 50)
    pDC->BitBlt(200, 50, 100, 100,
                &memDC, 150, 150, SRCCOPY);

    memDC.SelectObject(pOldBitmap);
}
```

---

## 6. StretchBlt for Scaling

### StretchBlt Function:

```cpp
BOOL StretchBlt(int x, int y, int nWidth, int nHeight,
                CDC* pSrcDC, int xSrc, int ySrc,
                int nSrcWidth, int nSrcHeight, DWORD dwRop);
```

### Scaling Example:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CBitmap bitmap;
    bitmap.LoadBitmap(IDB_MYBITMAP);

    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    BITMAP bm;
    bitmap.GetBitmap(&bm);

    // Original size
    pDC->BitBlt(50, 50, bm.bmWidth, bm.bmHeight,
                &memDC, 0, 0, SRCCOPY);
    pDC->TextOut(50, 30, _T("Original"));

    // 50% size
    pDC->StretchBlt(200, 50,
                    bm.bmWidth / 2, bm.bmHeight / 2,
                    &memDC, 0, 0, bm.bmWidth, bm.bmHeight,
                    SRCCOPY);
    pDC->TextOut(200, 30, _T("50%"));

    // 200% size
    pDC->StretchBlt(350, 50,
                    bm.bmWidth * 2, bm.bmHeight * 2,
                    &memDC, 0, 0, bm.bmWidth, bm.bmHeight,
                    SRCCOPY);
    pDC->TextOut(350, 30, _T("200%"));

    memDC.SelectObject(pOldBitmap);
}
```

### Stretch Mode:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CBitmap bitmap;
    bitmap.LoadBitmap(IDB_MYBITMAP);

    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    BITMAP bm;
    bitmap.GetBitmap(&bm);

    int y = 50;

    // BLACKONWHITE - Fastest, lowest quality
    pDC->SetStretchBltMode(BLACKONWHITE);
    pDC->StretchBlt(50, y, 200, 150,
                    &memDC, 0, 0, bm.bmWidth, bm.bmHeight,
                    SRCCOPY);
    pDC->TextOut(50, y - 20, _T("BLACKONWHITE"));
    y += 170;

    // COLORONCOLOR - Good balance
    pDC->SetStretchBltMode(COLORONCOLOR);
    pDC->StretchBlt(50, y, 200, 150,
                    &memDC, 0, 0, bm.bmWidth, bm.bmHeight,
                    SRCCOPY);
    pDC->TextOut(50, y - 20, _T("COLORONCOLOR"));
    y += 170;

    // HALFTONE - Best quality, slowest
    pDC->SetStretchBltMode(HALFTONE);
    pDC->SetBrushOrg(0, 0);  // Required for HALFTONE
    pDC->StretchBlt(50, y, 200, 150,
                    &memDC, 0, 0, bm.bmWidth, bm.bmHeight,
                    SRCCOPY);
    pDC->TextOut(50, y - 20, _T("HALFTONE"));

    memDC.SelectObject(pOldBitmap);
}
```

---

## 7. Transparent Bitmaps

### TransparentBlt Function:

```cpp
BOOL TransparentBlt(int xDest, int yDest,
                   int nDestWidth, int nDestHeight,
                   CDC* pSrcDC,
                   int xSrc, int ySrc,
                   int nSrcWidth, int nSrcHeight,
                   UINT crTransparent);
```

### Example with Transparent Background:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Draw background
    pDC->FillSolidRect(0, 0, 600, 400, RGB(200, 200, 255));

    // Load sprite with magenta background
    CBitmap bitmap;
    bitmap.LoadBitmap(IDB_SPRITE);

    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    BITMAP bm;
    bitmap.GetBitmap(&bm);

    // Draw without transparency
    pDC->BitBlt(50, 50, bm.bmWidth, bm.bmHeight,
                &memDC, 0, 0, SRCCOPY);
    pDC->TextOut(50, 30, _T("With Background"));

    // Draw with transparency (magenta = RGB(255, 0, 255))
    pDC->TransparentBlt(200, 50, bm.bmWidth, bm.bmHeight,
                        &memDC, 0, 0, bm.bmWidth, bm.bmHeight,
                        RGB(255, 0, 255));
    pDC->TextOut(200, 30, _T("Transparent"));

    memDC.SelectObject(pOldBitmap);
}
```

### Manual Transparent Blitting (for older systems):

```cpp
void DrawTransparentBitmap(CDC* pDC, int x, int y,
                          CBitmap* pBitmap, COLORREF crTransparent)
{
    CDC memDC, maskDC;
    memDC.CreateCompatibleDC(pDC);
    maskDC.CreateCompatibleDC(pDC);

    BITMAP bm;
    pBitmap->GetBitmap(&bm);

    // Select source bitmap
    CBitmap* pOldMemBitmap = memDC.SelectObject(pBitmap);

    // Create mask bitmap
    CBitmap maskBitmap;
    maskBitmap.CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
    CBitmap* pOldMaskBitmap = maskDC.SelectObject(&maskBitmap);

    // Create mask
    memDC.SetBkColor(crTransparent);
    maskDC.BitBlt(0, 0, bm.bmWidth, bm.bmHeight,
                  &memDC, 0, 0, SRCCOPY);

    // Draw masked bitmap
    pDC->BitBlt(x, y, bm.bmWidth, bm.bmHeight,
                &memDC, 0, 0, SRCINVERT);
    pDC->BitBlt(x, y, bm.bmWidth, bm.bmHeight,
                &maskDC, 0, 0, SRCAND);
    pDC->BitBlt(x, y, bm.bmWidth, bm.bmHeight,
                &memDC, 0, 0, SRCINVERT);

    // Cleanup
    memDC.SelectObject(pOldMemBitmap);
    maskDC.SelectObject(pOldMaskBitmap);
}
```

---

## 8. Creating Bitmaps Dynamically

### Creating Compatible Bitmap:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    // Create bitmap
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);

    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(pDC, 200, 200);

    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    // Draw into bitmap
    memDC.FillSolidRect(0, 0, 200, 200, RGB(255, 255, 200));

    CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
    CPen* pOldPen = memDC.SelectObject(&pen);

    for (int i = 0; i < 200; i += 20)
    {
        memDC.MoveTo(0, i);
        memDC.LineTo(200, i);
        memDC.MoveTo(i, 0);
        memDC.LineTo(i, 200);
    }

    memDC.SelectObject(pOldPen);

    // Copy to screen
    pDC->BitBlt(50, 50, 200, 200, &memDC, 0, 0, SRCCOPY);

    memDC.SelectObject(pOldBitmap);
}
```

### Creating from Pixel Data:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    int width = 100, height = 100;
    BYTE* pixels = new BYTE[width * height * 3];  // 24-bit RGB

    // Fill with gradient
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = (y * width + x) * 3;
            pixels[index + 0] = (BYTE)((x * 255) / width);   // Blue
            pixels[index + 1] = (BYTE)((y * 255) / height);  // Green
            pixels[index + 2] = 128;                          // Red
        }
    }

    CBitmap bitmap;
    bitmap.CreateBitmap(width, height, 1, 24, pixels);

    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    pDC->BitBlt(50, 50, width, height, &memDC, 0, 0, SRCCOPY);

    memDC.SelectObject(pOldBitmap);
    delete[] pixels;
}
```

---

## 9. Bitmap Information and Manipulation

### Getting Bitmap Information:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CBitmap bitmap;
    bitmap.LoadBitmap(IDB_MYBITMAP);

    BITMAP bm;
    bitmap.GetBitmap(&bm);

    int y = 50;
    CString info;

    info.Format(_T("Width: %d pixels"), bm.bmWidth);
    pDC->TextOut(50, y, info);
    y += 25;

    info.Format(_T("Height: %d pixels"), bm.bmHeight);
    pDC->TextOut(50, y, info);
    y += 25;

    info.Format(_T("Bits Per Pixel: %d"), bm.bmBitsPixel);
    pDC->TextOut(50, y, info);
    y += 25;

    info.Format(_T("Planes: %d"), bm.bmPlanes);
    pDC->TextOut(50, y, info);
    y += 25;

    info.Format(_T("Width Bytes: %d"), bm.bmWidthBytes);
    pDC->TextOut(50, y, info);
}
```

### Modifying Bitmap Pixels:

```cpp
void InvertBitmap(CBitmap* pBitmap)
{
    BITMAP bm;
    pBitmap->GetBitmap(&bm);

    int size = bm.bmWidthBytes * bm.bmHeight;
    BYTE* pixels = new BYTE[size];

    // Get pixel data
    pBitmap->GetBitmapBits(size, pixels);

    // Invert all pixels
    for (int i = 0; i < size; i++)
    {
        pixels[i] = 255 - pixels[i];
    }

    // Set modified pixels
    pBitmap->SetBitmapBits(size, pixels);

    delete[] pixels;
}

void CMyView::OnDraw(CDC* pDC)
{
    CBitmap bitmap;
    bitmap.LoadBitmap(IDB_MYBITMAP);

    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    BITMAP bm;
    bitmap.GetBitmap(&bm);

    // Draw original
    pDC->BitBlt(50, 50, bm.bmWidth, bm.bmHeight,
                &memDC, 0, 0, SRCCOPY);
    pDC->TextOut(50, 30, _T("Original"));

    // Invert and draw
    memDC.SelectObject(pOldBitmap);
    InvertBitmap(&bitmap);
    pOldBitmap = memDC.SelectObject(&bitmap);

    pDC->BitBlt(200, 50, bm.bmWidth, bm.bmHeight,
                &memDC, 0, 0, SRCCOPY);
    pDC->TextOut(200, 30, _T("Inverted"));

    memDC.SelectObject(pOldBitmap);
}
```

---

## 10. Practical Examples

### Image Viewer:

```cpp
class CImageView : public CView
{
protected:
    CImage m_image;
    CString m_filename;
    double m_zoom;

public:
    CImageView() : m_zoom(1.0) {}

    void LoadImage(LPCTSTR filename)
    {
        m_image.Destroy();
        if (SUCCEEDED(m_image.Load(filename)))
        {
            m_filename = filename;
            Invalidate();
        }
    }

    void OnDraw(CDC* pDC) override
    {
        if (m_image.IsNull())
            return;

        int width = (int)(m_image.GetWidth() * m_zoom);
        int height = (int)(m_image.GetHeight() * m_zoom);

        pDC->SetStretchBltMode(HALFTONE);
        m_image.StretchBlt(pDC->m_hDC, 0, 0, width, height);

        // Draw info
        CString info;
        info.Format(_T("%s - %dx%d - Zoom: %.0f%%"),
                   m_filename,
                   m_image.GetWidth(),
                   m_image.GetHeight(),
                   m_zoom * 100);
        pDC->SetBkMode(TRANSPARENT);
        pDC->TextOut(10, 10, info);
    }

    void ZoomIn() { m_zoom *= 1.25; Invalidate(); }
    void ZoomOut() { m_zoom *= 0.8; Invalidate(); }
};
```

### Sprite Sheet Animation:

```cpp
class CSpriteView : public CView
{
protected:
    CBitmap m_spriteSheet;
    int m_currentFrame;
    int m_frameWidth, m_frameHeight;
    int m_framesPerRow;
    int m_totalFrames;

public:
    CSpriteView() : m_currentFrame(0) {}

    BOOL LoadSpriteSheet(UINT nID, int frameWidth, int frameHeight,
                        int framesPerRow, int totalFrames)
    {
        if (!m_spriteSheet.LoadBitmap(nID))
            return FALSE;

        m_frameWidth = frameWidth;
        m_frameHeight = frameHeight;
        m_framesPerRow = framesPerRow;
        m_totalFrames = totalFrames;

        return TRUE;
    }

    void OnDraw(CDC* pDC) override
    {
        if (!m_spriteSheet.GetSafeHandle())
            return;

        CDC memDC;
        memDC.CreateCompatibleDC(pDC);
        CBitmap* pOldBitmap = memDC.SelectObject(&m_spriteSheet);

        // Calculate source position
        int row = m_currentFrame / m_framesPerRow;
        int col = m_currentFrame % m_framesPerRow;

        int srcX = col * m_frameWidth;
        int srcY = row * m_frameHeight;

        // Draw current frame
        pDC->BitBlt(100, 100, m_frameWidth, m_frameHeight,
                    &memDC, srcX, srcY, SRCCOPY);

        memDC.SelectObject(pOldBitmap);
    }

    void NextFrame()
    {
        m_currentFrame = (m_currentFrame + 1) % m_totalFrames;
        Invalidate();
    }
};
```

---

## 11. Exercises

### Exercise 1: Bitmap Tiler

```cpp
void TileBitmap(CDC* pDC, CBitmap* pBitmap, CRect rect)
{
    BITMAP bm;
    pBitmap->GetBitmap(&bm);

    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap* pOldBitmap = memDC.SelectObject(pBitmap);

    for (int y = rect.top; y < rect.bottom; y += bm.bmHeight)
    {
        for (int x = rect.left; x < rect.right; x += bm.bmWidth)
        {
            pDC->BitBlt(x, y, bm.bmWidth, bm.bmHeight,
                        &memDC, 0, 0, SRCCOPY);
        }
    }

    memDC.SelectObject(pOldBitmap);
}
```

### Exercise 2: Bitmap Effects

```cpp
void ApplyGrayscale(CBitmap* pBitmap)
{
    BITMAP bm;
    pBitmap->GetBitmap(&bm);

    if (bm.bmBitsPixel != 24)
        return;

    int size = bm.bmWidthBytes * bm.bmHeight;
    BYTE* pixels = new BYTE[size];

    pBitmap->GetBitmapBits(size, pixels);

    for (int i = 0; i < size; i += 3)
    {
        BYTE gray = (BYTE)(pixels[i] * 0.114 +      // Blue
                          pixels[i + 1] * 0.587 +   // Green
                          pixels[i + 2] * 0.299);   // Red

        pixels[i] = pixels[i + 1] = pixels[i + 2] = gray;
    }

    pBitmap->SetBitmapBits(size, pixels);
    delete[] pixels;
}
```

---

## Key Takeaways

1. ✅ CBitmap wraps Windows HBITMAP
2. ✅ Use LoadBitmap for resources, LoadImage for files
3. ✅ BitBlt for copying, StretchBlt for scaling
4. ✅ Memory DC required for bitmap operations
5. ✅ TransparentBlt for transparent backgrounds
6. ✅ Always restore original bitmap in DC
7. ✅ CImage simpler for file loading
8. ✅ Use HALFTONE mode for best quality scaling

---

## Next Lesson Preview

In **Lesson 53: Double Buffering**, we'll learn:
- Preventing flicker
- Memory DC techniques
- Implementing double buffering
- Optimization strategies

**Congratulations on completing Lesson 52!**
