using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Threading;

namespace CustomControlsApp.Controls
{
    /// <summary>
    /// High-performance particle system using OnRender.
    /// Demonstrates rendering thousands of particles efficiently.
    /// </summary>
    public class ParticleSystem : FrameworkElement
    {
        private class Particle
        {
            public double X;
            public double Y;
            public double VelocityX;
            public double VelocityY;
            public Color Color;
            public double Life; // 0.0 to 1.0
        }

        private Particle[] _particles = Array.Empty<Particle>();
        private DispatcherTimer _updateTimer;
        private Random _random = new Random();
        private Pen _particlePen;

        public double ParticleSize { get; set; } = 3.0;
        public double SpeedMultiplier { get; set; } = 1.0;

        public ParticleSystem()
        {
            _particlePen = new Pen(Brushes.White, 1);
            _particlePen.Freeze();

            ClipToBounds = true;
        }

        public void InitializeParticles(int count)
        {
            _particles = new Particle[count];

            for (int i = 0; i < count; i++)
            {
                _particles[i] = CreateRandomParticle();
            }

            InvalidateVisual();
        }

        private Particle CreateRandomParticle()
        {
            double width = ActualWidth > 0 ? ActualWidth : 800;
            double height = ActualHeight > 0 ? ActualHeight : 600;

            return new Particle
            {
                X = _random.NextDouble() * width,
                Y = _random.NextDouble() * height,
                VelocityX = (_random.NextDouble() - 0.5) * 4,
                VelocityY = (_random.NextDouble() - 0.5) * 4,
                Color = GetRandomColor(),
                Life = _random.NextDouble()
            };
        }

        private Color GetRandomColor()
        {
            return Color.FromRgb(
                (byte)_random.Next(100, 256),
                (byte)_random.Next(100, 256),
                (byte)_random.Next(100, 256));
        }

        public void StartSimulation()
        {
            if (_updateTimer == null)
            {
                _updateTimer = new DispatcherTimer
                {
                    Interval = TimeSpan.FromMilliseconds(16) // ~60 FPS
                };
                _updateTimer.Tick += UpdateTimer_Tick;
            }

            _updateTimer.Start();
        }

        public void StopSimulation()
        {
            _updateTimer?.Stop();
        }

        private void UpdateTimer_Tick(object? sender, EventArgs e)
        {
            UpdateParticles();
            InvalidateVisual();
        }

        private void UpdateParticles()
        {
            double width = ActualWidth;
            double height = ActualHeight;

            if (width <= 0 || height <= 0) return;

            foreach (Particle p in _particles)
            {
                // Update position
                p.X += p.VelocityX * SpeedMultiplier;
                p.Y += p.VelocityY * SpeedMultiplier;

                // Bounce off edges
                if (p.X < 0 || p.X > width)
                {
                    p.VelocityX = -p.VelocityX;
                    p.X = Math.Clamp(p.X, 0, width);
                }

                if (p.Y < 0 || p.Y > height)
                {
                    p.VelocityY = -p.VelocityY;
                    p.Y = Math.Clamp(p.Y, 0, height);
                }

                // Update life (for pulsing effect)
                p.Life += 0.02;
                if (p.Life > 1.0) p.Life = 0.0;
            }
        }

        protected override void OnRender(DrawingContext drawingContext)
        {
            base.OnRender(drawingContext);

            double width = ActualWidth;
            double height = ActualHeight;

            if (width <= 0 || height <= 0) return;

            // Draw background
            drawingContext.DrawRectangle(Brushes.Black, null, new Rect(0, 0, width, height));

            // Draw all particles in a single pass
            foreach (Particle p in _particles)
            {
                // Calculate alpha based on life for pulsing effect
                byte alpha = (byte)(128 + 127 * Math.Sin(p.Life * Math.PI * 2));
                Color particleColor = Color.FromArgb(alpha, p.Color.R, p.Color.G, p.Color.B);

                SolidColorBrush brush = new SolidColorBrush(particleColor);
                brush.Freeze();

                // Draw particle as small circle
                drawingContext.DrawEllipse(brush, null, new Point(p.X, p.Y), ParticleSize, ParticleSize);
            }

            // Draw particle count
            FormattedText text = new FormattedText(
                $"Particles: {_particles.Length:N0}",
                System.Globalization.CultureInfo.CurrentCulture,
                FlowDirection.LeftToRight,
                new Typeface("Segoe UI"),
                14,
                Brushes.White,
                VisualTreeHelper.GetDpi(this).PixelsPerDip);

            drawingContext.DrawText(text, new Point(10, 10));
        }

        protected override void OnRenderSizeChanged(SizeChangedInfo sizeInfo)
        {
            base.OnRenderSizeChanged(sizeInfo);

            // Reinitialize particles when size changes
            if (_particles.Length > 0)
            {
                int count = _particles.Length;
                InitializeParticles(count);
            }
        }
    }
}
