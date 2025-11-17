# Module 4: ROI Basics (15 hours)

## Overview

**This is the core module!** Learn to create, manipulate, and visualize Regions of Interest (ROI) in 3D space. Build interactive tools for selecting and working with 3D volumes - essential for medical imaging, data analysis, and scientific visualization.

## Learning Objectives

- Understand 3D ROI concepts and data structures
- Create interactive bounding box ROI
- Implement ROI selection tools
- Visualize ROI boundaries
- Extract data within ROI

## What is a 3D ROI?

A **Region of Interest (ROI)** is a selected 3D volume used to:
- Focus analysis on specific areas
- Filter and extract data
- Measure volumes and distances
- Crop and manipulate 3D datasets
- Highlight important regions

Common ROI shapes:
- **Box**: Axis-aligned bounding box (most common)
- **Sphere**: Radial selection
- **Cylinder**: Tubular volumes
- **Arbitrary**: Free-form selection

## Lessons

### Lesson 4.1 - What is 3D ROI (2 hours)
**Folder:** `Lesson01-ROIConcepts/`

Introduction to ROI theory and applications.

**Topics:**
- ROI definition and use cases
- ROI data structures
- Medical imaging ROI
- Scientific visualization ROI
- ROI coordinate systems

**Key Data Structure:**
```csharp
public class ROI3D
{
    public string Name { get; set; }
    public Point3D Center { get; set; }
    public Size3D Size { get; set; }
    public Color VisualizationColor { get; set; }
    public bool IsVisible { get; set; }
    public bool IsSelected { get; set; }

    // Bounding box properties
    public double MinX => Center.X - Size.X / 2;
    public double MaxX => Center.X + Size.X / 2;
    public double MinY => Center.Y - Size.Y / 2;
    public double MaxY => Center.Y + Size.Y / 2;
    public double MinZ => Center.Z - Size.Z / 2;
    public double MaxZ => Center.Z + Size.Z / 2;

    public bool ContainsPoint(Point3D point)
    {
        return point.X >= MinX && point.X <= MaxX &&
               point.Y >= MinY && point.Y <= MaxY &&
               point.Z >= MinZ && point.Z <= MaxZ;
    }

    public double Volume => Size.X * Size.Y * Size.Z;

    public Rect3D GetBounds()
    {
        return new Rect3D(MinX, MinY, MinZ, Size.X, Size.Y, Size.Z);
    }
}
```

**Project:** ROI data structure with visualization examples

---

### Lesson 4.2 - Bounding Box ROI (3 hours)
**Folder:** `Lesson02-BoundingBoxROI/`

Create and render axis-aligned bounding box ROI.

**Topics:**
- Box wireframe rendering
- Filled box with transparency
- Box handles and corners
- Box highlighting
- Multiple box rendering

**ROI Visualization:**
```csharp
public class ROIVisualizer
{
    public Model3DGroup CreateROIVisualization(ROI3D roi)
    {
        var group = new Model3DGroup();

        // Create wireframe edges
        group.Children.Add(CreateWireframeBox(roi));

        // Create semi-transparent faces
        if (roi.IsSelected)
        {
            group.Children.Add(CreateTransparentBox(roi, 0.2));
        }

        // Create corner handles
        group.Children.Add(CreateCornerHandles(roi));

        return group;
    }

    private Model3D CreateWireframeBox(ROI3D roi)
    {
        var group = new Model3DGroup();
        double thickness = 0.02;
        Color color = roi.IsSelected ? Colors.Yellow : roi.VisualizationColor;

        // 12 edges of the box
        Point3D[] corners = GetCorners(roi);

        // Bottom face edges
        group.Children.Add(CreateLine(corners[0], corners[1], color, thickness));
        group.Children.Add(CreateLine(corners[1], corners[2], color, thickness));
        group.Children.Add(CreateLine(corners[2], corners[3], color, thickness));
        group.Children.Add(CreateLine(corners[3], corners[0], color, thickness));

        // Top face edges
        group.Children.Add(CreateLine(corners[4], corners[5], color, thickness));
        group.Children.Add(CreateLine(corners[5], corners[6], color, thickness));
        group.Children.Add(CreateLine(corners[6], corners[7], color, thickness));
        group.Children.Add(CreateLine(corners[7], corners[4], color, thickness));

        // Vertical edges
        group.Children.Add(CreateLine(corners[0], corners[4], color, thickness));
        group.Children.Add(CreateLine(corners[1], corners[5], color, thickness));
        group.Children.Add(CreateLine(corners[2], corners[6], color, thickness));
        group.Children.Add(CreateLine(corners[3], corners[7], color, thickness));

        return group;
    }

    private Point3D[] GetCorners(ROI3D roi)
    {
        return new Point3D[]
        {
            new Point3D(roi.MinX, roi.MinY, roi.MinZ),  // 0: Bottom-left-front
            new Point3D(roi.MaxX, roi.MinY, roi.MinZ),  // 1: Bottom-right-front
            new Point3D(roi.MaxX, roi.MaxY, roi.MinZ),  // 2: Top-right-front
            new Point3D(roi.MinX, roi.MaxY, roi.MinZ),  // 3: Top-left-front
            new Point3D(roi.MinX, roi.MinY, roi.MaxZ),  // 4: Bottom-left-back
            new Point3D(roi.MaxX, roi.MinY, roi.MaxZ),  // 5: Bottom-right-back
            new Point3D(roi.MaxX, roi.MaxY, roi.MaxZ),  // 6: Top-right-back
            new Point3D(roi.MinX, roi.MaxY, roi.MaxZ),  // 7: Top-left-back
        };
    }

    private Model3D CreateCornerHandles(ROI3D roi)
    {
        var group = new Model3DGroup();
        Point3D[] corners = GetCorners(roi);
        double handleSize = 0.1;

        foreach (var corner in corners)
        {
            var handle = CreateSphere(corner, handleSize, Colors.White);
            group.Children.Add(handle);
        }

        return group;
    }
}
```

**Project:** Interactive bounding box ROI with visualization

---

### Lesson 4.3 - Interactive ROI Selection (4 hours)
**Folder:** `Lesson03-InteractiveSelection/`

Implement mouse-based ROI creation and manipulation.

**Topics:**
- Click-and-drag ROI creation
- 3D picking and hit testing
- Converting 2D mouse to 3D coordinates
- Visual feedback during creation
- Multi-step selection workflow

**Mouse-to-3D Conversion:**
```csharp
public class ROISelector
{
    private Point? selectionStart;
    private ROI3D? currentROI;

    public void OnMouseDown(Point mousePosition, Viewport3D viewport)
    {
        // Project mouse position to 3D plane
        Point3D? worldPosition = Get3DPosition(mousePosition, viewport);
        if (worldPosition.HasValue)
        {
            selectionStart = mousePosition;
            currentROI = new ROI3D
            {
                Center = worldPosition.Value,
                Size = new Size3D(0, 0, 0),
                VisualizationColor = Colors.Cyan
            };
        }
    }

    public void OnMouseMove(Point mousePosition, Viewport3D viewport)
    {
        if (selectionStart.HasValue && currentROI != null)
        {
            Point3D? currentPosition = Get3DPosition(mousePosition, viewport);
            if (currentPosition.HasValue)
            {
                // Update ROI size based on drag
                UpdateROIFromDrag(selectionStart.Value, mousePosition, currentPosition.Value);
            }
        }
    }

    public void OnMouseUp(Point mousePosition, Viewport3D viewport)
    {
        if (currentROI != null && currentROI.Volume > 0.001)
        {
            // Finalize ROI
            FinalizeROI(currentROI);
        }

        selectionStart = null;
        currentROI = null;
    }

    private Point3D? Get3DPosition(Point mousePoint, Viewport3D viewport)
    {
        // Raycast from camera through mouse position
        Point3D rayOrigin;
        Vector3D rayDirection;

        if (GetRayFromMouse(mousePoint, viewport, out rayOrigin, out rayDirection))
        {
            // Intersect with a plane (e.g., XZ plane at Y=0)
            return IntersectPlane(rayOrigin, rayDirection, new Point3D(0, 0, 0), new Vector3D(0, 1, 0));
        }

        return null;
    }

    private bool GetRayFromMouse(Point mousePoint, Viewport3D viewport, out Point3D origin, out Vector3D direction)
    {
        if (viewport.Camera is PerspectiveCamera camera)
        {
            // Convert 2D screen point to 3D ray
            var hitTestPoint = new Point(
                mousePoint.X / viewport.ActualWidth,
                mousePoint.Y / viewport.ActualHeight);

            origin = camera.Position;

            // Calculate ray direction based on camera frustum
            Vector3D lookDir = camera.LookDirection;
            lookDir.Normalize();

            Vector3D right = Vector3D.CrossProduct(lookDir, camera.UpDirection);
            right.Normalize();

            Vector3D up = Vector3D.CrossProduct(right, lookDir);

            double aspectRatio = viewport.ActualWidth / viewport.ActualHeight;
            double fovRad = camera.FieldOfView * Math.PI / 180.0;
            double tanFov = Math.Tan(fovRad / 2.0);

            double x = (hitTestPoint.X - 0.5) * 2.0;
            double y = (0.5 - hitTestPoint.Y) * 2.0;

            direction = lookDir + (right * x * tanFov * aspectRatio) + (up * y * tanFov);
            direction.Normalize();

            return true;
        }

        origin = new Point3D();
        direction = new Vector3D();
        return false;
    }

    private Point3D? IntersectPlane(Point3D rayOrigin, Vector3D rayDirection, Point3D planePoint, Vector3D planeNormal)
    {
        double denominator = Vector3D.DotProduct(rayDirection, planeNormal);

        if (Math.Abs(denominator) < 0.0001)
            return null; // Ray parallel to plane

        Vector3D diff = planePoint - rayOrigin;
        double t = Vector3D.DotProduct(diff, planeNormal) / denominator;

        if (t < 0)
            return null; // Intersection behind ray origin

        return rayOrigin + rayDirection * t;
    }
}
```

**Project:** Full interactive ROI creation tool

---

### Lesson 4.4 - ROI Visualization (3 hours)
**Folder:** `Lesson04-ROIVisualization/`

Advanced ROI rendering and visual effects.

**Topics:**
- Transparency and overlay rendering
- ROI labeling
- Dimension indicators
- Color coding by value
- Animation effects

**Enhanced Visualization:**
```csharp
public class AdvancedROIRenderer
{
    public Model3DGroup CreateEnhancedROI(ROI3D roi, ROIVisualizationStyle style)
    {
        var group = new Model3DGroup();

        switch (style)
        {
            case ROIVisualizationStyle.Wireframe:
                group.Children.Add(CreateWireframe(roi));
                break;

            case ROIVisualizationStyle.Solid:
                group.Children.Add(CreateSolidBox(roi, 0.3));
                break;

            case ROIVisualizationStyle.WireframeWithHandles:
                group.Children.Add(CreateWireframe(roi));
                group.Children.Add(CreateHandles(roi));
                break;

            case ROIVisualizationStyle.DimensionLines:
                group.Children.Add(CreateWireframe(roi));
                group.Children.Add(CreateDimensionLines(roi));
                break;
        }

        // Add label
        if (!string.IsNullOrEmpty(roi.Name))
        {
            group.Children.Add(Create3DLabel(roi.Center, roi.Name));
        }

        return group;
    }

    private Model3D CreateDimensionLines(ROI3D roi)
    {
        var group = new Model3DGroup();
        Color dimColor = Colors.White;
        double offset = 0.3;

        // X dimension
        Point3D xStart = new Point3D(roi.MinX, roi.MinY - offset, roi.MinZ);
        Point3D xEnd = new Point3D(roi.MaxX, roi.MinY - offset, roi.MinZ);
        group.Children.Add(CreateDimensionLine(xStart, xEnd, roi.Size.X, dimColor));

        // Y dimension
        Point3D yStart = new Point3D(roi.MinX - offset, roi.MinY, roi.MinZ);
        Point3D yEnd = new Point3D(roi.MinX - offset, roi.MaxY, roi.MinZ);
        group.Children.Add(CreateDimensionLine(yStart, yEnd, roi.Size.Y, dimColor));

        // Z dimension
        Point3D zStart = new Point3D(roi.MinX, roi.MinY, roi.MinZ);
        Point3D zEnd = new Point3D(roi.MinX, roi.MinY, roi.MaxZ);
        group.Children.Add(CreateDimensionLine(zStart, zEnd, roi.Size.Z, dimColor));

        return group;
    }
}

public enum ROIVisualizationStyle
{
    Wireframe,
    Solid,
    WireframeWithHandles,
    DimensionLines,
    Highlight
}
```

**Project:** ROI visualization library with multiple styles

---

### Lesson 4.5 - ROI Data Extraction (3 hours)
**Folder:** `Lesson05-DataExtraction/`

Extract and analyze data within ROI bounds.

**Topics:**
- Point filtering by ROI
- Statistical analysis
- Data export
- Multi-ROI operations
- ROI-based coloring

**Data Extraction:**
```csharp
public class ROIDataExtractor
{
    public List<Point3D> GetPointsInROI(List<Point3D> allPoints, ROI3D roi)
    {
        return allPoints.Where(p => roi.ContainsPoint(p)).ToList();
    }

    public ROIStatistics CalculateStatistics(List<Point3D> points, List<double> values, ROI3D roi)
    {
        var pointsInROI = new List<(Point3D point, double value)>();

        for (int i = 0; i < points.Count; i++)
        {
            if (roi.ContainsPoint(points[i]))
            {
                pointsInROI.Add((points[i], values[i]));
            }
        }

        if (pointsInROI.Count == 0)
            return new ROIStatistics();

        return new ROIStatistics
        {
            PointCount = pointsInROI.Count,
            MinValue = pointsInROI.Min(p => p.value),
            MaxValue = pointsInROI.Max(p => p.value),
            MeanValue = pointsInROI.Average(p => p.value),
            StdDeviation = CalculateStdDev(pointsInROI.Select(p => p.value)),
            Volume = roi.Volume,
            Density = pointsInROI.Count / roi.Volume
        };
    }

    public void ColorPointsByROI(List<Point3D> points, List<Color> colors, ROI3D roi, Color insideColor, Color outsideColor)
    {
        for (int i = 0; i < points.Count; i++)
        {
            colors[i] = roi.ContainsPoint(points[i]) ? insideColor : outsideColor;
        }
    }

    private double CalculateStdDev(IEnumerable<double> values)
    {
        double mean = values.Average();
        double sumSquaredDiff = values.Sum(v => Math.Pow(v - mean, 2));
        return Math.Sqrt(sumSquaredDiff / values.Count());
    }
}

public class ROIStatistics
{
    public int PointCount { get; set; }
    public double MinValue { get; set; }
    public double MaxValue { get; set; }
    public double MeanValue { get; set; }
    public double StdDeviation { get; set; }
    public double Volume { get; set; }
    public double Density { get; set; }

    public override string ToString()
    {
        return $"Points: {PointCount}\n" +
               $"Mean: {MeanValue:F2} ± {StdDeviation:F2}\n" +
               $"Range: [{MinValue:F2}, {MaxValue:F2}]\n" +
               $"Volume: {Volume:F2}\n" +
               $"Density: {Density:F2} points/unit³";
    }
}
```

**Project:** Complete ROI analysis tool with data extraction and statistics

---

## Complete Example: ROI Application

```csharp
public class ROIApplication
{
    private List<ROI3D> rois = new List<ROI3D>();
    private ROI3D? selectedROI;
    private ROIVisualizer visualizer = new ROIVisualizer();
    private ROISelector selector = new ROISelector();
    private ROIDataExtractor extractor = new ROIDataExtractor();

    public void CreateNewROI(Point3D center, Size3D size, string name)
    {
        var roi = new ROI3D
        {
            Name = name,
            Center = center,
            Size = size,
            VisualizationColor = GetNextColor(),
            IsVisible = true
        };

        rois.Add(roi);
        UpdateVisualization();
    }

    public void SelectROI(ROI3D roi)
    {
        if (selectedROI != null)
            selectedROI.IsSelected = false;

        selectedROI = roi;
        roi.IsSelected = true;
        UpdateVisualization();
    }

    public void DeleteROI(ROI3D roi)
    {
        rois.Remove(roi);
        if (selectedROI == roi)
            selectedROI = null;
        UpdateVisualization();
    }

    public ROIStatistics AnalyzeROI(ROI3D roi, List<Point3D> dataPoints, List<double> values)
    {
        return extractor.CalculateStatistics(dataPoints, values, roi);
    }

    private void UpdateVisualization()
    {
        // Update 3D scene with ROI visualizations
        // Implementation depends on your viewport setup
    }
}
```

## Applications

ROI tools are used in:

1. **Medical Imaging**
   - Tumor volume measurement
   - Organ segmentation
   - Treatment planning

2. **Scientific Visualization**
   - Data filtering
   - Region comparison
   - Feature extraction

3. **3D Scanning**
   - Point cloud cropping
   - Object isolation
   - Noise removal

4. **CAD/Engineering**
   - Part selection
   - Measurement
   - Collision detection

## Next Module

**Module 5: Advanced ROI Manipulation** - Resize, transform, multiple ROIs, and advanced interactions!

---

**Time Investment:** 15 hours total
**Difficulty:** Intermediate

**This module is essential for all advanced 3D data interaction!**
