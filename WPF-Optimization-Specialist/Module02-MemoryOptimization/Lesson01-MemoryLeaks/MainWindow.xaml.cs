using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Threading;

namespace MemoryLeaks;

public partial class MainWindow : Window
{
    // Scenario 1: Event Handler Leaks
    public static event EventHandler? StaticEvent;
    private static int _leakyEventWindowCount = 0;
    private static int _fixedEventWindowCount = 0;

    // Scenario 2: Static References
    private static readonly List<object> StaticObjectList = new();
    private static readonly List<WeakReference> StaticWeakReferenceList = new();

    // Scenario 5: Collection Change Notifications
    private static readonly ObservableCollection<string> SharedCollection = new();
    private readonly List<LeakyCollectionSubscriber> _leakySubscribers = new();
    private readonly List<FixedCollectionSubscriber> _fixedSubscribers = new();

    // Scenario 6: Routed Events
    private int _routedEventHandlerCount = 0;
    private readonly List<UIElement> _elementsWithHandlers = new();

    // Scenario 7: Commands
    private static int _leakyCommandCount = 0;
    private static int _fixedCommandCount = 0;

    // Scenario 9: Visual Tree References
    private readonly List<UIElement> _leakedVisualElements = new();
    private int _properVisualCount = 0;

    // Scenario 10: DataContext Leaks
    private static int _leakyViewModelCount = 0;
    private static int _fixedViewModelCount = 0;

    private readonly DispatcherTimer _memoryUpdateTimer;

    public MainWindow()
    {
        InitializeComponent();

        _memoryUpdateTimer = new DispatcherTimer
        {
            Interval = TimeSpan.FromSeconds(2)
        };
        _memoryUpdateTimer.Tick += (s, e) => UpdateMemoryStatistics();
        _memoryUpdateTimer.Start();

        UpdateMemoryStatistics();
    }

    #region Scenario 1: Event Handler Leaks

    private void CreateLeakyEventWindow_Click(object sender, RoutedEventArgs e)
    {
        var window = new LeakyEventWindow();
        window.Show();
        UpdateEventLeakCount();
    }

    private void CreateFixedEventWindow_Click(object sender, RoutedEventArgs e)
    {
        var window = new FixedEventWindow();
        window.Show();
        UpdateEventLeakCount();
    }

    private void UpdateEventLeakCount()
    {
        EventLeakInstanceCount.Text = $"Leaky: {_leakyEventWindowCount}, Fixed: {_fixedEventWindowCount}";
    }

    public static void RaiseStaticEvent()
    {
        StaticEvent?.Invoke(null, EventArgs.Empty);
    }

    #endregion

    #region Scenario 2: Static References

    private void AddToStaticList_Click(object sender, RoutedEventArgs e)
    {
        // Add large object to static list (causes leak)
        var largeObject = new byte[1024 * 1024]; // 1MB
        StaticObjectList.Add(largeObject);
        UpdateStaticListCount();
    }

    private void ClearStaticList_Click(object sender, RoutedEventArgs e)
    {
        StaticObjectList.Clear();
        UpdateStaticListCount();
    }

    private void AddWeakReference_Click(object sender, RoutedEventArgs e)
    {
        // Add as weak reference (allows GC)
        var largeObject = new byte[1024 * 1024]; // 1MB
        StaticWeakReferenceList.Add(new WeakReference(largeObject));
        UpdateStaticListCount();
    }

    private void UpdateStaticListCount()
    {
        var aliveWeakRefs = StaticWeakReferenceList.Count(wr => wr.IsAlive);
        StaticListCount.Text = $"Strong: {StaticObjectList.Count}, Weak: {aliveWeakRefs}/{StaticWeakReferenceList.Count}";
    }

    #endregion

    #region Scenario 3: Timer Leaks

    private void CreateLeakyTimerWindow_Click(object sender, RoutedEventArgs e)
    {
        var window = new LeakyTimerWindow();
        window.Show();
        UpdateTimerWindowCount();
    }

    private void CreateFixedTimerWindow_Click(object sender, RoutedEventArgs e)
    {
        var window = new FixedTimerWindow();
        window.Show();
        UpdateTimerWindowCount();
    }

    private void UpdateTimerWindowCount()
    {
        TimerWindowCount.Text = $"Leaky: {LeakyTimerWindow.InstanceCount}, Fixed: {FixedTimerWindow.InstanceCount}";
    }

    #endregion

    #region Scenario 4: Dependency Property Callbacks

    private void CreateLeakyDPControls_Click(object sender, RoutedEventArgs e)
    {
        for (int i = 0; i < 5; i++)
        {
            var control = new LeakyDPControl
            {
                Height = 30,
                Margin = new Thickness(5)
            };
            DPControlsPanel.Children.Add(control);
        }
    }

    private void CreateFixedDPControls_Click(object sender, RoutedEventArgs e)
    {
        for (int i = 0; i < 5; i++)
        {
            var control = new FixedDPControl
            {
                Height = 30,
                Margin = new Thickness(5)
            };
            DPControlsPanel.Children.Add(control);
        }
    }

    private void ClearDPPanel_Click(object sender, RoutedEventArgs e)
    {
        DPControlsPanel.Children.Clear();
    }

    #endregion

    #region Scenario 5: Collection Change Notifications

    private void AddLeakyCollectionSubscriber_Click(object sender, RoutedEventArgs e)
    {
        var subscriber = new LeakyCollectionSubscriber(SharedCollection);
        _leakySubscribers.Add(subscriber);
        UpdateCollectionSubscriberCount();
    }

    private void AddFixedCollectionSubscriber_Click(object sender, RoutedEventArgs e)
    {
        var subscriber = new FixedCollectionSubscriber(SharedCollection);
        _fixedSubscribers.Add(subscriber);
        UpdateCollectionSubscriberCount();
    }

    private void ModifyCollection_Click(object sender, RoutedEventArgs e)
    {
        SharedCollection.Add($"Item {SharedCollection.Count + 1}");
    }

    private void UpdateCollectionSubscriberCount()
    {
        CollectionSubscriberCount.Text = $"Leaky: {_leakySubscribers.Count}, Fixed: {_fixedSubscribers.Count}";
    }

    #endregion

    #region Scenario 6: Routed Events

    private void AddLeakyRoutedHandlers_Click(object sender, RoutedEventArgs e)
    {
        var button = new Button { Content = "Test Button", Margin = new Thickness(5) };

        // Add handler but never remove it
        button.PreviewMouseDown += LeakyMouseDownHandler;

        _elementsWithHandlers.Add(button);
        _routedEventHandlerCount++;

        UpdateRoutedEventHandlerCount();
    }

    private void AddFixedRoutedHandlers_Click(object sender, RoutedEventArgs e)
    {
        var button = new Button { Content = "Test Button", Margin = new Thickness(5) };

        // Will be properly removed
        RoutedEventHandler handler = FixedMouseDownHandler;
        button.PreviewMouseDown += handler;

        // Store for later removal
        button.Tag = handler;
        _elementsWithHandlers.Add(button);
        _routedEventHandlerCount++;

        UpdateRoutedEventHandlerCount();
    }

    private void ClearRoutedHandlers_Click(object sender, RoutedEventArgs e)
    {
        foreach (var element in _elementsWithHandlers)
        {
            if (element is Button button && button.Tag is RoutedEventHandler handler)
            {
                button.PreviewMouseDown -= handler;
            }
        }

        _elementsWithHandlers.Clear();
        _routedEventHandlerCount = 0;
        UpdateRoutedEventHandlerCount();
    }

    private void LeakyMouseDownHandler(object sender, MouseButtonEventArgs e)
    {
        // Handler that never gets removed
    }

    private void FixedMouseDownHandler(object sender, RoutedEventArgs e)
    {
        // Handler that gets properly removed
    }

    private void UpdateRoutedEventHandlerCount()
    {
        RoutedEventHandlerCount.Text = $"Attached: {_routedEventHandlerCount}";
    }

    #endregion

    #region Scenario 7: Command Bindings

    private void CreateLeakyCommandBinding_Click(object sender, RoutedEventArgs e)
    {
        var window = new LeakyCommandWindow();
        window.Show();
        UpdateCommandInstanceCount();
    }

    private void CreateFixedCommandBinding_Click(object sender, RoutedEventArgs e)
    {
        var window = new FixedCommandWindow();
        window.Show();
        UpdateCommandInstanceCount();
    }

    private void UpdateCommandInstanceCount()
    {
        CommandInstanceCount.Text = $"Leaky: {_leakyCommandCount}, Fixed: {_fixedCommandCount}";
    }

    #endregion

    #region Scenario 8: Resource Dictionaries

    private void LoadHeavyResources_Click(object sender, RoutedEventArgs e)
    {
        var dict = new ResourceDictionary();

        // Add some heavy resources
        for (int i = 0; i < 10; i++)
        {
            var gradient = new LinearGradientBrush();
            gradient.GradientStops.Add(new GradientStop(Colors.Red, 0.0));
            gradient.GradientStops.Add(new GradientStop(Colors.Blue, 1.0));
            dict.Add($"Gradient{i}", gradient);
        }

        Resources.MergedDictionaries.Add(dict);
        UpdateResourceDictionaryCount();
    }

    private void UnloadResources_Click(object sender, RoutedEventArgs e)
    {
        Resources.MergedDictionaries.Clear();
        UpdateResourceDictionaryCount();
    }

    private void UseFreezableResources_Click(object sender, RoutedEventArgs e)
    {
        var dict = new ResourceDictionary();

        // Add frozen resources (more efficient)
        for (int i = 0; i < 10; i++)
        {
            var gradient = new LinearGradientBrush();
            gradient.GradientStops.Add(new GradientStop(Colors.Green, 0.0));
            gradient.GradientStops.Add(new GradientStop(Colors.Yellow, 1.0));
            gradient.Freeze(); // Freeze for better performance and sharing
            dict.Add($"FrozenGradient{i}", gradient);
        }

        Resources.MergedDictionaries.Add(dict);
        UpdateResourceDictionaryCount();
    }

    private void UpdateResourceDictionaryCount()
    {
        ResourceDictionaryCount.Text = $"Merged: {Resources.MergedDictionaries.Count}";
    }

    #endregion

    #region Scenario 9: Visual Tree References

    private void CreateVisualTreeLeak_Click(object sender, RoutedEventArgs e)
    {
        var button = new Button
        {
            Content = $"Leaked Button {_leakedVisualElements.Count + 1}",
            Tag = new byte[1024 * 100] // 100KB
        };

        // Keep reference (causes leak)
        _leakedVisualElements.Add(button);

        UpdateVisualElementCount();
    }

    private void CreateVisualTreeFixed_Click(object sender, RoutedEventArgs e)
    {
        var button = new Button
        {
            Content = $"Proper Button {_properVisualCount + 1}",
            Tag = new byte[1024 * 100] // 100KB
        };

        // Don't keep reference (allows GC)
        _properVisualCount++;

        UpdateVisualElementCount();
    }

    private void ClearVisualReferences_Click(object sender, RoutedEventArgs e)
    {
        _leakedVisualElements.Clear();
        _properVisualCount = 0;
        UpdateVisualElementCount();
    }

    private void UpdateVisualElementCount()
    {
        VisualElementCount.Text = $"Leaked: {_leakedVisualElements.Count}, Proper: {_properVisualCount}";
    }

    #endregion

    #region Scenario 10: DataContext Leaks

    private void CreateLeakyViewModelWindow_Click(object sender, RoutedEventArgs e)
    {
        var window = new LeakyViewModelWindow();
        window.Show();
        UpdateViewModelInstanceCount();
    }

    private void CreateFixedViewModelWindow_Click(object sender, RoutedEventArgs e)
    {
        var window = new FixedViewModelWindow();
        window.Show();
        UpdateViewModelInstanceCount();
    }

    private void UpdateViewModelInstanceCount()
    {
        ViewModelInstanceCount.Text = $"Leaky: {_leakyViewModelCount}, Fixed: {_fixedViewModelCount}";
    }

    #endregion

    #region Common Methods

    private void CloseAllChildWindows_Click(object sender, RoutedEventArgs e)
    {
        var windows = Application.Current.Windows.Cast<Window>().ToList();
        foreach (var window in windows)
        {
            if (window != this)
            {
                window.Close();
            }
        }
    }

    private void ForceGC_Click(object sender, RoutedEventArgs e)
    {
        GC.Collect();
        GC.WaitForPendingFinalizers();
        GC.Collect();
        UpdateMemoryStatistics();
    }

    private void UpdateStats_Click(object sender, RoutedEventArgs e)
    {
        UpdateMemoryStatistics();
        UpdateEventLeakCount();
        UpdateStaticListCount();
        UpdateTimerWindowCount();
        UpdateCollectionSubscriberCount();
        UpdateRoutedEventHandlerCount();
        UpdateCommandInstanceCount();
        UpdateResourceDictionaryCount();
        UpdateVisualElementCount();
        UpdateViewModelInstanceCount();
    }

    private void UpdateMemoryStatistics()
    {
        var memoryInMB = GC.GetTotalMemory(false) / (1024.0 * 1024.0);
        MemoryUsageText.Text = $"{memoryInMB:F2} MB";
        Gen0Count.Text = GC.CollectionCount(0).ToString();
        Gen1Count.Text = GC.CollectionCount(1).ToString();
        Gen2Count.Text = GC.CollectionCount(2).ToString();
    }

    protected override void OnClosing(CancelEventArgs e)
    {
        _memoryUpdateTimer.Stop();
        base.OnClosing(e);
    }

    #endregion
}

#region Scenario 1: Event Handler Windows

public class LeakyEventWindow : Window
{
    public LeakyEventWindow()
    {
        Title = "Leaky Event Window";
        Width = 300;
        Height = 200;
        Content = new TextBlock
        {
            Text = "This window subscribes to a static event\nbut never unsubscribes.\n\nClose and check memory!",
            TextAlignment = TextAlignment.Center,
            VerticalAlignment = VerticalAlignment.Center,
            TextWrapping = TextWrapping.Wrap
        };

        // Subscribe to static event but never unsubscribe - MEMORY LEAK!
        MainWindow.StaticEvent += OnStaticEvent;

        Interlocked.Increment(ref MainWindow._leakyEventWindowCount);
    }

    private void OnStaticEvent(object? sender, EventArgs e)
    {
        // This keeps the window alive even after closing
    }
}

public class FixedEventWindow : Window
{
    public FixedEventWindow()
    {
        Title = "Fixed Event Window";
        Width = 300;
        Height = 200;
        Content = new TextBlock
        {
            Text = "This window properly unsubscribes\nfrom the static event.\n\nClose and check memory!",
            TextAlignment = TextAlignment.Center,
            VerticalAlignment = VerticalAlignment.Center,
            TextWrapping = TextWrapping.Wrap
        };

        // Subscribe to static event
        MainWindow.StaticEvent += OnStaticEvent;

        // Unsubscribe when closing - PROPER!
        Closed += (s, e) =>
        {
            MainWindow.StaticEvent -= OnStaticEvent;
            Interlocked.Decrement(ref MainWindow._fixedEventWindowCount);
        };

        Interlocked.Increment(ref MainWindow._fixedEventWindowCount);
    }

    private void OnStaticEvent(object? sender, EventArgs e)
    {
        // Handler that gets properly removed
    }
}

#endregion

#region Scenario 3: Timer Windows

public class LeakyTimerWindow : Window
{
    public static int InstanceCount = 0;
    private readonly DispatcherTimer _timer;
    private int _tickCount = 0;
    private readonly TextBlock _statusText;

    public LeakyTimerWindow()
    {
        Title = "Leaky Timer Window";
        Width = 300;
        Height = 200;

        _statusText = new TextBlock
        {
            Text = "Timer ticking...\nTicks: 0\n\nClose this window and watch it leak!",
            TextAlignment = TextAlignment.Center,
            VerticalAlignment = VerticalAlignment.Center,
            TextWrapping = TextWrapping.Wrap
        };
        Content = _statusText;

        _timer = new DispatcherTimer { Interval = TimeSpan.FromSeconds(1) };
        _timer.Tick += OnTick;
        _timer.Start();

        Interlocked.Increment(ref InstanceCount);

        // Timer never stopped - MEMORY LEAK!
    }

    private void OnTick(object? sender, EventArgs e)
    {
        _tickCount++;
        _statusText.Text = $"Timer ticking...\nTicks: {_tickCount}\n\nClose this window and watch it leak!";
    }
}

public class FixedTimerWindow : Window
{
    public static int InstanceCount = 0;
    private readonly DispatcherTimer _timer;
    private int _tickCount = 0;
    private readonly TextBlock _statusText;

    public FixedTimerWindow()
    {
        Title = "Fixed Timer Window";
        Width = 300;
        Height = 200;

        _statusText = new TextBlock
        {
            Text = "Timer ticking...\nTicks: 0\n\nThis timer stops on close!",
            TextAlignment = TextAlignment.Center,
            VerticalAlignment = VerticalAlignment.Center,
            TextWrapping = TextWrapping.Wrap
        };
        Content = _statusText;

        _timer = new DispatcherTimer { Interval = TimeSpan.FromSeconds(1) };
        _timer.Tick += OnTick;
        _timer.Start();

        // Stop timer when closing - PROPER!
        Closed += (s, e) =>
        {
            _timer.Stop();
            _timer.Tick -= OnTick;
            Interlocked.Decrement(ref InstanceCount);
        };

        Interlocked.Increment(ref InstanceCount);
    }

    private void OnTick(object? sender, EventArgs e)
    {
        _tickCount++;
        _statusText.Text = $"Timer ticking...\nTicks: {_tickCount}\n\nThis timer stops on close!";
    }
}

#endregion

#region Scenario 4: Dependency Property Controls

public class LeakyDPControl : Control
{
    public static readonly DependencyProperty ValueProperty =
        DependencyProperty.Register(nameof(Value), typeof(int), typeof(LeakyDPControl),
            new PropertyMetadata(0, OnValueChanged));

    public int Value
    {
        get => (int)GetValue(ValueProperty);
        set => SetValue(ValueProperty, value);
    }

    private static void OnValueChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
    {
        // This callback captures 'd' which can cause leaks in certain scenarios
        var control = (LeakyDPControl)d;

        // Creating closure that captures 'control' - potential leak
        var timer = new DispatcherTimer { Interval = TimeSpan.FromMilliseconds(100) };
        timer.Tick += (s, args) =>
        {
            // This closure keeps 'control' alive
            control.Background = Brushes.Red;
        };
        timer.Start();
    }

    public LeakyDPControl()
    {
        Background = Brushes.LightBlue;
        Value = 1; // Trigger the callback
    }
}

public class FixedDPControl : Control
{
    public static readonly DependencyProperty ValueProperty =
        DependencyProperty.Register(nameof(Value), typeof(int), typeof(FixedDPControl),
            new PropertyMetadata(0, OnValueChanged));

    public int Value
    {
        get => (int)GetValue(ValueProperty);
        set => SetValue(ValueProperty, value);
    }

    private static void OnValueChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
    {
        // Use static method or avoid closures that capture the control
        var control = (FixedDPControl)d;
        control.Background = Brushes.LightGreen;
    }

    public FixedDPControl()
    {
        Background = Brushes.LightGreen;
        Value = 1;
    }
}

#endregion

#region Scenario 5: Collection Subscribers

public class LeakyCollectionSubscriber
{
    private readonly ObservableCollection<string> _collection;

    public LeakyCollectionSubscriber(ObservableCollection<string> collection)
    {
        _collection = collection;
        // Subscribe but never unsubscribe - MEMORY LEAK!
        _collection.CollectionChanged += OnCollectionChanged;
    }

    private void OnCollectionChanged(object? sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
    {
        // Handle collection changes
    }
}

public class FixedCollectionSubscriber : IDisposable
{
    private readonly ObservableCollection<string> _collection;
    private bool _disposed = false;

    public FixedCollectionSubscriber(ObservableCollection<string> collection)
    {
        _collection = collection;
        _collection.CollectionChanged += OnCollectionChanged;
    }

    private void OnCollectionChanged(object? sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
    {
        // Handle collection changes
    }

    public void Dispose()
    {
        if (!_disposed)
        {
            _collection.CollectionChanged -= OnCollectionChanged;
            _disposed = true;
        }
    }
}

#endregion

#region Scenario 7: Command Windows

public class LeakyCommandWindow : Window
{
    public LeakyCommandWindow()
    {
        Title = "Leaky Command Window";
        Width = 300;
        Height = 200;

        var command = new LeakyCommand(() => MessageBox.Show("Executed!"));
        var button = new Button
        {
            Content = "Execute Command",
            Command = command,
            VerticalAlignment = VerticalAlignment.Center,
            Margin = new Thickness(20)
        };

        Content = button;
        Interlocked.Increment(ref MainWindow._leakyCommandCount);
    }
}

public class FixedCommandWindow : Window
{
    private readonly FixedCommand _command;

    public FixedCommandWindow()
    {
        Title = "Fixed Command Window";
        Width = 300;
        Height = 200;

        _command = new FixedCommand(() => MessageBox.Show("Executed!"));
        var button = new Button
        {
            Content = "Execute Command",
            Command = _command,
            VerticalAlignment = VerticalAlignment.Center,
            Margin = new Thickness(20)
        };

        Content = button;

        Closed += (s, e) =>
        {
            _command.Dispose();
            Interlocked.Decrement(ref MainWindow._fixedCommandCount);
        };

        Interlocked.Increment(ref MainWindow._fixedCommandCount);
    }
}

public class LeakyCommand : ICommand
{
    private readonly Action _execute;

    public LeakyCommand(Action execute)
    {
        _execute = execute;
        // Subscribe to CommandManager - MEMORY LEAK!
        CommandManager.RequerySuggested += OnRequerySuggested;
    }

    private void OnRequerySuggested(object? sender, EventArgs e)
    {
        CanExecuteChanged?.Invoke(this, EventArgs.Empty);
    }

    public event EventHandler? CanExecuteChanged;

    public bool CanExecute(object? parameter) => true;

    public void Execute(object? parameter) => _execute();
}

public class FixedCommand : ICommand, IDisposable
{
    private readonly Action _execute;
    private bool _disposed = false;

    public FixedCommand(Action execute)
    {
        _execute = execute;
    }

    public event EventHandler? CanExecuteChanged
    {
        add => CommandManager.RequerySuggested += value;
        remove => CommandManager.RequerySuggested -= value;
    }

    public bool CanExecute(object? parameter) => true;

    public void Execute(object? parameter) => _execute();

    public void Dispose()
    {
        if (!_disposed)
        {
            _disposed = true;
        }
    }
}

#endregion

#region Scenario 10: ViewModel Windows

public class LeakyViewModelWindow : Window
{
    public LeakyViewModelWindow()
    {
        Title = "Leaky ViewModel Window";
        Width = 300;
        Height = 200;

        var viewModel = new LeakyViewModel();
        DataContext = viewModel;

        var textBlock = new TextBlock
        {
            Text = "ViewModel with memory leak\n\nDataContext not cleaned up!",
            TextAlignment = TextAlignment.Center,
            VerticalAlignment = VerticalAlignment.Center,
            TextWrapping = TextWrapping.Wrap
        };

        Content = textBlock;

        Interlocked.Increment(ref MainWindow._leakyViewModelCount);
        // ViewModel never disposed - MEMORY LEAK!
    }
}

public class FixedViewModelWindow : Window
{
    private readonly FixedViewModel _viewModel;

    public FixedViewModelWindow()
    {
        Title = "Fixed ViewModel Window";
        Width = 300;
        Height = 200;

        _viewModel = new FixedViewModel();
        DataContext = _viewModel;

        var textBlock = new TextBlock
        {
            Text = "ViewModel properly disposed\n\nDataContext cleaned up!",
            TextAlignment = TextAlignment.Center,
            VerticalAlignment = VerticalAlignment.Center,
            TextWrapping = TextWrapping.Wrap
        };

        Content = textBlock;

        // Dispose ViewModel when closing - PROPER!
        Closed += (s, e) =>
        {
            _viewModel.Dispose();
            DataContext = null;
            Interlocked.Decrement(ref MainWindow._fixedViewModelCount);
        };

        Interlocked.Increment(ref MainWindow._fixedViewModelCount);
    }
}

public class LeakyViewModel : INotifyPropertyChanged
{
    private readonly DispatcherTimer _timer;

    public LeakyViewModel()
    {
        _timer = new DispatcherTimer { Interval = TimeSpan.FromSeconds(1) };
        _timer.Tick += (s, e) => OnPropertyChanged(nameof(CurrentTime));
        _timer.Start();
        // Timer never stopped - MEMORY LEAK!
    }

    public string CurrentTime => DateTime.Now.ToString("HH:mm:ss");

    public event PropertyChangedEventHandler? PropertyChanged;

    protected virtual void OnPropertyChanged(string propertyName)
    {
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }
}

public class FixedViewModel : INotifyPropertyChanged, IDisposable
{
    private readonly DispatcherTimer _timer;
    private bool _disposed = false;

    public FixedViewModel()
    {
        _timer = new DispatcherTimer { Interval = TimeSpan.FromSeconds(1) };
        _timer.Tick += OnTimerTick;
        _timer.Start();
    }

    private void OnTimerTick(object? sender, EventArgs e)
    {
        OnPropertyChanged(nameof(CurrentTime));
    }

    public string CurrentTime => DateTime.Now.ToString("HH:mm:ss");

    public event PropertyChangedEventHandler? PropertyChanged;

    protected virtual void OnPropertyChanged(string propertyName)
    {
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }

    public void Dispose()
    {
        if (!_disposed)
        {
            _timer.Stop();
            _timer.Tick -= OnTimerTick;
            _disposed = true;
        }
    }
}

#endregion
