# Lesson 23: Splitter Windows

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding Splitter Windows
2. Static Splitters
3. Dynamic Splitters
4. Creating Splitter Windows
5. Customizing Splitter Behavior
6. Nested Splitters
7. Splitter State Persistence
8. Advanced Splitter Techniques
9. Complete Examples
10. Exercises

---

## 1. Understanding Splitter Windows

### What are Splitter Windows?

**Splitter Windows** divide a frame window into multiple panes, each containing a view. Users can resize panes by dragging the splitter bar.

```
┌────────────────────────────────────┐
│                                    │
│            Pane 1                  │
│            (View)                  │
│                                    │
├════════════════════════════════════┤  ← Splitter Bar
│                                    │
│            Pane 2                  │
│            (View)                  │
│                                    │
└────────────────────────────────────┘
```

---

### Real-World Examples

- **Visual Studio** - Source code + output window
- **Excel** - Freeze panes
- **Outlook** - Folder list + message list + preview pane
- **Windows Explorer** - Folder tree + file list

---

### CSplitterWnd Class

```cpp
class CSplitterWnd : public CWnd
{
public:
    // Static splitter - fixed number of panes
    BOOL CreateStatic(CWnd* pParentWnd,
                     int nRows, int nCols,
                     DWORD dwStyle = WS_CHILD | WS_VISIBLE,
                     UINT nID = AFX_IDW_PANE_FIRST);

    // Dynamic splitter - user-controlled splitting
    BOOL Create(CWnd* pParentWnd,
               int nMaxRows, int nMaxCols,
               CSize sizeMin,
               CCreateContext* pContext,
               DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL,
               UINT nID = AFX_IDW_PANE_FIRST);

    // Create view in pane
    BOOL CreateView(int row, int col,
                   CRuntimeClass* pViewClass,
                   CSize sizeInit,
                   CCreateContext* pContext);

    // Get view from pane
    CWnd* GetPane(int row, int col) const;

    // Set sizes
    void SetRowInfo(int row, int cyIdeal, int cyMin);
    void SetColumnInfo(int col, int cxIdeal, int cxMin);
    void RecalcLayout();
};
```

---

## 2. Static Splitters

### What is a Static Splitter?

- **Fixed number** of panes
- Created at window creation time
- Panes cannot be closed
- Can contain different view types in each pane

---

### Creating a 2x1 Static Splitter (Horizontal)

```cpp
// In MainFrm.h
class CMainFrame : public CFrameWnd
{
protected:
    CSplitterWnd m_wndSplitter;

    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs,
                               CCreateContext* pContext);

    DECLARE_DYNCREATE(CMainFrame)
};

// In MainFrm.cpp
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs,
                               CCreateContext* pContext)
{
    // Create static splitter: 2 rows, 1 column
    if (!m_wndSplitter.CreateStatic(this, 2, 1))
    {
        TRACE0("Failed to create splitter\n");
        return FALSE;
    }

    // Create view in top pane (row 0, col 0)
    if (!m_wndSplitter.CreateView(0, 0,
        RUNTIME_CLASS(CMyView1),
        CSize(0, 200),  // Initial height: 200 pixels
        pContext))
    {
        TRACE0("Failed to create view 1\n");
        return FALSE;
    }

    // Create view in bottom pane (row 1, col 0)
    if (!m_wndSplitter.CreateView(1, 0,
        RUNTIME_CLASS(CMyView2),
        CSize(0, 0),    // Take remaining space
        pContext))
    {
        TRACE0("Failed to create view 2\n");
        return FALSE;
    }

    // Set active view (optional)
    SetActiveView((CView*)m_wndSplitter.GetPane(0, 0));

    return TRUE;
}
```

---

### Creating a 1x2 Static Splitter (Vertical)

```cpp
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs,
                               CCreateContext* pContext)
{
    // Create static splitter: 1 row, 2 columns
    if (!m_wndSplitter.CreateStatic(this, 1, 2))
        return FALSE;

    // Left pane
    if (!m_wndSplitter.CreateView(0, 0,
        RUNTIME_CLASS(CTreeView),
        CSize(200, 0),  // Initial width: 200 pixels
        pContext))
        return FALSE;

    // Right pane
    if (!m_wndSplitter.CreateView(0, 1,
        RUNTIME_CLASS(CListView),
        CSize(0, 0),    // Take remaining space
        pContext))
        return FALSE;

    return TRUE;
}
```

---

### Creating a 2x2 Static Splitter

```cpp
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs,
                               CCreateContext* pContext)
{
    // Create static splitter: 2 rows, 2 columns
    if (!m_wndSplitter.CreateStatic(this, 2, 2))
        return FALSE;

    // Top-left (0, 0)
    if (!m_wndSplitter.CreateView(0, 0,
        RUNTIME_CLASS(CMyView),
        CSize(300, 200), pContext))
        return FALSE;

    // Top-right (0, 1)
    if (!m_wndSplitter.CreateView(0, 1,
        RUNTIME_CLASS(CMyView),
        CSize(0, 200), pContext))
        return FALSE;

    // Bottom-left (1, 0)
    if (!m_wndSplitter.CreateView(1, 0,
        RUNTIME_CLASS(CMyView),
        CSize(300, 0), pContext))
        return FALSE;

    // Bottom-right (1, 1)
    if (!m_wndSplitter.CreateView(1, 1,
        RUNTIME_CLASS(CMyView),
        CSize(0, 0), pContext))
        return FALSE;

    return TRUE;
}
```

---

## 3. Dynamic Splitters

### What is a Dynamic Splitter?

- **Variable number** of panes (1, 2, or 4)
- User can split/unsplit windows
- All panes contain same view type
- Like Excel's freeze panes

---

### Creating a Dynamic Splitter

```cpp
// In MainFrm.h
class CMainFrame : public CFrameWnd
{
protected:
    CSplitterWnd m_wndSplitter;

    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs,
                               CCreateContext* pContext);
};

// In MainFrm.cpp
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs,
                               CCreateContext* pContext)
{
    // Create dynamic splitter: max 2 rows, 2 columns
    return m_wndSplitter.Create(this,
        2, 2,                           // Max rows, cols
        CSize(10, 10),                  // Min pane size
        pContext,
        WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL);
}
```

**User Actions**:
- Drag splitter boxes to create splits
- Double-click splitter bar to remove split

---

## 4. Creating Splitter Windows

### Setting Initial Splitter Position

```cpp
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs,
                               CCreateContext* pContext)
{
    if (!m_wndSplitter.CreateStatic(this, 2, 1))
        return FALSE;

    if (!m_wndSplitter.CreateView(0, 0,
        pContext->m_pNewViewClass, CSize(0, 0), pContext))
        return FALSE;

    if (!m_wndSplitter.CreateView(1, 0,
        pContext->m_pNewViewClass, CSize(0, 0), pContext))
        return FALSE;

    // Set initial row sizes
    m_wndSplitter.SetRowInfo(0, 300, 50);   // Row 0: 300px, min 50px
    m_wndSplitter.SetRowInfo(1, 200, 50);   // Row 1: 200px, min 50px
    m_wndSplitter.RecalcLayout();

    return TRUE;
}
```

---

### Setting Column Sizes

```cpp
// For vertical splitter (1 row, 2 columns)
m_wndSplitter.SetColumnInfo(0, 250, 100);  // Col 0: 250px, min 100px
m_wndSplitter.SetColumnInfo(1, 400, 100);  // Col 1: 400px, min 100px
m_wndSplitter.RecalcLayout();
```

---

### Getting Current Sizes

```cpp
int cy, cyMin;
m_wndSplitter.GetRowInfo(0, cy, cyMin);
TRACE("Row 0: current=%d, min=%d\n", cy, cyMin);

int cx, cxMin;
m_wndSplitter.GetColumnInfo(0, cx, cxMin);
TRACE("Col 0: current=%d, min=%d\n", cx, cxMin);
```

---

## 5. Customizing Splitter Behavior

### Customizing Minimum Sizes

```cpp
class CMyMainFrame : public CFrameWnd
{
protected:
    CSplitterWnd m_wndSplitter;

    // Override to customize tracking
    afx_msg void OnSize(UINT nType, int cx, int cy);
};

void CMyMainFrame::OnSize(UINT nType, int cx, int cy)
{
    CFrameWnd::OnSize(nType, cx, cy);

    if (m_wndSplitter.GetSafeHwnd())
    {
        // Enforce minimum sizes
        int cy0, cyMin0, cy1, cyMin1;
        m_wndSplitter.GetRowInfo(0, cy0, cyMin0);
        m_wndSplitter.GetRowInfo(1, cy1, cyMin1);

        // Top pane should be at least 25% of height
        int minTopHeight = cy / 4;
        if (cy0 < minTopHeight)
        {
            m_wndSplitter.SetRowInfo(0, minTopHeight, cyMin0);
            m_wndSplitter.RecalcLayout();
        }
    }
}
```

---

### Disabling Splitter Movement

```cpp
class CFixedSplitterWnd : public CSplitterWnd
{
protected:
    // Override to prevent dragging
    virtual void StartTracking(int ht)
    {
        // Do nothing - prevents dragging
    }
};

class CMainFrame : public CFrameWnd
{
protected:
    CFixedSplitterWnd m_wndSplitter;  // Use custom splitter
};
```

---

### Custom Splitter Bar Appearance

```cpp
class CCustomSplitterWnd : public CSplitterWnd
{
protected:
    virtual void OnDrawSplitter(CDC* pDC,
                               ESplitType nType,
                               const CRect& rect)
    {
        if (nType == splitBorder)
        {
            // Custom border drawing
            CBrush brush(RGB(200, 200, 200));
            pDC->FillRect(&rect, &brush);
        }
        else if (nType == splitBar)
        {
            // Custom splitter bar
            CBrush brush(RGB(150, 150, 150));
            pDC->FillRect(&rect, &brush);

            // Draw grip
            CPen pen(PS_SOLID, 1, RGB(100, 100, 100));
            CPen* pOldPen = pDC->SelectObject(&pen);

            int centerX = rect.left + rect.Width() / 2;
            int centerY = rect.top + rect.Height() / 2;

            for (int i = -10; i <= 10; i += 5)
            {
                pDC->MoveTo(centerX - 5, centerY + i);
                pDC->LineTo(centerX + 5, centerY + i);
            }

            pDC->SelectObject(pOldPen);
        }
        else
        {
            CSplitterWnd::OnDrawSplitter(pDC, nType, rect);
        }
    }
};
```

---

## 6. Nested Splitters

### Creating Nested Splitters

```cpp
// MainFrm.h
class CMainFrame : public CFrameWnd
{
protected:
    CSplitterWnd m_wndSplitterMain;   // Main splitter
    CSplitterWnd m_wndSplitterNested; // Nested splitter

    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs,
                               CCreateContext* pContext);
};

// MainFrm.cpp
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs,
                               CCreateContext* pContext)
{
    // Create main splitter: 1 row, 2 columns
    if (!m_wndSplitterMain.CreateStatic(this, 1, 2))
        return FALSE;

    // Left pane: single view
    if (!m_wndSplitterMain.CreateView(0, 0,
        RUNTIME_CLASS(CTreeView),
        CSize(200, 0), pContext))
        return FALSE;

    // Right pane: nested splitter (2 rows)
    if (!m_wndSplitterNested.CreateStatic(
        &m_wndSplitterMain,  // Parent is main splitter!
        2, 1,
        WS_CHILD | WS_VISIBLE,
        m_wndSplitterMain.IdFromRowCol(0, 1)))  // ID of right pane
        return FALSE;

    // Nested splitter - top pane
    if (!m_wndSplitterNested.CreateView(0, 0,
        RUNTIME_CLASS(CListView),
        CSize(0, 300), pContext))
        return FALSE;

    // Nested splitter - bottom pane
    if (!m_wndSplitterNested.CreateView(1, 0,
        RUNTIME_CLASS(CEditView),
        CSize(0, 0), pContext))
        return FALSE;

    return TRUE;
}
```

**Result**:
```
┌─────────┬────────────────┐
│         │                │
│         │   List View    │
│  Tree   │                │
│  View   ├────────────────┤
│         │                │
│         │   Edit View    │
│         │                │
└─────────┴────────────────┘
```

---

## 7. Splitter State Persistence

### Saving Splitter Position

```cpp
class CMainFrame : public CFrameWnd
{
protected:
    CSplitterWnd m_wndSplitter;

    void SaveSplitterPosition();
    void LoadSplitterPosition();

    virtual void OnClose();
};

void CMainFrame::SaveSplitterPosition()
{
    int cy, cyMin;
    m_wndSplitter.GetRowInfo(0, cy, cyMin);

    // Save to registry
    AfxGetApp()->WriteProfileInt(_T("Settings"), _T("SplitterPos"), cy);
}

void CMainFrame::LoadSplitterPosition()
{
    // Load from registry
    int cy = AfxGetApp()->GetProfileInt(_T("Settings"), _T("SplitterPos"), 200);

    // Apply to splitter
    m_wndSplitter.SetRowInfo(0, cy, 50);
    m_wndSplitter.RecalcLayout();
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs,
                               CCreateContext* pContext)
{
    // ... create splitter ...

    // Load saved position
    LoadSplitterPosition();

    return TRUE;
}

void CMainFrame::OnClose()
{
    SaveSplitterPosition();
    CFrameWnd::OnClose();
}
```

---

### Saving Multiple Splitter Positions

```cpp
void CMainFrame::SaveSplitterPositions()
{
    CString section = _T("Splitter");

    for (int row = 0; row < 2; row++)
    {
        int cy, cyMin;
        m_wndSplitter.GetRowInfo(row, cy, cyMin);

        CString key;
        key.Format(_T("Row%d"), row);
        AfxGetApp()->WriteProfileInt(section, key, cy);
    }

    for (int col = 0; col < 2; col++)
    {
        int cx, cxMin;
        m_wndSplitter.GetColumnInfo(col, cx, cxMin);

        CString key;
        key.Format(_T("Col%d"), col);
        AfxGetApp()->WriteProfileInt(section, key, cx);
    }
}

void CMainFrame::LoadSplitterPositions()
{
    CString section = _T("Splitter");

    for (int row = 0; row < 2; row++)
    {
        CString key;
        key.Format(_T("Row%d"), row);
        int cy = AfxGetApp()->GetProfileInt(section, key, 200);

        m_wndSplitter.SetRowInfo(row, cy, 50);
    }

    for (int col = 0; col < 2; col++)
    {
        CString key;
        key.Format(_T("Col%d"), col);
        int cx = AfxGetApp()->GetProfileInt(section, key, 200);

        m_wndSplitter.SetColumnInfo(col, cx, 50);
    }

    m_wndSplitter.RecalcLayout();
}
```

---

## 8. Advanced Splitter Techniques

### Accessing Panes

```cpp
// Get view from specific pane
CView* pView = (CView*)m_wndSplitter.GetPane(0, 1);

// Iterate all panes
for (int row = 0; row < 2; row++)
{
    for (int col = 0; col < 2; col++)
    {
        CWnd* pWnd = m_wndSplitter.GetPane(row, col);
        if (pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CView)))
        {
            CView* pView = (CView*)pWnd;
            // Process view...
        }
    }
}
```

---

### Synchronized Scrolling

```cpp
class CTextView : public CEditView
{
protected:
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

    DECLARE_MESSAGE_MAP()
};

void CTextView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    CEditView::OnVScroll(nSBCode, nPos, pScrollBar);

    // Synchronize other panes
    CMainFrame* pFrame = (CMainFrame*)GetParentFrame();
    CSplitterWnd* pSplitter = (CSplitterWnd*)GetParent();

    if (pSplitter && pSplitter->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
    {
        // Get other pane
        CView* pOtherView = (CView*)pSplitter->GetPane(1, 0);
        if (pOtherView && pOtherView != this)
        {
            // Synchronize scroll position
            SCROLLINFO si;
            GetScrollInfo(SB_VERT, &si);
            pOtherView->SendMessage(WM_VSCROLL,
                MAKEWPARAM(SB_THUMBPOSITION, si.nPos));
        }
    }
}
```

---

## 9. Complete Examples

### Example: File Manager with Preview

```cpp
// MainFrm.h
class CMainFrame : public CFrameWnd
{
protected:
    CSplitterWnd m_wndSplitter;

public:
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs,
                               CCreateContext* pContext);

    DECLARE_DYNCREATE(CMainFrame)
};

// MainFrm.cpp
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs,
                               CCreateContext* pContext)
{
    // Create vertical splitter: file list | preview
    if (!m_wndSplitter.CreateStatic(this, 1, 2))
        return FALSE;

    // Left: File list
    if (!m_wndSplitter.CreateView(0, 0,
        RUNTIME_CLASS(CFileListView),
        CSize(400, 0), pContext))
        return FALSE;

    // Right: Preview
    if (!m_wndSplitter.CreateView(0, 1,
        RUNTIME_CLASS(CPreviewView),
        CSize(0, 0), pContext))
        return FALSE;

    return TRUE;
}

// FileListView.h
class CFileListView : public CListView
{
protected:
    virtual void OnInitialUpdate();
    afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);

    DECLARE_MESSAGE_MAP()
};

void CFileListView::OnInitialUpdate()
{
    CListView::OnInitialUpdate();

    CListCtrl& list = GetListCtrl();
    list.ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL);

    list.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 200);
    list.InsertColumn(1, _T("Type"), LVCFMT_LEFT, 100);
    list.InsertColumn(2, _T("Size"), LVCFMT_RIGHT, 100);

    // Populate with files...
}

void CFileListView::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLISTVIEW* pNMLV = (NMLISTVIEW*)pNMHDR;

    if (pNMLV->uNewState & LVIS_SELECTED)
    {
        // Get selected file
        CString fileName = GetListCtrl().GetItemText(pNMLV->iItem, 0);

        // Notify preview pane
        CMainFrame* pFrame = (CMainFrame*)GetParentFrame();
        CSplitterWnd* pSplitter = (CSplitterWnd*)GetParent();
        CPreviewView* pPreview =
            (CPreviewView*)pSplitter->GetPane(0, 1);

        if (pPreview)
        {
            pPreview->PreviewFile(fileName);
        }
    }

    *pResult = 0;
}
```

---

## 10. Exercises

### Exercise 1: Simple Horizontal Splitter

Create a 2-pane horizontal splitter with:
- Top pane: Text editor
- Bottom pane: Output window

---

### Exercise 2: Three-Pane Layout

Create a layout with:
- Left pane: Tree view (200px wide)
- Right top pane: List view
- Right bottom pane: Details view

Use nested splitters.

---

### Exercise 3: Synchronized Scrolling

Create two text views side-by-side that scroll together.

---

### Exercise 4: Persistent Splitter

Save and restore splitter position between application runs.

---

### Exercise 5: Dynamic Splitter with Custom Views

Create a dynamic splitter where each pane can display different content based on selection.

---

## Key Takeaways

1. ✅ Static splitters have fixed panes
2. ✅ Dynamic splitters allow user splitting
3. ✅ OnCreateClient() creates splitters
4. ✅ CreateView() adds views to panes
5. ✅ SetRowInfo/SetColumnInfo control sizes
6. ✅ Nested splitters enable complex layouts
7. ✅ Save positions for better UX
8. ✅ GetPane() accesses individual panes
9. ✅ Custom splitter classes allow customization
10. ✅ Synchronized scrolling possible with messages

---

## Summary

Splitter windows enable powerful multi-pane layouts. Master static and dynamic splitters to create professional user interfaces!

---

## Next Lesson Preview

In **Lesson 24**, we'll explore:
- MDI (Multiple Document Interface) applications
- MDI architecture
- Document/view management in MDI
- Window management
- Complete MDI application

**Congratulations on completing Lesson 23!**
