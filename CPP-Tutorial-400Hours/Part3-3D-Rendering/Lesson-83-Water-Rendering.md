# Lesson 83: Water Rendering

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Water Rendering
2. Water Physics Simulation
3. Wave Generation and Animation
4. Water Surface Normals
5. Reflection and Refraction
6. Fresnel Effect
7. Caustics Rendering
8. Underwater Effects
9. Foam and Spray
10. Performance Optimization
11. Complete Water System
12. Exercises

---

## 1. Introduction to Water Rendering

Realistic water rendering is one of the most challenging aspects of real-time graphics, requiring simulation of multiple physical phenomena.

### Key Components of Realistic Water

- **Surface Geometry**: Waves and ripples
- **Reflections**: Mirror-like surface reflections
- **Refractions**: Light bending through water
- **Fresnel Effect**: Viewing angle-dependent reflection/refraction
- **Subsurface Scattering**: Light transmission through water
- **Caustics**: Focused light patterns on underwater surfaces
- **Foam**: White water on wave crests and shores
- **Underwater Effects**: Fog, god rays, particles

---

## 2. Water Physics Simulation

### Gerstner Waves Implementation

```cpp
// WaterSimulation.h
#pragma once
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

struct WaveParameters
{
    XMFLOAT2 Direction;    // Wave direction (normalized)
    float Wavelength;       // Distance between wave crests
    float Amplitude;        // Wave height
    float Steepness;        // How sharp/peaked the waves are
    float Speed;            // Wave propagation speed
};

class WaterPhysics
{
public:
    WaterPhysics();

    void AddWave(const WaveParameters& wave);
    void ClearWaves();

    // Calculate water position and normal at given point
    XMFLOAT3 GetWaterPosition(const XMFLOAT2& xzPos, float time) const;
    XMFLOAT3 GetWaterNormal(const XMFLOAT2& xzPos, float time) const;
    XMFLOAT3 GetWaterTangent(const XMFLOAT2& xzPos, float time) const;
    XMFLOAT3 GetWaterBitangent(const XMFLOAT2& xzPos, float time) const;

    const std::vector<WaveParameters>& GetWaves() const { return m_waves; }

private:
    std::vector<WaveParameters> m_waves;

    // Helper functions for Gerstner waves
    void EvaluateWave(const WaveParameters& wave, const XMFLOAT2& xzPos,
        float time, XMFLOAT3& position, XMFLOAT3& tangent, XMFLOAT3& binormal) const;
};

// WaterSimulation.cpp
#include "WaterSimulation.h"
#include <cmath>

WaterPhysics::WaterPhysics()
{
    // Add default ocean waves
    WaveParameters wave1;
    wave1.Direction = XMFLOAT2(1.0f, 0.0f);
    wave1.Wavelength = 60.0f;
    wave1.Amplitude = 1.0f;
    wave1.Steepness = 0.5f;
    wave1.Speed = 2.0f;
    m_waves.push_back(wave1);

    WaveParameters wave2;
    wave2.Direction = XMFLOAT2(0.7f, 0.7f);
    wave2.Wavelength = 30.0f;
    wave2.Amplitude = 0.5f;
    wave2.Steepness = 0.4f;
    wave2.Speed = 1.5f;
    m_waves.push_back(wave2);

    WaveParameters wave3;
    wave3.Direction = XMFLOAT2(0.0f, 1.0f);
    wave3.Wavelength = 15.0f;
    wave3.Amplitude = 0.25f;
    wave3.Steepness = 0.3f;
    wave3.Speed = 1.0f;
    m_waves.push_back(wave3);
}

void WaterPhysics::AddWave(const WaveParameters& wave)
{
    m_waves.push_back(wave);
}

void WaterPhysics::ClearWaves()
{
    m_waves.clear();
}

void WaterPhysics::EvaluateWave(const WaveParameters& wave, const XMFLOAT2& xzPos,
    float time, XMFLOAT3& position, XMFLOAT3& tangent, XMFLOAT3& binormal) const
{
    const float PI = 3.14159265359f;

    float k = 2.0f * PI / wave.Wavelength;  // Wave number
    float c = wave.Speed;                    // Phase speed
    float a = wave.Amplitude;                // Amplitude
    float Q = wave.Steepness / (k * a * static_cast<float>(m_waves.size()));

    float d = k * (wave.Direction.x * xzPos.x + wave.Direction.y * xzPos.y - c * time);
    float cosD = std::cos(d);
    float sinD = std::sin(d);

    // Position offset
    position.x += Q * a * wave.Direction.x * cosD;
    position.y += a * sinD;
    position.z += Q * a * wave.Direction.y * cosD;

    // Tangent
    float WA = k * a;
    tangent.x += -Q * wave.Direction.x * wave.Direction.x * WA * sinD;
    tangent.y += wave.Direction.x * WA * cosD;
    tangent.z += -Q * wave.Direction.x * wave.Direction.y * WA * sinD;

    // Binormal
    binormal.x += -Q * wave.Direction.x * wave.Direction.y * WA * sinD;
    binormal.y += wave.Direction.y * WA * cosD;
    binormal.z += -Q * wave.Direction.y * wave.Direction.y * WA * sinD;
}

XMFLOAT3 WaterPhysics::GetWaterPosition(const XMFLOAT2& xzPos, float time) const
{
    XMFLOAT3 position(xzPos.x, 0.0f, xzPos.y);
    XMFLOAT3 tangent(1.0f, 0.0f, 0.0f);
    XMFLOAT3 binormal(0.0f, 0.0f, 1.0f);

    for (const auto& wave : m_waves)
    {
        EvaluateWave(wave, xzPos, time, position, tangent, binormal);
    }

    return position;
}

XMFLOAT3 WaterPhysics::GetWaterNormal(const XMFLOAT2& xzPos, float time) const
{
    XMFLOAT3 position(xzPos.x, 0.0f, xzPos.y);
    XMFLOAT3 tangent(1.0f, 0.0f, 0.0f);
    XMFLOAT3 binormal(0.0f, 0.0f, 1.0f);

    for (const auto& wave : m_waves)
    {
        EvaluateWave(wave, xzPos, time, position, tangent, binormal);
    }

    // Calculate normal from tangent and binormal
    XMVECTOR t = XMLoadFloat3(&tangent);
    XMVECTOR b = XMLoadFloat3(&binormal);
    XMVECTOR n = XMVector3Normalize(XMVector3Cross(b, t));

    XMFLOAT3 normal;
    XMStoreFloat3(&normal, n);
    return normal;
}
```

---

## 3. Water Shader System

### Water Vertex Shader

```hlsl
// Water_VS.hlsl
cbuffer PerFrameBuffer : register(b0)
{
    matrix ViewProjection;
    matrix View;
    float4 CameraPosition;
    float Time;
    float3 Padding;
};

#define MAX_WAVES 8

struct Wave
{
    float2 Direction;
    float Wavelength;
    float Amplitude;
    float Steepness;
    float Speed;
    float2 Padding;
};

cbuffer WaveBuffer : register(b1)
{
    Wave Waves[MAX_WAVES];
    int WaveCount;
    float3 WavePadding;
};

struct VS_INPUT
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float3 WorldPos : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
    float2 TexCoord : TEXCOORD0;
    float4 ReflectionPos : TEXCOORD1;
    float4 RefractionPos : TEXCOORD2;
};

void GerstnerWave(Wave w, float2 xz, float time, inout float3 position,
                  inout float3 tangent, inout float3 binormal)
{
    float PI = 3.14159265359;
    float k = 2.0 * PI / w.Wavelength;
    float c = w.Speed;
    float a = w.Amplitude;
    float Q = w.Steepness / (k * a * float(WaveCount));

    float d = k * (dot(w.Direction, xz) - c * time);
    float cosD = cos(d);
    float sinD = sin(d);

    // Position
    position.x += Q * a * w.Direction.x * cosD;
    position.y += a * sinD;
    position.z += Q * a * w.Direction.y * cosD;

    // Tangent
    float WA = k * a;
    tangent.x += -Q * w.Direction.x * w.Direction.x * WA * sinD;
    tangent.y += w.Direction.x * WA * cosD;
    tangent.z += -Q * w.Direction.x * w.Direction.y * WA * sinD;

    // Binormal
    binormal.x += -Q * w.Direction.x * w.Direction.y * WA * sinD;
    binormal.y += w.Direction.y * WA * cosD;
    binormal.z += -Q * w.Direction.y * w.Direction.y * WA * sinD;
}

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // Initialize wave calculation
    float3 position = float3(input.Position.x, 0.0, input.Position.z);
    float3 tangent = float3(1, 0, 0);
    float3 binormal = float3(0, 0, 1);

    // Sum all waves
    [unroll]
    for(int i = 0; i < WaveCount; i++)
    {
        GerstnerWave(Waves[i], input.Position.xz, Time, position, tangent, binormal);
    }

    // Calculate normal
    float3 normal = normalize(cross(binormal, tangent));

    output.WorldPos = position;
    output.Normal = normal;
    output.Tangent = normalize(tangent);
    output.Binormal = normalize(binormal);
    output.TexCoord = input.TexCoord;

    // Transform to clip space
    output.Position = mul(float4(position, 1.0), ViewProjection);

    // Calculate reflection/refraction positions
    matrix reflectionView = View;
    reflectionView[3][1] = -reflectionView[3][1]; // Flip Y for reflection

    float4 reflectionPos = mul(float4(position, 1.0), reflectionView);
    output.ReflectionPos = mul(reflectionPos, ViewProjection);

    output.RefractionPos = output.Position;

    return output;
}
```

### Water Pixel Shader

```hlsl
// Water_PS.hlsl
cbuffer PerFrameBuffer : register(b0)
{
    matrix ViewProjection;
    matrix View;
    float4 CameraPosition;
    float Time;
    float3 Padding;
};

cbuffer WaterMaterialBuffer : register(b2)
{
    float4 WaterColor;
    float4 DeepWaterColor;
    float Roughness;
    float Metallic;
    float RefractionStrength;
    float FresnelPower;
    float WaveNormalStrength;
    float FoamAmount;
    float FoamCutoff;
    float CausticStrength;
};

Texture2D ReflectionTexture : register(t0);
Texture2D RefractionTexture : register(t1);
Texture2D NormalMap : register(t2);
Texture2D FoamTexture : register(t3);
Texture2D DepthTexture : register(t4);
Texture2D CausticsTexture : register(t5);
SamplerState SamplerLinear : register(s0);

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float3 WorldPos : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
    float2 TexCoord : TEXCOORD0;
    float4 ReflectionPos : TEXCOORD1;
    float4 RefractionPos : TEXCOORD2;
};

float3 GetWaterNormal(PS_INPUT input, float2 texCoord)
{
    // Construct TBN matrix
    float3x3 TBN = float3x3(
        normalize(input.Tangent),
        normalize(input.Binormal),
        normalize(input.Normal)
    );

    // Sample normal maps with different scales and speeds
    float2 normalUV1 = texCoord * 0.5 + float2(Time * 0.02, Time * 0.01);
    float2 normalUV2 = texCoord * 0.8 - float2(Time * 0.015, Time * 0.025);

    float3 normal1 = NormalMap.Sample(SamplerLinear, normalUV1).xyz * 2.0 - 1.0;
    float3 normal2 = NormalMap.Sample(SamplerLinear, normalUV2).xyz * 2.0 - 1.0;

    // Blend normals
    float3 normalMap = normalize(normal1 + normal2);
    normalMap.xy *= WaveNormalStrength;
    normalMap = normalize(normalMap);

    // Transform to world space
    return normalize(mul(normalMap, TBN));
}

float CalculateFresnel(float3 normal, float3 viewDir, float power)
{
    float fresnel = pow(1.0 - saturate(dot(normal, viewDir)), power);
    return fresnel;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    // Calculate final water normal
    float3 normal = GetWaterNormal(input, input.TexCoord);

    // View direction
    float3 viewDir = normalize(CameraPosition.xyz - input.WorldPos);

    // Calculate reflection coordinates
    float2 reflectionCoords = input.ReflectionPos.xy / input.ReflectionPos.w;
    reflectionCoords = reflectionCoords * 0.5 + 0.5;
    reflectionCoords.y = 1.0 - reflectionCoords.y;

    // Add distortion to reflection based on normal
    reflectionCoords += normal.xz * 0.05;

    // Sample reflection
    float3 reflectionColor = ReflectionTexture.Sample(SamplerLinear, reflectionCoords).rgb;

    // Calculate refraction coordinates
    float2 refractionCoords = input.RefractionPos.xy / input.RefractionPos.w;
    refractionCoords = refractionCoords * 0.5 + 0.5;
    refractionCoords.y = 1.0 - refractionCoords.y;

    // Add distortion to refraction
    refractionCoords += normal.xz * RefractionStrength;

    // Sample refraction
    float3 refractionColor = RefractionTexture.Sample(SamplerLinear, refractionCoords).rgb;

    // Calculate depth
    float depth = DepthTexture.Sample(SamplerLinear, refractionCoords).r;
    float waterDepth = saturate((depth - input.Position.z) * 10.0);

    // Blend deep and shallow water colors based on depth
    float3 waterColorBlend = lerp(WaterColor.rgb, DeepWaterColor.rgb, waterDepth);

    // Apply water color to refraction
    refractionColor *= waterColorBlend;

    // Add caustics to refraction
    float2 causticsUV = input.TexCoord * 2.0 + float2(Time * 0.03, Time * 0.02);
    float caustics = CausticsTexture.Sample(SamplerLinear, causticsUV).r;
    refractionColor += caustics * CausticStrength * waterDepth;

    // Calculate Fresnel
    float fresnel = CalculateFresnel(normal, viewDir, FresnelPower);

    // Blend reflection and refraction
    float3 finalColor = lerp(refractionColor, reflectionColor, fresnel);

    // Add foam
    float foamFactor = 1.0 - waterDepth;
    if(foamFactor > FoamCutoff)
    {
        float2 foamUV = input.TexCoord * 3.0 + float2(Time * 0.01, 0);
        float foam = FoamTexture.Sample(SamplerLinear, foamUV).r;
        foam *= smoothstep(FoamCutoff, 1.0, foamFactor);
        finalColor = lerp(finalColor, float3(1, 1, 1), foam * FoamAmount);
    }

    // Add specular highlights
    float3 lightDir = normalize(float3(0.5, 1.0, 0.3));
    float3 halfVector = normalize(lightDir + viewDir);
    float specular = pow(max(dot(normal, halfVector), 0.0), 128.0);
    finalColor += specular * 0.5;

    return float4(finalColor, 1.0);
}
```

---

## 4. Water Rendering System

```cpp
// WaterRenderer.h
#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include "WaterSimulation.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

struct WaterMaterial
{
    XMFLOAT4 WaterColor;
    XMFLOAT4 DeepWaterColor;
    float Roughness;
    float Metallic;
    float RefractionStrength;
    float FresnelPower;
    float WaveNormalStrength;
    float FoamAmount;
    float FoamCutoff;
    float CausticStrength;
};

class WaterRenderer
{
public:
    WaterRenderer();
    ~WaterRenderer();

    bool Initialize(ID3D11Device* device, int gridSize, float gridSpacing);
    void Update(float deltaTime);
    void Render(ID3D11DeviceContext* context, const XMMATRIX& viewProj,
        const XMFLOAT3& cameraPos);

    void SetWaterMaterial(const WaterMaterial& material) { m_material = material; }
    WaterPhysics& GetPhysics() { return m_physics; }

private:
    bool CreateMesh(ID3D11Device* device, int gridSize, float gridSpacing);
    bool CreateShaders(ID3D11Device* device);
    bool CreateRenderTargets(ID3D11Device* device);

    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11Buffer> m_indexBuffer;
    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3D11PixelShader> m_pixelShader;
    ComPtr<ID3D11InputLayout> m_inputLayout;
    ComPtr<ID3D11Buffer> m_perFrameBuffer;
    ComPtr<ID3D11Buffer> m_waveBuffer;
    ComPtr<ID3D11Buffer> m_materialBuffer;

    ComPtr<ID3D11Texture2D> m_reflectionTexture;
    ComPtr<ID3D11Texture2D> m_refractionTexture;
    ComPtr<ID3D11RenderTargetView> m_reflectionRTV;
    ComPtr<ID3D11RenderTargetView> m_refractionRTV;
    ComPtr<ID3D11ShaderResourceView> m_reflectionSRV;
    ComPtr<ID3D11ShaderResourceView> m_refractionSRV;

    ComPtr<ID3D11ShaderResourceView> m_normalMapSRV;
    ComPtr<ID3D11ShaderResourceView> m_foamTextureSRV;
    ComPtr<ID3D11ShaderResourceView> m_causticsTextureSRV;

    WaterPhysics m_physics;
    WaterMaterial m_material;

    int m_indexCount;
    float m_time;
};
```

---

## 5. Caustics Generation

```hlsl
// Caustics_CS.hlsl
cbuffer CausticsParams : register(b0)
{
    float Time;
    float Scale;
    float Intensity;
    float Speed;
};

RWTexture2D<float4> CausticsOutput : register(u0);

// Voronoi-based caustics
float voronoi(float2 x)
{
    float2 n = floor(x);
    float2 f = frac(x);

    float minDist = 1.0;

    for(int j = -1; j <= 1; j++)
    {
        for(int i = -1; i <= 1; i++)
        {
            float2 g = float2(i, j);
            float2 o = frac(sin(float2(dot(n + g, float2(127.1, 311.7)),
                                       dot(n + g, float2(269.5, 183.3)))) * 43758.5453);

            float2 r = g + o - f;
            float d = length(r);
            minDist = min(minDist, d);
        }
    }

    return minDist;
}

[numthreads(16, 16, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint width, height;
    CausticsOutput.GetDimensions(width, height);

    if(DTid.x >= width || DTid.y >= height)
        return;

    float2 uv = float2(DTid.xy) / float2(width, height);

    // Animated caustics using multiple Voronoi layers
    float2 uv1 = (uv + float2(Time * Speed * 0.1, 0)) * Scale;
    float2 uv2 = (uv - float2(0, Time * Speed * 0.15)) * Scale * 0.7;

    float caustic1 = voronoi(uv1);
    float caustic2 = voronoi(uv2);

    // Combine and enhance
    float caustic = min(caustic1, caustic2);
    caustic = pow(caustic, 3.0) * Intensity;

    CausticsOutput[DTid.xy] = float4(caustic, caustic, caustic, 1.0);
}
```

---

## 6. Underwater Effects

```hlsl
// Underwater_PS.hlsl
cbuffer UnderwaterParams : register(b0)
{
    float4 WaterColor;
    float4 SunDirection;
    float4 SunColor;
    float WaterLevel;
    float FogDensity;
    float GodRayIntensity;
    float Time;
};

Texture2D SceneTexture : register(t0);
Texture2D DepthTexture : register(t1);
Texture2D CausticsTexture : register(t2);
SamplerState SamplerLinear : register(s0);

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 ViewRay : TEXCOORD1;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    // Sample scene
    float4 sceneColor = SceneTexture.Sample(SamplerLinear, input.TexCoord);
    float depth = DepthTexture.Sample(SamplerLinear, input.TexCoord).r;

    // Reconstruct world position
    float3 worldPos = input.ViewRay * depth;

    // Check if underwater
    if(worldPos.y > WaterLevel)
    {
        return sceneColor; // Above water
    }

    // Calculate distance underwater
    float waterDepth = WaterLevel - worldPos.y;
    float viewDistance = length(worldPos);

    // Apply underwater fog
    float fogFactor = exp(-waterDepth * FogDensity);
    sceneColor.rgb = lerp(WaterColor.rgb, sceneColor.rgb, fogFactor);

    // Add caustics
    float2 causticsUV = worldPos.xz * 0.1 + float2(Time * 0.02, Time * 0.015);
    float caustics = CausticsTexture.Sample(SamplerLinear, causticsUV).r;
    sceneColor.rgb += caustics * (1.0 - fogFactor) * 0.5;

    // Add god rays
    float2 screenPos = input.TexCoord * 2.0 - 1.0;
    float2 sunScreenPos = float2(0.5, 0.8); // Sun position in screen space

    float godRay = 0.0;
    int samples = 32;
    float2 delta = (sunScreenPos - input.TexCoord) / float(samples);

    for(int i = 0; i < samples; i++)
    {
        float2 sampleUV = input.TexCoord + delta * float(i);
        float sampleDepth = DepthTexture.Sample(SamplerLinear, sampleUV).r;
        float3 samplePos = input.ViewRay * sampleDepth;

        if(samplePos.y > WaterLevel)
        {
            godRay += 1.0 / float(samples);
        }
    }

    godRay *= GodRayIntensity * (1.0 - fogFactor);
    sceneColor.rgb += SunColor.rgb * godRay;

    // Add chromatic aberration
    float aberration = 0.002 * waterDepth;
    float r = SceneTexture.Sample(SamplerLinear, input.TexCoord + float2(aberration, 0)).r;
    float b = SceneTexture.Sample(SamplerLinear, input.TexCoord - float2(aberration, 0)).b;
    sceneColor.r = lerp(sceneColor.r, r, 0.3);
    sceneColor.b = lerp(sceneColor.b, b, 0.3);

    return sceneColor;
}
```

---

## 7. Complete Water System Integration

```cpp
// WaterSystem.cpp
#include "WaterRenderer.h"

WaterRenderer::WaterRenderer()
    : m_indexCount(0), m_time(0.0f)
{
    // Initialize default water material
    m_material.WaterColor = XMFLOAT4(0.1f, 0.3f, 0.4f, 1.0f);
    m_material.DeepWaterColor = XMFLOAT4(0.0f, 0.1f, 0.2f, 1.0f);
    m_material.Roughness = 0.1f;
    m_material.Metallic = 0.0f;
    m_material.RefractionStrength = 0.05f;
    m_material.FresnelPower = 5.0f;
    m_material.WaveNormalStrength = 1.0f;
    m_material.FoamAmount = 0.8f;
    m_material.FoamCutoff = 0.7f;
    m_material.CausticStrength = 0.5f;
}

bool WaterRenderer::Initialize(ID3D11Device* device, int gridSize, float gridSpacing)
{
    if (!CreateMesh(device, gridSize, gridSpacing))
        return false;

    if (!CreateShaders(device))
        return false;

    if (!CreateRenderTargets(device))
        return false;

    // Load textures (normal map, foam, caustics)
    // Implementation depends on your texture loading system

    return true;
}

void WaterRenderer::Update(float deltaTime)
{
    m_time += deltaTime;
}

void WaterRenderer::Render(ID3D11DeviceContext* context,
    const XMMATRIX& viewProj, const XMFLOAT3& cameraPos)
{
    // Set shaders and input layout
    context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
    context->IASetInputLayout(m_inputLayout.Get());

    // Update constant buffers
    // ... (implementation details)

    // Set vertex and index buffers
    UINT stride = sizeof(float) * 5; // Position(3) + TexCoord(2)
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Render
    context->DrawIndexed(m_indexCount, 0, 0);
}
```

---

## 8. Foam Simulation

```hlsl
// FoamSimulation_CS.hlsl
struct FoamParticle
{
    float3 Position;
    float Life;
    float3 Velocity;
    float Size;
};

RWStructuredBuffer<FoamParticle> FoamParticles : register(u0);

cbuffer FoamParams : register(b0)
{
    float DeltaTime;
    float WaterLevel;
    float2 Padding;
};

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    FoamParticle particle = FoamParticles[DTid.x];

    if(particle.Life > 0.0)
    {
        // Update particle
        particle.Life -= DeltaTime;
        particle.Position += particle.Velocity * DeltaTime;

        // Simulate floating on water surface
        particle.Position.y = lerp(particle.Position.y, WaterLevel, 0.1);

        // Fade size
        particle.Size *= 0.99;
    }

    FoamParticles[DTid.x] = particle;
}
```

---

## 9. Exercises

### Exercise 1: Ocean Foam
Implement dynamic foam generation based on wave height and velocity.

### Exercise 2: Interactive Water
Add interactive ripples when objects enter the water.

### Exercise 3: Shoreline Waves
Create realistic wave breaking near shores.

### Exercise 4: Underwater Particles
Add floating particles and debris underwater.

### Exercise 5: Performance Optimization
Implement LOD system for water mesh based on distance.

---

## Key Takeaways

1. ✅ Gerstner waves create realistic ocean surfaces
2. ✅ Fresnel effect controls reflection/refraction blend
3. ✅ Normal mapping adds surface detail
4. ✅ Caustics enhance underwater realism
5. ✅ Foam particles add visual richness
6. ✅ Underwater effects create immersion
7. ✅ Multiple render targets enable complex water rendering
8. ✅ Performance optimization is crucial for large water bodies

---

## Summary Checklist

Before moving to Lesson 84, ensure you can:
- [ ] Implement Gerstner wave simulation
- [ ] Create water shaders with reflection/refraction
- [ ] Apply Fresnel effect correctly
- [ ] Generate procedural caustics
- [ ] Add foam and spray effects
- [ ] Implement underwater post-processing
- [ ] Optimize water rendering for performance
- [ ] Combine all water effects into a complete system

**Congratulations on completing Lesson 83!**

Next lesson: Terrain Rendering Techniques
