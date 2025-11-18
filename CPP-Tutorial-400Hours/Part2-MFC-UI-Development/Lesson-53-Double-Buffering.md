# Lesson 53: Double Buffering to Prevent Flicker

**Duration**: 5 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding Flicker
2. What is Double Buffering?
3. Basic Double Buffering Implementation
4. Optimized Double Buffering
5. CMemDC Class
6. Buffered Paint API
7. Performance Considerations
8. Complete Examples
9. Exercises

---

## 1. Understanding Flicker

**Flicker** occurs when the screen is erased and redrawn repeatedly, creating a visible flashing effect.

### Common Causes of Flicker:

1. **Background Erasure**: WM_ERASEBKGND clears background before redrawing
2. **Multiple Drawing Operations**: Each operation updates screen immediately
3. **Complex Graphics**: Slow drawing routines cause visible delays
4. **Frequent Updates**: Animation or real-time data display

### Example of Flickering Code:

```cpp
// ❌ BAD - This will flicker
void CMyView::OnDraw(CDC* pDC)
{
    // Background erased by default
    // Each shape drawn directly to screen

    for (int i = 0; i < 1000; i++)
    {
        CBrush brush(RGB(rand() % 256, rand() % 256, rand() % 256));
        CBrush* pOldBrush = pDC->SelectObject(&brush);

        int x = rand() % 800;
        int y = rand() % 600;

        pDC->Ellipse(x, y, x + 50, y + 50);
        pDC->SelectObject(pOldBrush);
    }
}
```

---

## 2. What is Double Buffering?

**Double buffering** draws everything to an off-screen bitmap first, then copies the complete image to the screen in one operation.

### How Double Buffering Works:

```
┌──────────────────────────────┐
│  1. Create Memory DC         │
│  2. Create Bitmap            │
│  3. Select Bitmap into DC    │
│  4. Draw to Memory DC        │
│  5. BitBlt to Screen         │
└──────────────────────────────┘
```

### Benefits:

- ✅ **No Flicker**: Single screen update
- ✅ **Smooth Animation**: Clean frame transitions
- ✅ **Professional Appearance**: Better user experience
- ✅ **Complex Graphics**: Handle intensive operations

---

## 3. Basic Double Buffering Implementation

### Simple Double Buffer:

```cpp
void CMyView::OnDraw(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    // Create memory DC
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);

    // Create bitmap
    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

    // Select bitmap into memory DC
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    // Draw to memory DC instead of screen
    memDC.FillSolidRect(&rect, RGB(255, 255, 255));

    // Draw content
    for (int i = 0; i < 100; i++)
    {
        CBrush brush(RGB(rand() % 256, rand() % 256, rand() % 256));
        CBrush* pOldBrush = memDC.SelectObject(&brush);

        int x = rand() % rect.Width();
        int y = rand() % rect.Height();

        memDC.Ellipse(x, y, x + 50, y + 50);
        memDC.SelectObject(pOldBrush);
    }

    // Copy entire buffer to screen in ONE operation
    pDC->BitBlt(0, 0, rect.Width(), rect.Height(),
                &memDC, 0, 0, SRCCOPY);

    // Cleanup
    memDC.SelectObject(pOldBitmap);
}
```

### Preventing Background Erasure:

```cpp
// In header file
class CMyView : public CView
{
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    DECLARE_MESSAGE_MAP()
};

// In implementation file
BEGIN_MESSAGE_MAP(CMyView, CView)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CMyView::OnEraseBkgnd(CDC* pDC)
{
    // Don't erase background - prevents flicker
    return TRUE;  // Return TRUE = "I handled it"
}

void CMyView::OnDraw(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    CDC memDC;
    memDC.CreateCompatibleDC(pDC);

    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    // Fill background in memory DC
    memDC.FillSolidRect(&rect, RGB(240, 240, 240));

    // ... draw content ...

    pDC->BitBlt(0, 0, rect.Width(), rect.Height(),
                &memDC, 0, 0, SRCCOPY);

    memDC.SelectObject(pOldBitmap);
}
```

---

## 4. Optimized Double Buffering

### Reusable Buffer Class:

```cpp
// DoubleBuffer.h
class CDoubleBuffer
{
protected:
    CDC m_memDC;
    CBitmap m_bitmap;
    CSize m_size;

public:
    CDoubleBuffer();
    ~CDoubleBuffer();

    CDC* GetMemDC(CDC* pDC, int width, int height);
    void Flush(CDC* pDC, int x = 0, int y = 0);
    void Clear();
};

// DoubleBuffer.cpp
CDoubleBuffer::CDoubleBuffer()
{
    m_size.cx = m_size.cy = 0;
}

CDoubleBuffer::~CDoubleBuffer()
{
    Clear();
}

CDC* CDoubleBuffer::GetMemDC(CDC* pDC, int width, int height)
{
    // Only recreate if size changed
    if (width != m_size.cx || height != m_size.cy)
    {
        Clear();

        m_memDC.CreateCompatibleDC(pDC);
        m_bitmap.CreateCompatibleBitmap(pDC, width, height);
        m_memDC.SelectObject(&m_bitmap);

        m_size.cx = width;
        m_size.cy = height;
    }

    return &m_memDC;
}

void CDoubleBuffer::Flush(CDC* pDC, int x, int y)
{
    pDC->BitBlt(x, y, m_size.cx, m_size.cy,
                &m_memDC, 0, 0, SRCCOPY);
}

void CDoubleBuffer::Clear()
{
    if (m_memDC.GetSafeHdc())
    {
        m_memDC.SelectObject((CBitmap*)NULL);
        m_bitmap.DeleteObject();
        m_memDC.DeleteDC();
    }

    m_size.cx = m_size.cy = 0;
}

// Usage in View
class CMyView : public CView
{
protected:
    CDoubleBuffer m_buffer;

    void OnDraw(CDC* pDC) override
    {
        CRect rect;
        GetClientRect(&rect);

        CDC* pMemDC = m_buffer.GetMemDC(pDC, rect.Width(), rect.Height());

        // Draw to pMemDC
        pMemDC->FillSolidRect(&rect, RGB(255, 255, 255));
        // ... more drawing ...

        // Flush to screen
        m_buffer.Flush(pDC);
    }
};
```

---

## 5. CMemDC Class

Windows provides `CMemDC` class for automatic double buffering (Vista and later):

```cpp
#include <afxwin.h>

void CMyView::OnPaint()
{
    CPaintDC dc(this);
    CRect rect;
    GetClientRect(&rect);

    // CMemDC automatically handles double buffering
    CMemDC memDC(dc, this);
    CDC* pDC = &memDC.GetDC();

    // Draw to memory DC
    pDC->FillSolidRect(&rect, RGB(255, 255, 255));

    CPen pen(PS_SOLID, 2, RGB(0, 0, 255));
    CPen* pOldPen = pDC->SelectObject(&pen);

    for (int i = 0; i < 100; i++)
    {
        pDC->MoveTo(i * 10, 0);
        pDC->LineTo(i * 10, rect.Height());
    }

    pDC->SelectObject(pOldPen);

    // Automatic BitBlt when memDC destructor is called
}
```

### Custom CMemDC Implementation (for older systems):

```cpp
class CMemoryDC : public CDC
{
protected:
    CBitmap m_bitmap;
    CBitmap* m_pOldBitmap;
    CDC* m_pDC;
    CRect m_rect;

public:
    CMemoryDC(CDC* pDC, CRect* pRect = NULL) : CDC()
    {
        ASSERT(pDC != NULL);

        m_pDC = pDC;
        m_pOldBitmap = NULL;

        if (pRect == NULL)
            pDC->GetClipBox(&m_rect);
        else
            m_rect = *pRect;

        CreateCompatibleDC(m_pDC);

        m_bitmap.CreateCompatibleBitmap(m_pDC,
                                        m_rect.Width(),
                                        m_rect.Height());

        m_pOldBitmap = SelectObject(&m_bitmap);

        SetWindowOrg(m_rect.left, m_rect.top);
    }

    ~CMemoryDC()
    {
        m_pDC->BitBlt(m_rect.left, m_rect.top,
                      m_rect.Width(), m_rect.Height(),
                      this, m_rect.left, m_rect.top, SRCCOPY);

        SelectObject(m_pOldBitmap);
    }
};

// Usage
void CMyView::OnDraw(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    CMemoryDC memDC(pDC, &rect);

    // Draw to memDC
    memDC.FillSolidRect(&rect, RGB(255, 255, 255));
    // ... more drawing ...

    // Automatic flush on destruction
}
```

---

## 6. Buffered Paint API

Windows 7 and later provide Buffered Paint API:

```cpp
#include <uxtheme.h>
#pragma comment(lib, "uxtheme.lib")

void CMyView::OnPaint()
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hWnd, &ps);

    // Begin buffered paint
    HDC hdcPaint = NULL;
    HPAINTBUFFER hBufferedPaint = BeginBufferedPaint(hdc,
                                                      &ps.rcPaint,
                                                      BPBF_COMPATIBLEBITMAP,
                                                      NULL,
                                                      &hdcPaint);

    if (hBufferedPaint && hdcPaint)
    {
        // Draw to hdcPaint
        CDC dc;
        dc.Attach(hdcPaint);

        CRect rect(ps.rcPaint);
        dc.FillSolidRect(&rect, RGB(255, 255, 255));

        // ... more drawing ...

        dc.Detach();

        // End buffered paint (automatic BitBlt)
        EndBufferedPaint(hBufferedPaint, TRUE);
    }

    EndPaint(m_hWnd, &ps);
}
```

---

## 7. Performance Considerations

### Optimization Tips:

```cpp
class COptimizedView : public CView
{
protected:
    CDC m_memDC;
    CBitmap m_bitmap;
    CSize m_bufferSize;
    BOOL m_bBufferValid;

public:
    COptimizedView() : m_bBufferValid(FALSE) {}

    void InvalidateBuffer()
    {
        m_bBufferValid = FALSE;
    }

    void OnDraw(CDC* pDC) override
    {
        CRect rect;
        GetClientRect(&rect);

        // Create buffer if needed or size changed
        if (!m_bBufferValid ||
            rect.Width() != m_bufferSize.cx ||
            rect.Height() != m_bufferSize.cy)
        {
            CreateBuffer(pDC, rect.Width(), rect.Height());
            DrawToBuffer();
            m_bBufferValid = TRUE;
        }

        // Just copy buffer to screen
        pDC->BitBlt(0, 0, rect.Width(), rect.Height(),
                    &m_memDC, 0, 0, SRCCOPY);
    }

protected:
    void CreateBuffer(CDC* pDC, int width, int height)
    {
        if (m_memDC.GetSafeHdc())
        {
            m_memDC.SelectObject((CBitmap*)NULL);
            m_bitmap.DeleteObject();
            m_memDC.DeleteDC();
        }

        m_memDC.CreateCompatibleDC(pDC);
        m_bitmap.CreateCompatibleBitmap(pDC, width, height);
        m_memDC.SelectObject(&m_bitmap);

        m_bufferSize.cx = width;
        m_bufferSize.cy = height;
    }

    void DrawToBuffer()
    {
        CRect rect(0, 0, m_bufferSize.cx, m_bufferSize.cy);

        // Draw to m_memDC
        m_memDC.FillSolidRect(&rect, RGB(255, 255, 255));

        // ... complex drawing operations ...
    }
};
```

### Dirty Rectangle Optimization:

```cpp
class CPartialUpdateView : public CView
{
protected:
    CDC m_memDC;
    CBitmap m_bitmap;
    CSize m_bufferSize;
    CRect m_dirtyRect;

public:
    void InvalidateRect(LPCRECT lpRect, BOOL bErase = TRUE)
    {
        m_dirtyRect.UnionRect(&m_dirtyRect, lpRect);
        CView::InvalidateRect(lpRect, bErase);
    }

    void OnDraw(CDC* pDC) override
    {
        CRect rect;
        GetClientRect(&rect);

        // Ensure buffer exists
        if (!m_memDC.GetSafeHdc())
        {
            CreateBuffer(pDC, rect.Width(), rect.Height());
            DrawEntireBuffer();
        }

        // Only redraw dirty area
        if (!m_dirtyRect.IsRectEmpty())
        {
            RedrawDirtyArea();
            m_dirtyRect.SetRectEmpty();
        }

        // Copy to screen
        pDC->BitBlt(0, 0, rect.Width(), rect.Height(),
                    &m_memDC, 0, 0, SRCCOPY);
    }

protected:
    void CreateBuffer(CDC* pDC, int width, int height);
    void DrawEntireBuffer();
    void RedrawDirtyArea();
};
```

---

## 8. Complete Examples

### Animated Ball (No Flicker):

```cpp
class CBallView : public CView
{
protected:
    CPoint m_ballPos;
    CPoint m_ballVel;
    int m_ballRadius;
    CDoubleBuffer m_buffer;

public:
    CBallView() : m_ballPos(100, 100), m_ballVel(5, 3), m_ballRadius(20)
    {
        SetTimer(1, 16, NULL);  // ~60 FPS
    }

    BOOL OnEraseBkgnd(CDC* pDC) override
    {
        return TRUE;  // Prevent flicker
    }

    void OnDraw(CDC* pDC) override
    {
        CRect rect;
        GetClientRect(&rect);

        // Get memory DC
        CDC* pMemDC = m_buffer.GetMemDC(pDC, rect.Width(), rect.Height());

        // Clear background
        pMemDC->FillSolidRect(&rect, RGB(255, 255, 255));

        // Draw ball
        CBrush brush(RGB(255, 0, 0));
        CBrush* pOldBrush = pMemDC->SelectObject(&brush);

        pMemDC->Ellipse(m_ballPos.x - m_ballRadius,
                       m_ballPos.y - m_ballRadius,
                       m_ballPos.x + m_ballRadius,
                       m_ballPos.y + m_ballRadius);

        pMemDC->SelectObject(pOldBrush);

        // Flush to screen
        m_buffer.Flush(pDC);
    }

    void OnTimer(UINT_PTR nIDEvent) override
    {
        CRect rect;
        GetClientRect(&rect);

        // Update position
        m_ballPos += m_ballVel;

        // Bounce off walls
        if (m_ballPos.x - m_ballRadius < 0 ||
            m_ballPos.x + m_ballRadius > rect.Width())
        {
            m_ballVel.x = -m_ballVel.x;
        }

        if (m_ballPos.y - m_ballRadius < 0 ||
            m_ballPos.y + m_ballRadius > rect.Height())
        {
            m_ballVel.y = -m_ballVel.y;
        }

        Invalidate(FALSE);
        CView::OnTimer(nIDEvent);
    }

    DECLARE_MESSAGE_MAP()
};
```

### Smooth Scrolling Text:

```cpp
class CScrollTextView : public CView
{
protected:
    int m_scrollPos;
    CString m_text;
    CDoubleBuffer m_buffer;

public:
    CScrollTextView()
    {
        m_scrollPos = 0;
        m_text = _T("This is smooth scrolling text with no flicker!");
        SetTimer(1, 30, NULL);  // ~33 FPS
    }

    BOOL OnEraseBkgnd(CDC* pDC) override
    {
        return TRUE;
    }

    void OnDraw(CDC* pDC) override
    {
        CRect rect;
        GetClientRect(&rect);

        CDC* pMemDC = m_buffer.GetMemDC(pDC, rect.Width(), rect.Height());

        // Clear background
        pMemDC->FillSolidRect(&rect, RGB(0, 0, 128));

        // Set up font
        CFont font;
        font.CreatePointFont(240, _T("Arial"));
        CFont* pOldFont = pMemDC->SelectObject(&font);

        pMemDC->SetTextColor(RGB(255, 255, 0));
        pMemDC->SetBkMode(TRANSPARENT);

        // Draw scrolling text
        pMemDC->TextOut(m_scrollPos, rect.Height() / 2 - 20, m_text);

        pMemDC->SelectObject(pOldFont);

        // Flush to screen
        m_buffer.Flush(pDC);
    }

    void OnTimer(UINT_PTR nIDEvent) override
    {
        CRect rect;
        GetClientRect(&rect);

        m_scrollPos -= 3;

        // Reset when off screen
        CClientDC dc(this);
        CFont font;
        font.CreatePointFont(240, _T("Arial"));
        CFont* pOldFont = dc.SelectObject(&font);

        CSize size = dc.GetTextExtent(m_text);
        dc.SelectObject(pOldFont);

        if (m_scrollPos + size.cx < 0)
            m_scrollPos = rect.Width();

        Invalidate(FALSE);
        CView::OnTimer(nIDEvent);
    }

    DECLARE_MESSAGE_MAP()
};
```

---

## 9. Exercises

### Exercise 1: Particle System

```cpp
struct Particle
{
    CPoint pos;
    CPoint vel;
    COLORREF color;
    int life;
};

class CParticleView : public CView
{
protected:
    std::vector<Particle> m_particles;
    CDoubleBuffer m_buffer;

public:
    CParticleView()
    {
        // Create particles
        for (int i = 0; i < 100; i++)
        {
            Particle p;
            p.pos = CPoint(400, 300);
            p.vel = CPoint(rand() % 10 - 5, rand() % 10 - 5);
            p.color = RGB(rand() % 256, rand() % 256, rand() % 256);
            p.life = 100;
            m_particles.push_back(p);
        }

        SetTimer(1, 16, NULL);
    }

    BOOL OnEraseBkgnd(CDC* pDC) override { return TRUE; }

    void OnDraw(CDC* pDC) override
    {
        CRect rect;
        GetClientRect(&rect);

        CDC* pMemDC = m_buffer.GetMemDC(pDC, rect.Width(), rect.Height());

        pMemDC->FillSolidRect(&rect, RGB(0, 0, 0));

        // Draw particles
        for (auto& p : m_particles)
        {
            CPen pen(PS_SOLID, 2, p.color);
            CPen* pOldPen = pMemDC->SelectObject(&pen);

            pMemDC->SetPixel(p.pos.x, p.pos.y, p.color);

            pMemDC->SelectObject(pOldPen);
        }

        m_buffer.Flush(pDC);
    }

    void OnTimer(UINT_PTR nIDEvent) override
    {
        // Update particles
        for (auto& p : m_particles)
        {
            p.pos += p.vel;
            p.vel.y += 1;  // Gravity
            p.life--;

            if (p.life <= 0)
            {
                // Respawn
                p.pos = CPoint(400, 300);
                p.vel = CPoint(rand() % 10 - 5, rand() % 10 - 5);
                p.life = 100;
            }
        }

        Invalidate(FALSE);
        CView::OnTimer(nIDEvent);
    }
};
```

---

## Key Takeaways

1. ✅ Double buffering eliminates flicker
2. ✅ Draw to memory DC, then BitBlt to screen
3. ✅ Handle OnEraseBkgnd to prevent clearing
4. ✅ Reuse buffer for better performance
5. ✅ CMemDC provides automatic buffering
6. ✅ Only update changed areas for optimization
7. ✅ Essential for animation and real-time graphics
8. ✅ Modern Windows provides built-in support

---

## Next Lesson Preview

In **Lesson 54: Coordinate Systems and Mapping Modes**, we'll learn:
- Understanding coordinate systems
- Mapping modes
- Coordinate transformations
- World transforms

**Congratulations on completing Lesson 53!**
