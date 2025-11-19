# Lesson 90: PBR-Rendering

## Overview
This lesson contains 15 complete, executable C++ programs demonstrating:
- PBR Rendering
- Progressive complexity from basics to advanced
- Real-world examples and best practices

## Files (15 total)

1. **01_MetallicWorkflow.cpp** - Metallicworkflow\n2. **02_SpecularWorkflow.cpp** - Specularworkflow\n3. **03_BaseColor.cpp** - Basecolor\n4. **04_Metallic.cpp** - Metallic\n5. **05_Roughness.cpp** - Roughness\n6. **06_CookTorrance.cpp** - Cooktorrance\n7. **07_GGXDistribution.cpp** - Ggxdistribution\n8. **08_SchlickFresnel.cpp** - Schlickfresnel\n9. **09_SmithGeometry.cpp** - Smithgeometry\n10. **10_ImageBasedLighting.cpp** - Imagebasedlighting\n11. **11_DiffuseIrradiance.cpp** - Diffuseirradiance\n12. **12_SpecularIBL.cpp** - Specularibl\n13. **13_ClearCoat.cpp** - Clearcoat\n14. **14_Subsurface.cpp** - Subsurface\n15. **15_PBROptimization.cpp** - Pbroptimization\n

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
