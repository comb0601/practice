using System.Windows;
using System.Windows.Media.Animation;
using System.Windows.Threading;

namespace Lesson01_FirstScene;

public partial class MainWindow : Window
{
    private Storyboard? rotationStoryboard;
    private bool isRotating = false;
    private DispatcherTimer? updateTimer;

    public MainWindow()
    {
        InitializeComponent();

        // Initialize the rotation storyboard
        rotationStoryboard = (Storyboard)FindResource("RotationStoryboard");

        // Timer to update the rotation angle display
        updateTimer = new DispatcherTimer
        {
            Interval = TimeSpan.FromMilliseconds(50)
        };
        updateTimer.Tick += UpdateTimer_Tick;
        updateTimer.Start();
    }

    private void UpdateTimer_Tick(object? sender, EventArgs e)
    {
        // Update the rotation angle display
        double angle = rotation.Angle % 360;
        rotationAngleText.Text = $"{angle:F1}°";
    }

    private void ToggleRotation_Click(object sender, RoutedEventArgs e)
    {
        if (isRotating)
        {
            rotationStoryboard?.Pause(this);
            btnToggleRotation.Content = "Resume Rotation";
        }
        else
        {
            if (rotationStoryboard != null)
            {
                if (rotation.Angle == 0)
                {
                    rotationStoryboard.Begin(this, true);
                }
                else
                {
                    rotationStoryboard.Resume(this);
                }
            }
            btnToggleRotation.Content = "Pause Rotation";
        }
        isRotating = !isRotating;
    }

    private void Reset_Click(object sender, RoutedEventArgs e)
    {
        rotationStoryboard?.Stop(this);
        rotation.Angle = 0;
        isRotating = false;
        btnToggleRotation.Content = "Start Rotation";
    }

    protected override void OnClosed(EventArgs e)
    {
        updateTimer?.Stop();
        base.OnClosed(e);
    }
}
