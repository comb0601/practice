# Lesson 64: DLLs with MFC (Regular DLL)

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to DLLs
2. Types of MFC DLLs
3. Creating a Regular DLL
4. Exporting Functions
5. Exporting Classes
6. Resource Management
7. DLL Initialization and Cleanup
8. Static vs Dynamic Linking
9. Version Control and Compatibility
10. Advanced Techniques
11. Best Practices
12. Exercises and Projects

---

## 1. Introduction to DLLs

### What is a DLL?

A **Dynamic Link Library (DLL)** is a Windows library containing functions, classes, and resources that can be shared by multiple applications simultaneously.

```cpp
/*
BENEFITS OF DLLs:

1. Code Reusability
   - Share code across multiple applications
   - Reduce application size
   - Centralized updates

2. Memory Efficiency
   - Single copy in memory
   - Shared by multiple processes
   - Reduced memory footprint

3. Modularity
   - Separate functionality into components
   - Easier maintenance
   - Independent versioning

4. Resource Sharing
   - Share dialogs, strings, icons
   - Centralized resources
   - Consistent UI

5. Plugin Architecture
   - Load/unload at runtime
   - Extensible applications
   - Third-party extensions
*/
```

---

## 2. Types of MFC DLLs

### Regular DLL vs Extension DLL

```cpp
/*
REGULAR DLL (Also called "Regular MFC DLL"):
- Can be called from any Windows application
- Uses MFC internally
- Exports C-style functions or C++ classes
- Can use MFC statically or dynamically
- Has its own MFC module state

EXTENSION DLL:
- Can only be called from MFC applications
- Exports MFC-derived classes
- Must use MFC dynamically
- Shares MFC module state with calling application
- Smaller and more efficient for MFC-to-MFC calls

CHOICE:
- Use Regular DLL for general-purpose libraries
- Use Extension DLL for MFC-specific functionality
*/
```

### Static vs Dynamic MFC Linking

```cpp
/*
STATIC LINKING:
- MFC code embedded in DLL
- Larger DLL size
- No external MFC DLL dependencies
- Independent of MFC version
- Each DLL has own copy of MFC

DYNAMIC LINKING:
- Requires MFC DLLs (mfc140.dll, etc.)
- Smaller DLL size
- Shared MFC runtime
- MFC version dependency
- More efficient memory usage
*/
```

---

## 3. Creating a Regular DLL

### Project Setup

```cpp
// Visual Studio:
// 1. File -> New -> Project
// 2. MFC DLL
// 3. Select "Regular DLL using shared MFC DLL" or "Regular DLL with MFC statically linked"
// 4. Project created with:
//    - MyDLL.h      - Main header
//    - MyDLL.cpp    - DLL initialization
//    - stdafx.h     - Precompiled headers
//    - MyDLL.def    - Export definitions (optional)
```

### Basic DLL Structure

```cpp
// MyMathDLL.h
#pragma once

#ifdef MYMATHDLL_EXPORTS
#define MYMATH_API __declspec(dllexport)
#else
#define MYMATH_API __declspec(dllimport)
#endif

// Exported functions
extern "C" MYMATH_API int Add(int a, int b);
extern "C" MYMATH_API int Subtract(int a, int b);
extern "C" MYMATH_API double Multiply(double a, double b);
extern "C" MYMATH_API double Divide(double a, double b);

// MyMathDLL.cpp
#include "stdafx.h"
#include "MyMathDLL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object
static CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(NULL);

    if (hModule != NULL)
    {
        // Initialize MFC
        if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
        {
            _tprintf(_T("Fatal Error: MFC initialization failed\n"));
            nRetCode = 1;
        }
    }
    else
    {
        _tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
        nRetCode = 1;
    }

    return nRetCode;
}

// Exported function implementations
MYMATH_API int Add(int a, int b)
{
    return a + b;
}

MYMATH_API int Subtract(int a, int b)
{
    return a - b;
}

MYMATH_API double Multiply(double a, double b)
{
    return a * b;
}

MYMATH_API double Divide(double a, double b)
{
    if (b == 0.0)
    {
        AfxThrowInvalidArgException();
    }
    return a / b;
}
```

### DLL Module Definition

```cpp
// MyMathDLL.def (Optional - alternative to __declspec)
LIBRARY "MyMathDLL"

EXPORTS
    Add         @1
    Subtract    @2
    Multiply    @3
    Divide      @4
```

---

## 4. Exporting Functions

### C-Style Function Export

```cpp
// StringUtilsDLL.h
#pragma once

#ifdef STRINGUTILSDLL_EXPORTS
#define STRINGUTILS_API __declspec(dllexport)
#else
#define STRINGUTILS_API __declspec(dllimport)
#endif

extern "C"
{
    // String manipulation functions
    STRINGUTILS_API BOOL ReverseString(
        LPCTSTR lpszInput,
        LPTSTR lpszOutput,
        int nBufferSize
    );

    STRINGUTILS_API BOOL ToUpperCase(
        LPCTSTR lpszInput,
        LPTSTR lpszOutput,
        int nBufferSize
    );

    STRINGUTILS_API BOOL ToLowerCase(
        LPCTSTR lpszInput,
        LPTSTR lpszOutput,
        int nBufferSize
    );

    STRINGUTILS_API int CountWords(LPCTSTR lpszInput);

    STRINGUTILS_API BOOL RemoveSpaces(
        LPCTSTR lpszInput,
        LPTSTR lpszOutput,
        int nBufferSize
    );
}

// StringUtilsDLL.cpp
#include "stdafx.h"
#include "StringUtilsDLL.h"

STRINGUTILS_API BOOL ReverseString(
    LPCTSTR lpszInput,
    LPTSTR lpszOutput,
    int nBufferSize)
{
    if (!lpszInput || !lpszOutput || nBufferSize <= 0)
        return FALSE;

    int len = _tcslen(lpszInput);
    if (len >= nBufferSize)
        return FALSE;

    for (int i = 0; i < len; i++)
    {
        lpszOutput[i] = lpszInput[len - 1 - i];
    }
    lpszOutput[len] = _T('\0');

    return TRUE;
}

STRINGUTILS_API BOOL ToUpperCase(
    LPCTSTR lpszInput,
    LPTSTR lpszOutput,
    int nBufferSize)
{
    if (!lpszInput || !lpszOutput || nBufferSize <= 0)
        return FALSE;

    _tcsncpy_s(lpszOutput, nBufferSize, lpszInput, _TRUNCATE);
    _tcsupr_s(lpszOutput, nBufferSize);

    return TRUE;
}

STRINGUTILS_API BOOL ToLowerCase(
    LPCTSTR lpszInput,
    LPTSTR lpszOutput,
    int nBufferSize)
{
    if (!lpszInput || !lpszOutput || nBufferSize <= 0)
        return FALSE;

    _tcsncpy_s(lpszOutput, nBufferSize, lpszInput, _TRUNCATE);
    _tcslwr_s(lpszOutput, nBufferSize);

    return TRUE;
}

STRINGUTILS_API int CountWords(LPCTSTR lpszInput)
{
    if (!lpszInput)
        return 0;

    int count = 0;
    BOOL inWord = FALSE;

    while (*lpszInput)
    {
        if (_istspace(*lpszInput))
        {
            inWord = FALSE;
        }
        else if (!inWord)
        {
            inWord = TRUE;
            count++;
        }
        lpszInput++;
    }

    return count;
}

STRINGUTILS_API BOOL RemoveSpaces(
    LPCTSTR lpszInput,
    LPTSTR lpszOutput,
    int nBufferSize)
{
    if (!lpszInput || !lpszOutput || nBufferSize <= 0)
        return FALSE;

    int j = 0;
    for (int i = 0; lpszInput[i] && j < nBufferSize - 1; i++)
    {
        if (!_istspace(lpszInput[i]))
        {
            lpszOutput[j++] = lpszInput[i];
        }
    }
    lpszOutput[j] = _T('\0');

    return TRUE;
}
```

### Complex Return Types

```cpp
// DataStructuresDLL.h
#pragma once

#ifdef DATASTRUCTURESDLL_EXPORTS
#define DATASTRUCTURES_API __declspec(dllexport)
#else
#define DATASTRUCTURES_API __declspec(dllimport)
#endif

// Structure for returning multiple values
struct DATASTRUCTURES_API PersonInfo
{
    TCHAR szName[100];
    int nAge;
    double dSalary;
    BOOL bIsActive;
};

// Array handling
extern "C"
{
    DATASTRUCTURES_API BOOL GetPersonInfo(
        int nId,
        PersonInfo* pInfo
    );

    DATASTRUCTURES_API int GetAllPersons(
        PersonInfo* pArray,
        int nMaxCount
    );

    DATASTRUCTURES_API void FreePersonArray(PersonInfo* pArray);

    // String allocation
    DATASTRUCTURES_API LPTSTR AllocateString(LPCTSTR lpszSource);
    DATASTRUCTURES_API void FreeString(LPTSTR lpszString);
}

// DataStructuresDLL.cpp
#include "stdafx.h"
#include "DataStructuresDLL.h"

// Sample database
static PersonInfo g_persons[] =
{
    { _T("John Doe"), 30, 50000.0, TRUE },
    { _T("Jane Smith"), 25, 55000.0, TRUE },
    { _T("Bob Johnson"), 35, 60000.0, FALSE },
};

DATASTRUCTURES_API BOOL GetPersonInfo(int nId, PersonInfo* pInfo)
{
    if (!pInfo || nId < 0 || nId >= _countof(g_persons))
        return FALSE;

    *pInfo = g_persons[nId];
    return TRUE;
}

DATASTRUCTURES_API int GetAllPersons(
    PersonInfo* pArray,
    int nMaxCount)
{
    if (!pArray || nMaxCount <= 0)
        return 0;

    int count = min(nMaxCount, _countof(g_persons));
    memcpy(pArray, g_persons, count * sizeof(PersonInfo));

    return count;
}

DATASTRUCTURES_API void FreePersonArray(PersonInfo* pArray)
{
    if (pArray)
    {
        delete[] pArray;
    }
}

DATASTRUCTURES_API LPTSTR AllocateString(LPCTSTR lpszSource)
{
    if (!lpszSource)
        return NULL;

    int len = _tcslen(lpszSource) + 1;
    LPTSTR lpszCopy = new TCHAR[len];
    _tcscpy_s(lpszCopy, len, lpszSource);

    return lpszCopy;
}

DATASTRUCTURES_API void FreeString(LPTSTR lpszString)
{
    if (lpszString)
    {
        delete[] lpszString;
    }
}
```

---

## 5. Exporting Classes

### Class Export

```cpp
// CalculatorDLL.h
#pragma once

#ifdef CALCULATORDLL_EXPORTS
#define CALCULATOR_API __declspec(dllexport)
#else
#define CALCULATOR_API __declspec(dllimport)
#endif

// Exported class
class CALCULATOR_API CCalculator
{
public:
    CCalculator();
    virtual ~CCalculator();

    // Basic operations
    double Add(double a, double b);
    double Subtract(double a, double b);
    double Multiply(double a, double b);
    double Divide(double a, double b);

    // Advanced operations
    double Power(double base, double exponent);
    double SquareRoot(double value);
    double Factorial(int n);

    // Memory operations
    void MemoryClear();
    void MemoryStore(double value);
    double MemoryRecall();
    void MemoryAdd(double value);

    // History
    int GetHistoryCount();
    BOOL GetHistoryItem(int index, CString& expression, double& result);
    void ClearHistory();

private:
    double m_memory;
    struct HistoryItem
    {
        CString expression;
        double result;
    };
    CArray<HistoryItem, HistoryItem&> m_history;

    void AddToHistory(const CString& expr, double result);
};

// Factory functions for C-style creation
extern "C"
{
    CALCULATOR_API CCalculator* CreateCalculator();
    CALCULATOR_API void DestroyCalculator(CCalculator* pCalc);
}

// CalculatorDLL.cpp
#include "stdafx.h"
#include "CalculatorDLL.h"
#include <math.h>

CCalculator::CCalculator() : m_memory(0.0)
{
}

CCalculator::~CCalculator()
{
    m_history.RemoveAll();
}

double CCalculator::Add(double a, double b)
{
    double result = a + b;
    CString expr;
    expr.Format(_T("%.2f + %.2f"), a, b);
    AddToHistory(expr, result);
    return result;
}

double CCalculator::Subtract(double a, double b)
{
    double result = a - b;
    CString expr;
    expr.Format(_T("%.2f - %.2f"), a, b);
    AddToHistory(expr, result);
    return result;
}

double CCalculator::Multiply(double a, double b)
{
    double result = a * b;
    CString expr;
    expr.Format(_T("%.2f * %.2f"), a, b);
    AddToHistory(expr, result);
    return result;
}

double CCalculator::Divide(double a, double b)
{
    if (b == 0.0)
    {
        AfxThrowInvalidArgException();
    }

    double result = a / b;
    CString expr;
    expr.Format(_T("%.2f / %.2f"), a, b);
    AddToHistory(expr, result);
    return result;
}

double CCalculator::Power(double base, double exponent)
{
    double result = pow(base, exponent);
    CString expr;
    expr.Format(_T("%.2f ^ %.2f"), base, exponent);
    AddToHistory(expr, result);
    return result;
}

double CCalculator::SquareRoot(double value)
{
    if (value < 0.0)
    {
        AfxThrowInvalidArgException();
    }

    double result = sqrt(value);
    CString expr;
    expr.Format(_T("sqrt(%.2f)"), value);
    AddToHistory(expr, result);
    return result;
}

double CCalculator::Factorial(int n)
{
    if (n < 0)
    {
        AfxThrowInvalidArgException();
    }

    double result = 1.0;
    for (int i = 2; i <= n; i++)
    {
        result *= i;
    }

    CString expr;
    expr.Format(_T("%d!"), n);
    AddToHistory(expr, result);
    return result;
}

void CCalculator::MemoryClear()
{
    m_memory = 0.0;
}

void CCalculator::MemoryStore(double value)
{
    m_memory = value;
}

double CCalculator::MemoryRecall()
{
    return m_memory;
}

void CCalculator::MemoryAdd(double value)
{
    m_memory += value;
}

int CCalculator::GetHistoryCount()
{
    return m_history.GetCount();
}

BOOL CCalculator::GetHistoryItem(int index, CString& expression, double& result)
{
    if (index < 0 || index >= m_history.GetCount())
        return FALSE;

    expression = m_history[index].expression;
    result = m_history[index].result;
    return TRUE;
}

void CCalculator::ClearHistory()
{
    m_history.RemoveAll();
}

void CCalculator::AddToHistory(const CString& expr, double result)
{
    HistoryItem item;
    item.expression = expr;
    item.result = result;
    m_history.Add(item);

    // Keep only last 100 items
    if (m_history.GetCount() > 100)
    {
        m_history.RemoveAt(0);
    }
}

// Factory functions
CALCULATOR_API CCalculator* CreateCalculator()
{
    return new CCalculator();
}

CALCULATOR_API void DestroyCalculator(CCalculator* pCalc)
{
    if (pCalc)
    {
        delete pCalc;
    }
}
```

### Abstract Interface Export

```cpp
// InterfaceDLL.h
#pragma once

#ifdef INTERFACEDLL_EXPORTS
#define INTERFACE_API __declspec(dllexport)
#else
#define INTERFACE_API __declspec(dllimport)
#endif

// Pure virtual interface
class INTERFACE_API ILogger
{
public:
    virtual ~ILogger() {}

    virtual void LogInfo(LPCTSTR lpszMessage) = 0;
    virtual void LogWarning(LPCTSTR lpszMessage) = 0;
    virtual void LogError(LPCTSTR lpszMessage) = 0;
    virtual void SetLogLevel(int level) = 0;
    virtual BOOL SaveToFile(LPCTSTR lpszFilePath) = 0;
    virtual void Clear() = 0;
};

// Factory function
extern "C"
{
    INTERFACE_API ILogger* CreateLogger();
    INTERFACE_API void DestroyLogger(ILogger* pLogger);
}

// InterfaceDLL.cpp
#include "stdafx.h"
#include "InterfaceDLL.h"

// Concrete implementation (not exported)
class CFileLogger : public ILogger
{
public:
    CFileLogger() : m_logLevel(0) {}

    virtual void LogInfo(LPCTSTR lpszMessage)
    {
        if (m_logLevel <= 0)
        {
            CString entry;
            entry.Format(_T("[INFO] %s"), lpszMessage);
            m_entries.Add(entry);
        }
    }

    virtual void LogWarning(LPCTSTR lpszMessage)
    {
        if (m_logLevel <= 1)
        {
            CString entry;
            entry.Format(_T("[WARNING] %s"), lpszMessage);
            m_entries.Add(entry);
        }
    }

    virtual void LogError(LPCTSTR lpszMessage)
    {
        if (m_logLevel <= 2)
        {
            CString entry;
            entry.Format(_T("[ERROR] %s"), lpszMessage);
            m_entries.Add(entry);
        }
    }

    virtual void SetLogLevel(int level)
    {
        m_logLevel = level;
    }

    virtual BOOL SaveToFile(LPCTSTR lpszFilePath)
    {
        try
        {
            CStdioFile file;
            if (!file.Open(lpszFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
                return FALSE;

            for (int i = 0; i < m_entries.GetCount(); i++)
            {
                file.WriteString(m_entries[i] + _T("\n"));
            }

            file.Close();
            return TRUE;
        }
        catch (CFileException* e)
        {
            e->Delete();
            return FALSE;
        }
    }

    virtual void Clear()
    {
        m_entries.RemoveAll();
    }

private:
    CStringArray m_entries;
    int m_logLevel;
};

INTERFACE_API ILogger* CreateLogger()
{
    return new CFileLogger();
}

INTERFACE_API void DestroyLogger(ILogger* pLogger)
{
    if (pLogger)
    {
        delete pLogger;
    }
}
```

---

## 6. Resource Management

### Sharing Resources from DLL

```cpp
// ResourceDLL.h
#pragma once

#ifdef RESOURCEDLL_EXPORTS
#define RESOURCE_API __declspec(dllexport)
#else
#define RESOURCE_API __declspec(dllimport)
#endif

// Resource IDs
#define IDD_SETTINGS_DIALOG     1000
#define IDI_MAIN_ICON          1001
#define IDS_APP_TITLE          1002
#define IDS_ERROR_MESSAGE      1003

extern "C"
{
    RESOURCE_API HINSTANCE GetResourceInstance();
    RESOURCE_API BOOL LoadDialogResource(HWND hWndParent, UINT nID);
    RESOURCE_API HICON LoadIconResource(UINT nID);
    RESOURCE_API BOOL LoadStringResource(UINT nID, CString& str);
}

// ResourceDLL.cpp
#include "stdafx.h"
#include "ResourceDLL.h"
#include "resource.h"

// Global module handle
static AFX_EXTENSION_MODULE ResourceDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        TRACE0("ResourceDLL.DLL Initializing!\n");

        // Extension DLL one-time initialization
        if (!AfxInitExtensionModule(ResourceDLL, hInstance))
            return 0;

        // Insert this DLL into the resource chain
        new CDynLinkLibrary(ResourceDLL);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        TRACE0("ResourceDLL.DLL Terminating!\n");
        AfxTermExtensionModule(ResourceDLL);
    }

    return 1;
}

RESOURCE_API HINSTANCE GetResourceInstance()
{
    return ResourceDLL.hModule;
}

RESOURCE_API BOOL LoadDialogResource(HWND hWndParent, UINT nID)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CWnd* pParent = CWnd::FromHandle(hWndParent);
    CDialog dlg(nID, pParent);

    return (dlg.DoModal() == IDOK);
}

RESOURCE_API HICON LoadIconResource(UINT nID)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return AfxGetApp()->LoadIcon(nID);
}

RESOURCE_API BOOL LoadStringResource(UINT nID, CString& str)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return str.LoadString(nID);
}
```

### Module State Management

```cpp
// Proper module state switching
class CMyDLLClass
{
public:
    void ShowDialog()
    {
        // Switch to DLL's module state
        AFX_MANAGE_STATE(AfxGetStaticModuleState());

        CDialog dlg(IDD_MY_DIALOG);
        dlg.DoModal();

        // Automatically restored when AFX_MANAGE_STATE goes out of scope
    }

    HICON GetIcon(UINT nID)
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        return AfxGetApp()->LoadIcon(nID);
    }

    CString GetString(UINT nID)
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        CString str;
        str.LoadString(nID);
        return str;
    }
};
```

---

## 7. DLL Initialization and Cleanup

### DllMain Implementation

```cpp
// MyDLL.cpp
#include "stdafx.h"
#include "MyDLL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE MyDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        TRACE0("MyDLL.DLL: DLL_PROCESS_ATTACH\n");

        // Initialize MFC extension module
        if (!AfxInitExtensionModule(MyDLL, hInstance))
            return 0;

        // Insert into resource chain
        new CDynLinkLibrary(MyDLL);

        // Custom initialization
        if (!InitializeDLL())
        {
            TRACE0("MyDLL: Initialization failed\n");
            return 0;
        }

        break;

    case DLL_PROCESS_DETACH:
        TRACE0("MyDLL.DLL: DLL_PROCESS_DETACH\n");

        // Custom cleanup
        CleanupDLL();

        // Terminate MFC extension module
        AfxTermExtensionModule(MyDLL);

        break;

    case DLL_THREAD_ATTACH:
        TRACE0("MyDLL.DLL: DLL_THREAD_ATTACH\n");
        // Per-thread initialization
        break;

    case DLL_THREAD_DETACH:
        TRACE0("MyDLL.DLL: DLL_THREAD_DETACH\n");
        // Per-thread cleanup
        break;
    }

    return 1;
}

BOOL InitializeDLL()
{
    // Initialize global resources
    // Load configuration
    // Initialize databases, etc.

    return TRUE;
}

void CleanupDLL()
{
    // Free global resources
    // Save configuration
    // Close databases, etc.
}
```

### Thread-Local Storage

```cpp
// ThreadLocalStorage.h
class CThreadLocalData
{
public:
    static BOOL Initialize();
    static void Cleanup();
    static CThreadLocalData* GetThreadData();

private:
    CThreadLocalData();
    ~CThreadLocalData();

    static DWORD s_tlsIndex;

    CString m_threadName;
    int m_threadCounter;
    void* m_pUserData;
};

// ThreadLocalStorage.cpp
DWORD CThreadLocalData::s_tlsIndex = TLS_OUT_OF_INDEXES;

BOOL CThreadLocalData::Initialize()
{
    s_tlsIndex = TlsAlloc();
    return (s_tlsIndex != TLS_OUT_OF_INDEXES);
}

void CThreadLocalData::Cleanup()
{
    if (s_tlsIndex != TLS_OUT_OF_INDEXES)
    {
        TlsFree(s_tlsIndex);
        s_tlsIndex = TLS_OUT_OF_INDEXES;
    }
}

CThreadLocalData* CThreadLocalData::GetThreadData()
{
    if (s_tlsIndex == TLS_OUT_OF_INDEXES)
        return NULL;

    CThreadLocalData* pData = (CThreadLocalData*)TlsGetValue(s_tlsIndex);

    if (!pData)
    {
        pData = new CThreadLocalData();
        TlsSetValue(s_tlsIndex, pData);
    }

    return pData;
}

CThreadLocalData::CThreadLocalData()
    : m_threadCounter(0), m_pUserData(NULL)
{
    m_threadName.Format(_T("Thread_%d"), GetCurrentThreadId());
}

CThreadLocalData::~CThreadLocalData()
{
    if (m_pUserData)
    {
        // Clean up user data
    }
}

// In DllMain
case DLL_PROCESS_ATTACH:
    CThreadLocalData::Initialize();
    break;

case DLL_THREAD_DETACH:
    {
        CThreadLocalData* pData = CThreadLocalData::GetThreadData();
        if (pData)
        {
            delete pData;
            TlsSetValue(CThreadLocalData::s_tlsIndex, NULL);
        }
    }
    break;

case DLL_PROCESS_DETACH:
    CThreadLocalData::Cleanup();
    break;
```

---

## 8. Static vs Dynamic Linking

### Loading DLL Dynamically

```cpp
// DynamicDLLLoader.h
class CDynamicDLLLoader
{
public:
    CDynamicDLLLoader();
    ~CDynamicDLLLoader();

    BOOL LoadDLL(LPCTSTR lpszDLLPath);
    void UnloadDLL();
    BOOL IsLoaded() const { return m_hModule != NULL; }

    // Get function pointer
    template<typename TFunc>
    TFunc GetFunction(LPCSTR lpszFuncName)
    {
        if (!m_hModule)
            return NULL;

        return (TFunc)GetProcAddress(m_hModule, lpszFuncName);
    }

private:
    HMODULE m_hModule;
};

// DynamicDLLLoader.cpp
CDynamicDLLLoader::CDynamicDLLLoader() : m_hModule(NULL)
{
}

CDynamicDLLLoader::~CDynamicDLLLoader()
{
    UnloadDLL();
}

BOOL CDynamicDLLLoader::LoadDLL(LPCTSTR lpszDLLPath)
{
    if (m_hModule)
        UnloadDLL();

    m_hModule = LoadLibrary(lpszDLLPath);
    return (m_hModule != NULL);
}

void CDynamicDLLLoader::UnloadDLL()
{
    if (m_hModule)
    {
        FreeLibrary(m_hModule);
        m_hModule = NULL;
    }
}

// Usage
void CMyApp::UseDynamicDLL()
{
    CDynamicDLLLoader loader;

    if (!loader.LoadDLL(_T("MyMathDLL.dll")))
    {
        AfxMessageBox(_T("Failed to load DLL"));
        return;
    }

    // Get function pointers
    typedef int (*PFN_ADD)(int, int);
    typedef int (*PFN_MULTIPLY)(double, double);

    PFN_ADD pfnAdd = loader.GetFunction<PFN_ADD>("Add");
    PFN_MULTIPLY pfnMultiply = loader.GetFunction<PFN_MULTIPLY>("Multiply");

    if (pfnAdd && pfnMultiply)
    {
        int sum = pfnAdd(5, 3);
        double product = pfnMultiply(4.5, 2.0);

        CString msg;
        msg.Format(_T("5 + 3 = %d\n4.5 * 2.0 = %.2f"), sum, product);
        AfxMessageBox(msg);
    }
}
```

---

## Key Takeaways

1. **Regular DLLs** can be called from any Windows application
2. **Export functions** using __declspec(dllexport)
3. **Factory functions** provide safer class instantiation
4. **Module state** must be managed correctly
5. **Resource management** requires AFX_MANAGE_STATE
6. **DllMain** handles initialization and cleanup
7. **Static linking** embeds MFC in DLL
8. **Dynamic linking** shares MFC runtime
9. **Thread-local storage** manages per-thread data
10. **Dynamic loading** provides runtime flexibility

---

## Next Lesson Preview

In **Lesson 65: MFC Extension DLLs**, we'll explore:
- Extension DLL architecture
- Exporting MFC-derived classes
- Sharing MFC state
- Resource sharing
- Best practices for extension DLLs

**Master DLL development for reusable components!**
