# Lesson 70: Vertex Shaders

## Vertex Shader Fundamentals

Vertex shaders process each vertex, transforming positions and passing data to the pixel shader.

## Transformation Pipeline

```
Model Space (Object coordinates)
    ↓ World Matrix
World Space (Scene coordinates)
    ↓ View Matrix
View Space (Camera coordinates)
    ↓ Projection Matrix
Clip Space (Homogeneous coordinates)
    ↓ Perspective Divide
NDC (Normalized Device Coordinates)
```

## Basic Transformation Shader

```hlsl
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VS_INPUT
{
    float3 position : POSITION;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // Transform to world space
    float4 worldPos = mul(float4(input.position, 1.0f), worldMatrix);

    // Transform to view space
    float4 viewPos = mul(worldPos, viewMatrix);

    // Transform to clip space
    output.position = mul(viewPos, projectionMatrix);

    return output;
}
```

## Combined Transform Shader

```hlsl
cbuffer TransformBuffer : register(b0)
{
    matrix worldViewProjection;  // Pre-multiplied matrix
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), worldViewProjection);
    return output;
}
```

## Passing Data to Pixel Shader

```hlsl
struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // Transform position
    output.worldPos = mul(float4(input.position, 1.0f), worldMatrix).xyz;
    float4 viewPos = mul(float4(output.worldPos, 1.0f), viewMatrix);
    output.position = mul(viewPos, projectionMatrix);

    // Transform normal
    output.normal = mul(input.normal, (float3x3)worldMatrix);

    // Pass through texture coordinates and color
    output.texcoord = input.texcoord;
    output.color = input.color;

    return output;
}
```

## Normal Transformation

```hlsl
// Correct normal transformation (handles non-uniform scaling)
float3x3 normalMatrix = transpose((float3x3)inverse(worldMatrix));
output.normal = mul(input.normal, normalMatrix);

// Simplified (if no non-uniform scaling)
output.normal = mul(input.normal, (float3x3)worldMatrix);

// Always normalize in pixel shader
output.normal = normalize(output.normal);
```

## Creating and Binding Constant Buffers (C++)

```cpp
struct MatrixBuffer
{
    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;
};

D3D11_BUFFER_DESC cbDesc = {};
cbDesc.ByteWidth = sizeof(MatrixBuffer);
cbDesc.Usage = D3D11_USAGE_DYNAMIC;
cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

ID3D11Buffer* constantBuffer;
device->CreateBuffer(&cbDesc, nullptr, &constantBuffer);

// Update constant buffer
D3D11_MAPPED_SUBRESOURCE mappedResource;
context->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

MatrixBuffer* dataPtr = (MatrixBuffer*)mappedResource.pData;
dataPtr->world = XMMatrixTranspose(worldMatrix);
dataPtr->view = XMMatrixTranspose(viewMatrix);
dataPtr->projection = XMMatrixTranspose(projectionMatrix);

context->Unmap(constantBuffer, 0);

// Bind to vertex shader
context->VSSetConstantBuffers(0, 1, &constantBuffer);
```

## Matrix Multiplication Order

```hlsl
// Row-major (HLSL default, DirectXMath)
output.position = mul(float4(input.position, 1.0f), worldViewProj);

// Column-major
output.position = mul(worldViewProj, float4(input.position, 1.0f));

// DirectX uses row-major by default
// OpenGL uses column-major
```

## Per-Vertex Lighting

```hlsl
cbuffer LightBuffer : register(b1)
{
    float3 lightDirection;
    float4 lightColor;
    float4 ambientColor;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // Transform position
    output.position = mul(float4(input.position, 1.0f), worldViewProj);

    // Transform normal
    float3 normal = normalize(mul(input.normal, (float3x3)worldMatrix));

    // Calculate lighting
    float lightIntensity = saturate(dot(normal, -lightDirection));
    output.color = ambientColor + (lightColor * lightIntensity);

    return output;
}
```

---

**Study Time:** 4 hours
