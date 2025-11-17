# WPF 3D ROI Examples - Complete Learning Package

This package contains **three complete WPF applications** demonstrating 3D Region of Interest (ROI) rendering at different complexity levels. Perfect for beginners who want to learn by example!

## 📦 What's Included

### 1. Easy Version - Basic 3D ROI
**Location:** `1_Easy_ROI_3D/`

A simple application to get you started with 3D graphics in WPF.

**Features:**
- ✅ Single 3D box ROI
- ✅ Position controls (X, Y, Z sliders)
- ✅ Size controls (Width, Height, Depth)
- ✅ Camera rotation (orbit around scene)
- ✅ Color selection
- ✅ 3D grid and axis helpers
- ✅ Semi-transparent rendering
- ✅ Wireframe edges

**Perfect for:** Complete beginners learning WPF 3D basics

---

### 2. Middle Version - Multiple ROIs with Shapes
**Location:** `2_Middle_ROI_3D/`

Intermediate application with multiple ROI support and different shapes.

**Features:**
- ✅ Multiple ROIs in the same scene
- ✅ Three shape types: **Box, Sphere, Cylinder**
- ✅ Add/Remove ROIs dynamically
- ✅ Individual ROI selection and editing
- ✅ Full rotation control (X, Y, Z axes)
- ✅ Opacity/transparency control
- ✅ Color customization per ROI
- ✅ Advanced camera controls
- ✅ ROI list management

**Perfect for:** Developers ready to handle multiple objects and interactions

---

### 3. Hard Version - Professional ROI Editor
**Location:** `3_Hard_ROI_3D/`

Advanced, production-ready 3D ROI editor with professional features.

**Features:**
- ✅ Five shape types: **Box, Sphere, Cylinder, Cone, Torus**
- ✅ Full ROI management (Add, Remove, Duplicate, Reorder)
- ✅ Mouse-controlled camera (Drag to rotate, Scroll to zoom)
- ✅ Save/Load scenes to JSON files
- ✅ Visibility toggles per ROI
- ✅ Wireframe mode
- ✅ Clipping plane visualization
- ✅ Volume calculations
- ✅ Distance measurements
- ✅ Statistics panel
- ✅ Professional UI with menus and status bar
- ✅ Color-coded axis visualization
- ✅ ROI list with visual indicators

**Perfect for:** Advanced developers building real applications

---

## 🚀 How to Run

### Prerequisites
- **Windows** operating system
- **.NET 6.0 SDK** or later ([Download here](https://dotnet.microsoft.com/download))
- Any IDE (Visual Studio, VS Code, Rider) or just command line

### Running Each Application

#### Option 1: Using Command Line (Easiest)

```bash
# Navigate to any version folder
cd 1_Easy_ROI_3D

# Run the application
dotnet run
```

#### Option 2: Build and Run Separately

```bash
# Build the project
dotnet build

# Run the executable
dotnet run
```

#### Option 3: Using Visual Studio

1. Open the `.csproj` file in Visual Studio
2. Press **F5** or click **Start**

---

## 📚 Learning Path

### For Absolute Beginners

**Start here:** `1_Easy_ROI_3D/`

1. **Run the application** and play with the sliders
2. **Open `MainWindow.xaml`** - See how the UI is built
3. **Open `MainWindow.xaml.cs`** - Understand the 3D logic
4. **Key concepts to learn:**
   - `Viewport3D` - The 3D view container
   - `PerspectiveCamera` - How camera works
   - `MeshGeometry3D` - Creating 3D shapes
   - `Transform3D` - Moving and positioning objects
   - `Material` - Colors and appearance

### For Intermediate Learners

**Move to:** `2_Middle_ROI_3D/`

1. **Study `ROI.cs`** - Object-oriented approach to ROI data
2. **Compare with Easy version** - See how complexity increased
3. **Key new concepts:**
   - Managing lists of 3D objects
   - Creating different mesh types (sphere, cylinder)
   - UI data binding to object properties
   - Transform groups for rotation

### For Advanced Developers

**Jump to:** `3_Hard_ROI_3D/`

1. **Explore the full architecture**
2. **Study the menu system and file I/O**
3. **Key advanced concepts:**
   - Mouse-based camera control
   - JSON serialization for save/load
   - Complex mesh generation (torus, cone)
   - Clipping planes
   - Professional UI patterns

---

## 🎮 Controls Guide

### Easy Version
- **Position Sliders:** Move the ROI in 3D space
- **Size Sliders:** Change ROI dimensions
- **Camera Angle:** Rotate around the scene
- **Camera Height:** Change viewing elevation
- **Color Combo:** Change ROI color
- **Reset Button:** Reset all values

### Middle Version
- **+ Add ROI:** Create new ROI
- **- Remove ROI:** Delete selected ROI
- **ROI List:** Click to select and edit
- **Shape Combo:** Change between Box/Sphere/Cylinder
- **Rotation Sliders:** Rotate ROI on each axis
- **Opacity Slider:** Control transparency

### Hard Version
- **Mouse Drag:** Rotate camera view (Left click + drag)
- **Mouse Wheel:** Zoom in/out
- **File Menu:**
  - New Scene: Clear all ROIs
  - Save Scene: Export to JSON
  - Load Scene: Import from JSON
- **View Menu:**
  - Toggle grid, axes, wireframe
- **Tools Menu:**
  - Measure distance between ROIs
  - Show statistics
- **ROI Manager:**
  - +/- buttons for add/remove
  - ⎘ Duplicate current ROI
  - ↑↓ Reorder ROIs
  - Checkboxes for visibility
- **Properties Panel:**
  - Full control over selected ROI
  - Clipping plane controls

---

## 🔍 Code Structure Explained

### Easy Version Files
```
1_Easy_ROI_3D/
├── Easy_ROI_3D.csproj    # Project configuration
├── App.xaml              # Application definition
├── App.xaml.cs           # Application code
├── MainWindow.xaml       # UI layout (XAML)
└── MainWindow.xaml.cs    # UI logic (C#)
```

### Middle & Hard Version Files
```
2_Middle_ROI_3D/
├── Middle_ROI_3D.csproj  # Project configuration
├── App.xaml              # Application definition
├── App.xaml.cs           # Application code
├── ROI.cs                # ROI data class
├── MainWindow.xaml       # UI layout
└── MainWindow.xaml.cs    # UI logic + 3D rendering
```

---

## 💡 Key Concepts Demonstrated

### 3D Rendering Basics
- **Viewport3D:** Container for 3D content
- **Camera:** Defines viewpoint (position, direction, field of view)
- **Lights:** Ambient, directional lighting for realism
- **Meshes:** Vertices and triangles forming 3D shapes
- **Materials:** Colors, transparency, specularity

### ROI-Specific Logic
- **Position:** X, Y, Z coordinates in 3D space
- **Size:** Width, Height, Depth dimensions
- **Rotation:** Euler angles for orientation
- **Transparency:** Alpha channel for see-through effect
- **Wireframe:** Edge visualization

### WPF Patterns
- **XAML UI:** Declarative interface design
- **Event Handlers:** Responding to user input
- **Data Binding:** Connecting UI to data
- **Transforms:** Moving, rotating, scaling objects

---

## 🎯 Common Use Cases

### Medical Imaging
Use these techniques to visualize:
- CT/MRI scan regions
- Tumor boundaries
- Organ segmentation
- Surgical planning

### Computer Vision
- Object detection bounding boxes
- 3D point cloud regions
- Depth map visualization
- Camera frustum visualization

### Engineering
- Part inspection zones
- Measurement regions
- Assembly collision detection
- Spatial analysis

### Gaming
- Trigger volumes
- Spawn regions
- Camera bounds
- Level design tools

---

## 🔧 Customization Ideas

### Easy Modifications
1. **Add more colors** - Edit the color combo items
2. **Change default size** - Modify initial values
3. **Adjust camera limits** - Change min/max in sliders
4. **Different grid size** - Modify grid creation loop

### Medium Difficulty
1. **Add new shapes** - Implement pyramid, ellipsoid
2. **Keyboard shortcuts** - Add KeyDown event handlers
3. **Multiple camera presets** - Save/load view positions
4. **ROI naming** - Let users rename ROIs

### Advanced Modifications
1. **Import 3D models** - Load OBJ/STL files
2. **Intersection detection** - Check if ROIs overlap
3. **Animation** - Animate ROI movements
4. **Export to other formats** - Write to XML, binary
5. **Undo/Redo system** - Command pattern implementation

---

## 📖 Learning Resources

### WPF 3D Documentation
- [Microsoft 3D Graphics Overview](https://docs.microsoft.com/en-us/dotnet/desktop/wpf/graphics-multimedia/3-d-graphics-overview)
- [Viewport3D Class](https://docs.microsoft.com/en-us/dotnet/api/system.windows.controls.viewport3d)
- [MeshGeometry3D](https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.media3d.meshgeometry3d)

### 3D Math Concepts
- Vector mathematics
- Transformation matrices
- Euler angles vs quaternions
- Mesh topology (vertices, faces, normals)

---

## ⚠️ Troubleshooting

### Application won't start
```bash
# Check if .NET 6 is installed
dotnet --version

# If not installed, download from:
# https://dotnet.microsoft.com/download/dotnet/6.0
```

### Build errors
```bash
# Clean and rebuild
dotnet clean
dotnet build
```

### 3D scene is black
- Check if camera is positioned correctly
- Verify lights are enabled
- Ensure materials have color/opacity

### Performance issues
- Reduce mesh division count in sphere/cylinder creation
- Limit number of ROIs in scene
- Disable wireframe on large objects

---

## 🎓 Next Steps

After mastering these examples:

1. **Combine features** - Take favorite parts from each version
2. **Add real data** - Load ROI coordinates from files
3. **Build tools** - Create measurement, analysis features
4. **Optimize** - Learn about 3D performance optimization
5. **Advanced rendering** - Explore shaders, textures, lighting models

---

## 💬 Tips for Learning

1. **Start small** - Run Easy version first, understand every line
2. **Experiment** - Change values, break things, fix them
3. **Compare versions** - See how same features evolve
4. **Read comments** - Code is documented for learning
5. **Build your own** - Start a new project applying these concepts

---

## 📝 Version Comparison Table

| Feature | Easy | Middle | Hard |
|---------|------|--------|------|
| Multiple ROIs | ❌ | ✅ | ✅ |
| Shape Types | Box only | 3 types | 5 types |
| Rotation Control | ❌ | ✅ | ✅ |
| Mouse Camera | ❌ | ❌ | ✅ |
| Save/Load | ❌ | ❌ | ✅ |
| Measurements | ❌ | ❌ | ✅ |
| Clipping Plane | ❌ | ❌ | ✅ |
| Wireframe Toggle | Basic | Per-ROI | Global + Per-ROI |
| UI Complexity | Simple | Medium | Professional |
| Code Lines | ~350 | ~600 | ~1000+ |
| Best For | Learning | Practice | Production |

---

## 🏆 Challenge Yourself

Try implementing these features:

### Beginner Challenges
- [ ] Add a reset camera button
- [ ] Create a "Random ROI" generator
- [ ] Add keyboard shortcuts (Delete to remove, etc.)
- [ ] Display ROI coordinates in a tooltip

### Intermediate Challenges
- [ ] Implement ROI grouping/folders
- [ ] Add a mini-map or overview camera
- [ ] Create ROI templates (save favorite configs)
- [ ] Multi-select ROIs (shift+click)

### Advanced Challenges
- [ ] Boolean operations (union, intersection, difference)
- [ ] ROI animation timeline
- [ ] Import/Export to medical imaging formats (DICOM)
- [ ] GPU-accelerated ray-casting
- [ ] Multi-viewport rendering (top, side, front views)

---

## 📄 License

These examples are provided as educational material. Feel free to use, modify, and learn from them!

---

## 🙋 Questions?

Study the code comments and experiment! The best way to learn is by doing.

**Happy Coding! 🚀**

---

*Created as a complete learning package for WPF 3D beginners to advanced developers*
