# Lesson 35: Lighting - Phong Lighting Model

## Phong Lighting Components

The Phong lighting model consists of three components:
1. **Ambient**: Background illumination
2. **Diffuse**: Directional light scattering
3. **Specular**: Shiny highlights

## Complete Implementation

### Vertex Shader

```glsl
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
```

### Fragment Shader

```glsl
#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main() {
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
} 
```

## C++ Setup

```cpp
shader.use();
shader.setVec3("lightPos", lightPos);
shader.setVec3("viewPos", camera.Position);
shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
shader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
```

## Blinn-Phong (Improved)

```glsl
// Blinn-Phong uses halfway vector instead of reflection
vec3 halfwayDir = normalize(lightDir + viewDir);
float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);
```
