using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Media3D;

namespace Lesson02_Coordinates;

public partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();
        InitializeScene();
    }

    private void InitializeScene()
    {
        CreateCoordinateAxes();
        CreateGridPlane();
        CreateTestPoint();
    }

    private void CreateCoordinateAxes()
    {
        var axesGroup = new Model3DGroup();

        // X-Axis (Red) - Left to Right
        axesGroup.Children.Add(CreateAxisArrow(
            new Point3D(0, 0, 0),
            new Point3D(5, 0, 0),
            Colors.Red,
            0.05));

        // Y-Axis (Green) - Down to Up
        axesGroup.Children.Add(CreateAxisArrow(
            new Point3D(0, 0, 0),
            new Point3D(0, 5, 0),
            Colors.Green,
            0.05));

        // Z-Axis (Blue) - In to Out
        axesGroup.Children.Add(CreateAxisArrow(
            new Point3D(0, 0, 0),
            new Point3D(0, 0, 5),
            Colors.Blue,
            0.05));

        // Add axis labels (small spheres at the end)
        axesGroup.Children.Add(CreateSphere(new Point3D(5.5, 0, 0), 0.15, Colors.Red));
        axesGroup.Children.Add(CreateSphere(new Point3D(0, 5.5, 0), 0.15, Colors.Green));
        axesGroup.Children.Add(CreateSphere(new Point3D(0, 0, 5.5), 0.15, Colors.Blue));

        coordinateAxes.Content = axesGroup;
    }

    private GeometryModel3D CreateAxisArrow(Point3D start, Point3D end, Color color, double thickness)
    {
        var meshBuilder = new MeshGeometry3D();

        // Create a simple cylinder for the axis
        Vector3D direction = end - start;
        double length = direction.Length;
        direction.Normalize();

        // Create cylinder positions (simplified - 8 sides)
        int sides = 8;
        for (int i = 0; i <= sides; i++)
        {
            double angle = (double)i / sides * 2 * Math.PI;
            double x = Math.Cos(angle) * thickness;
            double y = Math.Sin(angle) * thickness;

            // We'll create a simple line representation
            // In a real scenario, you'd create proper cylinder geometry
        }

        // For simplicity, create a box-based axis
        double halfThickness = thickness / 2;

        if (Math.Abs(direction.X) > 0.9) // X-axis
        {
            AddBox(meshBuilder, start, new Size3D(length, thickness, thickness));
        }
        else if (Math.Abs(direction.Y) > 0.9) // Y-axis
        {
            AddBox(meshBuilder, start, new Size3D(thickness, length, thickness));
        }
        else // Z-axis
        {
            AddBox(meshBuilder, start, new Size3D(thickness, thickness, length));
        }

        return new GeometryModel3D
        {
            Geometry = meshBuilder,
            Material = new DiffuseMaterial(new SolidColorBrush(color))
        };
    }

    private void AddBox(MeshGeometry3D mesh, Point3D origin, Size3D size)
    {
        double x = origin.X;
        double y = origin.Y;
        double z = origin.Z;
        double w = size.X;
        double h = size.Y;
        double d = size.Z;

        int baseIndex = mesh.Positions.Count;

        // Add 8 vertices
        mesh.Positions.Add(new Point3D(x, y, z));
        mesh.Positions.Add(new Point3D(x + w, y, z));
        mesh.Positions.Add(new Point3D(x + w, y + h, z));
        mesh.Positions.Add(new Point3D(x, y + h, z));
        mesh.Positions.Add(new Point3D(x, y, z + d));
        mesh.Positions.Add(new Point3D(x + w, y, z + d));
        mesh.Positions.Add(new Point3D(x + w, y + h, z + d));
        mesh.Positions.Add(new Point3D(x, y + h, z + d));

        // Add 12 triangles (2 per face)
        int[] indices = new int[]
        {
            0, 1, 2,  0, 2, 3,  // Front
            1, 5, 6,  1, 6, 2,  // Right
            5, 4, 7,  5, 7, 6,  // Back
            4, 0, 3,  4, 3, 7,  // Left
            3, 2, 6,  3, 6, 7,  // Top
            4, 5, 1,  4, 1, 0   // Bottom
        };

        foreach (int index in indices)
        {
            mesh.TriangleIndices.Add(baseIndex + index);
        }
    }

    private GeometryModel3D CreateSphere(Point3D center, double radius, Color color)
    {
        var mesh = new MeshGeometry3D();

        // Create a simple icosphere approximation
        int latitudes = 10;
        int longitudes = 10;

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

                mesh.Positions.Add(new Point3D(
                    center.X + radius * x,
                    center.Y + radius * y,
                    center.Z + radius * z));
            }
        }

        // Create triangles
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

        return new GeometryModel3D
        {
            Geometry = mesh,
            Material = new DiffuseMaterial(new SolidColorBrush(color))
        };
    }

    private void CreateGridPlane()
    {
        var gridGroup = new Model3DGroup();

        // Create grid lines on XZ plane at Y=0
        int gridSize = 10;
        double spacing = 1.0;
        Color gridColor = Color.FromRgb(60, 60, 60);

        for (int i = -gridSize; i <= gridSize; i++)
        {
            // Lines parallel to X-axis
            gridGroup.Children.Add(CreateLine(
                new Point3D(-gridSize * spacing, 0, i * spacing),
                new Point3D(gridSize * spacing, 0, i * spacing),
                gridColor, 0.01));

            // Lines parallel to Z-axis
            gridGroup.Children.Add(CreateLine(
                new Point3D(i * spacing, 0, -gridSize * spacing),
                new Point3D(i * spacing, 0, gridSize * spacing),
                gridColor, 0.01));
        }

        gridPlane.Content = gridGroup;
    }

    private GeometryModel3D CreateLine(Point3D start, Point3D end, Color color, double thickness)
    {
        var mesh = new MeshGeometry3D();
        AddBox(mesh, start, new Size3D(
            Math.Abs(end.X - start.X) > 0.01 ? Math.Abs(end.X - start.X) : thickness,
            thickness,
            Math.Abs(end.Z - start.Z) > 0.01 ? Math.Abs(end.Z - start.Z) : thickness));

        return new GeometryModel3D
        {
            Geometry = mesh,
            Material = new DiffuseMaterial(new SolidColorBrush(color))
        };
    }

    private void CreateTestPoint()
    {
        var sphere = CreateSphere(new Point3D(0, 0, 0), 0.3, Colors.Yellow);

        var model = new GeometryModel3D
        {
            Geometry = sphere.Geometry,
            Material = new DiffuseMaterial(new SolidColorBrush(Colors.Yellow)),
            BackMaterial = new DiffuseMaterial(new SolidColorBrush(Colors.Orange))
        };

        testPoint.Content = model;
        testPoint.Transform = new TranslateTransform3D(0, 0, 0);
    }

    private void PositionSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
    {
        if (testPoint?.Transform is TranslateTransform3D transform)
        {
            transform.OffsetX = sliderX.Value;
            transform.OffsetY = sliderY.Value;
            transform.OffsetZ = sliderZ.Value;

            // Update status text
            if (txtX != null)
            {
                txtX.Text = sliderX.Value.ToString("F1");
                txtY.Text = sliderY.Value.ToString("F1");
                txtZ.Text = sliderZ.Value.ToString("F1");
            }
        }
    }

    private void DisplayOption_Changed(object sender, RoutedEventArgs e)
    {
        if (gridPlane != null)
            gridPlane.Visibility = chkShowGrid.IsChecked == true ? Visibility.Visible : Visibility.Collapsed;

        if (coordinateAxes != null)
            coordinateAxes.Visibility = chkShowAxes.IsChecked == true ? Visibility.Visible : Visibility.Collapsed;
    }

    private void Reset_Click(object sender, RoutedEventArgs e)
    {
        sliderX.Value = 0;
        sliderY.Value = 0;
        sliderZ.Value = 0;
    }
}
