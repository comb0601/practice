# Lesson 57: Advanced Printing (Multi-page)

**Duration**: 5 hours
**Difficulty**: Advanced

## Table of Contents
1. Multi-page Documents
2. Page Breaks
3. Print Ranges
4. Collation and Copies
5. Two-Sided Printing
6. Advanced Pagination
7. Large Documents
8. Complete Example

---

## 1. Multi-page Documents

### Pagination Strategy:

```cpp
class CMyView : public CView
{
protected:
    CArray<int> m_pageStarts;  // Line number for each page start

    void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override
    {
        CalculatePageBreaks(pDC);
        pInfo->SetMaxPage(m_pageStarts.GetCount());
    }

    void CalculatePageBreaks(CDC* pDC)
    {
        m_pageStarts.RemoveAll();

        CMyDoc* pDoc = GetDocument();
        int totalLines = pDoc->GetLineCount();

        CFont font;
        Create PrintFont(pDC, &font);
        CFont* pOldFont = pDC->SelectObject(&font);

        TEXTMETRIC tm;
        pDC->GetTextMetrics(&tm);

        int pageHeight = pDC->GetDeviceCaps(VERTRES);
        int linesPerPage = (pageHeight - 200) / tm.tmHeight;

        int currentLine = 0;
        while (currentLine < totalLines)
        {
            m_pageStarts.Add(currentLine);
            currentLine += linesPerPage;
        }

        pDC->SelectObject(pOldFont);
    }

    void OnPrint(CDC* pDC, CPrintInfo* pInfo) override
    {
        int pageIndex = pInfo->m_nCurPage - 1;
        int startLine = m_pageStarts[pageIndex];

        int endLine;
        if (pageIndex + 1 < m_pageStarts.GetCount())
            endLine = m_pageStarts[pageIndex + 1];
        else
            endLine = GetDocument()->GetLineCount();

        DrawLines(pDC, startLine, endLine);
    }
};
```

---

## 2. Page Breaks

### Manual Page Breaks:

```cpp
class CDocument
{
public:
    struct PageBreak
    {
        int lineNumber;
        BOOL forced;  // TRUE = user-inserted
    };

    CArray<PageBreak> m_pageBreaks;
};

void CMyView::CalculatePageBreaks(CDC* pDC)
{
    CMyDoc* pDoc = GetDocument();

    m_pageStarts.RemoveAll();

    int currentLine = 0;
    int totalLines = pDoc->GetLineCount();

    while (currentLine < totalLines)
    {
        m_pageStarts.Add(currentLine);

        // Check for forced page break
        BOOL foundForced = FALSE;
        for (int i = 0; i < pDoc->m_pageBreaks.GetCount(); i++)
        {
            if (pDoc->m_pageBreaks[i].forced &&
                pDoc->m_pageBreaks[i].lineNumber > currentLine)
            {
                currentLine = pDoc->m_pageBreaks[i].lineNumber;
                foundForced = TRUE;
                break;
            }
        }

        if (!foundForced)
        {
            // Auto page break
            currentLine += CalculateLinesPerPage(pDC);
        }
    }
}
```

### Preventing Orphans/Widows:

```cpp
int CMyView::FindBestPageBreak(CDC* pDC, int startLine, int maxLine)
{
    CMyDoc* pDoc = GetDocument();

    // Look for natural break (empty line)
    for (int i = maxLine; i > startLine + 5; i--)
    {
        if (pDoc->GetLine(i).IsEmpty())
            return i;
    }

    // Avoid breaking in middle of paragraph
    for (int i = maxLine; i > startLine + 3; i--)
    {
        if (pDoc->IsLineEndOfParagraph(i))
            return i + 1;
    }

    // Use calculated break
    return maxLine;
}
```

---

## 3. Print Ranges

### Handling Print Ranges:

```cpp
void CMyView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
    // Calculate all pages
    CalculatePageBreaks(pDC);

    int totalPages = m_pageStarts.GetCount();
    pInfo->SetMaxPage(totalPages);

    // Check if user selected page range
    if (pInfo->m_pPD->PrintRange())
    {
        UINT fromPage = pInfo->GetFromPage();
        UINT toPage = pInfo->GetToPage();

        // Validate range
        if (fromPage < 1) fromPage = 1;
        if (toPage > (UINT)totalPages) toPage = totalPages;

        // Set actual range to print
        pInfo->m_nCurPage = fromPage;
        pInfo->SetMaxPage(toPage);
    }
}

void CMyView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
    // Check if should continue
    if (!pInfo->m_bContinuePrinting)
        return;

    UINT currentPage = pInfo->m_nCurPage;

    // Check if beyond requested range
    if (pInfo->m_pPD->PrintRange())
    {
        if (currentPage > pInfo->GetToPage())
        {
            pInfo->m_bContinuePrinting = FALSE;
            return;
        }
    }

    // Draw page
    DrawPage(pDC, currentPage);
}
```

---

## 4. Collation and Copies

### Handling Multiple Copies:

```cpp
void CMyView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
    // Get number of copies
    int nCopies = pInfo->m_pPD->m_pd.nCopies;
    BOOL bCollate = pInfo->m_pPD->m_pd.Flags & PD_COLLATE;

    if (bCollate)
    {
        // Print all pages, then repeat
        // Default MFC behavior
    }
    else
    {
        // Print each page nCopies times
        // Need custom handling
        m_printState.copies = nCopies;
        m_printState.currentCopy = 1;
    }
}

void CMyView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
    BOOL bCollate = pInfo->m_pPD->m_pd.Flags & PD_COLLATE;

    if (!bCollate)
    {
        // Print page multiple times
        for (int copy = 0; copy < m_printState.copies; copy++)
        {
            DrawPage(pDC, pInfo->m_nCurPage);

            if (copy < m_printState.copies - 1)
            {
                // Start new page (except last)
                pDC->EndPage();
                pDC->StartPage();
            }
        }
    }
    else
    {
        // Normal single page print
        DrawPage(pDC, pInfo->m_nCurPage);
    }
}
```

---

## 5. Two-Sided Printing

### Duplex Printing:

```cpp
void CMyView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
    // Check if duplex capable
    if (pDC->GetDeviceCaps(PHYSICALOFFSETY) == 0)
    {
        // Printer supports duplex
        DEVMODE* pDevMode = pInfo->m_pPD->GetDevMode();

        if (pDevMode)
        {
            // Set duplex mode
            pDevMode->dmFields |= DM_DUPLEX;
            pDevMode->dmDuplex = DMDUP_VERTICAL;  // Flip on long edge
            // or DMDUP_HORIZONTAL for short edge
        }
    }
}
```

### Alternating Page Layouts:

```cpp
void CMyView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
    UINT nPage = pInfo->m_nCurPage;
    BOOL bOddPage = (nPage % 2) == 1;

    if (bOddPage)
    {
        // Right page (odd)
        // Wider right margin for binding
        m_marginLeft = 1000;
        m_marginRight = 1500;
    }
    else
    {
        // Left page (even)
        // Wider left margin for binding
        m_marginLeft = 1500;
        m_marginRight = 1000;
    }

    DrawPage(pDC, nPage);
}
```

---

## 6. Advanced Pagination

### Dynamic Pagination:

```cpp
class CPaginationInfo
{
public:
    int pageNumber;
    int startParagraph;
    int startLine;
    CRect contentRect;
};

class CMyView : public CView
{
protected:
    CArray<CPaginationInfo> m_pages;

    void CalculateAdvancedPagination(CDC* pDC)
    {
        m_pages.RemoveAll();

        CMyDoc* pDoc = GetDocument();
        int paraCount = pDoc->GetParagraphCount();

        CFont font;
        CreatePrintFont(pDC, &font);
        CFont* pOldFont = pDC->SelectObject(&font);

        int currentPage = 1;
        int currentPara = 0;
        int yPos = 0;

        int pageHeight = GetPrintableHeight(pDC);

        while (currentPara < paraCount)
        {
            CPaginationInfo pageInfo;
            pageInfo.pageNumber = currentPage;
            pageInfo.startParagraph = currentPara;
            pageInfo.startLine = 0;

            yPos = 0;

            // Fit paragraphs into page
            while (currentPara < paraCount && yPos < pageHeight)
            {
                int paraHeight = CalculateParagraphHeight(pDC,
                                                         pDoc->GetParagraph(currentPara));

                if (yPos + paraHeight > pageHeight)
                {
                    // Won't fit, start new page
                    if (yPos == 0)
                    {
                        // Paragraph too big for one page
                        // Need to split
                        SplitParagraph(pDC, currentPara, pageHeight);
                    }
                    break;
                }

                yPos += paraHeight;
                currentPara++;
            }

            m_pages.Add(pageInfo);
            currentPage++;
        }

        pDC->SelectObject(pOldFont);
    }
};
```

---

## 7. Large Documents

### Efficient Printing:

```cpp
class CMyView : public CView
{
protected:
    BOOL m_bPaginationValid;
    CArray<int> m_pageCache;

    void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) override
    {
        // Invalidate pagination when document changes
        m_bPaginationValid = FALSE;

        CView::OnUpdate(pSender, lHint, pHint);
    }

    void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override
    {
        if (!m_bPaginationValid)
        {
            // Show progress dialog for large documents
            CProgressDlg dlg;
            dlg.Create(this);
            dlg.SetRange(0, GetDocument()->GetLineCount());

            CalculatePaginationWithProgress(pDC, &dlg);

            dlg.DestroyWindow();

            m_bPaginationValid = TRUE;
        }

        pInfo->SetMaxPage(m_pageCache.GetCount());
    }

    void CalculatePaginationWithProgress(CDC* pDC, CProgressDlg* pDlg)
    {
        CMyDoc* pDoc = GetDocument();
        int totalLines = pDoc->GetLineCount();

        m_pageCache.RemoveAll();

        int currentLine = 0;
        int linesPerPage = CalculateLinesPerPage(pDC);

        while (currentLine < totalLines)
        {
            m_pageCache.Add(currentLine);
            currentLine += linesPerPage;

            // Update progress
            if (pDlg)
            {
                pDlg->SetPos(currentLine);

                MSG msg;
                while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
        }
    }
};
```

---

## 8. Complete Example

### Full Advanced Printing:

```cpp
// MyView.h
class CMyView : public CView
{
protected:
    struct PageInfo
    {
        int startLine;
        int endLine;
        BOOL hasHeader;
        BOOL hasFooter;
    };

    CArray<PageInfo> m_pages;
    CFont m_headerFont, m_bodyFont, m_footerFont;

    BOOL m_bPaginationValid;

public:
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

protected:
    void CalculatePagination(CDC* pDC);
    void DrawPage(CDC* pDC, int pageNum);
    void DrawHeader(CDC* pDC, int pageNum);
    void DrawFooter(CDC* pDC, int pageNum, int totalPages);
    int GetPrintableHeight(CDC* pDC);
};

// MyView.cpp
BOOL CMyView::OnPreparePrinting(CPrintInfo* pInfo)
{
    return DoPreparePrinting(pInfo);
}

void CMyView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
    // Create fonts
    m_headerFont.CreateFont(-MulDiv(14, pDC->GetDeviceCaps(LOGPIXELSY), 72),
                           0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                           CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                           DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    m_bodyFont.CreateFont(-MulDiv(11, pDC->GetDeviceCaps(LOGPIXELSY), 72),
                         0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                         DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                         CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                         DEFAULT_PITCH | FF_SWISS, _T("Times New Roman"));

    m_footerFont.CreateFont(-MulDiv(9, pDC->GetDeviceCaps(LOGPIXELSY), 72),
                           0, 0, 0, FW_NORMAL, TRUE, FALSE, FALSE,
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                           CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                           DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Calculate pagination
    CalculatePagination(pDC);

    pInfo->SetMaxPage(m_pages.GetCount());
}

void CMyView::CalculatePagination(CDC* pDC)
{
    m_pages.RemoveAll();

    CMyDoc* pDoc = GetDocument();
    int totalLines = pDoc->GetLineCount();

    CFont* pOldFont = pDC->SelectObject(&m_bodyFont);

    TEXTMETRIC tm;
    pDC->GetTextMetrics(&tm);

    int lineHeight = tm.tmHeight + tm.tmExternalLeading;
    int printableHeight = GetPrintableHeight(pDC);
    int linesPerPage = printableHeight / lineHeight;

    int currentLine = 0;

    while (currentLine < totalLines)
    {
        PageInfo page;
        page.startLine = currentLine;
        page.endLine = min(currentLine + linesPerPage, totalLines);
        page.hasHeader = TRUE;
        page.hasFooter = TRUE;

        m_pages.Add(page);

        currentLine = page.endLine;
    }

    pDC->SelectObject(pOldFont);
}

void CMyView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
    int pageIndex = pInfo->m_nCurPage - 1;

    if (pageIndex >= m_pages.GetCount())
        return;

    DrawPage(pDC, pageIndex);
}

void CMyView::DrawPage(CDC* pDC, int pageNum)
{
    PageInfo& page = m_pages[pageNum];

    // Draw header
    if (page.hasHeader)
        DrawHeader(pDC, pageNum + 1);

    // Draw body
    CMyDoc* pDoc = GetDocument();
    CFont* pOldFont = pDC->SelectObject(&m_bodyFont);

    TEXTMETRIC tm;
    pDC->GetTextMetrics(&tm);

    int y = 150;  // Start below header
    int lineHeight = tm.tmHeight + tm.tmExternalLeading;

    for (int i = page.startLine; i < page.endLine; i++)
    {
        CString line = pDoc->GetLine(i);
        pDC->TextOut(100, y, line);
        y += lineHeight;
    }

    pDC->SelectObject(pOldFont);

    // Draw footer
    if (page.hasFooter)
        DrawFooter(pDC, pageNum + 1, m_pages.GetCount());
}

void CMyView::DrawHeader(CDC* pDC, int pageNum)
{
    CFont* pOldFont = pDC->SelectObject(&m_headerFont);

    CMyDoc* pDoc = GetDocument();
    CString title = pDoc->GetTitle();

    pDC->TextOut(100, 50, title);

    // Draw line under header
    CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);

    int pageWidth = pDC->GetDeviceCaps(HORZRES);
    pDC->MoveTo(100, 100);
    pDC->LineTo(pageWidth - 100, 100);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldFont);
}

void CMyView::DrawFooter(CDC* pDC, int pageNum, int totalPages)
{
    CFont* pOldFont = pDC->SelectObject(&m_footerFont);

    CString footer;
    footer.Format(_T("Page %d of %d"), pageNum, totalPages);

    int pageHeight = pDC->GetDeviceCaps(VERTRES);
    int pageWidth = pDC->GetDeviceCaps(HORZRES);

    // Center footer
    CSize textSize = pDC->GetTextExtent(footer);
    int x = (pageWidth - textSize.cx) / 2;
    int y = pageHeight - 100;

    pDC->TextOut(x, y, footer);

    pDC->SelectObject(pOldFont);
}

int CMyView::GetPrintableHeight(CDC* pDC)
{
    int pageHeight = pDC->GetDeviceCaps(VERTRES);
    return pageHeight - 250;  // Subtract header/footer space
}

void CMyView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
    // Clean up fonts
    if (m_headerFont.GetSafeHandle())
        m_headerFont.DeleteObject();

    if (m_bodyFont.GetSafeHandle())
        m_bodyFont.DeleteObject();

    if (m_footerFont.GetSafeHandle())
        m_footerFont.DeleteObject();

    m_pages.RemoveAll();

    CView::OnEndPrinting(pDC, pInfo);
}
```

---

## Key Takeaways

1. ✅ Calculate pagination in OnBeginPrinting
2. ✅ Store page breaks for efficient printing
3. ✅ Handle print ranges properly
4. ✅ Support collation and multiple copies
5. ✅ Consider duplex printing
6. ✅ Prevent orphans/widows
7. ✅ Show progress for large documents
8. ✅ Cache pagination when possible

---

## Next Lesson Preview

In **Lesson 58: Custom Drawing in Views**, we'll learn:
- Custom CView implementations
- Drawing optimization
- View updates
- Coordinate handling

**Congratulations on completing Lesson 57!**
