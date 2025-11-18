using System.Diagnostics;
using System.IO;
using System.Net.Http;
using System.Windows;
using System.Windows.Controls;

namespace AsyncPatterns;

public partial class MainWindow : Window
{
    private CancellationTokenSource? _cancellationTokenSource;
    private readonly HttpClient _httpClient = new();
    private readonly SemaphoreSlim _throttleSemaphore = new(3);

    public MainWindow()
    {
        InitializeComponent();
    }

    #region Basic Async/Await

    private void OnBlockingOperation(object sender, RoutedEventArgs e)
    {
        AsyncStatus.Text = "Running blocking operation... (UI will freeze!)";
        AsyncProgress.IsIndeterminate = true;

        // BAD: Blocking the UI thread
        Thread.Sleep(3000);

        AsyncProgress.IsIndeterminate = false;
        AsyncStatus.Text = "Blocking operation completed (notice the UI freeze)";
    }

    private async void OnAsyncOperation(object sender, RoutedEventArgs e)
    {
        AsyncStatus.Text = "Running async operation... (UI stays responsive!)";
        AsyncProgress.IsIndeterminate = true;
        CancelButton.IsEnabled = true;

        _cancellationTokenSource = new CancellationTokenSource();

        try
        {
            // GOOD: Async operation keeps UI responsive
            await Task.Delay(3000, _cancellationTokenSource.Token);

            AsyncStatus.Text = "Async operation completed (UI stayed responsive)";
        }
        catch (OperationCanceledException)
        {
            AsyncStatus.Text = "Operation was cancelled";
        }
        finally
        {
            AsyncProgress.IsIndeterminate = false;
            CancelButton.IsEnabled = false;
            _cancellationTokenSource?.Dispose();
        }
    }

    private void OnCancelAsync(object sender, RoutedEventArgs e)
    {
        _cancellationTokenSource?.Cancel();
    }

    private async void OnLoadFileAsync(object sender, RoutedEventArgs e)
    {
        DataResultBox.Text = "Loading file asynchronously...\n";

        try
        {
            // Simulate file loading
            await Task.Delay(1000);

            // Create sample data
            var tempFile = Path.GetTempFileName();
            var sampleData = string.Join("\n", Enumerable.Range(1, 100)
                .Select(i => $"Line {i}: Sample data from file"));

            await File.WriteAllTextAsync(tempFile, sampleData);

            // Read asynchronously
            var content = await File.ReadAllTextAsync(tempFile);
            DataResultBox.Text += $"Successfully loaded {content.Length} characters\n";
            DataResultBox.Text += content[..Math.Min(500, content.Length)] + "...";

            File.Delete(tempFile);
        }
        catch (Exception ex)
        {
            DataResultBox.Text += $"Error: {ex.Message}";
        }
    }

    private async void OnFetchWebDataAsync(object sender, RoutedEventArgs e)
    {
        DataResultBox.Text = "Fetching web data asynchronously...\n";

        try
        {
            var url = "https://api.github.com/repos/dotnet/wpf";
            _httpClient.DefaultRequestHeaders.UserAgent.ParseAdd("WPF-AsyncDemo");

            var response = await _httpClient.GetStringAsync(url);
            DataResultBox.Text += $"Successfully fetched {response.Length} characters\n";
            DataResultBox.Text += response[..Math.Min(500, response.Length)] + "...";
        }
        catch (Exception ex)
        {
            DataResultBox.Text += $"Error: {ex.Message}\n";
            DataResultBox.Text += "Note: This demo requires internet connection";
        }
    }

    private async void OnDatabaseQueryAsync(object sender, RoutedEventArgs e)
    {
        DataResultBox.Text = "Simulating database query...\n";

        try
        {
            // Simulate database query
            await Task.Run(async () =>
            {
                await Task.Delay(1500); // Simulate query time
                return Enumerable.Range(1, 50)
                    .Select(i => new
                    {
                        Id = i,
                        Name = $"Record {i}",
                        Timestamp = DateTime.Now.AddMinutes(-i)
                    });
            });

            DataResultBox.Text += "Query completed successfully!\n";
            DataResultBox.Text += "Retrieved 50 records from database\n";
            DataResultBox.Text += "Sample records:\n";

            for (int i = 1; i <= 10; i++)
            {
                DataResultBox.Text += $"  [{i}] Record {i} - {DateTime.Now.AddMinutes(-i):HH:mm:ss}\n";
            }
        }
        catch (Exception ex)
        {
            DataResultBox.Text += $"Error: {ex.Message}";
        }
    }

    private void OnSliderChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
    {
        if (SliderValueText != null)
        {
            SliderValueText.Text = ((int)e.NewValue).ToString();
        }
    }

    private async void OnStartLongTaskAsync(object sender, RoutedEventArgs e)
    {
        AsyncStatus.Text = "Long running task started - try moving the slider!";
        AsyncProgress.IsIndeterminate = true;

        await Task.Delay(5000);

        AsyncProgress.IsIndeterminate = false;
        AsyncStatus.Text = "Task completed - the slider stayed responsive!";
    }

    #endregion

    #region ConfigureAwait

    private async void OnConfigureAwaitFalse(object sender, RoutedEventArgs e)
    {
        LogConfigureAwait("=== ConfigureAwait(false) Test ===");
        LogConfigureAwait($"Before await - Thread: {Environment.CurrentManagedThreadId}");

        await Task.Delay(1000).ConfigureAwait(false);

        LogConfigureAwait($"After await - Thread: {Environment.CurrentManagedThreadId}");
        LogConfigureAwait("Note: Thread ID may differ (not on UI thread)");

        // This will throw exception if we try to update UI directly
        // because we're not on the UI thread anymore
        await Dispatcher.InvokeAsync(() =>
        {
            LogConfigureAwait("Had to use Dispatcher to update UI!");
        });
    }

    private async void OnConfigureAwaitTrue(object sender, RoutedEventArgs e)
    {
        LogConfigureAwait("=== ConfigureAwait(true) Test ===");
        LogConfigureAwait($"Before await - Thread: {Environment.CurrentManagedThreadId}");

        await Task.Delay(1000).ConfigureAwait(true);

        LogConfigureAwait($"After await - Thread: {Environment.CurrentManagedThreadId}");
        LogConfigureAwait("Note: Same thread (back on UI thread)");
        LogConfigureAwait("Can update UI directly without Dispatcher");
    }

    private async void OnWithoutConfigureAwait(object sender, RoutedEventArgs e)
    {
        LogConfigureAwait("=== Without ConfigureAwait (default behavior) ===");
        LogConfigureAwait($"Before await - Thread: {Environment.CurrentManagedThreadId}");

        await Task.Delay(1000); // Default is ConfigureAwait(true)

        LogConfigureAwait($"After await - Thread: {Environment.CurrentManagedThreadId}");
        LogConfigureAwait("Note: Default captures context (same as ConfigureAwait(true))");
    }

    private void LogConfigureAwait(string message)
    {
        ConfigureAwaitLog.Text += $"[{DateTime.Now:HH:mm:ss.fff}] {message}\n";
        ConfigureAwaitLog.ScrollToEnd();
    }

    private void OnClearConfigureAwaitLog(object sender, RoutedEventArgs e)
    {
        ConfigureAwaitLog.Clear();
    }

    private async void OnRunContextAnalysis(object sender, RoutedEventArgs e)
    {
        var results = new List<ContextAnalysisResult>();
        var stopwatch = Stopwatch.StartNew();

        // Test 1: With ConfigureAwait(false)
        var startThread = Environment.CurrentManagedThreadId;
        await Task.Delay(100).ConfigureAwait(false);
        var endThread = Environment.CurrentManagedThreadId;
        results.Add(new ContextAnalysisResult
        {
            Pattern = "ConfigureAwait(false)",
            StartThread = startThread,
            EndThread = endThread,
            ContextSwitched = startThread != endThread,
            ElapsedMs = stopwatch.ElapsedMilliseconds
        });

        await Dispatcher.InvokeAsync(() => { }); // Return to UI thread

        // Test 2: With ConfigureAwait(true)
        stopwatch.Restart();
        startThread = Environment.CurrentManagedThreadId;
        await Task.Delay(100).ConfigureAwait(true);
        endThread = Environment.CurrentManagedThreadId;
        results.Add(new ContextAnalysisResult
        {
            Pattern = "ConfigureAwait(true)",
            StartThread = startThread,
            EndThread = endThread,
            ContextSwitched = startThread != endThread,
            ElapsedMs = stopwatch.ElapsedMilliseconds
        });

        // Test 3: Default (no ConfigureAwait)
        stopwatch.Restart();
        startThread = Environment.CurrentManagedThreadId;
        await Task.Delay(100);
        endThread = Environment.CurrentManagedThreadId;
        results.Add(new ContextAnalysisResult
        {
            Pattern = "Default (no ConfigureAwait)",
            StartThread = startThread,
            EndThread = endThread,
            ContextSwitched = startThread != endThread,
            ElapsedMs = stopwatch.ElapsedMilliseconds
        });

        ContextAnalysisGrid.ItemsSource = results;
    }

    #endregion

    #region Error Handling

    private async void OnTryCatchAsync(object sender, RoutedEventArgs e)
    {
        ErrorHandlingLog.Text = "=== Try/Catch Pattern ===\n";

        try
        {
            ErrorHandlingLog.Text += "Starting operation...\n";
            await Task.Run(async () =>
            {
                await Task.Delay(500);
                throw new InvalidOperationException("Simulated error");
            });
        }
        catch (InvalidOperationException ex)
        {
            ErrorHandlingLog.Text += $"Caught exception: {ex.Message}\n";
            ErrorHandlingLog.Text += "Exception handled gracefully\n";
        }
    }

    private async void OnAggregateExceptionAsync(object sender, RoutedEventArgs e)
    {
        ErrorHandlingLog.Text = "=== AggregateException Handling ===\n";

        try
        {
            var tasks = new List<Task>();
            for (int i = 0; i < 5; i++)
            {
                var index = i;
                tasks.Add(Task.Run(async () =>
                {
                    await Task.Delay(100 * index);
                    if (index % 2 == 0)
                        throw new Exception($"Error in task {index}");
                }));
            }

            await Task.WhenAll(tasks);
        }
        catch (Exception ex)
        {
            ErrorHandlingLog.Text += $"Caught first exception: {ex.Message}\n";

            if (ex is AggregateException aggEx)
            {
                ErrorHandlingLog.Text += $"Total exceptions: {aggEx.InnerExceptions.Count}\n";
                foreach (var inner in aggEx.InnerExceptions)
                {
                    ErrorHandlingLog.Text += $"  - {inner.Message}\n";
                }
            }
        }
    }

    private async void OnTimeoutAsync(object sender, RoutedEventArgs e)
    {
        ErrorHandlingLog.Text = "=== Timeout Handling ===\n";

        using var cts = new CancellationTokenSource(TimeSpan.FromSeconds(2));

        try
        {
            ErrorHandlingLog.Text += "Starting operation with 2-second timeout...\n";
            await Task.Delay(5000, cts.Token); // Will timeout
        }
        catch (OperationCanceledException)
        {
            ErrorHandlingLog.Text += "Operation timed out and was cancelled\n";
        }
    }

    private async void OnStartCancellableTask(object sender, RoutedEventArgs e)
    {
        StartCancellableButton.IsEnabled = false;
        CancelTaskButton.IsEnabled = true;
        _cancellationTokenSource = new CancellationTokenSource();

        try
        {
            CancellableStatus.Text = "Task running...";
            CancellableProgress.Value = 0;

            for (int i = 0; i <= 100; i++)
            {
                _cancellationTokenSource.Token.ThrowIfCancellationRequested();

                CancellableProgress.Value = i;
                await Task.Delay(50, _cancellationTokenSource.Token);
            }

            CancellableStatus.Text = "Task completed successfully!";
        }
        catch (OperationCanceledException)
        {
            CancellableStatus.Text = "Task was cancelled";
        }
        finally
        {
            StartCancellableButton.IsEnabled = true;
            CancelTaskButton.IsEnabled = false;
            _cancellationTokenSource?.Dispose();
        }
    }

    private void OnCancelTask(object sender, RoutedEventArgs e)
    {
        _cancellationTokenSource?.Cancel();
    }

    #endregion

    #region Advanced Patterns

    private async void OnSequentialAsync(object sender, RoutedEventArgs e)
    {
        var stopwatch = Stopwatch.StartNew();
        ParallelStatus.Text = "Running 5 tasks sequentially...";

        for (int i = 1; i <= 5; i++)
        {
            await Task.Delay(500);
        }

        ParallelStatus.Text = $"Sequential completed in {stopwatch.ElapsedMilliseconds}ms";
    }

    private async void OnParallelAsync(object sender, RoutedEventArgs e)
    {
        var stopwatch = Stopwatch.StartNew();
        ParallelStatus.Text = "Running 5 tasks in parallel...";

        var tasks = Enumerable.Range(1, 5).Select(i => Task.Delay(500)).ToList();
        await Task.WhenAll(tasks);

        ParallelStatus.Text = $"Parallel completed in {stopwatch.ElapsedMilliseconds}ms (much faster!)";
    }

    private async void OnWhenAllAsync(object sender, RoutedEventArgs e)
    {
        var taskItems = new List<TaskProgressItem>();
        for (int i = 1; i <= 5; i++)
        {
            taskItems.Add(new TaskProgressItem { Name = $"Task {i}", Progress = 0, Status = "Starting..." });
        }

        ParallelTasks.ItemsSource = taskItems;
        ParallelStatus.Text = "Running WhenAll pattern...";

        var tasks = taskItems.Select(item => SimulateTaskProgress(item)).ToList();
        await Task.WhenAll(tasks);

        ParallelStatus.Text = "All tasks completed!";
    }

    private async void OnWhenAnyAsync(object sender, RoutedEventArgs e)
    {
        var taskItems = new List<TaskProgressItem>();
        for (int i = 1; i <= 5; i++)
        {
            var delay = Random.Shared.Next(1000, 3000);
            taskItems.Add(new TaskProgressItem
            {
                Name = $"Task {i} ({delay}ms)",
                Progress = 0,
                Status = "Starting..."
            });
        }

        ParallelTasks.ItemsSource = taskItems;
        ParallelStatus.Text = "Running WhenAny pattern...";

        var tasks = taskItems.Select(item => SimulateTaskProgress(item)).ToList();
        var firstCompleted = await Task.WhenAny(tasks);

        var completedItem = taskItems[tasks.IndexOf(firstCompleted)];
        ParallelStatus.Text = $"First completed: {completedItem.Name}";
    }

    private async Task SimulateTaskProgress(TaskProgressItem item)
    {
        var delay = Random.Shared.Next(1000, 3000);
        var steps = 100;
        var stepDelay = delay / steps;

        for (int i = 0; i <= steps; i++)
        {
            await Dispatcher.InvokeAsync(() =>
            {
                item.Progress = i;
                item.Status = i == steps ? "Completed" : $"{i}%";
            });

            await Task.Delay(stepDelay);
        }
    }

    private async void OnThrottledOperations(object sender, RoutedEventArgs e)
    {
        var maxConcurrent = (int)ThrottleSlider.Value;
        _throttleSemaphore?.Dispose();
        var semaphore = new SemaphoreSlim(maxConcurrent);

        ThrottleStatus.Text = $"Starting 20 operations with max {maxConcurrent} concurrent...";

        var stopwatch = Stopwatch.StartNew();
        var tasks = Enumerable.Range(1, 20).Select(i => ThrottledOperation(i, semaphore));
        await Task.WhenAll(tasks);

        ThrottleStatus.Text = $"Completed in {stopwatch.ElapsedMilliseconds}ms with max {maxConcurrent} concurrent operations";
    }

    private async Task ThrottledOperation(int id, SemaphoreSlim semaphore)
    {
        await semaphore.WaitAsync();
        try
        {
            await Task.Delay(500);
        }
        finally
        {
            semaphore.Release();
        }
    }

    #endregion
}

public class ContextAnalysisResult
{
    public string Pattern { get; set; } = "";
    public int StartThread { get; set; }
    public int EndThread { get; set; }
    public bool ContextSwitched { get; set; }
    public long ElapsedMs { get; set; }
}

public class TaskProgressItem : System.ComponentModel.INotifyPropertyChanged
{
    private string _name = "";
    private double _progress;
    private string _status = "";

    public string Name
    {
        get => _name;
        set { _name = value; OnPropertyChanged(nameof(Name)); }
    }

    public double Progress
    {
        get => _progress;
        set { _progress = value; OnPropertyChanged(nameof(Progress)); }
    }

    public string Status
    {
        get => _status;
        set { _status = value; OnPropertyChanged(nameof(Status)); }
    }

    public event System.ComponentModel.PropertyChangedEventHandler? PropertyChanged;

    protected void OnPropertyChanged(string propertyName)
    {
        PropertyChanged?.Invoke(this, new System.ComponentModel.PropertyChangedEventArgs(propertyName));
    }
}
