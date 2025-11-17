using System.Windows.Media;
using System.Windows.Media.Media3D;

namespace Lesson02_BoundingBoxROI;

/// <summary>
/// Creates 3D visualizations for ROI objects
/// </summary>
public class ROIVisualizer
{
    /// <summary>
    /// Create a complete visualization for an ROI including wireframe and handles
    /// </summary>
    public Model3DGroup CreateROIVisualization(ROI3D roi)
    {
        var group = new Model3DGroup();

        // Create wireframe edges
        group.Children.Add(CreateWireframeBox(roi));

        // Create semi-transparent faces if selected
        if (roi.IsSelected)
        {
            group.Children.Add(CreateTransparentBox(roi, 0.15));
        }

        // Create corner handles
        group.Children.Add(CreateCornerHandles(roi));

        return group;
    }

    /// <summary>
    /// Create wireframe representation of the ROI box
    /// </summary>
    private Model3D CreateWireframeBox(ROI3D roi)
    {
        var group = new Model3DGroup();
        double thickness = 0.03;
        Color color = roi.IsSelected ? Colors.Yellow : roi.VisualizationColor;

        Point3D[] corners = roi.GetCorners();

        // Bottom face edges (4 edges)
        group.Children.Add(CreateLine(corners[0], corners[1], color, thickness));
        group.Children.Add(CreateLine(corners[1], corners[2], color, thickness));
        group.Children.Add(CreateLine(corners[2], corners[3], color, thickness));
        group.Children.Add(CreateLine(corners[3], corners[0], color, thickness));

        // Top face edges (4 edges)
        group.Children.Add(CreateLine(corners[4], corners[5], color, thickness));
        group.Children.Add(CreateLine(corners[5], corners[6], color, thickness));
        group.Children.Add(CreateLine(corners[6], corners[7], color, thickness));
        group.Children.Add(CreateLine(corners[7], corners[4], color, thickness));

        // Vertical edges (4 edges)
        group.Children.Add(CreateLine(corners[0], corners[4], color, thickness));
        group.Children.Add(CreateLine(corners[1], corners[5], color, thickness));
        group.Children.Add(CreateLine(corners[2], corners[6], color, thickness));
        group.Children.Add(CreateLine(corners[3], corners[7], color, thickness));

        return group;
    }

    /// <summary>
    /// Create a line segment between two points
    /// </summary>
    private GeometryModel3D CreateLine(Point3D start, Point3D end, Color color, double thickness)
    {
        var mesh = new MeshGeometry3D();

        // Create a thin box representing the line
        Vector3D direction = end - start;
        double length = direction.Length;
        direction.Normalize();

        // Determine orientation and create appropriate box
        Vector3D offset = direction * length;

        if (Math.Abs(direction.X) > 0.9) // X-aligned
        {
            AddBox(mesh, start, new Size3D(length, thickness, thickness));
        }
        else if (Math.Abs(direction.Y) > 0.9) // Y-aligned
        {
            AddBox(mesh, start, new Size3D(thickness, length, thickness));
        }
        else // Z-aligned
        {
            AddBox(mesh, start, new Size3D(thickness, thickness, length));
        }

        return new GeometryModel3D
        {
            Geometry = mesh,
            Material = new DiffuseMaterial(new SolidColorBrush(color))
        };
    }

    /// <summary>
    /// Create transparent box faces for selected ROI
    /// </summary>
    private GeometryModel3D CreateTransparentBox(ROI3D roi, double opacity)
    {
        var mesh = new MeshGeometry3D();
        Point3D[] corners = roi.GetCorners();

        // Add all 6 faces
        // Front face
        AddQuad(mesh, corners[0], corners[1], corners[2], corners[3]);
        // Back face
        AddQuad(mesh, corners[5], corners[4], corners[7], corners[6]);
        // Left face
        AddQuad(mesh, corners[4], corners[0], corners[3], corners[7]);
        // Right face
        AddQuad(mesh, corners[1], corners[5], corners[6], corners[2]);
        // Top face
        AddQuad(mesh, corners[3], corners[2], corners[6], corners[7]);
        // Bottom face
        AddQuad(mesh, corners[4], corners[5], corners[1], corners[0]);

        var color = roi.VisualizationColor;
        var brush = new SolidColorBrush(Color.FromArgb((byte)(opacity * 255), color.R, color.G, color.B));

        return new GeometryModel3D
        {
            Geometry = mesh,
            Material = new DiffuseMaterial(brush),
            BackMaterial = new DiffuseMaterial(brush)
        };
    }

    /// <summary>
    /// Create small spheres at each corner as manipulation handles
    /// </summary>
    private Model3D CreateCornerHandles(ROI3D roi)
    {
        var group = new Model3DGroup();
        Point3D[] corners = roi.GetCorners();
        double handleSize = 0.15;

        Color handleColor = roi.IsSelected ? Colors.White : Colors.Gray;

        foreach (var corner in corners)
        {
            var handle = CreateSphere(corner, handleSize, handleColor);
            group.Children.Add(handle);
        }

        return group;
    }

    /// <summary>
    /// Create a sphere at the specified position
    /// </summary>
    private GeometryModel3D CreateSphere(Point3D center, double radius, Color color)
    {
        var mesh = new MeshGeometry3D();
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

    /// <summary>
    /// Add a box to the mesh
    /// </summary>
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

    /// <summary>
    /// Add a quadrilateral to the mesh
    /// </summary>
    private void AddQuad(MeshGeometry3D mesh, Point3D p1, Point3D p2, Point3D p3, Point3D p4)
    {
        int baseIndex = mesh.Positions.Count;

        mesh.Positions.Add(p1);
        mesh.Positions.Add(p2);
        mesh.Positions.Add(p3);
        mesh.Positions.Add(p4);

        // First triangle
        mesh.TriangleIndices.Add(baseIndex);
        mesh.TriangleIndices.Add(baseIndex + 1);
        mesh.TriangleIndices.Add(baseIndex + 2);

        // Second triangle
        mesh.TriangleIndices.Add(baseIndex);
        mesh.TriangleIndices.Add(baseIndex + 2);
        mesh.TriangleIndices.Add(baseIndex + 3);
    }
}
