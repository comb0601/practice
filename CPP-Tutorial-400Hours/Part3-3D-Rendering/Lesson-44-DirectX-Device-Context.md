# Lesson 44: DirectX Device and Context

**Duration**: 5 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding Device and Context
2. Device Capabilities and Feature Queries
3. Immediate Context vs Deferred Contexts
4. Resource Creation with Device
5. Command Execution with Context
6. Multi-Threaded Rendering
7. Performance Optimization
8. Complete Examples

---

## 1. Understanding Device and Context

### The Device (ID3D11Device):
- **Factory** for creating resources
- **Thread-safe**
- Creates: buffers, textures, shaders, states

### The Context (ID3D11DeviceContext):
- **Executor** of rendering commands
- **Not thread-safe** (immediate context)
- Sets pipeline state
- Issues draw calls

```cpp
// Device: Creates resources
ID3D11Buffer* buffer = nullptr;
device->CreateBuffer(&desc, &initData, &buffer);

// Context: Uses resources
context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
context->Draw(vertexCount, 0);
```

### Relationship:

```
┌──────────────────────────┐
│     ID3D11Device         │ → Creates Resources
│   (Thread-Safe)          │
└──────────────────────────┘
            ↓ creates
┌──────────────────────────┐
│  ID3D11DeviceContext     │ → Executes Commands
│  (Not Thread-Safe)       │
└──────────────────────────┘
            ↓ commands
┌──────────────────────────┐
│      GPU Hardware        │
└──────────────────────────┘
```

---

## 2. Device Capabilities and Feature Queries

### Complete Device Info Class:

```cpp
#include <d3d11.h>
#include <dxgi1_6.h>
#include <string>
#include <iostream>

class DeviceInfo {
public:
    static void PrintDeviceInfo(ID3D11Device* device) {
        // Feature level
        D3D_FEATURE_LEVEL featureLevel = device->GetFeatureLevel();
        std::cout << "Feature Level: " << GetFeatureLevelString(featureLevel) << "\n";

        // Threading support
        D3D11_FEATURE_DATA_THREADING threading = {};
        device->CheckFeatureSupport(D3D11_FEATURE_THREADING,
                                     &threading, sizeof(threading));
        std::cout << "Driver Concurrent Creates: "
                  << (threading.DriverConcurrentCreates ? "Yes" : "No") << "\n";
        std::cout << "Driver Command Lists: "
                  << (threading.DriverCommandLists ? "Yes" : "No") << "\n";

        // Doubles support
        D3D11_FEATURE_DATA_DOUBLES doubles = {};
        device->CheckFeatureSupport(D3D11_FEATURE_DOUBLES,
                                     &doubles, sizeof(doubles));
        std::cout << "Double Precision Support: "
                  << (doubles.DoublePrecisionFloatShaderOps ? "Yes" : "No") << "\n";

        // Format support
        PrintFormatSupport(device, DXGI_FORMAT_R8G8B8A8_UNORM);
        PrintFormatSupport(device, DXGI_FORMAT_R16G16B16A16_FLOAT);

        // Adapter info
        PrintAdapterInfo(device);
    }

    static void PrintFormatSupport(ID3D11Device* device, DXGI_FORMAT format) {
        UINT support = 0;
        device->CheckFormatSupport(format, &support);

        std::cout << "\nFormat " << format << " support:\n";
        if (support & D3D11_FORMAT_SUPPORT_TEXTURE2D)
            std::cout << "  ✓ Texture2D\n";
        if (support & D3D11_FORMAT_SUPPORT_RENDER_TARGET)
            std::cout << "  ✓ Render Target\n";
        if (support & D3D11_FORMAT_SUPPORT_DEPTH_STENCIL)
            std::cout << "  ✓ Depth Stencil\n";
        if (support & D3D11_FORMAT_SUPPORT_SHADER_SAMPLE)
            std::cout << "  ✓ Shader Sampling\n";
        if (support & D3D11_FORMAT_SUPPORT_BLENDABLE)
            std::cout << "  ✓ Blending\n";
    }

    static void PrintAdapterInfo(ID3D11Device* device) {
        ComPtr<IDXGIDevice> dxgiDevice;
        ComPtr<IDXGIAdapter> adapter;

        if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&dxgiDevice)))) {
            if (SUCCEEDED(dxgiDevice->GetAdapter(&adapter))) {
                DXGI_ADAPTER_DESC desc;
                adapter->GetDesc(&desc);

                std::wcout << L"\nAdapter: " << desc.Description << L"\n";
                std::cout << "Video Memory: "
                          << (desc.DedicatedVideoMemory / 1024 / 1024) << " MB\n";
                std::cout << "System Memory: "
                          << (desc.DedicatedSystemMemory / 1024 / 1024) << " MB\n";
                std::cout << "Shared Memory: "
                          << (desc.SharedSystemMemory / 1024 / 1024) << " MB\n";
            }
        }
    }

    static const char* GetFeatureLevelString(D3D_FEATURE_LEVEL level) {
        switch (level) {
            case D3D_FEATURE_LEVEL_11_1: return "11.1";
            case D3D_FEATURE_LEVEL_11_0: return "11.0";
            case D3D_FEATURE_LEVEL_10_1: return "10.1";
            case D3D_FEATURE_LEVEL_10_0: return "10.0";
            case D3D_FEATURE_LEVEL_9_3:  return "9.3";
            default: return "Unknown";
        }
    }

    // Check compute shader support
    static bool SupportsComputeShaders(ID3D11Device* device) {
        return device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0;
    }

    // Check tessellation support
    static bool SupportsTessellation(ID3D11Device* device) {
        return device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0;
    }

    // Get max texture size
    static UINT GetMaxTextureDimension(ID3D11Device* device) {
        D3D_FEATURE_LEVEL level = device->GetFeatureLevel();
        if (level >= D3D_FEATURE_LEVEL_11_0) return 16384;
        if (level >= D3D_FEATURE_LEVEL_10_0) return 8192;
        if (level >= D3D_FEATURE_LEVEL_9_3)  return 4096;
        return 2048;
    }
};
```

---

## 3. Immediate Context vs Deferred Contexts

### Immediate Context:
- **Single-threaded**
- Commands execute immediately
- Retrieved with `GetImmediateContext()`

```cpp
ID3D11DeviceContext* immediateContext = nullptr;
device->GetImmediateContext(&immediateContext);

// Commands execute right away
immediateContext->Draw(3, 0);  // Executes now
```

### Deferred Contexts:
- **Multi-threaded**
- Records commands to command lists
- Created with `CreateDeferredContext()`

```cpp
class DeferredContextManager {
public:
    static ID3D11DeviceContext* CreateDeferredContext(ID3D11Device* device) {
        ID3D11DeviceContext* deferredContext = nullptr;
        HRESULT hr = device->CreateDeferredContext(0, &deferredContext);

        if (FAILED(hr)) {
            return nullptr;
        }

        return deferredContext;
    }

    // Record commands
    static ID3D11CommandList* RecordCommands(
        ID3D11DeviceContext* deferredContext,
        std::function<void(ID3D11DeviceContext*)> recordFunc)
    {
        // Record commands
        recordFunc(deferredContext);

        // Finish and get command list
        ID3D11CommandList* commandList = nullptr;
        HRESULT hr = deferredContext->FinishCommandList(FALSE, &commandList);

        if (FAILED(hr)) {
            return nullptr;
        }

        return commandList;
    }

    // Execute command list
    static void ExecuteCommandList(
        ID3D11DeviceContext* immediateContext,
        ID3D11CommandList* commandList,
        bool restoreState = true)
    {
        immediateContext->ExecuteCommandList(commandList, restoreState);
    }
};

// Example usage
void MultiThreadedRendering(ID3D11Device* device,
                             ID3D11DeviceContext* immediateContext)
{
    // Create deferred contexts
    ID3D11DeviceContext* deferredContext1 =
        DeferredContextManager::CreateDeferredContext(device);
    ID3D11DeviceContext* deferredContext2 =
        DeferredContextManager::CreateDeferredContext(device);

    // Thread 1: Record commands
    ID3D11CommandList* commandList1 =
        DeferredContextManager::RecordCommands(deferredContext1,
            [](ID3D11DeviceContext* ctx) {
                // Record rendering commands
                // ctx->Draw(...);
            });

    // Thread 2: Record commands
    ID3D11CommandList* commandList2 =
        DeferredContextManager::RecordCommands(deferredContext2,
            [](ID3D11DeviceContext* ctx) {
                // Record more commands
                // ctx->Draw(...);
            });

    // Execute on immediate context (main thread)
    DeferredContextManager::ExecuteCommandList(immediateContext, commandList1);
    DeferredContextManager::ExecuteCommandList(immediateContext, commandList2);

    // Cleanup
    commandList1->Release();
    commandList2->Release();
    deferredContext1->Release();
    deferredContext2->Release();
}
```

### Comparison:

| Feature | Immediate Context | Deferred Context |
|---------|-------------------|------------------|
| Thread-safe | No | Yes (separate contexts) |
| Execution | Immediate | Deferred (via command lists) |
| Count | 1 per device | Multiple per device |
| Use case | Main rendering | Multi-threaded recording |

---

## 4. Resource Creation with Device

### Complete Resource Factory:

```cpp
class ResourceFactory {
public:
    // Create vertex buffer
    static ID3D11Buffer* CreateVertexBuffer(
        ID3D11Device* device,
        const void* data,
        UINT byteSize,
        D3D11_USAGE usage = D3D11_USAGE_DEFAULT)
    {
        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth = byteSize;
        desc.Usage = usage;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = (usage == D3D11_USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = data;

        ID3D11Buffer* buffer = nullptr;
        HRESULT hr = device->CreateBuffer(&desc,
                                          data ? &initData : nullptr,
                                          &buffer);
        return SUCCEEDED(hr) ? buffer : nullptr;
    }

    // Create index buffer
    static ID3D11Buffer* CreateIndexBuffer(
        ID3D11Device* device,
        const void* data,
        UINT byteSize,
        D3D11_USAGE usage = D3D11_USAGE_DEFAULT)
    {
        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth = byteSize;
        desc.Usage = usage;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.CPUAccessFlags = (usage == D3D11_USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = data;

        ID3D11Buffer* buffer = nullptr;
        HRESULT hr = device->CreateBuffer(&desc,
                                          data ? &initData : nullptr,
                                          &buffer);
        return SUCCEEDED(hr) ? buffer : nullptr;
    }

    // Create constant buffer
    static ID3D11Buffer* CreateConstantBuffer(
        ID3D11Device* device,
        UINT byteSize,
        D3D11_USAGE usage = D3D11_USAGE_DEFAULT)
    {
        // Constant buffer size must be multiple of 16
        UINT alignedSize = (byteSize + 15) & ~15;

        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth = alignedSize;
        desc.Usage = usage;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = (usage == D3D11_USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0;

        ID3D11Buffer* buffer = nullptr;
        HRESULT hr = device->CreateBuffer(&desc, nullptr, &buffer);
        return SUCCEEDED(hr) ? buffer : nullptr;
    }

    // Create 2D texture
    static ID3D11Texture2D* CreateTexture2D(
        ID3D11Device* device,
        UINT width,
        UINT height,
        DXGI_FORMAT format,
        const void* data = nullptr,
        UINT pitch = 0)
    {
        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = format;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = data;
        initData.SysMemPitch = pitch;

        ID3D11Texture2D* texture = nullptr;
        HRESULT hr = device->CreateTexture2D(&desc,
                                              data ? &initData : nullptr,
                                              &texture);
        return SUCCEEDED(hr) ? texture : nullptr;
    }

    // Create render target texture
    static ID3D11Texture2D* CreateRenderTarget(
        ID3D11Device* device,
        UINT width,
        UINT height,
        DXGI_FORMAT format)
    {
        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = format;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

        ID3D11Texture2D* texture = nullptr;
        HRESULT hr = device->CreateTexture2D(&desc, nullptr, &texture);
        return SUCCEEDED(hr) ? texture : nullptr;
    }

    // Create depth stencil texture
    static ID3D11Texture2D* CreateDepthStencil(
        ID3D11Device* device,
        UINT width,
        UINT height,
        DXGI_FORMAT format = DXGI_FORMAT_D24_UNORM_S8_UINT)
    {
        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = format;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        ID3D11Texture2D* texture = nullptr;
        HRESULT hr = device->CreateTexture2D(&desc, nullptr, &texture);
        return SUCCEEDED(hr) ? texture : nullptr;
    }
};
```

---

## 5. Command Execution with Context

### Pipeline State Management:

```cpp
class PipelineState {
public:
    struct State {
        ID3D11VertexShader* vertexShader = nullptr;
        ID3D11PixelShader* pixelShader = nullptr;
        ID3D11InputLayout* inputLayout = nullptr;
        ID3D11Buffer* vertexBuffers[8] = {};
        UINT strides[8] = {};
        UINT offsets[8] = {};
        ID3D11Buffer* indexBuffer = nullptr;
        DXGI_FORMAT indexFormat = DXGI_FORMAT_R32_UINT;
        ID3D11Buffer* vsConstantBuffers[4] = {};
        ID3D11Buffer* psConstantBuffers[4] = {};
        ID3D11RasterizerState* rasterizerState = nullptr;
        ID3D11DepthStencilState* depthStencilState = nullptr;
        ID3D11BlendState* blendState = nullptr;
        D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    };

    static void SetPipelineState(ID3D11DeviceContext* context, const State& state) {
        // Input Assembler
        context->IASetInputLayout(state.inputLayout);
        context->IASetVertexBuffers(0, 8, state.vertexBuffers,
                                     state.strides, state.offsets);
        context->IASetIndexBuffer(state.indexBuffer, state.indexFormat, 0);
        context->IASetPrimitiveTopology(state.topology);

        // Vertex Shader
        context->VSSetShader(state.vertexShader, nullptr, 0);
        context->VSSetConstantBuffers(0, 4, state.vsConstantBuffers);

        // Pixel Shader
        context->PSSetShader(state.pixelShader, nullptr, 0);
        context->PSSetConstantBuffers(0, 4, state.psConstantBuffers);

        // Rasterizer
        context->RSSetState(state.rasterizerState);

        // Output Merger
        context->OMSetDepthStencilState(state.depthStencilState, 0);
        context->OMSetBlendState(state.blendState, nullptr, 0xFFFFFFFF);
    }

    static void ClearPipelineState(ID3D11DeviceContext* context) {
        context->ClearState();
    }
};
```

### Draw Commands:

```cpp
class DrawCommands {
public:
    // Draw non-indexed
    static void Draw(ID3D11DeviceContext* context,
                     UINT vertexCount,
                     UINT startVertex = 0)
    {
        context->Draw(vertexCount, startVertex);
    }

    // Draw indexed
    static void DrawIndexed(ID3D11DeviceContext* context,
                            UINT indexCount,
                            UINT startIndex = 0,
                            INT baseVertex = 0)
    {
        context->DrawIndexed(indexCount, startIndex, baseVertex);
    }

    // Draw instanced
    static void DrawInstanced(ID3D11DeviceContext* context,
                               UINT vertexCountPerInstance,
                               UINT instanceCount,
                               UINT startVertex = 0,
                               UINT startInstance = 0)
    {
        context->DrawInstanced(vertexCountPerInstance, instanceCount,
                                startVertex, startInstance);
    }

    // Draw indexed instanced
    static void DrawIndexedInstanced(ID3D11DeviceContext* context,
                                      UINT indexCountPerInstance,
                                      UINT instanceCount,
                                      UINT startIndex = 0,
                                      INT baseVertex = 0,
                                      UINT startInstance = 0)
    {
        context->DrawIndexedInstanced(indexCountPerInstance, instanceCount,
                                       startIndex, baseVertex, startInstance);
    }

    // Dispatch compute shader
    static void Dispatch(ID3D11DeviceContext* context,
                         UINT threadGroupCountX,
                         UINT threadGroupCountY,
                         UINT threadGroupCountZ)
    {
        context->Dispatch(threadGroupCountX, threadGroupCountY,
                          threadGroupCountZ);
    }
};
```

---

## 6. Multi-Threaded Rendering

### Complete Multi-Threading Example:

```cpp
#include <thread>
#include <vector>
#include <functional>

class MultiThreadRenderer {
public:
    MultiThreadRenderer(ID3D11Device* device, UINT threadCount)
        : m_device(device)
    {
        device->GetImmediateContext(&m_immediateContext);

        // Create deferred contexts
        for (UINT i = 0; i < threadCount; ++i) {
            ID3D11DeviceContext* deferredContext = nullptr;
            device->CreateDeferredContext(0, &deferredContext);
            m_deferredContexts.push_back(deferredContext);
        }
    }

    ~MultiThreadRenderer() {
        for (auto ctx : m_deferredContexts) {
            ctx->Release();
        }
        m_immediateContext->Release();
    }

    // Render using multiple threads
    void RenderMultiThreaded(
        const std::vector<std::function<void(ID3D11DeviceContext*)>>& tasks)
    {
        std::vector<std::thread> threads;
        std::vector<ID3D11CommandList*> commandLists(tasks.size());

        // Record commands in parallel
        for (size_t i = 0; i < tasks.size(); ++i) {
            threads.emplace_back([this, i, &tasks, &commandLists]() {
                ID3D11DeviceContext* ctx = m_deferredContexts[i];

                // Record commands
                tasks[i](ctx);

                // Finish command list
                ctx->FinishCommandList(FALSE, &commandLists[i]);
            });
        }

        // Wait for all threads
        for (auto& thread : threads) {
            thread.join();
        }

        // Execute command lists on immediate context
        for (auto commandList : commandLists) {
            m_immediateContext->ExecuteCommandList(commandList, FALSE);
            commandList->Release();
        }
    }

private:
    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_immediateContext;
    std::vector<ID3D11DeviceContext*> m_deferredContexts;
};

// Example usage
void ExampleMultiThreadRendering(ID3D11Device* device) {
    MultiThreadRenderer renderer(device, 4); // 4 threads

    std::vector<std::function<void(ID3D11DeviceContext*)>> tasks;

    // Task 1: Render terrain
    tasks.push_back([](ID3D11DeviceContext* ctx) {
        // Set pipeline state
        // Draw terrain
        // ctx->DrawIndexed(...);
    });

    // Task 2: Render objects
    tasks.push_back([](ID3D11DeviceContext* ctx) {
        // Set pipeline state
        // Draw objects
        // ctx->DrawIndexed(...);
    });

    // Task 3: Render particles
    tasks.push_back([](ID3D11DeviceContext* ctx) {
        // Set pipeline state
        // Draw particles
        // ctx->Draw(...);
    });

    // Task 4: Render UI
    tasks.push_back([](ID3D11DeviceContext* ctx) {
        // Set pipeline state
        // Draw UI
        // ctx->Draw(...);
    });

    // Execute all tasks in parallel
    renderer.RenderMultiThreaded(tasks);
}
```

---

## 7. Performance Optimization

### Best Practices:

```cpp
class PerformanceOptimizer {
public:
    // 1. Batch draw calls
    static void BatchDrawCalls(ID3D11DeviceContext* context,
                                const std::vector<DrawCall>& drawCalls)
    {
        // Sort by pipeline state to minimize state changes
        auto sortedCalls = SortByPipelineState(drawCalls);

        for (const auto& call : sortedCalls) {
            // Only change state when different
            if (call.state != currentState) {
                SetPipelineState(context, call.state);
                currentState = call.state;
            }
            context->DrawIndexed(call.indexCount, call.startIndex, 0);
        }
    }

    // 2. Use Map/Unmap for dynamic updates
    static void UpdateDynamicBuffer(ID3D11DeviceContext* context,
                                     ID3D11Buffer* buffer,
                                     const void* data,
                                     UINT size)
    {
        D3D11_MAPPED_SUBRESOURCE mapped;
        HRESULT hr = context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD,
                                   0, &mapped);
        if (SUCCEEDED(hr)) {
            memcpy(mapped.pData, data, size);
            context->Unmap(buffer, 0);
        }
    }

    // 3. Minimize state changes
    static void OptimizeStateChanges(ID3D11DeviceContext* context) {
        // Cache current state
        // Only change when necessary
        // Sort draw calls to minimize changes
    }

    // 4. Use instancing
    static void DrawInstancedObjects(ID3D11DeviceContext* context,
                                      UINT indexCount,
                                      UINT instanceCount)
    {
        // Much faster than individual draw calls
        context->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
    }

    // 5. Profile with queries
    static void ProfileGPU(ID3D11Device* device,
                           ID3D11DeviceContext* context)
    {
        // Create timestamp query
        D3D11_QUERY_DESC queryDesc = {};
        queryDesc.Query = D3D11_QUERY_TIMESTAMP;

        ID3D11Query* queryStart = nullptr;
        ID3D11Query* queryEnd = nullptr;
        device->CreateQuery(&queryDesc, &queryStart);
        device->CreateQuery(&queryDesc, &queryEnd);

        // Issue queries
        context->End(queryStart);
        // ... rendering commands ...
        context->End(queryEnd);

        // Get results
        UINT64 startTime, endTime;
        while (context->GetData(queryStart, &startTime, sizeof(UINT64), 0) != S_OK);
        while (context->GetData(queryEnd, &endTime, sizeof(UINT64), 0) != S_OK);

        // Calculate elapsed time
        UINT64 elapsed = endTime - startTime;

        queryStart->Release();
        queryEnd->Release();
    }
};
```

---

## 8. Complete Examples

### Complete Device/Context Management Class:

```cpp
class D3D11DeviceManager {
public:
    bool Initialize() {
        if (!CreateDeviceAndContext()) {
            return false;
        }

        PrintDeviceInfo();
        return true;
    }

    ID3D11Device* GetDevice() const { return m_device.Get(); }
    ID3D11DeviceContext* GetContext() const { return m_immediateContext.Get(); }

    void BeginFrame() {
        // Clear any cached state
        m_immediateContext->ClearState();
    }

    void EndFrame() {
        // Flush commands
        m_immediateContext->Flush();
    }

    void Cleanup() {
        if (m_immediateContext) {
            m_immediateContext->ClearState();
            m_immediateContext->Flush();
        }

        m_immediateContext.Reset();

#ifdef _DEBUG
        if (m_device) {
            ID3D11Debug* debug = nullptr;
            m_device->QueryInterface(&debug);
            if (debug) {
                debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
                debug->Release();
            }
        }
#endif

        m_device.Reset();
    }

private:
    bool CreateDeviceAndContext() {
        UINT createDeviceFlags = 0;
#ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0
        };

        HRESULT hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            createDeviceFlags,
            featureLevels,
            ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION,
            &m_device,
            &m_featureLevel,
            &m_immediateContext
        );

        return SUCCEEDED(hr);
    }

    void PrintDeviceInfo() {
        DeviceInfo::PrintDeviceInfo(m_device.Get());
    }

private:
    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_immediateContext;
    D3D_FEATURE_LEVEL m_featureLevel;
};

// Main application
int main() {
    D3D11DeviceManager deviceManager;

    if (!deviceManager.Initialize()) {
        std::cerr << "Failed to initialize device\n";
        return -1;
    }

    ID3D11Device* device = deviceManager.GetDevice();
    ID3D11DeviceContext* context = deviceManager.GetContext();

    // Main loop
    while (running) {
        deviceManager.BeginFrame();

        // Rendering commands
        context->Draw(3, 0);

        deviceManager.EndFrame();
    }

    deviceManager.Cleanup();
    return 0;
}
```

---

## Key Takeaways

1. ✅ Device creates resources (thread-safe)
2. ✅ Context executes commands (not thread-safe)
3. ✅ Query device capabilities before use
4. ✅ Immediate context for single-threaded rendering
5. ✅ Deferred contexts for multi-threaded rendering
6. ✅ Command lists record and replay commands
7. ✅ Batch draw calls to reduce overhead
8. ✅ Minimize pipeline state changes
9. ✅ Use instancing for repeated geometry
10. ✅ Profile with queries for optimization

---

## Next Lesson Preview

In **Lesson 45: Vertex Buffers in DirectX**, we'll explore:
- Creating vertex buffers
- Vertex formats and layouts
- Dynamic vs static buffers
- Updating buffer data
- Multiple vertex streams
- Complete working examples

---

## Summary Checklist

Before moving to Lesson 45, ensure you can:
- [ ] Understand device vs context roles
- [ ] Query device capabilities
- [ ] Create resources with device
- [ ] Execute commands with context
- [ ] Use deferred contexts for multi-threading
- [ ] Create and execute command lists
- [ ] Optimize rendering performance
- [ ] Batch draw calls effectively
- [ ] Profile GPU performance
- [ ] Manage device lifecycle

**Excellent work on completing Lesson 44!**

You now understand the core DirectX device and context architecture!
