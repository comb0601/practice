# Lesson 73: INI Files and Configuration

**Duration**: 4 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to INI Files
2. Reading INI Files
3. Writing INI Files
4. CWinApp Profile Functions
5. INI vs Registry
6. Best Practices

---

## 1. Introduction to INI Files

### INI File Structure

```ini
; Comments start with semicolon
[Section1]
Key1=Value1
Key2=Value2

[Section2]
Key3=Value3
Key4=Value4
```

---

## 2. Reading INI Files

### GetPrivateProfileString

```cpp
class CINIFile
{
public:
    CINIFile(LPCTSTR lpszFilePath);

    CString ReadString(LPCTSTR lpszSection, LPCTSTR lpszKey,
                      LPCTSTR lpszDefault = _T(""));
    int ReadInt(LPCTSTR lpszSection, LPCTSTR lpszKey, int nDefault = 0);
    double ReadDouble(LPCTSTR lpszSection, LPCTSTR lpszKey, double dDefault = 0.0);
    BOOL ReadBool(LPCTSTR lpszSection, LPCTSTR lpszKey, BOOL bDefault = FALSE);

private:
    CString m_strFilePath;
};

CINIFile::CINIFile(LPCTSTR lpszFilePath)
    : m_strFilePath(lpszFilePath)
{
}

CString CINIFile::ReadString(LPCTSTR lpszSection, LPCTSTR lpszKey,
                            LPCTSTR lpszDefault)
{
    TCHAR szBuffer[512];
    GetPrivateProfileString(lpszSection, lpszKey, lpszDefault,
                           szBuffer, 512, m_strFilePath);
    return CString(szBuffer);
}

int CINIFile::ReadInt(LPCTSTR lpszSection, LPCTSTR lpszKey, int nDefault)
{
    return GetPrivateProfileInt(lpszSection, lpszKey, nDefault, m_strFilePath);
}

double CINIFile::ReadDouble(LPCTSTR lpszSection, LPCTSTR lpszKey, double dDefault)
{
    CString str = ReadString(lpszSection, lpszKey, _T(""));
    return str.IsEmpty() ? dDefault : _ttof(str);
}

BOOL CINIFile::ReadBool(LPCTSTR lpszSection, LPCTSTR lpszKey, BOOL bDefault)
{
    return (ReadInt(lpszSection, lpszKey, bDefault ? 1 : 0) != 0);
}
```

---

## 3. Writing INI Files

```cpp
class CINIFile
{
public:
    BOOL WriteString(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszValue);
    BOOL WriteInt(LPCTSTR lpszSection, LPCTSTR lpszKey, int nValue);
    BOOL WriteDouble(LPCTSTR lpszSection, LPCTSTR lpszKey, double dValue);
    BOOL WriteBool(LPCTSTR lpszSection, LPCTSTR lpszKey, BOOL bValue);
};

BOOL CINIFile::WriteString(LPCTSTR lpszSection, LPCTSTR lpszKey,
                          LPCTSTR lpszValue)
{
    return WritePrivateProfileString(lpszSection, lpszKey, lpszValue,
                                    m_strFilePath);
}

BOOL CINIFile::WriteInt(LPCTSTR lpszSection, LPCTSTR lpszKey, int nValue)
{
    CString str;
    str.Format(_T("%d"), nValue);
    return WriteString(lpszSection, lpszKey, str);
}

BOOL CINIFile::WriteDouble(LPCTSTR lpszSection, LPCTSTR lpszKey, double dValue)
{
    CString str;
    str.Format(_T("%.10f"), dValue);
    return WriteString(lpszSection, lpszKey, str);
}

BOOL CINIFile::WriteBool(LPCTSTR lpszSection, LPCTSTR lpszKey, BOOL bValue)
{
    return WriteInt(lpszSection, lpszKey, bValue ? 1 : 0);
}
```

---

## 4. CWinApp Profile Functions

```cpp
// Using CWinApp for application settings
void CMyApp::LoadSettings()
{
    m_strUsername = GetProfileString(_T("Settings"), _T("Username"), _T(""));
    m_nWindowX = GetProfileInt(_T("Settings"), _T("WindowX"), 100);
    m_nWindowY = GetProfileInt(_T("Settings"), _T("WindowY"), 100);
}

void CMyApp::SaveSettings()
{
    WriteProfileString(_T("Settings"), _T("Username"), m_strUsername);
    WriteProfileInt(_T("Settings"), _T("WindowX"), m_nWindowX);
    WriteProfileInt(_T("Settings"), _T("WindowY"), m_nWindowY);
}
```

---

## 5. INI vs Registry

```cpp
/*
INI FILES:
+ Simple text format
+ Easy to edit manually
+ Portable
+ No admin rights needed
- Limited security
- Slower than registry
- Limited to local files

REGISTRY:
+ Faster access
+ Better security
+ System-wide settings
+ Automatic backup
- Requires admin for HKLM
- Not portable
- Complex structure

RECOMMENDATION:
- Use INI for simple apps, portable settings
- Use Registry for system integration, complex apps
*/
```

---

## 6. Best Practices

```cpp
/*
INI FILE BEST PRACTICES:

1. FILE LOCATION
   - Store in application folder
   - Or in %APPDATA% for user settings
   - Never in Windows folder

2. STRUCTURE
   - Use meaningful section names
   - Group related settings
   - Document with comments

3. DEFAULTS
   - Always provide defaults
   - Handle missing values
   - Validate loaded data

4. SECURITY
   - Don't store passwords
   - Encrypt sensitive data
   - Validate file integrity

5. COMPATIBILITY
   - Version your settings
   - Migrate old settings
   - Handle format changes
*/
```

---

## Key Takeaways

1. **INI files** provide simple configuration
2. **GetPrivateProfileString** reads values
3. **WritePrivateProfileString** writes values
4. **CWinApp** integrates INI support
5. **INI vs Registry** depends on needs
6. **Best practices** ensure reliability

---

## Next Lesson Preview

In **Lesson 74: Clipboard Operations**, we'll explore:
- Copying text to clipboard
- Pasting from clipboard
- Custom clipboard formats
- Clipboard monitoring

**Master configuration management!**
