# Lesson 21: Advanced Serialization

**Duration**: 7 hours
**Difficulty**: Advanced

## Table of Contents
1. Object Serialization with DECLARE_SERIAL
2. Polymorphic Object Serialization
3. Schema Classes
4. Custom File Formats
5. Performance Optimization
6. Lazy Loading
7. Chunked File Format
8. Backward and Forward Compatibility
9. Serialization Security
10. Complete Advanced Examples
11. Exercises

---

## 1. Object Serialization with DECLARE_SERIAL

### The DECLARE_SERIAL Macro

For CObject-derived classes, MFC provides automatic serialization support:

```cpp
// In MyObject.h
class CMyObject : public CObject
{
    DECLARE_SERIAL(CMyObject)  // ← Add this

protected:
    int m_nValue;
    CString m_strName;

public:
    CMyObject();
    virtual ~CMyObject();

    virtual void Serialize(CArchive& ar);
};

// In MyObject.cpp
IMPLEMENT_SERIAL(CMyObject, CObject, 1)  // ← Add this
//                  ^       ^       ^
//                  |       |       Schema number
//                  |       Base class
//                  This class

CMyObject::CMyObject()
    : m_nValue(0)
{
}

CMyObject::~CMyObject()
{
}

void CMyObject::Serialize(CArchive& ar)
{
    CObject::Serialize(ar);  // Call base class

    if (ar.IsStoring())
    {
        ar << m_nValue;
        ar << m_strName;
    }
    else
    {
        ar >> m_nValue;
        ar >> m_strName;
    }
}
```

---

### Using DECLARE_SERIAL Objects

```cpp
// Can now serialize object pointers!
class CMyDocument : public CDocument
{
protected:
    CMyObject* m_pObject;

    virtual void Serialize(CArchive& ar)
    {
        if (ar.IsStoring())
        {
            // Write object pointer
            ar << m_pObject;  // MFC handles everything!
        }
        else
        {
            // Read object pointer
            ar >> m_pObject;  // MFC creates object automatically!
        }
    }
};
```

**How it works**:
1. MFC writes class name to archive
2. MFC writes object data (via Serialize)
3. On load, MFC reads class name
4. MFC creates new object of correct type
5. MFC calls Serialize to load data

---

### The Schema Number

```cpp
IMPLEMENT_SERIAL(CMyObject, CObject, 2)
//                                   ^
//                                   Schema version

void CMyObject::Serialize(CArchive& ar)
{
    CObject::Serialize(ar);

    UINT schema = ar.GetObjectSchema();  // Get version

    if (ar.IsStoring())
    {
        ar << m_nValue;
        ar << m_strName;
        ar << m_strEmail;  // Added in schema 2
    }
    else
    {
        ar >> m_nValue;
        ar >> m_strName;

        if (schema >= 2)
        {
            ar >> m_strEmail;
        }
        else
        {
            m_strEmail.Empty();
        }
    }
}
```

---

## 2. Polymorphic Object Serialization

### The Problem

```cpp
// Base class
class CShape : public CObject
{
    DECLARE_SERIAL(CShape)
public:
    virtual void Draw(CDC* pDC) = 0;
};

// Derived classes
class CRectangle : public CShape
{
    DECLARE_SERIAL(CRectangle)
    CRect m_rect;
};

class CCircle : public CShape
{
    DECLARE_SERIAL(CCircle)
    CPoint m_center;
    int m_radius;
};

// Document
class CDrawDoc : public CDocument
{
protected:
    CArray<CShape*, CShape*> m_shapes;  // Mixed types!
};
```

**How do we serialize mixed types?**

---

### The Solution - Runtime Type Information

```cpp
IMPLEMENT_SERIAL(CShape, CObject, 1)
IMPLEMENT_SERIAL(CRectangle, CShape, 1)
IMPLEMENT_SERIAL(CCircle, CShape, 1)

void CRectangle::Serialize(CArchive& ar)
{
    CShape::Serialize(ar);  // Call base class

    if (ar.IsStoring())
    {
        ar << m_rect;
    }
    else
    {
        ar >> m_rect;
    }
}

void CCircle::Serialize(CArchive& ar)
{
    CShape::Serialize(ar);  // Call base class

    if (ar.IsStoring())
    {
        ar << m_center;
        ar << m_radius;
    }
    else
    {
        ar >> m_center;
        ar >> m_radius;
    }
}

// Document serialization
void CDrawDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        ar << (int)m_shapes.GetSize();

        for (int i = 0; i < m_shapes.GetSize(); i++)
        {
            // MFC automatically writes correct class info!
            ar << m_shapes[i];
        }
    }
    else
    {
        int count;
        ar >> count;

        DeleteAllShapes();

        for (int i = 0; i < count; i++)
        {
            CShape* pShape = NULL;

            // MFC automatically creates correct derived class!
            ar >> pShape;

            m_shapes.Add(pShape);
        }
    }
}
```

**Magic**: MFC writes the class name, then creates the right derived class on load!

---

### IsKindOf() for Type Checking

```cpp
void CDrawDoc::ProcessShapes()
{
    for (int i = 0; i < m_shapes.GetSize(); i++)
    {
        CShape* pShape = m_shapes[i];

        if (pShape->IsKindOf(RUNTIME_CLASS(CRectangle)))
        {
            CRectangle* pRect = (CRectangle*)pShape;
            // Process rectangle
        }
        else if (pShape->IsKindOf(RUNTIME_CLASS(CCircle)))
        {
            CCircle* pCircle = (CCircle*)pShape;
            // Process circle
        }
    }
}
```

---

## 3. Schema Classes

### The CArchive Schema System

```cpp
class CMyComplexObject : public CObject
{
    DECLARE_SERIAL(CMyComplexObject)

protected:
    int m_nDataV1;
    CString m_strDataV2;
    double m_fDataV3;

public:
    virtual void Serialize(CArchive& ar);
};

IMPLEMENT_SERIAL(CMyComplexObject, CObject, 3)  // Schema 3

void CMyComplexObject::Serialize(CArchive& ar)
{
    CObject::Serialize(ar);

    UINT schema = ar.GetObjectSchema();

    if (ar.IsStoring())
    {
        // Always save current format
        ar << m_nDataV1;
        ar << m_strDataV2;
        ar << m_fDataV3;
    }
    else
    {
        // Load based on schema
        ar >> m_nDataV1;

        if (schema >= 2)
        {
            ar >> m_strDataV2;
        }
        else
        {
            m_strDataV2 = _T("Default");
        }

        if (schema >= 3)
        {
            ar >> m_fDataV3;
        }
        else
        {
            m_fDataV3 = 0.0;
        }
    }
}
```

---

### Version History Tracking

```cpp
class CDocument : public CDocument
{
protected:
    struct FileHistory
    {
        UINT nVersion;
        CString strDescription;
        CTime timeCreated;
    };

    CArray<FileHistory> m_history;

    virtual void Serialize(CArchive& ar)
    {
        const UINT CURRENT_VERSION = 5;

        if (ar.IsStoring())
        {
            ar << CURRENT_VERSION;

            // Add to history
            FileHistory entry;
            entry.nVersion = CURRENT_VERSION;
            entry.strDescription = _T("Saved with advanced features");
            entry.timeCreated = CTime::GetCurrentTime();
            m_history.Add(entry);

            // Save history
            ar << (int)m_history.GetSize();
            for (int i = 0; i < m_history.GetSize(); i++)
            {
                ar << m_history[i].nVersion;
                ar << m_history[i].strDescription;
                ar << m_history[i].timeCreated;
            }

            // Save data...
        }
        else
        {
            UINT version;
            ar >> version;

            // Load history
            int historyCount;
            ar >> historyCount;

            m_history.RemoveAll();
            for (int i = 0; i < historyCount; i++)
            {
                FileHistory entry;
                ar >> entry.nVersion;
                ar >> entry.strDescription;
                ar >> entry.timeCreated;
                m_history.Add(entry);
            }

            // Load data based on version...
        }
    }
};
```

---

## 4. Custom File Formats

### Chunked File Format

```cpp
// File structure:
// [HEADER]
// [CHUNK 1] [CHUNK 2] [CHUNK 3] ...
// Each chunk: [CHUNK_ID] [SIZE] [DATA]

enum ChunkType
{
    CHUNK_HEADER = 0x48445220,  // "HDR "
    CHUNK_DATA = 0x44415441,    // "DATA"
    CHUNK_METADATA = 0x4D455441, // "META"
    CHUNK_THUMBNAIL = 0x54484D42  // "THMB"
};

class CChunkedDocument : public CDocument
{
protected:
    void WriteChunk(CArchive& ar, DWORD chunkID, const void* pData, DWORD size)
    {
        ar << chunkID;
        ar << size;
        ar.Write(pData, size);
    }

    BOOL ReadChunk(CArchive& ar, DWORD& chunkID, CByteArray& data)
    {
        TRY
        {
            ar >> chunkID;

            DWORD size;
            ar >> size;

            if (size > 10 * 1024 * 1024)  // 10MB limit
            {
                return FALSE;  // Suspiciously large
            }

            data.SetSize(size);
            ar.Read(data.GetData(), size);

            return TRUE;
        }
        CATCH(CArchiveException, e)
        {
            return FALSE;
        }
        END_CATCH
    }

    virtual void Serialize(CArchive& ar)
    {
        if (ar.IsStoring())
        {
            // Write header chunk
            struct Header
            {
                DWORD signature;  // "MYDC"
                DWORD version;
                DWORD flags;
            } header;

            header.signature = 0x4D594443;  // "MYDC"
            header.version = 1;
            header.flags = 0;

            WriteChunk(ar, CHUNK_HEADER, &header, sizeof(header));

            // Write metadata chunk
            CString metadata = _T("Created by MyApp");
            WriteChunk(ar, CHUNK_METADATA,
                      (LPCTSTR)metadata,
                      metadata.GetLength() * sizeof(TCHAR));

            // Write data chunk
            CByteArray dataBuffer;
            SerializeToBuffer(dataBuffer);
            WriteChunk(ar, CHUNK_DATA,
                      dataBuffer.GetData(),
                      (DWORD)dataBuffer.GetSize());

            // Write thumbnail (optional)
            if (m_thumbnail.GetSafeHandle())
            {
                CByteArray thumbData;
                SaveBitmapToBuffer(m_thumbnail, thumbData);
                WriteChunk(ar, CHUNK_THUMBNAIL,
                          thumbData.GetData(),
                          (DWORD)thumbData.GetSize());
            }
        }
        else
        {
            // Read chunks
            while (TRUE)
            {
                DWORD chunkID;
                CByteArray chunkData;

                if (!ReadChunk(ar, chunkID, chunkData))
                    break;  // End of file

                switch (chunkID)
                {
                case CHUNK_HEADER:
                    ProcessHeaderChunk(chunkData);
                    break;

                case CHUNK_DATA:
                    ProcessDataChunk(chunkData);
                    break;

                case CHUNK_METADATA:
                    ProcessMetadataChunk(chunkData);
                    break;

                case CHUNK_THUMBNAIL:
                    ProcessThumbnailChunk(chunkData);
                    break;

                default:
                    // Unknown chunk - skip it
                    TRACE("Unknown chunk: 0x%08X\n", chunkID);
                    break;
                }
            }
        }
    }
};
```

---

### XML-Based Format

```cpp
#include <msxml2.h>
#pragma comment(lib, "msxml2.lib")

class CXMLDocument : public CDocument
{
protected:
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName)
    {
        TRY
        {
            // Create XML document
            MSXML2::IXMLDOMDocumentPtr pDoc;
            pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));

            // Create root element
            MSXML2::IXMLDOMElementPtr pRoot =
                pDoc->createElement(_T("MyDocument"));
            pDoc->appendChild(pRoot);

            // Add version
            pRoot->setAttribute(_T("version"), _variant_t((long)1));

            // Add data elements
            MSXML2::IXMLDOMElementPtr pTitle =
                pDoc->createElement(_T("Title"));
            pTitle->put_text(m_strTitle.AllocSysString());
            pRoot->appendChild(pTitle);

            MSXML2::IXMLDOMElementPtr pContent =
                pDoc->createElement(_T("Content"));
            pContent->put_text(m_strContent.AllocSysString());
            pRoot->appendChild(pContent);

            // Save to file
            pDoc->save(_variant_t(lpszPathName));

            SetModifiedFlag(FALSE);
            return TRUE;
        }
        CATCH(_com_error, e)
        {
            AfxMessageBox(e.ErrorMessage());
            return FALSE;
        }
        END_CATCH
    }

    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName)
    {
        TRY
        {
            // Load XML document
            MSXML2::IXMLDOMDocumentPtr pDoc;
            pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));

            if (pDoc->load(_variant_t(lpszPathName)) != VARIANT_TRUE)
            {
                AfxMessageBox(_T("Invalid XML file"));
                return FALSE;
            }

            // Get root element
            MSXML2::IXMLDOMElementPtr pRoot = pDoc->documentElement;

            // Get version
            _variant_t varVersion = pRoot->getAttribute(_T("version"));
            long version = varVersion.lVal;

            // Load data
            MSXML2::IXMLDOMNodePtr pTitle =
                pRoot->selectSingleNode(_T("Title"));
            if (pTitle)
            {
                m_strTitle = (LPCTSTR)pTitle->text;
            }

            MSXML2::IXMLDOMNodePtr pContent =
                pRoot->selectSingleNode(_T("Content"));
            if (pContent)
            {
                m_strContent = (LPCTSTR)pContent->text;
            }

            SetModifiedFlag(FALSE);
            UpdateAllViews(NULL);
            return TRUE;
        }
        CATCH(_com_error, e)
        {
            AfxMessageBox(e.ErrorMessage());
            return FALSE;
        }
        END_CATCH
    }
};
```

---

## 5. Performance Optimization

### Buffering

```cpp
class COptimizedDocument : public CDocument
{
protected:
    CByteArray m_largeData;

    virtual void Serialize(CArchive& ar)
    {
        if (ar.IsStoring())
        {
            // Write size first
            DWORD size = (DWORD)m_largeData.GetSize();
            ar << size;

            // Write in chunks for better performance
            const DWORD CHUNK_SIZE = 64 * 1024;  // 64KB chunks
            DWORD offset = 0;

            while (offset < size)
            {
                DWORD chunk = min(CHUNK_SIZE, size - offset);
                ar.Write(m_largeData.GetData() + offset, chunk);
                offset += chunk;

                // Update progress
                // ... (see next section)
            }
        }
        else
        {
            DWORD size;
            ar >> size;

            m_largeData.SetSize(size);

            const DWORD CHUNK_SIZE = 64 * 1024;
            DWORD offset = 0;

            while (offset < size)
            {
                DWORD chunk = min(CHUNK_SIZE, size - offset);
                ar.Read(m_largeData.GetData() + offset, chunk);
                offset += chunk;
            }
        }
    }
};
```

---

### Progress Indication

```cpp
class CProgressDocument : public CDocument
{
protected:
    CProgressDlg* m_pProgressDlg;

    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName)
    {
        // Show progress dialog
        m_pProgressDlg = new CProgressDlg;
        m_pProgressDlg->Create(IDD_PROGRESS, AfxGetMainWnd());
        m_pProgressDlg->ShowWindow(SW_SHOW);
        m_pProgressDlg->SetStatus(_T("Saving document..."));

        BOOL result = CDocument::OnSaveDocument(lpszPathName);

        m_pProgressDlg->DestroyWindow();
        delete m_pProgressDlg;
        m_pProgressDlg = NULL;

        return result;
    }

    virtual void Serialize(CArchive& ar)
    {
        if (ar.IsStoring())
        {
            int totalItems = m_objects.GetSize();

            for (int i = 0; i < totalItems; i++)
            {
                // Update progress
                if (m_pProgressDlg)
                {
                    int percent = (i * 100) / totalItems;
                    m_pProgressDlg->SetProgress(percent);

                    CString status;
                    status.Format(_T("Saving item %d of %d..."),
                                 i + 1, totalItems);
                    m_pProgressDlg->SetStatus(status);

                    // Process messages to keep UI responsive
                    MSG msg;
                    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                    {
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }
                }

                // Serialize object
                m_objects[i]->Serialize(ar);
            }
        }
    }
};
```

---

## 6. Lazy Loading

### On-Demand Data Loading

```cpp
class CLazyDocument : public CDocument
{
protected:
    // Metadata (always loaded)
    int m_nObjectCount;
    CArray<DWORD> m_objectOffsets;  // File positions

    // Data (loaded on demand)
    CArray<CMyObject*> m_objects;
    CArray<BOOL> m_objectsLoaded;

    CString m_strFilePath;

public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName)
    {
        m_strFilePath = lpszPathName;

        CFile file;
        if (!file.Open(lpszPathName, CFile::modeRead))
            return FALSE;

        CArchive ar(&file, CArchive::load);

        // Load only metadata
        ar >> m_nObjectCount;

        m_objectOffsets.SetSize(m_nObjectCount);
        m_objects.SetSize(m_nObjectCount);
        m_objectsLoaded.SetSize(m_nObjectCount);

        for (int i = 0; i < m_nObjectCount; i++)
        {
            ar >> m_objectOffsets[i];  // File position
            m_objects[i] = NULL;
            m_objectsLoaded[i] = FALSE;
        }

        ar.Close();
        file.Close();

        SetModifiedFlag(FALSE);
        return TRUE;
    }

    CMyObject* GetObject(int index)
    {
        if (index < 0 || index >= m_nObjectCount)
            return NULL;

        // Load on demand
        if (!m_objectsLoaded[index])
        {
            LoadObject(index);
        }

        return m_objects[index];
    }

protected:
    void LoadObject(int index)
    {
        CFile file;
        if (!file.Open(m_strFilePath, CFile::modeRead))
            return;

        // Seek to object position
        file.Seek(m_objectOffsets[index], CFile::begin);

        CArchive ar(&file, CArchive::load);

        // Load object
        CMyObject* pObj = new CMyObject;
        pObj->Serialize(ar);

        m_objects[index] = pObj;
        m_objectsLoaded[index] = TRUE;

        ar.Close();
        file.Close();
    }
};
```

---

## 7. Chunked File Format

### Complete Chunked Implementation

```cpp
class CChunkedFile
{
public:
    struct ChunkHeader
    {
        DWORD dwChunkID;
        DWORD dwSize;
        DWORD dwCRC;      // Checksum
    };

    static void WriteChunk(CArchive& ar,
                          DWORD dwChunkID,
                          const void* pData,
                          DWORD dwSize)
    {
        ChunkHeader header;
        header.dwChunkID = dwChunkID;
        header.dwSize = dwSize;
        header.dwCRC = CalculateCRC(pData, dwSize);

        ar.Write(&header, sizeof(header));
        ar.Write(pData, dwSize);
    }

    static BOOL ReadChunk(CArchive& ar,
                         DWORD& dwChunkID,
                         CByteArray& data)
    {
        ChunkHeader header;

        TRY
        {
            ar.Read(&header, sizeof(header));

            dwChunkID = header.dwChunkID;

            // Validate size
            if (header.dwSize > MAX_CHUNK_SIZE)
            {
                AfxThrowArchiveException(
                    CArchiveException::badSchema);
            }

            // Read data
            data.SetSize(header.dwSize);
            ar.Read(data.GetData(), header.dwSize);

            // Verify checksum
            DWORD crc = CalculateCRC(data.GetData(), header.dwSize);
            if (crc != header.dwCRC)
            {
                AfxThrowArchiveException(
                    CArchiveException::badSchema);
            }

            return TRUE;
        }
        CATCH(CArchiveException, e)
        {
            return FALSE;
        }
        END_CATCH
    }

    static DWORD CalculateCRC(const void* pData, DWORD dwSize)
    {
        // Simple CRC32 calculation
        DWORD crc = 0xFFFFFFFF;
        const BYTE* p = (const BYTE*)pData;

        for (DWORD i = 0; i < dwSize; i++)
        {
            crc ^= p[i];
            for (int j = 0; j < 8; j++)
            {
                if (crc & 1)
                    crc = (crc >> 1) ^ 0xEDB88320;
                else
                    crc >>= 1;
            }
        }

        return crc ^ 0xFFFFFFFF;
    }

    static const DWORD MAX_CHUNK_SIZE = 100 * 1024 * 1024;  // 100MB
};
```

---

## 8. Backward and Forward Compatibility

### Forward Compatibility

```cpp
// Allowing NEWER versions to be loaded by OLDER applications

class CForwardCompatibleDoc : public CDocument
{
protected:
    virtual void Serialize(CArchive& ar)
    {
        const DWORD CURRENT_VERSION = 2;
        const DWORD COMPATIBLE_VERSION = 1;

        if (ar.IsStoring())
        {
            ar << CURRENT_VERSION;
            ar << COMPATIBLE_VERSION;  // Minimum version required

            // Save v1 format data (compatible)
            ar << m_dataV1;

            // Save v2 format data (in separate optional chunk)
            ar << TRUE;  // V2 data follows
            ar << m_dataV2;
        }
        else
        {
            DWORD version, compatVersion;
            ar >> version;
            ar >> compatVersion;

            if (CURRENT_VERSION < compatVersion)
            {
                CString msg;
                msg.Format(
                    _T("This file requires version %d or higher.\n")
                    _T("You have version %d."),
                    compatVersion, CURRENT_VERSION);
                AfxMessageBox(msg);
                AfxThrowArchiveException(
                    CArchiveException::badSchema);
            }

            // Load v1 data (always present)
            ar >> m_dataV1;

            // Load v2 data if present
            if (version >= 2)
            {
                BOOL hasV2Data;
                ar >> hasV2Data;

                if (hasV2Data)
                {
                    ar >> m_dataV2;
                }
            }
        }
    }
};
```

---

## 9. Serialization Security

### Validation and Sanitization

```cpp
class CSecureDocument : public CDocument
{
protected:
    virtual void Serialize(CArchive& ar)
    {
        if (ar.IsStoring())
        {
            // Add signature
            const DWORD SIGNATURE = 0x4D594150;  // "MYAP"
            ar << SIGNATURE;

            ar << m_data;
        }
        else
        {
            // Verify signature
            DWORD signature;
            ar >> signature;

            if (signature != 0x4D594150)
            {
                AfxMessageBox(_T("Invalid file format"));
                AfxThrowArchiveException(
                    CArchiveException::badSchema);
            }

            // Read data
            ar >> m_data;

            // Validate data
            if (!ValidateData())
            {
                AfxMessageBox(_T("Invalid data in file"));
                OnNewDocument();  // Reset to empty
                AfxThrowUserException();
            }
        }
    }

    BOOL ValidateData()
    {
        // Check for suspicious values
        if (m_data < 0 || m_data > MAX_ALLOWED_VALUE)
            return FALSE;

        // Check for SQL injection patterns
        if (m_strText.Find(_T("--")) >= 0 ||
            m_strText.Find(_T("DROP TABLE")) >= 0)
            return FALSE;

        // Check string lengths
        if (m_strText.GetLength() > MAX_STRING_LENGTH)
            return FALSE;

        return TRUE;
    }

    static const int MAX_ALLOWED_VALUE = 1000000;
    static const int MAX_STRING_LENGTH = 10000;
};
```

---

## 10. Complete Advanced Examples

### Example: Multi-Format Document

```cpp
class CMultiFormatDoc : public CDocument
{
protected:
    enum FileFormat
    {
        FORMAT_BINARY = 1,
        FORMAT_XML = 2,
        FORMAT_JSON = 3
    };

    FileFormat m_saveFormat;

public:
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName)
    {
        CString ext = PathFindExtension(lpszPathName);
        ext.MakeLower();

        if (ext == _T(".xml"))
            m_saveFormat = FORMAT_XML;
        else if (ext == _T(".json"))
            m_saveFormat = FORMAT_JSON;
        else
            m_saveFormat = FORMAT_BINARY;

        return CDocument::OnSaveDocument(lpszPathName);
    }

    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName)
    {
        CString ext = PathFindExtension(lpszPathName);
        ext.MakeLower();

        if (ext == _T(".xml"))
            return LoadXML(lpszPathName);
        else if (ext == _T(".json"))
            return LoadJSON(lpszPathName);
        else
            return CDocument::OnOpenDocument(lpszPathName);
    }

    virtual void Serialize(CArchive& ar)
    {
        if (ar.IsStoring())
        {
            switch (m_saveFormat)
            {
            case FORMAT_BINARY:
                SerializeBinary(ar);
                break;
            case FORMAT_XML:
                // Not used - see OnSaveDocument
                break;
            case FORMAT_JSON:
                // Not used - see OnSaveDocument
                break;
            }
        }
        else
        {
            SerializeBinary(ar);
        }
    }

    void SerializeBinary(CArchive& ar)
    {
        // Standard binary serialization
        if (ar.IsStoring())
        {
            ar << m_data;
        }
        else
        {
            ar >> m_data;
        }
    }

    BOOL LoadXML(LPCTSTR lpszPathName)
    {
        // XML loading implementation
        return TRUE;
    }

    BOOL LoadJSON(LPCTSTR lpszPathName)
    {
        // JSON loading implementation
        return TRUE;
    }
};
```

---

## 11. Exercises

### Exercise 1: Polymorphic Shapes

Create a drawing application with polymorphic shapes (rectangles, circles, lines) that serializes correctly.

---

### Exercise 2: Versioned Document

Create a document with 3 versions, where each version adds new fields. Ensure all three versions can be loaded.

---

### Exercise 3: Chunked Format

Implement a chunked file format with header, data, and metadata chunks.

---

### Exercise 4: Progress Bar

Add a progress bar to serialization for documents with large data.

---

### Exercise 5: Lazy Loading

Implement lazy loading for a document with many large images.

---

## Key Takeaways

1. ✅ DECLARE_SERIAL enables object serialization
2. ✅ Polymorphic serialization preserves types
3. ✅ Schema numbers track object versions
4. ✅ Chunked formats enable extensibility
5. ✅ Lazy loading improves performance
6. ✅ Progress indication improves UX
7. ✅ Validate all loaded data
8. ✅ Support multiple file formats
9. ✅ Plan for backward compatibility
10. ✅ Secure against malicious files

---

## Summary

Advanced serialization techniques enable robust, extensible, and performant file formats. Master these to build professional applications!

---

## Next Lesson Preview

In **Lesson 22**, we'll explore:
- Multiple views of same document
- View coordination
- Splitter windows
- View switching
- Different view types

**Congratulations on completing Lesson 21!**
