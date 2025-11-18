using System.Windows;
using System.Windows.Media;
using BenchmarkDotNet.Attributes;

namespace BenchmarkingSuiteApp.Benchmarks
{
    [MemoryDiagnoser]
    [SimpleJob(warmupCount: 3, iterationCount: 5)]
    public class RenderingBenchmarks
    {
        [Benchmark(Baseline = true)]
        public SolidColorBrush CreateUnfrozenBrush()
        {
            return new SolidColorBrush(Colors.Red);
        }

        [Benchmark]
        public SolidColorBrush CreateFrozenBrush()
        {
            var brush = new SolidColorBrush(Colors.Red);
            brush.Freeze();
            return brush;
        }

        [Benchmark]
        public PathGeometry CreatePathGeometry()
        {
            var geometry = new PathGeometry();
            var figure = new PathFigure { StartPoint = new Point(0, 0) };
            figure.Segments.Add(new LineSegment(new Point(100, 100), true));
            geometry.Figures.Add(figure);
            return geometry;
        }

        [Benchmark]
        public StreamGeometry CreateStreamGeometry()
        {
            var geometry = new StreamGeometry();
            using (var ctx = geometry.Open())
            {
                ctx.BeginFigure(new Point(0, 0), false, false);
                ctx.LineTo(new Point(100, 100), true, false);
            }
            geometry.Freeze();
            return geometry;
        }

        [Benchmark]
        public void DrawingVisualOperations()
        {
            var visual = new DrawingVisual();
            var pen = new Pen(Brushes.Black, 1);
            pen.Freeze();

            using (var dc = visual.RenderOpen())
            {
                for (int i = 0; i < 100; i++)
                {
                    dc.DrawLine(pen, new Point(0, i), new Point(100, i));
                }
            }
        }
    }
}
