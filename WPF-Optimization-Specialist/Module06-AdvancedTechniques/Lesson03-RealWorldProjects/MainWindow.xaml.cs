using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Threading;

namespace RealWorldProjectsApp
{
    public partial class MainWindow : Window
    {
        private DispatcherTimer _case1UpdateTimer;
        private DispatcherTimer _memoryTimer;
        private Random _random = new Random();

        public MainWindow()
        {
            InitializeComponent();
            InitializeUI();
            StartMemoryMonitoring();
        }

        private void InitializeUI()
        {
            // Case 1 sliders
            Case1RowsSlider.ValueChanged += (s, e) =>
            {
                Case1RowsText.Text = $"{(int)e.NewValue:N0}";
            };

            Case1UpdateSlider.ValueChanged += (s, e) =>
            {
                Case1UpdateText.Text = ((int)e.NewValue).ToString();
            };

            // Case 2 sliders
            Case2ImageCountSlider.ValueChanged += (s, e) =>
            {
                Case2ImageCountText.Text = ((int)e.NewValue).ToString();
            };

            Case2ThumbnailSizeSlider.ValueChanged += (s, e) =>
            {
                Case2ThumbnailSizeText.Text = ((int)e.NewValue).ToString();
            };

            // Case 3 sliders
            Case3DepthSlider.ValueChanged += (s, e) =>
            {
                Case3DepthText.Text = ((int)e.NewValue).ToString();
            };

            Case3ChildrenSlider.ValueChanged += (s, e) =>
            {
                Case3ChildrenText.Text = ((int)e.NewValue).ToString();
            };

            UpdateSummary();
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

        #region Case Study 1: Large Data Grid

        private void Case1LoadBtn_Click(object sender, RoutedEventArgs e)
        {
            int rows = (int)Case1RowsSlider.Value;

            var sw = Stopwatch.StartNew();
            Case1BeforeControl.LoadData(rows);
            sw.Stop();
            long beforeTime = sw.ElapsedMilliseconds;
            long beforeMemory = GC.GetTotalMemory(true);

            sw.Restart();
            Case1AfterControl.LoadData(rows);
            sw.Stop();
            long afterTime = sw.ElapsedMilliseconds;
            long afterMemory = GC.GetTotalMemory(true);

            Case1BeforeStats.Text = $"Load Time: {beforeTime} ms\n" +
                                   $"Memory: {beforeMemory / 1024.0:F2} KB\n" +
                                   $"Rows: {rows:N0}\n" +
                                   $"Status: Loaded all rows";

            Case1AfterStats.Text = $"Load Time: {afterTime} ms\n" +
                                  $"Memory: {afterMemory / 1024.0:F2} KB\n" +
                                  $"Rows: {rows:N0}\n" +
                                  $"Status: Virtualized\n" +
                                  $"Speedup: {(double)beforeTime / afterTime:F2}x faster";

            UpdateStatus($"Data loaded: {rows:N0} rows. Before: {beforeTime}ms, After: {afterTime}ms");
            UpdateSummary();
        }

        private void Case1StartUpdatesBtn_Click(object sender, RoutedEventArgs e)
        {
            int updatesPerSec = (int)Case1UpdateSlider.Value;

            _case1UpdateTimer = new DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(1.0 / updatesPerSec)
            };
            _case1UpdateTimer.Tick += (s, args) =>
            {
                Case1BeforeControl.UpdateRandomRows(10);
                Case1AfterControl.UpdateRandomRows(10);
            };
            _case1UpdateTimer.Start();

            Case1StartUpdatesBtn.IsEnabled = false;
            Case1StopUpdatesBtn.IsEnabled = true;

            UpdateStatus($"Started updates: {updatesPerSec}/sec");
        }

        private void Case1StopUpdatesBtn_Click(object sender, RoutedEventArgs e)
        {
            _case1UpdateTimer?.Stop();

            Case1StartUpdatesBtn.IsEnabled = true;
            Case1StopUpdatesBtn.IsEnabled = false;

            UpdateStatus("Updates stopped");
        }

        #endregion

        #region Case Study 2: Image Gallery

        private void Case2LoadBtn_Click(object sender, RoutedEventArgs e)
        {
            int imageCount = (int)Case2ImageCountSlider.Value;
            int thumbnailSize = (int)Case2ThumbnailSizeSlider.Value;

            var sw = Stopwatch.StartNew();
            Case2BeforeControl.LoadImages(imageCount, thumbnailSize);
            sw.Stop();
            long beforeTime = sw.ElapsedMilliseconds;
            long beforeMemory = GC.GetTotalMemory(false);

            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();

            sw.Restart();
            Case2AfterControl.LoadImages(imageCount, thumbnailSize);
            sw.Stop();
            long afterTime = sw.ElapsedMilliseconds;
            long afterMemory = GC.GetTotalMemory(false);

            Case2BeforeStats.Text = $"Load Time: {beforeTime} ms\n" +
                                   $"Memory: {beforeMemory / (1024.0 * 1024.0):F2} MB\n" +
                                   $"Images: {imageCount}\n" +
                                   $"Avg: {(double)beforeTime / imageCount:F2} ms/image";

            Case2AfterStats.Text = $"Load Time: {afterTime} ms\n" +
                                  $"Memory: {afterMemory / (1024.0 * 1024.0):F2} MB\n" +
                                  $"Images: {imageCount}\n" +
                                  $"Avg: {(double)afterTime / imageCount:F2} ms/image\n" +
                                  $"Speedup: {(double)beforeTime / afterTime:F2}x faster\n" +
                                  $"Memory saved: {(beforeMemory - afterMemory) / (1024.0 * 1024.0):F2} MB";

            UpdateStatus($"Images loaded: {imageCount}. Before: {beforeTime}ms, After: {afterTime}ms");
            UpdateSummary();
        }

        private void Case2ClearBtn_Click(object sender, RoutedEventArgs e)
        {
            Case2BeforeControl.Clear();
            Case2AfterControl.Clear();

            UpdateStatus("Gallery cleared");
        }

        private void Case2GCBtn_Click(object sender, RoutedEventArgs e)
        {
            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();

            UpdateStatus("Garbage collection forced");
        }

        #endregion

        #region Case Study 3: Large Tree Structure

        private void Case3BuildBtn_Click(object sender, RoutedEventArgs e)
        {
            int depth = (int)Case3DepthSlider.Value;
            int children = (int)Case3ChildrenSlider.Value;

            var sw = Stopwatch.StartNew();
            Case3BeforeControl.BuildTree(depth, children);
            sw.Stop();
            long beforeTime = sw.ElapsedMilliseconds;
            int beforeNodes = Case3BeforeControl.GetNodeCount();

            sw.Restart();
            Case3AfterControl.BuildTree(depth, children);
            sw.Stop();
            long afterTime = sw.ElapsedMilliseconds;
            int afterNodes = Case3AfterControl.GetNodeCount();

            Case3BeforeStats.Text = $"Build Time: {beforeTime} ms\n" +
                                   $"Total Nodes: {beforeNodes:N0}\n" +
                                   $"Depth: {depth}\n" +
                                   $"Children/Node: {children}\n" +
                                   $"All nodes loaded";

            Case3AfterStats.Text = $"Build Time: {afterTime} ms\n" +
                                  $"Total Nodes: {afterNodes:N0}\n" +
                                  $"Depth: {depth}\n" +
                                  $"Children/Node: {children}\n" +
                                  $"Lazy loaded\n" +
                                  $"Speedup: {(double)beforeTime / Math.Max(1, afterTime):F2}x faster";

            UpdateStatus($"Tree built: {afterNodes:N0} nodes. Before: {beforeTime}ms, After: {afterTime}ms");
            UpdateSummary();
        }

        private void Case3ExpandBtn_Click(object sender, RoutedEventArgs e)
        {
            var sw = Stopwatch.StartNew();
            Case3BeforeControl.ExpandAll();
            sw.Stop();
            long beforeTime = sw.ElapsedMilliseconds;

            sw.Restart();
            Case3AfterControl.ExpandAll();
            sw.Stop();
            long afterTime = sw.ElapsedMilliseconds;

            UpdateStatus($"Expand All: Before {beforeTime}ms, After {afterTime}ms");
        }

        private void Case3CollapseBtn_Click(object sender, RoutedEventArgs e)
        {
            Case3BeforeControl.CollapseAll();
            Case3AfterControl.CollapseAll();

            UpdateStatus("Tree collapsed");
        }

        #endregion

        private void UpdateSummary()
        {
            Summary1Before.Text = "• Load Time: High\n" +
                                 "• Memory Usage: High\n" +
                                 "• UI Responsiveness: Poor\n" +
                                 "• Scroll Performance: Slow";

            Summary1After.Text = "• Load Time: Low (virtualized)\n" +
                                "• Memory Usage: Low (only visible)\n" +
                                "• UI Responsiveness: Excellent\n" +
                                "• Scroll Performance: Smooth";

            Summary2Before.Text = "• Load Time: Very High\n" +
                                 "• Memory Usage: Excessive\n" +
                                 "• Image Quality: Full resolution waste\n" +
                                 "• Memory Leaks: Yes";

            Summary2After.Text = "• Load Time: Fast (optimized decode)\n" +
                                "• Memory Usage: Minimal\n" +
                                "• Image Quality: Optimal for display\n" +
                                "• Memory Leaks: No";

            Summary3Before.Text = "• Load Time: Very Slow\n" +
                                 "• Memory Usage: All nodes loaded\n" +
                                 "• Expansion: Slow\n" +
                                 "• Initial Display: Slow";

            Summary3After.Text = "• Load Time: Instant\n" +
                                "• Memory Usage: Only loaded nodes\n" +
                                "• Expansion: Fast (lazy load)\n" +
                                "• Initial Display: Instant";

            OverallSummary.Text = "TYPICAL PERFORMANCE IMPROVEMENTS:\n\n" +
                                 "Case 1 (Data Grid):\n" +
                                 "  • Load time: 80-95% faster with virtualization\n" +
                                 "  • Memory: 90%+ reduction (only visible rows)\n" +
                                 "  • Scroll FPS: 10-15 FPS → 60 FPS\n\n" +
                                 "Case 2 (Image Gallery):\n" +
                                 "  • Load time: 70-90% faster with DecodePixelWidth\n" +
                                 "  • Memory: 85-95% reduction (thumbnail-sized decode)\n" +
                                 "  • Example: 100 images from 500MB → 50MB\n\n" +
                                 "Case 3 (Tree View):\n" +
                                 "  • Initial load: 95%+ faster with lazy loading\n" +
                                 "  • Memory: 80%+ reduction (only expanded nodes)\n" +
                                 "  • Expansion: Near-instant vs multi-second delays\n\n" +
                                 "KEY TAKEAWAY: Proper optimization can improve performance by 10-100x!";
        }

        private void UpdateStatus(string message)
        {
            StatusText.Text = message;
        }

        protected override void OnClosed(EventArgs e)
        {
            _case1UpdateTimer?.Stop();
            _memoryTimer?.Stop();
            base.OnClosed(e);
        }
    }
}
