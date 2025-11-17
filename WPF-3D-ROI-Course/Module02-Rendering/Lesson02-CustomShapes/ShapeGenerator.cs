using System.Windows.Media.Media3D;

namespace Lesson02_CustomShapes;

/// <summary>
/// Generates custom 3D shapes programmatically
/// </summary>
public static class ShapeGenerator
{
    public static MeshGeometry3D CreateCylinder(double radius, double height, int segments)
    {
        var mesh = new MeshGeometry3D();
        double angleStep = 2 * Math.PI / segments;

        // Bottom circle vertices
        for (int i = 0; i < segments; i++)
        {
            double angle = i * angleStep;
            mesh.Positions.Add(new Point3D(
                radius * Math.Cos(angle),
                0,
                radius * Math.Sin(angle)));
        }

        // Top circle vertices
        for (int i = 0; i < segments; i++)
        {
            double angle = i * angleStep;
            mesh.Positions.Add(new Point3D(
                radius * Math.Cos(angle),
                height,
                radius * Math.Sin(angle)));
        }

        // Side triangles
        for (int i = 0; i < segments; i++)
        {
            int next = (i + 1) % segments;
            int top = i + segments;
            int topNext = next + segments;

            mesh.TriangleIndices.Add(i);
            mesh.TriangleIndices.Add(top);
            mesh.TriangleIndices.Add(next);

            mesh.TriangleIndices.Add(next);
            mesh.TriangleIndices.Add(top);
            mesh.TriangleIndices.Add(topNext);
        }

        // Add centers for caps
        int bottomCenter = mesh.Positions.Count;
        mesh.Positions.Add(new Point3D(0, 0, 0));

        int topCenter = mesh.Positions.Count;
        mesh.Positions.Add(new Point3D(0, height, 0));

        // Bottom cap
        for (int i = 0; i < segments; i++)
        {
            int next = (i + 1) % segments;
            mesh.TriangleIndices.Add(bottomCenter);
            mesh.TriangleIndices.Add(next);
            mesh.TriangleIndices.Add(i);
        }

        // Top cap
        for (int i = 0; i < segments; i++)
        {
            int next = (i + 1) % segments;
            mesh.TriangleIndices.Add(topCenter);
            mesh.TriangleIndices.Add(i + segments);
            mesh.TriangleIndices.Add(next + segments);
        }

        return mesh;
    }

    public static MeshGeometry3D CreateCone(double baseRadius, double height, int segments)
    {
        var mesh = new MeshGeometry3D();
        double angleStep = 2 * Math.PI / segments;

        // Base circle vertices
        for (int i = 0; i < segments; i++)
        {
            double angle = i * angleStep;
            mesh.Positions.Add(new Point3D(
                baseRadius * Math.Cos(angle),
                0,
                baseRadius * Math.Sin(angle)));
        }

        // Apex
        int apex = mesh.Positions.Count;
        mesh.Positions.Add(new Point3D(0, height, 0));

        // Base center
        int baseCenter = mesh.Positions.Count;
        mesh.Positions.Add(new Point3D(0, 0, 0));

        // Side triangles
        for (int i = 0; i < segments; i++)
        {
            int next = (i + 1) % segments;
            mesh.TriangleIndices.Add(i);
            mesh.TriangleIndices.Add(apex);
            mesh.TriangleIndices.Add(next);
        }

        // Base cap
        for (int i = 0; i < segments; i++)
        {
            int next = (i + 1) % segments;
            mesh.TriangleIndices.Add(baseCenter);
            mesh.TriangleIndices.Add(next);
            mesh.TriangleIndices.Add(i);
        }

        return mesh;
    }

    public static MeshGeometry3D CreateTorus(double majorRadius, double minorRadius, int majorSegments, int minorSegments)
    {
        var mesh = new MeshGeometry3D();

        for (int i = 0; i <= majorSegments; i++)
        {
            double u = 2 * Math.PI * i / majorSegments;
            double cosU = Math.Cos(u);
            double sinU = Math.Sin(u);

            for (int j = 0; j <= minorSegments; j++)
            {
                double v = 2 * Math.PI * j / minorSegments;
                double cosV = Math.Cos(v);
                double sinV = Math.Sin(v);

                double x = (majorRadius + minorRadius * cosV) * cosU;
                double y = minorRadius * sinV;
                double z = (majorRadius + minorRadius * cosV) * sinU;

                mesh.Positions.Add(new Point3D(x, y, z));
            }
        }

        for (int i = 0; i < majorSegments; i++)
        {
            for (int j = 0; j < minorSegments; j++)
            {
                int current = i * (minorSegments + 1) + j;
                int next = current + minorSegments + 1;

                mesh.TriangleIndices.Add(current);
                mesh.TriangleIndices.Add(next);
                mesh.TriangleIndices.Add(current + 1);

                mesh.TriangleIndices.Add(current + 1);
                mesh.TriangleIndices.Add(next);
                mesh.TriangleIndices.Add(next + 1);
            }
        }

        return mesh;
    }

    public static MeshGeometry3D CreateSphere(double radius, int latSegments, int lonSegments)
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
            }
        }

        for (int lat = 0; lat < latSegments; lat++)
        {
            for (int lon = 0; lon < lonSegments; lon++)
            {
                int first = lat * (lonSegments + 1) + lon;
                int second = first + lonSegments + 1;

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

    public static MeshGeometry3D CreatePrism(int sides, double radius, double height)
    {
        var mesh = new MeshGeometry3D();
        double angleStep = 2 * Math.PI / sides;

        // Bottom vertices
        for (int i = 0; i < sides; i++)
        {
            double angle = i * angleStep;
            mesh.Positions.Add(new Point3D(
                radius * Math.Cos(angle),
                0,
                radius * Math.Sin(angle)));
        }

        // Top vertices
        for (int i = 0; i < sides; i++)
        {
            double angle = i * angleStep;
            mesh.Positions.Add(new Point3D(
                radius * Math.Cos(angle),
                height,
                radius * Math.Sin(angle)));
        }

        // Side faces
        for (int i = 0; i < sides; i++)
        {
            int next = (i + 1) % sides;
            int top = i + sides;
            int topNext = next + sides;

            mesh.TriangleIndices.Add(i);
            mesh.TriangleIndices.Add(top);
            mesh.TriangleIndices.Add(next);

            mesh.TriangleIndices.Add(next);
            mesh.TriangleIndices.Add(top);
            mesh.TriangleIndices.Add(topNext);
        }

        // Centers
        int bottomCenter = mesh.Positions.Count;
        mesh.Positions.Add(new Point3D(0, 0, 0));

        int topCenter = mesh.Positions.Count;
        mesh.Positions.Add(new Point3D(0, height, 0));

        // Bottom cap
        for (int i = 0; i < sides; i++)
        {
            int next = (i + 1) % sides;
            mesh.TriangleIndices.Add(bottomCenter);
            mesh.TriangleIndices.Add(next);
            mesh.TriangleIndices.Add(i);
        }

        // Top cap
        for (int i = 0; i < sides; i++)
        {
            int next = (i + 1) % sides;
            mesh.TriangleIndices.Add(topCenter);
            mesh.TriangleIndices.Add(i + sides);
            mesh.TriangleIndices.Add(next + sides);
        }

        return mesh;
    }
}
