# Lesson 65: MFC Extension DLLs

**Duration**: 7 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Extension DLLs
2. Creating an Extension DLL
3. Exporting MFC Classes
4. Resource Sharing
5. CDynLinkLibrary Class
6. Module State Management
7. Common Patterns
8. Best Practices
9. Practical Examples
10. Exercises and Projects

---

## 1. Introduction to Extension DLLs

### What is an MFC Extension DLL?

An **MFC Extension DLL** is a DLL specifically designed for use with MFC applications. It can export MFC-derived classes and shares the MFC module state with the calling application.

```cpp
/*
EXTENSION DLL vs REGULAR DLL:

Extension DLL:
+ Exports MFC classes directly
+ Shares MFC DLL state
+ Smaller size
+ MFC-to-MFC only
+ Requires dynamic MFC linking
- Cannot be called from non-MFC apps

Regular DLL:
+ Can be called from any application
+ Independent MFC state
+ More versatile
- Larger when statically linked
- Requires wrapper for MFC classes
*/
```

---

## 2. Creating an Extension DLL

### Project Setup

```cpp
// Visual Studio:
// 1. New Project -> MFC DLL
// 2. Select "MFC Extension DLL"
// 3. Generated files:
//    - MyExtDLL.h
//    - MyExtDLL.cpp
//    - stdafx.h
//    - resource.h
```

### Basic Extension DLL Structure

```cpp
// MyExtDLL.h
#pragma once

#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"

// MyExtDLL.cpp
#include "stdafx.h"
#include "MyExtDLL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE MyExtDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        TRACE0("MyExtDLL.DLL Initializing!\n");

        // Extension DLL one-time initialization
        if (!AfxInitExtensionModule(MyExtDLL, hInstance))
            return 0;

        // Insert this DLL into the resource chain
        new CDynLinkLibrary(MyExtDLL);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        TRACE0("MyExtDLL.DLL Terminating!\n");

        AfxTermExtensionModule(MyExtDLL);
    }

    return 1;
}
```

---

## 3. Exporting MFC Classes

### Exporting CObject-Derived Classes

```cpp
// CustomControl.h
#pragma once

#ifdef CUSTOMCONTROLDLL_EXPORTS
#define CUSTOMCONTROL_API AFX_EXT_CLASS
#else
#define CUSTOMCONTROL_API AFX_EXT_CLASS
#endif

// Custom button control
class CUSTOMCONTROL_API CCustomButton : public CButton
{
    DECLARE_DYNAMIC(CCustomButton)

public:
    CCustomButton();
    virtual ~CCustomButton();

    // Custom properties
    void SetGradientColors(COLORREF crStart, COLORREF crEnd);
    void SetRoundedCorners(BOOL bRounded, int nRadius = 10);
    void SetHoverEffect(BOOL bEnable);

    // Custom behavior
    void EnableAnimation(BOOL bEnable);
    void SetAnimationDuration(DWORD dwMilliseconds);

protected:
    DECLARE_MESSAGE_MAP()

    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
    void DrawButton(CDC* pDC);
    void DrawGradient(CDC* pDC, CRect rect, COLORREF crStart, COLORREF crEnd);

    COLORREF m_crStartColor;
    COLORREF m_crEndColor;
    BOOL m_bRoundedCorners;
    int m_nCornerRadius;
    BOOL m_bHoverEffect;
    BOOL m_bMouseOver;
    BOOL m_bPressed;
    BOOL m_bAnimation;
    DWORD m_dwAnimationDuration;
};

// CustomControl.cpp
#include "stdafx.h"
#include "CustomControl.h"

IMPLEMENT_DYNAMIC(CCustomButton, CButton)

BEGIN_MESSAGE_MAP(CCustomButton, CButton)
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CCustomButton::CCustomButton()
    : m_crStartColor(RGB(100, 150, 255))
    , m_crEndColor(RGB(50, 100, 200))
    , m_bRoundedCorners(TRUE)
    , m_nCornerRadius(10)
    , m_bHoverEffect(TRUE)
    , m_bMouseOver(FALSE)
    , m_bPressed(FALSE)
    , m_bAnimation(FALSE)
    , m_dwAnimationDuration(200)
{
}

CCustomButton::~CCustomButton()
{
}

void CCustomButton::SetGradientColors(COLORREF crStart, COLORREF crEnd)
{
    m_crStartColor = crStart;
    m_crEndColor = crEnd;
    Invalidate();
}

void CCustomButton::SetRoundedCorners(BOOL bRounded, int nRadius)
{
    m_bRoundedCorners = bRounded;
    m_nCornerRadius = nRadius;
    Invalidate();
}

void CCustomButton::SetHoverEffect(BOOL bEnable)
{
    m_bHoverEffect = bEnable;
}

void CCustomButton::EnableAnimation(BOOL bEnable)
{
    m_bAnimation = bEnable;
}

void CCustomButton::SetAnimationDuration(DWORD dwMilliseconds)
{
    m_dwAnimationDuration = dwMilliseconds;
}

void CCustomButton::OnPaint()
{
    CPaintDC dc(this);
    DrawButton(&dc);
}

void CCustomButton::DrawButton(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    CDC memDC;
    memDC.CreateCompatibleDC(pDC);

    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    // Determine colors based on state
    COLORREF crStart = m_crStartColor;
    COLORREF crEnd = m_crEndColor;

    if (m_bPressed)
    {
        crStart = RGB(GetRValue(crStart) * 0.7, GetGValue(crStart) * 0.7, GetBValue(crStart) * 0.7);
        crEnd = RGB(GetRValue(crEnd) * 0.7, GetGValue(crEnd) * 0.7, GetBValue(crEnd) * 0.7);
    }
    else if (m_bMouseOver && m_bHoverEffect)
    {
        crStart = RGB(min(255, GetRValue(crStart) * 1.1), min(255, GetGValue(crStart) * 1.1), min(255, GetBValue(crStart) * 1.1));
        crEnd = RGB(min(255, GetRValue(crEnd) * 1.1), min(255, GetGValue(crEnd) * 1.1), min(255, GetBValue(crEnd) * 1.1));
    }

    // Draw gradient background
    DrawGradient(&memDC, rect, crStart, crEnd);

    // Draw text
    CString strText;
    GetWindowText(strText);

    memDC.SetBkMode(TRANSPARENT);
    memDC.SetTextColor(RGB(255, 255, 255));

    CFont* pFont = GetFont();
    CFont* pOldFont = memDC.SelectObject(pFont);

    memDC.DrawText(strText, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    memDC.SelectObject(pOldFont);

    // Copy to screen
    pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

    memDC.SelectObject(pOldBitmap);
}

void CCustomButton::DrawGradient(CDC* pDC, CRect rect, COLORREF crStart, COLORREF crEnd)
{
    int nSteps = rect.Height();

    for (int i = 0; i < nSteps; i++)
    {
        int r = GetRValue(crStart) + (GetRValue(crEnd) - GetRValue(crStart)) * i / nSteps;
        int g = GetGValue(crStart) + (GetGValue(crEnd) - GetGValue(crStart)) * i / nSteps;
        int b = GetBValue(crStart) + (GetBValue(crEnd) - GetBValue(crStart)) * i / nSteps;

        CBrush brush(RGB(r, g, b));
        CRect lineRect(rect.left, rect.top + i, rect.right, rect.top + i + 1);
        pDC->FillRect(&lineRect, &brush);
    }
}

void CCustomButton::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bMouseOver)
    {
        m_bMouseOver = TRUE;
        Invalidate();

        TRACKMOUSEEVENT tme = { 0 };
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = m_hWnd;
        TrackMouseEvent(&tme);
    }

    CButton::OnMouseMove(nFlags, point);
}

void CCustomButton::OnMouseLeave()
{
    m_bMouseOver = FALSE;
    Invalidate();
}

void CCustomButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bPressed = TRUE;
    Invalidate();
    CButton::OnLButtonDown(nFlags, point);
}

void CCustomButton::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_bPressed = FALSE;
    Invalidate();
    CButton::OnLButtonUp(nFlags, point);
}

BOOL CCustomButton::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;  // Prevent flickering
}
```

### Exporting Dialog Classes

```cpp
// CustomDialog.h
class CUSTOMCONTROL_API CCustomDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CCustomDialog)

public:
    CCustomDialog(UINT nIDTemplate, CWnd* pParent = NULL);
    virtual ~CCustomDialog();

    // Custom initialization
    void SetTitle(LPCTSTR lpszTitle);
    void SetBackgroundColor(COLORREF color);
    void EnableDragging(BOOL bEnable);

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()

    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

private:
    CString m_strTitle;
    COLORREF m_crBackground;
    BOOL m_bEnableDragging;
    BOOL m_bDragging;
    CPoint m_ptDragStart;
};

// CustomDialog.cpp
IMPLEMENT_DYNAMIC(CCustomDialog, CDialogEx)

BEGIN_MESSAGE_MAP(CCustomDialog, CDialogEx)
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CCustomDialog::CCustomDialog(UINT nIDTemplate, CWnd* pParent)
    : CDialogEx(nIDTemplate, pParent)
    , m_crBackground(RGB(240, 240, 240))
    , m_bEnableDragging(FALSE)
    , m_bDragging(FALSE)
{
}

CCustomDialog::~CCustomDialog()
{
}

void CCustomDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BOOL CCustomDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    if (!m_strTitle.IsEmpty())
    {
        SetWindowText(m_strTitle);
    }

    return TRUE;
}

void CCustomDialog::SetTitle(LPCTSTR lpszTitle)
{
    m_strTitle = lpszTitle;
    if (m_hWnd)
    {
        SetWindowText(lpszTitle);
    }
}

void CCustomDialog::SetBackgroundColor(COLORREF color)
{
    m_crBackground = color;
    if (m_hWnd)
    {
        Invalidate();
    }
}

void CCustomDialog::EnableDragging(BOOL bEnable)
{
    m_bEnableDragging = bEnable;
}

BOOL CCustomDialog::OnEraseBkgnd(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);

    CBrush brush(m_crBackground);
    pDC->FillRect(&rect, &brush);

    return TRUE;
}

void CCustomDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (m_bEnableDragging)
    {
        m_bDragging = TRUE;
        m_ptDragStart = point;
        SetCapture();
    }

    CDialogEx::OnLButtonDown(nFlags, point);
}

void CCustomDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bDragging)
    {
        m_bDragging = FALSE;
        ReleaseCapture();
    }

    CDialogEx::OnLButtonUp(nFlags, point);
}

void CCustomDialog::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bDragging)
    {
        CPoint ptDelta = point - m_ptDragStart;
        CRect rect;
        GetWindowRect(&rect);
        SetWindowPos(NULL, rect.left + ptDelta.x, rect.top + ptDelta.y,
                     0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }

    CDialogEx::OnMouseMove(nFlags, point);
}
```

---

## 4. Resource Sharing

### Sharing Dialogs

```cpp
// SharedResources.h
#ifdef SHAREDRESOURCES_EXPORTS
#define SHARED_API AFX_EXT_CLASS
#else
#define SHARED_API
#endif

// Dialog resource IDs
#define IDD_ABOUTBOX        2000
#define IDD_SETTINGS        2001
#define IDD_LOGIN           2002

class SHARED_API CAboutDialog : public CDialogEx
{
public:
    CAboutDialog(CWnd* pParent = NULL);

    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
};

class SHARED_API CSettingsDialog : public CDialogEx
{
public:
    CSettingsDialog(CWnd* pParent = NULL);

    enum { IDD = IDD_SETTINGS };

    CString GetUsername() const { return m_strUsername; }
    CString GetPassword() const { return m_strPassword; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()

private:
    CString m_strUsername;
    CString m_strPassword;
};

// Usage in client application
void CMainFrame::OnHelpAbout()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CAboutDialog dlg;
    dlg.DoModal();
}
```

### Sharing Bitmaps and Icons

```cpp
// ImageResources.h
class SHARED_API CImageResources
{
public:
    static HICON LoadAppIcon(UINT nID);
    static HBITMAP LoadAppBitmap(UINT nID);
    static BOOL LoadImage(UINT nID, CImage& image);
};

// ImageResources.cpp
HICON CImageResources::LoadAppIcon(UINT nID)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return AfxGetApp()->LoadIcon(nID);
}

HBITMAP CImageResources::LoadAppBitmap(UINT nID)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (HBITMAP)::LoadImage(
        AfxGetResourceHandle(),
        MAKEINTRESOURCE(nID),
        IMAGE_BITMAP,
        0, 0,
        LR_DEFAULTCOLOR
    );
}

BOOL CImageResources::LoadImage(UINT nID, CImage& image)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    HBITMAP hBitmap = LoadAppBitmap(nID);
    if (!hBitmap)
        return FALSE;

    image.Attach(hBitmap);
    return TRUE;
}
```

---

## 5. CDynLinkLibrary Class

### Understanding CDynLinkLibrary

```cpp
/*
CDynLinkLibrary:
- Manages extension DLL resources
- Inserts DLL into resource chain
- Enables resource sharing
- Automatically cleaned up

Resource Chain:
Application -> Extension DLL 1 -> Extension DLL 2 -> System
*/

// In DllMain
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        // Initialize extension module
        if (!AfxInitExtensionModule(MyDLL, hInstance))
            return 0;

        // Insert into resource chain
        new CDynLinkLibrary(MyDLL);  // Automatically deleted
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        AfxTermExtensionModule(MyDLL);
    }

    return 1;
}
```

### Custom CDynLinkLibrary

```cpp
class CMyDynLinkLibrary : public CDynLinkLibrary
{
public:
    CMyDynLinkLibrary(AFX_EXTENSION_MODULE& state)
        : CDynLinkLibrary(state)
    {
        TRACE0("CMyDynLinkLibrary created\n");
    }

    virtual ~CMyDynLinkLibrary()
    {
        TRACE0("CMyDynLinkLibrary destroyed\n");
    }
};

// In DllMain
if (dwReason == DLL_PROCESS_ATTACH)
{
    if (!AfxInitExtensionModule(MyDLL, hInstance))
        return 0;

    new CMyDynLinkLibrary(MyDLL);
}
```

---

## 6. Module State Management

### AFX_MANAGE_STATE Macro

```cpp
// Always use AFX_MANAGE_STATE in exported functions
class MYEXT_API CMyExtClass
{
public:
    void ShowDialog()
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());

        CDialog dlg(IDD_MY_DIALOG);
        dlg.DoModal();
    }

    CString LoadString(UINT nID)
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());

        CString str;
        str.LoadString(nID);
        return str;
    }

    HICON GetIcon(UINT nID)
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        return AfxGetApp()->LoadIcon(nID);
    }
};
```

### Module State Switching

```cpp
class CModuleStateSwitcher
{
public:
    CModuleStateSwitcher()
    {
        m_pOldState = AfxSetModuleState(AfxGetStaticModuleState());
    }

    ~CModuleStateSwitcher()
    {
        AfxSetModuleState(m_pOldState);
    }

private:
    AFX_MODULE_STATE* m_pOldState;
};

// Usage
void MyFunction()
{
    CModuleStateSwitcher switcher;  // Auto switch and restore

    // Access DLL resources here
    CString str;
    str.LoadString(IDS_MY_STRING);
}
```

---

## 7. Common Patterns

### Custom Control Library

```cpp
// ControlLibrary.h
#ifdef CONTROLLIBRARY_EXPORTS
#define CONTROLLIB_API AFX_EXT_CLASS
#else
#define CONTROLLIB_API
#endif

class CONTROLLIB_API CNumericEdit : public CEdit
{
    DECLARE_DYNAMIC(CNumericEdit)

public:
    CNumericEdit();

    void SetRange(double dMin, double dMax);
    void SetDecimalPlaces(int nPlaces);
    double GetValue() const;
    void SetValue(double dValue);

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKillFocus(CWnd* pNewWnd);

private:
    BOOL ValidateInput();

    double m_dMinValue;
    double m_dMaxValue;
    int m_nDecimalPlaces;
};

class CONTROLLIB_API CColorPicker : public CButton
{
    DECLARE_DYNAMIC(CColorPicker)

public:
    CColorPicker();

    COLORREF GetColor() const { return m_color; }
    void SetColor(COLORREF color);

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnClicked();
    afx_msg void OnPaint();

private:
    COLORREF m_color;
};
```

### Utility Function Library

```cpp
// UtilityLib.h
class UTILITY_API CFileHelper
{
public:
    static BOOL FileExists(LPCTSTR lpszPath);
    static BOOL DirectoryExists(LPCTSTR lpszPath);
    static BOOL CreateDirectoryRecursive(LPCTSTR lpszPath);
    static CString GetFileExtension(LPCTSTR lpszPath);
    static CString GetFileName(LPCTSTR lpszPath);
    static BOOL CopyFileWithProgress(LPCTSTR lpszSource, LPCTSTR lpszDest, CWnd* pProgressWnd = NULL);
};

class UTILITY_API CStringHelper
{
public:
    static CString Trim(const CString& str);
    static CStringArray Split(const CString& str, TCHAR delimiter);
    static CString Join(const CStringArray& arr, LPCTSTR lpszSeparator);
    static BOOL IsNumeric(const CString& str);
    static BOOL IsEmail(const CString& str);
};

class UTILITY_API CRegistryHelper
{
public:
    static BOOL ReadString(HKEY hKey, LPCTSTR lpszPath, LPCTSTR lpszValue, CString& strData);
    static BOOL WriteString(HKEY hKey, LPCTSTR lpszPath, LPCTSTR lpszValue, LPCTSTR lpszData);
    static BOOL ReadDWORD(HKEY hKey, LPCTSTR lpszPath, LPCTSTR lpszValue, DWORD& dwData);
    static BOOL WriteDWORD(HKEY hKey, LPCTSTR lpszPath, LPCTSTR lpszValue, DWORD dwData);
};
```

---

## 8. Best Practices

### Design Guidelines

```cpp
/*
BEST PRACTICES FOR EXTENSION DLLs:

1. MODULE STATE
   - Always use AFX_MANAGE_STATE
   - Switch state before accessing resources
   - Automatic restoration with macro

2. RESOURCE MANAGEMENT
   - Use unique resource IDs (>= 2000)
   - Avoid conflicts with client apps
   - Document resource ranges

3. VERSIONING
   - Export version information
   - Maintain compatibility
   - Document breaking changes

4. INITIALIZATION
   - Minimal work in DllMain
   - Provide Init/Cleanup functions
   - Handle failures gracefully

5. THREADING
   - Use thread-safe designs
   - Document thread safety
   - Use synchronization when needed

6. MEMORY MANAGEMENT
   - Match new/delete pairs
   - Use same heap (same CRT)
   - Avoid cross-DLL memory transfers

7. EXCEPTION HANDLING
   - Don't let exceptions cross boundaries
   - Catch and convert to error codes
   - Document exception behavior

8. DOCUMENTATION
   - Document all exported items
   - Provide usage examples
   - Include header comments
*/
```

### Version Information

```cpp
// Version.h
#define MYEXT_VERSION_MAJOR     1
#define MYEXT_VERSION_MINOR     0
#define MYEXT_VERSION_BUILD     0
#define MYEXT_VERSION_REVISION  0

class MYEXT_API CVersionInfo
{
public:
    static DWORD GetVersion();
    static CString GetVersionString();
    static BOOL IsCompatible(DWORD dwRequiredVersion);
};

// Version.cpp
DWORD CVersionInfo::GetVersion()
{
    return MAKELONG(
        MAKEWORD(MYEXT_VERSION_REVISION, MYEXT_VERSION_BUILD),
        MAKEWORD(MYEXT_VERSION_MINOR, MYEXT_VERSION_MAJOR)
    );
}

CString CVersionInfo::GetVersionString()
{
    CString str;
    str.Format(_T("%d.%d.%d.%d"),
               MYEXT_VERSION_MAJOR,
               MYEXT_VERSION_MINOR,
               MYEXT_VERSION_BUILD,
               MYEXT_VERSION_REVISION);
    return str;
}

BOOL CVersionInfo::IsCompatible(DWORD dwRequiredVersion)
{
    return GetVersion() >= dwRequiredVersion;
}
```

---

## 9. Practical Examples

### Complete Control Library

```cpp
// See previous sections for CCustomButton, CNumericEdit, CColorPicker
// Package them together in one extension DLL

// ControlsDLL.h
#pragma once

#include "CustomButton.h"
#include "NumericEdit.h"
#include "ColorPicker.h"
#include "ProgressDialog.h"

// Export initialization function
extern "C" CONTROLSDLL_API BOOL InitializeControls();
extern "C" CONTROLSDLL_API void CleanupControls();
```

### Using the Extension DLL

```cpp
// Client application
// 1. Link with ControlsDLL.lib
// 2. Include ControlsDLL.h
// 3. Copy ControlsDLL.dll to output directory

// In application initialization
BOOL CMyApp::InitInstance()
{
    // ... standard initialization ...

    if (!InitializeControls())
    {
        AfxMessageBox(_T("Failed to initialize controls library"));
        return FALSE;
    }

    // ... rest of initialization ...

    return TRUE;
}

// In dialog
class CMyDialog : public CDialogEx
{
public:
    CMyDialog(CWnd* pParent = NULL);

protected:
    virtual BOOL OnInitDialog();

    CCustomButton m_btnOK;
    CNumericEdit m_editAmount;
    CColorPicker m_colorPicker;
};

BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Subclass controls
    m_btnOK.SubclassDlgItem(IDC_BTN_OK, this);
    m_editAmount.SubclassDlgItem(IDC_EDIT_AMOUNT, this);
    m_colorPicker.SubclassDlgItem(IDC_COLOR_PICKER, this);

    // Configure controls
    m_btnOK.SetGradientColors(RGB(0, 128, 255), RGB(0, 64, 192));
    m_btnOK.SetRoundedCorners(TRUE, 15);

    m_editAmount.SetRange(0.0, 10000.0);
    m_editAmount.SetDecimalPlaces(2);

    m_colorPicker.SetColor(RGB(255, 0, 0));

    return TRUE;
}
```

---

## Key Takeaways

1. **Extension DLLs** are optimized for MFC-to-MFC communication
2. **AFX_EXT_CLASS** exports MFC classes properly
3. **CDynLinkLibrary** manages resource chain
4. **AFX_MANAGE_STATE** is required for resource access
5. **Module state** must be managed carefully
6. **Version control** ensures compatibility
7. **Resource IDs** should be unique and high (>= 2000)
8. **Thread safety** must be considered
9. **Documentation** is essential
10. **Best practices** prevent common pitfalls

---

## Next Lesson Preview

In **Lesson 66: ActiveX Controls in MFC**, we'll explore:
- Creating ActiveX controls
- Control properties and methods
- Events and event sinking
- Property pages
- Control containers
- Licensing

**Master extension DLLs for powerful MFC libraries!**
