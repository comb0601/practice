# Lesson 73: Texture Mapping

## Texture Basics

Textures map 2D images onto 3D geometry using UV coordinates.

## Loading Textures from File

```cpp
#include <DDSTextureLoader.h>

ID3D11ShaderResourceView* textureView;
DirectX::CreateDDSTextureFromFile(device, L"texture.dds", nullptr, &textureView);

context->PSSetShaderResources(0, 1, &textureView);
```

## Creating Texture from Memory

```cpp
D3D11_TEXTURE2D_DESC texDesc = {};
texDesc.Width = 256;
texDesc.Height = 256;
texDesc.MipLevels = 1;
texDesc.ArraySize = 1;
texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
texDesc.SampleDesc.Count = 1;
texDesc.Usage = D3D11_USAGE_DEFAULT;
texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

ID3D11Texture2D* texture;
device->CreateTexture2D(&texDesc, nullptr, &texture);

D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
srvDesc.Format = texDesc.Format;
srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
srvDesc.Texture2D.MipLevels = 1;

ID3D11ShaderResourceView* srv;
device->CreateShaderResourceView(texture, &srvDesc, &srv);
```

## Sampler States

```cpp
D3D11_SAMPLER_DESC samplerDesc = {};
samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
samplerDesc.MaxAnisotropy = 1;
samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
samplerDesc.MinLOD = 0;
samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

ID3D11SamplerState* sampler;
device->CreateSamplerState(&samplerDesc, &sampler);

context->PSSetSamplers(0, 1, &sampler);
```

## Texture Sampling in Shader

```hlsl
Texture2D shaderTexture : register(t0);
SamplerState samplerState : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    return shaderTexture.Sample(samplerState, input.texcoord);
}
```

---

**Study Time:** 4 hours
