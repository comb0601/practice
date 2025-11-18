# Lesson 47: HLSL Shaders - Vertex and Pixel Shaders

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to HLSL
2. Shader Compilation
3. Vertex Shaders
4. Pixel Shaders
5. Semantics and Input/Output
6. Shader Resources
7. Complete Shader Pipeline
8. Working Examples

---

## 1. Introduction to HLSL

**HLSL** (High-Level Shading Language) is Microsoft's shading language for DirectX.

### Basic HLSL Syntax:

```hlsl
// Variable declaration
float value;
float4 color;
float3x3 matrix;

// Structures
struct VS_INPUT {
    float3 position : POSITION;
    float4 color : COLOR;
};

// Functions
float4 main(VS_INPUT input) : SV_TARGET {
    return input.color;
}
```

### HLSL Data Types:

| Type | Description | Size |
|------|-------------|------|
| float | 32-bit floating point | 4 bytes |
| float2 | 2D vector | 8 bytes |
| float3 | 3D vector | 12 bytes |
| float4 | 4D vector | 16 bytes |
| float4x4 | 4x4 matrix | 64 bytes |
| int | 32-bit integer | 4 bytes |
| uint | 32-bit unsigned | 4 bytes |
| bool | Boolean | 4 bytes |

---

## 2. Shader Compilation

### Compile Shader from File:

```cpp
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

class ShaderCompiler {
public:
    static ID3D11VertexShader* CompileVertexShader(
        ID3D11Device* device,
        const wchar_t* filename,
        const char* entryPoint,
        ID3DBlob** outBlob = nullptr)
    {
        ID3DBlob* shaderBlob = nullptr;
        ID3DBlob* errorBlob = nullptr;

        HRESULT hr = D3DCompileFromFile(
            filename,
            nullptr,                // Defines
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entryPoint,
            "vs_5_0",               // Shader model
            D3DCOMPILE_ENABLE_STRICTNESS,
            0,
            &shaderBlob,
            &errorBlob
        );

        if (FAILED(hr)) {
            if (errorBlob) {
                OutputDebugStringA((char*)errorBlob->GetBufferPointer());
                errorBlob->Release();
            }
            return nullptr;
        }

        ID3D11VertexShader* shader = nullptr;
        hr = device->CreateVertexShader(
            shaderBlob->GetBufferPointer(),
            shaderBlob->GetBufferSize(),
            nullptr,
            &shader
        );

        if (outBlob) {
            *outBlob = shaderBlob;
        } else {
            shaderBlob->Release();
        }

        return shader;
    }

    static ID3D11PixelShader* CompilePixelShader(
        ID3D11Device* device,
        const wchar_t* filename,
        const char* entryPoint)
    {
        ID3DBlob* shaderBlob = nullptr;
        ID3DBlob* errorBlob = nullptr;

        HRESULT hr = D3DCompileFromFile(
            filename,
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entryPoint,
            "ps_5_0",
            D3DCOMPILE_ENABLE_STRICTNESS,
            0,
            &shaderBlob,
            &errorBlob
        );

        if (FAILED(hr)) {
            if (errorBlob) {
                OutputDebugStringA((char*)errorBlob->GetBufferPointer());
                errorBlob->Release();
            }
            return nullptr;
        }

        ID3D11PixelShader* shader = nullptr;
        hr = device->CreatePixelShader(
            shaderBlob->GetBufferPointer(),
            shaderBlob->GetBufferSize(),
            nullptr,
            &shader
        );

        shaderBlob->Release();
        return shader;
    }

    // Compile from memory string
    static ID3D11VertexShader* CompileVertexShaderFromString(
        ID3D11Device* device,
        const char* shaderCode,
        const char* entryPoint,
        ID3DBlob** outBlob = nullptr)
    {
        ID3DBlob* shaderBlob = nullptr;
        ID3DBlob* errorBlob = nullptr;

        HRESULT hr = D3DCompile(
            shaderCode,
            strlen(shaderCode),
            nullptr,
            nullptr,
            nullptr,
            entryPoint,
            "vs_5_0",
            D3DCOMPILE_ENABLE_STRICTNESS,
            0,
            &shaderBlob,
            &errorBlob
        );

        if (FAILED(hr)) {
            if (errorBlob) {
                OutputDebugStringA((char*)errorBlob->GetBufferPointer());
                errorBlob->Release();
            }
            return nullptr;
        }

        ID3D11VertexShader* shader = nullptr;
        device->CreateVertexShader(
            shaderBlob->GetBufferPointer(),
            shaderBlob->GetBufferSize(),
            nullptr,
            &shader
        );

        if (outBlob) {
            *outBlob = shaderBlob;
        } else {
            shaderBlob->Release();
        }

        return shader;
    }
};
```

---

## 3. Vertex Shaders

### Basic Vertex Shader:

```hlsl
// VertexShader.hlsl
struct VS_INPUT {
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_OUTPUT main(VS_INPUT input) {
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.color = input.color;
    return output;
}
```

### Vertex Shader with Transformations:

```hlsl
// TransformVertexShader.hlsl
cbuffer ConstantBuffer : register(b0) {
    float4x4 world;
    float4x4 view;
    float4x4 projection;
};

struct VS_INPUT {
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT {
    float4 position : SV_POSITION;
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input) {
    VS_OUTPUT output;
    
    // Transform position
    float4 worldPos = mul(float4(input.position, 1.0f), world);
    float4 viewPos = mul(worldPos, view);
    output.position = mul(viewPos, projection);
    
    output.worldPos = worldPos.xyz;
    output.normal = mul(input.normal, (float3x3)world);
    output.texCoord = input.texCoord;
    
    return output;
}
```

---

## 4. Pixel Shaders

### Basic Pixel Shader:

```hlsl
// PixelShader.hlsl
struct PS_INPUT {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 main(PS_INPUT input) : SV_TARGET {
    return input.color;
}
```

### Textured Pixel Shader:

```hlsl
// TexturedPixelShader.hlsl
Texture2D diffuseTexture : register(t0);
SamplerState samplerState : register(s0);

struct PS_INPUT {
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET {
    return diffuseTexture.Sample(samplerState, input.texCoord);
}
```

### Lit Pixel Shader (Phong):

```hlsl
// LitPixelShader.hlsl
cbuffer LightBuffer : register(b0) {
    float3 lightDirection;
    float padding1;
    float4 lightColor;
    float4 ambientColor;
    float3 cameraPosition;
    float specularPower;
};

Texture2D diffuseTexture : register(t0);
SamplerState samplerState : register(s0);

struct PS_INPUT {
    float4 position : SV_POSITION;
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET {
    // Normalize interpolated normal
    float3 normal = normalize(input.normal);
    
    // Ambient
    float4 ambient = ambientColor;
    
    // Diffuse
    float3 lightDir = normalize(-lightDirection);
    float diffuseFactor = max(dot(normal, lightDir), 0.0f);
    float4 diffuse = diffuseFactor * lightColor;
    
    // Specular
    float3 viewDir = normalize(cameraPosition - input.worldPos);
    float3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), specularPower);
    float4 specular = specularFactor * lightColor;
    
    // Sample texture
    float4 textureColor = diffuseTexture.Sample(samplerState, input.texCoord);
    
    // Final color
    float4 finalColor = (ambient + diffuse) * textureColor + specular;
    finalColor.a = textureColor.a;
    
    return finalColor;
}
```

---

## 5. Semantics and Input/Output

### System Value Semantics:

| Semantic | Description |
|----------|-------------|
| SV_POSITION | Transformed vertex position |
| SV_TARGET | Render target output |
| SV_VertexID | Vertex ID |
| SV_InstanceID | Instance ID |
| SV_PrimitiveID | Primitive ID |
| SV_Depth | Depth value |

### User-Defined Semantics:

| Semantic | Description |
|----------|-------------|
| POSITION | Position |
| NORMAL | Normal vector |
| TEXCOORD | Texture coordinate |
| COLOR | Color |
| TANGENT | Tangent vector |
| BINORMAL | Binormal vector |

---

## 6. Shader Resources

### Constant Buffers:

```hlsl
cbuffer PerFrame : register(b0) {
    float4x4 viewProjection;
    float3 cameraPosition;
    float time;
};

cbuffer PerObject : register(b1) {
    float4x4 world;
    float4 color;
};
```

### Textures and Samplers:

```hlsl
Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D specularMap : register(t2);

SamplerState linearSampler : register(s0);
SamplerState pointSampler : register(s1);

float4 main(PS_INPUT input) : SV_TARGET {
    float4 diffuse = diffuseMap.Sample(linearSampler, input.texCoord);
    float3 normal = normalMap.Sample(linearSampler, input.texCoord).xyz;
    float specular = specularMap.Sample(linearSampler, input.texCoord).r;
    
    // Use sampled values...
    return diffuse;
}
```

---

## 7. Complete Shader Pipeline

### C++ Shader Manager:

```cpp
class ShaderManager {
public:
    bool Initialize(ID3D11Device* device) {
        // Compile vertex shader
        ID3DBlob* vsBlob = nullptr;
        m_vertexShader = ShaderCompiler::CompileVertexShader(
            device,
            L"shaders/VertexShader.hlsl",
            "main",
            &vsBlob
        );

        if (!m_vertexShader) return false;

        // Create input layout
        D3D11_INPUT_ELEMENT_DESC layout[] = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
             D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
             D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24,
             D3D11_INPUT_PER_VERTEX_DATA, 0}
        };

        HRESULT hr = device->CreateInputLayout(
            layout,
            ARRAYSIZE(layout),
            vsBlob->GetBufferPointer(),
            vsBlob->GetBufferSize(),
            &m_inputLayout
        );

        vsBlob->Release();
        if (FAILED(hr)) return false;

        // Compile pixel shader
        m_pixelShader = ShaderCompiler::CompilePixelShader(
            device,
            L"shaders/PixelShader.hlsl",
            "main"
        );

        return m_pixelShader != nullptr;
    }

    void Bind(ID3D11DeviceContext* context) {
        context->IASetInputLayout(m_inputLayout);
        context->VSSetShader(m_vertexShader, nullptr, 0);
        context->PSSetShader(m_pixelShader, nullptr, 0);
    }

    void Release() {
        if (m_inputLayout) m_inputLayout->Release();
        if (m_vertexShader) m_vertexShader->Release();
        if (m_pixelShader) m_pixelShader->Release();
    }

private:
    ID3D11InputLayout* m_inputLayout = nullptr;
    ID3D11VertexShader* m_vertexShader = nullptr;
    ID3D11PixelShader* m_pixelShader = nullptr;
};
```

---

## 8. Working Examples

### Complete Shader Example:

**Vertex Shader (VertexShader.hlsl):**
```hlsl
cbuffer MVP : register(b0) {
    float4x4 world;
    float4x4 view;
    float4x4 projection;
};

struct VS_INPUT {
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT {
    float4 position : SV_POSITION;
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input) {
    VS_OUTPUT output;
    
    float4 worldPos = mul(float4(input.position, 1.0f), world);
    float4 viewPos = mul(worldPos, view);
    output.position = mul(viewPos, projection);
    
    output.worldPos = worldPos.xyz;
    output.normal = mul(input.normal, (float3x3)world);
    output.texCoord = input.texCoord;
    
    return output;
}
```

**Pixel Shader (PixelShader.hlsl):**
```hlsl
cbuffer LightData : register(b0) {
    float3 lightDirection;
    float padding;
    float4 lightColor;
    float3 cameraPosition;
    float specularPower;
};

Texture2D diffuseTexture : register(t0);
SamplerState samplerState : register(s0);

struct PS_INPUT {
    float4 position : SV_POSITION;
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET {
    float3 normal = normalize(input.normal);
    float3 lightDir = normalize(-lightDirection);
    
    // Diffuse
    float diffuse = max(dot(normal, lightDir), 0.0f);
    
    // Specular
    float3 viewDir = normalize(cameraPosition - input.worldPos);
    float3 halfDir = normalize(lightDir + viewDir);
    float specular = pow(max(dot(normal, halfDir), 0.0f), specularPower);
    
    // Sample texture
    float4 texColor = diffuseTexture.Sample(samplerState, input.texCoord);
    
    // Combine
    float4 finalColor = texColor * (0.2f + 0.8f * diffuse) * lightColor;
    finalColor.rgb += specular * 0.5f;
    finalColor.a = texColor.a;
    
    return finalColor;
}
```

---

## Key Takeaways

1. ✅ HLSL is DirectX's shading language
2. ✅ Vertex shaders transform vertices
3. ✅ Pixel shaders calculate pixel colors
4. ✅ Compile shaders from files or strings
5. ✅ Semantics bind shader inputs/outputs
6. ✅ Constant buffers pass data to shaders
7. ✅ Textures sample images
8. ✅ SamplerStates control texture filtering
9. ✅ Input layouts match vertex structure
10. ✅ Always check compilation errors

---

## Next Lesson Preview

In **Lesson 48: Constant Buffers**, we'll explore:
- Creating constant buffers
- Updating buffer data
- Buffer alignment rules
- Multiple constant buffers
- Per-frame vs per-object buffers
- Performance optimization

---

## Summary Checklist

Before moving to Lesson 48, ensure you can:
- [ ] Write basic vertex shaders
- [ ] Write basic pixel shaders
- [ ] Compile shaders from files
- [ ] Create input layouts
- [ ] Use semantics correctly
- [ ] Bind shaders to pipeline
- [ ] Sample textures in shaders
- [ ] Implement basic lighting

**Excellent work on completing Lesson 47!**

You now understand HLSL and can write custom shaders!
