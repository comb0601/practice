using System;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Windows;
using System.Windows.Threading;
using BenchmarkDotNet.Running;
using BenchmarkDotNet.Configs;
using BenchmarkDotNet.Jobs;
using BenchmarkDotNet.Toolchains.InProcess.Emit;

namespace BenchmarkingSuiteApp
{
    public partial class MainWindow : Window
    {
        private DispatcherTimer _memoryTimer;
        private int _benchmarkCount = 0;

        public MainWindow()
        {
            InitializeComponent();
            InitializeUI();
            StartMemoryMonitoring();
        }

        private void InitializeUI()
        {
            CustomIterationsSlider.ValueChanged += (s, e) =>
            {
                CustomIterationsText.Text = ((int)e.NewValue).ToString();
            };
        }

        private void StartMemoryMonitoring()
        {
            _memoryTimer = new DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(1)
            };
            _memoryTimer.Tick += (s, e) =>
            {
                long memoryBytes = GC.GetTotalMemory(false);
                MemoryText.Text = $"{memoryBytes / (1024.0 * 1024.0):F2} MB";
            };
            _memoryTimer.Start();
        }

        #region Quick Benchmarks - Binding

        private void BenchmarkBindingMode_Click(object sender, RoutedEventArgs e)
        {
            UpdateStatus("Running binding mode benchmark...");

            var sw = Stopwatch.StartNew();
            int iterations = 100000;

            // Simulate OneWay binding overhead
            long oneWayTime = MeasureOperation(() =>
            {
                for (int i = 0; i < iterations; i++)
                {
                    var dummy = i.ToString(); // OneWay - just read
                }
            });

            // Simulate TwoWay binding overhead
            long twoWayTime = MeasureOperation(() =>
            {
                int value = 0;
                for (int i = 0; i < iterations; i++)
                {
                    value = i; // TwoWay - read and write
                    var dummy = value.ToString();
                }
            });

            BindingBenchmarkResults.Text = $"Binding Mode Benchmark ({iterations:N0} iterations):\n\n" +
                                          $"OneWay Binding:  {oneWayTime,8} ms  ({iterations / (double)oneWayTime:F0} ops/ms)\n" +
                                          $"TwoWay Binding:  {twoWayTime,8} ms  ({iterations / (double)twoWayTime:F0} ops/ms)\n\n" +
                                          $"Difference:      {twoWayTime - oneWayTime,8} ms\n" +
                                          $"Ratio:           {(double)twoWayTime / oneWayTime:F2}x slower\n\n" +
                                          $"Recommendation: Use OneWay bindings when possible for read-only data.";

            _benchmarkCount++;
            BenchmarkCountText.Text = _benchmarkCount.ToString();
            UpdateStatus("Binding mode benchmark completed");
        }

        private void BenchmarkPropertyNotify_Click(object sender, RoutedEventArgs e)
        {
            UpdateStatus("Running property notification benchmark...");

            int iterations = 50000;

            // Without notification
            long withoutNotifyTime = MeasureOperation(() =>
            {
                var obj = new SimpleObject();
                for (int i = 0; i < iterations; i++)
                {
                    obj.SetValueWithoutNotify(i);
                }
            });

            // With INotifyPropertyChanged
            long withNotifyTime = MeasureOperation(() =>
            {
                var obj = new NotifyObject();
                for (int i = 0; i < iterations; i++)
                {
                    obj.Value = i;
                }
            });

            BindingBenchmarkResults.Text = $"Property Notification Benchmark ({iterations:N0} iterations):\n\n" +
                                          $"Without INotifyPropertyChanged:  {withoutNotifyTime,8} ms\n" +
                                          $"With INotifyPropertyChanged:     {withNotifyTime,8} ms\n\n" +
                                          $"Overhead:                        {withNotifyTime - withoutNotifyTime,8} ms\n" +
                                          $"Overhead per operation:          {(withNotifyTime - withoutNotifyTime) * 1000000.0 / iterations:F2} ns\n\n" +
                                          $"Note: INotifyPropertyChanged adds ~{(double)withNotifyTime / withoutNotifyTime:F2}x overhead\n" +
                                          $"but is essential for data binding updates.";

            _benchmarkCount++;
            BenchmarkCountText.Text = _benchmarkCount.ToString();
            UpdateStatus("Property notification benchmark completed");
        }

        private void BenchmarkBindingPath_Click(object sender, RoutedEventArgs e)
        {
            UpdateStatus("Running binding path depth benchmark...");

            int iterations = 100000;

            // Shallow path (1 level)
            long shallowTime = MeasureOperation(() =>
            {
                var obj = new DataObject { Value = 42 };
                for (int i = 0; i < iterations; i++)
                {
                    var dummy = obj.Value;
                }
            });

            // Deep path (3 levels)
            long deepTime = MeasureOperation(() =>
            {
                var obj = new DataObject { Child = new DataObject { Child = new DataObject { Value = 42 } } };
                for (int i = 0; i < iterations; i++)
                {
                    var dummy = obj.Child?.Child?.Value ?? 0;
                }
            });

            BindingBenchmarkResults.Text = $"Binding Path Depth Benchmark ({iterations:N0} iterations):\n\n" +
                                          $"Shallow Path (1 level):  {shallowTime,8} ms\n" +
                                          $"Deep Path (3 levels):    {deepTime,8} ms\n\n" +
                                          $"Overhead:                {deepTime - shallowTime,8} ms\n" +
                                          $"Ratio:                   {(double)deepTime / shallowTime:F2}x slower\n\n" +
                                          $"Recommendation: Keep binding paths shallow (1-2 levels) for best performance.";

            _benchmarkCount++;
            BenchmarkCountText.Text = _benchmarkCount.ToString();
            UpdateStatus("Binding path benchmark completed");
        }

        #endregion

        #region Quick Benchmarks - Collections

        private void BenchmarkObservableAdd_Click(object sender, RoutedEventArgs e)
        {
            UpdateStatus("Running ObservableCollection benchmark...");

            int iterations = 10000;

            // ObservableCollection
            long observableTime = MeasureOperation(() =>
            {
                var collection = new System.Collections.ObjectModel.ObservableCollection<int>();
                for (int i = 0; i < iterations; i++)
                {
                    collection.Add(i);
                }
            });

            // List<T>
            long listTime = MeasureOperation(() =>
            {
                var list = new System.Collections.Generic.List<int>();
                for (int i = 0; i < iterations; i++)
                {
                    list.Add(i);
                }
            });

            CollectionBenchmarkResults.Text = $"Collection Add Benchmark ({iterations:N0} items):\n\n" +
                                             $"ObservableCollection<T>:  {observableTime,8} ms\n" +
                                             $"List<T>:                  {listTime,8} ms\n\n" +
                                             $"Overhead:                 {observableTime - listTime,8} ms\n" +
                                             $"Ratio:                    {(double)observableTime / listTime:F2}x slower\n\n" +
                                             $"Note: ObservableCollection raises CollectionChanged events for each Add.\n" +
                                             $"For bulk operations, consider using AddRange or disabling notifications.";

            _benchmarkCount++;
            BenchmarkCountText.Text = _benchmarkCount.ToString();
            UpdateStatus("ObservableCollection benchmark completed");
        }

        private void BenchmarkCollectionClear_Click(object sender, RoutedEventArgs e)
        {
            UpdateStatus("Running collection clear benchmark...");

            int size = 10000;

            // ObservableCollection Clear
            var obsCollection = new System.Collections.ObjectModel.ObservableCollection<int>();
            for (int i = 0; i < size; i++) obsCollection.Add(i);

            long observableTime = MeasureOperation(() =>
            {
                obsCollection.Clear();
                for (int i = 0; i < size; i++) obsCollection.Add(i);
            });

            // List Clear
            var list = new System.Collections.Generic.List<int>();
            for (int i = 0; i < size; i++) list.Add(i);

            long listTime = MeasureOperation(() =>
            {
                list.Clear();
                for (int i = 0; i < size; i++) list.Add(i);
            });

            CollectionBenchmarkResults.Text = $"Collection Clear + Refill Benchmark ({size:N0} items):\n\n" +
                                             $"ObservableCollection:  {observableTime,8} ms\n" +
                                             $"List:                  {listTime,8} ms\n\n" +
                                             $"Difference:            {observableTime - listTime,8} ms\n" +
                                             $"Ratio:                 {(double)observableTime / listTime:F2}x slower";

            _benchmarkCount++;
            BenchmarkCountText.Text = _benchmarkCount.ToString();
            UpdateStatus("Collection clear benchmark completed");
        }

        private void BenchmarkBulkOperations_Click(object sender, RoutedEventArgs e)
        {
            UpdateStatus("Running bulk operations benchmark...");

            int count = 1000;

            // Individual adds
            long individualTime = MeasureOperation(() =>
            {
                var collection = new System.Collections.ObjectModel.ObservableCollection<int>();
                for (int i = 0; i < count; i++)
                {
                    collection.Add(i);
                }
            });

            // Bulk add (using List then assigning)
            long bulkTime = MeasureOperation(() =>
            {
                var list = new System.Collections.Generic.List<int>();
                for (int i = 0; i < count; i++)
                {
                    list.Add(i);
                }
                var collection = new System.Collections.ObjectModel.ObservableCollection<int>(list);
            });

            CollectionBenchmarkResults.Text = $"Bulk Operations Benchmark ({count:N0} items):\n\n" +
                                             $"Individual Add calls:    {individualTime,8} ms\n" +
                                             $"Bulk initialization:     {bulkTime,8} ms\n\n" +
                                             $"Time saved:              {individualTime - bulkTime,8} ms\n" +
                                             $"Speedup:                 {(double)individualTime / bulkTime:F2}x faster\n\n" +
                                             $"Recommendation: For initial population, build List<T> first,\n" +
                                             $"then create ObservableCollection from it.";

            _benchmarkCount++;
            BenchmarkCountText.Text = _benchmarkCount.ToString();
            UpdateStatus("Bulk operations benchmark completed");
        }

        #endregion

        #region Quick Benchmarks - Rendering

        private void BenchmarkFrozenResources_Click(object sender, RoutedEventArgs e)
        {
            UpdateStatus("Running frozen resources benchmark...");

            int iterations = 100000;

            // Unfrozen brush
            long unfrozenTime = MeasureOperation(() =>
            {
                for (int i = 0; i < iterations; i++)
                {
                    var brush = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Colors.Red);
                    var dummy = brush.Color;
                }
            });

            // Frozen brush
            long frozenTime = MeasureOperation(() =>
            {
                for (int i = 0; i < iterations; i++)
                {
                    var brush = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Colors.Red);
                    brush.Freeze();
                    var dummy = brush.Color;
                }
            });

            RenderingBenchmarkResults.Text = $"Frozen Resources Benchmark ({iterations:N0} iterations):\n\n" +
                                            $"Unfrozen Brush:  {unfrozenTime,8} ms\n" +
                                            $"Frozen Brush:    {frozenTime,8} ms\n\n" +
                                            $"Difference:      {Math.Abs(unfrozenTime - frozenTime),8} ms\n\n" +
                                            $"Note: Frozen resources are thread-safe and render faster.\n" +
                                            $"Always freeze immutable Brush, Pen, and Geometry objects.";

            _benchmarkCount++;
            BenchmarkCountText.Text = _benchmarkCount.ToString();
            UpdateStatus("Frozen resources benchmark completed");
        }

        private void BenchmarkDrawingContext_Click(object sender, RoutedEventArgs e)
        {
            UpdateStatus("Running DrawingContext benchmark...");

            int iterations = 10000;

            var visual = new System.Windows.Media.DrawingVisual();
            var pen = new System.Windows.Media.Pen(System.Windows.Media.Brushes.Black, 1);
            pen.Freeze();

            long drawTime = MeasureOperation(() =>
            {
                for (int i = 0; i < iterations; i++)
                {
                    using (var dc = visual.RenderOpen())
                    {
                        dc.DrawLine(pen, new Point(0, 0), new Point(100, 100));
                    }
                }
            });

            RenderingBenchmarkResults.Text = $"DrawingContext Operations Benchmark ({iterations:N0} operations):\n\n" +
                                            $"DrawLine time:     {drawTime,8} ms\n" +
                                            $"Ops per second:    {iterations * 1000.0 / drawTime:N0}\n" +
                                            $"Time per op:       {drawTime * 1000000.0 / iterations:F2} ns\n\n" +
                                            $"Note: DrawingContext operations are very fast when using\n" +
                                            $"frozen Pen and Brush objects.";

            _benchmarkCount++;
            BenchmarkCountText.Text = _benchmarkCount.ToString();
            UpdateStatus("DrawingContext benchmark completed");
        }

        private void BenchmarkGeometry_Click(object sender, RoutedEventArgs e)
        {
            UpdateStatus("Running geometry creation benchmark...");

            int iterations = 10000;

            // PathGeometry
            long pathGeometryTime = MeasureOperation(() =>
            {
                for (int i = 0; i < iterations; i++)
                {
                    var geometry = new System.Windows.Media.PathGeometry();
                    var figure = new System.Windows.Media.PathFigure { StartPoint = new Point(0, 0) };
                    figure.Segments.Add(new System.Windows.Media.LineSegment(new Point(100, 100), true));
                    geometry.Figures.Add(figure);
                }
            });

            // StreamGeometry
            long streamGeometryTime = MeasureOperation(() =>
            {
                for (int i = 0; i < iterations; i++)
                {
                    var geometry = new System.Windows.Media.StreamGeometry();
                    using (var ctx = geometry.Open())
                    {
                        ctx.BeginFigure(new Point(0, 0), false, false);
                        ctx.LineTo(new Point(100, 100), true, false);
                    }
                    geometry.Freeze();
                }
            });

            RenderingBenchmarkResults.Text = $"Geometry Creation Benchmark ({iterations:N0} iterations):\n\n" +
                                            $"PathGeometry:    {pathGeometryTime,8} ms\n" +
                                            $"StreamGeometry:  {streamGeometryTime,8} ms\n\n" +
                                            $"Speedup:         {(double)pathGeometryTime / streamGeometryTime:F2}x faster\n\n" +
                                            $"Recommendation: Use StreamGeometry for better performance.\n" +
                                            $"It's lightweight and can be frozen for reuse.";

            _benchmarkCount++;
            BenchmarkCountText.Text = _benchmarkCount.ToString();
            UpdateStatus("Geometry benchmark completed");
        }

        #endregion

        #region BenchmarkDotNet Integration

        private void RunBenchmarkDotNet_Click(object sender, RoutedEventArgs e)
        {
            UpdateStatus("Preparing BenchmarkDotNet suite...");

            var selectedBenchmarks = new System.Collections.Generic.List<Type>();

            if (BenchDataBindingCheck.IsChecked == true)
                selectedBenchmarks.Add(typeof(Benchmarks.DataBindingBenchmarks));

            if (BenchCollectionsCheck.IsChecked == true)
                selectedBenchmarks.Add(typeof(Benchmarks.CollectionBenchmarks));

            if (BenchRenderingCheck.IsChecked == true)
                selectedBenchmarks.Add(typeof(Benchmarks.RenderingBenchmarks));

            if (BenchMemoryCheck.IsChecked == true)
                selectedBenchmarks.Add(typeof(Benchmarks.MemoryBenchmarks));

            if (BenchUnsafeCheck.IsChecked == true)
                selectedBenchmarks.Add(typeof(Benchmarks.UnsafeBenchmarks));

            if (selectedBenchmarks.Count == 0)
            {
                MessageBox.Show("Please select at least one benchmark suite to run.", "No Selection",
                    MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }

            BenchmarkDotNetResults.Text = "Running BenchmarkDotNet...\n" +
                                         "This may take several minutes.\n" +
                                         "Results will appear in BenchmarkDotNet.Artifacts folder.\n\n" +
                                         $"Selected suites: {selectedBenchmarks.Count}\n\n" +
                                         "Detailed results will be saved to:\n" +
                                         Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "BenchmarkDotNet.Artifacts");

            UpdateStatus($"Running {selectedBenchmarks.Count} benchmark suite(s)...");

            // Run in background to not block UI
            Task.Run(() =>
            {
                try
                {
                    var config = ManualConfig.Create(DefaultConfig.Instance)
                        .WithOptions(ConfigOptions.DisableOptimizationsValidator);

                    foreach (var benchmarkType in selectedBenchmarks)
                    {
                        BenchmarkRunner.Run(benchmarkType, config);
                    }

                    Dispatcher.Invoke(() =>
                    {
                        BenchmarkDotNetResults.Text += "\n\nBenchmarking completed!\n" +
                                                      "Check BenchmarkDotNet.Artifacts folder for detailed reports.";
                        UpdateStatus("BenchmarkDotNet completed successfully");
                        _benchmarkCount++;
                        BenchmarkCountText.Text = _benchmarkCount.ToString();
                    });
                }
                catch (Exception ex)
                {
                    Dispatcher.Invoke(() =>
                    {
                        BenchmarkDotNetResults.Text += $"\n\nError running benchmarks:\n{ex.Message}";
                        UpdateStatus("BenchmarkDotNet error");
                    });
                }
            });
        }

        #endregion

        #region Custom Benchmarks

        private void RunCustomBenchmark_Click(object sender, RoutedEventArgs e)
        {
            string name = CustomBenchmarkName.Text;
            int iterations = (int)CustomIterationsSlider.Value;
            int selectedOp = CustomOperationCombo.SelectedIndex;

            UpdateStatus($"Running custom benchmark: {name}...");

            long time = selectedOp switch
            {
                0 => BenchmarkStringConcatenation(iterations),
                1 => BenchmarkCollectionOps(iterations),
                2 => BenchmarkPropertySetter(iterations),
                3 => BenchmarkBindingUpdate(iterations),
                4 => BenchmarkVisualTreeCreation(iterations),
                _ => 0
            };

            CustomBenchmarkResults.Text = $"Custom Benchmark: {name}\n" +
                                         $"Operation: {((ComboBoxItem)CustomOperationCombo.SelectedItem).Content}\n" +
                                         $"Iterations: {iterations:N0}\n\n" +
                                         $"Total Time:      {time,8} ms\n" +
                                         $"Time per op:     {time * 1000000.0 / iterations:F2} ns\n" +
                                         $"Ops per second:  {iterations * 1000.0 / time:N0}";

            _benchmarkCount++;
            BenchmarkCountText.Text = _benchmarkCount.ToString();
            UpdateStatus("Custom benchmark completed");
        }

        private long BenchmarkStringConcatenation(int iterations)
        {
            return MeasureOperation(() =>
            {
                for (int i = 0; i < iterations; i++)
                {
                    string result = "Item " + i + " of " + iterations;
                }
            });
        }

        private long BenchmarkCollectionOps(int iterations)
        {
            return MeasureOperation(() =>
            {
                var list = new System.Collections.Generic.List<int>();
                for (int i = 0; i < iterations; i++)
                {
                    list.Add(i);
                    if (i % 100 == 0 && list.Count > 0)
                        list.RemoveAt(0);
                }
            });
        }

        private long BenchmarkPropertySetter(int iterations)
        {
            var obj = new NotifyObject();
            return MeasureOperation(() =>
            {
                for (int i = 0; i < iterations; i++)
                {
                    obj.Value = i;
                }
            });
        }

        private long BenchmarkBindingUpdate(int iterations)
        {
            return MeasureOperation(() =>
            {
                for (int i = 0; i < iterations; i++)
                {
                    var obj = new DataObject { Value = i };
                    var dummy = obj.Value;
                }
            });
        }

        private long BenchmarkVisualTreeCreation(int iterations)
        {
            return MeasureOperation(() =>
            {
                for (int i = 0; i < iterations; i++)
                {
                    var button = new System.Windows.Controls.Button
                    {
                        Content = $"Button {i}"
                    };
                }
            });
        }

        #endregion

        #region Template Benchmarks

        private void BenchmarkDispatcher_Click(object sender, RoutedEventArgs e)
        {
            UpdateStatus("Running dispatcher benchmark...");

            int iterations = 1000;

            long invokeTime = MeasureOperation(() =>
            {
                for (int i = 0; i < iterations; i++)
                {
                    Dispatcher.Invoke(() => { });
                }
            });

            long beginInvokeTime = MeasureOperation(() =>
            {
                for (int i = 0; i < iterations; i++)
                {
                    Dispatcher.BeginInvoke(() => { });
                }
                Dispatcher.Invoke(() => { }, DispatcherPriority.SystemIdle);
            });

            TemplateBenchmarkResults.Text = $"Dispatcher Benchmark ({iterations:N0} calls):\n\n" +
                                           $"Invoke:       {invokeTime,8} ms  (synchronous)\n" +
                                           $"BeginInvoke:  {beginInvokeTime,8} ms  (asynchronous)\n\n" +
                                           $"Note: Invoke blocks, BeginInvoke queues and returns immediately.";

            _benchmarkCount++;
            BenchmarkCountText.Text = _benchmarkCount.ToString();
            UpdateStatus("Dispatcher benchmark completed");
        }

        private void BenchmarkDependencyProperty_Click(object sender, RoutedEventArgs e)
        {
            UpdateStatus("Running dependency property benchmark...");

            int iterations = 100000;

            var button = new System.Windows.Controls.Button();

            long dpTime = MeasureOperation(() =>
            {
                for (int i = 0; i < iterations; i++)
                {
                    button.SetValue(System.Windows.Controls.Button.ContentProperty, i);
                }
            });

            long clrTime = MeasureOperation(() =>
            {
                var obj = new SimpleObject();
                for (int i = 0; i < iterations; i++)
                {
                    obj.SetValueWithoutNotify(i);
                }
            });

            TemplateBenchmarkResults.Text = $"Dependency Property Benchmark ({iterations:N0} sets):\n\n" +
                                           $"DependencyProperty:  {dpTime,8} ms\n" +
                                           $"CLR Property:        {clrTime,8} ms\n\n" +
                                           $"Overhead:            {dpTime - clrTime,8} ms\n" +
                                           $"Ratio:               {(double)dpTime / clrTime:F2}x slower\n\n" +
                                           $"Note: DependencyProperty provides features like binding,\n" +
                                           $"animation, and inheritance at the cost of performance.";

            _benchmarkCount++;
            BenchmarkCountText.Text = _benchmarkCount.ToString();
            UpdateStatus("Dependency property benchmark completed");
        }

        private void BenchmarkRoutedEvents_Click(object sender, RoutedEventArgs e)
        {
            TemplateBenchmarkResults.Text = "Routed Events Benchmark:\n\n" +
                                           "This benchmark requires a visual tree setup.\n" +
                                           "Check BenchmarkDotNet results for detailed analysis.";
            UpdateStatus("Routed events benchmark - see BenchmarkDotNet for full results");
        }

        private void BenchmarkStyles_Click(object sender, RoutedEventArgs e)
        {
            UpdateStatus("Running style application benchmark...");

            int iterations = 10000;

            // Without style
            long withoutStyleTime = MeasureOperation(() =>
            {
                for (int i = 0; i < iterations; i++)
                {
                    var button = new System.Windows.Controls.Button
                    {
                        Width = 100,
                        Height = 30,
                        Background = System.Windows.Media.Brushes.Blue
                    };
                }
            });

            // With style
            var style = new Style(typeof(System.Windows.Controls.Button));
            style.Setters.Add(new Setter(System.Windows.Controls.Button.WidthProperty, 100.0));
            style.Setters.Add(new Setter(System.Windows.Controls.Button.HeightProperty, 30.0));
            style.Setters.Add(new Setter(System.Windows.Controls.Button.BackgroundProperty, System.Windows.Media.Brushes.Blue));
            style.Seal();

            long withStyleTime = MeasureOperation(() =>
            {
                for (int i = 0; i < iterations; i++)
                {
                    var button = new System.Windows.Controls.Button
                    {
                        Style = style
                    };
                }
            });

            TemplateBenchmarkResults.Text = $"Style Application Benchmark ({iterations:N0} buttons):\n\n" +
                                           $"Without Style:  {withoutStyleTime,8} ms\n" +
                                           $"With Style:     {withStyleTime,8} ms\n\n" +
                                           $"Difference:     {Math.Abs(withStyleTime - withoutStyleTime),8} ms";

            _benchmarkCount++;
            BenchmarkCountText.Text = _benchmarkCount.ToString();
            UpdateStatus("Style benchmark completed");
        }

        private void BenchmarkTemplates_Click(object sender, RoutedEventArgs e)
        {
            TemplateBenchmarkResults.Text = "Template Expansion Benchmark:\n\n" +
                                           "Template expansion is complex and depends on visual tree.\n" +
                                           "See BenchmarkDotNet suite for comprehensive analysis.";
            UpdateStatus("Template benchmark - see BenchmarkDotNet for full results");
        }

        private void BenchmarkAnimation_Click(object sender, RoutedEventArgs e)
        {
            TemplateBenchmarkResults.Text = "Animation Performance Benchmark:\n\n" +
                                           "Animation benchmarking requires frame timing analysis.\n" +
                                           "Use profiling tools for accurate animation performance data.";
            UpdateStatus("Animation benchmark - use profiling tools for accurate results");
        }

        #endregion

        #region Results & Analysis

        private void RefreshResults_Click(object sender, RoutedEventArgs e)
        {
            UpdateStatus("Refreshing results...");

            ResultsSummary.Text = $"Benchmark Results Summary\n" +
                                 $"========================\n\n" +
                                 $"Total benchmarks run: {_benchmarkCount}\n\n" +
                                 $"Quick benchmarks provide immediate feedback for common scenarios.\n" +
                                 $"BenchmarkDotNet provides production-grade statistical analysis.\n\n" +
                                 $"For detailed BenchmarkDotNet results, check:\n" +
                                 $"{Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "BenchmarkDotNet.Artifacts")}\n\n" +
                                 $"Files include:\n" +
                                 $"  • HTML reports (interactive)\n" +
                                 $"  • CSV data (for analysis)\n" +
                                 $"  • Markdown summaries\n" +
                                 $"  • Console logs";

            UpdateStatus("Results refreshed");
        }

        private void ExportCSV_Click(object sender, RoutedEventArgs e)
        {
            UpdateStatus("CSV export available in BenchmarkDotNet.Artifacts folder");
            MessageBox.Show("BenchmarkDotNet automatically exports results to CSV format.\n\n" +
                          $"Check folder:\n{Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "BenchmarkDotNet.Artifacts")}",
                          "Export Information", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void OpenReportsFolder_Click(object sender, RoutedEventArgs e)
        {
            string artifactsPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "BenchmarkDotNet.Artifacts");

            if (Directory.Exists(artifactsPath))
            {
                Process.Start(new ProcessStartInfo
                {
                    FileName = artifactsPath,
                    UseShellExecute = true
                });
                UpdateStatus("Opened reports folder");
            }
            else
            {
                MessageBox.Show("No benchmark results found yet.\nRun BenchmarkDotNet suite to generate reports.",
                    "No Results", MessageBoxButton.OK, MessageBoxImage.Information);
            }
        }

        #endregion

        #region Helper Methods

        private long MeasureOperation(Action operation)
        {
            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();

            var sw = Stopwatch.StartNew();
            operation();
            sw.Stop();

            return sw.ElapsedMilliseconds;
        }

        private void UpdateStatus(string message)
        {
            StatusText.Text = message;
        }

        #endregion

        #region Helper Classes

        private class SimpleObject
        {
            private int _value;

            public void SetValueWithoutNotify(int value)
            {
                _value = value;
            }
        }

        private class NotifyObject : System.ComponentModel.INotifyPropertyChanged
        {
            private int _value;

            public int Value
            {
                get => _value;
                set
                {
                    if (_value != value)
                    {
                        _value = value;
                        PropertyChanged?.Invoke(this, new System.ComponentModel.PropertyChangedEventArgs(nameof(Value)));
                    }
                }
            }

            public event System.ComponentModel.PropertyChangedEventHandler? PropertyChanged;
        }

        private class DataObject
        {
            public int Value { get; set; }
            public DataObject? Child { get; set; }
        }

        #endregion

        protected override void OnClosed(EventArgs e)
        {
            _memoryTimer?.Stop();
            base.OnClosed(e);
        }
    }
}
