# Lesson 03: Large Object Heap Management

## Overview
Master the Large Object Heap (LOH) - a specialized memory region for large objects in .NET. Learn about LOH allocation, fragmentation issues, compaction strategies, and pooling techniques to build memory-efficient WPF applications.

## Learning Objectives

By completing this lesson, you will:

1. **Understand LOH Fundamentals**
   - Learn the 85 KB threshold for LOH allocation
   - Understand why LOH exists and its characteristics
   - Recognize when objects go to LOH vs SOH
   - Monitor LOH size and fragmentation

2. **Master Fragmentation Management**
   - Understand how LOH fragmentation occurs
   - Visualize fragmentation patterns
   - Recognize fragmentation symptoms
   - Apply compaction when appropriate

3. **Implement Pooling Strategies**
   - Use ArrayPool<T> for large buffers
   - Create custom object pools for large objects
   - Measure performance improvements
   - Reduce LOH pressure through reuse

4. **Apply Optimization Techniques**
   - Stay below LOH threshold when possible
   - Segment large data structures
   - Use memory-mapped files for very large data
   - Choose appropriate data layouts (SoA vs AoS)

## The 85 KB Threshold

### Why 85,000 Bytes?

Objects >= 85,000 bytes are allocated on the Large Object Heap because:
- **Performance:** Small objects are compacted frequently; large objects are expensive to move
- **Heap pressure:** Separating large objects prevents SOH fragmentation
- **Collection strategy:** LOH is swept (not compacted by default) during Gen2 collections

### What Goes to LOH?

```csharp
// These go to LOH:
byte[] large1 = new byte[85000];        // Exactly at threshold
byte[] large2 = new byte[100 * 1024];   // 100 KB
int[] large3 = new int[21250];          // 21,250 × 4 = 85,000 bytes
double[] large4 = new double[10625];    // 10,625 × 8 = 85,000 bytes

// These stay on SOH:
byte[] small1 = new byte[84999];        // Just under threshold
byte[] small2 = new byte[80 * 1024];    // 80 KB
int[] small3 = new int[21249];          // 21,249 × 4 = 84,996 bytes
```

### Checking Object Location

```csharp
var array = new byte[100 * 1024];
int generation = GC.GetGeneration(array);
// Generation 2 = on LOH
```

## LOH Characteristics

### 1. Direct Gen2 Allocation
LOH objects skip Gen0 and Gen1, going directly to Gen2:
- Collected only during Gen2 GC (infrequent)
- Live longer than small objects
- Not compacted by default

### 2. No Compaction (Default)
Unlike SOH, LOH is swept but not compacted:
- **Sweep:** Dead objects are marked as free space
- **No compaction:** Objects don't move to consolidate free space
- **Result:** Fragmentation can occur

### 3. Memory Fragmentation
Gaps between live objects prevent allocation of large contiguous blocks:
```
Before: [Obj1][Obj2][Obj3][Obj4][Obj5]
After:  [Obj1][Free][Obj3][Free][Obj5]
```
Even with plenty of free space, allocation might fail if no single free block is large enough.

## Fragmentation Problems

### Symptoms
- OutOfMemoryException despite available memory
- Increasing working set size
- Poor memory locality
- Degraded performance

### Demonstration
```csharp
// Create fragmentation
List<byte[]> blocks = new();
for (int i = 0; i < 100; i++)
{
    blocks.Add(new byte[200 * 1024]); // 200 KB each
}

// Release every other block
for (int i = 1; i < blocks.Count; i += 2)
{
    blocks[i] = null;
}

GC.Collect(); // Gaps now exist

// Try to allocate large contiguous block
// May fail even though total free space is sufficient
var large = new byte[1024 * 1024]; // Might throw OOM!
```

## LOH Compaction

### Enabling Compaction (.NET 4.5.1+)

```csharp
// One-time compaction
GCSettings.LargeObjectHeapCompactionMode = GCLargeObjectHeapCompactionMode.CompactOnce;
GC.Collect(2, GCCollectionMode.Aggressive, blocking: true, compacting: true);

// Compaction occurs on next blocking Gen2 GC
// Automatically reverts to Default after compaction
```

### When to Compact
**Use compaction when:**
- Fragmentation is causing allocation failures
- After releasing many large objects
- During application idle time
- Before loading large datasets

**Avoid compaction:**
- In tight loops (expensive operation)
- During time-critical sections
- If LOH is small

### Trade-offs
- **Benefit:** Eliminates fragmentation, consolidates free space
- **Cost:** Expensive operation (moving large objects)
- **Pause:** Can cause noticeable GC pause

## Buffer Pooling Strategies

### ArrayPool<T> (Built-in)

```csharp
// Best practice for temporary large buffers
var buffer = ArrayPool<byte>.Shared.Rent(100 * 1024);
try
{
    // Use buffer
    ProcessData(buffer);
}
finally
{
    ArrayPool<byte>.Shared.Return(buffer);
}
```

**Benefits:**
- Zero allocations after warm-up
- Thread-safe
- Automatic size management
- No LOH fragmentation

### Custom Object Pool

```csharp
public class LargeObjectPool<T> where T : class, new()
{
    private readonly ConcurrentBag<T> _pool = new();
    private readonly Func<T> _factory;
    private readonly Action<T> _reset;

    public LargeObjectPool(Func<T> factory, Action<T> reset)
    {
        _factory = factory;
        _reset = reset;
    }

    public T Rent()
    {
        return _pool.TryTake(out var item) ? item : _factory();
    }

    public void Return(T item)
    {
        _reset(item);
        _pool.Add(item);
    }
}

// Usage
var pool = new LargeObjectPool<byte[]>(
    () => new byte[500 * 1024],
    arr => Array.Clear(arr)
);

var buffer = pool.Rent();
// Use buffer
pool.Return(buffer);
```

## Optimization Strategies

### Strategy 1: Stay Below Threshold

```csharp
// Instead of 100 KB buffer (LOH)
byte[] large = new byte[100 * 1024];

// Use 80 KB buffer (SOH)
byte[] small = new byte[80 * 1024];
// Benefit: Compacted during GC, no fragmentation
```

### Strategy 2: Segment Large Data

```csharp
// Bad: Single large array on LOH
byte[] singleArray = new byte[10 * 1024 * 1024]; // 10 MB

// Good: Segmented arrays on SOH
const int segmentSize = 80 * 1024;
const int numSegments = (10 * 1024 * 1024) / segmentSize;
byte[][] segments = new byte[numSegments][];
for (int i = 0; i < numSegments; i++)
{
    segments[i] = new byte[segmentSize];
}

// Access with indexing:
int GetSegment(int index) => index / segmentSize;
int GetOffset(int index) => index % segmentSize;

byte GetByte(int index)
{
    return segments[GetSegment(index)][GetOffset(index)];
}
```

### Strategy 3: Memory-Mapped Files

```csharp
// For very large data (GB+), use memory-mapped files
using var mmf = MemoryMappedFile.CreateNew("LargeData", 1L * 1024 * 1024 * 1024); // 1 GB
using var accessor = mmf.CreateViewAccessor();

// Read/write without managed heap allocation
accessor.Write(0, (byte)42);
byte value = accessor.ReadByte(0);

// OS manages the memory, not GC
```

**Benefits:**
- No managed heap pressure
- OS handles paging
- Can exceed available RAM
- Shared between processes

### Strategy 4: Struct of Arrays (SoA)

```csharp
// Array of Structs (AoS) - Bad for large datasets
struct Vector3D { public double X, Y, Z; }
Vector3D[] aos = new Vector3D[30000]; // 720 KB → LOH

// Struct of Arrays (SoA) - Good
class Vectors
{
    public double[] X = new double[30000]; // 240 KB → SOH
    public double[] Y = new double[30000]; // 240 KB → SOH
    public double[] Z = new double[30000]; // 240 KB → SOH
}

// Benefits:
// - Each array stays on SOH
// - Better cache locality for processing
// - Can be compacted
```

## Real-World Scenarios

### Image Processing

```csharp
// 4K image: 3840 × 2160 × 4 bytes (RGBA) = ~32 MB per frame

// Bad: Allocate per frame
for (int i = 0; i < 30; i++) // 30 frames
{
    var buffer = new byte[32 * 1024 * 1024];
    ProcessFrame(buffer);
    // 30 × 32 MB = massive LOH pressure!
}

// Good: Pool buffers
var buffer = ArrayPool<byte>.Shared.Rent(32 * 1024 * 1024);
try
{
    for (int i = 0; i < 30; i++)
    {
        ProcessFrame(buffer);
        // Same buffer reused, zero LOH allocations!
    }
}
finally
{
    ArrayPool<byte>.Shared.Return(buffer);
}
```

### Network Buffers

```csharp
// Many connections with large buffers

// Bad: Allocate per connection
class Connection
{
    private byte[] _buffer = new byte[128 * 1024]; // 128 KB
}
// 1000 connections = 128 MB on LOH!

// Good: Pool buffers
class Connection
{
    private byte[]? _buffer;

    public void Start()
    {
        _buffer = ArrayPool<byte>.Shared.Rent(128 * 1024);
    }

    public void Stop()
    {
        if (_buffer != null)
        {
            ArrayPool<byte>.Shared.Return(_buffer);
            _buffer = null;
        }
    }
}
```

### Data Caching

```csharp
// Caching large objects

// Bad: Long-lived LOH objects
private static Dictionary<string, byte[]> _cache = new();
_cache["key"] = new byte[1024 * 1024]; // 1 MB on LOH forever

// Better: Use WeakReference or size limits
private static Dictionary<string, WeakReference<byte[]>> _cache = new();

// Or use MemoryCache with size limits
var cache = new MemoryCache(new MemoryCacheOptions
{
    SizeLimit = 100 * 1024 * 1024 // 100 MB max
});
```

## Monitoring LOH

### GC Statistics

```csharp
var gcInfo = GC.GetGCMemoryInfo();

Console.WriteLine($"Heap Size: {gcInfo.HeapSizeBytes / 1024.0 / 1024.0:F2} MB");
Console.WriteLine($"Fragmented: {gcInfo.FragmentedBytes / 1024.0 / 1024.0:F2} MB");
Console.WriteLine($"Gen2 Collections: {GC.CollectionCount(2)}");
```

### Performance Counters
- `.NET CLR Memory\Large Object Heap size`
- `.NET CLR Memory\Gen 2 heap size`
- `.NET CLR Memory\# Induced GC`

## Exercises

### Exercise 1: Threshold Exploration
1. Allocate arrays of 84 KB (stays on SOH)
2. Allocate arrays of 85 KB (goes to LOH)
3. Compare GC generations
4. Observe memory behavior

### Exercise 2: Fragmentation Creation
1. Create fragmentation (allocate then release every other)
2. Visualize the gaps
3. Attempt large allocation
4. Compact and retry

### Exercise 3: Pooling Performance
1. Process 1000 buffers without pooling
2. Process 1000 buffers with ArrayPool
3. Compare time, memory, GC collections
4. Calculate efficiency gain

### Exercise 4: Real-World Simulation
Simulate image processing pipeline:
- Load 100 images
- Process each with pooled buffers
- Monitor LOH size
- Compare with non-pooled version

## Best Practices

1. **Pool Large Buffers:** Always use ArrayPool<T> for temporary large buffers
2. **Avoid LOH When Possible:** Stay under 85 KB threshold if feasible
3. **Segment Large Data:** Use multiple smaller arrays instead of one large array
4. **Monitor Fragmentation:** Watch for increasing LOH size without allocation increase
5. **Compact Sparingly:** Only when fragmentation causes issues
6. **Use MMF for Very Large Data:** Memory-mapped files for GB+ datasets
7. **Cache Wisely:** Don't cache large objects indefinitely

## Building and Running

```bash
dotnet build
dotnet run
```

## Requirements
- .NET 9.0 SDK
- Windows OS

## Additional Resources
- [Large Object Heap](https://docs.microsoft.com/en-us/dotnet/standard/garbage-collection/large-object-heap)
- [ArrayPool<T>](https://docs.microsoft.com/en-us/dotnet/api/system.buffers.arraypool-1)
- [Memory-Mapped Files](https://docs.microsoft.com/en-us/dotnet/standard/io/memory-mapped-files)

## Summary

The Large Object Heap requires special attention in high-performance applications. Key takeaways:

- Objects >= 85 KB go to LOH (Gen2)
- LOH is not compacted by default → fragmentation risk
- Pool large buffers with ArrayPool<T>
- Stay under threshold or segment data when possible
- Use memory-mapped files for very large datasets
- Monitor fragmentation and compact when needed

Understanding LOH behavior is essential for building WPF applications that handle large data efficiently.
