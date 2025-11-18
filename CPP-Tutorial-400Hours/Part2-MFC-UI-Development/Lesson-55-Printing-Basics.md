# Lesson 55: Printing Basics

**Duration**: 5 hours
**Difficulty**: Intermediate

## Table of Contents
1. MFC Printing Architecture
2. Print Document Methods
3. Starting a Print Job
4. OnPreparePrinting
5. OnBeginPrinting
6. OnPrint
7. OnEndPrinting
8. Print Dialog
9. Page Setup
10. Complete Example

---

## 1. MFC Printing Architecture

MFC provides a framework for printing through the CView class.

### Printing Process Flow:

```
User selects Print
     ↓
OnPreparePrinting()   - Initialize print job
     ↓
OnBeginPrinting()     - Allocate resources
     ↓
OnPrepareDC()         - Set up DC for each page
     ↓
OnPrint()             - Draw each page
     ↓  (Repeat for each page)
OnEndPrinting()       - Clean up resources
```

### Key Methods:

```cpp
virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
```

---

## 2. Print Document Methods

### CPrintInfo Structure:

```cpp
class CPrintInfo
{
public:
    CPrintDialog* m_pPD;           // Pointer to print dialog
    BOOL m_bDirect;                // Direct printing flag
    BOOL m_bPreview;               // Print preview flag
    BOOL m_bContinuePrinting;      // Continue flag
    UINT m_nCurPage;               // Current page number
    UINT m_nNumPreviewPages;       // Preview pages
    CString m_strPageDesc;         // Page description
    LPVOID m_lpUserData;           // User data
    CRect m_rectDraw;              // Drawing rectangle

    void SetMinPage(UINT nMinPage);
    void SetMaxPage(UINT nMaxPage);
    UINT GetMinPage() const;
    UINT GetMaxPage() const;
    UINT GetFromPage() const;
    UINT GetToPage() const;
};
```

---

## 3. Starting a Print Job

### Enable Printing in View:

```cpp
// Add to message map
BEGIN_MESSAGE_MAP(CMyView, CView)
    ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()
```

### Basic Printing Implementation:

```cpp
// MyView.h
class CMyView : public CView
{
protected:
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

    DECLARE_MESSAGE_MAP()
};

// MyView.cpp
BOOL CMyView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // Default preparation
    return DoPreparePrinting(pInfo);
}

void CMyView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
    // Allocate GDI resources
}

void CMyView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
    // Draw the page
}

void CMyView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
    // Free GDI resources
}
```

---

## 4. OnPreparePrinting

### Purpose:
- Set page range
- Modify print dialog
- Initialize pagination

### Implementation:

```cpp
BOOL CMyView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // Set max page number (if known)
    pInfo->SetMaxPage(10);

    // Default handling (shows print dialog)
    return DoPreparePrinting(pInfo);
}
```

### Advanced OnPreparePrinting:

```cpp
BOOL CMyView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // Calculate number of pages
    CMyDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    int pageCount = CalculatePageCount(pDoc);

    // Set page range
    pInfo->SetMinPage(1);
    pInfo->SetMaxPage(pageCount);

    // Enable page range in dialog
    pInfo->m_pPD->m_pd.nMinPage = 1;
    pInfo->m_pPD->m_pd.nMaxPage = pageCount;
    pInfo->m_pPD->m_pd.Flags &= ~PD_NOPAGENUMS;

    return DoPreparePrinting(pInfo);
}

int CMyView::CalculatePageCount(CMyDoc* pDoc)
{
    // Example: 50 lines per page
    int lineCount = pDoc->GetLineCount();
    int linesPerPage = 50;

    return (lineCount + linesPerPage - 1) / linesPerPage;
}
```

---

## 5. OnBeginPrinting

### Purpose:
- Allocate printing resources
- Create fonts, pens, brushes
- Calculate pagination

### Implementation:

```cpp
class CMyView : public CView
{
protected:
    CFont m_printFont;
    CPen m_printPen;
    int m_nPrintPages;

    void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override
    {
        // Create printer font
        m_printFont.CreateFont(
            -MulDiv(12, pDC->GetDeviceCaps(LOGPIXELSY), 72),  // 12 pt
            0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS, _T("Arial"));

        // Create printer pen
        m_printPen.CreatePen(PS_SOLID,
            pDC->GetDeviceCaps(LOGPIXELSX) / 72,  // 1 point
            RGB(0, 0, 0));

        // Calculate number of pages
        CMyDoc* pDoc = GetDocument();
        m_nPrintPages = CalculatePages(pDC, pDoc);

        pInfo->SetMaxPage(m_nPrintPages);
    }

    void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override
    {
        // Clean up
        if (m_printFont.GetSafeHandle())
            m_printFont.DeleteObject();

        if (m_printPen.GetSafeHandle())
            m_printPen.DeleteObject();
    }

    int CalculatePages(CDC* pDC, CMyDoc* pDoc)
    {
        // Calculate how many pages needed
        CFont* pOldFont = pDC->SelectObject(&m_printFont);

        TEXTMETRIC tm;
        pDC->GetTextMetrics(&tm);

        int linesPerPage = pDC->GetDeviceCaps(VERTRES) / tm.tmHeight;
        int totalLines = pDoc->GetLineCount();

        pDC->SelectObject(pOldFont);

        return (totalLines + linesPerPage - 1) / linesPerPage;
    }
};
```

---

## 6. OnPrint

### Purpose:
- Draw one page
- Called once per page

### Basic Implementation:

```cpp
void CMyView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
    ASSERT_VALID(pDC);

    // Get current page number
    UINT nPage = pInfo->m_nCurPage;

    // Set up mapping mode
    pDC->SetMapMode(MM_LOENGLISH);

    // Draw page content
    DrawPage(pDC, nPage);

    // Draw header/footer
    DrawHeader(pDC, nPage);
    DrawFooter(pDC, nPage, pInfo->GetMaxPage());
}

void CMyView::DrawPage(CDC* pDC, UINT nPage)
{
    CMyDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    // Calculate what to draw on this page
    int startLine = (nPage - 1) * 50;  // 50 lines per page
    int endLine = min(startLine + 50, pDoc->GetLineCount());

    CFont* pOldFont = pDC->SelectObject(&m_printFont);

    TEXTMETRIC tm;
    pDC->GetTextMetrics(&tm);

    int y = -100;  // Start 1 inch from top

    for (int i = startLine; i < endLine; i++)
    {
        CString line = pDoc->GetLine(i);
        pDC->TextOut(100, y, line);  // 1 inch from left
        y -= tm.tmHeight;
    }

    pDC->SelectObject(pOldFont);
}

void CMyView::DrawHeader(CDC* pDC, UINT nPage)
{
    CFont* pOldFont = pDC->SelectObject(&m_printFont);

    CString header;
    header.Format(_T("Document Title - Page %d"), nPage);

    pDC->TextOut(100, -50, header);  // 0.5 inch from top

    // Draw line under header
    CPen* pOldPen = pDC->SelectObject(&m_printPen);
    pDC->MoveTo(100, -80);
    pDC->LineTo(750, -80);  // 7.5 inches wide
    pDC->SelectObject(pOldPen);

    pDC->SelectObject(pOldFont);
}

void CMyView::DrawFooter(CDC* pDC, UINT nPage, UINT nMaxPage)
{
    CFont* pOldFont = pDC->SelectObject(&m_printFont);

    // Draw line above footer
    CPen* pOldPen = pDC->SelectObject(&m_printPen);
    pDC->MoveTo(100, -1020);
    pDC->LineTo(750, -1020);  // 10.2 inches from top
    pDC->SelectObject(pOldPen);

    CString footer;
    footer.Format(_T("Page %d of %d"), nPage, nMaxPage);

    pDC->TextOut(100, -1050, footer);

    pDC->SelectObject(pOldFont);
}
```

---

## 7. OnEndPrinting

### Purpose:
- Free resources allocated in OnBeginPrinting
- Clean up

### Implementation:

```cpp
void CMyView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
    // Delete GDI objects
    if (m_printFont.GetSafeHandle())
        m_printFont.DeleteObject();

    if (m_printPen.GetSafeHandle())
        m_printPen.DeleteObject();

    // Other cleanup
    CView::OnEndPrinting(pDC, pInfo);
}
```

---

## 8. Print Dialog

### Customizing Print Dialog:

```cpp
BOOL CMyView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // Get print dialog
    CPrintDialog* pDlg = pInfo->m_pPD;

    // Enable collate
    pDlg->m_pd.Flags &= ~PD_NOCOLLATE;

    // Set default copies
    pDlg->m_pd.nCopies = 1;

    // Disable "Print to file"
    pDlg->m_pd.Flags |= PD_HIDEPRINTTOFILE;

    // Set page range
    pInfo->SetMinPage(1);
    pInfo->SetMaxPage(100);

    return DoPreparePrinting(pInfo);
}
```

### Getting Print Settings:

```cpp
void CMyView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
    // Check if printing all pages or range
    if (pInfo->m_pPD->PrintAll())
    {
        // Print all pages
    }
    else if (pInfo->m_pPD->PrintRange())
    {
        // Print page range
        UINT nFromPage = pInfo->GetFromPage();
        UINT nToPage = pInfo->GetToPage();

        // Adjust pagination
    }
    else if (pInfo->m_pPD->PrintSelection())
    {
        // Print selection
    }

    // Get number of copies
    int nCopies = pInfo->m_pPD->m_pd.nCopies;

    // Get collate flag
    BOOL bCollate = pInfo->m_pPD->m_pd.Flags & PD_COLLATE;
}
```

---

## 9. Page Setup

### Implementing Page Setup Dialog:

```cpp
// Add to message map
ON_COMMAND(ID_FILE_PAGE_SETUP, &CMyView::OnFilePageSetup)

void CMyView::OnFilePageSetup()
{
    CPageSetupDialog dlg;

    // Set initial values
    dlg.m_psd.Flags |= PSD_MARGINS;
    dlg.m_psd.rtMargin.left = 1000;    // 1 inch (in 1/1000 inch)
    dlg.m_psd.rtMargin.top = 1000;
    dlg.m_psd.rtMargin.right = 1000;
    dlg.m_psd.rtMargin.bottom = 1000;

    if (dlg.DoModal() == IDOK)
    {
        // Get new margins
        m_marginLeft = dlg.m_psd.rtMargin.left;
        m_marginTop = dlg.m_psd.rtMargin.top;
        m_marginRight = dlg.m_psd.rtMargin.right;
        m_marginBottom = dlg.m_psd.rtMargin.bottom;

        // Get paper size
        DEVMODE* pDevMode = dlg.GetDevMode();
        if (pDevMode)
        {
            // pDevMode->dmPaperSize
            // pDevMode->dmOrientation
        }
    }
}
```

---

## 10. Complete Example

### Full Printing Implementation:

```cpp
// MyView.h
class CMyView : public CView
{
protected:
    CFont m_printFont;
    int m_marginLeft, m_marginTop, m_marginRight, m_marginBottom;
    int m_linesPerPage;

public:
    CMyView();

    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

    DECLARE_MESSAGE_MAP()
};

// MyView.cpp
CMyView::CMyView()
{
    // Default margins (1 inch = 1000 units in 1/1000 inch)
    m_marginLeft = 1000;
    m_marginTop = 1000;
    m_marginRight = 1000;
    m_marginBottom = 1000;
    m_linesPerPage = 0;
}

BOOL CMyView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // Will calculate pages in OnBeginPrinting
    pInfo->SetMaxPage(1);  // Set to at least 1

    return DoPreparePrinting(pInfo);
}

void CMyView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
    // Create printer font (12 point)
    LOGFONT lf;
    memset(&lf, 0, sizeof(LOGFONT));
    lf.lfHeight = -MulDiv(12, pDC->GetDeviceCaps(LOGPIXELSY), 72);
    lstrcpy(lf.lfFaceName, _T("Arial"));

    m_printFont.CreateFontIndirect(&lf);

    // Calculate lines per page
    CFont* pOldFont = pDC->SelectObject(&m_printFont);

    TEXTMETRIC tm;
    pDC->GetTextMetrics(&tm);

    int pageHeight = pDC->GetDeviceCaps(VERTRES);
    int printableHeight = pageHeight -
                          MulDiv(m_marginTop + m_marginBottom,
                                pDC->GetDeviceCaps(LOGPIXELSY), 1000);

    m_linesPerPage = printableHeight / tm.tmHeight;

    pDC->SelectObject(pOldFont);

    // Calculate total pages
    CMyDoc* pDoc = GetDocument();
    int totalLines = pDoc->GetLineCount();
    int totalPages = (totalLines + m_linesPerPage - 1) / m_linesPerPage;

    pInfo->SetMaxPage(totalPages);
}

void CMyView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
    CMyDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    UINT nPage = pInfo->m_nCurPage;

    // Calculate margins in device units
    int marginL = MulDiv(m_marginLeft, pDC->GetDeviceCaps(LOGPIXELSX), 1000);
    int marginT = MulDiv(m_marginTop, pDC->GetDeviceCaps(LOGPIXELSY), 1000);

    // Select font
    CFont* pOldFont = pDC->SelectObject(&m_printFont);

    TEXTMETRIC tm;
    pDC->GetTextMetrics(&tm);

    // Calculate which lines to print
    int startLine = (nPage - 1) * m_linesPerPage;
    int endLine = min(startLine + m_linesPerPage, pDoc->GetLineCount());

    // Print lines
    int y = marginT;

    for (int i = startLine; i < endLine; i++)
    {
        CString line = pDoc->GetLine(i);
        pDC->TextOut(marginL, y, line);
        y += tm.tmHeight;
    }

    // Print footer with page number
    y = pDC->GetDeviceCaps(VERTRES) -
        MulDiv(m_marginBottom, pDC->GetDeviceCaps(LOGPIXELSY), 1000);

    CString footer;
    footer.Format(_T("Page %d of %d"), nPage, pInfo->GetMaxPage());

    CSize textSize = pDC->GetTextExtent(footer);
    int centerX = pDC->GetDeviceCaps(HORZRES) / 2 - textSize.cx / 2;

    pDC->TextOut(centerX, y, footer);

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

1. ✅ OnPreparePrinting sets up pagination
2. ✅ OnBeginPrinting allocates resources
3. ✅ OnPrint draws each page
4. ✅ OnEndPrinting cleans up
5. ✅ Use printer DC for correct sizing
6. ✅ Handle margins properly
7. ✅ Calculate pages based on content
8. ✅ Add headers and footers for professional output

---

## Next Lesson Preview

In **Lesson 56: Print Preview**, we'll learn:
- Implementing print preview
- Custom preview views
- Zoom and navigation
- Preview toolbar

**Congratulations on completing Lesson 55!**
