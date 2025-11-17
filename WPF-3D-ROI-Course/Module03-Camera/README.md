# Module 3: Camera and Viewport Control (10 hours)

## Overview

Master interactive 3D navigation! Learn to implement orbit cameras, pan/zoom controls, and mouse-based 3D interaction essential for professional 3D applications.

## Learning Objectives

- Implement orbit/arcball camera
- Add pan and zoom functionality
- Handle mouse input for 3D navigation
- Create camera presets and views
- Build professional camera controls

## Lessons

### Lesson 3.1 - Camera Basics (2 hours)
**Folder:** `Lesson01-CameraBasics/`

Understand PerspectiveCamera and OrthographicCamera in detail.

**Topics:**
- PerspectiveCamera properties
- OrthographicCamera for technical views
- Field of view and camera frustum
- Near and far clipping planes
- Camera coordinate system

**Key Concepts:**
```csharp
// Perspective camera - realistic view
var perspectiveCamera = new PerspectiveCamera
{
    Position = new Point3D(5, 3, 5),
    LookDirection = new Vector3D(-5, -3, -5),
    UpDirection = new Vector3D(0, 1, 0),
    FieldOfView = 60  // 45-90 typical
};

// Orthographic camera - no perspective (CAD/technical)
var orthoCamera = new OrthographicCamera
{
    Position = new Point3D(10, 10, 10),
    LookDirection = new Vector3D(-1, -1, -1),
    UpDirection = new Vector3D(0, 1, 0),
    Width = 10  // View width in world units
};
```

**Project:** Camera comparison tool showing perspective vs orthographic

---

### Lesson 3.2 - Orbit Camera (3 hours)
**Folder:** `Lesson02-OrbitCamera/`

Implement an orbit/arcball camera for intuitive 3D navigation.

**Topics:**
- Spherical coordinates (radius, theta, phi)
- Orbit around target point
- Maintaining up vector
- Gimbal lock prevention
- Smooth interpolation

**Implementation:**
```csharp
public class OrbitCamera
{
    public Point3D Target { get; set; } = new Point3D(0, 0, 0);
    public double Distance { get; set; } = 10;
    public double Theta { get; set; } = 45;  // Horizontal angle
    public double Phi { get; set; } = 30;    // Vertical angle

    public void UpdateCamera(PerspectiveCamera camera)
    {
        double thetaRad = Theta * Math.PI / 180;
        double phiRad = Phi * Math.PI / 180;

        double x = Distance * Math.Sin(phiRad) * Math.Cos(thetaRad);
        double y = Distance * Math.Cos(phiRad);
        double z = Distance * Math.Sin(phiRad) * Math.Sin(thetaRad);

        camera.Position = new Point3D(
            Target.X + x,
            Target.Y + y,
            Target.Z + z);

        camera.LookDirection = Target - camera.Position;
        camera.UpDirection = new Vector3D(0, 1, 0);
    }

    public void Orbit(double deltaTheta, double deltaPhi)
    {
        Theta += deltaTheta;
        Phi = Math.Clamp(Phi + deltaPhi, 1, 179); // Prevent gimbal lock
    }

    public void Zoom(double delta)
    {
        Distance = Math.Clamp(Distance * (1 - delta * 0.01), 1, 1000);
    }
}
```

**Project:** Full orbit camera with mouse control

---

### Lesson 3.3 - Pan and Zoom (2 hours)
**Folder:** `Lesson03-PanZoom/`

Add panning and zooming capabilities to your camera.

**Topics:**
- Panning in screen space
- Camera-relative movement
- Zoom vs dolly
- Constraint systems
- Smooth damping

**Pan Implementation:**
```csharp
public void Pan(double deltaX, double deltaY, PerspectiveCamera camera)
{
    // Get camera right and up vectors
    Vector3D lookDir = camera.LookDirection;
    lookDir.Normalize();

    Vector3D right = Vector3D.CrossProduct(lookDir, camera.UpDirection);
    right.Normalize();

    Vector3D up = Vector3D.CrossProduct(right, lookDir);
    up.Normalize();

    // Move target in screen space
    double panSpeed = Distance * 0.001;
    Target += right * (deltaX * panSpeed);
    Target += up * (deltaY * panSpeed);
}
```

**Project:** Combined orbit/pan/zoom camera controller

---

### Lesson 3.4 - Mouse Interaction (3 hours)
**Folder:** `Lesson04-MouseInteraction/`

Complete mouse-based 3D navigation system.

**Topics:**
- Mouse event handling
- Left/Right/Middle button mapping
- Drag operations
- Scroll wheel zoom
- Touch support basics

**Complete Implementation:**
```csharp
public class CameraController
{
    private Point lastMousePosition;
    private bool isOrbiting;
    private bool isPanning;

    public void OnMouseDown(MouseButtonEventArgs e)
    {
        if (e.LeftButton == MouseButtonState.Pressed)
        {
            isOrbiting = true;
            lastMousePosition = e.GetPosition(viewport);
            viewport.CaptureMouse();
        }
        else if (e.MiddleButton == MouseButtonState.Pressed)
        {
            isPanning = true;
            lastMousePosition = e.GetPosition(viewport);
            viewport.CaptureMouse();
        }
    }

    public void OnMouseMove(MouseEventArgs e)
    {
        Point currentPosition = e.GetPosition(viewport);
        Vector delta = currentPosition - lastMousePosition;

        if (isOrbiting)
        {
            orbitCamera.Orbit(delta.X * 0.5, delta.Y * 0.5);
            orbitCamera.UpdateCamera(camera);
        }
        else if (isPanning)
        {
            orbitCamera.Pan(-delta.X, delta.Y, camera);
            orbitCamera.UpdateCamera(camera);
        }

        lastMousePosition = currentPosition;
    }

    public void OnMouseUp(MouseButtonEventArgs e)
    {
        isOrbiting = false;
        isPanning = false;
        viewport.ReleaseMouseCapture();
    }

    public void OnMouseWheel(MouseWheelEventArgs e)
    {
        orbitCamera.Zoom(e.Delta * 0.1);
        orbitCamera.UpdateCamera(camera);
    }
}
```

**Project:** Professional 3D viewer with full camera control

**Common Patterns:**
- Left mouse: Orbit/Rotate
- Middle mouse: Pan
- Right mouse: Context menu
- Scroll wheel: Zoom
- Shift+Left: Pan (alternative)

---

## Practical Exercises

### Exercise 1: Camera Presets

Add preset views:
```csharp
public enum ViewPreset
{
    Front,    // (0, 0, distance)
    Back,     // (0, 0, -distance)
    Left,     // (-distance, 0, 0)
    Right,    // (distance, 0, 0)
    Top,      // (0, distance, 0)
    Bottom,   // (0, -distance, 0)
    Isometric // (1, 1, 1) * distance
}

public void SetPreset(ViewPreset preset, double distance)
{
    switch (preset)
    {
        case ViewPreset.Front:
            Theta = 0; Phi = 90;
            break;
        case ViewPreset.Isometric:
            Theta = 45; Phi = 35.264;  // arctan(1/sqrt(2))
            break;
        // ...
    }
    Distance = distance;
    UpdateCamera(camera);
}
```

### Exercise 2: Camera Animation

Smooth transitions between views:
```csharp
public void AnimateToView(double targetTheta, double targetPhi, double targetDistance, double duration)
{
    var storyboard = new Storyboard();

    var thetaAnim = new DoubleAnimation(Theta, targetTheta, TimeSpan.FromSeconds(duration))
    {
        EasingFunction = new CubicEase { EasingMode = EasingMode.EaseInOut }
    };

    var phiAnim = new DoubleAnimation(Phi, targetPhi, TimeSpan.FromSeconds(duration))
    {
        EasingFunction = new CubicEase { EasingMode = EasingMode.EaseInOut }
    };

    // Apply animations...
}
```

### Exercise 3: Focus on Object

Implement "fit to view":
```csharp
public void FocusOnBounds(Rect3D bounds)
{
    // Calculate center
    Target = new Point3D(
        bounds.X + bounds.SizeX / 2,
        bounds.Y + bounds.SizeY / 2,
        bounds.Z + bounds.SizeZ / 2);

    // Calculate distance to fit object in view
    double maxDimension = Math.Max(bounds.SizeX, Math.Max(bounds.SizeY, bounds.SizeZ));
    Distance = maxDimension / Math.Tan(camera.FieldOfView * Math.PI / 360) * 1.5;

    UpdateCamera(camera);
}
```

## Key Concepts

### Spherical Coordinates

Convert between Cartesian (x, y, z) and Spherical (r, θ, φ):

```
x = r * sin(φ) * cos(θ)
y = r * cos(φ)
z = r * sin(φ) * sin(θ)

r = sqrt(x² + y² + z²)
θ = atan2(z, x)
φ = acos(y / r)
```

### Camera Vectors

- **Position**: Where the camera is
- **LookDirection**: Where it's pointing (normalized = forward)
- **UpDirection**: Which way is up (usually (0, 1, 0))
- **Right**: Cross product of LookDirection × UpDirection

## Performance Tips

1. **Throttle Updates**: Don't update every mouse move
2. **Use Animations**: Smooth camera movement
3. **Cache Calculations**: Store computed vectors
4. **Limit Range**: Clamp angles and distance

## Next Module

**Module 4: ROI Basics** - Now that you can navigate 3D space, learn to select and manipulate regions of interest!

---

**Time Investment:** 10 hours total
**Difficulty:** Intermediate

This module is critical for interactive 3D applications and essential foundation for ROI manipulation!
