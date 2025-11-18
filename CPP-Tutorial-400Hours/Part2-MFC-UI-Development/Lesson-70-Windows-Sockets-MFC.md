# Lesson 70: Windows Sockets in MFC

**Duration**: 7 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Windows Sockets
2. CAsyncSocket Class
3. CSocket Class
4. Building a TCP Server
5. Building a TCP Client
6. Best Practices

---

## 1. Introduction to Windows Sockets

### Socket Programming Basics

```cpp
/*
SOCKET TYPES:
- Stream Sockets (TCP): Reliable, connection-oriented
- Datagram Sockets (UDP): Unreliable, connectionless

MFC SOCKET CLASSES:
- CAsyncSocket: Asynchronous, event-driven
- CSocket: Synchronous, easier to use

TYPICAL FLOW:
Server: Socket -> Bind -> Listen -> Accept -> Send/Receive -> Close
Client: Socket -> Connect -> Send/Receive -> Close
*/
```

---

## 2. CAsyncSocket Class

### Basic Server Implementation

```cpp
// ServerSocket.h
class CServerSocket : public CAsyncSocket
{
public:
    CServerSocket();
    virtual ~CServerSocket();

    virtual void OnAccept(int nErrorCode);

    CWnd* m_pWnd;
};

class CClientSocket : public CAsyncSocket
{
public:
    CClientSocket();
    virtual ~CClientSocket();

    virtual void OnReceive(int nErrorCode);
    virtual void OnClose(int nErrorCode);

    CWnd* m_pWnd;
};

// ServerSocket.cpp
CServerSocket::CServerSocket()
{
    m_pWnd = NULL;
}

CServerSocket::~CServerSocket()
{
}

void CServerSocket::OnAccept(int nErrorCode)
{
    if (nErrorCode == 0 && m_pWnd)
    {
        m_pWnd->PostMessage(WM_USER + 1, 0, 0);
    }
    CAsyncSocket::OnAccept(nErrorCode);
}

CClientSocket::CClientSocket()
{
    m_pWnd = NULL;
}

CClientSocket::~CClientSocket()
{
}

void CClientSocket::OnReceive(int nErrorCode)
{
    if (nErrorCode == 0 && m_pWnd)
    {
        m_pWnd->PostMessage(WM_USER + 2, 0, 0);
    }
    CAsyncSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode)
{
    if (m_pWnd)
    {
        m_pWnd->PostMessage(WM_USER + 3, 0, 0);
    }
    Close();
    CAsyncSocket::OnClose(nErrorCode);
}

// Server Dialog
class CServerDlg : public CDialogEx
{
public:
    CServerDlg(CWnd* pParent = NULL);

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg void OnBtnStart();
    afx_msg void OnBtnStop();
    afx_msg LRESULT OnAccept(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnReceive(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnClose(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()

private:
    CServerSocket m_serverSocket;
    CClientSocket m_clientSocket;
    CEdit m_editLog;
    int m_nPort;
};

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
    ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
    ON_MESSAGE(WM_USER + 1, OnAccept)
    ON_MESSAGE(WM_USER + 2, OnReceive)
    ON_MESSAGE(WM_USER + 3, OnClose)
END_MESSAGE_MAP()

void CServerDlg::OnBtnStart()
{
    m_nPort = 5000;

    if (!m_serverSocket.Create(m_nPort))
    {
        AfxMessageBox(_T("Failed to create server socket"));
        return;
    }

    if (!m_serverSocket.Listen())
    {
        AfxMessageBox(_T("Failed to listen"));
        return;
    }

    m_serverSocket.m_pWnd = this;

    CString msg;
    msg.Format(_T("Server listening on port %d\r\n"), m_nPort);
    AppendLog(msg);
}

LRESULT CServerDlg::OnAccept(WPARAM wParam, LPARAM lParam)
{
    if (!m_serverSocket.Accept(m_clientSocket))
    {
        AfxMessageBox(_T("Failed to accept connection"));
        return 0;
    }

    m_clientSocket.m_pWnd = this;

    CString strIP;
    UINT nPort;
    m_clientSocket.GetPeerName(strIP, nPort);

    CString msg;
    msg.Format(_T("Client connected from %s:%d\r\n"), strIP, nPort);
    AppendLog(msg);

    return 0;
}

LRESULT CServerDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{
    char buffer[1024];
    int nReceived = m_clientSocket.Receive(buffer, sizeof(buffer) - 1);

    if (nReceived > 0)
    {
        buffer[nReceived] = '\0';

        CString msg;
        msg.Format(_T("Received: %s\r\n"), CString(buffer));
        AppendLog(msg);

        // Echo back
        m_clientSocket.Send(buffer, nReceived);
    }

    return 0;
}
```

---

## 3. CSocket Class

### Simplified TCP Communication

```cpp
// SimpleTCPServer.h
class CSimpleTCPServer
{
public:
    CSimpleTCPServer();
    ~CSimpleTCPServer();

    BOOL Start(UINT nPort);
    void Stop();
    BOOL Send(const CString& data);

private:
    static UINT ListenThreadProc(LPVOID pParam);
    static UINT ClientThreadProc(LPVOID pParam);

    CSocket m_listenSocket;
    BOOL m_bRunning;
};

// SimpleTCPServer.cpp
CSimpleTCPServer::CSimpleTCPServer()
    : m_bRunning(FALSE)
{
}

CSimpleTCPServer::~CSimpleTCPServer()
{
    Stop();
}

BOOL CSimpleTCPServer::Start(UINT nPort)
{
    if (!m_listenSocket.Create(nPort))
    {
        return FALSE;
    }

    if (!m_listenSocket.Listen())
    {
        return FALSE;
    }

    m_bRunning = TRUE;

    AfxBeginThread(ListenThreadProc, this);

    return TRUE;
}

void CSimpleTCPServer::Stop()
{
    m_bRunning = FALSE;
    m_listenSocket.Close();
}

UINT CSimpleTCPServer::ListenThreadProc(LPVOID pParam)
{
    CSimpleTCPServer* pServer = (CSimpleTCPServer*)pParam;

    while (pServer->m_bRunning)
    {
        CSocket* pClientSocket = new CSocket;

        if (pServer->m_listenSocket.Accept(*pClientSocket))
        {
            AfxBeginThread(ClientThreadProc, pClientSocket);
        }
        else
        {
            delete pClientSocket;
        }
    }

    return 0;
}

UINT CSimpleTCPServer::ClientThreadProc(LPVOID pParam)
{
    CSocket* pSocket = (CSocket*)pParam;

    char buffer[1024];

    while (TRUE)
    {
        int nReceived = pSocket->Receive(buffer, sizeof(buffer) - 1);

        if (nReceived <= 0)
        {
            break;
        }

        buffer[nReceived] = '\0';

        // Echo back
        pSocket->Send(buffer, nReceived);
    }

    pSocket->Close();
    delete pSocket;

    return 0;
}
```

---

## Key Takeaways

1. **CAsyncSocket** provides event-driven networking
2. **CSocket** offers synchronous operation
3. **TCP** ensures reliable data transfer
4. **Server** listens and accepts connections
5. **Client** connects to server
6. **Threading** handles multiple clients
7. **Error handling** is critical
8. **Best practices** ensure robust applications

---

## Next Lesson Preview

In **Lesson 71: Internet Programming (WinInet)**, we'll explore:
- HTTP requests
- FTP operations
- Internet sessions
- Download/upload files

**Master socket programming for network applications!**
