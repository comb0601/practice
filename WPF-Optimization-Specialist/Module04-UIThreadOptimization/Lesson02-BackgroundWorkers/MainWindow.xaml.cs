using System.Collections.Concurrent;
using System.ComponentModel;
using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;

namespace BackgroundWorkers;

public partial class MainWindow : Window
{
    private BackgroundWorker? _backgroundWorker;
    private BackgroundWorker? _dataProcessor;
    private DispatcherTimer? _dispatcherTimer;
    private CancellationTokenSource? _producerConsumerCts;
    private int _timerTicks = 0;

    public MainWindow()
    {
        InitializeComponent();
    }

    #region BackgroundWorker

    private void OnStartBackgroundWorker(object sender, RoutedEventArgs e)
    {
        StartBgWorkerButton.IsEnabled = false;
        CancelBgWorkerButton.IsEnabled = true;
        BgWorkerLog.Clear();

        _backgroundWorker = new BackgroundWorker
        {
            WorkerReportsProgress = true,
            WorkerSupportsCancellation = true
        };

        _backgroundWorker.DoWork += (s, args) =>
        {
            LogBgWorker("Work started on background thread");
            LogBgWorker($"Thread ID: {Environment.CurrentManagedThreadId}");

            for (int i = 1; i <= 100; i++)
            {
                if (_backgroundWorker.CancellationPending)
                {
                    args.Cancel = true;
                    LogBgWorker("Cancellation requested");
                    return;
                }

                // Simulate work
                Thread.Sleep(50);

                // Report progress
                _backgroundWorker.ReportProgress(i, $"Processing step {i}");
            }

            args.Result = "Work completed successfully!";
        };

        _backgroundWorker.ProgressChanged += (s, args) =>
        {
            BgWorkerProgress.Value = args.ProgressPercentage;
            BgWorkerStatus.Text = args.UserState?.ToString() ?? "";
        };

        _backgroundWorker.RunWorkerCompleted += (s, args) =>
        {
            StartBgWorkerButton.IsEnabled = true;
            CancelBgWorkerButton.IsEnabled = false;

            if (args.Cancelled)
            {
                BgWorkerStatus.Text = "Work was cancelled";
                LogBgWorker("Work cancelled by user");
            }
            else if (args.Error != null)
            {
                BgWorkerStatus.Text = $"Error: {args.Error.Message}";
                LogBgWorker($"Error occurred: {args.Error.Message}");
            }
            else
            {
                BgWorkerStatus.Text = args.Result?.ToString() ?? "Completed";
                LogBgWorker(args.Result?.ToString() ?? "");
            }
        };

        _backgroundWorker.RunWorkerAsync();
        LogBgWorker("BackgroundWorker started");
    }

    private void OnCancelBackgroundWorker(object sender, RoutedEventArgs e)
    {
        _backgroundWorker?.CancelAsync();
        LogBgWorker("Cancel requested...");
    }

    private void LogBgWorker(string message)
    {
        Dispatcher.InvokeAsync(() =>
        {
            BgWorkerLog.Text += $"[{DateTime.Now:HH:mm:ss.fff}] {message}\n";
            BgWorkerLog.ScrollToEnd();
        });
    }

    private void OnProcessData(object sender, RoutedEventArgs e)
    {
        ProcessDataButton.IsEnabled = false;
        var itemCount = (int)ItemCountSlider.Value;

        _dataProcessor = new BackgroundWorker
        {
            WorkerReportsProgress = true
        };

        var stopwatch = Stopwatch.StartNew();

        _dataProcessor.DoWork += (s, args) =>
        {
            var count = (int)args.Argument!;
            var sum = 0L;
            var results = new List<int>();

            for (int i = 0; i < count; i++)
            {
                // Simulate data processing
                sum += i;
                if (i % 2 == 0) results.Add(i);

                // Report progress every 1000 items
                if (i % 1000 == 0)
                {
                    var progress = (int)((i / (double)count) * 100);
                    _dataProcessor.ReportProgress(progress);
                }
            }

            args.Result = new { Count = count, Sum = sum, ResultCount = results.Count };
        };

        _dataProcessor.ProgressChanged += (s, args) =>
        {
            ProcessProgress.Value = args.ProgressPercentage;
            ProcessStatus.Text = $"Processing... {args.ProgressPercentage}%";
        };

        _dataProcessor.RunWorkerCompleted += (s, args) =>
        {
            stopwatch.Stop();
            ProcessDataButton.IsEnabled = true;

            if (args.Result is { } result)
            {
                var data = result as dynamic;
                ProcessStatus.Text = "Processing completed!";
                ProcessResults.Text = $"Processed: {data.Count:N0} items\n" +
                                    $"Sum: {data.Sum:N0}\n" +
                                    $"Results: {data.ResultCount:N0} items\n" +
                                    $"Time: {stopwatch.ElapsedMilliseconds}ms";
            }
        };

        _dataProcessor.RunWorkerAsync(itemCount);
    }

    #endregion

    #region Thread & ThreadPool

    private void OnUseNewThread(object sender, RoutedEventArgs e)
    {
        ThreadLog.Clear();
        var stopwatch = Stopwatch.StartNew();

        var thread = new Thread(() =>
        {
            LogThread($"New Thread started - ID: {Environment.CurrentManagedThreadId}");
            LogThread($"IsBackground: {Thread.CurrentThread.IsBackground}");
            LogThread($"IsThreadPoolThread: {Thread.CurrentThread.IsThreadPoolThread}");

            Thread.Sleep(1000);

            LogThread($"Work completed in {stopwatch.ElapsedMilliseconds}ms");
        })
        {
            IsBackground = true,
            Name = "CustomWorkerThread"
        };

        LogThread("Starting new thread...");
        thread.Start();
    }

    private void OnUseThreadPool(object sender, RoutedEventArgs e)
    {
        ThreadLog.Clear();
        var stopwatch = Stopwatch.StartNew();

        ThreadPool.QueueUserWorkItem(_ =>
        {
            LogThread($"ThreadPool thread - ID: {Environment.CurrentManagedThreadId}");
            LogThread($"IsThreadPoolThread: {Thread.CurrentThread.IsThreadPoolThread}");

            Thread.Sleep(1000);

            LogThread($"Work completed in {stopwatch.ElapsedMilliseconds}ms");
        });

        LogThread("Queued work to ThreadPool...");
    }

    private void OnCompareThreading(object sender, RoutedEventArgs e)
    {
        ThreadLog.Clear();
        const int taskCount = 10;

        // Test 1: New threads
        var sw1 = Stopwatch.StartNew();
        var threads = new Thread[taskCount];

        for (int i = 0; i < taskCount; i++)
        {
            threads[i] = new Thread(() => Thread.Sleep(100)) { IsBackground = true };
            threads[i].Start();
        }

        foreach (var thread in threads)
        {
            thread.Join();
        }

        sw1.Stop();
        LogThread($"New Thread() x{taskCount}: {sw1.ElapsedMilliseconds}ms");

        // Test 2: ThreadPool
        var sw2 = Stopwatch.StartNew();
        var countdown = new CountdownEvent(taskCount);

        for (int i = 0; i < taskCount; i++)
        {
            ThreadPool.QueueUserWorkItem(_ =>
            {
                Thread.Sleep(100);
                countdown.Signal();
            });
        }

        countdown.Wait();
        sw2.Stop();
        LogThread($"ThreadPool x{taskCount}: {sw2.ElapsedMilliseconds}ms");

        LogThread($"\nThreadPool is {sw1.ElapsedMilliseconds - sw2.ElapsedMilliseconds}ms faster!");
        LogThread("ThreadPool reuses threads, reducing overhead");
    }

    private void LogThread(string message)
    {
        Dispatcher.InvokeAsync(() =>
        {
            ThreadLog.Text += $"[{DateTime.Now:HH:mm:ss.fff}] {message}\n";
            ThreadLog.ScrollToEnd();
        });
    }

    private void OnGetThreadPoolInfo(object sender, RoutedEventArgs e)
    {
        ThreadPool.GetAvailableThreads(out int workerThreads, out int ioThreads);
        ThreadPool.GetMaxThreads(out int maxWorkerThreads, out int maxIoThreads);

        WorkerThreadsText.Text = workerThreads.ToString();
        IOThreadsText.Text = ioThreads.ToString();
        MaxWorkerThreadsText.Text = maxWorkerThreads.ToString();
        MaxIOThreadsText.Text = maxIoThreads.ToString();
        ProcessorCountText.Text = Environment.ProcessorCount.ToString();
    }

    private void OnSimulateThreadPoolSaturation(object sender, RoutedEventArgs e)
    {
        ThreadLog.Clear();
        LogThread("Saturating ThreadPool with long-running tasks...");

        ThreadPool.GetMaxThreads(out int maxWorkerThreads, out _);
        var taskCount = maxWorkerThreads + 10;

        for (int i = 0; i < taskCount; i++)
        {
            var taskId = i;
            ThreadPool.QueueUserWorkItem(_ =>
            {
                LogThread($"Task {taskId} started on thread {Environment.CurrentManagedThreadId}");
                Thread.Sleep(3000);
                LogThread($"Task {taskId} completed");
            });
        }

        LogThread($"Queued {taskCount} tasks (max threads: {maxWorkerThreads})");
        LogThread("Watch as ThreadPool manages thread allocation...");
    }

    #endregion

    #region Dispatcher Optimization

    private void OnQueueDispatcherUpdates(object sender, RoutedEventArgs e)
    {
        var priority = DispatcherPriorityCombo.SelectedIndex switch
        {
            0 => DispatcherPriority.Background,
            1 => DispatcherPriority.Input,
            2 => DispatcherPriority.Normal,
            3 => DispatcherPriority.Send,
            _ => DispatcherPriority.Normal
        };

        DispatcherStatus.Text = $"Queueing 100 updates at {priority} priority...";
        var stopwatch = Stopwatch.StartNew();

        Task.Run(() =>
        {
            for (int i = 1; i <= 100; i++)
            {
                var progress = i;
                Dispatcher.InvokeAsync(() =>
                {
                    DispatcherProgress.Value = progress;
                    DispatcherStatus.Text = $"Update {progress}/100";
                }, priority);

                Thread.Sleep(20);
            }

            Dispatcher.InvokeAsync(() =>
            {
                stopwatch.Stop();
                DispatcherPerformance.Text = $"Completed in {stopwatch.ElapsedMilliseconds}ms using {priority} priority";
            }, priority);
        });
    }

    private void OnIndividualUpdates(object sender, RoutedEventArgs e)
    {
        UpdatesListBox.Items.Clear();
        BatchingResults.Text = "Running individual updates...";

        var stopwatch = Stopwatch.StartNew();

        Task.Run(() =>
        {
            for (int i = 1; i <= 100; i++)
            {
                var item = $"Item {i}";
                Dispatcher.Invoke(() => UpdatesListBox.Items.Add(item));
                Thread.Sleep(10);
            }

            Dispatcher.Invoke(() =>
            {
                stopwatch.Stop();
                BatchingResults.Text = $"Individual updates: {stopwatch.ElapsedMilliseconds}ms (slower - many context switches)";
            });
        });
    }

    private void OnBatchedUpdates(object sender, RoutedEventArgs e)
    {
        UpdatesListBox.Items.Clear();
        BatchingResults.Text = "Running batched updates...";

        var stopwatch = Stopwatch.StartNew();

        Task.Run(() =>
        {
            var items = new List<string>();
            for (int i = 1; i <= 100; i++)
            {
                items.Add($"Item {i}");
                Thread.Sleep(10);
            }

            Dispatcher.Invoke(() =>
            {
                foreach (var item in items)
                {
                    UpdatesListBox.Items.Add(item);
                }

                stopwatch.Stop();
                BatchingResults.Text = $"Batched updates: {stopwatch.ElapsedMilliseconds}ms (faster - single context switch)";
            });
        });
    }

    private void OnStartTimer(object sender, RoutedEventArgs e)
    {
        _timerTicks = 0;
        StartTimerButton.IsEnabled = false;
        StopTimerButton.IsEnabled = true;
        TimerIntervalSlider.IsEnabled = false;

        var interval = TimeSpan.FromMilliseconds(TimerIntervalSlider.Value);

        _dispatcherTimer = new DispatcherTimer(DispatcherPriority.Normal)
        {
            Interval = interval
        };

        _dispatcherTimer.Tick += (s, args) =>
        {
            _timerTicks++;
            TimerUpdateCount.Text = _timerTicks.ToString();
            TimerProgress.Value = (_timerTicks % 100);
        };

        _dispatcherTimer.Start();
    }

    private void OnStopTimer(object sender, RoutedEventArgs e)
    {
        _dispatcherTimer?.Stop();
        _dispatcherTimer = null;

        StartTimerButton.IsEnabled = true;
        StopTimerButton.IsEnabled = false;
        TimerIntervalSlider.IsEnabled = true;
    }

    #endregion

    #region Advanced Threading

    private void OnStartProducerConsumer(object sender, RoutedEventArgs e)
    {
        StartPCButton.IsEnabled = false;
        StopPCButton.IsEnabled = true;
        ProducerConsumerLog.Clear();

        var producerCount = (int)ProducerCountSlider.Value;
        var consumerCount = (int)ConsumerCountSlider.Value;

        _producerConsumerCts = new CancellationTokenSource();
        var queue = new BlockingCollection<int>(boundedCapacity: 10);
        var produced = 0;
        var consumed = 0;

        // Start producers
        for (int i = 0; i < producerCount; i++)
        {
            var producerId = i + 1;
            Task.Run(() =>
            {
                try
                {
                    while (!_producerConsumerCts.Token.IsCancellationRequested)
                    {
                        var item = Interlocked.Increment(ref produced);
                        queue.Add(item, _producerConsumerCts.Token);
                        LogPC($"Producer {producerId} produced: {item}");
                        Thread.Sleep(Random.Shared.Next(100, 500));
                    }
                }
                catch (OperationCanceledException) { }
            });
        }

        // Start consumers
        for (int i = 0; i < consumerCount; i++)
        {
            var consumerId = i + 1;
            Task.Run(() =>
            {
                try
                {
                    while (!_producerConsumerCts.Token.IsCancellationRequested)
                    {
                        var item = queue.Take(_producerConsumerCts.Token);
                        Interlocked.Increment(ref consumed);
                        LogPC($"Consumer {consumerId} consumed: {item}");
                        Thread.Sleep(Random.Shared.Next(200, 600));
                    }
                }
                catch (OperationCanceledException) { }
            });
        }

        // Update stats
        Task.Run(() =>
        {
            while (!_producerConsumerCts.Token.IsCancellationRequested)
            {
                Dispatcher.InvokeAsync(() =>
                {
                    PCStats.Text = $"Produced: {produced} | Consumed: {consumed} | Queue: {queue.Count}";
                });
                Thread.Sleep(100);
            }
        });
    }

    private void OnStopProducerConsumer(object sender, RoutedEventArgs e)
    {
        _producerConsumerCts?.Cancel();
        StartPCButton.IsEnabled = true;
        StopPCButton.IsEnabled = false;
        LogPC("Producer-Consumer stopped");
    }

    private void LogPC(string message)
    {
        Dispatcher.InvokeAsync(() =>
        {
            ProducerConsumerLog.Text += $"[{DateTime.Now:HH:mm:ss.fff}] {message}\n";
            ProducerConsumerLog.ScrollToEnd();
        }, DispatcherPriority.Background);
    }

    private void OnRaceConditionUnsafe(object sender, RoutedEventArgs e)
    {
        RaceConditionDetails.Text = "Running 10 threads incrementing counter 10,000 times each (no lock)...\n";
        var counter = 0;
        var tasks = new List<Task>();

        for (int i = 0; i < 10; i++)
        {
            tasks.Add(Task.Run(() =>
            {
                for (int j = 0; j < 10000; j++)
                {
                    counter++; // RACE CONDITION!
                }
            }));
        }

        Task.WhenAll(tasks).ContinueWith(_ =>
        {
            Dispatcher.Invoke(() =>
            {
                RaceConditionResult.Text = $"Result: {counter:N0} (Expected: 100,000)";
                RaceConditionResult.Foreground = System.Windows.Media.Brushes.Red;
                RaceConditionDetails.Text += $"Actual: {counter:N0}\n";
                RaceConditionDetails.Text += $"Lost updates: {100000 - counter:N0}\n";
                RaceConditionDetails.Text += "UNSAFE: Race condition caused data loss!";
            });
        });
    }

    private void OnRaceConditionSafe(object sender, RoutedEventArgs e)
    {
        RaceConditionDetails.Text = "Running 10 threads incrementing counter 10,000 times each (with lock)...\n";
        var counter = 0;
        var lockObject = new object();
        var tasks = new List<Task>();

        for (int i = 0; i < 10; i++)
        {
            tasks.Add(Task.Run(() =>
            {
                for (int j = 0; j < 10000; j++)
                {
                    lock (lockObject)
                    {
                        counter++;
                    }
                }
            }));
        }

        Task.WhenAll(tasks).ContinueWith(_ =>
        {
            Dispatcher.Invoke(() =>
            {
                RaceConditionResult.Text = $"Result: {counter:N0} (Expected: 100,000)";
                RaceConditionResult.Foreground = System.Windows.Media.Brushes.Green;
                RaceConditionDetails.Text += $"Actual: {counter:N0}\n";
                RaceConditionDetails.Text += "SAFE: Lock prevented race condition!";
            });
        });
    }

    private void OnRaceConditionInterlocked(object sender, RoutedEventArgs e)
    {
        RaceConditionDetails.Text = "Running 10 threads incrementing counter 10,000 times each (with Interlocked)...\n";
        var counter = 0;
        var tasks = new List<Task>();

        for (int i = 0; i < 10; i++)
        {
            tasks.Add(Task.Run(() =>
            {
                for (int j = 0; j < 10000; j++)
                {
                    Interlocked.Increment(ref counter);
                }
            }));
        }

        Task.WhenAll(tasks).ContinueWith(_ =>
        {
            Dispatcher.Invoke(() =>
            {
                RaceConditionResult.Text = $"Result: {counter:N0} (Expected: 100,000)";
                RaceConditionResult.Foreground = System.Windows.Media.Brushes.Blue;
                RaceConditionDetails.Text += $"Actual: {counter:N0}\n";
                RaceConditionDetails.Text += "SAFE & FAST: Interlocked is lock-free!";
            });
        });
    }

    private void OnConcurrentCollectionTest(object sender, RoutedEventArgs e)
    {
        var results = new List<CollectionTestResult>();
        const int operations = 100000;
        const int threadCount = 4;

        // Test ConcurrentBag
        var sw1 = Stopwatch.StartNew();
        var concurrentBag = new ConcurrentBag<int>();
        Parallel.For(0, operations, i => concurrentBag.Add(i));
        sw1.Stop();
        results.Add(new CollectionTestResult
        {
            Collection = "ConcurrentBag",
            Operations = operations,
            Threads = threadCount,
            TimeMs = sw1.ElapsedMilliseconds,
            ThreadSafe = "Yes"
        });

        // Test ConcurrentQueue
        var sw2 = Stopwatch.StartNew();
        var concurrentQueue = new ConcurrentQueue<int>();
        Parallel.For(0, operations, i => concurrentQueue.Enqueue(i));
        sw2.Stop();
        results.Add(new CollectionTestResult
        {
            Collection = "ConcurrentQueue",
            Operations = operations,
            Threads = threadCount,
            TimeMs = sw2.ElapsedMilliseconds,
            ThreadSafe = "Yes"
        });

        // Test ConcurrentDictionary
        var sw3 = Stopwatch.StartNew();
        var concurrentDict = new ConcurrentDictionary<int, int>();
        Parallel.For(0, operations, i => concurrentDict.TryAdd(i, i));
        sw3.Stop();
        results.Add(new CollectionTestResult
        {
            Collection = "ConcurrentDictionary",
            Operations = operations,
            Threads = threadCount,
            TimeMs = sw3.ElapsedMilliseconds,
            ThreadSafe = "Yes"
        });

        CollectionTestGrid.ItemsSource = results;
    }

    #endregion
}

public class CollectionTestResult
{
    public string Collection { get; set; } = "";
    public int Operations { get; set; }
    public int Threads { get; set; }
    public long TimeMs { get; set; }
    public string ThreadSafe { get; set; } = "";
}
