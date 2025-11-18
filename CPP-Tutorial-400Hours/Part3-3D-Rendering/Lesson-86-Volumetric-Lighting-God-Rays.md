# Lesson 86: Volumetric Lighting (God Rays)

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Volumetric Lighting
2. Light Shaft Theory
3. Ray Marching Fundamentals
4. Screen-Space God Rays
5. Volumetric Light Scattering
6. Volumetric Shadows
7. Performance Optimization
8. Atmospheric Integration
9. Complete Implementation
10. Advanced Techniques
11. Practical Applications
12. Exercises

---

## 1. Introduction to Volumetric Lighting

Volumetric lighting simulates light scattering through participating media (air, fog, dust), creating visible light shafts known as "god rays" or "crepuscular rays."

### Physical Basis

Light traveling through air interacts with particles:
- **Scattering**: Light redirected by particles
- **Absorption**: Energy absorbed by medium
- **Emission**: Light emitted from medium (rare in games)

### Rendering Approaches

1. **Screen-Space**: Post-process effect
2. **Ray Marching**: Volume sampling
3. **Light Shafts**: Radial blur technique
4. **Voxel-Based**: 3D grid traversal

---

## 2. Screen-Space God Rays Implementation

```cpp
// GodRaysRenderer.h
#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

struct GodRaysParams
{
    XMFLOAT4 LightScreenPos;   // Light position in screen space
    XMFLOAT4 LightColor;
    float Density;
    float Weight;
    float Decay;
    float Exposure;
    int NumSamples;
    XMFLOAT3 Padding;
};

class GodRaysRenderer
{
public:
    GodRaysRenderer();
    ~GodRaysRenderer();

    bool Initialize(ID3D11Device* device, int width, int height);
    void Render(ID3D11DeviceContext* context,
                ID3D11ShaderResourceView* sceneTexture,
                ID3D11ShaderResourceView* depthTexture,
                const XMFLOAT3& lightPos,
                const XMMATRIX& viewProj);

    ID3D11ShaderResourceView* GetGodRaysTexture() const { return m_godRaysSRV.Get(); }

private:
    bool CreateShaders(ID3D11Device* device);
    bool CreateRenderTargets(ID3D11Device* device, int width, int height);

    ComPtr<ID3D11PixelShader> m_occlusionPS;
    ComPtr<ID3D11PixelShader> m_godRaysPS;
    ComPtr<ID3D11PixelShader> m_combinePS;

    ComPtr<ID3D11Texture2D> m_occlusionTexture;
    ComPtr<ID3D11Texture2D> m_godRaysTexture;
    ComPtr<ID3D11RenderTargetView> m_occlusionRTV;
    ComPtr<ID3D11RenderTargetView> m_godRaysRTV;
    ComPtr<ID3D11ShaderResourceView> m_occlusionSRV;
    ComPtr<ID3D11ShaderResourceView> m_godRaysSRV;

    ComPtr<ID3D11Buffer> m_constantBuffer;
    ComPtr<ID3D11SamplerState> m_samplerState;

    int m_width;
    int m_height;
    GodRaysParams m_params;
};

// GodRaysRenderer.cpp
#include "GodRaysRenderer.h"

GodRaysRenderer::GodRaysRenderer()
    : m_width(0), m_height(0)
{
    // Initialize default parameters
    m_params.Density = 0.5f;
    m_params.Weight = 0.01f;
    m_params.Decay = 0.95f;
    m_params.Exposure = 0.2f;
    m_params.NumSamples = 64;
    m_params.LightColor = XMFLOAT4(1.0f, 0.95f, 0.8f, 1.0f);
}

GodRaysRenderer::~GodRaysRenderer()
{
}

bool GodRaysRenderer::Initialize(ID3D11Device* device, int width, int height)
{
    m_width = width;
    m_height = height;

    if (!CreateShaders(device))
        return false;

    if (!CreateRenderTargets(device, width, height))
        return false;

    // Create constant buffer
    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.ByteWidth = sizeof(GodRaysParams);
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT hr = device->CreateBuffer(&cbDesc, nullptr, &m_constantBuffer);
    if (FAILED(hr))
        return false;

    // Create sampler state
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    hr = device->CreateSamplerState(&samplerDesc, &m_samplerState);
    return SUCCEEDED(hr);
}

bool GodRaysRenderer::CreateRenderTargets(ID3D11Device* device, int width, int height)
{
    // Create lower resolution textures for better performance
    int godRaysWidth = width / 2;
    int godRaysHeight = height / 2;

    // Occlusion texture (full res)
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

    HRESULT hr = device->CreateTexture2D(&texDesc, nullptr, &m_occlusionTexture);
    if (FAILED(hr)) return false;

    hr = device->CreateRenderTargetView(m_occlusionTexture.Get(), nullptr, &m_occlusionRTV);
    if (FAILED(hr)) return false;

    hr = device->CreateShaderResourceView(m_occlusionTexture.Get(), nullptr, &m_occlusionSRV);
    if (FAILED(hr)) return false;

    // God rays texture (half res)
    texDesc.Width = godRaysWidth;
    texDesc.Height = godRaysHeight;

    hr = device->CreateTexture2D(&texDesc, nullptr, &m_godRaysTexture);
    if (FAILED(hr)) return false;

    hr = device->CreateRenderTargetView(m_godRaysTexture.Get(), nullptr, &m_godRaysRTV);
    if (FAILED(hr)) return false;

    hr = device->CreateShaderResourceView(m_godRaysTexture.Get(), nullptr, &m_godRaysSRV);
    return SUCCEEDED(hr);
}

void GodRaysRenderer::Render(ID3D11DeviceContext* context,
                              ID3D11ShaderResourceView* sceneTexture,
                              ID3D11ShaderResourceView* depthTexture,
                              const XMFLOAT3& lightPos,
                              const XMMATRIX& viewProj)
{
    // Transform light position to screen space
    XMVECTOR lightPosVec = XMLoadFloat3(&lightPos);
    XMVECTOR screenPos = XMVector3TransformCoord(lightPosVec, viewProj);

    // Convert to [0,1] range
    m_params.LightScreenPos.x = XMVectorGetX(screenPos) * 0.5f + 0.5f;
    m_params.LightScreenPos.y = -XMVectorGetY(screenPos) * 0.5f + 0.5f;
    m_params.LightScreenPos.z = XMVectorGetZ(screenPos);
    m_params.LightScreenPos.w = 1.0f;

    // Update constant buffer
    D3D11_MAPPED_SUBRESOURCE mapped;
    context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    memcpy(mapped.pData, &m_params, sizeof(GodRaysParams));
    context->Unmap(m_constantBuffer.Get(), 0);

    // Step 1: Generate occlusion mask
    context->OMSetRenderTargets(1, m_occlusionRTV.GetAddressOf(), nullptr);
    float clearColor[4] = { 0, 0, 0, 0 };
    context->ClearRenderTargetView(m_occlusionRTV.Get(), clearColor);

    context->PSSetShader(m_occlusionPS.Get(), nullptr, 0);
    context->PSSetShaderResources(0, 1, &sceneTexture);
    context->PSSetShaderResources(1, 1, &depthTexture);
    context->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
    context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

    // Draw fullscreen quad (implementation depends on your framework)
    // DrawFullscreenQuad(context);

    // Step 2: Generate god rays using radial blur
    context->OMSetRenderTargets(1, m_godRaysRTV.GetAddressOf(), nullptr);
    context->ClearRenderTargetView(m_godRaysRTV.Get(), clearColor);

    context->PSSetShader(m_godRaysPS.Get(), nullptr, 0);
    context->PSSetShaderResources(0, 1, m_occlusionSRV.GetAddressOf());
    context->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

    // Draw fullscreen quad
    // DrawFullscreenQuad(context);

    // Unbind resources
    ID3D11ShaderResourceView* nullSRV = nullptr;
    context->PSSetShaderResources(0, 1, &nullSRV);
    context->PSSetShaderResources(1, 1, &nullSRV);
}
```

---

## 3. God Rays Shaders

### Occlusion Pass Shader

```hlsl
// GodRaysOcclusion_PS.hlsl
cbuffer GodRaysParams : register(b0)
{
    float4 LightScreenPos;
    float4 LightColor;
    float Density;
    float Weight;
    float Decay;
    float Exposure;
    int NumSamples;
    float3 Padding;
};

Texture2D SceneTexture : register(t0);
Texture2D DepthTexture : register(t1);
SamplerState SamplerLinear : register(s0);

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    // Sample depth
    float depth = DepthTexture.Sample(SamplerLinear, input.TexCoord).r;

    // Check if this pixel can see the light source
    float4 sceneColor = SceneTexture.Sample(SamplerLinear, input.TexCoord);

    // Create occlusion mask
    // If pixel is in front of light or very bright, consider it occluded
    float occlusion = 0.0;

    if(depth >= 0.999) // Sky/background
    {
        // Check if we're near the light position
        float2 delta = input.TexCoord - LightScreenPos.xy;
        float dist = length(delta);

        if(dist < 0.1) // Within light radius
        {
            occlusion = 1.0 - (dist / 0.1);
        }
    }

    return float4(occlusion, occlusion, occlusion, 1.0) * LightColor;
}
```

### God Rays Generation Shader

```hlsl
// GodRaysGeneration_PS.hlsl
cbuffer GodRaysParams : register(b0)
{
    float4 LightScreenPos;
    float4 LightColor;
    float Density;
    float Weight;
    float Decay;
    float Exposure;
    int NumSamples;
    float3 Padding;
};

Texture2D OcclusionTexture : register(t0);
SamplerState SamplerLinear : register(s0);

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    // Calculate vector from pixel to light source in screen space
    float2 deltaTexCoord = input.TexCoord - LightScreenPos.xy;

    // Divide by number of samples and scale by density
    deltaTexCoord *= (1.0 / float(NumSamples)) * Density;

    // Set up illumination decay factor
    float illuminationDecay = 1.0;

    // Initialize color
    float4 color = float4(0, 0, 0, 0);

    // Evaluate summation
    float2 texCoord = input.TexCoord;

    [loop]
    for(int i = 0; i < NumSamples; i++)
    {
        // Step sample location along ray
        texCoord -= deltaTexCoord;

        // Sample occlusion texture
        float4 sample = OcclusionTexture.Sample(SamplerLinear, texCoord);

        // Apply weight and decay
        sample *= illuminationDecay * Weight;

        // Accumulate
        color += sample;

        // Update decay factor
        illuminationDecay *= Decay;
    }

    // Apply exposure
    color *= Exposure;

    return color;
}
```

### Combine Pass Shader

```hlsl
// GodRaysCombine_PS.hlsl
Texture2D SceneTexture : register(t0);
Texture2D GodRaysTexture : register(t1);
SamplerState SamplerLinear : register(s0);

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 sceneColor = SceneTexture.Sample(SamplerLinear, input.TexCoord);
    float4 godRaysColor = GodRaysTexture.Sample(SamplerLinear, input.TexCoord);

    // Additive blend
    return sceneColor + godRaysColor;
}
```

---

## 4. Volumetric Ray Marching

```hlsl
// VolumetricLighting_PS.hlsl
cbuffer VolumetricParams : register(b0)
{
    matrix InverseViewProj;
    float4 CameraPosition;
    float4 LightPosition;
    float4 LightColor;
    float ScatteringCoeff;
    float ExtinctionCoeff;
    int NumSamples;
    float MaxDistance;
};

Texture2D DepthTexture : register(t0);
Texture2D ShadowMap : register(t1);
SamplerState SamplerLinear : register(s0);

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

// Reconstruct world position from depth
float3 GetWorldPosition(float2 texCoord, float depth)
{
    float4 clipPos = float4(texCoord * 2.0 - 1.0, depth, 1.0);
    clipPos.y = -clipPos.y;

    float4 worldPos = mul(clipPos, InverseViewProj);
    return worldPos.xyz / worldPos.w;
}

// Sample volumetric lighting along ray
float4 RayMarch(float3 rayStart, float3 rayEnd, int steps)
{
    float3 rayDir = rayEnd - rayStart;
    float rayLength = length(rayDir);
    rayDir /= rayLength;

    float stepSize = rayLength / float(steps);
    float3 stepVec = rayDir * stepSize;

    float3 currentPos = rayStart;
    float4 accumulatedLight = float4(0, 0, 0, 0);
    float transmittance = 1.0;

    [loop]
    for(int i = 0; i < steps; i++)
    {
        // Calculate distance to light
        float3 toLightVec = LightPosition.xyz - currentPos;
        float distToLight = length(toLightVec);
        float3 lightDir = toLightVec / distToLight;

        // Attenuation
        float attenuation = 1.0 / (1.0 + distToLight * distToLight * 0.01);

        // Shadow test (simplified - should use shadow map)
        float shadow = 1.0;

        // Calculate in-scattering
        float scattering = ScatteringCoeff * stepSize;

        // Phase function (isotropic)
        float phase = 0.0795774715; // 1 / (4 * PI)

        // Accumulate light
        float3 inscatter = LightColor.rgb * scattering * phase * attenuation * shadow;
        accumulatedLight.rgb += inscatter * transmittance;

        // Update transmittance
        float extinction = ExtinctionCoeff * stepSize;
        transmittance *= exp(-extinction);

        // Early exit if transmittance is very low
        if(transmittance < 0.01)
            break;

        // Step along ray
        currentPos += stepVec;
    }

    accumulatedLight.a = 1.0 - transmittance;
    return accumulatedLight;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float depth = DepthTexture.Sample(SamplerLinear, input.TexCoord).r;

    // Reconstruct world positions
    float3 worldPos = GetWorldPosition(input.TexCoord, depth);
    float3 rayStart = CameraPosition.xyz;
    float3 rayEnd = worldPos;

    // Limit ray length
    float3 rayDir = rayEnd - rayStart;
    float rayLength = length(rayDir);

    if(rayLength > MaxDistance)
    {
        rayEnd = rayStart + normalize(rayDir) * MaxDistance;
    }

    // Perform ray marching
    float4 volumetricLight = RayMarch(rayStart, rayEnd, NumSamples);

    return volumetricLight;
}
```

---

## 5. Volumetric Shadows

```hlsl
// VolumetricShadows_PS.hlsl

Texture3D VolumeTexture : register(t0);
Texture2D ShadowMap : register(t1);
SamplerState SamplerLinear : register(s0);

cbuffer ShadowParams : register(b0)
{
    matrix LightViewProj;
    float4 LightDirection;
    int NumSamples;
    float ShadowDensity;
    float2 Padding;
};

float SampleShadowMap(float3 worldPos)
{
    // Transform to light space
    float4 lightSpacePos = mul(float4(worldPos, 1.0), LightViewProj);
    lightSpacePos.xyz /= lightSpacePos.w;

    // Convert to texture coordinates
    float2 shadowUV = lightSpacePos.xy * 0.5 + 0.5;
    shadowUV.y = 1.0 - shadowUV.y;

    // Sample shadow map
    float shadowDepth = ShadowMap.Sample(SamplerLinear, shadowUV).r;

    // Compare depths
    return (lightSpacePos.z - 0.001 < shadowDepth) ? 1.0 : 0.0;
}

float CalculateVolumetricShadow(float3 worldPos, float3 lightDir, float maxDist)
{
    float shadow = 0.0;
    float stepSize = maxDist / float(NumSamples);

    for(int i = 0; i < NumSamples; i++)
    {
        float t = float(i) / float(NumSamples);
        float3 samplePos = worldPos + lightDir * t * maxDist;

        // Sample volume density
        float density = VolumeTexture.Sample(SamplerLinear, samplePos * 0.1).r;

        // Accumulate shadow
        shadow += density * stepSize;

        // Early exit
        if(shadow > 0.99)
            break;
    }

    return saturate(1.0 - shadow * ShadowDensity);
}
```

---

## 6. Performance Optimization Techniques

### Temporal Reprojection

```hlsl
// TemporalReprojection.hlsl
cbuffer TemporalParams : register(b0)
{
    matrix CurrentViewProj;
    matrix PreviousViewProj;
    float BlendFactor;
    float3 Padding;
};

Texture2D CurrentFrame : register(t0);
Texture2D PreviousFrame : register(t1);
Texture2D VelocityBuffer : register(t2);
SamplerState SamplerLinear : register(s0);

float4 TemporalReprojection(float2 texCoord)
{
    // Sample current frame
    float4 currentColor = CurrentFrame.Sample(SamplerLinear, texCoord);

    // Get velocity
    float2 velocity = VelocityBuffer.Sample(SamplerLinear, texCoord).xy;

    // Reproject to previous frame
    float2 prevTexCoord = texCoord - velocity;

    // Check if previous coordinate is valid
    if(prevTexCoord.x < 0.0 || prevTexCoord.x > 1.0 ||
       prevTexCoord.y < 0.0 || prevTexCoord.y > 1.0)
    {
        return currentColor;
    }

    // Sample previous frame
    float4 previousColor = PreviousFrame.Sample(SamplerLinear, prevTexCoord);

    // Temporal blend
    return lerp(previousColor, currentColor, BlendFactor);
}
```

### Bilateral Upsampling

```hlsl
// BilateralUpsample.hlsl
Texture2D LowResTexture : register(t0);
Texture2D HighResDepth : register(t1);
Texture2D LowResDepth : register(t2);
SamplerState SamplerLinear : register(s0);

float4 BilateralUpsample(float2 highResTexCoord, float2 lowResTexCoord)
{
    float highResDepth = HighResDepth.Sample(SamplerLinear, highResTexCoord).r;

    float2 lowResTexelSize = 1.0 / float2(512, 512); // Low res size

    float4 result = float4(0, 0, 0, 0);
    float totalWeight = 0.0;

    // Sample 2x2 neighborhood
    for(int y = -1; y <= 1; y++)
    {
        for(int x = -1; x <= 1; x++)
        {
            float2 offset = float2(x, y) * lowResTexelSize;
            float2 sampleUV = lowResTexCoord + offset;

            float4 sampleColor = LowResTexture.Sample(SamplerLinear, sampleUV);
            float sampleDepth = LowResDepth.Sample(SamplerLinear, sampleUV).r;

            // Bilateral weight based on depth difference
            float depthDiff = abs(highResDepth - sampleDepth);
            float weight = exp(-depthDiff * 10.0);

            result += sampleColor * weight;
            totalWeight += weight;
        }
    }

    return result / max(totalWeight, 0.0001);
}
```

---

## 7. Practical Implementation

```cpp
// VolumetricLightingSystem.h
#pragma once
#include "GodRaysRenderer.h"

class VolumetricLightingSystem
{
public:
    VolumetricLightingSystem();
    ~VolumetricLightingSystem();

    bool Initialize(ID3D11Device* device, int width, int height);
    void Update(float deltaTime);
    void Render(ID3D11DeviceContext* context,
                const SceneData& scene,
                const CameraData& camera);

private:
    std::unique_ptr<GodRaysRenderer> m_godRays;

    bool m_useTemporalReprojection;
    bool m_useBilateralUpsampling;
    float m_quality; // 0.5 = half res, 1.0 = full res
};
```

---

## 8. Exercises

### Exercise 1: Colored Light Shafts
Implement colored volumetric lighting with RGB scattering.

### Exercise 2: Multiple Light Sources
Extend system to support multiple volumetric lights.

### Exercise 3: Temporal Filtering
Add temporal anti-aliasing for smoother results.

### Exercise 4: Volumetric Fog
Combine god rays with distance fog.

### Exercise 5: Performance Analysis
Profile and optimize using different resolutions and sample counts.

---

## Key Takeaways

1. ✅ God rays create atmospheric lighting effects
2. ✅ Screen-space methods are fast but approximate
3. ✅ Ray marching provides accurate volumetric lighting
4. ✅ Temporal reprojection reduces noise
5. ✅ Bilateral upsampling maintains quality at lower cost
6. ✅ Volumetric shadows add realism
7. ✅ Multiple optimization techniques are essential
8. ✅ Balance quality vs performance based on platform

---

## Summary Checklist

Before moving to Lesson 87, ensure you can:
- [ ] Implement screen-space god rays
- [ ] Create volumetric ray marching
- [ ] Add volumetric shadows
- [ ] Use temporal reprojection
- [ ] Implement bilateral upsampling
- [ ] Optimize for real-time performance
- [ ] Combine with other atmospheric effects
- [ ] Debug and profile volumetric lighting

**Congratulations on completing Lesson 86!**

Next lesson: Fog and Atmospheric Scattering
