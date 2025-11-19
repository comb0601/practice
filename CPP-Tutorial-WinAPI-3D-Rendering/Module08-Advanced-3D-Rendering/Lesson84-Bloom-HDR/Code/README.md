# Lesson 84: Bloom-HDR

## Overview
This lesson contains 15 complete, executable C++ programs demonstrating:
- Bloom HDR
- Progressive complexity from basics to advanced
- Real-world examples and best practices

## Files (15 total)

1. **01_HDRRendering.cpp** - Hdrrendering\n2. **02_FloatingPointBuffers.cpp** - Floatingpointbuffers\n3. **03_BloomExtraction.cpp** - Bloomextraction\n4. **04_BrightPass.cpp** - Brightpass\n5. **05_DownSample.cpp** - Downsample\n6. **06_UpSample.cpp** - Upsample\n7. **07_BloomComposite.cpp** - Bloomcomposite\n8. **08_ToneMapping.cpp** - Tonemapping\n9. **09_ACES.cpp** - Aces\n10. **10_Reinhard.cpp** - Reinhard\n11. **11_Uncharted2.cpp** - Uncharted2\n12. **12_ExposureControl.cpp** - Exposurecontrol\n13. **13_AutoExposure.cpp** - Autoexposure\n14. **14_EyeAdaptation.cpp** - Eyeadaptation\n15. **15_Glare.cpp** - Glare\n

## Compilation

### Windows (Visual Studio)
```bat
cl /EHsc /Fe:output.exe filename.cpp d3d11.lib dxgi.lib
```

### Linux/Mac (GCC/Clang)
```bash
g++ -std=c++17 -O2 -o output filename.cpp
```

## Learning Path
1. Start with file 01 (basics)
2. Progress sequentially through numbered files
3. Each file builds on previous concepts
4. Run and modify code to understand concepts

## Additional Resources
- Official Microsoft DirectX Documentation
- Graphics Programming Black Book
- Real-Time Rendering (4th Edition)
- Game Engine Architecture

---
Generated for C++ Tutorial: WinAPI & 3D Rendering Course
