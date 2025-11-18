# Lesson 71: Pixel Shaders

## Pixel Shader Fundamentals

Pixel shaders calculate the final color of each pixel. They receive interpolated data from the vertex shader and output colors to render targets.

## Basic Pixel Shader

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

## Texture Sampling

```hlsl
Texture2D shaderTexture : register(t0);
SamplerState samplerState : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return shaderTexture.Sample(samplerState, input.texcoord);
}
```

## Lighting Calculations

```hlsl
cbuffer LightBuffer : register(b0)
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    // Normalize interpolated normal
    float3 normal = normalize(input.normal);

    // Calculate diffuse lighting
    float diffuseIntensity = saturate(dot(normal, -lightDirection));

    // Sample texture
    float4 textureColor = shaderTexture.Sample(samplerState, input.texcoord);

    // Combine lighting
    float4 finalColor = ambientColor + (diffuseColor * diffuseIntensity);
    finalColor = saturate(finalColor);

    return finalColor * textureColor;
}
```

## Multiple Render Targets (MRT)

```hlsl
struct PS_OUTPUT
{
    float4 color : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 position : SV_TARGET2;
};

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    output.color = shaderTexture.Sample(samplerState, input.texcoord);
    output.normal = float4(normalize(input.normal), 1.0f);
    output.position = float4(input.worldPos, 1.0f);
    return output;
}
```

## Discarding Pixels

```hlsl
float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = shaderTexture.Sample(samplerState, input.texcoord);

    // Discard transparent pixels
    if (color.a < 0.1f)
        discard;

    return color;
}
```

---

**Study Time:** 3-4 hours
