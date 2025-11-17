using System;
using System.Windows.Media.Media3D;
using Professional3DROIViewer.Models;

namespace Professional3DROIViewer.Services
{
    /// <summary>
    /// Generates 3D meshes for different ROI shapes
    /// </summary>
    public static class MeshGenerator
    {
        // ============================================
        // BOX MESH GENERATION
        // ============================================

        public static MeshGeometry3D CreateBox(double width, double height, double depth)
        {
            var mesh = new MeshGeometry3D();
            double w = width / 2, h = height / 2, d = depth / 2;

            // 8 vertices of the box
            Point3D[] vertices = new[]
            {
                new Point3D(-w, -h, -d), new Point3D(w, -h, -d),
                new Point3D(w, h, -d), new Point3D(-w, h, -d),
                new Point3D(-w, -h, d), new Point3D(w, -h, d),
                new Point3D(w, h, d), new Point3D(-w, h, d)
            };

            foreach (var v in vertices)
                mesh.Positions.Add(v);

            // 12 triangles (6 faces * 2 triangles)
            int[] indices = {
                0,1,2, 0,2,3,   // Front
                5,4,7, 5,7,6,   // Back
                4,0,3, 4,3,7,   // Left
                1,5,6, 1,6,2,   // Right
                3,2,6, 3,6,7,   // Top
                4,5,1, 4,1,0    // Bottom
            };

            foreach (var i in indices)
                mesh.TriangleIndices.Add(i);

            return mesh;
        }

        // ============================================
        // SPHERE MESH GENERATION
        // ============================================

        public static MeshGeometry3D CreateSphere(double radius, int divisions = 24)
        {
            var mesh = new MeshGeometry3D();

            // Generate vertices using spherical coordinates
            for (int lat = 0; lat <= divisions; lat++)
            {
                double theta = lat * Math.PI / divisions;
                double sinTheta = Math.Sin(theta);
                double cosTheta = Math.Cos(theta);

                for (int lon = 0; lon <= divisions; lon++)
                {
                    double phi = lon * 2 * Math.PI / divisions;
                    double sinPhi = Math.Sin(phi);
                    double cosPhi = Math.Cos(phi);

                    double x = cosPhi * sinTheta;
                    double y = cosTheta;
                    double z = sinPhi * sinTheta;

                    mesh.Positions.Add(new Point3D(x * radius, y * radius, z * radius));
                }
            }

            // Generate triangle indices
            for (int lat = 0; lat < divisions; lat++)
            {
                for (int lon = 0; lon < divisions; lon++)
                {
                    int first = (lat * (divisions + 1)) + lon;
                    int second = first + divisions + 1;

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

        // ============================================
        // CYLINDER MESH GENERATION
        // ============================================

        public static MeshGeometry3D CreateCylinder(double radius, double height, int divisions = 24)
        {
            var mesh = new MeshGeometry3D();
            double halfHeight = height / 2;

            // Top and bottom center points
            mesh.Positions.Add(new Point3D(0, halfHeight, 0));   // 0
            mesh.Positions.Add(new Point3D(0, -halfHeight, 0));  // 1

            // Generate rim vertices
            for (int i = 0; i <= divisions; i++)
            {
                double angle = 2 * Math.PI * i / divisions;
                double x = radius * Math.Cos(angle);
                double z = radius * Math.Sin(angle);

                mesh.Positions.Add(new Point3D(x, halfHeight, z));   // Top rim
                mesh.Positions.Add(new Point3D(x, -halfHeight, z));  // Bottom rim
            }

            // Top cap triangles
            for (int i = 0; i < divisions; i++)
            {
                mesh.TriangleIndices.Add(0);
                mesh.TriangleIndices.Add(2 + i * 2);
                mesh.TriangleIndices.Add(2 + (i + 1) * 2);
            }

            // Bottom cap triangles
            for (int i = 0; i < divisions; i++)
            {
                mesh.TriangleIndices.Add(1);
                mesh.TriangleIndices.Add(3 + (i + 1) * 2);
                mesh.TriangleIndices.Add(3 + i * 2);
            }

            // Side triangles
            for (int i = 0; i < divisions; i++)
            {
                int topCurrent = 2 + i * 2;
                int bottomCurrent = 3 + i * 2;
                int topNext = 2 + (i + 1) * 2;
                int bottomNext = 3 + (i + 1) * 2;

                mesh.TriangleIndices.Add(topCurrent);
                mesh.TriangleIndices.Add(bottomCurrent);
                mesh.TriangleIndices.Add(topNext);

                mesh.TriangleIndices.Add(bottomCurrent);
                mesh.TriangleIndices.Add(bottomNext);
                mesh.TriangleIndices.Add(topNext);
            }

            return mesh;
        }

        // ============================================
        // CONE MESH GENERATION
        // ============================================

        public static MeshGeometry3D CreateCone(double radius, double height, int divisions = 24)
        {
            var mesh = new MeshGeometry3D();

            mesh.Positions.Add(new Point3D(0, height / 2, 0));   // Tip (0)
            mesh.Positions.Add(new Point3D(0, -height / 2, 0));  // Base center (1)

            // Base rim vertices
            for (int i = 0; i <= divisions; i++)
            {
                double angle = 2 * Math.PI * i / divisions;
                double x = radius * Math.Cos(angle);
                double z = radius * Math.Sin(angle);
                mesh.Positions.Add(new Point3D(x, -height / 2, z));
            }

            // Side triangles (tip to base rim)
            for (int i = 0; i < divisions; i++)
            {
                mesh.TriangleIndices.Add(0);
                mesh.TriangleIndices.Add(2 + i);
                mesh.TriangleIndices.Add(2 + (i + 1));
            }

            // Base triangles
            for (int i = 0; i < divisions; i++)
            {
                mesh.TriangleIndices.Add(1);
                mesh.TriangleIndices.Add(2 + (i + 1));
                mesh.TriangleIndices.Add(2 + i);
            }

            return mesh;
        }

        // ============================================
        // ELLIPSOID MESH GENERATION
        // ============================================

        public static MeshGeometry3D CreateEllipsoid(double radiusX, double radiusY, double radiusZ, int divisions = 24)
        {
            var mesh = new MeshGeometry3D();

            for (int lat = 0; lat <= divisions; lat++)
            {
                double theta = lat * Math.PI / divisions;
                double sinTheta = Math.Sin(theta);
                double cosTheta = Math.Cos(theta);

                for (int lon = 0; lon <= divisions; lon++)
                {
                    double phi = lon * 2 * Math.PI / divisions;
                    double sinPhi = Math.Sin(phi);
                    double cosPhi = Math.Cos(phi);

                    double x = cosPhi * sinTheta * radiusX;
                    double y = cosTheta * radiusY;
                    double z = sinPhi * sinTheta * radiusZ;

                    mesh.Positions.Add(new Point3D(x, y, z));
                }
            }

            for (int lat = 0; lat < divisions; lat++)
            {
                for (int lon = 0; lon < divisions; lon++)
                {
                    int first = (lat * (divisions + 1)) + lon;
                    int second = first + divisions + 1;

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

        // ============================================
        // TORUS MESH GENERATION
        // ============================================

        public static MeshGeometry3D CreateTorus(double majorRadius, double minorRadius, int majorDivisions = 24, int minorDivisions = 16)
        {
            var mesh = new MeshGeometry3D();

            for (int i = 0; i <= majorDivisions; i++)
            {
                double u = 2 * Math.PI * i / majorDivisions;

                for (int j = 0; j <= minorDivisions; j++)
                {
                    double v = 2 * Math.PI * j / minorDivisions;

                    double x = (majorRadius + minorRadius * Math.Cos(v)) * Math.Cos(u);
                    double y = minorRadius * Math.Sin(v);
                    double z = (majorRadius + minorRadius * Math.Cos(v)) * Math.Sin(u);

                    mesh.Positions.Add(new Point3D(x, y, z));
                }
            }

            for (int i = 0; i < majorDivisions; i++)
            {
                for (int j = 0; j < minorDivisions; j++)
                {
                    int first = i * (minorDivisions + 1) + j;
                    int second = first + minorDivisions + 1;

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

        // ============================================
        // ROI TO MESH CONVERTER
        // ============================================

        /// <summary>
        /// Creates a mesh for the specified ROI
        /// </summary>
        public static MeshGeometry3D CreateMeshForROI(ROI roi)
        {
            return roi.Shape switch
            {
                ROIShape.Box => CreateBox(roi.Width, roi.Height, roi.Depth),
                ROIShape.Sphere => CreateSphere(roi.Width / 2),
                ROIShape.Cylinder => CreateCylinder(roi.Width / 2, roi.Height),
                ROIShape.Cone => CreateCone(roi.Width / 2, roi.Height),
                ROIShape.Ellipsoid => CreateEllipsoid(roi.Width / 2, roi.Height / 2, roi.Depth / 2),
                ROIShape.Torus => CreateTorus(roi.Width / 2, roi.Depth / 4),
                _ => CreateBox(roi.Width, roi.Height, roi.Depth)
            };
        }

        // ============================================
        // WIREFRAME GENERATION
        // ============================================

        /// <summary>
        /// Creates wireframe edges for a mesh
        /// </summary>
        public static Model3DGroup CreateWireframe(MeshGeometry3D mesh, System.Windows.Media.Color color)
        {
            var group = new Model3DGroup();
            var processedEdges = new System.Collections.Generic.HashSet<(int, int)>();

            // Extract unique edges from triangles
            for (int i = 0; i < mesh.TriangleIndices.Count; i += 3)
            {
                int i0 = mesh.TriangleIndices[i];
                int i1 = mesh.TriangleIndices[i + 1];
                int i2 = mesh.TriangleIndices[i + 2];

                AddEdge(group, mesh, i0, i1, color, processedEdges);
                AddEdge(group, mesh, i1, i2, color, processedEdges);
                AddEdge(group, mesh, i2, i0, color, processedEdges);
            }

            return group;
        }

        private static void AddEdge(Model3DGroup group, MeshGeometry3D mesh, int idx1, int idx2,
            System.Windows.Media.Color color, System.Collections.Generic.HashSet<(int, int)> processedEdges)
        {
            // Ensure consistent edge ordering
            var edge = idx1 < idx2 ? (idx1, idx2) : (idx2, idx1);

            if (processedEdges.Add(edge))
            {
                var p1 = mesh.Positions[idx1];
                var p2 = mesh.Positions[idx2];

                var lineMesh = CreateLine(p1, p2, 0.02);
                var material = new System.Windows.Media.Media3D.DiffuseMaterial(
                    new System.Windows.Media.SolidColorBrush(color));

                group.Children.Add(new GeometryModel3D(lineMesh, material));
            }
        }

        private static MeshGeometry3D CreateLine(Point3D start, Point3D end, double thickness)
        {
            var mesh = new MeshGeometry3D();
            var direction = end - start;
            direction.Normalize();

            var perpendicular = Vector3D.CrossProduct(direction, new Vector3D(0, 1, 0));
            if (perpendicular.Length < 0.001)
                perpendicular = Vector3D.CrossProduct(direction, new Vector3D(1, 0, 0));
            perpendicular.Normalize();

            var p1 = start + perpendicular * thickness;
            var p2 = start - perpendicular * thickness;
            var p3 = end + perpendicular * thickness;
            var p4 = end - perpendicular * thickness;

            mesh.Positions.Add(p1);
            mesh.Positions.Add(p2);
            mesh.Positions.Add(p3);
            mesh.Positions.Add(p4);

            mesh.TriangleIndices.Add(0); mesh.TriangleIndices.Add(1); mesh.TriangleIndices.Add(2);
            mesh.TriangleIndices.Add(1); mesh.TriangleIndices.Add(3); mesh.TriangleIndices.Add(2);

            return mesh;
        }
    }
}
