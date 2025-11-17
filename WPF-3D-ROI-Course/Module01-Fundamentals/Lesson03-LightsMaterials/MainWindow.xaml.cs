using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Media3D;

namespace Lesson03_LightsMaterials;

public partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();
        CreateSpheres();
        UpdateActiveLightsCount();
    }

    private void CreateSpheres()
    {
        // Create sphere geometry
        var sphereGeometry = CreateSphereGeometry(1.0, 30, 30);

        // Assign to all spheres
        diffuseSphere.Geometry = sphereGeometry;
        specularSphere.Geometry = sphereGeometry;
        emissiveSphere.Geometry = sphereGeometry;
        combinedSphere.Geometry = sphereGeometry;
        metallicSphere.Geometry = sphereGeometry;
    }

    private MeshGeometry3D CreateSphereGeometry(double radius, int latitudes, int longitudes)
    {
        var mesh = new MeshGeometry3D();

        for (int lat = 0; lat <= latitudes; lat++)
        {
            double theta = lat * Math.PI / latitudes;
            double sinTheta = Math.Sin(theta);
            double cosTheta = Math.Cos(theta);

            for (int lon = 0; lon <= longitudes; lon++)
            {
                double phi = lon * 2 * Math.PI / longitudes;
                double sinPhi = Math.Sin(phi);
                double cosPhi = Math.Cos(phi);

                double x = cosPhi * sinTheta;
                double y = cosTheta;
                double z = sinPhi * sinTheta;

                mesh.Positions.Add(new Point3D(radius * x, radius * y, radius * z));
            }
        }

        for (int lat = 0; lat < latitudes; lat++)
        {
            for (int lon = 0; lon < longitudes; lon++)
            {
                int first = lat * (longitudes + 1) + lon;
                int second = first + longitudes + 1;

                mesh.TriangleIndices.Add(first);
                mesh.TriangleIndices.Add(second);
                mesh.TriangleIndices.Add(first + 1);

                mesh.TriangleIndices.Add(second);
                mesh.TriangleIndices.Add(second + 1);
                mesh.TriangleIndices.Add(first + 1);
            }
        }

        return mesh;
    }

    private void Light_Changed(object sender, RoutedEventArgs e)
    {
        ambientLightModel.Visibility = chkAmbient.IsChecked == true ? Visibility.Visible : Visibility.Collapsed;
        directionalLightModel.Visibility = chkDirectional.IsChecked == true ? Visibility.Visible : Visibility.Collapsed;
        pointLightModel.Visibility = chkPoint.IsChecked == true ? Visibility.Visible : Visibility.Collapsed;
        spotLightModel.Visibility = chkSpot.IsChecked == true ? Visibility.Visible : Visibility.Collapsed;

        UpdateActiveLightsCount();
    }

    private void UpdateActiveLightsCount()
    {
        int count = 0;
        if (chkAmbient?.IsChecked == true) count++;
        if (chkDirectional?.IsChecked == true) count++;
        if (chkPoint?.IsChecked == true) count++;
        if (chkSpot?.IsChecked == true) count++;

        if (txtActiveLights != null)
            txtActiveLights.Text = count.ToString();
    }

    private void AmbientIntensity_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
    {
        if (ambientLight != null)
        {
            byte intensity = (byte)(sliderAmbient.Value * 255);
            ambientLight.Color = Color.FromRgb(intensity, intensity, intensity);
        }
    }

    private void DirectionalIntensity_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
    {
        if (directionalLight != null)
        {
            byte intensity = (byte)(Math.Min(sliderDirectional.Value, 1.0) * 255);
            directionalLight.Color = Color.FromRgb(intensity, intensity, intensity);
        }
    }

    private void PointPosition_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
    {
        if (pointLight != null)
        {
            pointLight.Position = new Point3D(sliderPointX.Value, 2, 3);
        }
    }

    private void SpotAngle_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
    {
        if (spotLight != null)
        {
            spotLight.OuterConeAngle = sliderSpotAngle.Value;
            spotLight.InnerConeAngle = sliderSpotAngle.Value * 0.75;
        }
    }

    private void SpecularPower_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
    {
        if (specularMaterial != null)
        {
            specularMaterial.SpecularPower = sliderSpecularPower.Value;
        }
    }

    private void Emissive_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
    {
        if (emissiveMaterial != null)
        {
            byte intensity = (byte)(sliderEmissive.Value * 255);
            emissiveMaterial.Brush = new SolidColorBrush(Color.FromRgb(intensity, (byte)(intensity * 0.4), 0));
        }
    }
}
