using System;
using System.Windows;
using System.Windows.Media;

namespace CustomControlsApp.Controls
{
    /// <summary>
    /// High-performance chart control using OnRender for optimal rendering performance.
    /// Demonstrates efficient drawing of large datasets without visual tree overhead.
    /// </summary>
    public class HighPerformanceChart : FrameworkElement
    {
        private double[] _data = Array.Empty<double>();
        private Pen _linePen;
        private Pen _gridPen;
        private Pen _axisPen;
        private Brush _backgroundBrush;
        private Brush _textBrush;

        public HighPerformanceChart()
        {
            // Create and freeze brushes and pens for performance
            _linePen = new Pen(Brushes.DodgerBlue, 2);
            _linePen.Freeze();

            _gridPen = new Pen(Brushes.LightGray, 0.5);
            _gridPen.Freeze();

            _axisPen = new Pen(Brushes.Black, 1);
            _axisPen.Freeze();

            _backgroundBrush = Brushes.White;
            _textBrush = Brushes.Black;

            // Enable layout
            ClipToBounds = true;
        }

        public void SetData(double[] data)
        {
            _data = data ?? Array.Empty<double>();
            InvalidateVisual();
        }

        public void ShiftData(double newValue)
        {
            if (_data.Length == 0) return;

            // Shift all data left and add new value
            for (int i = 0; i < _data.Length - 1; i++)
            {
                _data[i] = _data[i + 1];
            }
            _data[_data.Length - 1] = newValue;

            InvalidateVisual();
        }

        protected override void OnRender(DrawingContext drawingContext)
        {
            base.OnRender(drawingContext);

            double width = ActualWidth;
            double height = ActualHeight;

            if (width <= 0 || height <= 0) return;

            // Draw background
            drawingContext.DrawRectangle(_backgroundBrush, null, new Rect(0, 0, width, height));

            // Draw grid
            DrawGrid(drawingContext, width, height);

            // Draw axes
            DrawAxes(drawingContext, width, height);

            // Draw data
            if (_data.Length > 1)
            {
                DrawData(drawingContext, width, height);
            }

            // Draw statistics
            DrawStatistics(drawingContext, width);
        }

        private void DrawGrid(DrawingContext dc, double width, double height)
        {
            // Vertical grid lines
            int verticalLines = 10;
            for (int i = 0; i <= verticalLines; i++)
            {
                double x = i * width / verticalLines;
                dc.DrawLine(_gridPen, new Point(x, 0), new Point(x, height));
            }

            // Horizontal grid lines
            int horizontalLines = 8;
            for (int i = 0; i <= horizontalLines; i++)
            {
                double y = i * height / horizontalLines;
                dc.DrawLine(_gridPen, new Point(0, y), new Point(width, y));
            }
        }

        private void DrawAxes(DrawingContext dc, double width, double height)
        {
            // Bottom axis
            dc.DrawLine(_axisPen, new Point(0, height), new Point(width, height));

            // Left axis
            dc.DrawLine(_axisPen, new Point(0, 0), new Point(0, height));
        }

        private void DrawData(DrawingContext dc, double width, double height)
        {
            if (_data.Length < 2) return;

            // Find min and max for scaling
            double min = double.MaxValue;
            double max = double.MinValue;

            foreach (double value in _data)
            {
                if (value < min) min = value;
                if (value > max) max = value;
            }

            double range = max - min;
            if (range < 0.001) range = 1.0; // Avoid division by zero

            // Create geometry for the line
            StreamGeometry geometry = new StreamGeometry();

            using (StreamGeometryContext ctx = geometry.Open())
            {
                double xStep = width / (_data.Length - 1);
                double firstY = height - ((_data[0] - min) / range * height);

                ctx.BeginFigure(new Point(0, firstY), false, false);

                for (int i = 1; i < _data.Length; i++)
                {
                    double x = i * xStep;
                    double y = height - ((_data[i] - min) / range * height);
                    ctx.LineTo(new Point(x, y), true, false);
                }
            }

            geometry.Freeze();
            dc.DrawGeometry(null, _linePen, geometry);
        }

        private void DrawStatistics(DrawingContext dc, double width)
        {
            if (_data.Length == 0) return;

            string stats = $"Points: {_data.Length:N0}";
            FormattedText text = new FormattedText(
                stats,
                System.Globalization.CultureInfo.CurrentCulture,
                FlowDirection.LeftToRight,
                new Typeface("Segoe UI"),
                12,
                _textBrush,
                VisualTreeHelper.GetDpi(this).PixelsPerDip);

            dc.DrawText(text, new Point(width - text.Width - 10, 10));
        }

        protected override Size MeasureOverride(Size availableSize)
        {
            // Request as much space as available
            return availableSize;
        }
    }
}
