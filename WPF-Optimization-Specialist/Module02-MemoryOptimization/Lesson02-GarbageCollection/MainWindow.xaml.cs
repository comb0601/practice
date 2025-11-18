using System.Buffers;
using System.ComponentModel;
using System.Diagnostics;
using System.Runtime;
using System.Windows;
using System.Windows.Threading;

namespace GarbageCollection;

public partial class MainWindow : Window
{
    // Generation tracking
    private readonly List<object> _gen0Objects = new();
    private readonly List<object> _gen1Objects = new();
    private readonly List<object> _gen2Objects = new();

    // Object pooling
    private readonly Stack<ReusableObject> _objectPool = new();
    private int _poolUsageCount = 0;

    // Memory pressure tracking
    private long _currentMemoryPressure = 0;
    private int _pressureCollections = 0;

    // GC notifications
    private bool _gcNotificationsEnabled = false;
    private CancellationTokenSource? _notificationCts;

    // Statistics
    private readonly DispatcherTimer _statsTimer;
    private readonly Stopwatch _allocationStopwatch = new();
    private long _lastTotalMemory = 0;
    private int _excessiveAllocationCount = 0;
    private int _arrayPoolUsageCount = 0;

    // Finalization tracking
    private static int _finalizableObjectCount = 0;

    // Collection tracking
    private int _lastGen0Collections = 0;
    private int _lastGen1Collections = 0;
    private int _lastGen2Collections = 0;

    public MainWindow()
    {
        InitializeComponent();

        _statsTimer = new DispatcherTimer
        {
            Interval = TimeSpan.FromSeconds(1)
        };
        _statsTimer.Tick += (s, e) => UpdateLiveStatistics();
        _statsTimer.Start();

        RefreshGCInfo();
        UpdateLiveStatistics();
        UpdateGenerationStats();
    }

    #region Generations Tab

    private void AllocateGen0_Click(object sender, RoutedEventArgs e)
    {
        // Allocate objects that will stay in Gen0
        for (int i = 0; i < 1000; i++)
        {
            _gen0Objects.Add(new byte[1024]); // 1 KB each
        }
        UpdateGenerationStats();
    }

    private void PromoteToGen1_Click(object sender, RoutedEventArgs e)
    {
        // Perform Gen0 collection to promote objects
        GC.Collect(0);
        GC.WaitForPendingFinalizers();

        // Move objects to Gen1 tracking
        _gen1Objects.AddRange(_gen0Objects);
        _gen0Objects.Clear();

        UpdateGenerationStats();
    }

    private void PromoteToGen2_Click(object sender, RoutedEventArgs e)
    {
        // Perform Gen1 collection to promote objects
        GC.Collect(1);
        GC.WaitForPendingFinalizers();

        // Move objects to Gen2 tracking
        _gen2Objects.AddRange(_gen1Objects);
        _gen1Objects.Clear();

        UpdateGenerationStats();
    }

    private void ClearAllocations_Click(object sender, RoutedEventArgs e)
    {
        _gen0Objects.Clear();
        _gen1Objects.Clear();
        _gen2Objects.Clear();

        GC.Collect();
        GC.WaitForPendingFinalizers();
        GC.Collect();

        UpdateGenerationStats();
    }

    private void UpdateGenerationStats()
    {
        // Calculate sizes
        long gen0Size = _gen0Objects.Count * 1024; // Each object is 1 KB
        long gen1Size = _gen1Objects.Count * 1024;
        long gen2Size = _gen2Objects.Count * 1024;

        Gen0ObjectCount.Text = $"Objects: {_gen0Objects.Count}";
        Gen0Size.Text = $"Size: {gen0Size / 1024} KB";
        Gen0Collections.Text = $"Collections: {GC.CollectionCount(0)}";

        Gen1ObjectCount.Text = $"Objects: {_gen1Objects.Count}";
        Gen1Size.Text = $"Size: {gen1Size / 1024} KB";
        Gen1Collections.Text = $"Collections: {GC.CollectionCount(1)}";

        Gen2ObjectCount.Text = $"Objects: {_gen2Objects.Count}";
        Gen2Size.Text = $"Size: {gen2Size / 1024} KB";
        Gen2Collections.Text = $"Collections: {GC.CollectionCount(2)}";
    }

    #endregion

    #region GC Modes Tab

    private void RefreshGCInfo_Click(object sender, RoutedEventArgs e)
    {
        RefreshGCInfo();
    }

    private void RefreshGCInfo()
    {
        var isServerGC = GCSettings.IsServerGC;
        var latencyMode = GCSettings.LatencyMode;

        GCModeInfo.Text = $"GC Mode: {(isServerGC ? "Server GC" : "Workstation GC")}";
        GCLatencyMode.Text = $"Latency Mode: {latencyMode}";
        GCConcurrentMode.Text = $"Concurrent: {(GCSettings.LatencyMode != GCLatencyMode.Batch)}";
    }

    private void SetInteractiveMode_Click(object sender, RoutedEventArgs e)
    {
        GCSettings.LatencyMode = GCLatencyMode.Interactive;
        RefreshGCInfo();
        MessageBox.Show("Latency mode set to Interactive (default for UI apps)", "GC Mode", MessageBoxButton.OK, MessageBoxImage.Information);
    }

    private void SetLowLatencyMode_Click(object sender, RoutedEventArgs e)
    {
        var result = MessageBox.Show(
            "LowLatency mode minimizes GC pauses but may increase memory usage.\n\n" +
            "Use only for short critical sections. Continue?",
            "Warning",
            MessageBoxButton.YesNo,
            MessageBoxImage.Warning);

        if (result == MessageBoxResult.Yes)
        {
            GCSettings.LatencyMode = GCLatencyMode.LowLatency;
            RefreshGCInfo();
        }
    }

    private void SetBatchMode_Click(object sender, RoutedEventArgs e)
    {
        GCSettings.LatencyMode = GCLatencyMode.Batch;
        RefreshGCInfo();
        MessageBox.Show("Latency mode set to Batch (disables concurrent GC)", "GC Mode", MessageBoxButton.OK, MessageBoxImage.Information);
    }

    #endregion

    #region Collection Triggers Tab

    private void CollectGen0_Click(object sender, RoutedEventArgs e)
    {
        PerformCollection(0);
    }

    private void CollectGen1_Click(object sender, RoutedEventArgs e)
    {
        PerformCollection(1);
    }

    private void CollectGen2_Click(object sender, RoutedEventArgs e)
    {
        PerformCollection(2);
    }

    private void FullCollect_Click(object sender, RoutedEventArgs e)
    {
        PerformCollection(2, compact: true);
    }

    private void PerformCollection(int generation, bool compact = false)
    {
        var beforeMemory = GC.GetTotalMemory(false);
        var beforeGen0 = GC.CollectionCount(0);
        var beforeGen1 = GC.CollectionCount(1);
        var beforeGen2 = GC.CollectionCount(2);

        BeforeCollectionMemory.Text = $"Memory: {beforeMemory / (1024.0 * 1024.0):F2} MB";

        if (compact)
        {
            GC.Collect(generation, GCCollectionMode.Aggressive, blocking: true, compacting: true);
        }
        else
        {
            GC.Collect(generation);
        }

        GC.WaitForPendingFinalizers();

        var afterMemory = GC.GetTotalMemory(false);
        var afterGen0 = GC.CollectionCount(0);
        var afterGen1 = GC.CollectionCount(1);
        var afterGen2 = GC.CollectionCount(2);

        AfterCollectionMemory.Text = $"Memory: {afterMemory / (1024.0 * 1024.0):F2} MB (Freed: {(beforeMemory - afterMemory) / (1024.0 * 1024.0):F2} MB)";

        CollectionStats.Text = $"Gen0: {afterGen0 - beforeGen0}, Gen1: {afterGen1 - beforeGen1}, Gen2: {afterGen2 - beforeGen2}";
    }

    private void AllocateLargeMemory_Click(object sender, RoutedEventArgs e)
    {
        AllocationStatus.Text = "Status: Allocating 100 MB...";

        var task = Task.Run(() =>
        {
            var data = new List<byte[]>();
            for (int i = 0; i < 100; i++)
            {
                data.Add(new byte[1024 * 1024]); // 1 MB
                Thread.Sleep(50);
            }

            Dispatcher.Invoke(() =>
            {
                AllocationStatus.Text = "Status: Allocated 100 MB";
                UpdateAutoCollectionCount();
            });
        });
    }

    private void AllocateManySmall_Click(object sender, RoutedEventArgs e)
    {
        AllocationStatus.Text = "Status: Allocating many small objects...";

        var task = Task.Run(() =>
        {
            for (int i = 0; i < 1000000; i++)
            {
                var obj = new object();
                if (i % 10000 == 0)
                {
                    Thread.Sleep(10);
                }
            }

            Dispatcher.Invoke(() =>
            {
                AllocationStatus.Text = "Status: Allocated 1,000,000 objects";
                UpdateAutoCollectionCount();
            });
        });
    }

    private void StopAllocation_Click(object sender, RoutedEventArgs e)
    {
        AllocationStatus.Text = "Status: Idle";
    }

    private void UpdateAutoCollectionCount()
    {
        var currentGen0 = GC.CollectionCount(0);
        var currentGen1 = GC.CollectionCount(1);
        var currentGen2 = GC.CollectionCount(2);

        AutoCollectionCount.Text = $"Auto-collections detected - Gen0: {currentGen0}, Gen1: {currentGen1}, Gen2: {currentGen2}";
    }

    #endregion

    #region Allocation Patterns Tab

    private void UseObjectPool_Click(object sender, RoutedEventArgs e)
    {
        // Rent object from pool or create new
        for (int i = 0; i < 100; i++)
        {
            ReusableObject obj;
            if (_objectPool.Count > 0)
            {
                obj = _objectPool.Pop();
                obj.Reset();
            }
            else
            {
                obj = new ReusableObject();
            }

            // Use object...
            obj.DoWork();

            // Return to pool
            _objectPool.Push(obj);
        }

        _poolUsageCount += 100;
        PoolStats.Text = $"Pool: {_objectPool.Count} objects (Reused {_poolUsageCount} times)";
    }

    private void ExcessiveAllocations_Click(object sender, RoutedEventArgs e)
    {
        // Bad pattern: creating many short-lived objects
        for (int i = 0; i < 100000; i++)
        {
            var obj = new DisposableObject();
            obj.DoWork();
            // Object immediately eligible for collection
        }

        _excessiveAllocationCount += 100000;
        ExcessiveStats.Text = $"Objects: {_excessiveAllocationCount} (High GC pressure!)";

        // Force update to show GC impact
        UpdateLiveStatistics();
    }

    private void UseArrayPool_Click(object sender, RoutedEventArgs e)
    {
        // Good pattern: using ArrayPool to avoid allocations
        for (int i = 0; i < 100; i++)
        {
            var buffer = ArrayPool<byte>.Shared.Rent(1024);

            // Use buffer...
            Array.Fill(buffer, (byte)42);

            // Return to pool
            ArrayPool<byte>.Shared.Return(buffer);
        }

        _arrayPoolUsageCount += 100;
        ArrayPoolStats.Text = $"Arrays: {_arrayPoolUsageCount} (Zero allocations!)";
    }

    #endregion

    #region Finalization Tab

    private void CreateWithFinalizer_Click(object sender, RoutedEventArgs e)
    {
        for (int i = 0; i < 10; i++)
        {
            _ = new FinalizableObject();
        }

        FinalizerStats.Text = $"Created 10 finalizable objects. Total created: {_finalizableObjectCount}\n" +
                              $"Note: Objects require 2 GC cycles to be fully collected.";
    }

    private void CreateWithDispose_Click(object sender, RoutedEventArgs e)
    {
        for (int i = 0; i < 10; i++)
        {
            using var obj = new DisposableObject();
            obj.DoWork();
        }

        FinalizerStats.Text = "Created and properly disposed 10 objects.\n" +
                              "These are collected in a single GC cycle (more efficient).";
    }

    private void ForceFinalization_Click(object sender, RoutedEventArgs e)
    {
        GC.Collect();
        GC.WaitForPendingFinalizers();
        GC.Collect();

        FinalizerStats.Text = "Forced finalization of all pending objects.\n" +
                              "Check the output window to see finalizers executing.";
    }

    #endregion

    #region Memory Pressure Tab

    private void AddMemoryPressure_Click(object sender, RoutedEventArgs e)
    {
        long pressure = 10 * 1024 * 1024; // 10 MB
        GC.AddMemoryPressure(pressure);
        _currentMemoryPressure += pressure;

        UpdateMemoryPressureStats();

        MessageBox.Show(
            "Added 10 MB of memory pressure.\n\n" +
            "This informs the GC that we're holding 10 MB of unmanaged memory,\n" +
            "which may trigger collections sooner.",
            "Memory Pressure",
            MessageBoxButton.OK,
            MessageBoxImage.Information);
    }

    private void RemoveMemoryPressure_Click(object sender, RoutedEventArgs e)
    {
        if (_currentMemoryPressure > 0)
        {
            GC.RemoveMemoryPressure(_currentMemoryPressure);
            _currentMemoryPressure = 0;
            UpdateMemoryPressureStats();

            MessageBox.Show(
                "Removed all memory pressure.\n\n" +
                "GC now knows we've released the unmanaged memory.",
                "Memory Pressure",
                MessageBoxButton.OK,
                MessageBoxImage.Information);
        }
    }

    private void UpdateMemoryPressureStats()
    {
        MemoryPressureAmount.Text = $"{_currentMemoryPressure / (1024.0 * 1024.0):F2} MB";

        var currentCollections = GC.CollectionCount(0) + GC.CollectionCount(1) + GC.CollectionCount(2);
        PressureCollections.Text = $"Collections triggered: {currentCollections}";
    }

    #endregion

    #region GC Notifications Tab

    private void EnableGCNotifications_Click(object sender, RoutedEventArgs e)
    {
        if (_gcNotificationsEnabled)
        {
            MessageBox.Show("GC notifications are already enabled.", "Info", MessageBoxButton.OK, MessageBoxImage.Information);
            return;
        }

        try
        {
            // Register for full GC notifications
            GC.RegisterForFullGCNotification(10, 10);
            _gcNotificationsEnabled = true;

            _notificationCts = new CancellationTokenSource();
            Task.Run(() => MonitorGCNotifications(_notificationCts.Token));

            LogNotification("GC notifications enabled. Monitoring for full collections...");
        }
        catch (Exception ex)
        {
            MessageBox.Show($"Error enabling notifications: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
        }
    }

    private void DisableGCNotifications_Click(object sender, RoutedEventArgs e)
    {
        if (!_gcNotificationsEnabled)
        {
            MessageBox.Show("GC notifications are not enabled.", "Info", MessageBoxButton.OK, MessageBoxImage.Information);
            return;
        }

        _notificationCts?.Cancel();
        GC.CancelFullGCNotification();
        _gcNotificationsEnabled = false;

        LogNotification("GC notifications disabled.");
    }

    private void ClearNotificationLog_Click(object sender, RoutedEventArgs e)
    {
        Dispatcher.Invoke(() => NotificationLog.Text = "Log cleared.\n");
    }

    private void MonitorGCNotifications(CancellationToken cancellationToken)
    {
        while (!cancellationToken.IsCancellationRequested)
        {
            try
            {
                var status = GC.WaitForFullGCApproach(1000);

                if (status == GCNotificationStatus.Succeeded)
                {
                    LogNotification($"[{DateTime.Now:HH:mm:ss}] Full GC is approaching!");

                    // Wait for the GC to complete
                    status = GC.WaitForFullGCComplete(5000);

                    if (status == GCNotificationStatus.Succeeded)
                    {
                        LogNotification($"[{DateTime.Now:HH:mm:ss}] Full GC completed.");
                    }
                }
            }
            catch (Exception)
            {
                break;
            }
        }
    }

    private void LogNotification(string message)
    {
        Dispatcher.Invoke(() =>
        {
            NotificationLog.Text += message + "\n";
        });
    }

    #endregion

    #region Live Statistics

    private void RefreshStats_Click(object sender, RoutedEventArgs e)
    {
        UpdateLiveStatistics();
    }

    private void ResetCounters_Click(object sender, RoutedEventArgs e)
    {
        _lastGen0Collections = GC.CollectionCount(0);
        _lastGen1Collections = GC.CollectionCount(1);
        _lastGen2Collections = GC.CollectionCount(2);
        _lastTotalMemory = GC.GetTotalMemory(false);
        _allocationStopwatch.Restart();
    }

    private void UpdateLiveStatistics()
    {
        // Memory stats
        var totalMemory = GC.GetTotalMemory(false);
        TotalMemory.Text = $"Total: {totalMemory / (1024.0 * 1024.0):F2} MB";

        var gcInfo = GC.GetGCMemoryInfo();
        HeapSize.Text = $"Heap: {gcInfo.HeapSizeBytes / (1024.0 * 1024.0):F2} MB";
        FragmentedBytes.Text = $"Fragmented: {gcInfo.FragmentedBytes / (1024.0 * 1024.0):F2} MB";

        // Collection counts
        var gen0Count = GC.CollectionCount(0);
        var gen1Count = GC.CollectionCount(1);
        var gen2Count = GC.CollectionCount(2);

        LiveGen0Count.Text = gen0Count.ToString();
        LiveGen1Count.Text = gen1Count.ToString();
        LiveGen2Count.Text = gen2Count.ToString();

        // Performance stats
        var totalTime = gcInfo.PauseDurations.Length > 0 ? gcInfo.PauseDurations.Sum(d => d.TotalMilliseconds) : 0;
        GCTimePercent.Text = $"GC Time: {gcInfo.PauseTimePercentage:F2}%";

        var lastPause = gcInfo.PauseDurations.Length > 0 ? gcInfo.PauseDurations[^1].TotalMilliseconds : 0;
        PauseDuration.Text = $"Last Pause: {lastPause:F2} ms";

        // Calculate allocation rate
        if (_allocationStopwatch.IsRunning && _allocationStopwatch.Elapsed.TotalSeconds > 0)
        {
            var allocatedBytes = totalMemory - _lastTotalMemory;
            var rate = allocatedBytes / _allocationStopwatch.Elapsed.TotalSeconds;
            AllocationRate.Text = $"Allocation: {rate / (1024.0 * 1024.0):F2} MB/s";
        }
        else
        {
            _allocationStopwatch.Start();
            _lastTotalMemory = totalMemory;
        }
    }

    #endregion

    #region Window Lifecycle

    protected override void OnClosing(CancelEventArgs e)
    {
        _statsTimer.Stop();
        _notificationCts?.Cancel();

        if (_gcNotificationsEnabled)
        {
            GC.CancelFullGCNotification();
        }

        if (_currentMemoryPressure > 0)
        {
            GC.RemoveMemoryPressure(_currentMemoryPressure);
        }

        base.OnClosing(e);
    }

    #endregion
}

#region Helper Classes

public class ReusableObject
{
    private byte[]? _data;

    public ReusableObject()
    {
        _data = new byte[1024]; // 1 KB
    }

    public void DoWork()
    {
        // Simulate work
        if (_data != null)
        {
            Array.Fill(_data, (byte)42);
        }
    }

    public void Reset()
    {
        if (_data != null)
        {
            Array.Clear(_data);
        }
    }
}

public class DisposableObject : IDisposable
{
    private byte[]? _data;
    private bool _disposed = false;

    public DisposableObject()
    {
        _data = new byte[1024]; // 1 KB
    }

    public void DoWork()
    {
        if (_disposed)
            throw new ObjectDisposedException(nameof(DisposableObject));

        if (_data != null)
        {
            Array.Fill(_data, (byte)42);
        }
    }

    public void Dispose()
    {
        if (!_disposed)
        {
            _data = null;
            _disposed = true;
        }
    }
}

public class FinalizableObject
{
    private byte[]? _data;

    public FinalizableObject()
    {
        _data = new byte[10 * 1024]; // 10 KB
        Interlocked.Increment(ref MainWindow._finalizableObjectCount);
    }

    ~FinalizableObject()
    {
        // Finalizer runs on dedicated thread
        Debug.WriteLine($"Finalizer executing for object at {DateTime.Now:HH:mm:ss.fff}");
        _data = null;
    }
}

#endregion
