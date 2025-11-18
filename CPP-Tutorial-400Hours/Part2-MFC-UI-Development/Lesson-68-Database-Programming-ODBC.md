# Lesson 68: Database Programming with MFC (ODBC)

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to ODBC
2. Setting Up ODBC Data Sources
3. CDatabase Class
4. CRecordset Basics
5. SQL Queries and Operations
6. Transactions
7. Data Binding to Controls
8. Error Handling
9. Best Practices
10. Exercises and Projects

---

## 1. Introduction to ODBC

### What is ODBC?

**Open Database Connectivity (ODBC)** is a standard API for accessing database management systems (DBMS). MFC provides classes that wrap ODBC functionality.

```cpp
/*
ODBC ARCHITECTURE:

Application (MFC)
    ↓
ODBC Driver Manager
    ↓
ODBC Driver (SQL Server, MySQL, etc.)
    ↓
Database

MFC ODBC CLASSES:
- CDatabase: Database connection
- CRecordset: Query results
- CRecordView: View with data binding
- CFieldExchange: Data transfer
*/
```

---

## 2. Setting Up ODBC Data Sources

### Creating DSN

```cpp
// Manual DSN creation through ODBC Data Source Administrator
// Or programmatically:

BOOL CreateDSN(LPCTSTR lpszDSN, LPCTSTR lpszDriver, LPCTSTR lpszServer,
               LPCTSTR lpszDatabase)
{
    CString strAttributes;
    strAttributes.Format(
        _T("DSN=%s\0")
        _T("SERVER=%s\0")
        _T("DATABASE=%s\0")
        _T("\0"),
        lpszDSN, lpszServer, lpszDatabase
    );

    return SQLConfigDataSource(
        NULL,
        ODBC_ADD_DSN,
        lpszDriver,
        strAttributes
    );
}

// Usage
void CreateMyDSN()
{
    CreateDSN(
        _T("MyAppDB"),
        _T("SQL Server"),
        _T("localhost"),
        _T("MyDatabase")
    );
}
```

---

## 3. CDatabase Class

### Opening Database Connection

```cpp
// DatabaseHelper.h
class CDatabaseHelper
{
public:
    CDatabaseHelper();
    ~CDatabaseHelper();

    BOOL Connect(LPCTSTR lpszDSN, LPCTSTR lpszUser = NULL,
                 LPCTSTR lpszPassword = NULL);
    BOOL ConnectWithConnectionString(LPCTSTR lpszConnectionString);
    void Disconnect();
    BOOL IsOpen() const { return m_database.IsOpen(); }

    CDatabase* GetDatabase() { return &m_database; }

    BOOL ExecuteSQL(LPCTSTR lpszSQL);
    BOOL BeginTrans();
    BOOL CommitTrans();
    BOOL Rollback();

private:
    CDatabase m_database;
};

// DatabaseHelper.cpp
CDatabaseHelper::CDatabaseHelper()
{
}

CDatabaseHelper::~CDatabaseHelper()
{
    Disconnect();
}

BOOL CDatabaseHelper::Connect(LPCTSTR lpszDSN, LPCTSTR lpszUser,
                              LPCTSTR lpszPassword)
{
    try
    {
        CString strConnect;

        if (lpszUser && lpszPassword)
        {
            strConnect.Format(_T("DSN=%s;UID=%s;PWD=%s"),
                            lpszDSN, lpszUser, lpszPassword);
        }
        else
        {
            strConnect.Format(_T("DSN=%s"), lpszDSN);
        }

        return m_database.Open(NULL, FALSE, FALSE, strConnect);
    }
    catch (CDBException* e)
    {
        TRACE(_T("Database connection failed: %s\n"), e->m_strError);
        e->Delete();
        return FALSE;
    }
}

BOOL CDatabaseHelper::ConnectWithConnectionString(LPCTSTR lpszConnectionString)
{
    try
    {
        return m_database.OpenEx(lpszConnectionString, CDatabase::noOdbcDialog);
    }
    catch (CDBException* e)
    {
        TRACE(_T("Database connection failed: %s\n"), e->m_strError);
        e->Delete();
        return FALSE;
    }
}

void CDatabaseHelper::Disconnect()
{
    if (m_database.IsOpen())
    {
        try
        {
            m_database.Close();
        }
        catch (CDBException* e)
        {
            e->Delete();
        }
    }
}

BOOL CDatabaseHelper::ExecuteSQL(LPCTSTR lpszSQL)
{
    try
    {
        m_database.ExecuteSQL(lpszSQL);
        return TRUE;
    }
    catch (CDBException* e)
    {
        TRACE(_T("SQL execution failed: %s\n"), e->m_strError);
        e->Delete();
        return FALSE;
    }
}

BOOL CDatabaseHelper::BeginTrans()
{
    return m_database.BeginTrans();
}

BOOL CDatabaseHelper::CommitTrans()
{
    return m_database.CommitTrans();
}

BOOL CDatabaseHelper::Rollback()
{
    return m_database.Rollback();
}
```

---

## 4. CRecordset Basics

### Creating Recordset Class

```cpp
// CustomerSet.h
class CCustomerSet : public CRecordset
{
public:
    CCustomerSet(CDatabase* pDatabase = NULL);
    DECLARE_DYNAMIC(CCustomerSet)

    // Field/Param Data
    long m_CustomerID;
    CString m_FirstName;
    CString m_LastName;
    CString m_Email;
    CString m_Phone;
    COleDateTime m_CreatedDate;

    // Overrides
    virtual CString GetDefaultConnect();
    virtual CString GetDefaultSQL();
    virtual void DoFieldExchange(CFieldExchange* pFX);

    // Implementation
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
};

// CustomerSet.cpp
IMPLEMENT_DYNAMIC(CCustomerSet, CRecordset)

CCustomerSet::CCustomerSet(CDatabase* pdb)
    : CRecordset(pdb)
{
    m_CustomerID = 0;
    m_FirstName = _T("");
    m_LastName = _T("");
    m_Email = _T("");
    m_Phone = _T("");

    m_nFields = 6;
    m_nDefaultType = snapshot;
}

CString CCustomerSet::GetDefaultConnect()
{
    return _T("DSN=MyAppDB");
}

CString CCustomerSet::GetDefaultSQL()
{
    return _T("[Customers]");
}

void CCustomerSet::DoFieldExchange(CFieldExchange* pFX)
{
    pFX->SetFieldType(CFieldExchange::outputColumn);

    RFX_Long(pFX, _T("[CustomerID]"), m_CustomerID);
    RFX_Text(pFX, _T("[FirstName]"), m_FirstName);
    RFX_Text(pFX, _T("[LastName]"), m_LastName);
    RFX_Text(pFX, _T("[Email]"), m_Email);
    RFX_Text(pFX, _T("[Phone]"), m_Phone);
    RFX_Date(pFX, _T("[CreatedDate]"), m_CreatedDate);
}

// Using recordset
void DisplayCustomers()
{
    CDatabase db;
    if (!db.OpenEx(_T("DSN=MyAppDB"), CDatabase::noOdbcDialog))
    {
        AfxMessageBox(_T("Failed to connect to database"));
        return;
    }

    CCustomerSet rs(&db);

    try
    {
        rs.Open();

        while (!rs.IsEOF())
        {
            CString msg;
            msg.Format(_T("ID: %d, Name: %s %s, Email: %s"),
                      rs.m_CustomerID,
                      rs.m_FirstName,
                      rs.m_LastName,
                      rs.m_Email);

            TRACE(_T("%s\n"), msg);

            rs.MoveNext();
        }

        rs.Close();
    }
    catch (CDBException* e)
    {
        AfxMessageBox(e->m_strError);
        e->Delete();
    }

    db.Close();
}
```

### Dynamic Recordsets

```cpp
// Without derived class
void QueryDynamic(CDatabase* pDB, LPCTSTR lpszSQL)
{
    CRecordset rs(pDB);

    try
    {
        rs.Open(CRecordset::forwardOnly, lpszSQL, CRecordset::readOnly);

        short nFields = rs.GetODBCFieldCount();

        while (!rs.IsEOF())
        {
            CString row;

            for (short i = 0; i < nFields; i++)
            {
                CDBVariant var;
                rs.GetFieldValue(i, var);

                CString value;
                switch (var.m_dwType)
                {
                case DBVT_STRING:
                    value = *var.m_pstring;
                    break;

                case DBVT_LONG:
                    value.Format(_T("%ld"), var.m_lVal);
                    break;

                case DBVT_DOUBLE:
                    value.Format(_T("%.2f"), var.m_dblVal);
                    break;

                case DBVT_DATE:
                    value = var.m_pdate->Format(_T("%Y-%m-%d"));
                    break;

                case DBVT_NULL:
                    value = _T("NULL");
                    break;
                }

                row += value + _T("\t");
            }

            TRACE(_T("%s\n"), row);
            rs.MoveNext();
        }

        rs.Close();
    }
    catch (CDBException* e)
    {
        AfxMessageBox(e->m_strError);
        e->Delete();
    }
}
```

---

## 5. SQL Queries and Operations

### CRUD Operations

```cpp
// Create
BOOL AddCustomer(CDatabase* pDB, const CString& firstName,
                 const CString& lastName, const CString& email)
{
    try
    {
        CString strSQL;
        strSQL.Format(
            _T("INSERT INTO Customers (FirstName, LastName, Email, CreatedDate) ")
            _T("VALUES ('%s', '%s', '%s', GETDATE())"),
            firstName, lastName, email
        );

        pDB->ExecuteSQL(strSQL);
        return TRUE;
    }
    catch (CDBException* e)
    {
        AfxMessageBox(e->m_strError);
        e->Delete();
        return FALSE;
    }
}

// Read
BOOL GetCustomer(CDatabase* pDB, long customerID, CString& firstName,
                 CString& lastName, CString& email)
{
    CString strSQL;
    strSQL.Format(
        _T("SELECT FirstName, LastName, Email FROM Customers WHERE CustomerID = %ld"),
        customerID
    );

    CRecordset rs(pDB);

    try
    {
        rs.Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly);

        if (!rs.IsEOF())
        {
            rs.GetFieldValue(_T("FirstName"), firstName);
            rs.GetFieldValue(_T("LastName"), lastName);
            rs.GetFieldValue(_T("Email"), email);
            rs.Close();
            return TRUE;
        }

        rs.Close();
        return FALSE;
    }
    catch (CDBException* e)
    {
        AfxMessageBox(e->m_strError);
        e->Delete();
        return FALSE;
    }
}

// Update
BOOL UpdateCustomer(CDatabase* pDB, long customerID,
                   const CString& firstName, const CString& lastName,
                   const CString& email)
{
    try
    {
        CString strSQL;
        strSQL.Format(
            _T("UPDATE Customers SET FirstName='%s', LastName='%s', Email='%s' ")
            _T("WHERE CustomerID=%ld"),
            firstName, lastName, email, customerID
        );

        pDB->ExecuteSQL(strSQL);
        return TRUE;
    }
    catch (CDBException* e)
    {
        AfxMessageBox(e->m_strError);
        e->Delete();
        return FALSE;
    }
}

// Delete
BOOL DeleteCustomer(CDatabase* pDB, long customerID)
{
    try
    {
        CString strSQL;
        strSQL.Format(_T("DELETE FROM Customers WHERE CustomerID=%ld"), customerID);

        pDB->ExecuteSQL(strSQL);
        return TRUE;
    }
    catch (CDBException* e)
    {
        AfxMessageBox(e->m_strError);
        e->Delete();
        return FALSE;
    }
}
```

### Parameterized Queries

```cpp
class CCustomerParamSet : public CRecordset
{
public:
    CCustomerParamSet(CDatabase* pdb = NULL);

    // Parameters
    CString m_strLastNameParam;

    // Fields
    long m_CustomerID;
    CString m_FirstName;
    CString m_LastName;
    CString m_Email;

    virtual CString GetDefaultSQL();
    virtual void DoFieldExchange(CFieldExchange* pFX);

    DECLARE_DYNAMIC(CCustomerParamSet)
};

IMPLEMENT_DYNAMIC(CCustomerParamSet, CRecordset)

CCustomerParamSet::CCustomerParamSet(CDatabase* pdb)
    : CRecordset(pdb)
{
    m_CustomerID = 0;
    m_nFields = 4;
    m_nParams = 1;
}

CString CCustomerParamSet::GetDefaultSQL()
{
    return _T("SELECT CustomerID, FirstName, LastName, Email ")
           _T("FROM Customers WHERE LastName = ?");
}

void CCustomerParamSet::DoFieldExchange(CFieldExchange* pFX)
{
    pFX->SetFieldType(CFieldExchange::outputColumn);
    RFX_Long(pFX, _T("CustomerID"), m_CustomerID);
    RFX_Text(pFX, _T("FirstName"), m_FirstName);
    RFX_Text(pFX, _T("LastName"), m_LastName);
    RFX_Text(pFX, _T("Email"), m_Email);

    pFX->SetFieldType(CFieldExchange::param);
    RFX_Text(pFX, _T("LastNameParam"), m_strLastNameParam);
}

// Usage
void FindCustomersByLastName(CDatabase* pDB, const CString& lastName)
{
    CCustomerParamSet rs(pDB);
    rs.m_strLastNameParam = lastName;

    try
    {
        rs.Open();

        while (!rs.IsEOF())
        {
            CString msg;
            msg.Format(_T("%s %s - %s"),
                      rs.m_FirstName, rs.m_LastName, rs.m_Email);
            TRACE(_T("%s\n"), msg);

            rs.MoveNext();
        }

        rs.Close();
    }
    catch (CDBException* e)
    {
        AfxMessageBox(e->m_strError);
        e->Delete();
    }
}
```

---

## 6. Transactions

### Transaction Management

```cpp
class CTransactionManager
{
public:
    CTransactionManager(CDatabase* pDB);
    ~CTransactionManager();

    BOOL Begin();
    BOOL Commit();
    BOOL Rollback();

private:
    CDatabase* m_pDB;
    BOOL m_bTransactionActive;
};

CTransactionManager::CTransactionManager(CDatabase* pDB)
    : m_pDB(pDB), m_bTransactionActive(FALSE)
{
}

CTransactionManager::~CTransactionManager()
{
    if (m_bTransactionActive)
    {
        Rollback();
    }
}

BOOL CTransactionManager::Begin()
{
    if (m_bTransactionActive)
        return FALSE;

    m_bTransactionActive = m_pDB->BeginTrans();
    return m_bTransactionActive;
}

BOOL CTransactionManager::Commit()
{
    if (!m_bTransactionActive)
        return FALSE;

    BOOL bSuccess = m_pDB->CommitTrans();
    m_bTransactionActive = FALSE;
    return bSuccess;
}

BOOL CTransactionManager::Rollback()
{
    if (!m_bTransactionActive)
        return FALSE;

    BOOL bSuccess = m_pDB->Rollback();
    m_bTransactionActive = FALSE;
    return bSuccess;
}

// Usage example
BOOL TransferFunds(CDatabase* pDB, long fromAccount, long toAccount, double amount)
{
    CTransactionManager trans(pDB);

    if (!trans.Begin())
    {
        AfxMessageBox(_T("Failed to begin transaction"));
        return FALSE;
    }

    try
    {
        // Debit from account
        CString strSQL;
        strSQL.Format(
            _T("UPDATE Accounts SET Balance = Balance - %.2f WHERE AccountID = %ld"),
            amount, fromAccount
        );
        pDB->ExecuteSQL(strSQL);

        // Credit to account
        strSQL.Format(
            _T("UPDATE Accounts SET Balance = Balance + %.2f WHERE AccountID = %ld"),
            amount, toAccount
        );
        pDB->ExecuteSQL(strSQL);

        // Commit transaction
        if (trans.Commit())
        {
            AfxMessageBox(_T("Transfer successful"));
            return TRUE;
        }
        else
        {
            AfxMessageBox(_T("Failed to commit transaction"));
            return FALSE;
        }
    }
    catch (CDBException* e)
    {
        trans.Rollback();
        AfxMessageBox(_T("Transaction failed: ") + e->m_strError);
        e->Delete();
        return FALSE;
    }
}
```

---

## 7. Data Binding to Controls

### List Control Binding

```cpp
class CCustomerListDlg : public CDialogEx
{
public:
    CCustomerListDlg(CWnd* pParent = NULL);

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg void OnBtnRefresh();
    afx_msg void OnBtnAdd();
    afx_msg void OnBtnEdit();
    afx_msg void OnBtnDelete();

    DECLARE_MESSAGE_MAP()

private:
    void LoadCustomers();

    CListCtrl m_listCustomers;
    CDatabase m_database;
};

BOOL CCustomerListDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Setup list control
    m_listCustomers.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    m_listCustomers.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 50);
    m_listCustomers.InsertColumn(1, _T("First Name"), LVCFMT_LEFT, 100);
    m_listCustomers.InsertColumn(2, _T("Last Name"), LVCFMT_LEFT, 100);
    m_listCustomers.InsertColumn(3, _T("Email"), LVCFMT_LEFT, 200);

    // Connect to database
    if (m_database.OpenEx(_T("DSN=MyAppDB"), CDatabase::noOdbcDialog))
    {
        LoadCustomers();
    }

    return TRUE;
}

void CCustomerListDlg::LoadCustomers()
{
    m_listCustomers.DeleteAllItems();

    CCustomerSet rs(&m_database);

    try
    {
        rs.Open();

        int nItem = 0;
        while (!rs.IsEOF())
        {
            CString strID;
            strID.Format(_T("%ld"), rs.m_CustomerID);

            int nIndex = m_listCustomers.InsertItem(nItem, strID);
            m_listCustomers.SetItemText(nIndex, 1, rs.m_FirstName);
            m_listCustomers.SetItemText(nIndex, 2, rs.m_LastName);
            m_listCustomers.SetItemText(nIndex, 3, rs.m_Email);

            m_listCustomers.SetItemData(nIndex, rs.m_CustomerID);

            rs.MoveNext();
            nItem++;
        }

        rs.Close();
    }
    catch (CDBException* e)
    {
        AfxMessageBox(e->m_strError);
        e->Delete();
    }
}
```

---

## 8. Error Handling

```cpp
void HandleDatabaseError(CDBException* e)
{
    CString strError;
    strError.Format(
        _T("Database Error:\n\n")
        _T("Error: %s\n")
        _T("Error Code: %ld\n")
        _T("Native Error: %ld\n")
        _T("ODBC State: %s"),
        e->m_strError,
        e->m_nRetCode,
        e->m_lNativeError,
        e->m_strStateNativeOrigin
    );

    AfxMessageBox(strError);
    e->Delete();
}

// Usage
try
{
    // Database operations
}
catch (CDBException* e)
{
    HandleDatabaseError(e);
}
```

---

## 9. Best Practices

```cpp
/*
ODBC BEST PRACTICES:

1. CONNECTION MANAGEMENT
   - Open once, use many times
   - Close when done
   - Use connection pooling
   - Handle connection failures

2. SQL QUERIES
   - Use parameterized queries
   - Avoid SQL injection
   - Optimize queries
   - Use indexes

3. TRANSACTIONS
   - Keep transactions short
   - Handle errors properly
   - Always commit or rollback
   - Don't nest transactions

4. PERFORMANCE
   - Use appropriate recordset types
   - Fetch only needed columns
   - Limit result sets
   - Use stored procedures

5. ERROR HANDLING
   - Catch CDBException
   - Log errors
   - Provide user feedback
   - Graceful degradation

6. SECURITY
   - Never store passwords in plain text
   - Use Windows Authentication when possible
   - Validate all user input
   - Use least privilege principle
*/
```

---

## Key Takeaways

1. **ODBC** provides database-independent access
2. **CDatabase** manages connections
3. **CRecordset** represents query results
4. **DoFieldExchange** binds data
5. **Transactions** ensure data integrity
6. **Parameterized queries** prevent SQL injection
7. **Error handling** is critical
8. **Connection management** affects performance
9. **Data binding** simplifies UI
10. **Best practices** ensure robust applications

---

## Next Lesson Preview

In **Lesson 69: ADO Database Access**, we'll explore:
- ADO architecture
- Connection strings
- Recordsets and commands
- ADO vs ODBC
- Best practices

**Master database programming for data-driven applications!**
