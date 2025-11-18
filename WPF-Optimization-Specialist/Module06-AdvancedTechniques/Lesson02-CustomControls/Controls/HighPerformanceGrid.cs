using System;
using System.Windows;
using System.Windows.Media;

namespace CustomControlsApp.Controls
{
    /// <summary>
    /// High-performance data grid using OnRender instead of visual tree.
    /// Demonstrates efficient rendering of tabular data without cell visuals.
    /// </summary>
    public class HighPerformanceGrid : FrameworkElement
    {
        private string[,]? _data;
        private int _rows;
        private int _columns;
        private double _rowHeight = 30;
        private double _columnWidth = 120;
        private Random _random = new Random();

        private Pen _gridPen;
        private Pen _headerPen;
        private Brush _headerBrush;
        private Brush _evenRowBrush;
        private Brush _oddRowBrush;
        private Brush _textBrush;

        public bool AlternateRowColors { get; set; } = true;

        public HighPerformanceGrid()
        {
            _gridPen = new Pen(Brushes.LightGray, 1);
            _gridPen.Freeze();

            _headerPen = new Pen(Brushes.Black, 2);
            _headerPen.Freeze();

            _headerBrush = new SolidColorBrush(Color.FromRgb(70, 130, 180));
            _headerBrush.Freeze();

            _evenRowBrush = Brushes.White;
            _oddRowBrush = new SolidColorBrush(Color.FromRgb(245, 245, 245));
            _oddRowBrush.Freeze();

            _textBrush = Brushes.Black;
        }

        public void GenerateData(int rows, int columns)
        {
            _rows = rows;
            _columns = columns;
            _data = new string[rows + 1, columns]; // +1 for header row

            // Generate header
            for (int col = 0; col < columns; col++)
            {
                _data[0, col] = $"Column {col + 1}";
            }

            // Generate data
            for (int row = 1; row <= rows; row++)
            {
                for (int col = 0; col < columns; col++)
                {
                    _data[row, col] = GenerateRandomCellData(row, col);
                }
            }

            Width = _columnWidth * columns;
            Height = _rowHeight * (rows + 1);

            InvalidateVisual();
        }

        private string GenerateRandomCellData(int row, int col)
        {
            return col switch
            {
                0 => $"ID-{row:D6}",
                1 => $"Item {_random.Next(1000, 9999)}",
                2 => $"${_random.Next(100, 10000):N2}",
                3 => DateTime.Now.AddDays(-_random.Next(0, 365)).ToString("yyyy-MM-dd"),
                4 => _random.Next(1, 100).ToString(),
                _ => $"Data {_random.Next(1000, 9999)}"
            };
        }

        public void UpdateRandomCells(int count)
        {
            if (_data == null) return;

            for (int i = 0; i < count; i++)
            {
                int row = _random.Next(1, _rows + 1);
                int col = _random.Next(0, _columns);
                _data[row, col] = GenerateRandomCellData(row, col);
            }

            InvalidateVisual();
        }

        protected override void OnRender(DrawingContext drawingContext)
        {
            base.OnRender(drawingContext);

            if (_data == null) return;

            double width = _columnWidth * _columns;
            double height = _rowHeight * (_rows + 1);

            // Draw background
            drawingContext.DrawRectangle(Brushes.White, null, new Rect(0, 0, width, height));

            // Draw header
            DrawHeader(drawingContext);

            // Draw data rows
            for (int row = 1; row <= _rows; row++)
            {
                DrawRow(drawingContext, row);
            }

            // Draw grid lines
            DrawGridLines(drawingContext);
        }

        private void DrawHeader(DrawingContext dc)
        {
            double y = 0;
            double height = _rowHeight;

            // Draw header background
            dc.DrawRectangle(_headerBrush, null, new Rect(0, y, _columnWidth * _columns, height));

            // Draw header text
            for (int col = 0; col < _columns; col++)
            {
                double x = col * _columnWidth;
                string text = _data![0, col];

                FormattedText formattedText = new FormattedText(
                    text,
                    System.Globalization.CultureInfo.CurrentCulture,
                    FlowDirection.LeftToRight,
                    new Typeface(new FontFamily("Segoe UI"), FontStyles.Normal, FontWeights.Bold, FontStretches.Normal),
                    14,
                    Brushes.White,
                    VisualTreeHelper.GetDpi(this).PixelsPerDip);

                dc.DrawText(formattedText, new Point(x + 5, y + (height - formattedText.Height) / 2));
            }

            // Draw header bottom line
            dc.DrawLine(_headerPen, new Point(0, height), new Point(_columnWidth * _columns, height));
        }

        private void DrawRow(DrawingContext dc, int row)
        {
            double y = row * _rowHeight;
            double height = _rowHeight;

            // Draw row background (alternating colors)
            if (AlternateRowColors && row % 2 == 0)
            {
                dc.DrawRectangle(_oddRowBrush, null, new Rect(0, y, _columnWidth * _columns, height));
            }

            // Draw cell text
            for (int col = 0; col < _columns; col++)
            {
                double x = col * _columnWidth;
                string text = _data![row, col];

                FormattedText formattedText = new FormattedText(
                    text,
                    System.Globalization.CultureInfo.CurrentCulture,
                    FlowDirection.LeftToRight,
                    new Typeface("Segoe UI"),
                    12,
                    _textBrush,
                    VisualTreeHelper.GetDpi(this).PixelsPerDip);

                dc.DrawText(formattedText, new Point(x + 5, y + (height - formattedText.Height) / 2));
            }
        }

        private void DrawGridLines(DrawingContext dc)
        {
            double width = _columnWidth * _columns;
            double height = _rowHeight * (_rows + 1);

            // Draw vertical lines
            for (int col = 0; col <= _columns; col++)
            {
                double x = col * _columnWidth;
                dc.DrawLine(_gridPen, new Point(x, 0), new Point(x, height));
            }

            // Draw horizontal lines
            for (int row = 0; row <= _rows + 1; row++)
            {
                double y = row * _rowHeight;
                dc.DrawLine(_gridPen, new Point(0, y), new Point(width, y));
            }
        }
    }
}
