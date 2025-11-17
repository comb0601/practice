# Lesson 2.1 - Mesh Geometries

**Duration:** 3 hours

## MeshGeometry3D Deep Dive

### Key Properties

1. **Positions** (Point3DCollection): Vertices of the mesh
2. **TriangleIndices** (Int32Collection): Define triangles using vertex indices
3. **Normals** (Vector3DCollection): Surface normals for lighting
4. **TextureCoordinates** (PointCollection): UV mapping for textures

### Example: Custom Triangle

```csharp
var mesh = new MeshGeometry3D();

// Define 3 vertices
mesh.Positions.Add(new Point3D(0, 1, 0));   // Top
mesh.Positions.Add(new Point3D(-1, -1, 0)); // Bottom left
mesh.Positions.Add(new Point3D(1, -1, 0));  // Bottom right

// Define triangle (indices into Positions)
mesh.TriangleIndices.Add(0);
mesh.TriangleIndices.Add(1);
mesh.TriangleIndices.Add(2);

// Calculate normals
mesh.Normals.Add(new Vector3D(0, 0, 1));
mesh.Normals.Add(new Vector3D(0, 0, 1));
mesh.Normals.Add(new Vector3D(0, 0, 1));
```

### Winding Order

Triangles have a **front face** and **back face** determined by winding order:
- **Counter-clockwise**: Front face (visible)
- **Clockwise**: Back face (culled)

### Calculating Normals

```csharp
public static Vector3D CalculateNormal(Point3D p1, Point3D p2, Point3D p3)
{
    Vector3D v1 = p2 - p1;
    Vector3D v2 = p3 - p1;
    Vector3D normal = Vector3D.CrossProduct(v1, v2);
    normal.Normalize();
    return normal;
}
```

## Project

Build an interactive mesh visualizer showing vertices and triangle connectivity.
