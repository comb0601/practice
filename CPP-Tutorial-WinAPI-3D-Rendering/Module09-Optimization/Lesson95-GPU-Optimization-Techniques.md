# Lesson 95: GPU Optimization Techniques
**Estimated Time: 3-4 hours**

## Table of Contents
1. [Introduction to GPU Optimization](#introduction)
2. [Understanding GPU Architecture](#gpu-architecture)
3. [Draw Call Optimization](#draw-calls)
4. [Texture Optimization](#textures)
5. [Shader Optimization](#shaders)
6. [State Change Minimization](#state-changes)
7. [GPU Memory Management](#gpu-memory)
8. [Occlusion and Frustum Culling](#culling)
9. [Complete Examples](#complete-examples)
10. [Exercises](#exercises)

## Introduction

GPU performance bottlenecks:
1. **CPU-GPU communication** (draw calls, uploads)
2. **Memory bandwidth** (texture fetches)
3. **Fill rate** (pixel shader complexity)
4. **Vertex processing** (vertex shader, geometry)

## Understanding GPU Architecture

### GPU vs CPU

| Aspect | CPU | GPU |
|--------|-----|-----|
| Cores | 4-16 | 1000-10000 |
| Clock Speed | 3-5 GHz | 1-2 GHz |
| Design | Low latency | High throughput |
| Best For | Serial tasks | Parallel tasks |

### GPU Pipeline Stages

```
Vertex Shader → Rasterization → Pixel Shader → Output Merger
     ↓              ↓                ↓             ↓
Transform       Convert to        Color/        Blend/
vertices        fragments        lighting       depth test
```

## Draw Call Optimization

### The Cost of Draw Calls

Each draw call involves:
- CPU overhead (~0.1-1 ms)
- Driver validation
- State changes
- GPU command submission

**Problem:**
```cpp
// 10,000 draw calls = 100-1000ms CPU time!
for (int i = 0; i < 10000; ++i) {
    DrawMesh(mesh[i]);  // Each call has overhead
}
```

### Solutions

**1. Static Batching** - Merge static meshes
**2. Dynamic Batching** - Combine similar objects
**3. Instancing** - Draw many copies efficiently (Lesson 96)

## Texture Optimization

### Texture Atlasing

Combine multiple textures into one:

```
Before: 100 textures, 100 material switches
[Tex1][Tex2][Tex3]...[Tex100]

After: 1 atlas, 1 material
[Tex1 Tex2 Tex3]
[Tex4 Tex5 Tex6]
...
```

**Benefits:**
- Fewer texture binds
- Better batching
- Reduced draw calls

### Mipmapping

```cpp
// Generate mipmaps
glGenerateMipmap(GL_TEXTURE_2D);

// Level 0: 1024×1024 (full resolution)
// Level 1: 512×512
// Level 2: 256×256
// ...
// Level 10: 1×1
```

**Benefits:**
- Better cache usage
- Reduces aliasing
- Faster rendering of distant objects

### Texture Compression

| Format | Compression | Quality | Use Case |
|--------|------------|---------|----------|
| DXT1 | 6:1 | Good | Opaque |
| DXT5 | 4:1 | Better | With alpha |
| BC7 | 3:1 | Best | Modern |

**Savings:**
```
Uncompressed: 1024×1024×4 = 4 MB
DXT5:         1024×1024×1 = 1 MB (4x smaller!)
```

## Shader Optimization

### Pixel Shader Optimization

**Expensive Operations (avoid in pixel shader):**
- `pow()`, `exp()`, `log()` - Very expensive
- `sin()`, `cos()` - Expensive
- Division - Moderate cost
- Square root - Moderate cost

**Optimization Techniques:**

**1. Move to Vertex Shader**
```cpp
// BAD: Per-pixel lighting
float4 PSMain(PS_INPUT input) : SV_Target {
    float3 L = normalize(lightPos - input.worldPos);  // Per pixel!
    float3 N = normalize(input.normal);
    float NdotL = max(dot(N, L), 0);
    return diffuseColor * NdotL;
}

// BETTER: Per-vertex lighting
VS_OUTPUT VSMain(VS_INPUT input) {
    VS_OUTPUT output;
    float3 L = normalize(lightPos - worldPos);
    float3 N = normalize(normal);
    output.lighting = max(dot(N, L), 0);  // Calculate once per vertex
    return output;
}

float4 PSMain(PS_INPUT input) : SV_Target {
    return diffuseColor * input.lighting;  // Just multiply
}
```

**2. Precalculate Constants**
```cpp
// BAD
cbuffer Constants {
    float3 lightDir;
};

float4 PSMain(PS_INPUT input) : SV_Target {
    float3 L = normalize(lightDir);  // Normalizing constant!
    // ...
}

// GOOD
cbuffer Constants {
    float3 lightDirNormalized;  // Pre-normalized on CPU
};

float4 PSMain(PS_INPUT input) : SV_Target {
    float3 L = lightDirNormalized;  // Already normalized
    // ...
}
```

**3. Use Approximations**
```cpp
// Expensive
float x = pow(base, exponent);

// Cheaper for specific cases
float x2 = x * x;           // pow(x, 2)
float x_half = sqrt(x);     // pow(x, 0.5)

// Fast specular approximation (Blinn-Phong)
float spec = pow(max(dot(N, H), 0), shininess);

// Faster approximation
float spec = exp2(shininess * log2(max(dot(N, H), 0)));
```

### Vertex Shader Optimization

**Keep vertex shaders simple:**
- Minimize texture reads
- Avoid branching
- Minimize ALU operations

## State Change Minimization

### The Cost of State Changes

State changes force GPU pipeline flushes:

```cpp
// SLOW: Many state changes
for (Mesh* mesh : meshes) {
    SetTexture(mesh->texture);    // State change
    SetMaterial(mesh->material);  // State change
    DrawMesh(mesh);
}
```

### Sort by State

```cpp
// FAST: Sort to minimize state changes
std::sort(meshes.begin(), meshes.end(), [](Mesh* a, Mesh* b) {
    if (a->shader != b->shader) return a->shader < b->shader;
    if (a->texture != b->texture) return a->texture < b->texture;
    return a->material < b->material;
});

// Now many meshes can share same state
ID3D11ShaderResourceView* currentTexture = nullptr;
for (Mesh* mesh : meshes) {
    if (mesh->texture != currentTexture) {
        SetTexture(mesh->texture);
        currentTexture = mesh->texture;
    }
    DrawMesh(mesh);
}
```

## GPU Memory Management

### Vertex Buffer Optimization

**Use Appropriate Usage:**

```cpp
// Static geometry (terrain, buildings)
D3D11_USAGE_IMMUTABLE  // Cannot modify, fastest

// Dynamic geometry (particles, updated each frame)
D3D11_USAGE_DYNAMIC    // CPU write, GPU read

// Staging (CPU read-back)
D3D11_USAGE_STAGING    // CPU ↔ GPU transfer
```

### Buffer Pooling

```cpp
// BAD: Create/destroy buffers every frame
void RenderParticles() {
    Buffer* vb = CreateVertexBuffer(particles);  // Allocation!
    DrawBuffer(vb);
    DestroyBuffer(vb);  // Deallocation!
}

// GOOD: Reuse buffers
class BufferPool {
    Buffer* buffer;
public:
    void RenderParticles() {
        if (!buffer) {
            buffer = CreateVertexBuffer(maxParticles);
        }
        UpdateBuffer(buffer, particles);  // Reuse existing buffer
        DrawBuffer(buffer);
    }
};
```

## Occlusion and Frustum Culling

### Frustum Culling

Don't submit objects outside camera view:

```cpp
bool IsInFrustum(const BoundingBox& box, const Frustum& frustum) {
    // Check if box intersects frustum planes
    for (const Plane& plane : frustum.planes) {
        if (DistanceToPlane(box, plane) < 0) {
            return false;  // Outside frustum
        }
    }
    return true;
}

void RenderScene() {
    Frustum frustum = ExtractFrustum(viewProjectionMatrix);

    for (Object* obj : objects) {
        if (IsInFrustum(obj->bounds, frustum)) {
            DrawObject(obj);  // Only draw visible objects
        }
    }
}
```

### Occlusion Culling

Don't draw objects hidden behind other objects:

**Techniques:**
1. **Hardware Occlusion Queries** - GPU-based
2. **Software Occlusion** - CPU rasterization
3. **Portal Culling** - For indoor scenes
4. **PVS (Potentially Visible Set)** - Precomputed

## Key Performance Metrics

### Measuring GPU Performance

```cpp
// DirectX 11: Timestamps
ID3D11Query* queryStart;
ID3D11Query* queryEnd;
ID3D11Query* queryDisjoint;

// Start timing
context->Begin(queryDisjoint);
context->End(queryStart);

// Render
Draw();

// End timing
context->End(queryEnd);
context->End(queryDisjoint);

// Get results
UINT64 startTime, endTime;
context->GetData(queryStart, &startTime, sizeof(UINT64), 0);
context->GetData(queryEnd, &endTime, sizeof(UINT64), 0);

D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjoint;
context->GetData(queryDisjoint, &disjoint, sizeof(disjoint), 0);

if (!disjoint.Disjoint) {
    double gpuTimeMs = (endTime - startTime) * 1000.0 / disjoint.Frequency;
}
```

## Summary

**Optimization Priority:**
1. **Reduce draw calls** (batching, instancing)
2. **Minimize state changes** (sort by state)
3. **Optimize textures** (atlasing, compression, mipmaps)
4. **Simplify shaders** (move to vertex, precalculate)
5. **Cull invisible objects** (frustum, occlusion)

**Common Bottlenecks:**
- Too many draw calls: Batch or instance
- High fill rate: Reduce overdraw, simpler shaders
- Memory bandwidth: Texture compression, mipmaps
- Vertex processing: LOD, fewer vertices

---

**Congratulations on completing Lesson 95!**
