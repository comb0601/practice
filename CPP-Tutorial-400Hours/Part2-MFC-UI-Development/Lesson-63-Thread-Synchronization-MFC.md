# Lesson 63: Thread Synchronization in MFC

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Thread Synchronization
2. Critical Sections (CCriticalSection)
3. Mutexes (CMutex)
4. Semaphores (CSemaphore)
5. Events (CEvent)
6. Interlocked Operations
7. Multiple Wait Operations
8. Deadlock Prevention
9. Reader-Writer Locks
10. Advanced Synchronization Patterns
11. Best Practices
12. Exercises and Projects

---

## 1. Introduction to Thread Synchronization

### Why Synchronization is Needed

```cpp
// PROBLEM: Race condition without synchronization
class CUnsafeCounter
{
public:
    CUnsafeCounter() : m_count(0) {}

    void Increment()
    {
        // NOT ATOMIC! Three operations:
        // 1. Read m_count
        // 2. Add 1
        // 3. Write back to m_count
        m_count++;
    }

    int GetCount() { return m_count; }

private:
    int m_count;  // Shared data - UNSAFE!
};

// Two threads incrementing simultaneously can lose updates!
// Thread 1: Read (0) -> Add 1 (1) -> [interrupted]
// Thread 2: Read (0) -> Add 1 (1) -> Write (1)
// Thread 1: [resumes] -> Write (1)
// Result: 1 instead of 2!
```

### MFC Synchronization Objects

```cpp
/*
MFC provides several synchronization classes:

1. CCriticalSection
   - Fast, lightweight
   - Single process only
   - No ownership tracking
   - Best for short critical sections

2. CMutex
   - Named or unnamed
   - Cross-process capable
   - Ownership tracking
   - Handles abandoned mutex

3. CSemaphore
   - Counting semaphore
   - Multiple concurrent access
   - Resource pool management

4. CEvent
   - Signaling between threads
   - Manual or auto-reset
   - Waiting for conditions

5. CSingleLock / CMultiLock
   - Helper classes for locking
   - RAII pattern
   - Automatic unlock
*/
```

---

## 2. Critical Sections (CCriticalSection)

### Basic Usage

```cpp
// CriticalSectionExample.h
class CSafeCounter
{
public:
    CSafeCounter() : m_count(0) {}

    void Increment()
    {
        CSingleLock lock(&m_cs, TRUE);  // Auto-lock
        m_count++;
        // Automatically unlocked when lock goes out of scope
    }

    void Decrement()
    {
        CSingleLock lock(&m_cs, TRUE);
        m_count--;
    }

    int GetCount()
    {
        CSingleLock lock(&m_cs, TRUE);
        return m_count;
    }

    void Add(int value)
    {
        CSingleLock lock(&m_cs, TRUE);
        m_count += value;
    }

private:
    int m_count;
    CCriticalSection m_cs;
};

// Thread function using safe counter
UINT IncrementThreadProc(LPVOID pParam)
{
    CSafeCounter* pCounter = (CSafeCounter*)pParam;

    for (int i = 0; i < 1000; i++)
    {
        pCounter->Increment();
        Sleep(1);  // Simulate other work
    }

    return 0;
}

// Using the safe counter
void CMyApp::TestSafeCounter()
{
    CSafeCounter counter;

    // Create multiple threads
    const int THREAD_COUNT = 5;
    CWinThread* threads[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        threads[i] = AfxBeginThread(
            IncrementThreadProc,
            &counter,
            THREAD_PRIORITY_NORMAL,
            0,
            CREATE_SUSPENDED
        );
        threads[i]->m_bAutoDelete = FALSE;
    }

    // Start all threads
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        threads[i]->ResumeThread();
    }

    // Wait for all threads
    HANDLE handles[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        handles[i] = threads[i]->m_hThread;
    }

    WaitForMultipleObjects(THREAD_COUNT, handles, TRUE, INFINITE);

    // Cleanup
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        delete threads[i];
    }

    CString msg;
    msg.Format(_T("Final count: %d (expected: %d)"),
               counter.GetCount(), THREAD_COUNT * 1000);
    AfxMessageBox(msg);
}
```

### Advanced Critical Section Usage

```cpp
// Thread-safe collection
template<typename T>
class CThreadSafeArray
{
public:
    CThreadSafeArray() {}

    void Add(const T& item)
    {
        CSingleLock lock(&m_cs, TRUE);
        m_array.Add(item);
    }

    BOOL RemoveAt(int index)
    {
        CSingleLock lock(&m_cs, TRUE);
        if (index >= 0 && index < m_array.GetCount())
        {
            m_array.RemoveAt(index);
            return TRUE;
        }
        return FALSE;
    }

    T GetAt(int index)
    {
        CSingleLock lock(&m_cs, TRUE);
        if (index >= 0 && index < m_array.GetCount())
        {
            return m_array[index];
        }
        throw CException();  // Or return default value
    }

    int GetCount()
    {
        CSingleLock lock(&m_cs, TRUE);
        return m_array.GetCount();
    }

    void Clear()
    {
        CSingleLock lock(&m_cs, TRUE);
        m_array.RemoveAll();
    }

    // Iterate with callback (keeps lock during iteration)
    void ForEach(void (*callback)(const T& item))
    {
        CSingleLock lock(&m_cs, TRUE);
        for (int i = 0; i < m_array.GetCount(); i++)
        {
            callback(m_array[i]);
        }
    }

    // Copy all items (safer for iteration outside lock)
    void GetAll(CArray<T, const T&>& dest)
    {
        CSingleLock lock(&m_cs, TRUE);
        dest.RemoveAll();
        dest.Append(m_array);
    }

private:
    CArray<T, const T&> m_array;
    CCriticalSection m_cs;
};

// Usage
void ProcessItems(const CString& item)
{
    TRACE(_T("Item: %s\n"), item);
}

void CMyApp::TestThreadSafeArray()
{
    CThreadSafeArray<CString> items;

    items.Add(_T("Item 1"));
    items.Add(_T("Item 2"));
    items.Add(_T("Item 3"));

    items.ForEach(ProcessItems);

    // Or copy for safe iteration
    CArray<CString, const CString&> copy;
    items.GetAll(copy);

    for (int i = 0; i < copy.GetCount(); i++)
    {
        TRACE(_T("%s\n"), copy[i]);
    }
}
```

---

## 3. Mutexes (CMutex)

### Basic Mutex Usage

```cpp
// Named mutex for single instance application
class CSingleInstanceApp
{
public:
    CSingleInstanceApp(LPCTSTR lpszMutexName)
    {
        m_pMutex = new CMutex(
            FALSE,              // Initially not owned
            lpszMutexName,      // Mutex name
            NULL                // Security attributes
        );

        m_bAlreadyRunning = (GetLastError() == ERROR_ALREADY_EXISTS);
    }

    ~CSingleInstanceApp()
    {
        if (m_pMutex)
            delete m_pMutex;
    }

    BOOL IsAlreadyRunning() const
    {
        return m_bAlreadyRunning;
    }

private:
    CMutex* m_pMutex;
    BOOL m_bAlreadyRunning;
};

// In application class
BOOL CMyApp::InitInstance()
{
    CSingleInstanceApp singleInstance(_T("MyUniqueAppMutex_{GUID}"));

    if (singleInstance.IsAlreadyRunning())
    {
        AfxMessageBox(_T("Application is already running!"));
        return FALSE;
    }

    // Continue with normal initialization
    return TRUE;
}
```

### Cross-Process Synchronization

```cpp
// SharedResource.h
class CSharedResourceManager
{
public:
    CSharedResourceManager()
        : m_pMutex(NULL),
          m_pSharedMemory(NULL),
          m_hMapFile(NULL)
    {
    }

    ~CSharedResourceManager()
    {
        Cleanup();
    }

    BOOL Initialize()
    {
        // Create named mutex
        m_pMutex = new CMutex(
            FALSE,
            _T("Global\\MySharedResourceMutex")
        );

        // Create file mapping
        m_hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            sizeof(SharedData),
            _T("Global\\MySharedMemory")
        );

        if (m_hMapFile == NULL)
            return FALSE;

        m_pSharedMemory = (SharedData*)MapViewOfFile(
            m_hMapFile,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            sizeof(SharedData)
        );

        if (m_pSharedMemory == NULL)
            return FALSE;

        return TRUE;
    }

    BOOL WriteData(const CString& data)
    {
        CSingleLock lock(m_pMutex, TRUE);

        if (m_pSharedMemory)
        {
            _tcscpy_s(m_pSharedMemory->buffer, 256, data);
            m_pSharedMemory->timestamp = GetTickCount();
            return TRUE;
        }

        return FALSE;
    }

    BOOL ReadData(CString& data)
    {
        CSingleLock lock(m_pMutex, TRUE);

        if (m_pSharedMemory)
        {
            data = m_pSharedMemory->buffer;
            return TRUE;
        }

        return FALSE;
    }

    void Cleanup()
    {
        if (m_pSharedMemory)
        {
            UnmapViewOfFile(m_pSharedMemory);
            m_pSharedMemory = NULL;
        }

        if (m_hMapFile)
        {
            CloseHandle(m_hMapFile);
            m_hMapFile = NULL;
        }

        if (m_pMutex)
        {
            delete m_pMutex;
            m_pMutex = NULL;
        }
    }

private:
    struct SharedData
    {
        TCHAR buffer[256];
        DWORD timestamp;
    };

    CMutex* m_pMutex;
    SharedData* m_pSharedMemory;
    HANDLE m_hMapFile;
};
```

### Abandoned Mutex Handling

```cpp
BOOL SafeMutexLock(CMutex* pMutex, DWORD dwTimeout = INFINITE)
{
    CSingleLock lock(pMutex);
    BOOL bLocked = lock.Lock(dwTimeout);

    if (bLocked)
    {
        // Check if mutex was abandoned
        DWORD dwResult = WaitForSingleObject(pMutex->m_hObject, 0);

        if (dwResult == WAIT_ABANDONED)
        {
            TRACE(_T("Warning: Mutex was abandoned!\n"));
            // Take ownership and continue
            // May need to verify/repair protected data
        }

        return TRUE;
    }

    return FALSE;
}
```

---

## 4. Semaphores (CSemaphore)

### Resource Pool Management

```cpp
// ResourcePool.h
template<typename T>
class CResourcePool
{
public:
    CResourcePool(int maxResources)
        : m_semaphore(maxResources, maxResources),
          m_maxResources(maxResources)
    {
    }

    ~CResourcePool()
    {
        CSingleLock lock(&m_mutex, TRUE);
        while (!m_availableResources.IsEmpty())
        {
            T* pResource = m_availableResources.RemoveHead();
            delete pResource;
        }
    }

    void AddResource(T* pResource)
    {
        CSingleLock lock(&m_mutex, TRUE);
        m_availableResources.AddTail(pResource);
    }

    T* AcquireResource(DWORD dwTimeout = INFINITE)
    {
        // Wait for available resource
        CSingleLock semLock(&m_semaphore);
        if (!semLock.Lock(dwTimeout))
            return NULL;

        // Get resource from pool
        CSingleLock mutexLock(&m_mutex, TRUE);
        if (m_availableResources.IsEmpty())
            return NULL;

        T* pResource = m_availableResources.RemoveHead();
        m_activeResources.AddTail(pResource);

        return pResource;
    }

    void ReleaseResource(T* pResource)
    {
        CSingleLock lock(&m_mutex, TRUE);

        POSITION pos = m_activeResources.Find(pResource);
        if (pos != NULL)
        {
            m_activeResources.RemoveAt(pos);
            m_availableResources.AddTail(pResource);

            // Signal resource available
            m_semaphore.Unlock();
        }
    }

    int GetAvailableCount()
    {
        CSingleLock lock(&m_mutex, TRUE);
        return m_availableResources.GetCount();
    }

    int GetActiveCount()
    {
        CSingleLock lock(&m_mutex, TRUE);
        return m_activeResources.GetCount();
    }

private:
    CSemaphore m_semaphore;
    CMutex m_mutex;
    CList<T*, T*> m_availableResources;
    CList<T*, T*> m_activeResources;
    int m_maxResources;
};

// Database connection pool example
class CDBConnection
{
public:
    CDBConnection(const CString& connectionString)
        : m_connectionString(connectionString),
          m_bConnected(FALSE)
    {
    }

    BOOL Connect()
    {
        // Simulate connection
        Sleep(100);
        m_bConnected = TRUE;
        return TRUE;
    }

    void Disconnect()
    {
        m_bConnected = FALSE;
    }

    BOOL ExecuteQuery(const CString& query)
    {
        if (!m_bConnected) return FALSE;

        TRACE(_T("Executing: %s\n"), query);
        Sleep(50);  // Simulate query execution
        return TRUE;
    }

private:
    CString m_connectionString;
    BOOL m_bConnected;
};

// Usage
void CMyApp::TestConnectionPool()
{
    CResourcePool<CDBConnection> pool(3);  // Max 3 connections

    // Initialize pool
    for (int i = 0; i < 3; i++)
    {
        CDBConnection* pConn = new CDBConnection(_T("Server=localhost"));
        pConn->Connect();
        pool.AddResource(pConn);
    }

    // Worker thread
    struct WorkerData
    {
        CResourcePool<CDBConnection>* pPool;
        int workerId;
    };

    auto worker = [](LPVOID pParam) -> UINT
    {
        WorkerData* pData = (WorkerData*)pParam;

        for (int i = 0; i < 5; i++)
        {
            // Acquire connection
            CDBConnection* pConn = pData->pPool->AcquireResource(5000);

            if (pConn)
            {
                CString query;
                query.Format(_T("SELECT * FROM Table%d"), pData->workerId);
                pConn->ExecuteQuery(query);

                // Release connection
                pData->pPool->ReleaseResource(pConn);
            }
            else
            {
                TRACE(_T("Worker %d: Timeout acquiring connection\n"),
                      pData->workerId);
            }

            Sleep(100);
        }

        delete pData;
        return 0;
    };

    // Start multiple workers
    for (int i = 0; i < 5; i++)
    {
        WorkerData* pData = new WorkerData;
        pData->pPool = &pool;
        pData->workerId = i;

        AfxBeginThread(worker, pData);
    }

    Sleep(5000);  // Let workers complete
}
```

### Throttling with Semaphores

```cpp
class CThrottledTaskExecutor
{
public:
    CThrottledTaskExecutor(int maxConcurrent)
        : m_semaphore(maxConcurrent, maxConcurrent),
          m_maxConcurrent(maxConcurrent)
    {
    }

    typedef UINT (*TaskFunc)(LPVOID);

    void ExecuteTask(TaskFunc func, LPVOID pParam)
    {
        // Wait for slot
        CSingleLock lock(&m_semaphore, TRUE);

        // Execute in new thread
        struct ThreadData
        {
            TaskFunc function;
            LPVOID parameter;
            CSemaphore* pSemaphore;
        };

        ThreadData* pData = new ThreadData;
        pData->function = func;
        pData->parameter = pParam;
        pData->pSemaphore = &m_semaphore;

        AfxBeginThread([](LPVOID p) -> UINT
        {
            ThreadData* pData = (ThreadData*)p;

            UINT result = pData->function(pData->parameter);

            // Release slot
            pData->pSemaphore->Unlock();

            delete pData;
            return result;
        }, pData);
    }

private:
    CSemaphore m_semaphore;
    int m_maxConcurrent;
};
```

---

## 5. Events (CEvent)

### Manual-Reset Event

```cpp
class CWorkerWithEvent
{
public:
    CWorkerWithEvent()
        : m_readyEvent(FALSE, FALSE),     // Manual reset, initially non-signaled
          m_stopEvent(TRUE, FALSE),       // Manual reset, initially non-signaled
          m_pThread(NULL)
    {
    }

    ~CWorkerWithEvent()
    {
        Stop();
    }

    BOOL Start()
    {
        m_stopEvent.ResetEvent();

        m_pThread = AfxBeginThread(
            ThreadProc,
            this,
            THREAD_PRIORITY_NORMAL,
            0,
            CREATE_SUSPENDED
        );

        if (m_pThread)
        {
            m_pThread->m_bAutoDelete = FALSE;
            m_pThread->ResumeThread();

            // Wait for thread to be ready
            WaitForSingleObject(m_readyEvent.m_hObject, 5000);

            return TRUE;
        }

        return FALSE;
    }

    void Stop()
    {
        if (m_pThread)
        {
            m_stopEvent.SetEvent();
            WaitForSingleObject(m_pThread->m_hThread, 5000);
            delete m_pThread;
            m_pThread = NULL;
        }
    }

    static UINT ThreadProc(LPVOID pParam)
    {
        CWorkerWithEvent* pThis = (CWorkerWithEvent*)pParam;
        return pThis->Run();
    }

    UINT Run()
    {
        // Initialization
        TRACE(_T("Thread initializing...\n"));
        Sleep(500);  // Simulate initialization

        // Signal ready
        m_readyEvent.SetEvent();

        // Main loop
        while (TRUE)
        {
            DWORD dwResult = WaitForSingleObject(
                m_stopEvent.m_hObject,
                100  // Check every 100ms
            );

            if (dwResult == WAIT_OBJECT_0)
            {
                TRACE(_T("Stop event signaled\n"));
                break;
            }

            // Do work
            TRACE(_T("Working...\n"));
        }

        TRACE(_T("Thread exiting\n"));
        return 0;
    }

private:
    CEvent m_readyEvent;
    CEvent m_stopEvent;
    CWinThread* m_pThread;
};
```

### Auto-Reset Event

```cpp
class CAutoResetEventDemo
{
public:
    CAutoResetEventDemo()
        : m_taskEvent(FALSE, TRUE),  // Auto-reset, initially non-signaled
          m_bShutdown(FALSE)
    {
    }

    void Start()
    {
        m_bShutdown = FALSE;

        AfxBeginThread([](LPVOID p) -> UINT
        {
            CAutoResetEventDemo* pThis = (CAutoResetEventDemo*)p;

            while (!pThis->m_bShutdown)
            {
                // Wait for task
                DWORD dwResult = WaitForSingleObject(
                    pThis->m_taskEvent.m_hObject,
                    1000
                );

                if (dwResult == WAIT_OBJECT_0)
                {
                    // Auto-reset event automatically resets after wait succeeds
                    TRACE(_T("Task triggered!\n"));
                    Sleep(500);  // Process task
                }
            }

            return 0;
        }, this);
    }

    void TriggerTask()
    {
        m_taskEvent.SetEvent();  // Will auto-reset after one thread wakes
    }

    void Shutdown()
    {
        m_bShutdown = TRUE;
        m_taskEvent.SetEvent();
    }

private:
    CEvent m_taskEvent;
    volatile BOOL m_bShutdown;
};
```

### Multiple Event Coordination

```cpp
class CMultiEventCoordinator
{
public:
    CMultiEventCoordinator()
        : m_event1(TRUE, FALSE),
          m_event2(TRUE, FALSE),
          m_event3(TRUE, FALSE)
    {
    }

    void WaitForAll()
    {
        HANDLE handles[3] =
        {
            m_event1.m_hObject,
            m_event2.m_hObject,
            m_event3.m_hObject
        };

        DWORD dwResult = WaitForMultipleObjects(
            3,
            handles,
            TRUE,     // Wait for all
            INFINITE
        );

        if (dwResult == WAIT_OBJECT_0)
        {
            TRACE(_T("All events signaled!\n"));
        }
    }

    void WaitForAny()
    {
        HANDLE handles[3] =
        {
            m_event1.m_hObject,
            m_event2.m_hObject,
            m_event3.m_hObject
        };

        DWORD dwResult = WaitForMultipleObjects(
            3,
            handles,
            FALSE,    // Wait for any
            INFINITE
        );

        if (dwResult >= WAIT_OBJECT_0 && dwResult < WAIT_OBJECT_0 + 3)
        {
            int eventIndex = dwResult - WAIT_OBJECT_0;
            TRACE(_T("Event %d signaled!\n"), eventIndex + 1);
        }
    }

    CEvent m_event1;
    CEvent m_event2;
    CEvent m_event3;
};
```

---

## 6. Interlocked Operations

### Atomic Operations

```cpp
class CAtomicCounter
{
public:
    CAtomicCounter() : m_value(0) {}

    // Atomic increment
    LONG Increment()
    {
        return InterlockedIncrement(&m_value);
    }

    // Atomic decrement
    LONG Decrement()
    {
        return InterlockedDecrement(&m_value);
    }

    // Atomic add
    LONG Add(LONG delta)
    {
        return InterlockedExchangeAdd(&m_value, delta);
    }

    // Atomic exchange (set and return old value)
    LONG Exchange(LONG newValue)
    {
        return InterlockedExchange(&m_value, newValue);
    }

    // Compare and exchange
    LONG CompareExchange(LONG exchange, LONG comparand)
    {
        return InterlockedCompareExchange(&m_value, exchange, comparand);
    }

    // Get current value
    LONG GetValue() const
    {
        return InterlockedCompareExchange(
            const_cast<LONG*>(&m_value),
            0,
            0
        );
    }

private:
    volatile LONG m_value;
};

// Lock-free stack using interlocked operations
template<typename T>
class CLockFreeStack
{
private:
    struct Node
    {
        T data;
        Node* next;
    };

public:
    CLockFreeStack() : m_head(NULL) {}

    ~CLockFreeStack()
    {
        while (m_head)
        {
            Node* temp = m_head;
            m_head = m_head->next;
            delete temp;
        }
    }

    void Push(const T& value)
    {
        Node* newNode = new Node;
        newNode->data = value;

        do
        {
            newNode->next = m_head;
        } while (InterlockedCompareExchangePointer(
            (PVOID*)&m_head,
            newNode,
            newNode->next
        ) != newNode->next);
    }

    BOOL Pop(T& value)
    {
        Node* oldHead;

        do
        {
            oldHead = m_head;
            if (oldHead == NULL)
                return FALSE;

        } while (InterlockedCompareExchangePointer(
            (PVOID*)&m_head,
            oldHead->next,
            oldHead
        ) != oldHead);

        value = oldHead->data;
        delete oldHead;
        return TRUE;
    }

private:
    Node* volatile m_head;
};
```

---

## 7. Multiple Wait Operations

### CMultiLock Usage

```cpp
class CMultiLockExample
{
public:
    CMultiLockExample()
        : m_mutex1(FALSE, _T("Mutex1")),
          m_mutex2(FALSE, _T("Mutex2")),
          m_event(TRUE, FALSE)
    {
    }

    void WaitForMultiple()
    {
        CSyncObject* objects[3] =
        {
            &m_mutex1,
            &m_mutex2,
            &m_event
        };

        CMultiLock multiLock(objects, 3);

        // Wait for all objects
        DWORD dwResult = multiLock.Lock(
            5000,    // Timeout
            FALSE    // Wait for all (TRUE = wait for any)
        );

        if (dwResult == WAIT_OBJECT_0)
        {
            // All objects acquired
            TRACE(_T("All synchronization objects acquired\n"));

            // Do work...

            // Automatically unlocked when multiLock goes out of scope
        }
        else if (dwResult == WAIT_TIMEOUT)
        {
            TRACE(_T("Timeout waiting for objects\n"));
        }
    }

    void WaitForAny()
    {
        CSyncObject* objects[3] =
        {
            &m_mutex1,
            &m_mutex2,
            &m_event
        };

        CMultiLock multiLock(objects, 3);

        DWORD dwResult = multiLock.Lock(
            INFINITE,
            TRUE      // Wait for any
        );

        if (dwResult >= WAIT_OBJECT_0 && dwResult < WAIT_OBJECT_0 + 3)
        {
            int index = dwResult - WAIT_OBJECT_0;
            TRACE(_T("Object %d signaled\n"), index);

            // Check which object was signaled
            if (multiLock.IsLocked(0))
            {
                TRACE(_T("Mutex1 locked\n"));
            }
            if (multiLock.IsLocked(1))
            {
                TRACE(_T("Mutex2 locked\n"));
            }
            if (multiLock.IsLocked(2))
            {
                TRACE(_T("Event signaled\n"));
            }
        }
    }

private:
    CMutex m_mutex1;
    CMutex m_mutex2;
    CEvent m_event;
};
```

---

## 8. Deadlock Prevention

### Deadlock Scenario

```cpp
// DEADLOCK EXAMPLE - Don't do this!
class CDeadlockExample
{
public:
    void Thread1()
    {
        CSingleLock lock1(&m_mutex1, TRUE);
        Sleep(100);  // Increase chance of deadlock
        CSingleLock lock2(&m_mutex2, TRUE);  // DEADLOCK!

        // Do work with both mutexes...
    }

    void Thread2()
    {
        CSingleLock lock2(&m_mutex2, TRUE);
        Sleep(100);  // Increase chance of deadlock
        CSingleLock lock1(&m_mutex1, TRUE);  // DEADLOCK!

        // Do work with both mutexes...
    }

private:
    CMutex m_mutex1;
    CMutex m_mutex2;
};
```

### Solution 1: Lock Ordering

```cpp
class CDeadlockFreeOrdering
{
public:
    void Thread1()
    {
        // Always acquire mutexes in same order
        CSingleLock lock1(&m_mutex1, TRUE);
        CSingleLock lock2(&m_mutex2, TRUE);

        // Do work...
    }

    void Thread2()
    {
        // Same order as Thread1
        CSingleLock lock1(&m_mutex1, TRUE);
        CSingleLock lock2(&m_mutex2, TRUE);

        // Do work...
    }

private:
    CMutex m_mutex1;
    CMutex m_mutex2;
};
```

### Solution 2: Try-Lock with Timeout

```cpp
class CDeadlockFreeTryLock
{
public:
    BOOL AcquireBothLocks(DWORD dwTimeout)
    {
        DWORD startTime = GetTickCount();

        while (TRUE)
        {
            // Try to acquire first lock
            CSingleLock lock1(&m_mutex1);
            if (!lock1.Lock(100))
            {
                if (GetTickCount() - startTime > dwTimeout)
                    return FALSE;
                continue;
            }

            // Try to acquire second lock
            CSingleLock lock2(&m_mutex2);
            if (!lock2.Lock(100))
            {
                if (GetTickCount() - startTime > dwTimeout)
                    return FALSE;

                lock1.Unlock();
                Sleep(10);  // Back off
                continue;
            }

            // Both locks acquired
            // Do work...

            return TRUE;
        }
    }

private:
    CMutex m_mutex1;
    CMutex m_mutex2;
};
```

### Solution 3: CMultiLock

```cpp
class CDeadlockFreeMultiLock
{
public:
    void DoWork()
    {
        CSyncObject* objects[2] = { &m_mutex1, &m_mutex2 };
        CMultiLock multiLock(objects, 2);

        // Atomic acquisition of all locks
        if (multiLock.Lock(5000))
        {
            // Both mutexes acquired atomically
            // Do work...

            // Automatically unlocked
        }
    }

private:
    CMutex m_mutex1;
    CMutex m_mutex2;
};
```

---

## 9. Reader-Writer Locks

### Implementation Using Events and Counters

```cpp
class CReaderWriterLock
{
public:
    CReaderWriterLock()
        : m_readerCount(0),
          m_writerCount(0),
          m_writeEvent(TRUE, TRUE)  // Initially signaled
    {
    }

    void AcquireReaderLock()
    {
        while (TRUE)
        {
            CSingleLock lock(&m_mutex, TRUE);

            // Wait if writer is active
            if (m_writerCount == 0)
            {
                m_readerCount++;
                return;
            }

            lock.Unlock();
            Sleep(10);
        }
    }

    void ReleaseReaderLock()
    {
        CSingleLock lock(&m_mutex, TRUE);
        m_readerCount--;

        if (m_readerCount == 0)
        {
            m_writeEvent.SetEvent();
        }
    }

    void AcquireWriterLock()
    {
        CSingleLock lock(&m_mutex, TRUE);

        m_writerCount++;

        // Wait for all readers to finish
        while (m_readerCount > 0)
        {
            lock.Unlock();
            WaitForSingleObject(m_writeEvent.m_hObject, 10);
            lock.Lock();
        }

        m_writeEvent.ResetEvent();
    }

    void ReleaseWriterLock()
    {
        CSingleLock lock(&m_mutex, TRUE);
        m_writerCount--;

        if (m_writerCount == 0)
        {
            m_writeEvent.SetEvent();
        }
    }

private:
    volatile LONG m_readerCount;
    volatile LONG m_writerCount;
    CMutex m_mutex;
    CEvent m_writeEvent;
};

// RAII wrappers
class CReaderLock
{
public:
    CReaderLock(CReaderWriterLock& lock) : m_lock(lock)
    {
        m_lock.AcquireReaderLock();
    }

    ~CReaderLock()
    {
        m_lock.ReleaseReaderLock();
    }

private:
    CReaderWriterLock& m_lock;
};

class CWriterLock
{
public:
    CWriterLock(CReaderWriterLock& lock) : m_lock(lock)
    {
        m_lock.AcquireWriterLock();
    }

    ~CWriterLock()
    {
        m_lock.ReleaseWriterLock();
    }

private:
    CReaderWriterLock& m_lock;
};

// Usage
class CSharedData
{
public:
    void WriteData(const CString& data)
    {
        CWriterLock lock(m_rwLock);
        m_data = data;
        TRACE(_T("Data written: %s\n"), data);
    }

    CString ReadData()
    {
        CReaderLock lock(m_rwLock);
        return m_data;
    }

private:
    CString m_data;
    CReaderWriterLock m_rwLock;
};
```

---

## Key Takeaways

1. **Critical sections** are fastest for single-process synchronization
2. **Mutexes** support cross-process synchronization
3. **Semaphores** manage resource pools and throttling
4. **Events** signal conditions between threads
5. **Interlocked operations** provide lock-free atomic operations
6. **CMultiLock** simplifies multiple wait operations
7. **Deadlocks** must be prevented through careful design
8. **Lock ordering** is crucial for deadlock prevention
9. **Reader-writer locks** optimize read-heavy scenarios
10. **RAII pattern** ensures proper lock cleanup

---

## Next Lesson Preview

In **Lesson 64: DLLs with MFC (Regular DLL)**, we'll explore:
- Creating regular DLLs with MFC
- Static vs dynamic linking
- Exporting functions and classes
- Resource management
- DLL initialization
- Best practices for MFC DLLs

**Master synchronization for bulletproof multithreading!**
