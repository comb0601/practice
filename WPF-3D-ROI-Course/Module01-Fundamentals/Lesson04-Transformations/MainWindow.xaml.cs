using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Animation;

namespace Lesson04_Transformations;

public partial class MainWindow : Window
{
    private Storyboard? animationStoryboard;
    private bool isAnimating = false;

    public MainWindow()
    {
        InitializeComponent();
        animationStoryboard = (Storyboard)FindResource("AnimationStoryboard");
    }

    private void Transform_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
    {
        if (translateTransform == null || rotationX == null || rotationY == null || rotationZ == null)
            return;

        // Update transforms
        translateTransform.OffsetX = sliderTranslateX.Value;
        translateTransform.OffsetY = sliderTranslateY.Value;
        translateTransform.OffsetZ = sliderTranslateZ.Value;

        rotationX.Angle = sliderRotateX.Value;
        rotationY.Angle = sliderRotateY.Value;
        rotationZ.Angle = sliderRotateZ.Value;
    }

    private void Scale_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
    {
        if (scaleTransform == null)
            return;

        if (chkUniformScale?.IsChecked == true)
        {
            // Uniform scaling - all axes same
            double scale = ((Slider)sender).Value;
            scaleTransform.ScaleX = scale;
            scaleTransform.ScaleY = scale;
            scaleTransform.ScaleZ = scale;

            // Update other sliders
            sliderScaleX.Value = scale;
            sliderScaleY.Value = scale;
            sliderScaleZ.Value = scale;
        }
        else
        {
            // Non-uniform scaling
            scaleTransform.ScaleX = sliderScaleX.Value;
            scaleTransform.ScaleY = sliderScaleY.Value;
            scaleTransform.ScaleZ = sliderScaleZ.Value;
        }
    }

    private void Animate_Click(object sender, RoutedEventArgs e)
    {
        if (isAnimating)
        {
            animationStoryboard?.Pause(this);
            btnAnimate.Content = "Resume Animation";
        }
        else
        {
            if (animationStoryboard != null)
            {
                if (rotationY.Angle == 0)
                {
                    animationStoryboard.Begin(this, true);
                }
                else
                {
                    animationStoryboard.Resume(this);
                }
            }
            btnAnimate.Content = "Pause Animation";
        }
        isAnimating = !isAnimating;
    }

    private void Reset_Click(object sender, RoutedEventArgs e)
    {
        // Stop animation
        animationStoryboard?.Stop(this);
        isAnimating = false;
        btnAnimate.Content = "Start Animation";

        // Reset all transforms
        sliderTranslateX.Value = 0;
        sliderTranslateY.Value = 0;
        sliderTranslateZ.Value = 0;

        sliderRotateX.Value = 0;
        sliderRotateY.Value = 0;
        sliderRotateZ.Value = 0;

        sliderScaleX.Value = 1;
        sliderScaleY.Value = 1;
        sliderScaleZ.Value = 1;
    }
}
