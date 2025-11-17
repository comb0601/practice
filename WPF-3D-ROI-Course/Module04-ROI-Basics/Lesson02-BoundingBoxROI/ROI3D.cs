using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Media3D;

namespace Lesson02_BoundingBoxROI;

/// <summary>
/// Represents a 3D Region of Interest (ROI) as an axis-aligned bounding box
/// </summary>
public class ROI3D
{
    public string Name { get; set; } = "ROI";
    public Point3D Center { get; set; }
    public Size3D Size { get; set; }
    public Color VisualizationColor { get; set; } = Colors.Cyan;
    public bool IsVisible { get; set; } = true;
    public bool IsSelected { get; set; } = false;

    // Bounding box properties
    public double MinX => Center.X - Size.X / 2;
    public double MaxX => Center.X + Size.X / 2;
    public double MinY => Center.Y - Size.Y / 2;
    public double MaxY => Center.Y + Size.Y / 2;
    public double MinZ => Center.Z - Size.Z / 2;
    public double MaxZ => Center.Z + Size.Z / 2;

    /// <summary>
    /// Check if a point is contained within this ROI
    /// </summary>
    public bool ContainsPoint(Point3D point)
    {
        return point.X >= MinX && point.X <= MaxX &&
               point.Y >= MinY && point.Y <= MaxY &&
               point.Z >= MinZ && point.Z <= MaxZ;
    }

    /// <summary>
    /// Get the volume of this ROI
    /// </summary>
    public double Volume => Size.X * Size.Y * Size.Z;

    /// <summary>
    /// Get the bounds as a Rect3D
    /// </summary>
    public Rect3D GetBounds()
    {
        return new Rect3D(MinX, MinY, MinZ, Size.X, Size.Y, Size.Z);
    }

    /// <summary>
    /// Get all 8 corner points of the bounding box
    /// </summary>
    public Point3D[] GetCorners()
    {
        return new Point3D[]
        {
            new Point3D(MinX, MinY, MinZ),  // 0: Bottom-left-front
            new Point3D(MaxX, MinY, MinZ),  // 1: Bottom-right-front
            new Point3D(MaxX, MaxY, MinZ),  // 2: Top-right-front
            new Point3D(MinX, MaxY, MinZ),  // 3: Top-left-front
            new Point3D(MinX, MinY, MaxZ),  // 4: Bottom-left-back
            new Point3D(MaxX, MinY, MaxZ),  // 5: Bottom-right-back
            new Point3D(MaxX, MaxY, MaxZ),  // 6: Top-right-back
            new Point3D(MinX, MaxY, MaxZ),  // 7: Top-left-back
        };
    }
}
