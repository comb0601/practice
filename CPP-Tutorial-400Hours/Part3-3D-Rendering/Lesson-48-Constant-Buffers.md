# Lesson 48: Constant Buffers

**Duration**: 5 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding Constant Buffers
2. Creating Constant Buffers
3. Buffer Alignment Rules
4. Updating Constant Buffers
5. Multiple Constant Buffers
6. Per-Frame vs Per-Object Buffers
7. Performance Optimization
8. Complete Examples

---

## 1. Understanding Constant Buffers

**Constant buffers** (cbuffers) provide data to shaders that remains constant for a draw call.

### Use Cases:
- Transformation matrices (world, view, projection)
- Light parameters
- Material properties
- Time and animation data
- Camera position

### CPU → GPU Data Flow:

```
CPU (C++ struct)
      ↓
Create/Update ConstantBuffer
      ↓
GPU Memory
      ↓
Shader Access (cbuffer)
```

---

## 2. Creating Constant Buffers

### Complete Constant Buffer Class:

```cpp
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

template<typename T>
class ConstantBuffer {
public:
    ConstantBuffer() = default;
    ~ConstantBuffer() { Release(); }

    bool Create(ID3D11Device* device) {
        Release();

        // Constant buffer size must be multiple of 16 bytes
        UINT size = (sizeof(T) + 15) & ~15;

        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth = size;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        HRESULT hr = device->CreateBuffer(&desc, nullptr, &m_buffer);
        return SUCCEEDED(hr);
    }

    void Update(ID3D11DeviceContext* context, const T& data) {
        D3D11_MAPPED_SUBRESOURCE mapped;
        HRESULT hr = context->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
        if (SUCCEEDED(hr)) {
            memcpy(mapped.pData, &data, sizeof(T));
            context->Unmap(m_buffer, 0);
        }
    }

    void BindVS(ID3D11DeviceContext* context, UINT slot) {
        context->VSSetConstantBuffers(slot, 1, &m_buffer);
    }

    void BindPS(ID3D11DeviceContext* context, UINT slot) {
        context->PSSetConstantBuffers(slot, 1, &m_buffer);
    }

    void BindBoth(ID3D11DeviceContext* context, UINT slot) {
        BindVS(context, slot);
        BindPS(context, slot);
    }

    void Release() {
        if (m_buffer) {
            m_buffer->Release();
            m_buffer = nullptr;
        }
    }

private:
    ID3D11Buffer* m_buffer = nullptr;
};
```

---

## 3. Buffer Alignment Rules

### Packing Rules:

**16-byte alignment**: Constant buffers must be multiples of 16 bytes

```cpp
// ❌ BAD: Not aligned (17 bytes)
struct BadCBuffer {
    XMFLOAT4X4 matrix;  // 64 bytes
    float value;        // 4 bytes
};                      // Total: 68 bytes (not multiple of 16)

// ✅ GOOD: Properly aligned (80 bytes)
struct GoodCBuffer {
    XMFLOAT4X4 matrix;  // 64 bytes
    float value;        // 4 bytes
    float padding[3];   // 12 bytes padding
};                      // Total: 80 bytes (5 * 16)
```

### HLSL Packing Rules:

```hlsl
// Variables pack into float4 (16-byte) registers

cbuffer BadLayout : register(b0) {
    float3 vec;     // Uses 12 bytes
    float scalar;   // Uses 4 bytes (same register)
    float3 vec2;    // Spans 2 registers! (inefficient)
};

cbuffer GoodLayout : register(b0) {
    float3 vec;     // 12 bytes
    float scalar;   // 4 bytes (total: 16 bytes = 1 register)
    float4 vec2;    // 16 bytes = 1 register (better)
};
```

### Helper Macros:

```cpp
// Ensure 16-byte alignment
#define ALIGN16 __declspec(align(16))

ALIGN16 struct TransformCBuffer {
    XMFLOAT4X4 world;
    XMFLOAT4X4 view;
    XMFLOAT4X4 projection;
};
```

---

## 4. Updating Constant Buffers

### Update Methods:

```cpp
class CBufferUpdater {
public:
    // Method 1: Map/Unmap (DYNAMIC usage)
    template<typename T>
    static void UpdateDynamic(ID3D11DeviceContext* context,
                               ID3D11Buffer* buffer,
                               const T& data)
    {
        D3D11_MAPPED_SUBRESOURCE mapped;
        HRESULT hr = context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD,
                                   0, &mapped);
        if (SUCCEEDED(hr)) {
            memcpy(mapped.pData, &data, sizeof(T));
            context->Unmap(buffer, 0);
        }
    }

    // Method 2: UpdateSubresource (DEFAULT usage)
    template<typename T>
    static void UpdateDefault(ID3D11DeviceContext* context,
                               ID3D11Buffer* buffer,
                               const T& data)
    {
        context->UpdateSubresource(buffer, 0, nullptr, &data, 0, 0);
    }
};

// Performance comparison:
// - Map/Unmap (DYNAMIC): Faster for per-frame updates
// - UpdateSubresource (DEFAULT): Better for occasional updates
```

---

## 5. Multiple Constant Buffers

### Common Buffer Layout:

```cpp
// Per-frame constant buffer (register b0)
struct PerFrameCBuffer {
    XMFLOAT4X4 view;
    XMFLOAT4X4 projection;
    XMFLOAT3 cameraPosition;
    float time;
    XMFLOAT3 lightDirection;
    float padding1;
    XMFLOAT4 lightColor;
};

// Per-object constant buffer (register b1)
struct PerObjectCBuffer {
    XMFLOAT4X4 world;
    XMFLOAT4 color;
};

// Material constant buffer (register b2)
struct MaterialCBuffer {
    XMFLOAT4 ambient;
    XMFLOAT4 diffuse;
    XMFLOAT4 specular;
    float shininess;
    float padding[3];
};

// Usage in HLSL:
/*
cbuffer PerFrame : register(b0) {
    float4x4 view;
    float4x4 projection;
    float3 cameraPosition;
    float time;
};

cbuffer PerObject : register(b1) {
    float4x4 world;
    float4 color;
};

cbuffer Material : register(b2) {
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float shininess;
};
*/
```

### Managing Multiple Buffers:

```cpp
class ConstantBufferManager {
public:
    bool Initialize(ID3D11Device* device) {
        if (!m_perFrameBuffer.Create(device)) return false;
        if (!m_perObjectBuffer.Create(device)) return false;
        if (!m_materialBuffer.Create(device)) return false;
        return true;
    }

    void UpdatePerFrame(ID3D11DeviceContext* context,
                         const PerFrameCBuffer& data)
    {
        m_perFrameBuffer.Update(context, data);
        m_perFrameBuffer.BindVS(context, 0);
        m_perFrameBuffer.BindPS(context, 0);
    }

    void UpdatePerObject(ID3D11DeviceContext* context,
                          const PerObjectCBuffer& data)
    {
        m_perObjectBuffer.Update(context, data);
        m_perObjectBuffer.BindVS(context, 1);
    }

    void UpdateMaterial(ID3D11DeviceContext* context,
                         const MaterialCBuffer& data)
    {
        m_materialBuffer.Update(context, data);
        m_materialBuffer.BindPS(context, 2);
    }

    void Release() {
        m_perFrameBuffer.Release();
        m_perObjectBuffer.Release();
        m_materialBuffer.Release();
    }

private:
    ConstantBuffer<PerFrameCBuffer> m_perFrameBuffer;
    ConstantBuffer<PerObjectCBuffer> m_perObjectBuffer;
    ConstantBuffer<MaterialCBuffer> m_materialBuffer;
};
```

---

## 6. Per-Frame vs Per-Object Buffers

### Update Frequency Strategy:

```cpp
class RenderBatch {
public:
    void Render(ID3D11DeviceContext* context,
                ConstantBufferManager& cbuffers)
    {
        // Update per-frame data ONCE per frame
        PerFrameCBuffer perFrame;
        perFrame.view = m_camera.GetViewMatrix();
        perFrame.projection = m_camera.GetProjectionMatrix();
        perFrame.cameraPosition = m_camera.GetPosition();
        perFrame.time = m_gameTime;

        cbuffers.UpdatePerFrame(context, perFrame);

        // Update per-object data for EACH object
        for (auto& object : m_objects) {
            PerObjectCBuffer perObject;
            perObject.world = object.GetWorldMatrix();
            perObject.color = object.GetColor();

            cbuffers.UpdatePerObject(context, perObject);
            object.Draw(context);
        }
    }

private:
    Camera m_camera;
    float m_gameTime;
    std::vector<GameObject> m_objects;
};
```

---

## 7. Performance Optimization

### Best Practices:

```cpp
class CBufferOptimization {
public:
    // ✅ GOOD: Group by update frequency
    struct OptimizedLayout {
        // Register b0: Per-frame (update once)
        struct PerFrame {
            XMFLOAT4X4 viewProjection;  // Pre-multiply on CPU
            XMFLOAT3 cameraPos;
            float time;
        };

        // Register b1: Per-object (update many times)
        struct PerObject {
            XMFLOAT4X4 world;
        };
    };

    // ❌ BAD: Mixed update frequencies
    struct BadLayout {
        XMFLOAT4X4 world;           // Per-object
        XMFLOAT4X4 view;            // Per-frame
        XMFLOAT4X4 projection;      // Per-frame
    };  // Wasteful: updates view/projection for every object!

    // ✅ GOOD: Pre-multiply matrices on CPU
    static XMFLOAT4X4 PreMultiplyViewProjection(
        const XMFLOAT4X4& view,
        const XMFLOAT4X4& projection)
    {
        XMMATRIX v = XMLoadFloat4x4(&view);
        XMMATRIX p = XMLoadFloat4x4(&projection);
        XMMATRIX vp = XMMatrixMultiply(v, p);

        XMFLOAT4X4 result;
        XMStoreFloat4x4(&result, vp);
        return result;
    }

    // ✅ GOOD: Minimize buffer updates
    void OptimizedRender(ID3D11DeviceContext* context) {
        // Sort objects by material to reduce buffer updates
        std::sort(m_objects.begin(), m_objects.end(),
            [](const Object& a, const Object& b) {
                return a.materialID < b.materialID;
            });

        UINT currentMaterial = UINT_MAX;
        for (const auto& obj : m_objects) {
            // Only update material buffer when it changes
            if (obj.materialID != currentMaterial) {
                UpdateMaterialBuffer(context, obj.materialID);
                currentMaterial = obj.materialID;
            }

            UpdateObjectBuffer(context, obj.worldMatrix);
            obj.Draw(context);
        }
    }

private:
    std::vector<Object> m_objects;
};
```

---

## 8. Complete Examples

### Example: Rotating Cube with Constant Buffers

**C++ Code:**
```cpp
#include <DirectXMath.h>
using namespace DirectX;

struct MVPCBuffer {
    XMFLOAT4X4 world;
    XMFLOAT4X4 view;
    XMFLOAT4X4 projection;
};

class RotatingCubeDemo {
public:
    bool Initialize(ID3D11Device* device) {
        return m_mvpBuffer.Create(device);
    }

    void Update(float deltaTime) {
        m_rotation += deltaTime;

        // Create rotation matrix
        XMMATRIX world = XMMatrixRotationY(m_rotation);

        // Camera
        XMVECTOR eye = XMVectorSet(0, 0, -5, 1);
        XMVECTOR at = XMVectorSet(0, 0, 0, 1);
        XMVECTOR up = XMVectorSet(0, 1, 0, 0);
        XMMATRIX view = XMMatrixLookAtLH(eye, at, up);

        // Projection
        XMMATRIX projection = XMMatrixPerspectiveFovLH(
            XM_PIDIV4,  // 45 degrees
            800.0f / 600.0f,  // Aspect ratio
            0.01f,      // Near plane
            100.0f      // Far plane
        );

        // Store in constant buffer
        MVPCBuffer cbData;
        XMStoreFloat4x4(&cbData.world, XMMatrixTranspose(world));
        XMStoreFloat4x4(&cbData.view, XMMatrixTranspose(view));
        XMStoreFloat4x4(&cbData.projection, XMMatrixTranspose(projection));

        m_cbData = cbData;
    }

    void Render(ID3D11DeviceContext* context) {
        // Update constant buffer
        m_mvpBuffer.Update(context, m_cbData);
        m_mvpBuffer.BindVS(context, 0);

        // Draw cube
        context->DrawIndexed(36, 0, 0);
    }

private:
    ConstantBuffer<MVPCBuffer> m_mvpBuffer;
    MVPCBuffer m_cbData;
    float m_rotation = 0.0f;
};
```

**HLSL Vertex Shader:**
```hlsl
cbuffer MVP : register(b0) {
    float4x4 world;
    float4x4 view;
    float4x4 projection;
};

struct VS_INPUT {
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VS_OUTPUT {
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};

VS_OUTPUT main(VS_INPUT input) {
    VS_OUTPUT output;

    // Transform position
    float4 worldPos = mul(float4(input.position, 1.0f), world);
    float4 viewPos = mul(worldPos, view);
    output.position = mul(viewPos, projection);

    // Transform normal
    output.normal = mul(input.normal, (float3x3)world);

    return output;
}
```

---

## Key Takeaways

1. ✅ Constant buffers pass data to shaders
2. ✅ Must be multiples of 16 bytes
3. ✅ Use DYNAMIC usage for frequent updates
4. ✅ Use DEFAULT usage for occasional updates
5. ✅ Separate buffers by update frequency
6. ✅ Pre-multiply matrices on CPU when possible
7. ✅ Sort objects to minimize buffer updates
8. ✅ Transpose matrices for HLSL (row-major)
9. ✅ Bind to specific register slots
10. ✅ Reuse buffers across frames

---

## Next Lesson Preview

In **Lesson 49: Input Layouts and Vertex Formats**, we'll explore:
- Creating input layouts
- Vertex attribute descriptions
- Multiple input layouts
- Instancing vertex attributes
- Vertex format best practices

---

## Summary Checklist

Before moving to Lesson 49, ensure you can:
- [ ] Create constant buffers
- [ ] Understand alignment rules
- [ ] Update buffers efficiently
- [ ] Use multiple constant buffers
- [ ] Bind buffers to shader stages
- [ ] Optimize buffer updates
- [ ] Pass matrices to shaders
- [ ] Handle per-frame vs per-object data

**Excellent work on completing Lesson 48!**

You now master constant buffers and shader data management!
