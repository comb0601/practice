using System.Diagnostics;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace RenderingPipeline;

public partial class MainWindow : Window
{
    private int _frameCount = 0;
    private int _eventCount = 0;
    private int _layoutPassCount = 0;
    private readonly Stopwatch _fpsStopwatch = new();
    private readonly Stopwatch _renderStopwatch = new();
    private DispatcherTimer? _animationTimer;
    private readonly List<string> _eventLog = new();

    public MainWindow()
    {
        InitializeComponent();
        Loaded += MainWindow_Loaded;
        UpdateSystemInfo();
    }

    private void MainWindow_Loaded(object sender, RoutedEventArgs e)
    {
        // Subscribe to rendering events
        CompositionTarget.Rendering += CompositionTarget_Rendering;
        _fpsStopwatch.Start();
    }

    private void CompositionTarget_Rendering(object? sender, EventArgs e)
    {
        _frameCount++;

        if (_fpsStopwatch.ElapsedMilliseconds >= 1000)
        {
            double fps = _frameCount / (_fpsStopwatch.ElapsedMilliseconds / 1000.0);
            TxtFPS.Text = $"{fps:F1}";
            TxtFrameCount.Text = _frameCount.ToString();
            _frameCount = 0;
            _fpsStopwatch.Restart();
        }
    }

    private void UpdateSystemInfo()
    {
        // Get render tier
        int tier = (RenderCapability.Tier >> 16);
        TxtRenderTier.Text = $"Tier {tier}";

        // Graphics mode
        TxtGraphicsMode.Text = tier switch
        {
            0 => "Software Rendering",
            1 => "Partial Hardware",
            2 => "Full Hardware",
            _ => "Unknown"
        };

        // DirectX version (WPF uses DirectX 9 or later)
        TxtDirectXVersion.Text = "DirectX 9+";

        // Max texture size
        TxtMaxTextureSize.Text = $"{RenderCapability.MaxHardwareTextureSize} x {RenderCapability.MaxHardwareTextureSize}";

        // DPI
        var source = PresentationSource.FromVisual(this);
        if (source?.CompositionTarget != null)
        {
            double dpiX = 96.0 * source.CompositionTarget.TransformToDevice.M11;
            double dpiY = 96.0 * source.CompositionTarget.TransformToDevice.M22;
            TxtDpiScale.Text = $"{dpiX:F0} x {dpiY:F0}";
        }

        // Refresh rate
        TxtRefreshRate.Text = "60 Hz (typical)";
    }

    private void BtnShowPipeline_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Showing WPF rendering pipeline overview...";

        var pipeline = new StackPanel { Margin = new Thickness(20) };

        // Pipeline stages
        var stages = new[]
        {
            ("1. Input & Events", "User input, mouse, keyboard, touch events", Colors.LightBlue),
            ("2. Layout - Measure", "Calculate desired size of all elements", Colors.LightGreen),
            ("3. Layout - Arrange", "Position elements in final locations", Colors.LightYellow),
            ("4. Render", "Generate visual instructions", Colors.LightCoral),
            ("5. Composition", "Combine visual layers", Colors.LightPink),
            ("6. DirectX", "Hardware-accelerated rendering", Colors.LightCyan),
            ("7. Display", "Present frame to screen", Colors.LightGray)
        };

        foreach (var (title, description, color) in stages)
        {
            var stagePanel = new StackPanel
            {
                Margin = new Thickness(0, 5, 0, 5)
            };

            var border = new Border
            {
                Background = new SolidColorBrush(color),
                BorderBrush = Brushes.DarkGray,
                BorderThickness = new Thickness(2),
                Padding = new Thickness(15),
                CornerRadius = new CornerRadius(5)
            };

            var content = new StackPanel();
            content.Children.Add(new TextBlock
            {
                Text = title,
                FontWeight = FontWeights.Bold,
                FontSize = 16
            });
            content.Children.Add(new TextBlock
            {
                Text = description,
                FontSize = 12,
                Foreground = Brushes.DarkSlateGray,
                Margin = new Thickness(0, 5, 0, 0)
            });

            border.Child = content;
            stagePanel.Children.Add(border);

            // Add arrow
            if (title != "7. Display")
            {
                var arrow = new TextBlock
                {
                    Text = "▼",
                    FontSize = 20,
                    HorizontalAlignment = HorizontalAlignment.Center,
                    Foreground = Brushes.Gray,
                    Margin = new Thickness(0, 5, 0, 5)
                };
                stagePanel.Children.Add(arrow);
            }

            pipeline.Children.Add(stagePanel);
        }

        VisualizationContainer.Child = pipeline;

        var details = new StringBuilder();
        details.AppendLine("WPF RENDERING PIPELINE");
        details.AppendLine("=====================\n");
        details.AppendLine("The WPF rendering pipeline consists of several stages:");
        details.AppendLine();
        details.AppendLine("1. INPUT & EVENTS");
        details.AppendLine("   - Process user interactions");
        details.AppendLine("   - Queue input events\n");
        details.AppendLine("2. LAYOUT - MEASURE PASS");
        details.AppendLine("   - Top-down traversal");
        details.AppendLine("   - Each element calculates desired size");
        details.AppendLine("   - Constraints propagated from parent\n");
        details.AppendLine("3. LAYOUT - ARRANGE PASS");
        details.AppendLine("   - Bottom-up traversal");
        details.AppendLine("   - Elements positioned in final locations");
        details.AppendLine("   - Actual sizes determined\n");
        details.AppendLine("4. RENDER");
        details.AppendLine("   - Generate drawing instructions");
        details.AppendLine("   - Create Visual layer");
        details.AppendLine("   - Build render tree\n");
        details.AppendLine("5. COMPOSITION");
        details.AppendLine("   - Combine multiple visual layers");
        details.AppendLine("   - Apply effects and transformations");
        details.AppendLine("   - Handle transparency\n");
        details.AppendLine("6. DIRECTX");
        details.AppendLine("   - Hardware-accelerated rendering");
        details.AppendLine("   - GPU processes instructions");
        details.AppendLine("   - Texture creation and management\n");
        details.AppendLine("7. DISPLAY");
        details.AppendLine("   - Present final frame");
        details.AppendLine("   - VSync synchronization");
        details.AppendLine("   - Screen update");

        TxtPipelineDetails.Text = details.ToString();
        TxtStatus.Text = "Pipeline overview displayed";
    }

    private void BtnRenderTiers_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Showing render tier information...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        // Current tier
        int currentTier = RenderCapability.Tier >> 16;

        panel.Children.Add(new TextBlock
        {
            Text = $"Current Render Tier: {currentTier}",
            FontSize = 20,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 20),
            Foreground = Brushes.DarkBlue
        });

        // Tier descriptions
        var tiers = new[]
        {
            ("Tier 0 - Software Rendering",
             "No graphics hardware acceleration\nAll rendering performed by CPU\nSlowest performance",
             Colors.IndianRed),

            ("Tier 1 - Partial Hardware Acceleration",
             "Some features hardware-accelerated\nLimited GPU capabilities\nModerate performance",
             Colors.Orange),

            ("Tier 2 - Full Hardware Acceleration",
             "All rendering features hardware-accelerated\nFull GPU support\nBest performance (DirectX 9+ compatible)",
             Colors.LightGreen)
        };

        for (int i = 0; i < tiers.Length; i++)
        {
            var (title, description, color) = tiers[i];

            var border = new Border
            {
                Background = new SolidColorBrush(color),
                BorderBrush = i == currentTier ? Brushes.DarkBlue : Brushes.Gray,
                BorderThickness = new Thickness(i == currentTier ? 3 : 1),
                Padding = new Thickness(15),
                Margin = new Thickness(0, 5, 0, 5),
                CornerRadius = new CornerRadius(5)
            };

            if (i == currentTier)
            {
                border.Effect = new System.Windows.Media.Effects.DropShadowEffect
                {
                    Color = Colors.DarkBlue,
                    BlurRadius = 10,
                    ShadowDepth = 3
                };
            }

            var content = new StackPanel();

            var titleBlock = new TextBlock
            {
                Text = title + (i == currentTier ? " ← YOUR SYSTEM" : ""),
                FontWeight = FontWeights.Bold,
                FontSize = 14
            };

            var descBlock = new TextBlock
            {
                Text = description,
                Margin = new Thickness(0, 5, 0, 0),
                TextWrapping = TextWrapping.Wrap
            };

            content.Children.Add(titleBlock);
            content.Children.Add(descBlock);
            border.Child = content;
            panel.Children.Add(border);
        }

        // Additional info
        var infoBox = new GroupBox
        {
            Header = "Hardware Capabilities",
            Margin = new Thickness(0, 20, 0, 0)
        };

        var infoPanel = new StackPanel();
        infoPanel.Children.Add(new TextBlock
        {
            Text = $"Max Texture Size: {RenderCapability.MaxHardwareTextureSize}x{RenderCapability.MaxHardwareTextureSize}",
            Margin = new Thickness(5)
        });

        infoPanel.Children.Add(new TextBlock
        {
            Text = $"Pixel Shader Version: {RenderCapability.MaxPixelShaderInstructionSlots(0, 0)}",
            Margin = new Thickness(5)
        });

        infoPanel.Children.Add(new TextBlock
        {
            Text = $"Is Hardware Accelerated: {(currentTier >= 1 ? "Yes" : "No")}",
            Margin = new Thickness(5),
            FontWeight = FontWeights.Bold,
            Foreground = currentTier >= 1 ? Brushes.Green : Brushes.Red
        });

        infoBox.Content = infoPanel;
        panel.Children.Add(infoBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        var details = new StringBuilder();
        details.AppendLine("RENDER TIER INFORMATION");
        details.AppendLine("======================\n");
        details.AppendLine($"Current Tier: {currentTier}\n");
        details.AppendLine("Tier 0: Software Rendering");
        details.AppendLine("  - No GPU acceleration");
        details.AppendLine("  - CPU performs all rendering");
        details.AppendLine("  - Slowest, highest CPU usage\n");
        details.AppendLine("Tier 1: Partial Hardware");
        details.AppendLine("  - Some GPU features available");
        details.AppendLine("  - Limited shader support");
        details.AppendLine("  - Better than software\n");
        details.AppendLine("Tier 2: Full Hardware");
        details.AppendLine("  - Complete GPU acceleration");
        details.AppendLine("  - DirectX 9+ compatible");
        details.AppendLine("  - Best performance");
        details.AppendLine("  - Modern graphics cards\n");
        details.AppendLine("IMPACT ON PERFORMANCE:");
        details.AppendLine("  - Tier 0: Avoid complex visuals");
        details.AppendLine("  - Tier 1: Moderate complexity OK");
        details.AppendLine("  - Tier 2: Full visual richness");

        TxtPipelineDetails.Text = details.ToString();
        TxtStatus.Text = $"Render Tier: {currentTier} - {(currentTier == 2 ? "Full Hardware Acceleration" : currentTier == 1 ? "Partial Hardware" : "Software Rendering")}";
    }

    private void BtnMeasureArrange_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating Measure and Arrange passes...";

        var mainPanel = new StackPanel { Margin = new Thickness(20) };

        // Create a custom panel to visualize measure/arrange
        var demoPanel = new MeasureArrangePanel();
        demoPanel.MeasureCompleted += (s, msg) => LogEvent($"Measure: {msg}");
        demoPanel.ArrangeCompleted += (s, msg) => LogEvent($"Arrange: {msg}");

        for (int i = 0; i < 5; i++)
        {
            demoPanel.Children.Add(new Border
            {
                Width = 80,
                Height = 60,
                Background = new SolidColorBrush(Color.FromRgb(
                    (byte)(100 + i * 30),
                    (byte)(150 + i * 20),
                    (byte)(200 + i * 10)
                )),
                BorderBrush = Brushes.Black,
                BorderThickness = new Thickness(2),
                Child = new TextBlock
                {
                    Text = $"Item {i + 1}",
                    HorizontalAlignment = HorizontalAlignment.Center,
                    VerticalAlignment = VerticalAlignment.Center,
                    FontWeight = FontWeights.Bold
                }
            });
        }

        var border = new Border
        {
            BorderBrush = Brushes.DarkBlue,
            BorderThickness = new Thickness(2),
            Padding = new Thickness(10),
            Child = demoPanel
        };

        mainPanel.Children.Add(new TextBlock
        {
            Text = "Layout System Demonstration",
            FontSize = 18,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 10)
        });

        mainPanel.Children.Add(border);

        var infoBox = new GroupBox
        {
            Header = "Measure/Arrange Process",
            Margin = new Thickness(0, 20, 0, 0)
        };

        infoBox.Content = new TextBlock
        {
            Text = "The layout system uses two passes:\n\n" +
                   "1. MEASURE PASS (Top-Down):\n" +
                   "   - Parent asks children for desired size\n" +
                   "   - Children measure themselves\n" +
                   "   - Size constraints propagate down\n\n" +
                   "2. ARRANGE PASS (Bottom-Up):\n" +
                   "   - Parent arranges children in final positions\n" +
                   "   - Children positioned based on measure\n" +
                   "   - Actual bounds finalized\n\n" +
                   "Check the Event Log to see the process in action!",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        mainPanel.Children.Add(infoBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = mainPanel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        var details = new StringBuilder();
        details.AppendLine("LAYOUT SYSTEM");
        details.AppendLine("=============\n");
        details.AppendLine("TWO-PASS LAYOUT:");
        details.AppendLine();
        details.AppendLine("Pass 1: MEASURE");
        details.AppendLine("  - Top-down traversal");
        details.AppendLine("  - Calculate desired sizes");
        details.AppendLine("  - Respect constraints");
        details.AppendLine("  - Override MeasureOverride()\n");
        details.AppendLine("Pass 2: ARRANGE");
        details.AppendLine("  - Bottom-up traversal");
        details.AppendLine("  - Finalize positions");
        details.AppendLine("  - Assign actual bounds");
        details.AppendLine("  - Override ArrangeOverride()\n");
        details.AppendLine("PERFORMANCE TIPS:");
        details.AppendLine("  - Minimize layout invalidation");
        details.AppendLine("  - Cache layout calculations");
        details.AppendLine("  - Avoid nested measure/arrange");
        details.AppendLine("  - Use appropriate panels");

        TxtPipelineDetails.Text = details.ToString();
        TxtLayoutPasses.Text = "Monitoring...";
        _layoutPassCount = 0;

        TxtStatus.Text = "Measure/Arrange demonstration active - watch the event log";
    }

    private void BtnRenderingEvents_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Monitoring rendering events...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "Rendering Events Monitor",
            FontSize = 18,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 10)
        });

        // Create animated content
        var animatedBorder = new Border
        {
            Width = 100,
            Height = 100,
            Background = Brushes.CornflowerBlue,
            BorderBrush = Brushes.Navy,
            BorderThickness = new Thickness(3),
            HorizontalAlignment = HorizontalAlignment.Center,
            Margin = new Thickness(0, 20, 0, 20)
        };

        var rotateTransform = new RotateTransform();
        animatedBorder.RenderTransform = rotateTransform;
        animatedBorder.RenderTransformOrigin = new Point(0.5, 0.5);

        panel.Children.Add(animatedBorder);

        // Animation controls
        var controlPanel = new WrapPanel { HorizontalAlignment = HorizontalAlignment.Center };

        var startButton = new Button { Content = "Start Animation" };
        startButton.Click += (s, e) =>
        {
            _animationTimer?.Stop();
            _animationTimer = new DispatcherTimer { Interval = TimeSpan.FromMilliseconds(16) };
            double angle = 0;
            _animationTimer.Tick += (s, e) =>
            {
                angle += 2;
                rotateTransform.Angle = angle;
                LogEvent($"Frame rendered: Angle = {angle:F1}°");
            };
            _animationTimer.Start();
            LogEvent("Animation started");
        };

        var stopButton = new Button { Content = "Stop Animation" };
        stopButton.Click += (s, e) =>
        {
            _animationTimer?.Stop();
            LogEvent("Animation stopped");
        };

        controlPanel.Children.Add(startButton);
        controlPanel.Children.Add(stopButton);
        panel.Children.Add(controlPanel);

        // Events info
        var eventsBox = new GroupBox
        {
            Header = "Key Rendering Events",
            Margin = new Thickness(0, 20, 0, 0)
        };

        eventsBox.Content = new TextBlock
        {
            Text = "• CompositionTarget.Rendering\n" +
                   "  - Fires before each frame render\n" +
                   "  - Use for animations\n" +
                   "  - Synchronized with display refresh\n\n" +
                   "• LayoutUpdated\n" +
                   "  - Fires after layout pass\n" +
                   "  - Indicates visual changes\n\n" +
                   "• SourceUpdated\n" +
                   "  - Data binding changes\n" +
                   "  - Triggers re-render\n\n" +
                   "Start animation to see events in the log!",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(eventsBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        var details = new StringBuilder();
        details.AppendLine("RENDERING EVENTS");
        details.AppendLine("================\n");
        details.AppendLine("CompositionTarget.Rendering:");
        details.AppendLine("  - Pre-frame callback");
        details.AppendLine("  - ~60 FPS (monitor dependent)");
        details.AppendLine("  - Animation hook point");
        details.AppendLine("  - High frequency event\n");
        details.AppendLine("LayoutUpdated:");
        details.AppendLine("  - Post-layout callback");
        details.AppendLine("  - Fires after measure/arrange");
        details.AppendLine("  - UI has changed\n");
        details.AppendLine("BEST PRACTICES:");
        details.AppendLine("  - Minimize work in events");
        details.AppendLine("  - Unsubscribe when done");
        details.AppendLine("  - Use CompositionTarget for animation");
        details.AppendLine("  - Avoid heavy calculations");

        TxtPipelineDetails.Text = details.ToString();
        TxtStatus.Text = "Rendering events monitor active - start animation to see events";
    }

    private void BtnDirtyRegions_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating dirty region optimization...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "Dirty Region Optimization",
            FontSize = 18,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 10)
        });

        var canvas = new Canvas
        {
            Width = 600,
            Height = 400,
            Background = Brushes.WhiteSmoke
        };

        // Create regions
        var regions = new List<Border>();
        for (int i = 0; i < 6; i++)
        {
            var region = new Border
            {
                Width = 150,
                Height = 100,
                Background = Brushes.LightBlue,
                BorderBrush = Brushes.DarkBlue,
                BorderThickness = new Thickness(2),
                Child = new TextBlock
                {
                    Text = $"Region {i + 1}\n(Click me)",
                    HorizontalAlignment = HorizontalAlignment.Center,
                    VerticalAlignment = VerticalAlignment.Center,
                    TextAlignment = TextAlignment.Center
                }
            };

            Canvas.SetLeft(region, (i % 3) * 200);
            Canvas.SetTop(region, (i / 3) * 150);

            int regionIndex = i;
            region.MouseDown += (s, e) =>
            {
                var border = (Border)s;
                border.Background = new SolidColorBrush(Color.FromRgb(
                    (byte)Random.Shared.Next(100, 255),
                    (byte)Random.Shared.Next(100, 255),
                    (byte)Random.Shared.Next(100, 255)
                ));
                LogEvent($"Region {regionIndex + 1} marked dirty - only this region re-rendered");
            };

            regions.Add(region);
            canvas.Children.Add(region);
        }

        panel.Children.Add(canvas);

        var infoBox = new GroupBox
        {
            Header = "Dirty Region Concept",
            Margin = new Thickness(0, 20, 0, 0)
        };

        infoBox.Content = new TextBlock
        {
            Text = "WPF optimizes rendering by tracking 'dirty regions':\n\n" +
                   "• Only changed areas are re-rendered\n" +
                   "• Unchanged content uses cached render\n" +
                   "• Reduces GPU workload significantly\n" +
                   "• Automatic optimization by WPF\n\n" +
                   "Click regions above to see partial updates.\n" +
                   "Notice only the clicked region is re-rendered!",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(infoBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        var details = new StringBuilder();
        details.AppendLine("DIRTY REGION OPTIMIZATION");
        details.AppendLine("========================\n");
        details.AppendLine("WPF tracks changed regions:");
        details.AppendLine();
        details.AppendLine("1. Element property changes");
        details.AppendLine("2. Mark element as 'dirty'");
        details.AppendLine("3. Propagate to parent");
        details.AppendLine("4. Calculate dirty bounds");
        details.AppendLine("5. Re-render only dirty area\n");
        details.AppendLine("BENEFITS:");
        details.AppendLine("  - Reduced render workload");
        details.AppendLine("  - Better performance");
        details.AppendLine("  - Automatic by WPF");
        details.AppendLine("  - GPU-friendly\n");
        details.AppendLine("OPTIMIZATION:");
        details.AppendLine("  - Minimize dirty area");
        details.AppendLine("  - Batch property changes");
        details.AppendLine("  - Use transforms (don't dirty)");
        details.AppendLine("  - Consider CacheMode");

        TxtPipelineDetails.Text = details.ToString();
        TxtStatus.Text = "Click regions to see dirty region optimization in action";
    }

    private void BtnCompositionTarget_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating CompositionTarget usage...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "CompositionTarget Frame Timing",
            FontSize = 18,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 10)
        });

        // FPS display
        var fpsDisplay = new TextBlock
        {
            FontSize = 48,
            FontWeight = FontWeights.Bold,
            HorizontalAlignment = HorizontalAlignment.Center,
            Margin = new Thickness(0, 20, 0, 20),
            Foreground = Brushes.DarkGreen
        };
        panel.Children.Add(fpsDisplay);

        // Animation area
        var canvas = new Canvas
        {
            Width = 600,
            Height = 300,
            Background = Brushes.Black
        };

        var balls = new List<Ellipse>();
        for (int i = 0; i < 20; i++)
        {
            var ball = new Ellipse
            {
                Width = 20,
                Height = 20,
                Fill = new SolidColorBrush(Color.FromRgb(
                    (byte)Random.Shared.Next(100, 255),
                    (byte)Random.Shared.Next(100, 255),
                    (byte)Random.Shared.Next(100, 255)
                ))
            };
            Canvas.SetLeft(ball, Random.Shared.Next(0, 580));
            Canvas.SetTop(ball, Random.Shared.Next(0, 280));
            canvas.Children.Add(ball);
            balls.Add(ball);
        }

        panel.Children.Add(canvas);

        // Start animation
        int frameCounter = 0;
        var frameStopwatch = Stopwatch.StartNew();

        EventHandler? renderHandler = null;
        renderHandler = (s, e) =>
        {
            frameCounter++;

            // Update FPS
            if (frameStopwatch.ElapsedMilliseconds >= 500)
            {
                double fps = frameCounter / (frameStopwatch.ElapsedMilliseconds / 1000.0);
                fpsDisplay.Text = $"{fps:F1} FPS";
                frameCounter = 0;
                frameStopwatch.Restart();
            }

            // Animate balls
            foreach (var ball in balls)
            {
                double left = Canvas.GetLeft(ball) + Random.Shared.Next(-5, 6);
                double top = Canvas.GetTop(ball) + Random.Shared.Next(-5, 6);

                if (left < 0) left = 0;
                if (left > 580) left = 580;
                if (top < 0) top = 0;
                if (top > 280) top = 280;

                Canvas.SetLeft(ball, left);
                Canvas.SetTop(ball, top);
            }
        };

        CompositionTarget.Rendering += renderHandler;

        // Clean up when cleared
        VisualizationContainer.LayoutUpdated += (s, e) =>
        {
            if (VisualizationContainer.Child == null && renderHandler != null)
            {
                CompositionTarget.Rendering -= renderHandler;
                renderHandler = null;
            }
        };

        var infoBox = new GroupBox
        {
            Header = "CompositionTarget.Rendering",
            Margin = new Thickness(0, 20, 0, 0)
        };

        infoBox.Content = new TextBlock
        {
            Text = "CompositionTarget.Rendering is WPF's frame callback:\n\n" +
                   "• Fires before each frame render\n" +
                   "• Synchronized with monitor refresh (usually 60Hz)\n" +
                   "• Perfect for animations\n" +
                   "• Provides consistent timing\n\n" +
                   "BEST PRACTICES:\n" +
                   "• Keep handler code lightweight\n" +
                   "• Unsubscribe when done\n" +
                   "• Use for smooth animations\n" +
                   "• Avoid heavy calculations",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(infoBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        var details = new StringBuilder();
        details.AppendLine("COMPOSITIONTARGET.RENDERING");
        details.AppendLine("===========================\n");
        details.AppendLine("Frame-synchronized callback:");
        details.AppendLine();
        details.AppendLine("TIMING:");
        details.AppendLine("  - Fires before each frame");
        details.AppendLine("  - ~60 times per second");
        details.AppendLine("  - VSync synchronized");
        details.AppendLine("  - Consistent intervals\n");
        details.AppendLine("USE CASES:");
        details.AppendLine("  - Smooth animations");
        details.AppendLine("  - Real-time updates");
        details.AppendLine("  - Game loops");
        details.AppendLine("  - Visual effects\n");
        details.AppendLine("CAUTIONS:");
        details.AppendLine("  - High frequency event");
        details.AppendLine("  - Keep handlers fast");
        details.AppendLine("  - Always unsubscribe");
        details.AppendLine("  - Can impact performance");

        TxtPipelineDetails.Text = details.ToString();
        TxtStatus.Text = "CompositionTarget animation running - showing frame-synchronized rendering";
    }

    private void BtnDirectXInfo_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Showing DirectX integration information...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "WPF and DirectX Integration",
            FontSize = 18,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 10)
        });

        // Architecture diagram
        var archBox = new GroupBox
        {
            Header = "WPF Graphics Architecture",
            Margin = new Thickness(0, 10, 0, 10)
        };

        var archStack = new StackPanel();
        var layers = new[]
        {
            ("Your WPF Application", Colors.LightBlue),
            ("WPF Visual Layer", Colors.LightGreen),
            ("Media Integration Layer (MIL)", Colors.LightYellow),
            ("DirectX 9+", Colors.LightCoral),
            ("Graphics Driver", Colors.LightPink),
            ("GPU Hardware", Colors.LightCyan)
        };

        foreach (var (layer, color) in layers)
        {
            archStack.Children.Add(new Border
            {
                Background = new SolidColorBrush(color),
                BorderBrush = Brushes.Black,
                BorderThickness = new Thickness(1),
                Padding = new Thickness(10),
                Margin = new Thickness(0, 2, 0, 2),
                Child = new TextBlock
                {
                    Text = layer,
                    HorizontalAlignment = HorizontalAlignment.Center,
                    FontWeight = FontWeights.Bold
                }
            });
        }

        archBox.Content = archStack;
        panel.Children.Add(archBox);

        // DirectX features
        var featuresBox = new GroupBox
        {
            Header = "DirectX Integration Benefits",
            Margin = new Thickness(0, 10, 0, 10)
        };

        featuresBox.Content = new TextBlock
        {
            Text = "• Hardware-accelerated rendering\n" +
                   "• GPU-based composition\n" +
                   "• Shader effects support\n" +
                   "• 3D transformations\n" +
                   "• Video playback acceleration\n" +
                   "• Texture management\n" +
                   "• Anti-aliasing support\n" +
                   "• High-performance graphics",
            Padding = new Thickness(10),
            TextWrapping = TextWrapping.Wrap
        };

        panel.Children.Add(featuresBox);

        // Performance impact
        var perfBox = new GroupBox
        {
            Header = "Performance Characteristics",
            Margin = new Thickness(0, 10, 0, 10)
        };

        var perfGrid = new Grid();
        perfGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(150) });
        perfGrid.ColumnDefinitions.Add(new ColumnDefinition());
        perfGrid.RowDefinitions.Add(new RowDefinition());
        perfGrid.RowDefinitions.Add(new RowDefinition());
        perfGrid.RowDefinitions.Add(new RowDefinition());

        AddGridText(perfGrid, 0, 0, "Software (Tier 0):", true);
        AddGridText(perfGrid, 0, 1, "All rendering on CPU - Very slow");

        AddGridText(perfGrid, 1, 0, "Partial HW (Tier 1):", true);
        AddGridText(perfGrid, 1, 1, "Some GPU features - Moderate");

        AddGridText(perfGrid, 2, 0, "Full HW (Tier 2):", true);
        AddGridText(perfGrid, 2, 1, "Full GPU power - Fast");

        perfBox.Content = perfGrid;
        panel.Children.Add(perfBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        var details = new StringBuilder();
        details.AppendLine("WPF & DIRECTX INTEGRATION");
        details.AppendLine("=========================\n");
        details.AppendLine("ARCHITECTURE:");
        details.AppendLine("  - WPF uses DirectX for rendering");
        details.AppendLine("  - Media Integration Layer (MIL)");
        details.AppendLine("  - Bridges managed/unmanaged code");
        details.AppendLine("  - DirectX 9 minimum requirement\n");
        details.AppendLine("BENEFITS:");
        details.AppendLine("  - Hardware acceleration");
        details.AppendLine("  - GPU-based rendering");
        details.AppendLine("  - Shader effects");
        details.AppendLine("  - 3D support");
        details.AppendLine("  - Video acceleration\n");
        details.AppendLine("RENDER TIERS:");
        int tier = RenderCapability.Tier >> 16;
        details.AppendLine($"  Current: Tier {tier}");
        details.AppendLine("  - Tier 0: Software only");
        details.AppendLine("  - Tier 1: Partial GPU");
        details.AppendLine("  - Tier 2: Full GPU (best)\n");
        details.AppendLine("OPTIMIZATION:");
        details.AppendLine("  - Ensure Tier 2 when possible");
        details.AppendLine("  - Update graphics drivers");
        details.AppendLine("  - Use GPU-friendly operations");
        details.AppendLine("  - Leverage hardware features");

        TxtPipelineDetails.Text = details.ToString();
        TxtStatus.Text = $"DirectX integration info displayed - Your system: Tier {tier}";
    }

    private void AddGridText(Grid grid, int row, int col, string text, bool bold = false)
    {
        var tb = new TextBlock
        {
            Text = text,
            Margin = new Thickness(5),
            TextWrapping = TextWrapping.Wrap
        };
        if (bold) tb.FontWeight = FontWeights.Bold;
        Grid.SetRow(tb, row);
        Grid.SetColumn(tb, col);
        grid.Children.Add(tb);
    }

    private void BtnClearDemo_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demo cleared - Ready for next demonstration";
    }

    private void ClearDemo()
    {
        _animationTimer?.Stop();
        VisualizationContainer.Child = null;
        TxtPipelineDetails.Text = "";
        _eventLog.Clear();
        TxtEventLog.Text = "";
        _eventCount = 0;
        TxtEventsFired.Text = "0";
    }

    private void LogEvent(string message)
    {
        _eventCount++;
        TxtEventsFired.Text = _eventCount.ToString();

        var timestamp = DateTime.Now.ToString("HH:mm:ss.fff");
        var logEntry = $"[{timestamp}] {message}";
        _eventLog.Add(logEntry);

        // Keep last 50 events
        if (_eventLog.Count > 50)
        {
            _eventLog.RemoveAt(0);
        }

        TxtEventLog.Text = string.Join("\n", _eventLog);
    }

    protected override void OnClosed(EventArgs e)
    {
        CompositionTarget.Rendering -= CompositionTarget_Rendering;
        _animationTimer?.Stop();
        base.OnClosed(e);
    }
}

// Custom panel to demonstrate measure/arrange
public class MeasureArrangePanel : Panel
{
    public event EventHandler<string>? MeasureCompleted;
    public event EventHandler<string>? ArrangeCompleted;

    protected override Size MeasureOverride(Size availableSize)
    {
        var desiredSize = new Size();

        foreach (UIElement child in InternalChildren)
        {
            child.Measure(availableSize);
            desiredSize.Width += child.DesiredSize.Width + 10;
            desiredSize.Height = Math.Max(desiredSize.Height, child.DesiredSize.Height);
        }

        MeasureCompleted?.Invoke(this, $"Desired size: {desiredSize.Width:F0} x {desiredSize.Height:F0}");
        return desiredSize;
    }

    protected override Size ArrangeOverride(Size finalSize)
    {
        double x = 0;

        foreach (UIElement child in InternalChildren)
        {
            child.Arrange(new Rect(x, 0, child.DesiredSize.Width, child.DesiredSize.Height));
            x += child.DesiredSize.Width + 10;
        }

        ArrangeCompleted?.Invoke(this, $"Final size: {finalSize.Width:F0} x {finalSize.Height:F0}");
        return finalSize;
    }
}
