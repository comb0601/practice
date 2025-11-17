using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Media3D;

namespace Lesson03_Texturing;

public partial class MainWindow : Window
{
    private Storyboard? rotationStoryboard;
    private Brush currentTexture = Brushes.White;

    public MainWindow()
    {
        InitializeComponent();
        rotationStoryboard = (Storyboard)FindResource("RotationStoryboard");
        rotationStoryboard.Begin(this, true);

        Checkerboard_Click(null, null);
        TexturedBox_Click(null, null);
    }

    private void Checkerboard_Click(object? sender, RoutedEventArgs? e)
    {
        var drawing = new DrawingBrush
        {
            Viewport = new Rect(0, 0, 0.25, 0.25),
            TileMode = TileMode.Tile
        };

        var group = new DrawingGroup();
        group.Children.Add(new GeometryDrawing(Brushes.White, null, new RectangleGeometry(new Rect(0, 0, 1, 1))));
        group.Children.Add(new GeometryDrawing(Brushes.Black, null, new RectangleGeometry(new Rect(0, 0, 0.5, 0.5))));
        group.Children.Add(new GeometryDrawing(Brushes.Black, null, new RectangleGeometry(new Rect(0.5, 0.5, 0.5, 0.5))));

        drawing.Drawing = group;
        currentTexture = drawing;
        UpdateModel();
    }

    private void Grid_Click(object sender, RoutedEventArgs e)
    {
        var drawing = new DrawingBrush
        {
            Viewport = new Rect(0, 0, 0.1, 0.1),
            TileMode = TileMode.Tile
        };

        var group = new DrawingGroup();
        group.Children.Add(new GeometryDrawing(Brushes.LightGray, null, new RectangleGeometry(new Rect(0, 0, 1, 1))));
        group.Children.Add(new GeometryDrawing(null, new Pen(Brushes.Blue, 0.1), new RectangleGeometry(new Rect(0, 0, 1, 1))));

        drawing.Drawing = group;
        currentTexture = drawing;
        UpdateModel();
    }

    private void Gradient_Click(object sender, RoutedEventArgs e)
    {
        currentTexture = new LinearGradientBrush(
            Colors.Blue, Colors.Red, new Point(0, 0), new Point(1, 1));
        UpdateModel();
    }

    private void Radial_Click(object sender, RoutedEventArgs e)
    {
        currentTexture = new RadialGradientBrush(Colors.Yellow, Colors.Red);
        UpdateModel();
    }

    private void MultiColor_Click(object sender, RoutedEventArgs e)
    {
        var gradient = new LinearGradientBrush();
        gradient.GradientStops.Add(new GradientStop(Colors.Red, 0.0));
        gradient.GradientStops.Add(new GradientStop(Colors.Orange, 0.2));
        gradient.GradientStops.Add(new GradientStop(Colors.Yellow, 0.4));
        gradient.GradientStops.Add(new GradientStop(Colors.Green, 0.6));
        gradient.GradientStops.Add(new GradientStop(Colors.Blue, 0.8));
        gradient.GradientStops.Add(new GradientStop(Colors.Purple, 1.0));
        currentTexture = gradient;
        UpdateModel();
    }

    private void TexturedBox_Click(object? sender, RoutedEventArgs? e)
    {
        var mesh = CreateBoxWithUVs();
        UpdateModelWithMesh(mesh);
    }

    private void TexturedCylinder_Click(object sender, RoutedEventArgs e)
    {
        var mesh = CreateCylinderWithUVs(1.0, 2.0, 24);
        UpdateModelWithMesh(mesh);
    }

    private void TexturedSphere_Click(object sender, RoutedEventArgs e)
    {
        var mesh = CreateSphereWithUVs(1.2, 20, 30);
        UpdateModelWithMesh(mesh);
    }

    private MeshGeometry3D CreateBoxWithUVs()
    {
        var mesh = new MeshGeometry3D();

        // Front face
        mesh.Positions.Add(new Point3D(-1, -1, 1));
        mesh.Positions.Add(new Point3D(1, -1, 1));
        mesh.Positions.Add(new Point3D(1, 1, 1));
        mesh.Positions.Add(new Point3D(-1, 1, 1));

        // Back face
        mesh.Positions.Add(new Point3D(1, -1, -1));
        mesh.Positions.Add(new Point3D(-1, -1, -1));
        mesh.Positions.Add(new Point3D(-1, 1, -1));
        mesh.Positions.Add(new Point3D(1, 1, -1));

        // Right face
        mesh.Positions.Add(new Point3D(1, -1, 1));
        mesh.Positions.Add(new Point3D(1, -1, -1));
        mesh.Positions.Add(new Point3D(1, 1, -1));
        mesh.Positions.Add(new Point3D(1, 1, 1));

        // Left face
        mesh.Positions.Add(new Point3D(-1, -1, -1));
        mesh.Positions.Add(new Point3D(-1, -1, 1));
        mesh.Positions.Add(new Point3D(-1, 1, 1));
        mesh.Positions.Add(new Point3D(-1, 1, -1));

        // Top face
        mesh.Positions.Add(new Point3D(-1, 1, 1));
        mesh.Positions.Add(new Point3D(1, 1, 1));
        mesh.Positions.Add(new Point3D(1, 1, -1));
        mesh.Positions.Add(new Point3D(-1, 1, -1));

        // Bottom face
        mesh.Positions.Add(new Point3D(-1, -1, -1));
        mesh.Positions.Add(new Point3D(1, -1, -1));
        mesh.Positions.Add(new Point3D(1, -1, 1));
        mesh.Positions.Add(new Point3D(-1, -1, 1));

        // UV coordinates for each face
        for (int i = 0; i < 6; i++)
        {
            mesh.TextureCoordinates.Add(new Point(0, 1));
            mesh.TextureCoordinates.Add(new Point(1, 1));
            mesh.TextureCoordinates.Add(new Point(1, 0));
            mesh.TextureCoordinates.Add(new Point(0, 0));
        }

        // Triangle indices (2 triangles per face)
        for (int i = 0; i < 6; i++)
        {
            int offset = i * 4;
            mesh.TriangleIndices.Add(offset); mesh.TriangleIndices.Add(offset + 1); mesh.TriangleIndices.Add(offset + 2);
            mesh.TriangleIndices.Add(offset); mesh.TriangleIndices.Add(offset + 2); mesh.TriangleIndices.Add(offset + 3);
        }

        return mesh;
    }

    private MeshGeometry3D CreateCylinderWithUVs(double radius, double height, int segments)
    {
        var mesh = new MeshGeometry3D();
        double angleStep = 2 * Math.PI / segments;

        // Side vertices and UVs
        for (int i = 0; i <= segments; i++)
        {
            double angle = i * angleStep;
            double u = (double)i / segments;

            mesh.Positions.Add(new Point3D(
                radius * Math.Cos(angle), 0, radius * Math.Sin(angle)));
            mesh.TextureCoordinates.Add(new Point(u, 1));

            mesh.Positions.Add(new Point3D(
                radius * Math.Cos(angle), height, radius * Math.Sin(angle)));
            mesh.TextureCoordinates.Add(new Point(u, 0));
        }

        // Side triangles
        for (int i = 0; i < segments; i++)
        {
            int bottom1 = i * 2;
            int top1 = bottom1 + 1;
            int bottom2 = (i + 1) * 2;
            int top2 = bottom2 + 1;

            mesh.TriangleIndices.Add(bottom1); mesh.TriangleIndices.Add(top1); mesh.TriangleIndices.Add(bottom2);
            mesh.TriangleIndices.Add(bottom2); mesh.TriangleIndices.Add(top1); mesh.TriangleIndices.Add(top2);
        }

        return mesh;
    }

    private MeshGeometry3D CreateSphereWithUVs(double radius, int latSegments, int lonSegments)
    {
        var mesh = new MeshGeometry3D();

        for (int lat = 0; lat <= latSegments; lat++)
        {
            double theta = lat * Math.PI / latSegments;
            double sinTheta = Math.Sin(theta);
            double cosTheta = Math.Cos(theta);

            for (int lon = 0; lon <= lonSegments; lon++)
            {
                double phi = lon * 2 * Math.PI / lonSegments;
                double sinPhi = Math.Sin(phi);
                double cosPhi = Math.Cos(phi);

                double x = radius * cosPhi * sinTheta;
                double y = radius * cosTheta;
                double z = radius * sinPhi * sinTheta;

                mesh.Positions.Add(new Point3D(x, y, z));
                mesh.TextureCoordinates.Add(new Point((double)lon / lonSegments, (double)lat / latSegments));
            }
        }

        for (int lat = 0; lat < latSegments; lat++)
        {
            for (int lon = 0; lon < lonSegments; lon++)
            {
                int first = lat * (lonSegments + 1) + lon;
                int second = first + lonSegments + 1;

                mesh.TriangleIndices.Add(first); mesh.TriangleIndices.Add(second); mesh.TriangleIndices.Add(first + 1);
                mesh.TriangleIndices.Add(second); mesh.TriangleIndices.Add(second + 1); mesh.TriangleIndices.Add(first + 1);
            }
        }

        return mesh;
    }

    private void UpdateModelWithMesh(MeshGeometry3D mesh)
    {
        var model = new GeometryModel3D
        {
            Geometry = mesh,
            Material = new DiffuseMaterial(currentTexture)
        };

        texturedModel.Content = model;
    }

    private void UpdateModel()
    {
        if (texturedModel.Content is GeometryModel3D model)
        {
            model.Material = new DiffuseMaterial(currentTexture);
        }
    }

    private void AutoRotate_Changed(object sender, RoutedEventArgs e)
    {
        if (rotationStoryboard == null) return;

        if (chkAutoRotate.IsChecked == true)
        {
            if (rotation.Angle == 0)
                rotationStoryboard.Begin(this, true);
            else
                rotationStoryboard.Resume(this);
        }
        else
        {
            rotationStoryboard.Pause(this);
        }
    }
}
