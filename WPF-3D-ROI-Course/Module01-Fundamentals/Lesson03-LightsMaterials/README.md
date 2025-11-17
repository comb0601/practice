# Lesson 1.3 - Lights and Materials

**Duration:** 2 hours
**Difficulty:** Beginner

## Overview

Master the art of lighting and material properties in WPF 3D. This lesson explores different light types and how materials respond to lighting, essential for creating realistic 3D scenes.

## Learning Objectives

- Understand different light types in WPF 3D
- Learn material properties: Diffuse, Specular, Emissive
- Combine materials for complex appearances
- Control light intensity and positioning
- Create realistic surface appearances

## Key Concepts

### Light Types

**1. AmbientLight**
- Uniform illumination from all directions
- No shadows or highlights
- Provides base lighting level
```csharp
var ambient = new AmbientLight(Color.FromRgb(32, 32, 32));
```

**2. DirectionalLight**
- Parallel light rays (like sunlight)
- Direction but no position
- Creates consistent shadows
```csharp
var directional = new DirectionalLight(Colors.White, new Vector3D(-1, -1, -1));
```

**3. PointLight**
- Radiates from a single point
- Has position and attenuation (falloff)
- Like a light bulb
```csharp
var point = new PointLight(Colors.Red, new Point3D(3, 2, 3));
point.Range = 15;
point.ConstantAttenuation = 1;
point.LinearAttenuation = 0.1;
point.QuadraticAttenuation = 0.01;
```

**4. SpotLight**
- Cone-shaped light
- Has position, direction, and cone angle
- Like a flashlight
```csharp
var spot = new SpotLight(Colors.Yellow, new Point3D(0, 5, 0), new Vector3D(0, -1, 0), 40, 30);
```

### Material Types

**1. DiffuseMaterial**
- Matte, non-shiny surface
- Scatters light evenly
- Most common material type
```csharp
var diffuse = new DiffuseMaterial(new SolidColorBrush(Colors.Blue));
```

**2. SpecularMaterial**
- Shiny highlights
- Reflects light directly
- SpecularPower controls shininess (1-200+)
```csharp
var specular = new SpecularMaterial(Brushes.White, 100);
```

**3. EmissiveMaterial**
- Self-illuminated appearance
- Not affected by lights
- Doesn't actually emit light
```csharp
var emissive = new EmissiveMaterial(new SolidColorBrush(Colors.Orange));
```

**4. MaterialGroup**
- Combines multiple materials
```csharp
var group = new MaterialGroup();
group.Children.Add(new DiffuseMaterial(Brushes.Blue));
group.Children.Add(new SpecularMaterial(Brushes.White, 50));
```

## Running the Application

1. Build and run:
   ```bash
   dotnet build
   dotnet run
   ```

2. You'll see five spheres demonstrating different materials

3. Use the checkboxes to enable/disable lights

4. Adjust sliders to modify light and material properties

## Exercises

### Exercise 1: Lighting Scenarios

Create these lighting scenarios and observe the results:

**Scenario 1: Sunset**
- Disable all lights except Directional
- Set Directional intensity low (0.3-0.5)
- Add an Ambient with warm orange tint

**Scenario 2: Studio Lighting**
- Enable all lights
- Ambient: Low (0.1)
- Directional: Medium (0.7)
- Point light: Positioned from side
- Spot light: From above

**Scenario 3: Night Scene**
- Only Point and Spot lights
- Very low Ambient
- Observe dramatic shadows

### Exercise 2: Create Custom Materials

Add a new sphere with a custom material:

```csharp
// Glass-like material
var glassMaterial = new MaterialGroup();
glassMaterial.Children.Add(new DiffuseMaterial(new SolidColorBrush(Color.FromArgb(100, 200, 220, 255))));
glassMaterial.Children.Add(new SpecularMaterial(Brushes.White, 200));

// Plastic material
var plasticMaterial = new MaterialGroup();
plasticMaterial.Children.Add(new DiffuseMaterial(Brushes.Red));
plasticMaterial.Children.Add(new SpecularMaterial(Brushes.White, 30));

// Rubber material
var rubberMaterial = new DiffuseMaterial(Brushes.Black);
// No specular - matte surface
```

### Exercise 3: Animated Light

Add a rotating point light:

```csharp
// In XAML:
<PointLight x:Name="rotatingLight" Color="Cyan" Position="3,2,0" Range="10">
    <PointLight.Transform>
        <RotateTransform3D>
            <RotateTransform3D.Rotation>
                <AxisAngleRotation3D x:Name="lightRotation" Axis="0,1,0" Angle="0"/>
            </RotateTransform3D.Rotation>
        </RotateTransform3D>
    </PointLight.Transform>
</PointLight>

// Animate it
<Storyboard x:Key="LightRotation" RepeatBehavior="Forever">
    <DoubleAnimation Storyboard.TargetName="lightRotation"
                     Storyboard.TargetProperty="Angle"
                     From="0" To="360" Duration="0:0:10"/>
</Storyboard>
```

## Understanding Light Attenuation

Point and Spot lights have attenuation (falloff):

```
Attenuation = 1 / (C + L*d + Q*d²)

Where:
C = ConstantAttenuation (base level)
L = LinearAttenuation (linear falloff)
Q = QuadraticAttenuation (squared falloff)
d = distance from light
```

**Realistic values:**
- **Room light**: C=1, L=0.1, Q=0.01
- **Bright light**: C=1, L=0.05, Q=0.005
- **Distant light**: C=0.5, L=0.05, Q=0

## Material Properties Deep Dive

### Specular Power

- **Low (1-10)**: Broad, dull highlights (plastic, rubber)
- **Medium (20-50)**: Semi-shiny (painted metal, wet surfaces)
- **High (100-200)**: Mirror-like (polished metal, glass)

### Material Combinations

```csharp
// Realistic wood
var wood = new DiffuseMaterial(new SolidColorBrush(Color.FromRgb(139, 90, 43)));

// Shiny metal
var metal = new MaterialGroup();
metal.Children.Add(new DiffuseMaterial(Brushes.Silver));
metal.Children.Add(new SpecularMaterial(Brushes.White, 150));

// Glowing object
var glow = new MaterialGroup();
glow.Children.Add(new DiffuseMaterial(Brushes.DarkBlue));
glow.Children.Add(new EmissiveMaterial(Brushes.Cyan));

// Rough stone
var stone = new MaterialGroup();
stone.Children.Add(new DiffuseMaterial(Brushes.Gray));
stone.Children.Add(new SpecularMaterial(Brushes.White, 5)); // Very low specular
```

## Common Issues

**Objects appear black:**
- Add an AmbientLight for base illumination
- Check DirectionalLight direction
- Ensure normals are calculated correctly

**No highlights visible:**
- Add SpecularMaterial
- Increase SpecularPower
- Adjust camera angle

**Everything looks flat:**
- Use multiple light sources
- Combine Ambient + Directional
- Add SpecularMaterial for depth

## Performance Considerations

- Each light increases rendering cost
- Use AmbientLight + 1-2 other lights for best performance
- Point and Spot lights are more expensive than Directional
- MaterialGroups add overhead - use sparingly

## Practical Applications

Understanding lights and materials is crucial for:
- Creating realistic 3D visualizations
- Medical imaging displays
- Product visualization
- Architectural renderings
- Scientific data visualization
- **ROI visualization** (upcoming modules)

## Next Steps

In **Lesson 1.4**, you'll learn about transformations - how to position, rotate, and scale objects in 3D space, including animation.

## Time Breakdown

- **30 min**: Understanding light types and properties
- **30 min**: Exploring material properties
- **45 min**: Hands-on experimentation
- **15 min**: Creating custom materials

## Key Takeaways

- WPF 3D supports 4 light types: Ambient, Directional, Point, Spot
- Materials define surface appearance: Diffuse (matte), Specular (shiny), Emissive (glowing)
- Combine materials using MaterialGroup for complex appearances
- SpecularPower controls shininess (1 = dull, 200+ = mirror)
- Use AmbientLight + DirectionalLight for most scenes
- Light attenuation controls falloff for Point and Spot lights

---

**Excellent progress!** Continue to **Lesson 1.4** to learn transformations and animations.
