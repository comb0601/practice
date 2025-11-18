using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Windows;
using System.Windows.Media;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace ResourceManagement;

public partial class MainWindow : Window
{
    // Disposable objects tracking
    private readonly List<DisposableResource> _disposableResources = new();
    private readonly List<FileStream> _leakyStreams = new();
    private readonly List<AsyncDisposableResource> _asyncResources = new();

    // Freezable tracking
    private readonly List<Brush> _unfrozenBrushes = new();
    private readonly List<Brush> _frozenBrushes = new();
    private int _frozenObjectCount = 0;

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

    #region IDisposable Pattern

    private void CreateDisposableObjects_Click(object sender, RoutedEventArgs e)
    {
        for (int i = 0; i < 10; i++)
        {
            _disposableResources.Add(new DisposableResource($"Resource-{i}"));
        }

        DisposableStatus.Text = $"Created {_disposableResources.Count} disposable objects.\n" +
            $"Active (not disposed): {_disposableResources.Count(r => !r.IsDisposed)}";
    }

    private void DisposeAll_Click(object sender, RoutedEventArgs e)
    {
        foreach (var resource in _disposableResources)
        {
            resource.Dispose();
        }

        DisposableStatus.Text = $"Disposed all resources.\n" +
            $"Active (not disposed): {_disposableResources.Count(r => !r.IsDisposed)}";
    }

    private void CheckDisposableStatus_Click(object sender, RoutedEventArgs e)
    {
        int active = _disposableResources.Count(r => !r.IsDisposed);
        int disposed = _disposableResources.Count(r => r.IsDisposed);

        MessageBox.Show(
            $"Total created: {_disposableResources.Count}\n" +
            $"Active: {active}\n" +
            $"Disposed: {disposed}",
            "Disposable Status",
            MessageBoxButton.OK,
            MessageBoxImage.Information);
    }

    private void WithoutUsing_Click(object sender, RoutedEventArgs e)
    {
        DisposableResource? resource = null;
        try
        {
            resource = new DisposableResource("Manual");
            // Use resource
            resource.DoWork();
        }
        finally
        {
            resource?.Dispose(); // Manual dispose
        }

        UsingResult.Text = "WITHOUT using: Manual Dispose() in finally block.\n" +
            "Prone to errors if developer forgets.";
    }

    private void WithUsing_Click(object sender, RoutedEventArgs e)
    {
        using (var resource = new DisposableResource("Using"))
        {
            // Use resource
            resource.DoWork();
        } // Automatic Dispose()

        UsingResult.Text = "WITH using: Automatic Dispose() when scope exits.\n" +
            "Guaranteed cleanup even if exception occurs!";
    }

    private void UsingDeclaration_Click(object sender, RoutedEventArgs e)
    {
        // C# 8+ using declaration
        using var resource = new DisposableResource("Declaration");

        // Use resource
        resource.DoWork();

        // Disposed at end of enclosing scope

        UsingResult.Text = "USING DECLARATION (C# 8+): Simplified syntax.\n" +
            "Disposed at end of method or block scope.";
    }

    #endregion

    #region Freezable Objects

    private void CreateUnfrozenBrushes_Click(object sender, RoutedEventArgs e)
    {
        var beforeMemory = GC.GetTotalMemory(false);

        for (int i = 0; i < 1000; i++)
        {
            var brush = new LinearGradientBrush(Colors.Red, Colors.Blue, 45);
            // Not frozen - each element gets a copy
            _unfrozenBrushes.Add(brush);
        }

        var afterMemory = GC.GetTotalMemory(false);

        UnfrozenStats.Text = $"Count: {_unfrozenBrushes.Count}\n" +
            $"Memory: {(afterMemory - beforeMemory) / 1024.0:F2} KB";

        UpdateFreezableComparison();
    }

    private void CreateFrozenBrushes_Click(object sender, RoutedEventArgs e)
    {
        var beforeMemory = GC.GetTotalMemory(false);

        // Create one brush and freeze it
        var frozenBrush = new LinearGradientBrush(Colors.Green, Colors.Yellow, 45);
        frozenBrush.Freeze(); // Make immutable

        for (int i = 0; i < 1000; i++)
        {
            // All elements share the same frozen brush
            _frozenBrushes.Add(frozenBrush);
        }

        var afterMemory = GC.GetTotalMemory(false);
        _frozenObjectCount++;

        FrozenStats.Text = $"Count: {_frozenBrushes.Count}\n" +
            $"Memory: {(afterMemory - beforeMemory) / 1024.0:F2} KB\n" +
            $"(All share 1 instance!)";

        UpdateFreezableComparison();
    }

    private void UpdateFreezableComparison()
    {
        if (_unfrozenBrushes.Count > 0 && _frozenBrushes.Count > 0)
        {
            FreezableMemoryComparison.Text =
                $"Unfrozen: 1000 separate instances\n" +
                $"Frozen: 1000 references to 1 shared instance\n" +
                $"Memory saved: ~95% with frozen brushes!";
        }
    }

    private void FreezeBrush_Click(object sender, RoutedEventArgs e)
    {
        var brush = new SolidColorBrush(Colors.Orange);

        FreezeExampleResult.Text = $"Before Freeze:\n" +
            $"  CanFreeze: {brush.CanFreeze}\n" +
            $"  IsFrozen: {brush.IsFrozen}\n";

        brush.Freeze();
        _frozenObjectCount++;

        FreezeExampleResult.Text += $"\nAfter Freeze:\n" +
            $"  IsFrozen: {brush.IsFrozen}\n" +
            $"  Now immutable and thread-safe!";
    }

    private void FreezeGeometry_Click(object sender, RoutedEventArgs e)
    {
        var geometry = new EllipseGeometry(new Point(50, 50), 30, 30);

        FreezeExampleResult.Text = $"EllipseGeometry created.\n";

        if (geometry.CanFreeze)
        {
            geometry.Freeze();
            _frozenObjectCount++;
            FreezeExampleResult.Text += $"Frozen! Can be shared across threads.";
        }
    }

    private void FreezeTransform_Click(object sender, RoutedEventArgs e)
    {
        var transform = new RotateTransform(45);

        FreezeExampleResult.Text = $"RotateTransform (45°) created.\n";

        transform.Freeze();
        _frozenObjectCount++;

        FreezeExampleResult.Text += $"Frozen! Immutable transform that can be reused.";
    }

    #endregion

    #region Resource Dictionaries

    private void LoadHeavyResources_Click(object sender, RoutedEventArgs e)
    {
        var dict = new ResourceDictionary();

        // Add heavy resources
        for (int i = 0; i < 50; i++)
        {
            var brush = new LinearGradientBrush();
            brush.GradientStops.Add(new GradientStop(Colors.Red, 0));
            brush.GradientStops.Add(new GradientStop(Colors.Blue, 1));
            dict.Add($"HeavyBrush{i}", brush);

            var geometry = new EllipseGeometry(new Point(0, 0), 100, 100);
            dict.Add($"HeavyGeometry{i}", geometry);
        }

        Resources.MergedDictionaries.Add(dict);

        ResourceDictStatus.Text = $"Loaded 100 heavy resources into merged dictionary.\n" +
            $"Total merged dictionaries: {Resources.MergedDictionaries.Count}";
    }

    private void UnloadResources_Click(object sender, RoutedEventArgs e)
    {
        Resources.MergedDictionaries.Clear();
        GC.Collect();

        ResourceDictStatus.Text = "Unloaded all merged dictionaries and performed GC.";
    }

    private void CheckResourceCount_Click(object sender, RoutedEventArgs e)
    {
        int totalResources = Resources.Count;
        int mergedCount = Resources.MergedDictionaries.Count;
        int mergedResources = Resources.MergedDictionaries.Sum(d => d.Count);

        MessageBox.Show(
            $"Window Resources: {totalResources}\n" +
            $"Merged Dictionaries: {mergedCount}\n" +
            $"Resources in Merged: {mergedResources}\n" +
            $"Total: {totalResources + mergedResources}",
            "Resource Count",
            MessageBoxButton.OK,
            MessageBoxImage.Information);
    }

    private void MergeDictionary_Click(object sender, RoutedEventArgs e)
    {
        var dict = new ResourceDictionary();

        for (int i = 0; i < 10; i++)
        {
            dict.Add($"DynamicBrush{i}", new SolidColorBrush(Colors.Purple));
        }

        Resources.MergedDictionaries.Add(dict);

        MergedDictStatus.Text = $"Merged dictionaries: {Resources.MergedDictionaries.Count}";
    }

    private void ClearMergedDictionaries_Click(object sender, RoutedEventArgs e)
    {
        Resources.MergedDictionaries.Clear();

        MergedDictStatus.Text = "Merged dictionaries: 0 (cleared)";
    }

    #endregion

    #region Stream Management

    private void OpenStreamsLeaky_Click(object sender, RoutedEventArgs e)
    {
        try
        {
            // Create temp files and open streams WITHOUT disposing
            for (int i = 0; i < 5; i++)
            {
                string tempFile = Path.GetTempFileName();
                var stream = File.OpenWrite(tempFile);
                stream.WriteByte(42);
                // NOT DISPOSED - LEAK!
                _leakyStreams.Add(stream);
            }

            StreamHandleStatus.Text = $"Opened {_leakyStreams.Count} streams WITHOUT disposing.\n" +
                "File handles remain open - this is a LEAK!";
        }
        catch (Exception ex)
        {
            StreamHandleStatus.Text = $"Error: {ex.Message}";
        }
    }

    private void OpenStreamsProper_Click(object sender, RoutedEventArgs e)
    {
        try
        {
            int streamsOpened = 0;

            // Properly dispose streams
            for (int i = 0; i < 5; i++)
            {
                string tempFile = Path.GetTempFileName();

                using (var stream = File.OpenWrite(tempFile))
                {
                    stream.WriteByte(42);
                    streamsOpened++;
                } // Automatically disposed!

                File.Delete(tempFile);
            }

            StreamHandleStatus.Text = $"Opened and properly disposed {streamsOpened} streams.\n" +
                "All file handles released immediately!";
        }
        catch (Exception ex)
        {
            StreamHandleStatus.Text = $"Error: {ex.Message}";
        }
    }

    private void CheckOpenHandles_Click(object sender, RoutedEventArgs e)
    {
        // Clean up leaked streams
        foreach (var stream in _leakyStreams)
        {
            stream.Dispose();
        }
        _leakyStreams.Clear();

        StreamHandleStatus.Text = "Cleaned up leaked streams.\n" +
            "Always use 'using' statements for streams!";
    }

    #endregion

    #region Async Disposal

    private void CreateAsyncResources_Click(object sender, RoutedEventArgs e)
    {
        for (int i = 0; i < 5; i++)
        {
            _asyncResources.Add(new AsyncDisposableResource($"AsyncResource-{i}"));
        }

        AsyncDisposalStatus.Text = $"Created {_asyncResources.Count} async disposable resources.\n" +
            "These require async disposal.";
    }

    private async void DisposeAsync_Click(object sender, RoutedEventArgs e)
    {
        int disposed = 0;

        foreach (var resource in _asyncResources)
        {
            await resource.DisposeAsync();
            disposed++;
        }

        _asyncResources.Clear();

        AsyncDisposalStatus.Text = $"Disposed {disposed} resources asynchronously.\n" +
            "Async disposal allows proper cleanup of async operations.";
    }

    #endregion

    #region Best Practices

    private async void RunBenchmark_Click(object sender, RoutedEventArgs e)
    {
        BenchmarkResults.Text = "Running benchmark...";

        await Task.Run(() =>
        {
            var sw = Stopwatch.StartNew();

            // Test 1: Unfrozen vs Frozen
            var unfrozenTime = TimeSpan.Zero;
            var frozenTime = TimeSpan.Zero;

            // Unfrozen
            sw.Restart();
            var unfrozenList = new List<Brush>();
            for (int i = 0; i < 10000; i++)
            {
                unfrozenList.Add(new SolidColorBrush(Colors.Red));
            }
            unfrozenTime = sw.Elapsed;

            // Frozen
            sw.Restart();
            var frozenBrush = new SolidColorBrush(Colors.Red);
            frozenBrush.Freeze();
            var frozenList = new List<Brush>();
            for (int i = 0; i < 10000; i++)
            {
                frozenList.Add(frozenBrush);
            }
            frozenTime = sw.Elapsed;

            Dispatcher.Invoke(() =>
            {
                BenchmarkResults.Text =
                    $"Created 10,000 brushes:\n" +
                    $"  Unfrozen (10,000 instances): {unfrozenTime.TotalMilliseconds:F2} ms\n" +
                    $"  Frozen (1 shared instance): {frozenTime.TotalMilliseconds:F2} ms\n" +
                    $"  Speedup: {unfrozenTime.TotalMilliseconds / frozenTime.TotalMilliseconds:F1}x\n\n" +
                    $"Memory savings and performance gain from freezing!";
            });
        });
    }

    #endregion

    #region Common Methods

    private void ForceGC_Click(object sender, RoutedEventArgs e)
    {
        GC.Collect();
        GC.WaitForPendingFinalizers();
        GC.Collect();

        UpdateStatistics();
    }

    private void UpdateStats_Click(object sender, RoutedEventArgs e)
    {
        UpdateStatistics();
    }

    private void UpdateStatistics()
    {
        var memory = GC.GetTotalMemory(false);
        MemoryUsage.Text = $"{memory / (1024.0 * 1024.0):F2} MB";

        int activeDisposables = _disposableResources.Count(r => !r.IsDisposed) +
                               _asyncResources.Count;
        ActiveDisposables.Text = activeDisposables.ToString();

        FrozenObjectCount.Text = _frozenObjectCount.ToString();
    }

    protected override void OnClosing(CancelEventArgs e)
    {
        _statsTimer.Stop();

        // Clean up all resources
        foreach (var resource in _disposableResources)
        {
            resource.Dispose();
        }

        foreach (var stream in _leakyStreams)
        {
            stream.Dispose();
        }

        base.OnClosing(e);
    }

    #endregion
}

#region Helper Classes

public class DisposableResource : IDisposable
{
    private byte[]? _data;
    private bool _disposed = false;

    public string Name { get; }
    public bool IsDisposed => _disposed;

    public DisposableResource(string name)
    {
        Name = name;
        _data = new byte[1024 * 10]; // 10 KB
        Debug.WriteLine($"{Name} created");
    }

    public void DoWork()
    {
        if (_disposed)
            throw new ObjectDisposedException(nameof(DisposableResource));

        if (_data != null)
        {
            Array.Fill(_data, (byte)42);
        }
    }

    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    protected virtual void Dispose(bool disposing)
    {
        if (!_disposed)
        {
            if (disposing)
            {
                // Dispose managed resources
                _data = null;
            }

            // Free unmanaged resources (if any)

            _disposed = true;
            Debug.WriteLine($"{Name} disposed");
        }
    }

    ~DisposableResource()
    {
        Dispose(false);
        Debug.WriteLine($"{Name} finalized");
    }
}

public class AsyncDisposableResource : IAsyncDisposable
{
    private byte[]? _data;
    private bool _disposed = false;

    public string Name { get; }

    public AsyncDisposableResource(string name)
    {
        Name = name;
        _data = new byte[1024 * 10]; // 10 KB
    }

    public async ValueTask DisposeAsync()
    {
        if (!_disposed)
        {
            // Perform async cleanup
            await Task.Delay(10); // Simulate async cleanup

            _data = null;
            _disposed = true;

            Debug.WriteLine($"{Name} disposed asynchronously");

            GC.SuppressFinalize(this);
        }
    }
}

#endregion
