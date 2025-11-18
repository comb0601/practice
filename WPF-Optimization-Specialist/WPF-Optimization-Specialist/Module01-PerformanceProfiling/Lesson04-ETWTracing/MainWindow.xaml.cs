using System;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Diagnostics.Tracing;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace Lesson04_ETWTracing
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// Demonstrates Event Tracing for Windows (ETW) implementation
    /// </summary>
    public partial class MainWindow : Window
    {
        private readonly WpfLearningEventSource eventSource;
        private readonly ObservableCollection<EventLogEntry> eventLog;
        private int totalEvents = 0;
        private int infoEvents = 0;
        private int warningEvents = 0;
        private int errorEvents = 0;
        private int criticalEvents = 0;
        private int verboseEvents = 0;
        private int operationId = 0;

        public MainWindow()
        {
            InitializeComponent();

            // Initialize ETW Event Source
            eventSource = WpfLearningEventSource.Instance;

            // Initialize event log
            eventLog = new ObservableCollection<EventLogEntry>();
            EventLogListBox.ItemsSource = eventLog;

            // Display Event Source information
            EventSourceGuidText.Text = eventSource.Guid.ToString();
            EventSourceNameText.Text = eventSource.Name;
            EventSourceStatusText.Text = eventSource.IsEnabled() ? "Active" : "Inactive";
            EventSourceStatusText.Foreground = eventSource.IsEnabled() ? Brushes.Green : Brushes.Red;

            // Update logman command with actual GUID
            LogmanCommandText.Text = $"logman start mysession -p {{{eventSource.Guid}}} -o trace.etl -ets";

            Loaded += MainWindow_Loaded;
        }

        private void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            AddLogEntry("Application started", EventLevel.Informational);
            eventSource.ApplicationStarted(Environment.UserName, Environment.MachineName);
        }

        #region Event Log Management

        private void AddLogEntry(string message, EventLevel level, string? details = null)
        {
            var entry = new EventLogEntry
            {
                Timestamp = DateTime.Now,
                Level = level,
                Message = message,
                Details = details ?? string.Empty
            };

            Dispatcher.Invoke(() =>
            {
                eventLog.Insert(0, entry);

                // Keep only last 1000 events
                if (eventLog.Count > 1000)
                {
                    eventLog.RemoveAt(eventLog.Count - 1);
                }

                // Update statistics
                totalEvents++;
                switch (level)
                {
                    case EventLevel.Informational:
                        infoEvents++;
                        break;
                    case EventLevel.Warning:
                        warningEvents++;
                        break;
                    case EventLevel.Error:
                        errorEvents++;
                        break;
                    case EventLevel.Critical:
                        criticalEvents++;
                        break;
                    case EventLevel.Verbose:
                        verboseEvents++;
                        break;
                }

                UpdateStatistics();
            });
        }

        private void UpdateStatistics()
        {
            TotalEventsText.Text = totalEvents.ToString();
            InfoEventsText.Text = infoEvents.ToString();
            WarningEventsText.Text = warningEvents.ToString();
            ErrorEventsText.Text = errorEvents.ToString();
            CriticalEventsText.Text = criticalEvents.ToString();
            VerboseEventsText.Text = verboseEvents.ToString();
        }

        private void ClearLog_Click(object sender, RoutedEventArgs e)
        {
            eventLog.Clear();
            totalEvents = 0;
            infoEvents = 0;
            warningEvents = 0;
            errorEvents = 0;
            criticalEvents = 0;
            verboseEvents = 0;
            UpdateStatistics();

            StatusText.Text = "Event log cleared";
        }

        private void FilterChanged(object sender, RoutedEventArgs e)
        {
            // This would typically filter the view, but for simplicity we'll just note it
            StatusText.Text = "Filter settings changed";
        }

        #endregion

        #region Informational Events

        private void LogUserAction_Click(object sender, RoutedEventArgs e)
        {
            string action = "ButtonClick";
            string target = "LogUserAction";

            eventSource.UserActionPerformed(action, target, Environment.UserName);
            AddLogEntry($"User action: {action} on {target}", EventLevel.Informational,
                       $"User: {Environment.UserName}");

            StatusText.Text = "User action event logged";
        }

        private void LogDataLoaded_Click(object sender, RoutedEventArgs e)
        {
            string source = "SampleDatabase";
            int recordCount = new Random().Next(100, 10000);
            long durationMs = new Random().Next(50, 500);

            eventSource.DataLoaded(source, recordCount, durationMs);
            AddLogEntry($"Data loaded from {source}: {recordCount} records in {durationMs}ms",
                       EventLevel.Informational,
                       $"Average: {(double)durationMs / recordCount:F4}ms per record");

            StatusText.Text = $"Data loaded: {recordCount} records";
        }

        private void LogConfigChange_Click(object sender, RoutedEventArgs e)
        {
            string setting = "ThemeColor";
            string oldValue = "Light";
            string newValue = "Dark";

            eventSource.ConfigurationChanged(setting, oldValue, newValue);
            AddLogEntry($"Configuration changed: {setting}", EventLevel.Informational,
                       $"Changed from '{oldValue}' to '{newValue}'");

            StatusText.Text = "Configuration change logged";
        }

        #endregion

        #region Warning Events

        private void LogSlowOperation_Click(object sender, RoutedEventArgs e)
        {
            string operationName = "DataProcessing";
            long actualDuration = new Random().Next(2000, 5000);
            long expectedDuration = 1000;

            eventSource.SlowOperationDetected(operationName, actualDuration, expectedDuration);
            AddLogEntry($"Slow operation detected: {operationName}", EventLevel.Warning,
                       $"Duration: {actualDuration}ms (expected: {expectedDuration}ms)");

            StatusText.Text = $"Slow operation warning: {actualDuration}ms";
        }

        private void LogResourceWarning_Click(object sender, RoutedEventArgs e)
        {
            string resourceType = "Memory";
            double currentUsage = 85.5;
            double threshold = 80.0;

            eventSource.ResourceThresholdExceeded(resourceType, currentUsage, threshold);
            AddLogEntry($"Resource threshold exceeded: {resourceType}", EventLevel.Warning,
                       $"Current: {currentUsage}%, Threshold: {threshold}%");

            StatusText.Text = $"Resource warning: {resourceType} at {currentUsage}%";
        }

        private void LogRetryAttempt_Click(object sender, RoutedEventArgs e)
        {
            string operation = "DatabaseConnection";
            int attemptNumber = new Random().Next(1, 5);
            int maxAttempts = 5;
            string reason = "Connection timeout";

            eventSource.RetryAttempt(operation, attemptNumber, maxAttempts, reason);
            AddLogEntry($"Retry attempt: {operation}", EventLevel.Warning,
                       $"Attempt {attemptNumber}/{maxAttempts} - Reason: {reason}");

            StatusText.Text = $"Retry {attemptNumber}/{maxAttempts}: {operation}";
        }

        #endregion

        #region Error Events

        private void LogOperationFailed_Click(object sender, RoutedEventArgs e)
        {
            string operation = "SaveDocument";
            string errorCode = "ERR_DISK_FULL";
            string errorMessage = "Insufficient disk space to complete operation";

            eventSource.OperationFailed(operation, errorCode, errorMessage);
            AddLogEntry($"Operation failed: {operation}", EventLevel.Error,
                       $"Error: {errorCode} - {errorMessage}");

            StatusText.Text = $"Operation failed: {operation}";
        }

        private void LogException_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                // Simulate an exception
                throw new InvalidOperationException("Simulated exception for ETW demonstration");
            }
            catch (Exception ex)
            {
                eventSource.ExceptionOccurred(ex.GetType().Name, ex.Message, ex.StackTrace ?? string.Empty);
                AddLogEntry($"Exception: {ex.GetType().Name}", EventLevel.Error,
                           $"Message: {ex.Message}");

                StatusText.Text = $"Exception logged: {ex.GetType().Name}";
            }
        }

        private void LogCriticalError_Click(object sender, RoutedEventArgs e)
        {
            string component = "DataLayer";
            string errorMessage = "Database connection pool exhausted";
            string impact = "Application unable to process requests";

            eventSource.CriticalError(component, errorMessage, impact);
            AddLogEntry($"Critical error in {component}", EventLevel.Critical,
                       $"{errorMessage} - Impact: {impact}");

            StatusText.Text = "Critical error logged";
        }

        #endregion

        #region Performance Events

        private async void StartStopOperation_Click(object sender, RoutedEventArgs e)
        {
            int currentOpId = ++operationId;
            string operationName = $"Operation_{currentOpId}";

            // Start operation
            eventSource.OperationStarted(currentOpId, operationName);
            AddLogEntry($"Operation started: {operationName}", EventLevel.Verbose,
                       $"Operation ID: {currentOpId}");

            StatusText.Text = $"Starting {operationName}...";

            // Simulate work
            await Task.Delay(new Random().Next(500, 2000));

            // Stop operation
            eventSource.OperationCompleted(currentOpId, operationName, true);
            AddLogEntry($"Operation completed: {operationName}", EventLevel.Verbose,
                       $"Operation ID: {currentOpId}, Success: True");

            StatusText.Text = $"Completed {operationName}";
        }

        private void LogWithDuration_Click(object sender, RoutedEventArgs e)
        {
            string taskName = "ImageProcessing";
            long duration = new Random().Next(100, 1000);
            int itemsProcessed = new Random().Next(10, 100);

            eventSource.TaskCompleted(taskName, duration, itemsProcessed);
            AddLogEntry($"Task completed: {taskName}", EventLevel.Informational,
                       $"Duration: {duration}ms, Items: {itemsProcessed}, Avg: {(double)duration / itemsProcessed:F2}ms/item");

            StatusText.Text = $"Task logged: {taskName} ({duration}ms)";
        }

        private async void SimulateComplexOp_Click(object sender, RoutedEventArgs e)
        {
            int opId = ++operationId;
            string operationName = "ComplexDataProcessing";

            // Start main operation
            eventSource.OperationStarted(opId, operationName);
            AddLogEntry($"Complex operation started: {operationName}", EventLevel.Informational);

            StatusText.Text = $"Running {operationName}...";

            var random = new Random();

            // Phase 1: Data Loading
            await Task.Delay(random.Next(200, 500));
            eventSource.DataLoaded("SourceSystem", random.Next(1000, 5000), random.Next(100, 300));
            AddLogEntry("Phase 1: Data loading completed", EventLevel.Verbose);

            // Phase 2: Processing (with potential warning)
            await Task.Delay(random.Next(300, 800));
            long processingTime = random.Next(500, 3000);
            if (processingTime > 1500)
            {
                eventSource.SlowOperationDetected("DataProcessing", processingTime, 1000);
                AddLogEntry("Phase 2: Processing slow", EventLevel.Warning);
            }
            else
            {
                AddLogEntry("Phase 2: Processing completed", EventLevel.Verbose);
            }

            // Phase 3: Validation
            await Task.Delay(random.Next(100, 300));
            AddLogEntry("Phase 3: Validation completed", EventLevel.Verbose);

            // Complete operation
            eventSource.OperationCompleted(opId, operationName, true);
            AddLogEntry($"Complex operation completed: {operationName}", EventLevel.Informational,
                       $"Operation ID: {opId}");

            StatusText.Text = $"Completed {operationName}";
        }

        #endregion

        #region Helper Methods

        private void ViewInPerfView_Click(object sender, RoutedEventArgs e)
        {
            var message = "To collect and view ETW traces:\n\n" +
                         "1. Download PerfView from:\n" +
                         "   https://github.com/microsoft/perfview/releases\n\n" +
                         "2. Run PerfView as Administrator:\n" +
                         $"   PerfView.exe collect -OnlyProviders=*{eventSource.Name}\n\n" +
                         "3. Perform actions in this application\n\n" +
                         "4. Stop collection and view the .etl file\n\n" +
                         "Alternative: Use Windows Performance Analyzer (WPA)\n" +
                         "or logman command shown in the UI.";

            MessageBox.Show(message, "Viewing ETW Traces", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        #endregion

        #region Event Log Entry Class

        private class EventLogEntry
        {
            public DateTime Timestamp { get; set; }
            public EventLevel Level { get; set; }
            public string Message { get; set; } = string.Empty;
            public string Details { get; set; } = string.Empty;

            public override string ToString()
            {
                string levelStr = Level switch
                {
                    EventLevel.Critical => "[CRITICAL]",
                    EventLevel.Error => "[ERROR]   ",
                    EventLevel.Warning => "[WARNING] ",
                    EventLevel.Informational => "[INFO]    ",
                    EventLevel.Verbose => "[VERBOSE] ",
                    _ => "[UNKNOWN] "
                };

                string timestamp = Timestamp.ToString("HH:mm:ss.fff");
                string details = !string.IsNullOrEmpty(Details) ? $" - {Details}" : "";

                return $"{timestamp} {levelStr} {Message}{details}";
            }
        }

        #endregion
    }

    #region Custom ETW Event Source

    /// <summary>
    /// Custom EventSource for WPF Learning Application
    /// Demonstrates various ETW event types and patterns
    /// </summary>
    [EventSource(Name = "WPF-Learning-App")]
    public sealed class WpfLearningEventSource : EventSource
    {
        // Singleton instance
        public static readonly WpfLearningEventSource Instance = new WpfLearningEventSource();

        // Private constructor for singleton
        private WpfLearningEventSource() : base(EventSourceSettings.EtwSelfDescribingEventFormat)
        {
        }

        #region Event Definitions

        // Event IDs should be unique and sequential
        private const int ApplicationStartedEventId = 1;
        private const int ApplicationStoppedEventId = 2;
        private const int UserActionPerformedEventId = 10;
        private const int DataLoadedEventId = 11;
        private const int ConfigurationChangedEventId = 12;
        private const int OperationStartedEventId = 20;
        private const int OperationCompletedEventId = 21;
        private const int TaskCompletedEventId = 22;
        private const int SlowOperationDetectedEventId = 30;
        private const int ResourceThresholdExceededEventId = 31;
        private const int RetryAttemptEventId = 32;
        private const int OperationFailedEventId = 40;
        private const int ExceptionOccurredEventId = 41;
        private const int CriticalErrorEventId = 42;

        #endregion

        #region Informational Events

        [Event(ApplicationStartedEventId, Level = EventLevel.Informational, Message = "Application started by {0} on {1}")]
        public void ApplicationStarted(string userName, string machineName)
        {
            if (IsEnabled())
            {
                WriteEvent(ApplicationStartedEventId, userName, machineName);
            }
        }

        [Event(ApplicationStoppedEventId, Level = EventLevel.Informational, Message = "Application stopped")]
        public void ApplicationStopped()
        {
            if (IsEnabled())
            {
                WriteEvent(ApplicationStoppedEventId);
            }
        }

        [Event(UserActionPerformedEventId, Level = EventLevel.Informational,
               Message = "User action: {0} on {1} by {2}")]
        public void UserActionPerformed(string action, string target, string userName)
        {
            if (IsEnabled())
            {
                WriteEvent(UserActionPerformedEventId, action, target, userName);
            }
        }

        [Event(DataLoadedEventId, Level = EventLevel.Informational,
               Message = "Data loaded from {0}: {1} records in {2}ms")]
        public void DataLoaded(string source, int recordCount, long durationMs)
        {
            if (IsEnabled())
            {
                WriteEvent(DataLoadedEventId, source, recordCount, durationMs);
            }
        }

        [Event(ConfigurationChangedEventId, Level = EventLevel.Informational,
               Message = "Configuration '{0}' changed from '{1}' to '{2}'")]
        public void ConfigurationChanged(string settingName, string oldValue, string newValue)
        {
            if (IsEnabled())
            {
                WriteEvent(ConfigurationChangedEventId, settingName, oldValue, newValue);
            }
        }

        #endregion

        #region Performance Events

        [Event(OperationStartedEventId, Level = EventLevel.Verbose,
               Opcode = EventOpcode.Start, Task = Tasks.Operation,
               Message = "Operation {0} started: {1}")]
        public void OperationStarted(int operationId, string operationName)
        {
            if (IsEnabled(EventLevel.Verbose, EventKeywords.None))
            {
                WriteEvent(OperationStartedEventId, operationId, operationName);
            }
        }

        [Event(OperationCompletedEventId, Level = EventLevel.Verbose,
               Opcode = EventOpcode.Stop, Task = Tasks.Operation,
               Message = "Operation {0} completed: {1}, Success: {2}")]
        public void OperationCompleted(int operationId, string operationName, bool success)
        {
            if (IsEnabled(EventLevel.Verbose, EventKeywords.None))
            {
                WriteEvent(OperationCompletedEventId, operationId, operationName, success);
            }
        }

        [Event(TaskCompletedEventId, Level = EventLevel.Informational,
               Message = "Task '{0}' completed in {1}ms, processed {2} items")]
        public void TaskCompleted(string taskName, long durationMs, int itemsProcessed)
        {
            if (IsEnabled())
            {
                WriteEvent(TaskCompletedEventId, taskName, durationMs, itemsProcessed);
            }
        }

        #endregion

        #region Warning Events

        [Event(SlowOperationDetectedEventId, Level = EventLevel.Warning,
               Message = "Slow operation '{0}': {1}ms (expected: {2}ms)")]
        public void SlowOperationDetected(string operationName, long actualDuration, long expectedDuration)
        {
            if (IsEnabled(EventLevel.Warning, EventKeywords.None))
            {
                WriteEvent(SlowOperationDetectedEventId, operationName, actualDuration, expectedDuration);
            }
        }

        [Event(ResourceThresholdExceededEventId, Level = EventLevel.Warning,
               Message = "Resource '{0}' threshold exceeded: {1}% (threshold: {2}%)")]
        public void ResourceThresholdExceeded(string resourceType, double currentUsage, double threshold)
        {
            if (IsEnabled(EventLevel.Warning, EventKeywords.None))
            {
                WriteEvent(ResourceThresholdExceededEventId, resourceType, currentUsage, threshold);
            }
        }

        [Event(RetryAttemptEventId, Level = EventLevel.Warning,
               Message = "Retry attempt {1}/{2} for operation '{0}': {3}")]
        public void RetryAttempt(string operation, int attemptNumber, int maxAttempts, string reason)
        {
            if (IsEnabled(EventLevel.Warning, EventKeywords.None))
            {
                WriteEvent(RetryAttemptEventId, operation, attemptNumber, maxAttempts, reason);
            }
        }

        #endregion

        #region Error Events

        [Event(OperationFailedEventId, Level = EventLevel.Error,
               Message = "Operation '{0}' failed: {1} - {2}")]
        public void OperationFailed(string operation, string errorCode, string errorMessage)
        {
            if (IsEnabled(EventLevel.Error, EventKeywords.None))
            {
                WriteEvent(OperationFailedEventId, operation, errorCode, errorMessage);
            }
        }

        [Event(ExceptionOccurredEventId, Level = EventLevel.Error,
               Message = "Exception: {0} - {1}")]
        public void ExceptionOccurred(string exceptionType, string message, string stackTrace)
        {
            if (IsEnabled(EventLevel.Error, EventKeywords.None))
            {
                WriteEvent(ExceptionOccurredEventId, exceptionType, message, stackTrace);
            }
        }

        [Event(CriticalErrorEventId, Level = EventLevel.Critical,
               Message = "Critical error in {0}: {1} - Impact: {2}")]
        public void CriticalError(string component, string errorMessage, string impact)
        {
            if (IsEnabled(EventLevel.Critical, EventKeywords.None))
            {
                WriteEvent(CriticalErrorEventId, component, errorMessage, impact);
            }
        }

        #endregion

        #region Tasks and Keywords

        public static class Tasks
        {
            public const EventTask Operation = (EventTask)1;
            public const EventTask DataAccess = (EventTask)2;
            public const EventTask UserInterface = (EventTask)3;
        }

        public static class Keywords
        {
            public const EventKeywords Performance = (EventKeywords)0x0001;
            public const EventKeywords Diagnostic = (EventKeywords)0x0002;
            public const EventKeywords UserAction = (EventKeywords)0x0004;
        }

        #endregion
    }

    #endregion
}
