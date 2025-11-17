# Lesson 1.4 - Basic Transformations

**Duration:** 2 hours
**Difficulty:** Beginner

## Overview

Learn to move, rotate, and scale 3D objects using transforms. Master Transform3DGroup to combine multiple transformations and create complex animations.

## Learning Objectives

- TranslateTransform3D for positioning
- RotateTransform3D for rotation
- ScaleTransform3D for sizing
- Transform3DGroup for combining transforms
- Creating smooth animations

## Key Transformations

### Translate (Move)
```csharp
var translate = new TranslateTransform3D(x, y, z);
model.Transform = translate;
```

### Rotate
```csharp
var rotate = new RotateTransform3D(
    new AxisAngleRotation3D(new Vector3D(0,1,0), 45));
```

### Scale
```csharp
var scale = new ScaleTransform3D(2, 2, 2); // Double size
```

### Combine Multiple Transforms
```csharp
var group = new Transform3DGroup();
group.Children.Add(new ScaleTransform3D(1.5, 1.5, 1.5));
group.Children.Add(new RotateTransform3D(...));
group.Children.Add(new TranslateTransform3D(2, 0, 0));
model.Transform = group;
```

## Exercise

Create a solar system with rotating planets using Transform3DGroup!

See full implementation in source files.
