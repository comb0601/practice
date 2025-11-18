# Module 8: Advanced 3D Rendering (Lessons 81-90)

## Lesson 81: Shadow Mapping

### Shadow Map Creation
```hlsl
// First pass: Render from light's perspective to depth texture
float4 PS_ShadowMap(VS_OUTPUT input) : SV_TARGET
{
    return float4(input.position.z, 0, 0, 1);
}
```

### Shadow Sampling
```hlsl
float shadow = shadowMap.SampleCmpLevelZero(shadowSampler, projectedTexCoord, depth);
float4 finalColor = (ambient + diffuse * shadow) * textureColor;
```

### PCF (Percentage Closer Filtering)
```hlsl
float shadow = 0.0;
for (int x = -1; x <= 1; x++)
{
    for (int y = -1; y <= 1; y++)
    {
        float2 offset = float2(x, y) * texelSize;
        shadow += shadowMap.SampleCmpLevelZero(shadowSampler, texCoord + offset, depth);
    }
}
shadow /= 9.0;
```

## Lesson 82: Deferred Rendering

### G-Buffer Layout
```cpp
// Render Target 0: Albedo (RGB) + Specular (A)
// Render Target 1: Normal (RGB) + Roughness (A)
// Render Target 2: Position (RGB) + Metallic (A)
// Render Target 3: Emission (RGB)
```

### Geometry Pass
```hlsl
struct PS_OUTPUT
{
    float4 albedo : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 position : SV_TARGET2;
};

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    output.albedo = albedoTexture.Sample(sampler, input.texcoord);
    output.normal = float4(normalize(input.normal), input.roughness);
    output.position = float4(input.worldPos, input.metallic);
    return output;
}
```

### Lighting Pass
```hlsl
// Sample G-Buffer
float4 albedo = albedoBuffer.Sample(sampler, texCoord);
float3 normal = normalBuffer.Sample(sampler, texCoord).rgb;
float3 position = positionBuffer.Sample(sampler, texCoord).rgb;

// Calculate lighting for each light
float3 finalColor = 0;
for (int i = 0; i < numLights; i++)
{
    finalColor += CalculateLight(lights[i], position, normal, albedo);
}
```

## Lesson 83: Post-Processing Effects

### Render to Texture
```cpp
D3D11_TEXTURE2D_DESC texDesc = {};
texDesc.Width = screenWidth;
texDesc.Height = screenHeight;
texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;  // HDR format
texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

device->CreateTexture2D(&texDesc, nullptr, &renderTexture);
```

### Full-Screen Quad
```cpp
Vertex quad[] = {
    { -1.0f,  1.0f, 0.0f,  0.0f, 0.0f },  // Top-left
    {  1.0f,  1.0f, 0.0f,  1.0f, 0.0f },  // Top-right
    { -1.0f, -1.0f, 0.0f,  0.0f, 1.0f },  // Bottom-left
    {  1.0f, -1.0f, 0.0f,  1.0f, 1.0f }   // Bottom-right
};
```

### Post-Process Chain
```hlsl
// Bloom + Tone Mapping + Gamma Correction
float3 color = sceneTexture.Sample(sampler, texCoord).rgb;
float3 bloom = bloomTexture.Sample(sampler, texCoord).rgb;
color += bloom;
color = ACESFilm(color);  // Tone mapping
color = pow(color, 1.0/2.2);  // Gamma correction
```

## Lesson 84: Bloom & HDR

### Bloom Extraction
```hlsl
float4 main(PS_INPUT input) : SV_TARGET
{
    float3 color = sceneTexture.Sample(sampler, input.texcoord).rgb;
    float brightness = dot(color, float3(0.2126, 0.7152, 0.0722));
    if (brightness > threshold)
        return float4(color, 1.0);
    else
        return float4(0, 0, 0, 1);
}
```

### Gaussian Blur
```hlsl
float weights[5] = { 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 };
float3 result = texture.Sample(sampler, texCoord).rgb * weights[0];

for(int i = 1; i < 5; i++)
{
    result += texture.Sample(sampler, texCoord + offset * i).rgb * weights[i];
    result += texture.Sample(sampler, texCoord - offset * i).rgb * weights[i];
}
```

### Tone Mapping (ACES)
```hlsl
float3 ACESFilm(float3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return saturate((x*(a*x+b))/(x*(c*x+d)+e));
}
```

## Lesson 85: Screen Space Ambient Occlusion (SSAO)

### SSAO Algorithm
```hlsl
float occlusion = 0.0;
for (int i = 0; i < numSamples; i++)
{
    float3 samplePos = position + samples[i] * radius;
    float4 offset = mul(float4(samplePos, 1.0), projection);
    offset.xy /= offset.w;
    offset.xy = offset.xy * 0.5 + 0.5;

    float sampleDepth = depthTexture.Sample(sampler, offset.xy).r;

    float rangeCheck = abs(position.z - sampleDepth) < radius ? 1.0 : 0.0;
    occlusion += (sampleDepth >= samplePos.z ? 1.0 : 0.0) * rangeCheck;
}

occlusion = 1.0 - (occlusion / numSamples);
```

## Lesson 86: Particle Systems

### GPU Particle Update
```hlsl
struct Particle
{
    float3 position;
    float3 velocity;
    float life;
    float size;
};

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    Particle p = particles[DTid.x];

    p.velocity += gravity * deltaTime;
    p.position += p.velocity * deltaTime;
    p.life -= deltaTime;

    if (p.life <= 0)
    {
        // Respawn particle
        p.position = emitterPosition;
        p.velocity = RandomDirection() * initialSpeed;
        p.life = maxLife;
    }

    particles[DTid.x] = p;
}
```

## Lesson 87: Terrain Rendering

### Heightmap Vertex Shader
```hlsl
VS_OUTPUT main(VS_INPUT input)
{
    float height = heightmap.SampleLevel(sampler, input.texcoord, 0).r;
    float3 position = float3(input.position.x, height * maxHeight, input.position.z);

    VS_OUTPUT output;
    output.position = mul(float4(position, 1.0), worldViewProj);
    output.worldPos = position;
    return output;
}
```

### Tessellation for LOD
```hlsl
float CalculateTessFactor(float distance)
{
    float tessFactor = saturate((maxDistance - distance) / (maxDistance - minDistance));
    return lerp(minTess, maxTess, tessFactor);
}
```

## Lesson 88: Skybox & Environment Mapping

### Skybox Rendering
```hlsl
TextureCube skyboxTexture : register(t0);

float4 main(PS_INPUT input) : SV_TARGET
{
    return skyboxTexture.Sample(sampler, input.position.xyz);
}
```

### Reflection Mapping
```hlsl
float3 viewDir = normalize(cameraPos - input.worldPos);
float3 reflection = reflect(-viewDir, input.normal);
float4 reflectionColor = environmentMap.Sample(sampler, reflection);
```

## Lesson 89: Skeletal Animation

### Vertex Skinning
```hlsl
struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    uint4 boneIndices : BLENDINDICES;
    float4 boneWeights : BLENDWEIGHT;
};

VS_OUTPUT main(VS_INPUT input)
{
    float4 pos = 0;
    float3 normal = 0;

    for (int i = 0; i < 4; i++)
    {
        uint boneIndex = input.boneIndices[i];
        float weight = input.boneWeights[i];

        pos += weight * mul(float4(input.position, 1.0), boneMatrices[boneIndex]);
        normal += weight * mul(input.normal, (float3x3)boneMatrices[boneIndex]);
    }

    VS_OUTPUT output;
    output.position = mul(pos, viewProj);
    output.normal = normalize(normal);
    return output;
}
```

## Lesson 90: Physically Based Rendering (PBR)

### PBR Lighting Model
```hlsl
float3 F0 = lerp(0.04, albedo, metallic);  // Base reflectivity

// Cook-Torrance BRDF
float NDF = DistributionGGX(normal, halfway, roughness);
float G = GeometrySmith(normal, viewDir, lightDir, roughness);
float3 F = FresnelSchlick(max(dot(halfway, viewDir), 0.0), F0);

float3 numerator = NDF * G * F;
float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0);
float3 specular = numerator / max(denominator, 0.001);

float3 kD = (1.0 - F) * (1.0 - metallic);
float3 diffuse = kD * albedo / PI;

float NdotL = max(dot(normal, lightDir), 0.0);
float3 Lo = (diffuse + specular) * radiance * NdotL;
```

### Image Based Lighting (IBL)
```hlsl
// Diffuse irradiance
float3 irradiance = irradianceMap.Sample(sampler, normal).rgb;
float3 diffuse = irradiance * albedo;

// Specular reflection
float3 R = reflect(-viewDir, normal);
float3 prefilteredColor = prefilterMap.SampleLevel(sampler, R, roughness * MAX_LOD).rgb;
float2 brdf = brdfLUT.Sample(sampler, float2(NdotV, roughness)).rg;
float3 specular = prefilteredColor * (F * brdf.x + brdf.y);

float3 ambient = (kD * diffuse + specular) * ao;
```

---

## Study Plan

**Module 8 Total Study Time:** 50-60 hours

- Lesson 81 (Shadow Mapping): 6 hours
- Lesson 82 (Deferred Rendering): 7 hours
- Lesson 83 (Post-Processing): 5 hours
- Lesson 84 (Bloom & HDR): 5 hours
- Lesson 85 (SSAO): 6 hours
- Lesson 86 (Particles): 5 hours
- Lesson 87 (Terrain): 6 hours
- Lesson 88 (Skybox): 4 hours
- Lesson 89 (Animation): 7 hours
- Lesson 90 (PBR): 8 hours

**Each lesson includes:**
- Detailed markdown theory (4-5 hours of content)
- Complete C++ DirectX applications (5-10 programs)
- HLSL shader implementations
- Real-world examples and exercises

---

**End of Module 8**

**Complete Course Total:** 150-180 hours of study content
