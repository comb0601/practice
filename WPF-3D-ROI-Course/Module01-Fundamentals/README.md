# Module 1: WPF 3D Fundamentals (10 hours)

## Overview

This module introduces you to the foundations of WPF 3D graphics. You'll learn how to set up 3D scenes, understand coordinate systems, work with lights and materials, and create basic transformations.

## Learning Objectives

By the end of this module, you will be able to:
- Create and configure a basic 3D viewport
- Understand WPF 3D coordinate system and transformations
- Work with lights and materials
- Apply transformations to 3D objects
- Manage multiple objects in a 3D scene

## Prerequisites

- .NET 9.0 SDK installed
- Visual Studio 2022 or later
- Basic C# and XAML knowledge

## Lessons

### Lesson 1.1 - First 3D Scene (2 hours)
**Folder:** `Lesson01-FirstScene/`

Learn to create your very first 3D scene in WPF with a simple cube.

**Concepts:**
- Viewport3D basics
- PerspectiveCamera
- ModelVisual3D
- Basic cube geometry
- DirectionalLight

**Exercise:** Add a second primitive shape to the scene

---

### Lesson 1.2 - Understanding 3D Coordinates (2 hours)
**Folder:** `Lesson02-Coordinates/`

Master the WPF 3D coordinate system and visualization.

**Concepts:**
- Right-handed coordinate system
- X, Y, Z axes visualization
- Point3D and Vector3D
- Coordinate debugging tools

**Exercise:** Create a coordinate axis helper visualization

---

### Lesson 1.3 - Lights and Materials (2 hours)
**Folder:** `Lesson03-LightsMaterials/`

Explore different lighting types and material properties.

**Concepts:**
- DirectionalLight, PointLight, SpotLight
- AmbientLight
- DiffuseMaterial, SpecularMaterial
- Material groups
- Emissive materials

**Exercise:** Create a scene with multiple light sources

---

### Lesson 1.4 - Basic Transformations (2 hours)
**Folder:** `Lesson04-Transformations/`

Learn to transform 3D objects in space.

**Concepts:**
- TranslateTransform3D
- RotateTransform3D
- ScaleTransform3D
- Transform3DGroup
- Animation basics

**Exercise:** Animate a rotating and scaling cube

---

### Lesson 1.5 - Multiple Objects (2 hours)
**Folder:** `Lesson05-MultipleObjects/`

Work with multiple objects and organize complex scenes.

**Concepts:**
- Model3DGroup
- Organizing scene hierarchy
- Naming and referencing objects
- Performance considerations

**Exercise:** Create a simple 3D chess board with pieces

---

## How to Use This Module

1. **Start with Lesson 1.1** and work through each lesson sequentially
2. **Read the README** in each lesson folder for detailed explanations
3. **Build and run** each project to see the concepts in action
4. **Complete the exercises** to reinforce your learning
5. **Experiment** by modifying the code

## Building the Projects

Each lesson is a standalone WPF application:

```bash
cd Lesson01-FirstScene
dotnet build
dotnet run
```

Or open the `.sln` file in Visual Studio.

## Next Steps

After completing Module 1, proceed to **Module 2: Basic 3D Rendering** to learn advanced mesh manipulation and custom geometries.

---

**Time Investment:** 10 hours total
**Difficulty:** Beginner
**Prerequisites:** Basic C# and XAML
