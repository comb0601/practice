using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows;
using System.Windows.Threading;

namespace WeakReferences;

public partial class MainWindow : Window
{
    // Strong references
    private readonly List<object> _strongReferences = new();

    // Weak references
    private readonly List<WeakReference<object>> _weakReferences = new();

    // Event sources
    private readonly EventSource _eventSource = new();
    private readonly List<StandardSubscriber> _standardSubscribers = new();
    private readonly List<WeakSubscriber> _weakSubscribers = new();

    // PropertyChanged source
    private readonly ObservableViewModel _viewModel = new();
    private readonly List<IWeakEventListener> _propertyListeners = new();

    // Collection
    private readonly ObservableCollection<string> _collection = new();
    private bool _collectionSubscribed = false;

    // ConditionalWeakTable
    private readonly ConditionalWeakTable<object, ObjectMetadata> _metadata = new();
    private readonly List<object> _cwtObjects = new();

    // Custom weak event manager
    private readonly CustomWeakEventManager _customEventManager = new();

    // Caches
    private readonly Dictionary<string, CachedObject> _strongCache = new();
    private readonly Dictionary<string, WeakReference<CachedObject>> _weakCache = new();

    // Statistics timer
    private readonly DispatcherTimer _statsTimer;

    public MainWindow()
    {
        InitializeComponent();

        _statsTimer = new DispatcherTimer { Interval = TimeSpan.FromSeconds(2) };
        _statsTimer.Tick += (s, e) => UpdateStatistics();
        _statsTimer.Start();

        UpdateStatistics();
    }

    #region WeakReference Basics

    private void CreateStrongReferences_Click(object sender, RoutedEventArgs e)
    {
        for (int i = 0; i < 100; i++)
        {
            _strongReferences.Add(new LargeObject(i));
        }

        UpdateReferenceStats();
        MessageBox.Show("Created 100 strong references.\nThese objects CANNOT be collected.",
            "Strong References", MessageBoxButton.OK, MessageBoxImage.Information);
    }

    private void CreateWeakReferences_Click(object sender, RoutedEventArgs e)
    {
        for (int i = 0; i < 100; i++)
        {
            var obj = new LargeObject(i);
            _weakReferences.Add(new WeakReference<object>(obj));
        }

        UpdateReferenceStats();
        MessageBox.Show("Created 100 weak references.\nThese objects CAN be collected after GC.",
            "Weak References", MessageBoxButton.OK, MessageBoxImage.Information);
    }

    private void CheckReferences_Click(object sender, RoutedEventArgs e)
    {
        UpdateReferenceStats();

        int weakAlive = _weakReferences.Count(wr => wr.TryGetTarget(out _));

        MessageBox.Show(
            $"Strong References: {_strongReferences.Count} (all alive)\n" +
            $"Weak References: {_weakReferences.Count} total, {weakAlive} alive\n\n" +
            $"Weak references that pointed to collected objects: {_weakReferences.Count - weakAlive}",
            "Reference Status",
            MessageBoxButton.OK,
            MessageBoxImage.Information);
    }

    private void UpdateReferenceStats()
    {
        StrongRefCount.Text = $"Created: {_strongReferences.Count}";
        StrongRefAlive.Text = $"Alive: {_strongReferences.Count}";

        int weakAlive = _weakReferences.Count(wr => wr.TryGetTarget(out _));
        WeakRefCount.Text = $"Created: {_weakReferences.Count}";
        WeakRefAlive.Text = $"Alive: {weakAlive}";
    }

    #endregion

    #region Weak Event Pattern

    private void SubscribeStandard_Click(object sender, RoutedEventArgs e)
    {
        var subscriber = new StandardSubscriber();
        _eventSource.SomeEvent += subscriber.OnEvent;
        _standardSubscribers.Add(subscriber); // Keep reference to prevent collection

        UpdateSubscriberStats();
        MessageBox.Show("Subscribed with standard event (creates strong reference)",
            "Standard Subscription", MessageBoxButton.OK, MessageBoxImage.Information);
    }

    private void SubscribeWeak_Click(object sender, RoutedEventArgs e)
    {
        var subscriber = new WeakSubscriber();
        _eventSource.SubscribeWeak(subscriber);
        _weakSubscribers.Add(subscriber);

        UpdateSubscriberStats();
        MessageBox.Show("Subscribed with weak event (creates weak reference)",
            "Weak Subscription", MessageBoxButton.OK, MessageBoxImage.Information);
    }

    private void TriggerEvents_Click(object sender, RoutedEventArgs e)
    {
        _eventSource.RaiseEvent();
        MessageBox.Show("Event triggered. Check output for subscriber notifications.",
            "Event Raised", MessageBoxButton.OK, MessageBoxImage.Information);
    }

    private void ClearSubscribers_Click(object sender, RoutedEventArgs e)
    {
        _standardSubscribers.Clear();
        _weakSubscribers.Clear();

        GC.Collect();
        GC.WaitForPendingFinalizers();

        _eventSource.Cleanup();
        UpdateSubscriberStats();

        MessageBox.Show("Cleared subscriber references and performed GC.\n" +
            "Standard event subscribers still held by event source!\n" +
            "Weak event subscribers released.",
            "Cleared", MessageBoxButton.OK, MessageBoxImage.Information);
    }

    private void UpdateSubscriberStats()
    {
        _eventSource.Cleanup(); // Remove dead weak references

        StandardSubscribers.Text = $"Standard: {_standardSubscribers.Count} subscribers";
        WeakSubscribers.Text = $"Weak: {_eventSource.WeakSubscriberCount} subscribers";
    }

    #endregion

    #region WeakEventManager

    private void SubscribePropertyChanged_Click(object sender, RoutedEventArgs e)
    {
        var listener = new PropertyChangeListener();
        PropertyChangedEventManager.AddHandler(_viewModel, listener.OnPropertyChanged, string.Empty);
        _propertyListeners.Add(listener);

        PropertyChangedStatus.Text = $"Subscribed {_propertyListeners.Count} listener(s) using WeakEventManager";
    }

    private void UpdateProperty_Click(object sender, RoutedEventArgs e)
    {
        _viewModel.Name = $"Updated at {DateTime.Now:HH:mm:ss}";
        PropertyChangedStatus.Text = $"Property updated: {_viewModel.Name}";
    }

    private void RemoveListener_Click(object sender, RoutedEventArgs e)
    {
        _propertyListeners.Clear();
        GC.Collect();

        PropertyChangedStatus.Text = "Listeners cleared and GC performed. Weak references allow automatic cleanup!";
    }

    private void SubscribeCollection_Click(object sender, RoutedEventArgs e)
    {
        if (!_collectionSubscribed)
        {
            CollectionChangedEventManager.AddHandler(_collection, OnCollectionChanged);
            _collectionSubscribed = true;
            CollectionStatus.Text = "Subscribed to collection using WeakEventManager";
        }
    }

    private void ModifyCollection_Click(object sender, RoutedEventArgs e)
    {
        _collection.Add($"Item {_collection.Count + 1}");
        CollectionStatus.Text = $"Collection modified. Count: {_collection.Count}";
    }

    private void OnCollectionChanged(object? sender, NotifyCollectionChangedEventArgs e)
    {
        CollectionStatus.Text = $"Collection changed! Action: {e.Action}, Count: {_collection.Count}";
    }

    #endregion

    #region ConditionalWeakTable

    private void CreateCWTObjects_Click(object sender, RoutedEventArgs e)
    {
        for (int i = 0; i < 10; i++)
        {
            _cwtObjects.Add(new object());
        }

        CWTStatus.Text = $"Created {_cwtObjects.Count} objects (no metadata yet)";
    }

    private void AttachMetadata_Click(object sender, RoutedEventArgs e)
    {
        if (_cwtObjects.Count == 0)
        {
            MessageBox.Show("Create objects first!", "Error", MessageBoxButton.OK, MessageBoxImage.Warning);
            return;
        }

        foreach (var obj in _cwtObjects)
        {
            _metadata.Add(obj, new ObjectMetadata
            {
                CreatedAt = DateTime.Now,
                Description = $"Metadata for object {obj.GetHashCode()}"
            });
        }

        CWTStatus.Text = $"Attached metadata to {_cwtObjects.Count} objects";
    }

    private void RetrieveMetadata_Click(object sender, RoutedEventArgs e)
    {
        if (_cwtObjects.Count == 0)
        {
            MessageBox.Show("Create objects first!", "Error", MessageBoxButton.OK, MessageBoxImage.Warning);
            return;
        }

        var sb = new System.Text.StringBuilder();
        foreach (var obj in _cwtObjects.Take(3)) // Show first 3
        {
            if (_metadata.TryGetValue(obj, out var metadata))
            {
                sb.AppendLine($"Object {obj.GetHashCode()}: {metadata.Description}");
            }
        }

        CWTStatus.Text = $"Retrieved metadata:\n{sb}";
    }

    private void ClearCWTObjects_Click(object sender, RoutedEventArgs e)
    {
        _cwtObjects.Clear();
        GC.Collect();

        CWTStatus.Text = "Objects cleared and GC performed.\nMetadata automatically removed by ConditionalWeakTable!";
    }

    #endregion

    #region Custom Weak Event Manager

    private void AddWeakHandler_Click(object sender, RoutedEventArgs e)
    {
        var handler = new CustomEventHandler();
        _customEventManager.AddHandler(handler);

        CustomEventStatus.Text = $"Added weak handler. Active handlers: {_customEventManager.ActiveHandlerCount}";
    }

    private void RaiseCustomEvent_Click(object sender, RoutedEventArgs e)
    {
        _customEventManager.RaiseEvent("Custom event data");
        CustomEventStatus.Text = $"Event raised. Active handlers: {_customEventManager.ActiveHandlerCount}";
    }

    private void ShowActiveHandlers_Click(object sender, RoutedEventArgs e)
    {
        int count = _customEventManager.ActiveHandlerCount;
        MessageBox.Show(
            $"Active handlers: {count}\n\n" +
            $"Handlers are held with weak references.\n" +
            $"They will be automatically removed when collected.",
            "Handler Status",
            MessageBoxButton.OK,
            MessageBoxImage.Information);
    }

    #endregion

    #region Caching Patterns

    private void AddToStrongCache_Click(object sender, RoutedEventArgs e)
    {
        for (int i = 0; i < 10; i++)
        {
            string key = $"strong_{_strongCache.Count}";
            _strongCache[key] = new CachedObject(key);
        }

        UpdateCacheStats();
    }

    private void AddToWeakCache_Click(object sender, RoutedEventArgs e)
    {
        for (int i = 0; i < 10; i++)
        {
            string key = $"weak_{_weakCache.Count}";
            var obj = new CachedObject(key);
            _weakCache[key] = new WeakReference<CachedObject>(obj);
        }

        UpdateCacheStats();
    }

    private void AccessStrongCache_Click(object sender, RoutedEventArgs e)
    {
        int accessed = 0;
        foreach (var kvp in _strongCache.Take(5))
        {
            kvp.Value.LastAccessed = DateTime.Now;
            accessed++;
        }

        CacheAccessResult.Text = $"Accessed {accessed} items from strong cache. All items always available.";
    }

    private void AccessWeakCache_Click(object sender, RoutedEventArgs e)
    {
        int accessed = 0;
        int collected = 0;

        foreach (var kvp in _weakCache.Take(5))
        {
            if (kvp.Value.TryGetTarget(out var obj))
            {
                obj.LastAccessed = DateTime.Now;
                accessed++;
            }
            else
            {
                collected++;
            }
        }

        CacheAccessResult.Text = $"Accessed {accessed} items, {collected} were collected.\n" +
            "Weak cache allows GC to reclaim memory under pressure.";
    }

    private void ClearCaches_Click(object sender, RoutedEventArgs e)
    {
        _strongCache.Clear();
        _weakCache.Clear();
        GC.Collect();

        UpdateCacheStats();
        CacheAccessResult.Text = "Both caches cleared.";
    }

    private void UpdateCacheStats()
    {
        StrongCacheStats.Text = $"Items: {_strongCache.Count}";

        int weakAlive = _weakCache.Count(kvp => kvp.Value.TryGetTarget(out _));
        WeakCacheStats.Text = $"Items: {_weakCache.Count}, Alive: {weakAlive}";
    }

    #endregion

    #region Common Methods

    private void ForceGC_Click(object sender, RoutedEventArgs e)
    {
        GC.Collect();
        GC.WaitForPendingFinalizers();
        GC.Collect();

        UpdateStatistics();
        UpdateReferenceStats();
        UpdateSubscriberStats();
        UpdateCacheStats();

        MessageBox.Show("Forced garbage collection complete.", "GC",
            MessageBoxButton.OK, MessageBoxImage.Information);
    }

    private void UpdateStats_Click(object sender, RoutedEventArgs e)
    {
        UpdateStatistics();
        UpdateReferenceStats();
        UpdateSubscriberStats();
        UpdateCacheStats();
    }

    private void UpdateStatistics()
    {
        var memory = GC.GetTotalMemory(false);
        MemoryUsage.Text = $"{memory / (1024.0 * 1024.0):F2} MB";
        Gen0Collections.Text = GC.CollectionCount(0).ToString();
        Gen1Collections.Text = GC.CollectionCount(1).ToString();
        Gen2Collections.Text = GC.CollectionCount(2).ToString();
    }

    protected override void OnClosing(CancelEventArgs e)
    {
        _statsTimer.Stop();
        base.OnClosing(e);
    }

    #endregion
}

#region Helper Classes

public class LargeObject
{
    private byte[] _data = new byte[1024 * 100]; // 100 KB
    public int Id { get; }

    public LargeObject(int id)
    {
        Id = id;
    }
}

public class EventSource
{
    public event EventHandler? SomeEvent;
    private readonly List<WeakReference<WeakSubscriber>> _weakSubscribers = new();

    public int WeakSubscriberCount => _weakSubscribers.Count(wr => wr.TryGetTarget(out _));

    public void RaiseEvent()
    {
        SomeEvent?.Invoke(this, EventArgs.Empty);

        // Raise for weak subscribers
        Cleanup();
        foreach (var weakRef in _weakSubscribers.ToList())
        {
            if (weakRef.TryGetTarget(out var subscriber))
            {
                subscriber.OnEvent(this, EventArgs.Empty);
            }
        }
    }

    public void SubscribeWeak(WeakSubscriber subscriber)
    {
        _weakSubscribers.Add(new WeakReference<WeakSubscriber>(subscriber));
    }

    public void Cleanup()
    {
        _weakSubscribers.RemoveAll(wr => !wr.TryGetTarget(out _));
    }
}

public class StandardSubscriber
{
    public void OnEvent(object? sender, EventArgs e)
    {
        System.Diagnostics.Debug.WriteLine($"StandardSubscriber {GetHashCode()} received event");
    }
}

public class WeakSubscriber
{
    public void OnEvent(object? sender, EventArgs e)
    {
        System.Diagnostics.Debug.WriteLine($"WeakSubscriber {GetHashCode()} received event");
    }
}

public class ObservableViewModel : INotifyPropertyChanged
{
    private string _name = "Initial";

    public string Name
    {
        get => _name;
        set
        {
            if (_name != value)
            {
                _name = value;
                OnPropertyChanged();
            }
        }
    }

    public event PropertyChangedEventHandler? PropertyChanged;

    protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
    {
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }
}

public class PropertyChangeListener : IWeakEventListener
{
    public bool ReceiveWeakEvent(Type managerType, object sender, EventArgs e)
    {
        if (e is PropertyChangedEventArgs args)
        {
            System.Diagnostics.Debug.WriteLine($"Property changed: {args.PropertyName}");
        }
        return true;
    }

    public void OnPropertyChanged(object? sender, PropertyChangedEventArgs e)
    {
        System.Diagnostics.Debug.WriteLine($"Property changed: {e.PropertyName}");
    }
}

public class ObjectMetadata
{
    public DateTime CreatedAt { get; set; }
    public string Description { get; set; } = string.Empty;
}

public class CustomWeakEventManager
{
    private readonly List<WeakReference<CustomEventHandler>> _handlers = new();

    public int ActiveHandlerCount
    {
        get
        {
            Cleanup();
            return _handlers.Count;
        }
    }

    public void AddHandler(CustomEventHandler handler)
    {
        _handlers.Add(new WeakReference<CustomEventHandler>(handler));
    }

    public void RaiseEvent(string data)
    {
        Cleanup();

        foreach (var weakRef in _handlers.ToList())
        {
            if (weakRef.TryGetTarget(out var handler))
            {
                handler.HandleEvent(data);
            }
        }
    }

    private void Cleanup()
    {
        _handlers.RemoveAll(wr => !wr.TryGetTarget(out _));
    }
}

public class CustomEventHandler
{
    public void HandleEvent(string data)
    {
        System.Diagnostics.Debug.WriteLine($"CustomEventHandler {GetHashCode()} handled: {data}");
    }
}

public class CachedObject
{
    private byte[] _data = new byte[1024 * 50]; // 50 KB
    public string Key { get; }
    public DateTime LastAccessed { get; set; }

    public CachedObject(string key)
    {
        Key = key;
        LastAccessed = DateTime.Now;
    }
}

#endregion
