# Lesson 45: Vertex Buffers in DirectX

**Duration**: 5 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding Vertex Buffers
2. Creating Vertex Buffers
3. Vertex Structures and Formats
4. Static vs Dynamic Buffers
5. Updating Buffer Data
6. Multiple Vertex Streams
7. Drawing with Vertex Buffers
8. Complete Working Examples

---

## 1. Understanding Vertex Buffers

A **vertex buffer** is a memory buffer containing vertex data (positions, colors, normals, etc.) used for rendering.

### Vertex Data Flow:

```
CPU Memory (Vertex Array)
        ↓
    CreateBuffer()
        ↓
GPU Memory (Vertex Buffer)
        ↓
    IASetVertexBuffers()
        ↓
    Input Assembler
        ↓
    Vertex Shader
```

---

## 2. Creating Vertex Buffers

### Complete Vertex Buffer Class:

```cpp
#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

class VertexBuffer {
public:
    VertexBuffer() = default;
    ~VertexBuffer() { Release(); }

    bool Create(ID3D11Device* device,
                const void* vertexData,
                UINT vertexCount,
                UINT vertexSize,
                D3D11_USAGE usage = D3D11_USAGE_DEFAULT,
                UINT cpuAccessFlags = 0)
    {
        Release();

        m_vertexCount = vertexCount;
        m_vertexSize = vertexSize;
        m_usage = usage;

        // Buffer description
        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth = vertexSize * vertexCount;
        desc.Usage = usage;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = cpuAccessFlags;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        // Initial data
        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = vertexData;

        // Create buffer
        HRESULT hr = device->CreateBuffer(
            &desc,
            vertexData ? &initData : nullptr,
            &m_buffer
        );

        return SUCCEEDED(hr);
    }

    void Bind(ID3D11DeviceContext* context, UINT slot = 0) {
        UINT offset = 0;
        context->IASetVertexBuffers(slot, 1, &m_buffer, &m_vertexSize, &offset);
    }

    void Release() {
        if (m_buffer) {
            m_buffer->Release();
            m_buffer = nullptr;
        }
    }

    ID3D11Buffer* GetBuffer() const { return m_buffer; }
    UINT GetVertexCount() const { return m_vertexCount; }
    UINT GetVertexSize() const { return m_vertexSize; }

private:
    ID3D11Buffer* m_buffer = nullptr;
    UINT m_vertexCount = 0;
    UINT m_vertexSize = 0;
    D3D11_USAGE m_usage;
};
```

---

## 3. Vertex Structures and Formats

### Common Vertex Formats:

```cpp
// Position only
struct VertexPos {
    XMFLOAT3 position;

    static const D3D11_INPUT_ELEMENT_DESC* GetLayout(UINT& count) {
        static D3D11_INPUT_ELEMENT_DESC layout[] = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
             D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
        count = ARRAYSIZE(layout);
        return layout;
    }
};

// Position + Color
struct VertexPosColor {
    XMFLOAT3 position;
    XMFLOAT4 color;

    static const D3D11_INPUT_ELEMENT_DESC* GetLayout(UINT& count) {
        static D3D11_INPUT_ELEMENT_DESC layout[] = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
             D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
             D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
        count = ARRAYSIZE(layout);
        return layout;
    }
};

// Position + Normal + TexCoord
struct VertexPosNormalTex {
    XMFLOAT3 position;
    XMFLOAT3 normal;
    XMFLOAT2 texCoord;

    static const D3D11_INPUT_ELEMENT_DESC* GetLayout(UINT& count) {
        static D3D11_INPUT_ELEMENT_DESC layout[] = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
             D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
             D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24,
             D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
        count = ARRAYSIZE(layout);
        return layout;
    }
};

// Full vertex (everything)
struct VertexFull {
    XMFLOAT3 position;
    XMFLOAT3 normal;
    XMFLOAT3 tangent;
    XMFLOAT3 bitangent;
    XMFLOAT2 texCoord;
    XMFLOAT4 color;

    static const D3D11_INPUT_ELEMENT_DESC* GetLayout(UINT& count) {
        static D3D11_INPUT_ELEMENT_DESC layout[] = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
             D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
             D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24,
             D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36,
             D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48,
             D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 56,
             D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
        count = ARRAYSIZE(layout);
        return layout;
    }
};
```

---

## 4. Static vs Dynamic Buffers

### Buffer Usage Types:

```cpp
enum D3D11_USAGE {
    D3D11_USAGE_DEFAULT = 0,    // GPU read/write, no CPU access
    D3D11_USAGE_IMMUTABLE = 1,  // GPU read only, set once
    D3D11_USAGE_DYNAMIC = 2,    // GPU read, CPU write
    D3D11_USAGE_STAGING = 3     // CPU read/write, for GPU↔CPU transfer
};
```

### Comparison:

| Usage | GPU Access | CPU Access | Best For |
|-------|------------|------------|----------|
| DEFAULT | Read/Write | None | Static geometry |
| IMMUTABLE | Read | None (init only) | Never-changing data |
| DYNAMIC | Read | Write (Map/Unmap) | Per-frame updates |
| STAGING | Read/Write | Read/Write | Data readback |

### Creating Different Buffer Types:

```cpp
class BufferFactory {
public:
    // Static buffer (IMMUTABLE) - set once, never change
    static ID3D11Buffer* CreateStaticBuffer(
        ID3D11Device* device,
        const void* data,
        UINT size)
    {
        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth = size;
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = data;

        ID3D11Buffer* buffer = nullptr;
        device->CreateBuffer(&desc, &initData, &buffer);
        return buffer;
    }

    // Dynamic buffer - update every frame
    static ID3D11Buffer* CreateDynamicBuffer(
        ID3D11Device* device,
        UINT size)
    {
        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth = size;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        ID3D11Buffer* buffer = nullptr;
        device->CreateBuffer(&desc, nullptr, &buffer);
        return buffer;
    }

    // Default buffer - occasional updates
    static ID3D11Buffer* CreateDefaultBuffer(
        ID3D11Device* device,
        const void* data,
        UINT size)
    {
        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth = size;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = data;

        ID3D11Buffer* buffer = nullptr;
        device->CreateBuffer(&desc, data ? &initData : nullptr, &buffer);
        return buffer;
    }
};
```

---

## 5. Updating Buffer Data

### Map/Unmap (DYNAMIC buffers):

```cpp
class BufferUpdater {
public:
    // Update dynamic buffer (fast)
    static bool UpdateDynamicBuffer(
        ID3D11DeviceContext* context,
        ID3D11Buffer* buffer,
        const void* data,
        UINT size)
    {
        D3D11_MAPPED_SUBRESOURCE mapped;
        HRESULT hr = context->Map(
            buffer,
            0,
            D3D11_MAP_WRITE_DISCARD,  // Discard previous contents
            0,
            &mapped
        );

        if (FAILED(hr)) {
            return false;
        }

        memcpy(mapped.pData, data, size);
        context->Unmap(buffer, 0);
        return true;
    }

    // Update default buffer (slower)
    static void UpdateDefaultBuffer(
        ID3D11DeviceContext* context,
        ID3D11Buffer* buffer,
        const void* data,
        UINT size)
    {
        context->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);
    }

    // Partial update
    static bool UpdateBufferPartial(
        ID3D11DeviceContext* context,
        ID3D11Buffer* buffer,
        const void* data,
        UINT offset,
        UINT size)
    {
        D3D11_MAPPED_SUBRESOURCE mapped;
        HRESULT hr = context->Map(
            buffer,
            0,
            D3D11_MAP_WRITE_NO_OVERWRITE,  // Append mode
            0,
            &mapped
        );

        if (FAILED(hr)) {
            return false;
        }

        char* dest = (char*)mapped.pData + offset;
        memcpy(dest, data, size);
        context->Unmap(buffer, 0);
        return true;
    }
};
```

### Example: Animating Vertices

```cpp
class AnimatedVertexBuffer {
public:
    bool Create(ID3D11Device* device, UINT maxVertices) {
        m_maxVertices = maxVertices;
        UINT bufferSize = maxVertices * sizeof(VertexPosColor);

        m_buffer = BufferFactory::CreateDynamicBuffer(device, bufferSize);
        return m_buffer != nullptr;
    }

    void Update(ID3D11DeviceContext* context,
                const std::vector<VertexPosColor>& vertices)
    {
        if (vertices.size() > m_maxVertices) {
            return; // Buffer too small
        }

        BufferUpdater::UpdateDynamicBuffer(
            context,
            m_buffer,
            vertices.data(),
            (UINT)(vertices.size() * sizeof(VertexPosColor))
        );

        m_currentVertexCount = (UINT)vertices.size();
    }

    void Bind(ID3D11DeviceContext* context) {
        UINT stride = sizeof(VertexPosColor);
        UINT offset = 0;
        context->IASetVertexBuffers(0, 1, &m_buffer, &stride, &offset);
    }

    void Draw(ID3D11DeviceContext* context) {
        context->Draw(m_currentVertexCount, 0);
    }

    void Release() {
        if (m_buffer) {
            m_buffer->Release();
            m_buffer = nullptr;
        }
    }

private:
    ID3D11Buffer* m_buffer = nullptr;
    UINT m_maxVertices = 0;
    UINT m_currentVertexCount = 0;
};

// Usage example
void AnimateTriangle(ID3D11DeviceContext* context,
                      AnimatedVertexBuffer& vertexBuffer,
                      float time)
{
    std::vector<VertexPosColor> vertices = {
        {XMFLOAT3(0.0f, 0.5f * sinf(time), 0.0f), XMFLOAT4(1, 0, 0, 1)},
        {XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(0, 1, 0, 1)},
        {XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(0, 0, 1, 1)}
    };

    vertexBuffer.Update(context, vertices);
    vertexBuffer.Bind(context);
    vertexBuffer.Draw(context);
}
```

---

## 6. Multiple Vertex Streams

Use multiple vertex buffers simultaneously:

```cpp
class MultiStreamVertex {
public:
    // Stream 0: Position
    struct StreamPosition {
        XMFLOAT3 position;
    };

    // Stream 1: Attributes
    struct StreamAttributes {
        XMFLOAT3 normal;
        XMFLOAT2 texCoord;
        XMFLOAT4 color;
    };

    static const D3D11_INPUT_ELEMENT_DESC* GetLayout(UINT& count) {
        static D3D11_INPUT_ELEMENT_DESC layout[] = {
            // Stream 0
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
             D3D11_INPUT_PER_VERTEX_DATA, 0},

            // Stream 1
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0,
             D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 12,
             D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 20,
             D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
        count = ARRAYSIZE(layout);
        return layout;
    }
};

// Bind multiple streams
void BindMultipleStreams(ID3D11DeviceContext* context,
                          ID3D11Buffer* positionBuffer,
                          ID3D11Buffer* attributeBuffer)
{
    ID3D11Buffer* buffers[] = {positionBuffer, attributeBuffer};
    UINT strides[] = {sizeof(XMFLOAT3), sizeof(MultiStreamVertex::StreamAttributes)};
    UINT offsets[] = {0, 0};

    context->IASetVertexBuffers(0, 2, buffers, strides, offsets);
}
```

---

## 7. Drawing with Vertex Buffers

### Complete Drawing Pipeline:

```cpp
class TriangleRenderer {
public:
    bool Initialize(ID3D11Device* device) {
        // Create vertex data
        VertexPosColor vertices[] = {
            {XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1, 0, 0, 1)},   // Top (red)
            {XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(0, 1, 0, 1)},  // Right (green)
            {XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(0, 0, 1, 1)}  // Left (blue)
        };

        // Create vertex buffer
        return m_vertexBuffer.Create(
            device,
            vertices,
            3,
            sizeof(VertexPosColor),
            D3D11_USAGE_IMMUTABLE
        );
    }

    void Render(ID3D11DeviceContext* context) {
        // Bind vertex buffer
        m_vertexBuffer.Bind(context);

        // Set primitive topology
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Draw
        context->Draw(m_vertexBuffer.GetVertexCount(), 0);
    }

    void Release() {
        m_vertexBuffer.Release();
    }

private:
    VertexBuffer m_vertexBuffer;
};
```

---

## 8. Complete Working Examples

### Example 1: Colored Triangle

```cpp
// Complete program to render a colored triangle

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

// Global variables
ID3D11Device* g_device = nullptr;
ID3D11DeviceContext* g_context = nullptr;
IDXGISwapChain* g_swapChain = nullptr;
ID3D11RenderTargetView* g_rtv = nullptr;
ID3D11VertexShader* g_vertexShader = nullptr;
ID3D11PixelShader* g_pixelShader = nullptr;
ID3D11InputLayout* g_inputLayout = nullptr;
ID3D11Buffer* g_vertexBuffer = nullptr;

// Vertex structure
struct Vertex {
    XMFLOAT3 position;
    XMFLOAT4 color;
};

// Shader source code
const char* g_vertexShaderCode = R"(
struct VS_INPUT {
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_OUTPUT main(VS_INPUT input) {
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.color = input.color;
    return output;
}
)";

const char* g_pixelShaderCode = R"(
struct PS_INPUT {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 main(PS_INPUT input) : SV_TARGET {
    return input.color;
}
)";

bool InitializeDirectX(HWND hWnd) {
    // Create device and swap chain
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;

    D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION,
        &scd, &g_swapChain, &g_device, nullptr, &g_context
    );

    // Create render target view
    ID3D11Texture2D* backBuffer = nullptr;
    g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    g_device->CreateRenderTargetView(backBuffer, nullptr, &g_rtv);
    backBuffer->Release();

    g_context->OMSetRenderTargets(1, &g_rtv, nullptr);

    // Set viewport
    D3D11_VIEWPORT viewport = {0, 0, 800, 600, 0, 1};
    g_context->RSSetViewports(1, &viewport);

    // Compile shaders
    ID3DBlob* vsBlob = nullptr;
    D3DCompile(g_vertexShaderCode, strlen(g_vertexShaderCode),
                nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, &vsBlob, nullptr);
    g_device->CreateVertexShader(vsBlob->GetBufferPointer(),
                                  vsBlob->GetBufferSize(), nullptr, &g_vertexShader);

    ID3DBlob* psBlob = nullptr;
    D3DCompile(g_pixelShaderCode, strlen(g_pixelShaderCode),
                nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &psBlob, nullptr);
    g_device->CreatePixelShader(psBlob->GetBufferPointer(),
                                 psBlob->GetBufferSize(), nullptr, &g_pixelShader);

    // Create input layout
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    g_device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(),
                                 vsBlob->GetBufferSize(), &g_inputLayout);

    vsBlob->Release();
    psBlob->Release();

    // Create vertex buffer
    Vertex vertices[] = {
        {XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1, 0, 0, 1)},
        {XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(0, 1, 0, 1)},
        {XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(0, 0, 1, 1)}
    };

    D3D11_BUFFER_DESC bd = {};
    bd.ByteWidth = sizeof(vertices);
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    g_device->CreateBuffer(&bd, &initData, &g_vertexBuffer);

    return true;
}

void Render() {
    // Clear
    float clearColor[4] = {0.0f, 0.2f, 0.4f, 1.0f};
    g_context->ClearRenderTargetView(g_rtv, clearColor);

    // Set pipeline state
    g_context->IASetInputLayout(g_inputLayout);
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    g_context->IASetVertexBuffers(0, 1, &g_vertexBuffer, &stride, &offset);
    g_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    g_context->VSSetShader(g_vertexShader, nullptr, 0);
    g_context->PSSetShader(g_pixelShader, nullptr, 0);

    // Draw
    g_context->Draw(3, 0);

    // Present
    g_swapChain->Present(1, 0);
}

void Cleanup() {
    if (g_vertexBuffer) g_vertexBuffer->Release();
    if (g_inputLayout) g_inputLayout->Release();
    if (g_pixelShader) g_pixelShader->Release();
    if (g_vertexShader) g_vertexShader->Release();
    if (g_rtv) g_rtv->Release();
    if (g_swapChain) g_swapChain->Release();
    if (g_context) g_context->Release();
    if (g_device) g_device->Release();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    // Create window
    WNDCLASSEX wc = {sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0, 0,
                      hInstance, nullptr, nullptr, nullptr, nullptr,
                      L"DX11Triangle", nullptr};
    RegisterClassEx(&wc);
    HWND hWnd = CreateWindow(wc.lpszClassName, L"Colored Triangle",
                              WS_OVERLAPPEDWINDOW, 100, 100, 800, 600,
                              nullptr, nullptr, hInstance, nullptr);

    InitializeDirectX(hWnd);
    ShowWindow(hWnd, nCmdShow);

    // Main loop
    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            Render();
        }
    }

    Cleanup();
    return 0;
}
```

---

## Key Takeaways

1. ✅ Vertex buffers store vertex data on GPU
2. ✅ Create buffers with D3D11_BUFFER_DESC
3. ✅ Static buffers (IMMUTABLE) for fixed geometry
4. ✅ Dynamic buffers for per-frame updates
5. ✅ Use Map/Unmap to update dynamic buffers
6. ✅ Vertex structures define data layout
7. ✅ Multiple streams allow flexible vertex formats
8. ✅ Bind buffers with IASetVertexBuffers
9. ✅ Draw with context->Draw()
10. ✅ Always release resources

---

## Next Lesson Preview

In **Lesson 46: Index Buffers and Indexed Drawing**, we'll explore:
- Creating index buffers
- 16-bit vs 32-bit indices
- Indexed vs non-indexed drawing
- Triangle strips and fans
- Optimizing mesh data
- Complete indexed mesh rendering

---

## Summary Checklist

Before moving to Lesson 46, ensure you can:
- [ ] Create vertex buffers
- [ ] Define vertex structures
- [ ] Choose appropriate buffer usage
- [ ] Update dynamic buffers
- [ ] Use multiple vertex streams
- [ ] Bind vertex buffers
- [ ] Draw geometry
- [ ] Release resources properly

**Excellent work on completing Lesson 45!**

You now understand vertex buffers and can render geometry in DirectX!
