# Lesson 67: COM and MFC

**Duration**: 7 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to COM
2. COM Fundamentals
3. Creating COM Objects in MFC
4. IUnknown and Reference Counting
5. IDispatch and Automation
6. Type Libraries
7. COM Error Handling
8. Best Practices
9. Exercises and Projects

---

## 1. Introduction to COM

### What is COM?

**Component Object Model (COM)** is Microsoft's platform-independent, distributed, object-oriented system for creating binary software components that can interact.

```cpp
/*
COM PRINCIPLES:

1. Binary Standard
   - Language independent
   - Version independent
   - Location transparent

2. Interface-Based
   - Contracts between components
   - Immutable interfaces
   - Multiple interfaces per object

3. Reference Counted
   - Automatic lifetime management
   - AddRef/Release pattern
   - No memory leaks

4. GUID-Based Identification
   - Globally unique identifiers
   - CLSIDs and IIDs
   - Registry-based discovery
*/
```

---

## 2. COM Fundamentals

### IUnknown Interface

```cpp
// IUnknown - base interface for all COM objects
interface IUnknown
{
    virtual HRESULT QueryInterface(REFIID riid, void** ppvObject) = 0;
    virtual ULONG AddRef() = 0;
    virtual ULONG Release() = 0;
};

// Simple COM object implementation
class CSimpleCOMObject : public IUnknown
{
public:
    CSimpleCOMObject() : m_cRef(1) {}

    // IUnknown implementation
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject)
    {
        if (riid == IID_IUnknown)
        {
            *ppvObject = static_cast<IUnknown*>(this);
            AddRef();
            return S_OK;
        }

        *ppvObject = NULL;
        return E_NOINTERFACE;
    }

    STDMETHODIMP_(ULONG) AddRef()
    {
        return InterlockedIncrement(&m_cRef);
    }

    STDMETHODIMP_(ULONG) Release()
    {
        ULONG cRef = InterlockedDecrement(&m_cRef);
        if (cRef == 0)
        {
            delete this;
        }
        return cRef;
    }

protected:
    virtual ~CSimpleCOMObject() {}

private:
    LONG m_cRef;
};
```

---

## 3. Creating COM Objects in MFC

### CCmdTarget-Based COM Objects

```cpp
// MathComponent.h
[
    uuid(12345678-1234-1234-1234-123456789ABC),
    version(1.0),
    helpstring("Math Component Type Library")
]
library MathLib
{
    importlib("stdole32.tlb");

    [
        uuid(12345678-1234-1234-1234-123456789ABD),
        helpstring("IMathOperations Interface")
    ]
    interface IMathOperations : IDispatch
    {
        [id(1)] HRESULT Add([in] long a, [in] long b, [out, retval] long* result);
        [id(2)] HRESULT Subtract([in] long a, [in] long b, [out, retval] long* result);
        [id(3)] HRESULT Multiply([in] long a, [in] long b, [out, retval] long* result);
        [id(4)] HRESULT Divide([in] long a, [in] long b, [out, retval] double* result);
    };

    [
        uuid(12345678-1234-1234-1234-123456789ABE),
        helpstring("MathComponent Class")
    ]
    coclass MathComponent
    {
        [default] interface IMathOperations;
    };
};

// CMathComponent.h
class CMathComponent : public CCmdTarget
{
    DECLARE_DYNCREATE(CMathComponent)

public:
    CMathComponent();

protected:
    virtual ~CMathComponent();

    DECLARE_MESSAGE_MAP()
    DECLARE_DISPATCH_MAP()
    DECLARE_INTERFACE_MAP()
    DECLARE_OLECREATE(CMathComponent)

    // Automation methods
    afx_msg long Add(long a, long b);
    afx_msg long Subtract(long a, long b);
    afx_msg long Multiply(long a, long b);
    afx_msg double Divide(long a, long b);
};

// CMathComponent.cpp
IMPLEMENT_DYNCREATE(CMathComponent, CCmdTarget)

IMPLEMENT_OLECREATE(CMathComponent, "MathComponent.Application",
    0x12345678, 0x1234, 0x1234,
    0x12, 0x34, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBE)

BEGIN_MESSAGE_MAP(CMathComponent, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CMathComponent, CCmdTarget)
    DISP_FUNCTION(CMathComponent, "Add", Add, VT_I4, VTS_I4 VTS_I4)
    DISP_FUNCTION(CMathComponent, "Subtract", Subtract, VT_I4, VTS_I4 VTS_I4)
    DISP_FUNCTION(CMathComponent, "Multiply", Multiply, VT_I4, VTS_I4 VTS_I4)
    DISP_FUNCTION(CMathComponent, "Divide", Divide, VT_R8, VTS_I4 VTS_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CMathComponent, CCmdTarget)
    INTERFACE_PART(CMathComponent, IID_IMathOperations, Dispatch)
END_INTERFACE_MAP()

CMathComponent::CMathComponent()
{
    EnableAutomation();
}

CMathComponent::~CMathComponent()
{
}

long CMathComponent::Add(long a, long b)
{
    return a + b;
}

long CMathComponent::Subtract(long a, long b)
{
    return a - b;
}

long CMathComponent::Multiply(long a, long b)
{
    return a * b;
}

double CMathComponent::Divide(long a, long b)
{
    if (b == 0)
    {
        AfxThrowOleException(E_INVALIDARG);
    }
    return (double)a / b;
}
```

---

## 4. IUnknown and Reference Counting

### Smart Pointers

```cpp
// Using CComPtr (ATL smart pointer)
void UseMathComponent()
{
    CComPtr<IMathOperations> spMath;

    HRESULT hr = spMath.CoCreateInstance(CLSID_MathComponent);
    if (SUCCEEDED(hr))
    {
        long result;
        hr = spMath->Add(10, 20, &result);
        if (SUCCEEDED(hr))
        {
            CString msg;
            msg.Format(_T("10 + 20 = %d"), result);
            AfxMessageBox(msg);
        }

        // Automatically released when spMath goes out of scope
    }
}

// Custom smart pointer wrapper
template<typename T>
class CComPtr
{
public:
    CComPtr() : m_p(NULL) {}

    ~CComPtr()
    {
        Release();
    }

    void Release()
    {
        if (m_p)
        {
            m_p->Release();
            m_p = NULL;
        }
    }

    HRESULT CoCreateInstance(REFCLSID rclsid)
    {
        Release();
        return ::CoCreateInstance(rclsid, NULL, CLSCTX_INPROC_SERVER,
                                  __uuidof(T), (void**)&m_p);
    }

    T* operator->() const
    {
        return m_p;
    }

    T** operator&()
    {
        Release();
        return &m_p;
    }

private:
    T* m_p;

    // Prevent copying
    CComPtr(const CComPtr&);
    CComPtr& operator=(const CComPtr&);
};
```

---

## 5. IDispatch and Automation

### Creating Automation Server

```cpp
// DataProcessor.h
class CDataProcessor : public CCmdTarget
{
    DECLARE_DYNCREATE(CDataProcessor)

public:
    CDataProcessor();

protected:
    virtual ~CDataProcessor();

    DECLARE_DISPATCH_MAP()
    DECLARE_INTERFACE_MAP()
    DECLARE_OLECREATE(CDataProcessor)

    // Properties
    afx_msg BSTR GetLastError();
    afx_msg long GetItemCount();

    // Methods
    afx_msg BOOL LoadFile(LPCTSTR lpszFilePath);
    afx_msg BOOL SaveFile(LPCTSTR lpszFilePath);
    afx_msg BSTR ProcessData(LPCTSTR lpszInput);
    afx_msg VARIANT GetItem(long index);
    afx_msg void ClearAll();

private:
    CString m_strLastError;
    CStringArray m_items;
};

// DataProcessor.cpp
IMPLEMENT_DYNCREATE(CDataProcessor, CCmdTarget)

IMPLEMENT_OLECREATE(CDataProcessor, "DataProcessor.Application",
    0x87654321, 0x4321, 0x4321,
    0x87, 0x65, 0x87, 0x65, 0x43, 0x21, 0x43, 0x21)

BEGIN_DISPATCH_MAP(CDataProcessor, CCmdTarget)
    DISP_PROPERTY_EX(CDataProcessor, "LastError", GetLastError, SetNotSupported, VT_BSTR)
    DISP_PROPERTY_EX(CDataProcessor, "ItemCount", GetItemCount, SetNotSupported, VT_I4)
    DISP_FUNCTION(CDataProcessor, "LoadFile", LoadFile, VT_BOOL, VTS_BSTR)
    DISP_FUNCTION(CDataProcessor, "SaveFile", SaveFile, VT_BOOL, VTS_BSTR)
    DISP_FUNCTION(CDataProcessor, "ProcessData", ProcessData, VT_BSTR, VTS_BSTR)
    DISP_FUNCTION(CDataProcessor, "GetItem", GetItem, VT_VARIANT, VTS_I4)
    DISP_FUNCTION(CDataProcessor, "ClearAll", ClearAll, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

CDataProcessor::CDataProcessor()
{
    EnableAutomation();
}

CDataProcessor::~CDataProcessor()
{
}

BSTR CDataProcessor::GetLastError()
{
    return m_strLastError.AllocSysString();
}

long CDataProcessor::GetItemCount()
{
    return m_items.GetCount();
}

BOOL CDataProcessor::LoadFile(LPCTSTR lpszFilePath)
{
    try
    {
        CStdioFile file;
        if (!file.Open(lpszFilePath, CFile::modeRead | CFile::typeText))
        {
            m_strLastError = _T("Failed to open file");
            return FALSE;
        }

        m_items.RemoveAll();
        CString line;
        while (file.ReadString(line))
        {
            m_items.Add(line);
        }

        file.Close();
        m_strLastError.Empty();
        return TRUE;
    }
    catch (CFileException* e)
    {
        TCHAR szError[256];
        e->GetErrorMessage(szError, 256);
        m_strLastError = szError;
        e->Delete();
        return FALSE;
    }
}

VARIANT CDataProcessor::GetItem(long index)
{
    COleVariant var;

    if (index >= 0 && index < m_items.GetCount())
    {
        var = m_items[index];
    }
    else
    {
        var.ChangeType(VT_NULL);
        m_strLastError = _T("Index out of range");
    }

    return var.Detach();
}
```

### Using Automation Objects

```cpp
// Client using automation
void UseDataProcessor()
{
    COleException e;

    CLSID clsid;
    if (FAILED(CLSIDFromProgID(L"DataProcessor.Application", &clsid)))
    {
        AfxMessageBox(_T("Failed to get CLSID"));
        return;
    }

    IDispatch* pDisp = NULL;
    if (FAILED(CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER,
                                 IID_IDispatch, (void**)&pDisp)))
    {
        AfxMessageBox(_T("Failed to create instance"));
        return;
    }

    // Call LoadFile method
    DISPID dispidLoadFile;
    OLECHAR* szMember = L"LoadFile";
    if (SUCCEEDED(pDisp->GetIDsOfNames(IID_NULL, &szMember, 1,
                                       LOCALE_USER_DEFAULT, &dispidLoadFile)))
    {
        VARIANT varResult;
        DISPPARAMS dispparams = {NULL, NULL, 0, 0};
        VARIANTARG arg;

        arg.vt = VT_BSTR;
        arg.bstrVal = SysAllocString(L"data.txt");

        dispparams.rgvarg = &arg;
        dispparams.cArgs = 1;

        HRESULT hr = pDisp->Invoke(dispidLoadFile, IID_NULL, LOCALE_USER_DEFAULT,
                                   DISPATCH_METHOD, &dispparams, &varResult,
                                   NULL, NULL);

        SysFreeString(arg.bstrVal);

        if (SUCCEEDED(hr) && varResult.vt == VT_BOOL && varResult.boolVal)
        {
            AfxMessageBox(_T("File loaded successfully"));
        }
    }

    pDisp->Release();
}

// Easier with COleDispatchDriver
class CDataProcessorDriver : public COleDispatchDriver
{
public:
    CDataProcessorDriver() {}
    CDataProcessorDriver(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}

    BOOL LoadFile(LPCTSTR lpszFilePath)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x1, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, lpszFilePath);
        return result;
    }

    CString ProcessData(LPCTSTR lpszInput)
    {
        CString result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x2, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, lpszInput);
        return result;
    }

    long GetItemCount()
    {
        long result;
        InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
};

void UseDataProcessorEasy()
{
    CDataProcessorDriver driver;

    if (!driver.CreateDispatch(_T("DataProcessor.Application")))
    {
        AfxMessageBox(_T("Failed to create object"));
        return;
    }

    if (driver.LoadFile(_T("data.txt")))
    {
        CString msg;
        msg.Format(_T("Loaded %d items"), driver.GetItemCount());
        AfxMessageBox(msg);
    }
}
```

---

## 6. Type Libraries

### Creating Type Library

```cpp
// Math.idl
import "oaidl.idl";
import "ocidl.idl";

[
    uuid(12345678-1234-1234-1234-123456789ABC),
    version(1.0),
    helpstring("Advanced Math Library")
]
library AdvancedMathLib
{
    importlib("stdole32.tlb");

    [
        uuid(12345678-1234-1234-1234-123456789ABD),
        helpstring("IAdvancedMath Interface"),
        oleautomation,
        dual
    ]
    interface IAdvancedMath : IDispatch
    {
        [id(1), helpstring("Calculate factorial")]
        HRESULT Factorial([in] long n, [out, retval] double* result);

        [id(2), helpstring("Calculate power")]
        HRESULT Power([in] double base, [in] double exponent,
                     [out, retval] double* result);

        [id(3), helpstring("Calculate square root")]
        HRESULT SquareRoot([in] double value, [out, retval] double* result);

        [propget, id(4), helpstring("Get PI")]
        HRESULT PI([out, retval] double* pVal);

        [propget, id(5), helpstring("Get E")]
        HRESULT E([out, retval] double* pVal);
    };

    [
        uuid(12345678-1234-1234-1234-123456789ABE),
        helpstring("AdvancedMath Class")
    ]
    coclass AdvancedMath
    {
        [default] interface IAdvancedMath;
    };
};

// Compile with:
// midl Math.idl
```

### Using Type Library

```cpp
// Import type library
#import "Math.tlb" no_namespace

void UseTypedMath()
{
    CoInitialize(NULL);

    try
    {
        IAdvancedMathPtr spMath;
        spMath.CreateInstance(__uuidof(AdvancedMath));

        double result;

        // Call methods
        spMath->Factorial(10, &result);
        CString msg;
        msg.Format(_T("10! = %.0f"), result);
        AfxMessageBox(msg);

        spMath->Power(2.0, 8.0, &result);
        msg.Format(_T("2^8 = %.0f"), result);
        AfxMessageBox(msg);

        // Get properties
        double pi;
        spMath->get_PI(&pi);
        msg.Format(_T("PI = %.10f"), pi);
        AfxMessageBox(msg);
    }
    catch (_com_error& e)
    {
        CString msg;
        msg.Format(_T("Error: %s"), e.ErrorMessage());
        AfxMessageBox(msg);
    }

    CoUninitialize();
}
```

---

## 7. COM Error Handling

### Error Information

```cpp
// Setting error information
HRESULT SetErrorInfo(LPCTSTR lpszDescription, REFGUID guid = GUID_NULL)
{
    ICreateErrorInfo* pCreateErrorInfo;
    IErrorInfo* pErrorInfo;

    HRESULT hr = CreateErrorInfo(&pCreateErrorInfo);
    if (FAILED(hr))
        return hr;

    pCreateErrorInfo->SetDescription((LPOLESTR)CComBSTR(lpszDescription));
    pCreateErrorInfo->SetGUID(guid);

    hr = pCreateErrorInfo->QueryInterface(IID_IErrorInfo,
                                          (void**)&pErrorInfo);

    if (SUCCEEDED(hr))
    {
        ::SetErrorInfo(0, pErrorInfo);
        pErrorInfo->Release();
    }

    pCreateErrorInfo->Release();
    return S_OK;
}

// In COM object method
STDMETHODIMP CMyObject::Divide(long a, long b, double* result)
{
    if (b == 0)
    {
        SetErrorInfo(_T("Division by zero"), IID_IMyInterface);
        return E_INVALIDARG;
    }

    *result = (double)a / b;
    return S_OK;
}

// Retrieving error information
void HandleCOMError(HRESULT hr)
{
    if (FAILED(hr))
    {
        IErrorInfo* pErrorInfo = NULL;
        if (GetErrorInfo(0, &pErrorInfo) == S_OK)
        {
            BSTR bstrDescription;
            pErrorInfo->GetDescription(&bstrDescription);

            CString msg = bstrDescription;
            AfxMessageBox(msg);

            SysFreeString(bstrDescription);
            pErrorInfo->Release();
        }
        else
        {
            CString msg;
            msg.Format(_T("COM Error: 0x%08X"), hr);
            AfxMessageBox(msg);
        }
    }
}
```

---

## 8. Best Practices

```cpp
/*
COM BEST PRACTICES:

1. INTERFACE DESIGN
   - Keep interfaces simple
   - Version carefully
   - Never change published interfaces
   - Use [in], [out], [in,out] correctly

2. THREADING
   - Understand apartment models
   - STA vs MTA
   - Marshal interfaces properly
   - Use CoInitialize/CoUninitialize

3. ERROR HANDLING
   - Always return HRESULT
   - Use SetErrorInfo
   - Document error codes
   - Check all return values

4. MEMORY MANAGEMENT
   - Use SysAllocString/SysFreeString
   - CoTaskMemAlloc/CoTaskMemFree
   - Match allocator/deallocator
   - Smart pointers prevent leaks

5. REGISTRATION
   - Self-registration
   - Proper registry entries
   - Type library registration
   - Clean unregistration

6. VERSIONING
   - New interfaces for changes
   - Maintain backward compatibility
   - Version resources
   - Document changes

7. SECURITY
   - Validate all inputs
   - Check buffer sizes
   - Use safe string functions
   - Authentication/authorization

8. PERFORMANCE
   - Minimize cross-apartment calls
   - Batch operations
   - Cache frequently used data
   - Profile COM calls
*/
```

---

## Key Takeaways

1. **COM** provides language-independent components
2. **IUnknown** is the base for all COM interfaces
3. **Reference counting** manages object lifetime
4. **IDispatch** enables automation
5. **Type libraries** provide type information
6. **MFC CCmdTarget** simplifies COM implementation
7. **Smart pointers** prevent memory leaks
8. **Error information** improves debugging
9. **Threading models** affect COM behavior
10. **Best practices** ensure robust COM components

---

## Next Lesson Preview

In **Lesson 68: Database Programming with MFC (ODBC)**, we'll explore:
- ODBC architecture
- Database connections
- Recordsets
- SQL queries
- Transactions
- Data binding

**Master COM for powerful component development!**
