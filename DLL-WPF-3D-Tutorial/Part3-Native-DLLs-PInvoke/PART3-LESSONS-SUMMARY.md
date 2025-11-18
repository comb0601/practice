# Part 3: Native DLLs & P/Invoke - Lessons 11-15

This part covers interoperability between C# and native (C/C++) code.

## Lessons in This Part

### Lesson 11: Understanding Native vs Managed
- CLR and managed code architecture
- Native (unmanaged) code overview  
- The interop boundary and performance
- When to use native DLLs
- COM interop basics

### Lesson 12: P/Invoke Basics
- DllImport attribute fundamentals
- Calling Windows API functions
- Basic type marshaling (int, bool, string)
- Entry point and calling conventions
- Error handling with GetLastError

### Lesson 13: Marshaling Data Types
- Simple types (integers, floats, booleans)
- Strings and character encoding (ANSI, Unicode)
- Arrays and pointers
- Structures and classes
- Callbacks and function pointers
- Custom marshaling attributes

### Lesson 14: Creating C++ DLLs for C#
- Setting up C++ DLL project in Visual Studio
- __declspec(dllexport) and extern "C"
- Creating C-compatible APIs
- Build configurations (x86/x64)
- Practical example: Math library in C++

### Lesson 15: Advanced P/Invoke
- SafeHandle and resource management
- Platform-specific code (#if/#elif)
- Blittable vs non-blittable types
- Performance optimization tips
- Common pitfalls and solutions
- Debugging native crashes

## Learning Path

These lessons build on each other:
1. Start with Lesson 11 to understand the concepts
2. Practice P/Invoke with Lesson 12
3. Master marshaling with Lesson 13
4. Create your own native DLL in Lesson 14  
5. Apply advanced techniques from Lesson 15

## Prerequisites

- Completed Part 1 and Part 2
- Basic C++ knowledge (helpful but not required)
- Visual Studio with C++ workload installed

## Projects

By the end of this part, you'll be able to:
- Call any Windows API function from C#
- Create high-performance C++ DLLs
- Integrate existing C++ libraries into C# apps
- Build cross-language systems

---

**Start with:** [Lesson 11: Understanding Native vs Managed](Lesson11-Understanding-Native-vs-Managed.md)
