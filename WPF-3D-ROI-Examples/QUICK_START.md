# 🚀 QUICK START GUIDE

## For Absolute Beginners

### Step 1: Check Prerequisites

Open PowerShell or Command Prompt and run:
```bash
dotnet --version
```

If you see a version number (like `6.0.xxx` or `7.0.xxx`), you're good!

If not, download .NET from: https://dotnet.microsoft.com/download

---

### Step 2: Choose Your Level

#### I'm a complete beginner → Start with **EASY**
```bash
cd 1_Easy_ROI_3D
dotnet run
```

#### I have some C# experience → Try **MIDDLE**
```bash
cd 2_Middle_ROI_3D
dotnet run
```

#### I'm ready for production code → Jump to **HARD**
```bash
cd 3_Hard_ROI_3D
dotnet run
```

---

### Step 3: Explore the UI

**Easy Version:**
1. Move the sliders → See the 3D box move and resize
2. Rotate the camera → View from different angles
3. Change colors → Pick your favorite

**Middle Version:**
1. Click "+ Add ROI" → Create multiple regions
2. Select from list → Edit each one individually
3. Try different shapes → Box, Sphere, Cylinder
4. Rotate the ROIs → Use rotation sliders

**Hard Version:**
1. Drag with mouse → Rotate camera freely
2. Scroll mouse wheel → Zoom in/out
3. Add multiple ROIs → Build complex scenes
4. Save your work → File → Save Scene
5. Try all shapes → Box, Sphere, Cylinder, Cone, Torus

---

### Step 4: Read the Code

Open the files in your favorite editor:

**Visual Studio Code:**
```bash
code .
```

**Visual Studio:**
- Open the `.csproj` file

**Any Text Editor:**
- Just browse the folder and open `.cs` and `.xaml` files

---

## 🎯 Your First Modification

### Easy Challenge: Change Default Color

**File:** `1_Easy_ROI_3D/MainWindow.xaml.cs`

**Find this line (~17):**
```csharp
private Color roiColor = Color.FromArgb(128, 255, 0, 0); // Red
```

**Change to green:**
```csharp
private Color roiColor = Color.FromArgb(128, 0, 255, 0); // Green
```

**Run again:**
```bash
dotnet run
```

You did it! 🎉

---

## 💡 Understanding the Files

### Every project has:

```
AppName.csproj     ← Project settings (don't touch for now)
App.xaml          ← Application startup (rarely modified)
App.xaml.cs       ← Application code (rarely modified)
MainWindow.xaml    ← UI DESIGN (what you see)
MainWindow.xaml.cs ← UI LOGIC (how it works)
ROI.cs            ← Data structure (Middle/Hard only)
```

### What to study:

1. **MainWindow.xaml** - Learn how UI is built
2. **MainWindow.xaml.cs** - Learn how 3D works
3. **ROI.cs** - Learn how data is organized

---

## 🐛 Common Issues

### "dotnet: command not found"
→ Install .NET SDK from Microsoft

### "The application did not start"
→ Make sure you're in the correct folder
→ Run `dotnet build` first, then `dotnet run`

### "The 3D scene is empty/black"
→ Check if you have proper graphics drivers
→ Try running on a different computer

### "Too many errors, I'm confused"
→ Start with Easy version
→ Read one file at a time
→ Don't rush!

---

## 📚 Learning Order

### Day 1: Easy Version
- [ ] Run the application
- [ ] Play with all controls
- [ ] Read `MainWindow.xaml` (UI structure)
- [ ] Read `MainWindow.xaml.cs` (3D logic)
- [ ] Change the default color
- [ ] Change the grid size

### Day 2: Middle Version
- [ ] Run the application
- [ ] Create 5 different ROIs
- [ ] Try all shapes
- [ ] Read `ROI.cs` (data structure)
- [ ] Compare code with Easy version
- [ ] Add a new color option

### Day 3: Hard Version
- [ ] Run the application
- [ ] Try all menu options
- [ ] Save and load a scene
- [ ] Read the complete code
- [ ] Understand mouse camera control
- [ ] Try clipping plane feature

### Week 2: Build Your Own
- [ ] Create new project
- [ ] Copy concepts you learned
- [ ] Add your own features
- [ ] Share with others!

---

## 🎮 Controls Cheat Sheet

### EASY
- Sliders = Control ROI
- Reset = Start over

### MIDDLE
- + = Add ROI
- - = Remove ROI
- Click list = Select ROI
- Sliders = Edit selected

### HARD
- Drag mouse = Rotate view
- Scroll wheel = Zoom
- +/- buttons = Add/Remove
- ⎘ = Duplicate
- Ctrl+S = Save (coming soon!)
- Checkboxes = Show/Hide

---

## 🏆 Your Learning Path

```
EASY (1-2 hours)
  ↓
Understand basic 3D
  ↓
MIDDLE (2-4 hours)
  ↓
Learn object management
  ↓
HARD (4-8 hours)
  ↓
Master professional patterns
  ↓
BUILD YOUR OWN!
```

---

## 🎯 What You'll Learn

### From EASY:
✅ WPF 3D basics
✅ Camera positioning
✅ Creating meshes
✅ Materials and colors
✅ Transformations

### From MIDDLE:
✅ Managing multiple objects
✅ Different mesh types
✅ Object-oriented design
✅ UI-data binding
✅ Rotation transforms

### From HARD:
✅ Mouse interaction
✅ File I/O (save/load)
✅ Complex mesh generation
✅ Professional UI patterns
✅ Advanced features

---

## 💬 Pro Tips

1. **Don't rush** - Understand Easy before Middle
2. **Type, don't copy** - Typing helps learning
3. **Break things** - Best way to learn
4. **Use debugger** - Step through code
5. **Ask questions** - Search online, read docs
6. **Build projects** - Apply what you learned

---

## 🚦 You're Ready When...

### Ready for Middle?
- [x] Easy app runs successfully
- [x] You understand Viewport3D
- [x] You can create a simple mesh
- [x] You modified something successfully

### Ready for Hard?
- [x] Middle app runs successfully
- [x] You understand ROI class
- [x] You can add new shapes
- [x] You understand transforms

### Ready to Build Your Own?
- [x] Hard app runs successfully
- [x] You understand all 3 versions
- [x] You've modified all versions
- [x] You have an idea for a project

---

**NOW GO CODE! 🚀**

Remember: Every expert was once a beginner. You got this! 💪
