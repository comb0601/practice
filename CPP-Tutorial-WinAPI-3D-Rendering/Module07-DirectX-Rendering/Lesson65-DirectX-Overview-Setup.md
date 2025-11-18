# Lesson 65: DirectX Overview & Setup

## Table of Contents
1. [Introduction to DirectX](#introduction-to-directx)
2. [DirectX Architecture](#directx-architecture)
3. [Direct3D 11 Overview](#direct3d-11-overview)
4. [Setting Up Development Environment](#setting-up-development-environment)
5. [Project Configuration](#project-configuration)
6. [COM and DirectX](#com-and-directx)
7. [Error Handling](#error-handling)
8. [Code Examples](#code-examples)

## Introduction to DirectX

### What is DirectX?

DirectX is a collection of Application Programming Interfaces (APIs) developed by Microsoft for handling multimedia-related tasks, particularly game programming and video rendering on Microsoft platforms. DirectX provides a standardized interface to interact with hardware components such as graphics cards, sound cards, and input devices.

**Key Components of DirectX:**

1. **Direct3D** - 3D graphics rendering (our primary focus)
2. **Direct2D** - 2D graphics rendering
3. **DirectWrite** - Text rendering
4. **DirectSound** - Audio playback
5. **DirectInput** - Input device handling
6. **DirectPlay** - Network communication
7. **DirectCompute** - General-purpose GPU computing

### Why DirectX 11?

DirectX 11, released in 2009, remains one of the most widely used graphics APIs for several reasons:

1. **Hardware Support**: Supported by the vast majority of gaming PCs
2. **Feature Set**: Provides tessellation, compute shaders, and multithreading
3. **Stability**: Mature API with extensive documentation and resources
4. **Balance**: Good balance between modern features and broad compatibility
5. **Learning Foundation**: Excellent foundation for understanding DirectX 12 and Vulkan

### DirectX Version History

- **DirectX 9** (2002): Fixed-function pipeline, shader model 3.0
- **DirectX 10** (2006): Unified shader architecture, geometry shaders
- **DirectX 11** (2009): Tessellation, compute shaders, multithreading
- **DirectX 11.1** (2012): Stereoscopic 3D improvements
- **DirectX 12** (2015): Low-level hardware access, reduced CPU overhead
- **DirectX 12 Ultimate** (2020): Ray tracing, variable rate shading, mesh shaders

## DirectX Architecture

### The Graphics Pipeline

DirectX 11 implements a programmable graphics pipeline that transforms 3D scene data into 2D pixels on your screen. Understanding this pipeline is crucial for effective DirectX programming.

**Pipeline Stages:**

1. **Input Assembler (IA)** - Reads vertex and index data from buffers
2. **Vertex Shader (VS)** - Transforms vertices from model space to clip space
3. **Hull Shader (HS)** - Tessellation control (optional)
4. **Tessellator** - Subdivides primitives (optional)
5. **Domain Shader (DS)** - Positions tessellated vertices (optional)
6. **Geometry Shader (GS)** - Generates or modifies primitives (optional)
7. **Rasterizer (RS)** - Converts primitives to pixels
8. **Pixel Shader (PS)** - Calculates final pixel colors
9. **Output Merger (OM)** - Combines pixel shader output with render targets

### Hardware Abstraction Layer (HAL)

DirectX uses a Hardware Abstraction Layer to provide a consistent interface across different graphics hardware:

```
Application Code
       |
    DirectX API
       |
    HAL Layer
       |
Graphics Driver
       |
Graphics Hardware
```

**Benefits:**
- Write once, run on any compatible hardware
- Hardware vendors optimize drivers for DirectX
- Applications don't need hardware-specific code

## Direct3D 11 Overview

### Core Direct3D 11 Concepts

**1. Device and Device Context**

- **ID3D11Device**: Represents the graphics adapter, used for creating resources
- **ID3D11DeviceContext**: Represents the rendering context, used for rendering

Think of the Device as a factory that creates objects, and the DeviceContext as the worker that uses those objects to render.

**2. Swap Chain**

The swap chain manages the buffers used for presenting rendered images to the screen. It implements double or triple buffering to avoid screen tearing.

```
Front Buffer (displayed) <--> Back Buffer (rendering)
```

**3. Resources**

DirectX 11 resources include:
- **Buffers**: Vertex buffers, index buffers, constant buffers
- **Textures**: 1D, 2D, 3D textures, texture arrays, cube maps
- **Views**: Different ways to access resources (RenderTargetView, ShaderResourceView, etc.)

**4. Shaders**

Small programs that run on the GPU to process vertices and pixels. Written in HLSL (High-Level Shading Language).

**5. States**

State objects control how the pipeline operates:
- **Rasterizer State**: Culling, fill mode, depth bias
- **Blend State**: How colors are combined
- **Depth-Stencil State**: Depth and stencil testing
- **Sampler State**: Texture sampling and filtering

## Setting Up Development Environment

### Required Software

**1. Visual Studio**

Download and install Visual Studio 2019 or later (Community Edition is free):
- Visit: https://visualstudio.microsoft.com/
- Install "Desktop development with C++" workload
- Ensure Windows SDK is included

**2. Windows SDK**

The Windows SDK includes DirectX headers and libraries:
- Installed automatically with Visual Studio
- Default location: `C:\Program Files (x86)\Windows Kits\10\`
- Contains:
  - Include files: `Include\um\d3d11.h`
  - Library files: `Lib\x64\d3d11.lib`

**3. DirectX SDK (Optional)**

For DirectX 11, the Windows SDK is sufficient. However, for older samples and utilities:
- Legacy DirectX SDK (June 2010) can be useful
- Contains samples, documentation, and tools
- Not required for modern DirectX 11 development

### Verifying Your Setup

**Check DirectX Version on Your System:**

1. Press `Win + R`
2. Type `dxdiag`
3. Click "OK"
4. Check "DirectX Version" on System tab
5. Check "Feature Levels" on Display tab

**Feature Levels:**

DirectX 11 introduced feature levels to support different hardware capabilities:

- **D3D_FEATURE_LEVEL_11_1**: Full DirectX 11.1 support
- **D3D_FEATURE_LEVEL_11_0**: Full DirectX 11 support
- **D3D_FEATURE_LEVEL_10_1**: DirectX 10.1 hardware
- **D3D_FEATURE_LEVEL_10_0**: DirectX 10 hardware
- **D3D_FEATURE_LEVEL_9_3**: DirectX 9 hardware (shader model 3.0)

## Project Configuration

### Creating a DirectX 11 Project

**Step 1: Create New Project in Visual Studio**

1. File → New → Project
2. Select "Empty Project" (C++)
3. Name: "DirectX11Tutorial"
4. Click "Create"

**Step 2: Add Source File**

1. Right-click "Source Files" in Solution Explorer
2. Add → New Item → C++ File (.cpp)
3. Name: "main.cpp"

**Step 3: Configure Project Properties**

Right-click project → Properties → Configuration: All Configurations

**Include Directories:**
```
$(WindowsSDK_IncludePath)
```

**Library Directories:**
```
$(WindowsSDK_LibraryPath_x64)
```

**Linker → Input → Additional Dependencies:**
```
d3d11.lib
dxgi.lib
d3dcompiler.lib
dxguid.lib
winmm.lib
```

**C/C++ → Preprocessor → Preprocessor Definitions:**
```
_UNICODE
UNICODE
```

**C/C++ → Language → C++ Language Standard:**
```
ISO C++17 Standard (/std:c++17)
```

### Project Structure Best Practices

```
DirectX11Tutorial/
├── Source/
│   ├── main.cpp
│   ├── Graphics.cpp
│   └── Graphics.h
├── Shaders/
│   ├── VertexShader.hlsl
│   └── PixelShader.hlsl
├── Assets/
│   ├── Textures/
│   └── Models/
└── Include/
    └── (third-party headers)
```

## COM and DirectX

### Component Object Model (COM)

DirectX uses COM for object management. Understanding COM is essential for DirectX programming.

**Key COM Concepts:**

**1. Interface-Based Programming**

COM objects are accessed through interfaces (pure abstract classes):
```cpp
ID3D11Device*         // Pointer to interface
```

**2. Reference Counting**

COM objects manage their lifetime through reference counting:

```cpp
// AddRef() - Increment reference count
// Release() - Decrement reference count, destroy when reaches 0

ID3D11Device* device = nullptr;
// Create device (reference count = 1)

device->AddRef();  // Reference count = 2
device->Release(); // Reference count = 1
device->Release(); // Reference count = 0, object destroyed
```

**3. Smart Pointers for COM**

Use `Microsoft::WRL::ComPtr` to manage COM objects automatically:

```cpp
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

ComPtr<ID3D11Device> device;
// Automatically calls Release() when ComPtr goes out of scope
```

**Alternative: ATL CComPtr**
```cpp
#include <atlbase.h>
CComPtr<ID3D11Device> device;
```

### COM in Practice

**Creating a COM Object:**

```cpp
// Manual reference management
ID3D11Device* device = nullptr;
HRESULT hr = D3D11CreateDevice(
    nullptr,
    D3D_DRIVER_TYPE_HARDWARE,
    nullptr,
    0,
    nullptr,
    0,
    D3D11_SDK_VERSION,
    &device,  // Receives interface pointer
    nullptr,
    nullptr
);

if (SUCCEEDED(hr)) {
    // Use device
    device->Release();  // Must manually release
}

// Using ComPtr
ComPtr<ID3D11Device> device;
HRESULT hr = D3D11CreateDevice(
    nullptr,
    D3D_DRIVER_TYPE_HARDWARE,
    nullptr,
    0,
    nullptr,
    0,
    D3D11_SDK_VERSION,
    &device,  // ComPtr handles reference counting
    nullptr,
    nullptr
);
// Automatically released when ComPtr goes out of scope
```

## Error Handling

### HRESULT Return Values

Most DirectX functions return HRESULT, a 32-bit value indicating success or failure.

**Checking HRESULT:**

```cpp
HRESULT hr = SomeDirectXFunction();

// Method 1: SUCCEEDED/FAILED macros
if (SUCCEEDED(hr)) {
    // Success
}
if (FAILED(hr)) {
    // Failure
}

// Method 2: Direct comparison
if (hr == S_OK) {
    // Success
}
if (hr != S_OK) {
    // Failure
}
```

**Common HRESULT Values:**

- `S_OK` (0x00000000): Success
- `E_FAIL` (0x80004005): Unspecified failure
- `E_INVALIDARG` (0x80070057): Invalid argument
- `E_OUTOFMEMORY` (0x8007000E): Out of memory
- `DXGI_ERROR_DEVICE_REMOVED`: GPU device has been removed
- `DXGI_ERROR_DEVICE_RESET`: GPU device needs to be reset

### Error Handling Helper

```cpp
#include <comdef.h>

inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        _com_error err(hr);
        LPCTSTR errMsg = err.ErrorMessage();
        MessageBox(nullptr, errMsg, L"DirectX Error", MB_OK | MB_ICONERROR);
        throw std::runtime_error("DirectX operation failed");
    }
}

// Usage
ThrowIfFailed(device->CreateBuffer(&bufferDesc, nullptr, &buffer));
```

### Debug Layer

DirectX 11 includes a debug layer that provides detailed error messages and validation:

```cpp
UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

D3D11CreateDevice(
    nullptr,
    D3D_DRIVER_TYPE_HARDWARE,
    nullptr,
    createDeviceFlags,  // Enable debug layer in debug builds
    nullptr,
    0,
    D3D11_SDK_VERSION,
    &device,
    nullptr,
    &context
);
```

**Debug Output:**

When the debug layer is enabled, DirectX outputs warnings and errors to the Visual Studio Output window:

```
D3D11 WARNING: ID3D11DeviceContext::Draw: Vertex Buffer at the input vertex slot 0 is not big enough for what the Draw*() call expects to traverse. This is OK, as reading off the end of the Buffer is defined to return 0. However the developer probably did not intend to rely on this behavior. [ EXECUTION WARNING #356: DEVICE_DRAW_VERTEX_BUFFER_TOO_SMALL]
```

## Code Examples

This lesson includes several example applications demonstrating DirectX setup and basic concepts. Each example builds upon the previous one.

### Example Programs

1. **01-CheckDirectXSupport.cpp** - Verify DirectX 11 support on your system
2. **02-EnumerateAdapters.cpp** - List all graphics adapters
3. **03-CreateDevice.cpp** - Create Direct3D device and context
4. **04-CreateWindow.cpp** - Create a window with DirectX
5. **05-MessageLoop.cpp** - Implement Windows message loop
6. **06-SwapChainSetup.cpp** - Create and configure swap chain
7. **07-COMSmartPointers.cpp** - Demonstrate COM smart pointer usage
8. **08-ErrorHandling.cpp** - Comprehensive error handling example
9. **09-DebugLayer.cpp** - Using DirectX debug layer
10. **10-FeatureLevels.cpp** - Detect and use feature levels

### Key Takeaways

After completing this lesson, you should understand:

1. **DirectX Architecture**: The overall structure and components of DirectX
2. **Direct3D 11 Pipeline**: The stages of the graphics pipeline
3. **Development Setup**: How to configure Visual Studio for DirectX development
4. **COM Basics**: Reference counting and smart pointers
5. **Error Handling**: Using HRESULT and the debug layer
6. **Project Configuration**: Proper linker and compiler settings

### Practice Exercises

1. **Environment Setup**: Create a new DirectX 11 project from scratch
2. **Feature Detection**: Write a program that detects your GPU's capabilities
3. **Adapter Enumeration**: List all display outputs for each adapter
4. **Debug Messages**: Enable the debug layer and intentionally trigger warnings
5. **Error Handler**: Create a robust error handling system with logging

### Additional Resources

**Official Documentation:**
- Microsoft DirectX 11 Documentation: https://docs.microsoft.com/en-us/windows/win32/direct3d11
- DirectX Graphics Samples: https://github.com/microsoft/DirectX-Graphics-Samples

**Books:**
- "Introduction to 3D Game Programming with DirectX 11" by Frank Luna
- "Practical Rendering and Computation with Direct3D 11" by Jason Zink

**Online Resources:**
- RasterTek DirectX 11 Tutorials: http://www.rastertek.com/tutdx11.html
- Braynzar Soft DirectX 11 Tutorials: https://www.braynzarsoft.net/viewtutorial/q16390-braynzar-soft-directx-11-tutorials

### Next Lesson Preview

In Lesson 66, we'll dive into initializing Direct3D 11:
- Creating the device and device context
- Setting up the swap chain
- Creating render target views
- Configuring the viewport
- Clearing the screen to a solid color

This will be your first step toward rendering actual 3D graphics!

---

## Study Time Estimate

- **Reading and Understanding**: 2-3 hours
- **Setting Up Environment**: 1 hour
- **Running Examples**: 1-2 hours
- **Practice Exercises**: 2-3 hours
- **Total**: 6-9 hours

## Glossary

**API (Application Programming Interface)**: A set of functions and protocols for building software applications

**COM (Component Object Model)**: Microsoft's binary-interface standard for software components

**Device**: The Direct3D object representing the graphics adapter, used for creating resources

**Device Context**: The Direct3D object representing the rendering context, used for rendering commands

**Feature Level**: A well-defined set of GPU functionality

**HAL (Hardware Abstraction Layer)**: Software layer that provides a consistent interface to hardware

**HLSL (High-Level Shading Language)**: DirectX's shader programming language

**HRESULT**: A 32-bit value indicating success or failure of a function call

**Pipeline**: The series of stages that transform 3D data into 2D pixels

**Swap Chain**: The buffers used for presenting rendered images to the screen

**Vertex**: A point in 3D space with associated attributes (position, color, texture coordinates, etc.)

---

**End of Lesson 65**

Continue to Lesson 66: Initializing Direct3D 11
