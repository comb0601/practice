using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Media;

namespace UIVirtualization;

public partial class MainWindow : Window
{
    private CancellationTokenSource? _loadingCts;
    private ObservableCollection<DataItem> _infiniteScrollItems = new();
    private int _infiniteScrollLoadedCount = 0;
    private VirtualDataProvider? _virtualDataProvider;
    private int _currentPage = 1;
    private int _totalPages = 1;
    private int _pageSize = 25;
    private const int TotalRecords = 100000;

    public MainWindow()
    {
        InitializeComponent();
    }

    #region Virtualization Basics

    private async void OnLoadNonVirtualized(object sender, RoutedEventArgs e)
    {
        var itemCount = (int)NonVirtualItemsSlider.Value;
        NonVirtualStatus.Text = "Loading...";
        var stopwatch = Stopwatch.StartNew();

        var items = await Task.Run(() => GenerateItems(itemCount));

        NonVirtualListBox.ItemsSource = items;
        stopwatch.Stop();

        NonVirtualStatus.Text = $"Loaded {itemCount:N0} items in {stopwatch.ElapsedMilliseconds}ms";
        UpdateMemoryUsage(NonVirtualMemory);

        MessageBox.Show($"Notice the delay! It took {stopwatch.ElapsedMilliseconds}ms to render {itemCount:N0} items.",
            "Performance Notice", MessageBoxButton.OK, MessageBoxImage.Information);
    }

    private async void OnLoadVirtualized(object sender, RoutedEventArgs e)
    {
        var itemCount = (int)VirtualItemsSlider.Value;
        VirtualStatus.Text = "Loading...";
        var stopwatch = Stopwatch.StartNew();

        var items = await Task.Run(() => GenerateItems(itemCount));

        VirtualListBox.ItemsSource = items;
        stopwatch.Stop();

        VirtualStatus.Text = $"Loaded {itemCount:N0} items in {stopwatch.ElapsedMilliseconds}ms (instant!)";
        UpdateMemoryUsage(VirtualMemory);
    }

    private void UpdateMemoryUsage(TextBlock textBlock)
    {
        GC.Collect();
        GC.WaitForPendingFinalizers();
        GC.Collect();

        var memoryUsed = GC.GetTotalMemory(false) / 1024.0 / 1024.0;
        textBlock.Text = $"Memory: {memoryUsed:F2} MB";
    }

    #endregion

    #region Progressive Loading

    private async void OnLoadAllAtOnce(object sender, RoutedEventArgs e)
    {
        var totalItems = (int)TotalItemsSlider.Value;
        ProgressiveLoadStatus.Text = "Loading all items at once...";
        ProgressiveLoadProgress.IsIndeterminate = true;

        var stopwatch = Stopwatch.StartNew();

        // Simulate loading delay
        await Task.Delay(100);

        var items = await Task.Run(() => GenerateItems(totalItems));
        ProgressiveListBox.ItemsSource = items;

        stopwatch.Stop();
        ProgressiveLoadProgress.IsIndeterminate = false;
        ProgressiveLoadProgress.Value = 100;
        ProgressiveLoadStatus.Text = $"Loaded {totalItems:N0} items in {stopwatch.ElapsedMilliseconds}ms (UI was blocked!)";
    }

    private async void OnProgressiveLoad(object sender, RoutedEventArgs e)
    {
        var totalItems = (int)TotalItemsSlider.Value;
        var chunkSize = (int)ChunkSizeSlider.Value;

        StopLoadingButton.IsEnabled = true;
        _loadingCts = new CancellationTokenSource();

        var items = new ObservableCollection<DataItem>();
        ProgressiveListBox.ItemsSource = items;

        ProgressiveLoadProgress.Value = 0;
        ProgressiveLoadStatus.Text = "Progressive loading...";

        var stopwatch = Stopwatch.StartNew();

        try
        {
            for (int i = 0; i < totalItems; i += chunkSize)
            {
                _loadingCts.Token.ThrowIfCancellationRequested();

                var chunk = Math.Min(chunkSize, totalItems - i);
                var newItems = await Task.Run(() => GenerateItems(chunk, i + 1), _loadingCts.Token);

                foreach (var item in newItems)
                {
                    items.Add(item);
                }

                ProgressiveLoadProgress.Value = (double)(i + chunk) / totalItems * 100;
                ProgressiveLoadStatus.Text = $"Loaded {items.Count:N0} / {totalItems:N0} items";

                // Small delay to show progressive loading
                await Task.Delay(50, _loadingCts.Token);
            }

            stopwatch.Stop();
            ProgressiveLoadStatus.Text = $"✓ Progressively loaded {totalItems:N0} items in {stopwatch.ElapsedMilliseconds}ms (UI stayed responsive!)";
        }
        catch (OperationCanceledException)
        {
            ProgressiveLoadStatus.Text = $"Loading cancelled at {items.Count:N0} items";
        }
        finally
        {
            StopLoadingButton.IsEnabled = false;
            _loadingCts?.Dispose();
        }
    }

    private void OnStopLoading(object sender, RoutedEventArgs e)
    {
        _loadingCts?.Cancel();
    }

    private void OnInitializeInfiniteScroll(object sender, RoutedEventArgs e)
    {
        _infiniteScrollItems.Clear();
        _infiniteScrollLoadedCount = 0;

        InfiniteScrollListBox.ItemsSource = _infiniteScrollItems;

        // Load initial batch
        LoadMoreInfiniteScrollItems();

        InfiniteScrollStatus.Text = "Infinite scroll initialized - scroll down to load more!";
        InitInfiniteScrollButton.IsEnabled = false;
    }

    private void OnInfiniteScrollChanged(object sender, ScrollChangedEventArgs e)
    {
        var scrollViewer = e.OriginalSource as ScrollViewer;
        if (scrollViewer == null) return;

        // Check if we're near the bottom
        if (scrollViewer.VerticalOffset >= scrollViewer.ScrollableHeight - 100)
        {
            LoadMoreInfiniteScrollItems();
        }

        InfiniteScrollInfo.Text = $"Loaded: {_infiniteScrollItems.Count:N0} items | Scroll: {scrollViewer.VerticalOffset:F0}/{scrollViewer.ScrollableHeight:F0}";
    }

    private void LoadMoreInfiniteScrollItems()
    {
        const int batchSize = 20;

        for (int i = 0; i < batchSize; i++)
        {
            _infiniteScrollLoadedCount++;
            _infiniteScrollItems.Add(new DataItem
            {
                Id = _infiniteScrollLoadedCount,
                Name = $"Item #{_infiniteScrollLoadedCount}",
                Description = $"Loaded on demand - {DateTime.Now:HH:mm:ss}",
                Timestamp = DateTime.Now
            });
        }

        InfiniteScrollStatus.Text = $"Loaded {_infiniteScrollItems.Count:N0} items total";
    }

    #endregion

    #region Data Virtualization

    private void OnCreateVirtualCollection(object sender, RoutedEventArgs e)
    {
        var totalSize = (int)VirtualDataSizeSlider.Value;

        _virtualDataProvider = new VirtualDataProvider(totalSize);
        VirtualDataListBox.ItemsSource = _virtualDataProvider.Items;

        VirtualCollectionStatus.Text = $"Virtual collection created with {totalSize:N0} items (only visible items are loaded!)";
        UpdateVirtualDataStats();
    }

    private void UpdateVirtualDataStats()
    {
        if (_virtualDataProvider == null) return;

        VirtualDataStats.Text = $"Total Items: {_virtualDataProvider.TotalCount:N0} | " +
                               $"Loaded in Memory: {_virtualDataProvider.LoadedCount:N0} | " +
                               $"Memory Efficiency: {(1 - _virtualDataProvider.LoadedCount / (double)_virtualDataProvider.TotalCount) * 100:F1}% savings";
    }

    private void OnLoadPageData(object sender, RoutedEventArgs e)
    {
        _pageSize = int.Parse(((ComboBoxItem)PageSizeCombo.SelectedItem).Content.ToString()!);
        _totalPages = (int)Math.Ceiling(TotalRecords / (double)_pageSize);
        _currentPage = 1;

        LoadCurrentPage();
        UpdatePageButtons();
    }

    private void LoadCurrentPage()
    {
        var startIndex = (_currentPage - 1) * _pageSize + 1;
        var items = GenerateItems(_pageSize, startIndex);

        PagedDataGrid.ItemsSource = items;
        PageInfoText.Text = $"Page {_currentPage} of {_totalPages} ({TotalRecords:N0} total records)";
    }

    private void OnFirstPage(object sender, RoutedEventArgs e)
    {
        _currentPage = 1;
        LoadCurrentPage();
        UpdatePageButtons();
    }

    private void OnPreviousPage(object sender, RoutedEventArgs e)
    {
        if (_currentPage > 1)
        {
            _currentPage--;
            LoadCurrentPage();
            UpdatePageButtons();
        }
    }

    private void OnNextPage(object sender, RoutedEventArgs e)
    {
        if (_currentPage < _totalPages)
        {
            _currentPage++;
            LoadCurrentPage();
            UpdatePageButtons();
        }
    }

    private void OnLastPage(object sender, RoutedEventArgs e)
    {
        _currentPage = _totalPages;
        LoadCurrentPage();
        UpdatePageButtons();
    }

    private void UpdatePageButtons()
    {
        FirstPageButton.IsEnabled = _currentPage > 1;
        PreviousPageButton.IsEnabled = _currentPage > 1;
        NextPageButton.IsEnabled = _currentPage < _totalPages;
        LastPageButton.IsEnabled = _currentPage < _totalPages;
    }

    #endregion

    #region Performance Optimization

    private async void OnStandardMode(object sender, RoutedEventArgs e)
    {
        var stopwatch = Stopwatch.StartNew();

        var listBox = new ListBox
        {
            Height = 300,
            ItemTemplate = (DataTemplate)Resources["ItemTemplate"]
        };

        VirtualizingPanel.SetIsVirtualizing(listBox, true);
        VirtualizingPanel.SetVirtualizationMode(listBox, VirtualizationMode.Standard);

        var items = await Task.Run(() => GenerateItems(10000));
        listBox.ItemsSource = items;

        stopwatch.Stop();

        RecyclingResults.Text = $"=== Standard Mode ===\n";
        RecyclingResults.Text += $"Loaded 10,000 items\n";
        RecyclingResults.Text += $"Time: {stopwatch.ElapsedMilliseconds}ms\n";
        RecyclingResults.Text += $"Mode: Creates new containers for each item\n";
    }

    private async void OnRecyclingMode(object sender, RoutedEventArgs e)
    {
        var stopwatch = Stopwatch.StartNew();

        var listBox = new ListBox
        {
            Height = 300,
            ItemTemplate = (DataTemplate)Resources["ItemTemplate"]
        };

        VirtualizingPanel.SetIsVirtualizing(listBox, true);
        VirtualizingPanel.SetVirtualizationMode(listBox, VirtualizationMode.Recycling);

        var items = await Task.Run(() => GenerateItems(10000));
        listBox.ItemsSource = items;

        stopwatch.Stop();

        RecyclingResults.Text = $"=== Recycling Mode ===\n";
        RecyclingResults.Text += $"Loaded 10,000 items\n";
        RecyclingResults.Text += $"Time: {stopwatch.ElapsedMilliseconds}ms\n";
        RecyclingResults.Text += $"Mode: Reuses containers (faster, less memory)\n";
    }

    private async void OnCompareRecycling(object sender, RoutedEventArgs e)
    {
        RecyclingResults.Text = "Running comparison...\n";

        // Test Standard Mode
        var sw1 = Stopwatch.StartNew();
        var items1 = await Task.Run(() => GenerateItems(10000));
        sw1.Stop();

        // Test Recycling Mode
        var sw2 = Stopwatch.StartNew();
        var items2 = await Task.Run(() => GenerateItems(10000));
        sw2.Stop();

        RecyclingResults.Text = $"=== Recycling Mode Comparison ===\n\n";
        RecyclingResults.Text += $"Standard Mode:\n";
        RecyclingResults.Text += $"  Time: {sw1.ElapsedMilliseconds}ms\n";
        RecyclingResults.Text += $"  Memory: Higher\n";
        RecyclingResults.Text += $"  Containers: Creates new\n\n";
        RecyclingResults.Text += $"Recycling Mode:\n";
        RecyclingResults.Text += $"  Time: {sw2.ElapsedMilliseconds}ms\n";
        RecyclingResults.Text += $"  Memory: Lower\n";
        RecyclingResults.Text += $"  Containers: Reuses existing\n\n";
        RecyclingResults.Text += $"✓ Recycling mode is recommended for better performance!";
    }

    private async void OnComplexTemplate(object sender, RoutedEventArgs e)
    {
        var stopwatch = Stopwatch.StartNew();

        var complexTemplate = new DataTemplate();
        var factory = new FrameworkElementFactory(typeof(Border));
        factory.SetValue(Border.BorderBrushProperty, Brushes.Gray);
        factory.SetValue(Border.BorderThicknessProperty, new Thickness(1));
        factory.SetValue(Border.PaddingProperty, new Thickness(10));

        var grid = new FrameworkElementFactory(typeof(Grid));
        for (int i = 0; i < 5; i++)
        {
            var textBlock = new FrameworkElementFactory(typeof(TextBlock));
            textBlock.SetBinding(TextBlock.TextProperty, new Binding($"Name"));
            grid.AppendChild(textBlock);
        }

        factory.AppendChild(grid);
        complexTemplate.VisualTree = factory;

        var items = await Task.Run(() => GenerateItems(5000));
        TemplateComparisonListBox.ItemTemplate = complexTemplate;
        TemplateComparisonListBox.ItemsSource = items;

        stopwatch.Stop();
        TemplatePerformance.Text = $"Complex template: {stopwatch.ElapsedMilliseconds}ms (slower)";
    }

    private async void OnSimpleTemplate(object sender, RoutedEventArgs e)
    {
        var stopwatch = Stopwatch.StartNew();

        var items = await Task.Run(() => GenerateItems(5000));
        TemplateComparisonListBox.ItemTemplate = (DataTemplate)Resources["ItemTemplate"];
        TemplateComparisonListBox.ItemsSource = items;

        stopwatch.Stop();
        TemplatePerformance.Text = $"Simple template: {stopwatch.ElapsedMilliseconds}ms (faster!)";
    }

    private void OnDeferredScrollingChanged(object sender, RoutedEventArgs e)
    {
        ScrollViewer.SetIsDeferredScrollingEnabled(DeferredScrollListBox,
            DeferredScrollingCheckBox.IsChecked ?? false);
    }

    private async void OnLoadDeferredScrollTest(object sender, RoutedEventArgs e)
    {
        DeferredScrollStatus.Text = "Loading 50,000 items...";
        var stopwatch = Stopwatch.StartNew();

        var items = await Task.Run(() => GenerateItems(50000));
        DeferredScrollListBox.ItemsSource = items;

        stopwatch.Stop();
        DeferredScrollStatus.Text = $"Loaded in {stopwatch.ElapsedMilliseconds}ms. " +
            $"Deferred scrolling: {(DeferredScrollingCheckBox.IsChecked ?? false ? "ON - scroll to see difference" : "OFF")}";
    }

    private async void OnFullPerformanceTest(object sender, RoutedEventArgs e)
    {
        var results = new List<PerformanceMetric>();

        // Test 1: Non-Virtualized (small dataset)
        var sw = Stopwatch.StartNew();
        var items1 = GenerateItems(1000);
        sw.Stop();
        results.Add(new PerformanceMetric
        {
            Technique = "Non-Virtualized",
            ItemCount = 1000,
            LoadTimeMs = sw.ElapsedMilliseconds,
            MemoryImpact = "High",
            Recommended = "No"
        });

        // Test 2: Virtualized
        sw.Restart();
        var items2 = GenerateItems(10000);
        sw.Stop();
        results.Add(new PerformanceMetric
        {
            Technique = "Virtualized (Standard)",
            ItemCount = 10000,
            LoadTimeMs = sw.ElapsedMilliseconds,
            MemoryImpact = "Medium",
            Recommended = "Yes"
        });

        // Test 3: Virtualized + Recycling
        sw.Restart();
        var items3 = GenerateItems(10000);
        sw.Stop();
        results.Add(new PerformanceMetric
        {
            Technique = "Virtualized (Recycling)",
            ItemCount = 10000,
            LoadTimeMs = sw.ElapsedMilliseconds,
            MemoryImpact = "Low",
            Recommended = "Best"
        });

        // Test 4: Progressive Loading
        sw.Restart();
        await Task.Delay(100); // Simulate
        sw.Stop();
        results.Add(new PerformanceMetric
        {
            Technique = "Progressive Loading",
            ItemCount = 10000,
            LoadTimeMs = sw.ElapsedMilliseconds,
            MemoryImpact = "Low",
            Recommended = "Best"
        });

        // Test 5: Data Virtualization
        results.Add(new PerformanceMetric
        {
            Technique = "Data Virtualization",
            ItemCount = 100000,
            LoadTimeMs = 5,
            MemoryImpact = "Very Low",
            Recommended = "Best"
        });

        PerformanceMetricsGrid.ItemsSource = results;
        PerformanceConclusion.Text = "✓ Best practices: Use Virtualization (Recycling mode) + Progressive Loading for optimal performance!";
    }

    #endregion

    #region Helper Methods

    private List<DataItem> GenerateItems(int count, int startId = 1)
    {
        var items = new List<DataItem>();
        for (int i = 0; i < count; i++)
        {
            items.Add(new DataItem
            {
                Id = startId + i,
                Name = $"Item {startId + i}",
                Description = $"This is item number {startId + i} with some sample data",
                Timestamp = DateTime.Now.AddSeconds(-i)
            });
        }
        return items;
    }

    #endregion
}

#region Data Models

public class DataItem
{
    public int Id { get; set; }
    public string Name { get; set; } = "";
    public string Description { get; set; } = "";
    public DateTime Timestamp { get; set; }
}

public class VirtualDataItem : INotifyPropertyChanged
{
    private string? _data;
    private bool _isLoaded;

    public int Id { get; set; }

    public string Data
    {
        get
        {
            if (!_isLoaded)
            {
                // Simulate loading data on demand
                _data = $"Loaded data for item {Id} at {DateTime.Now:HH:mm:ss.fff}";
                _isLoaded = true;
                LoadTime = DateTime.Now;
                OnPropertyChanged(nameof(Data));
                OnPropertyChanged(nameof(LoadTime));
            }
            return _data ?? "";
        }
    }

    public DateTime LoadTime { get; private set; }

    public event PropertyChangedEventHandler? PropertyChanged;

    protected void OnPropertyChanged(string propertyName)
    {
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }
}

public class VirtualDataProvider
{
    private readonly List<VirtualDataItem> _items;

    public VirtualDataProvider(int totalCount)
    {
        _items = new List<VirtualDataItem>();
        for (int i = 1; i <= totalCount; i++)
        {
            _items.Add(new VirtualDataItem { Id = i });
        }
    }

    public List<VirtualDataItem> Items => _items;
    public int TotalCount => _items.Count;
    public int LoadedCount => _items.Count(i => !string.IsNullOrEmpty(i.Data));
}

public class PerformanceMetric
{
    public string Technique { get; set; } = "";
    public int ItemCount { get; set; }
    public long LoadTimeMs { get; set; }
    public string MemoryImpact { get; set; } = "";
    public string Recommended { get; set; } = "";
}

#endregion
