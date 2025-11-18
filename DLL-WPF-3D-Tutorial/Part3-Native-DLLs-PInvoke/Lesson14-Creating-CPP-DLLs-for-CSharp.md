# Lesson 14: Creating C++ DLLs for C#

## 📚 Learning Objectives

By the end of this lesson, you will:
- Set up C++ DLL projects in Visual Studio
- Export functions for C# consumption
- Use `extern "C"` and `__declspec(dllexport)`
- Handle different calling conventions
- Create C-compatible APIs
- Build for x86 and x64 platforms
- Debug C++ DLLs from C# applications

## 🎯 Why Create C++ DLLs?

**Use cases for C++ DLLs:**
- High-performance algorithms
- Legacy code integration
- Direct hardware access
- Existing C++ libraries
- Platform-specific features

## 🛠️ Setting Up C++ DLL Project

### Step 1: Create New Project

```
Visual Studio → New Project → Dynamic-Link Library (DLL)
Name: MathLibraryNative
Platform: Windows
Language: C++
```

### Step 2: Project Structure

```
MathLibraryNative/
├── MathLibraryNative.h      (Header file)
├── MathLibraryNative.cpp    (Implementation)
├── dllmain.cpp              (DLL entry point)
└── MathLibraryNative.vcxproj (Project file)
```

## 📝 Creating Exportable Functions

### Basic Function Export

**MathLibraryNative.h:**
```cpp
#pragma once

#ifdef MATHLIB_EXPORTS
#define MATHLIB_API __declspec(dllexport)
#else
#define MATHLIB_API __declspec(dllimport)
#endif

// C linkage to prevent name mangling
extern "C" {
    // Simple math functions
    MATHLIB_API int Add(int a, int b);
    MATHLIB_API int Subtract(int a, int b);
    MATHLIB_API int Multiply(int a, int b);
    MATHLIB_API double Divide(int a, int b);
    
    // Advanced functions
    MATHLIB_API double CalculatePi(int iterations);
    MATHLIB_API void ProcessArray(int* arr, int count);
}
```

**MathLibraryNative.cpp:**
```cpp
#include "pch.h"
#include "MathLibraryNative.h"
#include <cmath>

extern "C" {
    
    MATHLIB_API int Add(int a, int b)
    {
        return a + b;
    }
    
    MATHLIB_API int Subtract(int a, int b)
    {
        return a - b;
    }
    
    MATHLIB_API int Multiply(int a, int b)
    {
        return a * b;
    }
    
    MATHLIB_API double Divide(int a, int b)
    {
        if (b == 0)
            return 0.0;
        return static_cast<double>(a) / b;
    }
    
    MATHLIB_API double CalculatePi(int iterations)
    {
        double pi = 0.0;
        for (int i = 0; i < iterations; i++)
        {
            pi += std::pow(-1.0, i) / (2 * i + 1);
        }
        return pi * 4.0;
    }
    
    MATHLIB_API void ProcessArray(int* arr, int count)
    {
        for (int i = 0; i < count; i++)
        {
            arr[i] *= 2;  // Double each element
        }
    }
}
```

### Understanding `extern "C"`

```cpp
// Without extern "C":
// C++ compiler mangles names: Add → ?Add@@YAHHH@Z

// With extern "C":
// Simple name: Add (can be called from C#!)

extern "C" {
    __declspec(dllexport) int Add(int a, int b);
    // Exports as: Add
}
```

## 🏗️ Building the DLL

### Project Configuration

**Properties → C/C++ → Preprocessor:**
```
Preprocessor Definitions: MATHLIB_EXPORTS;%(PreprocessorDefinitions)
```

**Build for x64:**
```
Configuration Manager → Active Solution Platform → x64
Build → Build Solution
```

**Output:**
```
x64/Debug/MathLibraryNative.dll
x64/Debug/MathLibraryNative.lib (import library)
```

## 🔗 Using from C#

### C# P/Invoke Wrapper

```csharp
using System;
using System.Runtime.InteropServices;

namespace MathLibraryWrapper
{
    public static class NativeMath
    {
        const string DLL_NAME = "MathLibraryNative.dll";
        
        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int Add(int a, int b);
        
        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int Subtract(int a, int b);
        
        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int Multiply(int a, int b);
        
        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern double Divide(int a, int b);
        
        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern double CalculatePi(int iterations);
        
        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ProcessArray(
            [In, Out] int[] array,
            int count);
    }
    
    class Program
    {
        static void Main()
        {
            // Test basic operations
            Console.WriteLine($"5 + 3 = {NativeMath.Add(5, 3)}");
            Console.WriteLine($"5 - 3 = {NativeMath.Subtract(5, 3)}");
            Console.WriteLine($"5 * 3 = {NativeMath.Multiply(5, 3)}");
            Console.WriteLine($"5 / 3 = {NativeMath.Divide(5, 3):F2}");
            
            // Calculate Pi
            double pi = NativeMath.CalculatePi(1000000);
            Console.WriteLine($"Pi ≈ {pi}");
            
            // Process array
            int[] numbers = { 1, 2, 3, 4, 5 };
            NativeMath.ProcessArray(numbers, numbers.Length);
            Console.WriteLine($"Processed: {string.Join(", ", numbers)}");
        }
    }
}
```

## 📦 Working with Structures

### C++ Structure Definition

**ImageProcessor.h:**
```cpp
#pragma once

#ifdef IMAGEPROC_EXPORTS
#define IMAGEPROC_API __declspec(dllexport)
#else
#define IMAGEPROC_API __declspec(dllimport)
#endif

extern "C" {
    
    struct Color
    {
        unsigned char R;
        unsigned char G;
        unsigned char B;
        unsigned char A;
    };
    
    struct Image
    {
        int Width;
        int Height;
        Color* Pixels;
    };
    
    // Create/destroy image
    IMAGEPROC_API Image* CreateImage(int width, int height);
    IMAGEPROC_API void DestroyImage(Image* img);
    
    // Process image
    IMAGEPROC_API void FillImage(Image* img, Color color);
    IMAGEPROC_API void ConvertToGrayscale(Image* img);
}
```

**ImageProcessor.cpp:**
```cpp
#include "pch.h"
#include "ImageProcessor.h"
#include <cstring>

extern "C" {
    
    IMAGEPROC_API Image* CreateImage(int width, int height)
    {
        Image* img = new Image;
        img->Width = width;
        img->Height = height;
        img->Pixels = new Color[width * height];
        return img;
    }
    
    IMAGEPROC_API void DestroyImage(Image* img)
    {
        if (img)
        {
            delete[] img->Pixels;
            delete img;
        }
    }
    
    IMAGEPROC_API void FillImage(Image* img, Color color)
    {
        if (!img || !img->Pixels)
            return;
            
        int totalPixels = img->Width * img->Height;
        for (int i = 0; i < totalPixels; i++)
        {
            img->Pixels[i] = color;
        }
    }
    
    IMAGEPROC_API void ConvertToGrayscale(Image* img)
    {
        if (!img || !img->Pixels)
            return;
            
        int totalPixels = img->Width * img->Height;
        for (int i = 0; i < totalPixels; i++)
        {
            Color& pixel = img->Pixels[i];
            unsigned char gray = static_cast<unsigned char>(
                0.299 * pixel.R + 
                0.587 * pixel.G + 
                0.114 * pixel.B);
            
            pixel.R = gray;
            pixel.G = gray;
            pixel.B = gray;
        }
    }
}
```

### C# Usage

```csharp
using System;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)]
public struct Color
{
    public byte R;
    public byte G;
    public byte B;
    public byte A;
}

public static class ImageProcessor
{
    const string DLL = "ImageProcessor.dll";
    
    [DllImport(DLL)]
    public static extern IntPtr CreateImage(int width, int height);
    
    [DllImport(DLL)]
    public static extern void DestroyImage(IntPtr img);
    
    [DllImport(DLL)]
    public static extern void FillImage(IntPtr img, Color color);
    
    [DllImport(DLL)]
    public static extern void ConvertToGrayscale(IntPtr img);
}

class Program
{
    static void Main()
    {
        // Create image
        IntPtr imgPtr = ImageProcessor.CreateImage(100, 100);
        
        try
        {
            // Fill with red
            Color red = new Color { R = 255, G = 0, B = 0, A = 255 };
            ImageProcessor.FillImage(imgPtr, red);
            
            // Convert to grayscale
            ImageProcessor.ConvertToGrayscale(imgPtr);
            
            Console.WriteLine("Image processed!");
        }
        finally
        {
            // Clean up
            ImageProcessor.DestroyImage(imgPtr);
        }
    }
}
```

## 🎮 Real-World Example: 3D Math Library

**Vector3D.h:**
```cpp
#pragma once

#ifdef MATH3D_EXPORTS
#define MATH3D_API __declspec(dllexport)
#else
#define MATH3D_API __declspec(dllimport)
#endif

extern "C" {
    
    struct Vector3
    {
        float X;
        float Y;
        float Z;
    };
    
    struct Matrix4x4
    {
        float M[16];  // Row-major order
    };
    
    // Vector operations
    MATH3D_API Vector3 VectorAdd(Vector3 a, Vector3 b);
    MATH3D_API Vector3 VectorSubtract(Vector3 a, Vector3 b);
    MATH3D_API Vector3 VectorScale(Vector3 v, float scale);
    MATH3D_API float VectorDot(Vector3 a, Vector3 b);
    MATH3D_API Vector3 VectorCross(Vector3 a, Vector3 b);
    MATH3D_API float VectorLength(Vector3 v);
    MATH3D_API Vector3 VectorNormalize(Vector3 v);
    
    // Matrix operations
    MATH3D_API Matrix4x4 MatrixIdentity();
    MATH3D_API Matrix4x4 MatrixMultiply(Matrix4x4 a, Matrix4x4 b);
    MATH3D_API Vector3 MatrixTransformVector(Matrix4x4 m, Vector3 v);
    MATH3D_API Matrix4x4 MatrixTranslation(float x, float y, float z);
    MATH3D_API Matrix4x4 MatrixRotationX(float angle);
    MATH3D_API Matrix4x4 MatrixRotationY(float angle);
    MATH3D_API Matrix4x4 MatrixRotationZ(float angle);
}
```

**Vector3D.cpp:**
```cpp
#include "pch.h"
#include "Vector3D.h"
#include <cmath>

extern "C" {
    
    MATH3D_API Vector3 VectorAdd(Vector3 a, Vector3 b)
    {
        return { a.X + b.X, a.Y + b.Y, a.Z + b.Z };
    }
    
    MATH3D_API Vector3 VectorSubtract(Vector3 a, Vector3 b)
    {
        return { a.X - b.X, a.Y - b.Y, a.Z - b.Z };
    }
    
    MATH3D_API Vector3 VectorScale(Vector3 v, float scale)
    {
        return { v.X * scale, v.Y * scale, v.Z * scale };
    }
    
    MATH3D_API float VectorDot(Vector3 a, Vector3 b)
    {
        return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
    }
    
    MATH3D_API Vector3 VectorCross(Vector3 a, Vector3 b)
    {
        return {
            a.Y * b.Z - a.Z * b.Y,
            a.Z * b.X - a.X * b.Z,
            a.X * b.Y - a.Y * b.X
        };
    }
    
    MATH3D_API float VectorLength(Vector3 v)
    {
        return std::sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
    }
    
    MATH3D_API Vector3 VectorNormalize(Vector3 v)
    {
        float len = VectorLength(v);
        if (len > 0.0f)
        {
            return VectorScale(v, 1.0f / len);
        }
        return v;
    }
    
    MATH3D_API Matrix4x4 MatrixIdentity()
    {
        Matrix4x4 m = { 0 };
        m.M[0] = m.M[5] = m.M[10] = m.M[15] = 1.0f;
        return m;
    }
    
    MATH3D_API Matrix4x4 MatrixTranslation(float x, float y, float z)
    {
        Matrix4x4 m = MatrixIdentity();
        m.M[12] = x;
        m.M[13] = y;
        m.M[14] = z;
        return m;
    }
    
    MATH3D_API Matrix4x4 MatrixRotationY(float angle)
    {
        Matrix4x4 m = MatrixIdentity();
        float c = std::cos(angle);
        float s = std::sin(angle);
        
        m.M[0] = c;
        m.M[2] = s;
        m.M[8] = -s;
        m.M[10] = c;
        
        return m;
    }
}
```

## 🐛 Debugging C++ DLLs

### Enable Debugging

**C++ Project Properties:**
```
Configuration: Debug
C/C++ → Optimization: Disabled (/Od)
C/C++ → Debug Info: Full (/Zi)
Linker → Debugging → Generate Debug Info: Yes
```

### Debug from C#

```
1. Set C# project as startup
2. Set breakpoint in C++ code
3. Project → Properties → Debug
4. Enable native code debugging:
   ☑ Enable native code debugging
5. Press F5
```

## 📋 Best Practices

```cpp
// ✅ DO: Use extern "C"
extern "C" {
    __declspec(dllexport) int MyFunction();
}

// ✅ DO: Handle null pointers
MATHLIB_API void ProcessData(int* data, int count)
{
    if (!data || count <= 0)
        return;
    // Process...
}

// ✅ DO: Provide cleanup functions
MATHLIB_API void* Create();
MATHLIB_API void Destroy(void* ptr);

// ❌ DON'T: Return C++ objects
// ❌ DON'T: Use std::string in exports
// ❌ DON'T: Throw C++ exceptions across boundary
```

## 📝 Summary

✅ Use `extern "C"` to prevent name mangling
✅ `__declspec(dllexport)` makes functions visible
✅ Match calling conventions (typically Cdecl for C++)
✅ Provide create/destroy functions for resources
✅ Always validate pointers and parameters
✅ Build for correct platform (x86/x64)

## 🚀 Next Steps

**Lesson 15:** Advanced P/Invoke - SafeHandle, platform-specific code, performance optimization

## 💪 Exercises

1. Create C++ DLL with string manipulation functions
2. Build image processing DLL with filters
3. Implement 3D transformation library
4. Create physics simulation DLL

---

**Outstanding!** 🎉 You can now create professional C++ DLLs for C#!

**Next:** [Lesson 15: Advanced P/Invoke](Lesson15-Advanced-PInvoke.md)
