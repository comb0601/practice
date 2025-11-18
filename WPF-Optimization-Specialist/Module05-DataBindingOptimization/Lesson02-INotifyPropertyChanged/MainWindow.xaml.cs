using System.ComponentModel;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Windows;

namespace INotifyPropertyChangedDemo
{
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        private readonly Stopwatch _stopwatch = new();
        private string _statusMessage = "Ready - Explore efficient INotifyPropertyChanged implementations";
        private double _currentMemory = 0;
        private int _naiveEventCount = 0;
        private int _optimizedEventCount = 0;
        private string _naiveExecutionTime = "";
        private string _optimizedExecutionTime = "";
        private string _performanceComparison = "";
        private string _lastPropertyChanged = "";
        private string _withoutBatchTime = "";
        private string _withBatchTime = "";
        private string _batchPerformance = "";
        private string _validationMessage = "";
        private string _performanceSummary = "";

        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;

            // Initialize models
            NaiveModel = new NaivePropertyModel();
            OptimizedModel = new OptimizedPropertyModel();
            CallerMemberModel = new CallerMemberNameModel();
            SetPropertyModel = new SetPropertyPatternModel();
            DependentModel = new DependentPropertiesModel();
            BatchModel = new BatchUpdateModel();
            ValidationModel = new ValidatingModel();

            // Subscribe to events for counting
            NaiveModel.PropertyChanged += (s, e) => NaiveEventCount++;
            OptimizedModel.PropertyChanged += (s, e) => OptimizedEventCount++;
            CallerMemberModel.PropertyChanged += (s, e) =>
            {
                LastPropertyChanged = $"PropertyChanged(\"{e.PropertyName}\")";
            };

            UpdateMemoryUsage();
            UpdatePerformanceSummary();

            // Start timer for memory monitoring
            var timer = new System.Windows.Threading.DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(1)
            };
            timer.Tick += (s, e) => UpdateMemoryUsage();
            timer.Start();
        }

        #region Properties

        public NaivePropertyModel NaiveModel { get; }
        public OptimizedPropertyModel OptimizedModel { get; }
        public CallerMemberNameModel CallerMemberModel { get; }
        public SetPropertyPatternModel SetPropertyModel { get; }
        public DependentPropertiesModel DependentModel { get; }
        public BatchUpdateModel BatchModel { get; }
        public ValidatingModel ValidationModel { get; }

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

        public int NaiveEventCount
        {
            get => _naiveEventCount;
            set => SetProperty(ref _naiveEventCount, value);
        }

        public int OptimizedEventCount
        {
            get => _optimizedEventCount;
            set => SetProperty(ref _optimizedEventCount, value);
        }

        public string NaiveExecutionTime
        {
            get => _naiveExecutionTime;
            set => SetProperty(ref _naiveExecutionTime, value);
        }

        public string OptimizedExecutionTime
        {
            get => _optimizedExecutionTime;
            set => SetProperty(ref _optimizedExecutionTime, value);
        }

        public string PerformanceComparison
        {
            get => _performanceComparison;
            set => SetProperty(ref _performanceComparison, value);
        }

        public string LastPropertyChanged
        {
            get => _lastPropertyChanged;
            set => SetProperty(ref _lastPropertyChanged, value);
        }

        public string WithoutBatchTime
        {
            get => _withoutBatchTime;
            set => SetProperty(ref _withoutBatchTime, value);
        }

        public string WithBatchTime
        {
            get => _withBatchTime;
            set => SetProperty(ref _withBatchTime, value);
        }

        public string BatchPerformance
        {
            get => _batchPerformance;
            set => SetProperty(ref _batchPerformance, value);
        }

        public string ValidationMessage
        {
            get => _validationMessage;
            set => SetProperty(ref _validationMessage, value);
        }

        public string PerformanceSummary
        {
            get => _performanceSummary;
            set => SetProperty(ref _performanceSummary, value);
        }

        #endregion

        #region Event Handlers

        private void TestNaive_Click(object sender, RoutedEventArgs e)
        {
            NaiveEventCount = 0;
            StatusMessage = "Testing naive implementation...";

            _stopwatch.Restart();
            for (int i = 0; i < 1000; i++)
            {
                NaiveModel.Name = "Same Value"; // Always setting same value
            }
            _stopwatch.Stop();

            NaiveExecutionTime = $"Time: {_stopwatch.Elapsed.TotalMilliseconds:F3}ms";
            StatusMessage = $"Naive test complete: {NaiveEventCount} events raised (should be 1!)";
            UpdatePerformanceSummary();
        }

        private void TestOptimized_Click(object sender, RoutedEventArgs e)
        {
            OptimizedEventCount = 0;
            StatusMessage = "Testing optimized implementation...";

            _stopwatch.Restart();
            for (int i = 0; i < 1000; i++)
            {
                OptimizedModel.Name = "Same Value"; // Always setting same value
            }
            _stopwatch.Stop();

            OptimizedExecutionTime = $"Time: {_stopwatch.Elapsed.TotalMilliseconds:F3}ms";
            StatusMessage = $"Optimized test complete: {OptimizedEventCount} events raised (perfect!)";

            if (_naiveEventCount > 0 && _optimizedEventCount > 0)
            {
                var improvement = ((double)_naiveEventCount / _optimizedEventCount - 1) * 100;
                PerformanceComparison = $"Optimized prevented {_naiveEventCount - _optimizedEventCount} unnecessary events! ({improvement:F0}% improvement)";
            }

            UpdatePerformanceSummary();
        }

        private void UpdateWithoutBatch_Click(object sender, RoutedEventArgs e)
        {
            StatusMessage = "Updating 100 properties without batching...";

            _stopwatch.Restart();
            for (int i = 0; i < 100; i++)
            {
                BatchModel.Property1 = $"Value {i}";
                BatchModel.Property2 = $"Value {i}";
                BatchModel.Property3 = $"Value {i}";
                BatchModel.Property4 = $"Value {i}";
                BatchModel.Property5 = $"Value {i}";
            }
            _stopwatch.Stop();

            WithoutBatchTime = $"Without Batch: {_stopwatch.Elapsed.TotalMilliseconds:F3}ms (500 events)";
            StatusMessage = "Update without batching complete";
            UpdatePerformanceSummary();
        }

        private void UpdateWithBatch_Click(object sender, RoutedEventArgs e)
        {
            StatusMessage = "Updating 100 properties with batching...";

            _stopwatch.Restart();
            BatchModel.BeginUpdate();
            for (int i = 0; i < 100; i++)
            {
                BatchModel.Property1 = $"Value {i}";
                BatchModel.Property2 = $"Value {i}";
                BatchModel.Property3 = $"Value {i}";
                BatchModel.Property4 = $"Value {i}";
                BatchModel.Property5 = $"Value {i}";
            }
            BatchModel.EndUpdate();
            _stopwatch.Stop();

            WithBatchTime = $"With Batch: {_stopwatch.Elapsed.TotalMilliseconds:F3}ms (1 event)";

            if (!string.IsNullOrEmpty(_withoutBatchTime))
            {
                // Extract times for comparison
                var withoutMs = double.Parse(_withoutBatchTime.Split(':')[1].Split('m')[0]);
                var withMs = _stopwatch.Elapsed.TotalMilliseconds;
                var improvement = ((withoutMs - withMs) / withoutMs * 100);
                BatchPerformance = $"Batching is {improvement:F0}% faster! (500 events → 1 event)";
            }

            StatusMessage = "Update with batching complete";
            UpdatePerformanceSummary();
        }

        private void ValidateAge_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (int.TryParse(AgeTextBox.Text, out int age))
                {
                    ValidationModel.Age = age;
                    ValidationMessage = $"✅ Age successfully set to {age}";
                    StatusMessage = $"Age validated and set to {age}";
                }
                else
                {
                    ValidationMessage = "❌ Please enter a valid number";
                }
            }
            catch (ArgumentException ex)
            {
                ValidationMessage = $"❌ Validation failed: {ex.Message}";
                StatusMessage = "Age validation failed";
            }
        }

        #endregion

        #region Helper Methods

        private void UpdateMemoryUsage()
        {
            CurrentMemory = GC.GetTotalMemory(false) / 1024.0 / 1024.0;
        }

        private void UpdatePerformanceSummary()
        {
            var summary = new List<string>
            {
                "=== INPC Performance Summary ===",
                "",
                $"Naive Events:     {NaiveEventCount,6} (Always raises)",
                $"Optimized Events: {OptimizedEventCount,6} (Equality check)",
                ""
            };

            if (NaiveEventCount > 0 && OptimizedEventCount > 0)
            {
                summary.Add($"Prevented Events: {NaiveEventCount - OptimizedEventCount,6} (Saved!)");
                var improvement = ((double)NaiveEventCount / OptimizedEventCount - 1) * 100;
                summary.Add($"Improvement:      {improvement,6:F0}%");
            }

            summary.Add("");
            summary.Add("Best Practices:");
            summary.Add("  ✓ Always check equality before raising events");
            summary.Add("  ✓ Use CallerMemberName attribute");
            summary.Add("  ✓ Use SetProperty<T> helper method");
            summary.Add("  ✓ Batch updates when changing multiple properties");
            summary.Add("  ✓ Notify dependent properties explicitly");

            PerformanceSummary = string.Join("\n", summary);
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

    #region Model Classes

    // Naive implementation - always raises events
    public class NaivePropertyModel : INotifyPropertyChanged
    {
        private string _name = "";

        public string Name
        {
            get => _name;
            set
            {
                _name = value;
                OnPropertyChanged(nameof(Name)); // Always raises!
            }
        }

        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanged(string? propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }

    // Optimized implementation - checks equality
    public class OptimizedPropertyModel : INotifyPropertyChanged
    {
        private string _name = "";

        public string Name
        {
            get => _name;
            set
            {
                if (_name == value) return; // Equality check!
                _name = value;
                OnPropertyChanged(nameof(Name));
            }
        }

        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanged(string? propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }

    // CallerMemberName demonstration
    public class CallerMemberNameModel : INotifyPropertyChanged
    {
        private string _name = "";

        public string Name
        {
            get => _name;
            set
            {
                if (_name == value) return;
                _name = value;
                OnPropertyChanged(); // No parameter needed!
            }
        }

        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }

    // SetProperty pattern
    public class SetPropertyPatternModel : INotifyPropertyChanged
    {
        private string _firstName = "";
        private string _lastName = "";

        public string FirstName
        {
            get => _firstName;
            set
            {
                if (SetProperty(ref _firstName, value))
                {
                    OnPropertyChanged(nameof(FullName));
                }
            }
        }

        public string LastName
        {
            get => _lastName;
            set
            {
                if (SetProperty(ref _lastName, value))
                {
                    OnPropertyChanged(nameof(FullName));
                }
            }
        }

        public string FullName => $"{FirstName} {LastName}".Trim();

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
    }

    // Dependent properties
    public class DependentPropertiesModel : INotifyPropertyChanged
    {
        private string _firstName = "John";
        private string _lastName = "Doe";

        public string FirstName
        {
            get => _firstName;
            set
            {
                if (SetProperty(ref _firstName, value))
                {
                    OnPropertyChanged(nameof(FullName));
                    OnPropertyChanged(nameof(Initials));
                    OnPropertyChanged(nameof(DisplayFormat));
                }
            }
        }

        public string LastName
        {
            get => _lastName;
            set
            {
                if (SetProperty(ref _lastName, value))
                {
                    OnPropertyChanged(nameof(FullName));
                    OnPropertyChanged(nameof(Initials));
                    OnPropertyChanged(nameof(DisplayFormat));
                }
            }
        }

        public string FullName => $"{FirstName} {LastName}";

        public string Initials
        {
            get
            {
                if (string.IsNullOrEmpty(FirstName) || string.IsNullOrEmpty(LastName))
                    return "";
                return $"{FirstName[0]}{LastName[0]}";
            }
        }

        public string DisplayFormat => $"{LastName}, {FirstName} ({Initials})";

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
    }

    // Batch updates
    public class BatchUpdateModel : INotifyPropertyChanged
    {
        private bool _isBatchUpdate;
        private string _property1 = "";
        private string _property2 = "";
        private string _property3 = "";
        private string _property4 = "";
        private string _property5 = "";

        public string Property1
        {
            get => _property1;
            set => SetProperty(ref _property1, value);
        }

        public string Property2
        {
            get => _property2;
            set => SetProperty(ref _property2, value);
        }

        public string Property3
        {
            get => _property3;
            set => SetProperty(ref _property3, value);
        }

        public string Property4
        {
            get => _property4;
            set => SetProperty(ref _property4, value);
        }

        public string Property5
        {
            get => _property5;
            set => SetProperty(ref _property5, value);
        }

        public void BeginUpdate()
        {
            _isBatchUpdate = true;
        }

        public void EndUpdate()
        {
            _isBatchUpdate = false;
            OnPropertyChanged(string.Empty); // Notify all properties changed
        }

        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
        {
            if (!_isBatchUpdate)
            {
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
            }
        }

        protected bool SetProperty<T>(ref T field, T value, [CallerMemberName] string? propertyName = null)
        {
            if (EqualityComparer<T>.Default.Equals(field, value))
                return false;

            field = value;
            OnPropertyChanged(propertyName);
            return true;
        }
    }

    // Validating model with INotifyPropertyChanging
    public class ValidatingModel : INotifyPropertyChanging, INotifyPropertyChanged
    {
        private int _age = 25;

        public int Age
        {
            get => _age;
            set
            {
                OnPropertyChanging();

                if (value < 0 || value > 120)
                    throw new ArgumentException("Age must be between 0 and 120");

                _age = value;
                OnPropertyChanged();
            }
        }

        public event PropertyChangingEventHandler? PropertyChanging;
        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanging([CallerMemberName] string? propertyName = null)
        {
            PropertyChanging?.Invoke(this, new PropertyChangingEventArgs(propertyName));
        }

        protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }

    #endregion
}
