# Lesson 68: Vertex & Index Buffers

## Overview

Buffers are fundamental to DirectX rendering. This lesson covers creating, managing, and using vertex and index buffers for efficient geometry rendering.

## Vertex Buffers

### Creating a Vertex Buffer

```cpp
struct Vertex
{
    float x, y, z;     // Position
    float r, g, b, a;  // Color
};

Vertex vertices[] = {
    { -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
    {  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
    {  0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }
};

D3D11_BUFFER_DESC bufferDesc = {};
bufferDesc.Usage = D3D11_USAGE_DEFAULT;
bufferDesc.ByteWidth = sizeof(vertices);
bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

D3D11_SUBRESOURCE_DATA initData = { vertices };

ID3D11Buffer* vertexBuffer;
device->CreateBuffer(&bufferDesc, &initData, &vertexBuffer);
```

### Binding Vertex Buffer

```cpp
UINT stride = sizeof(Vertex);
UINT offset = 0;
context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
```

## Index Buffers

### Why Use Index Buffers?

**Without Indices (6 vertices for 2 triangles):**
```
v0, v1, v2, v1, v3, v2  // Duplicated vertices
```

**With Indices (4 vertices + 6 indices):**
```
Vertices: v0, v1, v2, v3
Indices: 0, 1, 2, 1, 3, 2  // Reference by index
```

### Creating Index Buffer

```cpp
UINT indices[] = { 0, 1, 2, 1, 3, 2 };

D3D11_BUFFER_DESC indexBufferDesc = {};
indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
indexBufferDesc.ByteWidth = sizeof(indices);
indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

D3D11_SUBRESOURCE_DATA indexData = { indices };

ID3D11Buffer* indexBuffer;
device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
```

### Drawing with Indices

```cpp
context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
context->DrawIndexed(6, 0, 0);  // 6 indices, start at 0
```

## Buffer Usage Types

```cpp
// DEFAULT: GPU read/write, no CPU access
bufferDesc.Usage = D3D11_USAGE_DEFAULT;

// IMMUTABLE: GPU read only, set once at creation
bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;

// DYNAMIC: GPU read, CPU write (frequently updated)
bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

// STAGING: CPU read/write (for copying data)
bufferDesc.Usage = D3D11_USAGE_STAGING;
bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
```

## Input Layouts

Input layouts describe the vertex structure to DirectX:

```cpp
D3D11_INPUT_ELEMENT_DESC layout[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

device->CreateInputLayout(layout, 2, vertexShaderBlob->GetBufferPointer(),
    vertexShaderBlob->GetBufferSize(), &inputLayout);

context->IASetInputLayout(inputLayout);
```

## Dynamic Buffers

### Updating Dynamic Buffers

```cpp
D3D11_MAPPED_SUBRESOURCE mappedResource;
context->Map(dynamicBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

Vertex* vertices = (Vertex*)mappedResource.pData;
// Modify vertices
vertices[0].x += 0.01f;

context->Unmap(dynamicBuffer, 0);
```

## Best Practices

1. **Use Index Buffers** for meshes with shared vertices
2. **Immutable Buffers** for static geometry (best performance)
3. **Dynamic Buffers** for frequently updated data
4. **Batch Draw Calls** to reduce overhead
5. **Align Vertex Data** for optimal performance

---

**Study Time:** 3-4 hours
