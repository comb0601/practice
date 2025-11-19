# Lesson 82: Deferred-Rendering

## Overview
This lesson contains 15 complete, executable C++ programs demonstrating:
- Deferred Rendering
- Progressive complexity from basics to advanced
- Real-world examples and best practices

## Files (15 total)

1. **01_GBuffer.cpp** - Gbuffer\n2. **02_GeometryPass.cpp** - Geometrypass\n3. **03_LightingPass.cpp** - Lightingpass\n4. **04_AlbedoBuffer.cpp** - Albedobuffer\n5. **05_NormalBuffer.cpp** - Normalbuffer\n6. **06_PositionBuffer.cpp** - Positionbuffer\n7. **07_MultipleRenderTargets.cpp** - Multiplerendertargets\n8. **08_LightAccumulation.cpp** - Lightaccumulation\n9. **09_TiledDeferred.cpp** - Tileddeferred\n10. **10_ClusteredDeferred.cpp** - Clustereddeferred\n11. **11_DeferredDecals.cpp** - Deferreddecals\n12. **12_TransparencyInDeferred.cpp** - Transparencyindeferred\n13. **13_ReconstructPosition.cpp** - Reconstructposition\n14. **14_GBufferOptimization.cpp** - Gbufferoptimization\n15. **15_BandwidthReduction.cpp** - Bandwidthreduction\n

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
