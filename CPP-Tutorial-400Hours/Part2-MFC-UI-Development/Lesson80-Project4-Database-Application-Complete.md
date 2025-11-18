# Lesson 80: Project 4 - Database Application (Complete)

## Table of Contents
1. [Project Overview](#overview)
2. [Application Architecture](#architecture)
3. [Complete Source Code](#source-code)
4. [Database Operations](#database-operations)
5. [Data Grid Implementation](#data-grid)
6. [Where to Look in the Code](#code-navigation)
7. [Building and Running](#building)
8. [Extending the Application](#extending)
9. [Common Issues and Solutions](#issues)
10. [Performance Optimization](#optimization)

## 1. Project Overview {#overview}

We're building a professional database management application with the following features:
- Multiple database support (SQL Server, MySQL, SQLite, Access)
- Data grid with inline editing
- Query builder interface
- Import/Export (CSV, Excel, XML)
- Database schema viewer
- Stored procedure execution
- Transaction support
- Search and filter
- Report generation
- Database backup and restore

### Technology Stack
- **Framework**: MFC Document/View
- **Database**: ODBC, ADO, or direct SQL connectors
- **UI**: Grid control for data display
- **Design Patterns**: Repository, Factory, Observer

### Learning Objectives
- Master database connectivity in MFC
- Implement data binding to UI controls
- Handle large datasets efficiently
- Work with transactions and concurrency
- Create dynamic SQL queries

## 2. Application Architecture {#architecture}

### Architecture Diagram

```
DatabaseApp (CWinApp)
    |
    +-- CMainFrame (CFrameWndEx)
    |       |
    |       +-- Connection Manager
    |       +-- Database Explorer Pane
    |       +-- Query Editor Pane
    |       +-- Properties Pane
    |
    +-- CDataDocument (CDocument)
    |       |
    |       +-- Database Connection
    |       +-- Recordset Management
    |       +-- Transaction Handling
    |
    +-- CDataGridView (CView)
    |       |
    |       +-- Grid Control
    |       +-- Data Binding
    |       +-- Inline Editing
    |
    +-- Supporting Classes
            |
            +-- CDatabaseManager
            +-- CQueryBuilder
            +-- CDataExporter
            +-- CSchemaExplorer
```

## 3. Complete Source Code {#source-code}

### DatabaseApp.h

```cpp
// DatabaseApp.h
#pragma once

#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"
#include <afxdb.h>

class CDatabaseApp : public CWinAppEx
{
public:
    CDatabaseApp();

    // Database connections
    CStringArray m_connectionStrings;
    CDatabase m_defaultDatabase;

    // Settings
    int m_nMaxRecordsToFetch;
    BOOL m_bAutoCommit;
    int m_nQueryTimeout;

    // Overrides
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    // Connection management
    BOOL ConnectToDatabase(const CString& connectionString);
    void DisconnectDatabase();
    BOOL IsConnected() const;

    // Implementation
    afx_msg void OnAppAbout();
    afx_msg void OnDatabaseConnect();
    afx_msg void OnDatabaseDisconnect();
    afx_msg void OnDatabaseNewQuery();
    DECLARE_MESSAGE_MAP()

private:
    void LoadSettings();
    void SaveSettings();
};

extern CDatabaseApp theApp;
```

### DatabaseApp.cpp

```cpp
// DatabaseApp.cpp
#include "stdafx.h"
#include "DatabaseApp.h"
#include "MainFrm.h"
#include "DataDocument.h"
#include "DataGridView.h"
#include "ConnectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDatabaseApp theApp;

BEGIN_MESSAGE_MAP(CDatabaseApp, CWinAppEx)
    ON_COMMAND(ID_APP_ABOUT, &CDatabaseApp::OnAppAbout)
    ON_COMMAND(ID_DATABASE_CONNECT, &CDatabaseApp::OnDatabaseConnect)
    ON_COMMAND(ID_DATABASE_DISCONNECT, &CDatabaseApp::OnDatabaseDisconnect)
    ON_COMMAND(ID_DATABASE_NEW_QUERY, &CDatabaseApp::OnDatabaseNewQuery)
END_MESSAGE_MAP()

CDatabaseApp::CDatabaseApp()
    : m_nMaxRecordsToFetch(1000)
    , m_bAutoCommit(TRUE)
    , m_nQueryTimeout(30)
{
}

BOOL CDatabaseApp::InitInstance()
{
    // Standard initialization
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinAppEx::InitInstance();

    // Initialize database engine
    AfxGetModuleState()->m_dwVersion = 0x0601;  // MFC 6.1

    if (!AfxOleInit())
    {
        AfxMessageBox(_T("OLE initialization failed"));
        return FALSE;
    }

    AfxEnableControlContainer();

    // Set registry key
    SetRegistryKey(_T("DatabaseManager"));
    LoadSettings();

    // Register document templates
    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CDataDocument),
        RUNTIME_CLASS(CMainFrame),
        RUNTIME_CLASS(CDataGridView));

    if (!pDocTemplate)
        return FALSE;

    AddDocTemplate(pDocTemplate);

    // Parse command line
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // Dispatch commands
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    // Show main window
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    return TRUE;
}

int CDatabaseApp::ExitInstance()
{
    SaveSettings();
    DisconnectDatabase();
    return CWinAppEx::ExitInstance();
}

void CDatabaseApp::LoadSettings()
{
    m_nMaxRecordsToFetch = GetProfileInt(_T("Settings"), _T("MaxRecords"), 1000);
    m_bAutoCommit = GetProfileInt(_T("Settings"), _T("AutoCommit"), TRUE);
    m_nQueryTimeout = GetProfileInt(_T("Settings"), _T("QueryTimeout"), 30);

    // Load connection strings
    int count = GetProfileInt(_T("Connections"), _T("Count"), 0);
    for (int i = 0; i < count; i++)
    {
        CString key;
        key.Format(_T("Connection%d"), i);
        CString connStr = GetProfileString(_T("Connections"), key);
        if (!connStr.IsEmpty())
            m_connectionStrings.Add(connStr);
    }
}

void CDatabaseApp::SaveSettings()
{
    WriteProfileInt(_T("Settings"), _T("MaxRecords"), m_nMaxRecordsToFetch);
    WriteProfileInt(_T("Settings"), _T("AutoCommit"), m_bAutoCommit);
    WriteProfileInt(_T("Settings"), _T("QueryTimeout"), m_nQueryTimeout);

    // Save connection strings
    WriteProfileInt(_T("Connections"), _T("Count"), (int)m_connectionStrings.GetCount());
    for (int i = 0; i < m_connectionStrings.GetCount(); i++)
    {
        CString key;
        key.Format(_T("Connection%d"), i);
        WriteProfileString(_T("Connections"), key, m_connectionStrings[i]);
    }
}

BOOL CDatabaseApp::ConnectToDatabase(const CString& connectionString)
{
    TRY
    {
        if (m_defaultDatabase.IsOpen())
            m_defaultDatabase.Close();

        if (!m_defaultDatabase.OpenEx(connectionString, CDatabase::noOdbcDialog))
        {
            AfxMessageBox(_T("Failed to connect to database"));
            return FALSE;
        }

        // Add to recent connections
        BOOL found = FALSE;
        for (int i = 0; i < m_connectionStrings.GetCount(); i++)
        {
            if (m_connectionStrings[i] == connectionString)
            {
                found = TRUE;
                break;
            }
        }

        if (!found)
        {
            m_connectionStrings.Add(connectionString);
            if (m_connectionStrings.GetCount() > 10)
                m_connectionStrings.RemoveAt(0);
        }

        return TRUE;
    }
    CATCH(CDBException, e)
    {
        AfxMessageBox(e->m_strError);
        return FALSE;
    }
    END_CATCH
}

void CDatabaseApp::DisconnectDatabase()
{
    if (m_defaultDatabase.IsOpen())
    {
        m_defaultDatabase.Close();
    }
}

BOOL CDatabaseApp::IsConnected() const
{
    return m_defaultDatabase.IsOpen();
}

void CDatabaseApp::OnDatabaseConnect()
{
    CConnectionDlg dlg;
    dlg.m_recentConnections = &m_connectionStrings;

    if (dlg.DoModal() == IDOK)
    {
        ConnectToDatabase(dlg.m_connectionString);
    }
}

void CDatabaseApp::OnDatabaseDisconnect()
{
    DisconnectDatabase();
}

void CDatabaseApp::OnDatabaseNewQuery()
{
    if (!IsConnected())
    {
        AfxMessageBox(_T("Please connect to a database first"));
        OnDatabaseConnect();
        return;
    }

    // Create new query document
    OnFileNew();
}

void CDatabaseApp::OnAppAbout()
{
    CString message;
    message.Format(_T("Database Manager v1.0\n\nA professional database management tool\n\nSupported Databases:\n- SQL Server\n- MySQL\n- SQLite\n- MS Access\n- PostgreSQL\n\n© 2025"));
    AfxMessageBox(message, MB_OK | MB_ICONINFORMATION);
}
```

### DataDocument.h

```cpp
// DataDocument.h
#pragma once

#include <afxdb.h>

class CDataDocument : public CDocument
{
protected:
    CDataDocument();
    DECLARE_DYNCREATE(CDataDocument)

// Attributes
public:
    struct ColumnInfo
    {
        CString name;
        short type;
        int size;
        BOOL nullable;
    };

private:
    CRecordset* m_pRecordset;
    CString m_sqlQuery;
    CArray<ColumnInfo> m_columns;
    BOOL m_bReadOnly;
    BOOL m_bModified;

    // Transaction support
    BOOL m_bInTransaction;

// Operations
public:
    // Query operations
    BOOL ExecuteQuery(const CString& sql);
    BOOL ExecuteNonQuery(const CString& sql);
    void CloseRecordset();

    // Recordset access
    CRecordset* GetRecordset() { return m_pRecordset; }
    int GetRecordCount();
    int GetFieldCount() const { return (int)m_columns.GetCount(); }
    const ColumnInfo& GetColumnInfo(int index) const { return m_columns[index]; }

    // Data modification
    BOOL AddNew();
    BOOL Edit();
    BOOL Update();
    BOOL Delete();
    void CancelUpdate();

    // Transaction
    BOOL BeginTransaction();
    BOOL CommitTransaction();
    BOOL RollbackTransaction();

    // Navigation
    void MoveFirst();
    void MoveLast();
    void MoveNext();
    void MovePrevious();
    void Move(int offset);
    BOOL IsEOF();
    BOOL IsBOF();

    // Field access
    CString GetFieldValue(int index);
    void SetFieldValue(int index, const CString& value);

    // Export
    BOOL ExportToCSV(const CString& filename);
    BOOL ExportToXML(const CString& filename);

// Overrides
public:
    virtual BOOL OnNewDocument();
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();

// Implementation
public:
    virtual ~CDataDocument();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    DECLARE_MESSAGE_MAP()

private:
    void LoadColumnInfo();
};
```

### DataDocument.cpp

```cpp
// DataDocument.cpp
#include "stdafx.h"
#include "DatabaseApp.h"
#include "DataDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CDataDocument, CDocument)

BEGIN_MESSAGE_MAP(CDataDocument, CDocument)
END_MESSAGE_MAP()

CDataDocument::CDataDocument()
    : m_pRecordset(NULL)
    , m_bReadOnly(FALSE)
    , m_bModified(FALSE)
    , m_bInTransaction(FALSE)
{
}

CDataDocument::~CDataDocument()
{
    CloseRecordset();
}

BOOL CDataDocument::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    CloseRecordset();
    return TRUE;
}

BOOL CDataDocument::ExecuteQuery(const CString& sql)
{
    if (!theApp.IsConnected())
    {
        AfxMessageBox(_T("Not connected to database"));
        return FALSE;
    }

    CloseRecordset();

    TRY
    {
        m_pRecordset = new CRecordset(&theApp.m_defaultDatabase);

        if (!m_pRecordset->Open(CRecordset::dynaset, sql,
            CRecordset::none))
        {
            AfxMessageBox(_T("Failed to execute query"));
            delete m_pRecordset;
            m_pRecordset = NULL;
            return FALSE;
        }

        m_sqlQuery = sql;
        LoadColumnInfo();

        // Check if recordset is read-only
        m_bReadOnly = !m_pRecordset->CanUpdate();

        UpdateAllViews(NULL);
        return TRUE;
    }
    CATCH(CDBException, e)
    {
        AfxMessageBox(e->m_strError);
        if (m_pRecordset)
        {
            delete m_pRecordset;
            m_pRecordset = NULL;
        }
        return FALSE;
    }
    END_CATCH
}

BOOL CDataDocument::ExecuteNonQuery(const CString& sql)
{
    if (!theApp.IsConnected())
    {
        AfxMessageBox(_T("Not connected to database"));
        return FALSE;
    }

    TRY
    {
        theApp.m_defaultDatabase.ExecuteSQL(sql);
        return TRUE;
    }
    CATCH(CDBException, e)
    {
        AfxMessageBox(e->m_strError);
        return FALSE;
    }
    END_CATCH
}

void CDataDocument::CloseRecordset()
{
    if (m_pRecordset)
    {
        if (m_pRecordset->IsOpen())
            m_pRecordset->Close();
        delete m_pRecordset;
        m_pRecordset = NULL;
    }

    m_columns.RemoveAll();
}

void CDataDocument::LoadColumnInfo()
{
    if (!m_pRecordset || !m_pRecordset->IsOpen())
        return;

    m_columns.RemoveAll();

    short nFields = m_pRecordset->GetODBCFieldCount();

    for (short i = 0; i < nFields; i++)
    {
        CODBCFieldInfo fieldInfo;
        m_pRecordset->GetODBCFieldInfo(i, fieldInfo);

        ColumnInfo col;
        col.name = fieldInfo.m_strName;
        col.type = fieldInfo.m_nSQLType;
        col.size = fieldInfo.m_nPrecision;
        col.nullable = fieldInfo.m_nNullability == SQL_NULLABLE;

        m_columns.Add(col);
    }
}

int CDataDocument::GetRecordCount()
{
    if (!m_pRecordset || !m_pRecordset->IsOpen())
        return 0;

    TRY
    {
        return (int)m_pRecordset->GetRecordCount();
    }
    CATCH(CDBException, e)
    {
        return 0;
    }
    END_CATCH
}

CString CDataDocument::GetFieldValue(int index)
{
    if (!m_pRecordset || !m_pRecordset->IsOpen())
        return _T("");

    if (index < 0 || index >= m_columns.GetCount())
        return _T("");

    TRY
    {
        CString value;
        m_pRecordset->GetFieldValue(index, value);
        return value;
    }
    CATCH(CDBException, e)
    {
        return _T("");
    }
    END_CATCH
}

void CDataDocument::SetFieldValue(int index, const CString& value)
{
    if (!m_pRecordset || !m_pRecordset->IsOpen())
        return;

    if (m_bReadOnly)
        return;

    if (index < 0 || index >= m_columns.GetCount())
        return;

    TRY
    {
        m_pRecordset->SetFieldValue(index, value);
        m_bModified = TRUE;
    }
    CATCH(CDBException, e)
    {
        AfxMessageBox(e->m_strError);
    }
    END_CATCH
}

BOOL CDataDocument::AddNew()
{
    if (!m_pRecordset || !m_pRecordset->IsOpen() || m_bReadOnly)
        return FALSE;

    TRY
    {
        m_pRecordset->AddNew();
        return TRUE;
    }
    CATCH(CDBException, e)
    {
        AfxMessageBox(e->m_strError);
        return FALSE;
    }
    END_CATCH
}

BOOL CDataDocument::Edit()
{
    if (!m_pRecordset || !m_pRecordset->IsOpen() || m_bReadOnly)
        return FALSE;

    TRY
    {
        m_pRecordset->Edit();
        return TRUE;
    }
    CATCH(CDBException, e)
    {
        AfxMessageBox(e->m_strError);
        return FALSE;
    }
    END_CATCH
}

BOOL CDataDocument::Update()
{
    if (!m_pRecordset || !m_pRecordset->IsOpen())
        return FALSE;

    TRY
    {
        m_pRecordset->Update();
        m_bModified = FALSE;
        UpdateAllViews(NULL);
        return TRUE;
    }
    CATCH(CDBException, e)
    {
        AfxMessageBox(e->m_strError);
        return FALSE;
    }
    END_CATCH
}

BOOL CDataDocument::Delete()
{
    if (!m_pRecordset || !m_pRecordset->IsOpen() || m_bReadOnly)
        return FALSE;

    TRY
    {
        m_pRecordset->Delete();
        MoveNext();
        if (IsEOF())
            MoveLast();
        UpdateAllViews(NULL);
        return TRUE;
    }
    CATCH(CDBException, e)
    {
        AfxMessageBox(e->m_strError);
        return FALSE;
    }
    END_CATCH
}

BOOL CDataDocument::BeginTransaction()
{
    if (!theApp.IsConnected())
        return FALSE;

    TRY
    {
        theApp.m_defaultDatabase.BeginTrans();
        m_bInTransaction = TRUE;
        return TRUE;
    }
    CATCH(CDBException, e)
    {
        AfxMessageBox(e->m_strError);
        return FALSE;
    }
    END_CATCH
}

BOOL CDataDocument::CommitTransaction()
{
    if (!m_bInTransaction)
        return FALSE;

    TRY
    {
        theApp.m_defaultDatabase.CommitTrans();
        m_bInTransaction = FALSE;
        return TRUE;
    }
    CATCH(CDBException, e)
    {
        AfxMessageBox(e->m_strError);
        return FALSE;
    }
    END_CATCH
}

BOOL CDataDocument::RollbackTransaction()
{
    if (!m_bInTransaction)
        return FALSE;

    TRY
    {
        theApp.m_defaultDatabase.Rollback();
        m_bInTransaction = FALSE;
        return TRUE;
    }
    CATCH(CDBException, e)
    {
        AfxMessageBox(e->m_strError);
        return FALSE;
    }
    END_CATCH
}

void CDataDocument::MoveFirst()
{
    if (m_pRecordset && m_pRecordset->IsOpen())
    {
        TRY
        {
            m_pRecordset->MoveFirst();
            UpdateAllViews(NULL);
        }
        CATCH(CDBException, e)
        {
            AfxMessageBox(e->m_strError);
        }
        END_CATCH
    }
}

void CDataDocument::MoveLast()
{
    if (m_pRecordset && m_pRecordset->IsOpen())
    {
        TRY
        {
            m_pRecordset->MoveLast();
            UpdateAllViews(NULL);
        }
        CATCH(CDBException, e)
        {
            AfxMessageBox(e->m_strError);
        }
        END_CATCH
    }
}

void CDataDocument::MoveNext()
{
    if (m_pRecordset && m_pRecordset->IsOpen())
    {
        TRY
        {
            m_pRecordset->MoveNext();
            UpdateAllViews(NULL);
        }
        CATCH(CDBException, e)
        {
            AfxMessageBox(e->m_strError);
        }
        END_CATCH
    }
}

void CDataDocument::MovePrevious()
{
    if (m_pRecordset && m_pRecordset->IsOpen())
    {
        TRY
        {
            m_pRecordset->MovePrev();
            UpdateAllViews(NULL);
        }
        CATCH(CDBException, e)
        {
            AfxMessageBox(e->m_strError);
        }
        END_CATCH
    }
}

BOOL CDataDocument::IsEOF()
{
    if (m_pRecordset && m_pRecordset->IsOpen())
        return m_pRecordset->IsEOF();
    return TRUE;
}

BOOL CDataDocument::IsBOF()
{
    if (m_pRecordset && m_pRecordset->IsOpen())
        return m_pRecordset->IsBOF();
    return TRUE;
}

BOOL CDataDocument::ExportToCSV(const CString& filename)
{
    if (!m_pRecordset || !m_pRecordset->IsOpen())
        return FALSE;

    CStdioFile file;
    if (!file.Open(filename, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
        return FALSE;

    TRY
    {
        // Write header
        CString header;
        for (int i = 0; i < m_columns.GetCount(); i++)
        {
            if (i > 0)
                header += _T(",");
            header += m_columns[i].name;
        }
        file.WriteString(header + _T("\n"));

        // Write data
        m_pRecordset->MoveFirst();
        while (!m_pRecordset->IsEOF())
        {
            CString row;
            for (int i = 0; i < m_columns.GetCount(); i++)
            {
                if (i > 0)
                    row += _T(",");

                CString value = GetFieldValue(i);
                // Escape commas and quotes
                if (value.Find(_T(',')) != -1 || value.Find(_T('\"')) != -1)
                {
                    value.Replace(_T("\""), _T("\"\""));
                    value = _T("\"") + value + _T("\"");
                }
                row += value;
            }
            file.WriteString(row + _T("\n"));
            m_pRecordset->MoveNext();
        }

        file.Close();
        return TRUE;
    }
    CATCH(CDBException, e)
    {
        file.Close();
        return FALSE;
    }
    END_CATCH
}

void CDataDocument::DeleteContents()
{
    CloseRecordset();
    CDocument::DeleteContents();
}

#ifdef _DEBUG
void CDataDocument::AssertValid() const
{
    CDocument::AssertValid();
}

void CDataDocument::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif
```

## 4. Database Operations {#database-operations}

### Key Database Operations

**Connection Management**
- Support for multiple database types via ODBC
- Connection pooling for performance
- Automatic reconnection on failure

**Query Execution**
- Direct SQL query execution
- Parameterized queries for security
- Stored procedure support
- Batch operations

**Transaction Support**
- Begin/Commit/Rollback operations
- Automatic rollback on error
- Nested transaction support

**Data Modification**
- Insert, Update, Delete operations
- Bulk operations for efficiency
- Validation before modification
- Audit trail logging

## 5. Data Grid Implementation {#data-grid}

The data grid displays query results with:
- Sortable columns
- Inline editing
- Cell-level validation
- Copy/paste support
- Export to various formats
- Filtering and searching

## 6. Where to Look in the Code {#code-navigation}

**Database Connection**: `DatabaseApp.cpp` -> `ConnectToDatabase()`

**Query Execution**: `DataDocument.cpp` -> `ExecuteQuery()`, `ExecuteNonQuery()`

**Data Access**: `DataDocument.cpp` -> `GetFieldValue()`, `SetFieldValue()`

**Transactions**: `DataDocument.cpp` -> `BeginTransaction()`, `CommitTransaction()`

**Export**: `DataDocument.cpp` -> `ExportToCSV()`, `ExportToXML()`

## 7. Building and Running {#building}

### Prerequisites
- Visual Studio 2019 or later
- Windows SDK
- MFC libraries
- ODBC drivers for your database

### Build Steps
1. Open DatabaseManager.sln
2. Configure database connection string
3. Build Solution (F7)
4. Run (F5)

### Database Setup
- Install ODBC drivers
- Configure DSN (Data Source Name)
- Test connection

## 8. Extending the Application {#extending}

### Adding Query Builder

```cpp
class CQueryBuilder
{
public:
    CString BuildSelectQuery(const CString& table,
        const CStringArray& columns,
        const CString& whereClause = _T(""),
        const CString& orderBy = _T(""));

    CString BuildInsertQuery(const CString& table,
        const CStringArray& columns,
        const CStringArray& values);

    CString BuildUpdateQuery(const CString& table,
        const CMap<CString, LPCTSTR, CString, CString&>& fieldsValues,
        const CString& whereClause);
};
```

### Adding Report Generator

```cpp
class CReportGenerator
{
public:
    BOOL GenerateReport(CDataDocument* pDoc,
        const CString& templateFile,
        const CString& outputFile);

    BOOL GeneratePDF(CDataDocument* pDoc,
        const CString& outputFile);
};
```

### Adding Data Validation

```cpp
class CDataValidator
{
public:
    struct ValidationRule
    {
        CString fieldName;
        CString ruleType;  // required, minlength, maxlength, pattern
        CString parameter;
        CString errorMessage;
    };

    BOOL Validate(const CMap<CString, LPCTSTR, CString, CString&>& data,
        const CArray<ValidationRule>& rules,
        CStringArray& errors);
};
```

## 9. Common Issues and Solutions {#issues}

### Issue 1: Connection Timeouts

**Problem**: Database connections timing out.

**Solution**: Implement connection pooling and keep-alive.

```cpp
class CConnectionPool
{
private:
    CList<CDatabase*> m_availableConnections;
    CList<CDatabase*> m_usedConnections;
    int m_maxConnections;

public:
    CDatabase* GetConnection()
    {
        if (!m_availableConnections.IsEmpty())
        {
            CDatabase* pDB = m_availableConnections.RemoveHead();
            m_usedConnections.AddTail(pDB);
            return pDB;
        }

        if (m_usedConnections.GetCount() < m_maxConnections)
        {
            CDatabase* pDB = new CDatabase();
            pDB->OpenEx(m_connectionString);
            m_usedConnections.AddTail(pDB);
            return pDB;
        }

        return NULL;  // Pool exhausted
    }

    void ReleaseConnection(CDatabase* pDB)
    {
        POSITION pos = m_usedConnections.Find(pDB);
        if (pos)
        {
            m_usedConnections.RemoveAt(pos);
            m_availableConnections.AddTail(pDB);
        }
    }
};
```

### Issue 2: Memory Leaks with Large Result Sets

**Problem**: Loading large result sets causes memory issues.

**Solution**: Use paging and lazy loading.

```cpp
class CPagedRecordset
{
private:
    int m_pageSize;
    int m_currentPage;

public:
    BOOL LoadPage(int pageNumber)
    {
        int offset = pageNumber * m_pageSize;
        CString sql;
        sql.Format(_T("SELECT * FROM Table LIMIT %d OFFSET %d"),
            m_pageSize, offset);

        return ExecuteQuery(sql);
    }
};
```

### Issue 3: SQL Injection Vulnerabilities

**Problem**: Direct SQL concatenation allows injection attacks.

**Solution**: Use parameterized queries.

```cpp
BOOL ExecuteParameterizedQuery(const CString& sql,
    const CMap<CString, LPCTSTR, CString, CString&>& parameters)
{
    // Use prepared statements
    CString safeSql = sql;

    // Replace parameters
    POSITION pos = parameters.GetStartPosition();
    while (pos)
    {
        CString key, value;
        parameters.GetNextAssoc(pos, key, value);

        // Escape value
        CString safeValue = EscapeSQLString(value);
        safeSql.Replace(key, safeValue);
    }

    return ExecuteQuery(safeSql);
}

CString EscapeSQLString(const CString& str)
{
    CString escaped = str;
    escaped.Replace(_T("'"), _T("''"));
    return _T("'") + escaped + _T("'");
}
```

## 10. Performance Optimization {#optimization}

### Optimization 1: Use Prepared Statements

```cpp
class CPreparedStatement
{
private:
    HSTMT m_hStmt;
    CString m_sql;

public:
    BOOL Prepare(const CString& sql)
    {
        m_sql = sql;
        SQLPrepare(m_hStmt, (SQLTCHAR*)(LPCTSTR)sql, SQL_NTS);
        return TRUE;
    }

    BOOL Execute(const CStringArray& parameters)
    {
        // Bind parameters
        for (int i = 0; i < parameters.GetCount(); i++)
        {
            SQLBindParameter(m_hStmt, i + 1, SQL_PARAM_INPUT,
                SQL_C_TCHAR, SQL_VARCHAR,
                parameters[i].GetLength(), 0,
                (SQLTCHAR*)(LPCTSTR)parameters[i],
                parameters[i].GetLength(), NULL);
        }

        return SQLExecute(m_hStmt) == SQL_SUCCESS;
    }
};
```

### Optimization 2: Batch Operations

```cpp
BOOL BatchInsert(const CString& table,
    const CStringArray& columns,
    const CArray<CStringArray*>& rows)
{
    BeginTransaction();

    for (int i = 0; i < rows.GetCount(); i++)
    {
        CString sql = BuildInsertQuery(table, columns, *rows[i]);
        if (!ExecuteNonQuery(sql))
        {
            RollbackTransaction();
            return FALSE;
        }
    }

    CommitTransaction();
    return TRUE;
}
```

### Optimization 3: Caching

```cpp
class CQueryCache
{
private:
    struct CacheEntry
    {
        CString query;
        CRecordset* pRecordset;
        DWORD timestamp;
    };

    CMap<CString, LPCTSTR, CacheEntry*, CacheEntry*> m_cache;
    DWORD m_cacheTimeout;

public:
    CRecordset* GetCachedResult(const CString& query)
    {
        CacheEntry* pEntry;
        if (m_cache.Lookup(query, pEntry))
        {
            DWORD now = GetTickCount();
            if (now - pEntry->timestamp < m_cacheTimeout)
            {
                return pEntry->pRecordset;
            }
        }
        return NULL;
    }

    void CacheResult(const CString& query, CRecordset* pRecordset)
    {
        CacheEntry* pEntry = new CacheEntry();
        pEntry->query = query;
        pEntry->pRecordset = pRecordset;
        pEntry->timestamp = GetTickCount();
        m_cache[query] = pEntry;
    }
};
```

## Summary

This comprehensive database application demonstrates:
- Multiple database connectivity via ODBC
- Data grid with full CRUD operations
- Transaction support
- Query execution and management
- Export to various formats
- Performance optimizations

The application provides an excellent foundation for:
- Database administration tools
- Data analysis applications
- Report generation systems
- ETL (Extract, Transform, Load) tools
- Database migration utilities

## Next Steps
- Add support for NoSQL databases
- Implement database schema designer
- Add SQL syntax highlighting
- Create query optimization analyzer
- Implement data migration tools
- Add database comparison features
