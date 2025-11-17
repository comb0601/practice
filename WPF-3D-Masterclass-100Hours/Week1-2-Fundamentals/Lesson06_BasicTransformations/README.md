# Lesson 06: Basic Transformations

**Duration:** 2 hours
**Difficulty:** Intermediate ⭐⭐
**Prerequisites:** Lessons 1-5

---

## 🎯 Learning Objectives

By the end of this lesson, you will:
- ✅ Master TranslateTransform3D for moving objects
- ✅ Understand RotateTransform3D for spinning objects
- ✅ Use ScaleTransform3D for resizing
- ✅ Combine transforms using Transform3DGroup
- ✅ Understand transform order importance
- ✅ Apply transforms to create complex motions

---

## 📚 Theory (30 minutes)

### What are Transformations?

Transformations change the **position, rotation, or size** of 3D objects without modifying the mesh itself. Think of it like moving a chess piece vs carving a new piece.

### The Three Basic Transforms

#### 1. TranslateTransform3D - MOVE
Moves objects in 3D space along X, Y, Z axes.

```csharp
var translate = new TranslateTransform3D(2, 0, 0);
// Moves object 2 units right
```

#### 2. RotateTransform3D - SPIN
Rotates objects around an axis.

```csharp
var rotate = new RotateTransform3D(
    new AxisAngleRotation3D(new Vector3D(0, 1, 0), 45)
);
// Rotates 45° around Y-axis (vertical spin)
```

#### 3. ScaleTransform3D - RESIZE
Changes the size of objects.

```csharp
var scale = new ScaleTransform3D(2, 2, 2);
// Makes object 2x larger in all dimensions
```

### Transform Order Matters!

**Order affects the result:**

```csharp
// Option A: Rotate THEN Translate
// Object spins, then moves right
Transform3DGroup groupA = new Transform3DGroup();
groupA.Children.Add(rotate);
groupA.Children.Add(translate);

// Option B: Translate THEN Rotate
// Object moves right, then spins around origin (orbits!)
Transform3DGroup groupB = new Transform3DGroup();
groupB.Children.Add(translate);
groupB.Children.Add(rotate);
```

**Think of it like:**
- Option A: Spin a wheel, then push it
- Option B: Push a wheel away, then spin it around you

### Local vs World Space

- **Local Space:** Relative to the object itself
- **World Space:** Relative to the scene origin (0,0,0)

Transforms work in **local space first**, then affect children in their local space.

---

## 💻 Code Walkthrough (60 minutes)

### The Demo Application

This lesson's solution shows:
- A cube with three transform sliders
- Real-time transformation
- Visual difference between transform orders
- Multiple objects with different transforms

### Key Code Sections

#### Creating Transform Controls

```xaml
<Slider x:Name="translateX" Minimum="-5" Maximum="5"
        ValueChanged="Transform_Changed"/>
```

Sliders directly control transform values.

#### Applying Transforms

```csharp
var transformGroup = new Transform3DGroup();

// Add translation
transformGroup.Children.Add(new TranslateTransform3D(
    translateX.Value,
    translateY.Value,
    translateZ.Value
));

// Add rotation
transformGroup.Children.Add(new RotateTransform3D(
    new AxisAngleRotation3D(
        new Vector3D(0, 1, 0),
        rotationAngle.Value
    )
));

// Apply to model
geometryModel.Transform = transformGroup;
```

#### Continuous Rotation

```csharp
// In update loop
currentAngle += deltaTime * rotationSpeed;
rotation.Angle = currentAngle;
```

---

## 🏃 Running the Code (10 minutes)

```bash
cd Solution
dotnet run
```

**What You'll See:**
- A cube in the center
- Sliders for X, Y, Z movement
- Rotation controls
- Scale sliders
- Real-time updates

**Try This:**
1. Move X slider → Cube moves horizontally
2. Rotate Y slider → Cube spins
3. Scale up → Cube grows
4. Combine all → See complex motion!

---

## 🎨 Experiments (40 minutes)

### Experiment 1: Orbital Motion

Make an object orbit around a center point:

```csharp
// Animation loop
double time = 0;
void Update()
{
    time += 0.01;

    // Create orbit using transform order
    var group = new Transform3DGroup();

    // 1. Move away from center
    group.Children.Add(new TranslateTransform3D(3, 0, 0));

    // 2. Rotate around origin (creates orbit!)
    group.Children.Add(new RotateTransform3D(
        new AxisAngleRotation3D(new Vector3D(0, 1, 0), time * 50)
    ));

    model.Transform = group;
}
```

### Experiment 2: Planet with Moon

Create a planet with an orbiting moon using hierarchy:

```csharp
// Planet
var planet = CreateSphere();
planet.Transform = new RotateTransform3D(...); // Spin

// Moon (child of planet)
var moonVisual = new ModelVisual3D();
var moonGroup = new Transform3DGroup();
moonGroup.Children.Add(new TranslateTransform3D(2, 0, 0)); // Away
moonGroup.Children.Add(new RotateTransform3D(...)); // Orbit
moonVisual.Transform = moonGroup;

planetVisual.Children.Add(moonVisual); // Moon follows planet!
```

### Experiment 3: Pulsing Effect

Make object grow and shrink:

```csharp
double pulseTime = 0;
void Update()
{
    pulseTime += 0.1;
    double scale = 1 + 0.3 * Math.Sin(pulseTime);

    model.Transform = new ScaleTransform3D(scale, scale, scale);
}
```

### Experiment 4: Figure-8 Motion

Complex motion using math:

```csharp
double t = 0;
void Update()
{
    t += 0.02;

    double x = Math.Sin(t) * 3;
    double y = Math.Sin(t * 2) * 2;

    model.Transform = new TranslateTransform3D(x, y, 0);
}
```

---

## 💪 Challenge (30 minutes)

### Build a Spinning Top

Create a spinning top that:
1. **Spins** rapidly around its vertical axis
2. **Precesses** (tilts in a circle)
3. **Slows down** over time (optional)

**Requirements:**
- Use multiple RotateTransform3D
- Combine at least 2 axes of rotation
- Animate continuously
- Add visual trail (bonus)

**Hints:**
- Main spin: Fast rotation around Y-axis
- Precession: Slow rotation of tilt angle
- Use Transform3DGroup with proper order
- Update both rotations in animation loop

### Bonus: Mechanical Arm

Create a 3-segment robot arm:
- Base rotates horizontally
- Middle joint bends vertically
- End effector rotates

Use hierarchical transforms so each segment affects those after it.

---

## 🐛 Troubleshooting

### Object disappears when I transform
- **Check transform values** - Too large?
- **Camera position** - Can you still see it?
- **Scale to 0?** - Don't set scale to zero

### Rotation looks wrong
- **Check axis** - X, Y, or Z?
- **Check angle** - Degrees, not radians
- **Right-hand rule** - Thumb=axis, fingers=rotation direction

### Transforms don't combine as expected
- **Order matters!** - Try reversing order
- **Check pivot point** - Where is (0,0,0) for the object?

### Animation is choppy
- **Use DispatcherTimer** - Not Thread.Sleep
- **Smaller time steps** - More frequent updates
- **Interpolation** - Smooth between values

---

## 📖 Key Takeaways

✅ **Three transforms:** Translate (move), Rotate (spin), Scale (resize)
✅ **Order matters:** Different order = different result
✅ **Transform3DGroup:** Combines multiple transforms
✅ **Hierarchy:** Children inherit parent transforms
✅ **Animation:** Update transforms over time for motion
✅ **Math helps:** Sin/Cos create smooth, natural motion

---

## 🧮 Math Corner

### Rotation Axes

**Around X-axis:** Pitch (nod yes)
**Around Y-axis:** Yaw (shake no)
**Around Z-axis:** Roll (tilt head)

### Degrees vs Radians

- WPF uses **degrees** for AxisAngleRotation3D
- 360° = full circle
- 180° = half turn
- 90° = quarter turn

### Sine Wave Motion

```csharp
y = amplitude * Math.Sin(time * frequency)
```

- Amplitude: How far it moves
- Frequency: How fast it oscillates
- Creates smooth back-and-forth motion

---

## 🎯 Next Lesson

**Lesson 07: User Input Basics** - Make it interactive!

Topics:
- Mouse events in 3D
- Keyboard controls
- WASD navigation
- Dragging objects
- Interactive transformations

---

## 📚 Additional Resources

- [Transform3D Class](https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.media3d.transform3d)
- [AxisAngleRotation3D](https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.media3d.axisanglerotation3d)
- [Understanding 3D Rotations](https://en.wikipedia.org/wiki/Rotation_matrix)

---

## ✅ Checklist

Before moving to Lesson 07:
- [ ] Ran the solution successfully
- [ ] Moved objects with TranslateTransform3D
- [ ] Rotated objects around different axes
- [ ] Scaled objects larger and smaller
- [ ] Combined multiple transforms
- [ ] Understood transform order importance
- [ ] Created an orbiting object
- [ ] Attempted the spinning top challenge

**Ready for interactivity! 🎮**
