# Lesson 02: Camera Systems - Complete Guide

**Duration:** 2 hours | **Difficulty:** ⭐⭐ | **Status:** ✅ COMPLETE WITH CODE

---

## 🎯 What You'll Build

A **complete camera exploration tool** that lets you:
- Switch between Perspective and Orthographic cameras
- Control camera position with sliders
- Adjust field of view in real-time
- Use preset camera views (Front, Top, Side, Isometric)
- Understand how cameras affect 3D perception

**You'll see:** A 3D scene with cubes that you can view from any angle!

---

## 📚 Theory (20 minutes)

### What is a Camera?

In 3D graphics, a **camera** defines:
1. **WHERE** you're looking from (Position)
2. **WHAT DIRECTION** you're looking (LookDirection)
3. **WHICH WAY IS UP** (UpDirection)
4. **HOW WIDE** you can see (FieldOfView)

Think of it like a real camera:
- Position = Where you stand
- LookDirection = Where you point the camera
- UpDirection = Making sure the camera isn't tilted
- FieldOfView = Zoom level (wide angle vs telephoto)

### Two Camera Types

#### PerspectiveCamera
- **Realistic** - Objects farther away look smaller
- **Uses:** Games, realistic visualizations
- **Has:** Field of View (FOV)

```csharp
var camera = new PerspectiveCamera {
    Position = new Point3D(5, 3, 8),
    LookDirection = new Vector3D(-5, -3, -8),
    UpDirection = new Vector3D(0, 1, 0),
    FieldOfView = 60  // degrees
};
```

#### OrthographicCamera
- **Technical** - No perspective, parallel lines stay parallel
- **Uses:** CAD, technical drawings, isometric games
- **Has:** Width (how much you see)

```csharp
var camera = new OrthographicCamera {
    Position = new Point3D(5, 3, 8),
    LookDirection = new Vector3D(-5, -3, -8),
    UpDirection = new Vector3D(0, 1, 0),
    Width = 10
};
```

### Understanding Vectors

**Point3D** = A location in 3D space (where something IS)
**Vector3D** = A direction and distance (where something GOES)

Example:
```csharp
Point3D position = new Point3D(0, 5, 10);  // Camera is here
Vector3D lookDir = new Vector3D(0, -1, -2); // Looking this way
```

The camera looks FROM position ALONG lookDir.

---

## 💻 The Application

Run the solution to see:
- A scene with colored cubes
- Camera type switcher (Perspective/Orthographic)
- Position sliders (X, Y, Z)
- FOV/Width slider
- Preset buttons (Front, Top, Side, Iso)
- Real-time camera info display

### Key Features

**1. Interactive Camera Control**
Move sliders to change camera position in real-time

**2. Camera Type Switching**
Toggle between perspective and orthographic to see the difference

**3. Preset Views**
One-click camera positions for common viewpoints

**4. Visual Feedback**
See exactly where your camera is and what it's looking at

---

## 🎮 Running the Code

```bash
cd Solution
dotnet run
```

### What to Try

1. **Change Position Sliders**
   - Move X → Camera orbits left/right
   - Move Y → Camera goes up/down
   - Move Z → Camera moves closer/farther

2. **Switch Camera Types**
   - Click "Perspective" → Realistic view
   - Click "Orthographic" → Technical view
   - Notice how cubes look different!

3. **Adjust Field of View** (Perspective mode)
   - Low FOV (30°) → Zoomed in, telephoto
   - High FOV (90°) → Wide angle, fish-eye effect

4. **Try Preset Views**
   - Front: Look along Z-axis
   - Top: Bird's eye view
   - Side: Look from the side
   - Isometric: 45° angle (like classic games)

---

## 🔍 Code Exploration

### MainWindow.xaml

Key sections to understand:

**Camera Definition:**
```xaml
<Viewport3D.Camera>
    <PerspectiveCamera x:Name="perspectiveCamera"
                     Position="5,5,10"
                     LookDirection="-5,-5,-10"
                     UpDirection="0,1,0"
                     FieldOfView="60"/>
</Viewport3D.Camera>
```

**Control Sliders:**
```xaml
<Slider x:Name="posXSlider"
        Minimum="-15" Maximum="15"
        ValueChanged="CameraPosition_Changed"/>
```

### MainWindow.xaml.cs

Key functions to study:

**1. UpdateCamera()** - Applies slider values to camera
**2. SwitchToPerspective()** - Changes camera type
**3. SetPresetView()** - Preset camera positions
**4. CameraPosition_Changed()** - Real-time updates

---

## 🎨 Experiments

### Experiment 1: Orbit Camera

Make the camera orbit around the center:

```csharp
// Add to code:
double time = 0;
var timer = new DispatcherTimer { Interval = TimeSpan.FromMilliseconds(16) };
timer.Tick += (s, e) => {
    time += 0.02;
    double radius = 10;
    camera.Position = new Point3D(
        Math.Cos(time) * radius,
        5,
        Math.Sin(time) * radius
    );
    camera.LookDirection = new Vector3D(
        -camera.Position.X,
        -camera.Position.Y,
        -camera.Position.Z
    );
};
timer.Start();
```

### Experiment 2: First-Person Camera

Try looking around from a fixed position:
- Keep Position constant
- Change only LookDirection
- Add rotation based on mouse movement

### Experiment 3: Extreme FOV

Try these Field of View values:
- FOV = 10 → Super telephoto (like binoculars)
- FOV = 120 → Fish-eye lens effect
- FOV = 179 → Almost 180° vision!

Notice how it distorts the scene.

### Experiment 4: Flying Camera

Make camera respond to WASD keys:
- W = Move forward along LookDirection
- S = Move backward
- A/D = Strafe left/right
- Q/E = Up/down

---

## 💪 Challenge: Camera Path Animation

Create a smooth camera animation that:

1. **Starts** at Front view
2. **Orbits** around the scene (360°)
3. **Moves up** to Top view
4. **Returns** to original position
5. **Loops** continuously

**Requirements:**
- Use DispatcherTimer for animation
- Smooth transitions (use sine/cosine)
- Should take ~10 seconds for full path
- Camera always looks at center (0,0,0)

**Hints:**
```csharp
// Parametric circle for orbit
x = radius * cos(angle)
z = radius * sin(angle)

// Smooth up/down with sine
y = amplitude * sin(time)
```

---

## 📖 Key Concepts

### Camera Position
Where the camera exists in 3D space
- `Point3D(x, y, z)`
- Independent of what you're looking at

### Look Direction
A vector pointing where the camera faces
- `Vector3D(dx, dy, dz)`
- NOT a position, it's a direction!
- Length doesn't matter (normalized internally)

### Up Direction
Defines which way is "up" for the camera
- Usually `Vector3D(0, 1, 0)` (Y+ is up)
- Prevents camera from rolling/tilting

### Field of View (FOV)
How wide the camera can see (in degrees)
- 60° = Normal human vision
- 90° = Wide angle
- 30° = Telephoto/zoomed
- Only for PerspectiveCamera!

### Width (Orthographic)
How many units wide the view is
- Larger = Zoom out (see more)
- Smaller = Zoom in (see less)
- Only for OrthographicCamera!

---

## 🧮 Math Corner

### Calculating Look Direction

If you want to look AT a specific point:

```csharp
Point3D cameraPos = new Point3D(5, 5, 10);
Point3D targetPos = new Point3D(0, 0, 0);  // Look at origin

Vector3D lookDir = targetPos - cameraPos;
// Result: Vector3D(-5, -5, -10)
```

### Normalizing Vectors

To get a unit vector (length = 1):

```csharp
Vector3D dir = new Vector3D(3, 4, 0);
dir.Normalize();
// Now dir has length 1.0
// dir = (0.6, 0.8, 0)
```

### Orbit Calculation

To orbit at distance R:

```csharp
double angle = time * speed;
double radius = 10;

double x = Math.Cos(angle) * radius;
double z = Math.Sin(angle) * radius;
double y = 5; // constant height

camera.Position = new Point3D(x, y, z);
```

---

## 🐛 Common Issues

### Camera shows nothing (black screen)
- **Check Position** - Is it inside an object?
- **Check LookDirection** - Is it pointing away from scene?
- **Check near/far planes** - Objects might be clipped

### Objects look distorted
- **FOV too high** - Try 60° instead of 120°
- **Camera too close** - Move farther away
- **Wrong aspect ratio** - Check viewport size

### Camera doesn't update
- **Forgot to call UpdateCamera()** after changing values
- **Event not hooked up** - Check ValueChanged events
- **Wrong camera reference** - Make sure you're updating the active camera

### Up direction causes weird rotation
- **Not normalized** - UpDirection should be unit length
- **Parallel to LookDirection** - They must be perpendicular
- **Use (0,1,0)** for normal scenes

---

## 🎯 What You Learned

✅ Two camera types and when to use each
✅ Camera properties: Position, LookDirection, UpDirection
✅ Field of View and how it affects perception
✅ Preset camera views for common angles
✅ How to calculate camera positions mathematically
✅ Interactive camera control with UI
✅ Difference between Points and Vectors

---

## 📚 Additional Resources

- [PerspectiveCamera Class](https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.media3d.perspectivecamera)
- [OrthographicCamera Class](https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.media3d.orthographiccamera)
- [Understanding Camera Projection](https://en.wikipedia.org/wiki/3D_projection)
- [Field of View Explained](https://en.wikipedia.org/wiki/Field_of_view)

---

## ✅ Completion Checklist

Before moving to Lesson 03:

- [ ] Ran the application successfully
- [ ] Switched between Perspective and Orthographic
- [ ] Moved camera position with sliders
- [ ] Tried all preset views
- [ ] Adjusted Field of View and understood the effect
- [ ] Read and understood all code comments
- [ ] Completed at least 2 experiments
- [ ] Attempted the camera path challenge

---

## 🎮 Next Lesson

**Lesson 03: Basic Shapes** - Create cubes, spheres, and cylinders from scratch!

You'll learn:
- Mesh topology and vertex math
- Creating procedural shapes
- Optimization techniques
- Reusable shape generators

**Get ready to build geometry! 🎲**
