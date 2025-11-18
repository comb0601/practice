using System.Diagnostics;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Effects;
using System.Windows.Media.Media3D;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace GPUAcceleration;

public partial class MainWindow : Window
{
    private DispatcherTimer? _fpsTimer;
    private int _frameCount = 0;
    private readonly Stopwatch _fpsStopwatch = new();

    public MainWindow()
    {
        InitializeComponent();
        Loaded += MainWindow_Loaded;
    }

    private void MainWindow_Loaded(object sender, RoutedEventArgs e)
    {
        UpdateGPUInfo();
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

    private void UpdateGPUInfo()
    {
        int tier = RenderCapability.Tier >> 16;
        TxtRenderTier.Text = $"Tier {tier}";

        bool isGPUAccelerated = tier >= 1;
        TxtGPUAccelerated.Text = isGPUAccelerated ? "Yes" : "No (Software)";
        TxtGPUAccelerated.Foreground = isGPUAccelerated ? Brushes.Green : Brushes.Red;

        TxtMaxTextureSize.Text = $"{RenderCapability.MaxHardwareTextureSize}x{RenderCapability.MaxHardwareTextureSize}";

        var pixelShaderSlots = RenderCapability.MaxPixelShaderInstructionSlots(0, 0);
        TxtPixelShader.Text = pixelShaderSlots > 0 ? $"Supported ({pixelShaderSlots} slots)" : "Not Supported";

        TxtVideoMemory.Text = "Dynamic (Managed by DirectX)";
        TxtDirectXVersion.Text = tier >= 2 ? "DirectX 9+" : tier >= 1 ? "DirectX 9 (Limited)" : "Software";
        TxtEffectsSupported.Text = tier >= 1 ? "Yes" : "Limited";

        var features = new StringBuilder();
        features.AppendLine("SUPPORTED GPU FEATURES:");
        features.AppendLine();
        features.AppendLine($"✓ Render Tier: {tier}");
        features.AppendLine($"{(isGPUAccelerated ? "✓" : "✗")} Hardware Acceleration");
        features.AppendLine($"{(tier >= 2 ? "✓" : "✗")} Full Shader Support");
        features.AppendLine($"{(tier >= 1 ? "✓" : "✗")} Basic Effects");
        features.AppendLine($"✓ 3D Transformations");
        features.AppendLine($"✓ Composition");
        features.AppendLine($"{(tier >= 2 ? "✓" : "✗")} Video Acceleration");
        features.AppendLine();
        features.AppendLine("CAPABILITIES:");
        features.AppendLine($"Max Texture: {RenderCapability.MaxHardwareTextureSize}px");
        features.AppendLine($"Pixel Shader: {(pixelShaderSlots > 0 ? "Yes" : "No")}");

        TxtGPUFeatures.Text = features.ToString();
    }

    private void BtnGPUInfo_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Displaying GPU and hardware information...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "GPU Hardware Information",
            FontSize = 20,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 20),
            Foreground = Brushes.DarkBlue
        });

        int tier = RenderCapability.Tier >> 16;

        // Render Tier Display
        var tierBox = new Border
        {
            Background = tier switch
            {
                0 => Brushes.IndianRed,
                1 => Brushes.Orange,
                2 => Brushes.LightGreen,
                _ => Brushes.LightGray
            },
            BorderBrush = Brushes.DarkBlue,
            BorderThickness = new Thickness(3),
            Padding = new Thickness(20),
            Margin = new Thickness(0, 10, 0, 10),
            CornerRadius = new CornerRadius(10)
        };

        var tierContent = new StackPanel();
        tierContent.Children.Add(new TextBlock
        {
            Text = $"Render Tier {tier}",
            FontSize = 24,
            FontWeight = FontWeights.Bold,
            HorizontalAlignment = HorizontalAlignment.Center
        });

        tierContent.Children.Add(new TextBlock
        {
            Text = tier switch
            {
                0 => "Software Rendering - No GPU Acceleration",
                1 => "Partial Hardware Acceleration",
                2 => "Full Hardware Acceleration",
                _ => "Unknown"
            },
            FontSize = 16,
            HorizontalAlignment = HorizontalAlignment.Center,
            Margin = new Thickness(0, 10, 0, 0)
        });

        tierBox.Child = tierContent;
        panel.Children.Add(tierBox);

        // Capabilities Grid
        var capsBox = new GroupBox
        {
            Header = "Hardware Capabilities",
            Margin = new Thickness(0, 10, 0, 10)
        };

        var capsGrid = new Grid();
        capsGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(200) });
        capsGrid.ColumnDefinitions.Add(new ColumnDefinition());

        var capabilities = new[]
        {
            ("GPU Acceleration", tier >= 1 ? "Enabled" : "Disabled", tier >= 1),
            ("Maximum Texture Size", $"{RenderCapability.MaxHardwareTextureSize}x{RenderCapability.MaxHardwareTextureSize}", true),
            ("Pixel Shader Support", RenderCapability.MaxPixelShaderInstructionSlots(0, 0) > 0 ? "Yes" : "No",
             RenderCapability.MaxPixelShaderInstructionSlots(0, 0) > 0),
            ("Shader Effects", tier >= 1 ? "Supported" : "Limited", tier >= 1),
            ("3D Transforms", "Supported", true),
            ("Video Acceleration", tier >= 2 ? "Yes" : "Limited", tier >= 2),
            ("DirectX Version", tier >= 2 ? "9.0+" : tier >= 1 ? "9.0 (Limited)" : "Software", tier >= 1)
        };

        for (int i = 0; i < capabilities.Length; i++)
        {
            capsGrid.RowDefinitions.Add(new RowDefinition { Height = GridLength.Auto });

            var (name, value, isSupported) = capabilities[i];

            var nameBlock = new TextBlock
            {
                Text = name + ":",
                FontWeight = FontWeights.Bold,
                Margin = new Thickness(5)
            };
            Grid.SetRow(nameBlock, i);
            Grid.SetColumn(nameBlock, 0);
            capsGrid.Children.Add(nameBlock);

            var valueBlock = new TextBlock
            {
                Text = value,
                Margin = new Thickness(10, 5, 5, 5),
                Foreground = isSupported ? Brushes.DarkGreen : Brushes.DarkRed,
                FontWeight = FontWeights.Bold
            };
            Grid.SetRow(valueBlock, i);
            Grid.SetColumn(valueBlock, 1);
            capsGrid.Children.Add(valueBlock);
        }

        capsBox.Content = capsGrid;
        panel.Children.Add(capsBox);

        // Performance Impact
        var perfBox = new GroupBox
        {
            Header = "Performance Impact",
            Margin = new Thickness(0, 10, 0, 10)
        };

        perfBox.Content = new TextBlock
        {
            Text = tier switch
            {
                0 => "Software rendering uses CPU for all graphics operations.\n" +
                     "Expect slower performance and higher CPU usage.\n" +
                     "Recommendation: Simplify visuals, consider hardware upgrade.",
                1 => "Partial GPU acceleration available.\n" +
                     "Some features hardware-accelerated, others on CPU.\n" +
                     "Recommendation: Test features, moderate complexity OK.",
                2 => "Full GPU acceleration available.\n" +
                     "All rendering features hardware-accelerated.\n" +
                     "Recommendation: Use rich visuals, effects, and 3D transforms.",
                _ => "Unknown rendering tier."
            },
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(perfBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        TxtStatus.Text = $"GPU Info displayed - Tier {tier} - {(tier >= 1 ? "Hardware Accelerated" : "Software Rendering")}";
    }

    private void BtnHWAcceleration_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating hardware acceleration benefits...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "Hardware Acceleration Demonstration",
            FontSize = 18,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 10)
        });

        // Create two comparison areas
        var compareGrid = new Grid();
        compareGrid.ColumnDefinitions.Add(new ColumnDefinition());
        compareGrid.ColumnDefinitions.Add(new ColumnDefinition());

        // Software rendering simulation
        var softwareBox = new GroupBox
        {
            Header = "Software Rendering (CPU)",
            Margin = new Thickness(5)
        };

        var softwareCanvas = new Canvas
        {
            Width = 400,
            Height = 300,
            Background = Brushes.LightYellow
        };

        // Add many elements
        for (int i = 0; i < 50; i++)
        {
            var rect = new Rectangle
            {
                Width = 40,
                Height = 40,
                Fill = new SolidColorBrush(Color.FromRgb(
                    (byte)Random.Shared.Next(100, 255),
                    (byte)Random.Shared.Next(100, 255),
                    (byte)Random.Shared.Next(100, 255)
                )),
                Stroke = Brushes.Black,
                StrokeThickness = 1
            };

            Canvas.SetLeft(rect, Random.Shared.Next(0, 360));
            Canvas.SetTop(rect, Random.Shared.Next(0, 260));
            softwareCanvas.Children.Add(rect);
        }

        softwareBox.Content = softwareCanvas;
        Grid.SetColumn(softwareBox, 0);
        compareGrid.Children.Add(softwareBox);

        // Hardware rendering
        var hardwareBox = new GroupBox
        {
            Header = "Hardware Rendering (GPU)",
            Margin = new Thickness(5)
        };

        var hardwareCanvas = new Canvas
        {
            Width = 400,
            Height = 300,
            Background = Brushes.LightGreen
        };

        // Add elements with effects (GPU-accelerated)
        for (int i = 0; i < 50; i++)
        {
            var ellipse = new Ellipse
            {
                Width = 40,
                Height = 40,
                Fill = new RadialGradientBrush(
                    Color.FromRgb(
                        (byte)Random.Shared.Next(100, 255),
                        (byte)Random.Shared.Next(100, 255),
                        (byte)Random.Shared.Next(100, 255)
                    ),
                    Colors.White
                ),
                Effect = new DropShadowEffect
                {
                    BlurRadius = 5,
                    ShadowDepth = 3,
                    Opacity = 0.5
                }
            };

            Canvas.SetLeft(ellipse, Random.Shared.Next(0, 360));
            Canvas.SetTop(ellipse, Random.Shared.Next(0, 260));
            hardwareCanvas.Children.Add(ellipse);
        }

        hardwareBox.Content = hardwareCanvas;
        Grid.SetColumn(hardwareBox, 1);
        compareGrid.Children.Add(hardwareBox);

        panel.Children.Add(compareGrid);

        // Explanation
        var explanationBox = new GroupBox
        {
            Header = "Hardware Acceleration Benefits",
            Margin = new Thickness(0, 20, 0, 0)
        };

        explanationBox.Content = new TextBlock
        {
            Text = "HARDWARE ACCELERATION:\n\n" +
                   "Software Rendering (Left):\n" +
                   "• CPU handles all rendering\n" +
                   "• Slower for complex visuals\n" +
                   "• Limited effects support\n" +
                   "• Higher CPU usage\n\n" +
                   "Hardware Rendering (Right):\n" +
                   "• GPU handles rendering\n" +
                   "• Faster performance\n" +
                   "• Full effects support (shadows, blur, etc.)\n" +
                   "• Lower CPU usage\n" +
                   "• Better for animations\n\n" +
                   "The right side uses GPU-accelerated shader effects!",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(explanationBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        TxtElements.Text = "100";
        TxtStatus.Text = "Hardware acceleration comparison displayed";
    }

    private void BtnShaderEffects_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating GPU shader effects...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "GPU Shader Effects Gallery",
            FontSize = 18,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 10)
        });

        var wrapPanel = new WrapPanel();

        // Original (no effect)
        wrapPanel.Children.Add(CreateEffectSample("Original", null));

        // Blur Effect
        wrapPanel.Children.Add(CreateEffectSample("Blur", new BlurEffect { Radius = 10 }));

        // Drop Shadow
        wrapPanel.Children.Add(CreateEffectSample("Drop Shadow", new DropShadowEffect
        {
            BlurRadius = 15,
            ShadowDepth = 5,
            Color = Colors.Black,
            Opacity = 0.5
        }));

        // Outer Glow (using DropShadow with 0 depth)
        wrapPanel.Children.Add(CreateEffectSample("Outer Glow", new DropShadowEffect
        {
            BlurRadius = 20,
            ShadowDepth = 0,
            Color = Colors.Yellow,
            Opacity = 0.8
        }));

        // Multiple effects combined
        var multiEffect = new BlurEffect { Radius = 5 };
        wrapPanel.Children.Add(CreateEffectSample("Blur + Shadow", multiEffect));

        panel.Children.Add(wrapPanel);

        // Effects info
        var infoBox = new GroupBox
        {
            Header = "Shader Effects Information",
            Margin = new Thickness(0, 20, 0, 0)
        };

        infoBox.Content = new TextBlock
        {
            Text = "WPF includes GPU-accelerated shader effects:\n\n" +
                   "• BlurEffect: Gaussian blur (GPU-accelerated)\n" +
                   "• DropShadowEffect: Shadows and glows\n" +
                   "• Custom Effects: Create your own shaders (HLSL)\n\n" +
                   "PERFORMANCE:\n" +
                   "• Tier 2: Full hardware acceleration\n" +
                   "• Tier 1: Limited support\n" +
                   "• Tier 0: Software fallback (slow)\n\n" +
                   "BEST PRACTICES:\n" +
                   "• Use sparingly for best performance\n" +
                   "• Cache with BitmapCache when static\n" +
                   "• Test on target hardware\n" +
                   "• Consider software fallback",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(infoBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        TxtStatus.Text = "Shader effects gallery displayed - all effects are GPU-accelerated";
    }

    private Border CreateEffectSample(string name, Effect? effect)
    {
        var container = new Border
        {
            Width = 200,
            Height = 180,
            Margin = new Thickness(10),
            BorderBrush = Brushes.Gray,
            BorderThickness = new Thickness(1),
            Background = Brushes.WhiteSmoke
        };

        var stack = new StackPanel
        {
            HorizontalAlignment = HorizontalAlignment.Center,
            VerticalAlignment = VerticalAlignment.Center
        };

        var content = new Border
        {
            Width = 120,
            Height = 80,
            Background = new LinearGradientBrush(Colors.CornflowerBlue, Colors.LightBlue, 45),
            BorderBrush = Brushes.DarkBlue,
            BorderThickness = new Thickness(2),
            CornerRadius = new CornerRadius(10),
            Effect = effect,
            Child = new TextBlock
            {
                Text = "Sample",
                FontSize = 18,
                FontWeight = FontWeights.Bold,
                Foreground = Brushes.White,
                HorizontalAlignment = HorizontalAlignment.Center,
                VerticalAlignment = VerticalAlignment.Center
            }
        };

        stack.Children.Add(content);
        stack.Children.Add(new TextBlock
        {
            Text = name,
            FontWeight = FontWeights.Bold,
            HorizontalAlignment = HorizontalAlignment.Center,
            Margin = new Thickness(0, 10, 0, 0)
        });

        container.Child = stack;
        return container;
    }

    private void BtnBlurEffect_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Interactive blur effect demonstration...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "Interactive Blur Effect",
            FontSize = 18,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 10)
        });

        // Image/content to blur
        var contentBorder = new Border
        {
            Width = 600,
            Height = 300,
            Background = new LinearGradientBrush
            {
                StartPoint = new Point(0, 0),
                EndPoint = new Point(1, 1),
                GradientStops = new GradientStopCollection
                {
                    new GradientStop(Colors.Purple, 0.0),
                    new GradientStop(Colors.Blue, 0.3),
                    new GradientStop(Colors.Cyan, 0.6),
                    new GradientStop(Colors.Green, 1.0)
                }
            },
            Margin = new Thickness(0, 10, 0, 10)
        };

        var contentGrid = new Grid();
        for (int i = 0; i < 5; i++)
        {
            contentGrid.Children.Add(new TextBlock
            {
                Text = $"GPU-Accelerated Blur Effect - Line {i + 1}",
                FontSize = 24,
                FontWeight = FontWeights.Bold,
                Foreground = Brushes.White,
                Margin = new Thickness(20, 20 + i * 40, 20, 0),
                VerticalAlignment = VerticalAlignment.Top
            });
        }

        contentBorder.Child = contentGrid;
        panel.Children.Add(contentBorder);

        // Blur control
        var controlBox = new GroupBox
        {
            Header = "Blur Radius Control",
            Margin = new Thickness(0, 10, 0, 10)
        };

        var controlStack = new StackPanel();

        var slider = new Slider
        {
            Minimum = 0,
            Maximum = 30,
            Value = 0,
            TickFrequency = 1,
            IsSnapToTickEnabled = true,
            Margin = new Thickness(10)
        };

        var valueText = new TextBlock
        {
            Text = "Radius: 0",
            FontWeight = FontWeights.Bold,
            HorizontalAlignment = HorizontalAlignment.Center,
            Margin = new Thickness(10)
        };

        var blurEffect = new BlurEffect { Radius = 0 };
        contentBorder.Effect = blurEffect;

        slider.ValueChanged += (s, e) =>
        {
            blurEffect.Radius = e.NewValue;
            valueText.Text = $"Radius: {e.NewValue:F0}";
        };

        controlStack.Children.Add(valueText);
        controlStack.Children.Add(slider);
        controlBox.Content = controlStack;
        panel.Children.Add(controlBox);

        // Info
        var infoBox = new GroupBox
        {
            Header = "BlurEffect Information"
        };

        infoBox.Content = new TextBlock
        {
            Text = "BlurEffect is a GPU-accelerated Gaussian blur:\n\n" +
                   "• Rendered entirely on GPU (Tier 1+)\n" +
                   "• Radius: 0-30 (larger = more blur)\n" +
                   "• Performance impact increases with radius\n" +
                   "• Works on any UIElement\n" +
                   "• Can be combined with other effects\n\n" +
                   "Use the slider to adjust blur intensity!",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(infoBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        TxtStatus.Text = "Blur effect demo active - use slider to adjust blur radius";
    }

    private void BtnDropShadow_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Interactive drop shadow effect demonstration...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "Interactive Drop Shadow Effect",
            FontSize = 18,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 10)
        });

        // Content with shadow
        var contentBorder = new Border
        {
            Width = 400,
            Height = 200,
            Background = Brushes.White,
            BorderBrush = Brushes.DarkGray,
            BorderThickness = new Thickness(2),
            CornerRadius = new CornerRadius(10),
            Margin = new Thickness(50),
            Child = new TextBlock
            {
                Text = "Drop Shadow Effect",
                FontSize = 32,
                FontWeight = FontWeights.Bold,
                Foreground = Brushes.Navy,
                HorizontalAlignment = HorizontalAlignment.Center,
                VerticalAlignment = VerticalAlignment.Center
            }
        };

        var shadowEffect = new DropShadowEffect
        {
            Color = Colors.Black,
            BlurRadius = 15,
            ShadowDepth = 5,
            Opacity = 0.5
        };

        contentBorder.Effect = shadowEffect;
        panel.Children.Add(contentBorder);

        // Controls
        var controlBox = new GroupBox
        {
            Header = "Shadow Controls",
            Margin = new Thickness(0, 10, 0, 10)
        };

        var controlGrid = new Grid();
        controlGrid.RowDefinitions.Add(new RowDefinition());
        controlGrid.RowDefinitions.Add(new RowDefinition());
        controlGrid.RowDefinitions.Add(new RowDefinition());
        controlGrid.RowDefinitions.Add(new RowDefinition());
        controlGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(120) });
        controlGrid.ColumnDefinitions.Add(new ColumnDefinition());
        controlGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(60) });

        // Blur Radius
        AddSliderControl(controlGrid, 0, "Blur Radius:", 0, 30, 15, v => shadowEffect.BlurRadius = v);

        // Shadow Depth
        AddSliderControl(controlGrid, 1, "Shadow Depth:", 0, 20, 5, v => shadowEffect.ShadowDepth = v);

        // Opacity
        AddSliderControl(controlGrid, 2, "Opacity:", 0, 1, 0.5, v => shadowEffect.Opacity = v);

        // Direction
        AddSliderControl(controlGrid, 3, "Direction:", 0, 360, 315, v => shadowEffect.Direction = v);

        controlBox.Content = controlGrid;
        panel.Children.Add(controlBox);

        // Info
        var infoBox = new GroupBox
        {
            Header = "DropShadowEffect Information"
        };

        infoBox.Content = new TextBlock
        {
            Text = "DropShadowEffect creates GPU-accelerated shadows:\n\n" +
                   "Properties:\n" +
                   "• BlurRadius: Shadow softness (0-30)\n" +
                   "• ShadowDepth: Distance from element\n" +
                   "• Direction: Angle (0-360 degrees)\n" +
                   "• Opacity: Shadow transparency\n" +
                   "• Color: Shadow color\n\n" +
                   "Use for depth, emphasis, and visual hierarchy!",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(infoBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        TxtStatus.Text = "Drop shadow demo active - adjust shadow properties";
    }

    private void AddSliderControl(Grid grid, int row, string label, double min, double max, double initial, Action<double> onChange)
    {
        var labelBlock = new TextBlock
        {
            Text = label,
            VerticalAlignment = VerticalAlignment.Center,
            Margin = new Thickness(5)
        };
        Grid.SetRow(labelBlock, row);
        Grid.SetColumn(labelBlock, 0);
        grid.Children.Add(labelBlock);

        var slider = new Slider
        {
            Minimum = min,
            Maximum = max,
            Value = initial,
            Margin = new Thickness(5)
        };
        Grid.SetRow(slider, row);
        Grid.SetColumn(slider, 1);
        grid.Children.Add(slider);

        var valueBlock = new TextBlock
        {
            Text = initial.ToString("F1"),
            VerticalAlignment = VerticalAlignment.Center,
            Margin = new Thickness(5)
        };
        Grid.SetRow(valueBlock, row);
        Grid.SetColumn(valueBlock, 2);
        grid.Children.Add(valueBlock);

        slider.ValueChanged += (s, e) =>
        {
            valueBlock.Text = e.NewValue.ToString("F1");
            onChange(e.NewValue);
        };
    }

    private void Btn3DTransforms_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating GPU-accelerated 3D transforms...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "3D Transforms (GPU-Accelerated)",
            FontSize = 18,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 10)
        });

        // 3D Viewport
        var viewport = new Viewport3D
        {
            Width = 600,
            Height = 400,
            Margin = new Thickness(0, 10, 0, 10)
        };

        // Camera
        var camera = new PerspectiveCamera
        {
            Position = new Point3D(0, 0, 5),
            LookDirection = new Vector3D(0, 0, -5),
            UpDirection = new Vector3D(0, 1, 0),
            FieldOfView = 60
        };
        viewport.Camera = camera;

        // Light
        var modelGroup = new Model3DGroup();
        modelGroup.Children.Add(new DirectionalLight(Colors.White, new Vector3D(-1, -1, -1)));
        modelGroup.Children.Add(new AmbientLight(Colors.Gray));

        // Create 3D cube
        var cube = Create3DCube();
        modelGroup.Children.Add(cube);

        var modelVisual = new ModelVisual3D { Content = modelGroup };
        viewport.Children.Add(modelVisual);

        panel.Children.Add(viewport);

        // Animation controls
        var controlBox = new GroupBox
        {
            Header = "3D Animation Controls"
        };

        var controlStack = new StackPanel();

        var startButton = new Button
        {
            Content = "Start Rotation",
            Margin = new Thickness(5)
        };

        var stopButton = new Button
        {
            Content = "Stop Rotation",
            Margin = new Thickness(5)
        };

        var transform = new RotateTransform3D
        {
            Rotation = new AxisAngleRotation3D(new Vector3D(1, 1, 0), 0)
        };
        cube.Transform = transform;

        DispatcherTimer? timer = null;

        startButton.Click += (s, e) =>
        {
            timer?.Stop();
            timer = new DispatcherTimer { Interval = TimeSpan.FromMilliseconds(16) };
            timer.Tick += (s, e) =>
            {
                var rotation = (AxisAngleRotation3D)transform.Rotation;
                rotation.Angle = (rotation.Angle + 2) % 360;
            };
            timer.Start();
        };

        stopButton.Click += (s, e) =>
        {
            timer?.Stop();
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
            Header = "3D Transform Information"
        };

        infoBox.Content = new TextBlock
        {
            Text = "3D Transforms are fully GPU-accelerated:\n\n" +
                   "• Rendered on GPU for smooth performance\n" +
                   "• Use Viewport3D for 3D scenes\n" +
                   "• Apply to 2D elements with LayoutTransform3D\n" +
                   "• Perfect for card flips, rotations, perspectives\n" +
                   "• No layout recalculation during transform\n\n" +
                   "Start rotation to see GPU-accelerated 3D in action!",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(infoBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        TxtStatus.Text = "3D transforms demonstration - click Start Rotation";
    }

    private GeometryModel3D Create3DCube()
    {
        var meshBuilder = new MeshGeometry3D();

        // Vertices for a cube
        meshBuilder.Positions.Add(new Point3D(-1, -1, -1));
        meshBuilder.Positions.Add(new Point3D(1, -1, -1));
        meshBuilder.Positions.Add(new Point3D(1, 1, -1));
        meshBuilder.Positions.Add(new Point3D(-1, 1, -1));
        meshBuilder.Positions.Add(new Point3D(-1, -1, 1));
        meshBuilder.Positions.Add(new Point3D(1, -1, 1));
        meshBuilder.Positions.Add(new Point3D(1, 1, 1));
        meshBuilder.Positions.Add(new Point3D(-1, 1, 1));

        // Triangle indices
        int[] indices = { 0,1,2, 0,2,3, 1,5,6, 1,6,2, 5,4,7, 5,7,6, 4,0,3, 4,3,7, 3,2,6, 3,6,7, 4,5,1, 4,1,0 };
        foreach (var i in indices)
            meshBuilder.TriangleIndices.Add(i);

        var material = new DiffuseMaterial(new SolidColorBrush(Colors.CornflowerBlue));

        return new GeometryModel3D(meshBuilder, material);
    }

    private void BtnPerformanceTest_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Running GPU performance test...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "GPU Performance Test",
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

        var particles = new List<Ellipse>();
        var velocities = new List<Point>();

        // Create many particles with effects
        for (int i = 0; i < 200; i++)
        {
            var particle = new Ellipse
            {
                Width = 10,
                Height = 10,
                Fill = new SolidColorBrush(Color.FromRgb(
                    (byte)Random.Shared.Next(100, 255),
                    (byte)Random.Shared.Next(100, 255),
                    (byte)Random.Shared.Next(100, 255)
                )),
                Effect = new DropShadowEffect
                {
                    BlurRadius = 5,
                    ShadowDepth = 2,
                    Color = Colors.White,
                    Opacity = 0.8
                }
            };

            Canvas.SetLeft(particle, Random.Shared.Next(0, 790));
            Canvas.SetTop(particle, Random.Shared.Next(0, 490));

            particles.Add(particle);
            velocities.Add(new Point(
                Random.Shared.Next(-5, 6),
                Random.Shared.Next(-5, 6)
            ));

            canvas.Children.Add(particle);
        }

        panel.Children.Add(canvas);

        // Start animation
        var timer = new DispatcherTimer { Interval = TimeSpan.FromMilliseconds(16) };
        timer.Tick += (s, e) =>
        {
            for (int i = 0; i < particles.Count; i++)
            {
                double left = Canvas.GetLeft(particles[i]) + velocities[i].X;
                double top = Canvas.GetTop(particles[i]) + velocities[i].Y;

                if (left <= 0 || left >= 790)
                    velocities[i] = new Point(-velocities[i].X, velocities[i].Y);
                if (top <= 0 || top >= 490)
                    velocities[i] = new Point(velocities[i].X, -velocities[i].Y);

                Canvas.SetLeft(particles[i], left);
                Canvas.SetTop(particles[i], top);
            }
        };
        timer.Start();

        // Info
        var infoBox = new GroupBox
        {
            Header = "Performance Test Information",
            Margin = new Thickness(0, 20, 0, 0)
        };

        infoBox.Content = new TextBlock
        {
            Text = $"Rendering 200 particles with GPU effects:\n\n" +
                   $"• Each particle has DropShadowEffect\n" +
                   $"• Animated at 60 FPS\n" +
                   $"• GPU-accelerated rendering\n" +
                   $"• Watch FPS counter on right panel\n\n" +
                   $"On Tier 2 hardware, this should maintain 60 FPS.\n" +
                   $"On lower tiers, performance may degrade.",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(infoBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        TxtElements.Text = "200";
        TxtRenderCalls.Text = "Continuous";
        TxtStatus.Text = "GPU performance test running - watch FPS counter";
    }

    private void BtnClearDemo_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demo cleared - Ready for next demonstration";
    }

    private void ClearDemo()
    {
        VisualizationContainer.Child = null;
        TxtElements.Text = "0";
        TxtRenderCalls.Text = "0";
    }

    protected override void OnClosed(EventArgs e)
    {
        _fpsTimer?.Stop();
        base.OnClosed(e);
    }
}
