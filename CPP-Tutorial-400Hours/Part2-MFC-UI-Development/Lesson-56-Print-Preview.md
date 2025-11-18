# Lesson 56: Print Preview

**Duration**: 4 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding Print Preview
2. Enabling Print Preview
3. Print Preview Architecture
4. Customizing Preview
5. OnPrepareDC for Preview
6. Scaling Issues
7. Preview Toolbar
8. Complete Example

---

## 1. Understanding Print Preview

Print Preview shows how pages will look when printed without actually printing.

### Benefits:
✅ Save paper and ink
✅ Verify layout before printing
✅ Check page breaks
✅ Review headers/footers

---

## 2. Enabling Print Preview

### Basic Setup:

```cpp
// In message map
BEGIN_MESSAGE_MAP(CMyView, CView)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()
```

That's it! MFC handles the rest automatically.

---

## 3. Print Preview Architecture

### How It Works:

```
1. User clicks Print Preview
2. MFC creates CPreviewView
3. Calls your OnPrepareDC()
4. Calls your OnPrint()
5. Scales output to screen
```

### Key Difference:

```cpp
void CMyView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
    if (pInfo->m_bPreview)
    {
        // This is print preview
    }
    else
    {
        // This is actual printing
    }

    // Usually same code for both
}
```

---

## 4. Customizing Preview

### Detecting Preview Mode:

```cpp
void CMyView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
    CView::OnPrepareDC(pDC, pInfo);

    if (pInfo && pInfo->m_bPreview)
    {
        // In print preview mode
        // Adjust as needed
    }
}
```

### Custom Page Count:

```cpp
BOOL CMyView::OnPreparePrinting(CPrintInfo* pInfo)
{
    if (pInfo->m_bPreview)
    {
        // Preview-specific setup
        pInfo->SetMaxPage(10);
    }
    else
    {
        // Actual print setup
        int pages = CalculatePages();
        pInfo->SetMaxPage(pages);
    }

    return DoPreparePrinting(pInfo);
}
```

---

## 5. OnPrepareDC for Preview

### Proper Implementation:

```cpp
void CMyView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
    CView::OnPrepareDC(pDC, pInfo);

    // Set mapping mode for both print and preview
    if (pDC->IsPrinting() || (pInfo && pInfo->m_bPreview))
    {
        pDC->SetMapMode(MM_LOENGLISH);

        // Set viewport based on margins
        int marginLeft = MulDiv(m_marginLeft,
                               pDC->GetDeviceCaps(LOGPIXELSX), 1000);
        int marginTop = MulDiv(m_marginTop,
                              pDC->GetDeviceCaps(LOGPIXELSY), 1000);

        pDC->SetViewportOrg(marginLeft, marginTop);
    }
}
```

---

## 6. Scaling Issues

### Problem:
Preview DC has different resolution than printer DC.

### Solution:
Use same code for both, let MFC handle scaling.

```cpp
void CMyView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
    // Get device capabilities
    int horz = pDC->GetDeviceCaps(HORZRES);
    int vert = pDC->GetDeviceCaps(VERTRES);
    int dpiX = pDC->GetDeviceCaps(LOGPIXELSX);
    int dpiY = pDC->GetDeviceCaps(LOGPIXELSY);

    // These values automatically adjust for preview vs print
    // Use them consistently
}
```

---

## 7. Preview Toolbar

### Default Toolbar:
- Next Page
- Previous Page
- Print
- Zoom In
- Zoom Out
- Close

### Custom Preview Frame:

```cpp
class CMyPreviewFrame : public CPreviewFrameWnd
{
protected:
    DECLARE_DYNCREATE(CMyPreviewFrame)

public:
    virtual BOOL OnCreateToolBar();
};

IMPLEMENT_DYNCREATE(CMyPreviewFrame, CPreviewFrameWnd)

BOOL CMyPreviewFrame::OnCreateToolBar()
{
    // Create custom toolbar
    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT))
        return FALSE;

    // Add buttons
    // ID_PREVIEW_PRINT, ID_PREVIEW_NEXT, etc.

    return TRUE;
}
```

---

## 8. Complete Example

### Full Print/Preview Implementation:

```cpp
// MyView.h
class CMyView : public CView
{
protected:
    CFont m_printFont;
    int m_marginLeft, m_marginTop;

public:
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

    DECLARE_MESSAGE_MAP()
};

// MyView.cpp
BOOL CMyView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // Calculate pages
    CMyDoc* pDoc = GetDocument();
    int totalPages = pDoc->GetPageCount();

    pInfo->SetMinPage(1);
    pInfo->SetMaxPage(totalPages);

    return DoPreparePrinting(pInfo);
}

void CMyView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
    // Create font (same for print and preview)
    LOGFONT lf;
    memset(&lf, 0, sizeof(LOGFONT));
    lf.lfHeight = -MulDiv(12, pDC->GetDeviceCaps(LOGPIXELSY), 72);
    _tcscpy_s(lf.lfFaceName, _T("Arial"));

    m_printFont.CreateFontIndirect(&lf);

    m_marginLeft = 1000;  // 1 inch
    m_marginTop = 1000;
}

void CMyView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
    CView::OnPrepareDC(pDC, pInfo);

    if (pDC->IsPrinting() || (pInfo && pInfo->m_bPreview))
    {
        pDC->SetMapMode(MM_LOENGLISH);

        int marginL = MulDiv(m_marginLeft,
                            pDC->GetDeviceCaps(LOGPIXELSX), 1000);
        int marginT = MulDiv(m_marginTop,
                            pDC->GetDeviceCaps(LOGPIXELSY), 1000);

        pDC->SetViewportOrg(marginL, marginT);
    }
}

void CMyView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
    CMyDoc* pDoc = GetDocument();
    UINT nPage = pInfo->m_nCurPage;

    // Select font
    CFont* pOldFont = pDC->SelectObject(&m_printFont);

    // Draw page content
    CString pageText = pDoc->GetPageText(nPage);
    pDC->TextOut(0, 0, pageText);

    // Draw page number
    CString footer;
    footer.Format(_T("Page %d"), nPage);
    pDC->TextOut(0, -1000, footer);  // 10 inches down

    pDC->SelectObject(pOldFont);
}

void CMyView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
    if (m_printFont.GetSafeHandle())
        m_printFont.DeleteObject();

    CView::OnEndPrinting(pDC, pInfo);
}
```

---

## Key Takeaways

1. ✅ Print Preview uses same OnPrint() code
2. ✅ Check pInfo->m_bPreview if needed
3. ✅ MFC handles scaling automatically
4. ✅ Use device capabilities correctly
5. ✅ OnPrepareDC sets up both print and preview
6. ✅ Default preview toolbar is functional
7. ✅ No special code needed in most cases

---

## Next Lesson Preview

In **Lesson 57: Advanced Printing**, we'll learn:
- Multi-page documents
- Page breaks
- Print ranges
- Collation

**Congratulations on completing Lesson 56!**
