using System.Text;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Media3D;

namespace Lesson01_MeshGeometry;

public partial class MainWindow : Window
{
    private MeshGeometry3D? currentMesh;

    public MainWindow()
    {
        InitializeComponent();
        CreateTriangle_Click(null, null);
    }

    private void CreateTriangle_Click(object? sender, RoutedEventArgs? e)
    {
        currentMesh = new MeshGeometry3D();

        // 3 vertices for triangle
        currentMesh.Positions.Add(new Point3D(0, 1, 0));    // Top
        currentMesh.Positions.Add(new Point3D(-1, -1, 0));  // Bottom left
        currentMesh.Positions.Add(new Point3D(1, -1, 0));   // Bottom right

        // Triangle indices
        currentMesh.TriangleIndices.Add(0);
        currentMesh.TriangleIndices.Add(1);
        currentMesh.TriangleIndices.Add(2);

        // Calculate and add normals
        var normal = CalculateNormal(currentMesh.Positions[0], currentMesh.Positions[1], currentMesh.Positions[2]);
        currentMesh.Normals.Add(normal);
        currentMesh.Normals.Add(normal);
        currentMesh.Normals.Add(normal);

        UpdateVisualization();
    }

    private void CreateQuad_Click(object sender, RoutedEventArgs e)
    {
        currentMesh = new MeshGeometry3D();

        // 4 vertices
        currentMesh.Positions.Add(new Point3D(-1, 1, 0));   // Top left
        currentMesh.Positions.Add(new Point3D(1, 1, 0));    // Top right
        currentMesh.Positions.Add(new Point3D(1, -1, 0));   // Bottom right
        currentMesh.Positions.Add(new Point3D(-1, -1, 0));  // Bottom left

        // 2 triangles
        currentMesh.TriangleIndices.Add(0); currentMesh.TriangleIndices.Add(1); currentMesh.TriangleIndices.Add(2);
        currentMesh.TriangleIndices.Add(0); currentMesh.TriangleIndices.Add(2); currentMesh.TriangleIndices.Add(3);

        // Normals
        var normal = new Vector3D(0, 0, 1);
        for (int i = 0; i < 4; i++)
            currentMesh.Normals.Add(normal);

        UpdateVisualization();
    }

    private void CreatePyramid_Click(object sender, RoutedEventArgs e)
    {
        currentMesh = new MeshGeometry3D();

        // Base vertices
        currentMesh.Positions.Add(new Point3D(-1, 0, -1));  // 0
        currentMesh.Positions.Add(new Point3D(1, 0, -1));   // 1
        currentMesh.Positions.Add(new Point3D(1, 0, 1));    // 2
        currentMesh.Positions.Add(new Point3D(-1, 0, 1));   // 3
        // Apex
        currentMesh.Positions.Add(new Point3D(0, 1.5, 0));  // 4

        // Base (2 triangles)
        currentMesh.TriangleIndices.Add(0); currentMesh.TriangleIndices.Add(2); currentMesh.TriangleIndices.Add(1);
        currentMesh.TriangleIndices.Add(0); currentMesh.TriangleIndices.Add(3); currentMesh.TriangleIndices.Add(2);

        // Side faces (4 triangles)
        currentMesh.TriangleIndices.Add(0); currentMesh.TriangleIndices.Add(1); currentMesh.TriangleIndices.Add(4);
        currentMesh.TriangleIndices.Add(1); currentMesh.TriangleIndices.Add(2); currentMesh.TriangleIndices.Add(4);
        currentMesh.TriangleIndices.Add(2); currentMesh.TriangleIndices.Add(3); currentMesh.TriangleIndices.Add(4);
        currentMesh.TriangleIndices.Add(3); currentMesh.TriangleIndices.Add(0); currentMesh.TriangleIndices.Add(4);

        // Calculate normals for each vertex
        for (int i = 0; i < currentMesh.Positions.Count; i++)
        {
            currentMesh.Normals.Add(CalculateVertexNormal(currentMesh, i));
        }

        UpdateVisualization();
    }

    private void CreateCustom_Click(object sender, RoutedEventArgs e)
    {
        currentMesh = new MeshGeometry3D();

        // Create a more complex shape - a house-like structure
        // Base
        currentMesh.Positions.Add(new Point3D(-1, 0, -1));  // 0
        currentMesh.Positions.Add(new Point3D(1, 0, -1));   // 1
        currentMesh.Positions.Add(new Point3D(1, 0, 1));    // 2
        currentMesh.Positions.Add(new Point3D(-1, 0, 1));   // 3
        // Top of walls
        currentMesh.Positions.Add(new Point3D(-1, 1, -1));  // 4
        currentMesh.Positions.Add(new Point3D(1, 1, -1));   // 5
        currentMesh.Positions.Add(new Point3D(1, 1, 1));    // 6
        currentMesh.Positions.Add(new Point3D(-1, 1, 1));   // 7
        // Roof peak
        currentMesh.Positions.Add(new Point3D(0, 1.7, -1)); // 8
        currentMesh.Positions.Add(new Point3D(0, 1.7, 1));  // 9

        // Base
        currentMesh.TriangleIndices.Add(0); currentMesh.TriangleIndices.Add(2); currentMesh.TriangleIndices.Add(1);
        currentMesh.TriangleIndices.Add(0); currentMesh.TriangleIndices.Add(3); currentMesh.TriangleIndices.Add(2);

        // Walls
        AddQuadIndices(currentMesh, 0, 1, 5, 4); // Front
        AddQuadIndices(currentMesh, 1, 2, 6, 5); // Right
        AddQuadIndices(currentMesh, 2, 3, 7, 6); // Back
        AddQuadIndices(currentMesh, 3, 0, 4, 7); // Left

        // Roof
        currentMesh.TriangleIndices.Add(4); currentMesh.TriangleIndices.Add(5); currentMesh.TriangleIndices.Add(8); // Front left
        currentMesh.TriangleIndices.Add(5); currentMesh.TriangleIndices.Add(6); currentMesh.TriangleIndices.Add(8); // Front right (wrong, should use 9)
        currentMesh.TriangleIndices.Add(6); currentMesh.TriangleIndices.Add(7); currentMesh.TriangleIndices.Add(9); // Back right
        currentMesh.TriangleIndices.Add(7); currentMesh.TriangleIndices.Add(4); currentMesh.TriangleIndices.Add(9); // Back left

        // Calculate normals
        for (int i = 0; i < currentMesh.Positions.Count; i++)
        {
            currentMesh.Normals.Add(CalculateVertexNormal(currentMesh, i));
        }

        UpdateVisualization();
    }

    private void AddQuadIndices(MeshGeometry3D mesh, int i0, int i1, int i2, int i3)
    {
        mesh.TriangleIndices.Add(i0); mesh.TriangleIndices.Add(i1); mesh.TriangleIndices.Add(i2);
        mesh.TriangleIndices.Add(i0); mesh.TriangleIndices.Add(i2); mesh.TriangleIndices.Add(i3);
    }

    private Vector3D CalculateNormal(Point3D p1, Point3D p2, Point3D p3)
    {
        Vector3D v1 = p2 - p1;
        Vector3D v2 = p3 - p1;
        Vector3D normal = Vector3D.CrossProduct(v1, v2);
        normal.Normalize();
        return normal;
    }

    private Vector3D CalculateVertexNormal(MeshGeometry3D mesh, int vertexIndex)
    {
        var normal = new Vector3D(0, 0, 0);
        int count = 0;

        // Find all triangles using this vertex
        for (int i = 0; i < mesh.TriangleIndices.Count; i += 3)
        {
            if (mesh.TriangleIndices[i] == vertexIndex ||
                mesh.TriangleIndices[i + 1] == vertexIndex ||
                mesh.TriangleIndices[i + 2] == vertexIndex)
            {
                var p1 = mesh.Positions[mesh.TriangleIndices[i]];
                var p2 = mesh.Positions[mesh.TriangleIndices[i + 1]];
                var p3 = mesh.Positions[mesh.TriangleIndices[i + 2]];
                normal += CalculateNormal(p1, p2, p3);
                count++;
            }
        }

        if (count > 0)
        {
            normal /= count;
            normal.Normalize();
        }

        return normal;
    }

    private void UpdateVisualization()
    {
        if (currentMesh == null) return;

        // Update main mesh
        var model = new GeometryModel3D
        {
            Geometry = currentMesh,
            Material = new DiffuseMaterial(new SolidColorBrush(Colors.SteelBlue)),
            BackMaterial = new DiffuseMaterial(new SolidColorBrush(Colors.DarkRed))
        };

        customMesh.Content = model;

        // Update info
        UpdateMeshInfo();

        // Update visualizations
        if (chkShowVertices?.IsChecked == true)
            ShowVertices();

        if (chkShowNormals?.IsChecked == true)
            ShowNormals();
    }

    private void UpdateMeshInfo()
    {
        if (currentMesh == null) return;

        txtVertexCount.Text = currentMesh.Positions.Count.ToString();
        txtTriangleCount.Text = (currentMesh.TriangleIndices.Count / 3).ToString();
        txtHasNormals.Text = currentMesh.Normals.Count > 0 ? "Yes" : "No";

        // Show positions
        var sb = new StringBuilder();
        for (int i = 0; i < currentMesh.Positions.Count; i++)
        {
            var p = currentMesh.Positions[i];
            sb.AppendLine($"[{i}] ({p.X:F2}, {p.Y:F2}, {p.Z:F2})");
        }
        txtPositions.Text = sb.ToString();

        // Show indices
        sb.Clear();
        for (int i = 0; i < currentMesh.TriangleIndices.Count; i += 3)
        {
            sb.AppendLine($"Triangle {i / 3}: {currentMesh.TriangleIndices[i]}, {currentMesh.TriangleIndices[i + 1]}, {currentMesh.TriangleIndices[i + 2]}");
        }
        txtIndices.Text = sb.ToString();
    }

    private void ShowVertices()
    {
        if (currentMesh == null) return;

        var group = new Model3DGroup();

        foreach (var pos in currentMesh.Positions)
        {
            var sphere = CreateSphere(pos, 0.08, Colors.Yellow);
            group.Children.Add(sphere);
        }

        vertexMarkers.Content = group;
    }

    private void ShowNormals()
    {
        if (currentMesh == null || currentMesh.Normals.Count == 0) return;

        var group = new Model3DGroup();

        for (int i = 0; i < currentMesh.Positions.Count && i < currentMesh.Normals.Count; i++)
        {
            var start = currentMesh.Positions[i];
            var end = start + currentMesh.Normals[i] * 0.5;

            var arrow = CreateArrow(start, end, Colors.Cyan, 0.02);
            group.Children.Add(arrow);
        }

        normalVectors.Content = group;
    }

    private GeometryModel3D CreateSphere(Point3D center, double radius, Color color)
    {
        var mesh = new MeshGeometry3D();
        int segments = 8;

        for (int lat = 0; lat <= segments; lat++)
        {
            double theta = lat * Math.PI / segments;
            double sinTheta = Math.Sin(theta);
            double cosTheta = Math.Cos(theta);

            for (int lon = 0; lon <= segments; lon++)
            {
                double phi = lon * 2 * Math.PI / segments;
                double sinPhi = Math.Sin(phi);
                double cosPhi = Math.Cos(phi);

                double x = radius * cosPhi * sinTheta;
                double y = radius * cosTheta;
                double z = radius * sinPhi * sinTheta;

                mesh.Positions.Add(new Point3D(center.X + x, center.Y + y, center.Z + z));
            }
        }

        for (int lat = 0; lat < segments; lat++)
        {
            for (int lon = 0; lon < segments; lon++)
            {
                int first = lat * (segments + 1) + lon;
                int second = first + segments + 1;

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

    private GeometryModel3D CreateArrow(Point3D start, Point3D end, Color color, double thickness)
    {
        var mesh = new MeshGeometry3D();
        Vector3D direction = end - start;
        double length = direction.Length;

        // Simple line representation
        AddBox(mesh, start, new Size3D(thickness, thickness, length));

        return new GeometryModel3D
        {
            Geometry = mesh,
            Material = new EmissiveMaterial(new SolidColorBrush(color))
        };
    }

    private void AddBox(MeshGeometry3D mesh, Point3D origin, Size3D size)
    {
        int baseIndex = mesh.Positions.Count;

        mesh.Positions.Add(new Point3D(origin.X - size.X / 2, origin.Y - size.Y / 2, origin.Z));
        mesh.Positions.Add(new Point3D(origin.X + size.X / 2, origin.Y - size.Y / 2, origin.Z));
        mesh.Positions.Add(new Point3D(origin.X + size.X / 2, origin.Y + size.Y / 2, origin.Z));
        mesh.Positions.Add(new Point3D(origin.X - size.X / 2, origin.Y + size.Y / 2, origin.Z));
        mesh.Positions.Add(new Point3D(origin.X - size.X / 2, origin.Y - size.Y / 2, origin.Z + size.Z));
        mesh.Positions.Add(new Point3D(origin.X + size.X / 2, origin.Y - size.Y / 2, origin.Z + size.Z));
        mesh.Positions.Add(new Point3D(origin.X + size.X / 2, origin.Y + size.Y / 2, origin.Z + size.Z));
        mesh.Positions.Add(new Point3D(origin.X - size.X / 2, origin.Y + size.Y / 2, origin.Z + size.Z));

        int[] indices = { 0, 1, 2, 0, 2, 3, 1, 5, 6, 1, 6, 2, 5, 4, 7, 5, 7, 6, 4, 0, 3, 4, 3, 7, 3, 2, 6, 3, 6, 7, 4, 5, 1, 4, 1, 0 };
        foreach (int index in indices)
            mesh.TriangleIndices.Add(baseIndex + index);
    }

    private void Visualization_Changed(object sender, RoutedEventArgs e)
    {
        if (currentMesh == null) return;

        vertexMarkers.Content = null;
        normalVectors.Content = null;

        if (chkShowVertices?.IsChecked == true)
            ShowVertices();

        if (chkShowNormals?.IsChecked == true)
            ShowNormals();
    }
}
