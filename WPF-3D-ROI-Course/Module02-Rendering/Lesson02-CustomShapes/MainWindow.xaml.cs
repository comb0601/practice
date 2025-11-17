using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Media3D;

namespace Lesson02_CustomShapes;

public partial class MainWindow : Window
{
    private string currentShape = "Cylinder";
    private Storyboard? rotationStoryboard;

    public MainWindow()
    {
        InitializeComponent();
        rotationStoryboard = (Storyboard)FindResource("RotationStoryboard");
        rotationStoryboard.Begin(this, true);
        Cylinder_Click(null, null);
    }

    private void Cylinder_Click(object? sender, RoutedEventArgs? e)
    {
        currentShape = "Cylinder";
        UpdateShape();
    }

    private void Cone_Click(object sender, RoutedEventArgs e)
    {
        currentShape = "Cone";
        UpdateShape();
    }

    private void Torus_Click(object sender, RoutedEventArgs e)
    {
        currentShape = "Torus";
        UpdateShape();
    }

    private void Sphere_Click(object sender, RoutedEventArgs e)
    {
        currentShape = "Sphere";
        UpdateShape();
    }

    private void Prism6_Click(object sender, RoutedEventArgs e)
    {
        currentShape = "Prism6";
        UpdateShape();
    }

    private void Prism8_Click(object sender, RoutedEventArgs e)
    {
        currentShape = "Prism8";
        UpdateShape();
    }

    private void Parameters_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
    {
        UpdateShape();
    }

    private void UpdateShape()
    {
        if (sliderSegments == null || sliderRadius == null || sliderHeight == null)
            return;

        int segments = (int)sliderSegments.Value;
        double radius = sliderRadius.Value;
        double height = sliderHeight.Value;

        MeshGeometry3D? mesh = null;

        switch (currentShape)
        {
            case "Cylinder":
                mesh = ShapeGenerator.CreateCylinder(radius, height, segments);
                txtShapeName.Text = "Cylinder";
                txtShapeInfo.Text = $"Radius: {radius:F1}, Height: {height:F1}, Segments: {segments}\n" +
                                   $"Vertices: {mesh.Positions.Count}, Triangles: {mesh.TriangleIndices.Count / 3}";
                break;

            case "Cone":
                mesh = ShapeGenerator.CreateCone(radius, height, segments);
                txtShapeName.Text = "Cone";
                txtShapeInfo.Text = $"Base Radius: {radius:F1}, Height: {height:F1}, Segments: {segments}\n" +
                                   $"Vertices: {mesh.Positions.Count}, Triangles: {mesh.TriangleIndices.Count / 3}";
                break;

            case "Torus":
                double minorRadius = radius * 0.3;
                mesh = ShapeGenerator.CreateTorus(radius, minorRadius, segments, segments / 2);
                txtShapeName.Text = "Torus";
                txtShapeInfo.Text = $"Major: {radius:F1}, Minor: {minorRadius:F1}, Segments: {segments}\n" +
                                   $"Vertices: {mesh.Positions.Count}, Triangles: {mesh.TriangleIndices.Count / 3}";
                break;

            case "Sphere":
                mesh = ShapeGenerator.CreateSphere(radius, segments / 2, segments);
                txtShapeName.Text = "Sphere";
                txtShapeInfo.Text = $"Radius: {radius:F1}, Lat/Lon Segments: {segments}\n" +
                                   $"Vertices: {mesh.Positions.Count}, Triangles: {mesh.TriangleIndices.Count / 3}";
                break;

            case "Prism6":
                mesh = ShapeGenerator.CreatePrism(6, radius, height);
                txtShapeName.Text = "Hexagonal Prism";
                txtShapeInfo.Text = $"Sides: 6, Radius: {radius:F1}, Height: {height:F1}\n" +
                                   $"Vertices: {mesh.Positions.Count}, Triangles: {mesh.TriangleIndices.Count / 3}";
                break;

            case "Prism8":
                mesh = ShapeGenerator.CreatePrism(8, radius, height);
                txtShapeName.Text = "Octagonal Prism";
                txtShapeInfo.Text = $"Sides: 8, Radius: {radius:F1}, Height: {height:F1}\n" +
                                   $"Vertices: {mesh.Positions.Count}, Triangles: {mesh.TriangleIndices.Count / 3}";
                break;
        }

        if (mesh != null)
        {
            var material = new MaterialGroup();
            material.Children.Add(new DiffuseMaterial(new SolidColorBrush(Colors.SteelBlue)));
            material.Children.Add(new SpecularMaterial(Brushes.White, 60));

            shapeModel.Content = new GeometryModel3D
            {
                Geometry = mesh,
                Material = material,
                BackMaterial = new DiffuseMaterial(new SolidColorBrush(Colors.DarkRed))
            };
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

    private void Wireframe_Changed(object sender, RoutedEventArgs e)
    {
        // Wireframe would require more complex implementation
        // For now, we'll just change the material
        UpdateShape();
    }
}
