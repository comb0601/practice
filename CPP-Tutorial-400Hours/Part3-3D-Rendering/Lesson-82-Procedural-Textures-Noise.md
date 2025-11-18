# Lesson 82: Procedural Textures and Noise

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Procedural Textures
2. Noise Functions Overview
3. Perlin Noise Implementation
4. Simplex Noise
5. Worley (Cellular) Noise
6. Fractal Brownian Motion (fBm)
7. Texture Synthesis Techniques
8. Procedural Materials
9. 3D Noise for Volumetric Effects
10. GPU-Based Noise Generation
11. Complete Implementation
12. Exercises

---

## 1. Introduction to Procedural Textures

Procedural textures are generated algorithmically rather than stored as image files. This approach offers several advantages:

### Benefits of Procedural Textures

- **Memory Efficiency**: No texture memory required
- **Resolution Independence**: Infinite detail at any zoom level
- **Parameterization**: Easy real-time adjustments
- **No Repetition**: Seamless, non-repeating patterns
- **3D Coherence**: Solid textures work in 3D space

### Common Applications

- Terrain generation (heightmaps)
- Marble and wood patterns
- Clouds and fog
- Organic surfaces
- Displacement mapping
- Animated effects

---

## 2. Noise Functions Overview

Noise functions are the foundation of procedural texture generation. They produce pseudo-random values with specific characteristics.

### Types of Noise

1. **Value Noise**: Interpolates random values at grid points
2. **Gradient Noise (Perlin)**: Uses gradient vectors for smoother results
3. **Simplex Noise**: Optimized version of Perlin noise
4. **Worley Noise**: Distance-based cellular patterns
5. **Wavelet Noise**: Frequency-band limited noise

### Noise Properties

```cpp
// Ideal noise characteristics:
// - Continuous: smooth transitions
// - Repeatable: same input = same output
// - Controllable: frequency, amplitude, octaves
// - Efficient: fast computation on GPU
```

---

## 3. Perlin Noise Implementation

Perlin noise is the most widely used noise function, invented by Ken Perlin in 1983.

### Perlin Noise C++ Implementation

```cpp
// PerlinNoise.h
#pragma once
#include <vector>
#include <random>
#include <DirectXMath.h>

using namespace DirectX;

class PerlinNoise
{
public:
    PerlinNoise(unsigned int seed = 0);

    // 1D, 2D, and 3D Perlin noise functions
    float Noise1D(float x) const;
    float Noise2D(float x, float y) const;
    float Noise3D(float x, float y, float z) const;

    // Fractal Brownian Motion (multiple octaves)
    float FBM2D(float x, float y, int octaves = 6, float lacunarity = 2.0f, float gain = 0.5f) const;
    float FBM3D(float x, float y, float z, int octaves = 6, float lacunarity = 2.0f, float gain = 0.5f) const;

    // Turbulence (absolute value of noise)
    float Turbulence2D(float x, float y, int octaves = 6) const;

private:
    std::vector<int> m_permutation;

    float Fade(float t) const;
    float Lerp(float a, float b, float t) const;
    float Grad(int hash, float x, float y, float z) const;
};

// PerlinNoise.cpp
#include "PerlinNoise.h"
#include <algorithm>
#include <numeric>

PerlinNoise::PerlinNoise(unsigned int seed)
{
    // Initialize permutation table
    m_permutation.resize(256);
    std::iota(m_permutation.begin(), m_permutation.end(), 0);

    // Shuffle using seed
    std::default_random_engine engine(seed);
    std::shuffle(m_permutation.begin(), m_permutation.end(), engine);

    // Duplicate for easy wrapping
    m_permutation.insert(m_permutation.end(), m_permutation.begin(), m_permutation.end());
}

float PerlinNoise::Fade(float t) const
{
    // 6t^5 - 15t^4 + 10t^3 (smootherstep)
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float PerlinNoise::Lerp(float a, float b, float t) const
{
    return a + t * (b - a);
}

float PerlinNoise::Grad(int hash, float x, float y, float z) const
{
    // Convert low 4 bits of hash into gradient vector
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float PerlinNoise::Noise3D(float x, float y, float z) const
{
    // Find unit cube containing point
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;
    int Z = static_cast<int>(std::floor(z)) & 255;

    // Find relative position in cube
    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);

    // Compute fade curves
    float u = Fade(x);
    float v = Fade(y);
    float w = Fade(z);

    // Hash coordinates of cube corners
    int A = m_permutation[X] + Y;
    int AA = m_permutation[A] + Z;
    int AB = m_permutation[A + 1] + Z;
    int B = m_permutation[X + 1] + Y;
    int BA = m_permutation[B] + Z;
    int BB = m_permutation[B + 1] + Z;

    // Blend results from 8 corners
    float res = Lerp(
        Lerp(
            Lerp(Grad(m_permutation[AA], x, y, z),
                 Grad(m_permutation[BA], x - 1, y, z), u),
            Lerp(Grad(m_permutation[AB], x, y - 1, z),
                 Grad(m_permutation[BB], x - 1, y - 1, z), u), v),
        Lerp(
            Lerp(Grad(m_permutation[AA + 1], x, y, z - 1),
                 Grad(m_permutation[BA + 1], x - 1, y, z - 1), u),
            Lerp(Grad(m_permutation[AB + 1], x, y - 1, z - 1),
                 Grad(m_permutation[BB + 1], x - 1, y - 1, z - 1), u), v), w);

    return res;
}

float PerlinNoise::Noise2D(float x, float y) const
{
    return Noise3D(x, y, 0.0f);
}

float PerlinNoise::Noise1D(float x) const
{
    return Noise3D(x, 0.0f, 0.0f);
}

float PerlinNoise::FBM2D(float x, float y, int octaves, float lacunarity, float gain) const
{
    float sum = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float maxValue = 0.0f;

    for (int i = 0; i < octaves; ++i)
    {
        sum += Noise2D(x * frequency, y * frequency) * amplitude;
        maxValue += amplitude;

        amplitude *= gain;
        frequency *= lacunarity;
    }

    return sum / maxValue; // Normalize to [-1, 1]
}

float PerlinNoise::FBM3D(float x, float y, float z, int octaves, float lacunarity, float gain) const
{
    float sum = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float maxValue = 0.0f;

    for (int i = 0; i < octaves; ++i)
    {
        sum += Noise3D(x * frequency, y * frequency, z * frequency) * amplitude;
        maxValue += amplitude;

        amplitude *= gain;
        frequency *= lacunarity;
    }

    return sum / maxValue;
}

float PerlinNoise::Turbulence2D(float x, float y, int octaves) const
{
    float sum = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;

    for (int i = 0; i < octaves; ++i)
    {
        sum += std::abs(Noise2D(x * frequency, y * frequency)) * amplitude;
        amplitude *= 0.5f;
        frequency *= 2.0f;
    }

    return sum;
}
```

### Perlin Noise Shader Implementation

```hlsl
// PerlinNoise.hlsl

// Permutation table (should match C++ version)
static const int permutation[512] = {
    151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
    8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
    35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
    134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
    55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
    18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
    250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
    189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,
    172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,
    228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
    107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
    // Duplicate for wrapping
    151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
    8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
    35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
    134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
    55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
    18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
    250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
    189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,
    172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,
    228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
    107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

float fade(float t)
{
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

float grad(int hash, float x, float y, float z)
{
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float perlinNoise3D(float3 p)
{
    int3 P = int3(floor(p)) & 255;
    p -= floor(p);

    float3 f = float3(fade(p.x), fade(p.y), fade(p.z));

    int A = permutation[P.x] + P.y;
    int AA = permutation[A] + P.z;
    int AB = permutation[A + 1] + P.z;
    int B = permutation[P.x + 1] + P.y;
    int BA = permutation[B] + P.z;
    int BB = permutation[B + 1] + P.z;

    return lerp(
        lerp(
            lerp(grad(permutation[AA], p.x, p.y, p.z),
                 grad(permutation[BA], p.x - 1, p.y, p.z), f.x),
            lerp(grad(permutation[AB], p.x, p.y - 1, p.z),
                 grad(permutation[BB], p.x - 1, p.y - 1, p.z), f.x), f.y),
        lerp(
            lerp(grad(permutation[AA + 1], p.x, p.y, p.z - 1),
                 grad(permutation[BA + 1], p.x - 1, p.y, p.z - 1), f.x),
            lerp(grad(permutation[AB + 1], p.x, p.y - 1, p.z - 1),
                 grad(permutation[BB + 1], p.x - 1, p.y - 1, p.z - 1), f.x), f.y), f.z);
}

float perlinNoise2D(float2 p)
{
    return perlinNoise3D(float3(p, 0.0));
}

// Fractal Brownian Motion
float fbm2D(float2 p, int octaves, float lacunarity, float gain)
{
    float sum = 0.0;
    float amplitude = 1.0;
    float frequency = 1.0;
    float maxValue = 0.0;

    for(int i = 0; i < octaves; i++)
    {
        sum += perlinNoise2D(p * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= gain;
        frequency *= lacunarity;
    }

    return sum / maxValue;
}

float fbm3D(float3 p, int octaves, float lacunarity, float gain)
{
    float sum = 0.0;
    float amplitude = 1.0;
    float frequency = 1.0;
    float maxValue = 0.0;

    for(int i = 0; i < octaves; i++)
    {
        sum += perlinNoise3D(p * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= gain;
        frequency *= lacunarity;
    }

    return sum / maxValue;
}
```

---

## 4. Simplex Noise

Simplex noise is an improved version of Perlin noise with better gradient distribution and lower computational complexity.

### Simplex Noise Shader

```hlsl
// SimplexNoise.hlsl

// Simplex noise 2D
float simplexNoise2D(float2 v)
{
    const float F2 = 0.366025403; // (sqrt(3) - 1) / 2
    const float G2 = 0.211324865; // (3 - sqrt(3)) / 6

    // Skew input space to determine simplex cell
    float s = (v.x + v.y) * F2;
    int2 i = floor(v + s);

    float t = (i.x + i.y) * G2;
    float2 X0 = i - t;
    float2 x0 = v - X0;

    // Determine which simplex we're in
    int2 i1 = (x0.x > x0.y) ? int2(1, 0) : int2(0, 1);

    // Offsets for corners
    float2 x1 = x0 - i1 + G2;
    float2 x2 = x0 - 1.0 + 2.0 * G2;

    // Permutation and gradient
    i = i & 255;
    int gi0 = permutation[i.x + permutation[i.y]] % 12;
    int gi1 = permutation[i.x + i1.x + permutation[i.y + i1.y]] % 12;
    int gi2 = permutation[i.x + 1 + permutation[i.y + 1]] % 12;

    // Gradient directions (12 directions)
    static const float2 grad2[12] = {
        float2(1, 1), float2(-1, 1), float2(1, -1), float2(-1, -1),
        float2(1, 0), float2(-1, 0), float2(1, 0), float2(-1, 0),
        float2(0, 1), float2(0, -1), float2(0, 1), float2(0, -1)
    };

    // Calculate contribution from each corner
    float n0, n1, n2;

    float t0 = 0.5 - dot(x0, x0);
    if(t0 < 0) n0 = 0.0;
    else
    {
        t0 *= t0;
        n0 = t0 * t0 * dot(grad2[gi0], x0);
    }

    float t1 = 0.5 - dot(x1, x1);
    if(t1 < 0) n1 = 0.0;
    else
    {
        t1 *= t1;
        n1 = t1 * t1 * dot(grad2[gi1], x1);
    }

    float t2 = 0.5 - dot(x2, x2);
    if(t2 < 0) n2 = 0.0;
    else
    {
        t2 *= t2;
        n2 = t2 * t2 * dot(grad2[gi2], x2);
    }

    // Add contributions and scale to [-1, 1]
    return 70.0 * (n0 + n1 + n2);
}

// Simplex noise 3D
float simplexNoise3D(float3 v)
{
    const float F3 = 1.0 / 3.0;
    const float G3 = 1.0 / 6.0;

    float s = (v.x + v.y + v.z) * F3;
    int3 i = floor(v + s);

    float t = (i.x + i.y + i.z) * G3;
    float3 X0 = i - t;
    float3 x0 = v - X0;

    // Determine simplex
    int3 i1, i2;
    if(x0.x >= x0.y)
    {
        if(x0.y >= x0.z) { i1 = int3(1, 0, 0); i2 = int3(1, 1, 0); }
        else if(x0.x >= x0.z) { i1 = int3(1, 0, 0); i2 = int3(1, 0, 1); }
        else { i1 = int3(0, 0, 1); i2 = int3(1, 0, 1); }
    }
    else
    {
        if(x0.y < x0.z) { i1 = int3(0, 0, 1); i2 = int3(0, 1, 1); }
        else if(x0.x < x0.z) { i1 = int3(0, 1, 0); i2 = int3(0, 1, 1); }
        else { i1 = int3(0, 1, 0); i2 = int3(1, 1, 0); }
    }

    float3 x1 = x0 - i1 + G3;
    float3 x2 = x0 - i2 + 2.0 * G3;
    float3 x3 = x0 - 1.0 + 3.0 * G3;

    i = i & 255;

    // Calculate contributions
    float n0, n1, n2, n3;

    float t0 = 0.6 - dot(x0, x0);
    if(t0 < 0) n0 = 0.0;
    else
    {
        t0 *= t0;
        int gi = permutation[i.x + permutation[i.y + permutation[i.z]]] % 12;
        n0 = t0 * t0 * grad(gi, x0.x, x0.y, x0.z);
    }

    float t1 = 0.6 - dot(x1, x1);
    if(t1 < 0) n1 = 0.0;
    else
    {
        t1 *= t1;
        int gi = permutation[i.x + i1.x + permutation[i.y + i1.y + permutation[i.z + i1.z]]] % 12;
        n1 = t1 * t1 * grad(gi, x1.x, x1.y, x1.z);
    }

    float t2 = 0.6 - dot(x2, x2);
    if(t2 < 0) n2 = 0.0;
    else
    {
        t2 *= t2;
        int gi = permutation[i.x + i2.x + permutation[i.y + i2.y + permutation[i.z + i2.z]]] % 12;
        n2 = t2 * t2 * grad(gi, x2.x, x2.y, x2.z);
    }

    float t3 = 0.6 - dot(x3, x3);
    if(t3 < 0) n3 = 0.0;
    else
    {
        t3 *= t3;
        int gi = permutation[i.x + 1 + permutation[i.y + 1 + permutation[i.z + 1]]] % 12;
        n3 = t3 * t3 * grad(gi, x3.x, x3.y, x3.z);
    }

    return 32.0 * (n0 + n1 + n2 + n3);
}
```

---

## 5. Worley (Cellular) Noise

Worley noise creates cellular patterns based on distances to feature points.

### Worley Noise Implementation

```hlsl
// WorleyNoise.hlsl

// Hash function for random feature points
float2 hash2D(float2 p)
{
    p = float2(dot(p, float2(127.1, 311.7)),
               dot(p, float2(269.5, 183.3)));
    return frac(sin(p) * 43758.5453);
}

float3 hash3D(float3 p)
{
    p = float3(dot(p, float3(127.1, 311.7, 74.7)),
               dot(p, float3(269.5, 183.3, 246.1)),
               dot(p, float3(113.5, 271.9, 124.6)));
    return frac(sin(p) * 43758.5453);
}

// 2D Worley noise
float worleyNoise2D(float2 p, out float2 id, out float2 nearestPoint)
{
    float2 n = floor(p);
    float2 f = frac(p);

    float minDist = 1.0;
    nearestPoint = float2(0, 0);
    id = float2(0, 0);

    // Search 3x3 neighboring cells
    for(int j = -1; j <= 1; j++)
    {
        for(int i = -1; i <= 1; i++)
        {
            float2 neighbor = float2(i, j);
            float2 point = hash2D(n + neighbor);
            float2 diff = neighbor + point - f;
            float dist = length(diff);

            if(dist < minDist)
            {
                minDist = dist;
                nearestPoint = point;
                id = n + neighbor;
            }
        }
    }

    return minDist;
}

// 3D Worley noise
float worleyNoise3D(float3 p, out float3 id, out float3 nearestPoint)
{
    float3 n = floor(p);
    float3 f = frac(p);

    float minDist = 1.0;
    nearestPoint = float3(0, 0, 0);
    id = float3(0, 0, 0);

    // Search 3x3x3 neighboring cells
    for(int k = -1; k <= 1; k++)
    {
        for(int j = -1; j <= 1; j++)
        {
            for(int i = -1; i <= 1; i++)
            {
                float3 neighbor = float3(i, j, k);
                float3 point = hash3D(n + neighbor);
                float3 diff = neighbor + point - f;
                float dist = length(diff);

                if(dist < minDist)
                {
                    minDist = dist;
                    nearestPoint = point;
                    id = n + neighbor;
                }
            }
        }
    }

    return minDist;
}

// Worley noise with F1-F2 (for cell edge detection)
float2 worleyNoiseF1F2(float2 p)
{
    float2 n = floor(p);
    float2 f = frac(p);

    float F1 = 8.0;
    float F2 = 8.0;

    for(int j = -1; j <= 1; j++)
    {
        for(int i = -1; i <= 1; i++)
        {
            float2 neighbor = float2(i, j);
            float2 point = hash2D(n + neighbor);
            float dist = length(neighbor + point - f);

            if(dist < F1)
            {
                F2 = F1;
                F1 = dist;
            }
            else if(dist < F2)
            {
                F2 = dist;
            }
        }
    }

    return float2(F1, F2);
}
```

---

## 6. Fractal Brownian Motion (fBm)

fBm combines multiple octaves of noise at different frequencies and amplitudes.

### Advanced fBm Variations

```hlsl
// AdvancedFBM.hlsl

// Domain warping FBM
float domainWarpedFBM(float2 p, int octaves)
{
    float2 q = float2(fbm2D(p, octaves, 2.0, 0.5),
                      fbm2D(p + float2(5.2, 1.3), octaves, 2.0, 0.5));

    float2 r = float2(fbm2D(p + 4.0 * q + float2(1.7, 9.2), octaves, 2.0, 0.5),
                      fbm2D(p + 4.0 * q + float2(8.3, 2.8), octaves, 2.0, 0.5));

    return fbm2D(p + 4.0 * r, octaves, 2.0, 0.5);
}

// Ridged multifractal
float ridgedMultifractal(float2 p, int octaves, float lacunarity, float gain)
{
    float sum = 0.0;
    float amplitude = 0.5;
    float frequency = 1.0;

    for(int i = 0; i < octaves; i++)
    {
        float n = perlinNoise2D(p * frequency);
        n = abs(n); // Take absolute value
        n = 1.0 - n; // Invert
        n = n * n; // Square for sharper ridges

        sum += n * amplitude;
        amplitude *= gain;
        frequency *= lacunarity;
    }

    return sum;
}

// Billowy noise
float billowyNoise(float2 p, int octaves, float lacunarity, float gain)
{
    float sum = 0.0;
    float amplitude = 1.0;
    float frequency = 1.0;

    for(int i = 0; i < octaves; i++)
    {
        float n = abs(perlinNoise2D(p * frequency));
        sum += n * amplitude;
        amplitude *= gain;
        frequency *= lacunarity;
    }

    return sum;
}

// Swiss turbulence
float swissTurbulence(float2 p, int octaves, float lacunarity, float gain, float warp)
{
    float sum = 0.0;
    float amplitude = 1.0;
    float frequency = 1.0;
    float2 dsum = float2(0, 0);

    for(int i = 0; i < octaves; i++)
    {
        float2 warped = p * frequency + warp * dsum;
        float n = perlinNoise2D(warped);

        // Compute derivatives (approximate)
        float2 d;
        d.x = perlinNoise2D(warped + float2(0.01, 0)) - n;
        d.y = perlinNoise2D(warped + float2(0, 0.01)) - n;

        sum += amplitude * (1.0 - abs(n));
        dsum += amplitude * d * -n;

        amplitude *= gain;
        frequency *= lacunarity;
    }

    return sum;
}
```

---

## 7. Procedural Materials

### Marble Material

```hlsl
// ProceduralMaterials.hlsl

float4 marblePattern(float3 pos, float scale, float3 color1, float3 color2)
{
    // Create turbulent pattern
    float turbulence = fbm3D(pos * scale, 6, 2.0, 0.5);

    // Add veining
    float pattern = pos.y * 4.0 + turbulence * 8.0;

    // Create marble bands
    float marble = abs(sin(pattern));
    marble = pow(marble, 0.5);

    // Blend colors
    float3 color = lerp(color1, color2, marble);

    // Add subtle noise for texture
    color *= 0.8 + 0.2 * fbm3D(pos * scale * 4.0, 3, 2.0, 0.5);

    return float4(color, 1.0);
}

// Wood material
float4 woodPattern(float3 pos, float scale, float3 darkColor, float3 lightColor)
{
    // Distance from center (wood rings)
    float dist = length(pos.xz) * scale;

    // Add noise for irregular rings
    float noise = fbm2D(pos.xz * scale, 4, 2.0, 0.5) * 2.0;

    // Create ring pattern
    float rings = frac(dist + noise);

    // Sharpen transition between rings
    rings = smoothstep(0.3, 0.7, rings);

    // Add grain along y-axis
    float grain = fbm3D(pos * scale * float3(20, 1, 20), 3, 2.0, 0.5);
    rings *= 0.8 + 0.2 * grain;

    // Blend colors
    float3 color = lerp(darkColor, lightColor, rings);

    return float4(color, 1.0);
}

// Rock/Stone material
float4 rockPattern(float3 pos, float scale)
{
    // Base rock color
    float3 baseColor = float3(0.4, 0.35, 0.3);

    // Large features
    float largeNoise = fbm3D(pos * scale * 0.5, 4, 2.0, 0.5);

    // Medium details
    float mediumNoise = fbm3D(pos * scale * 2.0, 5, 2.0, 0.5);

    // Fine details
    float fineNoise = fbm3D(pos * scale * 8.0, 6, 2.0, 0.5);

    // Combine layers
    float combined = largeNoise * 0.5 + mediumNoise * 0.3 + fineNoise * 0.2;

    // Color variation
    float3 color = baseColor * (0.7 + 0.6 * combined);

    // Add some color spots
    float2 worley;
    float3 cellId, cellPoint;
    float cells = worleyNoise3D(pos * scale * 3.0, cellId, cellPoint);

    float3 spotColor = float3(0.3, 0.25, 0.2);
    color = lerp(color, spotColor, smoothstep(0.8, 1.0, cells));

    return float4(color, 1.0);
}

// Cloud material
float4 cloudPattern(float3 pos, float time, float density)
{
    // Animate clouds
    float3 animPos = pos + float3(time * 0.1, 0, time * 0.05);

    // Multiple layers of noise
    float cloud = fbm3D(animPos * 2.0, 6, 2.0, 0.5);

    // Add billowy character
    cloud = billowyNoise(animPos.xz * 3.0, 4, 2.0, 0.5) * 0.5 + cloud * 0.5;

    // Apply density threshold
    cloud = smoothstep(density - 0.1, density + 0.1, cloud);

    // Color based on density
    float3 cloudColor = lerp(float3(0.8, 0.8, 0.9), float3(1, 1, 1), cloud);

    return float4(cloudColor, cloud);
}
```

---

## 8. GPU-Based Procedural Texture Generation

### Texture Generator System

```cpp
// ProceduralTextureGenerator.h
#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <string>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

enum class NoiseType
{
    Perlin,
    Simplex,
    Worley,
    FBM,
    Marble,
    Wood,
    Rock,
    Clouds
};

struct TextureGenParams
{
    XMFLOAT4 Color1;
    XMFLOAT4 Color2;
    float Scale;
    float Time;
    int Octaves;
    float Lacunarity;
    float Gain;
    NoiseType Type;
    XMFLOAT2 Padding;
};

class ProceduralTextureGenerator
{
public:
    ProceduralTextureGenerator();
    ~ProceduralTextureGenerator();

    bool Initialize(ID3D11Device* device, int width, int height);
    void GenerateTexture(ID3D11DeviceContext* context, const TextureGenParams& params);

    ID3D11ShaderResourceView* GetTexture() const { return m_textureSRV.Get(); }
    ID3D11Texture2D* GetTextureResource() const { return m_texture.Get(); }

private:
    bool CreateComputeShader(ID3D11Device* device);
    bool CreateTexture(ID3D11Device* device, int width, int height);

    ComPtr<ID3D11ComputeShader> m_computeShader;
    ComPtr<ID3D11Texture2D> m_texture;
    ComPtr<ID3D11UnorderedAccessView> m_textureUAV;
    ComPtr<ID3D11ShaderResourceView> m_textureSRV;
    ComPtr<ID3D11Buffer> m_constantBuffer;

    int m_width;
    int m_height;
};

// ProceduralTextureGenerator.cpp
#include "ProceduralTextureGenerator.h"
#include <d3dcompiler.h>

ProceduralTextureGenerator::ProceduralTextureGenerator()
    : m_width(0), m_height(0)
{
}

ProceduralTextureGenerator::~ProceduralTextureGenerator()
{
}

bool ProceduralTextureGenerator::Initialize(ID3D11Device* device, int width, int height)
{
    m_width = width;
    m_height = height;

    if (!CreateComputeShader(device))
        return false;

    if (!CreateTexture(device, width, height))
        return false;

    // Create constant buffer
    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.ByteWidth = sizeof(TextureGenParams);
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT hr = device->CreateBuffer(&cbDesc, nullptr, &m_constantBuffer);
    return SUCCEEDED(hr);
}

bool ProceduralTextureGenerator::CreateTexture(ID3D11Device* device, int width, int height)
{
    // Create texture
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;

    HRESULT hr = device->CreateTexture2D(&texDesc, nullptr, &m_texture);
    if (FAILED(hr)) return false;

    // Create UAV
    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
    uavDesc.Format = texDesc.Format;
    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    uavDesc.Texture2D.MipSlice = 0;

    hr = device->CreateUnorderedAccessView(m_texture.Get(), &uavDesc, &m_textureUAV);
    if (FAILED(hr)) return false;

    // Create SRV
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    hr = device->CreateShaderResourceView(m_texture.Get(), &srvDesc, &m_textureSRV);
    return SUCCEEDED(hr);
}

void ProceduralTextureGenerator::GenerateTexture(ID3D11DeviceContext* context,
    const TextureGenParams& params)
{
    // Update constant buffer
    D3D11_MAPPED_SUBRESOURCE mapped;
    context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    memcpy(mapped.pData, &params, sizeof(TextureGenParams));
    context->Unmap(m_constantBuffer.Get(), 0);

    // Set compute shader and resources
    context->CSSetShader(m_computeShader.Get(), nullptr, 0);
    context->CSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
    context->CSSetUnorderedAccessViews(0, 1, m_textureUAV.GetAddressOf(), nullptr);

    // Dispatch compute shader
    UINT groupsX = (m_width + 15) / 16;
    UINT groupsY = (m_height + 15) / 16;
    context->Dispatch(groupsX, groupsY, 1);

    // Unbind UAV
    ID3D11UnorderedAccessView* nullUAV = nullptr;
    context->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
}
```

### Compute Shader for Texture Generation

```hlsl
// ProceduralTextureGen_CS.hlsl

// Include all noise functions from previous sections
#include "PerlinNoise.hlsl"
#include "SimplexNoise.hlsl"
#include "WorleyNoise.hlsl"
#include "ProceduralMaterials.hlsl"

cbuffer TextureParams : register(b0)
{
    float4 Color1;
    float4 Color2;
    float Scale;
    float Time;
    int Octaves;
    float Lacunarity;
    float Gain;
    int NoiseTypeValue; // 0=Perlin, 1=Simplex, 2=Worley, etc.
    float2 Padding;
};

RWTexture2D<float4> OutputTexture : register(u0);

[numthreads(16, 16, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint width, height;
    OutputTexture.GetDimensions(width, height);

    if(DTid.x >= width || DTid.y >= height)
        return;

    float2 uv = float2(DTid.xy) / float2(width, height);
    float3 pos = float3(uv * Scale, Time);

    float4 color = float4(0, 0, 0, 1);

    // Select noise type
    switch(NoiseTypeValue)
    {
        case 0: // Perlin
        {
            float noise = fbm2D(uv * Scale, Octaves, Lacunarity, Gain);
            noise = noise * 0.5 + 0.5; // Map to [0,1]
            color = lerp(Color1, Color2, noise);
            break;
        }

        case 1: // Simplex
        {
            float noise = simplexNoise2D(uv * Scale);
            noise = noise * 0.5 + 0.5;
            color = lerp(Color1, Color2, noise);
            break;
        }

        case 2: // Worley
        {
            float2 id;
            float2 point;
            float dist = worleyNoise2D(uv * Scale, id, point);
            color = lerp(Color1, Color2, dist);
            break;
        }

        case 3: // FBM
        {
            float noise = fbm2D(uv * Scale, Octaves, Lacunarity, Gain);
            noise = noise * 0.5 + 0.5;
            color = lerp(Color1, Color2, noise);
            break;
        }

        case 4: // Marble
        {
            color = marblePattern(pos, Scale, Color1.rgb, Color2.rgb);
            break;
        }

        case 5: // Wood
        {
            color = woodPattern(pos, Scale, Color1.rgb, Color2.rgb);
            break;
        }

        case 6: // Rock
        {
            color = rockPattern(pos, Scale);
            break;
        }

        case 7: // Clouds
        {
            color = cloudPattern(pos, Time, 0.5);
            break;
        }
    }

    OutputTexture[DTid.xy] = color;
}
```

---

## 9. Complete Application Example

```cpp
// ProceduralTextureApp.cpp
#include "ProceduralTextureGenerator.h"

class ProceduralTextureApp
{
public:
    bool Initialize(ID3D11Device* device);
    void Update(float deltaTime);
    void Render(ID3D11DeviceContext* context);

private:
    std::unique_ptr<ProceduralTextureGenerator> m_textureGen;
    TextureGenParams m_params;
    float m_time;
};

bool ProceduralTextureApp::Initialize(ID3D11Device* device)
{
    m_textureGen = std::make_unique<ProceduralTextureGenerator>();
    if (!m_textureGen->Initialize(device, 1024, 1024))
        return false;

    // Initialize parameters
    m_params.Color1 = XMFLOAT4(0.2f, 0.1f, 0.05f, 1.0f);
    m_params.Color2 = XMFLOAT4(0.8f, 0.6f, 0.4f, 1.0f);
    m_params.Scale = 4.0f;
    m_params.Octaves = 6;
    m_params.Lacunarity = 2.0f;
    m_params.Gain = 0.5f;
    m_params.Type = NoiseType::Marble;

    m_time = 0.0f;
    return true;
}

void ProceduralTextureApp::Update(float deltaTime)
{
    m_time += deltaTime;
    m_params.Time = m_time * 0.1f;
}

void ProceduralTextureApp::Render(ID3D11DeviceContext* context)
{
    // Generate procedural texture
    m_textureGen->GenerateTexture(context, m_params);

    // Use the generated texture for rendering
    // ...
}
```

---

## 10. Exercises

### Exercise 1: Custom Noise Function
Create a hybrid noise function that blends Perlin and Worley noise.

### Exercise 2: Animated Lava Material
Implement an animated lava material using domain-warped FBM.

### Exercise 3: 3D Volumetric Texture
Generate a 3D texture for volumetric clouds or smoke.

### Exercise 4: Noise-Based Displacement
Use procedural noise for vertex displacement in a terrain shader.

### Exercise 5: Interactive Material Editor
Create a UI to adjust procedural texture parameters in real-time.

---

## Key Takeaways

1. ✅ Procedural textures save memory and provide infinite detail
2. ✅ Perlin noise is versatile for organic patterns
3. ✅ Simplex noise offers better performance than Perlin
4. ✅ Worley noise creates cellular structures
5. ✅ FBM adds realistic complexity through octaves
6. ✅ Domain warping creates sophisticated patterns
7. ✅ GPU compute shaders enable real-time generation
8. ✅ Combining noise types produces unique materials

---

## Summary Checklist

Before moving to Lesson 83, ensure you can:
- [ ] Implement Perlin noise in C++ and HLSL
- [ ] Create simplex noise shaders
- [ ] Generate Worley noise patterns
- [ ] Use FBM with multiple octaves
- [ ] Apply domain warping techniques
- [ ] Create procedural materials (marble, wood, rock)
- [ ] Generate textures on GPU with compute shaders
- [ ] Combine different noise types effectively

**Congratulations on completing Lesson 82!**

Next lesson: Water Rendering Techniques
