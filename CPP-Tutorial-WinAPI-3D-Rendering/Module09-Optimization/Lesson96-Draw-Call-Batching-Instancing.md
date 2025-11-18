# Lesson 96: Draw Call Batching & Instancing
**Estimated Time: 3-4 hours**

## Table of Contents
1. [Introduction to Batching](#introduction)
2. [The Draw Call Problem](#draw-call-problem)
3. [Static Batching](#static-batching)
4. [Dynamic Batching](#dynamic-batching)
5. [Hardware Instancing](#hardware-instancing)
6. [Instanced Rendering in DirectX](#instanced-directx)
7. [Performance Comparison](#performance-comparison)
8. [Complete Examples](#complete-examples)
9. [Exercises](#exercises)

## Introduction

**The Problem:** Draw calls are expensive!

```
10,000 individual draw calls = 100-500ms CPU time
1 instanced draw call = 0.5ms CPU time

Speedup: 200-1000x!
```

## The Draw Call Problem

### What is a Draw Call?

```cpp
// One draw call
DrawIndexed(indexCount, startIndex, baseVertex);
```

**Each draw call involves:**
1. CPU validates state
2. Driver translates to GPU commands
3. CPU submits command buffer
4. GPU receives and executes

**Cost:** 0.01-1ms CPU time per call

### The 10,000 Object Problem

```cpp
// Naive rendering: 10,000 draw calls
for (int i = 0; i < 10000; ++i) {
    SetTransform(objects[i].transform);
    Draw(objects[i].mesh);
}

// CPU time: 10,000 × 0.1ms = 1000ms = 1 second!
// Even though GPU could render this in 1ms
```

## Static Batching

### Concept

Combine multiple static meshes into one big mesh:

```
Before:
DrawMesh(tree1)    // Draw call 1
DrawMesh(tree2)    // Draw call 2
DrawMesh(tree3)    // Draw call 3

After:
CombinedMesh = tree1 + tree2 + tree3
DrawMesh(CombinedMesh)  // One draw call!
```

### Implementation

```cpp
struct MeshBatch {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
};

MeshBatch CombineMeshes(const std::vector<Mesh*>& meshes,
                        const std::vector<Matrix>& transforms) {
    MeshBatch batch;

    for (size_t i = 0; i < meshes.size(); ++i) {
        Mesh* mesh = meshes[i];
        Matrix transform = transforms[i];

        // Transform vertices to world space
        for (const Vertex& v : mesh->vertices) {
            Vertex transformed = v;
            transformed.position = transform * v.position;
            transformed.normal = transform * v.normal;
            batch.vertices.push_back(transformed);
        }

        // Add indices (offsetted)
        uint32_t vertexOffset = batch.vertices.size() - mesh->vertices.size();
        for (uint32_t idx : mesh->indices) {
            batch.indices.push_back(idx + vertexOffset);
        }
    }

    // Create GPU buffers
    batch.vertexBuffer = CreateVertexBuffer(batch.vertices);
    batch.indexBuffer = CreateIndexBuffer(batch.indices);

    return batch;
}
```

### Pros and Cons

**Pros:**
- Massive draw call reduction
- Simple to implement
- Works with any shader

**Cons:**
- Only for static geometry
- Increased memory usage
- Cannot move individual objects
- Frustum culling less efficient

## Dynamic Batching

### Concept

Batch objects that share materials/textures:

```cpp
// Group by material
std::map<Material*, std::vector<Mesh*>> batches;

for (Mesh* mesh : meshes) {
    batches[mesh->material].push_back(mesh);
}

// Draw each batch
for (auto& [material, meshes] : batches) {
    SetMaterial(material);

    for (Mesh* mesh : meshes) {
        SetTransform(mesh->transform);
        Draw(mesh);
    }
}

// Fewer material changes = faster
```

### Requirements

Objects must share:
- Same shader
- Same material
- Same texture(s)
- Similar vertex format

## Hardware Instancing

### Concept

Draw many copies of the same mesh with one draw call:

```cpp
// Traditional: N draw calls
for (int i = 0; i < N; ++i) {
    SetTransform(transforms[i]);
    DrawMesh(mesh);  // N calls
}

// Instanced: 1 draw call
DrawMeshInstanced(mesh, transforms, N);  // 1 call!
```

### How It Works

**1. Store per-instance data:**
```cpp
struct InstanceData {
    Matrix worldMatrix;
    float4 color;
};

InstanceData instances[1000];
```

**2. Create instance buffer:**
```cpp
D3D11_BUFFER_DESC bufferDesc = {};
bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
bufferDesc.ByteWidth = sizeof(InstanceData) * instanceCount;

device->CreateBuffer(&bufferDesc, nullptr, &instanceBuffer);
```

**3. Set input layout:**
```cpp
D3D11_INPUT_ELEMENT_DESC layout[] = {
    // Per-vertex data
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
     D3D11_INPUT_PER_VERTEX_DATA, 0},

    // Per-instance data
    {"WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,
     D3D11_INPUT_PER_INSTANCE_DATA, 1},
    {"WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
     D3D11_INPUT_PER_INSTANCE_DATA, 1},
    // ...
};
```

**4. Draw instanced:**
```cpp
context->DrawIndexedInstanced(
    indexCount,      // Indices per instance
    instanceCount,   // Number of instances
    0,               // Start index
    0,               // Base vertex
    0                // Start instance
);
```

## Instanced Rendering in DirectX

### Vertex Shader

```hlsl
cbuffer PerFrame : register(b0) {
    matrix viewProjection;
};

struct VS_INPUT {
    float3 position : POSITION;
    float3 normal : NORMAL;

    // Per-instance data
    row_major matrix world : WORLD;  // Instance transform
    float4 color : COLOR;            // Instance color
};

struct VS_OUTPUT {
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float4 color : COLOR;
};

VS_OUTPUT main(VS_INPUT input) {
    VS_OUTPUT output;

    // Transform by instance matrix
    float4 worldPos = mul(float4(input.position, 1), input.world);
    output.position = mul(worldPos, viewProjection);

    // Transform normal
    output.normal = mul(input.normal, (float3x3)input.world);

    // Pass instance color
    output.color = input.color;

    return output;
}
```

### C++ Setup

```cpp
class InstancedRenderer {
private:
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
    ID3D11Buffer* instanceBuffer;

    struct InstanceData {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4 color;
    };

    std::vector<InstanceData> instances;

public:
    void AddInstance(const Matrix& transform, const Color& color) {
        InstanceData data;
        data.world = transform;
        data.color = color;
        instances.push_back(data);
    }

    void Render(ID3D11DeviceContext* context) {
        // Update instance buffer
        D3D11_MAPPED_SUBRESOURCE mapped;
        context->Map(instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
        memcpy(mapped.pData, instances.data(),
               instances.size() * sizeof(InstanceData));
        context->Unmap(instanceBuffer, 0);

        // Set vertex buffers
        ID3D11Buffer* buffers[] = { vertexBuffer, instanceBuffer };
        UINT strides[] = { sizeof(Vertex), sizeof(InstanceData) };
        UINT offsets[] = { 0, 0 };
        context->IASetVertexBuffers(0, 2, buffers, strides, offsets);
        context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

        // Draw all instances
        context->DrawIndexedInstanced(
            indexCount,
            instances.size(),  // Instance count
            0, 0, 0
        );
    }
};
```

## Performance Comparison

### Benchmark Results (10,000 cubes)

```
Method                  | Draw Calls | CPU Time | GPU Time
------------------------|------------|----------|----------
Individual draws        | 10,000     | 500 ms   | 5 ms
Static batch            | 1          | 0.5 ms   | 5 ms
Hardware instancing     | 1          | 0.5 ms   | 2 ms

CPU Speedup: 1000x
```

### When to Use Each Method

**Individual Draws:**
- ✓ Few objects (< 100)
- ✓ Each object unique
- ✗ Never for many objects

**Static Batching:**
- ✓ Static, non-moving objects
- ✓ Simple frustum culling acceptable
- ✗ Dynamic objects
- ✗ Need individual culling

**Dynamic Batching:**
- ✓ Many small objects
- ✓ Share materials
- ✗ Large objects

**Hardware Instancing:**
- ✓ Many copies of same mesh
- ✓ Different transforms/colors
- ✓ Dynamic objects
- ✓ Individual culling needed
- Best choice for most cases!

## Advanced Instancing

### GPU Culling with Instancing

```cpp
// Compute shader culls instances before rendering
[numthreads(64, 1, 1)]
void CullInstances(uint3 threadID : SV_DispatchThreadID) {
    uint instanceID = threadID.x;
    if (instanceID >= instanceCount) return;

    InstanceData instance = inputInstances[instanceID];

    // Frustum culling
    if (IsInFrustum(instance.bounds, frustum)) {
        uint outputIndex;
        InterlockedAdd(visibleCount, 1, outputIndex);
        visibleInstances[outputIndex] = instance;
    }
}

// Then draw only visible instances
context->DrawIndexedInstanced(indexCount, visibleCount, 0, 0, 0);
```

## Summary

**Key Takeaways:**
1. **Draw calls are expensive** - Minimize them!
2. **Static batching** - For static geometry
3. **Hardware instancing** - For repeated meshes
4. **Sort by state** - Minimize material changes
5. **Measure impact** - Profile before/after

**Typical Results:**
- 10,000 objects: Individual (500ms) → Instanced (0.5ms)
- **Speedup: 1000x!**

---

**Congratulations on completing Lesson 96!**
