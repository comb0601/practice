# Module 5: Advanced ROI Manipulation (15 hours)

## Overview

Master advanced ROI techniques including resizing, transforming, managing multiple ROIs, collision detection, and clipping planes. Build professional-grade ROI manipulation tools.

## Learning Objectives

- Resize ROI with handles
- Transform ROI (rotate, move, scale)
- Manage multiple ROIs
- Detect ROI collisions and intersections
- Use clipping planes with ROI
- Save and load ROI configurations

## Lessons

### Lesson 5.1 - Resize and Transform ROI (3 hours)
**Folder:** `Lesson01-ResizeTransform/`

**Topics:**
- Interactive resize handles
- Proportional vs free scaling
- Rotation gizmos
- Snap-to-grid
- Transformation constraints

**Resize Handle System:**
```csharp
public class ROIManipulator
{
    public enum HandleType
    {
        Corner,          // 8 corner handles
        Edge,            // 12 edge midpoint handles
        Face,            // 6 face center handles
        RotationRing     // Rotation gizmo
    }

    public class ResizeHandle
    {
        public HandleType Type { get; set; }
        public Point3D Position { get; set; }
        public Vector3D Direction { get; set; }
        public GeometryModel3D Visual { get; set; }
    }

    public void ResizeROI(ROI3D roi, ResizeHandle handle, Vector3D delta)
    {
        switch (handle.Type)
        {
            case HandleType.Corner:
                // Resize from corner - affects 3 dimensions
                ResizeFromCorner(roi, handle, delta);
                break;

            case HandleType.Edge:
                // Resize along edge - affects 2 dimensions
                ResizeFromEdge(roi, handle, delta);
                break;

            case HandleType.Face:
                // Resize from face - affects 1 dimension
                ResizeFromFace(roi, handle, delta);
                break;
        }
    }

    private void ResizeFromCorner(ROI3D roi, ResizeHandle handle, Vector3D delta)
    {
        // Project delta onto the corner direction
        Vector3D cornerDir = handle.Position - roi.Center;
        cornerDir.Normalize();

        double scale = Vector3D.DotProduct(delta, cornerDir);

        // Expand/contract ROI maintaining opposite corner
        roi.Size = new Size3D(
            Math.Max(0.1, roi.Size.X + scale * Math.Sign(cornerDir.X)),
            Math.Max(0.1, roi.Size.Y + scale * Math.Sign(cornerDir.Y)),
            Math.Max(0.1, roi.Size.Z + scale * Math.Sign(cornerDir.Z))
        );

        // Adjust center to keep opposite corner fixed
        roi.Center += cornerDir * scale / 2;
    }
}
```

**Project:** Interactive ROI with resize handles

---

### Lesson 5.2 - Multiple ROIs (3 hours)
**Folder:** `Lesson02-MultipleROIs/`

**Topics:**
- ROI collection management
- Multi-selection
- ROI hierarchy
- Grouped operations
- ROI layers

**Multi-ROI Manager:**
```csharp
public class ROIManager
{
    private List<ROI3D> rois = new List<ROI3D>();
    private HashSet<ROI3D> selectedROIs = new HashSet<ROI3D>();

    public void SelectROI(ROI3D roi, bool multiSelect = false)
    {
        if (!multiSelect)
            ClearSelection();

        selectedROIs.Add(roi);
        roi.IsSelected = true;
    }

    public void SelectROIsInBox(Rect3D selectionBox)
    {
        foreach (var roi in rois)
        {
            if (BoundsIntersect(roi.GetBounds(), selectionBox))
            {
                SelectROI(roi, true);
            }
        }
    }

    public void TransformSelected(Transform3D transform)
    {
        foreach (var roi in selectedROIs)
        {
            ApplyTransform(roi, transform);
        }
    }

    public List<ROI3D> GetROIsContainingPoint(Point3D point)
    {
        return rois.Where(r => r.ContainsPoint(point)).ToList();
    }

    public void MergeROIs(List<ROI3D> roisToMerge)
    {
        if (roisToMerge.Count < 2) return;

        // Calculate bounding box of all ROIs
        double minX = roisToMerge.Min(r => r.MinX);
        double maxX = roisToMerge.Max(r => r.MaxX);
        double minY = roisToMerge.Min(r => r.MinY);
        double maxY = roisToMerge.Max(r => r.MaxY);
        double minZ = roisToMerge.Min(r => r.MinZ);
        double maxZ = roisToMerge.Max(r => r.MaxZ);

        var mergedROI = new ROI3D
        {
            Name = "Merged ROI",
            Center = new Point3D(
                (minX + maxX) / 2,
                (minY + maxY) / 2,
                (minZ + maxZ) / 2),
            Size = new Size3D(
                maxX - minX,
                maxY - minY,
                maxZ - minZ)
        };

        // Remove original ROIs and add merged
        foreach (var roi in roisToMerge)
            rois.Remove(roi);

        rois.Add(mergedROI);
    }
}
```

**Project:** Multi-ROI application with selection and grouping

---

### Lesson 5.3 - ROI Collision Detection (3 hours)
**Folder:** `Lesson03-CollisionDetection/`

**Topics:**
- AABB collision detection
- ROI intersection calculation
- Overlap visualization
- Collision response
- Spatial partitioning

**Collision Detection:**
```csharp
public class ROICollisionDetector
{
    public bool CheckCollision(ROI3D roi1, ROI3D roi2)
    {
        return roi1.MaxX >= roi2.MinX && roi1.MinX <= roi2.MaxX &&
               roi1.MaxY >= roi2.MinY && roi1.MinY <= roi2.MaxY &&
               roi1.MaxZ >= roi2.MinZ && roi1.MinZ <= roi2.MaxZ;
    }

    public ROI3D? GetIntersection(ROI3D roi1, ROI3D roi2)
    {
        if (!CheckCollision(roi1, roi2))
            return null;

        double minX = Math.Max(roi1.MinX, roi2.MinX);
        double maxX = Math.Min(roi1.MaxX, roi2.MaxX);
        double minY = Math.Max(roi1.MinY, roi2.MinY);
        double maxY = Math.Min(roi1.MaxY, roi2.MaxY);
        double minZ = Math.Max(roi1.MinZ, roi2.MinZ);
        double maxZ = Math.Min(roi1.MaxZ, roi2.MaxZ);

        return new ROI3D
        {
            Name = "Intersection",
            Center = new Point3D(
                (minX + maxX) / 2,
                (minY + maxY) / 2,
                (minZ + maxZ) / 2),
            Size = new Size3D(
                maxX - minX,
                maxY - minY,
                maxZ - minZ),
            VisualizationColor = Colors.Purple
        };
    }

    public double CalculateOverlapVolume(ROI3D roi1, ROI3D roi2)
    {
        var intersection = GetIntersection(roi1, roi2);
        return intersection?.Volume ?? 0;
    }

    public List<ROI3D> GetCollidingROIs(ROI3D roi, List<ROI3D> allROIs)
    {
        return allROIs.Where(r => r != roi && CheckCollision(roi, r)).ToList();
    }
}
```

**Project:** ROI collision visualization and analysis tool

---

### Lesson 5.4 - ROI Clipping Planes (3 hours)
**Folder:** `Lesson04-ClippingPlanes/`

**Topics:**
- Clipping plane concept
- ROI-based clipping
- Multiple clipping planes
- Inside/outside visualization
- Cross-section views

**Clipping Implementation:**
```csharp
public class ROIClipper
{
    public Model3DGroup ClipModelByROI(GeometryModel3D model, ROI3D roi, bool keepInside)
    {
        var result = new Model3DGroup();

        if (model.Geometry is MeshGeometry3D mesh)
        {
            var clippedMesh = new MeshGeometry3D();

            // Process each triangle
            for (int i = 0; i < mesh.TriangleIndices.Count; i += 3)
            {
                Point3D p1 = mesh.Positions[mesh.TriangleIndices[i]];
                Point3D p2 = mesh.Positions[mesh.TriangleIndices[i + 1]];
                Point3D p3 = mesh.Positions[mesh.TriangleIndices[i + 2]];

                bool p1Inside = roi.ContainsPoint(p1);
                bool p2Inside = roi.ContainsPoint(p2);
                bool p3Inside = roi.ContainsPoint(p3);

                int insideCount = (p1Inside ? 1 : 0) + (p2Inside ? 1 : 0) + (p3Inside ? 1 : 0);

                if (keepInside && insideCount == 3)
                {
                    // All points inside - keep triangle
                    AddTriangle(clippedMesh, p1, p2, p3);
                }
                else if (!keepInside && insideCount == 0)
                {
                    // All points outside - keep triangle
                    AddTriangle(clippedMesh, p1, p2, p3);
                }
                else if (insideCount > 0 && insideCount < 3)
                {
                    // Partial - need to clip triangle
                    // (Advanced: implement triangle-plane intersection)
                }
            }

            result.Children.Add(new GeometryModel3D(clippedMesh, model.Material));
        }

        return result;
    }

    // Create cross-section plane at ROI boundary
    public GeometryModel3D CreateCrossSectionPlane(ROI3D roi, Axis axis, double position)
    {
        var mesh = new MeshGeometry3D();

        switch (axis)
        {
            case Axis.X:
                mesh.Positions.Add(new Point3D(position, roi.MinY, roi.MinZ));
                mesh.Positions.Add(new Point3D(position, roi.MaxY, roi.MinZ));
                mesh.Positions.Add(new Point3D(position, roi.MaxY, roi.MaxZ));
                mesh.Positions.Add(new Point3D(position, roi.MinY, roi.MaxZ));
                break;
            // Similar for Y and Z axes
        }

        mesh.TriangleIndices.Add(0); mesh.TriangleIndices.Add(1); mesh.TriangleIndices.Add(2);
        mesh.TriangleIndices.Add(0); mesh.TriangleIndices.Add(2); mesh.TriangleIndices.Add(3);

        var material = new DiffuseMaterial(new SolidColorBrush(Color.FromArgb(128, 255, 255, 0)));

        return new GeometryModel3D(mesh, material);
    }
}

public enum Axis { X, Y, Z }
```

**Project:** 3D model viewer with ROI clipping

---

### Lesson 5.5 - ROI Saving and Loading (3 hours)
**Folder:** `Lesson05-SaveLoad/`

**Topics:**
- JSON serialization
- XML format
- Binary format
- ROI file format design
- Import/export workflows

**Serialization:**
```csharp
public class ROISerializer
{
    public void SaveToJson(List<ROI3D> rois, string filePath)
    {
        var data = rois.Select(r => new ROIData
        {
            Name = r.Name,
            CenterX = r.Center.X,
            CenterY = r.Center.Y,
            CenterZ = r.Center.Z,
            SizeX = r.Size.X,
            SizeY = r.Size.Y,
            SizeZ = r.Size.Z,
            Color = r.VisualizationColor.ToString()
        }).ToList();

        string json = System.Text.Json.JsonSerializer.Serialize(data, new JsonSerializerOptions
        {
            WriteIndented = true
        });

        File.WriteAllText(filePath, json);
    }

    public List<ROI3D> LoadFromJson(string filePath)
    {
        string json = File.ReadAllText(filePath);
        var data = System.Text.Json.JsonSerializer.Deserialize<List<ROIData>>(json);

        return data?.Select(d => new ROI3D
        {
            Name = d.Name,
            Center = new Point3D(d.CenterX, d.CenterY, d.CenterZ),
            Size = new Size3D(d.SizeX, d.SizeY, d.SizeZ),
            VisualizationColor = (Color)ColorConverter.ConvertFromString(d.Color ?? "Blue")
        }).ToList() ?? new List<ROI3D>();
    }
}

public class ROIData
{
    public string Name { get; set; } = "";
    public double CenterX { get; set; }
    public double CenterY { get; set; }
    public double CenterZ { get; set; }
    public double SizeX { get; set; }
    public double SizeY { get; set; }
    public double SizeZ { get; set; }
    public string Color { get; set; } = "Blue";
}
```

**Project:** ROI library manager with save/load functionality

---

## Complete Advanced ROI System

Combining all lessons into a professional ROI system with:
- Interactive creation and manipulation
- Multi-ROI management
- Collision detection
- Clipping and visualization
- Persistent storage

## Next Module

**Module 6: Data Visualization with ROI** - Apply ROI to real-world data visualization scenarios!

---

**Time Investment:** 15 hours total
**Difficulty:** Advanced
