# Lesson 04: Weak References and Event Patterns

## Overview
Master weak references and weak event patterns to prevent memory leaks while maintaining flexible object relationships. Learn WeakReference<T>, WeakEventManager, ConditionalWeakTable, and custom weak event implementations.

## Learning Objectives

1. **Understand Weak References**
   - Learn WeakReference vs WeakReference<T>
   - Understand when objects can be collected
   - Implement weak caching patterns
   - Track objects without preventing GC

2. **Master Weak Event Patterns**
   - Prevent event handler memory leaks
   - Use WPF's WeakEventManager
   - Implement custom weak event managers
   - Choose between strong and weak subscriptions

3. **Use ConditionalWeakTable**
   - Attach metadata without preventing GC
   - Extend objects you don't control
   - Implement per-object caching
   - Understand lifetime relationships

4. **Apply Practical Patterns**
   - Weak caching for optional data
   - Event subscription without leaks
   - ViewModel lifecycle management
   - Memory-efficient object tracking

## WeakReference Fundamentals

### Strong vs Weak References

```csharp
// Strong reference - prevents GC
var strongRef = new MyObject();
// Object cannot be collected while strongRef is in scope

// Weak reference - allows GC
var weakRef = new WeakReference<MyObject>(new MyObject());
// Object CAN be collected even though weakRef exists
```

### Using WeakReference<T>

```csharp
// Create weak reference
var obj = new LargeObject();
var weakRef = new WeakReference<LargeObject>(obj);

// Later... check if still alive
if (weakRef.TryGetTarget(out var target))
{
    // Object still exists
    target.DoWork();
}
else
{
    // Object was collected
    Console.WriteLine("Object no longer available");
}
```

### WeakReference vs WeakReference<T>

```csharp
// Old non-generic version (.NET 1.0)
var oldWeak = new WeakReference(obj);
if (oldWeak.IsAlive)
{
    var target = (MyObject)oldWeak.Target; // Casting required
}

// New generic version (.NET 4.5+) - Preferred!
var newWeak = new WeakReference<MyObject>(obj);
if (newWeak.TryGetTarget(out var target))
{
    // Type-safe, no casting
    target.DoWork();
}
```

## Weak Event Pattern

### The Problem: Event Leaks

```csharp
// Publisher
public class DataService
{
    public event EventHandler DataChanged;
}

// Subscriber
public class DataConsumer
{
    public DataConsumer(DataService service)
    {
        // Creates strong reference from service to this
        service.DataChanged += OnDataChanged;

        // BUG: If we don't unsubscribe, DataConsumer
        // cannot be collected while service exists!
    }

    private void OnDataChanged(object sender, EventArgs e) { }
}
```

### Solution 1: Unsubscribe

```csharp
public class DataConsumer : IDisposable
{
    private readonly DataService _service;

    public DataConsumer(DataService service)
    {
        _service = service;
        _service.DataChanged += OnDataChanged;
    }

    public void Dispose()
    {
        _service.DataChanged -= OnDataChanged;
    }

    private void OnDataChanged(object sender, EventArgs e) { }
}
```

### Solution 2: Weak Event Pattern

```csharp
public class WeakEventSource
{
    private readonly List<WeakReference<IEventHandler>> _handlers = new();

    public void Subscribe(IEventHandler handler)
    {
        _handlers.Add(new WeakReference<IEventHandler>(handler));
    }

    public void RaiseEvent()
    {
        // Remove dead references
        _handlers.RemoveAll(wr => !wr.TryGetTarget(out _));

        // Invoke alive handlers
        foreach (var weakRef in _handlers.ToList())
        {
            if (weakRef.TryGetTarget(out var handler))
            {
                handler.HandleEvent();
            }
        }
    }
}
```

## WPF WeakEventManager

### PropertyChangedEventManager

```csharp
// Instead of standard event subscription
viewModel.PropertyChanged += OnPropertyChanged;

// Use WeakEventManager
PropertyChangedEventManager.AddHandler(
    viewModel,
    OnPropertyChanged,
    propertyName: string.Empty); // Empty for all properties

private void OnPropertyChanged(object sender, PropertyChangedEventArgs e)
{
    // Handle property change
}

// No need to unsubscribe! GC handles cleanup automatically
```

### CollectionChangedEventManager

```csharp
var collection = new ObservableCollection<string>();

// Weak subscription
CollectionChangedEventManager.AddHandler(
    collection,
    OnCollectionChanged);

private void OnCollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
{
    // Handle collection change
}
```

### Custom WeakEventManager

```csharp
public class MyEventManager : WeakEventManager
{
    private MyEventManager() { }

    public static void AddHandler(object source, EventHandler handler)
    {
        CurrentManager.ProtectedAddHandler(source, handler);
    }

    public static void RemoveHandler(object source, EventHandler handler)
    {
        CurrentManager.ProtectedRemoveHandler(source, handler);
    }

    private static MyEventManager CurrentManager
    {
        get
        {
            var manager = (MyEventManager)GetCurrentManager(typeof(MyEventManager));
            if (manager == null)
            {
                manager = new MyEventManager();
                SetCurrentManager(typeof(MyEventManager), manager);
            }
            return manager;
        }
    }

    protected override void StartListening(object source)
    {
        if (source is MySource mySource)
        {
            mySource.MyEvent += DeliverEvent;
        }
    }

    protected override void StopListening(object source)
    {
        if (source is MySource mySource)
        {
            mySource.MyEvent -= DeliverEvent;
        }
    }
}
```

## ConditionalWeakTable<TKey, TValue>

### Basic Usage

```csharp
// Attach metadata to objects without preventing GC
var metadata = new ConditionalWeakTable<object, ObjectMetadata>();

var obj = new MyObject();
metadata.Add(obj, new ObjectMetadata { Info = "Important data" });

// Retrieve metadata
if (metadata.TryGetValue(obj, out var data))
{
    Console.WriteLine(data.Info);
}

// When obj is collected, metadata is automatically removed!
```

### Extending Objects

```csharp
// Extend sealed class with custom data
public static class UIElementExtensions
{
    private static ConditionalWeakTable<UIElement, UIMetadata> _metadata = new();

    public static void SetMetadata(this UIElement element, UIMetadata data)
    {
        _metadata.AddOrUpdate(element, data);
    }

    public static UIMetadata? GetMetadata(this UIElement element)
    {
        _metadata.TryGetValue(element, out var data);
        return data;
    }
}

// Usage
var button = new Button();
button.SetMetadata(new UIMetadata { CustomInfo = "My data" });
```

### Per-Object Caching

```csharp
public class ExpensiveCalculator
{
    private static ConditionalWeakTable<object, ComputedResult> _cache = new();

    public ComputedResult Calculate(object input)
    {
        // Check cache
        if (_cache.TryGetValue(input, out var cached))
        {
            return cached;
        }

        // Compute
        var result = PerformExpensiveCalculation(input);

        // Cache with object lifetime
        _cache.Add(input, result);

        return result;
    }
}
```

## Weak Caching Patterns

### Simple Weak Cache

```csharp
public class WeakCache<TKey, TValue> where TKey : notnull where TValue : class
{
    private readonly Dictionary<TKey, WeakReference<TValue>> _cache = new();

    public void Add(TKey key, TValue value)
    {
        _cache[key] = new WeakReference<TValue>(value);
    }

    public bool TryGet(TKey key, out TValue? value)
    {
        if (_cache.TryGetValue(key, out var weakRef))
        {
            return weakRef.TryGetTarget(out value);
        }

        value = null;
        return false;
    }

    public void Cleanup()
    {
        var deadKeys = _cache
            .Where(kvp => !kvp.Value.TryGetTarget(out _))
            .Select(kvp => kvp.Key)
            .ToList();

        foreach (var key in deadKeys)
        {
            _cache.Remove(key);
        }
    }
}
```

### Hybrid Cache (Strong + Weak)

```csharp
public class HybridCache<TKey, TValue> where TKey : notnull where TValue : class
{
    private readonly Dictionary<TKey, TValue> _strongCache = new();
    private readonly Dictionary<TKey, WeakReference<TValue>> _weakCache = new();
    private readonly int _strongCacheLimit;

    public HybridCache(int strongCacheLimit = 10)
    {
        _strongCacheLimit = strongCacheLimit;
    }

    public void Add(TKey key, TValue value)
    {
        // Add to strong cache
        _strongCache[key] = value;

        // Promote to weak if strong cache is full
        if (_strongCache.Count > _strongCacheLimit)
        {
            var oldest = _strongCache.First();
            _strongCache.Remove(oldest.Key);
            _weakCache[oldest.Key] = new WeakReference<TValue>(oldest.Value);
        }
    }

    public bool TryGet(TKey key, out TValue? value)
    {
        // Check strong cache first
        if (_strongCache.TryGetValue(key, out value))
        {
            return true;
        }

        // Check weak cache
        if (_weakCache.TryGetValue(key, out var weakRef) &&
            weakRef.TryGetTarget(out value))
        {
            // Promote back to strong cache on access
            _strongCache[key] = value;
            _weakCache.Remove(key);
            return true;
        }

        value = null;
        return false;
    }
}
```

## Practical Examples

### ViewModel Weak Subscriptions

```csharp
public class ViewModel : INotifyPropertyChanged
{
    private string _data = "";

    public string Data
    {
        get => _data;
        set
        {
            _data = value;
            OnPropertyChanged();
        }
    }

    public event PropertyChangedEventHandler? PropertyChanged;

    protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
    {
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }
}

public class View
{
    public View(ViewModel viewModel)
    {
        // Weak subscription - no leak if View is not disposed properly
        PropertyChangedEventManager.AddHandler(
            viewModel,
            OnViewModelPropertyChanged,
            string.Empty);
    }

    private void OnViewModelPropertyChanged(object? sender, PropertyChangedEventArgs e)
    {
        // Update UI
    }
}
```

### Image Cache

```csharp
public class ImageCache
{
    private readonly Dictionary<string, WeakReference<BitmapImage>> _cache = new();

    public BitmapImage GetImage(string path)
    {
        // Check cache
        if (_cache.TryGetValue(path, out var weakRef) &&
            weakRef.TryGetTarget(out var image))
        {
            return image;
        }

        // Load image
        image = new BitmapImage(new Uri(path));

        // Cache weakly - allows GC under memory pressure
        _cache[path] = new WeakReference<BitmapImage>(image);

        return image;
    }
}
```

## Best Practices

1. **Use WeakReference<T>** instead of WeakReference (generic is faster, type-safe)

2. **Clean up dead references** periodically:
   ```csharp
   list.RemoveAll(wr => !wr.TryGetTarget(out _));
   ```

3. **WeakEventManager for WPF events** (PropertyChanged, CollectionChanged)

4. **ConditionalWeakTable for metadata** attachment without preventing GC

5. **Weak caching for optional data** that can be recreated

6. **Don't use weak references for critical data** that must not be collected

7. **Test with GC.Collect()** to verify weak reference behavior

## Common Pitfalls

### Pitfall 1: Weak Reference to Value Type
```csharp
// WRONG - value type boxed, then immediately eligible for collection
var weakRef = new WeakReference<int>(42);
// int is boxed, no other reference, collected immediately!

// Use reference types with weak references
var weakRef = new WeakReference<MyClass>(new MyClass());
```

### Pitfall 2: Assuming Weak References Stay Alive
```csharp
var weak = new WeakReference<MyClass>(new MyClass());

// WRONG - might be null!
weak.TryGetTarget(out var target);
target.DoWork(); // NullReferenceException possible!

// CORRECT - always check
if (weak.TryGetTarget(out var target))
{
    target.DoWork();
}
```

### Pitfall 3: Weak References to Short-Lived Objects
```csharp
// Pointless - object collected almost immediately
var weak = new WeakReference<byte[]>(new byte[1024]);
GC.Collect();
// Likely collected already!

// Better - keep strong reference for short-term use
var buffer = new byte[1024];
// Use buffer
// Let it be collected naturally when out of scope
```

## Exercises

Complete exercises in the application to master weak references and event patterns.

## Building and Running

```bash
dotnet build
dotnet run
```

## Requirements
- .NET 9.0 SDK
- Windows OS

## Summary

Weak references and weak event patterns are essential for preventing memory leaks in event-driven applications. Master these concepts to build robust, memory-efficient WPF applications.
