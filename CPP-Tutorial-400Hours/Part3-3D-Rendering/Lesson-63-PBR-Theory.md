# Lesson 63: Physically Based Rendering (PBR) - Theory

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to PBR
2. The Rendering Equation
3. BRDF (Bidirectional Reflectance Distribution Function)
4. Cook-Torrance BRDF
5. Fresnel Equations
6. Microfacet Theory
7. Energy Conservation
8. Material Properties

---

## 1. Introduction to PBR

### What is Physically Based Rendering?

PBR is an approach to shading and rendering that aims to accurately represent how light interacts with surfaces in the real world, following the laws of physics.

```cpp
#include <glm/glm.hpp>
#include <cmath>
#include <iostream>

// PBR Theory Concepts in C++

class PBRTheory
{
public:
    // Core PBR principles:
    // 1. Energy Conservation: Reflected light <= Incident light
    // 2. Physically accurate materials
    // 3. Microfacet theory
    // 4. Fresnel reflectance

    struct Material
    {
        glm::vec3 albedo;      // Base color
        float metallic;        // 0 = dielectric, 1 = metal
        float roughness;       // 0 = smooth, 1 = rough
        float ao;              // Ambient occlusion

        Material()
            : albedo(1.0f), metallic(0.0f),
              roughness(0.5f), ao(1.0f) {}
    };

    struct Light
    {
        glm::vec3 position;
        glm::vec3 color;
        float intensity;

        Light(glm::vec3 pos, glm::vec3 col, float i)
            : position(pos), color(col), intensity(i) {}
    };

    // Why PBR?
    static void explainPBRBenefits()
    {
        std::cout << "=== PBR Benefits ===" << std::endl;
        std::cout << "1. Physically accurate lighting" << std::endl;
        std::cout << "2. Consistent results under different lighting" << std::endl;
        std::cout << "3. Intuitive material parameters" << std::endl;
        std::cout << "4. Easier for artists to create realistic materials" << std::endl;
        std::cout << "5. Industry standard (used in games and films)" << std::endl;
    }

    // Traditional vs PBR comparison
    static void compareRenderingModels()
    {
        std::cout << "\n=== Traditional Phong vs PBR ===" << std::endl;

        std::cout << "\nPhong Shading:" << std::endl;
        std::cout << "  - Ambient + Diffuse + Specular" << std::endl;
        std::cout << "  - Not energy conserving" << std::endl;
        std::cout << "  - Parameters: Ka, Kd, Ks, shininess" << std::endl;
        std::cout << "  - Simple but unrealistic" << std::endl;

        std::cout << "\nPBR:" << std::endl;
        std::cout << "  - Cook-Torrance BRDF" << std::endl;
        std::cout << "  - Energy conserving" << std::endl;
        std::cout << "  - Parameters: albedo, metallic, roughness" << std::endl;
        std::cout << "  - Physically accurate" << std::endl;
    }
};
```

---

## 2. The Rendering Equation

### Understanding the Rendering Equation

The rendering equation describes how light interacts with surfaces:

**L_o(p,ω_o) = L_e(p,ω_o) + ∫_Ω f_r(p,ω_i,ω_o) L_i(p,ω_i) (n·ω_i) dω_i**

Where:
- L_o = Outgoing radiance
- L_e = Emitted radiance
- f_r = BRDF
- L_i = Incoming radiance
- n·ω_i = Cosine term

```cpp
class RenderingEquation
{
public:
    // Simplified rendering equation implementation
    static glm::vec3 calculateOutgoingRadiance(
        const glm::vec3& position,
        const glm::vec3& normal,
        const glm::vec3& viewDir,
        const std::vector<PBRTheory::Light>& lights,
        const PBRTheory::Material& material)
    {
        glm::vec3 Lo(0.0f); // Outgoing radiance

        // For each light, integrate incoming radiance
        for(const auto& light : lights)
        {
            // Calculate per-light radiance
            glm::vec3 lightDir = glm::normalize(light.position - position);
            glm::vec3 radiance = light.color * light.intensity;

            // BRDF evaluation (simplified)
            glm::vec3 f_r = evaluateBRDF(normal, viewDir, lightDir, material);

            // Cosine term
            float NdotL = glm::max(glm::dot(normal, lightDir), 0.0f);

            // Add to outgoing radiance
            Lo += f_r * radiance * NdotL;
        }

        return Lo;
    }

private:
    static glm::vec3 evaluateBRDF(
        const glm::vec3& N,
        const glm::vec3& V,
        const glm::vec3& L,
        const PBRTheory::Material& material)
    {
        // Placeholder - will implement in next sections
        return material.albedo / glm::pi<float>();
    }
};

// Mathematical concepts
class PBRMath
{
public:
    // Radiance: Power per unit solid angle per unit projected area
    struct Radiance
    {
        float value; // W/(sr·m²)

        static void explain()
        {
            std::cout << "Radiance measures light intensity:" << std::endl;
            std::cout << "  - Direction-dependent" << std::endl;
            std::cout << "  - Measured in W/(sr·m²)" << std::endl;
            std::cout << "  - What our eyes perceive as brightness" << std::endl;
        }
    };

    // Solid angle
    static float solidAngle(float radius, float distance)
    {
        // Ω = A / r²
        float area = glm::pi<float>() * radius * radius;
        return area / (distance * distance);
    }

    // Hemisphere integration
    static void explainHemisphereIntegration()
    {
        std::cout << "\n=== Hemisphere Integration ===" << std::endl;
        std::cout << "We integrate over the hemisphere around the normal:" << std::endl;
        std::cout << "  ∫_Ω f(ω) dω" << std::endl;
        std::cout << "In practice, we sum over discrete light sources" << std::endl;
    }
};
```

---

## 3. BRDF (Bidirectional Reflectance Distribution Function)

### Understanding BRDF

The BRDF describes how light is reflected at an opaque surface.

```cpp
class BRDFTheory
{
public:
    // BRDF properties
    static void explainBRDF()
    {
        std::cout << "=== BRDF Properties ===" << std::endl;
        std::cout << "1. Helmholtz Reciprocity: f_r(ω_i,ω_o) = f_r(ω_o,ω_i)" << std::endl;
        std::cout << "2. Energy Conservation: ∫ f_r dω <= 1" << std::endl;
        std::cout << "3. Non-negative: f_r >= 0" << std::endl;
    }

    // BRDF types
    enum class BRDFType
    {
        DIFFUSE,    // Lambertian
        SPECULAR,   // Mirror-like
        GLOSSY,     // Combination
        COOK_TORRANCE // PBR standard
    };

    // Lambertian (Diffuse) BRDF
    static glm::vec3 lambertianBRDF(const glm::vec3& albedo)
    {
        // f_r = albedo / π
        return albedo / glm::pi<float>();
    }

    // Perfect specular BRDF
    static glm::vec3 specularBRDF(
        const glm::vec3& normal,
        const glm::vec3& lightDir,
        const glm::vec3& viewDir,
        const glm::vec3& albedo)
    {
        // Mirror reflection
        glm::vec3 reflectDir = glm::reflect(-lightDir, normal);
        float spec = glm::dot(reflectDir, viewDir);

        if(spec > 0.999f) // Delta function approximation
            return albedo;
        else
            return glm::vec3(0.0f);
    }

    // BRDF visualization
    class BRDFVisualizer
    {
    public:
        static void visualizeDiffuse()
        {
            std::cout << "\n=== Diffuse BRDF ===" << std::endl;
            std::cout << "Equal reflection in all directions" << std::endl;
            std::cout << "Independent of view angle" << std::endl;
            std::cout << "Constant: f_r = albedo/π" << std::endl;
        }

        static void visualizeSpecular()
        {
            std::cout << "\n=== Specular BRDF ===" << std::endl;
            std::cout << "Mirror-like reflection" << std::endl;
            std::cout << "Depends on view angle" << std::endl;
            std::cout << "Delta function at reflection direction" << std::endl;
        }

        static void visualizeGlossy()
        {
            std::cout << "\n=== Glossy BRDF ===" << std::endl;
            std::cout << "Combination of diffuse and specular" << std::endl;
            std::cout << "Lobe around reflection direction" << std::endl;
            std::cout << "Width controlled by roughness" << std::endl;
        }
    };
};
```

---

## 4. Cook-Torrance BRDF

### The Cook-Torrance Reflectance Model

The industry-standard BRDF for PBR:

**f_r = k_d * f_lambert + k_s * f_cook-torrance**

**f_cook-torrance = DFG / (4(ω_o · n)(ω_i · n))**

```cpp
class CookTorranceBRDF
{
public:
    // Cook-Torrance BRDF components
    // f_r = k_d * (albedo/π) + k_s * (DFG / (4(N·V)(N·L)))

    static glm::vec3 evaluate(
        const glm::vec3& N,  // Normal
        const glm::vec3& V,  // View direction
        const glm::vec3& L,  // Light direction
        const glm::vec3& albedo,
        float metallic,
        float roughness)
    {
        glm::vec3 H = glm::normalize(V + L);

        // Calculate reflectance at normal incidence
        glm::vec3 F0 = glm::vec3(0.04f);
        F0 = glm::mix(F0, albedo, metallic);

        // Cook-Torrance BRDF terms
        float NDF = distributionGGX(N, H, roughness);
        float G = geometrySmith(N, V, L, roughness);
        glm::vec3 F = fresnelSchlick(glm::max(glm::dot(H, V), 0.0f), F0);

        // Calculate specular
        glm::vec3 numerator = NDF * G * F;
        float denominator = 4.0f * glm::max(glm::dot(N, V), 0.0f) *
                           glm::max(glm::dot(N, L), 0.0f) + 0.0001f;
        glm::vec3 specular = numerator / denominator;

        // Energy conservation
        glm::vec3 kS = F;
        glm::vec3 kD = glm::vec3(1.0f) - kS;
        kD *= 1.0f - metallic;

        // Diffuse
        glm::vec3 diffuse = kD * albedo / glm::pi<float>();

        return diffuse + specular;
    }

    // Normal Distribution Function (D)
    static float distributionGGX(
        const glm::vec3& N,
        const glm::vec3& H,
        float roughness)
    {
        float a = roughness * roughness;
        float a2 = a * a;
        float NdotH = glm::max(glm::dot(N, H), 0.0f);
        float NdotH2 = NdotH * NdotH;

        float nom = a2;
        float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
        denom = glm::pi<float>() * denom * denom;

        return nom / denom;
    }

    // Geometry Function (G)
    static float geometrySchlickGGX(float NdotV, float roughness)
    {
        float r = (roughness + 1.0f);
        float k = (r * r) / 8.0f;

        float nom = NdotV;
        float denom = NdotV * (1.0f - k) + k;

        return nom / denom;
    }

    static float geometrySmith(
        const glm::vec3& N,
        const glm::vec3& V,
        const glm::vec3& L,
        float roughness)
    {
        float NdotV = glm::max(glm::dot(N, V), 0.0f);
        float NdotL = glm::max(glm::dot(N, L), 0.0f);
        float ggx2 = geometrySchlickGGX(NdotV, roughness);
        float ggx1 = geometrySchlickGGX(NdotL, roughness);

        return ggx1 * ggx2;
    }

    // Fresnel Function (F)
    static glm::vec3 fresnelSchlick(float cosTheta, const glm::vec3& F0)
    {
        return F0 + (glm::vec3(1.0f) - F0) *
               pow(glm::clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
    }

    static void explainComponents()
    {
        std::cout << "=== Cook-Torrance Components ===" << std::endl;
        std::cout << "\nD - Normal Distribution Function (NDF):" << std::endl;
        std::cout << "  - Describes microfacet distribution" << std::endl;
        std::cout << "  - Controls specular lobe shape" << std::endl;
        std::cout << "  - Common: GGX/Trowbridge-Reitz" << std::endl;

        std::cout << "\nF - Fresnel Term:" << std::endl;
        std::cout << "  - Describes reflected vs refracted light" << std::endl;
        std::cout << "  - Depends on view angle" << std::endl;
        std::cout << "  - Common: Schlick approximation" << std::endl;

        std::cout << "\nG - Geometry Function:" << std::endl;
        std::cout << "  - Describes self-shadowing of microfacets" << std::endl;
        std::cout << "  - Prevents energy loss at grazing angles" << std::endl;
        std::cout << "  - Common: Smith's method" << std::endl;
    }
};
```

---

## 5. Fresnel Equations

### Understanding Fresnel Reflectance

```cpp
class FresnelTheory
{
public:
    // Fresnel effect: More reflection at grazing angles
    static void explainFresnel()
    {
        std::cout << "=== Fresnel Effect ===" << std::endl;
        std::cout << "Light reflection increases at grazing angles" << std::endl;
        std::cout << "Examples:" << std::endl;
        std::cout << "  - Water appears more reflective at distance" << std::endl;
        std::cout << "  - Fabric edges catch light" << std::endl;
        std::cout << "  - All materials show this behavior" << std::endl;
    }

    // Full Fresnel equations (complex)
    static float fresnelDielectric(float cosTheta, float ior)
    {
        // Simplified Fresnel for dielectrics
        float F0 = pow((1.0f - ior) / (1.0f + ior), 2.0f);
        return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
    }

    // Schlick approximation (fast)
    static glm::vec3 fresnelSchlick(
        float cosTheta,
        const glm::vec3& F0)
    {
        return F0 + (glm::vec3(1.0f) - F0) *
               pow(glm::clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
    }

    // Fresnel with roughness
    static glm::vec3 fresnelSchlickRoughness(
        float cosTheta,
        const glm::vec3& F0,
        float roughness)
    {
        return F0 + (glm::max(glm::vec3(1.0f - roughness), F0) - F0) *
               pow(glm::clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
    }

    // F0 values for common materials
    struct F0Values
    {
        static constexpr float WATER = 0.02f;
        static constexpr float PLASTIC = 0.04f;
        static constexpr float GLASS = 0.04f;
        static constexpr float DIAMOND = 0.17f;

        static glm::vec3 IRON()    { return glm::vec3(0.56f, 0.57f, 0.58f); }
        static glm::vec3 COPPER()  { return glm::vec3(0.95f, 0.64f, 0.54f); }
        static glm::vec3 GOLD()    { return glm::vec3(1.00f, 0.71f, 0.29f); }
        static glm::vec3 ALUMINUM(){ return glm::vec3(0.91f, 0.92f, 0.92f); }
        static glm::vec3 SILVER()  { return glm::vec3(0.95f, 0.93f, 0.88f); }

        static void printTable()
        {
            std::cout << "\n=== F0 Values (Reflectance at 0°) ===" << std::endl;
            std::cout << "Dielectrics:" << std::endl;
            std::cout << "  Water:    0.02" << std::endl;
            std::cout << "  Plastic:  0.04" << std::endl;
            std::cout << "  Glass:    0.04" << std::endl;
            std::cout << "  Diamond:  0.17" << std::endl;

            std::cout << "\nMetals (RGB):" << std::endl;
            std::cout << "  Iron:     (0.56, 0.57, 0.58)" << std::endl;
            std::cout << "  Copper:   (0.95, 0.64, 0.54)" << std::endl;
            std::cout << "  Gold:     (1.00, 0.71, 0.29)" << std::endl;
            std::cout << "  Aluminum: (0.91, 0.92, 0.92)" << std::endl;
            std::cout << "  Silver:   (0.95, 0.93, 0.88)" << std::endl;
        }
    };

    // Visualize Fresnel effect
    static void visualizeFresnelCurve()
    {
        std::cout << "\n=== Fresnel Reflectance Curve ===" << std::endl;
        std::cout << "Angle | Reflectance (F0=0.04)" << std::endl;
        std::cout << "------|----------------------" << std::endl;

        glm::vec3 F0(0.04f);
        for(int angle = 0; angle <= 90; angle += 15)
        {
            float radians = glm::radians((float)angle);
            float cosTheta = cos(radians);
            glm::vec3 F = fresnelSchlick(cosTheta, F0);

            printf("%3d°  | %.3f\n", angle, F.r);
        }
    }
};
```

---

## 6. Microfacet Theory

### Understanding Surface Roughness

```cpp
class MicrofacetTheory
{
public:
    // Microfacet model: Surface composed of tiny perfect mirrors
    static void explainMicrofacets()
    {
        std::cout << "=== Microfacet Theory ===" << std::endl;
        std::cout << "Surfaces are composed of microscopic facets" << std::endl;
        std::cout << "Each facet is a perfect mirror" << std::endl;
        std::cout << "Roughness = distribution of facet orientations" << std::endl;
        std::cout << "\nSmooth surface:" << std::endl;
        std::cout << "  - Aligned facets" << std::endl;
        std::cout << "  - Sharp reflection" << std::endl;
        std::cout << "\nRough surface:" << std::endl;
        std::cout << "  - Random facet orientations" << std::endl;
        std::cout << "  - Blurred reflection" << std::endl;
    }

    // Normal distribution functions
    class NDF
    {
    public:
        // GGX / Trowbridge-Reitz (industry standard)
        static float GGX(float NdotH, float roughness)
        {
            float a = roughness * roughness;
            float a2 = a * a;
            float NdotH2 = NdotH * NdotH;

            float nom = a2;
            float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
            denom = glm::pi<float>() * denom * denom;

            return nom / denom;
        }

        // Beckmann distribution
        static float Beckmann(float NdotH, float roughness)
        {
            float a = roughness * roughness;
            float a2 = a * a;
            float NdotH2 = NdotH * NdotH;
            float NdotH4 = NdotH2 * NdotH2;

            float nom = exp((NdotH2 - 1.0f) / (a2 * NdotH2));
            float denom = glm::pi<float>() * a2 * NdotH4;

            return nom / denom;
        }

        // Blinn-Phong (traditional)
        static float BlinnPhong(float NdotH, float shininess)
        {
            return ((shininess + 2.0f) / (2.0f * glm::pi<float>())) *
                   pow(NdotH, shininess);
        }

        static void compareNDFs()
        {
            std::cout << "\n=== Normal Distribution Functions ===" << std::endl;
            std::cout << "\nGGX/Trowbridge-Reitz:" << std::endl;
            std::cout << "  + Industry standard" << std::endl;
            std::cout << "  + Long highlight tail" << std::endl;
            std::cout << "  + Realistic metals" << std::endl;

            std::cout << "\nBeckmann:" << std::endl;
            std::cout << "  + Sharp highlights" << std::endl;
            std::cout << "  - Less realistic tail" << std::endl;

            std::cout << "\nBlinn-Phong:" << std::endl;
            std::cout << "  + Simple, fast" << std:endl;
            std::cout << "  - Not physically based" << std::endl;
        }
    };

    // Geometry/Shadowing functions
    class GeometryFunction
    {
    public:
        // Smith's method (separable)
        static float Smith(
            float NdotV,
            float NdotL,
            float roughness)
        {
            float k = (roughness + 1.0f) * (roughness + 1.0f) / 8.0f;

            float G_V = NdotV / (NdotV * (1.0f - k) + k);
            float G_L = NdotL / (NdotL * (1.0f - k) + k);

            return G_V * G_L;
        }

        static void explainShadowingMasking()
        {
            std::cout << "\n=== Geometry Function ===" << std::endl;
            std::cout << "Accounts for microfacet self-shadowing:" << std::endl;
            std::cout << "\nShadowing:" << std::endl;
            std::cout << "  - Microfacets block light from reaching other facets" << std::endl;
            std::cout << "\nMasking:" << std::endl;
            std::cout << "  - Microfacets block reflected light to viewer" << std::endl;
            std::cout << "\nMore prominent at grazing angles" << std::endl;
        }
    };

    // Roughness parameterization
    static float alphaFromRoughness(float roughness)
    {
        // Convert perceptual roughness to alpha
        // Disney's remapping for more linear perception
        return roughness * roughness;
    }

    static void visualizeRoughness()
    {
        std::cout << "\n=== Roughness Values ===" << std::endl;
        std::cout << "Value | Appearance" << std::endl;
        std::cout << "------|------------------" << std::endl;
        std::cout << "0.0   | Mirror (unrealistic)" << std::endl;
        std::cout << "0.2   | Polished metal" << std::endl;
        std::cout << "0.4   | Brushed metal" << std::endl;
        std::cout << "0.6   | Worn metal" << std::endl;
        std::cout << "0.8   | Rough metal" << std::endl;
        std::cout << "1.0   | Very rough/matte" << std::endl;
    }
};
```

---

## 7. Energy Conservation

### Maintaining Physical Accuracy

```cpp
class EnergyConservation
{
public:
    // Energy conservation principle
    static void explainPrinciple()
    {
        std::cout << "=== Energy Conservation ===" << std::endl;
        std::cout << "Fundamental law: Reflected light <= Incident light" << std::endl;
        std::cout << "\nIn PBR:" << std::endl;
        std::cout << "  Diffuse + Specular <= 1.0" << std::endl;
        std::cout << "  k_d + k_s <= 1.0" << std::endl;
        std::cout << "\nViolations lead to:" << std::endl;
        std::cout << "  - Unrealistic bright surfaces" << std::endl;
        std::cout << "  - Energy creation (impossible)" << std::endl;
    }

    // Calculate diffuse and specular weights
    static void calculateWeights(
        float metallic,
        const glm::vec3& F,
        glm::vec3& kD,
        glm::vec3& kS)
    {
        // Specular weight from Fresnel
        kS = F;

        // Diffuse weight
        kD = glm::vec3(1.0f) - kS;

        // Metals have no diffuse lighting
        kD *= (1.0f - metallic);
    }

    // Verify energy conservation
    static bool verifyConservation(
        const glm::vec3& diffuse,
        const glm::vec3& specular)
    {
        glm::vec3 total = diffuse + specular;
        return total.r <= 1.0001f &&
               total.g <= 1.0001f &&
               total.b <= 1.0001f;
    }

    // Metals vs Dielectrics
    class MaterialTypes
    {
    public:
        static void explainMetals()
        {
            std::cout << "\n=== Metals ===" << std::endl;
            std::cout << "Properties:" << std::endl;
            std::cout << "  - No diffuse reflection" << std::endl;
            std::cout << "  - All reflection is specular" << std::endl;
            std::cout << "  - Colored specular (from albedo)" << std::endl;
            std::cout << "  - High F0 values" << std::endl;
            std::cout << "  - metallic = 1.0" << std::endl;
        }

        static void explainDielectrics()
        {
            std::cout << "\n=== Dielectrics ===" << std::endl;
            std::cout << "Properties:" << std::endl;
            std::cout << "  - Both diffuse and specular" << std::endl;
            std::cout << "  - Colored diffuse (from albedo)" << std::endl;
            std::cout << "  - White/colorless specular" << std::endl;
            std::cout << "  - Low F0 values (~0.04)" << std::endl;
            std::cout << "  - metallic = 0.0" << std::endl;
        }

        static void explainMetallicWorkflow()
        {
            std::cout << "\n=== Metallic Workflow ===" << std::endl;
            std::cout << "Use metallic parameter to interpolate:" << std::endl;
            std::cout << "  F0 = mix(vec3(0.04), albedo, metallic)" << std::endl;
            std::cout << "  kD *= (1.0 - metallic)" << std::endl;
            std::cout << "\nmetallic = 0.0:" << std::endl;
            std::cout << "  - Full dielectric behavior" << std::endl;
            std::cout << "  - Colored diffuse, white specular" << std::endl;
            std::cout << "\nmetallic = 1.0:" << std::endl;
            std::cout << "  - Full metal behavior" << std::endl;
            std::cout << "  - No diffuse, colored specular" << std::endl;
        }
    };
};
```

---

## 8. Material Properties

### PBR Material Parameters

```cpp
class PBRMaterials
{
public:
    struct MaterialDefinition
    {
        glm::vec3 albedo;
        float metallic;
        float roughness;
        float ao;

        std::string name;

        MaterialDefinition(const std::string& n,
                          glm::vec3 a, float m, float r, float o = 1.0f)
            : name(n), albedo(a), metallic(m), roughness(r), ao(o) {}

        void print() const
        {
            printf("%s:\n", name.c_str());
            printf("  Albedo:    (%.2f, %.2f, %.2f)\n",
                   albedo.r, albedo.g, albedo.b);
            printf("  Metallic:  %.2f\n", metallic);
            printf("  Roughness: %.2f\n", roughness);
            printf("  AO:        %.2f\n", ao);
        }
    };

    // Common material presets
    static MaterialDefinition Gold()
    {
        return MaterialDefinition("Gold",
            glm::vec3(1.0f, 0.71f, 0.29f), 1.0f, 0.2f);
    }

    static MaterialDefinition Iron()
    {
        return MaterialDefinition("Iron",
            glm::vec3(0.56f, 0.57f, 0.58f), 1.0f, 0.5f);
    }

    static MaterialDefinition Copper()
    {
        return MaterialDefinition("Copper",
            glm::vec3(0.95f, 0.64f, 0.54f), 1.0f, 0.3f);
    }

    static MaterialDefinition Plastic()
    {
        return MaterialDefinition("Plastic",
            glm::vec3(0.8f, 0.0f, 0.0f), 0.0f, 0.5f);
    }

    static MaterialDefinition Rubber()
    {
        return MaterialDefinition("Rubber",
            glm::vec3(0.1f, 0.1f, 0.1f), 0.0f, 0.9f);
    }

    static MaterialDefinition Wood()
    {
        return MaterialDefinition("Wood",
            glm::vec3(0.6f, 0.4f, 0.2f), 0.0f, 0.8f);
    }

    static void printMaterialLibrary()
    {
        std::cout << "=== PBR Material Library ===" << std::endl;
        std::cout << std::endl;

        Gold().print();
        std::cout << std::endl;
        Iron().print();
        std::cout << std::endl;
        Copper().print();
        std::cout << std::endl;
        Plastic().print();
        std::cout << std::endl;
        Rubber().print();
        std::cout << std::endl;
        Wood().print();
    }

    // Material authoring guidelines
    static void authoringGuidelines()
    {
        std::cout << "\n=== Material Authoring Guidelines ===" << std::endl;

        std::cout << "\nAlbedo:" << std::endl;
        std::cout << "  - sRGB color space" << std::endl;
        std::cout << "  - No lighting information" << std::endl;
        std::cout << "  - Typical range: 50-240 sRGB" << std::endl;
        std::cout << "  - Avoid pure black/white" << std::endl;

        std::cout << "\nMetallic:" << std::endl;
        std::cout << "  - Binary in reality: 0 or 1" << std::endl;
        std::cout << "  - Intermediate values for blending" << std::endl;
        std::cout << "  - Use masks for metal/non-metal regions" << std::endl;

        std::cout << "\nRoughness:" << std::endl;
        std::cout << "  - Perceptually linear" << std::endl;
        std::cout << "  - 0.0 = mirror (rare)" << std::endl;
        std::cout << "  - 1.0 = completely matte" << std::endl;
        std::cout << "  - Variation adds realism" << std::endl;

        std::cout << "\nAmbient Occlusion:" << std::endl;
        std::cout << "  - Baked shadow information" << std::endl;
        std::cout << "  - Multiplied with ambient lighting" << std::endl;
        std::cout << "  - 0.0 = fully occluded, 1.0 = no occlusion" << std::endl;
    }
};
```

---

## Key Takeaways

1. **PBR follows laws of physics**
2. **Energy conservation is fundamental**
3. **Cook-Torrance BRDF is industry standard**
4. **Microfacet theory explains roughness**
5. **Fresnel effect occurs on all materials**
6. **Materials defined by albedo, metallic, roughness**
7. **Metals vs dielectrics have different properties**
8. **Understanding theory enables better implementation**

---

## Summary Checklist

Before moving to Lesson 64, ensure you understand:
- [ ] The rendering equation
- [ ] BRDF properties and types
- [ ] Cook-Torrance BRDF components (D, F, G)
- [ ] Fresnel equations and Schlick approximation
- [ ] Microfacet theory
- [ ] Normal distribution functions
- [ ] Energy conservation principles
- [ ] Metallic workflow
- [ ] Material parameter meanings
- [ ] F0 values for common materials

---

## Next Lesson Preview

In **Lesson 64**, we'll implement:
- Complete PBR shader
- Material system
- Multiple light types
- Texture-based materials
- IBL preparation
- Optimization techniques

**PBR Theory mastered!**
