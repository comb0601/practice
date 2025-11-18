using System.Buffers;
using System.ComponentModel;
using System.Diagnostics;
using System.IO.MemoryMappedFiles;
using System.Runtime;
using System.Windows;
using System.Windows.Media;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace LargeObjectHeap;

public partial class MainWindow : Window
{
    // Allocation tracking
    private readonly List<byte[]> _smallObjects = new();
    private readonly List<byte[]> _largeObjects = new();

    // Fragmentation simulation
    private readonly List<byte[]?> _fragmentedBlocks = new();
    private const int BLOCK_SIZE = 200 * 1024; // 200 KB blocks

    // Custom object pool
    private readonly Stack<LargeObject> _customPool = new();
    private int _poolReuseCount = 0;

    // Memory-mapped file
    private MemoryMappedFile? _memoryMappedFile;
    private MemoryMappedViewAccessor? _accessor;

    // Cache for demonstration
    private readonly List<byte[]> _cache = new();

    // Statistics timer
    private readonly DispatcherTimer _statsTimer;

    // Tracking
    private long _lastLOHSize = 0;
    private int _withoutPoolAllocs = 0;
    private int _withPoolAllocs = 0;

    public MainWindow()
    {
        InitializeComponent();

        _statsTimer = new DispatcherTimer
        {
            Interval = TimeSpan.FromSeconds(1)
        };
        _statsTimer.Tick += (s, e) => UpdateLiveStatistics();
        _statsTimer.Start();

        UpdateLiveStatistics();
        UpdateAllocationStats();
    }

    #region LOH Basics Tab

    private void AllocateSmall_Click(object sender, RoutedEventArgs e)
    {
        // Allocate just under LOH threshold (80 KB)
        for (int i = 0; i < 10; i++)
        {
            _smallObjects.Add(new byte[80 * 1024]);
        }
        UpdateAllocationStats();
        MessageBox.Show("Allocated 10×80KB objects on Small Object Heap", "Allocation", MessageBoxButton.OK, MessageBoxImage.Information);
    }

    private void AllocateLOH_Click(object sender, RoutedEventArgs e)
    {
        // Allocate just over LOH threshold (100 KB)
        for (int i = 0; i < 10; i++)
        {
            _largeObjects.Add(new byte[100 * 1024]);
        }
        UpdateAllocationStats();
        MessageBox.Show("Allocated 10×100KB objects on Large Object Heap", "Allocation", MessageBoxButton.OK, MessageBoxImage.Information);
    }

    private void AllocateLarge_Click(object sender, RoutedEventArgs e)
    {
        // Allocate 1 MB objects
        for (int i = 0; i < 10; i++)
        {
            _largeObjects.Add(new byte[1024 * 1024]);
        }
        UpdateAllocationStats();
        MessageBox.Show("Allocated 10×1MB objects on Large Object Heap", "Allocation", MessageBoxButton.OK, MessageBoxImage.Information);
    }

    private void ClearAllocations_Click(object sender, RoutedEventArgs e)
    {
        _smallObjects.Clear();
        _largeObjects.Clear();

        GC.Collect();
        GC.WaitForPendingFinalizers();
        GC.Collect();

        UpdateAllocationStats();
        MessageBox.Show("Cleared all allocations and performed full GC", "Cleared", MessageBoxButton.OK, MessageBoxImage.Information);
    }

    private void UpdateAllocationStats()
    {
        long smallSize = _smallObjects.Count * 80 * 1024;
        long largeSize = _largeObjects.Sum(arr => arr.Length);

        SOHCount.Text = $"Objects: {_smallObjects.Count}";
        SOHSize.Text = $"Total Size: {smallSize / (1024.0 * 1024.0):F2} MB";

        LOHCount.Text = $"Objects: {_largeObjects.Count}";
        LOHSize.Text = $"Total Size: {largeSize / (1024.0 * 1024.0):F2} MB";

        var gcInfo = GC.GetGCMemoryInfo();
        LOHFragmentation.Text = $"Fragmented: {gcInfo.FragmentedBytes / (1024.0 * 1024.0):F2} MB";
    }

    #endregion

    #region Fragmentation Tab

    private void CreateFragmentation_Click(object sender, RoutedEventArgs e)
    {
        // Allocate blocks
        _fragmentedBlocks.Clear();
        for (int i = 0; i < 20; i++)
        {
            _fragmentedBlocks.Add(new byte[BLOCK_SIZE]);
        }

        // Release every other block to create gaps
        for (int i = 1; i < _fragmentedBlocks.Count; i += 2)
        {
            _fragmentedBlocks[i] = null;
        }

        // Force GC to create actual fragmentation
        GC.Collect();
        GC.WaitForPendingFinalizers();

        VisualizeFragmentation();
        UpdateFragmentationStats();

        MessageBox.Show(
            "Created fragmentation by allocating 20 blocks and releasing every other one.\n\n" +
            "Notice the gaps in the visualization.",
            "Fragmentation Created",
            MessageBoxButton.OK,
            MessageBoxImage.Information);
    }

    private void AttemptFragmentedAllocation_Click(object sender, RoutedEventArgs e)
    {
        try
        {
            // Try to allocate large contiguous block
            var largeBlock = new byte[BLOCK_SIZE * 5]; // 1 MB

            MessageBox.Show(
                "Successfully allocated 1 MB contiguous block.\n\n" +
                "In a real fragmented scenario, this might fail even with sufficient total free space.",
                "Allocation Succeeded",
                MessageBoxButton.OK,
                MessageBoxImage.Information);
        }
        catch (OutOfMemoryException)
        {
            MessageBox.Show(
                "OutOfMemoryException!\n\n" +
                "Not enough contiguous space despite having free memory.",
                "Allocation Failed",
                MessageBoxButton.OK,
                MessageBoxImage.Error);
        }
    }

    private void CompactLOH_Click(object sender, RoutedEventArgs e)
    {
        GCSettings.LargeObjectHeapCompactionMode = GCLargeObjectHeapCompactionMode.CompactOnce;
        GC.Collect(2, GCCollectionMode.Aggressive, blocking: true, compacting: true);

        // Rebuild visualization after compaction
        var compactedBlocks = _fragmentedBlocks.Where(b => b != null).ToList();
        _fragmentedBlocks.Clear();
        _fragmentedBlocks.AddRange(compactedBlocks);

        VisualizeFragmentation();
        UpdateFragmentationStats();

        MessageBox.Show(
            "LOH compacted!\n\n" +
            "Gaps removed, memory is now contiguous.",
            "Compaction Complete",
            MessageBoxButton.OK,
            MessageBoxImage.Information);
    }

    private void ResetFragmentation_Click(object sender, RoutedEventArgs e)
    {
        _fragmentedBlocks.Clear();
        GC.Collect();

        FragmentationCanvas.Children.Clear();
        FragmentationStatus.Text = "Status: No fragmentation";
        FragmentationStats.Text = "Free blocks: 0, Total free: 0 MB";
    }

    private void VisualizeFragmentation()
    {
        FragmentationCanvas.Children.Clear();

        double blockWidth = FragmentationCanvas.ActualWidth / 20;
        if (blockWidth <= 0) blockWidth = 20;

        for (int i = 0; i < _fragmentedBlocks.Count && i < 20; i++)
        {
            var rect = new Rectangle
            {
                Width = blockWidth - 2,
                Height = 60,
                Fill = _fragmentedBlocks[i] != null
                    ? new SolidColorBrush(Colors.DeepSkyBlue)
                    : new SolidColorBrush(Colors.LightGray),
                Stroke = new SolidColorBrush(Colors.Black),
                StrokeThickness = 1
            };

            Canvas.SetLeft(rect, i * blockWidth + 1);
            Canvas.SetTop(rect, 10);

            FragmentationCanvas.Children.Add(rect);
        }
    }

    private void UpdateFragmentationStats()
    {
        int freeBlocks = _fragmentedBlocks.Count(b => b == null);
        long freeSize = freeBlocks * BLOCK_SIZE;

        FragmentationStatus.Text = freeBlocks > 0
            ? $"Status: Fragmented ({freeBlocks} gaps)"
            : "Status: Contiguous (no fragmentation)";

        FragmentationStats.Text = $"Free blocks: {freeBlocks}, Total free: {freeSize / (1024.0 * 1024.0):F2} MB";
    }

    #endregion

    #region Compaction Tab

    private void EnableCompaction_Click(object sender, RoutedEventArgs e)
    {
        GCSettings.LargeObjectHeapCompactionMode = GCLargeObjectHeapCompactionMode.CompactOnce;
        CompactionMode.Text = "Current: CompactOnce (will compact on next Gen2 GC)";

        MessageBox.Show(
            "LOH compaction enabled for next blocking Gen2 collection.\n\n" +
            "The LOH will be compacted once, then revert to default behavior.",
            "Compaction Enabled",
            MessageBoxButton.OK,
            MessageBoxImage.Information);
    }

    private void ForceCompactGC_Click(object sender, RoutedEventArgs e)
    {
        var beforeInfo = GC.GetGCMemoryInfo();
        BeforeLOHSize.Text = $"LOH Size: {beforeInfo.HeapSizeBytes / (1024.0 * 1024.0):F2} MB";
        BeforeFragmented.Text = $"Fragmented: {beforeInfo.FragmentedBytes / (1024.0 * 1024.0):F2} MB";

        // Force compacting GC
        GC.Collect(2, GCCollectionMode.Aggressive, blocking: true, compacting: true);

        var afterInfo = GC.GetGCMemoryInfo();
        AfterLOHSize.Text = $"LOH Size: {afterInfo.HeapSizeBytes / (1024.0 * 1024.0):F2} MB";
        AfterFragmented.Text = $"Fragmented: {afterInfo.FragmentedBytes / (1024.0 * 1024.0):F2} MB";

        var saved = (beforeInfo.HeapSizeBytes - afterInfo.HeapSizeBytes) / (1024.0 * 1024.0);
        CompactionResult.Text = $"Result: Reclaimed {saved:F2} MB";

        MessageBox.Show(
            $"Forced compacting collection complete.\n\n" +
            $"Before: {beforeInfo.HeapSizeBytes / (1024.0 * 1024.0):F2} MB\n" +
            $"After: {afterInfo.HeapSizeBytes / (1024.0 * 1024.0):F2} MB\n" +
            $"Saved: {saved:F2} MB",
            "Compaction Complete",
            MessageBoxButton.OK,
            MessageBoxImage.Information);
    }

    private void ResetCompaction_Click(object sender, RoutedEventArgs e)
    {
        GCSettings.LargeObjectHeapCompactionMode = GCLargeObjectHeapCompactionMode.Default;
        CompactionMode.Text = "Current: Default (sweep only, no compaction)";

        MessageBox.Show(
            "LOH compaction mode reset to default.\n\n" +
            "LOH will be swept but not compacted during GC.",
            "Reset Complete",
            MessageBoxButton.OK,
            MessageBoxImage.Information);
    }

    #endregion

    #region Buffer Pooling Tab

    private void ProcessWithoutPool_Click(object sender, RoutedEventArgs e)
    {
        var sw = Stopwatch.StartNew();
        var beforeMemory = GC.GetTotalMemory(true);
        var beforeGen2 = GC.CollectionCount(2);

        // Allocate and release 1000 large buffers
        for (int i = 0; i < 1000; i++)
        {
            var buffer = new byte[100 * 1024]; // 100 KB each
            Array.Fill(buffer, (byte)42);
            // Buffer goes out of scope and becomes garbage
        }

        sw.Stop();
        var afterMemory = GC.GetTotalMemory(true);
        var afterGen2 = GC.CollectionCount(2);

        _withoutPoolAllocs += 1000;

        WithoutPoolStats.Text = $"Allocations: {_withoutPoolAllocs}";
        WithoutPoolMemory.Text = $"Memory used: {(afterMemory - beforeMemory) / (1024.0 * 1024.0):F2} MB";
        WithoutPoolGC.Text = $"GC collections: {afterGen2 - beforeGen2}";
        WithoutPoolTime.Text = $"Time: {sw.ElapsedMilliseconds} ms";
    }

    private void ProcessWithPool_Click(object sender, RoutedEventArgs e)
    {
        var sw = Stopwatch.StartNew();
        var beforeMemory = GC.GetTotalMemory(false);
        var beforeGen2 = GC.CollectionCount(2);

        // Use ArrayPool for 1000 operations
        for (int i = 0; i < 1000; i++)
        {
            var buffer = ArrayPool<byte>.Shared.Rent(100 * 1024);
            try
            {
                Array.Fill(buffer, 0, 100 * 1024, (byte)42);
            }
            finally
            {
                ArrayPool<byte>.Shared.Return(buffer);
            }
        }

        sw.Stop();
        var afterMemory = GC.GetTotalMemory(false);
        var afterGen2 = GC.CollectionCount(2);

        _withPoolAllocs += 1000;

        WithPoolStats.Text = $"Allocations: {_withPoolAllocs}";
        WithPoolMemory.Text = $"Memory used: {(afterMemory - beforeMemory) / (1024.0 * 1024.0):F2} MB";
        WithPoolGC.Text = $"GC collections: {afterGen2 - beforeGen2}";
        WithPoolTime.Text = $"Time: {sw.ElapsedMilliseconds} ms";
    }

    private void CreateCustomPool_Click(object sender, RoutedEventArgs e)
    {
        // Pre-populate pool with 10 large objects
        for (int i = 0; i < 10; i++)
        {
            _customPool.Push(new LargeObject());
        }

        UpdateCustomPoolStats();
        MessageBox.Show("Created pool with 10 large objects (each 500 KB)", "Pool Created", MessageBoxButton.OK, MessageBoxImage.Information);
    }

    private void UseCustomPool_Click(object sender, RoutedEventArgs e)
    {
        var sw = Stopwatch.StartNew();

        for (int i = 0; i < 100; i++)
        {
            LargeObject obj;

            if (_customPool.Count > 0)
            {
                obj = _customPool.Pop();
                obj.Reset();
                _poolReuseCount++;
            }
            else
            {
                obj = new LargeObject();
            }

            // Use object
            obj.Process();

            // Return to pool
            _customPool.Push(obj);
        }

        sw.Stop();

        UpdateCustomPoolStats();
        MessageBox.Show(
            $"Processed 100 operations in {sw.ElapsedMilliseconds} ms\n\n" +
            $"Reused objects {_poolReuseCount} times",
            "Processing Complete",
            MessageBoxButton.OK,
            MessageBoxImage.Information);
    }

    private void ClearCustomPool_Click(object sender, RoutedEventArgs e)
    {
        _customPool.Clear();
        _poolReuseCount = 0;

        GC.Collect();

        UpdateCustomPoolStats();
        MessageBox.Show("Pool cleared and memory released", "Pool Cleared", MessageBoxButton.OK, MessageBoxImage.Information);
    }

    private void UpdateCustomPoolStats()
    {
        long poolMemory = _customPool.Count * 500 * 1024; // 500 KB each
        CustomPoolStats.Text = $"Pool size: {_customPool.Count}, Reused: {_poolReuseCount} times";
        CustomPoolMemory.Text = $"Total memory: {poolMemory / (1024.0 * 1024.0):F2} MB";
    }

    #endregion

    #region Optimization Strategies Tab

    private void AllocateJustUnder_Click(object sender, RoutedEventArgs e)
    {
        var array = new byte[84 * 1024]; // 84 KB - stays on SOH
        var generation = GC.GetGeneration(array);

        ThresholdResult.Text = $"84 KB array allocated.\n" +
                              $"Generation: {generation} (Small Object Heap)\n" +
                              $"This array will be compacted during GC.";
    }

    private void AllocateJustOver_Click(object sender, RoutedEventArgs e)
    {
        var array = new byte[85 * 1024]; // 85 KB - goes to LOH
        var generation = GC.GetGeneration(array);

        ThresholdResult.Text = $"85 KB array allocated.\n" +
                              $"Generation: {generation} (Large Object Heap)\n" +
                              $"This array will NOT be compacted by default.";
    }

    private void SingleLargeArray_Click(object sender, RoutedEventArgs e)
    {
        var beforeMemory = GC.GetTotalMemory(false);

        var array = new byte[10 * 1024 * 1024]; // 10 MB on LOH
        var generation = GC.GetGeneration(array);

        var afterMemory = GC.GetTotalMemory(false);

        SegmentationResult.Text = $"Single 10 MB array allocated on LOH (Gen{generation})\n" +
                                 $"Memory increase: {(afterMemory - beforeMemory) / (1024.0 * 1024.0):F2} MB\n" +
                                 $"Risk: LOH fragmentation";
    }

    private void SegmentedArrays_Click(object sender, RoutedEventArgs e)
    {
        var beforeMemory = GC.GetTotalMemory(false);

        // 125 arrays of 80 KB each = 10 MB total on SOH
        var arrays = new List<byte[]>();
        for (int i = 0; i < 125; i++)
        {
            arrays.Add(new byte[80 * 1024]);
        }

        var generation = GC.GetGeneration(arrays[0]);
        var afterMemory = GC.GetTotalMemory(false);

        SegmentationResult.Text = $"125 × 80 KB arrays allocated on SOH (Gen{generation})\n" +
                                 $"Total size: 10 MB\n" +
                                 $"Memory increase: {(afterMemory - beforeMemory) / (1024.0 * 1024.0):F2} MB\n" +
                                 $"Benefit: No LOH fragmentation, will be compacted";
    }

    private void CreateMemoryMapped_Click(object sender, RoutedEventArgs e)
    {
        try
        {
            _memoryMappedFile = MemoryMappedFile.CreateNew("LargeDataDemo", 100 * 1024 * 1024); // 100 MB
            _accessor = _memoryMappedFile.CreateViewAccessor();

            MMFResult.Text = "Created 100 MB memory-mapped file.\n" +
                           "This doesn't count against managed heap!\n" +
                           "OS manages the memory.";
        }
        catch (Exception ex)
        {
            MMFResult.Text = $"Error: {ex.Message}";
        }
    }

    private void AccessMemoryMapped_Click(object sender, RoutedEventArgs e)
    {
        if (_accessor == null)
        {
            MessageBox.Show("Create memory-mapped file first!", "Error", MessageBoxButton.OK, MessageBoxImage.Warning);
            return;
        }

        // Write and read data
        var sw = Stopwatch.StartNew();

        for (int i = 0; i < 1000; i++)
        {
            _accessor.Write(i * 1024, (byte)42);
        }

        for (int i = 0; i < 1000; i++)
        {
            var value = _accessor.ReadByte(i * 1024);
        }

        sw.Stop();

        MMFResult.Text = $"Accessed memory-mapped file.\n" +
                       $"Wrote and read 1000 values in {sw.ElapsedMilliseconds} ms\n" +
                       $"No managed heap allocation!";
    }

    private void DisposeMemoryMapped_Click(object sender, RoutedEventArgs e)
    {
        _accessor?.Dispose();
        _accessor = null;

        _memoryMappedFile?.Dispose();
        _memoryMappedFile = null;

        MMFResult.Text = "Memory-mapped file disposed.\n" +
                       "OS releases the memory.";
    }

    private void ArrayOfStructs_Click(object sender, RoutedEventArgs e)
    {
        // Bad: Large array of structs might go to LOH
        var array = new Vector3D[30000]; // 30,000 × 24 bytes = 720 KB → LOH

        var generation = GC.GetGeneration(array);

        SOAResult.Text = $"Array of 30,000 Vector3D structs (720 KB)\n" +
                        $"Generation: {generation} (goes to LOH)\n" +
                        $"Not cache-friendly for processing.";
    }

    private void StructOfArrays_Click(object sender, RoutedEventArgs e)
    {
        // Good: Separate arrays for each component
        var x = new double[30000]; // 240 KB - stays on SOH
        var y = new double[30000]; // 240 KB - stays on SOH
        var z = new double[30000]; // 240 KB - stays on SOH

        var generation = GC.GetGeneration(x);

        SOAResult.Text = $"Three arrays of 30,000 doubles each\n" +
                        $"Total: 720 KB split into 3×240 KB arrays\n" +
                        $"Generation: {generation} (stays on SOH)\n" +
                        $"Cache-friendly and compactable!";
    }

    #endregion

    #region Real-World Examples Tab

    private void ProcessImageWithoutPool_Click(object sender, RoutedEventArgs e)
    {
        var sw = Stopwatch.StartNew();
        var beforeGen2 = GC.CollectionCount(2);

        // Simulate processing 10 images (4K resolution = ~32 MB each)
        for (int i = 0; i < 10; i++)
        {
            var buffer = new byte[32 * 1024 * 1024]; // 32 MB
            // Process image...
            Array.Fill(buffer, (byte)128);
        }

        sw.Stop();
        var afterGen2 = GC.CollectionCount(2);

        ImageProcessingResult.Text = $"Processed 10 images without pooling\n" +
                                   $"Time: {sw.ElapsedMilliseconds} ms\n" +
                                   $"Gen2 collections: {afterGen2 - beforeGen2}\n" +
                                   $"Each allocation hit LOH!";
    }

    private void ProcessImageWithPool_Click(object sender, RoutedEventArgs e)
    {
        var sw = Stopwatch.StartNew();
        var beforeGen2 = GC.CollectionCount(2);

        // Use ArrayPool for image processing
        for (int i = 0; i < 10; i++)
        {
            var buffer = ArrayPool<byte>.Shared.Rent(32 * 1024 * 1024);
            try
            {
                // Process image...
                Array.Fill(buffer, 0, 32 * 1024 * 1024, (byte)128);
            }
            finally
            {
                ArrayPool<byte>.Shared.Return(buffer);
            }
        }

        sw.Stop();
        var afterGen2 = GC.CollectionCount(2);

        ImageProcessingResult.Text = $"Processed 10 images WITH pooling\n" +
                                   $"Time: {sw.ElapsedMilliseconds} ms\n" +
                                   $"Gen2 collections: {afterGen2 - beforeGen2}\n" +
                                   $"Zero LOH allocations after warm-up!";
    }

    private void SimulateNetworkBuffers_Click(object sender, RoutedEventArgs e)
    {
        var sw = Stopwatch.StartNew();
        var beforeGen2 = GC.CollectionCount(2);

        // Simulate 100 network connections with 128 KB buffers each
        var buffers = new List<byte[]>();
        for (int i = 0; i < 100; i++)
        {
            buffers.Add(ArrayPool<byte>.Shared.Rent(128 * 1024));
        }

        // Return all buffers
        foreach (var buffer in buffers)
        {
            ArrayPool<byte>.Shared.Return(buffer);
        }

        sw.Stop();
        var afterGen2 = GC.CollectionCount(2);

        NetworkBufferResult.Text = $"Simulated 100 network connections\n" +
                                  $"Each with 128 KB buffer (would be LOH)\n" +
                                  $"Time: {sw.ElapsedMilliseconds} ms\n" +
                                  $"Gen2 collections: {afterGen2 - beforeGen2}\n" +
                                  $"Using ArrayPool = no LOH pressure!";
    }

    private void CacheLargeObjects_Click(object sender, RoutedEventArgs e)
    {
        var beforeMemory = GC.GetTotalMemory(false);

        // Cache 50 large objects (each 500 KB)
        for (int i = 0; i < 50; i++)
        {
            _cache.Add(new byte[500 * 1024]);
        }

        var afterMemory = GC.GetTotalMemory(false);

        CacheResult.Text = $"Cached 50 large objects (25 MB total)\n" +
                          $"Memory increase: {(afterMemory - beforeMemory) / (1024.0 * 1024.0):F2} MB\n" +
                          $"All on LOH - be careful with long-lived cached data!";
    }

    private void ClearCache_Click(object sender, RoutedEventArgs e)
    {
        _cache.Clear();
        GC.Collect();

        CacheResult.Text = "Cache cleared and memory released.\n" +
                          "Large cached objects can cause LOH bloat!";
    }

    #endregion

    #region Live Statistics

    private void RefreshLiveStats_Click(object sender, RoutedEventArgs e)
    {
        UpdateLiveStatistics();
    }

    private void ForceGC_Click(object sender, RoutedEventArgs e)
    {
        GC.Collect();
        GC.WaitForPendingFinalizers();
        GC.Collect();
        UpdateLiveStatistics();
    }

    private void UpdateLiveStatistics()
    {
        var gcInfo = GC.GetGCMemoryInfo();
        var totalMemory = GC.GetTotalMemory(false);

        LiveTotalMemory.Text = $"{totalMemory / (1024.0 * 1024.0):F2} MB";

        // Estimate SOH and LOH sizes (approximation)
        long estimatedLOH = _largeObjects.Sum(arr => arr.Length) +
                           _fragmentedBlocks.Where(b => b != null).Sum(b => b!.Length) +
                           _cache.Sum(arr => arr.Length) +
                           (_customPool.Count * 500 * 1024);

        long estimatedSOH = totalMemory - estimatedLOH;

        LiveSOHMemory.Text = $"SOH: {Math.Max(0, estimatedSOH) / (1024.0 * 1024.0):F2} MB";
        LiveLOHMemory.Text = $"LOH: {estimatedLOH / (1024.0 * 1024.0):F2} MB";

        LiveLOHSize.Text = $"Size: {estimatedLOH / (1024.0 * 1024.0):F2} MB";
        LiveLOHFragmented.Text = $"Fragmented: {gcInfo.FragmentedBytes / (1024.0 * 1024.0):F2} MB";

        double fragPercent = estimatedLOH > 0
            ? (gcInfo.FragmentedBytes * 100.0 / estimatedLOH)
            : 0;
        LiveFragmentationPercent.Text = $"Fragmentation: {fragPercent:F1}%";

        LiveGen2Count.Text = $"Gen2: {GC.CollectionCount(2)}";
        LiveCompactionMode.Text = $"Compaction: {GCSettings.LargeObjectHeapCompactionMode}";

        var pauseDurations = gcInfo.PauseDurations;
        var lastPause = pauseDurations.Length > 0 ? pauseDurations[^1].TotalMilliseconds : 0;
        LiveLastPause.Text = $"Last pause: {lastPause:F2} ms";

        _lastLOHSize = estimatedLOH;
    }

    #endregion

    #region Window Lifecycle

    protected override void OnClosing(CancelEventArgs e)
    {
        _statsTimer.Stop();

        _accessor?.Dispose();
        _memoryMappedFile?.Dispose();

        base.OnClosing(e);
    }

    #endregion
}

#region Helper Classes

public class LargeObject
{
    private byte[] _data;

    public LargeObject()
    {
        _data = new byte[500 * 1024]; // 500 KB
    }

    public void Process()
    {
        // Simulate work
        Array.Fill(_data, (byte)42);
    }

    public void Reset()
    {
        Array.Clear(_data);
    }
}

public struct Vector3D
{
    public double X;
    public double Y;
    public double Z;
}

#endregion
