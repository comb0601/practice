using System;
using System.Windows;
using System.Windows.Media;

namespace CustomControlsApp.Controls
{
    /// <summary>
    /// High-performance gauge control using OnRender.
    /// Demonstrates efficient circular rendering with gradients and text.
    /// </summary>
    public class CustomGauge : FrameworkElement
    {
        public static readonly DependencyProperty ValueProperty =
            DependencyProperty.Register(nameof(Value), typeof(double), typeof(CustomGauge),
                new FrameworkPropertyMetadata(0.0, FrameworkPropertyMetadataOptions.AffectsRender));

        public static readonly DependencyProperty MinimumProperty =
            DependencyProperty.Register(nameof(Minimum), typeof(double), typeof(CustomGauge),
                new FrameworkPropertyMetadata(0.0, FrameworkPropertyMetadataOptions.AffectsRender));

        public static readonly DependencyProperty MaximumProperty =
            DependencyProperty.Register(nameof(Maximum), typeof(double), typeof(CustomGauge),
                new FrameworkPropertyMetadata(100.0, FrameworkPropertyMetadataOptions.AffectsRender));

        public double Value
        {
            get => (double)GetValue(ValueProperty);
            set => SetValue(ValueProperty, value);
        }

        public double Minimum
        {
            get => (double)GetValue(MinimumProperty);
            set => SetValue(MinimumProperty, value);
        }

        public double Maximum
        {
            get => (double)GetValue(MaximumProperty);
            set => SetValue(MaximumProperty, value);
        }

        public bool ShowLabels { get; set; } = true;
        public bool ShowTicks { get; set; } = true;
        public bool UseGradient { get; set; } = true;

        private Pen _outlinePen;
        private Pen _tickPen;
        private Brush _backgroundBrush;

        public CustomGauge()
        {
            _outlinePen = new Pen(Brushes.DarkGray, 3);
            _outlinePen.Freeze();

            _tickPen = new Pen(Brushes.Gray, 1);
            _tickPen.Freeze();

            _backgroundBrush = Brushes.WhiteSmoke;

            Width = 300;
            Height = 300;
        }

        protected override void OnRender(DrawingContext drawingContext)
        {
            base.OnRender(drawingContext);

            double width = ActualWidth;
            double height = ActualHeight;
            double size = Math.Min(width, height);
            Point center = new Point(width / 2, height / 2);
            double radius = size / 2 - 30;

            if (radius <= 0) return;

            // Draw background circle
            drawingContext.DrawEllipse(_backgroundBrush, _outlinePen, center, radius + 10, radius + 10);

            // Draw gauge arc
            DrawGaugeArc(drawingContext, center, radius);

            // Draw ticks
            if (ShowTicks)
            {
                DrawTicks(drawingContext, center, radius);
            }

            // Draw labels
            if (ShowLabels)
            {
                DrawLabels(drawingContext, center, radius);
            }

            // Draw needle
            DrawNeedle(drawingContext, center, radius);

            // Draw center value
            DrawCenterValue(drawingContext, center);
        }

        private void DrawGaugeArc(DrawingContext dc, Point center, double radius)
        {
            double startAngle = 135; // degrees
            double endAngle = 45;    // degrees
            double totalAngle = 360 - startAngle + endAngle;

            // Calculate current value position
            double range = Maximum - Minimum;
            double normalizedValue = (Value - Minimum) / range;
            double valueAngle = normalizedValue * totalAngle;

            // Draw background arc
            DrawArc(dc, center, radius, startAngle, startAngle + totalAngle, Brushes.LightGray, 15);

            // Draw value arc with gradient
            Brush valueBrush;
            if (UseGradient)
            {
                GradientStopCollection stops = new GradientStopCollection
                {
                    new GradientStop(Colors.Red, 0.0),
                    new GradientStop(Colors.Yellow, 0.5),
                    new GradientStop(Colors.Lime, 1.0)
                };

                valueBrush = new LinearGradientBrush(stops, new Point(0, 0), new Point(1, 0));
                valueBrush.Freeze();
            }
            else
            {
                valueBrush = Brushes.DodgerBlue;
            }

            DrawArc(dc, center, radius, startAngle, startAngle + valueAngle, valueBrush, 15);
        }

        private void DrawArc(DrawingContext dc, Point center, double radius, double startAngle, double endAngle, Brush brush, double thickness)
        {
            double startRad = startAngle * Math.PI / 180;
            double endRad = endAngle * Math.PI / 180;

            Point startPoint = new Point(
                center.X + radius * Math.Cos(startRad),
                center.Y + radius * Math.Sin(startRad));

            Point endPoint = new Point(
                center.X + radius * Math.Cos(endRad),
                center.Y + radius * Math.Sin(endRad));

            bool isLargeArc = Math.Abs(endAngle - startAngle) > 180;

            PathFigure pathFigure = new PathFigure { StartPoint = startPoint };
            pathFigure.Segments.Add(new ArcSegment(
                endPoint,
                new Size(radius, radius),
                0,
                isLargeArc,
                SweepDirection.Clockwise,
                true));

            PathGeometry pathGeometry = new PathGeometry();
            pathGeometry.Figures.Add(pathFigure);

            Pen pen = new Pen(brush, thickness);
            pen.Freeze();

            dc.DrawGeometry(null, pen, pathGeometry);
        }

        private void DrawTicks(DrawingContext dc, Point center, double radius)
        {
            double startAngle = 135;
            double totalAngle = 270;
            int tickCount = 11;

            for (int i = 0; i < tickCount; i++)
            {
                double angle = (startAngle + (i * totalAngle / (tickCount - 1))) * Math.PI / 180;
                double innerRadius = radius - 5;
                double outerRadius = radius + 5;

                Point inner = new Point(
                    center.X + innerRadius * Math.Cos(angle),
                    center.Y + innerRadius * Math.Sin(angle));

                Point outer = new Point(
                    center.X + outerRadius * Math.Cos(angle),
                    center.Y + outerRadius * Math.Sin(angle));

                Pen tickPen = new Pen(Brushes.Black, i % 2 == 0 ? 2 : 1);
                tickPen.Freeze();

                dc.DrawLine(tickPen, inner, outer);
            }
        }

        private void DrawLabels(DrawingContext dc, Point center, double radius)
        {
            double startAngle = 135;
            double totalAngle = 270;
            int labelCount = 6;

            for (int i = 0; i < labelCount; i++)
            {
                double value = Minimum + (i * (Maximum - Minimum) / (labelCount - 1));
                double angle = (startAngle + (i * totalAngle / (labelCount - 1))) * Math.PI / 180;
                double labelRadius = radius + 20;

                Point labelPos = new Point(
                    center.X + labelRadius * Math.Cos(angle),
                    center.Y + labelRadius * Math.Sin(angle));

                FormattedText text = new FormattedText(
                    value.ToString("F0"),
                    System.Globalization.CultureInfo.CurrentCulture,
                    FlowDirection.LeftToRight,
                    new Typeface("Segoe UI"),
                    14,
                    Brushes.Black,
                    VisualTreeHelper.GetDpi(this).PixelsPerDip);

                dc.DrawText(text, new Point(labelPos.X - text.Width / 2, labelPos.Y - text.Height / 2));
            }
        }

        private void DrawNeedle(DrawingContext dc, Point center, double radius)
        {
            double startAngle = 135;
            double totalAngle = 270;
            double range = Maximum - Minimum;
            double normalizedValue = Math.Clamp((Value - Minimum) / range, 0, 1);
            double angle = (startAngle + normalizedValue * totalAngle) * Math.PI / 180;

            double needleLength = radius - 10;
            Point needleEnd = new Point(
                center.X + needleLength * Math.Cos(angle),
                center.Y + needleLength * Math.Sin(angle));

            // Draw needle shadow
            Pen shadowPen = new Pen(new SolidColorBrush(Color.FromArgb(50, 0, 0, 0)), 4);
            dc.DrawLine(shadowPen, new Point(center.X + 2, center.Y + 2),
                new Point(needleEnd.X + 2, needleEnd.Y + 2));

            // Draw needle
            Pen needlePen = new Pen(Brushes.Red, 3);
            needlePen.Freeze();
            dc.DrawLine(needlePen, center, needleEnd);

            // Draw center circle
            dc.DrawEllipse(Brushes.DarkRed, null, center, 8, 8);
            dc.DrawEllipse(Brushes.Red, null, center, 5, 5);
        }

        private void DrawCenterValue(DrawingContext dc, Point center)
        {
            FormattedText text = new FormattedText(
                Value.ToString("F1"),
                System.Globalization.CultureInfo.CurrentCulture,
                FlowDirection.LeftToRight,
                new Typeface("Segoe UI"),
                24,
                Brushes.Black,
                VisualTreeHelper.GetDpi(this).PixelsPerDip);

            dc.DrawText(text, new Point(center.X - text.Width / 2, center.Y + 30));
        }
    }
}
