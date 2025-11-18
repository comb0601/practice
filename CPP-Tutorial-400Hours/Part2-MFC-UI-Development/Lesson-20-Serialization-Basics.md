# Lesson 20: Serialization Basics

**Duration**: 7 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding Serialization
2. The CArchive Class
3. Basic Serialization Operations
4. Serializing Simple Types
5. Serializing CString
6. Serializing Collections
7. File Versioning
8. Error Handling
9. Text vs Binary Serialization
10. Complete Examples
11. Exercises

---

## 1. Understanding Serialization

### What is Serialization?

**Serialization** is the process of converting data structures or objects into a format that can be stored in a file or transmitted over a network, and then reconstructing them later.

```
   MEMORY                    FILE                     MEMORY
   ┌────────┐              ┌─────────┐              ┌────────┐
   │ Object │   ────→      │  Bytes  │   ────→      │ Object │
   │  Data  │   Serialize  │ on Disk │   Deserialize│  Data  │
   └────────┘              └─────────┘              └────────┘
```

### Why Serialize?

1. **Save Documents** - Persist user data between sessions
2. **File Formats** - Create custom file formats (.mya, .dwg, etc.)
3. **Copy/Paste** - Transfer data via clipboard
4. **Network** - Send data over network
5. **Undo/Redo** - Save states for undo functionality

---

## 2. The CArchive Class

### What is CArchive?

`CArchive` is MFC's serialization engine. It provides:
- Stream-based I/O
- Type-safe reading/writing
- Object pointer management
- Version control support

```cpp
class CArchive
{
public:
    // Construction
    CArchive(CFile* pFile, UINT nMode);

    // Check mode
    BOOL IsStoring() const;    // Writing to file?
    BOOL IsLoading() const;    // Reading from file?

    // Get associated file
    CFile* GetFile() const;

    // Operators for simple types
    CArchive& operator<<(int i);
    CArchive& operator<<(DWORD dw);
    CArchive& operator<<(float f);
    CArchive& operator<<(CString& str);
    // ... etc

    CArchive& operator>>(int& i);
    CArchive& operator>>(DWORD& dw);
    CArchive& operator>>(float& f);
    CArchive& operator>>(CString& str);
    // ... etc

    // String operations
    void WriteString(LPCTSTR lpsz);
    BOOL ReadString(CString& str);

    // Raw data
    UINT Read(void* lpBuf, UINT nMax);
    void Write(const void* lpBuf, UINT nMax);

    // Object serialization
    void WriteObject(const CObject* pOb);
    CObject* ReadObject(const CRuntimeClass* pClass);
};
```

---

### How CArchive Works

```cpp
// Framework creates CArchive for you:

// SAVING
CFile file;
file.Open(lpszPathName, CFile::modeCreate | CFile::modeWrite);
CArchive ar(&file, CArchive::store);  // "store" = save mode
Serialize(ar);                         // Your function called
ar.Close();
file.Close();

// LOADING
CFile file;
file.Open(lpszPathName, CFile::modeRead);
CArchive ar(&file, CArchive::load);    // "load" = read mode
Serialize(ar);                          // Your function called
ar.Close();
file.Close();
```

**You only write this**:
```cpp
void CMyDocument::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // Write data
        ar << m_data;
    }
    else
    {
        // Read data
        ar >> m_data;
    }
}
```

---

## 3. Basic Serialization Operations

### The Serialize() Pattern

```cpp
void CMyDocument::Serialize(CArchive& ar)
{
    // ALWAYS check mode first
    if (ar.IsStoring())
    {
        // *** SAVING ***
        // Write data in specific order
        ar << member1;
        ar << member2;
        ar << member3;
    }
    else
    {
        // *** LOADING ***
        // Read data in SAME order!
        ar >> member1;
        ar >> member2;
        ar >> member3;
    }
}
```

**CRITICAL RULES**:
1. ✅ **Same order** - Read in same order as written
2. ✅ **All members** - Serialize all important data
3. ✅ **Version number** - Always include version
4. ✅ **Validate** - Check loaded data is valid
5. ✅ **Error handling** - Use TRY/CATCH blocks

---

## 4. Serializing Simple Types

### Built-in Type Support

CArchive directly supports these types:

```cpp
void CMyDocument::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // Integers
        char c = 'A';
        short s = 100;
        int i = 1000;
        long l = 100000L;
        ar << c << s << i << l;

        // Unsigned integers
        BYTE b = 255;
        WORD w = 65535;
        DWORD dw = 4294967295UL;
        ar << b << w << dw;

        // Floating point
        float f = 3.14f;
        double d = 2.71828;
        ar << f << d;

        // Boolean (serialize as BYTE or int)
        BOOL bFlag = TRUE;
        ar << (BYTE)bFlag;  // Save as byte

        // COLORREF
        COLORREF color = RGB(255, 0, 0);
        ar << color;  // COLORREF is DWORD

        // CString (covered next section)
        CString str = _T("Hello");
        ar << str;
    }
    else
    {
        // Load in SAME order
        char c;
        short s;
        int i;
        long l;
        ar >> c >> s >> i >> l;

        BYTE b;
        WORD w;
        DWORD dw;
        ar >> b >> w >> dw;

        float f;
        double d;
        ar >> f >> d;

        BYTE bFlag;
        ar >> bFlag;
        BOOL bValue = bFlag;

        COLORREF color;
        ar >> color;

        CString str;
        ar >> str;
    }
}
```

---

### Enums and Custom Types

```cpp
// Enum
enum DocumentType
{
    DOC_TYPE_TEXT = 0,
    DOC_TYPE_IMAGE = 1,
    DOC_TYPE_BINARY = 2
};

// Serialize as int
void CMyDocument::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        ar << (int)m_docType;
    }
    else
    {
        int type;
        ar >> type;
        m_docType = (DocumentType)type;

        // Validate
        if (m_docType < DOC_TYPE_TEXT || m_docType > DOC_TYPE_BINARY)
        {
            m_docType = DOC_TYPE_TEXT;  // Default
        }
    }
}

// Struct/POD types
struct MyData
{
    int value1;
    double value2;
    char name[32];
};

void Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        ar << m_data.value1;
        ar << m_data.value2;
        ar.Write(m_data.name, sizeof(m_data.name));
    }
    else
    {
        ar >> m_data.value1;
        ar >> m_data.value2;
        ar.Read(m_data.name, sizeof(m_data.name));
    }
}
```

---

## 5. Serializing CString

### CString Serialization

```cpp
void CMyDocument::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // Method 1: Using operator<<
        ar << m_strText;
        ar << m_strAuthor;

        // Method 2: WriteString (adds newline)
        ar.WriteString(m_strTitle);
        ar.WriteString(_T("\n"));
    }
    else
    {
        // Method 1: Using operator>>
        ar >> m_strText;
        ar >> m_strAuthor;

        // Method 2: ReadString (reads line)
        ar.ReadString(m_strTitle);
    }
}
```

**operator<< vs WriteString**:
- `operator<<` - Binary format, more efficient, exact preservation
- `WriteString` - Text format, human-readable, line-based

---

### Unicode Considerations

```cpp
void CMyDocument::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        CString str = _T("Hello Unicode: äöü");

        // CString handles Unicode automatically
        ar << str;

        // For compatibility, save as UTF-8
        CStringA utf8 = CW2A(str, CP_UTF8);
        ar << utf8.GetLength();
        ar.Write(utf8, utf8.GetLength());
    }
    else
    {
        CString str;
        ar >> str;

        // Load UTF-8
        int len;
        ar >> len;
        CStringA utf8;
        ar.Read(utf8.GetBuffer(len), len);
        utf8.ReleaseBuffer(len);

        CString str2 = CA2W(utf8, CP_UTF8);
    }
}
```

---

## 6. Serializing Collections

### CArray Serialization

```cpp
class CMyDocument : public CDocument
{
protected:
    CArray<int, int> m_numbers;
    CArray<CPoint, CPoint&> m_points;
    CArray<CString, CString&> m_strings;

    virtual void Serialize(CArchive& ar)
    {
        if (ar.IsStoring())
        {
            // CArray has built-in Serialize()!
            m_numbers.Serialize(ar);
            m_points.Serialize(ar);
            m_strings.Serialize(ar);
        }
        else
        {
            m_numbers.Serialize(ar);
            m_points.Serialize(ar);
            m_strings.Serialize(ar);
        }
    }
};
```

**How CArray::Serialize() works**:
```cpp
// Pseudo-code
void CArray::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        ar << GetSize();  // Write count
        for (int i = 0; i < GetSize(); i++)
            ar << GetAt(i);  // Write each element
    }
    else
    {
        int count;
        ar >> count;  // Read count
        SetSize(count);
        for (int i = 0; i < count; i++)
            ar >> GetAt(i);  // Read each element
    }
}
```

---

### CList Serialization

```cpp
class CMyDocument : public CDocument
{
protected:
    CList<int, int> m_numberList;
    CList<CString, CString&> m_stringList;

    virtual void Serialize(CArchive& ar)
    {
        if (ar.IsStoring())
        {
            m_numberList.Serialize(ar);
            m_stringList.Serialize(ar);
        }
        else
        {
            m_numberList.Serialize(ar);
            m_stringList.Serialize(ar);
        }
    }
};
```

---

### CMap Serialization

```cpp
class CMyDocument : public CDocument
{
protected:
    CMap<CString, LPCTSTR, int, int> m_map;

    virtual void Serialize(CArchive& ar)
    {
        if (ar.IsStoring())
        {
            m_map.Serialize(ar);
        }
        else
        {
            m_map.Serialize(ar);
        }
    }
};
```

---

### Object Arrays (Pointers)

```cpp
class CMyDocument : public CDocument
{
protected:
    CArray<CMyObject*, CMyObject*> m_objects;

    virtual void Serialize(CArchive& ar)
    {
        if (ar.IsStoring())
        {
            // Save count
            ar << (int)m_objects.GetSize();

            // Save each object
            for (int i = 0; i < m_objects.GetSize(); i++)
            {
                CMyObject* pObj = m_objects[i];
                pObj->Serialize(ar);
            }
        }
        else
        {
            // Load count
            int count;
            ar >> count;

            // Clear existing
            DeleteAllObjects();
            m_objects.SetSize(count);

            // Load each object
            for (int i = 0; i < count; i++)
            {
                CMyObject* pObj = new CMyObject;
                pObj->Serialize(ar);
                m_objects[i] = pObj;
            }
        }
    }

    void DeleteAllObjects()
    {
        for (int i = 0; i < m_objects.GetSize(); i++)
        {
            delete m_objects[i];
        }
        m_objects.RemoveAll();
    }
};
```

---

## 7. File Versioning

### Why Version Files?

```
Version 1:     [Name] [Age]
Version 2:     [Name] [Age] [Email]
Version 3:     [Name] [Age] [Email] [Phone]

Need to load ALL three versions correctly!
```

---

### Basic Versioning Pattern

```cpp
class CMyDocument : public CDocument
{
protected:
    CString m_strName;
    int m_nAge;
    CString m_strEmail;    // Added in version 2
    CString m_strPhone;    // Added in version 3

    virtual void Serialize(CArchive& ar)
    {
        const DWORD FILE_VERSION = 3;

        if (ar.IsStoring())
        {
            // ALWAYS save version first
            ar << FILE_VERSION;

            // Save all current data
            ar << m_strName;
            ar << m_nAge;
            ar << m_strEmail;
            ar << m_strPhone;
        }
        else
        {
            // Read version
            DWORD version;
            ar >> version;

            // Check if too new
            if (version > FILE_VERSION)
            {
                AfxMessageBox(_T("File created with newer version"));
                AfxThrowArchiveException(CArchiveException::badSchema);
            }

            // Load data based on version
            ar >> m_strName;
            ar >> m_nAge;

            if (version >= 2)
            {
                ar >> m_strEmail;
            }
            else
            {
                m_strEmail.Empty();  // Default for old files
            }

            if (version >= 3)
            {
                ar >> m_strPhone;
            }
            else
            {
                m_strPhone.Empty();  // Default for old files
            }
        }
    }
};
```

---

### Advanced Versioning with Schema

```cpp
class CMyDocument : public CDocument
{
protected:
    UINT m_nSchemaVersion;

    virtual void Serialize(CArchive& ar)
    {
        const UINT CURRENT_SCHEMA = 5;

        if (ar.IsStoring())
        {
            ar << CURRENT_SCHEMA;
            SerializeData(ar, CURRENT_SCHEMA);
        }
        else
        {
            ar >> m_nSchemaVersion;

            if (m_nSchemaVersion > CURRENT_SCHEMA)
            {
                CString msg;
                msg.Format(
                    _T("This file requires version %d or higher.\n")
                    _T("You have version %d."),
                    m_nSchemaVersion, CURRENT_SCHEMA);
                AfxMessageBox(msg);
                AfxThrowArchiveException(CArchiveException::badSchema);
            }

            SerializeData(ar, m_nSchemaVersion);

            // Upgrade old formats
            if (m_nSchemaVersion < CURRENT_SCHEMA)
            {
                UpgradeFormat(m_nSchemaVersion, CURRENT_SCHEMA);
            }
        }
    }

    void SerializeData(CArchive& ar, UINT schema)
    {
        if (ar.IsStoring())
        {
            // Save based on current schema
            ar << m_data1;
            ar << m_data2;
            if (schema >= 2)
                ar << m_data3;
            if (schema >= 5)
                ar << m_data4;
        }
        else
        {
            // Load based on file schema
            ar >> m_data1;
            ar >> m_data2;
            if (schema >= 2)
                ar >> m_data3;
            if (schema >= 5)
                ar >> m_data4;
        }
    }

    void UpgradeFormat(UINT fromVersion, UINT toVersion)
    {
        // Convert old data to new format
        if (fromVersion < 2)
        {
            // Upgrade from v1 to v2
            m_data3 = ConvertData(m_data1);
        }
        if (fromVersion < 5)
        {
            // Upgrade from v2-4 to v5
            m_data4 = CalculateNewField();
        }
    }
};
```

---

## 8. Error Handling

### Exception Handling in Serialization

```cpp
BOOL CMyDocument::OnOpenDocument(LPCTSTR lpszPathName)
{
    TRY
    {
        if (!CDocument::OnOpenDocument(lpszPathName))
            return FALSE;

        // Validate data
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

        CString msg;
        msg.Format(_T("File error: %s"), szError);
        AfxMessageBox(msg);
        return FALSE;
    }
    CATCH(CArchiveException, e)
    {
        CString msg;
        switch (e->m_cause)
        {
        case CArchiveException::badSchema:
            msg = _T("Invalid file format or version");
            break;
        case CArchiveException::endOfFile:
            msg = _T("Unexpected end of file");
            break;
        case CArchiveException::readOnly:
            msg = _T("Cannot write to read-only file");
            break;
        default:
            msg = _T("Archive error");
            break;
        }
        AfxMessageBox(msg);
        return FALSE;
    }
    CATCH(CMemoryException, e)
    {
        AfxMessageBox(_T("Out of memory"));
        return FALSE;
    }
    CATCH_ALL(e)
    {
        AfxMessageBox(_T("Unknown error loading file"));
        return FALSE;
    }
    END_CATCH_ALL

    return FALSE;
}
```

---

### Throwing Custom Exceptions

```cpp
void CMyDocument::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // Validate before saving
        if (!ValidateData())
        {
            AfxMessageBox(_T("Invalid data - cannot save"));
            AfxThrowUserException();
        }

        ar << m_data;
    }
    else
    {
        ar >> m_data;

        // Validate loaded data
        if (m_data < 0 || m_data > 1000)
        {
            AfxMessageBox(_T("Invalid data in file"));
            AfxThrowArchiveException(CArchiveException::badSchema);
        }
    }
}
```

---

## 9. Text vs Binary Serialization

### Binary Serialization (Default)

```cpp
// Efficient, compact, not human-readable
void CMyDocument::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        ar << m_nValue;     // 4 bytes
        ar << m_strText;    // Length + data
    }
    else
    {
        ar >> m_nValue;
        ar >> m_strText;
    }
}

// File content (binary):
// 00 00 00 0A 00 05 H e l l o
```

**Advantages**:
- ✅ Compact file size
- ✅ Fast
- ✅ Exact preservation
- ✅ Supports binary data

**Disadvantages**:
- ❌ Not human-readable
- ❌ Not editable
- ❌ Platform-dependent (endianness)

---

### Text Serialization

```cpp
// Human-readable, larger size
void CMyDocument::SerializeText(CArchive& ar)
{
    if (ar.IsStoring())
    {
        CString line;
        line.Format(_T("%d\n"), m_nValue);
        ar.WriteString(line);

        ar.WriteString(m_strText);
        ar.WriteString(_T("\n"));
    }
    else
    {
        CString line;
        ar.ReadString(line);
        m_nValue = _ttoi(line);

        ar.ReadString(m_strText);
    }
}

// File content (text):
// 10
// Hello
```

**Advantages**:
- ✅ Human-readable
- ✅ Editable in text editor
- ✅ Easy to debug
- ✅ Cross-platform

**Disadvantages**:
- ❌ Larger file size
- ❌ Slower
- ❌ Precision loss (floats)
- ❌ More complex parsing

---

### Hybrid Approach

```cpp
void CMyDocument::Serialize(CArchive& ar)
{
    const DWORD VERSION = 1;

    if (ar.IsStoring())
    {
        // Binary header
        ar << VERSION;
        ar << m_nObjectCount;

        // Text data
        CString header;
        header.Format(_T("Document: %s\nAuthor: %s\n"),
                     m_strTitle, m_strAuthor);
        ar.WriteString(header);

        // Binary payload
        for (int i = 0; i < m_nObjectCount; i++)
        {
            m_objects[i]->Serialize(ar);  // Binary
        }
    }
    else
    {
        DWORD version;
        ar >> version;
        ar >> m_nObjectCount;

        CString line;
        ar.ReadString(line);  // Title line
        ar.ReadString(line);  // Author line

        for (int i = 0; i < m_nObjectCount; i++)
        {
            CMyObject* pObj = new CMyObject;
            pObj->Serialize(ar);
            m_objects.Add(pObj);
        }
    }
}
```

---

## 10. Complete Examples

### Example 1: Simple Document

```cpp
// SimpleDoc.h
class CSimpleDoc : public CDocument
{
protected:
    CString m_strTitle;
    CString m_strContent;
    CTime m_timeCreated;
    int m_nWordCount;

public:
    CSimpleDoc();

    void SetContent(const CString& content);
    CString GetContent() const { return m_strContent; }

protected:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);

    DECLARE_DYNCREATE(CSimpleDoc)
};

// SimpleDoc.cpp
IMPLEMENT_DYNCREATE(CSimpleDoc, CDocument)

CSimpleDoc::CSimpleDoc()
    : m_nWordCount(0)
{
}

BOOL CSimpleDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    m_strTitle = _T("Untitled");
    m_strContent.Empty();
    m_timeCreated = CTime::GetCurrentTime();
    m_nWordCount = 0;

    return TRUE;
}

void CSimpleDoc::Serialize(CArchive& ar)
{
    const DWORD VERSION = 1;

    if (ar.IsStoring())
    {
        ar << VERSION;
        ar << m_strTitle;
        ar << m_strContent;
        ar << m_timeCreated;
        ar << m_nWordCount;
    }
    else
    {
        DWORD version;
        ar >> version;

        if (version > VERSION)
        {
            AfxMessageBox(_T("File version not supported"));
            AfxThrowArchiveException(CArchiveException::badSchema);
        }

        ar >> m_strTitle;
        ar >> m_strContent;
        ar >> m_timeCreated;
        ar >> m_nWordCount;
    }
}

void CSimpleDoc::SetContent(const CString& content)
{
    m_strContent = content;

    // Calculate word count
    m_nWordCount = 0;
    BOOL inWord = FALSE;

    for (int i = 0; i < content.GetLength(); i++)
    {
        if (_istspace(content[i]))
        {
            inWord = FALSE;
        }
        else if (!inWord)
        {
            m_nWordCount++;
            inWord = TRUE;
        }
    }

    SetModifiedFlag();
    UpdateAllViews(NULL);
}
```

---

### Example 2: Drawing Document

```cpp
// DrawDoc.h
class CDrawDoc : public CDocument
{
protected:
    CObArray m_objects;          // Array of CDrawObject*
    COLORREF m_bgColor;
    CString m_strAuthor;
    int m_nNextObjectID;

public:
    CDrawDoc();
    virtual ~CDrawDoc();

    void AddObject(CDrawObject* pObj);
    void RemoveObject(int index);
    int GetObjectCount() const;
    CDrawObject* GetObject(int index) const;

protected:
    virtual BOOL OnNewDocument();
    virtual void DeleteContents();
    virtual void Serialize(CArchive& ar);

    void SerializeObjects(CArchive& ar);

    DECLARE_DYNCREATE(CDrawDoc)
};

// DrawDoc.cpp
IMPLEMENT_DYNCREATE(CDrawDoc, CDocument)

CDrawDoc::CDrawDoc()
    : m_bgColor(RGB(255, 255, 255))
    , m_nNextObjectID(1)
{
}

CDrawDoc::~CDrawDoc()
{
    DeleteContents();
}

BOOL CDrawDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    m_bgColor = RGB(255, 255, 255);
    m_strAuthor.Empty();
    m_nNextObjectID = 1;

    return TRUE;
}

void CDrawDoc::DeleteContents()
{
    for (int i = 0; i < m_objects.GetSize(); i++)
    {
        CDrawObject* pObj = (CDrawObject*)m_objects[i];
        delete pObj;
    }
    m_objects.RemoveAll();

    CDocument::DeleteContents();
}

void CDrawDoc::Serialize(CArchive& ar)
{
    const DWORD VERSION = 2;

    if (ar.IsStoring())
    {
        // Header
        ar << VERSION;
        ar << m_bgColor;
        ar << m_strAuthor;
        ar << m_nNextObjectID;

        // Objects
        SerializeObjects(ar);
    }
    else
    {
        // Header
        DWORD version;
        ar >> version;

        if (version > VERSION)
        {
            AfxMessageBox(_T("Unsupported file version"));
            AfxThrowArchiveException(CArchiveException::badSchema);
        }

        ar >> m_bgColor;

        if (version >= 2)
        {
            ar >> m_strAuthor;
            ar >> m_nNextObjectID;
        }
        else
        {
            m_strAuthor.Empty();
            m_nNextObjectID = 1;
        }

        // Objects
        SerializeObjects(ar);
    }
}

void CDrawDoc::SerializeObjects(CArchive& ar)
{
    if (ar.IsStoring())
    {
        ar << (int)m_objects.GetSize();

        for (int i = 0; i < m_objects.GetSize(); i++)
        {
            CDrawObject* pObj = (CDrawObject*)m_objects[i];

            // Write object type
            ar << pObj->GetType();

            // Write object data
            pObj->Serialize(ar);
        }
    }
    else
    {
        int count;
        ar >> count;

        DeleteContents();

        for (int i = 0; i < count; i++)
        {
            int type;
            ar >> type;

            // Create object based on type
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
    pObj->SetID(m_nNextObjectID++);
    m_objects.Add(pObj);
    SetModifiedFlag();
    UpdateAllViews(NULL);
}
```

---

## 11. Exercises

### Exercise 1: Basic Serialization

Create a document that serializes:
- Text content
- Font name and size
- Text color
- Background color

---

### Exercise 2: Array Serialization

Create a document with an array of points. Allow user to click to add points. Serialize the points.

---

### Exercise 3: Versioning

Create a document with version 1 having two fields. Add a third field in version 2. Ensure version 1 files can still be loaded.

---

### Exercise 4: Error Handling

Add comprehensive error handling to serialization with specific error messages for different failure types.

---

### Exercise 5: Text Format

Implement text-based serialization (human-readable) for a simple document.

---

## Key Takeaways

1. ✅ Serialization saves/loads document data
2. ✅ CArchive handles the low-level details
3. ✅ IsStoring() determines save vs load
4. ✅ Read in same order as written
5. ✅ Always include version number
6. ✅ Validate loaded data
7. ✅ Handle errors with TRY/CATCH
8. ✅ Collections have built-in Serialize()
9. ✅ Binary is efficient, text is readable
10. ✅ Support backward compatibility

---

## Summary

Serialization is how your documents persist. Master the basics here, and in the next lesson we'll explore advanced serialization techniques!

---

## Next Lesson Preview

In **Lesson 21**, we'll explore:
- Advanced serialization techniques
- Object serialization
- Schema evolution
- Custom file formats
- Optimization

**Congratulations on completing Lesson 20!**
