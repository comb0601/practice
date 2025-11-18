using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;

namespace BindingPerformance
{
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        private readonly Random _random = new();
        private readonly Stopwatch _stopwatch = new();
        private string _oneWayValue = "Initial OneWay Value";
        private string _twoWayValue = "Initial TwoWay Value";
        private string _oneWayBindingTime = "OneWay: Not measured";
        private string _twoWayBindingTime = "TwoWay: Not measured";
        private string _performanceDifference = "Difference: Run test to compare";
        private int _propertyChangedCount = 0;
        private int _lostFocusCount = 0;
        private int _explicitCount = 0;
        private string _propertyChangedText = "";
        private string _lostFocusText = "";
        private string _explicitText = "";
        private string _statusMessage = "Ready - Explore different binding performance scenarios";
        private double _currentMemory = 0;
        private string _pathBenchmarkResults = "Click 'Benchmark Paths' to run tests";
        private string _creationTime = "Creation Time: Not tested";
        private string _updateTime = "Update Time: Not tested";
        private string _memoryUsage = "Memory Usage: Not tested";
        private string _updatePerformanceMetrics = "Type in the textboxes above to see update frequency metrics";

        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;
            UpdateMemoryUsage();

            // Start a timer to update memory usage
            var timer = new System.Windows.Threading.DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(1)
            };
            timer.Tick += (s, e) => UpdateMemoryUsage();
            timer.Start();
        }

        #region Properties

        public string OneWayValue
        {
            get => _oneWayValue;
            set => SetProperty(ref _oneWayValue, value);
        }

        public string TwoWayValue
        {
            get => _twoWayValue;
            set => SetProperty(ref _twoWayValue, value);
        }

        public string StaticValue => "This value never changes - OneTime binding is perfect!";

        public string OneWayBindingTime
        {
            get => _oneWayBindingTime;
            set => SetProperty(ref _oneWayBindingTime, value);
        }

        public string TwoWayBindingTime
        {
            get => _twoWayBindingTime;
            set => SetProperty(ref _twoWayBindingTime, value);
        }

        public string PerformanceDifference
        {
            get => _performanceDifference;
            set => SetProperty(ref _performanceDifference, value);
        }

        public int PropertyChangedCount
        {
            get => _propertyChangedCount;
            set => SetProperty(ref _propertyChangedCount, value);
        }

        public int LostFocusCount
        {
            get => _lostFocusCount;
            set => SetProperty(ref _lostFocusCount, value);
        }

        public int ExplicitCount
        {
            get => _explicitCount;
            set => SetProperty(ref _explicitCount, value);
        }

        public string PropertyChangedText
        {
            get => _propertyChangedText;
            set
            {
                if (SetProperty(ref _propertyChangedText, value))
                {
                    PropertyChangedCount++;
                    UpdatePerformanceMetrics();
                }
            }
        }

        public string LostFocusText
        {
            get => _lostFocusText;
            set
            {
                if (SetProperty(ref _lostFocusText, value))
                {
                    LostFocusCount++;
                    UpdatePerformanceMetrics();
                }
            }
        }

        public string ExplicitText
        {
            get => _explicitText;
            set
            {
                if (SetProperty(ref _explicitText, value))
                {
                    ExplicitCount++;
                    UpdatePerformanceMetrics();
                }
            }
        }

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

        public string PathBenchmarkResults
        {
            get => _pathBenchmarkResults;
            set => SetProperty(ref _pathBenchmarkResults, value);
        }

        public string CreationTime
        {
            get => _creationTime;
            set => SetProperty(ref _creationTime, value);
        }

        public string UpdateTime
        {
            get => _updateTime;
            set => SetProperty(ref _updateTime, value);
        }

        public string MemoryUsage
        {
            get => _memoryUsage;
            set => SetProperty(ref _memoryUsage, value);
        }

        public string UpdatePerformanceMetrics
        {
            get => _updatePerformanceMetrics;
            set => SetProperty(ref _updatePerformanceMetrics, value);
        }

        // Path optimization properties
        public string DirectProperty => $"Direct Access - Fast! ({DateTime.Now:HH:mm:ss})";

        public NestedObjectModel NestedObject { get; } = new NestedObjectModel();

        public ObservableCollection<ItemModel> Items { get; } = new ObservableCollection<ItemModel>
        {
            new ItemModel { Id = 1, Name = "First Item", Value = "Value 1" },
            new ItemModel { Id = 2, Name = "Second Item", Value = "Value 2" },
            new ItemModel { Id = 3, Name = "Third Item", Value = "Value 3" }
        };

        public string CachedComplexValue
        {
            get
            {
                // Simulate complex calculation
                var result = Math.Sqrt(DateTime.Now.Ticks % 10000);
                return $"Cached Result: {result:F2}";
            }
        }

        #endregion

        #region Event Handlers

        private void UpdateValues_Click(object sender, RoutedEventArgs e)
        {
            StatusMessage = "Updating binding values and measuring performance...";

            // Measure OneWay binding update
            _stopwatch.Restart();
            for (int i = 0; i < 1000; i++)
            {
                OneWayValue = $"OneWay Update {_random.Next(1000)}";
            }
            _stopwatch.Stop();
            var oneWayTime = _stopwatch.Elapsed.TotalMilliseconds;
            OneWayBindingTime = $"OneWay: {oneWayTime:F3}ms for 1000 updates";

            // Measure TwoWay binding update
            _stopwatch.Restart();
            for (int i = 0; i < 1000; i++)
            {
                TwoWayValue = $"TwoWay Update {_random.Next(1000)}";
            }
            _stopwatch.Stop();
            var twoWayTime = _stopwatch.Elapsed.TotalMilliseconds;
            TwoWayBindingTime = $"TwoWay: {twoWayTime:F3}ms for 1000 updates";

            // Calculate difference
            var difference = ((twoWayTime - oneWayTime) / oneWayTime * 100);
            PerformanceDifference = $"TwoWay is {difference:F1}% slower than OneWay";

            StatusMessage = "Performance measurement complete";
        }

        private void ResetCounters_Click(object sender, RoutedEventArgs e)
        {
            PropertyChangedCount = 0;
            LostFocusCount = 0;
            ExplicitCount = 0;
            PropertyChangedText = "";
            LostFocusText = "";
            ExplicitText = "";
            UpdatePerformanceMetrics();
            StatusMessage = "Counters reset";
        }

        private void UpdateExplicitSource_Click(object sender, RoutedEventArgs e)
        {
            var binding = BindingOperations.GetBindingExpression(ExplicitTextBox, TextBox.TextProperty);
            binding?.UpdateSource();
            StatusMessage = "Explicit binding source updated manually";
        }

        private void BenchmarkPaths_Click(object sender, RoutedEventArgs e)
        {
            StatusMessage = "Running path benchmark tests...";

            const int iterations = 10000;
            var results = new List<string>();

            // Benchmark direct property
            _stopwatch.Restart();
            for (int i = 0; i < iterations; i++)
            {
                var _ = DirectProperty;
            }
            _stopwatch.Stop();
            var directTime = _stopwatch.Elapsed.TotalMilliseconds;
            results.Add($"Direct Property: {directTime:F3}ms ({iterations} accesses)");

            // Benchmark nested property
            _stopwatch.Restart();
            for (int i = 0; i < iterations; i++)
            {
                var _ = NestedObject.Level1.Level2.Value;
            }
            _stopwatch.Stop();
            var nestedTime = _stopwatch.Elapsed.TotalMilliseconds;
            results.Add($"Nested Property: {nestedTime:F3}ms ({iterations} accesses)");

            // Benchmark collection indexer
            _stopwatch.Restart();
            for (int i = 0; i < iterations; i++)
            {
                var _ = Items[0].Name;
            }
            _stopwatch.Stop();
            var indexerTime = _stopwatch.Elapsed.TotalMilliseconds;
            results.Add($"Collection Indexer: {indexerTime:F3}ms ({iterations} accesses)");

            // Performance comparison
            results.Add("");
            results.Add($"Nested is {(nestedTime / directTime):F2}x slower than Direct");
            results.Add($"Indexer is {(indexerTime / directTime):F2}x slower than Direct");

            PathBenchmarkResults = string.Join("\n", results);
            StatusMessage = "Path benchmark complete";
        }

        private void CreateOneWayBindings_Click(object sender, RoutedEventArgs e)
        {
            if (!int.TryParse(ItemCountTextBox.Text, out int count) || count <= 0 || count > 10000)
            {
                MessageBox.Show("Please enter a valid number between 1 and 10000", "Invalid Input");
                return;
            }

            StatusMessage = $"Creating {count} items with OneWay bindings...";
            var startMemory = GC.GetTotalMemory(true) / 1024.0 / 1024.0;

            _stopwatch.Restart();
            var items = new ObservableCollection<BindingTestItem>();
            for (int i = 0; i < count; i++)
            {
                items.Add(new BindingTestItem { Id = i + 1, Value = $"OneWay Item {i + 1}" });
            }
            BindingTestItemsControl.ItemsSource = items;
            _stopwatch.Stop();

            CreationTime = $"Creation Time: {_stopwatch.Elapsed.TotalMilliseconds:F2}ms for {count} OneWay items";

            // Measure update time
            _stopwatch.Restart();
            foreach (var item in items)
            {
                item.Value = $"Updated {_random.Next(1000)}";
            }
            _stopwatch.Stop();
            UpdateTime = $"Update Time: {_stopwatch.Elapsed.TotalMilliseconds:F2}ms for {count} updates";

            var endMemory = GC.GetTotalMemory(true) / 1024.0 / 1024.0;
            MemoryUsage = $"Memory Usage: {(endMemory - startMemory):F2} MB for {count} items";

            StatusMessage = $"Created {count} items with OneWay bindings";
        }

        private void CreateTwoWayBindings_Click(object sender, RoutedEventArgs e)
        {
            if (!int.TryParse(ItemCountTextBox.Text, out int count) || count <= 0 || count > 10000)
            {
                MessageBox.Show("Please enter a valid number between 1 and 10000", "Invalid Input");
                return;
            }

            StatusMessage = $"Creating {count} items with TwoWay bindings...";
            var startMemory = GC.GetTotalMemory(true) / 1024.0 / 1024.0;

            _stopwatch.Restart();
            var items = new ObservableCollection<BindingTestItem>();
            for (int i = 0; i < count; i++)
            {
                items.Add(new BindingTestItem { Id = i + 1, Value = $"TwoWay Item {i + 1}" });
            }
            BindingTestItemsControl.ItemsSource = items;
            _stopwatch.Stop();

            CreationTime = $"Creation Time: {_stopwatch.Elapsed.TotalMilliseconds:F2}ms for {count} TwoWay items";

            // Measure update time
            _stopwatch.Restart();
            foreach (var item in items)
            {
                item.Value = $"Updated {_random.Next(1000)}";
            }
            _stopwatch.Stop();
            UpdateTime = $"Update Time: {_stopwatch.Elapsed.TotalMilliseconds:F2}ms for {count} updates";

            var endMemory = GC.GetTotalMemory(true) / 1024.0 / 1024.0;
            MemoryUsage = $"Memory Usage: {(endMemory - startMemory):F2} MB for {count} items ({(endMemory - startMemory) * 1024 / count:F2} KB per item)";

            StatusMessage = $"Created {count} items with TwoWay bindings";
        }

        private void ClearBindings_Click(object sender, RoutedEventArgs e)
        {
            BindingTestItemsControl.ItemsSource = null;
            CreationTime = "Creation Time: Not tested";
            UpdateTime = "Update Time: Not tested";
            MemoryUsage = "Memory Usage: Not tested";
            StatusMessage = "Bindings cleared";
            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();
        }

        #endregion

        #region Helper Methods

        private void UpdateMemoryUsage()
        {
            CurrentMemory = GC.GetTotalMemory(false) / 1024.0 / 1024.0;
        }

        private void UpdatePerformanceMetrics()
        {
            var metrics = new List<string>
            {
                $"PropertyChanged: {PropertyChangedCount} updates (High frequency)",
                $"LostFocus: {LostFocusCount} updates (Optimized)",
                $"Explicit: {ExplicitCount} updates (Manual)",
                "",
                $"Efficiency ratio: PropertyChanged:{PropertyChangedCount} vs LostFocus:{LostFocusCount}"
            };

            if (PropertyChangedCount > 0 && LostFocusCount > 0)
            {
                var ratio = (double)PropertyChangedCount / LostFocusCount;
                metrics.Add($"PropertyChanged is {ratio:F1}x more frequent than LostFocus");
            }

            UpdatePerformanceMetrics = string.Join("\n", metrics);
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

    public class NestedObjectModel
    {
        public Level1Model Level1 { get; } = new Level1Model();
    }

    public class Level1Model
    {
        public Level2Model Level2 { get; } = new Level2Model();
    }

    public class Level2Model
    {
        public string Value => $"Nested Value - {DateTime.Now:HH:mm:ss}";
    }

    public class ItemModel
    {
        public int Id { get; set; }
        public string Name { get; set; } = "";
        public string Value { get; set; } = "";
    }

    public class BindingTestItem : INotifyPropertyChanged
    {
        private int _id;
        private string _value = "";

        public int Id
        {
            get => _id;
            set
            {
                _id = value;
                OnPropertyChanged();
            }
        }

        public string Value
        {
            get => _value;
            set
            {
                _value = value;
                OnPropertyChanged();
            }
        }

        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }

    #endregion
}
