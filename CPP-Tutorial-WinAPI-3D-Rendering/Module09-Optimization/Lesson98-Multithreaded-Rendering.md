# Lesson 98: Multithreaded Rendering
**Estimated Time: 3-4 hours**

## Table of Contents
1. [Introduction to Multithreading](#introduction)
2. [Why Multithreaded Rendering?](#why-multithreading)
3. [DirectX 11 Deferred Contexts](#deferred-contexts)
4. [Command List Generation](#command-lists)
5. [Parallel Scene Processing](#parallel-scene)
6. [Thread Pool for Rendering](#thread-pool)
7. [Job System Implementation](#job-system)
8. [Synchronization and Safety](#synchronization)
9. [Complete Examples](#complete-examples)
10. [Exercises](#exercises)

## Introduction

**Goal:** Utilize multiple CPU cores for rendering to reduce frame time.

### CPU Usage Without Multithreading

```
Core 0: [████████████████] Rendering (100%)
Core 1: [░░░░] Idle (5%)
Core 2: [░░░░] Idle (5%)
Core 3: [░░░░] Idle (5%)

Result: 75% of CPU power wasted!
```

### CPU Usage With Multithreading

```
Core 0: [████████] Main thread (50%)
Core 1: [████████] Render worker (50%)
Core 2: [████████] Render worker (50%)
Core 3: [████████] Render worker (50%)

Result: 4x workload capacity!
```

## Why Multithreaded Rendering?

### The Problem

Modern games have:
- 10,000-100,000 objects
- Complex culling
- Many draw calls
- Animation/physics updates

**Single thread can't handle it all in 16ms (60 FPS)**

### Solutions

**1. Parallel Scene Processing**
```
Thread 1: Frustum cull objects 0-2500
Thread 2: Frustum cull objects 2500-5000
Thread 3: Frustum cull objects 5000-7500
Thread 4: Frustum cull objects 7500-10000
```

**2. Parallel Command Generation**
```
Thread 1: Generate draw commands for terrain
Thread 2: Generate draw commands for vegetation
Thread 3: Generate draw commands for buildings
Thread 4: Generate draw commands for characters
```

**3. Parallel System Updates**
```
Thread 1: Update particle systems
Thread 2: Update animation
Thread 3: Update physics
Thread 4: Update AI
```

## DirectX 11 Deferred Contexts

### Immediate vs Deferred Context

**Immediate Context:**
- One per device
- Commands execute immediately
- Thread-unsafe

**Deferred Context:**
- Multiple allowed
- Records commands to command list
- Thread-safe

### Creating Deferred Contexts

```cpp
ID3D11Device* device;
ID3D11DeviceContext* immediateContext;
std::vector<ID3D11DeviceContext*> deferredContexts;

void CreateDeferredContexts(int count) {
    for (int i = 0; i < count; ++i) {
        ID3D11DeviceContext* deferred;
        HRESULT hr = device->CreateDeferredContext(0, &deferred);
        if (SUCCEEDED(hr)) {
            deferredContexts.push_back(deferred);
        }
    }
}
```

### Recording Commands

```cpp
ID3D11CommandList* RecordDrawCommands(ID3D11DeviceContext* deferredContext,
                                       const std::vector<Mesh*>& meshes) {
    // Set state
    deferredContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deferredContext->VSSetShader(vertexShader, nullptr, 0);
    deferredContext->PSSetShader(pixelShader, nullptr, 0);

    // Record draw calls
    for (Mesh* mesh : meshes) {
        deferredContext->IASetVertexBuffers(0, 1, &mesh->vertexBuffer, ...);
        deferredContext->IASetIndexBuffer(mesh->indexBuffer, ...);
        deferredContext->DrawIndexed(mesh->indexCount, 0, 0);
    }

    // Finish recording and create command list
    ID3D11CommandList* commandList;
    deferredContext->FinishCommandList(FALSE, &commandList);

    return commandList;
}
```

### Executing Command Lists

```cpp
void ExecuteCommandLists(ID3D11DeviceContext* immediateContext,
                         const std::vector<ID3D11CommandList*>& commandLists) {
    for (ID3D11CommandList* cmdList : commandLists) {
        immediateContext->ExecuteCommandList(cmdList, FALSE);
        cmdList->Release();
    }
}
```

## Parallel Scene Processing

### Worker Thread Structure

```cpp
struct RenderWorker {
    std::thread thread;
    ID3D11DeviceContext* deferredContext;
    std::vector<Mesh*> meshes;
    ID3D11CommandList* commandList;
    std::atomic<bool> ready;
    std::atomic<bool> done;

    void WorkerFunction() {
        while (ready.load()) {
            if (meshes.empty()) {
                std::this_thread::sleep_for(std::chrono::microseconds(100));
                continue;
            }

            // Record commands
            commandList = RecordDrawCommands(deferredContext, meshes);

            // Clear for next frame
            meshes.clear();
            done.store(true);
        }
    }
};
```

### Parallel Culling and Rendering

```cpp
class ParallelRenderer {
private:
    std::vector<RenderWorker*> workers;
    std::vector<GameObject*> allObjects;

public:
    void Initialize(int workerCount) {
        for (int i = 0; i < workerCount; ++i) {
            RenderWorker* worker = new RenderWorker();
            device->CreateDeferredContext(0, &worker->deferredContext);
            worker->ready = true;
            worker->done = false;
            worker->thread = std::thread(&RenderWorker::WorkerFunction, worker);
            workers.push_back(worker);
        }
    }

    void RenderFrame(const Camera& camera) {
        // 1. Parallel frustum culling
        Frustum frustum = ExtractFrustum(camera.GetViewProjection());

        // Divide objects among workers
        int objectsPerWorker = allObjects.size() / workers.size();

        for (size_t i = 0; i < workers.size(); ++i) {
            int start = i * objectsPerWorker;
            int end = (i == workers.size() - 1) ? allObjects.size()
                                                 : (i + 1) * objectsPerWorker;

            // Cull objects for this worker
            for (int j = start; j < end; ++j) {
                if (IsInFrustum(allObjects[j]->bounds, frustum)) {
                    workers[i]->meshes.push_back(allObjects[j]->mesh);
                }
            }

            workers[i]->done.store(false);
        }

        // 2. Wait for workers to finish
        for (RenderWorker* worker : workers) {
            while (!worker->done.load()) {
                std::this_thread::yield();
            }
        }

        // 3. Execute command lists on immediate context
        for (RenderWorker* worker : workers) {
            if (worker->commandList) {
                immediateContext->ExecuteCommandList(worker->commandList, FALSE);
                worker->commandList->Release();
                worker->commandList = nullptr;
            }
        }
    }
};
```

## Thread Pool for Rendering

### Thread Pool Implementation

```cpp
class ThreadPool {
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> stop;

public:
    ThreadPool(int threadCount) : stop(false) {
        for (int i = 0; i < threadCount; ++i) {
            threads.emplace_back([this] {
                while (true) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] {
                            return stop.load() || !tasks.empty();
                        });

                        if (stop.load() && tasks.empty()) {
                            return;
                        }

                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        stop.store(true);
        condition.notify_all();
        for (std::thread& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    template<typename F>
    void Enqueue(F&& task) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            tasks.emplace(std::forward<F>(task));
        }
        condition.notify_one();
    }

    void WaitAll() {
        while (true) {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (tasks.empty()) break;
            std::this_thread::yield();
        }
    }
};
```

### Using Thread Pool for Rendering

```cpp
ThreadPool pool(4);

void RenderScene() {
    std::vector<ID3D11CommandList*> commandLists(4);

    // Submit tasks
    pool.Enqueue([&, 0] {
        commandLists[0] = RenderTerrain(deferredContexts[0]);
    });

    pool.Enqueue([&, 1] {
        commandLists[1] = RenderVegetation(deferredContexts[1]);
    });

    pool.Enqueue([&, 2] {
        commandLists[2] = RenderBuildings(deferredContexts[2]);
    });

    pool.Enqueue([&, 3] {
        commandLists[3] = RenderCharacters(deferredContexts[3]);
    });

    // Wait for completion
    pool.WaitAll();

    // Execute on immediate context
    for (ID3D11CommandList* cmdList : commandLists) {
        if (cmdList) {
            immediateContext->ExecuteCommandList(cmdList, FALSE);
            cmdList->Release();
        }
    }
}
```

## Job System Implementation

### Job Structure

```cpp
struct Job {
    std::function<void()> function;
    std::atomic<int>* counter;  // For dependencies

    Job(std::function<void()> func, std::atomic<int>* cnt = nullptr)
        : function(func), counter(cnt) {}
};

class JobSystem {
private:
    ThreadPool pool;
    std::queue<Job> jobs;
    std::mutex jobMutex;

public:
    JobSystem(int threadCount) : pool(threadCount) {}

    void Submit(Job job) {
        pool.Enqueue([job] {
            job.function();
            if (job.counter) {
                job.counter->fetch_sub(1);
            }
        });
    }

    void WaitForCounter(std::atomic<int>& counter) {
        while (counter.load() > 0) {
            std::this_thread::yield();
        }
    }
};
```

### Example: Parallel Animation Update

```cpp
JobSystem jobSystem(4);

void UpdateAnimations(std::vector<AnimatedMesh*>& meshes) {
    std::atomic<int> remainingJobs(meshes.size());

    for (AnimatedMesh* mesh : meshes) {
        Job job([mesh] {
            mesh->UpdateAnimation(deltaTime);
        }, &remainingJobs);

        jobSystem.Submit(job);
    }

    // Wait for all animations to finish
    jobSystem.WaitForCounter(remainingJobs);

    // Now safe to render
}
```

## Synchronization and Safety

### Thread-Safe Resource Access

```cpp
// BAD: Race condition
std::vector<Mesh*> visibleMeshes;  // Shared

void Worker1() {
    visibleMeshes.push_back(mesh1);  // Race!
}

void Worker2() {
    visibleMeshes.push_back(mesh2);  // Race!
}

// GOOD: Per-thread storage
struct WorkerData {
    std::vector<Mesh*> visibleMeshes;
};

std::vector<WorkerData> workerData(4);

void Worker1() {
    workerData[0].visibleMeshes.push_back(mesh1);  // Safe
}

void Worker2() {
    workerData[1].visibleMeshes.push_back(mesh2);  // Safe
}
```

### Lock-Free Queues

```cpp
template<typename T>
class LockFreeQueue {
private:
    std::atomic<size_t> head;
    std::atomic<size_t> tail;
    std::vector<T> buffer;

public:
    LockFreeQueue(size_t capacity) : head(0), tail(0), buffer(capacity) {}

    bool Push(const T& item) {
        size_t t = tail.load();
        size_t next = (t + 1) % buffer.size();

        if (next == head.load()) {
            return false;  // Queue full
        }

        buffer[t] = item;
        tail.store(next);
        return true;
    }

    bool Pop(T& item) {
        size_t h = head.load();

        if (h == tail.load()) {
            return false;  // Queue empty
        }

        item = buffer[h];
        head.store((h + 1) % buffer.size());
        return true;
    }
};
```

## Performance Impact

### Benchmark: 10,000 Objects

```
Single-threaded:
  Culling:  8ms
  Drawing:  10ms
  Total:    18ms
  FPS:      55

4-threaded:
  Culling:  2ms (4x speedup)
  Drawing:  3ms (command list generation parallel)
  Total:    5ms
  FPS:      200

Speedup: 3.6x
```

### Best Practices

1. **Minimize synchronization** - Use per-thread data
2. **Batch work** - Don't create job per object
3. **Profile** - Find actual bottlenecks
4. **Avoid false sharing** - Cache line alignment
5. **Keep workers busy** - Balance workload

## Summary

**Key Concepts:**
1. **Deferred contexts** for parallel command generation
2. **Thread pools** for task distribution
3. **Job systems** for dependency management
4. **Lock-free structures** for performance
5. **Careful synchronization** to avoid races

**Typical Speedups:**
- 2-4x with 4 cores
- Diminishing returns beyond 4-8 threads
- Best for CPU-bound scenarios

**When to Use:**
- Large scenes (10,000+ objects)
- Complex culling/sorting
- Heavy CPU workload
- Target: High-end PCs with many cores

---

**Congratulations on completing Lesson 98 and Module 9!**

**You've mastered optimization techniques that can make your applications 10-1000x faster!**
