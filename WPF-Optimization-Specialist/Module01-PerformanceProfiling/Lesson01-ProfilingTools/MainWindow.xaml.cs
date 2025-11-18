using System.Diagnostics;
using System.Text;
using System.Windows;
using System.Windows.Controls;

namespace Lesson01_ProfilingTools
{
    public partial class MainWindow : Window
    {
        private long _memoryBefore;
        private int _gcGen0Before;
        private int _gcGen1Before;
        private int _gcGen2Before;

        public MainWindow()
        {
            InitializeComponent();
            AppendOutput("Application started. Ready to profile different scenarios.");
            AppendOutput("═══════════════════════════════════════════════════════════════");
            AppendOutput("INSTRUCTIONS:");
            AppendOutput("1. Run this application with Visual Studio Profiler (Alt+F2)");
            AppendOutput("2. Or use PerfView for ETW-based profiling");
            AppendOutput("3. Or use dotTrace for detailed CPU profiling");
            AppendOutput("4. Click any scenario button to generate profiling data");
            AppendOutput("═══════════════════════════════════════════════════════════════\n");
        }

        private void BtnCpuHeavy_Click(object sender, RoutedEventArgs e)
        {
            RunScenario("CPU Heavy Calculation", () =>
            {
                const int iterations = 10_000_000;
                double result = 0;

                for (int i = 0; i < iterations; i++)
                {
                    // Simulate CPU-intensive calculation
                    result += Math.Sqrt(i) * Math.Sin(i) * Math.Cos(i);
                    result -= Math.Tan(i % 100 + 1);
                    result *= Math.Log(i + 1);
                }

                return $"Completed {iterations:N0} complex mathematical operations. Result: {result:E2}";
            });
        }

        private void BtnRecursion_Click(object sender, RoutedEventArgs e)
        {
            RunScenario("Deep Recursion", () =>
            {
                long result = Fibonacci(35);
                return $"Fibonacci(35) = {result:N0} (This creates deep call stacks)";
            });
        }

        private void BtnMemoryAllocation_Click(object sender, RoutedEventArgs e)
        {
            RunScenario("Large Memory Allocations", () =>
            {
                List<byte[]> allocations = new();
                const int arrayCount = 1000;
                const int arraySize = 1024 * 100; // 100 KB each

                for (int i = 0; i < arrayCount; i++)
                {
                    allocations.Add(new byte[arraySize]);
                }

                long totalBytes = (long)arrayCount * arraySize;
                return $"Allocated {arrayCount:N0} arrays, total {totalBytes / 1024 / 1024:N0} MB. " +
                       $"Watch memory usage in profiler!";
            });
        }

        private void BtnStringOperations_Click(object sender, RoutedEventArgs e)
        {
            RunScenario("String Operations (Inefficient)", () =>
            {
                // Inefficient string concatenation - demonstrates allocation overhead
                string result = "";
                const int iterations = 10000;

                for (int i = 0; i < iterations; i++)
                {
                    result += $"Line {i}: This is a string concatenation operation. ";
                }

                // Compare with StringBuilder (shown in output)
                StringBuilder sb = new();
                for (int i = 0; i < iterations; i++)
                {
                    sb.Append($"Line {i}: This is a string concatenation operation. ");
                }

                return $"Created {iterations:N0} string concatenations.\n" +
                       $"   String += created massive Gen0/Gen1 pressure!\n" +
                       $"   StringBuilder is more efficient (check GC stats).";
            });
        }

        private void BtnUiBlocking_Click(object sender, RoutedEventArgs e)
        {
            RunScenario("UI Thread Blocking", () =>
            {
                // Intentionally block UI thread - BAD PRACTICE!
                Thread.Sleep(3000); // 3 seconds of blocking
                return "UI thread was blocked for 3 seconds. Application was unresponsive!\n" +
                       "   This is what you should NEVER do in production.\n" +
                       "   Use async/await instead (covered in Module 4).";
            });
        }

        private void BtnMassiveUi_Click(object sender, RoutedEventArgs e)
        {
            RunScenario("Creating Massive UI Elements", () =>
            {
                // Create many UI elements - demonstrates UI performance issues
                StackPanel container = new();
                const int elementCount = 5000;

                for (int i = 0; i < elementCount; i++)
                {
                    TextBlock tb = new()
                    {
                        Text = $"Element {i}: This is a test element with some text content.",
                        Margin = new Thickness(2)
                    };
                    container.Children.Add(tb);
                }

                return $"Created {elementCount:N0} TextBlock elements in memory.\n" +
                       $"   Visual tree complexity impacts rendering performance.\n" +
                       $"   Use virtualization for large lists (Module 3, Lesson 5).";
            });
        }

        private void BtnLinqQuery_Click(object sender, RoutedEventArgs e)
        {
            RunScenario("Complex LINQ Queries", () =>
            {
                // Generate test data
                var data = Enumerable.Range(0, 100000)
                    .Select(i => new
                    {
                        Id = i,
                        Name = $"Item_{i}",
                        Value = Random.Shared.Next(1000),
                        Category = i % 10,
                        IsActive = i % 2 == 0
                    })
                    .ToList();

                // Complex LINQ query with multiple operations
                var result = data
                    .Where(x => x.IsActive)
                    .GroupBy(x => x.Category)
                    .Select(g => new
                    {
                        Category = g.Key,
                        Count = g.Count(),
                        Average = g.Average(x => x.Value),
                        Max = g.Max(x => x.Value),
                        Min = g.Min(x => x.Value)
                    })
                    .OrderByDescending(x => x.Average)
                    .ToList();

                return $"Processed {data.Count:N0} items with complex LINQ query.\n" +
                       $"   Result: {result.Count} categories\n" +
                       $"   LINQ creates iterator chains - check allocation profiling.";
            });
        }

        private void BtnClearResults_Click(object sender, RoutedEventArgs e)
        {
            TxtOutput.Text = "";
            TxtExecutionTime.Text = "0 ms";
            TxtMemoryAllocated.Text = "0 KB";
            TxtGcCollections.Text = "0";
            TxtOperationsPerSec.Text = "0";
            TxtStatus.Text = "Results cleared";
            AppendOutput("Results cleared. Ready for new profiling session.\n");
        }

        private void RunScenario(string scenarioName, Func<string> scenario)
        {
            TxtStatus.Text = $"Running: {scenarioName}...";
            AppendOutput($"\n{'═',60}");
            AppendOutput($"SCENARIO: {scenarioName}");
            AppendOutput($"Started at: {DateTime.Now:HH:mm:ss.fff}");
            AppendOutput($"{'─',60}");

            // Force GC before starting
            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();

            // Capture metrics before
            _memoryBefore = GC.GetTotalMemory(false);
            _gcGen0Before = GC.CollectionCount(0);
            _gcGen1Before = GC.CollectionCount(1);
            _gcGen2Before = GC.CollectionCount(2);

            // Run scenario with timing
            Stopwatch sw = Stopwatch.StartNew();
            string result = scenario();
            sw.Stop();

            // Capture metrics after
            long memoryAfter = GC.GetTotalMemory(false);
            int gcGen0After = GC.CollectionCount(0);
            int gcGen1After = GC.CollectionCount(1);
            int gcGen2After = GC.CollectionCount(2);

            // Calculate results
            long memoryDelta = memoryAfter - _memoryBefore;
            int gcGen0 = gcGen0After - _gcGen0Before;
            int gcGen1 = gcGen1After - _gcGen1Before;
            int gcGen2 = gcGen2After - _gcGen2Before;

            // Update UI
            TxtExecutionTime.Text = $"{sw.ElapsedMilliseconds:N0} ms";
            TxtMemoryAllocated.Text = $"{memoryDelta / 1024:N0} KB";
            TxtGcCollections.Text = $"G0:{gcGen0} G1:{gcGen1} G2:{gcGen2}";

            double opsPerSec = sw.ElapsedMilliseconds > 0 ? 1000.0 / sw.ElapsedMilliseconds : 0;
            TxtOperationsPerSec.Text = $"{opsPerSec:F2}";

            // Output results
            AppendOutput($"\nRESULT: {result}");
            AppendOutput($"\n📊 PERFORMANCE METRICS:");
            AppendOutput($"   ⏱️  Execution Time: {sw.ElapsedMilliseconds:N0} ms ({sw.Elapsed.TotalSeconds:F3}s)");
            AppendOutput($"   💾 Memory Delta: {memoryDelta / 1024:N0} KB ({memoryDelta / 1024.0 / 1024.0:F2} MB)");
            AppendOutput($"   🗑️  GC Collections: Gen0={gcGen0}, Gen1={gcGen1}, Gen2={gcGen2}");
            AppendOutput($"   ⚡ Operations/sec: {opsPerSec:F2}");
            AppendOutput($"\n💡 PROFILING TIP:");
            AppendOutput(GetProfilingTip(scenarioName));
            AppendOutput($"{'═',60}\n");

            TxtStatus.Text = $"Completed: {scenarioName}";
        }

        private string GetProfilingTip(string scenarioName)
        {
            return scenarioName switch
            {
                "CPU Heavy Calculation" =>
                    "   Use CPU sampling in Visual Studio Profiler or PerfView.\n" +
                    "   Look for 'Hot Path' in CPU usage to find bottlenecks.",

                "Deep Recursion" =>
                    "   Check call tree depth in profiler.\n" +
                    "   Look for stack frames and consider iterative approach.",

                "Large Memory Allocations" =>
                    "   Use Memory profiler to see allocation patterns.\n" +
                    "   Check GC heap size and generation promotions.",

                "String Operations (Inefficient)" =>
                    "   Memory profiler will show massive string allocations.\n" +
                    "   Compare GC pressure with StringBuilder approach.",

                "UI Thread Blocking" =>
                    "   Timeline profiler shows UI thread responsiveness.\n" +
                    "   Look for long-running synchronous operations.",

                "Creating Massive UI Elements" =>
                    "   Visual Tree profiler shows element count.\n" +
                    "   Measure layout and render time impact.",

                "Complex LINQ Queries" =>
                    "   CPU profiler shows LINQ iterator overhead.\n" +
                    "   Memory profiler shows closure allocations.",

                _ => "   Run with different profilers to see various metrics."
            };
        }

        private long Fibonacci(int n)
        {
            // Intentionally inefficient recursive implementation for profiling
            if (n <= 1) return n;
            return Fibonacci(n - 1) + Fibonacci(n - 2);
        }

        private void AppendOutput(string text)
        {
            TxtOutput.Text += text + "\n";
        }
    }
}
