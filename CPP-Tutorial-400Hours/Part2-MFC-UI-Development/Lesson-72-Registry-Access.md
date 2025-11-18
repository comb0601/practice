# Lesson 72: Registry Access

**Duration**: 5 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to Windows Registry
2. Reading Registry Values
3. Writing Registry Values
4. Registry Key Management
5. Security and Best Practices

---

## 1. Introduction to Windows Registry

### Registry Structure

```cpp
/*
REGISTRY HIVES:
- HKEY_CLASSES_ROOT (HKCR): File associations, COM objects
- HKEY_CURRENT_USER (HKCU): Current user settings
- HKEY_LOCAL_MACHINE (HKLM): System-wide settings
- HKEY_USERS (HKU): All user profiles
- HKEY_CURRENT_CONFIG (HKCC): Hardware configuration

TYPICAL USAGE:
- Store application settings
- Save user preferences
- Register file associations
- Store license information
*/
```

---

## 2. Reading Registry Values

### CRegKey Class

```cpp
class CRegistryHelper
{
public:
    static BOOL ReadString(HKEY hKeyRoot, LPCTSTR lpszKey,
                          LPCTSTR lpszValue, CString& strData);
    static BOOL ReadDWORD(HKEY hKeyRoot, LPCTSTR lpszKey,
                         LPCTSTR lpszValue, DWORD& dwData);
    static BOOL WriteString(HKEY hKeyRoot, LPCTSTR lpszKey,
                           LPCTSTR lpszValue, LPCTSTR lpszData);
    static BOOL WriteDWORD(HKEY hKeyRoot, LPCTSTR lpszKey,
                          LPCTSTR lpszValue, DWORD dwData);
};

BOOL CRegistryHelper::ReadString(HKEY hKeyRoot, LPCTSTR lpszKey,
                                LPCTSTR lpszValue, CString& strData)
{
    CRegKey reg;
    LONG lResult = reg.Open(hKeyRoot, lpszKey, KEY_READ);

    if (lResult != ERROR_SUCCESS)
        return FALSE;

    TCHAR szBuffer[256];
    ULONG nChars = 256;

    lResult = reg.QueryStringValue(lpszValue, szBuffer, &nChars);

    if (lResult == ERROR_SUCCESS)
    {
        strData = szBuffer;
        reg.Close();
        return TRUE;
    }

    reg.Close();
    return FALSE;
}

BOOL CRegistryHelper::ReadDWORD(HKEY hKeyRoot, LPCTSTR lpszKey,
                               LPCTSTR lpszValue, DWORD& dwData)
{
    CRegKey reg;
    LONG lResult = reg.Open(hKeyRoot, lpszKey, KEY_READ);

    if (lResult != ERROR_SUCCESS)
        return FALSE;

    lResult = reg.QueryDWORDValue(lpszValue, dwData);

    reg.Close();
    return (lResult == ERROR_SUCCESS);
}
```

---

## 3. Writing Registry Values

```cpp
BOOL CRegistryHelper::WriteString(HKEY hKeyRoot, LPCTSTR lpszKey,
                                 LPCTSTR lpszValue, LPCTSTR lpszData)
{
    CRegKey reg;
    LONG lResult = reg.Create(hKeyRoot, lpszKey);

    if (lResult != ERROR_SUCCESS)
        return FALSE;

    lResult = reg.SetStringValue(lpszValue, lpszData);

    reg.Close();
    return (lResult == ERROR_SUCCESS);
}

BOOL CRegistryHelper::WriteDWORD(HKEY hKeyRoot, LPCTSTR lpszKey,
                                LPCTSTR lpszValue, DWORD dwData)
{
    CRegKey reg;
    LONG lResult = reg.Create(hKeyRoot, lpszKey);

    if (lResult != ERROR_SUCCESS)
        return FALSE;

    lResult = reg.SetDWORDValue(lpszValue, dwData);

    reg.Close();
    return (lResult == ERROR_SUCCESS);
}

// Application settings example
class CAppSettings
{
public:
    static BOOL LoadSettings();
    static BOOL SaveSettings();

    static CString GetUsername();
    static void SetUsername(const CString& username);
    static int GetWindowX();
    static void SetWindowX(int x);

private:
    static CString m_strUsername;
    static int m_nWindowX;
    static int m_nWindowY;
    static BOOL m_bMaximized;
};

BOOL CAppSettings::LoadSettings()
{
    CRegistryHelper::ReadString(HKEY_CURRENT_USER,
        _T("Software\\MyApp\\Settings"), _T("Username"), m_strUsername);

    DWORD dwValue;
    if (CRegistryHelper::ReadDWORD(HKEY_CURRENT_USER,
        _T("Software\\MyApp\\Settings"), _T("WindowX"), dwValue))
    {
        m_nWindowX = dwValue;
    }

    return TRUE;
}

BOOL CAppSettings::SaveSettings()
{
    CRegistryHelper::WriteString(HKEY_CURRENT_USER,
        _T("Software\\MyApp\\Settings"), _T("Username"), m_strUsername);

    CRegistryHelper::WriteDWORD(HKEY_CURRENT_USER,
        _T("Software\\MyApp\\Settings"), _T("WindowX"), m_nWindowX);

    return TRUE;
}
```

---

## 4. Registry Key Management

```cpp
class CRegistryKeyManager
{
public:
    static BOOL KeyExists(HKEY hKeyRoot, LPCTSTR lpszKey);
    static BOOL CreateKey(HKEY hKeyRoot, LPCTSTR lpszKey);
    static BOOL DeleteKey(HKEY hKeyRoot, LPCTSTR lpszKey);
    static BOOL DeleteValue(HKEY hKeyRoot, LPCTSTR lpszKey, LPCTSTR lpszValue);
    static BOOL EnumerateSubKeys(HKEY hKeyRoot, LPCTSTR lpszKey, CStringArray& subKeys);
};

BOOL CRegistryKeyManager::KeyExists(HKEY hKeyRoot, LPCTSTR lpszKey)
{
    CRegKey reg;
    LONG lResult = reg.Open(hKeyRoot, lpszKey, KEY_READ);
    reg.Close();
    return (lResult == ERROR_SUCCESS);
}

BOOL CRegistryKeyManager::CreateKey(HKEY hKeyRoot, LPCTSTR lpszKey)
{
    CRegKey reg;
    LONG lResult = reg.Create(hKeyRoot, lpszKey);
    reg.Close();
    return (lResult == ERROR_SUCCESS);
}

BOOL CRegistryKeyManager::DeleteKey(HKEY hKeyRoot, LPCTSTR lpszKey)
{
    return (RegDeleteKey(hKeyRoot, lpszKey) == ERROR_SUCCESS);
}

BOOL CRegistryKeyManager::EnumerateSubKeys(HKEY hKeyRoot, LPCTSTR lpszKey,
                                          CStringArray& subKeys)
{
    CRegKey reg;
    LONG lResult = reg.Open(hKeyRoot, lpszKey, KEY_READ);

    if (lResult != ERROR_SUCCESS)
        return FALSE;

    DWORD dwIndex = 0;
    TCHAR szKeyName[256];

    while (TRUE)
    {
        DWORD dwNameLen = 256;
        lResult = RegEnumKeyEx(reg.m_hKey, dwIndex, szKeyName, &dwNameLen,
                               NULL, NULL, NULL, NULL);

        if (lResult != ERROR_SUCCESS)
            break;

        subKeys.Add(szKeyName);
        dwIndex++;
    }

    reg.Close();
    return TRUE;
}
```

---

## 5. Security and Best Practices

```cpp
/*
REGISTRY BEST PRACTICES:

1. USE APPROPRIATE HIVE
   - HKCU for user-specific settings
   - HKLM for system-wide (requires admin)

2. SECURITY
   - Always check for errors
   - Use least privilege
   - Validate data read from registry
   - Don't store sensitive data

3. PERFORMANCE
   - Cache values when possible
   - Close registry keys promptly
   - Batch operations

4. CLEANUP
   - Remove keys on uninstall
   - Don't leave orphaned entries
   - Test uninstall process

5. COMPATIBILITY
   - Handle missing keys gracefully
   - Provide default values
   - Version your settings
*/
```

---

## Key Takeaways

1. **Registry** stores application configuration
2. **CRegKey** simplifies registry access
3. **HKCU** for user settings
4. **HKLM** for system settings
5. **Error handling** is essential
6. **Security** requires validation
7. **Best practices** ensure reliability

---

## Next Lesson Preview

In **Lesson 73: INI Files and Configuration**, we'll explore:
- Reading INI files
- Writing INI files
- Configuration management
- INI vs Registry

**Master registry programming for configuration!**
