# Lesson 75: Drag and Drop

**Duration**: 6 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to OLE Drag and Drop
2. Drag Source Implementation
3. Drop Target Implementation
4. COleDropTarget Class
5. COleDataSource Class
6. Visual Feedback
7. File Drag and Drop
8. Advanced Techniques
9. Best Practices
10. Exercises and Projects

---

## 1. Introduction to OLE Drag and Drop

### Drag and Drop Concepts

```cpp
/*
OLE DRAG AND DROP:
- Based on OLE (Object Linking and Embedding)
- Inter-application support
- Multiple data formats
- Visual feedback

COMPONENTS:
- Drag Source: Initiates drag operation
- Drop Target: Receives dropped data
- Data Object: Contains dragged data
- Drop Effect: Copy, Move, Link

TYPICAL FLOW:
1. User starts drag (mouse down + move)
2. DoDragDrop called
3. Cursor changes over drop target
4. OnDragEnter, OnDragOver events
5. User releases mouse
6. OnDrop event
7. Data transferred
*/
```

---

## 2. Drag Source Implementation

### COleDataSource

```cpp
class CDragSourceView : public CView
{
protected:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

    DECLARE_MESSAGE_MAP()

private:
    CPoint m_ptStartDrag;
    BOOL m_bDragging;
};

BEGIN_MESSAGE_MAP(CDragSourceView, CView)
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CDragSourceView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_ptStartDrag = point;
    m_bDragging = FALSE;

    CView::OnLButtonDown(nFlags, point);
}

void CDragSourceView::OnMouseMove(UINT nFlags, CPoint point)
{
    if ((nFlags & MK_LBUTTON) && !m_bDragging)
    {
        // Check if drag threshold exceeded
        CSize dragSize(abs(point.x - m_ptStartDrag.x),
                      abs(point.y - m_ptStartDrag.y));

        if (dragSize.cx > 5 || dragSize.cy > 5)
        {
            m_bDragging = TRUE;

            // Create data source
            COleDataSource dataSource;

            // Add text data
            CString strData = _T("Dragged Text Data");

            HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE,
                (strData.GetLength() + 1) * sizeof(TCHAR));

            if (hGlobal)
            {
                LPTSTR lpszData = (LPTSTR)GlobalLock(hGlobal);
                _tcscpy_s(lpszData, strData.GetLength() + 1, strData);
                GlobalUnlock(hGlobal);

                dataSource.CacheGlobalData(CF_UNICODETEXT, hGlobal);

                // Perform drag-drop operation
                DROPEFFECT dropEffect = dataSource.DoDragDrop(
                    DROPEFFECT_COPY | DROPEFFECT_MOVE);

                if (dropEffect == DROPEFFECT_MOVE)
                {
                    // Handle move operation
                    TRACE(_T("Data moved\n"));
                }
                else if (dropEffect == DROPEFFECT_COPY)
                {
                    TRACE(_T("Data copied\n"));
                }
            }

            m_bDragging = FALSE;
        }
    }

    CView::OnMouseMove(nFlags, point);
}
```

---

## 3. Drop Target Implementation

### COleDropTarget

```cpp
class CDropTargetView : public CView
{
public:
    CDropTargetView();

protected:
    virtual void OnInitialUpdate();

    DECLARE_MESSAGE_MAP()

private:
    COleDropTarget m_dropTarget;
};

// In view class
void CDropTargetView::OnInitialUpdate()
{
    CView::OnInitialUpdate();

    // Register drop target
    m_dropTarget.Register(this);
}

// Create custom drop target class
class CMyDropTarget : public COleDropTarget
{
public:
    virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
                                   DWORD dwKeyState, CPoint point);
    virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
                                  DWORD dwKeyState, CPoint point);
    virtual void OnDragLeave(CWnd* pWnd);
    virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
                       DROPEFFECT dropEffect, CPoint point);
};

DROPEFFECT CMyDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
                                      DWORD dwKeyState, CPoint point)
{
    // Check if we can handle this data
    if (pDataObject->IsDataAvailable(CF_UNICODETEXT))
    {
        // Determine drop effect based on key state
        if (dwKeyState & MK_CONTROL)
            return DROPEFFECT_COPY;
        else
            return DROPEFFECT_MOVE;
    }

    return DROPEFFECT_NONE;
}

DROPEFFECT CMyDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
                                     DWORD dwKeyState, CPoint point)
{
    // Same logic as OnDragEnter
    return OnDragEnter(pWnd, pDataObject, dwKeyState, point);
}

void CMyDropTarget::OnDragLeave(CWnd* pWnd)
{
    // Clean up any visual feedback
    pWnd->Invalidate();
}

BOOL CMyDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
                          DROPEFFECT dropEffect, CPoint point)
{
    if (pDataObject->IsDataAvailable(CF_UNICODETEXT))
    {
        HGLOBAL hGlobal = pDataObject->GetGlobalData(CF_UNICODETEXT);

        if (hGlobal)
        {
            LPTSTR lpszText = (LPTSTR)GlobalLock(hGlobal);

            if (lpszText)
            {
                CString strText = lpszText;
                GlobalUnlock(hGlobal);

                // Process dropped text
                CDropTargetView* pView = (CDropTargetView*)pWnd;
                AfxMessageBox(_T("Dropped: ") + strText);

                return TRUE;
            }
        }
    }

    return FALSE;
}
```

---

## 4. COleDropTarget Class

### Complete Drop Target Example

```cpp
class CListDropTarget : public COleDropTarget
{
public:
    CListDropTarget(CListCtrl* pList);

    virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
                                   DWORD dwKeyState, CPoint point);
    virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
                                  DWORD dwKeyState, CPoint point);
    virtual void OnDragLeave(CWnd* pWnd);
    virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
                       DROPEFFECT dropEffect, CPoint point);

private:
    CListCtrl* m_pList;
    int m_nDropIndex;
};

CListDropTarget::CListDropTarget(CListCtrl* pList)
    : m_pList(pList), m_nDropIndex(-1)
{
}

DROPEFFECT CListDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
                                        DWORD dwKeyState, CPoint point)
{
    if (!pDataObject->IsDataAvailable(CF_UNICODETEXT))
        return DROPEFFECT_NONE;

    return (dwKeyState & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
}

DROPEFFECT CListDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
                                       DWORD dwKeyState, CPoint point)
{
    if (!pDataObject->IsDataAvailable(CF_UNICODETEXT))
        return DROPEFFECT_NONE;

    // Find insertion point
    LVHITTESTINFO hitTest;
    hitTest.pt = point;
    int nIndex = m_pList->HitTest(&hitTest);

    if (nIndex != m_nDropIndex)
    {
        m_nDropIndex = nIndex;
        // Update visual feedback
    }

    return (dwKeyState & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
}

void CListDropTarget::OnDragLeave(CWnd* pWnd)
{
    m_nDropIndex = -1;
    m_pList->Invalidate();
}

BOOL CListDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
                            DROPEFFECT dropEffect, CPoint point)
{
    if (!pDataObject->IsDataAvailable(CF_UNICODETEXT))
        return FALSE;

    HGLOBAL hGlobal = pDataObject->GetGlobalData(CF_UNICODETEXT);

    if (!hGlobal)
        return FALSE;

    LPTSTR lpszText = (LPTSTR)GlobalLock(hGlobal);

    if (lpszText)
    {
        CString strText = lpszText;
        GlobalUnlock(hGlobal);

        // Insert item at drop location
        int nIndex = (m_nDropIndex >= 0) ? m_nDropIndex : m_pList->GetItemCount();
        m_pList->InsertItem(nIndex, strText);

        return TRUE;
    }

    return FALSE;
}
```

---

## 5. COleDataSource Class

### Multiple Data Formats

```cpp
void CreateMultiFormatDataSource(COleDataSource& dataSource, const CString& strText)
{
    // Add text format
    HGLOBAL hText = GlobalAlloc(GMEM_MOVEABLE,
        (strText.GetLength() + 1) * sizeof(TCHAR));

    if (hText)
    {
        LPTSTR lpszText = (LPTSTR)GlobalLock(hText);
        _tcscpy_s(lpszText, strText.GetLength() + 1, strText);
        GlobalUnlock(hText);

        dataSource.CacheGlobalData(CF_UNICODETEXT, hText);
    }

    // Add custom format
    UINT cfCustom = RegisterClipboardFormat(_T("MyApp.CustomData"));

    struct CustomData
    {
        int nValue;
        TCHAR szText[100];
    };

    HGLOBAL hCustom = GlobalAlloc(GMEM_MOVEABLE, sizeof(CustomData));

    if (hCustom)
    {
        CustomData* pData = (CustomData*)GlobalLock(hCustom);
        pData->nValue = 42;
        _tcscpy_s(pData->szText, 100, strText);
        GlobalUnlock(hCustom);

        dataSource.CacheGlobalData(cfCustom, hCustom);
    }
}
```

---

## 6. Visual Feedback

### Custom Drag Cursor

```cpp
class CCustomDragSource
{
public:
    static DROPEFFECT DoDragDrop(COleDataSource* pDataSource, DWORD dwEffects);

private:
    static SCODE STDMETHODCALLTYPE QueryContinueDrag(BOOL bEscapePressed,
                                                     DWORD dwKeyState);
    static SCODE STDMETHODCALLTYPE GiveFeedback(DWORD dwEffect);
};

DROPEFFECT CCustomDragSource::DoDragDrop(COleDataSource* pDataSource,
                                        DWORD dwEffects)
{
    // Set custom cursors
    HCURSOR hCursorCopy = AfxGetApp()->LoadCursor(IDC_CURSOR_COPY);
    HCURSOR hCursorMove = AfxGetApp()->LoadCursor(IDC_CURSOR_MOVE);
    HCURSOR hCursorNone = AfxGetApp()->LoadCursor(IDC_CURSOR_NONE);

    // Perform drag-drop with custom feedback
    return pDataSource->DoDragDrop(dwEffects);
}
```

---

## 7. File Drag and Drop

### Accepting Files from Explorer

```cpp
class CFileDropTarget : public COleDropTarget
{
public:
    virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
                       DROPEFFECT dropEffect, CPoint point);

private:
    void ProcessDroppedFiles(HDROP hDrop);
};

BOOL CFileDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
                             DROPEFFECT dropEffect, CPoint point)
{
    if (pDataObject->IsDataAvailable(CF_HDROP))
    {
        HGLOBAL hGlobal = pDataObject->GetGlobalData(CF_HDROP);

        if (hGlobal)
        {
            HDROP hDrop = (HDROP)hGlobal;
            ProcessDroppedFiles(hDrop);
            return TRUE;
        }
    }

    return FALSE;
}

void CFileDropTarget::ProcessDroppedFiles(HDROP hDrop)
{
    UINT nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);

    for (UINT i = 0; i < nFiles; i++)
    {
        TCHAR szFilePath[MAX_PATH];
        DragQueryFile(hDrop, i, szFilePath, MAX_PATH);

        CString msg;
        msg.Format(_T("Dropped file: %s"), szFilePath);
        TRACE(_T("%s\n"), msg);

        // Process file...
    }
}
```

---

## 8. Advanced Techniques

### Delayed Rendering

```cpp
class CDelayedDataSource : public COleDataSource
{
public:
    virtual BOOL OnRenderGlobalData(LPFORMATETC lpFormatEtc, HGLOBAL* phGlobal);

private:
    CString m_strData;
};

BOOL CDelayedDataSource::OnRenderGlobalData(LPFORMATETC lpFormatEtc,
                                           HGLOBAL* phGlobal)
{
    if (lpFormatEtc->cfFormat == CF_UNICODETEXT)
    {
        // Generate data on demand
        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE,
            (m_strData.GetLength() + 1) * sizeof(TCHAR));

        if (hGlobal)
        {
            LPTSTR lpszData = (LPTSTR)GlobalLock(hGlobal);
            _tcscpy_s(lpszData, m_strData.GetLength() + 1, m_strData);
            GlobalUnlock(hGlobal);

            *phGlobal = hGlobal;
            return TRUE;
        }
    }

    return FALSE;
}
```

---

## 9. Best Practices

```cpp
/*
DRAG AND DROP BEST PRACTICES:

1. USER EXPERIENCE
   - Provide clear visual feedback
   - Use appropriate cursors
   - Highlight drop targets
   - Support standard key modifiers (Ctrl, Shift)

2. DATA HANDLING
   - Support multiple formats
   - Use standard clipboard formats
   - Register custom formats once
   - Implement delayed rendering for large data

3. ERROR HANDLING
   - Validate dropped data
   - Check memory allocations
   - Handle failed operations gracefully
   - Provide user feedback

4. PERFORMANCE
   - Don't block during drag
   - Cache data when possible
   - Use delayed rendering
   - Minimize OnDragOver processing

5. COMPATIBILITY
   - Support standard formats
   - Work with other applications
   - Test with Explorer
   - Handle edge cases

6. CLEANUP
   - Release resources properly
   - Unregister drop targets
   - Free global memory
   - Handle exceptions

7. ACCESSIBILITY
   - Provide keyboard alternatives
   - Support screen readers
   - Clear visual indicators
   - Document drag-drop features
*/
```

---

## Key Takeaways

1. **OLE drag-drop** enables data transfer
2. **COleDataSource** provides dragged data
3. **COleDropTarget** receives dropped data
4. **Visual feedback** improves user experience
5. **Multiple formats** enhance compatibility
6. **File drops** integrate with Explorer
7. **Delayed rendering** optimizes performance
8. **Best practices** ensure robust implementation
9. **Error handling** prevents crashes
10. **Testing** ensures compatibility

---

## Course Completion

Congratulations! You've completed **MFC Advanced Topics (Lessons 61-75)**!

### What You've Learned

1. **Multithreading**: Worker and UI threads, synchronization
2. **DLLs**: Regular and extension DLLs
3. **ActiveX**: Controls and COM integration
4. **Databases**: ODBC and ADO
5. **Networking**: Sockets and WinInet
6. **System Integration**: Registry, INI files, clipboard
7. **User Interaction**: Drag and drop

### Next Steps

- Practice building complete applications
- Explore Windows API documentation
- Study design patterns
- Build portfolio projects
- Contribute to open-source
- Stay updated with modern C++ standards

**Congratulations on mastering MFC Advanced Topics!**
