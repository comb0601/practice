# Lesson 2.5 - Point Cloud Rendering

**Duration:** 3 hours

## Point Cloud Fundamentals

Point clouds are collections of 3D points, each with position and optionally color/intensity.

### Use Cases
- LiDAR data
- 3D scanning
- Medical imaging (CT/MRI)
- Scientific simulations
- Particle systems

### Rendering Strategy

```csharp
public class PointCloudRenderer
{
    public Model3DGroup CreatePointCloud(List<Point3D> points, List<Color> colors, double pointSize)
    {
        var group = new Model3DGroup();

        for (int i = 0; i < points.Count; i++)
        {
            // Create small sphere or box for each point
            var geometry = CreatePointGeometry(pointSize);
            var material = new DiffuseMaterial(new SolidColorBrush(colors[i]));

            var model = new GeometryModel3D(geometry, material);
            model.Transform = new TranslateTransform3D(
                points[i].X,
                points[i].Y,
                points[i].Z);

            group.Children.Add(model);
        }

        return group;
    }

    private MeshGeometry3D CreatePointGeometry(double size)
    {
        // Create tiny cube or use instanced rendering
        return CreateCube(size);
    }
}
```

### Optimization Techniques

1. **LOD**: Reduce point density when zoomed out
2. **Culling**: Only render points in view
3. **Instancing**: Reuse geometry (advanced)
4. **Point sprites**: Use pixel-based rendering

### Color Mapping

```csharp
public Color MapValueToColor(double value, double min, double max)
{
    // Normalize to 0-1
    double normalized = (value - min) / (max - min);

    // Heat map: Blue -> Green -> Yellow -> Red
    if (normalized < 0.25)
    {
        double t = normalized / 0.25;
        return Color.FromRgb(0, (byte)(t * 255), 255);
    }
    else if (normalized < 0.5)
    {
        double t = (normalized - 0.25) / 0.25;
        return Color.FromRgb(0, 255, (byte)((1 - t) * 255));
    }
    else if (normalized < 0.75)
    {
        double t = (normalized - 0.5) / 0.25;
        return Color.FromRgb((byte)(t * 255), 255, 0);
    }
    else
    {
        double t = (normalized - 0.75) / 0.25;
        return Color.FromRgb(255, (byte)((1 - t) * 255), 0);
    }
}
```

## Project

Build a point cloud viewer that:
- Loads point data from CSV
- Colors points by value (heat map)
- Supports zooming and rotation
- Filters points by value range

This is essential preparation for ROI-based filtering in Module 4!
