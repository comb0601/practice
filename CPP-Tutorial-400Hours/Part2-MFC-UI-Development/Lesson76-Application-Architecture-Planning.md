# Lesson 76: Application Architecture Planning

## Table of Contents
1. [Introduction to MFC Application Architecture](#introduction)
2. [Understanding Design Patterns in MFC](#design-patterns)
3. [Layered Architecture Approach](#layered-architecture)
4. [Document/View Architecture Deep Dive](#document-view)
5. [Planning Your Application Structure](#planning)
6. [Data Management Strategies](#data-management)
7. [Component Organization](#component-organization)
8. [Communication Between Components](#component-communication)
9. [Resource Management Planning](#resource-management)
10. [Threading Architecture](#threading-architecture)
11. [Error Handling Strategy](#error-handling)
12. [Testing Strategy](#testing-strategy)
13. [Architecture Documentation](#documentation)
14. [Real-World Architecture Examples](#examples)
15. [Common Architecture Mistakes](#mistakes)

## 1. Introduction to MFC Application Architecture {#introduction}

Application architecture is the foundation upon which successful software is built. In MFC (Microsoft Foundation Classes), proper architecture planning is crucial because MFC's framework is opinionated and has specific patterns that work best with its design philosophy. This lesson will guide you through comprehensive architecture planning for MFC applications.

### Why Architecture Matters

Good architecture provides:
- **Maintainability**: Easy to understand and modify code
- **Scalability**: Ability to add features without major rewrites
- **Testability**: Components can be tested independently
- **Reusability**: Code can be reused across projects
- **Performance**: Efficient resource utilization
- **Team Collaboration**: Clear boundaries for team members

### The Cost of Poor Architecture

Poor architecture leads to:
- Spaghetti code that's difficult to maintain
- Tight coupling between components
- Difficulty adding new features
- Performance bottlenecks
- High bug density
- Extended development time
- Increased maintenance costs

### MFC's Architectural Philosophy

MFC is built around several core concepts:
- **Object-Oriented Design**: Everything is a class
- **Message Mapping**: Event handling through message maps
- **Document/View Architecture**: Separation of data and presentation
- **Application Framework**: CWinApp manages application lifecycle
- **Window Classes**: UI elements derive from CWnd
- **Command Routing**: Messages flow through a defined path

## 2. Understanding Design Patterns in MFC {#design-patterns}

MFC implements several classic design patterns that you should understand and leverage in your architecture.

### Model-View-Controller (MVC) Pattern

MFC's Document/View architecture is a variation of MVC:

```cpp
// Model (Document)
class CMyDocument : public CDocument
{
protected:
    CMyDocument();
    DECLARE_DYNCREATE(CMyDocument)

private:
    CString m_strData;           // Business data
    CArray<CDataItem*> m_items;  // Complex data structures

public:
    // Data access methods
    const CString& GetData() const { return m_strData; }
    void SetData(const CString& data);

    // Business logic
    BOOL ValidateData();
    void ProcessData();

    // Serialization
    virtual void Serialize(CArchive& ar);

protected:
    virtual BOOL OnNewDocument();
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};

// View (Presentation)
class CMyView : public CView
{
protected:
    CMyView();
    DECLARE_DYNCREATE(CMyView)

public:
    CMyDocument* GetDocument() const;

    // Rendering
    virtual void OnDraw(CDC* pDC);
    virtual void OnInitialUpdate();

    // User interaction
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

    DECLARE_MESSAGE_MAP()
};

// Controller (Command handlers in Document, View, or Frame)
void CMyView::OnEditCopy()
{
    CMyDocument* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    // Get data from model
    CString data = pDoc->GetData();

    // Copy to clipboard
    if (OpenClipboard())
    {
        EmptyClipboard();
        // ... clipboard operations
        CloseClipboard();
    }
}
```

### Singleton Pattern

The application object is a singleton:

```cpp
class CMyApp : public CWinApp
{
private:
    CConfigManager m_configManager;  // Singleton within app
    CLogger m_logger;                // Logging singleton

public:
    CMyApp();

    // Access to singletons
    static CConfigManager& GetConfig() {
        return ((CMyApp*)AfxGetApp())->m_configManager;
    }

    static CLogger& GetLogger() {
        return ((CMyApp*)AfxGetApp())->m_logger;
    }

    virtual BOOL InitInstance();
    virtual int ExitInstance();
};

// Usage anywhere in the application
void SomeFunction()
{
    CMyApp::GetLogger().Log("Operation started");
    CString value = CMyApp::GetConfig().GetValue("Setting");
}
```

### Observer Pattern

Document/View implements observer:

```cpp
// Document notifies all views when data changes
void CMyDocument::SetData(const CString& data)
{
    m_strData = data;
    SetModifiedFlag();
    UpdateAllViews(NULL);  // Notify all observers (views)
}

// Views respond to notifications
void CMyView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    // React to document changes
    switch (lHint)
    {
    case HINT_DATA_CHANGED:
        Invalidate();  // Redraw view
        break;

    case HINT_SELECTION_CHANGED:
        UpdateSelection();
        break;
    }
}
```

### Factory Pattern

Dynamic object creation in MFC:

```cpp
// Abstract factory for creating different view types
class CViewFactory
{
public:
    enum ViewType { VIEW_LIST, VIEW_TREE, VIEW_GRID };

    static CView* CreateView(ViewType type, CRuntimeClass* pViewClass)
    {
        CView* pView = NULL;

        switch (type)
        {
        case VIEW_LIST:
            pView = RUNTIME_CLASS(CMyListView)->CreateObject();
            break;

        case VIEW_TREE:
            pView = RUNTIME_CLASS(CMyTreeView)->CreateObject();
            break;

        case VIEW_GRID:
            pView = RUNTIME_CLASS(CMyGridView)->CreateObject();
            break;
        }

        return pView;
    }
};
```

### Command Pattern

MFC's command routing implements command pattern:

```cpp
// Command objects encapsulate operations
class CCommand
{
public:
    virtual ~CCommand() {}
    virtual void Execute() = 0;
    virtual void Undo() = 0;
    virtual CString GetDescription() const = 0;
};

class CInsertTextCommand : public CCommand
{
private:
    CMyDocument* m_pDoc;
    CString m_text;
    int m_position;

public:
    CInsertTextCommand(CMyDocument* pDoc, const CString& text, int pos)
        : m_pDoc(pDoc), m_text(text), m_position(pos) {}

    virtual void Execute()
    {
        m_pDoc->InsertText(m_position, m_text);
    }

    virtual void Undo()
    {
        m_pDoc->DeleteText(m_position, m_text.GetLength());
    }

    virtual CString GetDescription() const
    {
        return _T("Insert Text");
    }
};

// Command manager for undo/redo
class CCommandManager
{
private:
    CList<CCommand*> m_undoStack;
    CList<CCommand*> m_redoStack;

public:
    void ExecuteCommand(CCommand* pCommand)
    {
        pCommand->Execute();
        m_undoStack.AddTail(pCommand);

        // Clear redo stack
        POSITION pos = m_redoStack.GetHeadPosition();
        while (pos)
        {
            delete m_redoStack.GetNext(pos);
        }
        m_redoStack.RemoveAll();
    }

    void Undo()
    {
        if (!m_undoStack.IsEmpty())
        {
            CCommand* pCommand = m_undoStack.RemoveTail();
            pCommand->Undo();
            m_redoStack.AddTail(pCommand);
        }
    }

    void Redo()
    {
        if (!m_redoStack.IsEmpty())
        {
            CCommand* pCommand = m_redoStack.RemoveTail();
            pCommand->Execute();
            m_undoStack.AddTail(pCommand);
        }
    }
};
```

## 3. Layered Architecture Approach {#layered-architecture}

A well-structured MFC application should be organized in layers, each with specific responsibilities.

### Three-Tier Architecture

```cpp
// ============================================
// PRESENTATION LAYER (UI)
// ============================================
class CMyView : public CView
{
    // Only UI logic, no business logic
public:
    virtual void OnDraw(CDC* pDC);

    afx_msg void OnButtonClick()
    {
        // Get data from UI
        CString input;
        m_editControl.GetWindowText(input);

        // Call business layer
        CMyDocument* pDoc = GetDocument();
        if (pDoc->ProcessInput(input))
        {
            MessageBox(_T("Success!"));
        }
    }
};

// ============================================
// BUSINESS LOGIC LAYER
// ============================================
class CMyDocument : public CDocument
{
private:
    CDataAccessLayer m_dataLayer;  // Access to data layer

public:
    BOOL ProcessInput(const CString& input)
    {
        // Validate
        if (!ValidateInput(input))
            return FALSE;

        // Business logic
        CBusinessData data = TransformInput(input);

        // Save through data layer
        return m_dataLayer.SaveData(data);
    }

private:
    BOOL ValidateInput(const CString& input);
    CBusinessData TransformInput(const CString& input);
};

// ============================================
// DATA ACCESS LAYER
// ============================================
class CDataAccessLayer
{
private:
    CDatabase m_database;
    CString m_connectionString;

public:
    BOOL Connect()
    {
        return m_database.OpenEx(m_connectionString, CDatabase::noOdbcDialog);
    }

    BOOL SaveData(const CBusinessData& data)
    {
        CRecordset rs(&m_database);
        CString sql;
        sql.Format(_T("INSERT INTO Data VALUES ('%s', '%s')"),
            data.GetField1(), data.GetField2());

        TRY
        {
            m_database.ExecuteSQL(sql);
            return TRUE;
        }
        CATCH(CDBException, e)
        {
            AfxMessageBox(e->m_strError);
            return FALSE;
        }
        END_CATCH
    }

    BOOL LoadData(CBusinessData& data, int id)
    {
        // SQL query to load data
        CRecordset rs(&m_database);
        CString sql;
        sql.Format(_T("SELECT * FROM Data WHERE ID = %d"), id);

        TRY
        {
            rs.Open(CRecordset::snapshot, sql);
            if (!rs.IsEOF())
            {
                // Populate data object
                rs.GetFieldValue(_T("Field1"), data.m_field1);
                rs.GetFieldValue(_T("Field2"), data.m_field2);
                return TRUE;
            }
        }
        CATCH(CDBException, e)
        {
            AfxMessageBox(e->m_strError);
        }
        END_CATCH

        return FALSE;
    }
};
```

### Separating Concerns

Each layer should have a clear responsibility:

```cpp
// BAD: Mixing concerns
class CBadView : public CView
{
    afx_msg void OnSaveButton()
    {
        // UI code
        CString data;
        m_edit.GetWindowText(data);

        // Database code (WRONG - in UI layer!)
        CDatabase db;
        db.OpenEx(_T("DSN=MyDB"));
        CString sql;
        sql.Format(_T("INSERT INTO Table VALUES ('%s')"), data);
        db.ExecuteSQL(sql);

        // Business logic (WRONG - in UI layer!)
        double result = CalculateComplexBusinessLogic(data);
    }
};

// GOOD: Separation of concerns
class CGoodView : public CView
{
    afx_msg void OnSaveButton()
    {
        // Only UI logic
        CString data;
        m_edit.GetWindowText(data);

        // Delegate to business layer
        CMyDocument* pDoc = GetDocument();
        if (pDoc->SaveData(data))
        {
            MessageBox(_T("Saved successfully!"));
        }
        else
        {
            MessageBox(_T("Save failed!"));
        }
    }
};
```

## 4. Document/View Architecture Deep Dive {#document-view}

Understanding Document/View is essential for MFC architecture.

### When to Use Document/View

**Use Document/View when:**
- Your application works with documents (files)
- You need multiple views of the same data
- You want automatic serialization support
- You need print/print preview support
- You want standard File menu handling

**Don't use Document/View when:**
- Building a dialog-based application
- No document concept (e.g., calculator)
- Simple utility applications
- Real-time data streaming applications

### Advanced Document/View Patterns

```cpp
// Multiple document templates
BOOL CMyApp::InitInstance()
{
    // Template for text documents
    CMultiDocTemplate* pTextTemplate = new CMultiDocTemplate(
        IDR_TEXTTYPE,
        RUNTIME_CLASS(CTextDocument),
        RUNTIME_CLASS(CChildFrame),
        RUNTIME_CLASS(CTextView));
    AddDocTemplate(pTextTemplate);

    // Template for image documents
    CMultiDocTemplate* pImageTemplate = new CMultiDocTemplate(
        IDR_IMAGETYPE,
        RUNTIME_CLASS(CImageDocument),
        RUNTIME_CLASS(CChildFrame),
        RUNTIME_CLASS(CImageView));
    AddDocTemplate(pImageTemplate);

    // Template with multiple views
    CMultiDocTemplate* pMultiViewTemplate = new CMultiDocTemplate(
        IDR_MULTIVIEW,
        RUNTIME_CLASS(CDataDocument),
        RUNTIME_CLASS(CChildFrame),
        RUNTIME_CLASS(CListView));  // Default view
    AddDocTemplate(pMultiViewTemplate);

    return TRUE;
}

// Switching between views dynamically
void CSwitchViewFrame::OnViewSwitch()
{
    CView* pOldView = GetActiveView();
    CDocument* pDoc = pOldView->GetDocument();

    // Determine new view type
    CRuntimeClass* pNewViewClass;
    if (pOldView->IsKindOf(RUNTIME_CLASS(CListView)))
        pNewViewClass = RUNTIME_CLASS(CTreeView);
    else
        pNewViewClass = RUNTIME_CLASS(CListView);

    // Create new view
    CView* pNewView = (CView*)pNewViewClass->CreateObject();
    CCreateContext context;
    context.m_pCurrentDoc = pDoc;
    context.m_pNewViewClass = pNewViewClass;

    // Create the view window
    CRect rect;
    pOldView->GetClientRect(&rect);

    pNewView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD,
        rect, this, AFX_IDW_PANE_FIRST, &context);

    // Switch views
    SetActiveView(pNewView);
    pOldView->DestroyWindow();

    // Update view
    pNewView->OnInitialUpdate();
    RecalcLayout();
}
```

### Custom Document/View Communication

```cpp
// Custom update hints
#define HINT_DATA_CHANGED     1L
#define HINT_SELECTION_CHANGED 2L
#define HINT_ITEM_ADDED       3L
#define HINT_ITEM_DELETED     4L

// Document sends targeted updates
void CMyDocument::AddItem(CDataItem* pItem)
{
    m_items.Add(pItem);
    UpdateAllViews(NULL, HINT_ITEM_ADDED, (CObject*)pItem);
}

void CMyDocument::DeleteItem(int index)
{
    CDataItem* pItem = m_items[index];
    m_items.RemoveAt(index);
    UpdateAllViews(NULL, HINT_ITEM_DELETED, (CObject*)pItem);
}

// Views respond to specific updates
void CMyListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    switch (lHint)
    {
    case HINT_ITEM_ADDED:
        {
            CDataItem* pItem = (CDataItem*)pHint;
            AddItemToList(pItem);
        }
        break;

    case HINT_ITEM_DELETED:
        {
            CDataItem* pItem = (CDataItem*)pHint;
            RemoveItemFromList(pItem);
        }
        break;

    case HINT_DATA_CHANGED:
    default:
        RefreshAllItems();
        break;
    }
}
```

## 5. Planning Your Application Structure {#planning}

Before writing code, create a comprehensive plan.

### Application Planning Checklist

**1. Requirements Analysis**
- What is the core functionality?
- Who are the users?
- What platforms need to be supported?
- What are the performance requirements?
- What are the data requirements?

**2. Architecture Decisions**
- Document/View or dialog-based?
- SDI (Single Document Interface) or MDI (Multiple Document Interface)?
- What design patterns to use?
- Database requirements?
- Network requirements?

**3. Component Identification**
- What are the major components?
- How do components interact?
- What are the dependencies?
- What can be reused from libraries?

**4. Data Flow Design**
- How does data flow through the application?
- Where is data stored?
- How is data synchronized?
- What are the data validation rules?

**5. User Interface Planning**
- Main window layout?
- Dialog designs?
- Menu structure?
- Toolbar and status bar content?
- Keyboard shortcuts?

### Sample Application Plan

```cpp
/*
APPLICATION: Text Editor with Syntax Highlighting

ARCHITECTURE TYPE: Document/View, MDI

MAJOR COMPONENTS:
1. CTextEditorApp (CWinApp)
   - Application initialization
   - Document template management

2. CMainFrame (CMDIFrameWnd)
   - Main application window
   - Menu bar, toolbar, status bar
   - Window management

3. CTextDocument (CDocument)
   - Text content storage
   - File I/O operations
   - Undo/redo management

4. CTextView (CView)
   - Text rendering
   - Syntax highlighting
   - User input handling

5. CSyntaxHighlighter (Custom class)
   - Language parsing
   - Color scheme management

6. CUndoManager (Custom class)
   - Command pattern implementation
   - Undo/redo stack management

DATA FLOW:
File -> Document::OnOpenDocument() -> Parse content ->
Update views -> View::OnDraw() -> Render with highlighting

USER INTERFACE:
- Standard MDI interface
- Dockable panels for file browser and output
- Customizable toolbar
- Status bar showing line/column, file type

FEATURES:
Phase 1: Basic text editing, file I/O
Phase 2: Syntax highlighting for C++
Phase 3: Find/replace functionality
Phase 4: Multiple language support
Phase 5: Code folding and intellisense
*/
```

## 6. Data Management Strategies {#data-management}

How you manage data significantly impacts your architecture.

### In-Memory Data Management

```cpp
// Efficient data structure for large datasets
class CDataManager
{
private:
    // Use appropriate containers
    CMap<int, int, CDataItem*, CDataItem*> m_dataMap;  // Fast lookup
    CArray<CDataItem*> m_dataArray;                     // Ordered access
    CList<CDataItem*> m_dataList;                       // Frequent insertion/deletion

public:
    // Lazy loading
    CDataItem* GetItem(int id)
    {
        CDataItem* pItem = NULL;
        if (!m_dataMap.Lookup(id, pItem))
        {
            // Load from database
            pItem = LoadFromDatabase(id);
            if (pItem)
            {
                m_dataMap[id] = pItem;
            }
        }
        return pItem;
    }

    // Bulk operations
    void LoadRange(int startId, int endId)
    {
        CRecordset rs(&m_database);
        CString sql;
        sql.Format(_T("SELECT * FROM Data WHERE ID BETWEEN %d AND %d"),
            startId, endId);

        rs.Open(CRecordset::snapshot, sql);
        while (!rs.IsEOF())
        {
            CDataItem* pItem = CreateItemFromRecordset(&rs);
            m_dataMap[pItem->GetID()] = pItem;
            rs.MoveNext();
        }
    }

    // Memory management
    void ClearCache()
    {
        POSITION pos = m_dataMap.GetStartPosition();
        while (pos)
        {
            int key;
            CDataItem* pItem;
            m_dataMap.GetNextAssoc(pos, key, pItem);
            delete pItem;
        }
        m_dataMap.RemoveAll();
    }
};
```

### Database Integration Architecture

```cpp
// Repository pattern for data access
class CDataRepository
{
private:
    CDatabase* m_pDatabase;
    CString m_tableName;

public:
    CDataRepository(CDatabase* pDB, const CString& tableName)
        : m_pDatabase(pDB), m_tableName(tableName) {}

    // CRUD operations
    BOOL Create(const CDataItem& item)
    {
        CString sql;
        sql.Format(_T("INSERT INTO %s (Field1, Field2) VALUES ('%s', %d)"),
            m_tableName, item.GetField1(), item.GetField2());

        TRY
        {
            m_pDatabase->ExecuteSQL(sql);
            return TRUE;
        }
        CATCH(CDBException, e)
        {
            LogError(e->m_strError);
            return FALSE;
        }
        END_CATCH
    }

    CDataItem* Read(int id)
    {
        CRecordset rs(m_pDatabase);
        CString sql;
        sql.Format(_T("SELECT * FROM %s WHERE ID = %d"), m_tableName, id);

        TRY
        {
            rs.Open(CRecordset::snapshot, sql);
            if (!rs.IsEOF())
            {
                CDataItem* pItem = new CDataItem();
                // Populate from recordset
                return pItem;
            }
        }
        CATCH(CDBException, e)
        {
            LogError(e->m_strError);
        }
        END_CATCH

        return NULL;
    }

    BOOL Update(const CDataItem& item)
    {
        CString sql;
        sql.Format(_T("UPDATE %s SET Field1='%s', Field2=%d WHERE ID=%d"),
            m_tableName, item.GetField1(), item.GetField2(), item.GetID());

        TRY
        {
            m_pDatabase->ExecuteSQL(sql);
            return TRUE;
        }
        CATCH(CDBException, e)
        {
            LogError(e->m_strError);
            return FALSE;
        }
        END_CATCH
    }

    BOOL Delete(int id)
    {
        CString sql;
        sql.Format(_T("DELETE FROM %s WHERE ID = %d"), m_tableName, id);

        TRY
        {
            m_pDatabase->ExecuteSQL(sql);
            return TRUE;
        }
        CATCH(CDBException, e)
        {
            LogError(e->m_strError);
            return FALSE;
        }
        END_CATCH
    }

    // Query methods
    CArray<CDataItem*>* FindByField(const CString& field, const CString& value)
    {
        CArray<CDataItem*>* pResults = new CArray<CDataItem*>();
        CRecordset rs(m_pDatabase);
        CString sql;
        sql.Format(_T("SELECT * FROM %s WHERE %s = '%s'"),
            m_tableName, field, value);

        TRY
        {
            rs.Open(CRecordset::snapshot, sql);
            while (!rs.IsEOF())
            {
                CDataItem* pItem = new CDataItem();
                // Populate from recordset
                pResults->Add(pItem);
                rs.MoveNext();
            }
        }
        CATCH(CDBException, e)
        {
            LogError(e->m_strError);
            delete pResults;
            return NULL;
        }
        END_CATCH

        return pResults;
    }
};
```

### File-Based Data Management

```cpp
// Custom binary file format
class CCustomFileManager
{
private:
    struct FileHeader
    {
        DWORD signature;     // File type identifier
        DWORD version;       // File format version
        DWORD itemCount;     // Number of items
        DWORD reserved;      // For future use
    };

public:
    BOOL SaveToFile(const CString& filename, const CArray<CDataItem*>& items)
    {
        CFile file;
        if (!file.Open(filename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
            return FALSE;

        TRY
        {
            // Write header
            FileHeader header;
            header.signature = 0x4D594150;  // 'MYAP'
            header.version = 1;
            header.itemCount = items.GetCount();
            header.reserved = 0;
            file.Write(&header, sizeof(FileHeader));

            // Write items
            for (int i = 0; i < items.GetCount(); i++)
            {
                WriteItem(&file, items[i]);
            }

            file.Close();
            return TRUE;
        }
        CATCH(CFileException, e)
        {
            file.Close();
            return FALSE;
        }
        END_CATCH
    }

    BOOL LoadFromFile(const CString& filename, CArray<CDataItem*>& items)
    {
        CFile file;
        if (!file.Open(filename, CFile::modeRead | CFile::typeBinary))
            return FALSE;

        TRY
        {
            // Read header
            FileHeader header;
            file.Read(&header, sizeof(FileHeader));

            // Validate
            if (header.signature != 0x4D594150)
            {
                file.Close();
                return FALSE;
            }

            // Read items
            for (DWORD i = 0; i < header.itemCount; i++)
            {
                CDataItem* pItem = ReadItem(&file);
                if (pItem)
                    items.Add(pItem);
            }

            file.Close();
            return TRUE;
        }
        CATCH(CFileException, e)
        {
            file.Close();
            return FALSE;
        }
        END_CATCH
    }

private:
    void WriteItem(CFile* pFile, CDataItem* pItem)
    {
        // Write item data
        CArchive ar(pFile, CArchive::store);
        pItem->Serialize(ar);
    }

    CDataItem* ReadItem(CFile* pFile)
    {
        CDataItem* pItem = new CDataItem();
        CArchive ar(pFile, CArchive::load);
        pItem->Serialize(ar);
        return pItem;
    }
};
```

## 7. Component Organization {#component-organization}

Organizing code into logical components improves maintainability.

### File Organization Strategy

```
MyApplication/
├── App/
│   ├── MyApp.h                    // Application class
│   ├── MyApp.cpp
│   └── Resource.h                 // Resource IDs
├── MainFrame/
│   ├── MainFrm.h                  // Main window
│   ├── MainFrm.cpp
│   ├── ChildFrm.h                 // Child windows
│   └── ChildFrm.cpp
├── Documents/
│   ├── MyDocument.h               // Document classes
│   ├── MyDocument.cpp
│   ├── TextDocument.h
│   └── TextDocument.cpp
├── Views/
│   ├── MyView.h                   // View classes
│   ├── MyView.cpp
│   ├── ListView.h
│   └── ListView.cpp
├── Dialogs/
│   ├── PreferencesDlg.h           // Dialog classes
│   ├── PreferencesDlg.cpp
│   ├── FindDlg.h
│   └── FindDlg.cpp
├── Controls/
│   ├── CustomEdit.h               // Custom controls
│   ├── CustomEdit.cpp
│   ├── ColorPicker.h
│   └── ColorPicker.cpp
├── Business/
│   ├── DataManager.h              // Business logic
│   ├── DataManager.cpp
│   ├── ValidationEngine.h
│   └── ValidationEngine.cpp
├── Data/
│   ├── DatabaseManager.h          // Data access
│   ├── DatabaseManager.cpp
│   ├── DataRepository.h
│   └── DataRepository.cpp
├── Utilities/
│   ├── Logger.h                   // Utility classes
│   ├── Logger.cpp
│   ├── ConfigManager.h
│   └── ConfigManager.cpp
└── Resources/
    ├── MyApp.rc                   // Resources
    ├── Toolbar.bmp
    └── Icons/
```

### Component Interface Design

```cpp
// Well-defined interfaces for components
class IDataProvider
{
public:
    virtual ~IDataProvider() {}
    virtual BOOL LoadData(CDataItem& item, int id) = 0;
    virtual BOOL SaveData(const CDataItem& item) = 0;
    virtual CArray<CDataItem*>* GetAllData() = 0;
};

// Database implementation
class CDatabaseProvider : public IDataProvider
{
private:
    CDatabase m_database;

public:
    virtual BOOL LoadData(CDataItem& item, int id);
    virtual BOOL SaveData(const CDataItem& item);
    virtual CArray<CDataItem*>* GetAllData();
};

// File-based implementation
class CFileProvider : public IDataProvider
{
private:
    CString m_filename;

public:
    virtual BOOL LoadData(CDataItem& item, int id);
    virtual BOOL SaveData(const CDataItem& item);
    virtual CArray<CDataItem*>* GetAllData();
};

// Component uses interface, not implementation
class CDataManager
{
private:
    IDataProvider* m_pProvider;

public:
    CDataManager(IDataProvider* pProvider)
        : m_pProvider(pProvider) {}

    BOOL LoadItem(CDataItem& item, int id)
    {
        return m_pProvider->LoadData(item, id);
    }
};

// Easy to switch implementations
void InitializeDataManager()
{
    IDataProvider* pProvider;

    if (UseDatabase())
        pProvider = new CDatabaseProvider();
    else
        pProvider = new CFileProvider();

    CDataManager manager(pProvider);
}
```

## 8. Communication Between Components {#component-communication}

Components need to communicate efficiently without tight coupling.

### Event System

```cpp
// Event system for loose coupling
class IEventListener
{
public:
    virtual ~IEventListener() {}
    virtual void OnEvent(const CString& eventName, void* pData) = 0;
};

class CEventManager
{
private:
    CMap<CString, LPCTSTR, CList<IEventListener*>*, CList<IEventListener*>*> m_listeners;

public:
    void Subscribe(const CString& eventName, IEventListener* pListener)
    {
        CList<IEventListener*>* pList;
        if (!m_listeners.Lookup(eventName, pList))
        {
            pList = new CList<IEventListener*>();
            m_listeners[eventName] = pList;
        }
        pList->AddTail(pListener);
    }

    void Unsubscribe(const CString& eventName, IEventListener* pListener)
    {
        CList<IEventListener*>* pList;
        if (m_listeners.Lookup(eventName, pList))
        {
            POSITION pos = pList->Find(pListener);
            if (pos)
                pList->RemoveAt(pos);
        }
    }

    void Fire(const CString& eventName, void* pData = NULL)
    {
        CList<IEventListener*>* pList;
        if (m_listeners.Lookup(eventName, pList))
        {
            POSITION pos = pList->GetHeadPosition();
            while (pos)
            {
                IEventListener* pListener = pList->GetNext(pos);
                pListener->OnEvent(eventName, pData);
            }
        }
    }
};

// Usage
class CMyView : public CView, public IEventListener
{
public:
    CMyView()
    {
        GetEventManager()->Subscribe(_T("DataChanged"), this);
    }

    ~CMyView()
    {
        GetEventManager()->Unsubscribe(_T("DataChanged"), this);
    }

    virtual void OnEvent(const CString& eventName, void* pData)
    {
        if (eventName == _T("DataChanged"))
        {
            Invalidate();  // Redraw
        }
    }
};

// Another component fires the event
void CDataManager::UpdateData()
{
    // ... update data ...
    GetEventManager()->Fire(_T("DataChanged"));
}
```

### Message-Based Communication

```cpp
// Custom Windows messages
#define WM_DATA_UPDATED    (WM_USER + 100)
#define WM_OPERATION_COMPLETE (WM_USER + 101)

// Sending messages
void CDataManager::NotifyDataUpdated()
{
    CWnd* pMainWnd = AfxGetMainWnd();
    if (pMainWnd)
    {
        pMainWnd->PostMessage(WM_DATA_UPDATED, 0, 0);
    }
}

// Receiving messages
BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
    ON_MESSAGE(WM_DATA_UPDATED, OnDataUpdated)
    ON_MESSAGE(WM_OPERATION_COMPLETE, OnOperationComplete)
END_MESSAGE_MAP()

LRESULT CMainFrame::OnDataUpdated(WPARAM wParam, LPARAM lParam)
{
    // Update UI
    UpdateAllViews(NULL);
    return 0;
}
```

## 9. Resource Management Planning {#resource-management}

Proper resource management prevents leaks and improves performance.

### Resource Management Strategy

```cpp
// RAII (Resource Acquisition Is Initialization) pattern
class CResourceGuard
{
private:
    HGDIOBJ m_hOldObject;
    CDC* m_pDC;

public:
    CResourceGuard(CDC* pDC, HGDIOBJ hNewObject)
        : m_pDC(pDC)
    {
        m_hOldObject = pDC->SelectObject(hNewObject);
    }

    ~CResourceGuard()
    {
        if (m_pDC && m_hOldObject)
            m_pDC->SelectObject(m_hOldObject);
    }
};

// Usage
void CMyView::OnDraw(CDC* pDC)
{
    CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
    CResourceGuard guard(pDC, &pen);  // Auto cleanup

    pDC->MoveTo(0, 0);
    pDC->LineTo(100, 100);

    // pen automatically restored when guard goes out of scope
}

// Smart pointer for MFC objects
template<typename T>
class CAutoPtr
{
private:
    T* m_ptr;

public:
    CAutoPtr(T* ptr = NULL) : m_ptr(ptr) {}

    ~CAutoPtr()
    {
        delete m_ptr;
    }

    T* operator->() { return m_ptr; }
    T& operator*() { return *m_ptr; }
    T* Get() { return m_ptr; }

    T* Detach()
    {
        T* ptr = m_ptr;
        m_ptr = NULL;
        return ptr;
    }

private:
    // Prevent copying
    CAutoPtr(const CAutoPtr&);
    CAutoPtr& operator=(const CAutoPtr&);
};

// Usage
void ProcessData()
{
    CAutoPtr<CDataItem> pItem(new CDataItem());
    pItem->SetData("Test");
    // Automatically deleted when function exits
}
```

## 10. Threading Architecture {#threading-architecture}

Modern applications need responsive UIs through multithreading.

### Threading Strategy

```cpp
// Worker thread for background operations
UINT WorkerThreadProc(LPVOID pParam)
{
    CMyDocument* pDoc = (CMyDocument*)pParam;

    // Perform long operation
    pDoc->PerformLongOperation();

    // Notify UI thread
    AfxGetMainWnd()->PostMessage(WM_OPERATION_COMPLETE, 0, (LPARAM)pDoc);

    return 0;
}

// Starting the thread
void CMyDocument::StartBackgroundOperation()
{
    CWinThread* pThread = AfxBeginThread(
        WorkerThreadProc,
        this,
        THREAD_PRIORITY_NORMAL,
        0,
        CREATE_SUSPENDED);

    if (pThread)
    {
        pThread->m_bAutoDelete = TRUE;
        pThread->ResumeThread();
    }
}

// Thread-safe data access
class CThreadSafeDataManager
{
private:
    CCriticalSection m_criticalSection;
    CArray<CDataItem*> m_data;

public:
    void AddItem(CDataItem* pItem)
    {
        CSingleLock lock(&m_criticalSection);
        lock.Lock();
        m_data.Add(pItem);
        lock.Unlock();
    }

    CDataItem* GetItem(int index)
    {
        CSingleLock lock(&m_criticalSection);
        lock.Lock();
        CDataItem* pItem = NULL;
        if (index >= 0 && index < m_data.GetCount())
            pItem = m_data[index];
        lock.Unlock();
        return pItem;
    }
};
```

## 11. Error Handling Strategy {#error-handling}

Consistent error handling improves reliability and debugging.

### Error Handling Architecture

```cpp
// Error codes
enum ErrorCode
{
    ERROR_NONE = 0,
    ERROR_FILE_NOT_FOUND,
    ERROR_DATABASE_CONNECTION,
    ERROR_INVALID_DATA,
    ERROR_PERMISSION_DENIED,
    ERROR_UNKNOWN
};

// Error information
class CErrorInfo
{
public:
    ErrorCode code;
    CString message;
    CString details;
    CString function;
    int line;

    CErrorInfo()
        : code(ERROR_NONE), line(0) {}

    CErrorInfo(ErrorCode c, const CString& msg, const CString& func, int l)
        : code(c), message(msg), function(func), line(l) {}
};

// Error handler
class CErrorHandler
{
private:
    static CErrorInfo s_lastError;
    static CFile s_logFile;

public:
    static void SetError(ErrorCode code, const CString& message,
        const CString& function, int line)
    {
        s_lastError = CErrorInfo(code, message, function, line);
        LogError(s_lastError);
    }

    static const CErrorInfo& GetLastError()
    {
        return s_lastError;
    }

    static void LogError(const CErrorInfo& error)
    {
        CString logEntry;
        logEntry.Format(_T("[ERROR] %s in %s (line %d): %s\n"),
            GetErrorCodeString(error.code),
            error.function,
            error.line,
            error.message);

        // Write to log file
        if (s_logFile.m_hFile != CFile::hFileNull)
        {
            s_logFile.Write((LPCTSTR)logEntry, logEntry.GetLength() * sizeof(TCHAR));
        }

        // Output to debug window
        TRACE(logEntry);
    }

    static void DisplayError(CWnd* pParent = NULL)
    {
        CString message;
        message.Format(_T("%s\n\nFunction: %s\nLine: %d"),
            s_lastError.message,
            s_lastError.function,
            s_lastError.line);

        AfxMessageBox(message, MB_OK | MB_ICONERROR);
    }
};

// Macro for easy error reporting
#define SET_ERROR(code, message) \
    CErrorHandler::SetError(code, message, _T(__FUNCTION__), __LINE__)

// Usage
BOOL CMyDocument::LoadData(const CString& filename)
{
    CFile file;
    if (!file.Open(filename, CFile::modeRead))
    {
        SET_ERROR(ERROR_FILE_NOT_FOUND, _T("Could not open file: ") + filename);
        return FALSE;
    }

    // ... load data ...

    return TRUE;
}
```

## 12. Testing Strategy {#testing-strategy}

Plan for testing from the beginning.

### Testing Architecture

```cpp
// Testable design
class CDataProcessor
{
public:
    virtual BOOL ProcessData(const CString& input, CString& output)
    {
        // Business logic
        if (input.IsEmpty())
            return FALSE;

        output = input;
        output.MakeUpper();
        return TRUE;
    }
};

// Test harness
class CDataProcessorTest
{
public:
    static void RunAllTests()
    {
        TestEmptyInput();
        TestNormalInput();
        TestSpecialCharacters();
    }

private:
    static void TestEmptyInput()
    {
        CDataProcessor processor;
        CString output;
        BOOL result = processor.ProcessData(_T(""), output);
        ASSERT(!result);  // Should fail with empty input
    }

    static void TestNormalInput()
    {
        CDataProcessor processor;
        CString output;
        BOOL result = processor.ProcessData(_T("hello"), output);
        ASSERT(result);
        ASSERT(output == _T("HELLO"));
    }

    static void TestSpecialCharacters()
    {
        CDataProcessor processor;
        CString output;
        BOOL result = processor.ProcessData(_T("hello@123"), output);
        ASSERT(result);
        ASSERT(output == _T("HELLO@123"));
    }
};
```

## 13. Architecture Documentation {#documentation}

Document your architecture decisions.

### Architecture Decision Records (ADR)

```cpp
/*
ADR-001: Using Document/View Architecture

Date: 2025-01-15
Status: Accepted
Context: Need to support multiple views of the same document
Decision: Use MFC's Document/View architecture
Consequences:
- Automatic file I/O support
- Easy to add multiple views
- Built-in print support
- Slightly more complex than dialog-based
*/

/*
ADR-002: Database Access Layer

Date: 2025-01-20
Status: Accepted
Context: Need to abstract database access for testing
Decision: Implement Repository pattern with interfaces
Consequences:
- Easy to mock for unit testing
- Can switch database providers
- Additional abstraction layer
- Clear separation of concerns
*/
```

## 14. Real-World Architecture Examples {#examples}

### Example 1: Small Utility Application

```cpp
// Simple dialog-based application
// No Document/View needed
class CCalculatorDlg : public CDialogEx
{
private:
    double m_currentValue;
    double m_storedValue;
    Operation m_operation;

public:
    CCalculatorDlg(CWnd* pParent = NULL);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    afx_msg void OnNumberButton(UINT nID);
    afx_msg void OnOperationButton(UINT nID);
    afx_msg void OnEquals();

    DECLARE_MESSAGE_MAP()
};
```

### Example 2: Medium Document Editor

```cpp
// Document/View with single view type
class CTextEditorApp : public CWinApp
{
public:
    virtual BOOL InitInstance()
    {
        CSingleDocTemplate* pDocTemplate = new CSingleDocTemplate(
            IDR_MAINFRAME,
            RUNTIME_CLASS(CTextDocument),
            RUNTIME_CLASS(CMainFrame),
            RUNTIME_CLASS(CTextView));
        AddDocTemplate(pDocTemplate);

        CCommandLineInfo cmdInfo;
        ParseCommandLine(cmdInfo);
        ProcessShellCommand(cmdInfo);

        return TRUE;
    }
};
```

### Example 3: Large Enterprise Application

```cpp
// Complex MDI application with multiple document types
class CEnterpriseApp : public CWinApp
{
private:
    CDatabaseManager m_dbManager;
    CConfigManager m_configManager;
    CSecurityManager m_securityManager;
    CPluginManager m_pluginManager;

public:
    virtual BOOL InitInstance()
    {
        // Initialize subsystems
        if (!m_configManager.LoadConfig())
            return FALSE;

        if (!m_dbManager.Connect(m_configManager.GetConnectionString()))
            return FALSE;

        if (!m_securityManager.Authenticate())
            return FALSE;

        m_pluginManager.LoadPlugins();

        // Create document templates
        CreateDocumentTemplates();

        // Create main window
        CMainFrame* pMainFrame = new CMainFrame;
        if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
            return FALSE;

        m_pMainWnd = pMainFrame;
        pMainFrame->ShowWindow(m_nCmdShow);
        pMainFrame->UpdateWindow();

        return TRUE;
    }

    virtual int ExitInstance()
    {
        m_pluginManager.UnloadPlugins();
        m_dbManager.Disconnect();
        return CWinApp::ExitInstance();
    }
};
```

## 15. Common Architecture Mistakes {#mistakes}

### Mistake 1: God Objects

```cpp
// BAD: Everything in one class
class CBadDocument : public CDocument
{
private:
    CDatabase m_database;
    CArray<CData*> m_data;
    CSyntaxHighlighter m_highlighter;
    CUndoManager m_undoManager;
    CPrintManager m_printManager;
    CExportManager m_exportManager;
    // ... 50 more members

public:
    // ... 200 methods
};

// GOOD: Separate concerns
class CGoodDocument : public CDocument
{
private:
    CArray<CData*> m_data;
    CDataManager* m_pDataManager;
    CUndoManager* m_pUndoManager;

public:
    // Focused interface
    const CArray<CData*>& GetData() const;
    void SetData(const CArray<CData*>& data);
};
```

### Mistake 2: Tight Coupling

```cpp
// BAD: Direct dependencies
class CBadView : public CView
{
    void OnSave()
    {
        CDatabase db;  // Directly using database!
        db.SaveData(GetData());
    }
};

// GOOD: Dependency injection
class CGoodView : public CView
{
    void OnSave()
    {
        CMyDocument* pDoc = GetDocument();
        pDoc->Save();  // Document handles data storage
    }
};
```

### Mistake 3: No Error Handling

```cpp
// BAD: No error checking
void BadFunction()
{
    CFile file;
    file.Open(_T("data.txt"), CFile::modeRead);  // What if it fails?
    // ... use file ...
}

// GOOD: Proper error handling
BOOL GoodFunction()
{
    CFile file;
    if (!file.Open(_T("data.txt"), CFile::modeRead))
    {
        AfxMessageBox(_T("Could not open file"));
        return FALSE;
    }

    TRY
    {
        // ... use file ...
    }
    CATCH(CFileException, e)
    {
        e->ReportError();
        return FALSE;
    }
    END_CATCH

    return TRUE;
}
```

## Summary

Proper application architecture is the foundation of successful MFC development. Key takeaways:

1. **Understand MFC's patterns**: Document/View, command routing, message maps
2. **Plan before coding**: Requirements, components, data flow, UI
3. **Use layered architecture**: Separate presentation, business, and data layers
4. **Manage complexity**: Break into manageable components
5. **Design for testability**: Use interfaces and dependency injection
6. **Handle errors consistently**: Use a unified error handling strategy
7. **Document decisions**: Architecture Decision Records
8. **Consider threading**: Plan for responsive UI
9. **Manage resources**: RAII and smart pointers
10. **Avoid common mistakes**: God objects, tight coupling, poor error handling

In the next lessons, we'll apply these architectural principles to build complete real-world applications, starting with a full-featured text editor.

## Next Steps

- Practice designing architecture for different application types
- Study existing MFC applications to understand their architecture
- Create architecture diagrams for your projects
- Review and refactor existing code with these principles

Remember: Time spent on architecture planning pays dividends throughout the development lifecycle!
