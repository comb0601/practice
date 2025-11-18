# Lesson 81: Advanced Shader Techniques

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Advanced Shading
2. Normal Mapping and Tangent Space
3. Parallax Occlusion Mapping
4. Screen Space Reflections
5. Deferred Shading Architecture
6. Compute Shaders for Graphics
7. Tessellation Shaders
8. Geometry Shaders
9. Multi-Pass Rendering Techniques
10. Performance Optimization for Shaders
11. Complete Implementation Example
12. Exercises

---

## 1. Introduction to Advanced Shading

Modern real-time rendering relies heavily on sophisticated shader techniques to achieve photorealistic results while maintaining interactive frame rates. This lesson covers advanced shader programming techniques that form the foundation of modern game engines and visualization systems.

### What Makes a Shader "Advanced"?

Advanced shaders typically:
- Use multiple render passes
- Implement complex mathematical models
- Leverage modern GPU features (compute, tessellation)
- Optimize for parallel processing
- Handle edge cases gracefully
- Scale across different hardware

### Shader Pipeline Overview

```
Vertex Shader → Hull Shader → Tessellator → Domain Shader →
Geometry Shader → Rasterizer → Pixel Shader
```

Modern graphics APIs also support:
- **Compute Shaders**: General purpose GPU computation
- **Mesh Shaders**: New programmable geometry pipeline (DirectX 12)
- **Ray Tracing Shaders**: RT pipeline for realistic lighting

---

## 2. Normal Mapping and Tangent Space

Normal mapping adds surface detail without additional geometry by perturbing surface normals.

### Understanding Tangent Space

Tangent space is a local coordinate system where:
- **T (Tangent)**: Points along U texture coordinate
- **B (Bitangent)**: Points along V texture coordinate
- **N (Normal)**: Surface normal

### Complete Normal Mapping Implementation

```cpp
// AdvancedShaderSystem.h
#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <string>
#include <vector>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

struct Vertex_Normal
{
    XMFLOAT3 Position;
    XMFLOAT3 Normal;
    XMFLOAT2 TexCoord;
    XMFLOAT3 Tangent;
    XMFLOAT3 Bitangent;
};

struct CB_PerObject
{
    XMMATRIX World;
    XMMATRIX WorldViewProjection;
    XMMATRIX WorldInverseTranspose;
};

struct CB_PerFrame
{
    XMFLOAT4 CameraPosition;
    XMFLOAT4 LightDirection;
    XMFLOAT4 LightColor;
    float Time;
    float NormalMapStrength;
    float ParallaxScale;
    float ParallaxBias;
};

class NormalMappingShader
{
public:
    NormalMappingShader();
    ~NormalMappingShader();

    bool Initialize(ID3D11Device* device, const std::wstring& vsPath, const std::wstring& psPath);
    void Render(ID3D11DeviceContext* context, int indexCount);
    void SetShaderParameters(ID3D11DeviceContext* context,
        const CB_PerObject& objData,
        const CB_PerFrame& frameData,
        ID3D11ShaderResourceView* diffuseTexture,
        ID3D11ShaderResourceView* normalMap);

private:
    bool InitializeShader(ID3D11Device* device, const std::wstring& vsPath, const std::wstring& psPath);
    void ShutdownShader();

    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3D11PixelShader> m_pixelShader;
    ComPtr<ID3D11InputLayout> m_layout;
    ComPtr<ID3D11Buffer> m_constantBufferPerObject;
    ComPtr<ID3D11Buffer> m_constantBufferPerFrame;
    ComPtr<ID3D11SamplerState> m_samplerState;
};

// AdvancedShaderSystem.cpp
#include "AdvancedShaderSystem.h"
#include <d3dcompiler.h>
#include <fstream>

#pragma comment(lib, "d3dcompiler.lib")

NormalMappingShader::NormalMappingShader()
{
}

NormalMappingShader::~NormalMappingShader()
{
    ShutdownShader();
}

bool NormalMappingShader::Initialize(ID3D11Device* device,
    const std::wstring& vsPath, const std::wstring& psPath)
{
    return InitializeShader(device, vsPath, psPath);
}

bool NormalMappingShader::InitializeShader(ID3D11Device* device,
    const std::wstring& vsPath, const std::wstring& psPath)
{
    HRESULT hr;
    ComPtr<ID3DBlob> vertexShaderBuffer;
    ComPtr<ID3DBlob> pixelShaderBuffer;
    ComPtr<ID3DBlob> errorMessage;

    // Compile vertex shader
    hr = D3DCompileFromFile(vsPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0,
        &vertexShaderBuffer, &errorMessage);

    if (FAILED(hr))
    {
        if (errorMessage)
        {
            OutputDebugStringA((char*)errorMessage->GetBufferPointer());
        }
        return false;
    }

    // Compile pixel shader
    hr = D3DCompileFromFile(psPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0,
        &pixelShaderBuffer, &errorMessage);

    if (FAILED(hr))
    {
        if (errorMessage)
        {
            OutputDebugStringA((char*)errorMessage->GetBufferPointer());
        }
        return false;
    }

    // Create shaders
    hr = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(), nullptr, &m_vertexShader);
    if (FAILED(hr)) return false;

    hr = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
        pixelShaderBuffer->GetBufferSize(), nullptr, &m_pixelShader);
    if (FAILED(hr)) return false;

    // Create input layout
    D3D11_INPUT_ELEMENT_DESC polygonLayout[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    UINT numElements = ARRAYSIZE(polygonLayout);
    hr = device->CreateInputLayout(polygonLayout, numElements,
        vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
        &m_layout);
    if (FAILED(hr)) return false;

    // Create constant buffers
    D3D11_BUFFER_DESC constantBufferDesc = {};
    constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufferDesc.ByteWidth = sizeof(CB_PerObject);
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = device->CreateBuffer(&constantBufferDesc, nullptr, &m_constantBufferPerObject);
    if (FAILED(hr)) return false;

    constantBufferDesc.ByteWidth = sizeof(CB_PerFrame);
    hr = device->CreateBuffer(&constantBufferDesc, nullptr, &m_constantBufferPerFrame);
    if (FAILED(hr)) return false;

    // Create sampler state
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MaxAnisotropy = 16;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    hr = device->CreateSamplerState(&samplerDesc, &m_samplerState);
    if (FAILED(hr)) return false;

    return true;
}

void NormalMappingShader::SetShaderParameters(ID3D11DeviceContext* context,
    const CB_PerObject& objData,
    const CB_PerFrame& frameData,
    ID3D11ShaderResourceView* diffuseTexture,
    ID3D11ShaderResourceView* normalMap)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;

    // Update per-object constant buffer
    context->Map(m_constantBufferPerObject.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, &objData, sizeof(CB_PerObject));
    context->Unmap(m_constantBufferPerObject.Get(), 0);

    // Update per-frame constant buffer
    context->Map(m_constantBufferPerFrame.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, &frameData, sizeof(CB_PerFrame));
    context->Unmap(m_constantBufferPerFrame.Get(), 0);

    // Set constant buffers
    context->VSSetConstantBuffers(0, 1, m_constantBufferPerObject.GetAddressOf());
    context->VSSetConstantBuffers(1, 1, m_constantBufferPerFrame.GetAddressOf());
    context->PSSetConstantBuffers(0, 1, m_constantBufferPerObject.GetAddressOf());
    context->PSSetConstantBuffers(1, 1, m_constantBufferPerFrame.GetAddressOf());

    // Set textures
    context->PSSetShaderResources(0, 1, &diffuseTexture);
    context->PSSetShaderResources(1, 1, &normalMap);

    // Set sampler
    context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
}

void NormalMappingShader::Render(ID3D11DeviceContext* context, int indexCount)
{
    // Set input layout
    context->IASetInputLayout(m_layout.Get());

    // Set shaders
    context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    // Render
    context->DrawIndexed(indexCount, 0, 0);
}

void NormalMappingShader::ShutdownShader()
{
    m_vertexShader.Reset();
    m_pixelShader.Reset();
    m_layout.Reset();
    m_constantBufferPerObject.Reset();
    m_constantBufferPerFrame.Reset();
    m_samplerState.Reset();
}
```

### Normal Mapping Vertex Shader

```hlsl
// NormalMapping_VS.hlsl
cbuffer PerObjectBuffer : register(b0)
{
    matrix World;
    matrix WorldViewProjection;
    matrix WorldInverseTranspose;
};

cbuffer PerFrameBuffer : register(b1)
{
    float4 CameraPosition;
    float4 LightDirection;
    float4 LightColor;
    float Time;
    float NormalMapStrength;
    float ParallaxScale;
    float ParallaxBias;
};

struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float3 Tangent : TANGENT;
    float3 Bitangent : BITANGENT;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float3 WorldPos : POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Bitangent : BITANGENT;
    float3 ViewDirection : TEXCOORD1;
    float3 LightDir : TEXCOORD2;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // Transform position to clip space
    output.Position = mul(float4(input.Position, 1.0f), WorldViewProjection);

    // Calculate world position
    output.WorldPos = mul(float4(input.Position, 1.0f), World).xyz;

    // Pass texture coordinates
    output.TexCoord = input.TexCoord;

    // Transform normal, tangent, and bitangent to world space
    output.Normal = normalize(mul(input.Normal, (float3x3)WorldInverseTranspose));
    output.Tangent = normalize(mul(input.Tangent, (float3x3)World));
    output.Bitangent = normalize(mul(input.Bitangent, (float3x3)World));

    // Calculate view direction in world space
    output.ViewDirection = normalize(CameraPosition.xyz - output.WorldPos);

    // Calculate light direction in world space
    output.LightDir = normalize(-LightDirection.xyz);

    return output;
}
```

### Normal Mapping Pixel Shader

```hlsl
// NormalMapping_PS.hlsl
cbuffer PerObjectBuffer : register(b0)
{
    matrix World;
    matrix WorldViewProjection;
    matrix WorldInverseTranspose;
};

cbuffer PerFrameBuffer : register(b1)
{
    float4 CameraPosition;
    float4 LightDirection;
    float4 LightColor;
    float Time;
    float NormalMapStrength;
    float ParallaxScale;
    float ParallaxBias;
};

Texture2D DiffuseTexture : register(t0);
Texture2D NormalMap : register(t1);
SamplerState SamplerLinear : register(s0);

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float3 WorldPos : POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Bitangent : BITANGENT;
    float3 ViewDirection : TEXCOORD1;
    float3 LightDir : TEXCOORD2;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    // Construct TBN matrix (tangent space to world space)
    float3x3 TBN = float3x3(
        normalize(input.Tangent),
        normalize(input.Bitangent),
        normalize(input.Normal)
    );

    // Sample normal map
    float3 normalMapSample = NormalMap.Sample(SamplerLinear, input.TexCoord).xyz;

    // Convert from [0,1] to [-1,1]
    normalMapSample = normalMapSample * 2.0f - 1.0f;

    // Apply normal map strength
    normalMapSample.xy *= NormalMapStrength;
    normalMapSample = normalize(normalMapSample);

    // Transform normal from tangent space to world space
    float3 normal = normalize(mul(normalMapSample, TBN));

    // Sample diffuse texture
    float4 diffuseColor = DiffuseTexture.Sample(SamplerLinear, input.TexCoord);

    // Lighting calculations
    float3 lightDir = normalize(input.LightDir);
    float3 viewDir = normalize(input.ViewDirection);

    // Ambient
    float3 ambient = float3(0.2f, 0.2f, 0.2f) * diffuseColor.rgb;

    // Diffuse (Lambertian)
    float diffuseFactor = max(dot(normal, lightDir), 0.0f);
    float3 diffuse = diffuseFactor * LightColor.rgb * diffuseColor.rgb;

    // Specular (Blinn-Phong)
    float3 halfVector = normalize(lightDir + viewDir);
    float specularFactor = pow(max(dot(normal, halfVector), 0.0f), 32.0f);
    float3 specular = specularFactor * LightColor.rgb * 0.5f;

    // Combine lighting
    float3 finalColor = ambient + diffuse + specular;

    return float4(finalColor, diffuseColor.a);
}
```

---

## 3. Parallax Occlusion Mapping

Parallax Occlusion Mapping (POM) creates the illusion of depth on surfaces by offsetting texture coordinates based on view angle and height maps.

### Parallax Occlusion Mapping Pixel Shader

```hlsl
// ParallaxOcclusionMapping_PS.hlsl
Texture2D DiffuseTexture : register(t0);
Texture2D NormalMap : register(t1);
Texture2D HeightMap : register(t2);
SamplerState SamplerLinear : register(s0);

cbuffer PerFrameBuffer : register(b1)
{
    float4 CameraPosition;
    float4 LightDirection;
    float4 LightColor;
    float Time;
    float NormalMapStrength;
    float ParallaxScale;
    float ParallaxBias;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float3 WorldPos : POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Bitangent : BITANGENT;
    float3 ViewDirection : TEXCOORD1;
    float3 LightDir : TEXCOORD2;
};

// Parallax Occlusion Mapping function
float2 ParallaxOcclusionMapping(float2 texCoords, float3 viewDir, float3x3 TBN)
{
    // Transform view direction to tangent space
    float3 viewDirTangent = normalize(mul(transpose(TBN), viewDir));

    // Number of depth layers
    const float minLayers = 8.0f;
    const float maxLayers = 32.0f;
    float numLayers = lerp(maxLayers, minLayers, abs(dot(float3(0.0f, 0.0f, 1.0f), viewDirTangent)));

    // Calculate the size of each layer
    float layerDepth = 1.0f / numLayers;

    // Depth of current layer
    float currentLayerDepth = 0.0f;

    // Amount to shift texture coordinates per layer
    float2 P = viewDirTangent.xy * ParallaxScale;
    float2 deltaTexCoords = P / numLayers;

    // Initial values
    float2 currentTexCoords = texCoords;
    float currentDepthMapValue = HeightMap.Sample(SamplerLinear, currentTexCoords).r;

    // Iterate through depth layers
    [loop]
    while(currentLayerDepth < currentDepthMapValue)
    {
        // Shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;

        // Get depth value at current texture coordinates
        currentDepthMapValue = HeightMap.Sample(SamplerLinear, currentTexCoords).r;

        // Get depth of next layer
        currentLayerDepth += layerDepth;
    }

    // Parallax Occlusion Mapping with offset limiting
    float2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // Get depth after and before collision for linear interpolation
    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = HeightMap.Sample(SamplerLinear, prevTexCoords).r -
                        currentLayerDepth + layerDepth;

    // Interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    float2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0f - weight);

    return finalTexCoords;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    // Construct TBN matrix
    float3x3 TBN = float3x3(
        normalize(input.Tangent),
        normalize(input.Bitangent),
        normalize(input.Normal)
    );

    // Apply Parallax Occlusion Mapping
    float2 texCoords = ParallaxOcclusionMapping(input.TexCoord, input.ViewDirection, TBN);

    // Discard fragments outside texture bounds
    if(texCoords.x > 1.0f || texCoords.y > 1.0f || texCoords.x < 0.0f || texCoords.y < 0.0f)
        discard;

    // Sample textures with parallax-adjusted coordinates
    float4 diffuseColor = DiffuseTexture.Sample(SamplerLinear, texCoords);
    float3 normalMapSample = NormalMap.Sample(SamplerLinear, texCoords).xyz;

    // Convert normal from [0,1] to [-1,1]
    normalMapSample = normalMapSample * 2.0f - 1.0f;
    normalMapSample.xy *= NormalMapStrength;
    normalMapSample = normalize(normalMapSample);

    // Transform normal to world space
    float3 normal = normalize(mul(normalMapSample, TBN));

    // Lighting calculations
    float3 lightDir = normalize(input.LightDir);
    float3 viewDir = normalize(input.ViewDirection);

    // Ambient
    float3 ambient = float3(0.2f, 0.2f, 0.2f) * diffuseColor.rgb;

    // Diffuse
    float diffuseFactor = max(dot(normal, lightDir), 0.0f);
    float3 diffuse = diffuseFactor * LightColor.rgb * diffuseColor.rgb;

    // Specular
    float3 halfVector = normalize(lightDir + viewDir);
    float specularFactor = pow(max(dot(normal, halfVector), 0.0f), 64.0f);
    float3 specular = specularFactor * LightColor.rgb * 0.8f;

    // Self-shadowing
    float shadowMultiplier = 1.0f;
    if(diffuseFactor > 0.0f)
    {
        float2 shadowTexCoords = texCoords;
        float3 lightDirTangent = normalize(mul(transpose(TBN), lightDir));
        float shadowLayers = 15.0f;
        float layerHeight = 1.0f / shadowLayers;
        float2 texStep = ParallaxScale * lightDirTangent.xy / lightDirTangent.z / shadowLayers;

        float currentHeight = HeightMap.Sample(SamplerLinear, texCoords).r;
        float stepHeight = layerHeight;

        [loop]
        for(int i = 0; i < shadowLayers && stepHeight < 1.0f; i++)
        {
            shadowTexCoords += texStep;
            float sampledHeight = HeightMap.Sample(SamplerLinear, shadowTexCoords).r;

            if(sampledHeight > currentHeight + stepHeight)
            {
                shadowMultiplier = 0.3f;
                break;
            }

            stepHeight += layerHeight;
        }
    }

    // Apply shadow
    diffuse *= shadowMultiplier;
    specular *= shadowMultiplier;

    // Combine lighting
    float3 finalColor = ambient + diffuse + specular;

    return float4(finalColor, diffuseColor.a);
}
```

---

## 4. Screen Space Reflections (SSR)

Screen Space Reflections create realistic reflections by ray marching through the depth buffer.

### SSR Implementation

```cpp
// ScreenSpaceReflections.h
#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

struct CB_SSR_Params
{
    XMMATRIX ViewMatrix;
    XMMATRIX ProjectionMatrix;
    XMMATRIX InverseProjection;
    XMFLOAT4 ScreenSize; // x=width, y=height, z=1/width, w=1/height
    float MaxDistance;
    float Resolution;
    float Thickness;
    float StepCount;
};

class ScreenSpaceReflections
{
public:
    ScreenSpaceReflections();
    ~ScreenSpaceReflections();

    bool Initialize(ID3D11Device* device, int width, int height);
    void Render(ID3D11DeviceContext* context,
        ID3D11ShaderResourceView* colorBuffer,
        ID3D11ShaderResourceView* normalBuffer,
        ID3D11ShaderResourceView* depthBuffer,
        const CB_SSR_Params& params);

    ID3D11ShaderResourceView* GetReflectionTexture() const { return m_reflectionSRV.Get(); }

private:
    ComPtr<ID3D11PixelShader> m_pixelShader;
    ComPtr<ID3D11Buffer> m_constantBuffer;
    ComPtr<ID3D11Texture2D> m_reflectionTexture;
    ComPtr<ID3D11RenderTargetView> m_reflectionRTV;
    ComPtr<ID3D11ShaderResourceView> m_reflectionSRV;
    ComPtr<ID3D11SamplerState> m_samplerState;

    int m_width;
    int m_height;
};
```

### SSR Pixel Shader

```hlsl
// ScreenSpaceReflections_PS.hlsl
cbuffer SSRParams : register(b0)
{
    matrix ViewMatrix;
    matrix ProjectionMatrix;
    matrix InverseProjection;
    float4 ScreenSize;
    float MaxDistance;
    float Resolution;
    float Thickness;
    float StepCount;
};

Texture2D ColorBuffer : register(t0);
Texture2D NormalBuffer : register(t1);
Texture2D DepthBuffer : register(t2);
SamplerState SamplerLinear : register(s0);

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

// Convert screen space position to view space
float3 ScreenToView(float2 screenPos, float depth)
{
    float4 clipPos = float4(screenPos * 2.0f - 1.0f, depth, 1.0f);
    clipPos.y = -clipPos.y;
    float4 viewPos = mul(clipPos, InverseProjection);
    return viewPos.xyz / viewPos.w;
}

// Convert view space position to screen space
float3 ViewToScreen(float3 viewPos)
{
    float4 clipPos = mul(float4(viewPos, 1.0f), ProjectionMatrix);
    float3 ndcPos = clipPos.xyz / clipPos.w;
    ndcPos.y = -ndcPos.y;
    return float3(ndcPos.xy * 0.5f + 0.5f, ndcPos.z);
}

// Ray marching for SSR
bool RayMarch(float3 rayOrigin, float3 rayDir, out float2 hitPixel, out float3 hitPoint)
{
    float stepSize = MaxDistance / StepCount;
    float3 rayStep = rayDir * stepSize;
    float3 currentPos = rayOrigin + rayStep;

    [loop]
    for(int i = 0; i < (int)StepCount; i++)
    {
        float3 screenPos = ViewToScreen(currentPos);

        // Check if ray is outside screen
        if(screenPos.x < 0.0f || screenPos.x > 1.0f ||
           screenPos.y < 0.0f || screenPos.y > 1.0f)
        {
            return false;
        }

        // Sample depth at current screen position
        float sampledDepth = DepthBuffer.SampleLevel(SamplerLinear, screenPos.xy, 0).r;
        float3 sampledPos = ScreenToView(screenPos.xy, sampledDepth);

        // Check if ray intersects geometry
        float depthDiff = currentPos.z - sampledPos.z;

        if(depthDiff > 0.0f && depthDiff < Thickness)
        {
            // Binary search for more accurate hit point
            float3 searchStart = currentPos - rayStep;
            float3 searchEnd = currentPos;

            [loop]
            for(int j = 0; j < 8; j++)
            {
                float3 midPoint = (searchStart + searchEnd) * 0.5f;
                float3 midScreen = ViewToScreen(midPoint);
                float midDepth = DepthBuffer.SampleLevel(SamplerLinear, midScreen.xy, 0).r;
                float3 midSampledPos = ScreenToView(midScreen.xy, midDepth);

                if(midPoint.z > midSampledPos.z)
                {
                    searchEnd = midPoint;
                }
                else
                {
                    searchStart = midPoint;
                }
            }

            hitPoint = (searchStart + searchEnd) * 0.5f;
            float3 hitScreen = ViewToScreen(hitPoint);
            hitPixel = hitScreen.xy;
            return true;
        }

        currentPos += rayStep;
    }

    return false;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    // Sample G-Buffer
    float depth = DepthBuffer.Sample(SamplerLinear, input.TexCoord).r;
    float3 normal = NormalBuffer.Sample(SamplerLinear, input.TexCoord).xyz * 2.0f - 1.0f;
    float4 color = ColorBuffer.Sample(SamplerLinear, input.TexCoord);

    // Early exit for skybox
    if(depth >= 1.0f)
    {
        return float4(0, 0, 0, 0);
    }

    // Reconstruct view space position
    float3 viewPos = ScreenToView(input.TexCoord, depth);

    // Transform normal to view space
    float3 viewNormal = normalize(mul(normal, (float3x3)ViewMatrix));

    // Calculate reflection direction
    float3 viewDir = normalize(viewPos);
    float3 reflectionDir = reflect(viewDir, viewNormal);

    // Perform ray march
    float2 hitPixel;
    float3 hitPoint;
    bool hit = RayMarch(viewPos, reflectionDir, hitPixel, hitPoint);

    if(hit)
    {
        // Sample color at hit point
        float4 reflectionColor = ColorBuffer.Sample(SamplerLinear, hitPixel);

        // Calculate fade based on screen edge distance
        float2 edgeDist = abs(hitPixel - 0.5f) * 2.0f;
        float edgeFade = 1.0f - max(edgeDist.x, edgeDist.y);
        edgeFade = smoothstep(0.0f, 0.2f, edgeFade);

        // Calculate fade based on reflection angle
        float angleFade = 1.0f - max(0.0f, dot(viewDir, reflectionDir));
        angleFade = smoothstep(0.0f, 1.0f, angleFade);

        float finalFade = edgeFade * angleFade;

        return float4(reflectionColor.rgb, finalFade);
    }

    return float4(0, 0, 0, 0);
}
```

---

## 5. Deferred Shading Architecture

Deferred shading separates geometry rendering from lighting calculations, enabling efficient handling of multiple lights.

### Deferred Rendering System

```cpp
// DeferredRenderer.h
#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <vector>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

struct GBuffer
{
    ComPtr<ID3D11Texture2D> albedoTexture;
    ComPtr<ID3D11Texture2D> normalTexture;
    ComPtr<ID3D11Texture2D> positionTexture;
    ComPtr<ID3D11Texture2D> depthTexture;

    ComPtr<ID3D11RenderTargetView> albedoRTV;
    ComPtr<ID3D11RenderTargetView> normalRTV;
    ComPtr<ID3D11RenderTargetView> positionRTV;
    ComPtr<ID3D11DepthStencilView> depthDSV;

    ComPtr<ID3D11ShaderResourceView> albedoSRV;
    ComPtr<ID3D11ShaderResourceView> normalSRV;
    ComPtr<ID3D11ShaderResourceView> positionSRV;
    ComPtr<ID3D11ShaderResourceView> depthSRV;
};

struct PointLight
{
    XMFLOAT3 Position;
    float Radius;
    XMFLOAT3 Color;
    float Intensity;
};

class DeferredRenderer
{
public:
    DeferredRenderer();
    ~DeferredRenderer();

    bool Initialize(ID3D11Device* device, int width, int height);
    void BeginGeometryPass(ID3D11DeviceContext* context);
    void EndGeometryPass(ID3D11DeviceContext* context);
    void RenderLightingPass(ID3D11DeviceContext* context,
        const std::vector<PointLight>& lights,
        const XMMATRIX& viewMatrix,
        const XMFLOAT3& cameraPos);

    const GBuffer& GetGBuffer() const { return m_gBuffer; }

private:
    bool CreateGBuffer(ID3D11Device* device, int width, int height);

    GBuffer m_gBuffer;
    ComPtr<ID3D11PixelShader> m_lightingPS;
    ComPtr<ID3D11Buffer> m_lightBuffer;

    int m_width;
    int m_height;
};
```

### G-Buffer Geometry Pass Pixel Shader

```hlsl
// DeferredGeometry_PS.hlsl
struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float3 WorldPos : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 Albedo : SV_TARGET0;
    float4 Normal : SV_TARGET1;
    float4 Position : SV_TARGET2;
};

Texture2D AlbedoTexture : register(t0);
SamplerState SamplerLinear : register(s0);

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;

    // Sample albedo texture
    output.Albedo = AlbedoTexture.Sample(SamplerLinear, input.TexCoord);

    // Store normalized normal in [0,1] range
    output.Normal = float4(normalize(input.Normal) * 0.5f + 0.5f, 1.0f);

    // Store world position
    output.Position = float4(input.WorldPos, 1.0f);

    return output;
}
```

### Deferred Lighting Pass Pixel Shader

```hlsl
// DeferredLighting_PS.hlsl
#define MAX_LIGHTS 256

struct PointLight
{
    float3 Position;
    float Radius;
    float3 Color;
    float Intensity;
};

cbuffer LightBuffer : register(b0)
{
    PointLight Lights[MAX_LIGHTS];
    int LightCount;
    float3 CameraPosition;
};

Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D PositionTexture : register(t2);
SamplerState SamplerPoint : register(s0);

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    // Sample G-Buffer
    float4 albedo = AlbedoTexture.Sample(SamplerPoint, input.TexCoord);
    float3 normal = normalize(NormalTexture.Sample(SamplerPoint, input.TexCoord).xyz * 2.0f - 1.0f);
    float3 worldPos = PositionTexture.Sample(SamplerPoint, input.TexCoord).xyz;

    // Calculate view direction
    float3 viewDir = normalize(CameraPosition - worldPos);

    // Ambient lighting
    float3 ambient = albedo.rgb * 0.1f;

    // Accumulate lighting from all point lights
    float3 lighting = float3(0, 0, 0);

    [loop]
    for(int i = 0; i < LightCount; i++)
    {
        float3 lightDir = Lights[i].Position - worldPos;
        float distance = length(lightDir);

        // Skip lights that are too far
        if(distance > Lights[i].Radius)
            continue;

        lightDir = normalize(lightDir);

        // Attenuation
        float attenuation = 1.0f - saturate(distance / Lights[i].Radius);
        attenuation *= attenuation; // Quadratic falloff

        // Diffuse
        float diffuseFactor = max(dot(normal, lightDir), 0.0f);
        float3 diffuse = diffuseFactor * Lights[i].Color * Lights[i].Intensity;

        // Specular (Blinn-Phong)
        float3 halfVector = normalize(lightDir + viewDir);
        float specularFactor = pow(max(dot(normal, halfVector), 0.0f), 32.0f);
        float3 specular = specularFactor * Lights[i].Color * Lights[i].Intensity * 0.5f;

        // Add contribution
        lighting += (diffuse + specular) * attenuation;
    }

    // Combine lighting
    float3 finalColor = ambient + lighting * albedo.rgb;

    return float4(finalColor, 1.0f);
}
```

---

## 6. Compute Shaders for Graphics

Compute shaders enable general-purpose GPU computing for graphics tasks like particle systems, post-processing, and procedural generation.

### Particle System with Compute Shader

```cpp
// ComputeParticleSystem.h
#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

struct Particle
{
    XMFLOAT3 Position;
    float Life;
    XMFLOAT3 Velocity;
    float Size;
    XMFLOAT4 Color;
};

struct CB_ParticleParams
{
    float DeltaTime;
    float Time;
    XMFLOAT2 Padding;
    XMFLOAT3 EmitterPosition;
    float EmitRate;
    XMFLOAT3 Gravity;
    float ParticleLifetime;
};

class ComputeParticleSystem
{
public:
    ComputeParticleSystem();
    ~ComputeParticleSystem();

    bool Initialize(ID3D11Device* device, int maxParticles);
    void Update(ID3D11DeviceContext* context, float deltaTime, const XMFLOAT3& emitterPos);
    void Render(ID3D11DeviceContext* context, const XMMATRIX& viewProj);

private:
    ComPtr<ID3D11ComputeShader> m_computeShader;
    ComPtr<ID3D11Buffer> m_particleBuffer;
    ComPtr<ID3D11UnorderedAccessView> m_particleUAV;
    ComPtr<ID3D11ShaderResourceView> m_particleSRV;
    ComPtr<ID3D11Buffer> m_constantBuffer;

    int m_maxParticles;
    float m_time;
};
```

### Particle Update Compute Shader

```hlsl
// ParticleUpdate_CS.hlsl
struct Particle
{
    float3 Position;
    float Life;
    float3 Velocity;
    float Size;
    float4 Color;
};

cbuffer ParticleParams : register(b0)
{
    float DeltaTime;
    float Time;
    float2 Padding;
    float3 EmitterPosition;
    float EmitRate;
    float3 Gravity;
    float ParticleLifetime;
};

RWStructuredBuffer<Particle> ParticleBuffer : register(u0);

// Random number generation
float Random(float2 seed)
{
    return frac(sin(dot(seed, float2(12.9898f, 78.233f))) * 43758.5453f);
}

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint index = DTid.x;
    Particle particle = ParticleBuffer[index];

    // Update existing particle
    if(particle.Life > 0.0f)
    {
        // Update lifetime
        particle.Life -= DeltaTime;

        // Update physics
        particle.Velocity += Gravity * DeltaTime;
        particle.Position += particle.Velocity * DeltaTime;

        // Update color (fade out)
        particle.Color.a = particle.Life / ParticleLifetime;

        // Update size
        particle.Size = lerp(2.0f, 0.5f, 1.0f - (particle.Life / ParticleLifetime));
    }
    else
    {
        // Emit new particle
        float emitChance = EmitRate * DeltaTime;
        float2 randomSeed = float2(index, Time);

        if(Random(randomSeed) < emitChance)
        {
            // Initialize new particle
            particle.Position = EmitterPosition;
            particle.Life = ParticleLifetime;

            // Random velocity
            float theta = Random(randomSeed) * 6.28318f; // 2*PI
            float phi = Random(randomSeed + float2(1, 0)) * 3.14159f; // PI
            float speed = 5.0f + Random(randomSeed + float2(0, 1)) * 10.0f;

            particle.Velocity = float3(
                sin(phi) * cos(theta),
                cos(phi),
                sin(phi) * sin(theta)
            ) * speed;

            // Random color
            float hue = Random(randomSeed + float2(2, 0));
            particle.Color = float4(
                abs(sin(hue * 6.28318f)),
                abs(sin((hue + 0.333f) * 6.28318f)),
                abs(sin((hue + 0.666f) * 6.28318f)),
                1.0f
            );

            particle.Size = 1.0f;
        }
    }

    // Write updated particle
    ParticleBuffer[index] = particle;
}
```

---

## 7. Tessellation Shaders

Tessellation shaders dynamically subdivide geometry on the GPU for detail adaptation and displacement mapping.

### Tessellation Pipeline Shaders

```hlsl
// Tessellation_VS.hlsl (Vertex Shader)
struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.Position = input.Position;
    output.Normal = input.Normal;
    output.TexCoord = input.TexCoord;
    return output;
}

// Tessellation_HS.hlsl (Hull Shader)
cbuffer TessellationParams : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 CameraPosition;
    float TessellationFactor;
    float DisplacementScale;
    float2 Padding;
};

struct HS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

struct HS_OUTPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

struct HS_CONSTANT_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor;
    float InsideTessFactor : SV_InsideTessFactor;
};

// Calculate tessellation factors based on distance to camera
float CalculateTessellationFactor(float3 position)
{
    float distance = length(CameraPosition.xyz - position);
    float factor = saturate(1.0f - distance / 100.0f);
    return 1.0f + factor * TessellationFactor;
}

HS_CONSTANT_OUTPUT ConstantHS(InputPatch<HS_INPUT, 3> patch)
{
    HS_CONSTANT_OUTPUT output;

    // Calculate tessellation factors based on distance
    float3 center = (patch[0].Position + patch[1].Position + patch[2].Position) / 3.0f;
    float tessFactor = CalculateTessellationFactor(center);

    output.EdgeTessFactor[0] = tessFactor;
    output.EdgeTessFactor[1] = tessFactor;
    output.EdgeTessFactor[2] = tessFactor;
    output.InsideTessFactor = tessFactor;

    return output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
HS_OUTPUT main(InputPatch<HS_INPUT, 3> patch, uint id : SV_OutputControlPointID)
{
    HS_OUTPUT output;
    output.Position = patch[id].Position;
    output.Normal = patch[id].Normal;
    output.TexCoord = patch[id].TexCoord;
    return output;
}

// Tessellation_DS.hlsl (Domain Shader)
Texture2D DisplacementMap : register(t0);
SamplerState SamplerLinear : register(s0);

struct DS_OUTPUT
{
    float4 Position : SV_POSITION;
    float3 WorldPos : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

[domain("tri")]
DS_OUTPUT main(HS_CONSTANT_OUTPUT input,
    float3 barycentricCoords : SV_DomainLocation,
    const OutputPatch<HS_OUTPUT, 3> patch)
{
    DS_OUTPUT output;

    // Interpolate position using barycentric coordinates
    float3 position = barycentricCoords.x * patch[0].Position +
                      barycentricCoords.y * patch[1].Position +
                      barycentricCoords.z * patch[2].Position;

    // Interpolate normal
    float3 normal = normalize(
        barycentricCoords.x * patch[0].Normal +
        barycentricCoords.y * patch[1].Normal +
        barycentricCoords.z * patch[2].Normal
    );

    // Interpolate texture coordinates
    float2 texCoord = barycentricCoords.x * patch[0].TexCoord +
                      barycentricCoords.y * patch[1].TexCoord +
                      barycentricCoords.z * patch[2].TexCoord;

    // Sample displacement map
    float displacement = DisplacementMap.SampleLevel(SamplerLinear, texCoord, 0).r;

    // Apply displacement along normal
    position += normal * displacement * DisplacementScale;

    // Transform to world space
    output.WorldPos = mul(float4(position, 1.0f), World).xyz;
    output.Normal = normalize(mul(normal, (float3x3)World));
    output.TexCoord = texCoord;

    // Transform to clip space
    float4 worldPos = float4(output.WorldPos, 1.0f);
    float4 viewPos = mul(worldPos, View);
    output.Position = mul(viewPos, Projection);

    return output;
}
```

---

## 8. Performance Optimization for Shaders

### Optimization Techniques

1. **Minimize Texture Samples**
```hlsl
// Bad: Multiple samples
float4 color1 = Texture.Sample(Sampler, uv);
float4 color2 = Texture.Sample(Sampler, uv + offset1);
float4 color3 = Texture.Sample(Sampler, uv + offset2);

// Good: Use Gather for multiple samples
float4 samples = Texture.Gather(Sampler, uv);
```

2. **Use Lower Precision When Possible**
```hlsl
// Use half precision for intermediates
min16float roughness = Material.roughness;
min16float3 tangent = normalize(input.Tangent);
```

3. **Early Exit Optimization**
```hlsl
// Exit early for pixels that don't need processing
if(depth >= 1.0f)
    return BackgroundColor;

// Main shader logic here
```

4. **Branch Reduction**
```hlsl
// Bad: Dynamic branching
if(useLighting)
    color = CalculateLighting(input);
else
    color = BaseColor;

// Good: Use lerp
color = lerp(BaseColor, CalculateLighting(input), useLighting);
```

---

## 9. Complete Implementation Example

### Main Application

```cpp
// AdvancedShadingApp.cpp
#include "AdvancedShaderSystem.h"
#include "DeferredRenderer.h"
#include "ComputeParticleSystem.h"

class AdvancedShadingApp
{
public:
    bool Initialize(HWND hwnd, int width, int height);
    void Update(float deltaTime);
    void Render();
    void Shutdown();

private:
    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_context;
    ComPtr<IDXGISwapChain> m_swapChain;

    std::unique_ptr<NormalMappingShader> m_normalMappingShader;
    std::unique_ptr<DeferredRenderer> m_deferredRenderer;
    std::unique_ptr<ComputeParticleSystem> m_particleSystem;
    std::unique_ptr<ScreenSpaceReflections> m_ssrEffect;

    float m_time;
};

bool AdvancedShadingApp::Initialize(HWND hwnd, int width, int height)
{
    // Create D3D11 device and swap chain
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_DEBUG,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &m_swapChain,
        &m_device,
        &featureLevel,
        &m_context
    );

    if (FAILED(hr))
        return false;

    // Initialize shader systems
    m_normalMappingShader = std::make_unique<NormalMappingShader>();
    if (!m_normalMappingShader->Initialize(m_device.Get(),
        L"NormalMapping_VS.hlsl", L"NormalMapping_PS.hlsl"))
        return false;

    m_deferredRenderer = std::make_unique<DeferredRenderer>();
    if (!m_deferredRenderer->Initialize(m_device.Get(), width, height))
        return false;

    m_particleSystem = std::make_unique<ComputeParticleSystem>();
    if (!m_particleSystem->Initialize(m_device.Get(), 10000))
        return false;

    m_ssrEffect = std::make_unique<ScreenSpaceReflections>();
    if (!m_ssrEffect->Initialize(m_device.Get(), width, height))
        return false;

    m_time = 0.0f;
    return true;
}

void AdvancedShadingApp::Update(float deltaTime)
{
    m_time += deltaTime;

    // Update particle system
    XMFLOAT3 emitterPos(0.0f, 5.0f, 0.0f);
    m_particleSystem->Update(m_context.Get(), deltaTime, emitterPos);
}

void AdvancedShadingApp::Render()
{
    // Clear back buffer
    float clearColor[4] = { 0.1f, 0.1f, 0.15f, 1.0f };

    // Deferred rendering pass
    m_deferredRenderer->BeginGeometryPass(m_context.Get());

    // Render scene geometry here with normal mapping
    // ...

    m_deferredRenderer->EndGeometryPass(m_context.Get());

    // Lighting pass
    std::vector<PointLight> lights;
    // Add lights to the scene

    XMMATRIX viewMatrix = XMMatrixIdentity();
    XMFLOAT3 cameraPos(0, 0, -10);

    m_deferredRenderer->RenderLightingPass(m_context.Get(), lights, viewMatrix, cameraPos);

    // Apply screen space reflections
    // ...

    // Render particles
    XMMATRIX viewProj = XMMatrixIdentity();
    m_particleSystem->Render(m_context.Get(), viewProj);

    // Present
    m_swapChain->Present(1, 0);
}

void AdvancedShadingApp::Shutdown()
{
    m_normalMappingShader.reset();
    m_deferredRenderer.reset();
    m_particleSystem.reset();
    m_ssrEffect.reset();
}
```

---

## 10. Exercises

### Exercise 1: Implement Rim Lighting
Add rim lighting (Fresnel effect) to the normal mapping shader.

**Hint**: Use `pow(1.0 - dot(normal, viewDir), 3.0)` for rim factor.

### Exercise 2: Optimize SSR
Implement hierarchical ray marching for faster SSR.

### Exercise 3: Add Bloom Effect
Create a compute shader for bloom post-processing.

### Exercise 4: Distance-Based LOD
Implement automatic tessellation factor adjustment based on camera distance.

### Exercise 5: Multi-Bounce SSR
Extend SSR to support multiple reflection bounces.

---

## Key Takeaways

1. ✅ Normal mapping adds detail without geometry cost
2. ✅ Parallax Occlusion Mapping creates convincing depth
3. ✅ Screen Space Reflections provide realistic reflections cheaply
4. ✅ Deferred shading scales efficiently with many lights
5. ✅ Compute shaders enable GPU-accelerated effects
6. ✅ Tessellation adapts geometry detail dynamically
7. ✅ Shader optimization is critical for performance
8. ✅ Multi-pass rendering enables complex effects

---

## Summary Checklist

Before moving to Lesson 82, ensure you can:
- [ ] Implement normal mapping with tangent space
- [ ] Create parallax occlusion mapping with self-shadowing
- [ ] Build a screen space reflection system
- [ ] Design a deferred rendering pipeline
- [ ] Write compute shaders for particles
- [ ] Use tessellation for adaptive detail
- [ ] Optimize shaders for performance
- [ ] Combine multiple advanced techniques

**Congratulations on completing Lesson 81!**

Next lesson: Procedural Textures and Noise Generation
