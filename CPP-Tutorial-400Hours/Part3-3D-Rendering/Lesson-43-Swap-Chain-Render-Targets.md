# Lesson 43: Swap Chain and Render Targets

**Duration**: 5 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding Swap Chains
2. Double vs Triple Buffering
3. Swap Effects and Flip Models
4. Multiple Render Targets (MRT)
5. MSAA Configuration
6. Render Target Formats
7. sRGB and Gamma Correction
8. Full-Screen Exclusive Mode
9. Window Resize Handling
10. Complete Examples and Best Practices

---

## 1. Understanding Swap Chains

A **swap chain** is a collection of buffers used for displaying frames to the user.

### Basic Concept:

```
Front Buffer (Display) ←→ Back Buffer (Rendering)
        ↑                           ↓
        └─────── Present() ─────────┘
```

### Key Components:

```cpp
struct SwapChainComponents {
    // Buffers
    ID3D11Texture2D* backBuffer;       // Where we render
    ID3D11RenderTargetView* rtv;       // View for rendering

    // Configuration
    DXGI_FORMAT format;                 // Pixel format
    UINT width, height;                 // Resolution
    UINT bufferCount;                   // 1, 2, or 3
    DXGI_SWAP_EFFECT swapEffect;       // How buffers swap
    UINT refreshRate;                   // Hz (60, 144, etc.)
};
```

### Complete Swap Chain Class:

```cpp
#include <d3d11.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <vector>

using Microsoft::WRL::ComPtr;

class SwapChainManager {
public:
    struct Config {
        UINT width = 1920;
        UINT height = 1080;
        DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
        UINT bufferCount = 2;
        UINT refreshRate = 60;
        UINT msaaSampleCount = 1;
        UINT msaaQuality = 0;
        DXGI_SWAP_EFFECT swapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        bool windowed = true;
        bool allowTearing = false;
        bool sRGB = false;
    };

    SwapChainManager(ID3D11Device* device, HWND hWnd);
    ~SwapChainManager();

    bool Create(const Config& config);
    bool Resize(UINT width, UINT height);
    void Present(bool vsync);

    ID3D11RenderTargetView* GetRTV() const { return m_rtv.Get(); }
    IDXGISwapChain1* GetSwapChain() const { return m_swapChain.Get(); }

    const Config& GetConfig() const { return m_config; }
    bool IsFullScreen() const;
    bool SetFullScreen(bool fullscreen);

private:
    bool CreateSwapChain();
    bool CreateRenderTargetView();
    void CleanupRenderTarget();

private:
    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_context;
    ComPtr<IDXGISwapChain1> m_swapChain;
    ComPtr<ID3D11RenderTargetView> m_rtv;
    ComPtr<ID3D11Texture2D> m_backBuffer;

    HWND m_hWnd;
    Config m_config;
    bool m_tearingSupport;
};

SwapChainManager::SwapChainManager(ID3D11Device* device, HWND hWnd)
    : m_hWnd(hWnd)
    , m_tearingSupport(false)
{
    m_device = device;
    device->GetImmediateContext(&m_context);

    // Check tearing support
    ComPtr<IDXGIFactory5> factory5;
    if (SUCCEEDED(CreateDXGIFactory2(0, IID_PPV_ARGS(&factory5)))) {
        BOOL allowTearing = FALSE;
        factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING,
                                       &allowTearing, sizeof(allowTearing));
        m_tearingSupport = (allowTearing == TRUE);
    }
}

SwapChainManager::~SwapChainManager()
{
    CleanupRenderTarget();
    if (m_swapChain) {
        m_swapChain->SetFullscreenState(FALSE, nullptr);
    }
}

bool SwapChainManager::Create(const Config& config)
{
    m_config = config;

    if (!CreateSwapChain()) {
        return false;
    }

    if (!CreateRenderTargetView()) {
        return false;
    }

    return true;
}

bool SwapChainManager::CreateSwapChain()
{
    // Get DXGI factory
    ComPtr<IDXGIDevice> dxgiDevice;
    ComPtr<IDXGIAdapter> adapter;
    ComPtr<IDXGIFactory2> factory;

    if (FAILED(m_device.As(&dxgiDevice))) return false;
    if (FAILED(dxgiDevice->GetAdapter(&adapter))) return false;
    if (FAILED(adapter->GetParent(IID_PPV_ARGS(&factory)))) return false;

    // Configure swap chain description
    DXGI_SWAP_CHAIN_DESC1 desc = {};
    desc.Width = m_config.width;
    desc.Height = m_config.height;
    desc.Format = m_config.sRGB ? MakeSRGB(m_config.format) : m_config.format;
    desc.Stereo = FALSE;
    desc.SampleDesc.Count = m_config.msaaSampleCount;
    desc.SampleDesc.Quality = m_config.msaaQuality;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = m_config.bufferCount;
    desc.Scaling = DXGI_SCALING_STRETCH;
    desc.SwapEffect = m_config.swapEffect;
    desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    desc.Flags = 0;

    if (m_config.allowTearing && m_tearingSupport) {
        desc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
    }

    // Full-screen description
    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsDesc = {};
    fsDesc.RefreshRate.Numerator = m_config.refreshRate;
    fsDesc.RefreshRate.Denominator = 1;
    fsDesc.Windowed = m_config.windowed;
    fsDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    fsDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    // Create swap chain
    HRESULT hr = factory->CreateSwapChainForHwnd(
        m_device.Get(),
        m_hWnd,
        &desc,
        &fsDesc,
        nullptr,
        &m_swapChain
    );

    if (FAILED(hr)) {
        return false;
    }

    // Disable Alt+Enter
    factory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);

    return true;
}

bool SwapChainManager::CreateRenderTargetView()
{
    // Get back buffer
    HRESULT hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&m_backBuffer));
    if (FAILED(hr)) {
        return false;
    }

    // Create RTV
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = m_config.sRGB ? MakeSRGB(m_config.format) : m_config.format;
    rtvDesc.ViewDimension = m_config.msaaSampleCount > 1
                             ? D3D11_RTV_DIMENSION_TEXTURE2DMS
                             : D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0;

    hr = m_device->CreateRenderTargetView(m_backBuffer.Get(), &rtvDesc, &m_rtv);
    return SUCCEEDED(hr);
}

void SwapChainManager::CleanupRenderTarget()
{
    m_rtv.Reset();
    m_backBuffer.Reset();
}

bool SwapChainManager::Resize(UINT width, UINT height)
{
    if (width == 0 || height == 0) return false;

    m_config.width = width;
    m_config.height = height;

    // Release render target
    CleanupRenderTarget();
    m_context->ClearState();
    m_context->Flush();

    // Resize buffers
    HRESULT hr = m_swapChain->ResizeBuffers(
        m_config.bufferCount,
        width,
        height,
        m_config.format,
        m_config.allowTearing && m_tearingSupport
            ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0
    );

    if (FAILED(hr)) {
        return false;
    }

    // Recreate render target
    return CreateRenderTargetView();
}

void SwapChainManager::Present(bool vsync)
{
    UINT syncInterval = vsync ? 1 : 0;
    UINT flags = 0;

    if (!vsync && m_tearingSupport && m_config.allowTearing) {
        flags = DXGI_PRESENT_ALLOW_TEARING;
    }

    m_swapChain->Present(syncInterval, flags);
}

bool SwapChainManager::IsFullScreen() const
{
    BOOL fullscreen = FALSE;
    if (m_swapChain) {
        m_swapChain->GetFullscreenState(&fullscreen, nullptr);
    }
    return fullscreen == TRUE;
}

bool SwapChainManager::SetFullScreen(bool fullscreen)
{
    if (!m_swapChain) return false;

    HRESULT hr = m_swapChain->SetFullscreenState(fullscreen, nullptr);
    return SUCCEEDED(hr);
}

// Helper function
static DXGI_FORMAT MakeSRGB(DXGI_FORMAT format)
{
    switch (format) {
        case DXGI_FORMAT_R8G8B8A8_UNORM:
            return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        case DXGI_FORMAT_B8G8R8A8_UNORM:
            return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
        case DXGI_FORMAT_B8G8R8X8_UNORM:
            return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
        default:
            return format;
    }
}
```

---

## 2. Double vs Triple Buffering

### Double Buffering (2 buffers):

```
Frame N-1 (Display) → Front Buffer
Frame N   (Render)  → Back Buffer

Present() → Swap buffers
```

**Pros:**
- Lower memory usage
- Lower latency
- Standard approach

**Cons:**
- Can stall if GPU falls behind

### Triple Buffering (3 buffers):

```
Frame N-2 (Display) → Front Buffer
Frame N-1 (Queue)   → Middle Buffer
Frame N   (Render)  → Back Buffer
```

**Pros:**
- No stalling
- Smoother frame pacing
- Better for VRR displays

**Cons:**
- Higher memory usage
- Slightly higher latency
- More GPU memory

### Configuration:

```cpp
// Double buffering
DXGI_SWAP_CHAIN_DESC1 desc = {};
desc.BufferCount = 2;  // Recommended

// Triple buffering
desc.BufferCount = 3;  // For very high frame rates
```

---

## 3. Swap Effects and Flip Models

### Available Swap Effects:

```cpp
enum DXGI_SWAP_EFFECT {
    DXGI_SWAP_EFFECT_DISCARD = 0,         // Windows 7
    DXGI_SWAP_EFFECT_SEQUENTIAL = 1,       // Legacy
    DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL = 3,  // Windows 8+
    DXGI_SWAP_EFFECT_FLIP_DISCARD = 4      // Windows 10+ (Best)
};
```

### Comparison:

| Swap Effect | OS | Buffers | Tearing Support | Best For |
|-------------|----|----|----------------|----------|
| DISCARD | Win 7+ | 1 | No | Legacy |
| SEQUENTIAL | Win 7+ | ≥2 | No | Legacy |
| FLIP_SEQUENTIAL | Win 8+ | ≥2 | No | Modern apps |
| FLIP_DISCARD | Win 10+ | ≥2 | Yes | Gaming (Best) |

### Flip Model Benefits:

1. **Lower latency**
2. **Better performance**
3. **Tearing support** (for high refresh rate)
4. **HDR support**
5. **Full-screen optimizations**

### Tearing Support:

```cpp
class TearingSupport {
public:
    static bool CheckTearingSupport() {
        ComPtr<IDXGIFactory5> factory;
        if (FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(&factory)))) {
            return false;
        }

        BOOL allowTearing = FALSE;
        HRESULT hr = factory->CheckFeatureSupport(
            DXGI_FEATURE_PRESENT_ALLOW_TEARING,
            &allowTearing,
            sizeof(allowTearing)
        );

        return SUCCEEDED(hr) && allowTearing;
    }

    static void PresentWithTearing(IDXGISwapChain1* swapChain,
                                    bool vsync,
                                    bool tearingSupported)
    {
        UINT syncInterval = vsync ? 1 : 0;
        UINT flags = 0;

        if (!vsync && tearingSupported) {
            flags = DXGI_PRESENT_ALLOW_TEARING;
        }

        swapChain->Present(syncInterval, flags);
    }
};
```

---

## 4. Multiple Render Targets (MRT)

Render to multiple textures simultaneously.

### Use Cases:
- Deferred rendering (G-Buffer)
- Post-processing effects
- HDR + Bloom
- Screen-space effects

### MRT Setup:

```cpp
class MultipleRenderTargets {
public:
    static const UINT MAX_RENDER_TARGETS = 8;

    struct MRTConfig {
        UINT width;
        UINT height;
        DXGI_FORMAT formats[MAX_RENDER_TARGETS];
        UINT count;
    };

    MultipleRenderTargets(ID3D11Device* device)
        : m_device(device)
    {
        device->GetImmediateContext(&m_context);
    }

    bool Create(const MRTConfig& config) {
        m_config = config;

        for (UINT i = 0; i < config.count; ++i) {
            if (!CreateRenderTarget(i, config.formats[i])) {
                return false;
            }
        }

        return true;
    }

    void SetRenderTargets(ID3D11DepthStencilView* dsv = nullptr) {
        ID3D11RenderTargetView* rtvs[MAX_RENDER_TARGETS] = {};
        for (UINT i = 0; i < m_config.count; ++i) {
            rtvs[i] = m_rtvs[i].Get();
        }

        m_context->OMSetRenderTargets(m_config.count, rtvs, dsv);
    }

    void Clear(const float clearColors[][4]) {
        for (UINT i = 0; i < m_config.count; ++i) {
            m_context->ClearRenderTargetView(m_rtvs[i].Get(), clearColors[i]);
        }
    }

    ID3D11RenderTargetView* GetRTV(UINT index) {
        return index < m_config.count ? m_rtvs[index].Get() : nullptr;
    }

    ID3D11ShaderResourceView* GetSRV(UINT index) {
        return index < m_config.count ? m_srvs[index].Get() : nullptr;
    }

private:
    bool CreateRenderTarget(UINT index, DXGI_FORMAT format) {
        // Create texture
        D3D11_TEXTURE2D_DESC texDesc = {};
        texDesc.Width = m_config.width;
        texDesc.Height = m_config.height;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = format;
        texDesc.SampleDesc.Count = 1;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

        ComPtr<ID3D11Texture2D> texture;
        HRESULT hr = m_device->CreateTexture2D(&texDesc, nullptr, &texture);
        if (FAILED(hr)) return false;

        // Create RTV
        hr = m_device->CreateRenderTargetView(texture.Get(), nullptr,
                                               &m_rtvs[index]);
        if (FAILED(hr)) return false;

        // Create SRV
        hr = m_device->CreateShaderResourceView(texture.Get(), nullptr,
                                                  &m_srvs[index]);
        if (FAILED(hr)) return false;

        m_textures[index] = texture;
        return true;
    }

private:
    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_context;

    MRTConfig m_config;
    ComPtr<ID3D11Texture2D> m_textures[MAX_RENDER_TARGETS];
    ComPtr<ID3D11RenderTargetView> m_rtvs[MAX_RENDER_TARGETS];
    ComPtr<ID3D11ShaderResourceView> m_srvs[MAX_RENDER_TARGETS];
};

// Example: Deferred rendering G-Buffer
void SetupGBuffer(ID3D11Device* device) {
    MultipleRenderTargets::MRTConfig config;
    config.width = 1920;
    config.height = 1080;
    config.count = 4;
    config.formats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;      // Albedo
    config.formats[1] = DXGI_FORMAT_R16G16B16A16_FLOAT;  // Normal
    config.formats[2] = DXGI_FORMAT_R8G8B8A8_UNORM;      // Specular
    config.formats[3] = DXGI_FORMAT_R32_FLOAT;           // Depth

    MultipleRenderTargets gBuffer(device);
    gBuffer.Create(config);
}
```

---

## 5. MSAA Configuration

Multi-Sample Anti-Aliasing (MSAA) reduces jagged edges.

### Quality Level Check:

```cpp
class MSAAHelper {
public:
    struct MSAAQuality {
        UINT sampleCount;
        UINT maxQuality;
        bool supported;
    };

    static std::vector<MSAAQuality> GetSupportedMSAA(
        ID3D11Device* device,
        DXGI_FORMAT format)
    {
        std::vector<MSAAQuality> qualities;

        UINT sampleCounts[] = {1, 2, 4, 8, 16};

        for (UINT sampleCount : sampleCounts) {
            UINT qualityLevels = 0;
            HRESULT hr = device->CheckMultisampleQualityLevels(
                format,
                sampleCount,
                &qualityLevels
            );

            MSAAQuality quality;
            quality.sampleCount = sampleCount;
            quality.maxQuality = qualityLevels;
            quality.supported = SUCCEEDED(hr) && qualityLevels > 0;

            if (quality.supported) {
                qualities.push_back(quality);
            }
        }

        return qualities;
    }

    static void PrintMSAASupport(ID3D11Device* device, DXGI_FORMAT format) {
        auto qualities = GetSupportedMSAA(device, format);

        std::cout << "MSAA Support for format " << format << ":\n";
        for (const auto& q : qualities) {
            std::cout << "  " << q.sampleCount << "x MSAA: "
                      << q.maxQuality << " quality levels\n";
        }
    }

    static bool ValidateMSAA(ID3D11Device* device,
                              DXGI_FORMAT format,
                              UINT sampleCount,
                              UINT quality)
    {
        UINT qualityLevels = 0;
        HRESULT hr = device->CheckMultisampleQualityLevels(
            format,
            sampleCount,
            &qualityLevels
        );

        return SUCCEEDED(hr) && quality < qualityLevels;
    }
};

// Example usage
void ConfigureMSAA(ID3D11Device* device) {
    DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

    // Check support
    MSAAHelper::PrintMSAASupport(device, format);

    // Validate before use
    UINT sampleCount = 4;
    UINT quality = 0;

    if (MSAAHelper::ValidateMSAA(device, format, sampleCount, quality)) {
        std::cout << sampleCount << "x MSAA validated\n";
        // Use in swap chain
    }
}
```

### MSAA Swap Chain:

```cpp
DXGI_SWAP_CHAIN_DESC1 desc = {};
desc.SampleDesc.Count = 4;     // 4x MSAA
desc.SampleDesc.Quality = 0;   // Use quality level 0
```

### Resolving MSAA:

```cpp
void ResolveMSAA(ID3D11DeviceContext* context,
                 ID3D11Texture2D* msaaTexture,
                 ID3D11Texture2D* resolvedTexture,
                 DXGI_FORMAT format)
{
    context->ResolveSubresource(
        resolvedTexture,  // Destination
        0,                // Dest subresource
        msaaTexture,      // Source
        0,                // Source subresource
        format            // Format
    );
}
```

---

## 6. Render Target Formats

### Common Formats:

```cpp
enum CommonRenderTargetFormats {
    // 8-bit per channel
    RGBA_8_UNORM        = DXGI_FORMAT_R8G8B8A8_UNORM,      // Most common
    RGBA_8_UNORM_SRGB   = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, // Gamma corrected
    BGRA_8_UNORM        = DXGI_FORMAT_B8G8R8A8_UNORM,      // Direct2D compat

    // 16-bit per channel (HDR)
    RGBA_16_FLOAT       = DXGI_FORMAT_R16G16B16A16_FLOAT,  // HDR rendering
    RGBA_16_UNORM       = DXGI_FORMAT_R16G16B16A16_UNORM,  // High precision

    // 32-bit per channel
    RGBA_32_FLOAT       = DXGI_FORMAT_R32G32B32A32_FLOAT,  // Max precision

    // Single/dual channel
    R_32_FLOAT          = DXGI_FORMAT_R32_FLOAT,           // Depth, shadow maps
    RG_16_FLOAT         = DXGI_FORMAT_R16G16_FLOAT,        // Normals, velocity

    // HDR10
    RGB_10_A2_UNORM     = DXGI_FORMAT_R10G10B10A2_UNORM,   // HDR10 display
};
```

### Format Selection Guide:

| Use Case | Format | Reason |
|----------|--------|--------|
| Standard rendering | R8G8B8A8_UNORM | Balance of quality/performance |
| UI rendering | B8G8R8A8_UNORM | Direct2D compatibility |
| HDR rendering | R16G16B16A16_FLOAT | Wide color gamut |
| Shadow maps | R32_FLOAT | Depth precision |
| Normal maps | R16G16_FLOAT | Signed values |
| G-Buffer albedo | R8G8B8A8_UNORM | Color data |
| G-Buffer normal | R16G16B16A16_FLOAT | Precision |

### Format Capabilities:

```cpp
class FormatCapabilities {
public:
    static void CheckFormatSupport(ID3D11Device* device, DXGI_FORMAT format) {
        UINT support = 0;
        device->CheckFormatSupport(format, &support);

        std::cout << "Format " << format << " supports:\n";

        if (support & D3D11_FORMAT_SUPPORT_RENDER_TARGET)
            std::cout << "  - Render Target\n";
        if (support & D3D11_FORMAT_SUPPORT_TEXTURE2D)
            std::cout << "  - Texture2D\n";
        if (support & D3D11_FORMAT_SUPPORT_SHADER_SAMPLE)
            std::cout << "  - Shader Sampling\n";
        if (support & D3D11_FORMAT_SUPPORT_DEPTH_STENCIL)
            std::cout << "  - Depth Stencil\n";
        if (support & D3D11_FORMAT_SUPPORT_BLENDABLE)
            std::cout << "  - Blending\n";
        if (support & D3D11_FORMAT_SUPPORT_MULTISAMPLE_RESOLVE)
            std::cout << "  - MSAA Resolve\n";
    }
};
```

---

## 7. sRGB and Gamma Correction

### Understanding sRGB:

```cpp
// Linear space (shader math)
float3 linearColor = pow(srgbColor, 2.2f);

// sRGB space (display)
float3 srgbColor = pow(linearColor, 1.0f / 2.2f);
```

### Automatic sRGB Conversion:

```cpp
// Swap chain with sRGB
DXGI_SWAP_CHAIN_DESC1 desc = {};
desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;  // Auto gamma correction

// Texture with sRGB
D3D11_TEXTURE2D_DESC texDesc = {};
texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
```

### sRGB Best Practices:

```cpp
class GammaCorrection {
public:
    // Render pipeline:
    // 1. Render to linear format
    // 2. Convert to sRGB for display

    static void SetupLinearRenderTarget(ID3D11Device* device,
                                         UINT width, UINT height,
                                         ID3D11RenderTargetView** outRTV,
                                         ID3D11ShaderResourceView** outSRV)
    {
        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;  // Linear HDR
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

        ComPtr<ID3D11Texture2D> texture;
        device->CreateTexture2D(&desc, nullptr, &texture);
        device->CreateRenderTargetView(texture.Get(), nullptr, outRTV);
        device->CreateShaderResourceView(texture.Get(), nullptr, outSRV);
    }

    // Pixel shader for tone mapping + gamma correction
    static const char* GetToneMappingShader() {
        return R"(
            Texture2D hdrTexture : register(t0);
            SamplerState samplerState : register(s0);

            struct PS_INPUT {
                float4 position : SV_POSITION;
                float2 texcoord : TEXCOORD;
            };

            float4 main(PS_INPUT input) : SV_TARGET {
                float3 hdrColor = hdrTexture.Sample(samplerState, input.texcoord).rgb;

                // Tone mapping (Reinhard)
                float3 mapped = hdrColor / (hdrColor + 1.0f);

                // Gamma correction (done automatically if target is _SRGB format)
                // Otherwise: mapped = pow(mapped, 1.0f / 2.2f);

                return float4(mapped, 1.0f);
            }
        )";
    }
};
```

---

## 8. Full-Screen Exclusive Mode

### Benefits:
- Lower latency
- Better performance
- True VSync control
- Exclusive GPU access

### Implementation:

```cpp
class FullScreenManager {
public:
    static bool EnterFullScreen(IDXGISwapChain1* swapChain) {
        HRESULT hr = swapChain->SetFullscreenState(TRUE, nullptr);
        return SUCCEEDED(hr);
    }

    static bool ExitFullScreen(IDXGISwapChain1* swapChain) {
        HRESULT hr = swapChain->SetFullscreenState(FALSE, nullptr);
        return SUCCEEDED(hr);
    }

    static bool IsFullScreen(IDXGISwapChain1* swapChain) {
        BOOL fullscreen = FALSE;
        swapChain->GetFullscreenState(&fullscreen, nullptr);
        return fullscreen == TRUE;
    }

    static bool ToggleFullScreen(IDXGISwapChain1* swapChain) {
        bool isFullScreen = IsFullScreen(swapChain);
        return isFullScreen ? ExitFullScreen(swapChain)
                           : EnterFullScreen(swapChain);
    }

    // Get available display modes
    static std::vector<DXGI_MODE_DESC> GetDisplayModes(IDXGISwapChain1* swapChain) {
        std::vector<DXGI_MODE_DESC> modes;

        ComPtr<IDXGIOutput> output;
        swapChain->GetContainingOutput(&output);

        if (!output) return modes;

        UINT numModes = 0;
        DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

        // Get count
        output->GetDisplayModeList(format, 0, &numModes, nullptr);

        if (numModes > 0) {
            modes.resize(numModes);
            output->GetDisplayModeList(format, 0, &numModes, modes.data());
        }

        return modes;
    }

    static void PrintDisplayModes(IDXGISwapChain1* swapChain) {
        auto modes = GetDisplayModes(swapChain);

        std::cout << "Available display modes (" << modes.size() << "):\n";
        for (const auto& mode : modes) {
            float refreshRate = (float)mode.RefreshRate.Numerator /
                                mode.RefreshRate.Denominator;
            std::cout << "  " << mode.Width << "x" << mode.Height
                      << " @ " << refreshRate << " Hz\n";
        }
    }
};
```

---

## 9. Window Resize Handling

### Complete Resize Implementation:

```cpp
class ResizeHandler {
public:
    static bool HandleResize(
        IDXGISwapChain1* swapChain,
        ID3D11DeviceContext* context,
        ID3D11RenderTargetView** ioRTV,
        ID3D11DepthStencilView** ioDSV,
        UINT newWidth,
        UINT newHeight)
    {
        if (newWidth == 0 || newHeight == 0) {
            return false;
        }

        // 1. Release dependent resources
        if (*ioRTV) {
            (*ioRTV)->Release();
            *ioRTV = nullptr;
        }

        if (ioDSV) {
            (*ioDSV)->Release();
            *ioDSV = nullptr;
        }

        // 2. Unbind render targets
        context->OMSetRenderTargets(0, nullptr, nullptr);
        context->ClearState();
        context->Flush();

        // 3. Resize swap chain buffers
        HRESULT hr = swapChain->ResizeBuffers(
            0,                      // Keep buffer count
            newWidth,
            newHeight,
            DXGI_FORMAT_UNKNOWN,   // Keep format
            0                       // Keep flags
        );

        if (FAILED(hr)) {
            return false;
        }

        // 4. Recreate render target view
        ComPtr<ID3D11Texture2D> backBuffer;
        hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
        if (FAILED(hr)) return false;

        ComPtr<ID3D11Device> device;
        context->GetDevice(&device);

        hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, ioRTV);
        if (FAILED(hr)) return false;

        // 5. Recreate depth stencil view (if needed)
        if (ioDSV) {
            // Recreate depth buffer (code omitted for brevity)
        }

        // 6. Update viewport
        D3D11_VIEWPORT viewport = {};
        viewport.Width = (FLOAT)newWidth;
        viewport.Height = (FLOAT)newHeight;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        context->RSSetViewports(1, &viewport);

        return true;
    }
};

// WndProc integration
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_SIZE:
            if (g_swapChain && wParam != SIZE_MINIMIZED) {
                UINT width = LOWORD(lParam);
                UINT height = HIWORD(lParam);

                ResizeHandler::HandleResize(
                    g_swapChain,
                    g_context,
                    &g_rtv,
                    &g_dsv,
                    width,
                    height
                );
            }
            return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
```

---

## 10. Complete Examples and Best Practices

### Complete Application Example:

```cpp
// DXSwapChainApp.cpp
#include <windows.h>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <iostream>

using Microsoft::WRL::ComPtr;

class DXApp {
public:
    bool Initialize(HWND hWnd, UINT width, UINT height) {
        m_hWnd = hWnd;

        // Create device
        if (!CreateDevice()) return false;

        // Create swap chain
        SwapChainManager::Config config;
        config.width = width;
        config.height = height;
        config.bufferCount = 2;
        config.swapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        config.allowTearing = true;

        m_swapChain = std::make_unique<SwapChainManager>(m_device.Get(), hWnd);
        if (!m_swapChain->Create(config)) return false;

        return true;
    }

    void Render() {
        // Clear
        float clearColor[4] = {0.1f, 0.2f, 0.4f, 1.0f};
        m_context->ClearRenderTargetView(m_swapChain->GetRTV(), clearColor);

        // Bind render target
        ID3D11RenderTargetView* rtv = m_swapChain->GetRTV();
        m_context->OMSetRenderTargets(1, &rtv, nullptr);

        // Draw here...

        // Present
        m_swapChain->Present(true); // VSync on
    }

    void Resize(UINT width, UINT height) {
        m_swapChain->Resize(width, height);
    }

private:
    bool CreateDevice() {
        D3D_FEATURE_LEVEL featureLevel;
        HRESULT hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            D3D11_CREATE_DEVICE_DEBUG,
            nullptr, 0,
            D3D11_SDK_VERSION,
            &m_device,
            &featureLevel,
            &m_context
        );
        return SUCCEEDED(hr);
    }

private:
    HWND m_hWnd;
    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_context;
    std::unique_ptr<SwapChainManager> m_swapChain;
};

// Global app instance
DXApp* g_app = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_SIZE:
            if (g_app && wParam != SIZE_MINIMIZED) {
                g_app->Resize(LOWORD(lParam), HIWORD(lParam));
            }
            return 0;

        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) PostQuitMessage(0);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    // Create window
    WNDCLASSEX wc = {sizeof(WNDCLASSEX)};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"DXSwapChainApp";
    RegisterClassEx(&wc);

    HWND hWnd = CreateWindow(wc.lpszClassName, L"Swap Chain Demo",
                              WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                              1280, 720, nullptr, nullptr, hInstance, nullptr);

    // Initialize app
    g_app = new DXApp();
    if (!g_app->Initialize(hWnd, 1280, 720)) {
        delete g_app;
        return -1;
    }

    ShowWindow(hWnd, nCmdShow);

    // Main loop
    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            g_app->Render();
        }
    }

    delete g_app;
    return 0;
}
```

### Best Practices:

1. **Use Flip Model**: FLIP_DISCARD for best performance
2. **Enable Tearing**: For high refresh rate monitors
3. **Handle Resize**: Always resize swap chain buffers
4. **sRGB for Display**: Use _SRGB formats for final output
5. **HDR for Rendering**: Use floating-point formats internally
6. **Check MSAA Support**: Before enabling MSAA
7. **MRT for Deferred**: Use multiple render targets efficiently
8. **VSync Control**: Allow users to toggle VSync
9. **Full-Screen**: Implement exclusive full-screen mode
10. **Clean Shutdown**: Release all resources properly

---

## Key Takeaways

1. ✅ Swap chains manage double/triple buffering
2. ✅ Flip models provide best performance on modern Windows
3. ✅ Tearing support enables high refresh rates
4. ✅ Multiple render targets enable advanced techniques
5. ✅ MSAA quality levels must be checked before use
6. ✅ sRGB formats provide automatic gamma correction
7. ✅ Full-screen exclusive mode reduces latency
8. ✅ Window resize requires swap chain buffer resize
9. ✅ Format selection depends on use case
10. ✅ Proper resource management prevents leaks

---

## Next Lesson Preview

In **Lesson 44: DirectX Device and Context**, we'll explore:
- Device capabilities and queries
- Immediate vs deferred contexts
- Multi-threaded rendering
- Resource creation and management
- Command lists and execution
- Performance optimization techniques

---

## Summary Checklist

Before moving to Lesson 44, ensure you can:
- [ ] Configure swap chain with appropriate settings
- [ ] Implement double and triple buffering
- [ ] Use flip models for best performance
- [ ] Create and use multiple render targets
- [ ] Configure and validate MSAA settings
- [ ] Choose appropriate render target formats
- [ ] Implement sRGB/gamma correction
- [ ] Toggle full-screen exclusive mode
- [ ] Handle window resize properly
- [ ] Present frames with VSync control

**Excellent work on completing Lesson 43!**

You now master swap chain configuration and render target management!
