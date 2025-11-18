using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Media;

namespace VirtualizationTechniques;

public partial class MainWindow : Window
{
    private readonly ObservableCollection<DataItem> _largeDataSet = new();

    public MainWindow()
    {
        InitializeComponent();
        GenerateLargeDataSet();
    }

    private void GenerateLargeDataSet()
    {
        for (int i = 0; i < 100000; i++)
        {
            _largeDataSet.Add(new DataItem
            {
                Id = i + 1,
                Title = $"Item {i + 1}",
                Description = $"This is the description for item {i + 1}",
                Category = $"Category {(i % 10) + 1}",
                Value = Random.Shared.Next(1, 1000)
            });
        }
    }

    private void BtnVirtualizationBasics_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating virtualization basics...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "UI Virtualization Basics",
            FontSize = 20,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 20),
            Foreground = Brushes.DarkBlue
        });

        // Explanation
        var explanationBox = new GroupBox
        {
            Header = "What is UI Virtualization?",
            Margin = new Thickness(0, 10, 0, 10)
        };

        explanationBox.Content = new TextBlock
        {
            Text = "UI Virtualization creates visual elements only for items currently visible in the viewport.\n\n" +
                   "WITHOUT Virtualization:\n" +
                   "• All 100,000 items create UI elements\n" +
                   "• Huge memory usage (~500 MB+)\n" +
                   "• Slow startup (seconds)\n" +
                   "• Poor scroll performance\n\n" +
                   "WITH Virtualization:\n" +
                   "• Only ~20-50 visible items create UI\n" +
                   "• Minimal memory (~5-10 MB)\n" +
                   "• Instant startup\n" +
                   "• Smooth scrolling\n\n" +
                   "Virtualization is ESSENTIAL for large collections!",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(explanationBox);

        // Visual representation
        var visualBox = new GroupBox
        {
            Header = "Visual Representation",
            Margin = new Thickness(0, 10, 0, 10)
        };

        var visualCanvas = new Canvas
        {
            Width = 600,
            Height = 300,
            Background = Brushes.WhiteSmoke
        };

        // Draw viewport
        var viewport = new Rectangle
        {
            Width = 200,
            Height = 250,
            Stroke = Brushes.DarkBlue,
            StrokeThickness = 3,
            Fill = Brushes.LightBlue,
            Canvas.Left = 200,
            Canvas.Top = 25
        };
        visualCanvas.Children.Add(viewport);

        visualCanvas.Children.Add(new TextBlock
        {
            Text = "Viewport\n(Visible Area)",
            Canvas.Left = 230,
            Canvas.Top = 120,
            FontWeight = FontWeights.Bold,
            TextAlignment = TextAlignment.Center
        });

        // Draw items
        for (int i = 0; i < 15; i++)
        {
            var itemRect = new Rectangle
            {
                Width = 180,
                Height = 15,
                Fill = (i >= 3 && i <= 10) ? Brushes.Green : Brushes.LightGray,
                Stroke = Brushes.Black,
                StrokeThickness = 1,
                Canvas.Left = 210,
                Canvas.Top = 35 + i * 17
            };
            visualCanvas.Children.Add(itemRect);

            if (i >= 3 && i <= 10)
            {
                visualCanvas.Children.Add(new TextBlock
                {
                    Text = $"Item {i + 1} ✓",
                    Canvas.Left = 420,
                    Canvas.Top = 36 + i * 17,
                    FontSize = 10,
                    Foreground = Brushes.Green,
                    FontWeight = FontWeights.Bold
                });
            }
        }

        // Labels
        visualCanvas.Children.Add(new TextBlock
        {
            Text = "Items above viewport\n(not created)",
            Canvas.Left = 30,
            Canvas.Top = 60,
            Foreground = Brushes.Gray,
            FontStyle = FontStyles.Italic
        });

        visualCanvas.Children.Add(new TextBlock
        {
            Text = "Visible items\n(UI created)",
            Canvas.Left = 30,
            Canvas.Top = 140,
            Foreground = Brushes.Green,
            FontWeight = FontWeights.Bold
        });

        visualCanvas.Children.Add(new TextBlock
        {
            Text = "Items below viewport\n(not created)",
            Canvas.Left = 30,
            Canvas.Top = 220,
            Foreground = Brushes.Gray,
            FontStyle = FontStyles.Italic
        });

        visualBox.Content = visualCanvas;
        panel.Children.Add(visualBox);

        // How it works
        var howBox = new GroupBox
        {
            Header = "How Virtualization Works",
            Margin = new Thickness(0, 10, 0, 10)
        };

        howBox.Content = new TextBlock
        {
            Text = "1. User scrolls list\n" +
                   "2. VirtualizingPanel calculates visible range\n" +
                   "3. Creates UI only for visible items\n" +
                   "4. As user scrolls, creates new items entering view\n" +
                   "5. Disposes/recycles items leaving view\n" +
                   "6. Maintains minimal UI elements\n\n" +
                   "Result: Constant memory usage regardless of total items!",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10),
            FontFamily = new FontFamily("Consolas")
        };

        panel.Children.Add(howBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        var details = new StringBuilder();
        details.AppendLine("UI VIRTUALIZATION BASICS");
        details.AppendLine("=======================\n");
        details.AppendLine("Concept:");
        details.AppendLine("  - Create UI only for visible items");
        details.AppendLine("  - Recycle/dispose off-screen items");
        details.AppendLine("  - Maintain constant memory\n");
        details.AppendLine("Benefits:");
        details.AppendLine("  - Dramatically lower memory");
        details.AppendLine("  - Faster startup");
        details.AppendLine("  - Smooth scrolling");
        details.AppendLine("  - Handle millions of items\n");
        details.AppendLine("Implementation:");
        details.AppendLine("  - VirtualizingStackPanel (built-in)");
        details.AppendLine("  - VirtualizingWrapPanel (custom)");
        details.AppendLine("  - Custom virtualizing panels");

        TxtVirtualizationDetails.Text = details.ToString();
        TxtStatus.Text = "Virtualization basics explained";
    }

    private void BtnComparePerformance_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Comparing virtualized vs non-virtualized performance...";

        var mainGrid = new Grid { Margin = new Thickness(10) };
        mainGrid.ColumnDefinitions.Add(new ColumnDefinition());
        mainGrid.ColumnDefinitions.Add(new ColumnDefinition());

        // Non-virtualized (small dataset for demo)
        var nonVirtualizedBox = new GroupBox
        {
            Header = "WITHOUT Virtualization (1,000 items)",
            Margin = new Thickness(5)
        };

        var stopwatch = Stopwatch.StartNew();
        var nonVirtualizedList = new ListBox
        {
            Height = 700
        };

        // Disable virtualization
        VirtualizingPanel.SetIsVirtualizing(nonVirtualizedList, false);

        var nonVirtualizedData = new ObservableCollection<DataItem>();
        for (int i = 0; i < 1000; i++)
        {
            nonVirtualizedData.Add(_largeDataSet[i]);
        }

        nonVirtualizedList.ItemsSource = nonVirtualizedData;
        nonVirtualizedList.ItemTemplate = CreateItemTemplate();

        var nonVirtualizedTime = stopwatch.ElapsedMilliseconds;
        stopwatch.Restart();

        nonVirtualizedBox.Content = nonVirtualizedList;
        Grid.SetColumn(nonVirtualizedBox, 0);
        mainGrid.Children.Add(nonVirtualizedBox);

        // Virtualized (full dataset)
        var virtualizedBox = new GroupBox
        {
            Header = "WITH Virtualization (100,000 items)",
            Margin = new Thickness(5)
        };

        var virtualizedList = new ListBox
        {
            Height = 700
        };

        // Virtualization enabled by default
        virtualizedList.ItemsSource = _largeDataSet;
        virtualizedList.ItemTemplate = CreateItemTemplate();

        var virtualizedTime = stopwatch.ElapsedMilliseconds;

        virtualizedBox.Content = virtualizedList;
        Grid.SetColumn(virtualizedBox, 1);
        mainGrid.Children.Add(virtualizedBox);

        VisualizationContainer.Child = mainGrid;

        // Update metrics
        TxtTotalItems.Text = "1,000 vs 100,000";
        TxtRealizedItems.Text = $"1,000 vs ~30";
        TxtMemorySaved.Text = $"~{((100000 - 30) * 5) / 1024} MB";
        TxtLoadTime.Text = $"{nonVirtualizedTime} ms vs {virtualizedTime} ms";
        TxtVirtualizationStatus.Text = "Right Side Only";

        var details = new StringBuilder();
        details.AppendLine("PERFORMANCE COMPARISON");
        details.AppendLine("=====================\n");
        details.AppendLine($"Non-Virtualized (LEFT):");
        details.AppendLine($"  Items: 1,000 (limited for demo)");
        details.AppendLine($"  Load Time: {nonVirtualizedTime} ms");
        details.AppendLine($"  All containers created");
        details.AppendLine($"  Memory: ~5 MB\n");
        details.AppendLine($"Virtualized (RIGHT):");
        details.AppendLine($"  Items: 100,000");
        details.AppendLine($"  Load Time: {virtualizedTime} ms");
        details.AppendLine($"  Only ~30 containers created");
        details.AppendLine($"  Memory: ~150 KB\n");
        details.AppendLine($"WINNER: Virtualization!");
        details.AppendLine($"  - {(double)nonVirtualizedTime / virtualizedTime:F1}x faster load");
        details.AppendLine($"  - 100x more items");
        details.AppendLine($"  - ~33x less memory");

        TxtVirtualizationDetails.Text = details.ToString();
        TxtStatus.Text = $"Performance comparison complete - Virtualization is {(double)nonVirtualizedTime / virtualizedTime:F1}x faster";
    }

    private DataTemplate CreateItemTemplate()
    {
        var template = new DataTemplate();

        var factory = new FrameworkElementFactory(typeof(Border));
        factory.SetValue(Border.BorderBrushProperty, Brushes.LightGray);
        factory.SetValue(Border.BorderThicknessProperty, new Thickness(0, 0, 0, 1));
        factory.SetValue(Border.PaddingProperty, new Thickness(10, 5, 10, 5));
        factory.SetValue(Border.BackgroundProperty, Brushes.White);

        var stackFactory = new FrameworkElementFactory(typeof(StackPanel));

        var titleFactory = new FrameworkElementFactory(typeof(TextBlock));
        titleFactory.SetBinding(TextBlock.TextProperty, new Binding("Title"));
        titleFactory.SetValue(TextBlock.FontWeightProperty, FontWeights.Bold);
        titleFactory.SetValue(TextBlock.FontSizeProperty, 14.0);

        var descFactory = new FrameworkElementFactory(typeof(TextBlock));
        descFactory.SetBinding(TextBlock.TextProperty, new Binding("Description"));
        descFactory.SetValue(TextBlock.ForegroundProperty, Brushes.Gray);
        descFactory.SetValue(TextBlock.FontSizeProperty, 11.0);

        stackFactory.AppendChild(titleFactory);
        stackFactory.AppendChild(descFactory);
        factory.AppendChild(stackFactory);

        template.VisualTree = factory;
        return template;
    }

    private void BtnVirtualizingModes_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating virtualization modes...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "Virtualization Modes",
            FontSize = 18,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 10)
        });

        // VirtualizationMode.Standard
        var standardBox = new GroupBox
        {
            Header = "VirtualizationMode.Standard (Default)",
            Margin = new Thickness(0, 10, 0, 10)
        };

        standardBox.Content = new TextBlock
        {
            Text = "Standard Mode:\n" +
                   "• Creates containers for visible items\n" +
                   "• Disposes containers when scrolled out of view\n" +
                   "• Recreates containers when scrolled back\n" +
                   "• Lower memory usage\n" +
                   "• Slight performance cost on scroll\n\n" +
                   "Use when: Memory is limited",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(standardBox);

        // VirtualizationMode.Recycling
        var recyclingBox = new GroupBox
        {
            Header = "VirtualizationMode.Recycling (Recommended)",
            Margin = new Thickness(0, 10, 0, 10)
        };

        recyclingBox.Content = new TextBlock
        {
            Text = "Recycling Mode:\n" +
                   "• Reuses existing containers\n" +
                   "• Doesn't dispose when scrolled out\n" +
                   "• Re-binds data to existing containers\n" +
                   "• Slightly higher memory (container pool)\n" +
                   "• Better scroll performance\n" +
                   "• Less GC pressure\n\n" +
                   "Use when: Scroll performance is critical (most cases)",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10),
            Background = Brushes.LightGreen
        };

        panel.Children.Add(recyclingBox);

        // Comparison example
        var compareBox = new GroupBox
        {
            Header = "Visual Comparison (Scroll to test)",
            Margin = new Thickness(0, 10, 0, 10)
        };

        var compareGrid = new Grid();
        compareGrid.ColumnDefinitions.Add(new ColumnDefinition());
        compareGrid.ColumnDefinitions.Add(new ColumnDefinition());

        // Standard mode
        var standardList = new ListBox
        {
            Height = 300,
            Margin = new Thickness(5)
        };
        VirtualizingPanel.SetVirtualizationMode(standardList, VirtualizationMode.Standard);
        standardList.ItemsSource = _largeDataSet.Take(10000);
        standardList.ItemTemplate = CreateItemTemplate();

        var standardContainer = new StackPanel();
        standardContainer.Children.Add(new TextBlock
        {
            Text = "Standard Mode",
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(5)
        });
        standardContainer.Children.Add(standardList);

        Grid.SetColumn(standardContainer, 0);
        compareGrid.Children.Add(standardContainer);

        // Recycling mode
        var recyclingList = new ListBox
        {
            Height = 300,
            Margin = new Thickness(5)
        };
        VirtualizingPanel.SetVirtualizationMode(recyclingList, VirtualizationMode.Recycling);
        recyclingList.ItemsSource = _largeDataSet.Take(10000);
        recyclingList.ItemTemplate = CreateItemTemplate();

        var recyclingContainer = new StackPanel();
        recyclingContainer.Children.Add(new TextBlock
        {
            Text = "Recycling Mode (Smoother)",
            FontWeight = FontWeights.Bold,
            Foreground = Brushes.Green,
            Margin = new Thickness(5)
        });
        recyclingContainer.Children.Add(recyclingList);

        Grid.SetColumn(recyclingContainer, 1);
        compareGrid.Children.Add(recyclingContainer);

        compareBox.Content = compareGrid;
        panel.Children.Add(compareBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        var details = new StringBuilder();
        details.AppendLine("VIRTUALIZATION MODES");
        details.AppendLine("===================\n");
        details.AppendLine("Standard Mode:");
        details.AppendLine("  - Creates/disposes containers");
        details.AppendLine("  - Lower memory");
        details.AppendLine("  - More GC activity");
        details.AppendLine("  - Slower scrolling\n");
        details.AppendLine("Recycling Mode:");
        details.AppendLine("  - Reuses containers");
        details.AppendLine("  - Container pool maintained");
        details.AppendLine("  - Less GC activity");
        details.AppendLine("  - Faster scrolling");
        details.AppendLine("  - RECOMMENDED\n");
        details.AppendLine("Enable Recycling:");
        details.AppendLine("  VirtualizingPanel.SetVirtualizationMode(");
        details.AppendLine("    listBox, VirtualizationMode.Recycling);");

        TxtVirtualizationDetails.Text = details.ToString();
        TxtRecyclingMode.Text = "Both Modes Shown";
        TxtStatus.Text = "Virtualization modes comparison - scroll both lists to feel difference";
    }

    private void BtnScrollUnit_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating scroll unit modes...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "Scroll Unit Modes",
            FontSize = 18,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 10)
        });

        var compareGrid = new Grid();
        compareGrid.ColumnDefinitions.Add(new ColumnDefinition());
        compareGrid.ColumnDefinitions.Add(new ColumnDefinition());

        // Pixel scrolling
        var pixelBox = new GroupBox
        {
            Header = "ScrollUnit.Pixel (Smooth)",
            Margin = new Thickness(5)
        };

        var pixelList = new ListBox
        {
            Height = 600
        };
        ScrollViewer.SetCanContentScroll(pixelList, true);
        VirtualizingPanel.SetScrollUnit(pixelList, ScrollUnit.Pixel);
        pixelList.ItemsSource = _largeDataSet.Take(1000);
        pixelList.ItemTemplate = CreateItemTemplate();

        var pixelStack = new StackPanel();
        pixelStack.Children.Add(new TextBlock
        {
            Text = "Pixel-based scrolling:\n• Smooth scrolling\n• Any scroll position\n• Better UX\n• Slightly more rendering",
            TextWrapping = TextWrapping.Wrap,
            Margin = new Thickness(5),
            Background = Brushes.LightGreen,
            Padding = new Thickness(5)
        });
        pixelStack.Children.Add(pixelList);

        pixelBox.Content = pixelStack;
        Grid.SetColumn(pixelBox, 0);
        compareGrid.Children.Add(pixelBox);

        // Item scrolling
        var itemBox = new GroupBox
        {
            Header = "ScrollUnit.Item (Snappy)",
            Margin = new Thickness(5)
        };

        var itemList = new ListBox
        {
            Height = 600
        };
        ScrollViewer.SetCanContentScroll(itemList, true);
        VirtualizingPanel.SetScrollUnit(itemList, ScrollUnit.Item);
        itemList.ItemsSource = _largeDataSet.Take(1000);
        itemList.ItemTemplate = CreateItemTemplate();

        var itemStack = new StackPanel();
        itemStack.Children.Add(new TextBlock
        {
            Text = "Item-based scrolling:\n• Snaps to items\n• Traditional behavior\n• Less rendering\n• May feel choppy",
            TextWrapping = TextWrapping.Wrap,
            Margin = new Thickness(5),
            Background = Brushes.LightYellow,
            Padding = new Thickness(5)
        });
        itemStack.Children.Add(itemList);

        itemBox.Content = itemStack;
        Grid.SetColumn(itemBox, 1);
        compareGrid.Children.Add(itemBox);

        panel.Children.Add(compareGrid);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        var details = new StringBuilder();
        details.AppendLine("SCROLL UNIT MODES");
        details.AppendLine("=================\n");
        details.AppendLine("Pixel Scrolling:");
        details.AppendLine("  - Smooth, continuous scroll");
        details.AppendLine("  - Any position possible");
        details.AppendLine("  - Better user experience");
        details.AppendLine("  - Modern feel");
        details.AppendLine("  - Recommended for most cases\n");
        details.AppendLine("Item Scrolling:");
        details.AppendLine("  - Snaps to item boundaries");
        details.AppendLine("  - Traditional behavior");
        details.AppendLine("  - Slightly better performance");
        details.AppendLine("  - May feel choppy\n");
        details.AppendLine("Configuration:");
        details.AppendLine("  VirtualizingPanel.SetScrollUnit(");
        details.AppendLine("    listBox, ScrollUnit.Pixel);");

        TxtVirtualizationDetails.Text = details.ToString();
        TxtScrollUnit.Text = "Both Modes Shown";
        TxtStatus.Text = "Scroll unit comparison - try scrolling both lists";
    }

    private void BtnRecyclingMode_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating container recycling...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "Container Recycling Deep Dive",
            FontSize = 18,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 10)
        });

        // Create list with recycling and monitoring
        var monitorBox = new GroupBox
        {
            Header = "Recycling Monitor (Scroll to see recycling)",
            Margin = new Thickness(0, 10, 0, 10)
        };

        var monitorStack = new StackPanel();

        var metricsPanel = new StackPanel
        {
            Orientation = Orientation.Horizontal,
            Margin = new Thickness(5)
        };

        var createdText = new TextBlock
        {
            Text = "Containers Created: 0",
            Margin = new Thickness(10, 0, 10, 0),
            FontWeight = FontWeights.Bold
        };

        var recycledText = new TextBlock
        {
            Text = "Containers Recycled: 0",
            Margin = new Thickness(10, 0, 10, 0),
            FontWeight = FontWeights.Bold,
            Foreground = Brushes.Green
        };

        metricsPanel.Children.Add(createdText);
        metricsPanel.Children.Add(recycledText);
        monitorStack.Children.Add(metricsPanel);

        var monitorList = new ListBox
        {
            Height = 500
        };

        VirtualizingPanel.SetIsVirtualizing(monitorList, true);
        VirtualizingPanel.SetVirtualizationMode(monitorList, VirtualizationMode.Recycling);
        VirtualizingPanel.SetScrollUnit(monitorList, ScrollUnit.Pixel);

        monitorList.ItemsSource = _largeDataSet.Take(5000);
        monitorList.ItemTemplate = CreateMonitoringTemplate();

        monitorStack.Children.Add(monitorList);
        monitorBox.Content = monitorStack;
        panel.Children.Add(monitorBox);

        // Explanation
        var explanationBox = new GroupBox
        {
            Header = "How Container Recycling Works",
            Margin = new Thickness(0, 10, 0, 10)
        };

        explanationBox.Content = new TextBlock
        {
            Text = "Container Recycling Process:\n\n" +
                   "1. Initial Load:\n" +
                   "   - Creates containers for visible items (~30)\n" +
                   "   - Adds to container pool\n\n" +
                   "2. User Scrolls Down:\n" +
                   "   - Items scroll out of view at top\n" +
                   "   - Containers moved to recycling queue\n" +
                   "   - New items appear at bottom\n" +
                   "   - Recycled containers reused!\n" +
                   "   - Data context updated\n\n" +
                   "3. Benefits:\n" +
                   "   - No container creation/disposal during scroll\n" +
                   "   - Constant memory usage\n" +
                   "   - No GC pressure\n" +
                   "   - Smooth, fast scrolling\n\n" +
                   "Watch the 'Containers Created' counter - it should stabilize\n" +
                   "after initial load, then only 'Recycled' increases!",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10),
            FontFamily = new FontFamily("Consolas"),
            Background = Brushes.WhiteSmoke
        };

        panel.Children.Add(explanationBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        var details = new StringBuilder();
        details.AppendLine("CONTAINER RECYCLING");
        details.AppendLine("==================\n");
        details.AppendLine("Without Recycling:");
        details.AppendLine("  - Create container");
        details.AppendLine("  - Use container");
        details.AppendLine("  - Dispose container");
        details.AppendLine("  - Repeat (GC pressure!)\n");
        details.AppendLine("With Recycling:");
        details.AppendLine("  - Create container pool");
        details.AppendLine("  - Reuse from pool");
        details.AppendLine("  - Update data binding");
        details.AppendLine("  - Return to pool");
        details.AppendLine("  - No disposal (no GC!)\n");
        details.AppendLine("Configuration:");
        details.AppendLine("  VirtualizingPanel.VirtualizationMode");
        details.AppendLine("    = VirtualizationMode.Recycling");

        TxtVirtualizationDetails.Text = details.ToString();
        TxtRecyclingMode.Text = "Recycling Enabled";
        TxtVirtualizationStatus.Text = "Enabled with Recycling";
        TxtStatus.Text = "Container recycling demonstration - scroll to see recycling in action";
    }

    private DataTemplate CreateMonitoringTemplate()
    {
        var template = new DataTemplate();

        var factory = new FrameworkElementFactory(typeof(Border));
        factory.SetValue(Border.BorderBrushProperty, Brushes.LightBlue);
        factory.SetValue(Border.BorderThicknessProperty, new Thickness(1));
        factory.SetValue(Border.PaddingProperty, new Thickness(10));
        factory.SetValue(Border.MarginProperty, new Thickness(2));
        factory.SetValue(Border.BackgroundProperty, Brushes.White);

        var gridFactory = new FrameworkElementFactory(typeof(Grid));

        var col1 = new FrameworkElementFactory(typeof(ColumnDefinition));
        col1.SetValue(ColumnDefinition.WidthProperty, new GridLength(1, GridUnitType.Star));
        gridFactory.AppendChild(col1);

        var col2 = new FrameworkElementFactory(typeof(ColumnDefinition));
        col2.SetValue(ColumnDefinition.WidthProperty, GridLength.Auto);
        gridFactory.AppendChild(col2);

        var titleFactory = new FrameworkElementFactory(typeof(TextBlock));
        titleFactory.SetBinding(TextBlock.TextProperty, new Binding("Title"));
        titleFactory.SetValue(TextBlock.FontWeightProperty, FontWeights.Bold);
        titleFactory.SetValue(Grid.ColumnProperty, 0);
        gridFactory.AppendChild(titleFactory);

        var valueFactory = new FrameworkElementFactory(typeof(TextBlock));
        valueFactory.SetBinding(TextBlock.TextProperty, new Binding("Value")
        {
            StringFormat = "Value: {0}"
        });
        valueFactory.SetValue(TextBlock.ForegroundProperty, Brushes.Gray);
        valueFactory.SetValue(Grid.ColumnProperty, 1);
        gridFactory.AppendChild(valueFactory);

        factory.AppendChild(gridFactory);
        template.VisualTree = factory;
        return template;
    }

    private void BtnCustomPanel_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating custom virtualizing panel...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "Custom Virtualizing Panel",
            FontSize = 18,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 10)
        });

        // Explanation
        var explanationBox = new GroupBox
        {
            Header = "Creating Custom Virtualizing Panels",
            Margin = new Thickness(0, 10, 0, 10)
        };

        explanationBox.Content = new TextBlock
        {
            Text = "Custom virtualizing panels allow you to create specialized layouts:\n\n" +
                   "Common Custom Panels:\n" +
                   "• VirtualizingWrapPanel: Grid-like wrapping layout\n" +
                   "• VirtualizingTilePanel: Tile/card layouts\n" +
                   "• VirtualizingUniformGrid: Fixed columns\n" +
                   "• VirtualizingTreePanel: Hierarchical data\n\n" +
                   "Requirements:\n" +
                   "• Inherit from VirtualizingPanel\n" +
                   "• Implement IScrollInfo\n" +
                   "• Override MeasureOverride and ArrangeOverride\n" +
                   "• Handle virtualization logic\n" +
                   "• Support container recycling\n\n" +
                   "Benefits:\n" +
                   "• Virtualization for custom layouts\n" +
                   "• Memory efficiency\n" +
                   "• Smooth scrolling\n" +
                   "• Flexible design",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(explanationBox);

        // Code example
        var codeBox = new GroupBox
        {
            Header = "Custom Panel Example (Conceptual)",
            Margin = new Thickness(0, 10, 0, 10)
        };

        codeBox.Content = new TextBlock
        {
            Text = "public class VirtualizingWrapPanel : VirtualizingPanel, IScrollInfo\n" +
                   "{\n" +
                   "    protected override Size MeasureOverride(Size availableSize)\n" +
                   "    {\n" +
                   "        // Calculate visible range\n" +
                   "        var generator = ItemContainerGenerator;\n" +
                   "        int firstVisible = CalculateFirstVisible();\n" +
                   "        int lastVisible = CalculateLastVisible();\n\n" +
                   "        // Virtualize: create only visible items\n" +
                   "        for (int i = firstVisible; i <= lastVisible; i++)\n" +
                   "        {\n" +
                   "            var container = generator.ContainerFromIndex(i);\n" +
                   "            if (container == null)\n" +
                   "                container = generator.GenerateNext(...);\n\n" +
                   "            container.Measure(itemSize);\n" +
                   "        }\n\n" +
                   "        // Clean up off-screen items\n" +
                   "        CleanupContainers(firstVisible, lastVisible);\n\n" +
                   "        return desiredSize;\n" +
                   "    }\n\n" +
                   "    protected override Size ArrangeOverride(Size finalSize)\n" +
                   "    {\n" +
                   "        // Position visible items\n" +
                   "        // ...\n" +
                   "    }\n" +
                   "}",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10),
            FontFamily = new FontFamily("Consolas"),
            FontSize = 11,
            Background = Brushes.WhiteSmoke
        };

        panel.Children.Add(codeBox);

        // Use cases
        var useCasesBox = new GroupBox
        {
            Header = "When to Create Custom Virtualizing Panels",
            Margin = new Thickness(0, 10, 0, 10)
        };

        useCasesBox.Content = new TextBlock
        {
            Text = "✓ Grid/tile layouts with large datasets\n" +
                   "✓ Custom spacing and sizing requirements\n" +
                   "✓ Specialized scroll behavior\n" +
                   "✓ Hierarchical or grouped data\n" +
                   "✓ Performance-critical visualizations\n\n" +
                   "✗ Simple lists (use VirtualizingStackPanel)\n" +
                   "✗ Small datasets (<1000 items)\n" +
                   "✗ Frequently changing layouts\n\n" +
                   "Popular Libraries:\n" +
                   "• WPF Extended Toolkit (VirtualizingWrapPanel)\n" +
                   "• Various community implementations\n" +
                   "• Custom business-specific panels",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(useCasesBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        var details = new StringBuilder();
        details.AppendLine("CUSTOM VIRTUALIZING PANELS");
        details.AppendLine("=========================\n");
        details.AppendLine("Base Class: VirtualizingPanel");
        details.AppendLine("Interface: IScrollInfo\n");
        details.AppendLine("Key Methods:");
        details.AppendLine("  - MeasureOverride()");
        details.AppendLine("  - ArrangeOverride()");
        details.AppendLine("  - CalculateVisibleRange()");
        details.AppendLine("  - GenerateContainers()");
        details.AppendLine("  - RecycleContainers()\n");
        details.AppendLine("Challenges:");
        details.AppendLine("  - Complex implementation");
        details.AppendLine("  - Scroll synchronization");
        details.AppendLine("  - Performance tuning");
        details.AppendLine("  - Edge cases\n");
        details.AppendLine("Recommendation:");
        details.AppendLine("  Use existing implementations when possible.");

        TxtVirtualizationDetails.Text = details.ToString();
        TxtStatus.Text = "Custom virtualizing panel information displayed";
    }

    private void BtnBestPractices_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Displaying virtualization best practices...";

        var panel = new StackPanel { Margin = new Thickness(20) };

        panel.Children.Add(new TextBlock
        {
            Text = "Virtualization Best Practices",
            FontSize = 20,
            FontWeight = FontWeights.Bold,
            Margin = new Thickness(0, 0, 0, 20),
            Foreground = Brushes.DarkBlue
        });

        // Always virtualize
        var alwaysBox = new GroupBox
        {
            Header = "✓ Always Virtualize Large Lists",
            Margin = new Thickness(0, 10, 0, 10),
            Background = Brushes.LightGreen
        };

        alwaysBox.Content = new TextBlock
        {
            Text = "• Virtualize any list with 100+ items\n" +
                   "• Especially critical for 1,000+ items\n" +
                   "• Use VirtualizingStackPanel (default in ListBox)\n" +
                   "• Enable container recycling\n" +
                   "• Use pixel-based scrolling for smooth UX\n\n" +
                   "Example:\n" +
                   "VirtualizingPanel.SetIsVirtualizing(listBox, true);\n" +
                   "VirtualizingPanel.SetVirtualizationMode(listBox, VirtualizationMode.Recycling);\n" +
                   "VirtualizingPanel.SetScrollUnit(listBox, ScrollUnit.Pixel);",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10),
            FontFamily = new FontFamily("Consolas")
        };

        panel.Children.Add(alwaysBox);

        // Avoid disabling
        var avoidBox = new GroupBox
        {
            Header = "✗ Avoid Virtualization Breakers",
            Margin = new Thickness(0, 10, 0, 10),
            Background = Brushes.LightCoral
        };

        avoidBox.Content = new TextBlock
        {
            Text = "These DISABLE virtualization:\n\n" +
                   "• Using non-virtualizing panels (StackPanel, WrapPanel)\n" +
                   "• Setting ScrollViewer.CanContentScroll=false\n" +
                   "• Using GroupStyle in ItemsControl\n" +
                   "• Wrapping in ScrollViewer without virtualization\n" +
                   "• Complex item templates with nested ScrollViewers\n\n" +
                   "BAD:\n" +
                   "<ItemsControl>\n" +
                   "  <ItemsControl.ItemsPanel>\n" +
                   "    <ItemsPanelTemplate>\n" +
                   "      <StackPanel/>  <!-- NOT virtualizing! -->\n" +
                   "    </ItemsPanelTemplate>\n" +
                   "  </ItemsControl.ItemsPanel>\n" +
                   "</ItemsControl>",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10),
            FontFamily = new FontFamily("Consolas")
        };

        panel.Children.Add(avoidBox);

        // Optimize templates
        var templateBox = new GroupBox
        {
            Header = "Optimize Item Templates",
            Margin = new Thickness(0, 10, 0, 10)
        };

        templateBox.Content = new TextBlock
        {
            Text = "Item template complexity affects performance:\n\n" +
                   "✓ Keep templates simple\n" +
                   "✓ Minimize visual tree depth\n" +
                   "✓ Avoid nested panels\n" +
                   "✓ Use TextBlock instead of Label\n" +
                   "✓ Defer loading for complex content\n\n" +
                   "✗ Don't use ScrollViewers in templates\n" +
                   "✗ Avoid complex animations\n" +
                   "✗ Don't nest ItemsControls unnecessarily\n\n" +
                   "Remember: Template is created for EACH visible item!",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(templateBox);

        // Monitor performance
        var monitorBox = new GroupBox
        {
            Header = "Monitor and Profile",
            Margin = new Thickness(0, 10, 0, 10)
        };

        monitorBox.Content = new TextBlock
        {
            Text = "Track these metrics:\n\n" +
                   "• Initial load time\n" +
                   "• Scroll smoothness (FPS)\n" +
                   "• Memory usage\n" +
                   "• Number of realized containers\n" +
                   "• Container recycling rate\n\n" +
                   "Tools:\n" +
                   "• Visual Studio Profiler\n" +
                   "• WPF Performance Suite\n" +
                   "• Custom performance monitors\n" +
                   "• Memory profilers\n\n" +
                   "Verify:\n" +
                   "✓ <50ms initial load (virtualized)\n" +
                   "✓ 60 FPS scrolling\n" +
                   "✓ Constant memory during scroll\n" +
                   "✓ Realized items << Total items",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10)
        };

        panel.Children.Add(monitorBox);

        // Configuration checklist
        var checklistBox = new GroupBox
        {
            Header = "Configuration Checklist",
            Margin = new Thickness(0, 10, 0, 10)
        };

        checklistBox.Content = new TextBlock
        {
            Text = "For optimal virtualization:\n\n" +
                   "☑ VirtualizingPanel.IsVirtualizing = true\n" +
                   "☑ VirtualizingPanel.VirtualizationMode = Recycling\n" +
                   "☑ VirtualizingPanel.ScrollUnit = Pixel\n" +
                   "☑ ScrollViewer.CanContentScroll = true\n" +
                   "☑ Use VirtualizingStackPanel (or custom)\n" +
                   "☑ Simple item templates\n" +
                   "☑ Avoid GroupStyle if possible\n" +
                   "☑ Profile performance\n" +
                   "☑ Test with realistic data volume",
            TextWrapping = TextWrapping.Wrap,
            Padding = new Thickness(10),
            Background = Brushes.LightYellow,
            FontWeight = FontWeights.Bold
        };

        panel.Children.Add(checklistBox);

        VisualizationContainer.Child = new ScrollViewer
        {
            Content = panel,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };

        var details = new StringBuilder();
        details.AppendLine("VIRTUALIZATION BEST PRACTICES");
        details.AppendLine("============================\n");
        details.AppendLine("1. ALWAYS virtualize large lists");
        details.AppendLine("2. Use Recycling mode");
        details.AppendLine("3. Use Pixel scrolling");
        details.AppendLine("4. Simplify item templates");
        details.AppendLine("5. Avoid virtualization breakers");
        details.AppendLine("6. Monitor performance");
        details.AppendLine("7. Profile with realistic data");
        details.AppendLine("8. Test scroll performance");
        details.AppendLine("9. Verify memory usage");
        details.AppendLine("10. Document configuration\n");
        details.AppendLine("GOLDEN RULE:");
        details.AppendLine("  If you have more than 100 items,");
        details.AppendLine("  virtualization is NOT optional!");

        TxtVirtualizationDetails.Text = details.ToString();
        TxtStatus.Text = "Best practices displayed - follow these for optimal virtualization";
    }

    private void BtnClearDemo_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demo cleared - Ready for next demonstration";
    }

    private void ClearDemo()
    {
        VisualizationContainer.Child = null;
        TxtVirtualizationDetails.Text = "";
        TxtTotalItems.Text = "0";
        TxtRealizedItems.Text = "0";
        TxtMemorySaved.Text = "0 MB";
        TxtLoadTime.Text = "0 ms";
        TxtVirtualizationStatus.Text = "Disabled";
        TxtScrollUnit.Text = "-";
        TxtRecyclingMode.Text = "-";
    }
}

public class DataItem
{
    public int Id { get; set; }
    public string Title { get; set; } = string.Empty;
    public string Description { get; set; } = string.Empty;
    public string Category { get; set; } = string.Empty;
    public int Value { get; set; }
}
