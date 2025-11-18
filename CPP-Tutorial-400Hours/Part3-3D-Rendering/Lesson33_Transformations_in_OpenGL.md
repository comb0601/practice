# Lesson 33: Transformations in OpenGL

## Transformation Matrices with GLM

### Translation
Move objects in 3D space.

```cpp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));
shader.setMat4("transform", trans);
```

### Rotation
Rotate around an axis.

```cpp
// Rotate 45 degrees around Y-axis
float angle = glm::radians(45.0f);
glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, axis);

// Euler angles (pitch, yaw, roll)
glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1.0f, 0.0f, 0.0f));
glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), roll, glm::vec3(0.0f, 0.0f, 1.0f));
glm::mat4 rotation = rotZ * rotY * rotX;
```

### Scaling
Change object size.

```cpp
glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
```

### Combined Transformations (TRS Order)

```cpp
glm::mat4 model = glm::mat4(1.0f);
model = glm::translate(model, position);   // Translate first
model = glm::rotate(model, angle, axis);   // Then rotate
model = glm::scale(model, scale);          // Scale last

shader.setMat4("model", model);
```

## Transform Class

```cpp
class Transform {
public:
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);  // Euler angles
    glm::vec3 scale = glm::vec3(1.0f);
    
    glm::mat4 getModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        
        // Apply transformations in TRS order
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        
        return model;
    }
    
    void translate(const glm::vec3& offset) {
        position += offset;
    }
    
    void rotate(float angleDegrees, const glm::vec3& axis) {
        if (axis.x != 0.0f) rotation.x += angleDegrees;
        if (axis.y != 0.0f) rotation.y += angleDegrees;
        if (axis.z != 0.0f) rotation.z += angleDegrees;
    }
};
```

## Usage Example

```cpp
Transform cubeTransform;
cubeTransform.position = glm::vec3(0.0f, 1.0f, 0.0f);
cubeTransform.rotation = glm::vec3(0.0f, 45.0f, 0.0f);
cubeTransform.scale = glm::vec3(1.5f);

// In render loop
shader.use();
shader.setMat4("model", cubeTransform.getModelMatrix());
```
