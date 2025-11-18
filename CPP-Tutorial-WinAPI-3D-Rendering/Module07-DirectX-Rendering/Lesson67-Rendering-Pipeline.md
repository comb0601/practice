# Lesson 67: Rendering Pipeline

## Table of Contents
1. [Pipeline Overview](#pipeline-overview)
2. [Input Assembler Stage](#input-assembler-stage)
3. [Vertex Shader Stage](#vertex-shader-stage)
4. [Tessellation Stages](#tessellation-stages)
5. [Geometry Shader Stage](#geometry-shader-stage)
6. [Rasterizer Stage](#rasterizer-stage)
7. [Pixel Shader Stage](#pixel-shader-stage)
8. [Output Merger Stage](#output-merger-stage)

## Pipeline Overview

The Direct3D 11 rendering pipeline transforms 3D geometry into 2D pixels through multiple stages:

```
Vertex Data
    ↓
[Input Assembler] ←─ Vertex/Index Buffers
    ↓
[Vertex Shader] ←─── Constant Buffers
    ↓
[Hull Shader] (optional)
    ↓
[Tessellator] (optional)
    ↓
[Domain Shader] (optional)
    ↓
[Geometry Shader] (optional)
    ↓
[Rasterizer] ←────── Rasterizer State
    ↓
[Pixel Shader] ←───── Textures, Samplers
    ↓
[Output Merger] ←──── Blend State, Depth-Stencil State
    ↓
Render Target
```

### Programmable vs Fixed-Function Stages

**Programmable (Shader) Stages:**
- Vertex Shader (required)
- Hull Shader (optional)
- Domain Shader (optional)
- Geometry Shader (optional)
- Pixel Shader (required for color output)

**Fixed-Function Stages:**
- Input Assembler
- Tessellator
- Rasterizer
- Output Merger

## Input Assembler Stage

**Purpose:** Reads vertex and index data from buffers and assembles primitives.

### Primitive Topology

```cpp
// Set topology
context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
```

**Common Topologies:**
- **POINTLIST**: Individual points
- **LINELIST**: Separate line segments
- **LINESTRIP**: Connected lines
- **TRIANGLELIST**: Separate triangles (most common)
- **TRIANGLESTRIP**: Connected triangles

### Vertex Buffer Binding

```cpp
UINT stride = sizeof(Vertex);
UINT offset = 0;
context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
```

### Index Buffer Binding

```cpp
context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
```

## Vertex Shader Stage

**Purpose:** Processes each vertex, typically transforming from model space to clip space.

### Minimal Vertex Shader (HLSL)

```hlsl
struct VS_INPUT
{
    float3 position : POSITION;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
};

cbuffer ConstantBuffer : register(b0)
{
    matrix worldViewProj;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), worldViewProj);
    return output;
}
```

### Common Vertex Shader Operations

1. **Position Transformation**
2. **Normal Transformation**
3. **Texture Coordinate Passing**
4. **Color Calculations**
5. **Lighting Calculations**

## Rasterizer Stage

**Purpose:** Converts primitives into pixels, determining which pixels are covered.

### Rasterization Process

```
Triangle (3 vertices)
    ↓
Edge Equations
    ↓
Pixel Coverage Test
    ↓
Attribute Interpolation
    ↓
Fragments (potential pixels)
```

### Rasterizer State

```cpp
D3D11_RASTERIZER_DESC rastDesc = {};
rastDesc.FillMode = D3D11_FILL_SOLID;  // or WIREFRAME
rastDesc.CullMode = D3D11_CULL_BACK;   // Back-face culling
rastDesc.FrontCounterClockwise = FALSE;
rastDesc.DepthClipEnable = TRUE;

ID3D11RasterizerState* rastState;
device->CreateRasterizerState(&rastDesc, &rastState);
context->RSSetState(rastState);
```

## Pixel Shader Stage

**Purpose:** Calculates the final color of each pixel.

### Basic Pixel Shader

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

## Output Merger Stage

**Purpose:** Combines pixel shader output with render targets, applying blending and depth testing.

### Depth-Stencil Testing

```cpp
D3D11_DEPTH_STENCIL_DESC dsDesc = {};
dsDesc.DepthEnable = TRUE;
dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

ID3D11DepthStencilState* dsState;
device->CreateDepthStencilState(&dsDesc, &dsState);
context->OMSetDepthStencilState(dsState, 0);
```

### Blend State

```cpp
D3D11_BLEND_DESC blendDesc = {};
blendDesc.RenderTarget[0].BlendEnable = TRUE;
blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

ID3D11BlendState* blendState;
device->CreateBlendState(&blendDesc, &blendState);
context->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
```

## Data Flow Example

### Complete Pipeline Data Flow

```
1. Vertex Buffer: [(x,y,z), (r,g,b)]
         ↓
2. Input Assembler: Assembles vertices into triangles
         ↓
3. Vertex Shader: Transforms position, passes color
         ↓
4. Rasterizer: Determines covered pixels, interpolates attributes
         ↓
5. Pixel Shader: Receives interpolated color, outputs final color
         ↓
6. Output Merger: Writes to render target (if depth test passes)
         ↓
7. Back Buffer: Final image
```

## Performance Considerations

### Early-Z Optimization

Modern GPUs perform depth testing before pixel shader when possible:
- Reduces pixel shader invocations
- Requires depth writes enabled
- Works best with opaque geometry front-to-back

### State Changes

Minimize state changes for better performance:
```cpp
// Good: Group by state
for (mesh in opaqueObjects)
    DrawMesh(mesh);

for (mesh in transparentObjects)
    DrawMesh(mesh);

// Bad: Constant state switching
for (object in allObjects)
    SetState(object.state);
    Draw(object);
```

---

**Study Time:** 4-5 hours
