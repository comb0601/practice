# Professional 3D ROI Viewer - Technical Documentation

## Table of Contents
1. [Architecture Overview](#architecture-overview)
2. [Project Structure](#project-structure)
3. [Core Components](#core-components)
4. [Data Models](#data-models)
5. [Services](#services)
6. [Rendering Pipeline](#rendering-pipeline)
7. [File Format Specification](#file-format-specification)
8. [Extension Points](#extension-points)
9. [Performance Considerations](#performance-considerations)
10. [API Reference](#api-reference)

---

## Architecture Overview

### Design Pattern
The application follows a **Model-View-ViewModel (MVVM)-inspired** architecture adapted for WPF 3D:

```
┌─────────────────────────────────────────────┐
│            Presentation Layer               │
│  (MainWindow.xaml + MainWindow.xaml.cs)     │
│                                             │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐ │
│  │ Viewport │  │ ROI List │  │Properties│ │
│  │  3D View │  │ Manager  │  │  Panel   │ │
│  └──────────┘  └──────────┘  └──────────┘ │
└─────────────────────────────────────────────┘
                    ↕
┌─────────────────────────────────────────────┐
│            Business Logic Layer             │
│                                             │
│  ┌──────────────┐      ┌────────────────┐  │
│  │ ROI          │      │ MeshGenerator  │  │
│  │ Management   │      │ Service        │  │
│  └──────────────┘      └────────────────┘  │
└─────────────────────────────────────────────┘
                    ↕
┌─────────────────────────────────────────────┐
│               Data Layer                    │
│                                             │
│  ┌──────────────┐      ┌────────────────┐  │
│  │ ROI Model    │      │ Scene File     │  │
│  │ (ROI.cs)     │      │ (JSON)         │  │
│  └──────────────┘      └────────────────┘  │
└─────────────────────────────────────────────┘
```

### Technology Stack
- **Framework**: .NET 9.0
- **UI Framework**: WPF (Windows Presentation Foundation)
- **3D Graphics**: System.Windows.Media.Media3D
- **Data Binding**: INotifyPropertyChanged
- **Serialization**: System.Text.Json
- **Language**: C# 12

### Key Design Decisions

**Why WPF 3D instead of DirectX/OpenGL?**
- Native integration with WPF controls
- Data binding support for 3D properties
- Easier learning curve for .NET developers
- Hardware-accelerated through DirectX backend
- Good enough performance for ROI visualization (<1000 objects)

**Why Procedural Mesh Generation?**
- Full control over topology
- Dynamic LOD adjustment (future)
- No external mesh file dependencies
- Consistent vertex/normal generation
- Easy to add new shape types

**Why JSON for scene files?**
- Human-readable for debugging
- Easy to parse and generate
- Standard format with good tooling
- Version control friendly
- Extensible schema

---

## Project Structure

```
Professional-3D-ROI-Viewer/
├── README.md                    # Overview and features
├── USER_GUIDE.md                # End-user documentation
├── TECHNICAL_DOCUMENTATION.md   # This file
├── ROIViewer.csproj             # Project file (.NET 9.0)
│
├── App.xaml                     # Application resources
├── App.xaml.cs                  # Application entry point
│
├── MainWindow.xaml              # Main UI layout
├── MainWindow.xaml.cs           # Main window logic (~1200 lines)
│
├── Models/
│   └── ROI.cs                   # ROI data model with INotifyPropertyChanged
│
└── Services/
    └── MeshGenerator.cs         # Procedural mesh generation service
```

### File Responsibilities

**App.xaml / App.xaml.cs**
- Application-level resources (colors, styles)
- Dark theme definition
- Application startup logic
- Global exception handling (if implemented)

**MainWindow.xaml**
- Main window layout (3-panel design)
- Menu bar definition
- Toolbar buttons
- ROI list UI
- Properties panel controls
- 3D viewport container
- Status bar

**MainWindow.xaml.cs**
- Scene initialization (grid, axes, lights)
- ROI management (add, remove, duplicate, select)
- Camera control (orbit, pan, zoom)
- Property editing and data binding
- File I/O (save, load scenes)
- Event handlers for all UI controls
- Measurement and analysis tools

**Models/ROI.cs**
- ROI data structure
- Property change notifications
- Volume and surface area calculations
- Bounding box computation
- Overlap detection
- Clone method for duplication

**Services/MeshGenerator.cs**
- Static mesh generation functions
- Vertex and index generation algorithms
- Normal computation
- Texture coordinate generation (future)
- Wireframe extraction

---

## Core Components

### 1. ROI Model (`Models/ROI.cs`)

**Purpose**: Represents a single Region of Interest with all properties.

**Key Properties:**
```csharp
public class ROI : INotifyPropertyChanged
{
    // Identity
    public string Name { get; set; }
    public ROIShape Shape { get; set; }  // Enum: Box, Sphere, Cylinder, etc.

    // Transform
    public double PositionX/Y/Z { get; set; }    // Location in world space
    public double Width/Height/Depth { get; set; } // Size dimensions
    public double RotationX/Y/Z { get; set; }    // Euler angles (degrees)

    // Appearance
    public Color Color { get; set; }             // RGBA color
    public double Opacity { get; set; }          // 0.0 to 1.0
    public bool Visible { get; set; }            // Show/hide flag

    // Advanced
    public bool Locked { get; set; }             // Prevent editing
    public bool ShowWireframe { get; set; }      // Render as edges only
    public int Layer { get; set; }               // Organization layer

    // Computed (read-only)
    public double Volume { get; }                // Calculated based on shape
    public double SurfaceArea { get; }           // Calculated based on shape
    public Point3D Center { get; }               // Transformed center point
    public (double minX, ...) BoundingBox { get; } // AABB
}
```

**Key Methods:**
```csharp
// Create deep copy of ROI
public ROI Clone()

// Check if this ROI overlaps with another (AABB test)
public bool OverlapsWith(ROI other)

// Get axis-aligned bounding box (after transforms)
public (double minX, double maxX, ...) GetBoundingBox()

// Raise property change notification
protected void OnPropertyChanged(string propertyName)
```

**Volume Calculations:**
- **Box**: `Width × Height × Depth`
- **Sphere**: `(4/3) × π × r³` where `r = Width/2`
- **Cylinder**: `π × r² × Height` where `r = Width/2`
- **Cone**: `(1/3) × π × r² × Height`
- **Ellipsoid**: `(4/3) × π × (W/2) × (H/2) × (D/2)`
- **Torus**: `2π² × R × r²` where `R = Width/2`, `r = Depth/2`
- **Pyramid**: `(1/3) × Base² × Height` where `Base = Width`

**Surface Area Calculations:**
- **Box**: `2(WH + WD + HD)`
- **Sphere**: `4πr²`
- **Cylinder**: `2πr(h + r)`
- Others: Implemented with standard geometric formulas

### 2. Mesh Generator Service (`Services/MeshGenerator.cs`)

**Purpose**: Generate 3D meshes procedurally for each ROI shape.

**Public API:**
```csharp
public static class MeshGenerator
{
    // Generate mesh for any ROI based on its shape
    public static MeshGeometry3D CreateMeshForROI(ROI roi)

    // Individual shape generators
    public static MeshGeometry3D CreateBox(double width, height, depth)
    public static MeshGeometry3D CreateSphere(double radius, int divisions = 32)
    public static MeshGeometry3D CreateCylinder(double radius, height, int divisions = 32)
    public static MeshGeometry3D CreateCone(double radius, height, int divisions = 32)
    public static MeshGeometry3D CreateEllipsoid(double rx, ry, rz, int divisions = 32)
    public static MeshGeometry3D CreateTorus(double majorR, minorR, int divisions = 32)
    public static MeshGeometry3D CreatePyramid(double baseWidth, height)

    // Utility
    public static MeshGeometry3D CreateWireframe(MeshGeometry3D mesh)
}
```

**Mesh Generation Algorithms:**

**Box (Cube):**
```
8 vertices (corners)
12 triangles (2 per face × 6 faces)
Normals point outward from each face
```

**Sphere (UV Sphere):**
```
Vertices = (divisions + 1) × (divisions + 1)
Triangles = 2 × divisions × divisions

For each latitude θ from 0 to π:
  For each longitude φ from 0 to 2π:
    x = sin(θ) × cos(φ) × radius
    y = cos(θ) × radius
    z = sin(θ) × sin(φ) × radius

Normals = normalized vertex positions (for centered sphere)
```

**Cylinder:**
```
Top circle: divisions vertices
Bottom circle: divisions vertices
Sides: divisions × 2 triangles
Caps: divisions triangles each

For angle φ from 0 to 2π:
  Top:    x = cos(φ) × r, y = h/2,   z = sin(φ) × r
  Bottom: x = cos(φ) × r, y = -h/2,  z = sin(φ) × r
```

**Cone:**
```
Similar to cylinder but top radius = 0
Apex at (0, height/2, 0)
Base circle at y = -height/2
```

**Ellipsoid:**
```
Same as sphere but scale by (rx, ry, rz)
x = sin(θ) × cos(φ) × rx
y = cos(θ) × ry
z = sin(θ) × sin(φ) × rz
```

**Torus:**
```
For angle θ from 0 to 2π (around tube):
  For angle φ from 0 to 2π (around torus):
    x = (R + r×cos(θ)) × cos(φ)
    y = r × sin(θ)
    z = (R + r×cos(θ)) × sin(φ)

Where R = major radius, r = minor radius
```

**Pyramid:**
```
5 vertices: 4 base corners + 1 apex
6 triangles: 4 sides + 2 base
Apex at (0, height/2, 0)
Base centered at (0, -height/2, 0)
```

**Wireframe Extraction:**
```csharp
// Extract unique edges from triangles
Dictionary<Edge, bool> edges = new();
for (int i = 0; i < indices.Count; i += 3)
{
    AddEdge(indices[i], indices[i+1]);
    AddEdge(indices[i+1], indices[i+2]);
    AddEdge(indices[i+2], indices[i]);
}
// Create line list mesh from edges
```

### 3. Camera System

**Camera Type**: PerspectiveCamera
- Field of View: 60°
- Near Plane: 0.1 units
- Far Plane: 100 units

**Camera Control Model**: Spherical Orbit
```
Position calculated from:
  x = distance × cos(elevation) × cos(angle)
  y = distance × sin(elevation)
  z = distance × cos(elevation) × sin(angle)

Where:
  angle = azimuth angle (-∞ to +∞)
  elevation = pitch angle (-89° to +89°)
  distance = zoom level (2 to 50)

LookDirection = -Position (look at origin)
UpDirection = (0, 1, 0) always
```

**Mouse Control Mapping:**
- **Left Drag**:
  - Δx → Δangle (0.5° per pixel)
  - Δy → Δelevation (-0.5° per pixel)
- **Right Drag**:
  - Δx → Pan X (0.01 units per pixel)
  - Δy → Pan Y (0.01 units per pixel)
- **Wheel**:
  - Scroll → Zoom (1.2× per tick)

### 4. Lighting System

**Default Lighting Setup:**
```xml
<Model3DGroup>
  <!-- Ambient: base illumination -->
  <AmbientLight Color="#404040" />

  <!-- Directional: main light source -->
  <DirectionalLight Color="#FFFFFF" Direction="-1,-1,-1" />
</Model3DGroup>
```

**Material System:**
```csharp
// Standard material for ROIs
var material = new MaterialGroup();
material.Children.Add(new DiffuseMaterial(brush));     // Base color
material.Children.Add(new SpecularMaterial(whiteBrush, 50)); // Shininess
```

### 5. Transform System

**Transform Hierarchy:**
```
ROI Model → GeometryModel3D
  ↓
Transform3DGroup
  ├─ TranslateTransform3D (Position)
  ├─ RotateTransform3D X (RotationX)
  ├─ RotateTransform3D Y (RotationY)
  ├─ RotateTransform3D Z (RotationZ)
  └─ ScaleTransform3D (Width, Height, Depth)
```

**Application Order**: Scale → Rotate Z → Rotate Y → Rotate X → Translate

**Why This Order?**
- Scale first: Size in local space
- Rotate: Orient in local space
- Translate last: Move to world position

---

## Data Models

### ROI Data Model

**Class Diagram:**
```
┌─────────────────────────────────────┐
│          ROI                        │
├─────────────────────────────────────┤
│ - name: string                      │
│ - shape: ROIShape                   │
│ - positionX/Y/Z: double             │
│ - width/height/depth: double        │
│ - rotationX/Y/Z: double             │
│ - color: Color                      │
│ - opacity: double                   │
│ - visible: bool                     │
│ - locked: bool                      │
│ - showWireframe: bool               │
│ - layer: int                        │
├─────────────────────────────────────┤
│ + Volume: double { get; }           │
│ + SurfaceArea: double { get; }      │
│ + Center: Point3D { get; }          │
│ + BoundingBox: tuple { get; }       │
├─────────────────────────────────────┤
│ + Clone(): ROI                      │
│ + OverlapsWith(ROI): bool           │
│ + GetBoundingBox(): tuple           │
│ # OnPropertyChanged(string): void   │
└─────────────────────────────────────┘
         ↑
         │ implements
┌─────────────────────────┐
│ INotifyPropertyChanged  │
└─────────────────────────┘
```

**ROIShape Enum:**
```csharp
public enum ROIShape
{
    Box = 0,
    Sphere = 1,
    Cylinder = 2,
    Cone = 3,
    Ellipsoid = 4,
    Torus = 5,
    Pyramid = 6
}
```

### Scene Data Model

**JSON Schema:**
```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "type": "object",
  "properties": {
    "Version": {
      "type": "string",
      "description": "File format version"
    },
    "ROIs": {
      "type": "array",
      "items": {
        "type": "object",
        "properties": {
          "Name": { "type": "string" },
          "Shape": { "type": "string", "enum": ["Box", "Sphere", ...] },
          "PositionX": { "type": "number" },
          "PositionY": { "type": "number" },
          "PositionZ": { "type": "number" },
          "Width": { "type": "number", "minimum": 0.1 },
          "Height": { "type": "number", "minimum": 0.1 },
          "Depth": { "type": "number", "minimum": 0.1 },
          "RotationX": { "type": "number" },
          "RotationY": { "type": "number" },
          "RotationZ": { "type": "number" },
          "Color": { "type": "string", "pattern": "^#[0-9A-F]{8}$" },
          "Opacity": { "type": "number", "minimum": 0, "maximum": 1 },
          "Visible": { "type": "boolean" },
          "Locked": { "type": "boolean" },
          "ShowWireframe": { "type": "boolean" },
          "Layer": { "type": "integer" }
        },
        "required": ["Name", "Shape", "PositionX", "PositionY", "PositionZ"]
      }
    }
  },
  "required": ["Version", "ROIs"]
}
```

---

## Services

### MeshGenerator Service

**Design**: Static utility class (no state, pure functions)

**Responsibilities:**
1. Generate vertices for each shape type
2. Generate triangle indices for proper winding
3. Compute vertex normals for lighting
4. Extract wireframe edges from solid meshes
5. Provide consistent mesh quality (subdivision count)

**Performance Characteristics:**
- Box: O(1) - 8 vertices, 12 triangles
- Sphere: O(n²) - n² vertices where n = divisions
- Cylinder: O(n) - 2n vertices
- Cone: O(n) - n+1 vertices
- Ellipsoid: O(n²) - same as sphere
- Torus: O(n²) - n² vertices
- Pyramid: O(1) - 5 vertices, 6 triangles

**Memory Usage:**
- Each vertex: 24 bytes (Point3D) + 24 bytes (Vector3D normal) = 48 bytes
- Each triangle: 12 bytes (3 × Int32)
- Sphere (32 divisions): ~50 KB per mesh

**Quality Settings:**
```csharp
// Low quality (fast)
int divisions = 16;  // 256 vertices for sphere

// Medium quality (default)
int divisions = 32;  // 1024 vertices for sphere

// High quality (smooth)
int divisions = 64;  // 4096 vertices for sphere
```

---

## Rendering Pipeline

### Frame Update Sequence

```
1. User Input Event (mouse, keyboard, slider)
   ↓
2. Update Data Model (ROI properties)
   ↓
3. Property Changed Notification (INotifyPropertyChanged)
   ↓
4. Update Visual Representation
   a. Generate new mesh (if shape/size changed)
   b. Update transforms (if position/rotation changed)
   c. Update materials (if color/opacity changed)
   ↓
5. WPF Layout Pass
   ↓
6. DirectX Render (hardware accelerated)
   ↓
7. Display on Screen
```

### 3D Scene Graph

```
Viewport3D
 └─ ModelVisual3D (Scene Root)
     ├─ Model3DGroup (Lighting)
     │   ├─ AmbientLight
     │   └─ DirectionalLight
     ├─ GeometryModel3D (Grid)
     ├─ GeometryModel3D (X Axis - Red)
     ├─ GeometryModel3D (Y Axis - Green)
     ├─ GeometryModel3D (Z Axis - Blue)
     ├─ GeometryModel3D (ROI 1)
     │   ├─ MeshGeometry3D (vertices, indices, normals)
     │   ├─ Material (color, opacity)
     │   └─ Transform3DGroup (scale, rotate, translate)
     ├─ GeometryModel3D (ROI 2)
     └─ ... (more ROIs)
```

### Rendering Optimizations

**Implemented:**
- Cull back-facing triangles (default WPF behavior)
- Reuse mesh objects when shape doesn't change
- Only update changed properties (via INotifyPropertyChanged)
- Hardware acceleration through DirectX

**Future Optimizations:**
- Instancing for duplicate shapes
- Frustum culling for off-screen ROIs
- Level-of-detail (LOD) based on distance
- Occlusion culling for hidden ROIs
- Deferred rendering for transparency sorting

---

## File Format Specification

### .roiscene File Format

**Version**: 1.0
**Encoding**: UTF-8
**Format**: JSON

**Example File:**
```json
{
  "Version": "1.0",
  "ROIs": [
    {
      "Name": "Tumor Primary",
      "Shape": "Sphere",
      "PositionX": 2.5,
      "PositionY": 1.0,
      "PositionZ": -3.2,
      "Width": 3.0,
      "Height": 3.0,
      "Depth": 3.0,
      "RotationX": 0.0,
      "RotationY": 0.0,
      "RotationZ": 0.0,
      "Color": "#FFFF0000",
      "Opacity": 0.7,
      "Visible": true,
      "Locked": false,
      "ShowWireframe": false,
      "Layer": 0
    },
    {
      "Name": "Bounding Volume",
      "Shape": "Box",
      "PositionX": 0.0,
      "PositionY": 0.0,
      "PositionZ": 0.0,
      "Width": 10.0,
      "Height": 10.0,
      "Depth": 10.0,
      "RotationX": 0.0,
      "RotationY": 45.0,
      "RotationZ": 0.0,
      "Color": "#8000FF00",
      "Opacity": 0.3,
      "Visible": true,
      "Locked": true,
      "ShowWireframe": true,
      "Layer": 1
    }
  ]
}
```

**Serialization Code:**
```csharp
// Save
var json = JsonSerializer.Serialize(roiList, new JsonSerializerOptions
{
    WriteIndented = true
});
File.WriteAllText(filePath, json);

// Load
var json = File.ReadAllText(filePath);
var roiList = JsonSerializer.Deserialize<List<ROI>>(json);
```

**Versioning Strategy:**
- Current version: 1.0
- Future versions add optional fields (backward compatible)
- Major version bump for breaking changes
- Version check on load to handle migration

---

## Extension Points

### Adding New ROI Shapes

**Step 1**: Add to enum
```csharp
public enum ROIShape
{
    // ... existing shapes ...
    CustomShape = 7
}
```

**Step 2**: Add mesh generator
```csharp
public static MeshGeometry3D CreateCustomShape(double param1, double param2)
{
    var mesh = new MeshGeometry3D();
    // Generate vertices
    // Generate indices
    // Compute normals
    return mesh;
}
```

**Step 3**: Update dispatcher
```csharp
public static MeshGeometry3D CreateMeshForROI(ROI roi)
{
    return roi.Shape switch
    {
        // ... existing cases ...
        ROIShape.CustomShape => CreateCustomShape(roi.Width, roi.Height),
        _ => CreateBox(1, 1, 1)
    };
}
```

**Step 4**: Add volume calculation
```csharp
public double Volume => Shape switch
{
    // ... existing cases ...
    ROIShape.CustomShape => CalculateCustomVolume(),
    _ => 0
};
```

**Step 5**: Add menu item
```xml
<MenuItem Header="Add Custom Shape" Click="AddCustomShape_Click" />
```

### Adding Custom Properties

**Step 1**: Add property to ROI class
```csharp
private string _customProperty;
public string CustomProperty
{
    get => _customProperty;
    set { _customProperty = value; OnPropertyChanged(nameof(CustomProperty)); }
}
```

**Step 2**: Add UI control
```xml
<TextBox Text="{Binding SelectedROI.CustomProperty, UpdateSourceTrigger=PropertyChanged}" />
```

**Step 3**: Include in serialization (automatic with JsonSerializer)

### Plugin System (Future)

**Interface Design:**
```csharp
public interface IROIPlugin
{
    string Name { get; }
    string Description { get; }
    void Initialize(IROIViewerAPI api);
    void OnROICreated(ROI roi);
    void OnROIModified(ROI roi);
    void OnSceneLoaded(List<ROI> rois);
}

public interface IROIViewerAPI
{
    void AddROI(ROI roi);
    void RemoveROI(ROI roi);
    List<ROI> GetAllROIs();
    void RegisterMenuItem(string menuPath, Action callback);
    void ShowMessage(string message);
}
```

**Plugin Loading:**
```csharp
// Scan Plugins folder
var pluginDlls = Directory.GetFiles("Plugins", "*.dll");
foreach (var dll in pluginDlls)
{
    var assembly = Assembly.LoadFrom(dll);
    var pluginTypes = assembly.GetTypes()
        .Where(t => typeof(IROIPlugin).IsAssignableFrom(t));
    foreach (var type in pluginTypes)
    {
        var plugin = (IROIPlugin)Activator.CreateInstance(type);
        plugin.Initialize(apiImpl);
        _loadedPlugins.Add(plugin);
    }
}
```

---

## Performance Considerations

### Benchmarks (Estimated)

**Hardware**: Intel i5, 16GB RAM, GTX 1650

| Operation | ROI Count | Time (ms) | FPS |
|-----------|-----------|-----------|-----|
| Scene render | 10 | 16 | 60 |
| Scene render | 50 | 33 | 30 |
| Scene render | 100 | 66 | 15 |
| Scene render | 500 | 200 | 5 |
| Add ROI | 1 | 50 | - |
| Delete ROI | 1 | 10 | - |
| Update property | 1 | 16 | - |
| Save scene | 100 | 200 | - |
| Load scene | 100 | 300 | - |

### Bottlenecks

**Mesh Generation:**
- High-subdivision spheres (64+ divisions) slow
- Generated synchronously on UI thread
- **Solution**: Cache meshes, generate asynchronously

**Rendering:**
- Too many ROIs (>200) causes frame drops
- Transparent ROIs require sorted rendering (slow)
- **Solution**: LOD system, frustum culling, instancing

**Property Updates:**
- Slider updates trigger frequent redraws
- **Solution**: Throttle updates (16ms minimum)

**File I/O:**
- Large scenes (>1000 ROIs) slow to save/load
- **Solution**: Background thread, progress bar

### Memory Usage

**Per ROI:**
- Data model: ~200 bytes
- Mesh (32 divs): ~50 KB
- Visual: ~500 bytes
- **Total**: ~50 KB per ROI

**1000 ROIs**: ~50 MB

### Optimization Strategies

**Immediate:**
1. Reduce default divisions from 32 to 24
2. Implement mesh caching by shape+size
3. Throttle property update events
4. Use ObservableCollection for automatic UI updates

**Short-term:**
5. Implement frustum culling
6. Add LOD system (far = low poly, near = high poly)
7. Batch geometry updates
8. Use GPU instancing for duplicate shapes

**Long-term:**
9. Custom render loop (bypass WPF overhead)
10. Compute shaders for procedural generation
11. Deferred rendering pipeline
12. Streaming for large scenes

---

## API Reference

### ROI Class

```csharp
public class ROI : INotifyPropertyChanged
{
    // Constructor
    public ROI();

    // Properties
    public string Name { get; set; }
    public ROIShape Shape { get; set; }
    public double PositionX { get; set; }
    public double PositionY { get; set; }
    public double PositionZ { get; set; }
    public double Width { get; set; }
    public double Height { get; set; }
    public double Depth { get; set; }
    public double RotationX { get; set; }
    public double RotationY { get; set; }
    public double RotationZ { get; set; }
    public Color Color { get; set; }
    public double Opacity { get; set; }
    public bool Visible { get; set; }
    public bool Locked { get; set; }
    public bool ShowWireframe { get; set; }
    public int Layer { get; set; }

    // Computed Properties
    public double Volume { get; }
    public double SurfaceArea { get; }
    public Point3D Center { get; }

    // Methods
    public ROI Clone();
    public bool OverlapsWith(ROI other);
    public (double minX, double maxX, double minY,
            double maxY, double minZ, double maxZ) GetBoundingBox();

    // Events
    public event PropertyChangedEventHandler PropertyChanged;
    protected void OnPropertyChanged(string propertyName);
}
```

### MeshGenerator Class

```csharp
public static class MeshGenerator
{
    // Main API
    public static MeshGeometry3D CreateMeshForROI(ROI roi);

    // Shape Generators
    public static MeshGeometry3D CreateBox(
        double width, double height, double depth);

    public static MeshGeometry3D CreateSphere(
        double radius, int divisions = 32);

    public static MeshGeometry3D CreateCylinder(
        double radius, double height, int divisions = 32);

    public static MeshGeometry3D CreateCone(
        double radius, double height, int divisions = 32);

    public static MeshGeometry3D CreateEllipsoid(
        double radiusX, double radiusY, double radiusZ, int divisions = 32);

    public static MeshGeometry3D CreateTorus(
        double majorRadius, double minorRadius, int divisions = 32);

    public static MeshGeometry3D CreatePyramid(
        double baseWidth, double height);

    // Utilities
    public static MeshGeometry3D CreateWireframe(MeshGeometry3D solidMesh);
}
```

### MainWindow Methods (Key Public/Internal Methods)

```csharp
public partial class MainWindow : Window
{
    // Initialization
    private void InitializeScene();
    private void CreateGrid();
    private void CreateAxes();

    // ROI Management
    private void AddROI(ROI roi);
    private void RemoveSelectedROI();
    private void DuplicateSelectedROI();
    private void UpdateAllROIs();
    private GeometryModel3D CreateROIModel(ROI roi);

    // Camera Control
    private void UpdateCamera();
    private void SetPresetView(double x, double y, double z);

    // File Operations
    private void SaveSceneToFile(string filePath);
    private void LoadSceneFromFile(string filePath);

    // Property Editing
    private void LoadROIProperties(ROI roi);
    private void Property_Changed(object sender, EventArgs e);

    // Measurements
    private double CalculateDistance(ROI roi1, ROI roi2);
    private List<(ROI, ROI)> FindOverlaps();

    // Event Handlers
    private void Viewport_MouseMove(object sender, MouseEventArgs e);
    private void Viewport_MouseWheel(object sender, MouseWheelEventArgs e);
    private void ROIList_SelectionChanged(object sender, SelectionChangedEventArgs e);
    // ... (many more event handlers)
}
```

---

## Appendix A: Mathematical Formulas

### Coordinate Systems

**World Space**: Right-handed coordinate system
- X: Right
- Y: Up
- Z: Forward (toward viewer)

**Spherical to Cartesian Conversion:**
```
x = r × sin(θ) × cos(φ)
y = r × cos(θ)
z = r × sin(θ) × sin(φ)

Where:
  r = radius
  θ = polar angle (0 to π)
  φ = azimuthal angle (0 to 2π)
```

**Euler Angles to Rotation Matrix:**
```
Given angles: α (X), β (Y), γ (Z) in degrees

Rx(α) = [1    0         0      ]
        [0  cos(α)  -sin(α)]
        [0  sin(α)   cos(α)]

Ry(β) = [ cos(β)  0  sin(β)]
        [   0     1    0   ]
        [-sin(β)  0  cos(β)]

Rz(γ) = [cos(γ) -sin(γ)  0]
        [sin(γ)  cos(γ)  0]
        [  0       0     1]

Combined: R = Rz(γ) × Ry(β) × Rx(α)
```

### Bounding Box Computation

**Axis-Aligned Bounding Box (AABB):**
```
1. Get all 8 corners of rotated box
2. Transform each corner by rotation + translation
3. Find min/max of all transformed points

Corners (before transform):
  c1 = (-w/2, -h/2, -d/2)
  c2 = (+w/2, -h/2, -d/2)
  c3 = (-w/2, +h/2, -d/2)
  c4 = (+w/2, +h/2, -d/2)
  c5 = (-w/2, -h/2, +d/2)
  c6 = (+w/2, -h/2, +d/2)
  c7 = (-w/2, +h/2, +d/2)
  c8 = (+w/2, +h/2, +d/2)

Transform: c'i = R × ci + position

AABB:
  minX = min(c'1.x, c'2.x, ..., c'8.x)
  maxX = max(c'1.x, c'2.x, ..., c'8.x)
  (same for Y and Z)
```

### Overlap Detection

**AABB Intersection Test:**
```
bool Overlap(AABB a, AABB b)
{
    return (a.minX <= b.maxX && a.maxX >= b.minX) &&
           (a.minY <= b.maxY && a.maxY >= b.minY) &&
           (a.minZ <= b.maxZ && a.maxZ >= b.minZ);
}
```

---

## Appendix B: Code Examples

### Example 1: Creating and Adding a Custom ROI

```csharp
// Create new ROI
var roi = new ROI
{
    Name = "Custom Region",
    Shape = ROIShape.Sphere,
    PositionX = 5.0,
    PositionY = 2.0,
    PositionZ = -3.0,
    Width = 2.5,
    Height = 2.5,
    Depth = 2.5,
    Color = Colors.Blue,
    Opacity = 0.6,
    Visible = true
};

// Add to scene
AddROI(roi);

// Subscribe to property changes
roi.PropertyChanged += (s, e) =>
{
    Debug.WriteLine($"Property {e.PropertyName} changed");
    UpdateAllROIs();
};
```

### Example 2: Programmatic Camera Control

```csharp
// Set camera to specific position
cameraAngle = 45.0;
cameraElevation = 30.0;
cameraDistance = 15.0;
UpdateCamera();

// Animate camera orbit
var timer = new DispatcherTimer { Interval = TimeSpan.FromMilliseconds(16) };
timer.Tick += (s, e) =>
{
    cameraAngle += 1.0;
    if (cameraAngle > 360) cameraAngle -= 360;
    UpdateCamera();
};
timer.Start();
```

### Example 3: Batch ROI Operations

```csharp
// Create grid of ROIs
for (int x = -5; x <= 5; x += 2)
{
    for (int z = -5; z <= 5; z += 2)
    {
        var roi = new ROI
        {
            Name = $"Grid_{x}_{z}",
            Shape = ROIShape.Box,
            PositionX = x,
            PositionY = 0,
            PositionZ = z,
            Width = 1.0,
            Height = 0.5,
            Depth = 1.0,
            Color = Color.FromRgb(
                (byte)((x + 5) * 25),
                (byte)((z + 5) * 25),
                100),
            Opacity = 0.8
        };
        AddROI(roi);
    }
}
```

### Example 4: Finding All Overlaps

```csharp
var overlaps = new List<(ROI, ROI)>();
for (int i = 0; i < roiList.Count; i++)
{
    for (int j = i + 1; j < roiList.Count; j++)
    {
        if (roiList[i].OverlapsWith(roiList[j]))
        {
            overlaps.Add((roiList[i], roiList[j]));
        }
    }
}

// Display results
MessageBox.Show($"Found {overlaps.Count} overlapping pairs");
```

---

## Appendix C: Build and Deployment

### Building from Source

```bash
# Prerequisites
# - .NET 9.0 SDK
# - Windows 10/11

# Clone repository
git clone https://github.com/yourusername/roi-viewer.git
cd roi-viewer/Professional-3D-ROI-Viewer

# Restore dependencies
dotnet restore

# Build
dotnet build --configuration Release

# Run
dotnet run --configuration Release
```

### Creating Installer

```bash
# Using dotnet publish
dotnet publish --configuration Release --runtime win-x64 --self-contained

# Output in: bin/Release/net9.0-windows/win-x64/publish/

# Create installer with Inno Setup, WiX, or NSIS
```

### Unit Testing

```csharp
[TestClass]
public class ROITests
{
    [TestMethod]
    public void TestBoxVolume()
    {
        var roi = new ROI
        {
            Shape = ROIShape.Box,
            Width = 2,
            Height = 3,
            Depth = 4
        };
        Assert.AreEqual(24.0, roi.Volume, 0.001);
    }

    [TestMethod]
    public void TestOverlapDetection()
    {
        var roi1 = new ROI { PositionX = 0, Width = 2, Height = 2, Depth = 2 };
        var roi2 = new ROI { PositionX = 1, Width = 2, Height = 2, Depth = 2 };
        Assert.IsTrue(roi1.OverlapsWith(roi2));
    }
}
```

---

**Document Version**: 1.0
**Last Updated**: 2025
**Target Framework**: .NET 9.0
**Minimum WPF Version**: 4.8
