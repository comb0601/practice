# Lesson 4.2 - Bounding Box ROI

**Duration:** 3 hours
**Difficulty:** Intermediate

## Overview

Create and render professional-quality axis-aligned bounding box ROI with wireframes, transparency, and interactive handles. This is the foundation for all ROI manipulation.

## What You'll Build

A complete ROI visualization system featuring:
- Wireframe box rendering
- Semi-transparent fill for selected ROI
- Corner handles for manipulation
- Color-coded selection state
- Efficient mesh generation

## Key Classes

### ROI3D.cs
The core data model representing a 3D region of interest. Includes:
- Center point and size
- Bounding box calculations
- Point containment testing
- Corner point generation

### ROIVisualizer.cs
Generates 3D visual representations of ROI objects:
- Wireframe edges (12 lines)
- Transparent faces (6 quads)
- Corner handles (8 spheres)
- Selection highlighting

## Running This Lesson

This lesson provides the foundational classes used in the interactive application. To see them in action:

1. Copy these classes to your WPF project
2. Create a MainWindow with a Viewport3D
3. Instantiate an ROI3D object
4. Use ROIVisualizer to create its visualization
5. Add the visualization to your scene

## Example Usage

```csharp
// Create an ROI
var roi = new ROI3D
{
    Name = "Sample ROI",
    Center = new Point3D(0, 0, 0),
    Size = new Size3D(3, 2, 2),
    VisualizationColor = Colors.Cyan,
    IsSelected = true
};

// Visualize it
var visualizer = new ROIVisualizer();
var visualization = visualizer.CreateROIVisualization(roi);

// Add to scene
var visual = new ModelVisual3D { Content = visualization };
mainViewport.Children.Add(visual);
```

## Key Concepts Demonstrated

1. **Bounding Box Mathematics**
   - Center and half-extents representation
   - Min/Max coordinate calculation
   - Corner point generation

2. **3D Line Rendering**
   - Creating thin boxes as line segments
   - Axis-aligned optimization

3. **Transparency**
   - Alpha channel for selection feedback
   - Double-sided materials (BackMaterial)

4. **Procedural Geometry**
   - Sphere generation via spherical coordinates
   - Box mesh creation
   - Quad construction

## Next Steps

In the next lesson (4.3), you'll add mouse interaction to:
- Create ROI by clicking and dragging
- Select existing ROI
- Move and resize ROI
- Delete ROI

## Exercise

Extend the ROIVisualizer to add:
1. Dimension labels showing width, height, depth
2. A different visualization style (dashed lines, solid box, etc.)
3. Animation effects when selecting/deselecting

---

**Continue to Lesson 4.3 to make your ROI interactive!**
