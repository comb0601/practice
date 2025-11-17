# Lesson 01: Hello 3D World

**Duration:** 2 hours
**Difficulty:** Beginner ⭐
**Prerequisites:** None - Start here!

---

## 🎯 Learning Objectives

By the end of this lesson, you will:
- ✅ Understand what WPF 3D is and how it works
- ✅ Set up a basic 3D viewport
- ✅ Create your first 3D triangle
- ✅ Understand the camera, lights, and geometry
- ✅ Run your first 3D application

---

## 📚 Theory (30 minutes)

### What is WPF 3D?

**WPF (Windows Presentation Foundation)** includes built-in 3D graphics capabilities using DirectX under the hood. You can create 3D scenes using C# and XAML without writing complex graphics code.

### Core Components

Every WPF 3D scene needs these 4 components:

#### 1. Viewport3D
The container that displays 3D content. Think of it as a window into the 3D world.

```xaml
<Viewport3D>
    <!-- 3D content goes here -->
</Viewport3D>
```

#### 2. Camera
Defines the viewpoint - where you're looking from and what direction.

```csharp
PerspectiveCamera camera = new PerspectiveCamera();
camera.Position = new Point3D(0, 0, 5);  // 5 units away
camera.LookDirection = new Vector3D(0, 0, -1);  // Looking forward
```

#### 3. Light
Illuminates objects so you can see them. Without light, everything is black!

```csharp
DirectionalLight light = new DirectionalLight();
light.Direction = new Vector3D(-1, -1, -3);
```

#### 4. Geometry (3D Model)
The actual 3D shape you want to display.

```csharp
MeshGeometry3D mesh = new MeshGeometry3D();
// Define vertices and triangles
```

### Coordinate System

WPF 3D uses a **right-handed coordinate system**:
- **X-axis**: Right (positive) / Left (negative)
- **Y-axis**: Up (positive) / Down (negative)
- **Z-axis**: Toward viewer (positive) / Away from viewer (negative)

```
        Y ↑
        |
        |
        +-----→ X
       /
      /
     Z
```

---

## 💻 Code Walkthrough (60 minutes)

### Step 1: Create the Project

The Solution folder contains a complete working project. Let's understand each part.

### Step 2: MainWindow.xaml

This is the UI layout. We define:
- A Window with black background
- A Viewport3D to hold our 3D scene
- Camera attached to the viewport
- Lights as ModelVisual3D
- Our 3D triangle as ModelVisual3D

### Step 3: MainWindow.xaml.cs

This is where we create our 3D triangle:

**Key Concepts:**

```csharp
// 1. Create a mesh (the shape)
MeshGeometry3D mesh = new MeshGeometry3D();

// 2. Define 3 vertices (corners of triangle)
mesh.Positions.Add(new Point3D(0, 1, 0));   // Top
mesh.Positions.Add(new Point3D(-1, -1, 0)); // Bottom-left
mesh.Positions.Add(new Point3D(1, -1, 0));  // Bottom-right

// 3. Define triangle (which vertices connect)
mesh.TriangleIndices.Add(0);  // Use vertex 0
mesh.TriangleIndices.Add(1);  // Connect to vertex 1
mesh.TriangleIndices.Add(2);  // Connect to vertex 2

// 4. Add material (color)
Material material = new DiffuseMaterial(
    new SolidColorBrush(Colors.Red));

// 5. Combine into geometry model
GeometryModel3D model = new GeometryModel3D(mesh, material);
```

### Understanding Triangle Indices

Triangles are defined using **indices** that reference positions:
- Index 0 → First vertex (top)
- Index 1 → Second vertex (bottom-left)
- Index 2 → Third vertex (bottom-right)

**Winding order matters!** Counter-clockwise is front-facing.

---

## 🏃 Running the Code (10 minutes)

```bash
cd Solution
dotnet run
```

You should see a **red triangle** in the center of a black window!

### What You're Seeing

- Black background → Empty 3D space
- Red triangle → Your first 3D geometry
- Triangle appears flat → No rotation yet (coming in later lessons)

---

## 🎨 Experiments (30 minutes)

Try these modifications to understand better:

### Experiment 1: Change the Color

**File:** `MainWindow.xaml.cs`, line ~35

```csharp
// Change from Red to Blue
new SolidColorBrush(Colors.Blue)
```

### Experiment 2: Move the Camera

**File:** `MainWindow.xaml`, camera Position

```xaml
<!-- Move camera closer -->
Position="0,0,3"

<!-- Move camera to the right -->
Position="2,0,5"

<!-- Move camera up -->
Position="0,2,5"
```

### Experiment 3: Make the Triangle Bigger

**File:** `MainWindow.xaml.cs`, vertex positions

```csharp
// Change scale by multiplying coordinates
mesh.Positions.Add(new Point3D(0, 2, 0));    // Top (doubled)
mesh.Positions.Add(new Point3D(-2, -2, 0));  // Bottom-left
mesh.Positions.Add(new Point3D(2, -2, 0));   // Bottom-right
```

### Experiment 4: Add a Second Triangle

After the first triangle code, add:

```csharp
// Create a green triangle next to the red one
var mesh2 = new MeshGeometry3D();
mesh2.Positions.Add(new Point3D(2, 1, 0));
mesh2.Positions.Add(new Point3D(1, -1, 0));
mesh2.Positions.Add(new Point3D(3, -1, 0));
mesh2.TriangleIndices.Add(0);
mesh2.TriangleIndices.Add(1);
mesh2.TriangleIndices.Add(2);

var material2 = new DiffuseMaterial(new SolidColorBrush(Colors.Green));
var model2 = new GeometryModel3D(mesh2, material2);

// Add to scene (you'll need to find the ModelVisual3D)
```

---

## 💪 Challenge (30 minutes)

Create a **3D pyramid** (4 triangles forming a pyramid).

**Requirements:**
1. Use 5 vertices (4 base corners + 1 top point)
2. Create 4 triangles (base + 3 sides)
3. Use different colors for each face
4. Position camera to see multiple faces

**Hints:**
- Base vertices at Y = -1
- Top vertex at Y = 1
- Each triangle needs 3 indices
- You'll need 4 GeometryModel3D objects

---

## 🐛 Troubleshooting

### I see a black screen
- **Check the camera position** - Make sure Z > 0
- **Check the light** - Is it enabled?
- **Check vertex positions** - Are they at Z = 0?

### Triangle is backwards/invisible
- **Fix winding order** - Triangle indices should be counter-clockwise
- **Add BackMaterial** - Makes both sides visible:
  ```csharp
  model.BackMaterial = material;
  ```

### Application won't run
- **Check .NET version** - Should be 6.0 or later
- **Run `dotnet build`** - Check for errors
- **Check namespace** - Make sure XAML and C# match

---

## 📖 Key Takeaways

✅ **Viewport3D** is the container for 3D scenes
✅ **Camera** defines what you see and from where
✅ **Light** makes objects visible
✅ **MeshGeometry3D** defines 3D shapes using vertices
✅ **Triangle indices** connect vertices into faces
✅ **Material** gives objects color

---

## 🎯 Next Lesson

**Lesson 02: Camera Systems** - Learn to control your viewpoint like a pro!

Topics:
- PerspectiveCamera vs OrthographicCamera
- LookDirection and UpDirection
- Field of view
- Camera movement

---

## 📚 Additional Resources

- [Microsoft WPF 3D Overview](https://docs.microsoft.com/en-us/dotnet/desktop/wpf/graphics-multimedia/3-d-graphics-overview)
- [Viewport3D Class Reference](https://docs.microsoft.com/en-us/dotnet/api/system.windows.controls.viewport3d)
- [Understanding 3D Coordinates](https://en.wikipedia.org/wiki/Cartesian_coordinate_system)

---

## ✅ Checklist

Before moving to Lesson 02, make sure you:
- [ ] Successfully ran the solution
- [ ] Changed the triangle color
- [ ] Moved the camera position
- [ ] Created a second triangle
- [ ] Attempted the pyramid challenge
- [ ] Understand vertices, indices, and materials

**Time to level up! 🚀**
