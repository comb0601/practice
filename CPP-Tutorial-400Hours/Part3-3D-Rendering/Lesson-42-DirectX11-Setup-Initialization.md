# Lesson 42: DirectX 11 Setup and Initialization

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. DirectX 11 Initialization Overview
2. Adapter Enumeration and Selection
3. Feature Level Selection
4. Creating the Device and Context
5. Swap Chain Configuration
6. Render Target View Creation
7. Viewport Setup
8. Debug Layer and Error Handling
9. Building a Reusable D3D11 Class
10. Complete Working Example
11. Exercises and Best Practices

---

## 1. DirectX 11 Initialization Overview

DirectX 11 initialization involves several key steps that must be performed in the correct order:

### Initialization Flow:

```
1. Enumerate Graphics Adapters (GPUs)
        ↓
2. Select Appropriate Adapter
        ↓
3. Create Device and Device Context
        ↓
4. Create Swap Chain
        ↓
5. Get Back Buffer from Swap Chain
        ↓
6. Create Render Target View
        ↓
7. Set Render Target
        ↓
8. Create and Set Viewport
        ↓
9. (Optional) Create Depth/Stencil Buffer
        ↓
10. Ready to Render
```

### Key Objects:

| Object | Purpose |
|--------|---------|
| **ID3D11Device** | Creates resources (buffers, textures, shaders) |
| **ID3D11DeviceContext** | Issues rendering commands |
| **IDXGISwapChain** | Manages front/back buffers and presentation |
| **ID3D11RenderTargetView** | View of the back buffer for rendering |
| **ID3D11Texture2D** | 2D texture (back buffer, depth buffer) |
| **ID3D11DepthStencilView** | View of depth/stencil buffer |

---

## 2. Adapter Enumeration and Selection

### Understanding Adapters:

An **adapter** represents a graphics card (GPU). Systems can have multiple adapters:
- Dedicated GPU (NVIDIA, AMD)
- Integrated GPU (Intel, AMD APU)
- Software renderer (WARP)

### Enumerating Adapters:

```cpp
#include <d3d11.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <iostream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

struct AdapterInfo {
    std::wstring description;
    SIZE_T dedicatedVideoMemory;
    SIZE_T dedicatedSystemMemory;
    SIZE_T sharedSystemMemory;
    UINT vendorId;
    UINT deviceId;
    IDXGIAdapter1* adapter;
};

class AdapterEnumerator {
public:
    static std::vector<AdapterInfo> EnumerateAdapters() {
        std::vector<AdapterInfo> adapters;

        // Create DXGI Factory
        IDXGIFactory1* factory = nullptr;
        HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1),
                                        (void**)&factory);
        if (FAILED(hr)) {
            return adapters;
        }

        // Enumerate adapters
        IDXGIAdapter1* adapter = nullptr;
        UINT index = 0;

        while (factory->EnumAdapters1(index, &adapter) != DXGI_ERROR_NOT_FOUND) {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            AdapterInfo info;
            info.description = desc.Description;
            info.dedicatedVideoMemory = desc.DedicatedVideoMemory;
            info.dedicatedSystemMemory = desc.DedicatedSystemMemory;
            info.sharedSystemMemory = desc.SharedSystemMemory;
            info.vendorId = desc.VendorId;
            info.deviceId = desc.DeviceId;
            info.adapter = adapter;

            adapters.push_back(info);
            ++index;
        }

        factory->Release();
        return adapters;
    }

    static void PrintAdapterInfo(const AdapterInfo& info, UINT index) {
        std::wcout << L"\n--- Adapter " << index << L" ---\n";
        std::wcout << L"Description: " << info.description << L"\n";
        std::cout << "Dedicated Video Memory: "
                  << (info.dedicatedVideoMemory / 1024 / 1024) << " MB\n";
        std::cout << "Dedicated System Memory: "
                  << (info.dedicatedSystemMemory / 1024 / 1024) << " MB\n";
        std::cout << "Shared System Memory: "
                  << (info.sharedSystemMemory / 1024 / 1024) << " MB\n";

        // Vendor identification
        const char* vendor = "Unknown";
        switch (info.vendorId) {
            case 0x10DE: vendor = "NVIDIA"; break;
            case 0x1002: vendor = "AMD"; break;
            case 0x8086: vendor = "Intel"; break;
            case 0x1414: vendor = "Microsoft (Software)"; break;
        }
        std::cout << "Vendor: " << vendor << "\n";
    }

    static IDXGIAdapter1* SelectBestAdapter(const std::vector<AdapterInfo>& adapters) {
        if (adapters.empty()) return nullptr;

        // Priority: Most dedicated video memory
        SIZE_T maxMemory = 0;
        IDXGIAdapter1* bestAdapter = nullptr;

        for (const auto& info : adapters) {
            if (info.dedicatedVideoMemory > maxMemory) {
                maxMemory = info.dedicatedVideoMemory;
                bestAdapter = info.adapter;
            }
        }

        if (bestAdapter) {
            bestAdapter->AddRef(); // Add reference before returning
        }

        return bestAdapter;
    }
};

// Example usage
void TestAdapterEnumeration() {
    auto adapters = AdapterEnumerator::EnumerateAdapters();

    std::cout << "Found " << adapters.size() << " adapter(s)\n";

    for (UINT i = 0; i < adapters.size(); ++i) {
        AdapterEnumerator::PrintAdapterInfo(adapters[i], i);
    }

    IDXGIAdapter1* bestAdapter = AdapterEnumerator::SelectBestAdapter(adapters);
    if (bestAdapter) {
        std::cout << "\nSelected best adapter\n";
        bestAdapter->Release();
    }

    // Cleanup
    for (auto& info : adapters) {
        info.adapter->Release();
    }
}
```

### Vendor IDs:

- **0x10DE**: NVIDIA
- **0x1002**: AMD
- **0x8086**: Intel
- **0x1414**: Microsoft (Software/WARP)

---

## 3. Feature Level Selection

Feature levels define the capabilities of the hardware:

### Available Feature Levels:

```cpp
enum D3D_FEATURE_LEVEL {
    D3D_FEATURE_LEVEL_9_1   = 0x9100,  // Shader Model 2.0
    D3D_FEATURE_LEVEL_9_2   = 0x9200,
    D3D_FEATURE_LEVEL_9_3   = 0x9300,
    D3D_FEATURE_LEVEL_10_0  = 0xa000,  // Shader Model 4.0
    D3D_FEATURE_LEVEL_10_1  = 0xa100,  // Shader Model 4.1
    D3D_FEATURE_LEVEL_11_0  = 0xb000,  // Shader Model 5.0
    D3D_FEATURE_LEVEL_11_1  = 0xb100,  // Shader Model 5.0
    D3D_FEATURE_LEVEL_12_0  = 0xc000,  // DirectX 12 (via DX11 on 12)
    D3D_FEATURE_LEVEL_12_1  = 0xc100
};
```

### Feature Level Capabilities:

| Feature Level | Shader Model | Max Texture Size | Notable Features |
|---------------|--------------|------------------|------------------|
| 9.1 | 2.0 | 2048 | Basic shaders |
| 9.3 | 2.0 | 4096 | Instancing |
| 10.0 | 4.0 | 8192 | Geometry shader |
| 10.1 | 4.1 | 8192 | Cubemap arrays |
| 11.0 | 5.0 | 16384 | Tessellation, compute |
| 11.1 | 5.0 | 16384 | UAVs at all stages |

### Selecting Feature Levels:

```cpp
class FeatureLevelSelector {
public:
    static D3D_FEATURE_LEVEL* GetSupportedFeatureLevels(UINT& count) {
        // Request highest to lowest
        static D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_9_2,
            D3D_FEATURE_LEVEL_9_1
        };

        count = ARRAYSIZE(featureLevels);
        return featureLevels;
    }

    static const char* FeatureLevelToString(D3D_FEATURE_LEVEL level) {
        switch (level) {
            case D3D_FEATURE_LEVEL_11_1: return "11.1";
            case D3D_FEATURE_LEVEL_11_0: return "11.0";
            case D3D_FEATURE_LEVEL_10_1: return "10.1";
            case D3D_FEATURE_LEVEL_10_0: return "10.0";
            case D3D_FEATURE_LEVEL_9_3:  return "9.3";
            case D3D_FEATURE_LEVEL_9_2:  return "9.2";
            case D3D_FEATURE_LEVEL_9_1:  return "9.1";
            default: return "Unknown";
        }
    }

    static bool SupportsComputeShaders(D3D_FEATURE_LEVEL level) {
        return level >= D3D_FEATURE_LEVEL_11_0;
    }

    static bool SupportsTessellation(D3D_FEATURE_LEVEL level) {
        return level >= D3D_FEATURE_LEVEL_11_0;
    }

    static UINT GetMaxTextureSize(D3D_FEATURE_LEVEL level) {
        if (level >= D3D_FEATURE_LEVEL_11_0) return 16384;
        if (level >= D3D_FEATURE_LEVEL_10_0) return 8192;
        if (level >= D3D_FEATURE_LEVEL_9_3)  return 4096;
        return 2048;
    }
};
```

---

## 4. Creating the Device and Context

### Device Creation Flags:

```cpp
enum D3D11_CREATE_DEVICE_FLAG {
    D3D11_CREATE_DEVICE_SINGLETHREADED          = 0x1,    // Not thread-safe (faster)
    D3D11_CREATE_DEVICE_DEBUG                   = 0x2,    // Enable debug layer
    D3D11_CREATE_DEVICE_SWITCH_TO_REF          = 0x4,    // Use reference device
    D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING = 0x8,
    D3D11_CREATE_DEVICE_BGRA_SUPPORT           = 0x20,   // BGRA format support
    D3D11_CREATE_DEVICE_DEBUGGABLE             = 0x40,   // PIX debugging
    D3D11_CREATE_DEVICE_PREVENT_ALTERING_LAYER_SETTINGS = 0x80,
    D3D11_CREATE_DEVICE_DISABLE_GPU_TIMEOUT    = 0x100,  // No TDR timeout
    D3D11_CREATE_DEVICE_VIDEO_SUPPORT          = 0x800   // Video support
};
```

### Creating Device and Context:

```cpp
class DeviceCreator {
public:
    struct DeviceResult {
        ID3D11Device* device = nullptr;
        ID3D11DeviceContext* context = nullptr;
        D3D_FEATURE_LEVEL featureLevel;
        HRESULT hr;
        std::string errorMessage;
    };

    static DeviceResult CreateDevice(IDXGIAdapter1* adapter = nullptr) {
        DeviceResult result;

        // Determine driver type
        D3D_DRIVER_TYPE driverType = adapter ? D3D_DRIVER_TYPE_UNKNOWN
                                              : D3D_DRIVER_TYPE_HARDWARE;

        // Set creation flags
        UINT createDeviceFlags = 0;
#ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        // Add BGRA support for Direct2D interop
        createDeviceFlags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

        // Get feature levels
        UINT featureLevelCount;
        D3D_FEATURE_LEVEL* featureLevels =
            FeatureLevelSelector::GetSupportedFeatureLevels(featureLevelCount);

        // Create device and context
        result.hr = D3D11CreateDevice(
            adapter,                    // Adapter (nullptr = default)
            driverType,                 // Driver type
            nullptr,                    // Software rasterizer module
            createDeviceFlags,          // Flags
            featureLevels,             // Feature levels array
            featureLevelCount,         // Feature levels count
            D3D11_SDK_VERSION,         // SDK version
            &result.device,            // Output: Device
            &result.featureLevel,      // Output: Feature level
            &result.context            // Output: Device context
        );

        if (FAILED(result.hr)) {
            result.errorMessage = GetErrorMessage(result.hr);
            return result;
        }

        // Success
        std::cout << "✓ DirectX 11 Device created successfully\n";
        std::cout << "  Feature Level: "
                  << FeatureLevelSelector::FeatureLevelToString(result.featureLevel)
                  << "\n";

        return result;
    }

    static DeviceResult CreateDeviceWithAdapter(UINT adapterIndex = 0) {
        auto adapters = AdapterEnumerator::EnumerateAdapters();

        if (adapters.empty()) {
            DeviceResult result;
            result.hr = E_FAIL;
            result.errorMessage = "No adapters found";
            return result;
        }

        if (adapterIndex >= adapters.size()) {
            adapterIndex = 0;
        }

        DeviceResult result = CreateDevice(adapters[adapterIndex].adapter);

        // Cleanup adapters
        for (auto& info : adapters) {
            info.adapter->Release();
        }

        return result;
    }

private:
    static std::string GetErrorMessage(HRESULT hr) {
        switch (hr) {
            case D3D11_ERROR_FILE_NOT_FOUND:
                return "File not found";
            case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
                return "Too many unique state objects";
            case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
                return "Too many unique view objects";
            case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
                return "Deferred context map without initial discard";
            case E_INVALIDARG:
                return "Invalid argument";
            case E_OUTOFMEMORY:
                return "Out of memory";
            case E_NOTIMPL:
                return "Not implemented";
            default:
                return "Unknown error";
        }
    }
};
```

---

## 5. Swap Chain Configuration

### Swap Chain Description:

```cpp
struct SwapChainConfig {
    UINT width = 800;
    UINT height = 600;
    DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
    UINT refreshRateNumerator = 60;
    UINT refreshRateDenominator = 1;
    UINT bufferCount = 2;  // Double buffering
    UINT sampleCount = 1;  // MSAA sample count
    UINT sampleQuality = 0;
    DXGI_USAGE usage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    BOOL windowed = TRUE;
    DXGI_SWAP_EFFECT swapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    UINT flags = 0;
};

class SwapChainCreator {
public:
    static HRESULT CreateSwapChain(
        ID3D11Device* device,
        HWND hWnd,
        const SwapChainConfig& config,
        IDXGISwapChain** outSwapChain)
    {
        if (!device || !hWnd || !outSwapChain) {
            return E_INVALIDARG;
        }

        // Get DXGI Factory from device
        IDXGIDevice* dxgiDevice = nullptr;
        HRESULT hr = device->QueryInterface(__uuidof(IDXGIDevice),
                                             (void**)&dxgiDevice);
        if (FAILED(hr)) return hr;

        IDXGIAdapter* adapter = nullptr;
        hr = dxgiDevice->GetAdapter(&adapter);
        dxgiDevice->Release();
        if (FAILED(hr)) return hr;

        IDXGIFactory2* factory = nullptr;
        hr = adapter->GetParent(__uuidof(IDXGIFactory2), (void**)&factory);
        adapter->Release();
        if (FAILED(hr)) return hr;

        // Modern swap chain creation (Windows 8+)
        if (factory) {
            DXGI_SWAP_CHAIN_DESC1 scd = {};
            scd.Width = config.width;
            scd.Height = config.height;
            scd.Format = config.format;
            scd.Stereo = FALSE;
            scd.SampleDesc.Count = config.sampleCount;
            scd.SampleDesc.Quality = config.sampleQuality;
            scd.BufferUsage = config.usage;
            scd.BufferCount = config.bufferCount;
            scd.Scaling = DXGI_SCALING_STRETCH;
            scd.SwapEffect = config.swapEffect;
            scd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
            scd.Flags = config.flags;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsDesc = {};
            fsDesc.RefreshRate.Numerator = config.refreshRateNumerator;
            fsDesc.RefreshRate.Denominator = config.refreshRateDenominator;
            fsDesc.Windowed = config.windowed;
            fsDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
            fsDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

            IDXGISwapChain1* swapChain1 = nullptr;
            hr = factory->CreateSwapChainForHwnd(
                device,
                hWnd,
                &scd,
                &fsDesc,
                nullptr,
                &swapChain1
            );

            if (SUCCEEDED(hr)) {
                hr = swapChain1->QueryInterface(__uuidof(IDXGISwapChain),
                                                 (void**)outSwapChain);
                swapChain1->Release();
            }

            // Disable Alt+Enter fullscreen toggle (we'll handle it manually)
            factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);

            factory->Release();
        }

        return hr;
    }

    static HRESULT CreateSwapChainLegacy(
        ID3D11Device* device,
        HWND hWnd,
        const SwapChainConfig& config,
        IDXGISwapChain** outSwapChain)
    {
        // Legacy method (Windows 7)
        IDXGIDevice* dxgiDevice = nullptr;
        HRESULT hr = device->QueryInterface(__uuidof(IDXGIDevice),
                                             (void**)&dxgiDevice);
        if (FAILED(hr)) return hr;

        IDXGIAdapter* adapter = nullptr;
        hr = dxgiDevice->GetAdapter(&adapter);
        dxgiDevice->Release();
        if (FAILED(hr)) return hr;

        IDXGIFactory1* factory = nullptr;
        hr = adapter->GetParent(__uuidof(IDXGIFactory1), (void**)&factory);
        adapter->Release();
        if (FAILED(hr)) return hr;

        DXGI_SWAP_CHAIN_DESC scd = {};
        scd.BufferDesc.Width = config.width;
        scd.BufferDesc.Height = config.height;
        scd.BufferDesc.RefreshRate.Numerator = config.refreshRateNumerator;
        scd.BufferDesc.RefreshRate.Denominator = config.refreshRateDenominator;
        scd.BufferDesc.Format = config.format;
        scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        scd.SampleDesc.Count = config.sampleCount;
        scd.SampleDesc.Quality = config.sampleQuality;
        scd.BufferUsage = config.usage;
        scd.BufferCount = config.bufferCount;
        scd.OutputWindow = hWnd;
        scd.Windowed = config.windowed;
        scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        scd.Flags = config.flags;

        hr = factory->CreateSwapChain(device, &scd, outSwapChain);

        factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
        factory->Release();

        return hr;
    }
};
```

### Swap Effects:

| Swap Effect | Description | Best For |
|-------------|-------------|----------|
| DISCARD | Contents discarded after Present | Windows 7, simple cases |
| SEQUENTIAL | Buffers presented in order | Legacy |
| FLIP_SEQUENTIAL | Flip model, sequential | Windows 8+, lower latency |
| FLIP_DISCARD | Flip model, discard | Windows 10+, best performance |

---

## 6. Render Target View Creation

```cpp
class RenderTargetManager {
public:
    static HRESULT CreateRenderTargetView(
        ID3D11Device* device,
        IDXGISwapChain* swapChain,
        ID3D11RenderTargetView** outRTV)
    {
        if (!device || !swapChain || !outRTV) {
            return E_INVALIDARG;
        }

        // Get back buffer from swap chain
        ID3D11Texture2D* backBuffer = nullptr;
        HRESULT hr = swapChain->GetBuffer(
            0,
            __uuidof(ID3D11Texture2D),
            (void**)&backBuffer
        );

        if (FAILED(hr)) {
            return hr;
        }

        // Create render target view
        hr = device->CreateRenderTargetView(backBuffer, nullptr, outRTV);

        // Release back buffer (RTV holds its own reference)
        backBuffer->Release();

        if (SUCCEEDED(hr)) {
            std::cout << "✓ Render Target View created successfully\n";
        }

        return hr;
    }

    static HRESULT ResizeRenderTarget(
        ID3D11Device* device,
        ID3D11DeviceContext* context,
        IDXGISwapChain* swapChain,
        ID3D11RenderTargetView** ioRTV,
        UINT width,
        UINT height)
    {
        if (!device || !context || !swapChain || !ioRTV) {
            return E_INVALIDARG;
        }

        // Release old render target view
        if (*ioRTV) {
            (*ioRTV)->Release();
            *ioRTV = nullptr;
        }

        // Unbind render targets
        context->OMSetRenderTargets(0, nullptr, nullptr);

        // Resize swap chain buffers
        HRESULT hr = swapChain->ResizeBuffers(
            0,                          // Keep buffer count
            width,
            height,
            DXGI_FORMAT_UNKNOWN,       // Keep format
            0                           // Keep flags
        );

        if (FAILED(hr)) {
            return hr;
        }

        // Create new render target view
        return CreateRenderTargetView(device, swapChain, ioRTV);
    }
};
```

---

## 7. Viewport Setup

```cpp
class ViewportManager {
public:
    static D3D11_VIEWPORT CreateViewport(
        FLOAT width,
        FLOAT height,
        FLOAT minDepth = 0.0f,
        FLOAT maxDepth = 1.0f,
        FLOAT topLeftX = 0.0f,
        FLOAT topLeftY = 0.0f)
    {
        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = topLeftX;
        viewport.TopLeftY = topLeftY;
        viewport.Width = width;
        viewport.Height = height;
        viewport.MinDepth = minDepth;
        viewport.MaxDepth = maxDepth;
        return viewport;
    }

    static void SetViewport(
        ID3D11DeviceContext* context,
        const D3D11_VIEWPORT& viewport)
    {
        context->RSSetViewports(1, &viewport);
    }

    static void SetViewportFromWindow(
        ID3D11DeviceContext* context,
        HWND hWnd)
    {
        RECT rc;
        GetClientRect(hWnd, &rc);
        UINT width = rc.right - rc.left;
        UINT height = rc.bottom - rc.top;

        D3D11_VIEWPORT viewport = CreateViewport(
            (FLOAT)width,
            (FLOAT)height
        );
        SetViewport(context, viewport);
    }

    // Multiple viewports (split-screen, etc.)
    static void SetMultipleViewports(
        ID3D11DeviceContext* context,
        UINT width,
        UINT height,
        UINT rows,
        UINT cols)
    {
        FLOAT vpWidth = (FLOAT)width / cols;
        FLOAT vpHeight = (FLOAT)height / rows;

        std::vector<D3D11_VIEWPORT> viewports;

        for (UINT row = 0; row < rows; ++row) {
            for (UINT col = 0; col < cols; ++col) {
                D3D11_VIEWPORT vp = CreateViewport(
                    vpWidth,
                    vpHeight,
                    0.0f,
                    1.0f,
                    col * vpWidth,
                    row * vpHeight
                );
                viewports.push_back(vp);
            }
        }

        context->RSSetViewports((UINT)viewports.size(), viewports.data());
    }
};
```

---

## 8. Debug Layer and Error Handling

### Enabling Debug Layer:

```cpp
class DebugHelper {
public:
    static void EnableDebugLayer(ID3D11Device* device) {
#ifdef _DEBUG
        ID3D11Debug* debugDevice = nullptr;
        HRESULT hr = device->QueryInterface(__uuidof(ID3D11Debug),
                                             (void**)&debugDevice);
        if (SUCCEEDED(hr)) {
            ID3D11InfoQueue* infoQueue = nullptr;
            hr = debugDevice->QueryInterface(__uuidof(ID3D11InfoQueue),
                                              (void**)&infoQueue);
            if (SUCCEEDED(hr)) {
                infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
                infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);

                // Filter out unnecessary messages
                D3D11_MESSAGE_ID hide[] = {
                    D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                };

                D3D11_INFO_QUEUE_FILTER filter = {};
                filter.DenyList.NumIDs = _countof(hide);
                filter.DenyList.pIDList = hide;
                infoQueue->AddStorageFilterEntries(&filter);

                infoQueue->Release();
                std::cout << "✓ Debug layer enabled\n";
            }
            debugDevice->Release();
        }
#endif
    }

    static void ReportLiveObjects(ID3D11Device* device) {
#ifdef _DEBUG
        ID3D11Debug* debugDevice = nullptr;
        HRESULT hr = device->QueryInterface(__uuidof(ID3D11Debug),
                                             (void**)&debugDevice);
        if (SUCCEEDED(hr)) {
            debugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
            debugDevice->Release();
        }
#endif
    }
};
```

---

## 9. Building a Reusable D3D11 Class

```cpp
// D3D11Framework.h
#pragma once

#include <d3d11.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <string>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;

class D3D11Framework {
public:
    D3D11Framework();
    ~D3D11Framework();

    // Initialization
    bool Initialize(HWND hWnd, UINT width, UINT height, bool fullscreen = false);
    void Cleanup();

    // Frame operations
    void BeginFrame(const float clearColor[4]);
    void EndFrame();

    // Resize
    bool Resize(UINT width, UINT height);

    // Getters
    ID3D11Device* GetDevice() const { return m_device.Get(); }
    ID3D11DeviceContext* GetContext() const { return m_context.Get(); }
    IDXGISwapChain* GetSwapChain() const { return m_swapChain.Get(); }
    D3D_FEATURE_LEVEL GetFeatureLevel() const { return m_featureLevel; }

    // Info
    std::wstring GetAdapterDescription() const { return m_adapterDescription; }
    UINT GetWidth() const { return m_width; }
    UINT GetHeight() const { return m_height; }

private:
    bool CreateDeviceAndContext();
    bool CreateSwapChain(HWND hWnd);
    bool CreateRenderTargetView();
    void SetupViewport();

private:
    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_context;
    ComPtr<IDXGISwapChain> m_swapChain;
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;

    D3D_FEATURE_LEVEL m_featureLevel;
    std::wstring m_adapterDescription;

    UINT m_width;
    UINT m_height;
    bool m_fullscreen;
    HWND m_hWnd;
};
```

```cpp
// D3D11Framework.cpp
#include "D3D11Framework.h"
#include <iostream>

D3D11Framework::D3D11Framework()
    : m_featureLevel(D3D_FEATURE_LEVEL_11_0)
    , m_width(0)
    , m_height(0)
    , m_fullscreen(false)
    , m_hWnd(nullptr)
{
}

D3D11Framework::~D3D11Framework()
{
    Cleanup();
}

bool D3D11Framework::Initialize(HWND hWnd, UINT width, UINT height, bool fullscreen)
{
    m_hWnd = hWnd;
    m_width = width;
    m_height = height;
    m_fullscreen = fullscreen;

    if (!CreateDeviceAndContext()) {
        std::cerr << "Failed to create device and context\n";
        return false;
    }

    if (!CreateSwapChain(hWnd)) {
        std::cerr << "Failed to create swap chain\n";
        return false;
    }

    if (!CreateRenderTargetView()) {
        std::cerr << "Failed to create render target view\n";
        return false;
    }

    SetupViewport();

    DebugHelper::EnableDebugLayer(m_device.Get());

    std::cout << "✓ DirectX 11 Framework initialized successfully\n";
    std::wcout << L"  Adapter: " << m_adapterDescription << L"\n";
    std::cout << "  Resolution: " << m_width << "x" << m_height << "\n";

    return true;
}

bool D3D11Framework::CreateDeviceAndContext()
{
    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    createDeviceFlags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    HRESULT hr = D3D11CreateDevice(
        nullptr,                        // Default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &m_device,
        &m_featureLevel,
        &m_context
    );

    if (FAILED(hr)) {
        return false;
    }

    // Get adapter description
    ComPtr<IDXGIDevice> dxgiDevice;
    hr = m_device.As(&dxgiDevice);
    if (SUCCEEDED(hr)) {
        ComPtr<IDXGIAdapter> adapter;
        hr = dxgiDevice->GetAdapter(&adapter);
        if (SUCCEEDED(hr)) {
            DXGI_ADAPTER_DESC desc;
            adapter->GetDesc(&desc);
            m_adapterDescription = desc.Description;
        }
    }

    return true;
}

bool D3D11Framework::CreateSwapChain(HWND hWnd)
{
    SwapChainConfig config;
    config.width = m_width;
    config.height = m_height;
    config.windowed = !m_fullscreen;

    IDXGISwapChain* swapChain = nullptr;
    HRESULT hr = SwapChainCreator::CreateSwapChain(
        m_device.Get(),
        hWnd,
        config,
        &swapChain
    );

    if (SUCCEEDED(hr)) {
        m_swapChain.Attach(swapChain);
        return true;
    }

    return false;
}

bool D3D11Framework::CreateRenderTargetView()
{
    ID3D11RenderTargetView* rtv = nullptr;
    HRESULT hr = RenderTargetManager::CreateRenderTargetView(
        m_device.Get(),
        m_swapChain.Get(),
        &rtv
    );

    if (SUCCEEDED(hr)) {
        m_renderTargetView.Attach(rtv);
        m_context->OMSetRenderTargets(1, &rtv, nullptr);
        return true;
    }

    return false;
}

void D3D11Framework::SetupViewport()
{
    D3D11_VIEWPORT viewport = ViewportManager::CreateViewport(
        (FLOAT)m_width,
        (FLOAT)m_height
    );
    ViewportManager::SetViewport(m_context.Get(), viewport);
}

void D3D11Framework::BeginFrame(const float clearColor[4])
{
    m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
}

void D3D11Framework::EndFrame()
{
    m_swapChain->Present(1, 0); // VSync enabled
}

bool D3D11Framework::Resize(UINT width, UINT height)
{
    if (width == 0 || height == 0) {
        return false;
    }

    m_width = width;
    m_height = height;

    ID3D11RenderTargetView* rtv = m_renderTargetView.Get();
    HRESULT hr = RenderTargetManager::ResizeRenderTarget(
        m_device.Get(),
        m_context.Get(),
        m_swapChain.Get(),
        &rtv,
        width,
        height
    );

    if (FAILED(hr)) {
        return false;
    }

    m_renderTargetView.Attach(rtv);
    m_context->OMSetRenderTargets(1, &rtv, nullptr);
    SetupViewport();

    return true;
}

void D3D11Framework::Cleanup()
{
    if (m_context) {
        m_context->ClearState();
        m_context->Flush();
    }

    m_renderTargetView.Reset();
    m_swapChain.Reset();
    m_context.Reset();

#ifdef _DEBUG
    if (m_device) {
        DebugHelper::ReportLiveObjects(m_device.Get());
    }
#endif

    m_device.Reset();
}
```

---

## 10. Complete Working Example

```cpp
// Main.cpp - Complete DirectX 11 Application
#include <windows.h>
#include "D3D11Framework.h"
#include <iostream>

D3D11Framework* g_pD3D = nullptr;
HWND g_hWnd = nullptr;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    // Register window class
    WNDCLASSEX wc = {sizeof(WNDCLASSEX)};
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"DX11FrameworkClass";

    if (!RegisterClassEx(&wc)) return -1;

    // Create window
    RECT rc = {0, 0, 1280, 720};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    g_hWnd = CreateWindow(
        wc.lpszClassName,
        L"DirectX 11 Framework",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!g_hWnd) return -1;

    // Initialize DirectX
    g_pD3D = new D3D11Framework();
    if (!g_pD3D->Initialize(g_hWnd, 1280, 720, false)) {
        delete g_pD3D;
        return -1;
    }

    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    // Main loop
    MSG msg = {0};
    float clearColor[4] = {0.2f, 0.3f, 0.4f, 1.0f};

    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            g_pD3D->BeginFrame(clearColor);
            // Render here
            g_pD3D->EndFrame();
        }
    }

    delete g_pD3D;
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_SIZE:
            if (g_pD3D && wParam != SIZE_MINIMIZED) {
                UINT width = LOWORD(lParam);
                UINT height = HIWORD(lParam);
                g_pD3D->Resize(width, height);
            }
            return 0;

        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) {
                PostQuitMessage(0);
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
```

---

## 11. Exercises and Best Practices

### Exercise 1: Multiple Adapter Selection
Create a UI that lets users select which GPU to use if they have multiple adapters.

### Exercise 2: Display Mode Enumeration
Enumerate all supported display modes and allow selection:

```cpp
IDXGIOutput* output = nullptr;
adapter->EnumOutputs(0, &output);

UINT numModes = 0;
output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
                            0, &numModes, nullptr);

std::vector<DXGI_MODE_DESC> modes(numModes);
output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
                            0, &numModes, modes.data());
```

### Exercise 3: VSync Toggle
Implement runtime VSync toggle:

```cpp
m_swapChain->Present(m_vsyncEnabled ? 1 : 0, 0);
```

### Best Practices:

1. **Use ComPtr**: Automatic reference counting
2. **Enable Debug Layer**: In debug builds only
3. **Check HRESULT**: Always validate return values
4. **Handle Resize**: Properly resize swap chain buffers
5. **Release Resources**: In reverse creation order
6. **Feature Level**: Support multiple feature levels
7. **MSAA Support**: Check quality levels before use
8. **Full-Screen**: Use exclusive full-screen for best performance
9. **Multiple Monitors**: Handle adapter/output selection
10. **Error Messages**: Provide clear error messages

---

## Key Takeaways

1. ✅ DirectX initialization requires multiple steps in correct order
2. ✅ Adapter enumeration allows GPU selection
3. ✅ Feature levels ensure compatibility across hardware
4. ✅ Swap chain manages double/triple buffering
5. ✅ Render target view provides access to back buffer
6. ✅ Viewport defines rendering region
7. ✅ Debug layer helps catch errors during development
8. ✅ ComPtr simplifies resource management
9. ✅ Window resize requires swap chain buffer resize
10. ✅ Reusable framework class simplifies initialization

---

## Next Lesson Preview

In **Lesson 43: Swap Chain and Render Targets**, we'll explore:
- Advanced swap chain configurations
- Multiple render targets (MRT)
- MSAA configuration and quality levels
- Full-screen exclusive mode
- Render target formats and conversions
- Gamma correction and sRGB

---

## Summary Checklist

Before moving to Lesson 43, ensure you can:
- [ ] Enumerate and select graphics adapters
- [ ] Create DirectX 11 device and context
- [ ] Configure and create swap chain
- [ ] Create render target view from back buffer
- [ ] Set up viewport
- [ ] Handle window resize properly
- [ ] Enable debug layer in debug builds
- [ ] Use ComPtr for resource management
- [ ] Build a reusable D3D11 framework class
- [ ] Clear and present frames

**Excellent work on completing Lesson 42!**

You now have a solid understanding of DirectX 11 initialization and a reusable framework to build upon!
