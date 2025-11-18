# Lesson 61: Multithreading in MFC

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Multithreading
2. Understanding Threads in Windows
3. MFC Thread Architecture
4. Creating Threads with AfxBeginThread
5. Thread Types: Worker vs UI Threads
6. Thread Management and Control
7. Thread Communication
8. Thread Safety and Best Practices
9. Practical Examples
10. Common Pitfalls and Solutions
11. Exercises and Projects

---

## 1. Introduction to Multithreading

### What is Multithreading?

Multithreading is a programming technique that allows multiple threads of execution to run concurrently within a single process. Each thread represents an independent path of execution, enabling applications to perform multiple tasks simultaneously.

### Benefits of Multithreading

1. **Improved Responsiveness**: Keep UI responsive while performing long operations
2. **Better Performance**: Utilize multiple CPU cores effectively
3. **Concurrent Operations**: Execute multiple tasks simultaneously
4. **Resource Sharing**: Threads share process memory and resources
5. **Efficient I/O**: Handle multiple I/O operations concurrently

### When to Use Multithreading

```cpp
// Scenarios where multithreading is beneficial:
// 1. Long-running calculations
// 2. File I/O operations
// 3. Network communication
// 4. Database operations
// 5. Background processing
// 6. Real-time data processing
// 7. Parallel algorithms
```

---

## 2. Understanding Threads in Windows

### Process vs Thread

A **process** is an instance of a running application with its own memory space. A **thread** is a unit of execution within a process.

```cpp
/*
PROCESS
├── Memory Space
│   ├── Code Segment
│   ├── Data Segment
│   ├── Heap
│   └── Stack (per thread)
├── Thread 1 (Primary)
├── Thread 2 (Worker)
└── Thread 3 (Worker)
*/
```

### Thread Components

Each thread has:
- **Thread ID**: Unique identifier
- **Stack**: Local variables and function calls
- **Register Set**: CPU register values
- **Thread-Local Storage (TLS)**: Thread-specific data
- **Priority**: Scheduling priority

### Windows Thread API

```cpp
// Windows provides several APIs for thread management:
// - CreateThread(): Win32 API
// - _beginthread(): C runtime
// - AfxBeginThread(): MFC wrapper (RECOMMENDED)

// Why use AfxBeginThread()?
// 1. Automatic cleanup
// 2. MFC integration
// 3. Exception handling
// 4. Thread object management
```

---

## 3. MFC Thread Architecture

### CWinThread Class

MFC encapsulates threads in the `CWinThread` class, which provides:

```cpp
class CWinThread : public CCmdTarget
{
public:
    HANDLE m_hThread;           // Thread handle
    DWORD m_nThreadID;          // Thread ID
    int m_nThreadPriority;      // Thread priority
    BOOL m_bAutoDelete;         // Auto-delete flag

    // Virtual functions to override
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    virtual int Run();

    // Thread control
    DWORD SuspendThread();
    DWORD ResumeThread();
    BOOL PostThreadMessage(UINT message, WPARAM wParam, LPARAM lParam);

protected:
    CWinThread();
    virtual ~CWinThread();
};
```

### Thread Types in MFC

1. **Worker Threads**: Perform background tasks without UI
2. **UI Threads**: Have message pump and can create windows

---

## 4. Creating Threads with AfxBeginThread

### Worker Thread Creation

```cpp
// ThreadDemo.h
#pragma once

class CThreadDemoApp : public CWinApp
{
public:
    CThreadDemoApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

// Worker thread function signature
UINT WorkerThreadProc(LPVOID pParam);

// ThreadDemo.cpp
#include "stdafx.h"
#include "ThreadDemo.h"

// Simple worker thread function
UINT WorkerThreadProc(LPVOID pParam)
{
    // Get parameter
    int* pCounter = (int*)pParam;

    // Perform work
    for (int i = 0; i < 10; i++)
    {
        (*pCounter)++;
        Sleep(1000);  // Simulate work

        TRACE(_T("Worker thread: Counter = %d\n"), *pCounter);
    }

    return 0;  // Thread exit code
}

void CThreadDemoApp::OnStartWorkerThread()
{
    static int counter = 0;

    // Start worker thread
    CWinThread* pThread = AfxBeginThread(
        WorkerThreadProc,      // Thread function
        &counter,              // Parameter
        THREAD_PRIORITY_NORMAL,// Priority
        0,                     // Stack size (0 = default)
        0,                     // Creation flags (0 = running)
        NULL                   // Security attributes
    );

    if (pThread != NULL)
    {
        AfxMessageBox(_T("Worker thread started successfully!"));
    }
}
```

### Thread Function with Structure Parameter

```cpp
// ThreadData.h
struct ThreadData
{
    CString message;
    int iterations;
    HWND hWndNotify;  // Window to notify when complete
    volatile bool* pStopFlag;
};

// Worker function with structured data
UINT StructuredWorkerProc(LPVOID pParam)
{
    ThreadData* pData = (ThreadData*)pParam;

    for (int i = 0; i < pData->iterations; i++)
    {
        // Check stop flag
        if (pData->pStopFlag && *pData->pStopFlag)
        {
            TRACE(_T("Thread stopped by request\n"));
            break;
        }

        // Perform work
        TRACE(_T("%s - Iteration %d\n"), pData->message, i);
        Sleep(500);
    }

    // Notify completion
    if (pData->hWndNotify && ::IsWindow(pData->hWndNotify))
    {
        ::PostMessage(pData->hWndNotify, WM_USER + 100, 0, 0);
    }

    // Clean up
    delete pData;
    return 0;
}

// Starting the thread
void CMyDialog::OnStartStructuredThread()
{
    ThreadData* pData = new ThreadData;
    pData->message = _T("Processing");
    pData->iterations = 20;
    pData->hWndNotify = m_hWnd;
    pData->pStopFlag = &m_bStopThread;

    CWinThread* pThread = AfxBeginThread(
        StructuredWorkerProc,
        pData,
        THREAD_PRIORITY_NORMAL
    );
}
```

---

## 5. Thread Types: Worker vs UI Threads

### Worker Threads

Worker threads perform background tasks without creating windows or processing messages.

```cpp
// WorkerThread.h
class CCalculationThread
{
public:
    static UINT CalculateProc(LPVOID pParam);

    struct CalcData
    {
        double startValue;
        double endValue;
        int steps;
        double result;
        CEvent* pCompleteEvent;
    };
};

// WorkerThread.cpp
UINT CCalculationThread::CalculateProc(LPVOID pParam)
{
    CalcData* pData = (CalcData*)pParam;

    pData->result = 0.0;
    double step = (pData->endValue - pData->startValue) / pData->steps;

    for (int i = 0; i < pData->steps; i++)
    {
        double x = pData->startValue + i * step;
        pData->result += sin(x) * cos(x) * step;

        // Simulate complex calculation
        Sleep(10);
    }

    // Signal completion
    if (pData->pCompleteEvent)
    {
        pData->pCompleteEvent->SetEvent();
    }

    return 0;
}

// Using the worker thread
void CMyDialog::OnCalculate()
{
    CCalculationThread::CalcData* pData =
        new CCalculationThread::CalcData;

    pData->startValue = 0.0;
    pData->endValue = 3.14159 * 2;
    pData->steps = 1000;
    pData->pCompleteEvent = new CEvent(FALSE, TRUE);

    // Start calculation thread
    AfxBeginThread(
        CCalculationThread::CalculateProc,
        pData
    );

    // Wait for completion (in another thread or with timeout)
    DWORD dwResult = WaitForSingleObject(
        pData->pCompleteEvent->m_hObject,
        5000  // 5 second timeout
    );

    if (dwResult == WAIT_OBJECT_0)
    {
        CString msg;
        msg.Format(_T("Calculation result: %f"), pData->result);
        AfxMessageBox(msg);
    }

    delete pData->pCompleteEvent;
    delete pData;
}
```

### UI Threads

UI threads have a message pump and can create and manage windows.

```cpp
// UIThread.h
class CMyUIThread : public CWinThread
{
    DECLARE_DYNCREATE(CMyUIThread)

protected:
    CMyUIThread();
    virtual ~CMyUIThread();

public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    DECLARE_MESSAGE_MAP()
};

// UIThreadWindow.h
class CUIThreadWindow : public CFrameWnd
{
public:
    CUIThreadWindow();

protected:
    afx_msg void OnPaint();
    afx_msg void OnDestroy();
    DECLARE_MESSAGE_MAP()

private:
    int m_nCounter;
};

// UIThread.cpp
IMPLEMENT_DYNCREATE(CMyUIThread, CWinThread)

CMyUIThread::CMyUIThread()
{
    m_bAutoDelete = TRUE;
}

CMyUIThread::~CMyUIThread()
{
}

BOOL CMyUIThread::InitInstance()
{
    // Create and show window
    CUIThreadWindow* pFrame = new CUIThreadWindow;
    m_pMainWnd = pFrame;

    pFrame->Create(
        NULL,
        _T("UI Thread Window"),
        WS_OVERLAPPEDWINDOW,
        CRect(100, 100, 500, 400)
    );

    pFrame->ShowWindow(SW_SHOW);
    pFrame->UpdateWindow();

    return TRUE;
}

int CMyUIThread::ExitInstance()
{
    return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMyUIThread, CWinThread)
END_MESSAGE_MAP()

// UIThreadWindow.cpp
BEGIN_MESSAGE_MAP(CUIThreadWindow, CFrameWnd)
    ON_WM_PAINT()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

CUIThreadWindow::CUIThreadWindow() : m_nCounter(0)
{
}

void CUIThreadWindow::OnPaint()
{
    CPaintDC dc(this);

    CString str;
    str.Format(_T("UI Thread Counter: %d"), m_nCounter++);
    dc.TextOut(10, 10, str);
}

void CUIThreadWindow::OnDestroy()
{
    CFrameWnd::OnDestroy();
    PostQuitMessage(0);
}

// Starting UI thread
void CMainApp::OnCreateUIThread()
{
    CMyUIThread* pThread = (CMyUIThread*)
        AfxBeginThread(RUNTIME_CLASS(CMyUIThread));

    if (pThread == NULL)
    {
        AfxMessageBox(_T("Failed to create UI thread"));
    }
}
```

---

## 6. Thread Management and Control

### Thread Priorities

```cpp
// Thread priority levels
void CThreadManager::SetThreadPriorities()
{
    CWinThread* pThread = AfxBeginThread(WorkerProc, NULL);

    if (pThread)
    {
        // Set priority
        pThread->SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);

        /*
        Available priorities:
        THREAD_PRIORITY_IDLE           = -15
        THREAD_PRIORITY_LOWEST         = -2
        THREAD_PRIORITY_BELOW_NORMAL   = -1
        THREAD_PRIORITY_NORMAL         = 0  (default)
        THREAD_PRIORITY_ABOVE_NORMAL   = 1
        THREAD_PRIORITY_HIGHEST        = 2
        THREAD_PRIORITY_TIME_CRITICAL  = 15
        */
    }
}
```

### Suspending and Resuming Threads

```cpp
class CThreadController
{
public:
    CThreadController() : m_pThread(NULL), m_bSuspended(FALSE) {}

    void StartThread()
    {
        if (m_pThread == NULL)
        {
            m_pThread = AfxBeginThread(
                LongRunningProc,
                this,
                THREAD_PRIORITY_NORMAL,
                0,
                CREATE_SUSPENDED  // Create suspended
            );

            if (m_pThread)
            {
                m_bSuspended = TRUE;
            }
        }
    }

    void ResumeThread()
    {
        if (m_pThread && m_bSuspended)
        {
            DWORD dwCount = m_pThread->ResumeThread();
            if (dwCount == 1)  // Was suspended once
            {
                m_bSuspended = FALSE;
            }
        }
    }

    void SuspendThread()
    {
        if (m_pThread && !m_bSuspended)
        {
            m_pThread->SuspendThread();
            m_bSuspended = TRUE;
        }
    }

    static UINT LongRunningProc(LPVOID pParam)
    {
        CThreadController* pController = (CThreadController*)pParam;

        for (int i = 0; i < 100; i++)
        {
            // Do work
            TRACE(_T("Processing: %d%%\n"), i);
            Sleep(100);
        }

        return 0;
    }

private:
    CWinThread* m_pThread;
    BOOL m_bSuspended;
};
```

### Terminating Threads

```cpp
// Method 1: Natural termination (RECOMMENDED)
class CStoppableThread
{
public:
    CStoppableThread() : m_bStopRequested(FALSE), m_pThread(NULL) {}

    void Start()
    {
        m_bStopRequested = FALSE;
        m_pThread = AfxBeginThread(ThreadProc, this);
    }

    void Stop()
    {
        m_bStopRequested = TRUE;

        // Wait for thread to finish (with timeout)
        if (m_pThread && m_pThread->m_hThread)
        {
            DWORD dwResult = WaitForSingleObject(
                m_pThread->m_hThread,
                5000  // 5 second timeout
            );

            if (dwResult == WAIT_TIMEOUT)
            {
                TRACE(_T("Warning: Thread did not terminate gracefully\n"));
                // Consider TerminateThread as last resort (dangerous!)
            }
        }
    }

    static UINT ThreadProc(LPVOID pParam)
    {
        CStoppableThread* pThis = (CStoppableThread*)pParam;

        while (!pThis->m_bStopRequested)
        {
            // Do work
            Sleep(100);
        }

        TRACE(_T("Thread terminating gracefully\n"));
        return 0;
    }

private:
    volatile BOOL m_bStopRequested;
    CWinThread* m_pThread;
};

// Method 2: Using events for signaling
class CEventControlledThread
{
public:
    CEventControlledThread()
    {
        m_stopEvent.ResetEvent();
    }

    void Start()
    {
        m_stopEvent.ResetEvent();
        AfxBeginThread(ThreadProc, this);
    }

    void Stop()
    {
        m_stopEvent.SetEvent();
    }

    static UINT ThreadProc(LPVOID pParam)
    {
        CEventControlledThread* pThis = (CEventControlledThread*)pParam;

        while (TRUE)
        {
            // Wait for stop event or timeout
            DWORD dwResult = WaitForSingleObject(
                pThis->m_stopEvent.m_hObject,
                100  // Check every 100ms
            );

            if (dwResult == WAIT_OBJECT_0)
            {
                // Stop event signaled
                break;
            }

            // Do work here
            TRACE(_T("Working...\n"));
        }

        return 0;
    }

private:
    CEvent m_stopEvent;
};
```

---

## 7. Thread Communication

### Posting Messages to Threads

```cpp
// Custom message
#define WM_THREAD_COMMAND (WM_USER + 200)

// Thread that processes messages
UINT MessageProcessingThread(LPVOID pParam)
{
    MSG msg;

    // Create message queue
    PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

    HWND hWnd = (HWND)pParam;
    ::PostMessage(hWnd, WM_USER + 1, 0, 0);  // Signal ready

    // Message loop
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (msg.message == WM_THREAD_COMMAND)
        {
            int command = (int)msg.wParam;
            TRACE(_T("Thread received command: %d\n"), command);

            // Process command
            switch (command)
            {
            case 1:
                TRACE(_T("Executing command 1\n"));
                break;
            case 2:
                TRACE(_T("Executing command 2\n"));
                break;
            case 99:
                TRACE(_T("Thread shutdown requested\n"));
                return 0;
            }
        }
    }

    return 0;
}

// Dialog class that communicates with thread
class CThreadCommDialog : public CDialogEx
{
public:
    CThreadCommDialog() : m_pThread(NULL) {}

    void OnStartThread()
    {
        m_pThread = AfxBeginThread(
            MessageProcessingThread,
            m_hWnd,
            THREAD_PRIORITY_NORMAL,
            0,
            CREATE_SUSPENDED
        );

        if (m_pThread)
        {
            m_pThread->m_bAutoDelete = FALSE;  // Manual delete
            m_pThread->ResumeThread();
        }
    }

    void OnSendCommand(int command)
    {
        if (m_pThread && m_pThread->m_nThreadID)
        {
            ::PostThreadMessage(
                m_pThread->m_nThreadID,
                WM_THREAD_COMMAND,
                command,
                0
            );
        }
    }

    void OnStopThread()
    {
        if (m_pThread)
        {
            OnSendCommand(99);  // Shutdown command

            WaitForSingleObject(m_pThread->m_hThread, 2000);
            delete m_pThread;
            m_pThread = NULL;
        }
    }

protected:
    CWinThread* m_pThread;
};
```

### Using Window Messages for Communication

```cpp
// Progress notification example
#define WM_PROGRESS_UPDATE (WM_USER + 300)
#define WM_TASK_COMPLETE (WM_USER + 301)

struct ProgressData
{
    HWND hWndNotify;
    int totalSteps;
};

UINT ProgressWorkerProc(LPVOID pParam)
{
    ProgressData* pData = (ProgressData*)pParam;

    for (int i = 0; i <= pData->totalSteps; i++)
    {
        // Calculate progress percentage
        int percent = (i * 100) / pData->totalSteps;

        // Send progress update
        ::PostMessage(
            pData->hWndNotify,
            WM_PROGRESS_UPDATE,
            percent,
            0
        );

        // Simulate work
        Sleep(100);
    }

    // Send completion message
    ::PostMessage(pData->hWndNotify, WM_TASK_COMPLETE, 0, 0);

    delete pData;
    return 0;
}

// Dialog handling progress updates
class CProgressDialog : public CDialogEx
{
public:
    CProgressDialog() {}

protected:
    DECLARE_MESSAGE_MAP()

    afx_msg LRESULT OnProgressUpdate(WPARAM wParam, LPARAM lParam)
    {
        int percent = (int)wParam;
        m_progressCtrl.SetPos(percent);

        CString status;
        status.Format(_T("Progress: %d%%"), percent);
        SetDlgItemText(IDC_STATUS, status);

        return 0;
    }

    afx_msg LRESULT OnTaskComplete(WPARAM wParam, LPARAM lParam)
    {
        AfxMessageBox(_T("Task completed successfully!"));
        m_btnStart.EnableWindow(TRUE);
        return 0;
    }

    afx_msg void OnStartTask()
    {
        m_btnStart.EnableWindow(FALSE);
        m_progressCtrl.SetPos(0);

        ProgressData* pData = new ProgressData;
        pData->hWndNotify = m_hWnd;
        pData->totalSteps = 100;

        AfxBeginThread(ProgressWorkerProc, pData);
    }

private:
    CProgressCtrl m_progressCtrl;
    CButton m_btnStart;
};

BEGIN_MESSAGE_MAP(CProgressDialog, CDialogEx)
    ON_MESSAGE(WM_PROGRESS_UPDATE, OnProgressUpdate)
    ON_MESSAGE(WM_TASK_COMPLETE, OnTaskComplete)
    ON_BN_CLICKED(IDC_BTN_START, OnStartTask)
END_MESSAGE_MAP()
```

---

## 8. Thread Safety and Best Practices

### Thread-Safe Practices

```cpp
// 1. Avoid shared mutable state
class CThreadSafeCounter
{
public:
    CThreadSafeCounter() : m_counter(0)
    {
        InitializeCriticalSection(&m_cs);
    }

    ~CThreadSafeCounter()
    {
        DeleteCriticalSection(&m_cs);
    }

    void Increment()
    {
        EnterCriticalSection(&m_cs);
        m_counter++;
        LeaveCriticalSection(&m_cs);
    }

    int GetValue()
    {
        EnterCriticalSection(&m_cs);
        int value = m_counter;
        LeaveCriticalSection(&m_cs);
        return value;
    }

private:
    int m_counter;
    CRITICAL_SECTION m_cs;
};

// 2. Use MFC synchronization objects
class CMFCThreadSafeClass
{
public:
    CMFCThreadSafeClass() {}

    void AddItem(const CString& item)
    {
        CSingleLock lock(&m_mutex, TRUE);
        m_items.Add(item);
    }

    CString GetItem(int index)
    {
        CSingleLock lock(&m_mutex, TRUE);
        if (index >= 0 && index < m_items.GetCount())
        {
            return m_items[index];
        }
        return _T("");
    }

    int GetCount()
    {
        CSingleLock lock(&m_mutex, TRUE);
        return m_items.GetCount();
    }

private:
    CStringArray m_items;
    CMutex m_mutex;
};
```

### Best Practices

```cpp
/*
BEST PRACTICES FOR MFC MULTITHREADING:

1. THREAD CREATION
   - Use AfxBeginThread instead of CreateThread
   - Set appropriate thread priority
   - Consider thread pool for many short tasks

2. RESOURCE MANAGEMENT
   - Use m_bAutoDelete = TRUE for worker threads
   - Manually delete UI threads
   - Clean up thread resources in destructor

3. SYNCHRONIZATION
   - Use MFC sync objects (CMutex, CSemaphore, CEvent)
   - Minimize time in critical sections
   - Avoid deadlocks with consistent lock ordering

4. UI UPDATES
   - Never access UI from worker threads directly
   - Use PostMessage/SendMessage for UI updates
   - Consider using PostMessage over SendMessage

5. THREAD TERMINATION
   - Use flags for graceful shutdown
   - Avoid TerminateThread (unsafe)
   - Wait for thread completion with timeout

6. DEBUGGING
   - Use TRACE for thread debugging
   - Monitor thread count
   - Check for resource leaks
   - Use thread naming for debugging

7. PERFORMANCE
   - Don't create too many threads
   - Use thread pools for scalability
   - Profile thread performance
   - Consider async operations

8. ERROR HANDLING
   - Use structured exception handling
   - Report errors to main thread
   - Don't let exceptions escape thread function
*/
```

---

## 9. Practical Examples

### Example 1: File Search Application

```cpp
// FileSearchThread.h
struct SearchParams
{
    CString directory;
    CString pattern;
    HWND hWndNotify;
    volatile BOOL* pStopFlag;
};

class CFileSearchThread
{
public:
    static UINT SearchProc(LPVOID pParam);

private:
    static void SearchDirectory(
        const CString& dir,
        const CString& pattern,
        HWND hWnd,
        volatile BOOL* pStop
    );
};

// FileSearchThread.cpp
#define WM_FILE_FOUND (WM_USER + 400)
#define WM_SEARCH_COMPLETE (WM_USER + 401)

UINT CFileSearchThread::SearchProc(LPVOID pParam)
{
    SearchParams* pParams = (SearchParams*)pParam;

    SearchDirectory(
        pParams->directory,
        pParams->pattern,
        pParams->hWndNotify,
        pParams->pStopFlag
    );

    ::PostMessage(pParams->hWndNotify, WM_SEARCH_COMPLETE, 0, 0);

    delete pParams;
    return 0;
}

void CFileSearchThread::SearchDirectory(
    const CString& dir,
    const CString& pattern,
    HWND hWnd,
    volatile BOOL* pStop)
{
    if (pStop && *pStop) return;

    CFileFind finder;
    CString searchPath = dir + _T("\\*.*");

    BOOL bWorking = finder.FindFile(searchPath);

    while (bWorking)
    {
        if (pStop && *pStop) break;

        bWorking = finder.FindNextFile();

        if (finder.IsDots()) continue;

        if (finder.IsDirectory())
        {
            // Recursively search subdirectories
            SearchDirectory(
                finder.GetFilePath(),
                pattern,
                hWnd,
                pStop
            );
        }
        else
        {
            CString fileName = finder.GetFileName();
            if (fileName.Find(pattern) != -1)
            {
                // File found - notify main window
                CString* pFilePath = new CString(finder.GetFilePath());
                ::PostMessage(hWnd, WM_FILE_FOUND, 0, (LPARAM)pFilePath);
            }
        }
    }

    finder.Close();
}
```

### Example 2: Download Manager

```cpp
// DownloadThread.h
struct DownloadInfo
{
    CString url;
    CString savePath;
    HWND hWndNotify;
    DWORD threadId;
};

#define WM_DOWNLOAD_PROGRESS (WM_USER + 500)
#define WM_DOWNLOAD_COMPLETE (WM_USER + 501)
#define WM_DOWNLOAD_ERROR (WM_USER + 502)

UINT DownloadThreadProc(LPVOID pParam)
{
    DownloadInfo* pInfo = (DownloadInfo*)pParam;

    try
    {
        // Simulate download with progress
        for (int i = 0; i <= 100; i += 10)
        {
            Sleep(500);  // Simulate network delay

            // Send progress update
            ::PostMessage(
                pInfo->hWndNotify,
                WM_DOWNLOAD_PROGRESS,
                pInfo->threadId,
                i
            );
        }

        // Notify completion
        ::PostMessage(
            pInfo->hWndNotify,
            WM_DOWNLOAD_COMPLETE,
            pInfo->threadId,
            0
        );
    }
    catch (...)
    {
        ::PostMessage(
            pInfo->hWndNotify,
            WM_DOWNLOAD_ERROR,
            pInfo->threadId,
            0
        );
    }

    delete pInfo;
    return 0;
}

// Download manager class
class CDownloadManager
{
public:
    void StartDownload(const CString& url, const CString& savePath, HWND hWnd)
    {
        DownloadInfo* pInfo = new DownloadInfo;
        pInfo->url = url;
        pInfo->savePath = savePath;
        pInfo->hWndNotify = hWnd;
        pInfo->threadId = m_nextThreadId++;

        CWinThread* pThread = AfxBeginThread(
            DownloadThreadProc,
            pInfo,
            THREAD_PRIORITY_NORMAL
        );

        if (pThread)
        {
            m_threads[pInfo->threadId] = pThread;
        }
    }

private:
    DWORD m_nextThreadId = 1;
    CMap<DWORD, DWORD, CWinThread*, CWinThread*> m_threads;
};
```

---

## 10. Common Pitfalls and Solutions

### Pitfall 1: Accessing UI from Worker Thread

```cpp
// WRONG - Don't do this!
UINT BadThreadProc(LPVOID pParam)
{
    CEdit* pEdit = (CEdit*)pParam;
    pEdit->SetWindowText(_T("Updated"));  // CRASH!
    return 0;
}

// CORRECT - Use PostMessage
UINT GoodThreadProc(LPVOID pParam)
{
    HWND hEdit = (HWND)pParam;
    ::PostMessage(hEdit, WM_SETTEXT, 0, (LPARAM)_T("Updated"));
    return 0;
}
```

### Pitfall 2: Thread Resource Leaks

```cpp
// WRONG - Memory leak
void BadThreadStart()
{
    CWinThread* pThread = AfxBeginThread(WorkerProc, NULL);
    pThread->m_bAutoDelete = FALSE;  // Manual delete required!
    // Thread never deleted!
}

// CORRECT
void GoodThreadStart()
{
    CWinThread* pThread = AfxBeginThread(WorkerProc, NULL);
    pThread->m_bAutoDelete = TRUE;  // Automatic cleanup
}
```

### Pitfall 3: Race Conditions

```cpp
// WRONG - Race condition
class BadClass
{
public:
    void IncrementFromThread()
    {
        m_counter++;  // Not atomic!
    }

private:
    int m_counter;
};

// CORRECT - Thread-safe
class GoodClass
{
public:
    void IncrementFromThread()
    {
        InterlockedIncrement(&m_counter);
    }

private:
    volatile LONG m_counter;
};
```

---

## Key Takeaways

1. **Use AfxBeginThread** for creating threads in MFC applications
2. **Worker threads** perform background tasks without UI
3. **UI threads** have message pumps and can create windows
4. **Thread communication** uses PostMessage/SendMessage
5. **Synchronization** prevents race conditions and data corruption
6. **Graceful shutdown** is better than forced termination
7. **Thread safety** requires careful design and synchronization
8. **UI access** from worker threads must use messages
9. **Resource management** is critical to prevent leaks
10. **Best practices** ensure robust multithreaded applications

---

## Next Lesson Preview

In **Lesson 62: Worker Threads and UI Threads**, we'll explore:
- Detailed worker thread implementation
- UI thread message pumps
- Thread communication patterns
- Synchronization techniques
- Producer-consumer patterns
- Thread pools
- Advanced threading scenarios

**Master multithreading for responsive applications!**
