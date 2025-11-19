# Lesson 96: Batch-Rendering

## Overview
This lesson contains 15 complete, executable C++ programs demonstrating:
- Batch Rendering
- Progressive complexity from basics to advanced
- Real-world examples and best practices

## Files (15 total)

1. **01_StaticBatching.cpp** - Staticbatching\n2. **02_DynamicBatching.cpp** - Dynamicbatching\n3. **03_GeometryInstancing.cpp** - Geometryinstancing\n4. **04_MaterialBatching.cpp** - Materialbatching\n5. **05_DrawCallBatching.cpp** - Drawcallbatching\n6. **06_SortingForBatching.cpp** - Sortingforbatching\n7. **07_ZBufferOptimization.cpp** - Zbufferoptimization\n8. **08_StateChangeReduction.cpp** - Statechangereduction\n9. **09_ShaderPermutations.cpp** - Shaderpermutations\n10. **10_UberShaders.cpp** - Ubershaders\n11. **11_BatchingLimitations.cpp** - Batchinglimitations\n12. **12_IndirectRendering.cpp** - Indirectrendering\n13. **13_MultiDrawIndirect.cpp** - Multidrawindirect\n14. **14_GPUDrivenRendering.cpp** - Gpudrivenrendering\n15. **15_BatchingPerformance.cpp** - Batchingperformance\n

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
