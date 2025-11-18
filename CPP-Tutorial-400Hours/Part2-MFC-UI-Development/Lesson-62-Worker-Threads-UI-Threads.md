# Lesson 62: Worker Threads and UI Threads

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Understanding Thread Types
2. Worker Thread Deep Dive
3. UI Thread Architecture
4. Thread Message Pumps
5. Thread Communication Patterns
6. Producer-Consumer Pattern
7. Thread Pooling
8. Advanced Thread Management
9. Performance Considerations
10. Real-World Applications
11. Exercises and Projects

---

## 1. Understanding Thread Types

### Thread Type Comparison

```cpp
/*
WORKER THREAD:
- No message pump
- Background processing
- Cannot create windows
- Simpler to manage
- Better for CPU-intensive tasks
- Created with function pointer

UI THREAD:
- Has message pump
- Can create windows
- Process Windows messages
- More complex management
- GUI-related tasks
- Derived from CWinThread
*/
```

### When to Use Each Type

```cpp
// Use WORKER THREAD for:
// 1. File I/O operations
// 2. Network communication
// 3. Mathematical calculations
// 4. Data processing
// 5. Background tasks without UI

// Use UI THREAD for:
// 1. Separate windows/dialogs
// 2. Independent UI components
// 3. Modeless dialogs
// 4. Secondary application windows
// 5. UI-heavy concurrent operations
```

---

## 2. Worker Thread Deep Dive

### Basic Worker Thread Structure

```cpp
// WorkerThread.h
#pragma once

class CWorkerThreadExample
{
public:
    // Thread function signature
    static UINT ThreadFunction(LPVOID pParam);

    // Thread data structure
    struct ThreadData
    {
        CString taskName;
        int taskId;
        HWND hWndNotify;
        volatile BOOL* pCancelFlag;
        void* pUserData;
    };
};

// WorkerThread.cpp
UINT CWorkerThreadExample::ThreadFunction(LPVOID pParam)
{
    ThreadData* pData = (ThreadData*)pParam;

    TRACE(_T("Thread started: %s (ID: %d)\n"),
          pData->taskName, pData->taskId);

    // Perform work
    for (int i = 0; i < 100; i++)
    {
        // Check for cancellation
        if (pData->pCancelFlag && *pData->pCancelFlag)
        {
            TRACE(_T("Thread cancelled\n"));
            delete pData;
            return 1;  // Cancelled
        }

        // Do actual work
        Sleep(50);

        // Report progress
        if (pData->hWndNotify && ::IsWindow(pData->hWndNotify))
        {
            ::PostMessage(pData->hWndNotify, WM_USER + 100, i, pData->taskId);
        }
    }

    // Cleanup
    delete pData;
    TRACE(_T("Thread completed successfully\n"));
    return 0;  // Success
}

// Usage example
class CMyDialog : public CDialogEx
{
public:
    void OnStartWorkerThread()
    {
        CWorkerThreadExample::ThreadData* pData =
            new CWorkerThreadExample::ThreadData;

        pData->taskName = _T("Data Processing");
        pData->taskId = m_nextTaskId++;
        pData->hWndNotify = m_hWnd;
        pData->pCancelFlag = &m_bCancelFlag;
        pData->pUserData = NULL;

        m_bCancelFlag = FALSE;

        CWinThread* pThread = AfxBeginThread(
            CWorkerThreadExample::ThreadFunction,
            pData,
            THREAD_PRIORITY_NORMAL
        );

        if (pThread)
        {
            m_threads.Add(pThread);
        }
    }

private:
    CArray<CWinThread*, CWinThread*> m_threads;
    volatile BOOL m_bCancelFlag;
    int m_nextTaskId;
};
```

### Advanced Worker Thread with Return Values

```cpp
// ResultWorkerThread.h
template<typename TResult>
class CResultWorkerThread
{
public:
    struct ThreadContext
    {
        CEvent completeEvent;
        TResult result;
        BOOL success;
        CString errorMessage;

        ThreadContext() : completeEvent(FALSE, TRUE),
                          success(FALSE) {}
    };

    typedef BOOL (*WorkFunction)(ThreadContext* pContext);

    static UINT ThreadProc(LPVOID pParam);

    struct ThreadParams
    {
        WorkFunction workFunc;
        ThreadContext* pContext;
    };
};

template<typename TResult>
UINT CResultWorkerThread<TResult>::ThreadProc(LPVOID pParam)
{
    ThreadParams* pParams = (ThreadParams*)pParam;
    ThreadContext* pContext = pParams->pContext;

    try
    {
        // Execute work function
        pContext->success = pParams->workFunc(pContext);
    }
    catch (CException* e)
    {
        TCHAR szError[256];
        e->GetErrorMessage(szError, 256);
        pContext->errorMessage = szError;
        pContext->success = FALSE;
        e->Delete();
    }
    catch (...)
    {
        pContext->errorMessage = _T("Unknown exception occurred");
        pContext->success = FALSE;
    }

    // Signal completion
    pContext->completeEvent.SetEvent();

    delete pParams;
    return pContext->success ? 0 : 1;
}

// Usage example
BOOL CalculateWorker(CResultWorkerThread<double>::ThreadContext* pContext)
{
    // Perform complex calculation
    double sum = 0.0;
    for (int i = 0; i < 1000000; i++)
    {
        sum += sqrt((double)i);
    }

    pContext->result = sum;
    return TRUE;
}

void CMyDialog::OnCalculateInBackground()
{
    typedef CResultWorkerThread<double> CalcThread;

    CalcThread::ThreadContext* pContext = new CalcThread::ThreadContext;
    CalcThread::ThreadParams* pParams = new CalcThread::ThreadParams;

    pParams->workFunc = CalculateWorker;
    pParams->pContext = pContext;

    CWinThread* pThread = AfxBeginThread(
        CalcThread::ThreadProc,
        pParams,
        THREAD_PRIORITY_NORMAL
    );

    // Wait for result in another thread or with timeout
    DWORD dwResult = WaitForSingleObject(
        pContext->completeEvent.m_hObject,
        10000  // 10 second timeout
    );

    if (dwResult == WAIT_OBJECT_0)
    {
        if (pContext->success)
        {
            CString msg;
            msg.Format(_T("Result: %f"), pContext->result);
            AfxMessageBox(msg);
        }
        else
        {
            AfxMessageBox(_T("Calculation failed: ") + pContext->errorMessage);
        }
    }

    delete pContext;
}
```

### Worker Thread Pool

```cpp
// ThreadPool.h
class CThreadPool
{
public:
    typedef UINT (*TaskFunction)(LPVOID pParam);

    struct Task
    {
        TaskFunction function;
        LPVOID parameter;
        int taskId;
    };

    CThreadPool(int threadCount = 4);
    ~CThreadPool();

    int QueueTask(TaskFunction func, LPVOID pParam);
    void WaitForAll();
    void Shutdown();

private:
    static UINT WorkerThreadProc(LPVOID pParam);
    void WorkerLoop();

    CArray<CWinThread*, CWinThread*> m_threads;
    CList<Task, Task&> m_taskQueue;
    CMutex m_queueMutex;
    CSemaphore m_taskSemaphore;
    volatile BOOL m_bShutdown;
    int m_nextTaskId;
    volatile LONG m_activeTasks;
};

// ThreadPool.cpp
CThreadPool::CThreadPool(int threadCount)
    : m_taskSemaphore(0, LONG_MAX),
      m_bShutdown(FALSE),
      m_nextTaskId(1),
      m_activeTasks(0)
{
    // Create worker threads
    for (int i = 0; i < threadCount; i++)
    {
        CWinThread* pThread = AfxBeginThread(
            WorkerThreadProc,
            this,
            THREAD_PRIORITY_NORMAL,
            0,
            CREATE_SUSPENDED
        );

        if (pThread)
        {
            pThread->m_bAutoDelete = FALSE;
            pThread->ResumeThread();
            m_threads.Add(pThread);
        }
    }
}

CThreadPool::~CThreadPool()
{
    Shutdown();
}

int CThreadPool::QueueTask(TaskFunction func, LPVOID pParam)
{
    if (m_bShutdown) return -1;

    CSingleLock lock(&m_queueMutex, TRUE);

    Task task;
    task.function = func;
    task.parameter = pParam;
    task.taskId = m_nextTaskId++;

    m_taskQueue.AddTail(task);
    InterlockedIncrement(&m_activeTasks);

    // Signal that a task is available
    m_taskSemaphore.Unlock();

    return task.taskId;
}

void CThreadPool::WaitForAll()
{
    while (InterlockedCompareExchange(&m_activeTasks, 0, 0) > 0)
    {
        Sleep(100);
    }
}

void CThreadPool::Shutdown()
{
    m_bShutdown = TRUE;

    // Wake up all threads
    for (int i = 0; i < m_threads.GetCount(); i++)
    {
        m_taskSemaphore.Unlock();
    }

    // Wait for all threads to finish
    for (int i = 0; i < m_threads.GetCount(); i++)
    {
        CWinThread* pThread = m_threads[i];
        WaitForSingleObject(pThread->m_hThread, INFINITE);
        delete pThread;
    }

    m_threads.RemoveAll();

    // Clear remaining tasks
    CSingleLock lock(&m_queueMutex, TRUE);
    m_taskQueue.RemoveAll();
}

UINT CThreadPool::WorkerThreadProc(LPVOID pParam)
{
    CThreadPool* pPool = (CThreadPool*)pParam;
    pPool->WorkerLoop();
    return 0;
}

void CThreadPool::WorkerLoop()
{
    while (!m_bShutdown)
    {
        // Wait for task
        CSingleLock semLock(&m_taskSemaphore);
        if (!semLock.Lock(1000))  // 1 second timeout
            continue;

        if (m_bShutdown) break;

        // Get task from queue
        Task task;
        {
            CSingleLock queueLock(&m_queueMutex, TRUE);
            if (m_taskQueue.IsEmpty())
                continue;

            task = m_taskQueue.RemoveHead();
        }

        // Execute task
        try
        {
            task.function(task.parameter);
        }
        catch (...)
        {
            TRACE(_T("Exception in thread pool task %d\n"), task.taskId);
        }

        InterlockedDecrement(&m_activeTasks);
    }
}

// Usage example
UINT ProcessFileTask(LPVOID pParam)
{
    CString* pFileName = (CString*)pParam;
    TRACE(_T("Processing file: %s\n"), *pFileName);

    // Simulate file processing
    Sleep(1000);

    delete pFileName;
    return 0;
}

void CMyApp::ProcessMultipleFiles()
{
    CThreadPool pool(4);  // 4 worker threads

    // Queue multiple tasks
    for (int i = 0; i < 20; i++)
    {
        CString* pFileName = new CString;
        pFileName->Format(_T("file%d.dat"), i);
        pool.QueueTask(ProcessFileTask, pFileName);
    }

    // Wait for all tasks to complete
    pool.WaitForAll();

    AfxMessageBox(_T("All files processed!"));
}
```

---

## 3. UI Thread Architecture

### Basic UI Thread Implementation

```cpp
// MyUIThread.h
class CMyUIThread : public CWinThread
{
    DECLARE_DYNCREATE(CMyUIThread)

protected:
    CMyUIThread();
    virtual ~CMyUIThread();

public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    virtual int Run();

    // Custom initialization data
    struct InitData
    {
        CString windowTitle;
        CRect windowRect;
        HWND hWndParent;
    };

    void SetInitData(InitData* pData) { m_pInitData = pData; }

protected:
    DECLARE_MESSAGE_MAP()

private:
    InitData* m_pInitData;
};

// MyUIThreadWindow.h
class CMyUIThreadWindow : public CFrameWnd
{
public:
    CMyUIThreadWindow();

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg void OnDestroy();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnClose();

    DECLARE_MESSAGE_MAP()

private:
    int m_nCounter;
    CString m_strTitle;
};

// MyUIThread.cpp
IMPLEMENT_DYNCREATE(CMyUIThread, CWinThread)

CMyUIThread::CMyUIThread() : m_pInitData(NULL)
{
    m_bAutoDelete = FALSE;  // Manual deletion for UI threads
}

CMyUIThread::~CMyUIThread()
{
    if (m_pInitData)
        delete m_pInitData;
}

BEGIN_MESSAGE_MAP(CMyUIThread, CWinThread)
END_MESSAGE_MAP()

BOOL CMyUIThread::InitInstance()
{
    // Create main window for this thread
    CMyUIThreadWindow* pFrame = new CMyUIThreadWindow;
    m_pMainWnd = pFrame;

    // Apply initialization data
    CRect rect(100, 100, 600, 400);
    CString title = _T("UI Thread Window");

    if (m_pInitData)
    {
        rect = m_pInitData->windowRect;
        title = m_pInitData->windowTitle;
    }

    if (!pFrame->Create(
        NULL,
        title,
        WS_OVERLAPPEDWINDOW,
        rect))
    {
        return FALSE;
    }

    pFrame->ShowWindow(SW_SHOW);
    pFrame->UpdateWindow();

    return TRUE;
}

int CMyUIThread::ExitInstance()
{
    return CWinThread::ExitInstance();
}

int CMyUIThread::Run()
{
    // Custom message loop if needed
    return CWinThread::Run();
}

// MyUIThreadWindow.cpp
BEGIN_MESSAGE_MAP(CMyUIThreadWindow, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_DESTROY()
    ON_WM_TIMER()
    ON_WM_CLOSE()
END_MESSAGE_MAP()

CMyUIThreadWindow::CMyUIThreadWindow() : m_nCounter(0)
{
}

int CMyUIThreadWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Start update timer
    SetTimer(1, 1000, NULL);

    return 0;
}

void CMyUIThreadWindow::OnPaint()
{
    CPaintDC dc(this);

    CRect rect;
    GetClientRect(&rect);

    // Draw counter
    CString str;
    str.Format(_T("UI Thread Counter: %d"), m_nCounter);

    dc.SetTextColor(RGB(0, 0, 255));
    dc.SetBkMode(TRANSPARENT);
    dc.DrawText(str, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CMyUIThreadWindow::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1)
    {
        m_nCounter++;
        Invalidate();
    }

    CFrameWnd::OnTimer(nIDEvent);
}

void CMyUIThreadWindow::OnClose()
{
    KillTimer(1);
    CFrameWnd::OnClose();
}

void CMyUIThreadWindow::OnDestroy()
{
    CFrameWnd::OnDestroy();
    PostQuitMessage(0);
}
```

### Starting UI Threads

```cpp
// MainFrame.cpp
void CMainFrame::OnCreateUIThread()
{
    // Method 1: Simple creation
    CMyUIThread* pThread = (CMyUIThread*)
        AfxBeginThread(RUNTIME_CLASS(CMyUIThread),
                       THREAD_PRIORITY_NORMAL,
                       0,
                       CREATE_SUSPENDED);

    if (pThread)
    {
        // Set initialization data
        CMyUIThread::InitData* pData = new CMyUIThread::InitData;
        pData->windowTitle = _T("Custom UI Thread");
        pData->windowRect = CRect(200, 200, 700, 500);
        pData->hWndParent = m_hWnd;

        pThread->SetInitData(pData);

        // Start thread
        pThread->ResumeThread();

        // Store thread pointer for later cleanup
        m_uiThreads.Add(pThread);
    }
}

void CMainFrame::OnDestroyUIThreads()
{
    // Close all UI threads gracefully
    for (int i = 0; i < m_uiThreads.GetCount(); i++)
    {
        CMyUIThread* pThread = m_uiThreads[i];

        if (pThread && pThread->m_pMainWnd)
        {
            // Post WM_CLOSE to thread's main window
            pThread->m_pMainWnd->PostMessage(WM_CLOSE);

            // Wait for thread to terminate
            WaitForSingleObject(pThread->m_hThread, 5000);

            delete pThread;
        }
    }

    m_uiThreads.RemoveAll();
}
```

---

## 4. Thread Message Pumps

### Understanding the Message Pump

```cpp
// Typical Windows message pump
int CWinThread::Run()
{
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        // Translate accelerator keys
        if (!PreTranslateMessage(&msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}
```

### Custom Message Pump

```cpp
// CustomMessagePump.h
class CCustomUIThread : public CWinThread
{
    DECLARE_DYNCREATE(CCustomUIThread)

public:
    virtual int Run();

protected:
    BOOL ProcessIdle(LONG lCount);
    BOOL OnIdle(LONG lCount);
};

// CustomMessagePump.cpp
int CCustomUIThread::Run()
{
    MSG msg;
    LONG lIdleCount = 0;

    while (TRUE)
    {
        // Check for messages
        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (!PumpMessage())
            {
                return ExitInstance();
            }

            // Reset idle count
            lIdleCount = 0;
        }

        // Perform idle processing
        if (!OnIdle(lIdleCount++))
        {
            // Wait for message
            WaitMessage();
        }
    }
}

BOOL CCustomUIThread::OnIdle(LONG lCount)
{
    // Perform idle tasks
    if (lCount == 0)
    {
        // First idle - update UI
        if (m_pMainWnd)
        {
            m_pMainWnd->UpdateWindow();
        }
        return TRUE;  // More idle processing needed
    }

    // No more idle processing
    return FALSE;
}
```

### Message-Only Thread

```cpp
// MessageOnlyThread.h
class CMessageOnlyThread
{
public:
    static UINT ThreadProc(LPVOID pParam);

    struct ThreadContext
    {
        HWND hWnd;
        DWORD threadId;
        CEvent readyEvent;

        ThreadContext() : hWnd(NULL), threadId(0), readyEvent(FALSE, TRUE) {}
    };
};

// MessageOnlyThread.cpp
#define WM_CUSTOM_COMMAND (WM_USER + 600)

LRESULT CALLBACK MessageOnlyWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CUSTOM_COMMAND:
        {
            int command = (int)wParam;
            TRACE(_T("Received command: %d\n"), command);
            return 0;
        }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

UINT CMessageOnlyThread::ThreadProc(LPVOID pParam)
{
    ThreadContext* pContext = (ThreadContext*)pParam;

    // Register window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = MessageOnlyWndProc;
    wc.hInstance = AfxGetInstanceHandle();
    wc.lpszClassName = _T("MessageOnlyWindow");

    if (!RegisterClass(&wc))
    {
        pContext->readyEvent.SetEvent();
        return 1;
    }

    // Create message-only window
    pContext->hWnd = CreateWindow(
        _T("MessageOnlyWindow"),
        NULL,
        0,
        0, 0, 0, 0,
        HWND_MESSAGE,  // Message-only window
        NULL,
        AfxGetInstanceHandle(),
        NULL
    );

    if (!pContext->hWnd)
    {
        pContext->readyEvent.SetEvent();
        return 1;
    }

    pContext->threadId = GetCurrentThreadId();

    // Signal ready
    pContext->readyEvent.SetEvent();

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DestroyWindow(pContext->hWnd);
    UnregisterClass(_T("MessageOnlyWindow"), AfxGetInstanceHandle());

    return 0;
}

// Usage
void CMyApp::CreateMessageThread()
{
    CMessageOnlyThread::ThreadContext* pContext =
        new CMessageOnlyThread::ThreadContext;

    CWinThread* pThread = AfxBeginThread(
        CMessageOnlyThread::ThreadProc,
        pContext
    );

    // Wait for thread to be ready
    WaitForSingleObject(pContext->readyEvent.m_hObject, INFINITE);

    // Send message to thread
    if (pContext->hWnd)
    {
        ::PostMessage(pContext->hWnd, WM_CUSTOM_COMMAND, 42, 0);
    }
}
```

---

## 5. Thread Communication Patterns

### Pattern 1: Event-Based Communication

```cpp
class CEventBasedComm
{
public:
    struct SharedData
    {
        CEvent dataReadyEvent;
        CEvent dataProcessedEvent;
        CMutex dataMutex;
        CString data;
        BOOL bShutdown;

        SharedData()
            : dataReadyEvent(FALSE, TRUE),
              dataProcessedEvent(FALSE, TRUE),
              bShutdown(FALSE) {}
    };

    static UINT ProducerProc(LPVOID pParam)
    {
        SharedData* pData = (SharedData*)pParam;

        for (int i = 0; i < 10; i++)
        {
            if (pData->bShutdown) break;

            // Wait for previous data to be processed
            WaitForSingleObject(pData->dataProcessedEvent.m_hObject, INFINITE);

            // Produce new data
            {
                CSingleLock lock(&pData->dataMutex, TRUE);
                pData->data.Format(_T("Data item %d"), i);
                TRACE(_T("Produced: %s\n"), pData->data);
            }

            // Signal data ready
            pData->dataReadyEvent.SetEvent();
            pData->dataProcessedEvent.ResetEvent();

            Sleep(500);
        }

        return 0;
    }

    static UINT ConsumerProc(LPVOID pParam)
    {
        SharedData* pData = (SharedData*)pParam;

        // Initially signal that we're ready for data
        pData->dataProcessedEvent.SetEvent();

        while (!pData->bShutdown)
        {
            // Wait for data
            DWORD dwResult = WaitForSingleObject(
                pData->dataReadyEvent.m_hObject,
                1000
            );

            if (dwResult == WAIT_OBJECT_0)
            {
                // Process data
                CString processedData;
                {
                    CSingleLock lock(&pData->dataMutex, TRUE);
                    processedData = pData->data;
                }

                TRACE(_T("Consumed: %s\n"), processedData);
                Sleep(300);  // Simulate processing

                // Signal processing complete
                pData->dataReadyEvent.ResetEvent();
                pData->dataProcessedEvent.SetEvent();
            }
        }

        return 0;
    }
};
```

### Pattern 2: Queue-Based Communication

```cpp
template<typename T>
class CThreadSafeQueue
{
public:
    CThreadSafeQueue() : m_semaphore(0, LONG_MAX) {}

    void Enqueue(const T& item)
    {
        CSingleLock lock(&m_mutex, TRUE);
        m_queue.AddTail(item);
        m_semaphore.Unlock();
    }

    BOOL Dequeue(T& item, DWORD dwTimeout = INFINITE)
    {
        CSingleLock semLock(&m_semaphore);
        if (!semLock.Lock(dwTimeout))
            return FALSE;

        CSingleLock queueLock(&m_mutex, TRUE);
        if (m_queue.IsEmpty())
            return FALSE;

        item = m_queue.RemoveHead();
        return TRUE;
    }

    int GetCount()
    {
        CSingleLock lock(&m_mutex, TRUE);
        return m_queue.GetCount();
    }

private:
    CList<T, const T&> m_queue;
    CMutex m_mutex;
    CSemaphore m_semaphore;
};

// Usage with work items
struct WorkItem
{
    int id;
    CString data;
    HWND hWndNotify;
};

CThreadSafeQueue<WorkItem> g_workQueue;

UINT WorkerWithQueue(LPVOID pParam)
{
    BOOL* pShutdown = (BOOL*)pParam;

    while (!*pShutdown)
    {
        WorkItem item;
        if (g_workQueue.Dequeue(item, 1000))
        {
            // Process work item
            TRACE(_T("Processing item %d: %s\n"), item.id, item.data);
            Sleep(500);

            // Notify completion
            if (::IsWindow(item.hWndNotify))
            {
                ::PostMessage(item.hWndNotify, WM_USER + 700, item.id, 0);
            }
        }
    }

    return 0;
}
```

---

## 6. Producer-Consumer Pattern

### Complete Implementation

```cpp
// ProducerConsumer.h
class CProducerConsumer
{
public:
    CProducerConsumer(int bufferSize = 10);
    ~CProducerConsumer();

    void Start();
    void Stop();

    struct Item
    {
        int id;
        DWORD timestamp;
        CString data;
    };

private:
    static UINT ProducerThreadProc(LPVOID pParam);
    static UINT ConsumerThreadProc(LPVOID pParam);

    void ProducerLoop();
    void ConsumerLoop();

    CArray<Item, Item&> m_buffer;
    int m_bufferSize;
    CMutex m_bufferMutex;
    CSemaphore m_emptySlots;
    CSemaphore m_fullSlots;
    volatile BOOL m_bRunning;
    CWinThread* m_pProducerThread;
    CWinThread* m_pConsumerThread;
    int m_nextItemId;
};

// ProducerConsumer.cpp
CProducerConsumer::CProducerConsumer(int bufferSize)
    : m_bufferSize(bufferSize),
      m_emptySlots(bufferSize, bufferSize),
      m_fullSlots(0, bufferSize),
      m_bRunning(FALSE),
      m_pProducerThread(NULL),
      m_pConsumerThread(NULL),
      m_nextItemId(1)
{
}

CProducerConsumer::~CProducerConsumer()
{
    Stop();
}

void CProducerConsumer::Start()
{
    if (m_bRunning) return;

    m_bRunning = TRUE;
    m_nextItemId = 1;

    m_pProducerThread = AfxBeginThread(
        ProducerThreadProc,
        this,
        THREAD_PRIORITY_NORMAL,
        0,
        CREATE_SUSPENDED
    );

    m_pConsumerThread = AfxBeginThread(
        ConsumerThreadProc,
        this,
        THREAD_PRIORITY_NORMAL,
        0,
        CREATE_SUSPENDED
    );

    if (m_pProducerThread)
    {
        m_pProducerThread->m_bAutoDelete = FALSE;
        m_pProducerThread->ResumeThread();
    }

    if (m_pConsumerThread)
    {
        m_pConsumerThread->m_bAutoDelete = FALSE;
        m_pConsumerThread->ResumeThread();
    }
}

void CProducerConsumer::Stop()
{
    if (!m_bRunning) return;

    m_bRunning = FALSE;

    // Wait for threads to finish
    if (m_pProducerThread)
    {
        // Unblock if waiting
        m_emptySlots.Unlock();

        WaitForSingleObject(m_pProducerThread->m_hThread, 5000);
        delete m_pProducerThread;
        m_pProducerThread = NULL;
    }

    if (m_pConsumerThread)
    {
        // Unblock if waiting
        m_fullSlots.Unlock();

        WaitForSingleObject(m_pConsumerThread->m_hThread, 5000);
        delete m_pConsumerThread;
        m_pConsumerThread = NULL;
    }

    // Clear buffer
    CSingleLock lock(&m_bufferMutex, TRUE);
    m_buffer.RemoveAll();
}

UINT CProducerConsumer::ProducerThreadProc(LPVOID pParam)
{
    CProducerConsumer* pThis = (CProducerConsumer*)pParam;
    pThis->ProducerLoop();
    return 0;
}

UINT CProducerConsumer::ConsumerThreadProc(LPVOID pParam)
{
    CProducerConsumer* pThis = (CProducerConsumer*)pParam;
    pThis->ConsumerLoop();
    return 0;
}

void CProducerConsumer::ProducerLoop()
{
    while (m_bRunning)
    {
        // Wait for empty slot
        CSingleLock emptyLock(&m_emptySlots);
        if (!emptyLock.Lock(1000))
            continue;

        if (!m_bRunning) break;

        // Produce item
        Item item;
        item.id = m_nextItemId++;
        item.timestamp = GetTickCount();
        item.data.Format(_T("Item %d"), item.id);

        // Add to buffer
        {
            CSingleLock bufferLock(&m_bufferMutex, TRUE);
            m_buffer.Add(item);
            TRACE(_T("Produced: %s (Buffer: %d/%d)\n"),
                  item.data, m_buffer.GetCount(), m_bufferSize);
        }

        // Signal full slot
        m_fullSlots.Unlock();

        // Simulate production time
        Sleep(500);
    }

    TRACE(_T("Producer thread exiting\n"));
}

void CProducerConsumer::ConsumerLoop()
{
    while (m_bRunning)
    {
        // Wait for full slot
        CSingleLock fullLock(&m_fullSlots);
        if (!fullLock.Lock(1000))
            continue;

        if (!m_bRunning) break;

        // Remove from buffer
        Item item;
        {
            CSingleLock bufferLock(&m_bufferMutex, TRUE);
            if (m_buffer.GetCount() > 0)
            {
                item = m_buffer[0];
                m_buffer.RemoveAt(0);
                TRACE(_T("Consumed: %s (Buffer: %d/%d)\n"),
                      item.data, m_buffer.GetCount(), m_bufferSize);
            }
        }

        // Signal empty slot
        m_emptySlots.Unlock();

        // Simulate consumption time
        Sleep(800);
    }

    TRACE(_T("Consumer thread exiting\n"));
}

// Usage
void CMyApp::DemoProducerConsumer()
{
    CProducerConsumer pc(5);  // Buffer size 5

    pc.Start();

    // Let it run for 10 seconds
    Sleep(10000);

    pc.Stop();

    AfxMessageBox(_T("Producer-Consumer demo completed"));
}
```

---

## 7. Thread Pooling

### Advanced Thread Pool with Priority

```cpp
// PriorityThreadPool.h
class CPriorityThreadPool
{
public:
    enum Priority
    {
        LOW = 0,
        NORMAL = 1,
        HIGH = 2
    };

    struct Task
    {
        UINT (*function)(LPVOID);
        LPVOID parameter;
        Priority priority;
        int taskId;
        DWORD queueTime;

        BOOL operator<(const Task& other) const
        {
            if (priority != other.priority)
                return priority > other.priority;
            return queueTime < other.queueTime;
        }
    };

    CPriorityThreadPool(int threadCount = 4);
    ~CPriorityThreadPool();

    int QueueTask(UINT (*func)(LPVOID), LPVOID param, Priority priority = NORMAL);
    int GetQueuedTaskCount();
    int GetActiveThreadCount();
    void WaitForAll(DWORD timeout = INFINITE);
    void Shutdown();

private:
    static UINT WorkerProc(LPVOID pParam);
    void WorkerLoop();

    CArray<CWinThread*, CWinThread*> m_threads;
    CArray<Task, Task&> m_taskQueue;
    CMutex m_queueMutex;
    CEvent m_taskAvailable;
    CEvent m_allTasksComplete;
    volatile BOOL m_bShutdown;
    volatile LONG m_activeThreads;
    volatile LONG m_queuedTasks;
    int m_nextTaskId;
};

// Implementation
CPriorityThreadPool::CPriorityThreadPool(int threadCount)
    : m_taskAvailable(FALSE, TRUE),
      m_allTasksComplete(TRUE, TRUE),
      m_bShutdown(FALSE),
      m_activeThreads(0),
      m_queuedTasks(0),
      m_nextTaskId(1)
{
    for (int i = 0; i < threadCount; i++)
    {
        CWinThread* pThread = AfxBeginThread(
            WorkerProc,
            this,
            THREAD_PRIORITY_NORMAL,
            0,
            CREATE_SUSPENDED
        );

        if (pThread)
        {
            pThread->m_bAutoDelete = FALSE;
            pThread->ResumeThread();
            m_threads.Add(pThread);
        }
    }
}

int CPriorityThreadPool::QueueTask(
    UINT (*func)(LPVOID),
    LPVOID param,
    Priority priority)
{
    if (m_bShutdown) return -1;

    Task task;
    task.function = func;
    task.parameter = param;
    task.priority = priority;
    task.taskId = m_nextTaskId++;
    task.queueTime = GetTickCount();

    {
        CSingleLock lock(&m_queueMutex, TRUE);

        // Insert in priority order
        int insertPos = m_taskQueue.GetCount();
        for (int i = 0; i < m_taskQueue.GetCount(); i++)
        {
            if (task < m_taskQueue[i])
            {
                insertPos = i;
                break;
            }
        }

        m_taskQueue.InsertAt(insertPos, task);
        InterlockedIncrement(&m_queuedTasks);
        m_allTasksComplete.ResetEvent();
    }

    m_taskAvailable.SetEvent();

    return task.taskId;
}

void CPriorityThreadPool::WorkerLoop()
{
    while (!m_bShutdown)
    {
        Task task;
        BOOL bHasTask = FALSE;

        // Wait for task
        CSingleLock lock(&m_taskAvailable);
        if (!lock.Lock(1000))
            continue;

        if (m_bShutdown) break;

        // Get highest priority task
        {
            CSingleLock queueLock(&m_queueMutex, TRUE);
            if (!m_taskQueue.IsEmpty())
            {
                task = m_taskQueue[0];
                m_taskQueue.RemoveAt(0);
                bHasTask = TRUE;

                if (m_taskQueue.IsEmpty())
                {
                    m_taskAvailable.ResetEvent();
                }
            }
        }

        if (bHasTask)
        {
            InterlockedIncrement(&m_activeThreads);

            try
            {
                task.function(task.parameter);
            }
            catch (...)
            {
                TRACE(_T("Exception in task %d\n"), task.taskId);
            }

            InterlockedDecrement(&m_activeThreads);
            LONG remaining = InterlockedDecrement(&m_queuedTasks);

            if (remaining == 0 && m_taskQueue.IsEmpty())
            {
                m_allTasksComplete.SetEvent();
            }
        }
    }
}
```

---

## Key Takeaways

1. **Worker threads** handle background processing without UI
2. **UI threads** run their own message pump and create windows
3. **Thread pools** efficiently manage multiple concurrent tasks
4. **Message pumps** are essential for UI threads
5. **Producer-consumer** pattern coordinates data flow between threads
6. **Event-based communication** enables thread synchronization
7. **Queue-based patterns** decouple producers from consumers
8. **Priority queues** handle tasks based on importance
9. **Proper cleanup** prevents resource leaks
10. **Thread safety** requires careful synchronization

---

## Next Lesson Preview

In **Lesson 63: Thread Synchronization in MFC**, we'll explore:
- Critical sections and mutexes
- Semaphores and events
- Interlocked operations
- Deadlock prevention
- Reader-writer locks
- Synchronization best practices

**Master thread coordination for robust applications!**
