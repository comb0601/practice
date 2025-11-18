# Lesson 01: Async Patterns in WPF

## Overview
This lesson demonstrates async/await patterns in WPF applications, covering best practices for keeping the UI responsive while performing long-running operations.

## Learning Objectives
- Understand async/await fundamentals in WPF
- Master ConfigureAwait usage
- Implement proper error handling in async operations
- Apply advanced async patterns (WhenAll, WhenAny, throttling)

## Key Concepts

### 1. Basic Async/Await
- **Synchronous vs Asynchronous**: Compare blocking operations that freeze the UI vs async operations that keep it responsive
- **Async Data Loading**: Load data from files, web APIs, and databases without blocking
- **UI Responsiveness**: Keep controls interactive during long-running operations

### 2. ConfigureAwait Patterns
- **ConfigureAwait(false)**: Don't capture synchronization context - better performance for library code
- **ConfigureAwait(true)**: Capture context (default) - required when updating UI elements
- **Context Switching**: Analyze thread context behavior with different patterns

### 3. Error Handling
- **Try/Catch Pattern**: Proper exception handling in async methods
- **AggregateException**: Handle multiple exceptions from parallel operations
- **Timeout Handling**: Implement timeouts using CancellationTokenSource
- **Cancellation**: Graceful task cancellation with CancellationToken

### 4. Advanced Patterns
- **Sequential vs Parallel**: Compare execution time and performance
- **Task.WhenAll**: Wait for all tasks to complete
- **Task.WhenAny**: React to first completed task
- **Throttling**: Limit concurrent operations with SemaphoreSlim

## Code Structure

```
AsyncPatterns/
├── AsyncPatterns.csproj          # .NET 9.0 WPF project
├── App.xaml                      # Application resources and styles
├── App.xaml.cs                   # Application startup and exception handling
├── MainWindow.xaml               # Interactive UI with 4 tabs
└── MainWindow.xaml.cs            # Async pattern implementations
```

## Features

### Tab 1: Basic Async/Await
- **Blocking vs Async Comparison**: See the difference in UI responsiveness
- **Cancellable Operations**: Cancel long-running async tasks
- **Data Loading**: File I/O, HTTP requests, database queries
- **UI Interaction Test**: Verify responsiveness during async operations

### Tab 2: ConfigureAwait
- **ConfigureAwait(false) Demo**: Run continuations on thread pool
- **ConfigureAwait(true) Demo**: Run continuations on UI thread
- **Context Analysis**: View thread IDs and context switching behavior
- **Performance Comparison**: Measure impact of different patterns

### Tab 3: Error Handling
- **Try/Catch Pattern**: Basic exception handling
- **AggregateException**: Handle multiple exceptions
- **Timeout Scenarios**: Cancel operations after timeout
- **Cancellation Tokens**: Cooperative cancellation with progress tracking

### Tab 4: Advanced Patterns
- **Parallel Execution**: Compare sequential vs parallel performance
- **Task Coordination**: WhenAll and WhenAny patterns
- **Progress Tracking**: Visual progress for multiple concurrent tasks
- **Throttling**: Limit concurrent operations with configurable max

## Building and Running

### Prerequisites
- .NET 9.0 SDK or later
- Windows OS
- Visual Studio 2022 or VS Code with C# extension

### Build
```bash
cd Module04-UIThreadOptimization/Lesson01-AsyncPatterns
dotnet build
```

### Run
```bash
dotnet run
```

## Interactive Demonstrations

### 1. Blocking vs Async Operations
1. Click "Blocking (Bad)" - notice UI freezes
2. Try to interact with other controls - they won't respond
3. Click "Async (Good)" - notice UI stays responsive
4. Move the slider while async operation runs

### 2. ConfigureAwait Analysis
1. Run all three ConfigureAwait patterns
2. Observe thread IDs in the log
3. Note when Dispatcher is required
4. Run Context Analysis to see detailed comparison

### 3. Error Handling
1. Test Try/Catch pattern - see graceful error handling
2. Test AggregateException - see multiple errors
3. Test Timeout - see operation cancelled after 2 seconds
4. Start cancellable task and cancel mid-operation

### 4. Parallel Operations
1. Run Sequential - takes ~2.5 seconds
2. Run Parallel - takes ~0.5 seconds (5x faster!)
3. Run WhenAll - see all tasks complete together
4. Run WhenAny - see first task completion
5. Adjust throttle slider and see limited concurrency

## Best Practices Demonstrated

### ✅ DO
- Use async/await for I/O operations (file, network, database)
- Use ConfigureAwait(false) in library code
- Always handle OperationCanceledException
- Use CancellationToken for long-running operations
- Dispose CancellationTokenSource
- Use Task.WhenAll for parallel independent operations
- Use SemaphoreSlim for throttling concurrent operations

### ❌ DON'T
- Block the UI thread with Thread.Sleep or .Wait()
- Use async void except for event handlers
- Ignore exceptions in async code
- Forget to configure timeouts for network operations
- Use Task.Run for CPU-bound work in UI applications without careful consideration

## Performance Tips

1. **ConfigureAwait(false) for Library Code**
   - Avoids unnecessary context switches
   - Better performance in non-UI code
   - Can't access UI after ConfigureAwait(false)

2. **Parallel Operations**
   - Use Task.WhenAll for independent operations
   - Can reduce total execution time significantly
   - Be mindful of resource usage

3. **Throttling**
   - Limit concurrent operations to avoid resource exhaustion
   - Use SemaphoreSlim for async throttling
   - Tune max concurrency based on system resources

4. **Cancellation**
   - Always support cancellation for long operations
   - Check cancellation token regularly
   - Clean up resources in finally blocks

## Common Pitfalls

### Deadlocks
```csharp
// BAD: Can deadlock in UI applications
var result = SomeAsyncMethod().Result;

// GOOD: Use await
var result = await SomeAsyncMethod();
```

### Fire and Forget
```csharp
// BAD: Exception will be lost
Task.Run(() => DoSomething());

// GOOD: Await or handle exceptions
await Task.Run(() => DoSomething());
```

### Context Capture
```csharp
// BAD: Unnecessary context capture in library
await Task.Delay(1000); // ConfigureAwait(true) by default

// GOOD: Don't capture context
await Task.Delay(1000).ConfigureAwait(false);
```

## Key Takeaways

1. **Async/await keeps UI responsive** by not blocking the UI thread
2. **ConfigureAwait(false)** improves performance when UI updates aren't needed
3. **Always handle exceptions** in async methods
4. **Use CancellationToken** for user-cancellable operations
5. **Task.WhenAll/WhenAny** enable powerful parallel patterns
6. **Throttling prevents resource exhaustion** in high-concurrency scenarios

## Additional Resources

- [Async/Await Best Practices](https://docs.microsoft.com/en-us/archive/msdn-magazine/2013/march/async-await-best-practices-in-asynchronous-programming)
- [ConfigureAwait FAQ](https://devblogs.microsoft.com/dotnet/configureawait-faq/)
- [Task-based Asynchronous Pattern](https://docs.microsoft.com/en-us/dotnet/standard/asynchronous-programming-patterns/task-based-asynchronous-pattern-tap)

## Next Steps
- Proceed to Lesson 02: Background Workers for thread management
- Explore Lesson 03: Task Parallel Library for CPU-bound operations
- Study Lesson 04: UI Virtualization for large data sets
