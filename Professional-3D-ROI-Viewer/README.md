# Professional 3D ROI Viewer & Editor

**A Production-Ready WPF 3D Application for ROI Manipulation and Visualization**

---

## 🎯 Overview

This is a **professional-grade 3D Region of Interest (ROI) viewer and editor** built with WPF and C#. It demonstrates advanced 3D graphics programming, professional UI/UX design, and real-world application architecture.

### What Makes This Professional?

✅ **Advanced 3D Rendering**
- Multiple ROI shapes with smooth rendering
- Real-time mesh generation
- Professional lighting and materials
- Wireframe and solid rendering modes

✅ **Sophisticated ROI Management**
- Create, edit, delete, duplicate ROIs
- Multiple ROI types: Box, Sphere, Cylinder, Cone, Ellipsoid, Custom Mesh
- Hierarchical ROI organization
- ROI grouping and layering

✅ **Interactive 3D Manipulation**
- Mouse-based camera control (orbit, pan, zoom)
- Direct 3D manipulation handles
- Precise numeric input
- Snap-to-grid functionality

✅ **Advanced Visualization Features**
- Multiple clipping planes (X, Y, Z)
- Cross-section views
- Volume rendering concepts
- Transparency and blending
- Measurement tools

✅ **Professional UI/UX**
- Modern dark theme interface
- Contextual menus and toolbars
- Keyboard shortcuts
- Status bar with realtime information
- Dockable panels

✅ **Data Management**
- Save/Load scenes (JSON format)
- Export to various formats
- Import 3D models
- Batch operations
- Undo/Redo system

✅ **Analysis Tools**
- Volume calculations
- Distance measurements
- Center of mass
- Bounding box analysis
- Statistics panel

---

## 🚀 Features Overview

### Core Functionality

#### 1. ROI Creation & Editing
- **7 ROI Types:** Box, Sphere, Cylinder, Cone, Ellipsoid, Torus, Custom
- **Full 6-DOF Control:** Position (X,Y,Z) + Rotation (X,Y,Z)
- **Precise Sizing:** Width, Height, Depth controls
- **Real-time Preview:** See changes immediately
- **Material Editor:** Colors, transparency, specular properties

#### 2. 3D Visualization
- **Multi-viewport:** Single, 2-split, 4-split layouts
- **Camera Modes:** Perspective, Orthographic, Isometric
- **Rendering Modes:**
  - Solid fill
  - Wireframe
  - Wireframe on solid
  - Points only
  - X-Ray (transparent)

#### 3. Advanced Manipulation
- **3D Gizmos:** Visual manipulation handles
- **Transform Tools:**
  - Move (translate)
  - Rotate (on any axis)
  - Scale (uniform or per-axis)
- **Snapping:**
  - Snap to grid
  - Snap to other ROIs
  - Angular snapping (15°, 30°, 45°, 90°)

#### 4. Clipping & Slicing
- **Clipping Planes:** Up to 6 simultaneous planes (±X, ±Y, ±Z)
- **Cross-Sections:** View interior of ROIs
- **Dynamic Updates:** Real-time clipping visualization
- **Slice Export:** Export cross-section images

#### 5. Measurement & Analysis
- **Distance Tool:** Measure between points
- **Volume Calculator:** Precise volume for each ROI
- **Center of Mass:** Calculate centroid
- **Bounding Box:** Compute minimal bounding box
- **Overlap Detection:** Check ROI intersections

#### 6. Professional Tools
- **Layer System:** Organize ROIs in layers
- **Visibility Control:** Show/hide individual or groups
- **Locking:** Prevent accidental edits
- **Selection Sets:** Save selection groups
- **Batch Operations:** Apply operations to multiple ROIs

---

## 📁 Application Structure

```
Professional-3D-ROI-Viewer/
├── README.md                    ← This file
├── FEATURES.md                  ← Detailed feature documentation
├── USER_GUIDE.md                ← Complete user manual
├── TECHNICAL_GUIDE.md           ← Architecture and code guide
│
├── ROIViewer.csproj             ← Main project file
├── App.xaml                     ← Application definition
├── App.xaml.cs                  ← Application startup
│
├── MainWindow.xaml              ← Main window UI
├── MainWindow.xaml.cs           ← Main window logic
│
├── Models/                      ← Data models
│   ├── ROI.cs                   ← ROI data model
│   ├── Scene.cs                 ← Scene container
│   ├── Material.cs              ← Material properties
│   └── ViewportSettings.cs      ← Viewport configuration
│
├── ViewModels/                  ← MVVM view models
│   ├── MainViewModel.cs         ← Main window VM
│   ├── ROIViewModel.cs          ← ROI properties VM
│   └── SceneViewModel.cs        ← Scene management VM
│
├── Views/                       ← Additional windows/controls
│   ├── PropertiesPanel.xaml     ← ROI properties panel
│   ├── LayersPanel.xaml         ← Layer management
│   ├── MeasurementPanel.xaml    ← Measurement tools
│   └── SettingsWindow.xaml      ← Application settings
│
├── Services/                    ← Business logic
│   ├── MeshGenerator.cs         ← Procedural mesh creation
│   ├── CameraController.cs      ← Camera manipulation
│   ├── ROIManager.cs            ← ROI operations
│   ├── ClippingService.cs       ← Clipping plane logic
│   ├── MeasurementService.cs    ← Measurement calculations
│   └── FileService.cs           ← Save/Load operations
│
├── Helpers/                     ← Utility classes
│   ├── Math3D.cs                ← 3D math utilities
│   ├── MeshHelper.cs            ← Mesh manipulation
│   └── ColorHelper.cs           ← Color utilities
│
└── Resources/                   ← Assets
    ├── Icons/                   ← UI icons
    ├── Shaders/                 ← Effect files (if using)
    └── Samples/                 ← Sample ROI files
```

---

## 🎨 User Interface

### Main Window Layout

```
┌─────────────────────────────────────────────────────────┐
│ ☰ File  Edit  View  Tools  ROI  Window  Help     [_][□][X] │
├─────────────────────────────────────────────────────────┤
│ [🗎][💾][📁] | [+][-][⎘] | [⬅][⬆][⬇][➡] | [🔍+][🔍-]  │  Toolbar
├───────┬──────────────────────────────────┬──────────────┤
│       │                                  │ ROI          │
│ Scene │         3D Viewport              │ Properties   │
│ Tree  │                                  │              │
│       │    [Interactive 3D View]         │ ┌──────────┐ │
│  📁   │                                  │ │Name:     │ │
│  ├🔲  │                                  │ │Position: │ │
│  ├🔵  │                                  │ │Size:     │ │
│  └🔺  │                                  │ │Rotation: │ │
│       │                                  │ │Color:    │ │
│       │                                  │ │Opacity:  │ │
│       │                                  │ └──────────┘ │
│       │                                  │              │
├───────┴──────────────────────────────────┴──────────────┤
│ Ready | Camera: Orbit | ROIs: 3 | Selected: Box_001     │  Status
└─────────────────────────────────────────────────────────┘
```

### Key UI Elements

**1. Menu Bar**
- File: New, Open, Save, Export, Import, Recent Files, Exit
- Edit: Undo, Redo, Cut, Copy, Paste, Delete, Select All
- View: Viewports, Camera, Rendering Mode, Show/Hide Elements
- Tools: Measure, Clipping Planes, Grid Settings, Snap Options
- ROI: Create, Edit, Duplicate, Group, Align, Distribute
- Window: Panels, Layout Presets, Fullscreen
- Help: User Guide, Keyboard Shortcuts, About

**2. Toolbar** (Quick Access)
- File operations (New, Open, Save)
- ROI operations (Add, Remove, Duplicate)
- Navigation (Pan, Rotate, Zoom, Fit All)
- Tools (Measure, Clip, Grid Toggle)

**3. Scene Tree** (Left Panel)
- Hierarchical ROI list
- Layer organization
- Visibility toggles
- Lock icons
- Color indicators
- Search/filter

**4. 3D Viewport** (Center)
- Main rendering area
- Mouse interaction
- Overlay information
- Grid display
- Axis gizmo
- Manipulation handles

**5. Properties Panel** (Right)
- ROI properties
- Transform controls
- Material settings
- Advanced options
- Quick presets

**6. Bottom Panels** (Collapsible)
- Measurement results
- Clipping plane controls
- Layer manager
- Statistics

**7. Status Bar**
- Current tool
- Camera information
- ROI count
- Selection info
- Performance metrics

---

## 🎮 Controls & Shortcuts

### Mouse Controls

**3D Viewport:**
- **Left Click:** Select ROI
- **Left Drag:** Rotate camera (orbit)
- **Right Drag:** Pan camera
- **Middle Drag:** Pan camera (alternative)
- **Scroll Wheel:** Zoom in/out
- **Ctrl + Left Drag:** Multi-select ROIs
- **Alt + Left Drag:** Duplicate ROI
- **Double Click:** Focus on ROI

**With Gizmo Active:**
- **Drag X-Axis (Red):** Move along X
- **Drag Y-Axis (Green):** Move along Y
- **Drag Z-Axis (Blue):** Move along Z
- **Drag Rotation Ring:** Rotate

### Keyboard Shortcuts

**General:**
- `Ctrl + N` - New scene
- `Ctrl + O` - Open scene
- `Ctrl + S` - Save scene
- `Ctrl + Shift + S` - Save as
- `Ctrl + Z` - Undo
- `Ctrl + Y` - Redo
- `Delete` - Delete selected ROI
- `Escape` - Deselect all

**ROI Operations:**
- `Ctrl + D` - Duplicate selected
- `Ctrl + G` - Group selected
- `Ctrl + Shift + G` - Ungroup
- `H` - Hide selected
- `Shift + H` - Show all
- `L` - Lock selected
- `Shift + L` - Unlock all

**View:**
- `F` - Frame selected (fit in view)
- `A` - Frame all
- `1-7` - Front, Back, Left, Right, Top, Bottom, Isometric views
- `Space` - Toggle perspective/orthographic
- `W` - Toggle wireframe
- `G` - Toggle grid
- `X` - Toggle X-ray mode

**Tools:**
- `M` - Measure tool
- `C` - Clipping plane tool
- `R` - Rotate tool
- `T` - Translate tool
- `S` - Scale tool

---

## 🔧 Advanced Features

### 1. Clipping Planes

Slice through your ROIs to see inside!

**How to Use:**
1. Open Clipping Planes panel (Tools → Clipping Planes)
2. Enable desired clipping plane (X, Y, or Z)
3. Adjust slider to move plane position
4. Toggle display of clipping plane surface
5. Invert clipping direction if needed

**Use Cases:**
- View interior of complex ROIs
- Create cross-section views
- Analyze layered structures
- Export slice images

### 2. Measurement Tools

Precise measurements in 3D space.

**Available Measurements:**
- **Distance:** Between two points
- **Angle:** Between three points
- **Volume:** Of selected ROI
- **Surface Area:** Mesh surface area
- **Center Point:** Centroid location
- **Bounding Box:** Minimal enclosing box

**How to Measure:**
1. Select Measure tool
2. Click first point in 3D
3. Click second point (for distance)
4. Result shown in panel and overlay

### 3. Multi-Viewport Mode

View from multiple angles simultaneously!

**Layouts:**
- **Single:** Full viewport
- **Vertical Split:** Left/Right
- **Horizontal Split:** Top/Bottom
- **Quad:** Four equal viewports
- **Custom:** Resize as needed

**Synchronized:**
- ROI selection
- Manipulation operations
- Layer visibility

**Independent:**
- Camera positions
- Rendering modes
- Clipping planes (optional)

### 4. Export Capabilities

**Supported Formats:**
- **Scene Files:** `.roi` (JSON format)
- **3D Models:** `.obj`, `.stl` (export ROI meshes)
- **Images:** `.png`, `.jpg` (viewport screenshots)
- **Data:** `.csv` (ROI properties and measurements)

**Batch Export:**
- Export all ROIs as separate files
- Generate image sequence (rotating camera)
- Create contact sheet (multiple views)

---

## 💻 Technical Specifications

### Built With

- **.NET 9.0** - Latest framework
- **WPF** - Windows Presentation Foundation
- **C# 13** - Modern C# features
- **XAML** - Declarative UI

### Performance

- **Optimized Rendering:** Efficient mesh generation and caching
- **Selective Updates:** Only redraw when necessary
- **LOD System:** Level of detail for complex scenes
- **Spatial Indexing:** Fast hit testing and selection

### Requirements

- **OS:** Windows 10/11 (64-bit)
- **RAM:** 8GB minimum, 16GB recommended
- **GPU:** DirectX 11 compatible graphics card
- **Display:** 1920x1080 or higher
- **.NET:** 9.0 SDK/Runtime

---

## 📘 Quick Start Guide

### First Time Setup

1. **Install Prerequisites**
   ```bash
   # Check .NET version
   dotnet --version  # Should be 9.0.xxx
   ```

2. **Run the Application**
   ```bash
   cd Professional-3D-ROI-Viewer
   dotnet run
   ```

3. **Create Your First ROI**
   - Click `+` button in toolbar
   - Or: Menu → ROI → Create → Box
   - Adjust properties in right panel

4. **Manipulate the ROI**
   - Select ROI by clicking
   - Use sliders in properties panel
   - Or drag manipulation gizmo in 3D

5. **Save Your Work**
   - File → Save Scene (`Ctrl + S`)
   - Choose location and name
   - Save as `.roi` file

### Example Workflow

**Scenario:** Creating a multi-ROI anatomical model

1. **Create Base Structure**
   - Add Box for main body
   - Add Sphere for head
   - Add Cylinders for limbs

2. **Position & Size**
   - Use transform tools
   - Enable snap-to-grid
   - Align using distribution tools

3. **Group & Organize**
   - Select related ROIs
   - Ctrl + G to group
   - Name layers meaningfully

4. **Analyze**
   - Measure distances
   - Calculate volumes
   - Check for overlaps

5. **Export**
   - Save scene file
   - Export as OBJ for external use
   - Generate documentation images

---

## 🎓 Learning Resources

### Included Documentation

- **USER_GUIDE.md:** Step-by-step tutorials
- **FEATURES.md:** Complete feature reference
- **TECHNICAL_GUIDE.md:** Code architecture
- **EXAMPLES.md:** Sample projects and use cases

### Code Examples

Each major feature includes:
- Detailed code comments
- Usage examples
- Best practices
- Performance tips

---

## 🏆 Professional Use Cases

This application is suitable for:

### Medical Imaging
- Define regions of interest in CT/MRI scans
- Volume measurement and analysis
- Treatment planning
- Organ segmentation

### Engineering
- CAD model verification
- Assembly analysis
- Tolerance checking
- Measurement and inspection

### Scientific Research
- 3D data visualization
- Spatial analysis
- Experimental planning
- Results presentation

### Education
- Teaching 3D graphics concepts
- Demonstrating spatial relationships
- Interactive learning tool
- Project-based learning

---

## 🚀 What's Next?

This application demonstrates:

✅ **Professional WPF 3D Development**
✅ **Advanced Mesh Generation**
✅ **Interactive 3D Manipulation**
✅ **Production-Ready Architecture**
✅ **Modern UI/UX Design**
✅ **Data Management**
✅ **Real-World Applicability**

---

## 📄 License

Educational and commercial use allowed. See LICENSE file for details.

---

## 👨‍💻 About

Created as a demonstration of professional WPF 3D application development. This represents industry-standard practices, patterns, and architecture.

**Version:** 1.0
**Last Updated:** 2024

---

**Start exploring the world of professional 3D ROI visualization! 🎉**
