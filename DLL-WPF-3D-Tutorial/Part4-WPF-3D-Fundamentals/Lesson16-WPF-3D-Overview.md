# Lesson 16: WPF 3D Overview

## 📚 Learning Objectives

By the end of this lesson, you will understand:
- WPF's 3D capabilities and architecture
- The Viewport3D control
- Basic 3D coordinate system
- How 3D integrates with 2D XAML
- Setting up your first 3D scene
- The rendering pipeline

## 🎯 Introduction to WPF 3D

**WPF (Windows Presentation Foundation)** includes built-in 3D graphics support through **Viewport3D**.

### What Can You Build?

```
✅ 3D Model Viewers
✅ CAD/CAM Applications
✅ 3D Data Visualization
✅ Architectural Visualizations
✅ Simple 3D Games
✅ Product Configurators
✅ Scientific Simulations
```

### WPF 3D vs DirectX/OpenGL

| Feature | WPF 3D | DirectX/OpenGL |
|---------|--------|----------------|
| **Ease of Use** | ⭐⭐⭐⭐⭐ Easy | ⭐⭐ Complex |
| **Performance** | ⭐⭐⭐ Good | ⭐⭐⭐⭐⭐ Excellent |
| **XAML Integration** | ✅ Built-in | ❌ Manual |
| **Best For** | Business apps | Games, high-perf |
| **Learning Curve** | Gentle | Steep |

**Use WPF 3D when:**
- Building business/enterprise applications
- Need XAML integration
- Moderate 3D complexity
- Development speed matters

**Use DirectX/OpenGL when:**
- Building high-performance games
- Need advanced rendering techniques
- Maximum performance critical

## 🏗️ WPF 3D Architecture

### The 3D Pipeline

```
┌─────────────────────────────────────────────┐
│          Your WPF Application               │
└─────────────────┬───────────────────────────┘
                  │
┌─────────────────▼───────────────────────────┐
│            XAML Layer                       │
│  - Window, Grid, etc.                       │
│  - Viewport3D (3D container)                │
└─────────────────┬───────────────────────────┘
                  │
┌─────────────────▼───────────────────────────┐
│         3D Scene Graph                      │
│  - Models (geometry + materials)            │
│  - Lights                                   │
│  - Camera                                   │
└─────────────────┬───────────────────────────┘
                  │
┌─────────────────▼───────────────────────────┐
│        WPF 3D Rendering Engine              │
│  - Transform geometry                       │
│  - Apply lighting                           │
│  - Rasterization                            │
└─────────────────┬───────────────────────────┘
                  │
┌─────────────────▼───────────────────────────┐
│         DirectX (Hardware)                  │
│  - GPU rendering                            │
│  - Display to screen                        │
└─────────────────────────────────────────────┘
```

### Key Components

```
Viewport3D                 ← The 3D "container"
│
├── Camera                 ← Your viewpoint
│   ├── PerspectiveCamera  (realistic)
│   └── OrthographicCamera (isometric/technical)
│
├── ModelVisual3D          ← 3D objects
│   ├── GeometryModel3D    ← Shape + Material
│   │   ├── MeshGeometry3D (vertices, triangles)
│   │   └── Material       (appearance)
│   └── Light              ← Illumination
│       ├── DirectionalLight
│       ├── PointLight
│       ├── SpotLight
│       └── AmbientLight
│
└── Transforms             ← Position, rotation, scale
    ├── TranslateTransform3D
    ├── RotateTransform3D
    └── ScaleTransform3D
```

## 📐 3D Coordinate System

### Understanding 3D Space in WPF

```
       Y (Up)
       │
       │
       │
       └─────────── X (Right)
      ╱
     ╱
    ╱
   Z (Toward viewer)

Origin: (0, 0, 0)
```

**Coordinate System:**
- **X-axis:** Left (-) to Right (+)
- **Y-axis:** Down (-) to Up (+)
- **Z-axis:** Away from viewer (-) to Toward viewer (+)

**Right-handed system:** WPF uses a right-handed coordinate system.

### Example Positions

```
Point (2, 3, 5) means:
- 2 units to the right
- 3 units up
- 5 units toward the camera
```

## 🖼️ Your First 3D Scene

### Step 1: Create a WPF Application

```
Visual Studio:
1. File → New → Project
2. WPF Application (.NET Framework or .NET 6+)
3. Name: WPF3DDemo
4. Click Create
```

### Step 2: Add Viewport3D in XAML

**MainWindow.xaml:**

```xml
<Window x:Class="WPF3DDemo.MainWindow"
        xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
        Title="WPF 3D Demo" Height="600" Width="800">
    <Grid>
        <!-- 3D Viewport -->
        <Viewport3D>

            <!-- Camera: Your viewpoint -->
            <Viewport3D.Camera>
                <PerspectiveCamera Position="0,0,6"
                                   LookDirection="0,0,-1"
                                   UpDirection="0,1,0"
                                   FieldOfView="60"/>
            </Viewport3D.Camera>

            <!-- Model Container -->
            <ModelVisual3D>
                <ModelVisual3D.Content>
                    <Model3DGroup>

                        <!-- Light -->
                        <DirectionalLight Color="White"
                                          Direction="-1,-1,-3"/>

                        <!-- 3D Object: A Cube -->
                        <GeometryModel3D>
                            <GeometryModel3D.Geometry>
                                <MeshGeometry3D
                                    Positions="-1,-1,0 1,-1,0 1,1,0 -1,1,0"
                                    TriangleIndices="0,1,2 0,2,3"/>
                            </GeometryModel3D.Geometry>

                            <GeometryModel3D.Material>
                                <DiffuseMaterial Brush="Blue"/>
                            </GeometryModel3D.Material>
                        </GeometryModel3D>

                    </Model3DGroup>
                </ModelVisual3D.Content>
            </ModelVisual3D>

        </Viewport3D>
    </Grid>
</Window>
```

**Run it!** You should see a blue square (actually a 2D plane in 3D space).

### Step 3: Understanding the Code

#### Camera Setup

```xml
<PerspectiveCamera Position="0,0,6"        ← Camera location
                   LookDirection="0,0,-1"  ← Where camera points
                   UpDirection="0,1,0"     ← Which way is "up"
                   FieldOfView="60"/>      ← Viewing angle (degrees)
```

**Camera Position:** (0, 0, 6)
- X = 0 (centered horizontally)
- Y = 0 (centered vertically)
- Z = 6 (6 units toward the viewer)

**Look Direction:** (0, 0, -1)
- Looking straight into the scene (away from viewer)

#### Light Setup

```xml
<DirectionalLight Color="White"      ← Light color
                  Direction="-1,-1,-3"/>  ← Light direction
```

**Without light, you see nothing!** 3D objects need light to be visible.

#### Geometry Definition

```xml
<MeshGeometry3D
    Positions="-1,-1,0  1,-1,0  1,1,0  -1,1,0"
    TriangleIndices="0,1,2  0,2,3"/>
```

**Positions:** Vertices (corners) of the shape
```
Vertex 0: (-1, -1, 0) ← Bottom-left
Vertex 1: ( 1, -1, 0) ← Bottom-right
Vertex 2: ( 1,  1, 0) ← Top-right
Vertex 3: (-1,  1, 0) ← Top-left
```

**TriangleIndices:** How to connect vertices into triangles
```
Triangle 1: Vertices 0, 1, 2 (bottom-left, bottom-right, top-right)
Triangle 2: Vertices 0, 2, 3 (bottom-left, top-right, top-left)

Result: A square made of 2 triangles
```

#### Material

```xml
<DiffuseMaterial Brush="Blue"/>
```

Makes the object appear blue.

## 🎨 Creating a 3D Cube

Let's create a proper 3D cube (all 6 faces):

```xml
<GeometryModel3D>
    <GeometryModel3D.Geometry>
        <MeshGeometry3D>
            <!-- 8 vertices of a cube -->
            <MeshGeometry3D.Positions>
                <!-- Front face -->
                -1,-1,1  1,-1,1  1,1,1  -1,1,1
                <!-- Back face -->
                -1,-1,-1  1,-1,-1  1,1,-1  -1,1,-1
            </MeshGeometry3D.Positions>

            <!-- 12 triangles (2 per face × 6 faces) -->
            <MeshGeometry3D.TriangleIndices>
                <!-- Front face -->
                0,1,2  0,2,3
                <!-- Back face -->
                5,4,7  5,7,6
                <!-- Left face -->
                4,0,3  4,3,7
                <!-- Right face -->
                1,5,6  1,6,2
                <!-- Top face -->
                3,2,6  3,6,7
                <!-- Bottom face -->
                4,5,1  4,1,0
            </MeshGeometry3D.TriangleIndices>
        </MeshGeometry3D>
    </GeometryModel3D.Geometry>

    <GeometryModel3D.Material>
        <DiffuseMaterial Brush="Red"/>
    </GeometryModel3D.Material>
</GeometryModel3D>
```

## 🔄 Adding Rotation Animation

### XAML with Animation

```xml
<GeometryModel3D>
    <GeometryModel3D.Geometry>
        <!-- Cube geometry here -->
    </GeometryModel3D.Geometry>

    <GeometryModel3D.Material>
        <DiffuseMaterial Brush="Green"/>
    </GeometryModel3D.Material>

    <!-- Add rotation transform -->
    <GeometryModel3D.Transform>
        <RotateTransform3D>
            <RotateTransform3D.Rotation>
                <AxisAngleRotation3D x:Name="rotation"
                                     Axis="0,1,0"
                                     Angle="0"/>
            </RotateTransform3D.Rotation>
        </RotateTransform3D>
    </GeometryModel3D.Transform>
</GeometryModel3D>

<!-- Animation -->
<Viewport3D.Triggers>
    <EventTrigger RoutedEvent="Viewport3D.Loaded">
        <BeginStoryboard>
            <Storyboard>
                <DoubleAnimation Storyboard.TargetName="rotation"
                                 Storyboard.TargetProperty="Angle"
                                 From="0"
                                 To="360"
                                 Duration="0:0:4"
                                 RepeatBehavior="Forever"/>
            </Storyboard>
        </BeginStoryboard>
    </EventTrigger>
</Viewport3D.Triggers>
```

**Result:** Cube rotates around Y-axis continuously!

## 💻 Creating 3D Scene in Code-Behind

### MainWindow.xaml (Simple):

```xml
<Window x:Class="WPF3DDemo.MainWindow"
        xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
        Title="WPF 3D Demo" Height="600" Width="800"
        Loaded="Window_Loaded">
    <Grid>
        <Viewport3D x:Name="viewport"/>
    </Grid>
</Window>
```

### MainWindow.xaml.cs:

```csharp
using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Media3D;

namespace WPF3DDemo
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            // Create 3D scene programmatically
            Create3DScene();
        }

        private void Create3DScene()
        {
            // 1. Create Camera
            var camera = new PerspectiveCamera
            {
                Position = new Point3D(0, 0, 6),
                LookDirection = new Vector3D(0, 0, -1),
                UpDirection = new Vector3D(0, 1, 0),
                FieldOfView = 60
            };
            viewport.Camera = camera;

            // 2. Create Light
            var light = new DirectionalLight
            {
                Color = Colors.White,
                Direction = new Vector3D(-1, -1, -3)
            };

            // 3. Create Geometry (Triangle)
            var mesh = new MeshGeometry3D();
            mesh.Positions.Add(new Point3D(-1, -1, 0));  // Vertex 0
            mesh.Positions.Add(new Point3D(1, -1, 0));   // Vertex 1
            mesh.Positions.Add(new Point3D(0, 1, 0));    // Vertex 2

            mesh.TriangleIndices.Add(0);
            mesh.TriangleIndices.Add(1);
            mesh.TriangleIndices.Add(2);

            // 4. Create Material
            var material = new DiffuseMaterial(Brushes.Orange);

            // 5. Create GeometryModel3D
            var geometryModel = new GeometryModel3D
            {
                Geometry = mesh,
                Material = material
            };

            // 6. Create Model3DGroup
            var modelGroup = new Model3DGroup();
            modelGroup.Children.Add(light);
            modelGroup.Children.Add(geometryModel);

            // 7. Create ModelVisual3D
            var modelVisual = new ModelVisual3D
            {
                Content = modelGroup
            };

            // 8. Add to Viewport
            viewport.Children.Add(modelVisual);
        }
    }
}
```

## 🎓 Complete Example: Spinning Colored Cube

```csharp
using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Media3D;

namespace WPF3DDemo
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            Create3DScene();
        }

        private void Create3DScene()
        {
            // Camera
            viewport.Camera = new PerspectiveCamera
            {
                Position = new Point3D(0, 2, 8),
                LookDirection = new Vector3D(0, -0.5, -1),
                UpDirection = new Vector3D(0, 1, 0),
                FieldOfView = 60
            };

            // Light
            var light = new DirectionalLight(Colors.White, new Vector3D(-1, -1, -3));

            // Cube
            var cube = CreateCube();

            // Rotation
            var rotation = new AxisAngleRotation3D(new Vector3D(0, 1, 0), 0);
            var rotateTransform = new RotateTransform3D(rotation);
            cube.Transform = rotateTransform;

            // Model group
            var modelGroup = new Model3DGroup();
            modelGroup.Children.Add(light);
            modelGroup.Children.Add(cube);

            // Add to viewport
            var modelVisual = new ModelVisual3D { Content = modelGroup };
            viewport.Children.Add(modelVisual);

            // Animate rotation
            var animation = new DoubleAnimation
            {
                From = 0,
                To = 360,
                Duration = TimeSpan.FromSeconds(4),
                RepeatBehavior = RepeatBehavior.Forever
            };
            rotation.BeginAnimation(AxisAngleRotation3D.AngleProperty, animation);
        }

        private GeometryModel3D CreateCube()
        {
            var mesh = new MeshGeometry3D();

            // 8 vertices of cube
            mesh.Positions.Add(new Point3D(-1, -1, 1));  // 0: Front-bottom-left
            mesh.Positions.Add(new Point3D(1, -1, 1));   // 1: Front-bottom-right
            mesh.Positions.Add(new Point3D(1, 1, 1));    // 2: Front-top-right
            mesh.Positions.Add(new Point3D(-1, 1, 1));   // 3: Front-top-left
            mesh.Positions.Add(new Point3D(-1, -1, -1)); // 4: Back-bottom-left
            mesh.Positions.Add(new Point3D(1, -1, -1));  // 5: Back-bottom-right
            mesh.Positions.Add(new Point3D(1, 1, -1));   // 6: Back-top-right
            mesh.Positions.Add(new Point3D(-1, 1, -1));  // 7: Back-top-left

            // 12 triangles (6 faces × 2 triangles)
            int[] indices = {
                0,1,2, 0,2,3,  // Front
                5,4,7, 5,7,6,  // Back
                4,0,3, 4,3,7,  // Left
                1,5,6, 1,6,2,  // Right
                3,2,6, 3,6,7,  // Top
                4,5,1, 4,1,0   // Bottom
            };

            foreach (int index in indices)
                mesh.TriangleIndices.Add(index);

            // Gradient brush
            var brush = new LinearGradientBrush
            {
                StartPoint = new Point(0, 0),
                EndPoint = new Point(1, 1)
            };
            brush.GradientStops.Add(new GradientStop(Colors.Blue, 0.0));
            brush.GradientStops.Add(new GradientStop(Colors.Red, 1.0));

            return new GeometryModel3D
            {
                Geometry = mesh,
                Material = new DiffuseMaterial(brush)
            };
        }
    }
}
```

## 📝 Summary

### What We Learned

✅ WPF 3D uses Viewport3D for 3D content
✅ Essential components: Camera, Light, Geometry, Material
✅ WPF uses right-handed coordinate system
✅ 3D objects are made of triangles
✅ Can create 3D scenes in XAML or C#
✅ Animation works with 3D transforms

### Key Classes

- **Viewport3D** - Container for 3D content
- **PerspectiveCamera** - Your viewpoint
- **MeshGeometry3D** - 3D shape definition
- **GeometryModel3D** - Geometry + Material
- **Material** - Appearance (color, texture)
- **Light** - Illumination
- **Transform3D** - Position, rotation, scale

## 🚀 Next Steps

In **Lesson 17: 3D Geometry Basics**, we'll explore:
- Detailed mesh creation
- Normal vectors
- Texture coordinates
- Complex shapes
- Geometry helpers

## 💪 Practice Exercises

### Exercise 1: Create Different Shapes
Create:
- Pyramid (4 triangular faces)
- Rectangular prism
- Hexagon

### Exercise 2: Multiple Objects
Create a scene with:
- 3 cubes at different positions
- Different colors
- Different sizes

### Exercise 3: Camera Control
Add buttons to:
- Move camera left/right
- Move camera up/down
- Zoom in/out

---

**Excellent!** 🎉 You've created your first WPF 3D scene!

**Next Lesson:** [Lesson 17: 3D Geometry Basics](Lesson17-3D-Geometry-Basics.md)
