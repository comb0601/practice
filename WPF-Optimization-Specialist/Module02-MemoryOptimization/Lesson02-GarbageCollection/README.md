# Lesson 02: Garbage Collection Optimization

## Overview
This lesson provides comprehensive, hands-on demonstrations of .NET's garbage collector behavior, configuration options, and optimization techniques. Learn how to work with the GC effectively to build high-performance WPF applications.

## Learning Objectives

By completing this lesson, you will:

1. **Master GC Fundamentals**
   - Understand the three-generation garbage collection model
   - Learn how objects promote between generations
   - Recognize automatic vs. manual collection triggers
   - Monitor GC performance metrics in real-time

2. **Configure GC Modes**
   - Understand Workstation vs. Server GC
   - Learn about GC latency modes (Interactive, LowLatency, Batch)
   - Configure GC for different application scenarios
   - Understand trade-offs between throughput and latency

3. **Optimize Allocation Patterns**
   - Implement object pooling to reduce GC pressure
   - Use ArrayPool<T> for temporary buffers
   - Identify and fix excessive allocation patterns
   - Measure allocation rates and impact

4. **Work with Advanced Features**
   - Implement proper finalization patterns
   - Use memory pressure API for unmanaged resources
   - Register for GC notifications
   - Analyze GC pause durations and fragmentation

## Application Features

### 1. Generations Tab
**Understanding the Generation Model**

The .NET GC organizes objects into three generations:
- **Gen0:** Recently allocated objects (collected frequently)
- **Gen1:** Medium-lived objects (buffer generation)
- **Gen2:** Long-lived objects (collected infrequently)

**Demonstrations:**
- Allocate objects in Gen0
- Trigger collections to promote objects through generations
- Visualize generation sizes and collection counts
- Observe performance differences between generations

**Key Concepts:**
```csharp
// Check object generation
int generation = GC.GetGeneration(myObject);

// Object survives Gen0 collection -> promoted to Gen1
// Object survives Gen1 collection -> promoted to Gen2
```

### 2. GC Modes Tab
**Workstation vs. Server GC**

**Workstation GC:**
- Optimized for UI responsiveness
- Lower throughput, lower latency
- Single or concurrent collection threads

**Server GC:**
- Optimized for throughput
- Higher memory usage
- Dedicated thread per logical processor

**Latency Modes:**
- **Interactive:** Default for UI apps, balances latency and throughput
- **LowLatency:** Minimizes pauses (use only for critical sections)
- **Batch:** Disables concurrent GC for maximum throughput
- **SustainedLowLatency:** For server apps requiring consistent low latency

**Configuration Example:**
```csharp
// Temporarily reduce GC interference
var oldMode = GCSettings.LatencyMode;
try
{
    GCSettings.LatencyMode = GCLatencyMode.LowLatency;
    // Critical performance section
}
finally
{
    GCSettings.LatencyMode = oldMode;
}
```

### 3. Collection Triggers Tab
**When Collections Occur**

**Automatic Triggers:**
- Gen0 budget exhausted
- Explicit allocation fails
- System low memory notification
- End of GC.RegisterForFullGCNotification threshold

**Manual Collection:**
```csharp
// Collect specific generation
GC.Collect(0); // Gen0 only
GC.Collect(1); // Gen0 and Gen1
GC.Collect(2); // Full collection (all generations)

// Aggressive compacting collection
GC.Collect(2, GCCollectionMode.Aggressive,
    blocking: true, compacting: true);
```

**Best Practice:** Rarely call GC.Collect() in production. Let the GC make decisions based on heap pressure.

### 4. Allocation Patterns Tab
**Good vs. Bad Patterns**

**Bad Pattern - Excessive Allocations:**
```csharp
// Creates millions of short-lived objects
for (int i = 0; i < 1000000; i++)
{
    var temp = new MyObject();
    temp.Process();
    // temp becomes garbage immediately
}
```

**Good Pattern - Object Pooling:**
```csharp
// Reuse objects from pool
private Stack<MyObject> _pool = new();

MyObject GetObject()
{
    return _pool.Count > 0
        ? _pool.Pop()
        : new MyObject();
}

void ReturnObject(MyObject obj)
{
    obj.Reset();
    _pool.Push(obj);
}
```

**Optimal Pattern - ArrayPool:**
```csharp
// Zero-allocation temporary arrays
var buffer = ArrayPool<byte>.Shared.Rent(1024);
try
{
    // Use buffer
}
finally
{
    ArrayPool<byte>.Shared.Return(buffer);
}
```

### 5. Finalization Tab
**Finalizers vs. IDisposable**

**Finalizer Impact:**
- Objects with finalizers require TWO GC cycles to collect
- Finalizers run on dedicated thread (unpredictable timing)
- Can resurrect objects by creating new references

**Proper Pattern:**
```csharp
public class ResourceWrapper : IDisposable
{
    private IntPtr _handle;
    private bool _disposed = false;

    ~ResourceWrapper()
    {
        // Finalizer as safety net only
        Dispose(false);
    }

    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this); // Prevent finalization
    }

    protected virtual void Dispose(bool disposing)
    {
        if (!_disposed)
        {
            if (disposing)
            {
                // Dispose managed resources
            }

            // Free unmanaged resources
            if (_handle != IntPtr.Zero)
            {
                NativeMethods.CloseHandle(_handle);
                _handle = IntPtr.Zero;
            }

            _disposed = true;
        }
    }
}
```

### 6. Memory Pressure Tab
**Managing Unmanaged Memory**

When wrapping large unmanaged resources, inform the GC:

```csharp
public class UnmanagedBufferWrapper : IDisposable
{
    private IntPtr _buffer;
    private long _size;

    public UnmanagedBufferWrapper(long size)
    {
        _size = size;
        _buffer = Marshal.AllocHGlobal((int)size);

        // Inform GC about unmanaged memory
        GC.AddMemoryPressure(size);
    }

    public void Dispose()
    {
        if (_buffer != IntPtr.Zero)
        {
            Marshal.FreeHGlobal(_buffer);
            _buffer = IntPtr.Zero;

            // Remove pressure when freed
            GC.RemoveMemoryPressure(_size);
        }
    }
}
```

### 7. GC Notifications Tab
**Advanced Server Scenarios**

For server applications that want to prepare for full collections:

```csharp
// Register for notifications
GC.RegisterForFullGCNotification(10, 10);

// Monitor on background thread
while (true)
{
    var status = GC.WaitForFullGCApproach();
    if (status == GCNotificationStatus.Succeeded)
    {
        // Redirect traffic, prepare for GC

        status = GC.WaitForFullGCComplete();
        if (status == GCNotificationStatus.Succeeded)
        {
            // Resume normal operations
        }
    }
}
```

## Live Statistics Panel

The application provides real-time GC metrics:

**Memory Usage:**
- Total managed heap size
- Heap size (committed memory)
- Fragmented bytes (unusable memory)

**Collection Counts:**
- Gen0, Gen1, Gen2 collection counters
- Track collection frequency

**Performance Metrics:**
- GC time percentage
- Last pause duration
- Allocation rate (MB/s)

## Exercises

### Exercise 1: Generation Promotion
1. Click "Allocate Gen0 Objects" 5 times
2. Observe Gen0 object count and size
3. Click "Promote to Gen1"
4. Observe objects moving to Gen1
5. Click "Promote to Gen2"
6. Note how Gen2 collections are rare
7. Click "Clear All" and force full collection

**Expected Result:** Understand how objects move through generations.

### Exercise 2: GC Mode Experimentation
1. Note current latency mode (Interactive)
2. Allocate large amounts of memory
3. Observe collection behavior
4. Set to LowLatency mode
5. Allocate more memory
6. Observe reduced collection frequency (but higher memory usage)
7. Return to Interactive mode

**Expected Result:** See trade-offs between latency modes.

### Exercise 3: Manual vs. Automatic Collection
1. Click "Allocate 100 MB"
2. Watch auto-collection counts increase
3. Note memory usage
4. Click "Collect Gen2"
5. Compare before/after memory
6. Observe collections performed

**Expected Result:** Understand when GC runs automatically vs. manually.

### Exercise 4: Allocation Pattern Comparison
1. Click "Use Object Pool" 10 times
2. Note minimal GC impact
3. Click "Excessive Allocations" 5 times
4. Observe increased Gen0 collections
5. Click "Use ArrayPool" 10 times
6. Compare allocation rates in statistics

**Expected Result:** See dramatic difference in GC pressure.

### Exercise 5: Finalization Cost
1. Click "Create w/ Finalizer" 5 times
2. Force collection (Collect Gen2)
3. Note objects still in memory
4. Click "Force Finalization"
5. Observe 2-cycle cleanup
6. Click "Create w/ Dispose" 5 times
7. Force collection once
8. Compare cleanup efficiency

**Expected Result:** Finalizers require 2 GC cycles vs. 1 for IDisposable.

### Exercise 6: Memory Pressure Management
1. Click "Add Pressure (10 MB)" 10 times
2. Observe collection count increase
3. Note GC responding to pressure
4. Click "Remove Pressure"
5. Observe normal collection behavior resume

**Expected Result:** GC responds to memory pressure notifications.

### Exercise 7: GC Notification Monitoring
1. Click "Enable Notifications"
2. Click "Allocate 100 MB" multiple times
3. Watch notification log for approaching/completed events
4. Click "Disable Notifications"

**Expected Result:** See notification system in action.

### Exercise 8: Real-World Scenario
Create a simulated image processing pipeline:
1. Use ArrayPool for temporary buffers
2. Implement object pooling for result objects
3. Monitor GC statistics during processing
4. Compare with naive implementation
5. Measure GC time percentage difference

**Goal:** Apply multiple optimization techniques together.

## Best Practices

### 1. Let GC Work for You
```csharp
// DON'T do this in production
GC.Collect(); // Usually unnecessary

// DO monitor and optimize allocations instead
// Use profilers to identify allocation hot spots
```

### 2. Use Object Pooling Wisely
```csharp
// Good for frequently created objects
private ArrayPool<byte> _bufferPool = ArrayPool<byte>.Shared;

byte[] GetBuffer(int size) => _bufferPool.Rent(size);
void ReturnBuffer(byte[] buffer) => _bufferPool.Return(buffer);
```

### 3. Avoid Finalizers When Possible
```csharp
// Prefer IDisposable
public class Resource : IDisposable
{
    public void Dispose()
    {
        // Cleanup
        GC.SuppressFinalize(this); // Skip finalization
    }
}
```

### 4. Measure, Don't Guess
```csharp
// Profile before optimizing
var stopwatch = Stopwatch.StartNew();
// Code to measure
stopwatch.Stop();

var gen0Before = GC.CollectionCount(0);
// Code to measure
var gen0After = GC.CollectionCount(0);
Console.WriteLine($"GC Gen0: {gen0After - gen0Before}");
```

### 5. Use Appropriate Latency Mode
```csharp
// Only for short critical sections
var oldMode = GCSettings.LatencyMode;
try
{
    GCSettings.LatencyMode = GCLatencyMode.LowLatency;
    // Time-critical code (keep very short!)
}
finally
{
    GCSettings.LatencyMode = oldMode; // Always restore
}
```

## Performance Tips

1. **Reduce Allocations:** The best GC optimization is fewer allocations
2. **Pool Objects:** Reuse objects instead of creating new ones
3. **Use Structs:** For small, short-lived data (value types)
4. **ArrayPool:** For temporary buffers
5. **Avoid Finalizers:** Use IDisposable instead
6. **Monitor Metrics:** Watch Gen2 collections and heap size
7. **Batch Operations:** Group allocations to reduce GC frequency
8. **Large Object Heap:** Be aware of 85KB+ object allocation

## Common Issues and Solutions

### Issue: Frequent Gen2 Collections
**Cause:** Long-lived objects being created frequently
**Solution:** Use object pooling or restructure object lifetimes

### Issue: High Memory Usage
**Cause:** Objects not being released, or LOH fragmentation
**Solution:** Profile to find retention, use GC.Collect(2, Aggressive) if needed

### Issue: Long GC Pauses
**Cause:** Large Gen2 heap or LOH compaction
**Solution:** Reduce Gen2 promotions, use segments, consider server GC

### Issue: Memory Leaks Despite GC
**Cause:** Strong references preventing collection
**Solution:** Use memory profiler to find retention paths

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

- [Garbage Collection Fundamentals](https://docs.microsoft.com/en-us/dotnet/standard/garbage-collection/fundamentals)
- [GC Performance Counters](https://docs.microsoft.com/en-us/dotnet/framework/debug-trace-profile/performance-counters)
- [GCSettings Class](https://docs.microsoft.com/en-us/dotnet/api/system.runtime.gcsettings)
- [ArrayPool<T>](https://docs.microsoft.com/en-us/dotnet/api/system.buffers.arraypool-1)
- [Object Pooling Pattern](https://docs.microsoft.com/en-us/dotnet/standard/collections/pool)

## Summary

Understanding the garbage collector is essential for building high-performance .NET applications. This lesson provides:

- **7 interactive demonstrations** of GC behavior and configuration
- **Real-time statistics** showing GC impact
- **Side-by-side comparisons** of good vs. bad patterns
- **Practical exercises** for mastering GC optimization

Key takeaways:
- The GC usually makes better decisions than manual collection
- Reducing allocations is better than optimizing collection
- Use pooling patterns for frequently allocated objects
- Monitor Gen2 collections and pause durations
- IDisposable is better than finalizers
- Profile before optimizing

Master these concepts to build WPF applications with excellent memory performance characteristics.
