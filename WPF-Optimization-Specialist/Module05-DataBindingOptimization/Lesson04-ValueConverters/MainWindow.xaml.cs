using System.Collections.Concurrent;
using System.ComponentModel;
using System.Diagnostics;
using System.Globalization;
using System.Runtime.CompilerServices;
using System.Security.Cryptography;
using System.Text;
using System.Windows;
using System.Windows.Data;

namespace ValueConvertersDemo
{
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        private readonly Stopwatch _stopwatch = new();
        private string _statusMessage = "Ready - Explore value converter optimization techniques";
        private double _currentMemory = 0;

        // Naive vs Cached metrics
        private string _naiveConverterTime = "Time: Not tested";
        private int _naiveConverterCalls = 0;
        private int _naiveAllocations = 0;
        private string _cachedConverterTime = "Time: Not tested";
        private int _cachedConverterCalls = 0;
        private int _cachedAllocations = 0;
        private string _converterComparison = "";

        // Singleton test
        private string _singletonResult = "Not tested";

        // Expensive operations
        private string _expensiveNoCacheTime = "Time: Not tested";
        private string _expensiveCachedTime = "Time: Not tested";
        private string _expensiveComparisonResults = "Click 'Run Comparison' to test";

        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;

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

        public string NaiveConverterTime
        {
            get => _naiveConverterTime;
            set => SetProperty(ref _naiveConverterTime, value);
        }

        public int NaiveConverterCalls
        {
            get => _naiveConverterCalls;
            set => SetProperty(ref _naiveConverterCalls, value);
        }

        public int NaiveAllocations
        {
            get => _naiveAllocations;
            set => SetProperty(ref _naiveAllocations, value);
        }

        public string CachedConverterTime
        {
            get => _cachedConverterTime;
            set => SetProperty(ref _cachedConverterTime, value);
        }

        public int CachedConverterCalls
        {
            get => _cachedConverterCalls;
            set => SetProperty(ref _cachedConverterCalls, value);
        }

        public int CachedAllocations
        {
            get => _cachedAllocations;
            set => SetProperty(ref _cachedAllocations, value);
        }

        public string ConverterComparison
        {
            get => _converterComparison;
            set => SetProperty(ref _converterComparison, value);
        }

        public string SingletonResult
        {
            get => _singletonResult;
            set => SetProperty(ref _singletonResult, value);
        }

        public string ExpensiveNoCacheTime
        {
            get => _expensiveNoCacheTime;
            set => SetProperty(ref _expensiveNoCacheTime, value);
        }

        public string ExpensiveCachedTime
        {
            get => _expensiveCachedTime;
            set => SetProperty(ref _expensiveCachedTime, value);
        }

        public string ExpensiveComparisonResults
        {
            get => _expensiveComparisonResults;
            set => SetProperty(ref _expensiveComparisonResults, value);
        }

        #endregion

        #region Event Handlers

        private void TestNaiveConverter_Click(object sender, RoutedEventArgs e)
        {
            StatusMessage = "Testing naive converter...";

            var converter = new NaiveFormatConverter();
            _naiveConverterCalls = 0;
            _naiveAllocations = 0;

            // Test converting the same value 1000 times
            _stopwatch.Restart();
            for (int i = 0; i < 1000; i++)
            {
                var result = converter.Convert(123.45, typeof(string), null, CultureInfo.CurrentCulture);
                _naiveConverterCalls++;
                _naiveAllocations++; // Each call allocates a new string
            }
            _stopwatch.Stop();

            NaiveConverterTime = $"Time: {_stopwatch.Elapsed.TotalMilliseconds:F3}ms";
            NaiveConverterCalls = _naiveConverterCalls;
            NaiveAllocations = _naiveAllocations;
            StatusMessage = $"Naive converter tested: {_naiveConverterCalls} calls, {_naiveAllocations} allocations";
        }

        private void TestCachedConverter_Click(object sender, RoutedEventArgs e)
        {
            StatusMessage = "Testing cached converter...";

            var converter = new CachedFormatConverter();
            CachedFormatConverter.ClearCache(); // Start fresh
            _cachedConverterCalls = 0;
            _cachedAllocations = 0;

            // Test converting the same value 1000 times
            _stopwatch.Restart();
            for (int i = 0; i < 1000; i++)
            {
                var result = converter.Convert(123.45, typeof(string), null, CultureInfo.CurrentCulture);
                _cachedConverterCalls++;
                if (i == 0) _cachedAllocations = 1; // Only first call allocates
            }
            _stopwatch.Stop();

            CachedConverterTime = $"Time: {_stopwatch.Elapsed.TotalMilliseconds:F3}ms";
            CachedConverterCalls = _cachedConverterCalls;
            CachedAllocations = _cachedAllocations;

            if (_naiveConverterCalls > 0)
            {
                var speedup = (double)_naiveConverterCalls / _cachedConverterCalls *
                             (_stopwatch.Elapsed.TotalMilliseconds > 0
                                 ? double.Parse(_naiveConverterTime.Split(':')[1].Replace("ms", "")) / _stopwatch.Elapsed.TotalMilliseconds
                                 : 1);
                ConverterComparison = $"Cached converter is ~{speedup:F0}x faster! " +
                                    $"Saved {_naiveAllocations - _cachedAllocations} allocations.";
            }

            StatusMessage = $"Cached converter tested: {_cachedConverterCalls} calls, {_cachedAllocations} allocations";
        }

        private void TestSingleton_Click(object sender, RoutedEventArgs e)
        {
            StatusMessage = "Testing singleton pattern...";

            // Test that singleton instance is reused
            var instance1 = InstanceCachedFormatConverter.Instance;
            var instance2 = InstanceCachedFormatConverter.Instance;

            if (ReferenceEquals(instance1, instance2))
            {
                SingletonResult = "✅ Singleton pattern working correctly: Same instance reused.\n" +
                                "Memory efficient: Only one converter instance in entire application.";
            }
            else
            {
                SingletonResult = "❌ Not a true singleton: Different instances created.";
            }

            StatusMessage = "Singleton pattern tested";
        }

        private void TestExpensiveNoCacheClick(object sender, RoutedEventArgs e)
        {
            StatusMessage = "Testing expensive converter without caching...";

            var converter = new ExpensiveCalculationConverter();

            _stopwatch.Restart();
            for (int i = 0; i < 10; i++)
            {
                converter.Convert("test", typeof(string), null, CultureInfo.CurrentCulture);
            }
            _stopwatch.Stop();

            ExpensiveNoCacheTime = $"Time: {_stopwatch.Elapsed.TotalMilliseconds:F0}ms (10 conversions)";
            StatusMessage = "Expensive converter test complete (no cache)";
        }

        private void TestExpensiveCached_Click(object sender, RoutedEventArgs e)
        {
            StatusMessage = "Testing expensive converter with caching...";

            var converter = new CachedExpensiveCalculationConverter();
            CachedExpensiveCalculationConverter.ClearCache();

            _stopwatch.Restart();
            for (int i = 0; i < 10; i++)
            {
                converter.Convert("test", typeof(string), null, CultureInfo.CurrentCulture);
            }
            _stopwatch.Stop();

            ExpensiveCachedTime = $"Time: {_stopwatch.Elapsed.TotalMilliseconds:F0}ms (10 conversions, 9 from cache)";
            StatusMessage = "Expensive converter test complete (with cache)";
        }

        private void RunExpensiveComparison_Click(object sender, RoutedEventArgs e)
        {
            if (!int.TryParse(ExpensiveCountTextBox.Text, out int count) || count <= 0)
            {
                MessageBox.Show("Please enter a valid positive number", "Invalid Input");
                return;
            }

            StatusMessage = "Running expensive converter comparison...";

            var results = new List<string>
            {
                $"=== EXPENSIVE CONVERTER COMPARISON ({count} conversions) ===",
                ""
            };

            // Test without caching
            var noCacheConverter = new ExpensiveCalculationConverter();
            _stopwatch.Restart();
            for (int i = 0; i < count; i++)
            {
                noCacheConverter.Convert($"test{i % 10}", typeof(string), null, CultureInfo.CurrentCulture);
            }
            _stopwatch.Stop();
            var noCacheMs = _stopwatch.Elapsed.TotalMilliseconds;
            results.Add($"Without Cache:  {noCacheMs,8:F0}ms");

            // Test with caching
            var cachedConverter = new CachedExpensiveCalculationConverter();
            CachedExpensiveCalculationConverter.ClearCache();
            _stopwatch.Restart();
            for (int i = 0; i < count; i++)
            {
                cachedConverter.Convert($"test{i % 10}", typeof(string), null, CultureInfo.CurrentCulture);
            }
            _stopwatch.Stop();
            var cachedMs = _stopwatch.Elapsed.TotalMilliseconds;
            results.Add($"With Cache:     {cachedMs,8:F0}ms");
            results.Add("");
            results.Add($"Speedup:        {noCacheMs / cachedMs,8:F1}x faster");
            results.Add($"Time Saved:     {noCacheMs - cachedMs,8:F0}ms");
            results.Add("");
            results.Add($"Note: Only 10 unique values, so 90% of calls were cache hits!");

            ExpensiveComparisonResults = string.Join("\n", results);
            StatusMessage = "Expensive converter comparison complete";
        }

        #endregion

        #region Helper Methods

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

    #region Converter Implementations

    /// <summary>
    /// Naive converter - no caching, creates new string every time
    /// </summary>
    public class NaiveFormatConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is double d)
            {
                // Always creates new string - inefficient!
                return $"${d:N2}";
            }
            return value?.ToString() ?? "";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    /// <summary>
    /// Cached converter - stores results by input value
    /// </summary>
    public class CachedFormatConverter : IValueConverter
    {
        private static readonly Dictionary<double, string> _cache = new();

        public static void ClearCache() => _cache.Clear();

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is double d)
            {
                if (!_cache.TryGetValue(d, out var result))
                {
                    result = $"${d:N2}";
                    _cache[d] = result;
                }
                return result;
            }
            return value?.ToString() ?? "";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    /// <summary>
    /// Instance cached converter with singleton pattern
    /// </summary>
    public class InstanceCachedFormatConverter : IValueConverter
    {
        public static readonly InstanceCachedFormatConverter Instance = new();

        private static readonly Dictionary<double, string> _cache = new();

        private InstanceCachedFormatConverter() { }

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is double d)
            {
                if (!_cache.TryGetValue(d, out var result))
                {
                    result = $"${d:N2}";
                    _cache[d] = result;
                }
                return result;
            }
            return value?.ToString() ?? "";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    /// <summary>
    /// Naive bool to visibility converter - creates new Visibility objects
    /// </summary>
    public class NaiveBoolToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is bool b)
            {
                // Boxing creates new object every time!
                return b ? Visibility.Visible : Visibility.Collapsed;
            }
            return Visibility.Collapsed;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    /// <summary>
    /// Cached bool to visibility converter - reuses boxed Visibility objects
    /// </summary>
    public class CachedBoolToVisibilityConverter : IValueConverter
    {
        private static readonly object VisibleBox = Visibility.Visible;
        private static readonly object CollapsedBox = Visibility.Collapsed;

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is bool b)
            {
                // Returns cached boxed objects
                return b ? VisibleBox : CollapsedBox;
            }
            return CollapsedBox;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    /// <summary>
    /// Converter with expensive calculation - no caching
    /// </summary>
    public class ExpensiveCalculationConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is string input)
            {
                // Simulate expensive operation (10ms)
                Thread.Sleep(10);

                // Expensive calculation
                using var sha = SHA256.Create();
                var bytes = Encoding.UTF8.GetBytes(input);
                var hash = sha.ComputeHash(bytes);
                return BitConverter.ToString(hash).Replace("-", "").Substring(0, 8);
            }
            return "";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    /// <summary>
    /// Converter with expensive calculation - with caching
    /// </summary>
    public class CachedExpensiveCalculationConverter : IValueConverter
    {
        private static readonly ConcurrentDictionary<string, string> _cache = new();

        public static void ClearCache() => _cache.Clear();

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is string input)
            {
                return _cache.GetOrAdd(input, key =>
                {
                    // Only computed once per unique input
                    Thread.Sleep(10); // Simulate expensive operation

                    using var sha = SHA256.Create();
                    var bytes = Encoding.UTF8.GetBytes(key);
                    var hash = sha.ComputeHash(bytes);
                    return BitConverter.ToString(hash).Replace("-", "").Substring(0, 8);
                });
            }
            return "";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    #endregion
}
