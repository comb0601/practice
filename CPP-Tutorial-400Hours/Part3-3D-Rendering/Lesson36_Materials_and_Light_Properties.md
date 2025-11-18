# Lesson 36: Materials and Light Properties

## Material System

```cpp
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
```

## Fragment Shader

```glsl
void main() {
    // Ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
```

## C++ Material Setup

```cpp
// Emerald material
shader.setVec3("material.ambient", glm::vec3(0.0215, 0.1745, 0.0215));
shader.setVec3("material.diffuse", glm::vec3(0.07568, 0.61424, 0.07568));
shader.setVec3("material.specular", glm::vec3(0.633, 0.727811, 0.633));
shader.setFloat("material.shininess", 76.8f);

// Light properties
shader.setVec3("light.ambient", glm::vec3(0.2f));
shader.setVec3("light.diffuse", glm::vec3(0.5f));
shader.setVec3("light.specular", glm::vec3(1.0f));
```
