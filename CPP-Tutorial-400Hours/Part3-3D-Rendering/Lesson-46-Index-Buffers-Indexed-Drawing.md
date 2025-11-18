# Lesson 46: Index Buffers and Indexed Drawing

**Duration**: 5 hours
**Difficulty**: Intermediate

## Table of Contents
1. Understanding Index Buffers
2. Creating Index Buffers
3. 16-bit vs 32-bit Indices
4. Indexed Drawing
5. Triangle Strips and Fans
6. Index Buffer Optimization
7. Complete Mesh Class
8. Working Examples

---

## 1. Understanding Index Buffers

**Index buffers** store indices that reference vertices in vertex buffers, eliminating duplicate vertices.

### Without Index Buffer:

```cpp
// 2 triangles (quad) = 6 vertices
Vertex vertices[] = {
    // Triangle 1
    {-0.5f, -0.5f, 0.0f},  // Bottom-left
    { 0.5f, -0.5f, 0.0f},  // Bottom-right
    {-0.5f,  0.5f, 0.0f},  // Top-left

    // Triangle 2
    { 0.5f, -0.5f, 0.0f},  // Bottom-right (duplicate)
    { 0.5f,  0.5f, 0.0f},  // Top-right
    {-0.5f,  0.5f, 0.0f}   // Top-left (duplicate)
};
```

### With Index Buffer:

```cpp
// 4 unique vertices
Vertex vertices[] = {
    {-0.5f, -0.5f, 0.0f},  // 0: Bottom-left
    { 0.5f, -0.5f, 0.0f},  // 1: Bottom-right
    { 0.5f,  0.5f, 0.0f},  // 2: Top-right
    {-0.5f,  0.5f, 0.0f}   // 3: Top-left
};

// 6 indices (2 triangles)
UINT indices[] = {
    0, 1, 3,  // Triangle 1
    1, 2, 3   // Triangle 2
};
```

**Benefits**:
- Reduces memory usage
- Improves GPU cache efficiency
- Essential for complex meshes

---

## 2. Creating Index Buffers

### Complete Index Buffer Class:

```cpp
#include <d3d11.h>
#include <vector>

class IndexBuffer {
public:
    IndexBuffer() = default;
    ~IndexBuffer() { Release(); }

    bool Create(ID3D11Device* device,
                const void* indexData,
                UINT indexCount,
                DXGI_FORMAT format = DXGI_FORMAT_R32_UINT,
                D3D11_USAGE usage = D3D11_USAGE_DEFAULT)
    {
        Release();

        m_indexCount = indexCount;
        m_format = format;
        m_indexSize = (format == DXGI_FORMAT_R16_UINT) ? 2 : 4;

        // Buffer description
        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth = m_indexSize * indexCount;
        desc.Usage = usage;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.CPUAccessFlags = (usage == D3D11_USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0;

        // Initial data
        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = indexData;

        // Create buffer
        HRESULT hr = device->CreateBuffer(
            &desc,
            indexData ? &initData : nullptr,
            &m_buffer
        );

        return SUCCEEDED(hr);
    }

    void Bind(ID3D11DeviceContext* context) {
        context->IASetIndexBuffer(m_buffer, m_format, 0);
    }

    void Release() {
        if (m_buffer) {
            m_buffer->Release();
            m_buffer = nullptr;
        }
    }

    ID3D11Buffer* GetBuffer() const { return m_buffer; }
    UINT GetIndexCount() const { return m_indexCount; }
    DXGI_FORMAT GetFormat() const { return m_format; }

private:
    ID3D11Buffer* m_buffer = nullptr;
    UINT m_indexCount = 0;
    UINT m_indexSize = 0;
    DXGI_FORMAT m_format;
};
```

---

## 3. 16-bit vs 32-bit Indices

### Comparison:

| Format | Size | Max Vertices | Use Case |
|--------|------|--------------|----------|
| R16_UINT | 2 bytes | 65,535 | Small meshes |
| R32_UINT | 4 bytes | 4,294,967,295 | Large meshes |

### Creating Different Index Types:

```cpp
class IndexBufferFactory {
public:
    // Create 16-bit index buffer
    static ID3D11Buffer* Create16BitIndexBuffer(
        ID3D11Device* device,
        const USHORT* indices,
        UINT count)
    {
        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth = sizeof(USHORT) * count;
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = indices;

        ID3D11Buffer* buffer = nullptr;
        device->CreateBuffer(&desc, &initData, &buffer);
        return buffer;
    }

    // Create 32-bit index buffer
    static ID3D11Buffer* Create32BitIndexBuffer(
        ID3D11Device* device,
        const UINT* indices,
        UINT count)
    {
        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth = sizeof(UINT) * count;
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = indices;

        ID3D11Buffer* buffer = nullptr;
        device->CreateBuffer(&desc, &initData, &buffer);
        return buffer;
    }

    // Automatically choose format based on vertex count
    static ID3D11Buffer* CreateIndexBufferAuto(
        ID3D11Device* device,
        const std::vector<UINT>& indices32,
        UINT maxVertexIndex,
        DXGI_FORMAT& outFormat)
    {
        if (maxVertexIndex < 65536) {
            // Convert to 16-bit
            std::vector<USHORT> indices16;
            indices16.reserve(indices32.size());
            for (UINT index : indices32) {
                indices16.push_back((USHORT)index);
            }

            outFormat = DXGI_FORMAT_R16_UINT;
            return Create16BitIndexBuffer(device, indices16.data(),
                                           (UINT)indices16.size());
        } else {
            outFormat = DXGI_FORMAT_R32_UINT;
            return Create32BitIndexBuffer(device, indices32.data(),
                                           (UINT)indices32.size());
        }
    }
};
```

---

## 4. Indexed Drawing

### Draw Commands:

```cpp
class IndexedDrawing {
public:
    // Draw indexed geometry
    static void DrawIndexed(
        ID3D11DeviceContext* context,
        UINT indexCount,
        UINT startIndex = 0,
        INT baseVertex = 0)
    {
        context->DrawIndexed(indexCount, startIndex, baseVertex);
    }

    // Draw subset of mesh
    static void DrawIndexedSubset(
        ID3D11DeviceContext* context,
        UINT indexStart,
        UINT indexCount,
        INT baseVertex = 0)
    {
        context->DrawIndexed(indexCount, indexStart, baseVertex);
    }

    // Draw indexed instanced
    static void DrawIndexedInstanced(
        ID3D11DeviceContext* context,
        UINT indexCountPerInstance,
        UINT instanceCount,
        UINT startIndex = 0,
        INT baseVertex = 0,
        UINT startInstance = 0)
    {
        context->DrawIndexedInstanced(
            indexCountPerInstance,
            instanceCount,
            startIndex,
            baseVertex,
            startInstance
        );
    }
};

// Example usage
void RenderMesh(ID3D11DeviceContext* context,
                ID3D11Buffer* vertexBuffer,
                ID3D11Buffer* indexBuffer,
                UINT indexCount)
{
    // Bind buffers
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Draw
    context->DrawIndexed(indexCount, 0, 0);
}
```

---

## 5. Triangle Strips and Fans

### Primitive Topologies:

```cpp
enum D3D11_PRIMITIVE_TOPOLOGY {
    D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST = 4,      // Most common
    D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP = 5,     // Connected triangles
    D3D11_PRIMITIVE_TOPOLOGY_LINELIST = 2,          // Lines
    D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP = 3,         // Connected lines
    D3D11_PRIMITIVE_TOPOLOGY_POINTLIST = 1          // Points
};
```

### Triangle List vs Strip:

```cpp
class TopologyComparison {
public:
    // Triangle List: 3 indices per triangle
    // Quad = 2 triangles = 6 indices
    static void CreateQuadList(std::vector<UINT>& indices) {
        indices = {
            0, 1, 2,  // Triangle 1
            2, 1, 3   // Triangle 2
        };
    }

    // Triangle Strip: Shared edges
    // Quad = 2 triangles = 4 indices
    static void CreateQuadStrip(std::vector<UINT>& indices) {
        indices = {
            0, 1, 2, 3  // Forms 2 triangles
        };
    }

    // For a grid of triangles, strips are more efficient
    static void CreateGridStrip(std::vector<UINT>& indices,
                                 UINT width, UINT height)
    {
        for (UINT y = 0; y < height - 1; ++y) {
            for (UINT x = 0; x < width; ++x) {
                indices.push_back(y * width + x);
                indices.push_back((y + 1) * width + x);
            }

            // Degenerate triangles to start new strip
            if (y < height - 2) {
                indices.push_back((y + 1) * width + (width - 1));
                indices.push_back((y + 1) * width);
            }
        }
    }
};
```

---

## 6. Index Buffer Optimization

### Post-Transform Vertex Cache Optimization:

```cpp
#include <map>

class IndexOptimizer {
public:
    // Optimize for vertex cache (Forsyth algorithm approximation)
    static void OptimizeForVertexCache(
        std::vector<UINT>& indices,
        UINT cacheSize = 32)
    {
        struct Triangle {
            UINT v[3];
            bool rendered;
        };

        // Group triangles by shared vertices
        std::vector<Triangle> triangles;
        for (size_t i = 0; i < indices.size(); i += 3) {
            Triangle tri;
            tri.v[0] = indices[i];
            tri.v[1] = indices[i + 1];
            tri.v[2] = indices[i + 2];
            tri.rendered = false;
            triangles.push_back(tri);
        }

        // Reorder indices (simplified version)
        std::vector<UINT> optimized;
        std::vector<UINT> cache;

        while (optimized.size() < indices.size()) {
            // Find triangle with most vertices in cache
            int bestTri = -1;
            int bestScore = -1;

            for (size_t i = 0; i < triangles.size(); ++i) {
                if (triangles[i].rendered) continue;

                int score = 0;
                for (int j = 0; j < 3; ++j) {
                    auto it = std::find(cache.begin(), cache.end(),
                                         triangles[i].v[j]);
                    if (it != cache.end()) {
                        score++;
                    }
                }

                if (score > bestScore) {
                    bestScore = score;
                    bestTri = (int)i;
                }
            }

            if (bestTri == -1) {
                // Find any unrendered triangle
                for (size_t i = 0; i < triangles.size(); ++i) {
                    if (!triangles[i].rendered) {
                        bestTri = (int)i;
                        break;
                    }
                }
            }

            // Add triangle
            for (int j = 0; j < 3; ++j) {
                optimized.push_back(triangles[bestTri].v[j]);

                // Update cache
                UINT vertex = triangles[bestTri].v[j];
                auto it = std::find(cache.begin(), cache.end(), vertex);
                if (it != cache.end()) {
                    cache.erase(it);
                }
                cache.insert(cache.begin(), vertex);
                if (cache.size() > cacheSize) {
                    cache.pop_back();
                }
            }

            triangles[bestTri].rendered = true;
        }

        indices = optimized;
    }

    // Calculate ACMR (Average Cache Miss Ratio)
    static float CalculateACMR(const std::vector<UINT>& indices,
                                UINT cacheSize = 32)
    {
        std::vector<UINT> cache;
        UINT misses = 0;

        for (UINT index : indices) {
            auto it = std::find(cache.begin(), cache.end(), index);
            if (it == cache.end()) {
                // Cache miss
                misses++;
                cache.insert(cache.begin(), index);
                if (cache.size() > cacheSize) {
                    cache.pop_back();
                }
            } else {
                // Cache hit - move to front
                cache.erase(it);
                cache.insert(cache.begin(), index);
            }
        }

        return (float)misses / (indices.size() / 3);
    }
};
```

---

## 7. Complete Mesh Class

### Full-Featured Mesh Class:

```cpp
#include <DirectXMath.h>
using namespace DirectX;

struct MeshVertex {
    XMFLOAT3 position;
    XMFLOAT3 normal;
    XMFLOAT2 texCoord;
};

class Mesh {
public:
    Mesh() = default;
    ~Mesh() { Release(); }

    bool Create(ID3D11Device* device,
                const std::vector<MeshVertex>& vertices,
                const std::vector<UINT>& indices)
    {
        m_vertexCount = (UINT)vertices.size();
        m_indexCount = (UINT)indices.size();

        // Create vertex buffer
        D3D11_BUFFER_DESC vbDesc = {};
        vbDesc.ByteWidth = sizeof(MeshVertex) * m_vertexCount;
        vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
        vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA vbData = {};
        vbData.pSysMem = vertices.data();

        HRESULT hr = device->CreateBuffer(&vbDesc, &vbData, &m_vertexBuffer);
        if (FAILED(hr)) return false;

        // Create index buffer (choose optimal format)
        UINT maxIndex = *std::max_element(indices.begin(), indices.end());
        m_indexFormat = (maxIndex < 65536) ? DXGI_FORMAT_R16_UINT
                                            : DXGI_FORMAT_R32_UINT;

        D3D11_BUFFER_DESC ibDesc = {};
        ibDesc.ByteWidth = (m_indexFormat == DXGI_FORMAT_R16_UINT)
                          ? sizeof(USHORT) * m_indexCount
                          : sizeof(UINT) * m_indexCount;
        ibDesc.Usage = D3D11_USAGE_IMMUTABLE;
        ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA ibData = {};

        if (m_indexFormat == DXGI_FORMAT_R16_UINT) {
            std::vector<USHORT> indices16;
            indices16.reserve(indices.size());
            for (UINT idx : indices) {
                indices16.push_back((USHORT)idx);
            }
            ibData.pSysMem = indices16.data();
            hr = device->CreateBuffer(&ibDesc, &ibData, &m_indexBuffer);
        } else {
            ibData.pSysMem = indices.data();
            hr = device->CreateBuffer(&ibDesc, &ibData, &m_indexBuffer);
        }

        return SUCCEEDED(hr);
    }

    void Render(ID3D11DeviceContext* context) {
        // Bind buffers
        UINT stride = sizeof(MeshVertex);
        UINT offset = 0;
        context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
        context->IASetIndexBuffer(m_indexBuffer, m_indexFormat, 0);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Draw
        context->DrawIndexed(m_indexCount, 0, 0);
    }

    void Release() {
        if (m_vertexBuffer) {
            m_vertexBuffer->Release();
            m_vertexBuffer = nullptr;
        }
        if (m_indexBuffer) {
            m_indexBuffer->Release();
            m_indexBuffer = nullptr;
        }
    }

    UINT GetVertexCount() const { return m_vertexCount; }
    UINT GetIndexCount() const { return m_indexCount; }

private:
    ID3D11Buffer* m_vertexBuffer = nullptr;
    ID3D11Buffer* m_indexBuffer = nullptr;
    UINT m_vertexCount = 0;
    UINT m_indexCount = 0;
    DXGI_FORMAT m_indexFormat;
};
```

---

## 8. Working Examples

### Example 1: Render Cube

```cpp
class CubeMesh {
public:
    static bool Create(ID3D11Device* device, Mesh& outMesh) {
        // Vertices (24 vertices for proper normals per face)
        std::vector<MeshVertex> vertices = {
            // Front face (Z+)
            {{-1, -1,  1}, {0, 0, 1}, {0, 0}},
            {{ 1, -1,  1}, {0, 0, 1}, {1, 0}},
            {{ 1,  1,  1}, {0, 0, 1}, {1, 1}},
            {{-1,  1,  1}, {0, 0, 1}, {0, 1}},

            // Back face (Z-)
            {{ 1, -1, -1}, {0, 0, -1}, {0, 0}},
            {{-1, -1, -1}, {0, 0, -1}, {1, 0}},
            {{-1,  1, -1}, {0, 0, -1}, {1, 1}},
            {{ 1,  1, -1}, {0, 0, -1}, {0, 1}},

            // Right face (X+)
            {{ 1, -1,  1}, {1, 0, 0}, {0, 0}},
            {{ 1, -1, -1}, {1, 0, 0}, {1, 0}},
            {{ 1,  1, -1}, {1, 0, 0}, {1, 1}},
            {{ 1,  1,  1}, {1, 0, 0}, {0, 1}},

            // Left face (X-)
            {{-1, -1, -1}, {-1, 0, 0}, {0, 0}},
            {{-1, -1,  1}, {-1, 0, 0}, {1, 0}},
            {{-1,  1,  1}, {-1, 0, 0}, {1, 1}},
            {{-1,  1, -1}, {-1, 0, 0}, {0, 1}},

            // Top face (Y+)
            {{-1,  1,  1}, {0, 1, 0}, {0, 0}},
            {{ 1,  1,  1}, {0, 1, 0}, {1, 0}},
            {{ 1,  1, -1}, {0, 1, 0}, {1, 1}},
            {{-1,  1, -1}, {0, 1, 0}, {0, 1}},

            // Bottom face (Y-)
            {{-1, -1, -1}, {0, -1, 0}, {0, 0}},
            {{ 1, -1, -1}, {0, -1, 0}, {1, 0}},
            {{ 1, -1,  1}, {0, -1, 0}, {1, 1}},
            {{-1, -1,  1}, {0, -1, 0}, {0, 1}}
        };

        // Indices (36 indices for 12 triangles)
        std::vector<UINT> indices = {
            0, 1, 2,  2, 3, 0,    // Front
            4, 5, 6,  6, 7, 4,    // Back
            8, 9, 10, 10, 11, 8,  // Right
            12, 13, 14, 14, 15, 12, // Left
            16, 17, 18, 18, 19, 16, // Top
            20, 21, 22, 22, 23, 20  // Bottom
        };

        return outMesh.Create(device, vertices, indices);
    }
};

// Usage
Mesh cube;
CubeMesh::Create(device, cube);
cube.Render(context);
```

### Example 2: Procedural Sphere

```cpp
class SphereMesh {
public:
    static bool Create(ID3D11Device* device,
                        Mesh& outMesh,
                        float radius,
                        UINT segments,
                        UINT rings)
    {
        std::vector<MeshVertex> vertices;
        std::vector<UINT> indices;

        // Generate vertices
        for (UINT ring = 0; ring <= rings; ++ring) {
            float phi = XM_PI * ring / rings;
            float y = radius * cosf(phi);
            float ringRadius = radius * sinf(phi);

            for (UINT segment = 0; segment <= segments; ++segment) {
                float theta = XM_2PI * segment / segments;
                float x = ringRadius * cosf(theta);
                float z = ringRadius * sinf(theta);

                MeshVertex vertex;
                vertex.position = XMFLOAT3(x, y, z);

                // Normal (normalized position for sphere)
                XMVECTOR normal = XMVector3Normalize(
                    XMLoadFloat3(&vertex.position));
                XMStoreFloat3(&vertex.normal, normal);

                // Texture coordinates
                vertex.texCoord = XMFLOAT2(
                    (float)segment / segments,
                    (float)ring / rings
                );

                vertices.push_back(vertex);
            }
        }

        // Generate indices
        for (UINT ring = 0; ring < rings; ++ring) {
            for (UINT segment = 0; segment < segments; ++segment) {
                UINT current = ring * (segments + 1) + segment;
                UINT next = current + segments + 1;

                // Two triangles per quad
                indices.push_back(current);
                indices.push_back(next);
                indices.push_back(current + 1);

                indices.push_back(current + 1);
                indices.push_back(next);
                indices.push_back(next + 1);
            }
        }

        return outMesh.Create(device, vertices, indices);
    }
};

// Usage
Mesh sphere;
SphereMesh::Create(device, sphere, 1.0f, 32, 16);
sphere.Render(context);
```

---

## Key Takeaways

1. ✅ Index buffers eliminate duplicate vertices
2. ✅ Use 16-bit indices for small meshes (<65K vertices)
3. ✅ Use 32-bit indices for large meshes
4. ✅ DrawIndexed() renders indexed geometry
5. ✅ Triangle strips reduce index count
6. ✅ Optimize indices for vertex cache
7. ✅ Combine vertex and index buffers in mesh class
8. ✅ ACMR measures cache efficiency
9. ✅ Procedural meshes use indexed geometry
10. ✅ Always release buffers when done

---

## Next Lesson Preview

In **Lesson 47: HLSL Shaders - Vertex and Pixel Shaders**, we'll explore:
- HLSL syntax and structure
- Writing vertex shaders
- Writing pixel shaders
- Shader compilation
- Semantic binding
- Complete shader pipeline

---

## Summary Checklist

Before moving to Lesson 47, ensure you can:
- [ ] Create index buffers
- [ ] Choose between 16-bit and 32-bit indices
- [ ] Draw indexed geometry
- [ ] Use different primitive topologies
- [ ] Optimize indices for vertex cache
- [ ] Create complete mesh class
- [ ] Generate procedural meshes
- [ ] Calculate cache efficiency

**Excellent work on completing Lesson 46!**

You now understand indexed rendering and can create efficient meshes!
