# Lesson 1.1 - Your First 3D Scene

**Duration:** 2 hours
**Difficulty:** Beginner

## Overview

Welcome to your first WPF 3D lesson! In this lesson, you'll create a simple but complete 3D application featuring a rotating cube. This introduces you to the fundamental building blocks of WPF 3D graphics.

## Learning Objectives

- Understand the basic structure of a WPF 3D scene
- Learn about Viewport3D and its components
- Create a simple 3D geometry (cube)
- Add lighting to the scene
- Implement basic camera setup
- Add simple animations

## Key Concepts

### 1. Viewport3D

The `Viewport3D` is the container for all 3D content in WPF. Think of it as a window into the 3D world.

```xml
<Viewport3D>
    <!-- Camera, lights, and models go here -->
</Viewport3D>
```

### 2. PerspectiveCamera

The camera defines where you're looking from and in which direction:

- **Position**: Where the camera is located (0,0,6) - 6 units away from origin on Z-axis
- **LookDirection**: Which direction the camera points (0,0,-1) - looking toward negative Z
- **UpDirection**: Which way is "up" (0,1,0) - positive Y is up
- **FieldOfView**: The viewing angle in degrees (60°)

### 3. DirectionalLight

Lights illuminate the 3D objects:
- **DirectionalLight**: Light coming from a specific direction (like sunlight)
- **Color**: The color of the light
- **Direction**: Which way the light rays travel

### 4. GeometryModel3D

This combines three things:
1. **Geometry** (MeshGeometry3D): The shape (vertices and triangles)
2. **Material**: How the surface looks (color, texture)
3. **Transform**: Position, rotation, scale

### 5. MeshGeometry3D - The Cube

A cube is defined by:
- **8 vertices** (corner points)
- **12 triangles** (2 per face × 6 faces)

```
Vertices of a cube:
     4-------5
    /|      /|
   / |     / |
  0-------1  |
  |  7----|--6
  | /     | /
  |/      |/
  3-------2
```

The `TriangleIndices` property defines which vertices form each triangle.

## Code Walkthrough

### Step 1: Setting Up the Viewport

```xml
<Viewport3D x:Name="mainViewport">
    <Viewport3D.Camera>
        <PerspectiveCamera Position="0,0,6"
                         LookDirection="0,0,-1"
                         UpDirection="0,1,0"
                         FieldOfView="60"/>
    </Viewport3D.Camera>
    <!-- ... -->
</Viewport3D>
```

### Step 2: Adding Light

```xml
<ModelVisual3D>
    <ModelVisual3D.Content>
        <DirectionalLight Color="White" Direction="-1,-1,-1"/>
    </ModelVisual3D.Content>
</ModelVisual3D>
```

### Step 3: Creating the Cube Geometry

```xml
<MeshGeometry3D
    Positions="-1,1,1  1,1,1  1,-1,1  -1,-1,1
               -1,1,-1  1,1,-1  1,-1,-1  -1,-1,-1"
    TriangleIndices="0,1,2  0,2,3    <!-- Front face -->
                    1,5,6  1,6,2     <!-- Right face -->
                    5,4,7  5,7,6     <!-- Back face -->
                    4,0,3  4,3,7     <!-- Left face -->
                    4,5,1  4,1,0     <!-- Top face -->
                    3,2,6  3,6,7"/>  <!-- Bottom face -->
```

### Step 4: Adding Material (Color)

```xml
<GeometryModel3D.Material>
    <DiffuseMaterial Brush="#007ACC"/>
</GeometryModel3D.Material>
```

### Step 5: Adding Rotation Animation

```xml
<GeometryModel3D.Transform>
    <RotateTransform3D>
        <RotateTransform3D.Rotation>
            <AxisAngleRotation3D x:Name="rotation" Axis="0,1,0" Angle="0"/>
        </RotateTransform3D.Rotation>
    </RotateTransform3D>
</GeometryModel3D.Transform>
```

The animation is defined in the Window.Resources and controlled via code-behind.

## Running the Application

1. Build and run the application:
   ```bash
   dotnet build
   dotnet run
   ```

2. You should see a blue cube in the center of the window

3. Click "Start Rotation" to animate the cube

4. Click "Reset" to stop and reset the rotation

## Exercise

**Challenge 1: Add a Second Shape**

Add a sphere or pyramid next to the cube. You'll need to:
1. Create another `ModelVisual3D` element
2. Define a new geometry
3. Apply a different colored material
4. Position it using `TranslateTransform3D`

Hint: Use a pyramid with these vertices for a simple shape:
```
Positions: "0,1,0  -1,-1,-1  1,-1,-1  1,-1,1  -1,-1,1"
TriangleIndices: "0,1,2  0,2,3  0,3,4  0,4,1  1,4,3  1,3,2"
```

**Challenge 2: Change the Light**

Experiment with different light directions. Try:
- Direction="1,0,0" (from the right)
- Direction="0,-1,0" (from above)
- Add a second DirectionalLight with a different color

**Challenge 3: Camera Position**

Move the camera to different positions:
- Position="3,3,6" - view from upper right
- Position="-3,0,6" - view from left
- Position="0,5,5" - view from above

## Common Issues

**Black screen / Can't see the cube:**
- Check that the camera Position is positive Z (away from the origin)
- Ensure LookDirection points toward the cube (negative Z)
- Verify the DirectionalLight is present

**Cube looks flat:**
- Make sure you have proper lighting
- Check that all 12 triangles are defined in TriangleIndices

**Animation doesn't work:**
- Verify the Storyboard is in Window.Resources
- Check that the TargetName matches the AxisAngleRotation3D name

## Understanding the Coordinate System

WPF 3D uses a **right-handed coordinate system**:
- **X-axis**: Left (-) to Right (+)
- **Y-axis**: Down (-) to Up (+)
- **Z-axis**: Into screen (-) to Out of screen (+)

```
      +Y (Up)
       |
       |
       |_____+X (Right)
      /
     /
   +Z (Out)
```

## Next Steps

In **Lesson 1.2**, you'll create a visual coordinate system helper to better understand 3D positioning and learn advanced coordinate manipulation techniques.

## Time Breakdown

- **30 min**: Understanding concepts and code structure
- **30 min**: Building and running the application
- **30 min**: Completing exercises
- **30 min**: Experimentation and exploration

## Key Takeaways

- WPF 3D requires: Viewport3D, Camera, Lights, and Models
- Geometries are defined by vertices and triangle indices
- Materials define the appearance of surfaces
- Transforms can animate and position objects
- The coordinate system is right-handed with +Z coming out of the screen

---

**Congratulations!** You've created your first 3D WPF application. Continue to Lesson 1.2 to deepen your understanding of 3D coordinates.
