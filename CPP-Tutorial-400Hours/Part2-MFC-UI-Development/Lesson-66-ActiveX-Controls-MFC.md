# Lesson 66: ActiveX Controls in MFC

**Duration**: 7 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to ActiveX Controls
2. Creating an ActiveX Control
3. Properties, Methods, and Events
4. Property Pages
5. Persistence and Serialization
6. Control Containers
7. Advanced Features
8. Deployment
9. Best Practices
10. Exercises and Projects

---

## 1. Introduction to ActiveX Controls

### What is an ActiveX Control?

An **ActiveX control** is a reusable software component based on COM (Component Object Model) that can be embedded in applications, web pages, and development tools.

```cpp
/*
ACTIVEX CONTROL FEATURES:

1. Properties
   - Design-time and runtime settings
   - Custom property pages
   - Persistence

2. Methods
   - Functions callable by container
   - Automation support
   - Type library

3. Events
   - Fire events to container
   - Stock and custom events
   - Event sinking

4. Persistence
   - Save/load state
   - Property bag
   - Stream interface

5. Property Pages
   - Custom UI for properties
   - Multiple pages
   - Standard pages
*/
```

---

## 2. Creating an ActiveX Control

### Project Setup

```cpp
// Visual Studio:
// 1. New Project -> MFC ActiveX Control
// 2. Control names and settings
// 3. Generated files:
//    - MyControl.h/cpp      - Control class
//    - MyControlCtl.h/cpp   - Control implementation
//    - MyControlPpg.h/cpp   - Property page
//    - MyControl.idl        - Type library

// MyControlCtl.h
class CMyControlCtrl : public COleControl
{
    DECLARE_DYNCREATE(CMyControlCtrl)

public:
    CMyControlCtrl();

    // Overrides
    virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
    virtual void DoPropExchange(CPropExchange* pPX);
    virtual void OnResetState();
    virtual DWORD GetControlFlags();

    // Implementation
protected:
    ~CMyControlCtrl();

    DECLARE_OLECREATE_EX(CMyControlCtrl)
    DECLARE_OLETYPELIB(CMyControlCtrl)
    DECLARE_PROPPAGEIDS(CMyControlCtrl)
    DECLARE_OLECTLTYPE(CMyControlCtrl)
    DECLARE_MESSAGE_MAP()
    DECLARE_DISPATCH_MAP()
    DECLARE_EVENT_MAP()

    // Properties
    OLE_COLOR m_clrBackColor;
    OLE_COLOR m_clrForeColor;
    CString m_strCaption;
    BOOL m_bEnabled;

    // Stock properties
    afx_msg OLE_COLOR GetBackColor();
    afx_msg void SetBackColor(OLE_COLOR newValue);
    afx_msg OLE_COLOR GetForeColor();
    afx_msg void SetForeColor(OLE_COLOR newValue);
    afx_msg BSTR GetCaption();
    afx_msg void SetCaption(LPCTSTR newValue);
    afx_msg BOOL GetEnabled();
    afx_msg void SetEnabled(BOOL newValue);

    // Custom properties
    afx_msg short GetValue();
    afx_msg void SetValue(short newValue);

    // Methods
    afx_msg void Reset();
    afx_msg void Increment();
    afx_msg void Decrement();

    // Events
    void FireValueChanged(short newValue);
    void FireClicked();

private:
    short m_nValue;
};
```

---

## 3. Properties, Methods, and Events

### Stock Properties

```cpp
// MyControlCtl.cpp
BEGIN_DISPATCH_MAP(CMyControlCtrl, COleControl)
    // Stock properties
    DISP_PROPERTY_EX_ID(CMyControlCtrl, "BackColor", DISPID_BACKCOLOR,
                        GetBackColor, SetBackColor, VT_COLOR)
    DISP_PROPERTY_EX_ID(CMyControlCtrl, "ForeColor", DISPID_FORECOLOR,
                        GetForeColor, SetForeColor, VT_COLOR)
    DISP_PROPERTY_EX_ID(CMyControlCtrl, "Caption", DISPID_CAPTION,
                        GetCaption, SetCaption, VT_BSTR)
    DISP_PROPERTY_EX_ID(CMyControlCtrl, "Enabled", DISPID_ENABLED,
                        GetEnabled, SetEnabled, VT_BOOL)

    // Custom properties
    DISP_PROPERTY_EX(CMyControlCtrl, "Value", GetValue, SetValue, VT_I2)

    // Methods
    DISP_FUNCTION(CMyControlCtrl, "Reset", Reset, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION(CMyControlCtrl, "Increment", Increment, VT_EMPTY, VTS_NONE)
    DISP_FUNCTION(CMyControlCtrl, "Decrement", Decrement, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// Property implementations
OLE_COLOR CMyControlCtrl::GetBackColor()
{
    return m_clrBackColor;
}

void CMyControlCtrl::SetBackColor(OLE_COLOR newValue)
{
    m_clrBackColor = newValue;
    InvalidateControl();
    SetModifiedFlag();
}

OLE_COLOR CMyControlCtrl::GetForeColor()
{
    return m_clrForeColor;
}

void CMyControlCtrl::SetForeColor(OLE_COLOR newValue)
{
    m_clrForeColor = newValue;
    InvalidateControl();
    SetModifiedFlag();
}

BSTR CMyControlCtrl::GetCaption()
{
    return m_strCaption.AllocSysString();
}

void CMyControlCtrl::SetCaption(LPCTSTR newValue)
{
    m_strCaption = newValue;
    InvalidateControl();
    SetModifiedFlag();
}

BOOL CMyControlCtrl::GetEnabled()
{
    return m_bEnabled;
}

void CMyControlCtrl::SetEnabled(BOOL newValue)
{
    m_bEnabled = newValue;
    InvalidateControl();
    SetModifiedFlag();
}

short CMyControlCtrl::GetValue()
{
    return m_nValue;
}

void CMyControlCtrl::SetValue(short newValue)
{
    if (m_nValue != newValue)
    {
        m_nValue = newValue;
        InvalidateControl();
        SetModifiedFlag();
        FireValueChanged(m_nValue);
    }
}
```

### Custom Methods

```cpp
void CMyControlCtrl::Reset()
{
    SetValue(0);
}

void CMyControlCtrl::Increment()
{
    SetValue(m_nValue + 1);
}

void CMyControlCtrl::Decrement()
{
    SetValue(m_nValue - 1);
}

// Method with parameters and return value
DISPATCH_MAP implementation:
    DISP_FUNCTION(CMyControlCtrl, "Calculate", Calculate, VT_I4, VTS_I4 VTS_I4)

long CMyControlCtrl::Calculate(long a, long b)
{
    return a + b;
}
```

### Custom Events

```cpp
BEGIN_EVENT_MAP(CMyControlCtrl, COleControl)
    // Stock events
    EVENT_STOCK_CLICK()
    EVENT_STOCK_DBLCLICK()

    // Custom events
    EVENT_CUSTOM("ValueChanged", FireValueChanged, VTS_I2)
    EVENT_CUSTOM("LimitReached", FireLimitReached, VTS_NONE)
END_EVENT_MAP()

void CMyControlCtrl::FireValueChanged(short newValue)
{
    FireEvent(eventidValueChanged, EVENT_PARAM(VTS_I2), newValue);
}

void CMyControlCtrl::FireLimitReached()
{
    FireEvent(eventidLimitReached, EVENT_PARAM(VTS_NONE));
}

// Firing events in code
void CMyControlCtrl::SetValue(short newValue)
{
    if (newValue > 100)
    {
        newValue = 100;
        FireLimitReached();
    }

    if (m_nValue != newValue)
    {
        m_nValue = newValue;
        InvalidateControl();
        SetModifiedFlag();
        FireValueChanged(m_nValue);
    }
}
```

---

## 4. Property Pages

### Creating Property Pages

```cpp
// MyControlPpg.h
class CMyControlPropPage : public COlePropertyPage
{
    DECLARE_DYNCREATE(CMyControlPropPage)
    DECLARE_OLECREATE_EX(CMyControlPropPage)

public:
    CMyControlPropPage();

    enum { IDD = IDD_PROPPAGE_MYCONTROL };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()

    CString m_strCaption;
    short m_nValue;
    BOOL m_bEnabled;
};

// MyControlPpg.cpp
IMPLEMENT_DYNCREATE(CMyControlPropPage, COlePropertyPage)

BEGIN_MESSAGE_MAP(CMyControlPropPage, COlePropertyPage)
END_MESSAGE_MAP()

CMyControlPropPage::CMyControlPropPage() :
    COlePropertyPage(IDD, IDS_MYCONTROL_PPG)
{
}

void CMyControlPropPage::DoDataExchange(CDataExchange* pDX)
{
    DDP_Text(pDX, IDC_EDIT_CAPTION, m_strCaption, _T("Caption"));
    DDX_Text(pDX, IDC_EDIT_CAPTION, m_strCaption);

    DDP_Text(pDX, IDC_EDIT_VALUE, m_nValue, _T("Value"));
    DDX_Text(pDX, IDC_EDIT_VALUE, m_nValue);

    DDP_Check(pDX, IDC_CHECK_ENABLED, m_bEnabled, _T("Enabled"));
    DDX_Check(pDX, IDC_CHECK_ENABLED, m_bEnabled);

    DDP_PostProcessing(pDX);
}

// In MyControlCtl.cpp - associate property pages
BEGIN_PROPPAGEIDS(CMyControlCtrl, 2)
    PROPPAGEID(CMyControlPropPage::guid)
    PROPPAGEID(CLSID_CColorPropPage)  // Stock color property page
END_PROPPAGEIDS(CMyControlCtrl)
```

### Multiple Property Pages

```cpp
// GeneralPropPage.h
class CGeneralPropPage : public COlePropertyPage
{
    DECLARE_DYNCREATE(CGeneralPropPage)

public:
    enum { IDD = IDD_PROPPAGE_GENERAL };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    CString m_strCaption;
    CString m_strTooltip;
    BOOL m_bEnabled;
};

// AppearancePropPage.h
class CAppearancePropPage : public COlePropertyPage
{
    DECLARE_DYNCREATE(CAppearancePropPage)

public:
    enum { IDD = IDD_PROPPAGE_APPEARANCE };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    OLE_COLOR m_clrBackColor;
    OLE_COLOR m_clrForeColor;
    short m_nBorderWidth;
    short m_nStyle;
};

// In control - register multiple pages
BEGIN_PROPPAGEIDS(CMyControlCtrl, 3)
    PROPPAGEID(CGeneralPropPage::guid)
    PROPPAGEID(CAppearancePropPage::guid)
    PROPPAGEID(CLSID_CColorPropPage)
END_PROPPAGEIDS(CMyControlCtrl)
```

---

## 5. Persistence and Serialization

### DoPropExchange

```cpp
void CMyControlCtrl::DoPropExchange(CPropExchange* pPX)
{
    ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
    COleControl::DoPropExchange(pPX);

    // Stock properties
    PX_Color(pPX, _T("BackColor"), m_clrBackColor, RGB(255, 255, 255));
    PX_Color(pPX, _T("ForeColor"), m_clrForeColor, RGB(0, 0, 0));
    PX_String(pPX, _T("Caption"), m_strCaption, _T("MyControl"));
    PX_Bool(pPX, _T("Enabled"), m_bEnabled, TRUE);

    // Custom properties
    PX_Short(pPX, _T("Value"), m_nValue, 0);
    PX_Short(pPX, _T("MinValue"), m_nMinValue, 0);
    PX_Short(pPX, _T("MaxValue"), m_nMaxValue, 100);
}
```

### Advanced Persistence

```cpp
// Serialize complex data
void CMyControlCtrl::DoPropExchange(CPropExchange* pPX)
{
    ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
    COleControl::DoPropExchange(pPX);

    if (pPX->IsLoading())
    {
        // Load data
        CString strData;
        PX_String(pPX, _T("CustomData"), strData, _T(""));
        LoadFromString(strData);
    }
    else
    {
        // Save data
        CString strData = SaveToString();
        PX_String(pPX, _T("CustomData"), strData, _T(""));
    }
}

CString CMyControlCtrl::SaveToString()
{
    CString result;
    result.Format(_T("%d,%d,%s"),
                  m_nValue, m_nMaxValue, m_strCaption);
    return result;
}

void CMyControlCtrl::LoadFromString(const CString& str)
{
    int pos1 = str.Find(_T(','));
    int pos2 = str.Find(_T(','), pos1 + 1);

    if (pos1 != -1 && pos2 != -1)
    {
        m_nValue = _ttoi(str.Left(pos1));
        m_nMaxValue = _ttoi(str.Mid(pos1 + 1, pos2 - pos1 - 1));
        m_strCaption = str.Mid(pos2 + 1);
    }
}
```

---

## 6. Control Containers

### Using ActiveX Control in MFC

```cpp
// In dialog header
class CMyDialog : public CDialogEx
{
public:
    CMyDialog(CWnd* pParent = NULL);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()
    DECLARE_EVENTSINK_MAP()

    // Event handlers
    afx_msg void OnValueChangedMycontrol1(short newValue);
    afx_msg void OnClickedMycontrol1();

private:
    CMyControl m_control;
};

// In dialog implementation
BEGIN_EVENTSINK_MAP(CMyDialog, CDialogEx)
    ON_EVENT(CMyDialog, IDC_MYCONTROL1, 1, OnValueChangedMycontrol1, VTS_I2)
    ON_EVENT(CMyDialog, IDC_MYCONTROL1, DISPID_CLICK, OnClickedMycontrol1, VTS_NONE)
END_EVENTSINK_MAP()

void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MYCONTROL1, m_control);
}

BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Set control properties
    m_control.SetCaption(_T("Counter"));
    m_control.SetValue(50);
    m_control.SetBackColor(RGB(200, 220, 255));

    return TRUE;
}

void CMyDialog::OnValueChangedMycontrol1(short newValue)
{
    CString msg;
    msg.Format(_T("Value changed to: %d"), newValue);
    SetDlgItemText(IDC_STATIC_STATUS, msg);
}

void CMyDialog::OnClickedMycontrol1()
{
    AfxMessageBox(_T("Control clicked!"));
}

// Call control methods
void CMyDialog::OnBtnIncrement()
{
    m_control.Increment();
}

void CMyDialog::OnBtnReset()
{
    m_control.Reset();
}
```

---

## 7. Advanced Features

### Ambient Properties

```cpp
void CMyControlCtrl::OnAmbientPropertyChange(DISPID dispid)
{
    switch (dispid)
    {
    case DISPID_AMBIENT_BACKCOLOR:
        // Container background color changed
        InvalidateControl();
        break;

    case DISPID_AMBIENT_FONT:
        // Container font changed
        InvalidateControl();
        break;

    case DISPID_AMBIENT_DISPLAYNAME:
        // Control name changed
        break;

    case DISPID_AMBIENT_USERMODE:
        // Design/runtime mode changed
        break;
    }

    COleControl::OnAmbientPropertyChange(dispid);
}

// Using ambient properties
void CMyControlCtrl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
    // Get ambient background color
    OLE_COLOR clrBack;
    if (GetAmbientBackColor(clrBack))
    {
        CBrush brush(TranslateColor(clrBack));
        pdc->FillRect(rcBounds, &brush);
    }

    // Get ambient font
    LPFONTDISP pFontDisp = GetAmbientFont();
    if (pFontDisp)
    {
        CFont* pFont = SelectFontObject(pdc, pFontDisp);
        // Use font...
        pdc->SelectObject(pFont);
    }
}
```

### Keyboard and Mouse Support

```cpp
BEGIN_MESSAGE_MAP(CMyControlCtrl, COleControl)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_KEYDOWN()
    ON_WM_CHAR()
END_MESSAGE_MAP()

void CMyControlCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bPressed = TRUE;
    InvalidateControl();
    FireClick();

    COleControl::OnLButtonDown(nFlags, point);
}

void CMyControlCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    switch (nChar)
    {
    case VK_UP:
        Increment();
        break;

    case VK_DOWN:
        Decrement();
        break;

    case VK_HOME:
        SetValue(m_nMinValue);
        break;

    case VK_END:
        SetValue(m_nMaxValue);
        break;
    }

    COleControl::OnKeyDown(nChar, nRepCnt, nFlags);
}
```

### Subclassing Windows Controls

```cpp
class CMyButtonCtrl : public COleControl
{
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs)
    {
        cs.lpszClass = _T("BUTTON");
        return COleControl::PreCreateWindow(cs);
    }

    virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
    {
        // Not used - Windows draws the button
    }
};
```

---

## 8. Deployment

### Registering Controls

```cpp
// Automatic registration
// regsvr32 MyControl.ocx

// Manual registration in code
HRESULT RegisterControl()
{
    CString strPath;
    GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH);
    strPath.ReleaseBuffer();

    HINSTANCE hLib = LoadLibrary(strPath);
    if (!hLib)
        return E_FAIL;

    typedef HRESULT (STDAPICALLTYPE *PFNDLLREGISTERSERVER)(void);
    PFNDLLREGISTERSERVER pfnDllRegisterServer =
        (PFNDLLREGISTERSERVER)GetProcAddress(hLib, "DllRegisterServer");

    HRESULT hr = E_FAIL;
    if (pfnDllRegisterServer)
    {
        hr = pfnDllRegisterServer();
    }

    FreeLibrary(hLib);
    return hr;
}
```

### CAB File Distribution

```cpp
// Create .inf file for CAB packaging
[version]
signature="$CHICAGO$"
AdvancedINF=2.0

[Add.Code]
MyControl.ocx=MyControl.ocx
mfc140.dll=mfc140.dll

[MyControl.ocx]
file=MyControl.ocx
clsid={12345678-1234-1234-1234-123456789ABC}
RegisterServer=yes
FileVersion=1,0,0,0

// Use cabarc.exe to create CAB
// cabarc -s 6144 n MyControl.cab MyControl.ocx MyControl.inf
```

---

## 9. Best Practices

```cpp
/*
ACTIVEX CONTROL BEST PRACTICES:

1. DESIGN
   - Keep interface simple
   - Use stock properties when possible
   - Provide sensible defaults
   - Document all properties/methods/events

2. PERFORMANCE
   - Minimize OnDraw operations
   - Use InvalidateControl() wisely
   - Cache expensive calculations
   - Optimize for common scenarios

3. COMPATIBILITY
   - Version your interfaces
   - Maintain backward compatibility
   - Test with different containers
   - Handle missing ambient properties

4. SAFETY
   - Validate all inputs
   - Check parameter ranges
   - Handle failures gracefully
   - Don't crash the container

5. TESTING
   - Test in multiple containers
   - Test property persistence
   - Test event firing
   - Test with different settings

6. DEPLOYMENT
   - Sign your controls
   - Provide proper versioning
   - Include dependencies
   - Create proper installers
*/
```

---

## Key Takeaways

1. **ActiveX controls** are COM-based reusable components
2. **Properties** expose control settings
3. **Methods** provide functionality
4. **Events** notify containers of changes
5. **Property pages** provide UI for properties
6. **Persistence** saves/loads control state
7. **Ambient properties** inherit from container
8. **Event sinking** handles control events
9. **Registration** required for use
10. **Testing** ensures compatibility

---

## Next Lesson Preview

In **Lesson 67: COM and MFC**, we'll explore:
- COM fundamentals
- Creating COM objects
- Automation
- Type libraries
- COM interfaces
- ATL integration

**Master ActiveX controls for reusable components!**
