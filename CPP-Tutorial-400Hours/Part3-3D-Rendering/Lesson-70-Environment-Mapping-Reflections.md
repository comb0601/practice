# Lesson 70: Environment Mapping and Reflections

**Duration**: 7 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Environment Mapping
2. Reflection Mapping
3. Refraction and Fresnel
4. Parallax-Corrected Reflections
5. Reflection Probes
6. Screen-Space Reflections (SSR)
7. Planar Reflections
8. Exercises and Projects

---

## 1. Introduction to Environment Mapping

Environment mapping uses cubemaps to simulate reflections and refractions on surfaces.

```cpp
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>

class EnvironmentMapping
{
public:
    static const char* getReflectionShader()
    {
        return R"(
            #version 330 core
            out vec4 FragColor;

            in vec3 FragPos;
            in vec3 Normal;

            uniform vec3 cameraPos;
            uniform samplerCube environmentMap;

            void main()
            {
                vec3 I = normalize(FragPos - cameraPos);
                vec3 R = reflect(I, normalize(Normal));
                vec3 reflection = texture(environmentMap, R).rgb;

                FragColor = vec4(reflection, 1.0);
            }
        )";
    }
};
```

---

## Key Takeaways

1. **Environment mapping simulates reflections**
2. **Reflection uses view direction and normal**
3. **Refraction uses Snell's law**
4. **Fresnel combines reflection and refraction**
5. **Parallax correction improves accuracy**
6. **Reflection probes capture local environment**
7. **SSR provides accurate screen-space reflections**
8. **Multiple techniques for different scenarios**

---

## Summary Checklist

- [ ] Implement basic reflection mapping
- [ ] Add refraction with Fresnel
- [ ] Create parallax-corrected reflections
- [ ] Set up reflection probe system
- [ ] Implement SSR (optional)
- [ ] Add planar reflections for water
- [ ] Optimize reflection rendering
- [ ] Test with various materials

---

## Next Lesson Preview

In **Lesson 71**, we'll explore:
- Geometry shader fundamentals
- Point expansion
- Normal visualization
- Procedural geometry
- Fur/grass rendering
- Optimization techniques

**Environment Mapping mastered!**
