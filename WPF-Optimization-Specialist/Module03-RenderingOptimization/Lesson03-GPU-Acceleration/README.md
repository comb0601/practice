# Lesson 03: GPU Acceleration

## Overview
This lesson explores GPU acceleration in WPF applications, covering hardware-accelerated rendering, shader effects, and 3D transformations. Learn how to leverage modern graphics hardware for maximum performance.

## Learning Objectives
- Understand GPU vs CPU rendering
- Master hardware-accelerated shader effects
- Implement 3D transformations
- Optimize for different render tiers
- Measure and improve GPU performance

## Key Concepts

### GPU vs CPU Rendering

#### CPU Rendering (Software)
- All rendering performed by processor
- Slower for complex visuals
- No special hardware required
- Limited effects support
- Higher CPU usage

#### GPU Rendering (Hardware)
- Graphics card handles rendering
- Much faster performance
- Full effects support
- Lower CPU usage
- Better for animations and complex visuals

### Render Tiers

WPF categorizes hardware into three render tiers:

#### Tier 0 - Software Rendering
```
Characteristics:
- No GPU acceleration
- CPU performs all rendering
- Slowest performance
- Fallback mode

When Used:
- No compatible graphics hardware
- Very old systems
- Remote desktop (sometimes)
- Virtual machines (sometimes)

Recommendations:
- Minimize visual complexity
- Avoid effects
- Simple layouts only
- Consider hardware upgrade
```

#### Tier 1 - Partial Hardware Acceleration
```
Characteristics:
- Basic GPU features
- Limited shader support
- Some DirectX capabilities
- Moderate performance

When Used:
- Older graphics cards
- Limited GPU capabilities
- Entry-level hardware

Recommendations:
- Moderate visual complexity
- Test effects carefully
- Provide fallbacks
- Monitor performance
```

#### Tier 2 - Full Hardware Acceleration
```
Characteristics:
- Complete GPU acceleration
- Full shader support
- DirectX 9+ compatible
- Best performance

When Used:
- Modern graphics cards
- Recent hardware
- Gaming/professional GPUs

Recommendations:
- Use full visual richness
- Leverage all effects
- 3D transformations OK
- Maximum performance
```

### Shader Effects

WPF provides GPU-accelerated shader effects:

#### BlurEffect
```csharp
var blurEffect = new BlurEffect
{
    Radius = 10  // 0-30, larger = more blur
};
element.Effect = blurEffect;
```

**Properties:**
- Radius: Blur amount (0-30)
- GPU-accelerated Gaussian blur
- Performance impact increases with radius

**Use Cases:**
- Background blur
- Focus effects
- Depth of field
- Visual softening

#### DropShadowEffect
```csharp
var shadowEffect = new DropShadowEffect
{
    Color = Colors.Black,
    BlurRadius = 15,
    ShadowDepth = 5,
    Direction = 315,
    Opacity = 0.5
};
element.Effect = shadowEffect;
```

**Properties:**
- Color: Shadow color
- BlurRadius: Shadow softness
- ShadowDepth: Distance from element
- Direction: Angle (0-360 degrees)
- Opacity: Transparency (0-1)

**Use Cases:**
- Depth and elevation
- Visual hierarchy
- Emphasis
- Realism

#### Custom Shader Effects
```csharp
// Create custom effects with HLSL
public class CustomEffect : ShaderEffect
{
    // Pixel shader compiled from HLSL
    private static PixelShader _pixelShader = new PixelShader
    {
        UriSource = new Uri("pack://application:,,,/CustomShader.ps")
    };

    public CustomEffect()
    {
        PixelShader = _pixelShader;
    }
}
```

### 3D Transformations

WPF supports GPU-accelerated 3D:

#### Viewport3D
```csharp
var viewport = new Viewport3D
{
    Camera = new PerspectiveCamera
    {
        Position = new Point3D(0, 0, 5),
        LookDirection = new Vector3D(0, 0, -5)
    }
};
```

#### 3D Transforms on 2D Elements
```xaml
<Border>
    <Border.LayoutTransform>
        <RotateTransform3D>
            <RotateTransform3D.Rotation>
                <AxisAngleRotation3D Axis="0,1,0" Angle="45"/>
            </RotateTransform3D.Rotation>
        </RotateTransform3D>
    </Border.LayoutTransform>
</Border>
```

**Benefits:**
- GPU-accelerated
- No layout recalculation
- Smooth animations
- Perspective effects

### Hardware Capabilities Detection

```csharp
// Check render tier
int tier = RenderCapability.Tier >> 16;

// Check max texture size
int maxSize = RenderCapability.MaxHardwareTextureSize;

// Check pixel shader support
int shaderSlots = RenderCapability.MaxPixelShaderInstructionSlots(0, 0);

// Is hardware accelerated?
bool isHardware = tier >= 1;
```

## Demonstrations

### 1. GPU Information
Display detailed hardware capabilities and render tier information.

**Shows:**
- Current render tier
- GPU acceleration status
- Hardware capabilities
- DirectX version
- Supported features

### 2. Hardware Acceleration Comparison
Side-by-side comparison of software vs hardware rendering.

**Demonstrates:**
- Performance difference
- Visual complexity impact
- Effect support
- CPU vs GPU usage

### 3. Shader Effects Gallery
Showcase of built-in GPU-accelerated effects.

**Features:**
- BlurEffect samples
- DropShadowEffect variations
- Outer glow effects
- Combined effects
- Performance characteristics

### 4. Interactive Blur Effect
Real-time blur effect with adjustable parameters.

**Controls:**
- Blur radius (0-30)
- Live preview
- Performance monitoring
- GPU acceleration demonstration

### 5. Interactive Drop Shadow
Adjustable drop shadow with multiple parameters.

**Controls:**
- Blur radius
- Shadow depth
- Direction
- Opacity
- Real-time updates

### 6. 3D Transforms
GPU-accelerated 3D transformations demonstration.

**Features:**
- 3D viewport
- Rotating cube
- Smooth animation
- GPU rendering
- No layout impact

### 7. GPU Performance Test
Stress test with many GPU-accelerated elements.

**Tests:**
- 200 animated particles
- Shader effects on each
- FPS monitoring
- Performance metrics
- Hardware capability validation

## Performance Optimization

### Effect Optimization

**DO:**
```csharp
// Use effects when hardware accelerated
if (RenderCapability.Tier >= 0x00020000)
{
    element.Effect = new BlurEffect { Radius = 10 };
}

// Cache with BitmapCache for static content
element.CacheMode = new BitmapCache();
element.Effect = new DropShadowEffect();
```

**DON'T:**
```csharp
// Avoid excessive effect stacking
element.Effect = new BlurEffect();
// Multiple effects not directly supported

// Don't use huge blur radii
element.Effect = new BlurEffect { Radius = 100 }; // Too large!
```

### Transform Optimization

**DO:**
```csharp
// Use RenderTransform (GPU-accelerated, no layout)
element.RenderTransform = new RotateTransform(45);

// 3D transforms for perspective
element.LayoutTransform = new RotateTransform3D();
```

**DON'T:**
```csharp
// Avoid LayoutTransform for animations (causes layout)
element.LayoutTransform = new RotateTransform(45); // Layout pass!
```

### Render Tier Handling

```csharp
public void OptimizeForTier()
{
    int tier = RenderCapability.Tier >> 16;

    switch (tier)
    {
        case 0: // Software rendering
            // Minimize effects, simplify visuals
            DisableEffects();
            break;

        case 1: // Partial hardware
            // Use basic effects carefully
            UseBasicEffects();
            break;

        case 2: // Full hardware
            // Use all features
            UseFullEffects();
            break;
    }
}
```

## Building and Running

### Prerequisites
- .NET 9.0 SDK
- Windows OS
- DirectX 9+ compatible graphics card (for Tier 2)
- Updated graphics drivers
- Visual Studio 2022 or later (recommended)

### Build Instructions
```bash
# Navigate to lesson directory
cd Module03-RenderingOptimization/Lesson03-GPU-Acceleration

# Restore dependencies
dotnet restore

# Build the project
dotnet build

# Run the application
dotnet run
```

### Visual Studio
1. Open `GPUAcceleration.csproj`
2. Press F5 to build and run
3. Explore demonstrations

## Interactive Features

### Controls
- **GPU Information**: View hardware capabilities
- **HW Acceleration**: Compare CPU vs GPU rendering
- **Shader Effects**: Gallery of GPU effects
- **Blur Effect**: Interactive blur demonstration
- **Drop Shadow**: Adjustable shadow parameters
- **3D Transforms**: GPU-accelerated 3D rotation
- **Performance Test**: Stress test with 200 particles
- **Clear**: Reset demonstration area

### Metrics
- Render Tier (0, 1, or 2)
- GPU Acceleration Status
- Max Texture Size
- Pixel Shader Version
- FPS (Frames Per Second)
- Frame Time
- Element Count
- GPU Usage

## Advanced Topics

### Custom Shader Effects

Create custom effects with HLSL (High-Level Shader Language):

```hlsl
// CustomShader.fx
sampler2D input : register(S0);

float4 main(float2 uv : TEXCOORD) : COLOR
{
    float4 color = tex2D(input, uv);
    // Apply custom effect
    return color;
}
```

```csharp
// C# wrapper
public class CustomEffect : ShaderEffect
{
    public CustomEffect()
    {
        PixelShader = new PixelShader
        {
            UriSource = new Uri("pack://application:,,,/Shaders/CustomShader.ps")
        };
    }
}
```

### Visual Brush with GPU Acceleration

```csharp
var visualBrush = new VisualBrush
{
    Visual = complexElement,
    CacheMode = new BitmapCache() // GPU cache
};
```

### Hardware Acceleration Hints

```csharp
// Force render mode (use with caution)
RenderOptions.SetBitmapScalingMode(element, BitmapScalingMode.HighQuality);
RenderOptions.SetEdgeMode(element, EdgeMode.Aliased);
```

## Common Issues and Solutions

### Issue: Low Render Tier
**Solution**: Update graphics drivers, check hardware compatibility, ensure DirectX is installed

### Issue: Effects Not Working
**Solution**: Verify Tier 1+, check for software rendering mode, test on different hardware

### Issue: Poor Performance with Effects
**Solution**: Reduce effect parameters, use BitmapCache, limit number of effects, check render tier

### Issue: 3D Performance
**Solution**: Reduce polygon count, optimize textures, use simpler models, check GPU capabilities

## Best Practices Summary

1. **Check Render Tier**: Always verify hardware capabilities
2. **Optimize Effects**: Use appropriate effect parameters
3. **Cache When Possible**: Use BitmapCache for static content
4. **Prefer Transforms**: Use RenderTransform over layout changes
5. **Test Hardware**: Validate on target hardware configurations
6. **Provide Fallbacks**: Support lower render tiers
7. **Monitor Performance**: Track FPS and frame time
8. **Update Drivers**: Ensure latest graphics drivers

## Performance Checklist

- [ ] Verify Tier 2 rendering (if available)
- [ ] Use effects sparingly and appropriately
- [ ] Cache static content with BitmapCache
- [ ] Prefer RenderTransform over LayoutTransform
- [ ] Test on target hardware
- [ ] Monitor FPS and performance
- [ ] Provide fallbacks for lower tiers
- [ ] Update graphics drivers
- [ ] Optimize effect parameters
- [ ] Limit simultaneous effects

## Related Lessons
- **Lesson 01**: Visual Tree Optimization (Tree structure)
- **Lesson 02**: Rendering Pipeline (WPF internals)
- **Lesson 04**: Bitmap Caching (Render caching strategies)
- **Lesson 05**: Virtualization Techniques (Large collections)

## Additional Resources
- [WPF Graphics Rendering Overview](https://docs.microsoft.com/en-us/dotnet/desktop/wpf/graphics-multimedia/wpf-graphics-rendering-overview)
- [Graphics Rendering Tiers](https://docs.microsoft.com/en-us/dotnet/desktop/wpf/advanced/graphics-rendering-tiers)
- [Shader Effects](https://docs.microsoft.com/en-us/dotnet/desktop/wpf/graphics-multimedia/shader-effects)
- [3D Graphics Overview](https://docs.microsoft.com/en-us/dotnet/desktop/wpf/graphics-multimedia/3-d-graphics-overview)

## Exercise Challenges

1. **Challenge 1**: Create a custom shader effect using HLSL.

2. **Challenge 2**: Build an adaptive UI that optimizes based on render tier.

3. **Challenge 3**: Implement a particle system with GPU effects maintaining 60 FPS.

4. **Challenge 4**: Create a 3D card flip animation using GPU-accelerated transforms.

## Summary

GPU acceleration is crucial for high-performance WPF applications. By understanding render tiers, leveraging shader effects, and using 3D transformations effectively, you can create rich, smooth user interfaces. Always check hardware capabilities, optimize effect usage, and test on target hardware. With Tier 2 hardware, WPF can deliver stunning visuals with excellent performance through DirectX integration and GPU acceleration.
