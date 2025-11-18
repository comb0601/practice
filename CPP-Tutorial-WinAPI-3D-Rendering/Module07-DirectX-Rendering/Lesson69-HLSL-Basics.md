# Lesson 69: Shaders - HLSL Basics

## High-Level Shading Language (HLSL)

HLSL is DirectX's C-like shader programming language for writing vertex, pixel, and compute shaders.

## Basic Syntax

### Data Types

```hlsl
// Scalars
float  value;
int    count;
bool   flag;

// Vectors
float2 texcoord;  // 2D vector
float3 position;  // 3D vector
float4 color;     // 4D vector (RGBA)

// Matrices
float4x4 worldMatrix;
float3x3 rotationMatrix;

// Accessing components
float3 pos = float3(1, 2, 3);
float x = pos.x;  // or pos.r
float y = pos.y;  // or pos.g
float z = pos.z;  // or pos.b
```

### Structures

```hlsl
struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};
```

## Vertex Shader Example

```hlsl
cbuffer ConstantBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // Transform position
    float4 worldPos = mul(float4(input.position, 1.0f), world);
    float4 viewPos = mul(worldPos, view);
    output.position = mul(viewPos, projection);

    // Pass through color
    output.color = input.color;

    return output;
}
```

## Pixel Shader Example

```hlsl
struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return input.color;
}
```

## Constant Buffers

```hlsl
cbuffer PerFrameBuffer : register(b0)
{
    float4 lightDirection;
    float4 lightColor;
    float time;
};

cbuffer PerObjectBuffer : register(b1)
{
    matrix worldMatrix;
    float4 objectColor;
};
```

## Built-in Functions

```hlsl
// Math functions
float d = dot(a, b);           // Dot product
float3 c = cross(a, b);        // Cross product
float l = length(v);           // Vector length
float3 n = normalize(v);       // Normalize vector
float v = saturate(x);         // Clamp to [0,1]
float v = clamp(x, min, max);  // Clamp to range
float v = lerp(a, b, t);       // Linear interpolation

// Trigonometric
float s = sin(angle);
float c = cos(angle);
float t = tan(angle);

// Power functions
float p = pow(base, exponent);
float s = sqrt(value);
float e = exp(value);
float l = log(value);
```

## Semantics

Semantics bind shader inputs/outputs:

```hlsl
// Input semantics
POSITION  // Vertex position
NORMAL    // Vertex normal
TEXCOORD  // Texture coordinates
COLOR     // Vertex color

// Output semantics
SV_POSITION  // Transformed position (required for vertex shader)
SV_TARGET    // Render target output (required for pixel shader)
SV_DEPTH     // Depth value
```

## Shader Compilation (C++)

```cpp
ID3DBlob* shaderBlob = nullptr;
ID3DBlob* errorBlob = nullptr;

HRESULT hr = D3DCompileFromFile(
    L"shader.hlsl",           // Shader file
    nullptr,                  // Defines
    nullptr,                  // Include handler
    "VS_Main",                // Entry point
    "vs_5_0",                 // Target profile
    D3DCOMPILE_DEBUG,         // Compile flags
    0,                        // Effect flags
    &shaderBlob,              // Output blob
    &errorBlob                // Error messages
);

if (FAILED(hr))
{
    if (errorBlob)
    {
        OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        errorBlob->Release();
    }
}

device->CreateVertexShader(
    shaderBlob->GetBufferPointer(),
    shaderBlob->GetBufferSize(),
    nullptr,
    &vertexShader
);
```

## Shader Profiles

```
Vertex Shader: vs_4_0, vs_5_0
Pixel Shader: ps_4_0, ps_5_0
Geometry Shader: gs_4_0, gs_5_0
Compute Shader: cs_4_0, cs_5_0
Hull Shader: hs_5_0
Domain Shader: ds_5_0
```

---

**Study Time:** 4-5 hours
