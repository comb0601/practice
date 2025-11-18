# Lesson 05: Resource Management Best Practices

## Overview
Master proper resource management in WPF applications through IDisposable pattern, Freezable objects, resource dictionaries, and modern async disposal. Build applications that efficiently manage both managed and unmanaged resources.

## Learning Objectives

1. **Master IDisposable Pattern**
   - Implement proper disposal patterns
   - Use using statements effectively
   - Understand finalizers vs Dispose
   - Handle both managed and unmanaged resources

2. **Optimize with Freezable Objects**
   - Freeze Brushes, Geometries, and Transforms
   - Reduce memory usage through sharing
   - Understand thread-safety benefits
   - Identify when to freeze resources

3. **Manage Resource Dictionaries**
   - Use StaticResource vs DynamicResource appropriately
   - Clear merged dictionaries when done
   - Prevent resource dictionary leaks
   - Optimize resource sharing

4. **Apply Modern Patterns**
   - Use IAsyncDisposable for async cleanup
   - Implement proper stream management
   - Follow best practices for deterministic cleanup
   - Avoid common resource management pitfalls

## IDisposable Pattern

### Standard Implementation

```csharp
public class MyResource : IDisposable
{
    private IntPtr _unmanagedResource;
    private Stream? _managedResource;
    private bool _disposed = false;

    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this); // Prevent finalizer from running
    }

    protected virtual void Dispose(bool disposing)
    {
        if (!_disposed)
        {
            if (disposing)
            {
                // Dispose managed resources
                _managedResource?.Dispose();
                _managedResource = null;
            }

            // Free unmanaged resources
            if (_unmanagedResource != IntPtr.Zero)
            {
                NativeMethods.FreeHandle(_unmanagedResource);
                _unmanagedResource = IntPtr.Zero;
            }

            _disposed = true;
        }
    }

    ~MyResource()
    {
        // Finalizer as safety net
        Dispose(false);
    }
}
```

### Using Statements

```csharp
// Traditional using statement
using (var resource = new MyResource())
{
    resource.DoWork();
} // Dispose() called automatically

// C# 8+ using declaration
using var resource = new MyResource();
resource.DoWork();
// Disposed at end of scope
```

### Why GC.SuppressFinalize?

```csharp
public void Dispose()
{
    Dispose(true);

    // Tell GC to skip finalization
    // Saves the cost of running finalizer
    // Allows object to be collected in 1 GC cycle instead of 2
    GC.SuppressFinalize(this);
}
```

## Freezable Objects

### What is Freezable?

Freezable is a WPF base class for objects that can be made immutable (frozen):
- **Brush**, **Pen**, **Geometry**, **Transform**, **Animation**
- Once frozen, cannot be modified
- Can be safely shared across threads
- Significantly reduced memory usage

### Performance Benefits

```csharp
// BAD: Each element gets its own brush
for (int i = 0; i < 1000; i++)
{
    var element = new Rectangle();
    element.Fill = new SolidColorBrush(Colors.Red); // 1000 instances!
}

// GOOD: All elements share one frozen brush
var sharedBrush = new SolidColorBrush(Colors.Red);
sharedBrush.Freeze(); // Make immutable

for (int i = 0; i < 1000; i++)
{
    var element = new Rectangle();
    element.Fill = sharedBrush; // 1 shared instance!
}
```

### When to Freeze

**Always freeze:**
- Brushes, pens, geometries used by multiple elements
- Static resources that won't change
- Objects used across threads

**Don't freeze:**
- Objects that need to be animated
- Objects that change during application lifetime
- Objects used by single element with no sharing

### Freezing Examples

```csharp
// Brush
var brush = new LinearGradientBrush(Colors.Red, Colors.Blue, 45);
if (brush.CanFreeze) brush.Freeze();

// Geometry
var geometry = new EllipseGeometry(new Point(50, 50), 30, 30);
geometry.Freeze();

// Transform
var transform = new RotateTransform(45);
transform.Freeze();

// In ResourceDictionary
<ResourceDictionary>
    <SolidColorBrush x:Key="SharedBrush" Color="Red"
                     po:Freeze="True"/>
</ResourceDictionary>
```

### Thread Safety

```csharp
// Frozen objects are thread-safe
var brush = new SolidColorBrush(Colors.Blue);
brush.Freeze();

// Can now safely use from any thread
Task.Run(() =>
{
    // OK - brush is frozen
    someElement.Dispatcher.Invoke(() =>
    {
        someElement.Fill = brush;
    });
});
```

## Resource Dictionary Management

### StaticResource vs DynamicResource

```csharp
// StaticResource - resolved once at compile/load time
<Button Background="{StaticResource MyBrush}"/>
// Faster, but won't update if resource changes

// DynamicResource - can be updated at runtime
<Button Background="{DynamicResource MyBrush}"/>
// Slight overhead, but supports runtime changes
```

### Clearing Resources

```csharp
// Add resources
Resources.MergedDictionaries.Add(heavyDictionary);

// When done (e.g., unloading view)
Resources.MergedDictionaries.Clear();

// Or remove specific dictionary
Resources.MergedDictionaries.Remove(heavyDictionary);
```

### Resource Lifecycle

```csharp
public class MyView : UserControl, IDisposable
{
    private ResourceDictionary? _localResources;

    public MyView()
    {
        _localResources = new ResourceDictionary
        {
            Source = new Uri("Resources/MyResources.xaml", UriKind.Relative)
        };
        Resources.MergedDictionaries.Add(_localResources);
    }

    public void Dispose()
    {
        // Clear resources when view is disposed
        Resources.MergedDictionaries.Remove(_localResources);
        _localResources = null;
    }
}
```

## Stream Management

### Proper Pattern

```csharp
// Always use using for streams
using (var stream = File.OpenRead(path))
{
    // Read data
    var buffer = new byte[1024];
    stream.Read(buffer, 0, buffer.Length);
} // Stream closed and disposed automatically

// Or with using declaration
using var stream = File.OpenWrite(path);
stream.WriteByte(42);
// Disposed at end of method
```

### Common Mistakes

```csharp
// WRONG - Stream never disposed!
var stream = File.OpenRead(path);
var buffer = new byte[1024];
stream.Read(buffer, 0, buffer.Length);
// File handle remains open!

// WRONG - Might not dispose on exception
var stream = File.OpenRead(path);
try
{
    DoWork(stream);
    stream.Dispose();
}
catch
{
    // stream never disposed!
}

// CORRECT - using handles exceptions
using var stream = File.OpenRead(path);
DoWork(stream);
// Disposed even if exception occurs
```

## Async Disposal (IAsyncDisposable)

### When to Use

Use IAsyncDisposable for resources requiring async cleanup:
- Network connections
- Database connections
- Async file operations
- Cleanup requiring I/O

### Implementation

```csharp
public class AsyncResource : IAsyncDisposable
{
    private NetworkStream? _stream;

    public async ValueTask DisposeAsync()
    {
        if (_stream != null)
        {
            // Async cleanup
            await _stream.FlushAsync();
            await _stream.DisposeAsync();
            _stream = null;
        }

        GC.SuppressFinalize(this);
    }
}

// Usage
await using var resource = new AsyncResource();
await resource.DoWorkAsync();
// DisposeAsync() called automatically
```

### Supporting Both Patterns

```csharp
public class HybridResource : IDisposable, IAsyncDisposable
{
    private bool _disposed = false;

    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    public async ValueTask DisposeAsync()
    {
        await DisposeAsyncCore();
        Dispose(false);
        GC.SuppressFinalize(this);
    }

    protected virtual void Dispose(bool disposing)
    {
        if (!_disposed && disposing)
        {
            // Sync cleanup
            _disposed = true;
        }
    }

    protected virtual async ValueTask DisposeAsyncCore()
    {
        // Async cleanup
        await Task.CompletedTask;
    }
}
```

## Best Practices

### 1. Always Dispose IDisposable

```csharp
// Use using statements
using var disposable = CreateDisposable();

// Or implement IDisposable in container
public class Container : IDisposable
{
    private readonly IDisposable _resource;

    public Container()
    {
        _resource = CreateResource();
    }

    public void Dispose()
    {
        _resource?.Dispose();
    }
}
```

### 2. Freeze Shared Freezables

```csharp
// In ResourceDictionary or static fields
private static readonly Brush SharedBrush = CreateFrozenBrush();

private static Brush CreateFrozenBrush()
{
    var brush = new SolidColorBrush(Colors.Blue);
    brush.Freeze();
    return brush;
}
```

### 3. Clear Resources on Unload

```csharp
private void OnViewUnloaded(object sender, RoutedEventArgs e)
{
    // Clear resources
    Resources.MergedDictionaries.Clear();

    // Dispose subscriptions
    _subscription?.Dispose();

    // Clear references
    DataContext = null;
}
```

### 4. Use Finalizers as Safety Net

```csharp
public class MyResource : IDisposable
{
    ~MyResource()
    {
        // Safety net if Dispose() not called
        // But don't rely on this!
        Dispose(false);

        // In debug builds, warn about missed disposal
        Debug.Assert(false, "MyResource not disposed properly!");
    }

    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this); // Skip finalizer
    }
}
```

### 5. Test Disposal

```csharp
[TestMethod]
public void TestDisposal()
{
    var resource = new MyResource();
    Assert.IsFalse(resource.IsDisposed);

    resource.Dispose();
    Assert.IsTrue(resource.IsDisposed);

    // Should not throw
    resource.Dispose(); // Dispose should be idempotent
}
```

## Common Pitfalls

### Pitfall 1: Not Disposing

```csharp
// WRONG
var stream = File.OpenRead(path);
DoWork(stream);
// Leak!

// CORRECT
using var stream = File.OpenRead(path);
DoWork(stream);
```

### Pitfall 2: Disposing Too Early

```csharp
// WRONG
var bitmap = LoadBitmap();
bitmap.Dispose();
// Later...
image.Source = bitmap; // ObjectDisposedException!

// CORRECT
var bitmap = LoadBitmap();
image.Source = bitmap;
// Keep bitmap alive as long as needed
// Dispose when image no longer uses it
```

### Pitfall 3: Unfrozen Shared Resources

```csharp
// WRONG - Each element creates dependency
var brush = new SolidColorBrush(Colors.Red);
element1.Fill = brush;
element2.Fill = brush;
// Both elements track changes to brush!

// CORRECT - Frozen brush is immutable
brush.Freeze();
element1.Fill = brush;
element2.Fill = brush;
// No change tracking needed
```

## Performance Impact

### Memory Usage

- **Unfrozen Brushes:** ~200 bytes each × 1000 = 200 KB
- **Frozen Brush:** ~200 bytes × 1 = 200 bytes (99% savings!)

### GC Pressure

- **Without Disposal:** Finalizers run, 2 GC cycles required
- **With Disposal:** No finalizer, 1 GC cycle

### Resource Leaks

- **File Handles:** Limited per process (~10,000)
- **Memory:** Grows unbounded without disposal
- **Connections:** Database/network connections exhausted

## Exercises

Complete all exercises in the application to master resource management.

## Building and Running

```bash
dotnet build
dotnet run
```

## Requirements
- .NET 9.0 SDK
- Windows OS

## Summary

Proper resource management is essential for reliable, performant WPF applications:

- **Implement IDisposable** for all classes holding unmanaged resources
- **Use using statements** for deterministic cleanup
- **Freeze Freezable objects** for memory efficiency and thread safety
- **Clear resource dictionaries** when views are unloaded
- **Use IAsyncDisposable** for async resource cleanup
- **Test disposal** in unit tests
- **Monitor resources** with profilers

Master these patterns to build robust, production-ready applications.
