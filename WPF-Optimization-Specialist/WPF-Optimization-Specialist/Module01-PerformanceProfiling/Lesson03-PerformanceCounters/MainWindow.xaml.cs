using System;
using System.Diagnostics;
using System.Threading;
using System.Windows;
using System.Windows.Threading;

namespace Lesson03_PerformanceCounters
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// Demonstrates Windows Performance Counters for system and custom metrics
    /// </summary>
    public partial class MainWindow : Window
    {
        private DispatcherTimer? monitoringTimer;
        private const string CUSTOM_CATEGORY = "WPF Learning App";
        private const int UPDATE_INTERVAL_MS = 1000;

        // System Performance Counters
        private PerformanceCounter? cpuCounter;
        private PerformanceCounter? memoryCounter;
        private PerformanceCounter? diskCounter;
        private PerformanceCounter? processWorkingSetCounter;
        private PerformanceCounter? processPrivateBytesCounter;
        private PerformanceCounter? processThreadCountCounter;

        // Custom Performance Counters
        private PerformanceCounter? operationsPerSecCounter;
        private PerformanceCounter? totalOperationsCounter;
        private PerformanceCounter? avgOperationTimeCounter;
        private PerformanceCounter? avgOperationBaseCounter;
        private PerformanceCounter? activeConnectionsCounter;

        // Simulation variables
        private int operationCount = 0;
        private int connectionCount = 0;
        private readonly Random random = new();

        public MainWindow()
        {
            InitializeComponent();
            Loaded += MainWindow_Loaded;
            Closing += MainWindow_Closing;
        }

        private void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            InitializeSystemCounters();
            CheckCustomCountersExist();
        }

        private void MainWindow_Closing(object? sender, System.ComponentModel.CancelEventArgs e)
        {
            StopMonitoring();
            DisposeCounters();
        }

        #region System Performance Counters

        private void InitializeSystemCounters()
        {
            try
            {
                // CPU Counter
                cpuCounter = new PerformanceCounter("Processor", "% Processor Time", "_Total");

                // Memory Counter
                memoryCounter = new PerformanceCounter("Memory", "Available MBytes");

                // Disk Counter (may not be available on all systems)
                try
                {
                    diskCounter = new PerformanceCounter("PhysicalDisk", "% Disk Time", "_Total");
                }
                catch
                {
                    // Disk counter might not be available
                }

                // Process-specific counters
                string processName = Process.GetCurrentProcess().ProcessName;
                processWorkingSetCounter = new PerformanceCounter("Process", "Working Set", processName);
                processPrivateBytesCounter = new PerformanceCounter("Process", "Private Bytes", processName);
                processThreadCountCounter = new PerformanceCounter("Process", "Thread Count", processName);

                // Initial read to initialize counters
                cpuCounter.NextValue();
                memoryCounter.NextValue();
                diskCounter?.NextValue();

                StatusText.Text = "System performance counters initialized";
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error initializing system counters: {ex.Message}\n\n" +
                              "Some counters may require administrator privileges.",
                              "Initialization Error", MessageBoxButton.OK, MessageBoxImage.Warning);
            }
        }

        private void UpdateSystemCounters()
        {
            try
            {
                // CPU Usage
                if (cpuCounter != null)
                {
                    float cpuUsage = cpuCounter.NextValue();
                    CpuUsageText.Text = $"{cpuUsage:F1}%";
                    CpuProgressBar.Value = cpuUsage;
                }

                // Available Memory
                if (memoryCounter != null)
                {
                    float availableMemory = memoryCounter.NextValue();
                    MemoryText.Text = $"{availableMemory:F0} MB";
                    // Assume 16GB total for progress bar (adjust as needed)
                    MemoryProgressBar.Value = (availableMemory / 16384.0) * 100;
                }

                // Disk Time
                if (diskCounter != null)
                {
                    float diskTime = diskCounter.NextValue();
                    DiskTimeText.Text = $"{diskTime:F1}%";
                }

                // Process Working Set
                if (processWorkingSetCounter != null)
                {
                    float workingSet = processWorkingSetCounter.NextValue() / (1024 * 1024); // Convert to MB
                    WorkingSetText.Text = $"{workingSet:F1} MB";
                }

                // Process Private Bytes
                if (processPrivateBytesCounter != null)
                {
                    float privateBytes = processPrivateBytesCounter.NextValue() / (1024 * 1024); // Convert to MB
                    PrivateBytesText.Text = $"{privateBytes:F1} MB";
                }

                // Process Thread Count
                if (processThreadCountCounter != null)
                {
                    float threadCount = processThreadCountCounter.NextValue();
                    ThreadCountText.Text = $"{threadCount:F0}";
                }

                // Try to get network stats (may not be available)
                UpdateNetworkStats();
            }
            catch (Exception ex)
            {
                Debug.WriteLine($"Error updating system counters: {ex.Message}");
            }
        }

        private void UpdateNetworkStats()
        {
            try
            {
                // This is a simplified approach - actual network monitoring would need
                // to enumerate available network interfaces
                NetworkText.Text = "N/A";
            }
            catch
            {
                NetworkText.Text = "N/A";
            }
        }

        #endregion

        #region Custom Performance Counters

        private void CheckCustomCountersExist()
        {
            try
            {
                if (PerformanceCounterCategory.Exists(CUSTOM_CATEGORY))
                {
                    CounterStatusText.Text = "Status: Custom counters exist";
                    CounterStatusText.Foreground = System.Windows.Media.Brushes.Green;
                    InitializeCustomCounters();
                }
                else
                {
                    CounterStatusText.Text = "Status: Custom counters not created";
                    CounterStatusText.Foreground = System.Windows.Media.Brushes.Orange;
                }
            }
            catch (Exception ex)
            {
                CounterStatusText.Text = $"Status: Error checking counters - {ex.Message}";
                CounterStatusText.Foreground = System.Windows.Media.Brushes.Red;
            }
        }

        private void CreateCountersBtn_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                // Check if already exists
                if (PerformanceCounterCategory.Exists(CUSTOM_CATEGORY))
                {
                    MessageBox.Show("Custom performance counter category already exists!",
                                  "Already Exists", MessageBoxButton.OK, MessageBoxImage.Information);
                    return;
                }

                // Create the counter collection
                CounterCreationDataCollection counters = new CounterCreationDataCollection();

                // Operations Per Second (Rate Counter)
                CounterCreationData operationsPerSec = new CounterCreationData
                {
                    CounterName = "Operations/sec",
                    CounterHelp = "Number of operations processed per second",
                    CounterType = PerformanceCounterType.RateOfCountsPerSecond32
                };
                counters.Add(operationsPerSec);

                // Total Operations (Number Counter)
                CounterCreationData totalOperations = new CounterCreationData
                {
                    CounterName = "Total Operations",
                    CounterHelp = "Total number of operations since start",
                    CounterType = PerformanceCounterType.NumberOfItems64
                };
                counters.Add(totalOperations);

                // Average Operation Time (Average Counter)
                CounterCreationData avgOperationTime = new CounterCreationData
                {
                    CounterName = "Avg. Operation Time",
                    CounterHelp = "Average time per operation in milliseconds",
                    CounterType = PerformanceCounterType.AverageTimer32
                };
                counters.Add(avgOperationTime);

                // Base counter for average calculation
                CounterCreationData avgOperationBase = new CounterCreationData
                {
                    CounterName = "Avg. Operation Time Base",
                    CounterHelp = "Base counter for average operation time",
                    CounterType = PerformanceCounterType.AverageBase
                };
                counters.Add(avgOperationBase);

                // Active Connections (Number Counter)
                CounterCreationData activeConnections = new CounterCreationData
                {
                    CounterName = "Active Connections",
                    CounterHelp = "Number of currently active connections",
                    CounterType = PerformanceCounterType.NumberOfItems32
                };
                counters.Add(activeConnections);

                // Create the category with all counters
                PerformanceCounterCategory.Create(
                    CUSTOM_CATEGORY,
                    "Performance counters for WPF Learning Application",
                    PerformanceCounterCategoryType.SingleInstance,
                    counters);

                MessageBox.Show("Custom performance counters created successfully!\n\n" +
                              "You can now view them in Performance Monitor (perfmon.exe)",
                              "Success", MessageBoxButton.OK, MessageBoxImage.Information);

                CounterStatusText.Text = "Status: Custom counters created successfully";
                CounterStatusText.Foreground = System.Windows.Media.Brushes.Green;

                // Initialize the custom counters
                InitializeCustomCounters();
            }
            catch (UnauthorizedAccessException)
            {
                MessageBox.Show("Administrator privileges required to create performance counters.\n\n" +
                              "Please run this application as Administrator.",
                              "Access Denied", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error creating custom counters: {ex.Message}",
                              "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void DeleteCountersBtn_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!PerformanceCounterCategory.Exists(CUSTOM_CATEGORY))
                {
                    MessageBox.Show("Custom performance counter category does not exist!",
                                  "Not Found", MessageBoxButton.OK, MessageBoxImage.Information);
                    return;
                }

                var result = MessageBox.Show(
                    "Are you sure you want to delete the custom performance counter category?\n\n" +
                    "This will remove all custom counters.",
                    "Confirm Delete",
                    MessageBoxButton.YesNo,
                    MessageBoxImage.Warning);

                if (result == MessageBoxResult.Yes)
                {
                    // Dispose custom counters first
                    DisposeCustomCounters();

                    // Delete the category
                    PerformanceCounterCategory.Delete(CUSTOM_CATEGORY);

                    MessageBox.Show("Custom performance counters deleted successfully!",
                                  "Success", MessageBoxButton.OK, MessageBoxImage.Information);

                    CounterStatusText.Text = "Status: Custom counters deleted";
                    CounterStatusText.Foreground = System.Windows.Media.Brushes.Orange;

                    // Reset counter displays
                    OperationsPerSecText.Text = "0";
                    TotalOperationsText.Text = "0";
                    AvgOperationTimeText.Text = "0.00";
                    ActiveConnectionsText.Text = "0";
                }
            }
            catch (UnauthorizedAccessException)
            {
                MessageBox.Show("Administrator privileges required to delete performance counters.\n\n" +
                              "Please run this application as Administrator.",
                              "Access Denied", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error deleting custom counters: {ex.Message}",
                              "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void InitializeCustomCounters()
        {
            try
            {
                if (!PerformanceCounterCategory.Exists(CUSTOM_CATEGORY))
                {
                    return;
                }

                // Initialize custom counters in read/write mode
                operationsPerSecCounter = new PerformanceCounter(CUSTOM_CATEGORY, "Operations/sec", false);
                totalOperationsCounter = new PerformanceCounter(CUSTOM_CATEGORY, "Total Operations", false);
                avgOperationTimeCounter = new PerformanceCounter(CUSTOM_CATEGORY, "Avg. Operation Time", false);
                avgOperationBaseCounter = new PerformanceCounter(CUSTOM_CATEGORY, "Avg. Operation Time Base", false);
                activeConnectionsCounter = new PerformanceCounter(CUSTOM_CATEGORY, "Active Connections", false);

                // Reset all counters to zero
                operationsPerSecCounter.RawValue = 0;
                totalOperationsCounter.RawValue = 0;
                avgOperationTimeCounter.RawValue = 0;
                avgOperationBaseCounter.RawValue = 0;
                activeConnectionsCounter.RawValue = 0;

                SimulateOperationBtn.IsEnabled = true;
                AddConnectionBtn.IsEnabled = true;
                RemoveConnectionBtn.IsEnabled = true;
            }
            catch (Exception ex)
            {
                Debug.WriteLine($"Error initializing custom counters: {ex.Message}");
            }
        }

        private void UpdateCustomCounters()
        {
            try
            {
                if (operationsPerSecCounter != null)
                {
                    OperationsPerSecText.Text = $"{operationsPerSecCounter.NextValue():F2}";
                }

                if (totalOperationsCounter != null)
                {
                    TotalOperationsText.Text = $"{totalOperationsCounter.RawValue}";
                }

                if (avgOperationTimeCounter != null)
                {
                    AvgOperationTimeText.Text = $"{avgOperationTimeCounter.NextValue():F2}";
                }

                if (activeConnectionsCounter != null)
                {
                    ActiveConnectionsText.Text = $"{activeConnectionsCounter.RawValue}";
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine($"Error updating custom counters: {ex.Message}");
            }
        }

        #endregion

        #region Simulation Methods

        private void SimulateOperationBtn_Click(object sender, RoutedEventArgs e)
        {
            if (operationsPerSecCounter == null || totalOperationsCounter == null)
            {
                MessageBox.Show("Please create custom counters first!",
                              "Counters Not Created", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            // Simulate an operation with random duration
            int operationTimeMs = random.Next(10, 500);

            // Update counters
            operationsPerSecCounter.Increment();
            totalOperationsCounter.Increment();

            // Update average time counters
            avgOperationTimeCounter.IncrementBy(operationTimeMs);
            avgOperationBaseCounter.Increment();

            operationCount++;

            // Simulate the operation
            Thread.Sleep(operationTimeMs);

            StatusText.Text = $"Simulated operation #{operationCount} ({operationTimeMs}ms)";
        }

        private void AddConnectionBtn_Click(object sender, RoutedEventArgs e)
        {
            if (activeConnectionsCounter == null)
            {
                MessageBox.Show("Please create custom counters first!",
                              "Counters Not Created", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            connectionCount++;
            activeConnectionsCounter.RawValue = connectionCount;
            StatusText.Text = $"Added connection. Total: {connectionCount}";
        }

        private void RemoveConnectionBtn_Click(object sender, RoutedEventArgs e)
        {
            if (activeConnectionsCounter == null)
            {
                MessageBox.Show("Please create custom counters first!",
                              "Counters Not Created", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            if (connectionCount > 0)
            {
                connectionCount--;
                activeConnectionsCounter.RawValue = connectionCount;
                StatusText.Text = $"Removed connection. Total: {connectionCount}";
            }
            else
            {
                MessageBox.Show("No active connections to remove!",
                              "No Connections", MessageBoxButton.OK, MessageBoxImage.Information);
            }
        }

        #endregion

        #region Monitoring Control

        private void StartMonitoringBtn_Click(object sender, RoutedEventArgs e)
        {
            StartMonitoring();
        }

        private void StopMonitoringBtn_Click(object sender, RoutedEventArgs e)
        {
            StopMonitoring();
        }

        private void StartMonitoring()
        {
            if (monitoringTimer == null)
            {
                monitoringTimer = new DispatcherTimer
                {
                    Interval = TimeSpan.FromMilliseconds(UPDATE_INTERVAL_MS)
                };
                monitoringTimer.Tick += MonitoringTimer_Tick;
            }

            monitoringTimer.Start();

            StartMonitoringBtn.IsEnabled = false;
            StopMonitoringBtn.IsEnabled = true;
            StatusText.Text = "Monitoring active - updating every second";
        }

        private void StopMonitoring()
        {
            if (monitoringTimer != null)
            {
                monitoringTimer.Stop();
            }

            StartMonitoringBtn.IsEnabled = true;
            StopMonitoringBtn.IsEnabled = false;
            StatusText.Text = "Monitoring stopped";
        }

        private void MonitoringTimer_Tick(object? sender, EventArgs e)
        {
            UpdateSystemCounters();
            UpdateCustomCounters();
        }

        #endregion

        #region Cleanup

        private void DisposeCounters()
        {
            DisposeSystemCounters();
            DisposeCustomCounters();
        }

        private void DisposeSystemCounters()
        {
            cpuCounter?.Dispose();
            memoryCounter?.Dispose();
            diskCounter?.Dispose();
            processWorkingSetCounter?.Dispose();
            processPrivateBytesCounter?.Dispose();
            processThreadCountCounter?.Dispose();
        }

        private void DisposeCustomCounters()
        {
            operationsPerSecCounter?.Dispose();
            totalOperationsCounter?.Dispose();
            avgOperationTimeCounter?.Dispose();
            avgOperationBaseCounter?.Dispose();
            activeConnectionsCounter?.Dispose();

            operationsPerSecCounter = null;
            totalOperationsCounter = null;
            avgOperationTimeCounter = null;
            avgOperationBaseCounter = null;
            activeConnectionsCounter = null;
        }

        #endregion
    }
}
