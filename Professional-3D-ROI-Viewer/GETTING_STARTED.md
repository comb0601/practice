# Getting Started with Professional 3D ROI Viewer

## Quick Start Guide

This guide will get you up and running with the Professional 3D ROI Viewer in 5 minutes.

---

## Prerequisites

Before you begin, ensure you have:

- **Windows 10** or **Windows 11**
- **.NET 9.0 Runtime** - [Download here](https://dotnet.microsoft.com/download/dotnet/9.0)
- **4 GB RAM** minimum (8 GB recommended)
- **DirectX 11** compatible graphics card

---

## Installation

### Option 1: Run from Source (Developers)

```bash
# 1. Clone or download the repository
git clone <repository-url>
cd Professional-3D-ROI-Viewer

# 2. Restore dependencies
dotnet restore

# 3. Run the application
dotnet run
```

### Option 2: Run Published Build (End Users)

```bash
# 1. Build self-contained executable
dotnet publish --configuration Release --runtime win-x64 --self-contained

# 2. Navigate to output folder
cd bin/Release/net9.0-windows/win-x64/publish/

# 3. Run ROIViewer.exe
./ROIViewer.exe
```

---

## Your First Session

### Step 1: Launch the Application

When you first launch the app, you'll see:

```
┌─────────────────────────────────────────┐
│  Professional 3D ROI Viewer             │
├─────────────────────────────────────────┤
│                                         │
│  Empty scene with:                      │
│  • Grid (ground plane)                  │
│  • Coordinate axes (X=Red, Y=Green, Z=Blue)
│  • Dark background                      │
│                                         │
└─────────────────────────────────────────┘
```

### Step 2: Create Your First ROI

**Add a Box:**
1. Click **ROI** menu → **Add Box**
   - OR press `Ctrl+B`
   - OR click the **[+Box]** toolbar button

2. You'll see a red box appear at the center (origin)

**Success!** You've created your first ROI.

### Step 3: Move the ROI

Look at the **Properties Panel** on the right:

1. Find the **Position** section
2. Move the **X slider** left and right
   - Watch the box move horizontally
3. Move the **Y slider** up and down
   - Watch the box move vertically
4. Move the **Z slider** forward and back
   - Watch the box move in depth

### Step 4: Resize the ROI

In the **Size** section:

1. Move the **Width slider** to make the box wider/narrower
2. Move the **Height slider** to make it taller/shorter
3. Move the **Depth slider** to make it deeper/shallower

### Step 5: Rotate the ROI

In the **Rotation** section:

1. Move the **X slider** to rotate around the X axis (pitch)
2. Move the **Y slider** to rotate around the Y axis (yaw)
3. Move the **Z slider** to rotate around the Z axis (roll)

### Step 6: Change the Color

1. Click the **Color** button in the Properties panel
2. Pick a color from the color picker
3. Click **OK**
4. Adjust the **Opacity slider** to make it transparent/opaque

### Step 7: Control the Camera

**Rotate the view:**
- Click and drag with the **left mouse button**
- The camera orbits around the scene

**Pan the view:**
- Click and drag with the **right mouse button**
- The camera moves parallel to the screen

**Zoom in/out:**
- Scroll the **mouse wheel** up to zoom in
- Scroll the **mouse wheel** down to zoom out

### Step 8: Add More ROIs

Try adding different shapes:

1. **ROI** → **Add Sphere** (`Ctrl+S`)
2. **ROI** → **Add Cylinder** (`Ctrl+Y`)
3. **ROI** → **Add Cone**
4. **ROI** → **Add Torus** (`Ctrl+T`)

Each ROI appears in the **ROI List** on the left.

### Step 9: Select ROIs

Click on any ROI name in the left panel list to select it. The Properties panel will show that ROI's properties.

### Step 10: Save Your Work

1. Click **File** → **Save** (or press `Ctrl+S`)
2. Choose a location
3. Enter a filename (e.g., "MyFirstScene.roiscene")
4. Click **Save**

**Congratulations!** You've completed your first session.

---

## Common Tasks

### Creating Multiple ROIs

**Method 1: Add Menu**
1. Use **ROI** menu to add different shapes
2. Each appears at origin by default
3. Move them apart using Position sliders

**Method 2: Duplicate**
1. Select an existing ROI
2. Click **Duplicate** button
3. The copy appears offset by (1, 1, 1)
4. Adjust position as needed

### Organizing ROIs

**Hide/Show ROIs:**
- Click the checkbox next to the ROI name in the list
- Unchecked = hidden, Checked = visible

**Rename ROIs:**
1. Select ROI
2. Edit the **Name** field in Properties
3. Press Enter

**Delete ROIs:**
1. Select ROI
2. Click **Remove** button
3. Confirm deletion

### Viewing Options

**Preset Camera Views:**
- **View** → **Front View** (`F`)
- **View** → **Top View** (`T`)
- **View** → **Right View** (`R`)
- **View** → **Isometric View** (`I`)

**Toggle Display Elements:**
- **View** → **Show Grid** - Toggle ground grid
- **View** → **Show Axes** - Toggle coordinate axes

**Reset Camera:**
- **View** → **Reset Camera** (`Ctrl+R`)
- Returns to default position

### Measuring ROIs

**Check Volume:**
1. Select an ROI
2. Look at **Volume** in Properties panel
3. Updates automatically as you resize

**Measure Distance:**
1. Select first ROI
2. **Tools** → **Measure Distance**
3. Click second ROI
4. Distance shown in dialog

**Find Overlaps:**
1. **Tools** → **Check Overlaps**
2. View list of overlapping ROI pairs

### Loading Example Scenes

**Open Existing Scene:**
1. **File** → **Open** (`Ctrl+O`)
2. Browse to .roiscene file
3. Click **Open**
4. Scene loads, replacing current ROIs

**Note**: Always save current work first!

---

## Keyboard Shortcuts Reference

### Most Useful Shortcuts

| Action | Shortcut |
|--------|----------|
| Add Box | `Ctrl+B` |
| Add Sphere | `Ctrl+S` |
| Add Cylinder | `Ctrl+Y` |
| Save Scene | `Ctrl+S` |
| Open Scene | `Ctrl+O` |
| New Scene | `Ctrl+N` |
| Reset Camera | `Ctrl+R` |
| Delete ROI | `Delete` |

### View Shortcuts

| Action | Shortcut |
|--------|----------|
| Front View | `F` |
| Top View | `T` |
| Right View | `R` |
| Isometric | `I` |
| Toggle Grid | `G` |
| Toggle Axes | `A` |

---

## Tips for Beginners

### Understanding the Coordinate System

- **X Axis (Red)**: Left (-) to Right (+)
- **Y Axis (Green)**: Down (-) to Up (+)
- **Z Axis (Blue)**: Away (-) to Toward (+)

The origin (0, 0, 0) is where all three axes meet.

### Camera Control Best Practices

1. **Start Wide**: Zoom out to see the whole scene
2. **Orient First**: Use preset views (Front, Top, etc.)
3. **Then Orbit**: Fine-tune with mouse drag
4. **Reset Often**: Press `Ctrl+R` if you get lost

### Working Efficiently

1. **Use Keyboard Shortcuts**: Much faster than menus
2. **Save Frequently**: Use `Ctrl+S` often
3. **Name Your ROIs**: Makes selection easier later
4. **Use Colors**: Color-code by type or priority
5. **Start Simple**: Begin with boxes, add complexity later

### Avoiding Common Mistakes

**Problem**: ROI disappears after resizing
- **Cause**: Size set too small (< 0.1)
- **Fix**: Increase size sliders

**Problem**: Can't see ROI after creation
- **Cause**: Camera inside the ROI
- **Fix**: Zoom out with mouse wheel

**Problem**: ROI won't move
- **Cause**: ROI is locked
- **Fix**: Uncheck "Locked" in Properties

**Problem**: Sliders don't work
- **Cause**: No ROI selected
- **Fix**: Click ROI name in list

---

## Example Workflows

### Workflow 1: Medical Tumor Annotation

```
1. Create base anatomy ROI (box)
2. Add tumor ROI (sphere)
3. Set tumor color to red
4. Adjust size to match actual measurement
5. Position accurately using sliders
6. Add metastases as additional spheres
7. Use different colors for each
8. Measure volumes
9. Save as "Patient_001.roiscene"
```

### Workflow 2: Engineering Collision Zones

```
1. Create machine part ROIs (boxes/cylinders)
2. Position at actual locations
3. Create safety zone (large transparent sphere)
4. Set safety zone opacity to 30%
5. Check for overlaps
6. Adjust sizes to eliminate conflicts
7. Lock final positions
8. Export for simulation
```

### Workflow 3: Scientific Sampling Grid

```
1. Create first sample ROI (small box)
2. Duplicate to create grid
3. Adjust positions to regular spacing
4. Color-code by priority
5. Add exclusion zones (spheres)
6. Calculate total sample volume
7. Save grid template
```

---

## Next Steps

Now that you're familiar with the basics:

1. **Read the User Guide** - Comprehensive feature documentation
2. **Explore Advanced Features** - Wireframe mode, layers, measurements
3. **Try Complex Scenes** - Create 10+ ROI scenes
4. **Learn Shortcuts** - Memorize common keyboard shortcuts
5. **Read Technical Docs** - If you want to extend the application

---

## Getting Help

### Documentation

- **USER_GUIDE.md** - Detailed feature documentation
- **TECHNICAL_DOCUMENTATION.md** - Developer documentation
- **README.md** - Project overview

### Troubleshooting

**Application won't start:**
1. Verify .NET 9.0 is installed
2. Check system requirements
3. Try running from command line to see errors

**Performance is slow:**
1. Reduce number of visible ROIs
2. Hide complex shapes (torus)
3. Close other applications
4. Update graphics drivers

**File won't open:**
1. Check file extension (.roiscene)
2. Verify file is not corrupted
3. Try opening in text editor to check JSON

### Support

- **Issues**: Report bugs on GitHub
- **Questions**: Check discussions forum
- **Email**: [Your Support Email]

---

## Resources

### Learning Resources

- WPF 3D Tutorial: [Link to WPF-3D-Masterclass-100Hours]
- .NET Documentation: https://docs.microsoft.com/dotnet
- 3D Graphics Basics: [Link to tutorials]

### Related Projects

- **WPF-3D-ROI-Examples**: Simple examples (Easy/Middle/Hard)
- **WPF-3D-Masterclass-100Hours**: Full course curriculum

### Tools

- **Visual Studio 2022**: IDE for development
- **.NET 9.0 SDK**: Development kit
- **Git**: Version control

---

## Changelog

### Version 1.0 (Initial Release)
- Core ROI management (add, remove, duplicate)
- 7 ROI shape types
- Camera controls (orbit, pan, zoom)
- Property editing (position, size, rotation, color)
- File operations (save, load)
- Volume and surface area calculations
- Distance measurement
- Overlap detection
- Dark theme UI
- Grid and axes display

---

## What's Next?

Planned features for future releases:

- **Multi-viewport** (Front/Top/Side + 3D)
- **Undo/Redo** support
- **Clipping planes** for cross-sections
- **Boolean operations** (Union, Intersect, Subtract)
- **Export to STL/OBJ** for 3D printing
- **Animation** support
- **Scripting API** (Python/C#)
- **Plugin system** for extensions
- **Collaborative editing** (multi-user)

---

**Happy ROI Viewing!**

For more information, see the complete **USER_GUIDE.md** and **TECHNICAL_DOCUMENTATION.md**.

---

**Version**: 1.0
**Last Updated**: 2025
**Framework**: .NET 9.0
