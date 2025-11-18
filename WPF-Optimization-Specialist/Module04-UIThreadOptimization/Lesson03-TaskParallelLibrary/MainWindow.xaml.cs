using System.Diagnostics;
using System.Threading.Tasks.Dataflow;
using System.Windows;

namespace TaskParallelLibrary;

public partial class MainWindow : Window
{
    private CancellationTokenSource? _parallelCts;

    public MainWindow()
    {
        InitializeComponent();
    }

    #region Parallel Loops

    private async void OnSequentialFor(object sender, RoutedEventArgs e)
    {
        var iterations = (int)IterationsSlider.Value;
        ParallelForProgress.IsIndeterminate = true;
        ParallelForResults.Clear();

        await Task.Run(() =>
        {
            var stopwatch = Stopwatch.StartNew();
            long sum = 0;

            for (int i = 0; i < iterations; i++)
            {
                sum += ComputeHeavyOperation(i);
            }

            stopwatch.Stop();

            Dispatcher.Invoke(() =>
            {
                ParallelForResults.Text = $"=== Sequential For Loop ===\n";
                ParallelForResults.Text += $"Iterations: {iterations:N0}\n";
                ParallelForResults.Text += $"Time: {stopwatch.ElapsedMilliseconds}ms\n";
                ParallelForResults.Text += $"Result: {sum:N0}\n";
                ParallelForResults.Text += $"Cores used: 1\n";
                ParallelForProgress.IsIndeterminate = false;
            });
        });
    }

    private async void OnParallelFor(object sender, RoutedEventArgs e)
    {
        var iterations = (int)IterationsSlider.Value;
        ParallelForProgress.IsIndeterminate = true;
        ParallelForResults.Clear();

        await Task.Run(() =>
        {
            var stopwatch = Stopwatch.StartNew();
            long sum = 0;

            Parallel.For(0, iterations, i =>
            {
                var result = ComputeHeavyOperation(i);
                Interlocked.Add(ref sum, result);
            });

            stopwatch.Stop();

            Dispatcher.Invoke(() =>
            {
                ParallelForResults.Text = $"=== Parallel.For Loop ===\n";
                ParallelForResults.Text += $"Iterations: {iterations:N0}\n";
                ParallelForResults.Text += $"Time: {stopwatch.ElapsedMilliseconds}ms\n";
                ParallelForResults.Text += $"Result: {sum:N0}\n";
                ParallelForResults.Text += $"Cores available: {Environment.ProcessorCount}\n";
                ParallelForProgress.IsIndeterminate = false;
            });
        });
    }

    private async void OnCompareForLoops(object sender, RoutedEventArgs e)
    {
        var iterations = (int)IterationsSlider.Value;
        ParallelForProgress.IsIndeterminate = true;
        ParallelForResults.Clear();

        await Task.Run(() =>
        {
            // Sequential
            var sw1 = Stopwatch.StartNew();
            long sum1 = 0;
            for (int i = 0; i < iterations; i++)
            {
                sum1 += ComputeHeavyOperation(i);
            }
            sw1.Stop();

            // Parallel
            var sw2 = Stopwatch.StartNew();
            long sum2 = 0;
            Parallel.For(0, iterations, i =>
            {
                var result = ComputeHeavyOperation(i);
                Interlocked.Add(ref sum2, result);
            });
            sw2.Stop();

            var speedup = (double)sw1.ElapsedMilliseconds / sw2.ElapsedMilliseconds;

            Dispatcher.Invoke(() =>
            {
                ParallelForResults.Text = $"=== Performance Comparison ===\n\n";
                ParallelForResults.Text += $"Iterations: {iterations:N0}\n";
                ParallelForResults.Text += $"Processor Cores: {Environment.ProcessorCount}\n\n";
                ParallelForResults.Text += $"Sequential:\n";
                ParallelForResults.Text += $"  Time: {sw1.ElapsedMilliseconds}ms\n";
                ParallelForResults.Text += $"  Result: {sum1:N0}\n\n";
                ParallelForResults.Text += $"Parallel.For:\n";
                ParallelForResults.Text += $"  Time: {sw2.ElapsedMilliseconds}ms\n";
                ParallelForResults.Text += $"  Result: {sum2:N0}\n\n";
                ParallelForResults.Text += $"Speedup: {speedup:F2}x faster!\n";
                ParallelForResults.Text += $"Efficiency: {(speedup / Environment.ProcessorCount * 100):F1}%\n";
                ParallelForProgress.IsIndeterminate = false;
            });
        });
    }

    private long ComputeHeavyOperation(int value)
    {
        // Simulate CPU-intensive work
        var result = value;
        for (int i = 0; i < 100; i++)
        {
            result = (result * 31 + i) % 1000000;
        }
        return result;
    }

    private async void OnParallelForEach(object sender, RoutedEventArgs e)
    {
        ForEachProgress.IsIndeterminate = true;
        ForEachStatus.Text = "Processing images in parallel...";

        var images = Enumerable.Range(1, 20)
            .Select(i => new ImageData { Id = i, Name = $"Image{i}.jpg", Size = Random.Shared.Next(1000, 10000) })
            .ToList();

        var results = new List<ImageProcessingResult>();

        await Task.Run(() =>
        {
            var stopwatch = Stopwatch.StartNew();

            Parallel.ForEach(images, image =>
            {
                // Simulate image processing
                Thread.Sleep(Random.Shared.Next(100, 300));

                var result = new ImageProcessingResult
                {
                    ImageName = image.Name,
                    ProcessedSize = image.Size * 2,
                    ThreadId = Environment.CurrentManagedThreadId,
                    ProcessingTime = Random.Shared.Next(100, 300)
                };

                lock (results)
                {
                    results.Add(result);
                }
            });

            stopwatch.Stop();

            Dispatcher.Invoke(() =>
            {
                ForEachResultsGrid.ItemsSource = results;
                ForEachStatus.Text = $"Processed {images.Count} images in {stopwatch.ElapsedMilliseconds}ms";
                ForEachProgress.IsIndeterminate = false;
            });
        });
    }

    private async void OnParallelWithOptions(object sender, RoutedEventArgs e)
    {
        StartParallelOptionsButton.IsEnabled = false;
        CancelParallelButton.IsEnabled = true;
        _parallelCts = new CancellationTokenSource();

        var maxDegree = (int)MaxDegreeSlider.Value;

        await Task.Run(() =>
        {
            var options = new ParallelOptions
            {
                MaxDegreeOfParallelism = maxDegree,
                CancellationToken = _parallelCts.Token
            };

            try
            {
                Parallel.For(0, 100, options, i =>
                {
                    Dispatcher.InvokeAsync(() =>
                    {
                        ParallelOptionsStatus.Text = $"Processing item {i} on thread {Environment.CurrentManagedThreadId}";
                    });

                    Thread.Sleep(100);
                    options.CancellationToken.ThrowIfCancellationRequested();
                });

                Dispatcher.Invoke(() =>
                {
                    ParallelOptionsStatus.Text = "Completed successfully!";
                });
            }
            catch (OperationCanceledException)
            {
                Dispatcher.Invoke(() =>
                {
                    ParallelOptionsStatus.Text = "Operation cancelled";
                });
            }
            finally
            {
                Dispatcher.Invoke(() =>
                {
                    StartParallelOptionsButton.IsEnabled = true;
                    CancelParallelButton.IsEnabled = false;
                });
            }
        });
    }

    private void OnCancelParallel(object sender, RoutedEventArgs e)
    {
        _parallelCts?.Cancel();
    }

    #endregion

    #region PLINQ

    private async void OnLinqSequential(object sender, RoutedEventArgs e)
    {
        PLinqResults.Clear();

        await Task.Run(() =>
        {
            var stopwatch = Stopwatch.StartNew();

            var result = Enumerable.Range(1, 10000000)
                .Where(x => x % 2 == 0)
                .Select(x => Math.Sqrt(x))
                .Sum();

            stopwatch.Stop();

            Dispatcher.Invoke(() =>
            {
                PLinqResults.Text = $"=== Sequential LINQ ===\n";
                PLinqResults.Text += $"Processed: 10,000,000 items\n";
                PLinqResults.Text += $"Time: {stopwatch.ElapsedMilliseconds}ms\n";
                PLinqResults.Text += $"Result: {result:F2}\n";
            });
        });
    }

    private async void OnPLinq(object sender, RoutedEventArgs e)
    {
        PLinqResults.Clear();

        await Task.Run(() =>
        {
            var stopwatch = Stopwatch.StartNew();

            var result = Enumerable.Range(1, 10000000)
                .AsParallel()
                .Where(x => x % 2 == 0)
                .Select(x => Math.Sqrt(x))
                .Sum();

            stopwatch.Stop();

            Dispatcher.Invoke(() =>
            {
                PLinqResults.Text = $"=== Parallel LINQ (PLINQ) ===\n";
                PLinqResults.Text += $"Processed: 10,000,000 items\n";
                PLinqResults.Text += $"Time: {stopwatch.ElapsedMilliseconds}ms\n";
                PLinqResults.Text += $"Result: {result:F2}\n";
                PLinqResults.Text += $"Cores used: {Environment.ProcessorCount}\n";
            });
        });
    }

    private async void OnComparePLinq(object sender, RoutedEventArgs e)
    {
        PLinqResults.Clear();

        await Task.Run(() =>
        {
            var sw1 = Stopwatch.StartNew();
            var result1 = Enumerable.Range(1, 10000000)
                .Where(x => x % 2 == 0)
                .Select(x => Math.Sqrt(x))
                .Sum();
            sw1.Stop();

            var sw2 = Stopwatch.StartNew();
            var result2 = Enumerable.Range(1, 10000000)
                .AsParallel()
                .Where(x => x % 2 == 0)
                .Select(x => Math.Sqrt(x))
                .Sum();
            sw2.Stop();

            var speedup = (double)sw1.ElapsedMilliseconds / sw2.ElapsedMilliseconds;

            Dispatcher.Invoke(() =>
            {
                PLinqResults.Text = $"=== LINQ vs PLINQ Comparison ===\n\n";
                PLinqResults.Text += $"Sequential LINQ:\n";
                PLinqResults.Text += $"  Time: {sw1.ElapsedMilliseconds}ms\n";
                PLinqResults.Text += $"  Result: {result1:F2}\n\n";
                PLinqResults.Text += $"PLINQ:\n";
                PLinqResults.Text += $"  Time: {sw2.ElapsedMilliseconds}ms\n";
                PLinqResults.Text += $"  Result: {result2:F2}\n\n";
                PLinqResults.Text += $"Speedup: {speedup:F2}x faster!\n";
            });
        });
    }

    private async void OnPLinqFilterMap(object sender, RoutedEventArgs e)
    {
        await Task.Run(() =>
        {
            var data = Enumerable.Range(1, 1000)
                .AsParallel()
                .Where(x => x % 3 == 0)
                .Select(x => new { Number = x, Square = x * x, Cube = x * x * x })
                .Take(20)
                .ToList();

            Dispatcher.Invoke(() =>
            {
                PLinqDataGrid.ItemsSource = data;
                PLinqOperationStatus.Text = $"Filter & Map: Processed 1000 items, showing first 20 results";
            });
        });
    }

    private async void OnPLinqAggregation(object sender, RoutedEventArgs e)
    {
        await Task.Run(() =>
        {
            var stats = Enumerable.Range(1, 10000)
                .AsParallel()
                .Select(x => new { Value = x, IsEven = x % 2 == 0 })
                .GroupBy(x => x.IsEven)
                .Select(g => new
                {
                    Type = g.Key ? "Even" : "Odd",
                    Count = g.Count(),
                    Sum = g.Sum(x => x.Value),
                    Average = g.Average(x => x.Value)
                })
                .ToList();

            Dispatcher.Invoke(() =>
            {
                PLinqDataGrid.ItemsSource = stats;
                PLinqOperationStatus.Text = "Aggregation: Grouped and computed statistics";
            });
        });
    }

    private async void OnPLinqGrouping(object sender, RoutedEventArgs e)
    {
        await Task.Run(() =>
        {
            var groups = Enumerable.Range(1, 100)
                .AsParallel()
                .Select(x => new { Number = x, Group = x % 10 })
                .GroupBy(x => x.Group)
                .Select(g => new
                {
                    GroupId = g.Key,
                    Count = g.Count(),
                    Min = g.Min(x => x.Number),
                    Max = g.Max(x => x.Number)
                })
                .OrderBy(x => x.GroupId)
                .ToList();

            Dispatcher.Invoke(() =>
            {
                PLinqDataGrid.ItemsSource = groups;
                PLinqOperationStatus.Text = "Grouping: Organized into 10 groups";
            });
        });
    }

    private async void OnPLinqConfigured(object sender, RoutedEventArgs e)
    {
        var preserveOrder = PreserveOrderCheckBox.IsChecked ?? false;
        var degree = (int)PLinqDegreeSlider.Value;

        await Task.Run(() =>
        {
            var stopwatch = Stopwatch.StartNew();
            var query = Enumerable.Range(1, 1000000).AsParallel();

            if (degree > 0)
                query = query.WithDegreeOfParallelism(degree);

            if (preserveOrder)
                query = query.AsOrdered();

            var result = query
                .Where(x => x % 2 == 0)
                .Select(x => x * 2)
                .Take(10)
                .ToList();

            stopwatch.Stop();

            Dispatcher.Invoke(() =>
            {
                PLinqConfigStatus.Text = $"Configured PLINQ completed in {stopwatch.ElapsedMilliseconds}ms\n";
                PLinqConfigStatus.Text += $"Order preserved: {preserveOrder}\n";
                PLinqConfigStatus.Text += $"Degree of parallelism: {(degree > 0 ? degree.ToString() : "Auto")}\n";
                PLinqConfigStatus.Text += $"First 10 results: {string.Join(", ", result)}";
            });
        });
    }

    #endregion

    #region TPL Dataflow

    private async void OnActionBlock(object sender, RoutedEventArgs e)
    {
        DataflowLog.Clear();
        LogDataflow("=== ActionBlock Demo ===");

        var actionBlock = new ActionBlock<int>(async i =>
        {
            await Task.Delay(100);
            LogDataflow($"Processing item {i} on thread {Environment.CurrentManagedThreadId}");
        }, new ExecutionDataflowBlockOptions { MaxDegreeOfParallelism = 3 });

        for (int i = 1; i <= 10; i++)
        {
            await actionBlock.SendAsync(i);
        }

        actionBlock.Complete();
        await actionBlock.Completion;

        LogDataflow("ActionBlock completed!");
    }

    private async void OnTransformBlock(object sender, RoutedEventArgs e)
    {
        DataflowLog.Clear();
        LogDataflow("=== TransformBlock Demo ===");

        var transformBlock = new TransformBlock<int, string>(i =>
        {
            var result = $"Item {i} -> {i * i}";
            LogDataflow($"Transformed: {result}");
            return result;
        });

        var actionBlock = new ActionBlock<string>(s =>
        {
            LogDataflow($"Received: {s}");
        });

        transformBlock.LinkTo(actionBlock, new DataflowLinkOptions { PropagateCompletion = true });

        for (int i = 1; i <= 5; i++)
        {
            await transformBlock.SendAsync(i);
        }

        transformBlock.Complete();
        await actionBlock.Completion;

        LogDataflow("TransformBlock pipeline completed!");
    }

    private async void OnBufferBlock(object sender, RoutedEventArgs e)
    {
        DataflowLog.Clear();
        LogDataflow("=== BufferBlock Demo ===");

        var bufferBlock = new BufferBlock<int>();

        // Producer
        var producer = Task.Run(async () =>
        {
            for (int i = 1; i <= 10; i++)
            {
                await bufferBlock.SendAsync(i);
                LogDataflow($"Produced: {i}");
                await Task.Delay(50);
            }
            bufferBlock.Complete();
        });

        // Consumer
        var consumer = Task.Run(async () =>
        {
            while (await bufferBlock.OutputAvailableAsync())
            {
                var item = await bufferBlock.ReceiveAsync();
                LogDataflow($"Consumed: {item}");
                await Task.Delay(100);
            }
        });

        await Task.WhenAll(producer, consumer);
        LogDataflow("BufferBlock demo completed!");
    }

    private void OnClearDataflowLog(object sender, RoutedEventArgs e)
    {
        DataflowLog.Clear();
    }

    private async void OnStartPipeline(object sender, RoutedEventArgs e)
    {
        StartPipelineButton.IsEnabled = false;
        var itemCount = (int)PipelineItemsSlider.Value;

        // Reset progress
        DownloadProgress.Maximum = itemCount;
        ProcessPipelineProgress.Maximum = itemCount;
        TransformProgress.Maximum = itemCount;
        SaveProgress.Maximum = itemCount;

        int downloadCount = 0, processCount = 0, transformCount = 0, saveCount = 0;

        // Build pipeline
        var downloadBlock = new TransformBlock<int, string>(async i =>
        {
            await Task.Delay(50);
            Dispatcher.Invoke(() =>
            {
                DownloadProgress.Value = ++downloadCount;
                DownloadCount.Text = downloadCount.ToString();
            });
            return $"Data-{i}";
        }, new ExecutionDataflowBlockOptions { MaxDegreeOfParallelism = 3 });

        var processBlock = new TransformBlock<string, string>(async data =>
        {
            await Task.Delay(75);
            Dispatcher.Invoke(() =>
            {
                ProcessPipelineProgress.Value = ++processCount;
                ProcessCount.Text = processCount.ToString();
            });
            return data.ToUpper();
        }, new ExecutionDataflowBlockOptions { MaxDegreeOfParallelism = 2 });

        var transformBlock = new TransformBlock<string, string>(async data =>
        {
            await Task.Delay(60);
            Dispatcher.Invoke(() =>
            {
                TransformProgress.Value = ++transformCount;
                TransformCount.Text = transformCount.ToString();
            });
            return $"[{data}]";
        }, new ExecutionDataflowBlockOptions { MaxDegreeOfParallelism = 2 });

        var saveBlock = new ActionBlock<string>(async data =>
        {
            await Task.Delay(40);
            Dispatcher.Invoke(() =>
            {
                SaveProgress.Value = ++saveCount;
                SaveCount.Text = saveCount.ToString();
            });
        }, new ExecutionDataflowBlockOptions { MaxDegreeOfParallelism = 4 });

        // Link pipeline
        downloadBlock.LinkTo(processBlock, new DataflowLinkOptions { PropagateCompletion = true });
        processBlock.LinkTo(transformBlock, new DataflowLinkOptions { PropagateCompletion = true });
        transformBlock.LinkTo(saveBlock, new DataflowLinkOptions { PropagateCompletion = true });

        PipelineStatus.Text = "Pipeline running...";

        // Start pipeline
        var stopwatch = Stopwatch.StartNew();

        for (int i = 1; i <= itemCount; i++)
        {
            await downloadBlock.SendAsync(i);
        }

        downloadBlock.Complete();
        await saveBlock.Completion;

        stopwatch.Stop();
        PipelineStatus.Text = $"Pipeline completed {itemCount} items in {stopwatch.ElapsedMilliseconds}ms";
        StartPipelineButton.IsEnabled = true;
    }

    private async void OnBroadcastBlock(object sender, RoutedEventArgs e)
    {
        var broadcastBlock = new BroadcastBlock<int>(x => x);

        var target1 = new ActionBlock<int>(x => LogDataflow($"Target 1 received: {x}"));
        var target2 = new ActionBlock<int>(x => LogDataflow($"Target 2 received: {x}"));
        var target3 = new ActionBlock<int>(x => LogDataflow($"Target 3 received: {x}"));

        broadcastBlock.LinkTo(target1);
        broadcastBlock.LinkTo(target2);
        broadcastBlock.LinkTo(target3);

        DataflowLog.Clear();
        LogDataflow("=== BroadcastBlock Demo ===");

        for (int i = 1; i <= 5; i++)
        {
            await broadcastBlock.SendAsync(i);
            await Task.Delay(200);
        }

        AdvancedDataflowStatus.Text = "BroadcastBlock sent to 3 targets";
    }

    private async void OnBatchBlock(object sender, RoutedEventArgs e)
    {
        DataflowLog.Clear();
        LogDataflow("=== BatchBlock Demo ===");

        var batchBlock = new BatchBlock<int>(3);
        var actionBlock = new ActionBlock<int[]>(batch =>
        {
            LogDataflow($"Batch received: [{string.Join(", ", batch)}]");
        });

        batchBlock.LinkTo(actionBlock, new DataflowLinkOptions { PropagateCompletion = true });

        for (int i = 1; i <= 10; i++)
        {
            await batchBlock.SendAsync(i);
        }

        batchBlock.Complete();
        await actionBlock.Completion;

        AdvancedDataflowStatus.Text = "BatchBlock grouped items into batches of 3";
    }

    private async void OnJoinBlock(object sender, RoutedEventArgs e)
    {
        DataflowLog.Clear();
        LogDataflow("=== JoinBlock Demo ===");

        var joinBlock = new JoinBlock<int, string>();
        var actionBlock = new ActionBlock<Tuple<int, string>>(tuple =>
        {
            LogDataflow($"Joined: ({tuple.Item1}, {tuple.Item2})");
        });

        joinBlock.LinkTo(actionBlock, new DataflowLinkOptions { PropagateCompletion = true });

        for (int i = 1; i <= 5; i++)
        {
            await joinBlock.Target1.SendAsync(i);
            await joinBlock.Target2.SendAsync($"Item-{i}");
        }

        joinBlock.Complete();
        await actionBlock.Completion;

        AdvancedDataflowStatus.Text = "JoinBlock paired int and string inputs";
    }

    private void LogDataflow(string message)
    {
        Dispatcher.InvokeAsync(() =>
        {
            DataflowLog.Text += $"[{DateTime.Now:HH:mm:ss.fff}] {message}\n";
            DataflowLog.ScrollToEnd();
        });
    }

    #endregion

    #region Task Coordination

    private async void OnSimpleContinuation(object sender, RoutedEventArgs e)
    {
        ContinuationLog.Clear();
        ContinuationLog.Text = "=== Simple Continuation ===\n";

        await Task.Run(() => 42)
            .ContinueWith(t =>
            {
                Dispatcher.Invoke(() =>
                {
                    ContinuationLog.Text += $"Task 1 result: {t.Result}\n";
                });
                return t.Result * 2;
            })
            .ContinueWith(t =>
            {
                Dispatcher.Invoke(() =>
                {
                    ContinuationLog.Text += $"Task 2 result: {t.Result}\n";
                    ContinuationLog.Text += "Continuation chain completed!";
                });
            });
    }

    private async void OnMultipleContinuations(object sender, RoutedEventArgs e)
    {
        ContinuationLog.Clear();
        ContinuationLog.Text = "=== Multiple Continuations ===\n";

        var task = Task.Run(() =>
        {
            Thread.Sleep(500);
            return 100;
        });

        var continuation1 = task.ContinueWith(t =>
        {
            Dispatcher.Invoke(() =>
            {
                ContinuationLog.Text += $"Continuation 1: {t.Result} * 2 = {t.Result * 2}\n";
            });
        });

        var continuation2 = task.ContinueWith(t =>
        {
            Dispatcher.Invoke(() =>
            {
                ContinuationLog.Text += $"Continuation 2: {t.Result} + 10 = {t.Result + 10}\n";
            });
        });

        var continuation3 = task.ContinueWith(t =>
        {
            Dispatcher.Invoke(() =>
            {
                ContinuationLog.Text += $"Continuation 3: {t.Result} / 2 = {t.Result / 2}\n";
            });
        });

        await Task.WhenAll(continuation1, continuation2, continuation3);
        ContinuationLog.Text += "All continuations completed!";
    }

    private async void OnConditionalContinuation(object sender, RoutedEventArgs e)
    {
        ContinuationLog.Clear();
        ContinuationLog.Text = "=== Conditional Continuation ===\n";

        var task = Task.Run(() =>
        {
            if (Random.Shared.Next(2) == 0)
                throw new Exception("Simulated error");
            return 42;
        });

        var successContinuation = task.ContinueWith(t =>
        {
            Dispatcher.Invoke(() =>
            {
                ContinuationLog.Text += $"Success! Result: {t.Result}\n";
            });
        }, TaskContinuationOptions.OnlyOnRanToCompletion);

        var errorContinuation = task.ContinueWith(t =>
        {
            Dispatcher.Invoke(() =>
            {
                ContinuationLog.Text += $"Error: {t.Exception?.InnerException?.Message}\n";
            });
        }, TaskContinuationOptions.OnlyOnFaulted);

        await Task.WhenAll(successContinuation, errorContinuation);
    }

    private async void OnBarrierPattern(object sender, RoutedEventArgs e)
    {
        BarrierLog.Clear();

        var barrier = new Barrier(3, b =>
        {
            Dispatcher.InvokeAsync(() =>
            {
                BarrierLog.Text += $"--- Phase {b.CurrentPhaseNumber} completed ---\n";
            });
        });

        var tasks = new List<Task>();
        for (int i = 1; i <= 3; i++)
        {
            var taskId = i;
            tasks.Add(Task.Run(() =>
            {
                for (int phase = 1; phase <= 3; phase++)
                {
                    Dispatcher.InvokeAsync(() =>
                    {
                        BarrierLog.Text += $"Task {taskId}: Working on phase {phase}\n";
                    });
                    Thread.Sleep(Random.Shared.Next(100, 500));
                    barrier.SignalAndWait();
                }
            }));
        }

        await Task.WhenAll(tasks);
        barrier.Dispose();

        Dispatcher.Invoke(() =>
        {
            BarrierLog.Text += "All tasks synchronized and completed!";
        });
    }

    private async void OnLongRunningTask(object sender, RoutedEventArgs e)
    {
        TaskFactoryStatus.Text = "Starting LongRunning task...";

        await Task.Factory.StartNew(() =>
        {
            Thread.Sleep(2000);
            Dispatcher.Invoke(() =>
            {
                TaskFactoryStatus.Text = $"LongRunning task completed on dedicated thread (not ThreadPool)";
            });
        }, TaskCreationOptions.LongRunning);
    }

    private async void OnAttachedToParent(object sender, RoutedEventArgs e)
    {
        TaskFactoryStatus.Text = "Starting parent task with attached children...";

        await Task.Factory.StartNew(() =>
        {
            Task.Factory.StartNew(() =>
            {
                Thread.Sleep(500);
            }, TaskCreationOptions.AttachedToParent);

            Task.Factory.StartNew(() =>
            {
                Thread.Sleep(700);
            }, TaskCreationOptions.AttachedToParent);

        }).ContinueWith(_ =>
        {
            Dispatcher.Invoke(() =>
            {
                TaskFactoryStatus.Text = "Parent task waited for all attached child tasks to complete";
            });
        });
    }

    private async void OnCustomScheduler(object sender, RoutedEventArgs e)
    {
        TaskFactoryStatus.Text = "Running task on UI TaskScheduler...";

        var uiScheduler = TaskScheduler.FromCurrentSynchronizationContext();

        await Task.Factory.StartNew(() =>
        {
            // This runs on UI thread
            TaskFactoryStatus.Text = $"Running on thread {Environment.CurrentManagedThreadId} (UI thread)";
        }, CancellationToken.None, TaskCreationOptions.None, uiScheduler);
    }

    private async void OnFullComparison(object sender, RoutedEventArgs e)
    {
        var results = new List<PerformanceResult>();
        const int iterations = 1000000;

        // Sequential
        var sw = Stopwatch.StartNew();
        long sum = 0;
        for (int i = 0; i < iterations; i++)
        {
            sum += i % 100;
        }
        sw.Stop();
        results.Add(new PerformanceResult { Method = "Sequential For", TimeMs = sw.ElapsedMilliseconds });

        // Parallel.For
        sw.Restart();
        sum = 0;
        Parallel.For(0, iterations, i => { Interlocked.Add(ref sum, i % 100); });
        sw.Stop();
        results.Add(new PerformanceResult { Method = "Parallel.For", TimeMs = sw.ElapsedMilliseconds });

        // PLINQ
        sw.Restart();
        sum = Enumerable.Range(0, iterations).AsParallel().Sum(i => i % 100);
        sw.Stop();
        results.Add(new PerformanceResult { Method = "PLINQ", TimeMs = sw.ElapsedMilliseconds });

        // Task.WhenAll
        sw.Restart();
        var tasks = Enumerable.Range(0, 10).Select(i => Task.Run(() =>
        {
            long localSum = 0;
            for (int j = 0; j < iterations / 10; j++)
            {
                localSum += j % 100;
            }
            return localSum;
        }));
        await Task.WhenAll(tasks);
        sw.Stop();
        results.Add(new PerformanceResult { Method = "Task.WhenAll", TimeMs = sw.ElapsedMilliseconds });

        ComparisonGrid.ItemsSource = results;
    }

    #endregion
}

public class ImageData
{
    public int Id { get; set; }
    public string Name { get; set; } = "";
    public int Size { get; set; }
}

public class ImageProcessingResult
{
    public string ImageName { get; set; } = "";
    public int ProcessedSize { get; set; }
    public int ThreadId { get; set; }
    public int ProcessingTime { get; set; }
}

public class PerformanceResult
{
    public string Method { get; set; } = "";
    public long TimeMs { get; set; }
}
