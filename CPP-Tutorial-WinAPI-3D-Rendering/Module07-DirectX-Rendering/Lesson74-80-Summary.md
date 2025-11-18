# Lessons 74-80: DirectX Rendering Advanced Topics

## Lesson 74: Texture Sampling & Filtering

### Filter Types
- Point: Nearest neighbor
- Linear: Bilinear filtering
- Anisotropic: Best quality, performance cost

### Mipmap Levels
```cpp
D3D11_SAMPLER_DESC samplerDesc = {};
samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
samplerDesc.MaxAnisotropy = 16;
samplerDesc.MinLOD = 0;
samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
```

## Lesson 75: Basic Lighting (Phong Model)

### Lighting Components
```hlsl
float4 ambient = ambientColor;
float4 diffuse = saturate(dot(normal, -lightDir)) * lightColor;
float3 reflection = reflect(lightDir, normal);
float4 specular = pow(saturate(dot(reflection, viewDir)), shininess) * specularColor;
float4 finalColor = ambient + diffuse + specular;
```

## Lesson 76: Multiple Light Sources

```hlsl
for (int i = 0; i < numLights; i++)
{
    float3 lightDir = normalize(lights[i].position - worldPos);
    float distance = length(lights[i].position - worldPos);
    float attenuation = 1.0 / (distance * distance);
    finalColor += CalculateLight(lights[i], lightDir) * attenuation;
}
```

## Lesson 77: Normal Mapping

```hlsl
// Sample normal map
float3 normalMap = normalTexture.Sample(sampler, input.texcoord).rgb * 2.0 - 1.0;

// Transform to world space using TBN matrix
float3x3 TBN = float3x3(input.tangent, input.bitangent, input.normal);
float3 normal = normalize(mul(normalMap, TBN));
```

## Lesson 78: Blending & Transparency

```cpp
D3D11_BLEND_DESC blendDesc = {};
blendDesc.RenderTarget[0].BlendEnable = TRUE;
blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
```

## Lesson 79: Depth & Stencil Testing

```cpp
D3D11_DEPTH_STENCIL_DESC dsDesc = {};
dsDesc.DepthEnable = TRUE;
dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
```

## Lesson 80: Loading 3D Models

### OBJ File Parsing
```cpp
struct Vertex {
    XMFLOAT3 position;
    XMFLOAT3 normal;
    XMFLOAT2 texcoord;
};

vector<Vertex> vertices;
vector<UINT> indices;

// Parse OBJ file
ifstream file("model.obj");
string line;
while (getline(file, line)) {
    if (line[0] == 'v' && line[1] == ' ') {
        // Parse vertex position
    } else if (line[0] == 'v' && line[1] == 'n') {
        // Parse normal
    } else if (line[0] == 'v' && line[1] == 't') {
        // Parse texture coordinate
    } else if (line[0] == 'f') {
        // Parse face
    }
}
```

---

**Each lesson includes detailed theory, HLSL shaders, C++ implementation, and 5-7 complete example programs in the Code/ subdirectories.**

**Total Study Time (Lessons 74-80):** 25-30 hours
