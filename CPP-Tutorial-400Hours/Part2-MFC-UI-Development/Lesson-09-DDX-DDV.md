# Lesson 09: Dialog Data Exchange (DDX) and Validation (DDV)

**Duration**: 6-8 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding DDX
2. Understanding DDV  
3. Common DDX Functions
4. Common DDV Functions
5. UpdateData Function
6. Custom DDX/DDV Routines
7. Advanced Validation Techniques
8. Complete Examples

---

## 1. Understanding DDX

### What is DDX?

**Dialog Data Exchange (DDX)** automatically transfers data between:
- Dialog controls (UI) ↔ Member variables (data)

**Benefits**:
- No manual GetDlgItemText/SetDlgItemText calls
- Less code
- Centralized data transfer
- Automatic type conversion

### How DDX Works

```
UpdateData(TRUE):              UpdateData(FALSE):
Control → Variable             Variable → Control

User types "John"              m_strName = "Jane";
in edit control                UpdateData(FALSE);
                              ↓
UpdateData(TRUE);              Edit control shows "Jane"
↓
m_strName = "John"
```

### Basic DDX Example

```cpp
// In .h file
class CMyDialog : public CDialogEx
{
protected:
    CString m_strName;    // Variable for IDC_EDIT_NAME
    int m_nAge;           // Variable for IDC_EDIT_AGE
    BOOL m_bAccept;       // Variable for IDC_CHECK_ACCEPT

    virtual void DoDataExchange(CDataExchange* pDX);
};

// In .cpp file
void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    // Connect variables to controls
    DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
    DDX_Text(pDX, IDC_EDIT_AGE, m_nAge);
    DDX_Check(pDX, IDC_CHECK_ACCEPT, m_bAccept);
}

// Using the variables
void CMyDialog::OnBnClickedButtonSave()
{
    // Transfer: Control → Variable
    if (!UpdateData(TRUE))
    {
        return;  // Validation failed
    }

    // Now use the variables
    SaveData(m_strName, m_nAge, m_bAccept);
}

BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Set initial values
    m_strName = _T("Default Name");
    m_nAge = 25;
    m_bAccept = FALSE;

    // Transfer: Variable → Control
    UpdateData(FALSE);

    return TRUE;
}
```

---

## 2. Understanding DDV

### What is DDV?

**Dialog Data Validation (DDV)** validates data automatically when UpdateData(TRUE) is called.

**Benefits**:
- Automatic range checking
- User-friendly error messages
- Prevents invalid data entry
- Centralized validation

### Basic DDV Example

```cpp
void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    // DDX - transfer data
    DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
    DDX_Text(pDX, IDC_EDIT_AGE, m_nAge);

    // DDV - validate data
    DDV_MaxChars(pDX, m_strName, 50);        // Max 50 characters
    DDV_MinMaxInt(pDX, m_nAge, 0, 120);      // Age 0-120
}

// What happens
void CMyDialog::OnOK()
{
    if (!UpdateData(TRUE))
    {
        // DDV failed - user sees error message automatically
        // Dialog stays open
        // Focus set to offending control
        return;
    }

    // Data is valid
    CDialogEx::OnOK();
}
```

**Error Messages**:
```
DDV_MinMaxInt fails:
"Please enter an integer between 0 and 120."
(Focus set to IDC_EDIT_AGE)

DDV_MaxChars fails:
"Please enter no more than 50 characters."
```

---

## 3. Common DDX Functions

### Text DDX

```cpp
// String (CString)
DDX_Text(pDX, IDC_EDIT_NAME, m_strName);

// Integers
int m_nValue;
DDX_Text(pDX, IDC_EDIT_COUNT, m_nValue);

// Floating point
double m_dValue;
float m_fValue;
DDX_Text(pDX, IDC_EDIT_PRICE, m_dValue);
DDX_Text(pDX, IDC_EDIT_WEIGHT, m_fValue);

// UINT, LONG, DWORD
UINT m_uValue;
LONG m_lValue;
DWORD m_dwValue;
DDX_Text(pDX, IDC_EDIT_ID, m_uValue);
```

### Control DDX

```cpp
// Check box
BOOL m_bChecked;
DDX_Check(pDX, IDC_CHECK_OPTION, m_bChecked);

// Radio button (group)
int m_nRadio;  // 0 = first, 1 = second, 2 = third
DDX_Radio(pDX, IDC_RADIO_FIRST, m_nRadio);  // ID of FIRST radio

// List box (single selection)
int m_nListSel;
DDX_LBIndex(pDX, IDC_LIST_ITEMS, m_nListSel);

CString m_strListText;
DDX_LBString(pDX, IDC_LIST_ITEMS, m_strListText);

// Combo box
int m_nComboSel;
DDX_CBIndex(pDX, IDC_COMBO_CHOICES, m_nComboSel);

CString m_strComboText;
DDX_CBString(pDX, IDC_COMBO_CHOICES, m_strComboText);

// Slider
int m_nSliderPos;
DDX_Slider(pDX, IDC_SLIDER1, m_nSliderPos);

// Scroll bar
int m_nScrollPos;
DDX_Scroll(pDX, IDC_SCROLLBAR1, m_nScrollPos);
```

### Control Object DDX

```cpp
// Subclass control
CListBox m_listBox;
DDX_Control(pDX, IDC_LIST_ITEMS, m_listBox);

CComboBox m_comboBox;
DDX_Control(pDX, IDC_COMBO_CHOICES, m_comboBox);

CSliderCtrl m_slider;
DDX_Control(pDX, IDC_SLIDER1, m_slider);

// Now you can use member variable directly
void CMyDialog::OnButtonAdd()
{
    m_listBox.AddString(_T("New Item"));  // No GetDlgItem needed!
}
```

---

## 4. Common DDV Functions

### String Validation

```cpp
// Maximum length
DDV_MaxChars(pDX, m_strName, 50);

// Minimum length
if (pDX->m_bSaveAndValidate)
{
    if (m_strName.GetLength() < 3)
    {
        AfxMessageBox(_T("Name must be at least 3 characters"));
        pDX->Fail();
    }
}
```

### Numeric Validation

```cpp
// Integer range
DDV_MinMaxInt(pDX, m_nAge, 0, 120);
DDV_MinMaxUInt(pDX, m_uCount, 1, 1000);
DDV_MinMaxLong(pDX, m_lValue, -1000, 1000);

// Floating point range
DDV_MinMaxDouble(pDX, m_dPrice, 0.0, 9999.99);
DDV_MinMaxFloat(pDX, m_fWeight, 0.0f, 100.0f);

// Positive only
DDV_MinMaxInt(pDX, m_nCount, 1, INT_MAX);
```

### Custom Validation

```cpp
void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    DDX_Text(pDX, IDC_EDIT_EMAIL, m_strEmail);

    // Custom validation
    if (pDX->m_bSaveAndValidate)  // TRUE when UpdateData(TRUE)
    {
        // Validate email format
        if (m_strEmail.Find(_T("@")) == -1)
        {
            AfxMessageBox(_T("Invalid email address!"));
            pDX->PrepareEditCtrl(IDC_EDIT_EMAIL);  // Set focus to control
            pDX->Fail();  // Abort UpdateData
        }
    }
}
```

---

## 5. UpdateData Function

### UpdateData(TRUE) - Control to Variable

```cpp
void CMyDialog::OnBnClickedButtonSave()
{
    // Get data from controls → variables
    if (!UpdateData(TRUE))
    {
        // Validation failed
        // - Error message already shown
        // - Focus set to invalid control
        // - Dialog stays open
        return;
    }

    // Data is valid, use variables
    SaveToFile(m_strName, m_nAge);
}
```

**What Happens**:
1. For each DDX entry in DoDataExchange:
   - Get control value
   - Convert to variable type
   - Store in variable
2. For each DDV entry:
   - Validate value
   - If invalid: show error, set focus, abort
3. Return TRUE if successful, FALSE if validation failed

### UpdateData(FALSE) - Variable to Control

```cpp
BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Set variables
    m_strName = _T("John Doe");
    m_nAge = 30;
    m_bAccept = TRUE;

    // Transfer to controls
    UpdateData(FALSE);

    return TRUE;
}

void CMyDialog::OnBnClickedButtonReset()
{
    // Reset to defaults
    m_strName = _T("");
    m_nAge = 0;
    m_bAccept = FALSE;

    UpdateData(FALSE);  // Show in controls
}
```

**What Happens**:
1. For each DDX entry:
   - Convert variable to string/value
   - Set control value
2. DDV is NOT called (validation only on save)

---

## 6. Custom DDX/DDV Routines

### Creating Custom DDX

```cpp
// Custom DDX for date
void AFXAPI DDX_Date(CDataExchange* pDX, int nIDC, COleDateTime& value)
{
    HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);

    if (pDX->m_bSaveAndValidate)  // Control → Variable
    {
        CString strValue;
        ::GetWindowText(hWndCtrl, strValue.GetBuffer(256), 256);
        strValue.ReleaseBuffer();

        // Parse date (simple MM/DD/YYYY)
        int nMonth, nDay, nYear;
        if (_stscanf_s(strValue, _T("%d/%d/%d"), &nMonth, &nDay, &nYear) == 3)
        {
            value.SetDate(nYear, nMonth, nDay);
        }
        else
        {
            value.SetStatus(COleDateTime::invalid);
        }
    }
    else  // Variable → Control
    {
        CString strValue;
        if (value.GetStatus() == COleDateTime::valid)
        {
            strValue = value.Format(_T("%m/%d/%Y"));
        }

        AfxSetWindowText(hWndCtrl, strValue);
    }
}

// Usage
void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    DDX_Date(pDX, IDC_EDIT_DATE, m_dateValue);
}
```

### Creating Custom DDV

```cpp
// Custom DDV for phone number
void AFXAPI DDV_PhoneNumber(CDataExchange* pDX, const CString& value)
{
    if (!pDX->m_bSaveAndValidate)
        return;

    // Simple validation: must be 10 digits
    CString strDigits;
    for (int i = 0; i < value.GetLength(); i++)
    {
        if (isdigit(value[i]))
        {
            strDigits += value[i];
        }
    }

    if (strDigits.GetLength() != 10)
    {
        AfxMessageBox(_T("Please enter a valid 10-digit phone number"));
        pDX->Fail();
    }
}

// Usage
void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    DDX_Text(pDX, IDC_EDIT_PHONE, m_strPhone);
    DDV_PhoneNumber(pDX, m_strPhone);
}
```

---

## 7. Advanced Validation Techniques

### Conditional Validation

```cpp
void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    DDX_Check(pDX, IDC_CHECK_CUSTOM, m_bUseCustom);
    DDX_Text(pDX, IDC_EDIT_CUSTOM_VALUE, m_strCustomValue);

    // Only validate if checkbox is checked
    if (pDX->m_bSaveAndValidate && m_bUseCustom)
    {
        DDV_MaxChars(pDX, m_strCustomValue, 20);

        if (m_strCustomValue.IsEmpty())
        {
            AfxMessageBox(_T("Custom value required when option is checked"));
            pDX->PrepareEditCtrl(IDC_EDIT_CUSTOM_VALUE);
            pDX->Fail();
        }
    }
}
```

### Cross-Field Validation

```cpp
void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    DDX_Text(pDX, IDC_EDIT_START_DATE, m_dateStart);
    DDX_Text(pDX, IDC_EDIT_END_DATE, m_dateEnd);

    // Validate end date after start date
    if (pDX->m_bSaveAndValidate)
    {
        if (m_dateEnd < m_dateStart)
        {
            AfxMessageBox(_T("End date must be after start date"));
            pDX->PrepareEditCtrl(IDC_EDIT_END_DATE);
            pDX->Fail();
        }
    }
}
```

### Real-Time Validation

```cpp
// Validate on each keystroke
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_EN_CHANGE(IDC_EDIT_EMAIL, &CMyDialog::OnEnChangeEditEmail)
END_MESSAGE_MAP()

void CMyDialog::OnEnChangeEditEmail()
{
    CString strEmail;
    GetDlgItemText(IDC_EDIT_EMAIL, strEmail);

    // Show visual feedback
    CStatic* pStatus = (CStatic*)GetDlgItem(IDC_STATIC_STATUS);
    if (pStatus != nullptr)
    {
        if (IsValidEmail(strEmail))
        {
            pStatus->SetWindowText(_T("✓ Valid"));
            // Could change text color to green
        }
        else
        {
            pStatus->SetWindowText(_T("✗ Invalid"));
            // Could change text color to red
        }
    }
}

BOOL CMyDialog::IsValidEmail(const CString& strEmail)
{
    // Simple check
    return strEmail.Find(_T("@")) != -1 &&
           strEmail.Find(_T(".")) != -1;
}
```

---

## 8. Complete Examples

### Example: Registration Form

```cpp
class CRegistrationDialog : public CDialogEx
{
protected:
    CString m_strFirstName;
    CString m_strLastName;
    CString m_strEmail;
    CString m_strPhone;
    CString m_strPassword;
    CString m_strConfirmPassword;
    int m_nAge;
    BOOL m_bAgreeTerms;

    virtual void DoDataExchange(CDataExchange* pDX);
    virtual void OnOK();
};

void CRegistrationDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    // DDX
    DDX_Text(pDX, IDC_EDIT_FIRST_NAME, m_strFirstName);
    DDX_Text(pDX, IDC_EDIT_LAST_NAME, m_strLastName);
    DDX_Text(pDX, IDC_EDIT_EMAIL, m_strEmail);
    DDX_Text(pDX, IDC_EDIT_PHONE, m_strPhone);
    DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
    DDX_Text(pDX, IDC_EDIT_CONFIRM, m_strConfirmPassword);
    DDX_Text(pDX, IDC_EDIT_AGE, m_nAge);
    DDX_Check(pDX, IDC_CHECK_AGREE, m_bAgreeTerms);

    if (pDX->m_bSaveAndValidate)
    {
        // Validate first name
        if (m_strFirstName.IsEmpty())
        {
            AfxMessageBox(_T("First name is required"));
            pDX->PrepareEditCtrl(IDC_EDIT_FIRST_NAME);
            pDX->Fail();
        }

        // Validate email
        if (m_strEmail.Find(_T("@")) == -1)
        {
            AfxMessageBox(_T("Invalid email address"));
            pDX->PrepareEditCtrl(IDC_EDIT_EMAIL);
            pDX->Fail();
        }

        // Validate age
        if (m_nAge < 18)
        {
            AfxMessageBox(_T("Must be 18 or older"));
            pDX->PrepareEditCtrl(IDC_EDIT_AGE);
            pDX->Fail();
        }

        // Validate password match
        if (m_strPassword != m_strConfirmPassword)
        {
            AfxMessageBox(_T("Passwords do not match"));
            pDX->PrepareEditCtrl(IDC_EDIT_CONFIRM);
            pDX->Fail();
        }

        // Validate terms agreement
        if (!m_bAgreeTerms)
        {
            AfxMessageBox(_T("You must agree to the terms"));
            pDX->Fail();
        }
    }
}

void CRegistrationDialog::OnOK()
{
    if (!UpdateData(TRUE))
    {
        return;  // Validation failed
    }

    // All validations passed
    if (RegisterUser(m_strFirstName, m_strLastName, m_strEmail,
                    m_strPhone, m_strPassword, m_nAge))
    {
        MessageBox(_T("Registration successful!"));
        CDialogEx::OnOK();
    }
    else
    {
        MessageBox(_T("Registration failed!"));
    }
}
```

---

## Summary

### Key Takeaways

1. **DDX**: Automatic data transfer between controls and variables
2. **DDV**: Automatic validation with standard error messages
3. **UpdateData(TRUE)**: Control → Variable (with validation)
4. **UpdateData(FALSE)**: Variable → Control (no validation)
5. **Custom DDX/DDV**: Create specialized data exchange/validation
6. **m_bSaveAndValidate**: Check direction in DoDataExchange
7. **pDX->Fail()**: Abort UpdateData on validation error

### Best Practices

- Always call UpdateData(TRUE) before using member variables
- Always check UpdateData return value
- Use DDV for simple validation (range, length)
- Use custom validation for complex rules
- Validate on save, not on every keystroke (usually)
- Provide clear error messages

### Next Steps

In **Lesson 10: Resource Files and Resource Editor**, you'll learn:
- .rc file format
- Resource IDs
- Dialog templates
- String tables
- Icons and bitmaps
- Localization

---

**Next Lesson**: [Lesson 10: Resource Files and Resource Editor](Lesson-10-Resource-Files-Resource-Editor.md)
