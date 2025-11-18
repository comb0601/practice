# Lesson 72: Constant Buffers

## Constant Buffer Overview

Constant buffers provide shader-accessible data that remains constant during a draw call.

## Creating Constant Buffers

```cpp
struct MatrixBuffer {
    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;
};

D3D11_BUFFER_DESC cbDesc = {};
cbDesc.ByteWidth = sizeof(MatrixBuffer);
cbDesc.Usage = D3D11_USAGE_DYNAMIC;
cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

device->CreateBuffer(&cbDesc, nullptr, &constantBuffer);
```

## Updating Constant Buffers

```cpp
D3D11_MAPPED_SUBRESOURCE mappedResource;
context->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

MatrixBuffer* dataPtr = (MatrixBuffer*)mappedResource.pData;
dataPtr->world = XMMatrixTranspose(worldMatrix);
dataPtr->view = XMMatrixTranspose(viewMatrix);
dataPtr->projection = XMMatrixTranspose(projectionMatrix);

context->Unmap(constantBuffer, 0);

context->VSSetConstantBuffers(0, 1, &constantBuffer);
```

## HLSL Constant Buffer

```hlsl
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer LightBuffer : register(b1)
{
    float4 lightColor;
    float3 lightDirection;
    float padding;
};
```

## Alignment Rules

Constant buffers must be 16-byte aligned:

```cpp
struct ConstantBuffer {
    XMFLOAT4 color;     // 16 bytes - OK
    XMFLOAT3 position;  // 12 bytes - needs padding
    float padding;      // 4 bytes padding
};
```

---

**Study Time:** 3 hours
