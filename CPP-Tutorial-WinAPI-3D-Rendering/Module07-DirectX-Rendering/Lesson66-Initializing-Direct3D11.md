# Lesson 66: Initializing Direct3D 11

## Table of Contents
1. [Introduction](#introduction)
2. [Device and Device Context](#device-and-device-context)
3. [Swap Chain Creation](#swap-chain-creation)
4. [Render Target Views](#render-target-views)
5. [Viewport Configuration](#viewport-configuration)
6. [Complete Initialization Sequence](#complete-initialization-sequence)
7. [Clearing the Screen](#clearing-the-screen)
8. [Handling Window Resize](#handling-window-resize)

## Introduction

Initializing Direct3D 11 is the foundation of every DirectX application. This lesson covers the complete initialization sequence, from creating the device to presenting your first rendered frame.

### Key Initialization Steps

1. Create Device and Device Context
2. Create Swap Chain
3. Get Back Buffer from Swap Chain
4. Create Render Target View
5. Set Render Target
6. Configure Viewport
7. Clear and Present

### Why This Sequence Matters

Each step builds upon the previous one. The device creates resources, the swap chain manages buffers, the render target view provides access to those buffers, and the viewport defines the rendering area.

## Device and Device Context

### Understanding the Device

The **ID3D11Device** is your factory for creating DirectX resources:

```cpp
ID3D11Device* device = nullptr;
```

**Device Responsibilities:**
- Create buffers (vertex, index, constant)
- Create textures and views
- Create shaders
- Create state objects
- Query capabilities

### Understanding the Device Context

The **ID3D11DeviceContext** is your rendering command interface:

```cpp
ID3D11DeviceContext* context = nullptr;
```

**Context Responsibilities:**
- Issue draw calls
- Set pipeline state
- Bind resources to pipeline stages
- Update dynamic resources
- Execute command lists

### Factory vs. Worker Analogy

```
Device (Factory)           Device Context (Worker)
├── Creates buffers        ├── Binds buffers
├── Creates shaders        ├── Sets shaders
├── Creates textures       ├── Binds textures
└── Creates states         └── Sets states
```

### Creating Device and Context

```cpp
D3D_FEATURE_LEVEL featureLevel;
UINT createFlags = 0;

#ifdef _DEBUG
    createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

HRESULT hr = D3D11CreateDevice(
    nullptr,                      // Use default adapter
    D3D_DRIVER_TYPE_HARDWARE,     // Hardware acceleration
    nullptr,                      // No software module
    createFlags,                  // Creation flags
    nullptr,                      // Default feature levels
    0,                            // Number of feature levels
    D3D11_SDK_VERSION,            // SDK version
    &device,                      // OUTPUT: Device
    &featureLevel,                // OUTPUT: Feature level
    &context                      // OUTPUT: Context
);
```

## Swap Chain Creation

### What is a Swap Chain?

The swap chain manages the front and back buffers for rendering:

```
┌─────────────┐         ┌─────────────┐
│ Back Buffer │  Swap   │Front Buffer │
│  (Drawing)  │ <=====> │ (Displayed) │
└─────────────┘         └─────────────┘
```

**Benefits:**
- Prevents screen tearing
- Enables double/triple buffering
- Synchronizes with display refresh

### DXGI Swap Chain Description

```cpp
DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

// Buffer settings
swapChainDesc.BufferCount = 1;                          // Double buffering
swapChainDesc.BufferDesc.Width = width;
swapChainDesc.BufferDesc.Height = height;
swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

// Refresh rate
swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

// Usage and target
swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
swapChainDesc.OutputWindow = hWnd;

// Sampling
swapChainDesc.SampleDesc.Count = 1;                     // No MSAA
swapChainDesc.SampleDesc.Quality = 0;

// Window mode
swapChainDesc.Windowed = TRUE;

// Flags
swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
```

### Creating Device and Swap Chain Together

```cpp
HRESULT hr = D3D11CreateDeviceAndSwapChain(
    nullptr,                      // Use default adapter
    D3D_DRIVER_TYPE_HARDWARE,
    nullptr,
    createFlags,
    featureLevels,
    numFeatureLevels,
    D3D11_SDK_VERSION,
    &swapChainDesc,               // Swap chain description
    &swapChain,                   // OUTPUT: Swap chain
    &device,                      // OUTPUT: Device
    &featureLevel,                // OUTPUT: Feature level
    &context                      // OUTPUT: Context
);
```

### Swap Chain Formats

Common pixel formats:

- **DXGI_FORMAT_R8G8B8A8_UNORM**: 32-bit RGBA (most common)
- **DXGI_FORMAT_R16G16B16A16_FLOAT**: 64-bit HDR
- **DXGI_FORMAT_R10G10B10A2_UNORM**: 32-bit HDR
- **DXGI_FORMAT_B8G8R8A8_UNORM**: 32-bit BGRA (legacy)

## Render Target Views

### What is a Render Target View?

A **view** in DirectX is an interface to access resource data. The **Render Target View** (RTV) tells DirectX how to interpret the back buffer for rendering.

```
Back Buffer Texture ──→ Render Target View ──→ Pipeline Output Merger
    (Raw Data)           (Interpretation)         (Rendering)
```

### Creating a Render Target View

```cpp
// Step 1: Get back buffer from swap chain
ID3D11Texture2D* backBuffer = nullptr;
hr = swapChain->GetBuffer(
    0,                                  // Buffer index
    __uuidof(ID3D11Texture2D),         // Interface type
    (void**)&backBuffer                 // OUTPUT: Back buffer
);

if (FAILED(hr)) {
    // Handle error
    return false;
}

// Step 2: Create render target view
ID3D11RenderTargetView* renderTargetView = nullptr;
hr = device->CreateRenderTargetView(
    backBuffer,                         // Resource
    nullptr,                            // Default description
    &renderTargetView                   // OUTPUT: RTV
);

// Step 3: Release back buffer (we only need the view now)
backBuffer->Release();

if (FAILED(hr)) {
    // Handle error
    return false;
}

// Step 4: Bind render target to output merger stage
context->OMSetRenderTargets(
    1,                                  // Number of RTVs
    &renderTargetView,                  // Array of RTVs
    nullptr                             // Depth stencil view (none yet)
);
```

### Why We Need Views

DirectX uses views to:
- Allow multiple interpretations of same resource
- Enable type safety
- Support resource binding to pipeline stages
- Provide format conversion

## Viewport Configuration

### What is a Viewport?

The viewport defines the region of the render target where rendering occurs and how 3D coordinates map to screen coordinates.

```
3D Space (Clip Space)    Viewport Transform    2D Screen Space
     [-1, 1]          ────────────────→         [0, Width] x [0, Height]
```

### Viewport Structure

```cpp
D3D11_VIEWPORT viewport = {};
viewport.TopLeftX = 0.0f;                // X position
viewport.TopLeftY = 0.0f;                // Y position
viewport.Width = static_cast<float>(width);
viewport.Height = static_cast<float>(height);
viewport.MinDepth = 0.0f;                // Minimum depth (near plane)
viewport.MaxDepth = 1.0f;                // Maximum depth (far plane)

context->RSSetViewports(1, &viewport);
```

### Multiple Viewports

DirectX 11 supports up to 16 simultaneous viewports:

```cpp
D3D11_VIEWPORT viewports[2];

// Left half of screen
viewports[0].TopLeftX = 0.0f;
viewports[0].Width = width / 2.0f;

// Right half of screen
viewports[1].TopLeftX = width / 2.0f;
viewports[1].Width = width / 2.0f;

context->RSSetViewports(2, viewports);
```

**Use Cases:**
- Split-screen rendering
- Picture-in-picture
- Multi-view rendering
- VR rendering (one per eye)

## Complete Initialization Sequence

### Full Initialization Function

```cpp
class DirectX11App
{
private:
    HWND m_hWnd;
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_context;
    IDXGISwapChain* m_swapChain;
    ID3D11RenderTargetView* m_renderTargetView;
    D3D_FEATURE_LEVEL m_featureLevel;

public:
    bool Initialize(HWND hWnd, int width, int height)
    {
        m_hWnd = hWnd;

        // Step 1: Define swap chain
        DXGI_SWAP_CHAIN_DESC scd = {};
        scd.BufferCount = 1;
        scd.BufferDesc.Width = width;
        scd.BufferDesc.Height = height;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.BufferDesc.RefreshRate.Numerator = 60;
        scd.BufferDesc.RefreshRate.Denominator = 1;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.OutputWindow = hWnd;
        scd.SampleDesc.Count = 1;
        scd.SampleDesc.Quality = 0;
        scd.Windowed = TRUE;

        // Step 2: Feature levels
        D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0
        };

        UINT createFlags = 0;
        #ifdef _DEBUG
            createFlags |= D3D11_CREATE_DEVICE_DEBUG;
        #endif

        // Step 3: Create device and swap chain
        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            createFlags,
            featureLevels,
            ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION,
            &scd,
            &m_swapChain,
            &m_device,
            &m_featureLevel,
            &m_context
        );

        if (FAILED(hr)) return false;

        // Step 4: Create render target view
        if (!CreateRenderTarget()) return false;

        // Step 5: Set viewport
        SetViewport(width, height);

        return true;
    }

    bool CreateRenderTarget()
    {
        ID3D11Texture2D* backBuffer = nullptr;

        HRESULT hr = m_swapChain->GetBuffer(0,
            __uuidof(ID3D11Texture2D),
            (void**)&backBuffer);

        if (FAILED(hr)) return false;

        hr = m_device->CreateRenderTargetView(backBuffer,
            nullptr, &m_renderTargetView);

        backBuffer->Release();

        if (FAILED(hr)) return false;

        m_context->OMSetRenderTargets(1, &m_renderTargetView, nullptr);
        return true;
    }

    void SetViewport(int width, int height)
    {
        D3D11_VIEWPORT viewport = {};
        viewport.Width = static_cast<float>(width);
        viewport.Height = static_cast<float>(height);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;

        m_context->RSSetViewports(1, &viewport);
    }

    void Cleanup()
    {
        if (m_renderTargetView) m_renderTargetView->Release();
        if (m_swapChain) m_swapChain->Release();
        if (m_context) m_context->Release();
        if (m_device) m_device->Release();
    }
};
```

## Clearing the Screen

### The Clear Operation

```cpp
void Render()
{
    // Define clear color (RGBA, values 0.0-1.0)
    float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };  // Dark blue

    // Clear render target
    m_context->ClearRenderTargetView(m_renderTargetView, clearColor);

    // Present the frame
    m_swapChain->Present(
        1,      // Sync interval (1 = VSync on)
        0       // Flags
    );
}
```

### Present Modes

```cpp
// VSync On (recommended for most apps)
m_swapChain->Present(1, 0);

// VSync Off (for benchmarking)
m_swapChain->Present(0, 0);

// Discard previous frame
m_swapChain->Present(0, DXGI_PRESENT_DO_NOT_WAIT);
```

## Handling Window Resize

### Why Resize Matters

When window size changes:
- Back buffer size must match
- Render target must be recreated
- Viewport must be updated

### Resize Handler

```cpp
void OnResize(int newWidth, int newHeight)
{
    if (!m_swapChain) return;

    // Release render target
    m_context->OMSetRenderTargets(0, nullptr, nullptr);
    m_renderTargetView->Release();

    // Resize swap chain buffers
    HRESULT hr = m_swapChain->ResizeBuffers(
        1,                              // Buffer count
        newWidth,                       // New width
        newHeight,                      // New height
        DXGI_FORMAT_R8G8B8A8_UNORM,    // Format
        0                               // Flags
    );

    if (FAILED(hr)) return;

    // Recreate render target
    CreateRenderTarget();

    // Update viewport
    SetViewport(newWidth, newHeight);
}
```

### Handling WM_SIZE

```cpp
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static DirectX11App* app = nullptr;

    switch (msg)
    {
    case WM_CREATE:
        {
            CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
            app = (DirectX11App*)cs->lpCreateParams;
        }
        return 0;

    case WM_SIZE:
        if (app && wParam != SIZE_MINIMIZED)
        {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            app->OnResize(width, height);
        }
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
```

## Best Practices

### 1. Error Handling

Always check HRESULT:

```cpp
HRESULT hr = device->CreateBuffer(&desc, nullptr, &buffer);
if (FAILED(hr))
{
    // Log error, show message, cleanup, return
}
```

### 2. Resource Cleanup

Release in reverse order of creation:

```cpp
// Cleanup order
if (renderTargetView) renderTargetView->Release();
if (swapChain) swapChain->Release();
if (context) context->Release();
if (device) device->Release();
```

### 3. Null Checks

```cpp
if (m_renderTargetView)
{
    m_renderTargetView->Release();
    m_renderTargetView = nullptr;
}
```

### 4. Debug Layer

Enable in debug builds:

```cpp
#ifdef _DEBUG
    createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
```

## Common Errors

### 1. Swap Chain Creation Failure

**Problem:** Swap chain creation returns E_INVALIDARG

**Solutions:**
- Check window handle is valid
- Verify buffer dimensions > 0
- Ensure format is supported
- Check if window is minimized

### 2. Black Screen

**Problem:** Window shows black screen

**Possible Causes:**
- Render target not set
- Viewport not configured
- Present() not called
- Drawing before Clear()

### 3. Resize Crash

**Problem:** Crash when resizing window

**Solution:**
- Release render target before ResizeBuffers()
- Unbind from pipeline (OMSetRenderTargets)
- Recreate render target after resize

## Performance Considerations

### VSync vs. No VSync

```
VSync On:
✓ No screen tearing
✓ Lower power consumption
✗ Limited to 60 FPS (typically)

VSync Off:
✓ Higher frame rates
✗ Screen tearing
✗ Higher power consumption
```

### Buffer Count

- 1 buffer (double buffering): Standard, works for most cases
- 2 buffers (triple buffering): Smoother frame pacing
- 3+ buffers: Rarely needed, increases latency

## Next Steps

In Lesson 67, we'll explore the rendering pipeline in detail:
- Pipeline stages
- Data flow
- Shader interaction
- Primitive assembly

---

**Study Time:** 4-5 hours
**Practice Time:** 3-4 hours
**Total:** 7-9 hours
