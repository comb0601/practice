# Lesson 74: Clipboard Operations

**Duration**: 5 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to Clipboard
2. Text Operations
3. Bitmap Operations
4. Custom Clipboard Formats
5. Clipboard Monitoring
6. Best Practices

---

## 1. Introduction to Clipboard

### Clipboard Basics

```cpp
/*
CLIPBOARD CONCEPTS:
- System-wide data exchange
- Multiple formats supported
- Temporary storage
- Owner notification

STANDARD FORMATS:
- CF_TEXT: ANSI text
- CF_UNICODETEXT: Unicode text
- CF_BITMAP: Bitmap image
- CF_DIB: Device-independent bitmap
- CF_HDROP: File list
*/
```

---

## 2. Text Operations

### Copy and Paste Text

```cpp
class CClipboardHelper
{
public:
    static BOOL CopyText(LPCTSTR lpszText);
    static CString PasteText();
    static BOOL HasText();
};

BOOL CClipboardHelper::CopyText(LPCTSTR lpszText)
{
    if (!lpszText || !AfxGetMainWnd())
        return FALSE;

    if (!OpenClipboard(AfxGetMainWnd()->GetSafeHwnd()))
        return FALSE;

    EmptyClipboard();

    size_t nLen = _tcslen(lpszText) + 1;
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nLen * sizeof(TCHAR));

    if (hGlobal == NULL)
    {
        CloseClipboard();
        return FALSE;
    }

    LPTSTR lpszCopy = (LPTSTR)GlobalLock(hGlobal);
    _tcscpy_s(lpszCopy, nLen, lpszText);
    GlobalUnlock(hGlobal);

#ifdef UNICODE
    SetClipboardData(CF_UNICODETEXT, hGlobal);
#else
    SetClipboardData(CF_TEXT, hGlobal);
#endif

    CloseClipboard();
    return TRUE;
}

CString CClipboardHelper::PasteText()
{
    CString strResult;

    if (!AfxGetMainWnd() || !OpenClipboard(AfxGetMainWnd()->GetSafeHwnd()))
        return strResult;

#ifdef UNICODE
    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
#else
    HANDLE hData = GetClipboardData(CF_TEXT);
#endif

    if (hData != NULL)
    {
        LPTSTR lpszText = (LPTSTR)GlobalLock(hData);
        if (lpszText)
        {
            strResult = lpszText;
            GlobalUnlock(hData);
        }
    }

    CloseClipboard();
    return strResult;
}

BOOL CClipboardHelper::HasText()
{
#ifdef UNICODE
    return IsClipboardFormatAvailable(CF_UNICODETEXT);
#else
    return IsClipboardFormatAvailable(CF_TEXT);
#endif
}
```

---

## 3. Bitmap Operations

### Copy and Paste Bitmaps

```cpp
class CBitmapClipboard
{
public:
    static BOOL CopyBitmap(HBITMAP hBitmap);
    static HBITMAP PasteBitmap();
    static BOOL HasBitmap();
};

BOOL CBitmapClipboard::CopyBitmap(HBITMAP hBitmap)
{
    if (!hBitmap || !AfxGetMainWnd())
        return FALSE;

    if (!OpenClipboard(AfxGetMainWnd()->GetSafeHwnd()))
        return FALSE;

    EmptyClipboard();

    // Create a copy of the bitmap
    BITMAP bm;
    GetObject(hBitmap, sizeof(bm), &bm);

    HDC hDC = GetDC(NULL);
    HDC hMemDC = CreateCompatibleDC(hDC);

    HBITMAP hCopyBitmap = CreateCompatibleBitmap(hDC, bm.bmWidth, bm.bmHeight);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hCopyBitmap);

    HDC hSrcDC = CreateCompatibleDC(hDC);
    HBITMAP hOldSrcBitmap = (HBITMAP)SelectObject(hSrcDC, hBitmap);

    BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hSrcDC, 0, 0, SRCCOPY);

    SelectObject(hSrcDC, hOldSrcBitmap);
    DeleteDC(hSrcDC);

    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);
    ReleaseDC(NULL, hDC);

    SetClipboardData(CF_BITMAP, hCopyBitmap);
    CloseClipboard();

    return TRUE;
}

HBITMAP CBitmapClipboard::PasteBitmap()
{
    if (!AfxGetMainWnd() || !OpenClipboard(AfxGetMainWnd()->GetSafeHwnd()))
        return NULL;

    HBITMAP hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);

    if (hBitmap)
    {
        // Create a copy
        BITMAP bm;
        GetObject(hBitmap, sizeof(bm), &bm);

        HDC hDC = GetDC(NULL);
        HDC hMemDC = CreateCompatibleDC(hDC);

        HBITMAP hCopyBitmap = CreateCompatibleBitmap(hDC, bm.bmWidth, bm.bmHeight);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hCopyBitmap);

        HDC hSrcDC = CreateCompatibleDC(hDC);
        HBITMAP hOldSrcBitmap = (HBITMAP)SelectObject(hSrcDC, hBitmap);

        BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hSrcDC, 0, 0, SRCCOPY);

        SelectObject(hSrcDC, hOldSrcBitmap);
        DeleteDC(hSrcDC);

        SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
        ReleaseDC(NULL, hDC);

        CloseClipboard();
        return hCopyBitmap;
    }

    CloseClipboard();
    return NULL;
}

BOOL CBitmapClipboard::HasBitmap()
{
    return IsClipboardFormatAvailable(CF_BITMAP);
}
```

---

## 4. Custom Clipboard Formats

### Custom Data Format

```cpp
class CCustomClipboard
{
public:
    struct CustomData
    {
        int nVersion;
        TCHAR szTitle[100];
        int nValue;
        double dAmount;
    };

    static BOOL CopyCustomData(const CustomData& data);
    static BOOL PasteCustomData(CustomData& data);
    static BOOL HasCustomData();

private:
    static UINT GetCustomFormat();
};

UINT CCustomClipboard::GetCustomFormat()
{
    static UINT s_uFormat = 0;

    if (s_uFormat == 0)
    {
        s_uFormat = RegisterClipboardFormat(_T("MyApp.CustomData"));
    }

    return s_uFormat;
}

BOOL CCustomClipboard::CopyCustomData(const CustomData& data)
{
    if (!AfxGetMainWnd())
        return FALSE;

    if (!OpenClipboard(AfxGetMainWnd()->GetSafeHwnd()))
        return FALSE;

    EmptyClipboard();

    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, sizeof(CustomData));

    if (hGlobal == NULL)
    {
        CloseClipboard();
        return FALSE;
    }

    CustomData* pData = (CustomData*)GlobalLock(hGlobal);
    *pData = data;
    GlobalUnlock(hGlobal);

    SetClipboardData(GetCustomFormat(), hGlobal);
    CloseClipboard();

    return TRUE;
}

BOOL CCustomClipboard::PasteCustomData(CustomData& data)
{
    if (!AfxGetMainWnd() || !OpenClipboard(AfxGetMainWnd()->GetSafeHwnd()))
        return FALSE;

    HANDLE hData = GetClipboardData(GetCustomFormat());

    if (hData != NULL)
    {
        CustomData* pData = (CustomData*)GlobalLock(hData);
        if (pData)
        {
            data = *pData;
            GlobalUnlock(hData);
            CloseClipboard();
            return TRUE;
        }
    }

    CloseClipboard();
    return FALSE;
}

BOOL CCustomClipboard::HasCustomData()
{
    return IsClipboardFormatAvailable(GetCustomFormat());
}
```

---

## 5. Clipboard Monitoring

### Clipboard Change Notification

```cpp
class CClipboardMonitor : public CWnd
{
public:
    CClipboardMonitor();
    virtual ~CClipboardMonitor();

    BOOL Start();
    void Stop();

protected:
    afx_msg void OnChangeCbChain(HWND hWndRemove, HWND hWndAfter);
    afx_msg void OnDrawClipboard();

    DECLARE_MESSAGE_MAP()

private:
    HWND m_hWndNextViewer;
};

BEGIN_MESSAGE_MAP(CClipboardMonitor, CWnd)
    ON_WM_CHANGECBCHAIN()
    ON_WM_DRAWCLIPBOARD()
END_MESSAGE_MAP()

CClipboardMonitor::CClipboardMonitor()
    : m_hWndNextViewer(NULL)
{
}

CClipboardMonitor::~CClipboardMonitor()
{
    Stop();
}

BOOL CClipboardMonitor::Start()
{
    if (!CreateEx(0, AfxRegisterWndClass(0), _T("ClipboardMonitor"),
                  0, 0, 0, 0, 0, HWND_MESSAGE, NULL))
    {
        return FALSE;
    }

    m_hWndNextViewer = SetClipboardViewer(m_hWnd);
    return TRUE;
}

void CClipboardMonitor::Stop()
{
    if (m_hWnd)
    {
        ChangeClipboardChain(m_hWnd, m_hWndNextViewer);
        DestroyWindow();
    }
}

void CClipboardMonitor::OnChangeCbChain(HWND hWndRemove, HWND hWndAfter)
{
    if (m_hWndNextViewer == hWndRemove)
    {
        m_hWndNextViewer = hWndAfter;
    }
    else if (m_hWndNextViewer)
    {
        ::SendMessage(m_hWndNextViewer, WM_CHANGECBCHAIN,
                     (WPARAM)hWndRemove, (LPARAM)hWndAfter);
    }
}

void CClipboardMonitor::OnDrawClipboard()
{
    // Clipboard content changed
    TRACE(_T("Clipboard content changed\n"));

    // Notify application
    AfxGetMainWnd()->PostMessage(WM_USER + 100, 0, 0);

    // Forward to next viewer
    if (m_hWndNextViewer)
    {
        ::SendMessage(m_hWndNextViewer, WM_DRAWCLIPBOARD, 0, 0);
    }
}
```

---

## 6. Best Practices

```cpp
/*
CLIPBOARD BEST PRACTICES:

1. ERROR HANDLING
   - Always check OpenClipboard
   - Check allocation results
   - Close clipboard in all paths

2. MEMORY MANAGEMENT
   - Use GlobalAlloc/GlobalFree
   - Lock/Unlock global memory
   - Don't free clipboard data

3. FORMATS
   - Support multiple formats
   - Use standard formats when possible
   - Register custom formats once

4. PERFORMANCE
   - Keep clipboard open briefly
   - Don't poll clipboard
   - Use clipboard monitoring

5. USER EXPERIENCE
   - Provide visual feedback
   - Support Cut/Copy/Paste menu items
   - Implement Ctrl+C/V/X shortcuts
*/
```

---

## Key Takeaways

1. **Clipboard** enables data exchange
2. **OpenClipboard/CloseClipboard** required
3. **Global memory** for data storage
4. **Standard formats** for compatibility
5. **Custom formats** for specific data
6. **Monitoring** detects changes
7. **Best practices** ensure reliability

---

## Next Lesson Preview

In **Lesson 75: Drag and Drop**, we'll explore:
- OLE drag and drop
- Drag source implementation
- Drop target implementation
- Visual feedback

**Master clipboard operations!**
