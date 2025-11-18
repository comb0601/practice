# Lesson 69: ADO Database Access

**Duration**: 7 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to ADO
2. ADO Architecture
3. Connection Objects
4. Command Objects
5. Recordset Objects
6. ADO vs ODBC
7. Best Practices
8. Exercises and Projects

---

## 1. Introduction to ADO

### What is ADO?

**ActiveX Data Objects (ADO)** is a high-level, COM-based interface for accessing data from various sources including databases, spreadsheets, and text files.

```cpp
/*
ADO ADVANTAGES:

1. Easier to use than ODBC
2. COM-based (automation support)
3. Supports multiple data sources
4. Disconnected recordsets
5. Better for web applications
6. Rich feature set

ADO OBJECT MODEL:
Connection → Command → Recordset
           → Parameter
           → Field
*/

// Import ADO library
#import "C:\\Program Files\\Common Files\\System\\ado\\msado15.dll" \
    no_namespace rename("EOF", "EndOfFile")
```

---

## 2. ADO Architecture

### Basic Setup

```cpp
// ADOHelper.h
#pragma once

#import "msado15.dll" no_namespace rename("EOF", "EndOfFile")

class CADOHelper
{
public:
    CADOHelper();
    ~CADOHelper();

    BOOL Initialize();
    void Cleanup();

    _ConnectionPtr GetConnection() { return m_pConnection; }

private:
    _ConnectionPtr m_pConnection;
    BOOL m_bInitialized;
};

// ADOHelper.cpp
CADOHelper::CADOHelper()
    : m_bInitialized(FALSE)
{
}

CADOHelper::~CADOHelper()
{
    Cleanup();
}

BOOL CADOHelper::Initialize()
{
    if (m_bInitialized)
        return TRUE;

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
        return FALSE;

    m_bInitialized = TRUE;
    return TRUE;
}

void CADOHelper::Cleanup()
{
    if (m_pConnection && m_pConnection->State == adStateOpen)
    {
        m_pConnection->Close();
    }

    m_pConnection = NULL;

    if (m_bInitialized)
    {
        CoUninitialize();
        m_bInitialized = FALSE;
    }
}
```

---

## 3. Connection Objects

### Opening Connections

```cpp
class CADOConnection
{
public:
    CADOConnection();
    ~CADOConnection();

    BOOL Connect(LPCTSTR lpszConnectionString);
    BOOL ConnectSQLServer(LPCTSTR lpszServer, LPCTSTR lpszDatabase,
                         LPCTSTR lpszUser = NULL, LPCTSTR lpszPassword = NULL);
    void Disconnect();
    BOOL IsConnected();

    _ConnectionPtr GetConnection() { return m_pConnection; }

private:
    _ConnectionPtr m_pConnection;
};

CADOConnection::CADOConnection()
{
    CoInitialize(NULL);
    m_pConnection.CreateInstance(__uuidof(Connection));
}

CADOConnection::~CADOConnection()
{
    Disconnect();
    m_pConnection = NULL;
    CoUninitialize();
}

BOOL CADOConnection::Connect(LPCTSTR lpszConnectionString)
{
    try
    {
        m_pConnection->ConnectionString = _bstr_t(lpszConnectionString);
        m_pConnection->Open("", "", "", adConnectUnspecified);
        return TRUE;
    }
    catch (_com_error& e)
    {
        CString msg;
        msg.Format(_T("Connection failed: %s"), (LPCTSTR)e.Description());
        AfxMessageBox(msg);
        return FALSE;
    }
}

BOOL CADOConnection::ConnectSQLServer(LPCTSTR lpszServer, LPCTSTR lpszDatabase,
                                      LPCTSTR lpszUser, LPCTSTR lpszPassword)
{
    CString strConn;

    if (lpszUser && lpszPassword)
    {
        // SQL Server authentication
        strConn.Format(
            _T("Provider=SQLOLEDB;Data Source=%s;Initial Catalog=%s;User ID=%s;Password=%s;"),
            lpszServer, lpszDatabase, lpszUser, lpszPassword
        );
    }
    else
    {
        // Windows authentication
        strConn.Format(
            _T("Provider=SQLOLEDB;Data Source=%s;Initial Catalog=%s;Integrated Security=SSPI;"),
            lpszServer, lpszDatabase
        );
    }

    return Connect(strConn);
}

void CADOConnection::Disconnect()
{
    if (m_pConnection && m_pConnection->State == adStateOpen)
    {
        m_pConnection->Close();
    }
}

BOOL CADOConnection::IsConnected()
{
    return (m_pConnection && m_pConnection->State == adStateOpen);
}

// Usage
void TestConnection()
{
    CADOConnection conn;

    if (conn.ConnectSQLServer(_T("localhost"), _T("MyDatabase")))
    {
        AfxMessageBox(_T("Connected successfully!"));
        // Use connection...
        conn.Disconnect();
    }
}
```

---

## 4. Command Objects

### Executing Commands

```cpp
class CADOCommand
{
public:
    CADOCommand(_ConnectionPtr pConnection);

    BOOL ExecuteNonQuery(LPCTSTR lpszSQL, long* pRecordsAffected = NULL);
    _RecordsetPtr ExecuteQuery(LPCTSTR lpszSQL);
    BOOL ExecuteStoredProc(LPCTSTR lpszProcName);

    void AddParameter(LPCTSTR lpszName, DataTypeEnum type,
                     ParameterDirectionEnum direction, long size, const _variant_t& value);

private:
    _CommandPtr m_pCommand;
    _ConnectionPtr m_pConnection;
};

CADOCommand::CADOCommand(_ConnectionPtr pConnection)
    : m_pConnection(pConnection)
{
    m_pCommand.CreateInstance(__uuidof(Command));
    m_pCommand->ActiveConnection = m_pConnection;
}

BOOL CADOCommand::ExecuteNonQuery(LPCTSTR lpszSQL, long* pRecordsAffected)
{
    try
    {
        m_pCommand->CommandText = _bstr_t(lpszSQL);
        m_pCommand->CommandType = adCmdText;

        _variant_t vRecordsAffected;
        m_pCommand->Execute(&vRecordsAffected, NULL, adExecuteNoRecords);

        if (pRecordsAffected)
        {
            *pRecordsAffected = vRecordsAffected.lVal;
        }

        return TRUE;
    }
    catch (_com_error& e)
    {
        CString msg;
        msg.Format(_T("Execute failed: %s"), (LPCTSTR)e.Description());
        AfxMessageBox(msg);
        return FALSE;
    }
}

_RecordsetPtr CADOCommand::ExecuteQuery(LPCTSTR lpszSQL)
{
    try
    {
        m_pCommand->CommandText = _bstr_t(lpszSQL);
        m_pCommand->CommandType = adCmdText;

        return m_pCommand->Execute(NULL, NULL, adCmdText);
    }
    catch (_com_error& e)
    {
        CString msg;
        msg.Format(_T("Query failed: %s"), (LPCTSTR)e.Description());
        AfxMessageBox(msg);
        return NULL;
    }
}

void CADOCommand::AddParameter(LPCTSTR lpszName, DataTypeEnum type,
                               ParameterDirectionEnum direction,
                               long size, const _variant_t& value)
{
    try
    {
        _ParameterPtr pParam = m_pCommand->CreateParameter(
            _bstr_t(lpszName),
            type,
            direction,
            size,
            value
        );

        m_pCommand->Parameters->Append(pParam);
    }
    catch (_com_error& e)
    {
        TRACE(_T("AddParameter failed: %s\n"), (LPCTSTR)e.Description());
    }
}

BOOL CADOCommand::ExecuteStoredProc(LPCTSTR lpszProcName)
{
    try
    {
        m_pCommand->CommandText = _bstr_t(lpszProcName);
        m_pCommand->CommandType = adCmdStoredProc;

        m_pCommand->Execute(NULL, NULL, adCmdStoredProc);
        return TRUE;
    }
    catch (_com_error& e)
    {
        CString msg;
        msg.Format(_T("Stored procedure failed: %s"), (LPCTSTR)e.Description());
        AfxMessageBox(msg);
        return FALSE;
    }
}

// Usage examples
void InsertCustomer(CADOConnection& conn)
{
    CADOCommand cmd(conn.GetConnection());

    CString strSQL = _T("INSERT INTO Customers (FirstName, LastName, Email) VALUES ('John', 'Doe', 'john@example.com')");

    long nRecords = 0;
    if (cmd.ExecuteNonQuery(strSQL, &nRecords))
    {
        CString msg;
        msg.Format(_T("%ld record(s) inserted"), nRecords);
        AfxMessageBox(msg);
    }
}

void CallStoredProcedure(CADOConnection& conn)
{
    CADOCommand cmd(conn.GetConnection());

    cmd.AddParameter(_T("@CustomerID"), adInteger, adParamInput, 0, _variant_t((long)123));
    cmd.AddParameter(_T("@Result"), adInteger, adParamOutput, 0, _variant_t((long)0));

    if (cmd.ExecuteStoredProc(_T("usp_GetCustomerOrders")))
    {
        // Retrieve output parameters
        _ParameterPtr pParam = cmd.m_pCommand->Parameters->Item[_T("@Result")];
        long result = pParam->Value.lVal;

        CString msg;
        msg.Format(_T("Result: %ld"), result);
        AfxMessageBox(msg);
    }
}
```

---

## 5. Recordset Objects

### Working with Recordsets

```cpp
class CADORecordset
{
public:
    CADORecordset();
    ~CADORecordset();

    BOOL Open(_ConnectionPtr pConnection, LPCTSTR lpszSQL,
             CursorTypeEnum cursorType = adOpenForwardOnly,
             LockTypeEnum lockType = adLockReadOnly);

    BOOL IsEOF();
    BOOL IsBOF();
    void MoveNext();
    void MovePrevious();
    void MoveFirst();
    void MoveLast();

    _variant_t GetFieldValue(LPCTSTR lpszFieldName);
    _variant_t GetFieldValue(long index);

    BOOL AddNew();
    BOOL Update();
    BOOL Delete();

    long GetRecordCount();

    void Close();

    _RecordsetPtr GetRecordset() { return m_pRecordset; }

private:
    _RecordsetPtr m_pRecordset;
};

CADORecordset::CADORecordset()
{
    m_pRecordset.CreateInstance(__uuidof(Recordset));
}

CADORecordset::~CADORecordset()
{
    Close();
}

BOOL CADORecordset::Open(_ConnectionPtr pConnection, LPCTSTR lpszSQL,
                        CursorTypeEnum cursorType, LockTypeEnum lockType)
{
    try
    {
        m_pRecordset->Open(
            _bstr_t(lpszSQL),
            _variant_t((IDispatch*)pConnection, true),
            cursorType,
            lockType,
            adCmdText
        );

        return TRUE;
    }
    catch (_com_error& e)
    {
        CString msg;
        msg.Format(_T("Recordset open failed: %s"), (LPCTSTR)e.Description());
        AfxMessageBox(msg);
        return FALSE;
    }
}

BOOL CADORecordset::IsEOF()
{
    return (m_pRecordset && m_pRecordset->EndOfFile) ? TRUE : FALSE;
}

BOOL CADORecordset::IsBOF()
{
    return (m_pRecordset && m_pRecordset->BOF) ? TRUE : FALSE;
}

void CADORecordset::MoveNext()
{
    if (m_pRecordset && !m_pRecordset->EndOfFile)
    {
        m_pRecordset->MoveNext();
    }
}

void CADORecordset::MovePrevious()
{
    if (m_pRecordset && !m_pRecordset->BOF)
    {
        m_pRecordset->MovePrevious();
    }
}

void CADORecordset::MoveFirst()
{
    if (m_pRecordset)
    {
        m_pRecordset->MoveFirst();
    }
}

void CADORecordset::MoveLast()
{
    if (m_pRecordset)
    {
        m_pRecordset->MoveLast();
    }
}

_variant_t CADORecordset::GetFieldValue(LPCTSTR lpszFieldName)
{
    try
    {
        return m_pRecordset->Fields->GetItem(_bstr_t(lpszFieldName))->Value;
    }
    catch (_com_error&)
    {
        return _variant_t();
    }
}

_variant_t CADORecordset::GetFieldValue(long index)
{
    try
    {
        return m_pRecordset->Fields->GetItem(index)->Value;
    }
    catch (_com_error&)
    {
        return _variant_t();
    }
}

BOOL CADORecordset::AddNew()
{
    try
    {
        m_pRecordset->AddNew();
        return TRUE;
    }
    catch (_com_error& e)
    {
        TRACE(_T("AddNew failed: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
}

BOOL CADORecordset::Update()
{
    try
    {
        m_pRecordset->Update();
        return TRUE;
    }
    catch (_com_error& e)
    {
        TRACE(_T("Update failed: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
}

BOOL CADORecordset::Delete()
{
    try
    {
        m_pRecordset->Delete(adAffectCurrent);
        m_pRecordset->Update();
        return TRUE;
    }
    catch (_com_error& e)
    {
        TRACE(_T("Delete failed: %s\n"), (LPCTSTR)e.Description());
        return FALSE;
    }
}

long CADORecordset::GetRecordCount()
{
    if (m_pRecordset)
    {
        return m_pRecordset->RecordCount;
    }
    return 0;
}

void CADORecordset::Close()
{
    if (m_pRecordset && m_pRecordset->State == adStateOpen)
    {
        m_pRecordset->Close();
    }
}

// Usage examples
void DisplayCustomers(CADOConnection& conn)
{
    CADORecordset rs;

    if (rs.Open(conn.GetConnection(), _T("SELECT * FROM Customers")))
    {
        while (!rs.IsEOF())
        {
            _variant_t varID = rs.GetFieldValue(_T("CustomerID"));
            _variant_t varFirstName = rs.GetFieldValue(_T("FirstName"));
            _variant_t varLastName = rs.GetFieldValue(_T("LastName"));
            _variant_t varEmail = rs.GetFieldValue(_T("Email"));

            CString msg;
            msg.Format(_T("ID: %ld, Name: %s %s, Email: %s"),
                      varID.lVal,
                      (LPCTSTR)(_bstr_t)varFirstName,
                      (LPCTSTR)(_bstr_t)varLastName,
                      (LPCTSTR)(_bstr_t)varEmail);

            TRACE(_T("%s\n"), msg);

            rs.MoveNext();
        }

        rs.Close();
    }
}

void UpdateCustomerEmail(CADOConnection& conn, long customerID, LPCTSTR lpszNewEmail)
{
    CADORecordset rs;

    CString strSQL;
    strSQL.Format(_T("SELECT * FROM Customers WHERE CustomerID = %ld"), customerID);

    if (rs.Open(conn.GetConnection(), strSQL, adOpenKeyset, adLockOptimistic))
    {
        if (!rs.IsEOF())
        {
            rs.GetRecordset()->Fields->GetItem(_T("Email"))->Value = _bstr_t(lpszNewEmail);

            if (rs.Update())
            {
                AfxMessageBox(_T("Email updated successfully"));
            }
        }

        rs.Close();
    }
}

void AddNewCustomer(CADOConnection& conn, LPCTSTR lpszFirstName,
                   LPCTSTR lpszLastName, LPCTSTR lpszEmail)
{
    CADORecordset rs;

    if (rs.Open(conn.GetConnection(), _T("SELECT * FROM Customers"),
                adOpenKeyset, adLockOptimistic))
    {
        if (rs.AddNew())
        {
            rs.GetRecordset()->Fields->GetItem(_T("FirstName"))->Value = _bstr_t(lpszFirstName);
            rs.GetRecordset()->Fields->GetItem(_T("LastName"))->Value = _bstr_t(lpszLastName);
            rs.GetRecordset()->Fields->GetItem(_T("Email"))->Value = _bstr_t(lpszEmail);

            if (rs.Update())
            {
                AfxMessageBox(_T("Customer added successfully"));
            }
        }

        rs.Close();
    }
}
```

---

## 6. ADO vs ODBC

```cpp
/*
ADO vs ODBC COMPARISON:

ADO:
+ Easier to use
+ COM-based (automation)
+ Disconnected recordsets
+ Better for distributed apps
+ More flexible
- Requires COM
- Slightly slower
- More memory overhead

ODBC:
+ Direct database access
+ Better performance
+ Lower memory usage
+ More mature
- More complex
- Less flexible
- MFC-specific

WHEN TO USE:
- ADO: New applications, web services, automation
- ODBC: Legacy support, maximum performance, MFC-only
*/
```

---

## 7. Best Practices

```cpp
/*
ADO BEST PRACTICES:

1. CONNECTION MANAGEMENT
   - Use connection pooling
   - Close connections when done
   - Handle connection failures
   - Use connection strings wisely

2. RECORDSETS
   - Use appropriate cursor types
   - Choose correct lock types
   - Close recordsets promptly
   - Limit result sets

3. ERROR HANDLING
   - Always use try-catch with _com_error
   - Check for NULL recordsets
   - Validate field values
   - Log errors

4. PERFORMANCE
   - Use stored procedures
   - Batch operations
   - Use disconnected recordsets
   - Minimize round trips

5. SECURITY
   - Never hardcode passwords
   - Use Windows Authentication
   - Validate input
   - Use parameterized queries

6. MEMORY MANAGEMENT
   - Release COM objects
   - Use smart pointers
   - Avoid memory leaks
   - CoInitialize/CoUninitialize

7. TRANSACTIONS
   - Keep short
   - Always commit or rollback
   - Handle errors
   - Don't nest

8. DEBUGGING
   - Check HRESULT values
   - Use TRACE for logging
   - Test error paths
   - Monitor connections
*/
```

---

## Key Takeaways

1. **ADO** is COM-based, easier than ODBC
2. **Connection objects** manage database connections
3. **Command objects** execute SQL and stored procedures
4. **Recordset objects** represent query results
5. **Smart pointers** simplify COM object management
6. **Error handling** uses _com_error
7. **Disconnected recordsets** support distributed scenarios
8. **Performance** requires careful cursor/lock selection
9. **Security** demands input validation
10. **Best practices** ensure robust applications

---

## Next Lesson Preview

In **Lesson 70: Windows Sockets in MFC**, we'll explore:
- Socket programming basics
- CAsyncSocket class
- CSocket class
- Client-server applications
- Network protocols

**Master ADO for powerful database applications!**
