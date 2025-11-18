using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;

namespace VisualTreeOptimization;

public partial class MainWindow : Window
{
    private Stopwatch _stopwatch = new();

    public MainWindow()
    {
        InitializeComponent();
    }

    private void BtnSimpleTree_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating simple visual tree...";

        _stopwatch.Restart();

        // Simple tree with minimal depth
        var stackPanel = new StackPanel { Margin = new Thickness(10) };

        for (int i = 0; i < 50; i++)
        {
            var button = new Button
            {
                Content = $"Button {i + 1}",
                Margin = new Thickness(2),
                Padding = new Thickness(10, 5, 10, 5)
            };
            stackPanel.Children.Add(button);
        }

        VisualizationContainer.Child = stackPanel;

        _stopwatch.Stop();

        AnalyzeVisualTree(stackPanel);
        TxtStatus.Text = $"Simple tree created in {_stopwatch.ElapsedMilliseconds} ms - 50 buttons, minimal nesting";
    }

    private void BtnComplexTree_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating complex (inefficient) visual tree...";

        _stopwatch.Restart();

        // Complex tree with excessive nesting (anti-pattern)
        var outerPanel = new StackPanel { Margin = new Thickness(10) };

        for (int i = 0; i < 20; i++)
        {
            var groupBox = new GroupBox
            {
                Header = $"Group {i + 1}",
                Margin = new Thickness(2)
            };

            var border = new Border
            {
                BorderBrush = Brushes.Gray,
                BorderThickness = new Thickness(1),
                Padding = new Thickness(5)
            };

            var innerPanel = new StackPanel();

            var grid = new Grid();
            grid.ColumnDefinitions.Add(new ColumnDefinition());
            grid.RowDefinitions.Add(new RowDefinition());

            var viewBox = new Viewbox();

            var button = new Button
            {
                Content = $"Nested Button {i + 1}",
                Margin = new Thickness(2)
            };

            // Excessive nesting: GroupBox > Border > StackPanel > Grid > Viewbox > Button
            viewBox.Child = button;
            grid.Children.Add(viewBox);
            innerPanel.Children.Add(grid);
            border.Child = innerPanel;
            groupBox.Content = border;
            outerPanel.Children.Add(groupBox);
        }

        VisualizationContainer.Child = outerPanel;

        _stopwatch.Stop();

        AnalyzeVisualTree(outerPanel);
        TxtStatus.Text = $"Complex tree created in {_stopwatch.ElapsedMilliseconds} ms - Deep nesting (INEFFICIENT)";
    }

    private void BtnOptimizedTree_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating optimized visual tree...";

        _stopwatch.Restart();

        // Optimized tree with flat structure
        var grid = new Grid { Margin = new Thickness(10) };

        // Define columns and rows efficiently
        for (int i = 0; i < 5; i++)
        {
            grid.ColumnDefinitions.Add(new ColumnDefinition());
            grid.RowDefinitions.Add(new RowDefinition { Height = GridLength.Auto });
        }

        int count = 0;
        for (int row = 0; row < 5; row++)
        {
            for (int col = 0; col < 5; col++)
            {
                var border = new Border
                {
                    BorderBrush = Brushes.LightBlue,
                    BorderThickness = new Thickness(1),
                    Margin = new Thickness(2),
                    Padding = new Thickness(10),
                    Background = new SolidColorBrush(Color.FromRgb(
                        (byte)(200 + row * 10),
                        (byte)(220 + col * 7),
                        (byte)(240 - row * 5)
                    ))
                };

                var textBlock = new TextBlock
                {
                    Text = $"Item {++count}",
                    HorizontalAlignment = HorizontalAlignment.Center,
                    VerticalAlignment = VerticalAlignment.Center
                };

                border.Child = textBlock;
                Grid.SetRow(border, row);
                Grid.SetColumn(border, col);
                grid.Children.Add(border);
            }
        }

        VisualizationContainer.Child = grid;

        _stopwatch.Stop();

        AnalyzeVisualTree(grid);
        TxtStatus.Text = $"Optimized tree created in {_stopwatch.ElapsedMilliseconds} ms - Flat structure, efficient layout";
    }

    private void BtnCompareCanvasGrid_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Comparing Canvas vs Grid performance...";

        var mainPanel = new DockPanel { Margin = new Thickness(10) };

        // Canvas example (absolute positioning)
        var canvasGroup = new GroupBox
        {
            Header = "Canvas (Absolute Positioning - Fast)",
            Width = 400,
            Height = 300,
            Margin = new Thickness(5)
        };

        _stopwatch.Restart();
        var canvas = new Canvas { Background = Brushes.LightYellow };

        for (int i = 0; i < 100; i++)
        {
            var rect = new Rectangle
            {
                Width = 30,
                Height = 30,
                Fill = new SolidColorBrush(Color.FromRgb(
                    (byte)(i * 2 % 256),
                    (byte)(100 + i % 156),
                    (byte)(200 - i % 100)
                ))
            };

            Canvas.SetLeft(rect, (i % 10) * 35);
            Canvas.SetTop(rect, (i / 10) * 35);
            canvas.Children.Add(rect);
        }

        long canvasTime = _stopwatch.ElapsedMilliseconds;
        canvasGroup.Content = canvas;
        DockPanel.SetDock(canvasGroup, Dock.Left);
        mainPanel.Children.Add(canvasGroup);

        // Grid example (flexible layout)
        var gridGroup = new GroupBox
        {
            Header = "Grid (Flexible Layout - Moderate)",
            Width = 400,
            Height = 300,
            Margin = new Thickness(5)
        };

        _stopwatch.Restart();
        var grid = new Grid { Background = Brushes.LightGreen };

        for (int i = 0; i < 10; i++)
        {
            grid.RowDefinitions.Add(new RowDefinition());
            grid.ColumnDefinitions.Add(new ColumnDefinition());
        }

        for (int i = 0; i < 100; i++)
        {
            var rect = new Rectangle
            {
                Fill = new SolidColorBrush(Color.FromRgb(
                    (byte)(200 - i % 100),
                    (byte)(i * 2 % 256),
                    (byte)(100 + i % 156)
                )),
                Margin = new Thickness(2)
            };

            Grid.SetRow(rect, i / 10);
            Grid.SetColumn(rect, i % 10);
            grid.Children.Add(rect);
        }

        long gridTime = _stopwatch.ElapsedMilliseconds;
        gridGroup.Content = grid;
        mainPanel.Children.Add(gridGroup);

        VisualizationContainer.Child = mainPanel;

        TxtAnalysis.Text = $"Performance Comparison:\n\n" +
                          $"Canvas: {canvasTime} ms\n" +
                          $"  - Absolute positioning\n" +
                          $"  - No layout calculations\n" +
                          $"  - Fastest for fixed layouts\n\n" +
                          $"Grid: {gridTime} ms\n" +
                          $"  - Dynamic layout calculations\n" +
                          $"  - More flexible\n" +
                          $"  - Better for responsive UIs\n\n" +
                          $"Winner: {(canvasTime < gridTime ? "Canvas" : "Grid")} " +
                          $"({Math.Abs(canvasTime - gridTime)} ms faster)";

        TxtStatus.Text = "Canvas vs Grid comparison complete";
    }

    private void BtnCompareStackPanel_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demonstrating StackPanel performance characteristics...";

        var mainPanel = new StackPanel { Margin = new Thickness(10) };

        // Vertical StackPanel
        var verticalGroup = new GroupBox
        {
            Header = "Vertical StackPanel",
            Margin = new Thickness(5)
        };

        _stopwatch.Restart();
        var verticalStack = new StackPanel { Orientation = Orientation.Vertical };

        for (int i = 0; i < 50; i++)
        {
            verticalStack.Children.Add(new TextBlock
            {
                Text = $"Vertical Item {i + 1}",
                Background = i % 2 == 0 ? Brushes.White : Brushes.LightGray,
                Padding = new Thickness(5)
            });
        }

        long verticalTime = _stopwatch.ElapsedMilliseconds;
        verticalGroup.Content = new ScrollViewer
        {
            Content = verticalStack,
            MaxHeight = 200,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };
        mainPanel.Children.Add(verticalGroup);

        // Nested StackPanels (anti-pattern)
        var nestedGroup = new GroupBox
        {
            Header = "Nested StackPanels (SLOW - Anti-Pattern)",
            Margin = new Thickness(5)
        };

        _stopwatch.Restart();
        var outerStack = new StackPanel();

        for (int i = 0; i < 20; i++)
        {
            var innerStack = new StackPanel { Orientation = Orientation.Horizontal };
            innerStack.Children.Add(new TextBlock { Text = $"Row {i + 1}:", Width = 60, FontWeight = FontWeights.Bold });

            for (int j = 0; j < 5; j++)
            {
                innerStack.Children.Add(new Border
                {
                    Width = 50,
                    Height = 20,
                    Background = Brushes.LightBlue,
                    BorderBrush = Brushes.Blue,
                    BorderThickness = new Thickness(1),
                    Margin = new Thickness(2),
                    Child = new TextBlock
                    {
                        Text = $"C{j + 1}",
                        HorizontalAlignment = HorizontalAlignment.Center,
                        VerticalAlignment = VerticalAlignment.Center
                    }
                });
            }

            outerStack.Children.Add(innerStack);
        }

        long nestedTime = _stopwatch.ElapsedMilliseconds;
        nestedGroup.Content = new ScrollViewer
        {
            Content = outerStack,
            MaxHeight = 200,
            VerticalScrollBarVisibility = ScrollBarVisibility.Auto
        };
        mainPanel.Children.Add(nestedGroup);

        VisualizationContainer.Child = mainPanel;

        TxtAnalysis.Text = $"StackPanel Performance Analysis:\n\n" +
                          $"Simple Vertical: {verticalTime} ms\n" +
                          $"  - Single-direction layout\n" +
                          $"  - Simple and efficient\n\n" +
                          $"Nested StackPanels: {nestedTime} ms\n" +
                          $"  - Multiple layout passes\n" +
                          $"  - Slower performance\n" +
                          $"  - Consider Grid instead\n\n" +
                          $"Performance Impact: {(nestedTime > verticalTime ? $"{nestedTime - verticalTime} ms slower" : "Similar")}";

        TxtStatus.Text = "StackPanel performance comparison complete";
    }

    private void BtnAnalyzeTree_Click(object sender, RoutedEventArgs e)
    {
        if (VisualizationContainer.Child == null)
        {
            TxtStatus.Text = "No visual tree to analyze - create a demo first";
            MessageBox.Show("Please create a visual tree first by clicking one of the demo buttons.",
                          "No Visual Tree", MessageBoxButton.OK, MessageBoxImage.Information);
            return;
        }

        AnalyzeVisualTree(VisualizationContainer.Child as FrameworkElement);
        TxtStatus.Text = "Visual tree analysis complete";
    }

    private void BtnClearDemo_Click(object sender, RoutedEventArgs e)
    {
        ClearDemo();
        TxtStatus.Text = "Demo cleared - Ready for next demonstration";
    }

    private void ClearDemo()
    {
        VisualizationContainer.Child = null;
        TxtAnalysis.Text = "";
        TxtVisualChildren.Text = "0";
        TxtLogicalChildren.Text = "0";
        TxtTreeDepth.Text = "0";
        TxtRenderTime.Text = "0 ms";
        TxtLayoutTime.Text = "0 ms";
        TxtMemoryImpact.Text = "0 KB";
    }

    private void AnalyzeVisualTree(FrameworkElement? element)
    {
        if (element == null) return;

        int visualCount = CountVisualChildren(element);
        int logicalCount = CountLogicalChildren(element);
        int depth = GetTreeDepth(element);

        TxtVisualChildren.Text = visualCount.ToString();
        TxtLogicalChildren.Text = logicalCount.ToString();
        TxtTreeDepth.Text = depth.ToString();

        // Estimate memory impact (rough calculation)
        long estimatedMemory = (visualCount * 500 + logicalCount * 300) / 1024; // KB
        TxtMemoryImpact.Text = $"{estimatedMemory} KB (estimated)";

        // Build tree structure
        var analysis = new System.Text.StringBuilder();
        analysis.AppendLine("Visual Tree Structure:\n");
        BuildTreeString(element, analysis, 0);

        analysis.AppendLine("\n=== Summary ===");
        analysis.AppendLine($"Total Visual Elements: {visualCount}");
        analysis.AppendLine($"Total Logical Elements: {logicalCount}");
        analysis.AppendLine($"Maximum Depth: {depth}");
        analysis.AppendLine($"\nComplexity: {(depth > 10 ? "HIGH (Consider optimization)" : depth > 6 ? "MODERATE" : "LOW (Good)")}");

        TxtAnalysis.Text = analysis.ToString();
    }

    private int CountVisualChildren(DependencyObject parent)
    {
        int count = 1;
        for (int i = 0; i < VisualTreeHelper.GetChildrenCount(parent); i++)
        {
            count += CountVisualChildren(VisualTreeHelper.GetChild(parent, i));
        }
        return count;
    }

    private int CountLogicalChildren(DependencyObject parent)
    {
        int count = 1;
        foreach (var child in LogicalTreeHelper.GetChildren(parent))
        {
            if (child is DependencyObject depObj)
            {
                count += CountLogicalChildren(depObj);
            }
        }
        return count;
    }

    private int GetTreeDepth(DependencyObject element)
    {
        int maxDepth = 0;
        for (int i = 0; i < VisualTreeHelper.GetChildrenCount(element); i++)
        {
            int childDepth = GetTreeDepth(VisualTreeHelper.GetChild(element, i));
            maxDepth = Math.Max(maxDepth, childDepth);
        }
        return maxDepth + 1;
    }

    private void BuildTreeString(DependencyObject element, System.Text.StringBuilder sb, int depth)
    {
        if (depth > 15) // Limit depth to prevent excessive output
        {
            sb.AppendLine(new string(' ', depth * 2) + "... (truncated)");
            return;
        }

        string indent = new string(' ', depth * 2);
        string elementType = element.GetType().Name;

        if (element is FrameworkElement fe)
        {
            sb.AppendLine($"{indent}{elementType} (Name: {fe.Name ?? "unnamed"})");
        }
        else
        {
            sb.AppendLine($"{indent}{elementType}");
        }

        int childCount = VisualTreeHelper.GetChildrenCount(element);
        for (int i = 0; i < Math.Min(childCount, 50); i++) // Limit children shown
        {
            BuildTreeString(VisualTreeHelper.GetChild(element, i), sb, depth + 1);
        }

        if (childCount > 50)
        {
            sb.AppendLine($"{indent}  ... and {childCount - 50} more children");
        }
    }
}
