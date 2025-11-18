using System.Diagnostics;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Effects;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace BitmapCaching;

public partial class MainWindow : Window
{
    private int _frameCount = 0;
    private readonly Stopwatch _fpsStopwatch = new();
    private DispatcherTimer? _animationTimer;

    public MainWindow()
    {
        InitializeComponent();
        Loaded += MainWindow_Loaded;
    }

    private void MainWindow_Loaded(object sender, RoutedEventArgs e)
    {
        StartFPSMonitoring();
    }

    private void StartFPSMonitoring()
    {
        _fpsStopwatch.Start();
        CompositionTarget.Rendering += (s, e) =>
        {
            _frameCount++;
            if (_fpsStopwatch.ElapsedMilliseconds >= 1000)
            {
                double fps = _frameCount / (_fpsStopwatch.ElapsedMilliseconds / 1000.0);
                TxtFPS.Text = $"{fps:F1}";
                TxtFrameTime.Text = $"{1000.0 / fps:F2} ms";
                _frameCount = 0;
                _fpsStopwatch.Restart();
            }
        };
    }

    private void BtnCacheModeBasics_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating CacheMode basics...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "CacheMode and BitmapCache Basics",
            FontSize = 20,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 20),
            Foreground = Brushes.DarkBlue
        });

        // Example 1: No Cache
        var noCacheBox = CreateCacheExample(
            "No Cache (Default)",
            "Content re-rendered every frame",
            null,
            Brushes.LightCoral
        );
        panel.Children.Add(noCacheBox);

        // Example 2: BitmapCache
        var bitmapCacheBox = CreateCacheExample(
            "BitmapCache (Default Settings)",
            "Content rendered to bitmap once, reused",
            new BitmapCache(),
            Brushes.LightGreen
        );
        panel.Children.Add(bitmapCacheBox);

        // Example 3: BitmapCache with RenderAtScale
        var scaledCacheBox = CreateCacheExample(
            "BitmapCache with RenderAtScale = 2.0",
            "Higher quality cache for scaling",
            new BitmapCache { RenderAtScale = 2.0 },
            Brushes.LightBlue
        );
        panel.Children.Add(scaledCacheBox);

        // Example 4: BitmapCache with SnapsToDevicePixels
        var snappedCacheBox = CreateCacheExample(
            "BitmapCache with SnapsToDevicePixels",
            "Aligned to device pixels for crisp rendering",
            new BitmapCache { SnapsToDevicePixels = true },
            Brushes.LightYellow
        );
        panel.Children.Add(snappedCacheBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        var details = new StringBuilder();
        details.AppendLine("CACHEMODE & BITMAPCACHE");
        details.AppendLine("======================\n");
        details.AppendLine("CacheMode Property:");
        details.AppendLine("  - Controls render caching");
        details.AppendLine("  - Default: null (no cache)");
        details.AppendLine("  - BitmapCache: renders to bitmap\n");
        details.AppendLine("BitmapCache Properties:");
        details.AppendLine("  - RenderAtScale: cache quality");
        details.AppendLine("    Default: 1.0");
        details.AppendLine("    Higher: better quality, more memory");
        details.AppendLine("  - SnapsToDevicePixels: pixel alignment");
        details.AppendLine("    Default: false");
        details.AppendLine("    True: crisper rendering\n");
        details.AppendLine("BENEFITS:");
        details.AppendLine("  - Faster rendering (reuse bitmap)");
        details.AppendLine("  - Better for complex content");
        details.AppendLine("  - Ideal for transforms/opacity\n");
        details.AppendLine("COSTS:");
        details.AppendLine("  - Memory usage");
        details.AppendLine("  - Initial render overhead");
        details.AppendLine("  - Cache invalidation on changes");

        TxtCacheDetails.Text = details.ToString();
        TxtCacheEnabled.Text = "Multiple Examples";
        TxtStatus.Text = "CacheMode basics displayed - compare different cache configurations";
    }

    private Border CreateCacheExample(string title, string description, BitmapCache? cache, Brush background)
    {
        var container = new GroupBox
        {
            Header = title,
            Margin = new Thickness(0, 10, 0, 10)
        };

        var contentBorder = new Border
        {
            Background = background,
            BorderBrush = Brushes.DarkGray,
            BorderThickness = new Thickness(2),
            Padding = new Thickness(20),
            CacheMode = cache
        };

        var stack = new StackPanel();

        // Complex content to demonstrate caching benefit
        for (int i = 0; i < 3; i++)
        {
            var grid = new Grid();
            grid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(50) });
            grid.ColumnDefinitions.Add(new ColumnDefinition());

            var ellipse = new Ellipse
            {
                Width = 40,
                Height = 40,
                Fill = new RadialGradientBrush(Colors.White, Colors.Blue),
                Stroke = Brushes.DarkBlue,
                StrokeThickness = 2
            };
            Grid.SetColumn(ellipse, 0);
            grid.Children.Add(ellipse);

            var text = new TextBlock
            {
                Text = $"Complex rendered content line {i + 1}",
                VerticalAlignment = VerticalAlignment.Center,
                FontSize = 14,
                Margin = new Thickness(10, 0, 0, 0)
            };
            Grid.SetColumn(text, 1);
            grid.Children.Add(text);

            stack.Children.Add(grid);
        }

        var descBlock = new TextBlock
        {
            Text = description,
            FontStyle = FontStyles.Italic,
            Foreground = Brushes.DarkGray,
            Margin = new Thickness(0, 10, 0, 0)
        };
        stack.Children.Add(descBlock);

        contentBorder.Child = stack;
        container.Content = contentBorder;

        return container;
    }

    private void BtnCacheComparison_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Comparing cached vs uncached performance...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "Performance Comparison: Cached vs Uncached",
            FontSize = 18,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 10)
        });

        var compareGrid = new Grid();
        compareGrid.ColumnDefinitions.Add(new ColumnDefinition());
        compareGrid.ColumnDefinitions.Add(new ColumnDefinition());

        // Uncached version
        var uncachedBox = new GroupBox
        {
            Header = "WITHOUT BitmapCache (Slower)",
            Margin = new Thickness(5)
        };

        var uncachedContent = CreateComplexContent(false);
        uncachedBox.Content = uncachedContent;
        Grid.SetColumn(uncachedBox, 0);
        compareGrid.Children.Add(uncachedBox);

        // Cached version
        var cachedBox = new GroupBox
        {
            Header = "WITH BitmapCache (Faster)",
            Margin = new Thickness(5)
        };

        var cachedContent = CreateComplexContent(true);
        cachedBox.Content = cachedContent;
        Grid.SetColumn(cachedBox, 1);
        compareGrid.Children.Add(cachedBox);

        panel.Children.Add(compareGrid);

        // Animation controls
        var controlBox = new GroupBox
        {
            Header = "Animation Controls",
            Margin = new Thickness(0, 20, 0, 0)
        };

        var controlStack = new StackPanel();

        var startButton = new Button { Content = "Start Animation" };
        var stopButton = new Button { Content = "Stop Animation" };

        startButton.Click += (s, e) =>
        {
            StartContentAnimation(uncachedContent, cachedContent);
        };

        stopButton.Click += (s, e) =>
        {
            _animationTimer?.Stop();
        };

        controlStack.Children.Add(new WrapPanel
        {
            Children = { startButton, stopButton }
        });

        controlBox.Content = controlStack;
        panel.Children.Add(controlBox);

        // Info
        var infoBox = new GroupBox
        {
            Header = "Performance Analysis"
        };

        infoBox.Content = new TextBlock
        {
            Text = "When animating (rotation, opacity, etc.):\n\n" +
                   "WITHOUT Cache:\n" +
                   "• Content re-rendered every frame\n" +
                   "• Higher CPU/GPU usage\n" +
                   "• Slower for complex content\n\n" +
                   "WITH Cache:\n" +
                   "• Content rendered to bitmap once\n" +
                   "• Bitmap transformed each frame\n" +
                   "• Much faster for complex content\n" +
                   "• Lower CPU usage\n\n" +
                   "Start animation and watch FPS counter!",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(infoBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        TxtCacheDetails.Text = "Click 'Start Animation' to compare performance.\n" +
                               "The cached version should maintain higher FPS.";
        TxtCacheEnabled.Text = "Right Side Only";
        TxtStatus.Text = "Cache comparison ready - start animation to see difference";
    }

    private Border CreateComplexContent(bool useCache)
    {
        var border = new Border
        {
            Width = 300,
            Height = 300,
            Background = Brushes.White,
            BorderBrush = Brushes.Black,
            BorderThickness = new Thickness(2),
            CacheMode = useCache ? new BitmapCache() : null
        };

        var canvas = new Canvas();

        // Create complex vector graphics
        for (int i = 0; i < 30; i++)
        {
            var path = new System.Windows.Shapes.Path
            {
                Stroke = new SolidColorBrush(Color.FromRgb(
                    (byte)Random.Shared.Next(50, 255),
                    (byte)Random.Shared.Next(50, 255),
                    (byte)Random.Shared.Next(50, 255)
                )),
                StrokeThickness = 2,
                Data = new PathGeometry
                {
                    Figures = new PathFigureCollection
                    {
                        new PathFigure
                        {
                            StartPoint = new Point(Random.Shared.Next(0, 300), Random.Shared.Next(0, 300)),
                            Segments = new PathSegmentCollection
                            {
                                new BezierSegment(
                                    new Point(Random.Shared.Next(0, 300), Random.Shared.Next(0, 300)),
                                    new Point(Random.Shared.Next(0, 300), Random.Shared.Next(0, 300)),
                                    new Point(Random.Shared.Next(0, 300), Random.Shared.Next(0, 300)),
                                    true
                                )
                            }
                        }
                    }
                }
            };
            canvas.Children.Add(path);
        }

        // Add text
        canvas.Children.Add(new TextBlock
        {
            Text = useCache ? "CACHED" : "UNCACHED",
            FontSize = 24,
            FontWeight = FontWeights.Bold,
            Foreground = useCache ? Brushes.Green : Brushes.Red,
            Canvas.Left = 100,
            Canvas.Top = 130
        });

        border.Child = canvas;
        return border;
    }

    private void StartContentAnimation(Border uncached, Border cached)
    {
        _animationTimer?.Stop();
        _animationTimer = new DispatcherTimer { Interval = TimeSpan.FromMilliseconds(16) };

        var uncachedTransform = new RotateTransform();
        uncached.RenderTransform = uncachedTransform;
        uncached.RenderTransformOrigin = new Point(0.5, 0.5);

        var cachedTransform = new RotateTransform();
        cached.RenderTransform = cachedTransform;
        cached.RenderTransformOrigin = new Point(0.5, 0.5);

        double angle = 0;
        _animationTimer.Tick += (s, e) =>
        {
            angle += 2;
            uncachedTransform.Angle = angle;
            cachedTransform.Angle = angle;
        };

        _animationTimer.Start();
        TxtPerformance.Text = "Animating...";
    }

    private void BtnRenderScale_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating RenderAtScale property...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "BitmapCache RenderAtScale Property",
            FontSize = 18,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 10)
        });

        var wrapPanel = new WrapPanel();

        // Create samples with different RenderAtScale values
        var scales = new[] { 0.5, 1.0, 2.0, 4.0 };

        foreach (var scale in scales)
        {
            var sampleBox = new GroupBox
            {
                Header = $"RenderAtScale = {scale}",
                Width = 250,
                Height = 250,
                Margin = new Thickness(10)
            };

            var content = new Border
            {
                Background = new LinearGradientBrush(Colors.Purple, Colors.Blue, 45),
                CacheMode = new BitmapCache { RenderAtScale = scale }
            };

            var stack = new StackPanel
            {
                HorizontalAlignment = HorizontalAlignment.Center,
                VerticalAlignment = VerticalAlignment.Center
            };

            // Vector graphics that benefit from higher scale
            var ellipse = new Ellipse
            {
                Width = 100,
                Height = 100,
                Fill = Brushes.White,
                Stroke = Brushes.Yellow,
                StrokeThickness = 3
            };

            var text = new TextBlock
            {
                Text = $"Scale: {scale}x",
                FontSize = 18,
                FontWeight = FontWeights.Bold,
                Foreground = Brushes.White,
                HorizontalAlignment = HorizontalAlignment.Center,
                Margin = new Thickness(0, 10, 0, 0)
            };

            stack.Children.Add(ellipse);
            stack.Children.Add(text);
            content.Child = stack;

            // Apply scale transform to see quality difference
            content.LayoutTransform = new ScaleTransform(1.5, 1.5);

            sampleBox.Content = content;
            wrapPanel.Children.Add(sampleBox);
        }

        panel.Children.Add(wrapPanel);

        var infoBox = new GroupBox
        {
            Header = "RenderAtScale Information",
            Margin = new Thickness(0, 20, 0, 0)
        };

        infoBox.Content = new TextBlock
        {
            Text = "RenderAtScale controls cache bitmap quality:\n\n" +
                   "• 0.5: Lower quality, less memory (fuzzy when scaled)\n" +
                   "• 1.0: Default quality (may be fuzzy when scaled up)\n" +
                   "• 2.0: Higher quality (crisp when scaled moderately)\n" +
                   "• 4.0: Very high quality (crisp when scaled significantly)\n\n" +
                   "TRADE-OFFS:\n" +
                   "• Higher scale = better quality\n" +
                   "• Higher scale = more memory\n" +
                   "• Higher scale = longer initial render\n\n" +
                   "Use higher scales when content will be zoomed or scaled.\n" +
                   "Notice the quality difference in scaled samples above!",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(infoBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        TxtCacheDetails.Text = "RenderAtScale comparison showing different quality levels.\n" +
                               "Higher values produce better quality but use more memory.";
        TxtRenderScale.Text = "Multiple (0.5 to 4.0)";
        TxtStatus.Text = "RenderAtScale demonstration - compare quality at different scales";
    }

    private void BtnSnapsToPixels_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating SnapsToDevicePixels property...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "SnapsToDevicePixels Comparison",
            FontSize = 18,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 10)
        });

        var compareGrid = new Grid();
        compareGrid.ColumnDefinitions.Add(new ColumnDefinition());
        compareGrid.ColumnDefinitions.Add(new ColumnDefinition());

        // Without SnapsToDevicePixels
        var withoutBox = new GroupBox
        {
            Header = "SnapsToDevicePixels = false",
            Margin = new Thickness(5)
        };

        var withoutContent = new Border
        {
            Width = 400,
            Height = 300,
            Background = Brushes.White,
            CacheMode = new BitmapCache { SnapsToDevicePixels = false }
        };

        var withoutCanvas = new Canvas();
        for (int i = 0; i < 20; i++)
        {
            withoutCanvas.Children.Add(new Line
            {
                X1 = i * 20,
                Y1 = 0,
                X2 = i * 20,
                Y2 = 300,
                Stroke = Brushes.Black,
                StrokeThickness = 1
            });

            withoutCanvas.Children.Add(new Line
            {
                X1 = 0,
                Y1 = i * 15,
                X2 = 400,
                Y2 = i * 15,
                Stroke = Brushes.Black,
                StrokeThickness = 1
            });
        }

        withoutContent.Child = withoutCanvas;
        withoutBox.Content = withoutContent;
        Grid.SetColumn(withoutBox, 0);
        compareGrid.Children.Add(withoutBox);

        // With SnapsToDevicePixels
        var withBox = new GroupBox
        {
            Header = "SnapsToDevicePixels = true (Crisper)",
            Margin = new Thickness(5)
        };

        var withContent = new Border
        {
            Width = 400,
            Height = 300,
            Background = Brushes.White,
            CacheMode = new BitmapCache { SnapsToDevicePixels = true }
        };

        var withCanvas = new Canvas();
        for (int i = 0; i < 20; i++)
        {
            withCanvas.Children.Add(new Line
            {
                X1 = i * 20,
                Y1 = 0,
                X2 = i * 20,
                Y2 = 300,
                Stroke = Brushes.Black,
                StrokeThickness = 1
            });

            withCanvas.Children.Add(new Line
            {
                X1 = 0,
                Y1 = i * 15,
                X2 = 400,
                Y2 = i * 15,
                Stroke = Brushes.Black,
                StrokeThickness = 1
            });
        }

        withContent.Child = withCanvas;
        withBox.Content = withContent;
        Grid.SetColumn(withBox, 1);
        compareGrid.Children.Add(withBox);

        panel.Children.Add(compareGrid);

        var infoBox = new GroupBox
        {
            Header = "SnapsToDevicePixels Information",
            Margin = new Thickness(0, 20, 0, 0)
        };

        infoBox.Content = new TextBlock
        {
            Text = "SnapsToDevicePixels aligns cache to device pixels:\n\n" +
                   "false (Default):\n" +
                   "• May appear slightly blurry\n" +
                   "• Sub-pixel positioning\n" +
                   "• Better for transformations\n\n" +
                   "true:\n" +
                   "• Crisper rendering\n" +
                   "• Aligned to physical pixels\n" +
                   "• Better for static content\n" +
                   "• Especially important for text and lines\n\n" +
                   "Use SnapsToDevicePixels = true for:\n" +
                   "• Text rendering\n" +
                   "• Crisp lines and borders\n" +
                   "• UI elements that don't transform\n\n" +
                   "The right side should appear crisper!",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(infoBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        TxtCacheDetails.Text = "SnapsToDevicePixels comparison.\n" +
                               "Right side aligns to device pixels for crisper rendering.";
        TxtStatus.Text = "SnapsToDevicePixels demonstration - compare left (blurry) vs right (crisp)";
    }

    private void BtnComplexContent_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating caching with complex content...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "Complex Content Caching Benefit",
            FontSize = 18,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 10)
        });

        var mainBorder = new Border
        {
            Width = 700,
            Height = 500,
            Background = Brushes.White,
            BorderBrush = Brushes.Black,
            BorderThickness = new Thickness(2),
            CacheMode = new BitmapCache { RenderAtScale = 1.5 }
        };

        var canvas = new Canvas();

        // Create very complex vector graphics
        var random = new Random(42); // Seed for consistency

        // Gradients
        for (int i = 0; i < 20; i++)
        {
            var rect = new Rectangle
            {
                Width = 100,
                Height = 100,
                Fill = new LinearGradientBrush(
                    Color.FromRgb((byte)random.Next(255), (byte)random.Next(255), (byte)random.Next(255)),
                    Color.FromRgb((byte)random.Next(255), (byte)random.Next(255), (byte)random.Next(255)),
                    random.Next(360)
                ),
                Opacity = 0.5
            };
            Canvas.SetLeft(rect, random.Next(0, 600));
            Canvas.SetTop(rect, random.Next(0, 400));
            canvas.Children.Add(rect);
        }

        // Bezier curves
        for (int i = 0; i < 30; i++)
        {
            var path = new System.Windows.Shapes.Path
            {
                Stroke = new SolidColorBrush(Color.FromRgb(
                    (byte)random.Next(255),
                    (byte)random.Next(255),
                    (byte)random.Next(255)
                )),
                StrokeThickness = 3,
                Data = new PathGeometry
                {
                    Figures = new PathFigureCollection
                    {
                        new PathFigure
                        {
                            StartPoint = new Point(random.Next(0, 700), random.Next(0, 500)),
                            Segments = new PathSegmentCollection
                            {
                                new BezierSegment(
                                    new Point(random.Next(0, 700), random.Next(0, 500)),
                                    new Point(random.Next(0, 700), random.Next(0, 500)),
                                    new Point(random.Next(0, 700), random.Next(0, 500)),
                                    true
                                )
                            }
                        }
                    }
                }
            };
            canvas.Children.Add(path);
        }

        // Effects
        canvas.Effect = new DropShadowEffect
        {
            BlurRadius = 10,
            ShadowDepth = 3,
            Opacity = 0.5
        };

        mainBorder.Child = canvas;
        panel.Children.Add(mainBorder);

        // Controls
        var controlBox = new GroupBox
        {
            Header = "Transform Controls",
            Margin = new Thickness(0, 20, 0, 0)
        };

        var controlStack = new StackPanel();

        var transformGroup = new TransformGroup();
        var rotateTransform = new RotateTransform();
        var scaleTransform = new ScaleTransform(1, 1);
        transformGroup.Children.Add(rotateTransform);
        transformGroup.Children.Add(scaleTransform);

        mainBorder.RenderTransform = transformGroup;
        mainBorder.RenderTransformOrigin = new Point(0.5, 0.5);

        // Rotation slider
        var rotateSlider = new Slider
        {
            Minimum = 0,
            Maximum = 360,
            Value = 0,
            Margin = new Thickness(10)
        };

        var rotateLabel = new TextBlock
        {
            Text = "Rotation: 0°",
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(10)
        };

        rotateSlider.ValueChanged += (s, e) =>
        {
            rotateTransform.Angle = e.NewValue;
            rotateLabel.Text = $"Rotation: {e.NewValue:F0}°";
        };

        // Scale slider
        var scaleSlider = new Slider
        {
            Minimum = 0.5,
            Maximum = 2.0,
            Value = 1.0,
            Margin = new Thickness(10)
        };

        var scaleLabel = new TextBlock
        {
            Text = "Scale: 1.0x",
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(10)
        };

        scaleSlider.ValueChanged += (s, e) =>
        {
            scaleTransform.ScaleX = e.NewValue;
            scaleTransform.ScaleY = e.NewValue;
            scaleLabel.Text = $"Scale: {e.NewValue:F2}x";
        };

        controlStack.Children.Add(rotateLabel);
        controlStack.Children.Add(rotateSlider);
        controlStack.Children.Add(scaleLabel);
        controlStack.Children.Add(scaleSlider);

        controlBox.Content = controlStack;
        panel.Children.Add(controlBox);

        var infoBox = new GroupBox
        {
            Header = "Complex Content Caching"
        };

        infoBox.Content = new TextBlock
        {
            Text = "This complex visual contains:\n" +
                   "• 20 gradient-filled rectangles\n" +
                   "• 30 Bezier curve paths\n" +
                   "• Drop shadow effect\n" +
                   "• High opacity blending\n\n" +
                   "WITHOUT caching:\n" +
                   "• All vectors re-rendered on transform\n" +
                   "• Very slow performance\n\n" +
                   "WITH caching (enabled above):\n" +
                   "• Rendered to bitmap once\n" +
                   "• Bitmap transformed smoothly\n" +
                   "• Excellent performance\n\n" +
                   "Try rotating and scaling - smooth performance due to caching!",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(infoBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        TxtCacheEnabled.Text = "Yes (RenderAtScale = 1.5)";
        long estimatedSize = (700 * 500 * 4 * 15) / (1024 * 10); // Rough estimate
        TxtCacheSize.Text = $"~{estimatedSize} KB";
        TxtCacheDetails.Text = "Complex content cached for smooth transforms.\n" +
                               "Try the sliders to see GPU-accelerated bitmap transformation.";
        TxtStatus.Text = "Complex content demonstration - use sliders to transform cached bitmap";
    }

    private void BtnAnimationCache_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating caching for animations...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "Bitmap Caching for Smooth Animations",
            FontSize = 18,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 10)
        });

        var canvas = new Canvas
        {
            Width = 800,
            Height = 500,
            Background = Brushes.Black
        };

        var animatedElements = new List<Border>();

        for (int i = 0; i < 20; i++)
        {
            var element = new Border
            {
                Width = 100,
                Height = 100,
                Background = new RadialGradientBrush(
                    Color.FromRgb(
                        (byte)Random.Shared.Next(100, 255),
                        (byte)Random.Shared.Next(100, 255),
                        (byte)Random.Shared.Next(100, 255)
                    ),
                    Colors.White
                ),
                BorderBrush = Brushes.White,
                BorderThickness = new Thickness(2),
                CornerRadius = new CornerRadius(10),
                CacheMode = new BitmapCache(),
                Effect = new DropShadowEffect
                {
                    BlurRadius = 10,
                    ShadowDepth = 5,
                    Color = Colors.White,
                    Opacity = 0.5
                }
            };

            // Complex content inside
            var stack = new StackPanel
            {
                HorizontalAlignment = HorizontalAlignment.Center,
                VerticalAlignment = VerticalAlignment.Center
            };

            stack.Children.Add(new Ellipse
            {
                Width = 40,
                Height = 40,
                Fill = Brushes.Yellow
            });

            stack.Children.Add(new TextBlock
            {
                Text = $"#{i + 1}",
                Foreground = Brushes.White,
                FontWeight = FontWeights.Bold,
                HorizontalAlignment = HorizontalAlignment.Center
            });

            element.Child = stack;

            Canvas.SetLeft(element, Random.Shared.Next(0, 700));
            Canvas.SetTop(element, Random.Shared.Next(0, 400));

            element.RenderTransform = new RotateTransform();
            element.RenderTransformOrigin = new Point(0.5, 0.5);

            animatedElements.Add(element);
            canvas.Children.Add(element);
        }

        panel.Children.Add(canvas);

        // Controls
        var controlBox = new GroupBox
        {
            Header = "Animation Controls",
            Margin = new Thickness(0, 20, 0, 0)
        };

        var startButton = new Button { Content = "Start Animation", Margin = new Thickness(5) };
        var stopButton = new Button { Content = "Stop Animation", Margin = new Thickness(5) };
        var toggleCacheButton = new Button { Content = "Toggle Cache", Margin = new Thickness(5) };

        bool cacheEnabled = true;

        startButton.Click += (s, e) =>
        {
            _animationTimer?.Stop();
            _animationTimer = new DispatcherTimer { Interval = TimeSpan.FromMilliseconds(16) };

            _animationTimer.Tick += (s, e) =>
            {
                foreach (var element in animatedElements)
                {
                    var transform = (RotateTransform)element.RenderTransform;
                    transform.Angle = (transform.Angle + 2) % 360;
                }
            };

            _animationTimer.Start();
            TxtPerformance.Text = "Animating (watch FPS)";
        };

        stopButton.Click += (s, e) =>
        {
            _animationTimer?.Stop();
            TxtPerformance.Text = "Stopped";
        };

        toggleCacheButton.Click += (s, e) =>
        {
            cacheEnabled = !cacheEnabled;
            foreach (var element in animatedElements)
            {
                element.CacheMode = cacheEnabled ? new BitmapCache() : null;
            }
            TxtCacheEnabled.Text = cacheEnabled ? "Yes" : "No";
            toggleCacheButton.Content = cacheEnabled ? "Disable Cache" : "Enable Cache";
        };

        controlBox.Content = new WrapPanel
        {
            Children = { startButton, stopButton, toggleCacheButton }
        };

        panel.Children.Add(controlBox);

        var infoBox = new GroupBox
        {
            Header = "Animation Caching Benefits"
        };

        infoBox.Content = new TextBlock
        {
            Text = "Bitmap caching is ideal for animations:\n\n" +
                   "WITH Cache:\n" +
                   "• Complex content rendered once\n" +
                   "• Only transform updated each frame\n" +
                   "• Smooth 60 FPS performance\n" +
                   "• Lower CPU/GPU load\n\n" +
                   "WITHOUT Cache:\n" +
                   "• Full re-render every frame\n" +
                   "• Effects recalculated\n" +
                   "• Lower FPS (especially with effects)\n" +
                   "• Higher resource usage\n\n" +
                   "Try toggling cache while animating to see FPS impact!",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(infoBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        TxtCacheEnabled.Text = "Yes (20 elements)";
        TxtCacheDetails.Text = "20 complex elements with effects, all cached.\n" +
                               "Toggle cache during animation to see performance difference.";
        TxtStatus.Text = "Animation caching demo - start animation and try toggling cache";
    }

    private void BtnBestPractices_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Displaying bitmap caching best practices...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "Bitmap Caching Best Practices",
            FontSize = 20,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 20),
            Foreground = Brushes.DarkBlue
        });

        // When to use
        var whenBox = new GroupBox
        {
            Header = "✓ When to Use BitmapCache",
            Margin = new Thickness(0, 10, 0, 10)
        };

        whenBox.Content = new TextBlock
        {
            Text = "• Complex visual tree with many elements\n" +
                   "• Vector graphics and paths\n" +
                   "• Content with effects (blur, shadow)\n" +
                   "• Elements being transformed (rotate, scale, opacity)\n" +
                   "• Animated positioning (RenderTransform)\n" +
                   "• Static content that rarely changes\n" +
                   "• Repeating complex patterns",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10),
            FontSize = 13
        };

        panel.Children.Add(whenBox);

        // When NOT to use
        var whenNotBox = new GroupBox
        {
            Header = "✗ When NOT to Use BitmapCache",
            Margin = new Thickness(0, 10, 0, 10)
        };

        whenNotBox.Content = new TextBlock
        {
            Text = "• Content changes frequently\n" +
                   "• Simple visuals (single color, simple shapes)\n" +
                   "• Very large elements (memory concerns)\n" +
                   "• Text that needs to remain crisp at all scales\n" +
                   "• Frequent property changes (invalidates cache)\n" +
                   "• Memory-constrained environments",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10),
            FontSize = 13,
            Foreground = Brushes.DarkRed
        };

        panel.Children.Add(whenNotBox);

        // Property recommendations
        var propsBox = new GroupBox
        {
            Header = "Property Recommendations",
            Margin = new Thickness(0, 10, 0, 10)
        };

        var propsGrid = new Grid();
        propsGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(150) });
        propsGrid.ColumnDefinitions.Add(new ColumnDefinition());

        var props = new[]
        {
            ("RenderAtScale", "Use 1.0 for normal, 2.0+ if scaling up, 0.5 for memory savings"),
            ("SnapsToDevicePixels", "true for crisp text/lines, false for smooth transforms"),
            ("EnableClearType", "true for text rendering (when available)"),
            ("Cache Lifetime", "Clear when content changes significantly")
        };

        for (int i = 0; i < props.Length; i++)
        {
            propsGrid.RowDefinitions.Add(new RowDefinition { Height = GridLength.Auto });

            var (name, desc) = props[i];

            var nameBlock = new TextBlock
            {
                Text = name + ":",
                FontWeight = FontWeights.Bold,
                Margin = new Thickness(5),
                VerticalAlignment = VerticalAlignment.Top
            };
            Grid.SetRow(nameBlock, i);
            Grid.SetColumn(nameBlock, 0);
            propsGrid.Children.Add(nameBlock);

            var descBlock = new TextBlock
            {
                Text = desc,
                TextWrapping = TextWrapping.Wrap,
                Margin = new Thickness(10, 5, 5, 5)
            };
            Grid.SetRow(descBlock, i);
            Grid.SetColumn(descBlock, 1);
            propsGrid.Children.Add(descBlock);
        }

        propsBox.Content = propsGrid;
        panel.Children.Add(propsBox);

        // Performance tips
        var tipsBox = new GroupBox
        {
            Header = "Performance Tips",
            Margin = new Thickness(0, 10, 0, 10)
        };

        tipsBox.Content = new TextBlock
        {
            Text = "1. MEASURE IMPACT: Always profile before and after caching\n\n" +
                   "2. MEMORY AWARENESS: Cache size = Width × Height × 4 bytes × RenderAtScale²\n" +
                   "   Example: 1000×1000 @ 2.0 scale = 16 MB\n\n" +
                   "3. TRANSFORM PREFERENCE: Use RenderTransform (not LayoutTransform)\n\n" +
                   "4. CACHE INVALIDATION: Changing cached content forces re-render\n\n" +
                   "5. COMBINE WITH EFFECTS: Cache once, apply effects to bitmap\n\n" +
                   "6. TEST SCENARIOS: Different hardware may show different benefits\n\n" +
                   "7. CLEAR UNUSED: Set CacheMode = null when no longer needed",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10),
            FontSize = 12,
            FontFamily = new FontFamily("Consolas")
        };

        panel.Children.Add(tipsBox);

        // Code example
        var codeBox = new GroupBox
        {
            Header = "Code Example",
            Margin = new Thickness(0, 10, 0, 10)
        };

        codeBox.Content = new TextBlock
        {
            Text = "// Enable caching for complex, animated element\n" +
                   "myElement.CacheMode = new BitmapCache\n" +
                   "{\n" +
                   "    RenderAtScale = 2.0,  // High quality\n" +
                   "    SnapsToDevicePixels = true  // Crisp rendering\n" +
                   "};\n\n" +
                   "// Use RenderTransform for animations\n" +
                   "myElement.RenderTransform = new RotateTransform();\n\n" +
                   "// Clear cache when done\n" +
                   "myElement.CacheMode = null;",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10),
            FontFamily = new FontFamily("Consolas"),
            Background = Brushes.WhiteSmoke
        };

        panel.Children.Add(codeBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        TxtCacheDetails.Text = "Best practices for effective bitmap caching.\n" +
                               "Follow these guidelines for optimal performance.";
        TxtStatus.Text = "Best practices displayed - follow these guidelines for effective caching";
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
        TxtCacheDetails.Text = "";
        TxtCacheEnabled.Text = "No";
        TxtCacheSize.Text = "0 KB";
        TxtRenderScale.Text = "1.0";
        TxtPerformance.Text = "-";
    }

    protected override void OnClosed(EventArgs e)
    {
        _animationTimer?.Stop();
        base.OnClosed(e);
    }
}
