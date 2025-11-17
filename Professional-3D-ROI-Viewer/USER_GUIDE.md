# Professional 3D ROI Viewer - User Guide

## Table of Contents
1. [Getting Started](#getting-started)
2. [User Interface Overview](#user-interface-overview)
3. [Working with ROIs](#working-with-rois)
4. [Camera Controls](#camera-controls)
5. [Viewing Options](#viewing-options)
6. [Measurements and Analysis](#measurements-and-analysis)
7. [File Operations](#file-operations)
8. [Keyboard Shortcuts](#keyboard-shortcuts)
9. [Tips and Best Practices](#tips-and-best-practices)
10. [Troubleshooting](#troubleshooting)

---

## Getting Started

### System Requirements
- **Operating System**: Windows 10 or later
- **.NET Runtime**: .NET 9.0 or later
- **Graphics**: DirectX 11 compatible GPU
- **Memory**: 4 GB RAM minimum (8 GB recommended)

### First Launch
1. Launch the application
2. You'll see a 3D viewport with a grid and coordinate axes
3. The left panel shows the ROI list (initially empty)
4. The right panel contains property controls
5. Try adding your first ROI using **ROI → Add Box** or click the toolbar button

### Quick Start Tutorial

**Creating Your First ROI:**
1. Click **ROI → Add Box** (or press `Ctrl+B`)
2. A red box will appear at the origin (0, 0, 0)
3. Adjust properties in the right panel:
   - Change **Position** using X, Y, Z sliders
   - Modify **Size** using Width, Height, Depth sliders
   - Rotate using Rotation X, Y, Z sliders
4. Click the color button to change the ROI color

**Moving the Camera:**
- **Left-click and drag**: Orbit around the scene
- **Right-click and drag**: Pan the camera
- **Mouse wheel**: Zoom in/out
- **View menu**: Quick preset views (Front, Top, Right, etc.)

---

## User Interface Overview

### Main Window Layout

```
┌─────────────────────────────────────────────────────────────┐
│ File  Edit  View  ROI  Tools  Help                          │ Menu Bar
├─────────────────────────────────────────────────────────────┤
│ [+Box] [+Sphere] [+Cylinder] [Grid] [Axes] [Wireframe]      │ Toolbar
├──────────┬──────────────────────────────────┬───────────────┤
│ ROI List │                                  │  Properties   │
│          │                                  │               │
│ □ Box1   │       3D Viewport                │  Name: Box1   │
│ □ Sphere1│                                  │  Shape: Box   │
│ □ Cyl1   │                                  │               │
│          │                                  │  Position:    │
│ [Add]    │                                  │  X: [====]    │
│ [Remove] │                                  │  Y: [====]    │
│ [Dup]    │                                  │  Z: [====]    │
│          │                                  │               │
│          │                                  │  Size:        │
│          │                                  │  W: [====]    │
│          │                                  │  H: [====]    │
│          │                                  │  D: [====]    │
│          │                                  │               │
│          │                                  │  [Color]      │
│          │                                  │  Opacity: [=] │
├──────────┴──────────────────────────────────┴───────────────┤
│ ROIs: 3 | Camera: 5.0, 3.0, 8.0 | Zoom: 10.0              │ Status Bar
└─────────────────────────────────────────────────────────────┘
```

### Left Panel: ROI Manager
- **ROI List**: Shows all ROIs in your scene
  - Checkbox: Toggle visibility
  - Color indicator: Shows ROI color
  - Name: Double-click to rename
- **Buttons**:
  - **Add**: Create new ROI (opens shape menu)
  - **Remove**: Delete selected ROI
  - **Duplicate**: Clone selected ROI

### Center Panel: 3D Viewport
- Main viewing area for your 3D scene
- Shows grid (when enabled)
- Shows coordinate axes (when enabled)
- Interactive camera controls

### Right Panel: Properties
- **ROI Information**: Name and shape type
- **Position Controls**: X, Y, Z sliders (-10 to +10)
- **Size Controls**: Width, Height, Depth sliders (0.1 to 10)
- **Rotation Controls**: X, Y, Z sliders (0° to 360°)
- **Appearance**: Color picker, opacity slider
- **Visibility**: Show/hide, lock/unlock
- **Advanced**: Wireframe mode, layer assignment
- **Quick Actions**: Center to origin, reset rotation, reset scale

### Status Bar
- ROI count
- Current camera position
- Zoom level
- Selection information

---

## Working with ROIs

### Creating ROIs

**Available ROI Types:**

1. **Box** (`Ctrl+B`)
   - Rectangular prism with independent Width, Height, Depth
   - Default size: 2×2×2 units
   - Use for: Rectangular regions, bounding volumes

2. **Sphere** (`Ctrl+S`)
   - Perfect sphere with radius control
   - Size controlled by Width (diameter)
   - Use for: Circular regions, range detection

3. **Cylinder** (`Ctrl+Y`)
   - Circular cylinder with height and radius
   - Width controls diameter, Height controls length
   - Use for: Tubular regions, vertical bounds

4. **Cone**
   - Tapered cylinder from base to point
   - Base radius and height adjustable
   - Use for: Directional regions, spotlight volumes

5. **Ellipsoid**
   - Stretched sphere with three radii
   - Independent Width, Height, Depth control
   - Use for: Organic shapes, deformed volumes

6. **Torus** (`Ctrl+T`)
   - Donut shape with major and minor radius
   - Width controls outer ring, Height controls tube thickness
   - Use for: Ring regions, circular paths

7. **Pyramid**
   - Four-sided pyramid with square base
   - Base size and height adjustable
   - Use for: Hierarchical regions, peaked volumes

### Selecting ROIs

**Methods:**
- Click ROI name in the left panel list
- Click ROI in 3D viewport (upcoming feature)
- Use **Edit → Select All** to select multiple

**Selection Indicators:**
- Selected ROI is highlighted in the list
- Properties panel shows selected ROI details

### Editing ROI Properties

**Position:**
1. Select ROI in the list
2. Use Position X, Y, Z sliders in the right panel
3. Values range from -10 to +10 units
4. Click **Center to Origin** button to reset to (0,0,0)

**Size:**
1. Use Size Width, Height, Depth sliders
2. Values range from 0.1 to 10 units
3. Click **Reset Scale** to restore default size
4. For spheres: Width controls diameter (Height/Depth ignored)
5. For cylinders: Width=diameter, Height=length

**Rotation:**
1. Use Rotation X, Y, Z sliders
2. Values range from 0° to 360°
3. Rotations are applied in order: X → Y → Z (Euler angles)
4. Click **Reset Rotation** to return to 0°

**Appearance:**
1. Click **Color** button to open color picker
2. Use **Opacity** slider (0% = transparent, 100% = opaque)
3. Check **Show Wireframe** to see edges only
4. Uncheck visibility checkbox to hide ROI

### Duplicating ROIs

**Method 1: Duplicate Button**
1. Select ROI to duplicate
2. Click **Duplicate** button
3. New ROI appears offset by (1, 1, 1) from original

**Method 2: Copy/Paste (Future)**
- Use `Ctrl+C` to copy
- Use `Ctrl+V` to paste

### Deleting ROIs

1. Select ROI to delete
2. Click **Remove** button or press `Delete` key
3. Confirm deletion in dialog

**Bulk Delete:**
- Select multiple ROIs (upcoming feature)
- Press `Delete` key

---

## Camera Controls

### Mouse Controls

**Orbit (Rotate Around Scene):**
- **Left-click and drag** in viewport
- Camera rotates around the center point
- Horizontal drag: Rotate left/right (azimuth)
- Vertical drag: Rotate up/down (elevation)
- Elevation clamped to -89° to +89° (prevents flipping)

**Pan (Move Camera Parallel):**
- **Right-click and drag** in viewport
- Camera moves parallel to view plane
- Useful for repositioning focus point

**Zoom:**
- **Mouse wheel scroll**
- Scroll up: Zoom in (closer)
- Scroll down: Zoom out (farther)
- Zoom range: 2 to 50 units from center

**Reset View:**
- **View → Reset Camera** (`Ctrl+R`)
- Returns to default position: (5, 3, 8)

### Preset Views

Access via **View** menu:

- **Front View** (`F`): Camera at (0, 0, 15)
- **Back View**: Camera at (0, 0, -15)
- **Top View** (`T`): Camera at (0, 15, 0)
- **Bottom View**: Camera at (0, -15, 0)
- **Right View** (`R`): Camera at (15, 0, 0)
- **Left View**: Camera at (-15, 0, 0)
- **Isometric View** (`I`): Camera at (10, 10, 10)

All preset views automatically point toward the origin (0, 0, 0).

### Camera Properties

Current camera properties displayed in status bar:
- **Position**: Current X, Y, Z coordinates
- **Zoom Level**: Distance from center
- **Field of View**: 60° (fixed)

---

## Viewing Options

### Grid Display

**Toggle Grid:**
- **View → Show Grid** or toolbar button
- Default: Enabled

**Grid Properties:**
- Size: 20×20 units
- Spacing: 1 unit per line
- Color: Dark gray lines on darker background
- Position: Centered at origin in XZ plane (ground)

**Use Cases:**
- Spatial reference
- Scale estimation
- Alignment guide

### Axes Display

**Toggle Axes:**
- **View → Show Axes** or toolbar button
- Default: Enabled

**Axes Indicators:**
- **Red Arrow**: +X axis (right)
- **Green Arrow**: +Y axis (up)
- **Blue Arrow**: +Z axis (forward)
- Arrow length: 5 units
- Origin marked at (0, 0, 0)

### Wireframe Mode

**Per-ROI Wireframe:**
1. Select ROI
2. Check **Show Wireframe** in properties panel
3. ROI displays edges only (no solid fill)

**Global Wireframe:**
- **View → Wireframe Mode**
- Shows all ROIs as wireframes
- Useful for seeing overlapping regions

### Lighting Options

**Toggle Lighting:**
- **View → Toggle Lighting**
- Default: Directional light from top-right

**Lighting Modes:**
- **Full Lighting**: Realistic shading with shadows
- **Ambient Only**: Flat lighting (easier to see details)

---

## Measurements and Analysis

### Volume Calculation

**Automatic Display:**
- Volume shown in properties panel for selected ROI
- Updates in real-time as size changes
- Units: cubic units

**Volume Formulas:**
- **Box**: Width × Height × Depth
- **Sphere**: (4/3) × π × (Radius)³
- **Cylinder**: π × (Radius)² × Height
- **Cone**: (1/3) × π × (Radius)² × Height
- **Ellipsoid**: (4/3) × π × RadiusX × RadiusY × RadiusZ
- **Torus**: 2 × π² × MajorRadius × (MinorRadius)²
- **Pyramid**: (1/3) × BaseWidth² × Height

### Surface Area Calculation

**Automatic Display:**
- Surface area shown in properties panel
- Updates in real-time
- Units: square units

**Surface Area Formulas:**
- **Box**: 2(WH + WD + HD)
- **Sphere**: 4 × π × (Radius)²
- **Cylinder**: 2π × Radius × (Height + Radius)
- **Cone**: π × Radius × (Radius + SlantHeight)
- **Ellipsoid**: Approximate using Knud Thomsen's formula

### Distance Measurement

**Measure Between ROIs:**
1. Select first ROI
2. **Tools → Measure Distance**
3. Click second ROI
4. Distance displayed in dialog

**Distance Types:**
- **Center-to-Center**: Distance between ROI centers
- **Edge-to-Edge**: Closest approach (coming soon)
- **Overlap Amount**: Penetration depth (coming soon)

### Overlap Detection

**Check Overlaps:**
- **Tools → Check Overlaps**
- Displays list of all overlapping ROI pairs
- Uses Axis-Aligned Bounding Box (AABB) detection
- Useful for validation and conflict detection

**Overlap Visualization:**
- Coming soon: Highlight overlapping regions
- Coming soon: Color-code by overlap amount

---

## File Operations

### Saving Scenes

**Save Current Scene:**
1. **File → Save** (`Ctrl+S`)
2. Choose file location
3. Enter filename (*.roiscene extension)

**Save As:**
- **File → Save As...** (`Ctrl+Shift+S`)
- Save with new name or location

**File Format:**
- JSON-based text format
- Human-readable
- Includes all ROIs with full properties
- Example:
```json
{
  "ROIs": [
    {
      "Name": "Box1",
      "Shape": "Box",
      "PositionX": 0.0,
      "PositionY": 1.5,
      "PositionZ": 0.0,
      "Width": 2.0,
      "Height": 3.0,
      "Depth": 2.0,
      "Color": "#FFFF0000",
      "Opacity": 0.7
    }
  ]
}
```

### Loading Scenes

**Open Scene:**
1. **File → Open...** (`Ctrl+O`)
2. Browse to *.roiscene file
3. Scene loads, replacing current ROIs

**Recent Files:**
- **File → Recent** submenu
- Quick access to last 5 opened files

**Import Merge:**
- **File → Import...** (Coming soon)
- Add ROIs from file without clearing current scene

### Exporting

**Export Options (Coming Soon):**
- **Export to OBJ**: 3D mesh file for other applications
- **Export to STL**: For 3D printing
- **Export to CSV**: ROI properties as spreadsheet
- **Export Screenshot**: Save viewport as image

### New Scene

**Start Fresh:**
- **File → New** (`Ctrl+N`)
- Clears all ROIs
- Resets camera
- Prompts to save if unsaved changes exist

---

## Keyboard Shortcuts

### File Operations
- `Ctrl+N` - New scene
- `Ctrl+O` - Open scene
- `Ctrl+S` - Save scene
- `Ctrl+Shift+S` - Save As

### Edit Operations
- `Ctrl+Z` - Undo (coming soon)
- `Ctrl+Y` - Redo (coming soon)
- `Ctrl+C` - Copy ROI (coming soon)
- `Ctrl+V` - Paste ROI (coming soon)
- `Delete` - Delete selected ROI

### ROI Creation
- `Ctrl+B` - Add Box
- `Ctrl+S` - Add Sphere
- `Ctrl+Y` - Add Cylinder
- `Ctrl+T` - Add Torus

### View Controls
- `Ctrl+R` - Reset Camera
- `F` - Front view
- `T` - Top view
- `R` - Right view
- `I` - Isometric view
- `G` - Toggle Grid
- `A` - Toggle Axes

### Window
- `F11` - Toggle Fullscreen (coming soon)
- `Ctrl+W` - Close window

---

## Tips and Best Practices

### Performance Optimization

**For Complex Scenes:**
1. Use **Show Wireframe** for distant ROIs
2. Hide ROIs not currently needed
3. Reduce sphere/cylinder subdivisions in code for faster rendering
4. Use simpler shapes (boxes) instead of complex ones (torus) when possible

**Large ROI Counts:**
- 100+ ROIs: Expect some lag during camera movement
- Use layers to organize and hide groups
- Consider splitting into multiple scene files

### Workflow Recommendations

**Medical/Biological Imaging:**
1. Start with anatomical coordinate axes
2. Use consistent naming: "Tumor_1", "Tumor_2", etc.
3. Use color coding: Red for primary, Yellow for secondary
4. Save frequently during annotation sessions

**Engineering/CAD:**
1. Use grid for alignment
2. Enable snap-to-grid (coming soon)
3. Use precise numeric entry for positions
4. Lock ROIs once positioned correctly

**Scientific Visualization:**
1. Use transparent ROIs (50% opacity) to see overlaps
2. Use different shapes for different data types
3. Export scenes for documentation
4. Include measurement annotations

### Common Tasks

**Creating Symmetrical ROIs:**
1. Create first ROI at +X position
2. Duplicate ROI
3. Set second ROI X position to negative value
4. Both ROIs mirror across YZ plane

**Building Complex Regions:**
1. Start with simple shapes (boxes)
2. Position roughly with mouse
3. Fine-tune with numeric sliders
4. Check overlaps before finalizing
5. Save incrementally

**Scene Organization:**
1. Use consistent naming conventions
2. Group related ROIs with prefixes: "Brain_", "Liver_"
3. Assign layers for easy show/hide
4. Document in separate notes file

---

## Troubleshooting

### Application Won't Start

**Problem**: Double-clicking does nothing
- **Solution**: Install .NET 9.0 Runtime from Microsoft
- **Check**: Right-click → Properties → Unblock file if downloaded

**Problem**: Error about missing DLL
- **Solution**: Repair .NET installation
- **Try**: Run from command line to see error details

### Performance Issues

**Problem**: Slow camera movement
- **Solution**: Hide unused ROIs
- **Solution**: Reduce number of visible ROIs
- **Solution**: Close other GPU-intensive applications

**Problem**: Application freezes
- **Solution**: Task Manager → End Task → Restart
- **Prevention**: Save frequently with Auto-save

### Display Problems

**Problem**: Black viewport
- **Solution**: Update graphics drivers
- **Check**: DirectX 11 support
- **Try**: View → Reset Camera

**Problem**: ROIs not visible
- **Check**: ROI visibility checkbox is checked
- **Check**: Camera is not inside ROI
- **Try**: Zoom out with mouse wheel

**Problem**: Colors look wrong
- **Check**: Opacity is not set to 0%
- **Try**: View → Toggle Lighting
- **Update**: Graphics drivers

### File Issues

**Problem**: Can't open saved scene
- **Check**: File extension is .roiscene
- **Check**: File is not corrupted (open in text editor)
- **Try**: Open recent backup from temp folder

**Problem**: Save fails
- **Check**: Disk space available
- **Check**: Write permissions for folder
- **Try**: Save to different location

### Input Problems

**Problem**: Mouse controls not working
- **Check**: Viewport has focus (click in it first)
- **Try**: View → Reset Camera
- **Restart**: Application if persistent

**Problem**: Sliders not responding
- **Check**: ROI is selected in list
- **Try**: Click ROI name to reselect
- **Restart**: Application if persistent

### Getting Help

**Built-in Help:**
- **Help → User Guide** (opens this document)
- **Help → Keyboard Shortcuts** (quick reference)
- **Help → About** (version information)

**Online Resources:**
- GitHub repository: [Coming Soon]
- Issue tracker: Report bugs
- Discussions: Ask questions
- Wiki: Advanced tutorials

**Contact Support:**
- Email: [Your Email]
- Include: Application version, OS version, error messages
- Attach: Log file from Documents/ROIViewer/Logs

---

## Appendix A: Use Case Examples

### Medical Imaging Workflow

**Scenario**: Annotating tumor regions in 3D MRI scan

1. Load patient scan (future feature)
2. Create ROI for primary tumor (sphere)
3. Adjust position and size to match scan
4. Create ROIs for metastases (multiple spheres)
5. Use different colors for different stages
6. Measure volumes automatically
7. Check for overlaps with critical structures
8. Save scene with patient ID
9. Export measurements to CSV

### Engineering Application

**Scenario**: Defining collision volumes for robot arm

1. Create box ROIs for arm segments
2. Position at joint locations
3. Create spherical "danger zones" around gripper
4. Test range of motion by rotating ROIs
5. Check for overlaps (collisions)
6. Adjust sizes to minimize false positives
7. Export to OBJ for simulation software

### Scientific Research

**Scenario**: Marking sampling regions in 3D space

1. Create grid of cube ROIs (sampling volumes)
2. Use consistent spacing
3. Color-code by priority (red=high, yellow=medium, green=low)
4. Mark exclusion zones (spheres)
5. Calculate total sample volume
6. Generate sampling coordinates list
7. Export scene for field reference

---

## Appendix B: Advanced Features (Coming Soon)

- Multi-viewport display (Front/Top/Side + 3D)
- Custom clipping planes
- Boolean operations (Union, Intersect, Subtract)
- ROI animations
- Scripting API (Python)
- Plugin system
- Real-time data import
- Collaborative editing
- VR/AR support

---

**Document Version**: 1.0
**Last Updated**: 2025
**Application Version**: Professional 3D ROI Viewer 1.0
**Target Framework**: .NET 9.0
