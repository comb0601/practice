# Lesson 22: Multiple Views

**Duration**: 7 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding Multiple Views
2. Creating Multiple View Types
3. View Coordination and Synchronization
4. Dynamic View Creation
5. View Switching
6. View-Specific Commands
7. Data Sharing Between Views
8. Performance Considerations
9. Complete Multi-View Application
10. Exercises

---

## 1. Understanding Multiple Views

### What are Multiple Views?

Multiple views allow you to display the **same document data** in **different ways** simultaneously.

```
        ┌──────────────┐
        │  CDocument   │
        │  (ONE Data)  │
        └──────────────┘
               │
     ┌─────────┼─────────┬─────────┐
     ↓         ↓         ↓         ↓
┌─────────┐ ┌───────┐ ┌──────┐ ┌───────┐
│TextView │ │ Chart │ │ Grid │ │ Graph │
│ (View1) │ │(View2)│ │(View3│ │(View4)│
└─────────┘ └───────┘ └──────┘ └───────┘
```

---

### Real-World Examples

**Microsoft Excel**:
- Normal view (grid)
- Page Layout view
- Page Break Preview
- Chart view (of selected data)

**CAD Software**:
- Top view
- Front view
- Side view
- 3D perspective view

**Text Editor**:
- Source code view
- Design view (WYSIWYG)
- Outline view
- Split view

---

### Benefits

1. **Different Perspectives** - Same data, different visualizations
2. **Comparison** - View multiple aspects simultaneously
3. **Efficiency** - Work in multiple views at once
4. **Flexibility** - Users choose preferred view

---

## 2. Creating Multiple View Types

### Method 1: Splitter Windows (Same View Type)

```cpp
// Most common - split one view into multiple panes
// See Lesson 23 for detailed coverage

class CMainFrame : public CFrameWnd
{
protected:
    CSplitterWnd m_wndSplitter;

    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs,
                               CCreateContext* pContext)
    {
        // Create 2x2 splitter
        if (!m_wndSplitter.CreateStatic(this, 2, 2))
            return FALSE;

        // All panes use same view class
        if (!m_wndSplitter.CreateView(0, 0,
            pContext->m_pNewViewClass, CSize(0, 0), pContext))
            return FALSE;

        // ... create other panes ...

        return TRUE;
    }
};
```

---

### Method 2: MDI with Different View Types

```cpp
// Multiple Document Interface - different windows
// Each window can have different view type

class CMyApp : public CWinApp
{
    virtual BOOL InitInstance()
    {
        // Template 1: Text view
        CMultiDocTemplate* pTextTemplate = new CMultiDocTemplate(
            IDR_TEXT_VIEW,
            RUNTIME_CLASS(CMyDocument),
            RUNTIME_CLASS(CMDIChildWnd),
            RUNTIME_CLASS(CTextView));
        AddDocTemplate(pTextTemplate);

        // Template 2: Graph view
        CMultiDocTemplate* pGraphTemplate = new CMultiDocTemplate(
            IDR_GRAPH_VIEW,
            RUNTIME_CLASS(CMyDocument),
            RUNTIME_CLASS(CMDIChildWnd),
            RUNTIME_CLASS(CGraphView));
        AddDocTemplate(pGraphTemplate);

        // User can open same document in different views!

        return TRUE;
    }
};
```

---

### Method 3: Dynamic View Switching (SDI)

```cpp
// Single Document Interface - switch between views

class CMainFrame : public CFrameWnd
{
protected:
    CView* m_pCurrentView;
    CTextView* m_pTextView;
    CGraphView* m_pGraphView;

public:
    void SwitchToView(CRuntimeClass* pViewClass)
    {
        CView* pNewView = NULL;
        CView* pOldView = (CView*)GetActiveView();

        if (pViewClass == RUNTIME_CLASS(CTextView))
        {
            if (m_pTextView == NULL)
            {
                m_pTextView = (CTextView*)CreateView(RUNTIME_CLASS(CTextView));
            }
            pNewView = m_pTextView;
        }
        else if (pViewClass == RUNTIME_CLASS(CGraphView))
        {
            if (m_pGraphView == NULL)
            {
                m_pGraphView = (CGraphView*)CreateView(RUNTIME_CLASS(CGraphView));
            }
            pNewView = m_pGraphView;
        }

        if (pNewView == pOldView)
            return;

        // Switch views
        if (pOldView)
            pOldView->ShowWindow(SW_HIDE);

        pNewView->ShowWindow(SW_SHOW);
        SetActiveView(pNewView);
        pNewView->OnInitialUpdate();
        RecalcLayout();
    }

    CView* CreateView(CRuntimeClass* pViewClass)
    {
        CCreateContext context;
        context.m_pCurrentDoc = GetActiveDocument();
        context.m_pNewViewClass = pViewClass;

        CView* pView = (CView*)pViewClass->CreateObject();
        pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
                     CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, &context);

        return pView;
    }
};
```

---

## 3. View Coordination and Synchronization

### The UpdateAllViews() Pattern

```cpp
// In Document
void CMyDocument::SetValue(int value)
{
    m_nValue = value;
    SetModifiedFlag();

    // Notify ALL views
    UpdateAllViews(NULL);  // NULL = update all views
}

// Better: Use hints for efficiency
void CMyDocument::SetValue(int value)
{
    m_nValue = value;
    SetModifiedFlag();

    // Pass hint about what changed
    UpdateAllViews(NULL, HINT_VALUE_CHANGED, (CObject*)&value);
}
```

---

### Receiving Updates in Views

```cpp
// TextView.cpp
void CTextView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    if (lHint == 0)  // Full update
    {
        Invalidate();
    }
    else if (lHint == HINT_VALUE_CHANGED)
    {
        // Only update value display
        UpdateValueDisplay();
    }
    else if (lHint == HINT_COLOR_CHANGED)
    {
        // Only update colors
        UpdateColors();
        Invalidate();
    }
}

// GraphView.cpp
void CGraphView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    if (lHint == HINT_VALUE_CHANGED)
    {
        // Recalculate graph
        RecalculateGraph();
        Invalidate();
    }
    else
    {
        // Full redraw
        Invalidate();
    }
}
```

---

### Update Hints - Best Practices

```cpp
// In a header file shared by document and all views
// DocumentHints.h

#define HINT_FULL_UPDATE        0L
#define HINT_VALUE_CHANGED      1L
#define HINT_COLOR_CHANGED      2L
#define HINT_OBJECT_ADDED       3L
#define HINT_OBJECT_REMOVED     4L
#define HINT_OBJECT_MODIFIED    5L
#define HINT_SELECTION_CHANGED  6L

// In Document
void CMyDocument::AddObject(CDrawObject* pObj)
{
    m_objects.Add(pObj);
    SetModifiedFlag();

    // Pass hint AND pointer to new object
    UpdateAllViews(NULL, HINT_OBJECT_ADDED, pObj);
}

void CMyDocument::RemoveObject(int index)
{
    if (index >= 0 && index < m_objects.GetSize())
    {
        CDrawObject* pObj = m_objects[index];
        m_objects.RemoveAt(index);
        delete pObj;
        SetModifiedFlag();

        // Pass hint with index
        UpdateAllViews(NULL, HINT_OBJECT_REMOVED, (CObject*)(LONG_PTR)index);
    }
}

// In Views
void CListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    CListCtrl& list = GetListCtrl();

    switch (lHint)
    {
    case HINT_OBJECT_ADDED:
        {
            CDrawObject* pObj = (CDrawObject*)pHint;
            int index = list.GetItemCount();
            list.InsertItem(index, pObj->GetName());
        }
        break;

    case HINT_OBJECT_REMOVED:
        {
            int index = (int)(LONG_PTR)pHint;
            list.DeleteItem(index);
        }
        break;

    case HINT_FULL_UPDATE:
    default:
        RefreshList();
        break;
    }
}
```

---

## 4. Dynamic View Creation

### Creating Views at Runtime

```cpp
class CMainFrame : public CFrameWnd
{
protected:
    CRuntimeClass* m_pActiveViewClass;
    CView* m_pActiveView;

public:
    CView* CreateNewView(CRuntimeClass* pViewClass)
    {
        CDocument* pDoc = GetActiveDocument();
        if (!pDoc)
            return NULL;

        // Create context
        CCreateContext context;
        context.m_pCurrentDoc = pDoc;
        context.m_pNewViewClass = pViewClass;
        context.m_pNewDocTemplate = pDoc->GetDocTemplate();

        // Create view
        CView* pView = (CView*)pViewClass->CreateObject();

        if (pView == NULL)
            return NULL;

        // Create window
        DWORD dwStyle = AFX_WS_DEFAULT_VIEW;
        CRect rect(0, 0, 0, 0);

        if (!pView->Create(NULL, NULL, dwStyle, rect,
                          this, AFX_IDW_PANE_FIRST, &context))
        {
            TRACE0("Failed to create view\n");
            return NULL;
        }

        // Initialize view
        pView->OnInitialUpdate();

        return pView;
    }

    void SwitchView(CRuntimeClass* pNewViewClass)
    {
        CView* pOldActiveView = GetActiveView();

        // If already active, done
        if (pOldActiveView->IsKindOf(pNewViewClass))
            return;

        // Create new view
        CView* pNewView = CreateNewView(pNewViewClass);
        if (!pNewView)
            return;

        // Hide old view
        pOldActiveView->ShowWindow(SW_HIDE);
        pOldActiveView->SetDlgCtrlID(0);

        // Show new view
        pNewView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
        pNewView->ShowWindow(SW_SHOW);

        // Activate new view
        SetActiveView(pNewView);
        RecalcLayout();

        // Delete old view (optional - or cache for reuse)
        pOldActiveView->DestroyWindow();
    }
};
```

---

## 5. View Switching

### Menu-Based View Switching

```cpp
// In MainFrame.h
class CMainFrame : public CFrameWnd
{
protected:
    afx_msg void OnViewText();
    afx_msg void OnViewGraph();
    afx_msg void OnViewGrid();
    afx_msg void OnUpdateViewText(CCmdUI* pCmdUI);
    afx_msg void OnUpdateViewGraph(CCmdUI* pCmdUI);
    afx_msg void OnUpdateViewGrid(CCmdUI* pCmdUI);

    DECLARE_MESSAGE_MAP()
};

// In MainFrame.cpp
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_COMMAND(ID_VIEW_TEXT, &CMainFrame::OnViewText)
    ON_COMMAND(ID_VIEW_GRAPH, &CMainFrame::OnViewGraph)
    ON_COMMAND(ID_VIEW_GRID, &CMainFrame::OnViewGrid)
    ON_UPDATE_COMMAND_UI(ID_VIEW_TEXT, &CMainFrame::OnUpdateViewText)
    ON_UPDATE_COMMAND_UI(ID_VIEW_GRAPH, &CMainFrame::OnUpdateViewGraph)
    ON_UPDATE_COMMAND_UI(ID_VIEW_GRID, &CMainFrame::OnUpdateViewGrid)
END_MESSAGE_MAP()

void CMainFrame::OnViewText()
{
    SwitchView(RUNTIME_CLASS(CTextView));
}

void CMainFrame::OnViewGraph()
{
    SwitchView(RUNTIME_CLASS(CGraphView));
}

void CMainFrame::OnViewGrid()
{
    SwitchView(RUNTIME_CLASS(CGridView));
}

void CMainFrame::OnUpdateViewText(CCmdUI* pCmdUI)
{
    CView* pView = GetActiveView();
    pCmdUI->SetRadio(pView->IsKindOf(RUNTIME_CLASS(CTextView)));
}

void CMainFrame::OnUpdateViewGraph(CCmdUI* pCmdUI)
{
    CView* pView = GetActiveView();
    pCmdUI->SetRadio(pView->IsKindOf(RUNTIME_CLASS(CGraphView)));
}

void CMainFrame::OnUpdateViewGrid(CCmdUI* pCmdUI)
{
    CView* pView = GetActiveView();
    pCmdUI->SetRadio(pView->IsKindOf(RUNTIME_CLASS(CGridView)));
}
```

---

### Toolbar-Based View Switching

```cpp
// Add buttons to toolbar
// IDC_VIEW_TEXT, IDC_VIEW_GRAPH, IDC_VIEW_GRID

void CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    // ... create toolbar ...

    // Set button styles to radio buttons
    m_wndToolBar.SetButtonStyle(
        m_wndToolBar.CommandToIndex(ID_VIEW_TEXT),
        TBBS_BUTTON | TBBS_RADIOBUTTON);

    m_wndToolBar.SetButtonStyle(
        m_wndToolBar.CommandToIndex(ID_VIEW_GRAPH),
        TBBS_BUTTON | TBBS_RADIOBUTTON);

    m_wndToolBar.SetButtonStyle(
        m_wndToolBar.CommandToIndex(ID_VIEW_GRID),
        TBBS_BUTTON | TBBS_RADIOBUTTON);
}
```

---

## 6. View-Specific Commands

### Routing Commands to Specific Views

```cpp
// Some commands only make sense in certain views

// In TextView.h
class CTextView : public CEditView
{
protected:
    afx_msg void OnTextBold();
    afx_msg void OnTextItalic();
    afx_msg void OnUpdateTextBold(CCmdUI* pCmdUI);
    afx_msg void OnUpdateTextItalic(CCmdUI* pCmdUI);

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CTextView, CEditView)
    ON_COMMAND(ID_TEXT_BOLD, &CTextView::OnTextBold)
    ON_COMMAND(ID_TEXT_ITALIC, &CTextView::OnTextItalic)
    ON_UPDATE_COMMAND_UI(ID_TEXT_BOLD, &CTextView::OnUpdateTextBold)
    ON_UPDATE_COMMAND_UI(ID_TEXT_ITALIC, &CTextView::OnUpdateTextItalic)
END_MESSAGE_MAP()

void CTextView::OnTextBold()
{
    // Handle bold command
}

void CTextView::OnUpdateTextBold(CCmdUI* pCmdUI)
{
    // Enable only in text view
    pCmdUI->Enable(TRUE);
}

// In GraphView.h
class CGraphView : public CView
{
protected:
    afx_msg void OnGraphZoomIn();
    afx_msg void OnGraphZoomOut();
    afx_msg void OnUpdateGraphZoomIn(CCmdUI* pCmdUI);

    DECLARE_MESSAGE_MAP()
};

void CGraphView::OnUpdateGraphZoomIn(CCmdUI* pCmdUI)
{
    // Enable only if can zoom
    pCmdUI->Enable(m_nZoomLevel < MAX_ZOOM);
}

// In MainFrame - commands routed to active view automatically
```

---

## 7. Data Sharing Between Views

### View-to-View Communication

```cpp
// Method 1: Through Document (Recommended)

// View 1 modifies data
void CTextView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CMyDocument* pDoc = GetDocument();

    // Modify document
    pDoc->AppendChar(nChar);
    // Document calls UpdateAllViews()
    // View 2 receives OnUpdate() call

    CEditView::OnChar(nChar, nRepCnt, nFlags);
}

// View 2 receives update
void CGraphView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    if (lHint == HINT_TEXT_CHANGED)
    {
        // Update graph based on new text
        UpdateGraph();
    }
}
```

---

### Shared Selection State

```cpp
// Document tracks selection (shared by all views)
class CMyDocument : public CDocument
{
protected:
    CArray<int> m_selectedItems;

public:
    void SetSelection(const CArray<int>& selection)
    {
        m_selectedItems.Copy(selection);
        UpdateAllViews(NULL, HINT_SELECTION_CHANGED);
    }

    const CArray<int>& GetSelection() const
    {
        return m_selectedItems;
    }
};

// TextView shows selection
void CTextView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    if (lHint == HINT_SELECTION_CHANGED)
    {
        HighlightSelection();
    }
}

// ListView shows selection
void CListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    if (lHint == HINT_SELECTION_CHANGED)
    {
        CMyDocument* pDoc = GetDocument();
        const CArray<int>& selection = pDoc->GetSelection();

        CListCtrl& list = GetListCtrl();

        // Clear old selection
        for (int i = 0; i < list.GetItemCount(); i++)
        {
            list.SetItemState(i, 0, LVIS_SELECTED);
        }

        // Set new selection
        for (int i = 0; i < selection.GetSize(); i++)
        {
            int index = selection[i];
            list.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
        }
    }
}
```

---

## 8. Performance Considerations

### Lazy Updates

```cpp
class CGraphView : public CView
{
protected:
    BOOL m_bNeedsRecalc;
    CArray<double> m_calculatedData;

    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
    {
        // Mark as needing recalculation
        m_bNeedsRecalc = TRUE;

        // Don't recalculate now - wait for OnDraw
        Invalidate();
    }

    virtual void OnDraw(CDC* pDC)
    {
        if (m_bNeedsRecalc)
        {
            RecalculateGraph();  // Expensive operation
            m_bNeedsRecalc = FALSE;
        }

        DrawGraph(pDC);  // Fast operation
    }
};
```

---

### Partial Updates

```cpp
void CMyDocument::ModifyObject(int index)
{
    // Instead of full update:
    // UpdateAllViews(NULL);

    // Partial update with hints:
    UpdateAllViews(NULL, HINT_OBJECT_MODIFIED, (CObject*)(LONG_PTR)index);
}

void CListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    if (lHint == HINT_OBJECT_MODIFIED)
    {
        int index = (int)(LONG_PTR)pHint;

        // Update only one item
        CListCtrl& list = GetListCtrl();
        CMyDocument* pDoc = GetDocument();
        CMyObject* pObj = pDoc->GetObject(index);

        list.SetItemText(index, 0, pObj->GetName());
        list.SetItemText(index, 1, pObj->GetType());

        // Much faster than refreshing entire list!
    }
}
```

---

## 9. Complete Multi-View Application

### Example: Data Analysis Application

```cpp
// DataDoc.h
class CDataDoc : public CDocument
{
protected:
    CArray<double> m_values;
    CString m_strDataName;

    // Statistics (cached)
    double m_mean;
    double m_stdDev;
    BOOL m_bStatsDirty;

public:
    void AddValue(double value);
    void RemoveValue(int index);
    void SetValue(int index, double value);

    int GetValueCount() const { return (int)m_values.GetSize(); }
    double GetValue(int index) const;

    double GetMean();
    double GetStdDev();

protected:
    void CalculateStatistics();

    DECLARE_DYNCREATE(CDataDoc)
};

// DataDoc.cpp
void CDataDoc::AddValue(double value)
{
    m_values.Add(value);
    m_bStatsDirty = TRUE;
    SetModifiedFlag();
    UpdateAllViews(NULL, HINT_VALUE_ADDED, (CObject*)&value);
}

void CDataDoc::CalculateStatistics()
{
    if (!m_bStatsDirty)
        return;

    int count = (int)m_values.GetSize();
    if (count == 0)
    {
        m_mean = 0.0;
        m_stdDev = 0.0;
        return;
    }

    // Calculate mean
    double sum = 0.0;
    for (int i = 0; i < count; i++)
    {
        sum += m_values[i];
    }
    m_mean = sum / count;

    // Calculate standard deviation
    double sumSq = 0.0;
    for (int i = 0; i < count; i++)
    {
        double diff = m_values[i] - m_mean;
        sumSq += diff * diff;
    }
    m_stdDev = sqrt(sumSq / count);

    m_bStatsDirty = FALSE;
}

// TableView.h - Shows values in list
class CTableView : public CListView
{
protected:
    virtual void OnInitialUpdate();
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

    afx_msg void OnAddValue();
    afx_msg void OnDeleteValue();

    DECLARE_DYNCREATE(CTableView)
    DECLARE_MESSAGE_MAP()
};

// TableView.cpp
void CTableView::OnInitialUpdate()
{
    CListView::OnInitialUpdate();

    CListCtrl& list = GetListCtrl();
    list.ModifyStyle(0, LVS_REPORT);

    list.InsertColumn(0, _T("Index"), LVCFMT_LEFT, 100);
    list.InsertColumn(1, _T("Value"), LVCFMT_RIGHT, 150);

    RefreshList();
}

void CTableView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    CListCtrl& list = GetListCtrl();
    CDataDoc* pDoc = GetDocument();

    if (lHint == HINT_VALUE_ADDED)
    {
        int index = pDoc->GetValueCount() - 1;
        CString strIndex, strValue;
        strIndex.Format(_T("%d"), index);
        strValue.Format(_T("%.2f"), pDoc->GetValue(index));

        int item = list.InsertItem(index, strIndex);
        list.SetItemText(item, 1, strValue);
    }
    else
    {
        RefreshList();
    }
}

// ChartView.h - Shows values as bar chart
class CChartView : public CView
{
protected:
    virtual void OnDraw(CDC* pDC);
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

    DECLARE_DYNCREATE(CChartView)
};

// ChartView.cpp
void CChartView::OnDraw(CDC* pDC)
{
    CDataDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    CRect rect;
    GetClientRect(&rect);

    // Draw chart
    int count = pDoc->GetValueCount();
    if (count == 0)
        return;

    double maxValue = 0.0;
    for (int i = 0; i < count; i++)
    {
        double value = pDoc->GetValue(i);
        if (value > maxValue)
            maxValue = value;
    }

    int barWidth = rect.Width() / max(count, 1);
    int barSpacing = 2;

    for (int i = 0; i < count; i++)
    {
        double value = pDoc->GetValue(i);
        int barHeight = (int)((value / maxValue) * (rect.Height() - 40));

        int x = i * barWidth;
        int y = rect.bottom - barHeight - 20;

        CRect barRect(x + barSpacing, y,
                     x + barWidth - barSpacing, rect.bottom - 20);

        CBrush brush(RGB(100, 150, 200));
        pDC->FillRect(&barRect, &brush);
    }
}

// StatsView.h - Shows statistics
class CStatsView : public CFormView
{
public:
    enum { IDD = IDD_STATS_FORM };

protected:
    CStatic m_lblMean;
    CStatic m_lblStdDev;
    CStatic m_lblCount;

    virtual void DoDataExchange(CDataExchange* pDX);
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

    DECLARE_DYNCREATE(CStatsView)
};

void CStatsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    CDataDoc* pDoc = GetDocument();

    CString str;

    str.Format(_T("Count: %d"), pDoc->GetValueCount());
    m_lblCount.SetWindowText(str);

    str.Format(_T("Mean: %.2f"), pDoc->GetMean());
    m_lblMean.SetWindowText(str);

    str.Format(_T("Std Dev: %.2f"), pDoc->GetStdDev());
    m_lblStdDev.SetWindowText(str);
}
```

---

## 10. Exercises

### Exercise 1: Dual View

Create an application with two views:
- Text view (CEditView)
- Word count view (shows character count, word count, line count)

---

### Exercise 2: View Switching

Create an application with three switchable views:
- List view
- Icon view
- Details view

---

### Exercise 3: Synchronized Selection

Create two views that maintain synchronized selection:
- When item selected in view 1, automatically select in view 2

---

### Exercise 4: Chart and Table

Create an application with:
- Table view (CListView) to edit numbers
- Chart view (CView) to display as bar chart
- Changes in table immediately update chart

---

### Exercise 5: Mini-Map

Create a large scrollable drawing view with a minimap view showing the entire document and current viewport.

---

## Key Takeaways

1. ✅ Multiple views display same document differently
2. ✅ UpdateAllViews() synchronizes all views
3. ✅ Use hints for efficient updates
4. ✅ Views communicate through document
5. ✅ Dynamic view creation enables view switching
6. ✅ Each view type can have specific commands
7. ✅ Share selection state via document
8. ✅ Use lazy updates for performance
9. ✅ Partial updates faster than full refresh
10. ✅ Cache calculated data when possible

---

## Summary

Multiple views are powerful for showing data in different ways. Master view coordination and you can build sophisticated applications with rich user interfaces!

---

## Next Lesson Preview

In **Lesson 23**, we'll explore:
- Splitter windows in detail
- Static vs dynamic splitters
- Nested splitters
- Custom splitter behavior

**Congratulations on completing Lesson 22!**
