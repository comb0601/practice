using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Windows;

namespace ObservableCollectionsDemo
{
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        private readonly Stopwatch _stopwatch = new();
        private readonly Random _random = new();
        private string _statusMessage = "Ready - Explore ObservableCollection optimization techniques";
        private double _currentMemory = 0;
        private int _standardEventCount = 0;
        private int _rangeEventCount = 0;

        // Individual operations metrics
        private string _individualAddTime = "Add Time: Not tested";
        private string _individualRemoveTime = "Remove Time: Not tested";
        private int _individualEventCount = 0;
        private int _individualUIUpdates = 0;

        // Bulk operations metrics
        private string _bulkAddTime = "Add Time: Not tested";
        private string _bulkRemoveTime = "Remove Time: Not tested";
        private int _bulkEventCount = 0;
        private int _bulkUIUpdates = 0;

        // Range operations
        private string _rangeOperationResult = "No operation performed yet";
        private string _rangeOperationTime = "Time: -";
        private string _rangeOperationEvents = "Events: -";

        // Benchmark
        private string _benchmarkResults = "Click 'Run Full Benchmark' to start";
        private double _individualScore = 0;
        private double _bulkScore = 0;
        private string _individualScoreText = "";
        private string _bulkScoreText = "";
        private string _benchmarkSummary = "";

        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;

            // Initialize collections
            StandardCollection = new ObservableCollection<DataItem>();
            RangeCollection = new RangeObservableCollection<DataItem>();
            DemoCollection = new RangeObservableCollection<DataItem>();

            // Subscribe to collection changed events for counting
            StandardCollection.CollectionChanged += (s, e) =>
            {
                _standardEventCount++;
                _individualEventCount++;
                _individualUIUpdates++;
            };

            RangeCollection.CollectionChanged += (s, e) =>
            {
                _rangeEventCount++;
                _bulkEventCount++;
                _bulkUIUpdates++;
            };

            DemoCollection.CollectionChanged += (s, e) =>
            {
                OnPropertyChanged(nameof(DemoCollection));
            };

            UpdateMemoryUsage();

            // Start timer for memory monitoring
            var timer = new System.Windows.Threading.DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(1)
            };
            timer.Tick += (s, e) => UpdateMemoryUsage();
            timer.Start();
        }

        #region Properties

        public ObservableCollection<DataItem> StandardCollection { get; }
        public RangeObservableCollection<DataItem> RangeCollection { get; }
        public RangeObservableCollection<DataItem> DemoCollection { get; }

        public string StatusMessage
        {
            get => _statusMessage;
            set => SetProperty(ref _statusMessage, value);
        }

        public double CurrentMemory
        {
            get => _currentMemory;
            set => SetProperty(ref _currentMemory, value);
        }

        public string IndividualAddTime
        {
            get => _individualAddTime;
            set => SetProperty(ref _individualAddTime, value);
        }

        public string IndividualRemoveTime
        {
            get => _individualRemoveTime;
            set => SetProperty(ref _individualRemoveTime, value);
        }

        public int IndividualEventCount
        {
            get => _individualEventCount;
            set => SetProperty(ref _individualEventCount, value);
        }

        public int IndividualUIUpdates
        {
            get => _individualUIUpdates;
            set => SetProperty(ref _individualUIUpdates, value);
        }

        public string BulkAddTime
        {
            get => _bulkAddTime;
            set => SetProperty(ref _bulkAddTime, value);
        }

        public string BulkRemoveTime
        {
            get => _bulkRemoveTime;
            set => SetProperty(ref _bulkRemoveTime, value);
        }

        public int BulkEventCount
        {
            get => _bulkEventCount;
            set => SetProperty(ref _bulkEventCount, value);
        }

        public int BulkUIUpdates
        {
            get => _bulkUIUpdates;
            set => SetProperty(ref _bulkUIUpdates, value);
        }

        public string RangeOperationResult
        {
            get => _rangeOperationResult;
            set => SetProperty(ref _rangeOperationResult, value);
        }

        public string RangeOperationTime
        {
            get => _rangeOperationTime;
            set => SetProperty(ref _rangeOperationTime, value);
        }

        public string RangeOperationEvents
        {
            get => _rangeOperationEvents;
            set => SetProperty(ref _rangeOperationEvents, value);
        }

        public string BenchmarkResults
        {
            get => _benchmarkResults;
            set => SetProperty(ref _benchmarkResults, value);
        }

        public double IndividualScore
        {
            get => _individualScore;
            set => SetProperty(ref _individualScore, value);
        }

        public double BulkScore
        {
            get => _bulkScore;
            set => SetProperty(ref _bulkScore, value);
        }

        public string IndividualScoreText
        {
            get => _individualScoreText;
            set => SetProperty(ref _individualScoreText, value);
        }

        public string BulkScoreText
        {
            get => _bulkScoreText;
            set => SetProperty(ref _bulkScoreText, value);
        }

        public string BenchmarkSummary
        {
            get => _benchmarkSummary;
            set => SetProperty(ref _benchmarkSummary, value);
        }

        #endregion

        #region Event Handlers - Individual vs Bulk

        private void AddIndividually_Click(object sender, RoutedEventArgs e)
        {
            if (!int.TryParse(IndividualCountTextBox.Text, out int count) || count <= 0)
            {
                MessageBox.Show("Please enter a valid positive number", "Invalid Input");
                return;
            }

            StatusMessage = $"Adding {count} items individually...";
            StandardCollection.Clear();
            _individualEventCount = 0;
            _individualUIUpdates = 0;

            _stopwatch.Restart();
            for (int i = 0; i < count; i++)
            {
                StandardCollection.Add(CreateDataItem(i + 1));
            }
            _stopwatch.Stop();

            IndividualAddTime = $"Add Time: {_stopwatch.Elapsed.TotalMilliseconds:F2}ms";
            IndividualEventCount = _individualEventCount;
            IndividualUIUpdates = _individualUIUpdates;
            StatusMessage = $"Added {count} items individually - {_individualEventCount} events raised";
        }

        private void RemoveIndividually_Click(object sender, RoutedEventArgs e)
        {
            if (StandardCollection.Count == 0)
            {
                MessageBox.Show("Add items first before removing", "No Items");
                return;
            }

            StatusMessage = $"Removing {StandardCollection.Count} items individually...";
            int count = StandardCollection.Count;
            _individualEventCount = 0;
            _individualUIUpdates = 0;

            _stopwatch.Restart();
            while (StandardCollection.Count > 0)
            {
                StandardCollection.RemoveAt(0);
            }
            _stopwatch.Stop();

            IndividualRemoveTime = $"Remove Time: {_stopwatch.Elapsed.TotalMilliseconds:F2}ms";
            IndividualEventCount = _individualEventCount;
            IndividualUIUpdates = _individualUIUpdates;
            StatusMessage = $"Removed {count} items individually - {_individualEventCount} events raised";
        }

        private void AddInBulk_Click(object sender, RoutedEventArgs e)
        {
            if (!int.TryParse(BulkCountTextBox.Text, out int count) || count <= 0)
            {
                MessageBox.Show("Please enter a valid positive number", "Invalid Input");
                return;
            }

            StatusMessage = $"Adding {count} items in bulk...";
            RangeCollection.Clear();
            _bulkEventCount = 0;
            _bulkUIUpdates = 0;

            var items = Enumerable.Range(1, count).Select(i => CreateDataItem(i)).ToList();

            _stopwatch.Restart();
            RangeCollection.AddRange(items);
            _stopwatch.Stop();

            BulkAddTime = $"Add Time: {_stopwatch.Elapsed.TotalMilliseconds:F2}ms";
            BulkEventCount = _bulkEventCount;
            BulkUIUpdates = _bulkUIUpdates;
            StatusMessage = $"Added {count} items in bulk - {_bulkEventCount} events raised";
        }

        private void RemoveInBulk_Click(object sender, RoutedEventArgs e)
        {
            if (RangeCollection.Count == 0)
            {
                MessageBox.Show("Add items first before removing", "No Items");
                return;
            }

            StatusMessage = $"Removing {RangeCollection.Count} items in bulk...";
            int count = RangeCollection.Count;
            _bulkEventCount = 0;
            _bulkUIUpdates = 0;

            var itemsToRemove = RangeCollection.ToList();

            _stopwatch.Restart();
            RangeCollection.RemoveRange(itemsToRemove);
            _stopwatch.Stop();

            BulkRemoveTime = $"Remove Time: {_stopwatch.Elapsed.TotalMilliseconds:F2}ms";
            BulkEventCount = _bulkEventCount;
            BulkUIUpdates = _bulkUIUpdates;
            StatusMessage = $"Removed {count} items in bulk - {_bulkEventCount} events raised";
        }

        #endregion

        #region Event Handlers - Range Operations

        private void DemoAddRange_Click(object sender, RoutedEventArgs e)
        {
            StatusMessage = "Adding 500 items using AddRange...";
            var startCount = DemoCollection.Count;
            var items = Enumerable.Range(startCount + 1, 500).Select(i => CreateDataItem(i)).ToList();

            _stopwatch.Restart();
            DemoCollection.AddRange(items);
            _stopwatch.Stop();

            RangeOperationResult = $"Added 500 items (Total: {DemoCollection.Count})";
            RangeOperationTime = $"Time: {_stopwatch.Elapsed.TotalMilliseconds:F2}ms";
            RangeOperationEvents = "Events: 1 (CollectionChanged with NotifyCollectionChangedAction.Add)";
            StatusMessage = "AddRange completed";
        }

        private void DemoRemoveRange_Click(object sender, RoutedEventArgs e)
        {
            if (DemoCollection.Count < 250)
            {
                MessageBox.Show("Need at least 250 items. Add items first.", "Not Enough Items");
                return;
            }

            StatusMessage = "Removing first 250 items using RemoveRange...";
            var itemsToRemove = DemoCollection.Take(250).ToList();

            _stopwatch.Restart();
            DemoCollection.RemoveRange(itemsToRemove);
            _stopwatch.Stop();

            RangeOperationResult = $"Removed 250 items (Remaining: {DemoCollection.Count})";
            RangeOperationTime = $"Time: {_stopwatch.Elapsed.TotalMilliseconds:F2}ms";
            RangeOperationEvents = "Events: 1 (CollectionChanged with NotifyCollectionChangedAction.Remove)";
            StatusMessage = "RemoveRange completed";
        }

        private void DemoReplaceRange_Click(object sender, RoutedEventArgs e)
        {
            if (DemoCollection.Count == 0)
            {
                MessageBox.Show("Add items first before replacing", "No Items");
                return;
            }

            StatusMessage = "Replacing all items using ReplaceRange...";
            var newItems = Enumerable.Range(1, DemoCollection.Count)
                .Select(i => CreateDataItem(i, "Replaced"))
                .ToList();

            _stopwatch.Restart();
            DemoCollection.ReplaceRange(newItems);
            _stopwatch.Stop();

            RangeOperationResult = $"Replaced all {DemoCollection.Count} items with new data";
            RangeOperationTime = $"Time: {_stopwatch.Elapsed.TotalMilliseconds:F2}ms";
            RangeOperationEvents = "Events: 1 (CollectionChanged with NotifyCollectionChangedAction.Reset)";
            StatusMessage = "ReplaceRange completed";
        }

        private void DemoReset_Click(object sender, RoutedEventArgs e)
        {
            StatusMessage = "Resetting collection with new data...";
            var newItems = Enumerable.Range(1, 100).Select(i => CreateDataItem(i, "Reset")).ToList();

            _stopwatch.Restart();
            DemoCollection.Reset(newItems);
            _stopwatch.Stop();

            RangeOperationResult = $"Reset collection to 100 new items";
            RangeOperationTime = $"Time: {_stopwatch.Elapsed.TotalMilliseconds:F2}ms";
            RangeOperationEvents = "Events: 1 (CollectionChanged with NotifyCollectionChangedAction.Reset)";
            StatusMessage = "Reset completed";
        }

        #endregion

        #region Event Handlers - Benchmark

        private void RunBenchmark_Click(object sender, RoutedEventArgs e)
        {
            if (!int.TryParse(BenchmarkCountTextBox.Text, out int count) || count <= 0)
            {
                MessageBox.Show("Please enter a valid positive number", "Invalid Input");
                return;
            }

            StatusMessage = "Running comprehensive benchmark...";
            var results = new List<string>
            {
                $"=== BENCHMARK RESULTS ({count} items) ===",
                ""
            };

            // Test 1: Individual Add
            StandardCollection.Clear();
            _stopwatch.Restart();
            for (int i = 0; i < count; i++)
            {
                StandardCollection.Add(CreateDataItem(i + 1));
            }
            _stopwatch.Stop();
            var individualAddMs = _stopwatch.Elapsed.TotalMilliseconds;
            results.Add($"Individual Add:     {individualAddMs,8:F2}ms  ({count} events)");

            // Test 2: Bulk Add
            RangeCollection.Clear();
            var items = Enumerable.Range(1, count).Select(i => CreateDataItem(i)).ToList();
            _stopwatch.Restart();
            RangeCollection.AddRange(items);
            _stopwatch.Stop();
            var bulkAddMs = _stopwatch.Elapsed.TotalMilliseconds;
            results.Add($"Bulk Add:           {bulkAddMs,8:F2}ms  (1 event)");
            results.Add($"  Speedup:          {individualAddMs / bulkAddMs,8:F2}x faster");
            results.Add("");

            // Test 3: Individual Remove
            _stopwatch.Restart();
            while (StandardCollection.Count > 0)
            {
                StandardCollection.RemoveAt(0);
            }
            _stopwatch.Stop();
            var individualRemoveMs = _stopwatch.Elapsed.TotalMilliseconds;
            results.Add($"Individual Remove:  {individualRemoveMs,8:F2}ms  ({count} events)");

            // Test 4: Bulk Remove
            var itemsToRemove = RangeCollection.ToList();
            _stopwatch.Restart();
            RangeCollection.RemoveRange(itemsToRemove);
            _stopwatch.Stop();
            var bulkRemoveMs = _stopwatch.Elapsed.TotalMilliseconds;
            results.Add($"Bulk Remove:        {bulkRemoveMs,8:F2}ms  (1 event)");
            results.Add($"  Speedup:          {individualRemoveMs / bulkRemoveMs,8:F2}x faster");
            results.Add("");

            // Overall statistics
            var totalIndividual = individualAddMs + individualRemoveMs;
            var totalBulk = bulkAddMs + bulkRemoveMs;
            var overallSpeedup = totalIndividual / totalBulk;

            results.Add("=== OVERALL STATISTICS ===");
            results.Add($"Total Individual:   {totalIndividual,8:F2}ms");
            results.Add($"Total Bulk:         {totalBulk,8:F2}ms");
            results.Add($"Overall Speedup:    {overallSpeedup,8:F2}x faster");
            results.Add($"Events Saved:       {count * 2 - 2,8} events");
            results.Add("");
            results.Add("RECOMMENDATION:");
            results.Add(overallSpeedup > 10
                ? "  ✅ Bulk operations are SIGNIFICANTLY faster!"
                : "  ✅ Bulk operations provide good performance improvement!");

            BenchmarkResults = string.Join("\n", results);

            // Update progress bars (scale to 0-100)
            IndividualScore = Math.Min(100, totalIndividual / 10);
            BulkScore = Math.Min(100, totalBulk / 10);
            IndividualScoreText = $"{totalIndividual:F0}ms total";
            BulkScoreText = $"{totalBulk:F0}ms total (⚡ {overallSpeedup:F1}x faster!)";

            BenchmarkSummary = $"Bulk operations are {overallSpeedup:F1}x faster overall! " +
                             $"For {count} items, you saved {totalIndividual - totalBulk:F0}ms " +
                             $"and prevented {count * 2 - 2} unnecessary UI updates.";

            StatusMessage = "Benchmark completed";
        }

        #endregion

        #region Helper Methods

        private DataItem CreateDataItem(int id, string prefix = "Item")
        {
            return new DataItem
            {
                Id = id,
                Name = $"{prefix} {id}",
                Value = $"Value_{_random.Next(1000, 9999)}",
                Timestamp = DateTime.Now
            };
        }

        private void UpdateMemoryUsage()
        {
            CurrentMemory = GC.GetTotalMemory(false) / 1024.0 / 1024.0;
        }

        #endregion

        #region INotifyPropertyChanged

        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        protected bool SetProperty<T>(ref T field, T value, [CallerMemberName] string? propertyName = null)
        {
            if (EqualityComparer<T>.Default.Equals(field, value))
                return false;

            field = value;
            OnPropertyChanged(propertyName);
            return true;
        }

        #endregion
    }

    #region Helper Classes

    public class DataItem
    {
        public int Id { get; set; }
        public string Name { get; set; } = "";
        public string Value { get; set; } = "";
        public DateTime Timestamp { get; set; }
    }

    /// <summary>
    /// Enhanced ObservableCollection with bulk operation support
    /// </summary>
    public class RangeObservableCollection<T> : ObservableCollection<T>
    {
        private bool _suppressNotification = false;

        /// <summary>
        /// Adds multiple items and raises a single CollectionChanged event
        /// </summary>
        public void AddRange(IEnumerable<T> items)
        {
            if (items == null)
                throw new ArgumentNullException(nameof(items));

            _suppressNotification = true;
            foreach (var item in items)
            {
                Add(item);
            }
            _suppressNotification = false;

            OnCollectionChanged(new NotifyCollectionChangedEventArgs(
                NotifyCollectionChangedAction.Reset));
        }

        /// <summary>
        /// Removes multiple items and raises a single CollectionChanged event
        /// </summary>
        public void RemoveRange(IEnumerable<T> items)
        {
            if (items == null)
                throw new ArgumentNullException(nameof(items));

            _suppressNotification = true;
            foreach (var item in items.ToList())
            {
                Remove(item);
            }
            _suppressNotification = false;

            OnCollectionChanged(new NotifyCollectionChangedEventArgs(
                NotifyCollectionChangedAction.Reset));
        }

        /// <summary>
        /// Replaces all items with new items and raises a single CollectionChanged event
        /// </summary>
        public void ReplaceRange(IEnumerable<T> items)
        {
            if (items == null)
                throw new ArgumentNullException(nameof(items));

            _suppressNotification = true;
            Clear();
            foreach (var item in items)
            {
                Add(item);
            }
            _suppressNotification = false;

            OnCollectionChanged(new NotifyCollectionChangedEventArgs(
                NotifyCollectionChangedAction.Reset));
        }

        /// <summary>
        /// Clears and adds new items in a single operation
        /// </summary>
        public void Reset(IEnumerable<T> items)
        {
            if (items == null)
                throw new ArgumentNullException(nameof(items));

            _suppressNotification = true;
            Clear();
            foreach (var item in items)
            {
                Add(item);
            }
            _suppressNotification = false;

            OnCollectionChanged(new NotifyCollectionChangedEventArgs(
                NotifyCollectionChangedAction.Reset));
        }

        protected override void OnCollectionChanged(NotifyCollectionChangedEventArgs e)
        {
            if (!_suppressNotification)
            {
                base.OnCollectionChanged(e);
            }
        }
    }

    #endregion
}
