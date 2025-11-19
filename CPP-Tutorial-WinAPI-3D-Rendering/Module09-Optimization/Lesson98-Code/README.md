# Lesson 98: Multithreaded-Rendering

## Overview
This lesson contains 15 complete, executable C++ programs demonstrating:
- Multithreaded Rendering
- Progressive complexity from basics to advanced
- Real-world examples and best practices

## Files (15 total)

1. **01_RenderingArchitecture.cpp** - Renderingarchitecture\n2. **02_JobSystem.cpp** - Jobsystem\n3. **03_TaskParallelism.cpp** - Taskparallelism\n4. **04_DeferredContexts.cpp** - Deferredcontexts\n5. **05_CommandLists.cpp** - Commandlists\n6. **06_ParallelSubmission.cpp** - Parallelsubmission\n7. **07_SynchronizationPrimitives.cpp** - Synchronizationprimitives\n8. **08_LockFree.cpp** - Lockfree\n9. **09_ThreadSafety.cpp** - Threadsafety\n10. **10_DataRaces.cpp** - Dataraces\n11. **11_LoadBalancing.cpp** - Loadbalancing\n12. **12_ThreadPooling.cpp** - Threadpooling\n13. **13_WorkStealing.cpp** - Workstealing\n14. **14_ProfilerIntegration.cpp** - Profilerintegration\n15. **15_ScalabilityTesting.cpp** - Scalabilitytesting\n

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
