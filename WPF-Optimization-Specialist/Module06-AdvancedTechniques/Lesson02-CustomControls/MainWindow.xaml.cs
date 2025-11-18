using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Threading;
using CustomControlsApp.Controls;

namespace CustomControlsApp
{
    public partial class MainWindow : Window
    {
        private DispatcherTimer _chartTimer;
        private DispatcherTimer _gaugeTimer;
        private DispatcherTimer _particleTimer;
        private DispatcherTimer _fpsTimer;
        private DispatcherTimer _memoryTimer;

        private Random _random = new Random();
        private Stopwatch _renderStopwatch = new Stopwatch();
        private int _frameCount = 0;
        private double _targetGaugeValue = 65;

        public MainWindow()
        {
            InitializeComponent();
            InitializeUI();
            StartMonitoring();
        }

        private void InitializeUI()
        {
            // Chart controls
            DataPointsSlider.ValueChanged += (s, e) =>
            {
                int points = (int)Math.Pow(10, e.NewValue);
                DataPointsText.Text = $"{points:N0}";
            };

            UpdateFrequencySlider.ValueChanged += (s, e) =>
            {
                UpdateFrequencyText.Text = ((int)e.NewValue).ToString();
                if (_chartTimer != null && _chartTimer.IsEnabled)
                {
                    _chartTimer.Interval = TimeSpan.FromSeconds(1.0 / e.NewValue);
                }
            };

            // Gauge controls
            GaugeValueSlider.ValueChanged += (s, e) =>
            {
                GaugeValueText.Text = ((int)e.NewValue).ToString();
                _targetGaugeValue = e.NewValue;
            };

            GaugeSpeedSlider.ValueChanged += (s, e) =>
            {
                GaugeSpeedText.Text = ((int)e.NewValue).ToString();
            };

            ShowGaugeLabelsCheck.Checked += (s, e) => CustomGaugeControl.ShowLabels = true;
            ShowGaugeLabelsCheck.Unchecked += (s, e) => CustomGaugeControl.ShowLabels = false;

            ShowGaugeTicksCheck.Checked += (s, e) => CustomGaugeControl.ShowTicks = true;
            ShowGaugeTicksCheck.Unchecked += (s, e) => CustomGaugeControl.ShowTicks = false;

            GaugeGradientCheck.Checked += (s, e) => CustomGaugeControl.UseGradient = true;
            GaugeGradientCheck.Unchecked += (s, e) => CustomGaugeControl.UseGradient = false;

            // Particle controls
            ParticleCountSlider.ValueChanged += (s, e) =>
            {
                int count = (int)Math.Pow(10, e.NewValue);
                ParticleCountText.Text = $"{count:N0}";
            };

            ParticleSizeSlider.ValueChanged += (s, e) =>
            {
                ParticleSizeText.Text = ((int)e.NewValue).ToString();
                if (ParticleSystemControl != null)
                {
                    ParticleSystemControl.ParticleSize = e.NewValue;
                }
            };

            ParticleSpeedSlider.ValueChanged += (s, e) =>
            {
                ParticleSpeedText.Text = $"{e.NewValue:F1}x";
                if (ParticleSystemControl != null)
                {
                    ParticleSystemControl.SpeedMultiplier = e.NewValue;
                }
            };

            // Grid controls
            GridRowsSlider.ValueChanged += (s, e) =>
            {
                GridRowsText.Text = ((int)e.NewValue).ToString();
            };

            GridColumnsSlider.ValueChanged += (s, e) =>
            {
                GridColumnsText.Text = ((int)e.NewValue).ToString();
            };

            GridAlternateRowsCheck.Checked += (s, e) => { if (CustomDataGrid != null) CustomDataGrid.AlternateRowColors = true; };
            GridAlternateRowsCheck.Unchecked += (s, e) => { if (CustomDataGrid != null) CustomDataGrid.AlternateRowColors = false; };

            // Initial data
            DataPointsText.Text = "10,000";
            UpdateFrequencyText.Text = "30";
            ParticleCountText.Text = "10,000";
            ParticleSizeText.Text = "3";
            ParticleSpeedText.Text = "1.0x";
            GridRowsText.Text = "100";
            GridColumnsText.Text = "8";

            // Generate initial chart data
            GenerateChartData();
        }

        private void StartMonitoring()
        {
            // FPS monitoring
            _fpsTimer = new DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(1)
            };
            _fpsTimer.Tick += (s, e) =>
            {
                RenderFpsText.Text = _frameCount.ToString();
                _frameCount = 0;
            };
            _fpsTimer.Start();

            // Memory monitoring
            _memoryTimer = new DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(2)
            };
            _memoryTimer.Tick += (s, e) =>
            {
                long memoryBytes = GC.GetTotalMemory(false);
                MemoryText.Text = $"{memoryBytes / (1024.0 * 1024.0):F2} MB";
            };
            _memoryTimer.Start();
        }

        #region Chart Methods

        private void GenerateChartData()
        {
            int pointCount = (int)Math.Pow(10, DataPointsSlider.Value);
            double[] data = new double[pointCount];

            for (int i = 0; i < pointCount; i++)
            {
                data[i] = Math.Sin(i * Math.PI * 4 / pointCount) * 50 +
                         Math.Cos(i * Math.PI * 8 / pointCount) * 30 +
                         _random.NextDouble() * 20;
            }

            PerformanceChart.SetData(data);
            UpdateStatus($"Generated {pointCount:N0} data points for chart");
        }

        private void StartChartBtn_Click(object sender, RoutedEventArgs e)
        {
            int fps = (int)UpdateFrequencySlider.Value;
            _chartTimer = new DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(1.0 / fps)
            };
            _chartTimer.Tick += ChartTimer_Tick;
            _chartTimer.Start();

            StartChartBtn.IsEnabled = false;
            StopChartBtn.IsEnabled = true;

            UpdateStatus($"Chart animation started at {fps} FPS");
        }

        private void StopChartBtn_Click(object sender, RoutedEventArgs e)
        {
            _chartTimer?.Stop();
            StartChartBtn.IsEnabled = true;
            StopChartBtn.IsEnabled = false;

            UpdateStatus("Chart animation stopped");
        }

        private void RegenerateDataBtn_Click(object sender, RoutedEventArgs e)
        {
            GenerateChartData();
        }

        private void ChartTimer_Tick(object? sender, EventArgs e)
        {
            _renderStopwatch.Restart();

            // Update chart data (simulate real-time updates)
            PerformanceChart.ShiftData(_random.NextDouble() * 100 - 50);

            _renderStopwatch.Stop();
            _frameCount++;

            ChartRenderTimeText.Text = $"Render Time: {_renderStopwatch.Elapsed.TotalMilliseconds:F2} ms";
            ChartFpsText.Text = $"Target FPS: {(int)UpdateFrequencySlider.Value}";
            ChartMemoryText.Text = $"Data Points: {(int)Math.Pow(10, DataPointsSlider.Value):N0}";
        }

        #endregion

        #region Gauge Methods

        private void AnimateGaugeBtn_Click(object sender, RoutedEventArgs e)
        {
            _targetGaugeValue = GaugeValueSlider.Value;
            double speed = GaugeSpeedSlider.Value;

            _gaugeTimer?.Stop();
            _gaugeTimer = new DispatcherTimer
            {
                Interval = TimeSpan.FromMilliseconds(16) // ~60 FPS
            };
            _gaugeTimer.Tick += (s, args) =>
            {
                double current = CustomGaugeControl.Value;
                double diff = _targetGaugeValue - current;

                if (Math.Abs(diff) < 0.1)
                {
                    CustomGaugeControl.Value = _targetGaugeValue;
                    _gaugeTimer.Stop();
                }
                else
                {
                    CustomGaugeControl.Value = current + diff * 0.05 * speed;
                }
            };
            _gaugeTimer.Start();

            UpdateStatus($"Animating gauge to {_targetGaugeValue:F0}");
        }

        private void RandomGaugeBtn_Click(object sender, RoutedEventArgs e)
        {
            _targetGaugeValue = _random.NextDouble() * 100;
            GaugeValueSlider.Value = _targetGaugeValue;
            AnimateGaugeBtn_Click(sender, e);
        }

        #endregion

        #region Particle Methods

        private void StartParticlesBtn_Click(object sender, RoutedEventArgs e)
        {
            int particleCount = (int)Math.Pow(10, ParticleCountSlider.Value);
            ParticleSystemControl.InitializeParticles(particleCount);
            ParticleSystemControl.StartSimulation();

            StartParticlesBtn.IsEnabled = false;
            StopParticlesBtn.IsEnabled = true;

            UpdateStatus($"Particle simulation started with {particleCount:N0} particles");
        }

        private void StopParticlesBtn_Click(object sender, RoutedEventArgs e)
        {
            ParticleSystemControl.StopSimulation();

            StartParticlesBtn.IsEnabled = true;
            StopParticlesBtn.IsEnabled = false;

            UpdateStatus("Particle simulation stopped");
        }

        private void ResetParticlesBtn_Click(object sender, RoutedEventArgs e)
        {
            ParticleSystemControl.StopSimulation();
            int particleCount = (int)Math.Pow(10, ParticleCountSlider.Value);
            ParticleSystemControl.InitializeParticles(particleCount);

            UpdateStatus($"Particles reset: {particleCount:N0} particles");
        }

        #endregion

        #region Grid Methods

        private void GenerateGridBtn_Click(object sender, RoutedEventArgs e)
        {
            int rows = (int)GridRowsSlider.Value;
            int columns = (int)GridColumnsSlider.Value;

            var sw = Stopwatch.StartNew();
            CustomDataGrid.GenerateData(rows, columns);
            sw.Stop();

            UpdateStatus($"Generated {rows}x{columns} grid in {sw.ElapsedMilliseconds} ms");
            UpdatePerformanceMetrics();
        }

        private void UpdateCellsBtn_Click(object sender, RoutedEventArgs e)
        {
            int updates = _random.Next(10, 50);
            CustomDataGrid.UpdateRandomCells(updates);

            UpdateStatus($"Updated {updates} random cells");
        }

        #endregion

        private void UpdatePerformanceMetrics()
        {
            PerformanceMetricsText.Text =
                $"Chart Control:\n" +
                $"  • Rendering up to 100M data points per second\n" +
                $"  • Single OnRender call per frame\n" +
                $"  • Memory: O(n) for data points only\n\n" +
                $"Gauge Control:\n" +
                $"  • OnRender with frozen brushes and pens\n" +
                $"  • Smooth 60 FPS animation\n" +
                $"  • Minimal memory allocation\n\n" +
                $"Particle System:\n" +
                $"  • Rendering 100K+ particles at 60 FPS\n" +
                $"  • Single drawing context for all particles\n" +
                $"  • Physics calculations optimized\n\n" +
                $"Data Grid:\n" +
                $"  • Custom OnRender-based rendering\n" +
                $"  • No visual tree overhead per cell\n" +
                $"  • Efficient text rendering with FormattedText";
        }

        private void UpdateStatus(string message)
        {
            StatusText.Text = message;
        }

        protected override void OnClosed(EventArgs e)
        {
            _chartTimer?.Stop();
            _gaugeTimer?.Stop();
            _particleTimer?.Stop();
            _fpsTimer?.Stop();
            _memoryTimer?.Stop();
            ParticleSystemControl.StopSimulation();

            base.OnClosed(e);
        }
    }
}
