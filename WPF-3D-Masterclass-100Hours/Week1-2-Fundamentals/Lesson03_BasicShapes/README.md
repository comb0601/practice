# Lesson 03: Basic Shapes - Mesh Generation

**Duration:** 2 hours | **Difficulty:** ⭐⭐ | **Status:** ✅ COMPLETE WITH CODE

---

## 🎯 What You'll Build

A **Shape Generator Application** that creates:
- ✅ Cubes (6 faces, 12 triangles)
- ✅ Spheres (smooth, procedurally generated)
- ✅ Cylinders (with caps)
- ✅ Cones (pointed geometry)
- ✅ Torus (donut shape!)

**All generated from scratch using math!**

---

## 📚 Core Concepts

### What is a Mesh?

A **mesh** is a collection of:
1. **Vertices** (Point3D) - Corner points
2. **Triangle Indices** - Which vertices form triangles
3. **Normals** (optional) - Surface direction for lighting

### Creating a Simple Triangle

```csharp
var mesh = new MeshGeometry3D();

// Add 3 vertices
mesh.Positions.Add(new Point3D(0, 1, 0));   // Top
mesh.Positions.Add(new Point3D(-1, 0, 0));  // Left
mesh.Positions.Add(new Point3D(1, 0, 0));   // Right

// Connect them (counter-clockwise)
mesh.TriangleIndices.Add(0);
mesh.TriangleIndices.Add(1);
mesh.TriangleIndices.Add(2);
```

### Sphere Generation Formula

```csharp
for (int lat = 0; lat <= divisions; lat++) {
    double theta = lat * Math.PI / divisions;
    for (int lon = 0; lon <= divisions; lon++) {
        double phi = lon * 2 * Math.PI / divisions;

        double x = Math.Sin(theta) * Math.Cos(phi);
        double y = Math.Cos(theta);
        double z = Math.Sin(theta) * Math.Sin(phi);

        mesh.Positions.Add(new Point3D(x * radius, y * radius, z * radius));
    }
}
```

---

## 🎮 Running the Application

```bash
cd Solution
dotnet run
```

**Features:**
- Dropdown to select shape type
- Sliders for divisions/segments
- Real-time shape generation
- Vertex/triangle count display
- Wireframe toggle

---

## 💡 Key Learnings

✅ Procedural mesh generation
✅ Mathematical shape creation
✅ Vertex indexing
✅ Triangle topology
✅ Reusable shape functions

---

## ✅ Next: Lesson 04 - Lighting Systems
