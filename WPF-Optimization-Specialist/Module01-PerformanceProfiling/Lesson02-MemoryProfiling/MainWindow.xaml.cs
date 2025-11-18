using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Threading;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace Lesson02_MemoryProfiling
{
    public partial class MainWindow : Window
    {
        // Static collections that can cause leaks
        private static List<object> _staticLeakyList = new();

        // Timer references
        private List<DispatcherTimer> _leakyTimers = new();

        // Event subscribers that aren't cleaned up
        private List<LeakyEventPublisher> _eventPublishers = new();

        // Visual tree references
        private List<UIElement> _visualTreeReferences = new();

        // Binding references
        private List<FrameworkElement> _bindingLeaks = new();

        // Command references
        private List<ICommand> _commandLeaks = new();

        // ResourceDictionary references
        private List<ResourceDictionary> _resourceLeaks = new();

        // IDisposable objects not disposed
        private List<IDisposable> _disposableLeaks = new();

        // Dispatcher operations
        private List<DispatcherOperation> _dispatcherOperations = new();

        // Counter for leaked objects
        private int _leakedObjectCount = 0;

        public MainWindow()
        {
            InitializeComponent();
            AppendOutput("Memory Profiling Deep Dive - Interactive Demonstration");
            AppendOutput("═══════════════════════════════════════════════════════════════");
            AppendOutput("INSTRUCTIONS:");
            AppendOutput("1. Open Memory Profiler (Visual Studio → Debug → Performance Profiler)");
            AppendOutput("2. Select '.NET Object Allocation Tracking' and 'Memory Usage'");
            AppendOutput("3. Take snapshot BEFORE running leak scenarios");
            AppendOutput("4. Run leak scenarios multiple times");
            AppendOutput("5. Take snapshot AFTER and compare");
            AppendOutput("6. Analyze retention paths to find leak sources");
            AppendOutput("═══════════════════════════════════════════════════════════════\n");

            UpdateMemoryMetrics();

            // Start memory monitoring
            DispatcherTimer monitorTimer = new() { Interval = TimeSpan.FromSeconds(2) };
            monitorTimer.Tick += (s, e) => UpdateMemoryMetrics();
            monitorTimer.Start();
        }

        #region Leak Scenario 1: Event Handler Leak

        private void BtnEventHandlerLeak_Click(object sender, RoutedEventArgs e)
        {
            AppendOutput("\n═══════════════════════════════════════════════════════════════");
            AppendOutput("LEAK SCENARIO 1: Event Handler Memory Leak");
            AppendOutput("═══════════════════════════════════════════════════════════════");

            for (int i = 0; i < 100; i++)
            {
                var publisher = new LeakyEventPublisher();
                var subscriber = new LeakyEventSubscriber(publisher);

                // LEAK: Publisher keeps reference to subscriber through event
                // Even though subscriber goes out of scope, it's retained by publisher's event
                _eventPublishers.Add(publisher);
                _leakedObjectCount++;
            }

            AppendOutput($"Created 100 event publishers with subscribers");
            AppendOutput($"❌ PROBLEM: Event handlers create strong references");
            AppendOutput($"   - Publisher holds reference to subscriber through event");
            AppendOutput($"   - Subscriber cannot be garbage collected");
            AppendOutput($"   - Both objects leak even when subscriber is 'done'");
            AppendOutput($"\n✅ SOLUTION:");
            AppendOutput($"   - Unsubscribe from events: publisher.Event -= handler");
            AppendOutput($"   - Use WeakEventManager pattern (see Scenario 2)");
            AppendOutput($"   - Implement IDisposable and cleanup in Dispose()");
            AppendOutput($"\n🔍 PROFILER TIP:");
            AppendOutput($"   - Look for LeakyEventSubscriber instances in heap");
            AppendOutput($"   - Check retention path: shows event delegate chain");
            AppendOutput($"   - Count should match number of clicks × 100");

            TxtStatus.Text = "Event Handler Leak created - Check memory profiler";
        }

        #endregion

        #region Leak Scenario 2: WeakEventManager Missing

        private void BtnWeakEventManager_Click(object sender, RoutedEventArgs e)
        {
            AppendOutput("\n═══════════════════════════════════════════════════════════════");
            AppendOutput("LEAK SCENARIO 2: Not Using WeakEventManager");
            AppendOutput("═══════════════════════════════════════════════════════════════");

            for (int i = 0; i < 50; i++)
            {
                var notifyObject = new NotifyPropertyChangedObject();
                var window = new Window(); // Heavy object

                // LEAK: Strong reference through PropertyChanged event
                notifyObject.PropertyChanged += (s, e) =>
                {
                    // Window is captured in closure
                    var title = window.Title;
                };

                _eventPublishers.Add(notifyObject);
                _leakedObjectCount++;
            }

            AppendOutput($"Created 50 PropertyChanged subscriptions with Window closures");
            AppendOutput($"❌ PROBLEM: PropertyChanged creates strong references");
            AppendOutput($"   - INotifyPropertyChanged is common in WPF (ViewModels)");
            AppendOutput($"   - Each subscription prevents GC of subscriber");
            AppendOutput($"   - Closures capture additional objects (Window)");
            AppendOutput($"\n✅ SOLUTION:");
            AppendOutput($"   PropertyChangedEventManager.AddHandler(source, handler, \"\");");
            AppendOutput($"   - Uses weak references internally");
            AppendOutput($"   - Subscriber can be collected independently");
            AppendOutput($"\n🔍 PROFILER TIP:");
            AppendOutput($"   - Search for Window instances in unexpected count");
            AppendOutput($"   - Retention path shows PropertyChanged event chain");

            TxtStatus.Text = "WeakEventManager leak created - Should use WeakEventManager pattern";
        }

        #endregion

        #region Leak Scenario 3: Static Reference Leak

        private void BtnStaticReferenceLeak_Click(object sender, RoutedEventArgs e)
        {
            AppendOutput("\n═══════════════════════════════════════════════════════════════");
            AppendOutput("LEAK SCENARIO 3: Static Reference Memory Leak");
            AppendOutput("═══════════════════════════════════════════════════════════════");

            for (int i = 0; i < 100; i++)
            {
                var heavyObject = new HeavyObject(1024 * 100); // 100 KB each

                // LEAK: Static collection keeps objects alive forever
                _staticLeakyList.Add(heavyObject);
                _leakedObjectCount++;
            }

            long leakedMemory = _staticLeakyList.Count * 100;
            AppendOutput($"Added 100 objects (100 KB each) to static collection");
            AppendOutput($"Total leaked: ~{leakedMemory} KB ({leakedMemory / 1024.0:F2} MB)");
            AppendOutput($"❌ PROBLEM: Static fields live for application lifetime");
            AppendOutput($"   - Static collections accumulate objects");
            AppendOutput($"   - Objects never eligible for garbage collection");
            AppendOutput($"   - Common in singletons and service locators");
            AppendOutput($"\n✅ SOLUTION:");
            AppendOutput($"   - Avoid static collections or use WeakReference<T>");
            AppendOutput($"   - Implement cleanup methods to clear static data");
            AppendOutput($"   - Use ConditionalWeakTable<TKey, TValue> for associations");
            AppendOutput($"   - Consider scoped lifetime instead of singleton");
            AppendOutput($"\n🔍 PROFILER TIP:");
            AppendOutput($"   - Filter for objects with GC Root = 'static var'");
            AppendOutput($"   - Retention path shows: Static Field → Your Object");

            TxtStatus.Text = $"Static Reference Leak - {_staticLeakyList.Count} objects in static collection";
        }

        #endregion

        #region Leak Scenario 4: DispatcherTimer Leak

        private void BtnDispatcherTimerLeak_Click(object sender, RoutedEventArgs e)
        {
            AppendOutput("\n═══════════════════════════════════════════════════════════════");
            AppendOutput("LEAK SCENARIO 4: DispatcherTimer Memory Leak");
            AppendOutput("═══════════════════════════════════════════════════════════════");

            for (int i = 0; i < 50; i++)
            {
                var timer = new DispatcherTimer { Interval = TimeSpan.FromSeconds(1) };
                var heavyData = new byte[1024 * 50]; // 50 KB captured

                // LEAK: Timer Tick event captures 'heavyData' and 'this'
                timer.Tick += (s, e) =>
                {
                    // heavyData is captured in closure
                    var length = heavyData.Length;
                };

                timer.Start();
                _leakyTimers.Add(timer);
                _leakedObjectCount++;
            }

            AppendOutput($"Created 50 running DispatcherTimers with heavy closures");
            AppendOutput($"Each timer captures 50 KB of data");
            AppendOutput($"❌ PROBLEM: Running timers prevent garbage collection");
            AppendOutput($"   - Timer keeps ticking and holding references");
            AppendOutput($"   - Event handlers capture 'this' and closure variables");
            AppendOutput($"   - Timer internally registered with Dispatcher");
            AppendOutput($"\n✅ SOLUTION:");
            AppendOutput($"   timer.Stop(); // Stop the timer");
            AppendOutput($"   timer.Tick -= handler; // Unsubscribe");
            AppendOutput($"   - Or use weak event patterns");
            AppendOutput($"   - Implement IDisposable for cleanup");
            AppendOutput($"\n🔍 PROFILER TIP:");
            AppendOutput($"   - Look for DispatcherTimer instances");
            AppendOutput($"   - Check IsEnabled property (should be false when done)");
            AppendOutput($"   - Retention path: Dispatcher → Timer → Your closures");

            TxtStatus.Text = $"DispatcherTimer Leak - {_leakyTimers.Count} timers running";
        }

        #endregion

        #region Leak Scenario 5: DispatcherOperation Leak

        private void BtnDispatcherOperationLeak_Click(object sender, RoutedEventArgs e)
        {
            AppendOutput("\n═══════════════════════════════════════════════════════════════");
            AppendOutput("LEAK SCENARIO 5: DispatcherOperation Memory Leak");
            AppendOutput("═══════════════════════════════════════════════════════════════");

            for (int i = 0; i < 100; i++)
            {
                var heavyObject = new HeavyObject(1024 * 20); // 20 KB

                // LEAK: Keeping reference to DispatcherOperation
                var operation = Dispatcher.BeginInvoke(new Action(() =>
                {
                    // Heavy object captured in closure
                    var size = heavyObject.Data.Length;
                }), DispatcherPriority.Background);

                _dispatcherOperations.Add(operation);
                _leakedObjectCount++;
            }

            AppendOutput($"Created 100 DispatcherOperations with heavy closures");
            AppendOutput($"❌ PROBLEM: DispatcherOperation references kept alive");
            AppendOutput($"   - BeginInvoke returns DispatcherOperation");
            AppendOutput($"   - Storing operation reference prevents cleanup");
            AppendOutput($"   - Operation holds closure with captured variables");
            AppendOutput($"\n✅ SOLUTION:");
            AppendOutput($"   - Don't store DispatcherOperation if not needed");
            AppendOutput($"   - Use await Dispatcher.InvokeAsync() instead");
            AppendOutput($"   - Clear operation list after execution");
            AppendOutput($"\n🔍 PROFILER TIP:");
            AppendOutput($"   - Search for DispatcherOperation instances");
            AppendOutput($"   - Check Status property (Completed operations can be released)");

            TxtStatus.Text = "DispatcherOperation Leak created";
        }

        #endregion

        #region Leak Scenario 6: Visual Tree Reference Leak

        private void BtnVisualTreeLeak_Click(object sender, RoutedEventArgs e)
        {
            AppendOutput("\n═══════════════════════════════════════════════════════════════");
            AppendOutput("LEAK SCENARIO 6: Visual Tree Reference Memory Leak");
            AppendOutput("═══════════════════════════════════════════════════════════════");

            for (int i = 0; i < 50; i++)
            {
                // Create disconnected visual tree
                var grid = new Grid();
                for (int j = 0; j < 100; j++)
                {
                    grid.Children.Add(new TextBlock { Text = $"Element {j}" });
                }

                // LEAK: Keeping reference to visual elements not in window
                _visualTreeReferences.Add(grid);
                _leakedObjectCount++;
            }

            AppendOutput($"Created 50 Grid objects with 100 TextBlocks each (5000 elements)");
            AppendOutput($"❌ PROBLEM: Detached visual elements retained in memory");
            AppendOutput($"   - Elements not in current window tree");
            AppendOutput($"   - Still consuming memory (rendering resources, etc.)");
            AppendOutput($"   - Common when caching views or using tab controls");
            AppendOutput($"\n✅ SOLUTION:");
            AppendOutput($"   - Clear references when views are not needed");
            AppendOutput($"   - Use virtualization for large lists");
            AppendOutput($"   - Consider recreating UI instead of caching");
            AppendOutput($"   grid.Children.Clear(); // Release child elements");
            AppendOutput($"\n🔍 PROFILER TIP:");
            AppendOutput($"   - Look for Grid, TextBlock in unexpected quantities");
            AppendOutput($"   - Check if Parent property is null (detached)");

            TxtStatus.Text = "Visual Tree Leak - 5000+ detached UI elements";
        }

        #endregion

        #region Leak Scenario 7: Binding Memory Leak

        private void BtnBindingLeak_Click(object sender, RoutedEventArgs e)
        {
            AppendOutput("\n═══════════════════════════════════════════════════════════════");
            AppendOutput("LEAK SCENARIO 7: Data Binding Memory Leak");
            AppendOutput("═══════════════════════════════════════════════════════════════");

            for (int i = 0; i < 100; i++)
            {
                var textBlock = new TextBlock();
                var viewModel = new LeakyViewModel();

                // LEAK: Binding creates strong reference from source to target
                var binding = new Binding("LargeData")
                {
                    Source = viewModel,
                    Mode = BindingMode.OneWay
                };

                BindingOperations.SetBinding(textBlock, TextBlock.TextProperty, binding);

                _bindingLeaks.Add(textBlock);
                _leakedObjectCount++;
            }

            AppendOutput($"Created 100 TextBlocks with bindings to ViewModels");
            AppendOutput($"❌ PROBLEM: Bindings create strong references");
            AppendOutput($"   - Binding engine keeps source alive");
            AppendOutput($"   - Target (TextBlock) and Source (ViewModel) both retained");
            AppendOutput($"   - Common in dynamic UI scenarios");
            AppendOutput($"\n✅ SOLUTION:");
            AppendOutput($"   BindingOperations.ClearBinding(element, property);");
            AppendOutput($"   BindingOperations.ClearAllBindings(element);");
            AppendOutput($"   - Clear bindings when removing elements");
            AppendOutput($"   - Use {x:Null} to clear bindings in XAML");
            AppendOutput($"\n🔍 PROFILER TIP:");
            AppendOutput($"   - Search for BindingExpression objects");
            AppendOutput($"   - Retention path: BindingExpression → ViewModel");

            TxtStatus.Text = "Binding Leak created - 100 active bindings";
        }

        #endregion

        #region Leak Scenario 8: Command Binding Leak

        private void BtnCommandBindingLeak_Click(object sender, RoutedEventArgs e)
        {
            AppendOutput("\n═══════════════════════════════════════════════════════════════");
            AppendOutput("LEAK SCENARIO 8: Command Binding Memory Leak");
            AppendOutput("═══════════════════════════════════════════════════════════════");

            for (int i = 0; i < 100; i++)
            {
                var button = new Button();
                var viewModel = new LeakyViewModel();
                var command = new LeakyCommand(viewModel);

                // LEAK: Command holds reference to ViewModel
                // Button holds reference to Command
                button.Command = command;

                _commandLeaks.Add(command);
                _bindingLeaks.Add(button);
                _leakedObjectCount++;
            }

            AppendOutput($"Created 100 Buttons with Commands referencing ViewModels");
            AppendOutput($"❌ PROBLEM: Commands create reference chains");
            AppendOutput($"   - Button → Command → ViewModel");
            AppendOutput($"   - CanExecuteChanged event creates additional references");
            AppendOutput($"   - CommandManager.RequerySuggested holds references");
            AppendOutput($"\n✅ SOLUTION:");
            AppendOutput($"   button.Command = null; // Clear command");
            AppendOutput($"   - Unsubscribe from CanExecuteChanged");
            AppendOutput($"   - Use weak event pattern for CanExecuteChanged");
            AppendOutput($"   - Consider RelayCommand with weak references");
            AppendOutput($"\n🔍 PROFILER TIP:");
            AppendOutput($"   - Look for ICommand implementations");
            AppendOutput($"   - Check CommandManager subscribers");

            TxtStatus.Text = "Command Binding Leak - 100 command references";
        }

        #endregion

        #region Leak Scenario 9: ResourceDictionary Leak

        private void BtnResourceDictionaryLeak_Click(object sender, RoutedEventArgs e)
        {
            AppendOutput("\n═══════════════════════════════════════════════════════════════");
            AppendOutput("LEAK SCENARIO 9: ResourceDictionary Memory Leak");
            AppendOutput("═══════════════════════════════════════════════════════════════");

            for (int i = 0; i < 50; i++)
            {
                var resourceDict = new ResourceDictionary();

                // Add many resources
                for (int j = 0; j < 100; j++)
                {
                    var brush = new System.Windows.Media.SolidColorBrush(
                        System.Windows.Media.Color.FromRgb(
                            (byte)(i % 256),
                            (byte)(j % 256),
                            128));
                    resourceDict.Add($"Brush_{i}_{j}", brush);
                }

                // LEAK: ResourceDictionary not merged/cleared properly
                _resourceLeaks.Add(resourceDict);
                _leakedObjectCount++;
            }

            AppendOutput($"Created 50 ResourceDictionaries with 100 brushes each");
            AppendOutput($"Total: 5000 SolidColorBrush instances");
            AppendOutput($"❌ PROBLEM: ResourceDictionaries accumulate resources");
            AppendOutput($"   - Dynamically created resources not released");
            AppendOutput($"   - Merged dictionaries create complex chains");
            AppendOutput($"   - Styles and templates hold references");
            AppendOutput($"\n✅ SOLUTION:");
            AppendOutput($"   resourceDict.Clear(); // Clear resources");
            AppendOutput($"   element.Resources = null; // Clear element resources");
            AppendOutput($"   - Unmerge dictionaries when done");
            AppendOutput($"   - Don't cache ResourceDictionaries unnecessarily");
            AppendOutput($"\n🔍 PROFILER TIP:");
            AppendOutput($"   - Search for ResourceDictionary instances");
            AppendOutput($"   - Look for SolidColorBrush, Style objects");

            TxtStatus.Text = "ResourceDictionary Leak - 5000 brushes retained";
        }

        #endregion

        #region Leak Scenario 10: IDisposable Not Called

        private void BtnDisposableLeak_Click(object sender, RoutedEventArgs e)
        {
            AppendOutput("\n═══════════════════════════════════════════════════════════════");
            AppendOutput("LEAK SCENARIO 10: IDisposable Resources Not Disposed");
            AppendOutput("═══════════════════════════════════════════════════════════════");

            for (int i = 0; i < 100; i++)
            {
                // LEAK: Creating disposable objects without disposing
                var stream = new System.IO.MemoryStream(new byte[1024 * 50]); // 50 KB
                var reader = new System.IO.StreamReader(stream);

                // These should be disposed but aren't
                _disposableLeaks.Add(stream);
                _disposableLeaks.Add(reader);
                _leakedObjectCount += 2;
            }

            AppendOutput($"Created 200 IDisposable objects (MemoryStream, StreamReader)");
            AppendOutput($"Total leaked: ~{100 * 50} KB from streams alone");
            AppendOutput($"❌ PROBLEM: Unmanaged resources not released");
            AppendOutput($"   - File handles, database connections stay open");
            AppendOutput($"   - Memory not released until finalizer runs");
            AppendOutput($"   - Can exhaust system resources");
            AppendOutput($"\n✅ SOLUTION:");
            AppendOutput($"   using (var stream = new MemoryStream()) {{");
            AppendOutput($"       // Use stream");
            AppendOutput($"   }} // Automatically disposed");
            AppendOutput($"   Or:");
            AppendOutput($"   stream.Dispose();");
            AppendOutput($"\n🔍 PROFILER TIP:");
            AppendOutput($"   - Look for finalization queue length");
            AppendOutput($"   - Search for Stream, Connection, Handle types");
            AppendOutput($"   - Check for objects with finalizers not yet run");

            TxtStatus.Text = $"IDisposable Leak - {_disposableLeaks.Count} undisposed objects";
        }

        #endregion

        #region Cleanup and Utilities

        private void BtnForceGC_Click(object sender, RoutedEventArgs e)
        {
            AppendOutput("\n═══════════════════════════════════════════════════════════════");
            AppendOutput("Forcing Garbage Collection...");

            long before = GC.GetTotalMemory(false);

            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();

            long after = GC.GetTotalMemory(false);
            long freed = before - after;

            AppendOutput($"Memory before GC: {before / 1024 / 1024:N2} MB");
            AppendOutput($"Memory after GC:  {after / 1024 / 1024:N2} MB");
            AppendOutput($"Memory freed:     {freed / 1024 / 1024:N2} MB");
            AppendOutput($"\n💡 NOTE: Leaked objects will NOT be freed by GC!");
            AppendOutput($"   - They are still referenced (that's what makes them leaks)");
            AppendOutput($"   - Use memory profiler to find retention paths");

            UpdateMemoryMetrics();
            TxtStatus.Text = "Garbage Collection completed";
        }

        private void BtnCleanupLeaks_Click(object sender, RoutedEventArgs e)
        {
            AppendOutput("\n═══════════════════════════════════════════════════════════════");
            AppendOutput("Cleaning up ALL memory leaks...");

            // Cleanup event handlers
            foreach (var publisher in _eventPublishers)
            {
                if (publisher is IDisposable disposable)
                    disposable.Dispose();
            }
            _eventPublishers.Clear();

            // Stop and cleanup timers
            foreach (var timer in _leakyTimers)
            {
                timer.Stop();
                timer.Tick -= null; // Can't actually remove all handlers, but stopping helps
            }
            _leakyTimers.Clear();

            // Clear visual tree references
            foreach (var element in _visualTreeReferences)
            {
                if (element is Panel panel)
                    panel.Children.Clear();
            }
            _visualTreeReferences.Clear();

            // Clear bindings
            foreach (var element in _bindingLeaks)
            {
                BindingOperations.ClearAllBindings(element);
            }
            _bindingLeaks.Clear();

            // Clear commands
            _commandLeaks.Clear();

            // Clear resource dictionaries
            foreach (var dict in _resourceLeaks)
            {
                dict.Clear();
            }
            _resourceLeaks.Clear();

            // Dispose IDisposable objects
            foreach (var disposable in _disposableLeaks)
            {
                disposable.Dispose();
            }
            _disposableLeaks.Clear();

            // Clear dispatcher operations
            _dispatcherOperations.Clear();

            // Clear static list
            _staticLeakyList.Clear();

            _leakedObjectCount = 0;

            AppendOutput($"✅ All leak references cleared!");
            AppendOutput($"   - Event handlers cleared");
            AppendOutput($"   - Timers stopped");
            AppendOutput($"   - Visual tree references cleared");
            AppendOutput($"   - Bindings cleared");
            AppendOutput($"   - Resource dictionaries cleared");
            AppendOutput($"   - Disposable objects disposed");
            AppendOutput($"\n💡 Now force GC to reclaim memory");

            TxtStatus.Text = "All leaks cleaned up - Force GC to reclaim memory";
        }

        private void BtnClearResults_Click(object sender, RoutedEventArgs e)
        {
            TxtOutput.Text = "";
            TxtStatus.Text = "Results cleared";
            UpdateMemoryMetrics();
        }

        private void UpdateMemoryMetrics()
        {
            long totalMemory = GC.GetTotalMemory(false);
            TxtTotalMemory.Text = $"{totalMemory / 1024 / 1024:N2} MB";
            TxtGen0.Text = GC.CollectionCount(0).ToString();
            TxtGen1.Text = GC.CollectionCount(1).ToString();
            TxtGen2.Text = GC.CollectionCount(2).ToString();
            TxtLeakedObjects.Text = _leakedObjectCount.ToString();
        }

        private void AppendOutput(string text)
        {
            TxtOutput.Text += text + "\n";
        }

        #endregion
    }

    #region Helper Classes for Leak Demonstrations

    // Scenario 1: Event handler leak
    public class LeakyEventPublisher : IDisposable
    {
        public event EventHandler? SomeEvent;

        public void RaiseEvent()
        {
            SomeEvent?.Invoke(this, EventArgs.Empty);
        }

        public void Dispose()
        {
            SomeEvent = null;
        }
    }

    public class LeakyEventSubscriber
    {
        private byte[] _data = new byte[1024 * 10]; // 10 KB

        public LeakyEventSubscriber(LeakyEventPublisher publisher)
        {
            // LEAK: This creates strong reference
            publisher.SomeEvent += OnEvent;
        }

        private void OnEvent(object? sender, EventArgs e)
        {
            // Event handler
        }
    }

    // Scenario 2: WeakEventManager
    public class NotifyPropertyChangedObject : INotifyPropertyChanged, IDisposable
    {
        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public void Dispose()
        {
            PropertyChanged = null;
        }
    }

    // Scenario 3: Heavy object for static leak
    public class HeavyObject
    {
        public byte[] Data { get; }

        public HeavyObject(int sizeInBytes)
        {
            Data = new byte[sizeInBytes];
        }
    }

    // Scenario 7 & 8: Leaky ViewModel
    public class LeakyViewModel : INotifyPropertyChanged
    {
        private string _largeData = new string('X', 10000); // 10 KB string

        public string LargeData
        {
            get => _largeData;
            set
            {
                _largeData = value;
                OnPropertyChanged();
            }
        }

        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }

    // Scenario 8: Leaky Command
    public class LeakyCommand : ICommand
    {
        private readonly LeakyViewModel _viewModel;

        public LeakyCommand(LeakyViewModel viewModel)
        {
            _viewModel = viewModel;
            // LEAK: Subscribing to CommandManager
            CommandManager.RequerySuggested += OnRequerySuggested;
        }

        private void OnRequerySuggested(object? sender, EventArgs e)
        {
            CanExecuteChanged?.Invoke(this, EventArgs.Empty);
        }

        public event EventHandler? CanExecuteChanged;

        public bool CanExecute(object? parameter) => true;

        public void Execute(object? parameter)
        {
            // Command execution
        }
    }

    #endregion
}
