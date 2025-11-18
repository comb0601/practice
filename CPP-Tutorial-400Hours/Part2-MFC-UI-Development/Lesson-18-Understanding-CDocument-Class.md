# Lesson 18: Understanding CDocument Class

**Duration**: 8 hours
**Difficulty**: Intermediate

## Table of Contents
1. The CDocument Class - Deep Dive
2. Document Lifecycle
3. Data Management in Documents
4. The Modified Flag System
5. Document Views Relationship
6. Serialization Architecture
7. Document Templates and Registration
8. Multi-Document Type Support
9. Best Practices for Document Design
10. Complete Real-World Examples
11. Exercises

---

## 1. The CDocument Class - Deep Dive

### What is CDocument?

`CDocument` is the **MODEL** in the Model-View-Controller (MVC) pattern used by MFC. It represents your application's data and business logic.

```cpp
class CDocument : public CCmdTarget
{
protected:
    CDocument();  // Protected constructor - use DECLARE_DYNCREATE

public:
    // Document data management
    virtual BOOL OnNewDocument();
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();
    virtual void Serialize(CArchive& ar);

    // Modified flag
    void SetModifiedFlag(BOOL bModified = TRUE);
    BOOL IsModified();
    BOOL SaveModified();

    // View management
    void UpdateAllViews(CView* pSender, LPARAM lHint = 0L,
                       CObject* pHint = NULL);
    POSITION GetFirstViewPosition() const;
    CView* GetNextView(POSITION& rPosition) const;

    // File path
    const CString& GetPathName() const;
    const CString& GetTitle() const;
    void SetTitle(LPCTSTR lpszTitle);
    void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);

    // Document template
    CDocTemplate* GetDocTemplate() const;

protected:
    CString m_strPathName;      // Full path of document
    CString m_strTitle;         // Document title
    BOOL m_bModified;           // Modified flag
    CDocTemplate* m_pDocTemplate;  // Associated template
    CPtrList m_viewList;        // List of views
};
```

---

## 2. Document Lifecycle

### 2.1 Creation Sequence

```
User Action              → Framework Calls        → You Override
═══════════════════════════════════════════════════════════════════

File → New               → OnNewDocument()        → Initialize data
                          ↓
                         DeleteContents()        → Clean up old data
                          ↓
                         SetModifiedFlag(FALSE)
                          ↓
                         UpdateAllViews()

───────────────────────────────────────────────────────────────────

File → Open              → DeleteContents()       → Clean up old data
                          ↓
                         OnOpenDocument()        → Custom pre-load
                          ↓
                         Serialize(ar)           → Read data from file
                          ↓
                         SetModifiedFlag(FALSE)
                          ↓
                         UpdateAllViews()

───────────────────────────────────────────────────────────────────

File → Save              → OnSaveDocument()       → Custom pre-save
                          ↓
                         Serialize(ar)           → Write data to file
                          ↓
                         SetModifiedFlag(FALSE)

───────────────────────────────────────────────────────────────────

File → Close             → SaveModified()         → Prompt if modified
(or Exit)                 ↓
                         OnCloseDocument()       → Cleanup
                          ↓
                         DeleteContents()        → Delete data
                          ↓
                         Document destroyed
```

---

### 2.2 OnNewDocument() - Detailed

```cpp
// Default implementation
BOOL CDocument::OnNewDocument()
{
    DeleteContents();           // Clear old data
    SetModifiedFlag(FALSE);     // Not modified yet
    // m_strPathName is empty (not saved yet)
    return TRUE;
}

// Your override
BOOL CMyDocument::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // *** INITIALIZE YOUR DATA HERE ***

    // Simple data
    m_strText.Empty();
    m_nCounter = 0;
    m_fValue = 0.0f;

    // Arrays
    m_points.RemoveAll();
    m_strings.RemoveAll();

    // Complex objects
    m_data.Reset();

    // Set defaults
    m_font.CreatePointFont(100, _T("Arial"));
    m_color = RGB(0, 0, 0);

    return TRUE;
}
```

**WHEN CALLED**:
- Application startup
- File → New
- After closing modified document (if user chooses "Don't Save")

**WHY OVERRIDE**: To initialize document data to default state.

---

### 2.3 OnOpenDocument() - Detailed

```cpp
// Default implementation
BOOL CDocument::OnOpenDocument(LPCTSTR lpszPathName)
{
    DeleteContents();           // Clear old data
    SetModifiedFlag(FALSE);     // Loading - not modified

    CFile file;
    if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite))
    {
        // Show error message
        return FALSE;
    }

    CArchive ar(&file, CArchive::load);
    Serialize(ar);              // Load data
    ar.Close();
    file.Close();

    SetPathName(lpszPathName);  // Remember file path
    return TRUE;
}

// Your override
BOOL CMyDocument::OnOpenDocument(LPCTSTR lpszPathName)
{
    // Call base class (loads via Serialize)
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;

    // *** CUSTOM POST-LOAD PROCESSING ***

    // Validate loaded data
    if (!ValidateData())
    {
        AfxMessageBox(_T("Invalid data in file"));
        OnNewDocument();  // Reset to empty
        return FALSE;
    }

    // Convert old format to new
    if (m_nFileVersion < CURRENT_VERSION)
    {
        ConvertOldFormat();
    }

    // Rebuild derived data
    RebuildIndexes();
    CalculateDerivedValues();

    // Update UI
    UpdateAllViews(NULL);

    return TRUE;
}
```

**WHEN CALLED**:
- File → Open
- Double-click file in Explorer
- Command line: `MyApp.exe file.txt`
- Drag-drop file onto application

**WHY OVERRIDE**: To add custom post-load processing, validation, or format conversion.

---

### 2.4 OnSaveDocument() - Detailed

```cpp
// Default implementation
BOOL CDocument::OnSaveDocument(LPCTSTR lpszPathName)
{
    CFile file;
    if (!file.Open(lpszPathName,
        CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive))
    {
        // Show error message
        return FALSE;
    }

    CArchive ar(&file, CArchive::store);
    Serialize(ar);              // Save data
    ar.Close();
    file.Close();

    SetModifiedFlag(FALSE);     // Now saved - not modified
    SetPathName(lpszPathName);  // Remember file path
    return TRUE;
}

// Your override
BOOL CMyDocument::OnSaveDocument(LPCTSTR lpszPathName)
{
    // *** CUSTOM PRE-SAVE PROCESSING ***

    // Validate data before saving
    if (!ValidateData())
    {
        AfxMessageBox(_T("Cannot save - invalid data"));
        return FALSE;
    }

    // Update metadata
    m_timeLastSaved = CTime::GetCurrentTime();
    m_nSaveCount++;

    // Create backup
    if (PathFileExists(lpszPathName))
    {
        CString backup = lpszPathName;
        backup += _T(".bak");
        CopyFile(lpszPathName, backup, FALSE);
    }

    // Call base class (saves via Serialize)
    if (!CDocument::OnSaveDocument(lpszPathName))
        return FALSE;

    // *** CUSTOM POST-SAVE PROCESSING ***

    // Update status bar
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    if (pFrame)
    {
        pFrame->SetMessageText(_T("Document saved successfully"));
    }

    return TRUE;
}
```

**WHEN CALLED**:
- File → Save (if document has path)
- File → Save As
- Closing modified document (if user chooses "Save")

**WHY OVERRIDE**: To add validation, backup creation, or metadata updates.

---

### 2.5 DeleteContents() - Detailed

```cpp
// Default implementation
void CDocument::DeleteContents()
{
    // Base class does nothing
}

// Your override
void CMyDocument::DeleteContents()
{
    // *** DELETE ALL DOCUMENT DATA ***

    // Strings
    m_strText.Empty();
    m_strAuthor.Empty();

    // Arrays
    m_points.RemoveAll();
    m_lines.RemoveAll();

    // Lists
    POSITION pos = m_objectList.GetHeadPosition();
    while (pos)
    {
        CMyObject* pObj = m_objectList.GetNext(pos);
        delete pObj;  // Delete heap objects!
    }
    m_objectList.RemoveAll();

    // GDI objects
    if (m_font.GetSafeHandle())
        m_font.DeleteObject();

    if (m_bitmap.GetSafeHandle())
        m_bitmap.DeleteObject();

    // Reset counters
    m_nCounter = 0;

    // Call base class
    CDocument::DeleteContents();
}
```

**WHEN CALLED**:
- Before OnNewDocument()
- Before OnOpenDocument()
- Before document destruction

**WHY OVERRIDE**: To properly clean up all document data, especially heap-allocated objects.

**CRITICAL**: Always delete dynamically allocated objects here to prevent memory leaks!

---

### 2.6 Serialize() - Detailed

Covered extensively in Lessons 20-21, but here's the overview:

```cpp
void CMyDocument::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // *** SAVING ***
        ar << m_strText;
        ar << m_nCounter;
        ar << m_fValue;
        m_points.Serialize(ar);
    }
    else
    {
        // *** LOADING ***
        ar >> m_strText;
        ar >> m_nCounter;
        ar >> m_fValue;
        m_points.Serialize(ar);
    }
}
```

**WHEN CALLED**: During OnOpenDocument() and OnSaveDocument()

**WHY OVERRIDE**: To actually save/load your document data.

---

## 3. Data Management in Documents

### 3.1 What Data Goes in CDocument?

```cpp
class CMyDocument : public CDocument
{
protected:
    // ✅ YES - Put these in CDocument:

    // 1. Application data (the "model")
    CString m_strText;                    // Text content
    CArray<CPoint, CPoint&> m_drawPoints; // Drawing points
    CList<CMyObject*, CMyObject*> m_objects;  // Business objects

    // 2. Data that needs to be saved
    CTime m_timeCreated;
    CTime m_timeModified;
    CString m_strAuthor;

    // 3. Business logic state
    int m_nDocumentVersion;
    BOOL m_bReadOnly;
    DWORD m_dwFlags;

    // 4. Metadata
    int m_nSaveCount;
    CString m_strComments;

    // ❌ NO - Don't put these in CDocument:

    // Display preferences (put in CView!)
    // CFont m_displayFont;        // NO!
    // COLORREF m_textColor;       // NO!
    // int m_nZoomLevel;           // NO!

    // UI state (put in CView!)
    // CPoint m_scrollPosition;    // NO!
    // int m_nCaretPos;            // NO!
    // BOOL m_bShowGrid;           // NO! (unless it's saved with document)

    // Window-related (put in CFrameWnd!)
    // CToolBar m_toolbar;         // NO!
    // CStatusBar m_statusBar;     // NO!
};
```

---

### 3.2 Data Access Methods

**ALWAYS** provide accessor methods. **NEVER** make data members public!

```cpp
// ❌ WRONG - Public data
class CMyDocument : public CDocument
{
public:
    CString m_strText;  // BAD - direct access
};

// In view
CString text = pDoc->m_strText;  // Direct access - bad practice

// ✅ RIGHT - Accessor methods
class CMyDocument : public CDocument
{
protected:
    CString m_strText;  // GOOD - protected

public:
    // Getter
    CString GetText() const { return m_strText; }

    // Setter
    void SetText(const CString& text)
    {
        m_strText = text;
        SetModifiedFlag();      // Mark as modified
        UpdateAllViews(NULL);   // Notify views
    }

    // Append method
    void AppendText(const CString& text)
    {
        m_strText += text;
        SetModifiedFlag();
        UpdateAllViews(NULL);
    }

    // Query methods
    int GetTextLength() const { return m_strText.GetLength(); }
    BOOL IsEmpty() const { return m_strText.IsEmpty(); }
};

// In view
CString text = pDoc->GetText();  // Controlled access - good!
pDoc->SetText(_T("Hello"));      // Ensures modified flag and update
```

**WHY USE ACCESSORS**:
1. ✅ Encapsulation - hide implementation details
2. ✅ Can add validation
3. ✅ Can add logging
4. ✅ Can trigger side effects (SetModifiedFlag, UpdateAllViews)
5. ✅ Can change internal representation without breaking code

---

### 3.3 Complex Data Structures

```cpp
class CMyDocument : public CDocument
{
protected:
    // Array of objects
    CArray<CDrawObject*, CDrawObject*> m_objects;

public:
    // Add object
    void AddObject(CDrawObject* pObj)
    {
        ASSERT(pObj);
        m_objects.Add(pObj);
        SetModifiedFlag();
        UpdateAllViews(NULL, HINT_OBJECT_ADDED, pObj);
    }

    // Remove object
    void RemoveObject(int index)
    {
        if (index >= 0 && index < m_objects.GetSize())
        {
            CDrawObject* pObj = m_objects[index];
            m_objects.RemoveAt(index);
            delete pObj;  // Important: delete the object!
            SetModifiedFlag();
            UpdateAllViews(NULL, HINT_OBJECT_REMOVED);
        }
    }

    // Get object
    CDrawObject* GetObject(int index) const
    {
        if (index >= 0 && index < m_objects.GetSize())
            return m_objects[index];
        return NULL;
    }

    // Get count
    int GetObjectCount() const
    {
        return (int)m_objects.GetSize();
    }

    // Clear all
    void ClearAll()
    {
        for (int i = 0; i < m_objects.GetSize(); i++)
        {
            delete m_objects[i];
        }
        m_objects.RemoveAll();
        SetModifiedFlag();
        UpdateAllViews(NULL);
    }

protected:
    virtual void DeleteContents()
    {
        // Clean up all objects
        for (int i = 0; i < m_objects.GetSize(); i++)
        {
            delete m_objects[i];
        }
        m_objects.RemoveAll();

        CDocument::DeleteContents();
    }

    virtual void Serialize(CArchive& ar)
    {
        if (ar.IsStoring())
        {
            ar << (int)m_objects.GetSize();
            for (int i = 0; i < m_objects.GetSize(); i++)
            {
                m_objects[i]->Serialize(ar);
            }
        }
        else
        {
            int count;
            ar >> count;

            ClearAll();  // Clear existing

            for (int i = 0; i < count; i++)
            {
                CDrawObject* pObj = new CDrawObject;
                pObj->Serialize(ar);
                m_objects.Add(pObj);
            }
        }
    }
};
```

---

## 4. The Modified Flag System

### 4.1 Understanding the Modified Flag

The modified flag tells MFC whether the document has unsaved changes.

```cpp
class CDocument
{
protected:
    BOOL m_bModified;  // Internal modified flag

public:
    // Set modified flag
    void SetModifiedFlag(BOOL bModified = TRUE)
    {
        m_bModified = bModified;
    }

    // Check if modified
    BOOL IsModified()
    {
        return m_bModified;
    }

    // Prompt to save if modified
    BOOL SaveModified();
};
```

---

### 4.2 When to Call SetModifiedFlag()

```cpp
// ✅ Call SetModifiedFlag() when data changes
void CMyDocument::SetText(const CString& text)
{
    if (m_strText != text)  // Optimization: check if actually changed
    {
        m_strText = text;
        SetModifiedFlag(TRUE);   // ← Mark as modified
        UpdateAllViews(NULL);
    }
}

void CMyDocument::AddPoint(CPoint pt)
{
    m_points.Add(pt);
    SetModifiedFlag(TRUE);      // ← Mark as modified
    UpdateAllViews(NULL);
}

void CMyDocument::DeletePoint(int index)
{
    if (index >= 0 && index < m_points.GetSize())
    {
        m_points.RemoveAt(index);
        SetModifiedFlag(TRUE);  // ← Mark as modified
        UpdateAllViews(NULL);
    }
}
```

---

### 4.3 The SaveModified() Mechanism

```cpp
// Called automatically by framework before:
// - File → New
// - File → Open
// - File → Close
// - Application Exit

BOOL CDocument::SaveModified()
{
    if (!IsModified())
        return TRUE;  // Not modified - OK to proceed

    // Show message box
    CString prompt;
    prompt.Format(_T("Save changes to %s?"), GetTitle());

    int result = AfxMessageBox(prompt,
                              MB_YESNOCANCEL | MB_ICONQUESTION);

    switch (result)
    {
    case IDYES:
        // Save the document
        if (!DoFileSave())
            return FALSE;  // Save failed - abort operation
        return TRUE;       // Saved - OK to proceed

    case IDNO:
        // Don't save - discard changes
        SetModifiedFlag(FALSE);
        return TRUE;       // OK to proceed

    case IDCANCEL:
        // Cancel the operation
        return FALSE;      // Abort operation
    }

    return TRUE;
}
```

**Example Flow**:
```
User clicks File → New
  ↓
Is document modified?
  YES → Show "Save changes?" dialog
          User clicks "Yes" → Save → Proceed with New
          User clicks "No"  → Discard → Proceed with New
          User clicks "Cancel" → Abort (stay with current doc)
  NO → Proceed with New immediately
```

---

### 4.4 Custom Save Prompts

```cpp
class CMyDocument : public CDocument
{
protected:
    int m_nChangeCount;

public:
    virtual BOOL SaveModified()
    {
        if (!IsModified())
            return TRUE;

        // Custom message
        CString msg;
        msg.Format(
            _T("You have made %d changes to this document.\n")
            _T("Do you want to save before closing?"),
            m_nChangeCount);

        int result = AfxMessageBox(msg, MB_YESNOCANCEL | MB_ICONWARNING);

        // Same handling as base class
        switch (result)
        {
        case IDYES:
            return DoFileSave();
        case IDNO:
            SetModifiedFlag(FALSE);
            return TRUE;
        case IDCANCEL:
            return FALSE;
        }

        return TRUE;
    }
};
```

---

## 5. Document Views Relationship

### 5.1 One Document, Multiple Views

```cpp
┌──────────────────────────┐
│       CDocument          │
│    (One Instance)        │
└──────────────────────────┘
            │
            ├─────────┬─────────┬─────────┐
            ↓         ↓         ↓         ↓
        ┌──────┐  ┌──────┐  ┌──────┐  ┌──────┐
        │View 1│  │View 2│  │View 3│  │View 4│
        └──────┘  └──────┘  └──────┘  └──────┘
         Text     Graph     Table     Summary
```

**Document maintains a list of all its views**:
```cpp
class CDocument
{
protected:
    CPtrList m_viewList;  // List of CView* pointers

public:
    POSITION GetFirstViewPosition() const;
    CView* GetNextView(POSITION& rPosition) const;

    void UpdateAllViews(CView* pSender,
                       LPARAM lHint = 0L,
                       CObject* pHint = NULL);
};
```

---

### 5.2 Iterating Through Views

```cpp
// Get all views
void CMyDocument::NotifyAllViews()
{
    POSITION pos = GetFirstViewPosition();
    while (pos)
    {
        CView* pView = GetNextView(pos);

        // Type-safe cast
        if (pView->IsKindOf(RUNTIME_CLASS(CMySpecialView)))
        {
            CMySpecialView* pSpecial = (CMySpecialView*)pView;
            pSpecial->SpecialUpdate();
        }
    }
}

// Count views
int CMyDocument::GetViewCount()
{
    int count = 0;
    POSITION pos = GetFirstViewPosition();
    while (pos)
    {
        GetNextView(pos);
        count++;
    }
    return count;
}

// Find specific view type
CMyTextView* CMyDocument::GetTextView()
{
    POSITION pos = GetFirstViewPosition();
    while (pos)
    {
        CView* pView = GetNextView(pos);
        if (pView->IsKindOf(RUNTIME_CLASS(CMyTextView)))
        {
            return (CMyTextView*)pView;
        }
    }
    return NULL;
}
```

---

### 5.3 UpdateAllViews() - Deep Understanding

```cpp
void CDocument::UpdateAllViews(CView* pSender,
                              LPARAM lHint,
                              CObject* pHint)
{
    POSITION pos = GetFirstViewPosition();
    while (pos)
    {
        CView* pView = GetNextView(pos);
        if (pView != pSender)  // Don't update sender
        {
            pView->OnUpdate(pSender, lHint, pHint);
        }
    }
}
```

**Parameters**:
- `pSender`: View that caused the update (won't be updated)
- `lHint`: Hint about what changed (0 = full update)
- `pHint`: Pointer to additional information

**Example Usage**:
```cpp
// Define hints
#define HINT_TEXT_CHANGED       1
#define HINT_POINT_ADDED        2
#define HINT_POINT_REMOVED      3
#define HINT_COLOR_CHANGED      4

// In document
void CMyDocument::AddPoint(CPoint pt)
{
    m_points.Add(pt);
    SetModifiedFlag();

    // Pass hint and point
    UpdateAllViews(NULL, HINT_POINT_ADDED, (CObject*)&pt);
}

// In view
void CMyView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    if (lHint == HINT_POINT_ADDED)
    {
        // Only redraw new point
        CPoint* pPt = (CPoint*)pHint;
        CRect rect(pPt->x-10, pPt->y-10, pPt->x+10, pPt->y+10);
        InvalidateRect(rect);
    }
    else if (lHint == HINT_TEXT_CHANGED)
    {
        // Full redraw
        Invalidate();
    }
    else
    {
        // Unknown hint - full redraw
        Invalidate();
    }
}
```

---

## 6. Serialization Architecture

### 6.1 The CArchive Class

```cpp
class CArchive
{
public:
    // Check mode
    BOOL IsLoading() const;
    BOOL IsStoring() const;

    // Read/Write primitives
    CArchive& operator>>(BYTE& b);
    CArchive& operator>>(WORD& w);
    CArchive& operator>>(LONG& l);
    CArchive& operator>>(DWORD& dw);
    CArchive& operator>>(float& f);
    CArchive& operator>>(double& d);
    CArchive& operator>>(CString& str);
    // ... and many more

    CArchive& operator<<(BYTE b);
    CArchive& operator<<(WORD w);
    CArchive& operator<<(LONG l);
    // ... and many more

    // Read/Write objects
    CArchive& operator>>(CObject*& pOb);
    CArchive& operator<<(CObject* pOb);

    // String methods
    BOOL ReadString(CString& str);
    void WriteString(LPCTSTR lpsz);

    // Raw data
    UINT Read(void* lpBuf, UINT nMax);
    void Write(const void* lpBuf, UINT nMax);
};
```

---

### 6.2 Basic Serialization Pattern

```cpp
void CMyDocument::Serialize(CArchive& ar)
{
    // *** VERSION NUMBER (important!) ***
    const DWORD FILE_VERSION = 2;

    if (ar.IsStoring())
    {
        // *** SAVING ***
        ar << FILE_VERSION;         // Write version

        // Simple types
        ar << m_strText;
        ar << m_nCounter;
        ar << m_fValue;
        ar << m_color;              // COLORREF

        // CObject-derived (with IMPLEMENT_SERIAL)
        ar << &m_myObject;

        // Arrays
        m_points.Serialize(ar);

        // Collections
        ar << (int)m_list.GetCount();
        POSITION pos = m_list.GetHeadPosition();
        while (pos)
        {
            CMyItem* pItem = m_list.GetNext(pos);
            pItem->Serialize(ar);
        }
    }
    else
    {
        // *** LOADING ***
        DWORD version;
        ar >> version;              // Read version

        if (version > FILE_VERSION)
        {
            AfxThrowArchiveException(
                CArchiveException::badSchema);
        }

        // Simple types
        ar >> m_strText;
        ar >> m_nCounter;
        ar >> m_fValue;
        ar >> m_color;

        // CObject-derived
        ar >> &m_myObject;

        // Arrays
        m_points.Serialize(ar);

        // Collections
        int count;
        ar >> count;
        m_list.RemoveAll();
        for (int i = 0; i < count; i++)
        {
            CMyItem* pItem = new CMyItem;
            pItem->Serialize(ar);
            m_list.AddTail(pItem);
        }

        // Handle version differences
        if (version >= 2)
        {
            // New data in version 2
            ar >> m_newField;
        }
        else
        {
            // Set default for old files
            m_newField = DEFAULT_VALUE;
        }
    }
}
```

---

## 7. Document Templates and Registration

### 7.1 Single Document Template

```cpp
// In App::InitInstance()
CSingleDocTemplate* pDocTemplate;
pDocTemplate = new CSingleDocTemplate(
    IDR_MAINFRAME,                    // Resource ID
    RUNTIME_CLASS(CMyDoc),            // Document class
    RUNTIME_CLASS(CMainFrame),        // Frame window class
    RUNTIME_CLASS(CMyView));          // View class

AddDocTemplate(pDocTemplate);
```

**What This Does**:
1. Registers the document type
2. Associates resources (menu, icon, strings)
3. Tells MFC which classes to instantiate
4. Enables File→Open/Save for this type

---

### 7.2 Document Template Resources

**Resource String Format** (in string table):
```
IDR_MAINFRAME =
  "\n"                    // 1. Window title prefix
  "MyApp\n"               // 2. Root for document name
  "MyApp Document\n"      // 3. Document type name
  "MyApp Files (*.mya)\n" // 4. File filter
  ".mya\n"                // 5. File extension
  "MyApp.Document\n"      // 6. Registry file type ID
  "MyApp Document"        // 7. Registry file type description
```

**Example**:
```cpp
// In resource.rc
STRINGTABLE
BEGIN
    IDR_MAINFRAME "\nMyText\nMyText Document\nMyText Files (*.txt)\n.txt\nMyText.Document\nMyText Document"
END
```

---

### 7.3 Multiple Document Templates (SDI with Multiple Types)

```cpp
BOOL CMyApp::InitInstance()
{
    // ... initialization ...

    // Template 1: Text documents
    CSingleDocTemplate* pTextTemplate;
    pTextTemplate = new CSingleDocTemplate(
        IDR_TEXT_DOC,
        RUNTIME_CLASS(CTextDoc),
        RUNTIME_CLASS(CMainFrame),
        RUNTIME_CLASS(CTextView));
    AddDocTemplate(pTextTemplate);

    // Template 2: Image documents
    CSingleDocTemplate* pImageTemplate;
    pImageTemplate = new CSingleDocTemplate(
        IDR_IMAGE_DOC,
        RUNTIME_CLASS(CImageDoc),
        RUNTIME_CLASS(CMainFrame),
        RUNTIME_CLASS(CImageView));
    AddDocTemplate(pImageTemplate);

    // When user does File→New, they'll see a choice dialog
    // When user does File→Open, filter shows both types

    // ... rest of initialization ...
}
```

**File→New Dialog**:
```
┌────────────────────────────┐
│ New                        │
├────────────────────────────┤
│ Select document type:      │
│                            │
│ ○ Text Document            │
│ ○ Image Document           │
│                            │
│   [  OK  ] [ Cancel ]      │
└────────────────────────────┘
```

---

## 8. Multi-Document Type Support

### 8.1 Detecting Document Type

```cpp
class CMyDocument : public CDocument
{
protected:
    enum DocType
    {
        DOC_TYPE_TEXT,
        DOC_TYPE_BINARY,
        DOC_TYPE_XML
    };

    DocType m_docType;

public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName)
    {
        // Detect type from extension
        CString path = lpszPathName;
        path.MakeLower();

        if (path.Right(4) == _T(".txt"))
            m_docType = DOC_TYPE_TEXT;
        else if (path.Right(4) == _T(".bin"))
            m_docType = DOC_TYPE_BINARY;
        else if (path.Right(4) == _T(".xml"))
            m_docType = DOC_TYPE_XML;
        else
        {
            AfxMessageBox(_T("Unknown file type"));
            return FALSE;
        }

        // Load based on type
        return LoadDocument(lpszPathName);
    }

    BOOL LoadDocument(LPCTSTR lpszPathName)
    {
        switch (m_docType)
        {
        case DOC_TYPE_TEXT:
            return LoadTextFormat(lpszPathName);
        case DOC_TYPE_BINARY:
            return LoadBinaryFormat(lpszPathName);
        case DOC_TYPE_XML:
            return LoadXMLFormat(lpszPathName);
        }
        return FALSE;
    }
};
```

---

## 9. Best Practices for Document Design

### 9.1 Always Use Version Numbers

```cpp
void CMyDocument::Serialize(CArchive& ar)
{
    const DWORD CURRENT_VERSION = 3;

    if (ar.IsStoring())
    {
        ar << CURRENT_VERSION;
        // ... save data ...
    }
    else
    {
        DWORD version;
        ar >> version;

        if (version > CURRENT_VERSION)
        {
            AfxMessageBox(_T("This file was created with a newer version"));
            AfxThrowArchiveException(CArchiveException::badSchema);
        }

        // Load based on version
        if (version >= 1)
        {
            ar >> m_data1;
        }
        if (version >= 2)
        {
            ar >> m_data2;
        }
        if (version >= 3)
        {
            ar >> m_data3;
        }
    }
}
```

---

### 9.2 Validate Data

```cpp
class CMyDocument : public CDocument
{
protected:
    BOOL ValidateData() const
    {
        // Check for invalid values
        if (m_nValue < 0 || m_nValue > 100)
            return FALSE;

        // Check for required fields
        if (m_strName.IsEmpty())
            return FALSE;

        // Check for consistency
        if (m_points.GetSize() > 0 && m_colors.GetSize() != m_points.GetSize())
            return FALSE;

        return TRUE;
    }

public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName)
    {
        if (!CDocument::OnOpenDocument(lpszPathName))
            return FALSE;

        if (!ValidateData())
        {
            AfxMessageBox(_T("Invalid data in file"));
            OnNewDocument();
            return FALSE;
        }

        return TRUE;
    }
};
```

---

### 9.3 Handle Errors Gracefully

```cpp
BOOL CMyDocument::OnOpenDocument(LPCTSTR lpszPathName)
{
    TRY
    {
        if (!CDocument::OnOpenDocument(lpszPathName))
            return FALSE;

        // Validate
        if (!ValidateData())
        {
            AfxThrowUserException();
        }

        return TRUE;
    }
    CATCH(CFileException, e)
    {
        TCHAR szError[256];
        e->GetErrorMessage(szError, 256);
        AfxMessageBox(szError);
        return FALSE;
    }
    CATCH(CArchiveException, e)
    {
        AfxMessageBox(_T("File format error"));
        return FALSE;
    }
    CATCH(CUserException, e)
    {
        AfxMessageBox(_T("Invalid data in file"));
        return FALSE;
    }
    END_CATCH

    return FALSE;
}
```

---

## 10. Complete Real-World Examples

### Example: Drawing Application Document

```cpp
// DrawDoc.h
class CDrawDoc : public CDocument
{
protected:
    CObArray m_objects;        // Array of drawing objects
    COLORREF m_bgColor;        // Background color
    int m_nVersion;           // File version

public:
    CDrawDoc();
    virtual ~CDrawDoc();

    // Object management
    void AddObject(CDrawObject* pObj);
    void RemoveObject(int index);
    void RemoveAllObjects();
    int GetObjectCount() const;
    CDrawObject* GetObject(int index) const;

    // Background color
    void SetBackgroundColor(COLORREF color);
    COLORREF GetBackgroundColor() const { return m_bgColor; }

protected:
    virtual BOOL OnNewDocument();
    virtual void DeleteContents();
    virtual void Serialize(CArchive& ar);

    DECLARE_DYNCREATE(CDrawDoc)
};

// DrawDoc.cpp
IMPLEMENT_DYNCREATE(CDrawDoc, CDocument)

CDrawDoc::CDrawDoc()
    : m_bgColor(RGB(255, 255, 255))
    , m_nVersion(0)
{
}

CDrawDoc::~CDrawDoc()
{
    RemoveAllObjects();
}

BOOL CDrawDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    m_bgColor = RGB(255, 255, 255);
    m_nVersion = 1;

    return TRUE;
}

void CDrawDoc::DeleteContents()
{
    RemoveAllObjects();
    CDocument::DeleteContents();
}

void CDrawDoc::RemoveAllObjects()
{
    for (int i = 0; i < m_objects.GetSize(); i++)
    {
        CDrawObject* pObj = (CDrawObject*)m_objects[i];
        delete pObj;
    }
    m_objects.RemoveAll();
}

void CDrawDoc::Serialize(CArchive& ar)
{
    const int FILE_VERSION = 1;

    if (ar.IsStoring())
    {
        ar << FILE_VERSION;
        ar << m_bgColor;
        ar << (int)m_objects.GetSize();

        for (int i = 0; i < m_objects.GetSize(); i++)
        {
            CDrawObject* pObj = (CDrawObject*)m_objects[i];
            ar << pObj->GetType();  // Object type
            pObj->Serialize(ar);
        }
    }
    else
    {
        int version;
        ar >> version;

        if (version != FILE_VERSION)
        {
            AfxMessageBox(_T("Unsupported file version"));
            AfxThrowArchiveException(CArchiveException::badSchema);
        }

        ar >> m_bgColor;

        int count;
        ar >> count;

        RemoveAllObjects();

        for (int i = 0; i < count; i++)
        {
            int type;
            ar >> type;

            CDrawObject* pObj = CreateObjectByType(type);
            if (pObj)
            {
                pObj->Serialize(ar);
                m_objects.Add(pObj);
            }
        }
    }
}

void CDrawDoc::AddObject(CDrawObject* pObj)
{
    ASSERT(pObj);
    m_objects.Add(pObj);
    SetModifiedFlag();
    UpdateAllViews(NULL, HINT_OBJECT_ADDED, pObj);
}

void CDrawDoc::RemoveObject(int index)
{
    if (index >= 0 && index < m_objects.GetSize())
    {
        CDrawObject* pObj = (CDrawObject*)m_objects[index];
        m_objects.RemoveAt(index);
        delete pObj;
        SetModifiedFlag();
        UpdateAllViews(NULL, HINT_OBJECT_REMOVED);
    }
}
```

---

## 11. Exercises

### Exercise 1: Undo/Redo System

Implement undo/redo in document:
```cpp
class CMyDocument : public CDocument
{
    CArray<CCommand*> m_undoStack;
    CArray<CCommand*> m_redoStack;

public:
    void Undo();
    void Redo();
    BOOL CanUndo();
    BOOL CanRedo();
};
```

---

### Exercise 2: Auto-Save

Implement auto-save every 5 minutes:
- Use a timer
- Save to temporary file
- Restore on crash

---

### Exercise 3: Document Properties

Add document properties:
- Author
- Creation date
- Modified date
- Comments
- Show in dialog

---

### Exercise 4: Multi-Format Support

Support multiple file formats:
- .txt (text)
- .xml (XML)
- .bin (binary)
- Auto-detect on load

---

### Exercise 5: Large File Optimization

Optimize for large files:
- Lazy loading
- Progressive loading with progress bar
- Memory management

---

## Key Takeaways

1. ✅ CDocument is the MODEL in MVC
2. ✅ ALL data belongs in CDocument
3. ✅ Use accessor methods, not public data
4. ✅ Always call SetModifiedFlag() when data changes
5. ✅ Always call UpdateAllViews() after changes
6. ✅ Override OnNewDocument() to initialize
7. ✅ Override Serialize() to save/load
8. ✅ Override DeleteContents() to clean up
9. ✅ Use version numbers in file format
10. ✅ Validate data after loading

---

## Summary

The CDocument class is the heart of MFC's Document/View architecture. Master it, and you master MFC data management!

---

## Next Lesson Preview

In **Lesson 19**, we'll explore:
- The CView class in detail
- Different view types
- Drawing and rendering
- Input handling
- View preferences vs document data

**Congratulations on completing Lesson 18!**
