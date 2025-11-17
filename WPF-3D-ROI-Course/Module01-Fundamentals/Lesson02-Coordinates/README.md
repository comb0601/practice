# Lesson 1.2 - Understanding 3D Coordinates

**Duration:** 2 hours
**Difficulty:** Beginner

## Overview

This lesson provides an interactive exploration of the WPF 3D coordinate system. You'll build a visualization tool with coordinate axes, a grid plane, and an interactive point that you can move through 3D space.

## Learning Objectives

- Understand the right-handed coordinate system used in WPF 3D
- Visualize the X, Y, and Z axes
- Learn how Point3D and Vector3D work
- Create helper geometries (axes, grid planes)
- Work with TranslateTransform3D for positioning

## Key Concepts

### The Right-Handed Coordinate System

WPF 3D uses a **right-handed coordinate system**:

```
      +Y (Up)
       |
       |
       |_____+X (Right)
      /
     /
   +Z (Toward viewer)
```

**Memory trick:** Point your right thumb along +X (right), index finger along +Y (up), and your middle finger shows +Z (out of screen).

### Axes Color Convention

In this lesson and throughout the course, we use:
- **Red**: X-axis (left-right)
- **Green**: Y-axis (down-up)
- **Blue**: Z-axis (in-out)

This is a common convention in 3D graphics applications.

### Point3D vs Vector3D

**Point3D**: Represents a location in 3D space
```csharp
Point3D position = new Point3D(2.0, 3.0, 1.0);
```

**Vector3D**: Represents a direction and magnitude
```csharp
Vector3D direction = new Vector3D(1.0, 0.0, 0.0); // Points along +X axis
double length = direction.Length; // Magnitude
direction.Normalize(); // Convert to unit vector
```

## Code Walkthrough

### Creating the Coordinate Axes

```csharp
private void CreateCoordinateAxes()
{
    var axesGroup = new Model3DGroup();

    // X-Axis (Red)
    axesGroup.Children.Add(CreateAxisArrow(
        new Point3D(0, 0, 0),
        new Point3D(5, 0, 0),
        Colors.Red,
        0.05));

    // Y-Axis (Green)
    axesGroup.Children.Add(CreateAxisArrow(
        new Point3D(0, 0, 0),
        new Point3D(0, 5, 0),
        Colors.Green,
        0.05));

    // Z-Axis (Blue)
    axesGroup.Children.Add(CreateAxisArrow(
        new Point3D(0, 0, 0),
        new Point3D(0, 0, 5),
        Colors.Blue,
        0.05));

    coordinateAxes.Content = axesGroup;
}
```

Each axis is created as a thin box extending from the origin (0,0,0) to a point 5 units away.

### Creating a Grid Plane

The grid helps visualize the XZ plane at Y=0:

```csharp
private void CreateGridPlane()
{
    var gridGroup = new Model3DGroup();
    int gridSize = 10;
    double spacing = 1.0;

    for (int i = -gridSize; i <= gridSize; i++)
    {
        // Lines parallel to X-axis
        gridGroup.Children.Add(CreateLine(
            new Point3D(-gridSize * spacing, 0, i * spacing),
            new Point3D(gridSize * spacing, 0, i * spacing),
            gridColor, 0.01));

        // Lines parallel to Z-axis
        gridGroup.Children.Add(CreateLine(
            new Point3D(i * spacing, 0, -gridSize * spacing),
            new Point3D(i * spacing, 0, gridSize * spacing),
            gridColor, 0.01));
    }

    gridPlane.Content = gridGroup;
}
```

### Moving Objects with TranslateTransform3D

```csharp
private void PositionSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
{
    if (testPoint?.Transform is TranslateTransform3D transform)
    {
        transform.OffsetX = sliderX.Value;
        transform.OffsetY = sliderY.Value;
        transform.OffsetZ = sliderZ.Value;
    }
}
```

The `TranslateTransform3D` moves an object in 3D space without changing its rotation or scale.

### Creating Spheres Programmatically

The sphere is created using latitude/longitude parameterization:

```csharp
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
```

This is the spherical coordinate formula converted to Cartesian coordinates.

## Running the Application

1. Build and run:
   ```bash
   dotnet build
   dotnet run
   ```

2. Use the sliders on the right to move the yellow sphere

3. Toggle the grid and axes visibility

4. Watch the status bar show the current coordinates

## Interactive Exercises

### Exercise 1: Understand Each Axis

Move the sphere along each axis individually:
1. Set Y=0, Z=0, move X from -5 to 5 (left to right)
2. Set X=0, Z=0, move Y from -5 to 5 (down to up)
3. Set X=0, Y=0, move Z from -5 to 5 (in to out)

### Exercise 2: Diagonal Movement

Move the sphere to these positions and observe:
- (3, 3, 0) - Upper right, on grid
- (3, 0, 3) - Right and toward viewer
- (3, 3, 3) - Upper right corner toward viewer
- (-3, -3, -3) - Lower left corner away from viewer

### Exercise 3: Add More Spheres

Modify the code to add fixed spheres at interesting coordinates:
- Origin (0, 0, 0)
- Unit sphere at each positive axis endpoint: (1,0,0), (0,1,0), (0,0,1)
- Corner: (1,1,1)

Example code:
```csharp
private void AddFixedSpheres()
{
    var group = new Model3DGroup();

    // Origin
    group.Children.Add(CreateSphere(new Point3D(0, 0, 0), 0.2, Colors.White));

    // Unit positions
    group.Children.Add(CreateSphere(new Point3D(1, 0, 0), 0.15, Colors.Red));
    group.Children.Add(CreateSphere(new Point3D(0, 1, 0), 0.15, Colors.Green));
    group.Children.Add(CreateSphere(new Point3D(0, 0, 1), 0.15, Colors.Blue));

    // Add to scene
    var visual = new ModelVisual3D { Content = group };
    mainViewport.Children.Add(visual);
}
```

### Exercise 4: Create a Coordinate Helper Class

Create a reusable class for coordinate system visualization:

```csharp
public class CoordinateSystemHelper
{
    public static Model3DGroup CreateAxes(double length, double thickness)
    {
        // Implementation here
    }

    public static Model3DGroup CreateGrid(int size, double spacing, Color color)
    {
        // Implementation here
    }
}
```

## Understanding Transformations

### TranslateTransform3D (Position)
Moves an object without rotating or scaling it:
```csharp
var translate = new TranslateTransform3D(x, y, z);
```

### Combining Transforms
You can combine multiple transforms:
```csharp
var transformGroup = new Transform3DGroup();
transformGroup.Children.Add(new TranslateTransform3D(2, 0, 0));
transformGroup.Children.Add(new RotateTransform3D(...));
model.Transform = transformGroup;
```

## Common Coordinate Mistakes

1. **Forgetting Y is UP**: In many systems, Y points down. In WPF 3D, positive Y is UP.

2. **Wrong-handed system**: Using left-handed conventions from other systems.

3. **Camera confusion**: The camera LookDirection is where the camera points, not where it is.

4. **Transform order**: Transforms are applied in the order they're added to Transform3DGroup.

## Debugging Tips

When objects don't appear where expected:

1. **Check the camera**: Ensure the camera can see the position
2. **Verify coordinates**: Print Point3D values to debug output
3. **Use the grid**: Turn on the grid to see the XZ plane
4. **Simplify**: Test with one axis at a time

## Practical Applications

Understanding coordinates is essential for:
- Positioning 3D models correctly
- Calculating distances between points
- Creating measurement tools
- Implementing collision detection
- Building ROI selection tools (upcoming lessons)

## Next Steps

In **Lesson 1.3**, you'll explore lights and materials in detail, learning how to create realistic surface appearances and complex lighting scenarios.

## Time Breakdown

- **30 min**: Understanding coordinate systems theory
- **45 min**: Building and experimenting with the app
- **30 min**: Completing exercises
- **15 min**: Creating helper utilities

## Key Takeaways

- WPF 3D uses a right-handed coordinate system
- +X is right, +Y is up, +Z is toward the viewer
- Point3D represents positions, Vector3D represents directions
- TranslateTransform3D moves objects in 3D space
- Coordinate helpers (axes, grids) are invaluable debugging tools
- Always verify your coordinate assumptions visually

---

**Great work!** You now have a solid understanding of 3D coordinates. Continue to **Lesson 1.3** to learn about lights and materials.
