# Lesson 01: Memory Leaks Detection and Resolution

## Overview
This comprehensive lesson demonstrates 10 common memory leak scenarios in WPF applications, showing both problematic implementations and proper solutions. Understanding and preventing memory leaks is crucial for building performant, reliable WPF applications.

## Learning Objectives

By completing this lesson, you will:

1. **Understand Memory Leak Fundamentals**
   - Learn what causes memory leaks in .NET applications
   - Understand how the garbage collector works
   - Identify the impact of memory leaks on application performance

2. **Recognize Common Leak Patterns**
   - Event handler leaks from not unsubscribing
   - Static reference leaks
   - Timer and DispatcherTimer leaks
   - Dependency property callback leaks
   - Collection change notification leaks
   - Routed event leaks
   - Command binding leaks
   - Resource dictionary leaks
   - Visual tree reference leaks
   - DataContext and ViewModel leaks

3. **Apply Detection Techniques**
   - Monitor memory usage in real-time
   - Use GC statistics to identify issues
   - Track object lifetimes
   - Identify leak sources through systematic testing

4. **Implement Proper Solutions**
   - Unsubscribe from events properly
   - Use weak references where appropriate
   - Implement IDisposable pattern correctly
   - Clean up timers and resources
   - Manage ViewModel lifecycles

## Application Features

### 10 Interactive Leak Scenarios

#### 1. Event Handler Leaks
**Problem:** Objects subscribe to static or long-lived events but never unsubscribe, causing the publisher to hold references.

**Demonstration:**
- Create windows that subscribe to static events
- Compare leaky vs. fixed implementations
- Observe memory behavior after closing windows

**Solution:** Always unsubscribe from events in the Closed or Dispose handler.

#### 2. Static Reference Leaks
**Problem:** Static fields hold references for the application lifetime, preventing garbage collection.

**Demonstration:**
- Add large objects to static collections
- Compare strong references vs. WeakReference
- Clear static collections to reclaim memory

**Solution:** Use WeakReference for optional caching or clear collections when done.

#### 3. Timer Leaks
**Problem:** DispatcherTimer continues running after window closes, holding references through event handlers.

**Demonstration:**
- Create windows with timers that update UI
- Compare leaky vs. properly stopped timers
- Monitor instance counts after closing

**Solution:** Stop timers and unsubscribe from Tick events in cleanup code.

#### 4. Dependency Property Callbacks
**Problem:** DP callbacks that create closures can capture 'this' and prevent garbage collection.

**Demonstration:**
- Create controls with DP callbacks
- Observe different callback implementations
- Clear panels and check memory

**Solution:** Use static callbacks or avoid closures that capture instance references.

#### 5. Collection Change Notifications
**Problem:** Subscribing to ObservableCollection.CollectionChanged without unsubscribing.

**Demonstration:**
- Create subscribers to shared collections
- Compare leaky vs. disposable subscribers
- Modify collections to trigger notifications

**Solution:** Implement IDisposable and unsubscribe in Dispose method.

#### 6. Routed Event Leaks
**Problem:** Routed event handlers attached but never removed from elements.

**Demonstration:**
- Attach Preview and Bubble event handlers
- Compare leaked vs. properly removed handlers
- Track handler counts

**Solution:** Use RemoveHandler or implement weak event patterns.

#### 7. Command Binding Leaks
**Problem:** ICommand implementations subscribe to CommandManager.RequerySuggested or hold strong references.

**Demonstration:**
- Create windows with command bindings
- Compare leaky vs. weak event implementations
- Dispose commands properly

**Solution:** Use weak event pattern for CanExecuteChanged and implement IDisposable.

#### 8. Resource Dictionary Leaks
**Problem:** Merged resource dictionaries with heavy resources remain in memory.

**Demonstration:**
- Load resource dictionaries with gradients
- Compare regular vs. frozen resources
- Clear merged dictionaries

**Solution:** Clear merged dictionaries and freeze Freezable resources.

#### 9. Visual Tree Reference Leaks
**Problem:** Holding external references to visual elements prevents tree from being collected.

**Demonstration:**
- Create visual elements with external references
- Compare leaked vs. proper implementations
- Clear reference lists

**Solution:** Avoid storing references to visual elements or use WeakReference.

#### 10. DataContext Leaks
**Problem:** ViewModels not properly disposed or holding references to views.

**Demonstration:**
- Create windows with ViewModels
- Compare leaky vs. disposable ViewModels
- Clean up DataContext on close

**Solution:** Implement IDisposable in ViewModels and dispose in window Closed event.

## Memory Monitoring

The application includes real-time memory monitoring:

- **Total Memory Usage:** Current managed heap size
- **GC Collections:** Generation 0, 1, and 2 collection counts
- **Force GC:** Manually trigger garbage collection
- **Update Stats:** Refresh all statistics

## Key Concepts

### Garbage Collection Basics
- .NET uses automatic memory management
- Objects are collected when no longer referenced
- Three generations: 0 (short-lived), 1 (medium), 2 (long-lived)
- Memory leaks prevent objects from being collected

### Reference Relationships
- **Strong Reference:** Prevents garbage collection
- **Weak Reference:** Allows garbage collection
- **Event Subscriptions:** Create strong references
- **Static Fields:** Live for application lifetime

### IDisposable Pattern
```csharp
public class MyClass : IDisposable
{
    private bool _disposed = false;

    public void Dispose()
    {
        if (!_disposed)
        {
            // Unsubscribe from events
            // Stop timers
            // Release resources
            _disposed = true;
        }
    }
}
```

### Event Cleanup Pattern
```csharp
public class MyWindow : Window
{
    public MyWindow()
    {
        SomeObject.SomeEvent += OnSomeEvent;

        Closed += (s, e) =>
        {
            SomeObject.SomeEvent -= OnSomeEvent;
        };
    }
}
```

## Exercises

### Exercise 1: Event Handler Leak Detection
1. Create 10 leaky event windows
2. Close all windows
3. Force GC and observe memory
4. Create 10 fixed event windows
5. Close all windows
6. Force GC and compare memory usage

**Expected Result:** Fixed windows should be collected, leaky windows remain in memory.

### Exercise 2: Static Collection Management
1. Add 20 objects to the static list
2. Observe memory usage increase
3. Clear the static list
4. Force GC and observe memory decrease
5. Add 20 weak references
6. Force GC immediately
7. Update stats and see weak references die

**Expected Result:** Weak references allow GC, strong references prevent it.

### Exercise 3: Timer Lifecycle
1. Create 5 leaky timer windows
2. Close all windows
3. Wait 10 seconds
4. Force GC - timers still running, windows in memory
5. Create 5 fixed timer windows
6. Close all windows
7. Force GC - timers stopped, windows collected

**Expected Result:** Fixed timer windows release properly.

### Exercise 4: ViewModel Disposal
1. Create leaky ViewModel window
2. Close window
3. Check instance count (still counted)
4. Create fixed ViewModel window
5. Close window
6. Check instance count (decremented)

**Expected Result:** Fixed ViewModel is properly disposed.

### Exercise 5: Complete Leak Audit
1. Go through each of the 10 scenarios
2. Create multiple instances of leaky versions
3. Note memory usage before cleanup
4. Force GC and note memory (minimal decrease)
5. Create same number of fixed versions
6. Force GC and note memory (proper cleanup)
7. Document the memory difference

**Expected Result:** Proper cleanup should result in significantly lower memory usage.

### Exercise 6: Memory Profiling
1. Monitor Gen0, Gen1, Gen2 collection counts
2. Create various leak scenarios
3. Observe which generation objects end up in
4. Note that leaked objects promote to Gen2
5. Fixed implementations stay in Gen0/Gen1

**Expected Result:** Understand GC generation behavior.

### Exercise 7: Resource Dictionary Management
1. Load 50 heavy resource dictionaries
2. Observe memory increase
3. Unload all resources
4. Force GC and observe memory
5. Load 50 frozen resource dictionaries
6. Compare memory usage

**Expected Result:** Frozen resources are more memory efficient.

### Exercise 8: Real-World Scenario
Create a mini-application that:
- Uses a ViewModel with INotifyPropertyChanged
- Has a timer that updates a property
- Subscribes to application-wide events
- Uses command bindings
- Implements proper cleanup

**Goal:** Practice comprehensive resource management.

## Common Patterns and Best Practices

### Always Unsubscribe
```csharp
// Bad
public MyClass()
{
    SomeEvent += Handler;
}

// Good
public MyClass()
{
    SomeEvent += Handler;
    Closed += (s, e) => SomeEvent -= Handler;
}
```

### Use Weak Event Pattern
```csharp
WeakEventManager<TSource, TEventArgs>
    .AddHandler(source, "EventName", OnEvent);
```

### Dispose Pattern
```csharp
public class ViewModel : IDisposable
{
    private Timer? _timer;

    public void Dispose()
    {
        _timer?.Stop();
        _timer?.Dispose();
        _timer = null;
    }
}
```

### Avoid Static Collections
```csharp
// Bad
private static List<MyObject> _cache = new();

// Better
private static List<WeakReference<MyObject>> _cache = new();
```

## Debugging Tips

1. **Use Memory Profilers:**
   - Visual Studio Diagnostic Tools
   - dotMemory
   - PerfView

2. **Force GC During Testing:**
   - Call GC.Collect() to verify objects are collected
   - Check GC.GetTotalMemory() before and after

3. **Track Object Counts:**
   - Use static counters in constructors/finalizers
   - Monitor instance counts in real-time

4. **Check Event Handlers:**
   - Inspect event handler counts
   - Verify handlers are removed

5. **Visual Studio Diagnostics:**
   - Use Memory Usage tool
   - Take memory snapshots
   - Compare snapshots to find leaks

## Building and Running

```bash
# Build the project
dotnet build

# Run the application
dotnet run
```

## Requirements
- .NET 9.0 SDK
- Windows OS
- Visual Studio 2022 (recommended) or VS Code

## Additional Resources

- [.NET Memory Management](https://docs.microsoft.com/en-us/dotnet/standard/garbage-collection/)
- [Weak Event Patterns in WPF](https://docs.microsoft.com/en-us/dotnet/desktop/wpf/advanced/weak-event-patterns)
- [IDisposable Pattern](https://docs.microsoft.com/en-us/dotnet/standard/garbage-collection/implementing-dispose)
- [WPF Memory Management Best Practices](https://docs.microsoft.com/en-us/dotnet/desktop/wpf/advanced/optimizing-performance-other-recommendations)

## Summary

Memory leaks are a critical issue in WPF applications that can lead to poor performance, increased memory usage, and application crashes. This lesson provides hands-on experience with:

- 10 common memory leak scenarios
- Side-by-side comparison of leaky vs. fixed code
- Real-time memory monitoring
- Practical exercises for mastery

Understanding these patterns and solutions is essential for building robust, production-quality WPF applications.
