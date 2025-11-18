# Lesson 71: Internet Programming (WinInet)

**Duration**: 6 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to WinInet
2. CInternetSession Class
3. HTTP Operations
4. FTP Operations
5. File Download/Upload
6. Best Practices

---

## 1. Introduction to WinInet

### WinInet API Overview

```cpp
/*
WININET PROVIDES:
- HTTP/HTTPS support
- FTP support
- Gopher support (deprecated)
- Cookie management
- Proxy support

MFC WININET CLASSES:
- CInternetSession: Session management
- CHttpConnection: HTTP connections
- CHttpFile: HTTP requests
- CFtpConnection: FTP connections
- CFtpFileFind: FTP file operations
*/
```

---

## 2. CInternetSession Class

### Creating Internet Session

```cpp
class CInternetHelper
{
public:
    CInternetHelper(LPCTSTR lpszAgent = NULL);
    ~CInternetHelper();

    BOOL DownloadFile(LPCTSTR lpszURL, LPCTSTR lpszLocalFile);
    CString GetWebPage(LPCTSTR lpszURL);

private:
    CInternetSession m_session;
};

CInternetHelper::CInternetHelper(LPCTSTR lpszAgent)
    : m_session(lpszAgent ? lpszAgent : _T("MFC WinInet Client"))
{
}

CInternetHelper::~CInternetHelper()
{
    m_session.Close();
}

BOOL CInternetHelper::DownloadFile(LPCTSTR lpszURL, LPCTSTR lpszLocalFile)
{
    try
    {
        CStdioFile* pFile = m_session.OpenURL(lpszURL);

        if (!pFile)
            return FALSE;

        CFile outFile;
        if (!outFile.Open(lpszLocalFile, CFile::modeCreate | CFile::modeWrite))
        {
            delete pFile;
            return FALSE;
        }

        BYTE buffer[4096];
        UINT nRead;

        while ((nRead = pFile->Read(buffer, sizeof(buffer))) > 0)
        {
            outFile.Write(buffer, nRead);
        }

        outFile.Close();
        delete pFile;

        return TRUE;
    }
    catch (CInternetException* e)
    {
        e->Delete();
        return FALSE;
    }
}

CString CInternetHelper::GetWebPage(LPCTSTR lpszURL)
{
    CString strResult;

    try
    {
        CStdioFile* pFile = m_session.OpenURL(lpszURL);

        if (pFile)
        {
            CString strLine;
            while (pFile->ReadString(strLine))
            {
                strResult += strLine + _T("\r\n");
            }

            delete pFile;
        }
    }
    catch (CInternetException* e)
    {
        e->Delete();
    }

    return strResult;
}
```

---

## 3. HTTP Operations

### HTTP GET/POST

```cpp
class CHTTPClient
{
public:
    CHTTPClient();
    ~CHTTPClient();

    CString HttpGet(LPCTSTR lpszURL);
    CString HttpPost(LPCTSTR lpszURL, LPCTSTR lpszData);
    BOOL DownloadFile(LPCTSTR lpszURL, LPCTSTR lpszLocalPath,
                     HWND hWndProgress = NULL);

private:
    BOOL ParseURL(LPCTSTR lpszURL, CString& strServer,
                 CString& strObject, INTERNET_PORT& nPort);

    CInternetSession m_session;
};

CHTTPClient::CHTTPClient()
    : m_session(_T("MFC HTTP Client/1.0"))
{
}

CHTTPClient::~CHTTPClient()
{
    m_session.Close();
}

BOOL CHTTPClient::ParseURL(LPCTSTR lpszURL, CString& strServer,
                          CString& strObject, INTERNET_PORT& nPort)
{
    DWORD dwServiceType;
    if (!AfxParseURL(lpszURL, dwServiceType, strServer, strObject, nPort))
    {
        return FALSE;
    }

    if (dwServiceType != INTERNET_SERVICE_HTTP &&
        dwServiceType != INTERNET_SERVICE_HTTPS)
    {
        return FALSE;
    }

    if (nPort == 0)
    {
        nPort = (dwServiceType == INTERNET_SERVICE_HTTPS) ? INTERNET_DEFAULT_HTTPS_PORT
                                                           : INTERNET_DEFAULT_HTTP_PORT;
    }

    return TRUE;
}

CString CHTTPClient::HttpGet(LPCTSTR lpszURL)
{
    CString strResult;

    try
    {
        CString strServer, strObject;
        INTERNET_PORT nPort;

        if (!ParseURL(lpszURL, strServer, strObject, nPort))
        {
            return _T("");
        }

        CHttpConnection* pConnection = m_session.GetHttpConnection(
            strServer, nPort);

        if (!pConnection)
            return _T("");

        CHttpFile* pFile = pConnection->OpenRequest(
            CHttpConnection::HTTP_VERB_GET, strObject);

        if (!pFile)
        {
            delete pConnection;
            return _T("");
        }

        pFile->SendRequest();

        DWORD dwStatusCode;
        pFile->QueryInfoStatusCode(dwStatusCode);

        if (dwStatusCode == HTTP_STATUS_OK)
        {
            CString strLine;
            while (pFile->ReadString(strLine))
            {
                strResult += strLine + _T("\r\n");
            }
        }

        delete pFile;
        delete pConnection;
    }
    catch (CInternetException* e)
    {
        e->Delete();
    }

    return strResult;
}

CString CHTTPClient::HttpPost(LPCTSTR lpszURL, LPCTSTR lpszData)
{
    CString strResult;

    try
    {
        CString strServer, strObject;
        INTERNET_PORT nPort;

        if (!ParseURL(lpszURL, strServer, strObject, nPort))
        {
            return _T("");
        }

        CHttpConnection* pConnection = m_session.GetHttpConnection(
            strServer, nPort);

        if (!pConnection)
            return _T("");

        CHttpFile* pFile = pConnection->OpenRequest(
            CHttpConnection::HTTP_VERB_POST, strObject);

        if (!pFile)
        {
            delete pConnection;
            return _T("");
        }

        CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded\r\n");

        pFile->SendRequest(strHeaders, (LPVOID)(LPCTSTR)lpszData,
                          _tcslen(lpszData));

        DWORD dwStatusCode;
        pFile->QueryInfoStatusCode(dwStatusCode);

        if (dwStatusCode == HTTP_STATUS_OK)
        {
            CString strLine;
            while (pFile->ReadString(strLine))
            {
                strResult += strLine + _T("\r\n");
            }
        }

        delete pFile;
        delete pConnection;
    }
    catch (CInternetException* e)
    {
        e->Delete();
    }

    return strResult;
}
```

---

## 4. FTP Operations

### FTP Client Implementation

```cpp
class CFTPClient
{
public:
    CFTPClient();
    ~CFTPClient();

    BOOL Connect(LPCTSTR lpszServer, LPCTSTR lpszUser, LPCTSTR lpszPassword);
    void Disconnect();

    BOOL UploadFile(LPCTSTR lpszLocalFile, LPCTSTR lpszRemoteFile);
    BOOL DownloadFile(LPCTSTR lpszRemoteFile, LPCTSTR lpszLocalFile);
    BOOL DeleteFile(LPCTSTR lpszRemoteFile);
    BOOL GetFileList(CStringArray& files);

private:
    CInternetSession m_session;
    CFtpConnection* m_pConnection;
};

CFTPClient::CFTPClient()
    : m_session(_T("MFC FTP Client")), m_pConnection(NULL)
{
}

CFTPClient::~CFTPClient()
{
    Disconnect();
}

BOOL CFTPClient::Connect(LPCTSTR lpszServer, LPCTSTR lpszUser, LPCTSTR lpszPassword)
{
    try
    {
        m_pConnection = m_session.GetFtpConnection(
            lpszServer, lpszUser, lpszPassword);

        return (m_pConnection != NULL);
    }
    catch (CInternetException* e)
    {
        e->Delete();
        return FALSE;
    }
}

void CFTPClient::Disconnect()
{
    if (m_pConnection)
    {
        m_pConnection->Close();
        delete m_pConnection;
        m_pConnection = NULL;
    }
}

BOOL CFTPClient::UploadFile(LPCTSTR lpszLocalFile, LPCTSTR lpszRemoteFile)
{
    if (!m_pConnection)
        return FALSE;

    try
    {
        return m_pConnection->PutFile(lpszLocalFile, lpszRemoteFile);
    }
    catch (CInternetException* e)
    {
        e->Delete();
        return FALSE;
    }
}

BOOL CFTPClient::DownloadFile(LPCTSTR lpszRemoteFile, LPCTSTR lpszLocalFile)
{
    if (!m_pConnection)
        return FALSE;

    try
    {
        return m_pConnection->GetFile(lpszRemoteFile, lpszLocalFile);
    }
    catch (CInternetException* e)
    {
        e->Delete();
        return FALSE;
    }
}

BOOL CFTPClient::GetFileList(CStringArray& files)
{
    if (!m_pConnection)
        return FALSE;

    try
    {
        CFtpFileFind finder(m_pConnection);

        BOOL bWorking = finder.FindFile(NULL);

        while (bWorking)
        {
            bWorking = finder.FindNextFile();

            if (!finder.IsDots())
            {
                files.Add(finder.GetFileName());
            }
        }

        finder.Close();
        return TRUE;
    }
    catch (CInternetException* e)
    {
        e->Delete();
        return FALSE;
    }
}
```

---

## Key Takeaways

1. **WinInet** simplifies Internet programming
2. **CInternetSession** manages connections
3. **HTTP** supports GET and POST
4. **FTP** enables file transfers
5. **Exception handling** is critical
6. **Best practices** ensure reliability

---

## Next Lesson Preview

In **Lesson 72: Registry Access**, we'll explore:
- Reading registry values
- Writing registry values
- Registry keys
- Security considerations

**Master Internet programming with WinInet!**
