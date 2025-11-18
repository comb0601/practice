# Lesson 02: Background Workers & Threading

## Overview
This lesson covers background thread management in WPF applications, including BackgroundWorker, Thread, ThreadPool, Dispatcher optimization, and advanced threading patterns.

## Learning Objectives
- Master BackgroundWorker for simple background operations
- Understand Thread vs ThreadPool differences and performance
- Optimize Dispatcher usage for UI updates
- Implement thread-safe patterns and synchronization

## Key Concepts

### 1. BackgroundWorker Component
- **Progress Reporting**: Real-time progress updates to UI
- **Cancellation Support**: Graceful operation cancellation
- **Completion Events**: Handle success, cancellation, or errors
- **UI Thread Safety**: Automatic marshalling to UI thread

### 2. Thread & ThreadPool
- **Manual Thread Creation**: When and why to use `new Thread()`
- **ThreadPool Benefits**: Thread reuse and reduced overhead
- **Performance Comparison**: Measure ThreadPool efficiency
- **Thread Saturation**: Understanding ThreadPool limits

### 3. Dispatcher Optimization
- **Priority Levels**: Background, Input, Normal, Send
- **Batched Updates**: Reduce context switches for better performance
- **DispatcherTimer**: UI updates on timer
- **InvokeAsync vs Invoke**: Async dispatcher patterns

### 4. Advanced Threading
- **Producer-Consumer Pattern**: Thread-safe queue operations
- **Race Conditions**: Demonstration of unsafe vs safe code
- **Synchronization**: Lock, Interlocked, and concurrent collections
- **Thread Coordination**: BlockingCollection and semaphores

## Code Structure

```
BackgroundWorkers/
├── BackgroundWorkers.csproj      # .NET 9.0 WPF project
├── App.xaml                      # Application resources
├── App.xaml.cs                   # Global exception handling
├── MainWindow.xaml               # Interactive UI with 4 tabs
└── MainWindow.xaml.cs            # Threading implementations
```

## Features

### Tab 1: BackgroundWorker
- **Basic BackgroundWorker**: Progress reporting and cancellation
- **Data Processing**: Process large datasets with progress updates
- **Event Handling**: DoWork, ProgressChanged, RunWorkerCompleted
- **Thread Information**: View thread IDs and properties

### Tab 2: Thread & ThreadPool
- **Thread Creation**: Compare `new Thread()` vs `ThreadPool.QueueUserWorkItem()`
- **Performance Test**: Measure 10 tasks with both approaches
- **ThreadPool Statistics**: View available and max threads
- **Saturation Test**: Observe ThreadPool behavior under load

### Tab 3: Dispatcher
- **Priority Levels**: Test Background, Input, Normal, Send priorities
- **Update Batching**: Compare individual vs batched UI updates
- **DispatcherTimer**: Configurable interval timer with UI updates
- **Performance Metrics**: Measure dispatcher operation times

### Tab 4: Advanced Threading
- **Producer-Consumer**: Configurable producers/consumers with BlockingCollection
- **Race Conditions**: Three patterns - unsafe, lock, Interlocked
- **Thread Synchronization**: Visual demonstration of data races
- **Concurrent Collections**: Performance test of thread-safe collections

## Building and Running

### Prerequisites
- .NET 9.0 SDK or later
- Windows OS
- Visual Studio 2022 or VS Code

### Build
```bash
cd Module04-UIThreadOptimization/Lesson02-BackgroundWorkers
dotnet build
```

### Run
```bash
dotnet run
```

## Interactive Demonstrations

### 1. BackgroundWorker Basics
1. Click "Start Work" - observe progress updates
2. Click "Cancel" mid-operation - see graceful cancellation
3. Watch log for thread information
4. Adjust item count slider for data processing
5. Click "Process Data" - see large dataset handling

### 2. Threading Comparison
1. Click "Use new Thread()" - see thread creation
2. Click "Use ThreadPool" - see thread reuse
3. Click "Compare (10 tasks)" - observe performance difference
4. Click "Get ThreadPool Info" - view system thread limits
5. Click "Simulate ThreadPool Saturation" - watch thread allocation

### 3. Dispatcher Optimization
1. Select different priority levels
2. Click "Queue 100 Updates" - compare timing
3. Click "Individual Updates" - note slower performance
4. Click "Batched Updates" - see improvement
5. Start/Stop timer - observe smooth UI updates

### 4. Advanced Patterns
1. Adjust producer/consumer counts
2. Start producer-consumer - watch thread coordination
3. Test race conditions - see unsafe vs safe results
4. Run concurrent collection test - compare performance

## Best Practices Demonstrated

### ✅ DO
- Use BackgroundWorker for simple background tasks with progress
- Prefer ThreadPool over manual thread creation
- Batch UI updates when possible to reduce context switches
- Use appropriate Dispatcher priority levels
- Implement cancellation support for long operations
- Use thread-safe collections for concurrent access
- Use Interlocked for simple atomic operations

### ❌ DON'T
- Create many threads manually (use ThreadPool instead)
- Update UI directly from background threads
- Use Thread.Sleep in tight loops
- Ignore race conditions in multi-threaded code
- Forget to handle BackgroundWorker cancellation
- Use normal collections from multiple threads

## Performance Tips

### 1. BackgroundWorker
```csharp
// Report progress sparingly for better performance
if (i % 1000 == 0)  // Not every iteration
{
    worker.ReportProgress(progress);
}
```

### 2. ThreadPool vs Thread
```csharp
// GOOD: ThreadPool reuses threads
ThreadPool.QueueUserWorkItem(_ => DoWork());

// BAD: Creating threads is expensive
new Thread(() => DoWork()).Start();
```

### 3. Dispatcher Batching
```csharp
// BAD: Multiple context switches
foreach (var item in items)
    Dispatcher.Invoke(() => list.Add(item));

// GOOD: Single context switch
Dispatcher.Invoke(() => {
    foreach (var item in items)
        list.Add(item);
});
```

### 4. Thread Synchronization
```csharp
// GOOD: Lock-free atomic operation
Interlocked.Increment(ref counter);

// SLOWER: Lock overhead
lock (lockObject) { counter++; }

// BAD: Race condition
counter++;  // Unsafe!
```

## Common Pitfalls

### Cross-Thread UI Access
```csharp
// BAD: Will throw InvalidOperationException
Task.Run(() => textBlock.Text = "Hello");

// GOOD: Use Dispatcher
Task.Run(() => Dispatcher.Invoke(() => textBlock.Text = "Hello"));
```

### ThreadPool Saturation
```csharp
// BAD: Can saturate ThreadPool
for (int i = 0; i < 10000; i++)
    ThreadPool.QueueUserWorkItem(_ => LongRunningWork());

// GOOD: Limit concurrency
using var semaphore = new SemaphoreSlim(10);
```

### Resource Cleanup
```csharp
// BAD: Resources not cleaned up
var thread = new Thread(DoWork);
thread.Start();

// GOOD: Wait for completion and cleanup
thread.Start();
thread.Join();
```

## Threading Patterns

### Producer-Consumer
```csharp
var queue = new BlockingCollection<int>(boundedCapacity: 10);

// Producer
Task.Run(() => {
    for (int i = 0; i < 100; i++)
        queue.Add(i);
    queue.CompleteAdding();
});

// Consumer
Task.Run(() => {
    foreach (var item in queue.GetConsumingEnumerable())
        ProcessItem(item);
});
```

### Thread-Safe Counter
```csharp
// Fast and thread-safe
private int _counter;
Interlocked.Increment(ref _counter);

// Or use concurrent collection
private ConcurrentBag<int> _items = new();
_items.Add(value);
```

## Performance Metrics

### ThreadPool Efficiency
- **Thread Reuse**: ~10-100x faster than creating new threads
- **Context Switches**: Significantly reduced with batching
- **Memory**: Lower memory footprint with thread reuse

### Dispatcher Priority Impact
- **Background**: Lowest priority, processed when idle
- **Normal**: Standard priority for most operations
- **Send**: Highest priority, immediate processing

## Key Takeaways

1. **BackgroundWorker** is perfect for simple background tasks with UI updates
2. **ThreadPool** is almost always better than manual thread creation
3. **Batch Dispatcher updates** for significant performance improvements
4. **Thread synchronization** is critical - use locks or Interlocked
5. **Concurrent collections** simplify thread-safe operations
6. **Race conditions** cause subtle bugs - always test multi-threaded code

## Troubleshooting

### Issue: InvalidOperationException
**Problem**: Trying to access UI from background thread
**Solution**: Use `Dispatcher.Invoke()` or `Dispatcher.InvokeAsync()`

### Issue: ThreadPool Saturation
**Problem**: All ThreadPool threads are busy
**Solution**: Limit concurrency with SemaphoreSlim or use dedicated threads

### Issue: Race Conditions
**Problem**: Incorrect results with multi-threaded access
**Solution**: Use locks, Interlocked, or concurrent collections

## Additional Resources

- [BackgroundWorker Class](https://docs.microsoft.com/en-us/dotnet/api/system.componentmodel.backgroundworker)
- [ThreadPool Class](https://docs.microsoft.com/en-us/dotnet/api/system.threading.threadpool)
- [Dispatcher Class](https://docs.microsoft.com/en-us/dotnet/api/system.windows.threading.dispatcher)
- [Threading in C#](https://www.albahari.com/threading/)

## Next Steps
- Proceed to Lesson 03: Task Parallel Library for modern parallel patterns
- Explore Lesson 04: UI Virtualization for large data sets
- Review Lesson 01: Async Patterns for async/await patterns
