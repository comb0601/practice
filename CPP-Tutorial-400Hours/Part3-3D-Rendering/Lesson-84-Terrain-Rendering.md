# Lesson 84: Terrain Rendering

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Terrain Rendering
2. Heightmap Generation and Loading
3. Terrain Mesh Generation
4. LOD (Level of Detail) Systems
5. Quadtree Terrain
6. Texture Splatting
7. Triplanar Mapping
8. Terrain Normal Calculation
9. Clipmap Terrain for Large Worlds
10. Terrain Physics and Collision
11. Complete Terrain System
12. Exercises

---

## 1. Introduction to Terrain Rendering

Terrain rendering is crucial for creating expansive outdoor environments in games and simulations. Modern terrain systems must handle vast landscapes efficiently while maintaining visual quality.

### Key Challenges

- **Scale**: Rendering large areas (kilometers)
- **Detail**: Maintaining quality at all distances
- **Performance**: Real-time rendering requirements
- **Memory**: Managing texture and geometry data
- **Variety**: Avoiding repetitive patterns

### Terrain Representation Methods

1. **Heightmap**: 2D grid with height values
2. **Voxel**: 3D grid for caves and overhangs
3. **Procedural**: Algorithmically generated
4. **Mesh-based**: Artist-created geometry

---

## 2. Heightmap System

```cpp
// Heightmap.h
#pragma once
#include <DirectXMath.h>
#include <vector>
#include <string>

using namespace DirectX;

class Heightmap
{
public:
    Heightmap();
    ~Heightmap();

    // Load heightmap from file (grayscale image)
    bool LoadFromFile(const std::string& filename);

    // Generate procedural heightmap
    void GenerateProceduralHeightmap(int width, int height, int seed);

    // Get height at specific position
    float GetHeight(int x, int z) const;
    float GetHeightInterpolated(float x, float z) const;

    // Get normal at position
    XMFLOAT3 GetNormal(int x, int z) const;

    // Terrain modification
    void RaiseTerrain(float centerX, float centerZ, float radius, float amount);
    void SmoothTerrain(float centerX, float centerZ, float radius);

    // Accessors
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    float GetHeightScale() const { return m_heightScale; }
    const std::vector<float>& GetHeightData() const { return m_heightData; }

    void SetHeightScale(float scale) { m_heightScale = scale; }

private:
    std::vector<float> m_heightData;
    int m_width;
    int m_height;
    float m_heightScale;

    float BilinearInterpolate(float q11, float q12, float q21, float q22,
                              float x1, float x2, float y1, float y2,
                              float x, float y) const;
};

// Heightmap.cpp
#include "Heightmap.h"
#include "PerlinNoise.h" // From Lesson 82
#include <algorithm>
#include <cmath>

// For image loading, you might use stb_image or DirectXTex
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Heightmap::Heightmap()
    : m_width(0), m_height(0), m_heightScale(100.0f)
{
}

Heightmap::~Heightmap()
{
}

bool Heightmap::LoadFromFile(const std::string& filename)
{
    int channels;
    unsigned char* imageData = stbi_load(filename.c_str(), &m_width, &m_height,
                                         &channels, 1); // Force grayscale

    if (!imageData)
        return false;

    m_heightData.resize(m_width * m_height);

    // Convert to normalized float values [0, 1]
    for (int i = 0; i < m_width * m_height; ++i)
    {
        m_heightData[i] = static_cast<float>(imageData[i]) / 255.0f;
    }

    stbi_image_free(imageData);
    return true;
}

void Heightmap::GenerateProceduralHeightmap(int width, int height, int seed)
{
    m_width = width;
    m_height = height;
    m_heightData.resize(width * height);

    PerlinNoise noise(seed);

    // Generate multi-octave Perlin noise
    for (int z = 0; z < height; ++z)
    {
        for (int x = 0; x < width; ++x)
        {
            float nx = static_cast<float>(x) / width;
            float nz = static_cast<float>(z) / height;

            // Combine multiple octaves
            float elevation = 0.0f;

            // Large features
            elevation += noise.FBM2D(nx * 4.0f, nz * 4.0f, 4, 2.0f, 0.5f) * 0.5f;

            // Medium features
            elevation += noise.FBM2D(nx * 16.0f, nz * 16.0f, 4, 2.0f, 0.5f) * 0.25f;

            // Fine details
            elevation += noise.FBM2D(nx * 64.0f, nz * 64.0f, 4, 2.0f, 0.5f) * 0.125f;

            // Normalize to [0, 1]
            elevation = (elevation + 1.0f) * 0.5f;
            elevation = std::clamp(elevation, 0.0f, 1.0f);

            m_heightData[z * width + x] = elevation;
        }
    }
}

float Heightmap::GetHeight(int x, int z) const
{
    if (x < 0 || x >= m_width || z < 0 || z >= m_height)
        return 0.0f;

    return m_heightData[z * m_width + x] * m_heightScale;
}

float Heightmap::BilinearInterpolate(float q11, float q12, float q21, float q22,
                                     float x1, float x2, float y1, float y2,
                                     float x, float y) const
{
    float r1 = ((x2 - x) / (x2 - x1)) * q11 + ((x - x1) / (x2 - x1)) * q21;
    float r2 = ((x2 - x) / (x2 - x1)) * q12 + ((x - x1) / (x2 - x1)) * q22;
    return ((y2 - y) / (y2 - y1)) * r1 + ((y - y1) / (y2 - y1)) * r2;
}

float Heightmap::GetHeightInterpolated(float x, float z) const
{
    int x0 = static_cast<int>(std::floor(x));
    int z0 = static_cast<int>(std::floor(z));
    int x1 = x0 + 1;
    int z1 = z0 + 1;

    // Get fractional parts
    float fx = x - x0;
    float fz = z - z0;

    // Get heights at four corners
    float h00 = GetHeight(x0, z0);
    float h10 = GetHeight(x1, z0);
    float h01 = GetHeight(x0, z1);
    float h11 = GetHeight(x1, z1);

    // Bilinear interpolation
    return BilinearInterpolate(h00, h01, h10, h11,
                              static_cast<float>(x0), static_cast<float>(x1),
                              static_cast<float>(z0), static_cast<float>(z1),
                              x, z);
}

XMFLOAT3 Heightmap::GetNormal(int x, int z) const
{
    float hL = GetHeight(x - 1, z);
    float hR = GetHeight(x + 1, z);
    float hD = GetHeight(x, z - 1);
    float hU = GetHeight(x, z + 1);

    XMFLOAT3 normal;
    normal.x = hL - hR;
    normal.y = 2.0f;
    normal.z = hD - hU;

    XMVECTOR n = XMLoadFloat3(&normal);
    n = XMVector3Normalize(n);
    XMStoreFloat3(&normal, n);

    return normal;
}

void Heightmap::RaiseTerrain(float centerX, float centerZ, float radius, float amount)
{
    int minX = std::max(0, static_cast<int>(centerX - radius));
    int maxX = std::min(m_width - 1, static_cast<int>(centerX + radius));
    int minZ = std::max(0, static_cast<int>(centerZ - radius));
    int maxZ = std::min(m_height - 1, static_cast<int>(centerZ + radius));

    for (int z = minZ; z <= maxZ; ++z)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            float dx = x - centerX;
            float dz = z - centerZ;
            float distance = std::sqrt(dx * dx + dz * dz);

            if (distance <= radius)
            {
                float falloff = 1.0f - (distance / radius);
                falloff = falloff * falloff; // Smooth falloff

                int index = z * m_width + x;
                m_heightData[index] += (amount / m_heightScale) * falloff;
                m_heightData[index] = std::clamp(m_heightData[index], 0.0f, 1.0f);
            }
        }
    }
}

void Heightmap::SmoothTerrain(float centerX, float centerZ, float radius)
{
    int minX = std::max(1, static_cast<int>(centerX - radius));
    int maxX = std::min(m_width - 2, static_cast<int>(centerX + radius));
    int minZ = std::max(1, static_cast<int>(centerZ - radius));
    int maxZ = std::min(m_height - 2, static_cast<int>(centerZ + radius));

    std::vector<float> smoothedData = m_heightData;

    for (int z = minZ; z <= maxZ; ++z)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            float dx = x - centerX;
            float dz = z - centerZ;
            float distance = std::sqrt(dx * dx + dz * dz);

            if (distance <= radius)
            {
                // Average with neighbors
                float sum = 0.0f;
                int count = 0;

                for (int oz = -1; oz <= 1; ++oz)
                {
                    for (int ox = -1; ox <= 1; ++ox)
                    {
                        sum += m_heightData[(z + oz) * m_width + (x + ox)];
                        ++count;
                    }
                }

                float falloff = 1.0f - (distance / radius);
                int index = z * m_width + x;
                smoothedData[index] = m_heightData[index] * (1.0f - falloff) +
                                     (sum / count) * falloff;
            }
        }
    }

    m_heightData = smoothedData;
}
```

---

## 3. Terrain Mesh Generation

```cpp
// TerrainMesh.h
#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include "Heightmap.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

struct TerrainVertex
{
    XMFLOAT3 Position;
    XMFLOAT3 Normal;
    XMFLOAT2 TexCoord;
    XMFLOAT3 Tangent;
};

class TerrainMesh
{
public:
    TerrainMesh();
    ~TerrainMesh();

    bool Initialize(ID3D11Device* device, const Heightmap& heightmap,
                   float gridSpacing);
    void Render(ID3D11DeviceContext* context);

    int GetIndexCount() const { return m_indexCount; }

private:
    bool CreateBuffers(ID3D11Device* device, const std::vector<TerrainVertex>& vertices,
                      const std::vector<uint32_t>& indices);
    void CalculateTangents(std::vector<TerrainVertex>& vertices,
                          const std::vector<uint32_t>& indices);

    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11Buffer> m_indexBuffer;
    int m_indexCount;
};

// TerrainMesh.cpp
#include "TerrainMesh.h"

TerrainMesh::TerrainMesh()
    : m_indexCount(0)
{
}

TerrainMesh::~TerrainMesh()
{
}

bool TerrainMesh::Initialize(ID3D11Device* device, const Heightmap& heightmap,
                             float gridSpacing)
{
    int width = heightmap.GetWidth();
    int height = heightmap.GetHeight();

    std::vector<TerrainVertex> vertices;
    std::vector<uint32_t> indices;

    // Generate vertices
    vertices.reserve(width * height);

    for (int z = 0; z < height; ++z)
    {
        for (int x = 0; x < width; ++x)
        {
            TerrainVertex vertex;

            // Position
            vertex.Position.x = x * gridSpacing;
            vertex.Position.y = heightmap.GetHeight(x, z);
            vertex.Position.z = z * gridSpacing;

            // Normal
            vertex.Normal = heightmap.GetNormal(x, z);

            // Texture coordinates
            vertex.TexCoord.x = static_cast<float>(x) / (width - 1);
            vertex.TexCoord.y = static_cast<float>(z) / (height - 1);

            // Tangent (will be calculated later)
            vertex.Tangent = XMFLOAT3(1, 0, 0);

            vertices.push_back(vertex);
        }
    }

    // Generate indices
    indices.reserve((width - 1) * (height - 1) * 6);

    for (int z = 0; z < height - 1; ++z)
    {
        for (int x = 0; x < width - 1; ++x)
        {
            int topLeft = z * width + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * width + x;
            int bottomRight = bottomLeft + 1;

            // First triangle
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Second triangle
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    // Calculate tangents
    CalculateTangents(vertices, indices);

    m_indexCount = static_cast<int>(indices.size());

    return CreateBuffers(device, vertices, indices);
}

void TerrainMesh::CalculateTangents(std::vector<TerrainVertex>& vertices,
                                    const std::vector<uint32_t>& indices)
{
    std::vector<XMFLOAT3> tan1(vertices.size(), XMFLOAT3(0, 0, 0));
    std::vector<XMFLOAT3> tan2(vertices.size(), XMFLOAT3(0, 0, 0));

    // Calculate tangents for each triangle
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        uint32_t i0 = indices[i];
        uint32_t i1 = indices[i + 1];
        uint32_t i2 = indices[i + 2];

        XMVECTOR v0 = XMLoadFloat3(&vertices[i0].Position);
        XMVECTOR v1 = XMLoadFloat3(&vertices[i1].Position);
        XMVECTOR v2 = XMLoadFloat3(&vertices[i2].Position);

        XMVECTOR uv0 = XMLoadFloat2(&vertices[i0].TexCoord);
        XMVECTOR uv1 = XMLoadFloat2(&vertices[i1].TexCoord);
        XMVECTOR uv2 = XMLoadFloat2(&vertices[i2].TexCoord);

        XMVECTOR edge1 = v1 - v0;
        XMVECTOR edge2 = v2 - v0;
        XMVECTOR deltaUV1 = uv1 - uv0;
        XMVECTOR deltaUV2 = uv2 - uv0;

        float du1 = XMVectorGetX(deltaUV1);
        float dv1 = XMVectorGetY(deltaUV1);
        float du2 = XMVectorGetX(deltaUV2);
        float dv2 = XMVectorGetY(deltaUV2);

        float f = 1.0f / (du1 * dv2 - du2 * dv1);

        XMFLOAT3 tangent;
        tangent.x = f * (dv2 * XMVectorGetX(edge1) - dv1 * XMVectorGetX(edge2));
        tangent.y = f * (dv2 * XMVectorGetY(edge1) - dv1 * XMVectorGetY(edge2));
        tangent.z = f * (dv2 * XMVectorGetZ(edge1) - dv1 * XMVectorGetZ(edge2));

        // Accumulate tangents
        XMVECTOR t = XMLoadFloat3(&tangent);
        XMVECTOR t1_0 = XMLoadFloat3(&tan1[i0]);
        XMVECTOR t1_1 = XMLoadFloat3(&tan1[i1]);
        XMVECTOR t1_2 = XMLoadFloat3(&tan1[i2]);

        XMStoreFloat3(&tan1[i0], t1_0 + t);
        XMStoreFloat3(&tan1[i1], t1_1 + t);
        XMStoreFloat3(&tan1[i2], t1_2 + t);
    }

    // Orthogonalize and normalize tangents
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        XMVECTOR n = XMLoadFloat3(&vertices[i].Normal);
        XMVECTOR t = XMLoadFloat3(&tan1[i]);

        // Gram-Schmidt orthogonalize
        t = XMVector3Normalize(t - n * XMVector3Dot(n, t));

        XMStoreFloat3(&vertices[i].Tangent, t);
    }
}

bool TerrainMesh::CreateBuffers(ID3D11Device* device,
                                const std::vector<TerrainVertex>& vertices,
                                const std::vector<uint32_t>& indices)
{
    // Create vertex buffer
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vbDesc.ByteWidth = static_cast<UINT>(sizeof(TerrainVertex) * vertices.size());
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = vertices.data();

    HRESULT hr = device->CreateBuffer(&vbDesc, &vbData, &m_vertexBuffer);
    if (FAILED(hr))
        return false;

    // Create index buffer
    D3D11_BUFFER_DESC ibDesc = {};
    ibDesc.Usage = D3D11_USAGE_IMMUTABLE;
    ibDesc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * indices.size());
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA ibData = {};
    ibData.pSysMem = indices.data();

    hr = device->CreateBuffer(&ibDesc, &ibData, &m_indexBuffer);
    return SUCCEEDED(hr);
}

void TerrainMesh::Render(ID3D11DeviceContext* context)
{
    UINT stride = sizeof(TerrainVertex);
    UINT offset = 0;

    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->DrawIndexed(m_indexCount, 0, 0);
}
```

---

## 4. LOD (Level of Detail) System

```cpp
// TerrainLOD.h
#pragma once
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

struct LODLevel
{
    int GridSize;        // Vertices per side
    float MaxDistance;   // Maximum viewing distance
    float GridSpacing;   // Space between vertices
};

class TerrainLODManager
{
public:
    TerrainLODManager();

    void Initialize();
    int CalculateLOD(const XMFLOAT3& terrainChunkPos, const XMFLOAT3& cameraPos) const;
    float GetGridSpacing(int lod) const;

    const std::vector<LODLevel>& GetLODLevels() const { return m_lodLevels; }

private:
    std::vector<LODLevel> m_lodLevels;
};

// TerrainLOD.cpp
#include "TerrainLOD.h"
#include <cmath>

TerrainLODManager::TerrainLODManager()
{
}

void TerrainLODManager::Initialize()
{
    // Define LOD levels
    // LOD 0: Highest detail
    LODLevel lod0;
    lod0.GridSize = 65;
    lod0.MaxDistance = 100.0f;
    lod0.GridSpacing = 1.0f;
    m_lodLevels.push_back(lod0);

    // LOD 1: Medium detail
    LODLevel lod1;
    lod1.GridSize = 33;
    lod1.MaxDistance = 300.0f;
    lod1.GridSpacing = 2.0f;
    m_lodLevels.push_back(lod1);

    // LOD 2: Low detail
    LODLevel lod2;
    lod2.GridSize = 17;
    lod2.MaxDistance = 600.0f;
    lod2.GridSpacing = 4.0f;
    m_lodLevels.push_back(lod2);

    // LOD 3: Very low detail
    LODLevel lod3;
    lod3.GridSize = 9;
    lod3.MaxDistance = 1200.0f;
    lod3.GridSpacing = 8.0f;
    m_lodLevels.push_back(lod3);
}

int TerrainLODManager::CalculateLOD(const XMFLOAT3& terrainChunkPos,
                                    const XMFLOAT3& cameraPos) const
{
    float dx = terrainChunkPos.x - cameraPos.x;
    float dz = terrainChunkPos.z - cameraPos.z;
    float distance = std::sqrt(dx * dx + dz * dz);

    for (size_t i = 0; i < m_lodLevels.size(); ++i)
    {
        if (distance < m_lodLevels[i].MaxDistance)
            return static_cast<int>(i);
    }

    return static_cast<int>(m_lodLevels.size() - 1);
}

float TerrainLODManager::GetGridSpacing(int lod) const
{
    if (lod >= 0 && lod < static_cast<int>(m_lodLevels.size()))
        return m_lodLevels[lod].GridSpacing;

    return m_lodLevels.back().GridSpacing;
}
```

---

## 5. Texture Splatting Shader

```hlsl
// TerrainSplatting_PS.hlsl
cbuffer MaterialBuffer : register(b0)
{
    float4 SlopeThresholds; // x=grass, y=rock, z=snow, w=unused
    float4 HeightThresholds;
    float TextureScale;
    float BlendSharpness;
    float2 Padding;
};

Texture2D GrassTexture : register(t0);
Texture2D RockTexture : register(t1);
Texture2D SnowTexture : register(t2);
Texture2D SandTexture : register(t3);
Texture2D SplatMap : register(t4); // Optional artist-painted blend map
SamplerState SamplerLinear : register(s0);

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float3 WorldPos : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float3 Tangent : TANGENT;
};

float4 SampleTexture(Texture2D tex, float2 uv)
{
    return tex.Sample(SamplerLinear, uv * TextureScale);
}

float4 main(PS_INPUT input) : SV_TARGET
{
    // Normalize inputs
    float3 normal = normalize(input.Normal);

    // Calculate slope (0 = flat, 1 = vertical)
    float slope = 1.0 - normal.y;

    // Calculate height-based blending
    float height = input.WorldPos.y;

    // Sample textures
    float4 grassColor = SampleTexture(GrassTexture, input.TexCoord);
    float4 rockColor = SampleTexture(RockTexture, input.TexCoord);
    float4 snowColor = SampleTexture(SnowTexture, input.TexCoord);
    float4 sandColor = SampleTexture(SandTexture, input.TexCoord);

    // Calculate blend weights
    float grassWeight = 1.0 - smoothstep(0.0, SlopeThresholds.x, slope);
    grassWeight *= smoothstep(0.0, HeightThresholds.x, height);

    float rockWeight = smoothstep(SlopeThresholds.x, SlopeThresholds.y, slope);

    float snowWeight = smoothstep(HeightThresholds.y, HeightThresholds.z, height);
    snowWeight *= grassWeight; // Snow only on flatter surfaces

    float sandWeight = 1.0 - smoothstep(0.0, HeightThresholds.x, height);
    sandWeight *= grassWeight;

    // Normalize weights
    float totalWeight = grassWeight + rockWeight + snowWeight + sandWeight;
    if(totalWeight > 0.0)
    {
        grassWeight /= totalWeight;
        rockWeight /= totalWeight;
        snowWeight /= totalWeight;
        sandWeight /= totalWeight;
    }

    // Height-based blending (optional enhancement)
    float4 splatMap = SplatMap.Sample(SamplerLinear, input.TexCoord);

    grassWeight *= splatMap.r;
    rockWeight *= splatMap.g;
    snowWeight *= splatMap.b;
    sandWeight *= splatMap.a;

    // Renormalize after splatmap
    totalWeight = grassWeight + rockWeight + snowWeight + sandWeight;
    if(totalWeight > 0.0)
    {
        grassWeight /= totalWeight;
        rockWeight /= totalWeight;
        snowWeight /= totalWeight;
        sandWeight /= totalWeight;
    }

    // Blend textures
    float4 finalColor = grassColor * grassWeight +
                        rockColor * rockWeight +
                        snowColor * snowWeight +
                        sandColor * sandWeight;

    return finalColor;
}
```

---

## 6. Triplanar Mapping

```hlsl
// TriplanarMapping.hlsl

float4 TriplanarSample(Texture2D tex, float3 worldPos, float3 normal, float scale)
{
    // Calculate blend weights based on normal
    float3 blendWeights = abs(normal);
    blendWeights = blendWeights / (blendWeights.x + blendWeights.y + blendWeights.z);

    // Sample from three planes
    float4 xProjection = tex.Sample(SamplerLinear, worldPos.yz * scale);
    float4 yProjection = tex.Sample(SamplerLinear, worldPos.xz * scale);
    float4 zProjection = tex.Sample(SamplerLinear, worldPos.xy * scale);

    // Blend based on normal
    return xProjection * blendWeights.x +
           yProjection * blendWeights.y +
           zProjection * blendWeights.z;
}

float4 TriplanarSampleWithNormal(Texture2D albedoTex, Texture2D normalTex,
                                 float3 worldPos, float3 worldNormal,
                                 float scale, out float3 surfaceNormal)
{
    // Calculate blend weights
    float3 blendWeights = abs(worldNormal);
    blendWeights = pow(blendWeights, 4.0); // Sharpen transition
    blendWeights = blendWeights / (blendWeights.x + blendWeights.y + blendWeights.z);

    // Sample albedo
    float4 xAlbedo = albedoTex.Sample(SamplerLinear, worldPos.yz * scale);
    float4 yAlbedo = albedoTex.Sample(SamplerLinear, worldPos.xz * scale);
    float4 zAlbedo = albedoTex.Sample(SamplerLinear, worldPos.xy * scale);

    float4 albedo = xAlbedo * blendWeights.x +
                    yAlbedo * blendWeights.y +
                    zAlbedo * blendWeights.z;

    // Sample normals
    float3 xNormal = normalTex.Sample(SamplerLinear, worldPos.yz * scale).xyz * 2.0 - 1.0;
    float3 yNormal = normalTex.Sample(SamplerLinear, worldPos.xz * scale).xyz * 2.0 - 1.0;
    float3 zNormal = normalTex.Sample(SamplerLinear, worldPos.xy * scale).xyz * 2.0 - 1.0;

    // Transform normals to world space
    xNormal = float3(0, xNormal.y, xNormal.x);
    yNormal = float3(yNormal.x, 0, yNormal.y);
    zNormal = float3(zNormal.x, zNormal.y, 0);

    surfaceNormal = normalize(
        xNormal * blendWeights.x +
        yNormal * blendWeights.y +
        zNormal * blendWeights.z
    );

    return albedo;
}
```

---

## 7. Complete Terrain Renderer

```cpp
// TerrainRenderer.h
#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <memory>
#include "Heightmap.h"
#include "TerrainMesh.h"
#include "TerrainLOD.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class TerrainRenderer
{
public:
    TerrainRenderer();
    ~TerrainRenderer();

    bool Initialize(ID3D11Device* device, const std::string& heightmapFile);
    void Update(float deltaTime, const XMFLOAT3& cameraPos);
    void Render(ID3D11DeviceContext* context, const XMMATRIX& viewProj);

    Heightmap& GetHeightmap() { return m_heightmap; }
    float GetHeightAt(float x, float z) const;

private:
    bool CreateShaders(ID3D11Device* device);
    bool LoadTextures(ID3D11Device* device);

    Heightmap m_heightmap;
    std::unique_ptr<TerrainMesh> m_mesh;
    TerrainLODManager m_lodManager;

    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3D11PixelShader> m_pixelShader;
    ComPtr<ID3D11InputLayout> m_inputLayout;
    ComPtr<ID3D11Buffer> m_constantBuffer;

    ComPtr<ID3D11ShaderResourceView> m_grassTexture;
    ComPtr<ID3D11ShaderResourceView> m_rockTexture;
    ComPtr<ID3D11ShaderResourceView> m_snowTexture;
    ComPtr<ID3D11ShaderResourceView> m_sandTexture;

    ComPtr<ID3D11SamplerState> m_samplerState;

    XMFLOAT3 m_cameraPosition;
};
```

---

## 8. Clipmap Terrain

```cpp
// ClimapTerrain.h
// For very large terrains (e.g., entire planets)
#pragma once
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

class ClipmapLevel
{
public:
    ClipmapLevel(int size, float scale);

    void Update(const XMFLOAT2& center);
    void Render(ID3D11DeviceContext* context);

private:
    int m_size;
    float m_scale;
    XMFLOAT2 m_center;

    // Ring geometry for clipmap
    ComPtr<ID3D11Buffer> m_ringVertexBuffer;
    ComPtr<ID3D11Buffer> m_ringIndexBuffer;
};

class ClipmapTerrain
{
public:
    ClipmapTerrain();

    bool Initialize(ID3D11Device* device, int levelCount);
    void Update(const XMFLOAT3& cameraPos);
    void Render(ID3D11DeviceContext* context);

private:
    std::vector<std::unique_ptr<ClipmapLevel>> m_levels;
};
```

---

## 9. Terrain Physics

```cpp
// TerrainPhysics.h
#pragma once
#include "Heightmap.h"

class TerrainPhysics
{
public:
    TerrainPhysics(const Heightmap& heightmap);

    // Ray-terrain intersection
    bool IntersectRay(const XMFLOAT3& rayOrigin, const XMFLOAT3& rayDir,
                     XMFLOAT3& hitPoint, float maxDistance = 1000.0f) const;

    // Get height at world position
    float GetHeightAtPosition(float worldX, float worldZ) const;

    // Get normal at world position
    XMFLOAT3 GetNormalAtPosition(float worldX, float worldZ) const;

private:
    const Heightmap& m_heightmap;
    float m_gridSpacing;
};
```

---

## 10. Exercises

### Exercise 1: Dynamic Terrain Editing
Implement real-time terrain editing (raise/lower, smooth, flatten).

### Exercise 2: Vegetation Placement
Use terrain height and slope to procedurally place vegetation.

### Exercise 3: Terrain Streaming
Implement chunk-based streaming for very large terrains.

### Exercise 4: Advanced LOD
Add geomorphing to smooth LOD transitions.

### Exercise 5: Terrain Shadows
Implement cascaded shadow maps for terrain.

---

## Key Takeaways

1. ✅ Heightmaps efficiently represent terrain
2. ✅ LOD systems enable large-scale terrains
3. ✅ Texture splatting creates varied surfaces
4. ✅ Triplanar mapping eliminates stretching
5. ✅ Procedural generation creates infinite terrains
6. ✅ Clipmaps handle planetary-scale terrains
7. ✅ Physics integration enables gameplay
8. ✅ Optimization is critical for performance

---

## Summary Checklist

Before moving to Lesson 85, ensure you can:
- [ ] Load and generate heightmaps
- [ ] Create terrain meshes with LOD
- [ ] Implement texture splatting
- [ ] Use triplanar mapping
- [ ] Calculate terrain normals correctly
- [ ] Optimize terrain rendering
- [ ] Integrate terrain physics
- [ ] Handle large-scale terrains efficiently

**Congratulations on completing Lesson 84!**

Next lesson: Grass and Vegetation Rendering
