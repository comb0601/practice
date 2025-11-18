# Lesson 08: Matrix Multiplication and Composition

## Overview
Matrix multiplication is the cornerstone of 3D graphics transformations. This lesson explores advanced multiplication techniques, composition patterns, optimization strategies, and hierarchical transformation systems used in modern 3D engines.

## Table of Contents
1. [Matrix Multiplication Mathematics](#matrix-multiplication-mathematics)
2. [Composition Patterns](#composition-patterns)
3. [Matrix Stacks](#matrix-stacks)
4. [Hierarchical Transformations](#hierarchical-transformations)
5. [Optimization Techniques](#optimization-techniques)

## Matrix Multiplication Mathematics

### The Fundamental Operation

```cpp
// Matrix multiplication: C = A * B
Matrix4 Matrix4::operator*(const Matrix4& B) const {
    Matrix4 C;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            C.m[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                C.m[i][j] += m[i][k] * B.m[k][j];
            }
        }
    }
    return C;
}
```

### Properties

- **NOT commutative**: AB ≠ BA
- **Associative**: (AB)C = A(BC)
- **Distributive**: A(B+C) = AB + AC
- **Identity**: AI = IA = A

### Why Order Matters

```cpp
void demonstrateOrder() {
    Vector3 point(1, 0, 0);
    
    Matrix4 T = Matrix4::translation(5, 0, 0);
    Matrix4 R = Matrix4::rotationZ(PI/2);
    
    // T*R: First rotate, then translate
    Vector3 result1 = (T * R).transformPoint(point);
    // Result: (5, 1, 0) - rotated point is translated
    
    // R*T: First translate, then rotate
    Vector3 result2 = (R * T).transformPoint(point);
    // Result: (-0, 6, 0) - translated point orbits around origin
}
```

## Composition Patterns

### Complex Transformation Chains

```cpp
class TransformChain {
    std::vector<Matrix4> transforms;
    
public:
    void addTranslation(const Vector3& t) {
        transforms.push_back(Matrix4::translation(t));
    }
    
    void addRotation(const Vector3& axis, float angle) {
        transforms.push_back(Matrix4::rotationAxis(axis, angle));
    }
    
    void addScale(const Vector3& s) {
        transforms.push_back(Matrix4::scale(s));
    }
    
    Matrix4 compose() const {
        Matrix4 result;
        // Apply right to left (last added is applied first)
        for (int i = transforms.size() - 1; i >= 0; i--) {
            result = transforms[i] * result;
        }
        return result;
    }
};
```

## Matrix Stacks

### OpenGL-Style Matrix Stack

```cpp
class MatrixStack {
    std::vector<Matrix4> stack;
    Matrix4 current;
    
public:
    MatrixStack() {
        current = Matrix4::identity();
    }
    
    void push() {
        stack.push_back(current);
    }
    
    void pop() {
        if (!stack.empty()) {
            current = stack.back();
            stack.pop_back();
        }
    }
    
    void loadIdentity() {
        current = Matrix4::identity();
    }
    
    void multiply(const Matrix4& m) {
        current = current * m;
    }
    
    void translate(const Vector3& t) {
        multiply(Matrix4::translation(t));
    }
    
    void rotate(const Vector3& axis, float angle) {
        multiply(Matrix4::rotationAxis(axis, angle));
    }
    
    void scale(const Vector3& s) {
        multiply(Matrix4::scale(s));
    }
    
    const Matrix4& top() const {
        return current;
    }
};

// Usage example
void drawRobotArm(MatrixStack& stack) {
    stack.push();
        stack.translate(Vector3(0, 0, 0));  // Shoulder position
        drawShoulder();
        
        stack.push();
            stack.rotate(Vector3(0, 1, 0), shoulderAngle);
            stack.translate(Vector3(0, -2, 0));  // Upper arm
            drawUpperArm();
            
            stack.push();
                stack.rotate(Vector3(0, 1, 0), elbowAngle);
                stack.translate(Vector3(0, -2, 0));  // Forearm
                drawForearm();
                
                stack.push();
                    stack.rotate(Vector3(0, 1, 0), wristAngle);
                    stack.translate(Vector3(0, -1, 0));  // Hand
                    drawHand();
                stack.pop();
            stack.pop();
        stack.pop();
    stack.pop();
}
```

## Hierarchical Transformations

### Scene Graph Node

```cpp
class SceneNode {
public:
    Transform localTransform;
    Matrix4 worldMatrix;
    SceneNode* parent;
    std::vector<SceneNode*> children;
    
    SceneNode() : parent(nullptr) {
        worldMatrix = Matrix4::identity();
    }
    
    void updateWorldMatrix() {
        if (parent) {
            worldMatrix = parent->worldMatrix * localTransform.toMatrix();
        } else {
            worldMatrix = localTransform.toMatrix();
        }
        
        for (auto* child : children) {
            child->updateWorldMatrix();
        }
    }
    
    void addChild(SceneNode* child) {
        child->parent = this;
        children.push_back(child);
    }
    
    Vector3 getWorldPosition() const {
        return worldMatrix.getTranslation();
    }
};
```

### Complete Skeleton System

```cpp
struct Bone {
    std::string name;
    int parentIndex;
    Matrix4 bindPose;          // Initial bone transform
    Matrix4 currentPose;       // Animated transform
    Matrix4 inverseBindPose;   // For skinning
};

class Skeleton {
    std::vector<Bone> bones;
    std::vector<Matrix4> worldMatrices;
    
public:
    void updateWorldMatrices() {
        worldMatrices.resize(bones.size());
        
        for (size_t i = 0; i < bones.size(); i++) {
            if (bones[i].parentIndex == -1) {
                worldMatrices[i] = bones[i].currentPose;
            } else {
                worldMatrices[i] = worldMatrices[bones[i].parentIndex] * 
                                   bones[i].currentPose;
            }
        }
    }
    
    Matrix4 getSkinningMatrix(int boneIndex) const {
        return worldMatrices[boneIndex] * bones[boneIndex].inverseBindPose;
    }
};
```

## Optimization Techniques

### Specialized Multiplication

```cpp
// Optimized for translation-only matrices
Matrix4 fastTranslateMultiply(const Matrix4& A, const Vector3& translation) {
    Matrix4 result = A;
    result.m[0][3] += translation.x;
    result.m[1][3] += translation.y;
    result.m[2][3] += translation.z;
    return result;
}

// Optimized for orthogonal matrices (rotations)
Matrix4 fastOrthogonalMultiply(const Matrix4& A, const Matrix4& B) {
    // Assuming both are orthogonal, we can optimize
    Matrix4 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.m[i][j] = 0;
            for (int k = 0; k < 3; k++) {
                result.m[i][j] += A.m[i][k] * B.m[k][j];
            }
        }
    }
    // Translation part
    for (int i = 0; i < 3; i++) {
        result.m[i][3] = A.m[i][3] + A.m[i][0] * B.m[0][3] + 
                         A.m[i][1] * B.m[1][3] + A.m[i][2] * B.m[2][3];
    }
    return result;
}
```

### SIMD Matrix Multiplication

```cpp
#ifdef USE_SSE
#include <xmmintrin.h>

Matrix4 simdMultiply(const Matrix4& A, const Matrix4& B) {
    Matrix4 result;
    
    for (int i = 0; i < 4; i++) {
        __m128 row = _mm_set_ps(A.m[i][3], A.m[i][2], A.m[i][1], A.m[i][0]);
        
        for (int j = 0; j < 4; j++) {
            __m128 col = _mm_set_ps(B.m[3][j], B.m[2][j], B.m[1][j], B.m[0][j]);
            __m128 prod = _mm_mul_ps(row, col);
            
            // Horizontal add
            prod = _mm_hadd_ps(prod, prod);
            prod = _mm_hadd_ps(prod, prod);
            
            _mm_store_ss(&result.m[i][j], prod);
        }
    }
    
    return result;
}
#endif
```

## Practical Examples

### Camera Follow System

```cpp
class FollowCamera {
    Vector3 offset;
    float smoothSpeed;
    
public:
    Matrix4 calculateViewMatrix(const Vector3& targetPos, 
                                const Vector3& targetForward) {
        // Calculate desired position
        Vector3 desiredPos = targetPos - targetForward * offset.z + 
                            Vector3::up() * offset.y;
        
        // Smooth movement
        static Vector3 currentPos = desiredPos;
        currentPos = Vector3::lerp(currentPos, desiredPos, smoothSpeed);
        
        // Create view matrix
        return Matrix4::lookAt(currentPos, targetPos, Vector3::up());
    }
};
```

### Billboard System

```cpp
Matrix4 createBillboard(const Vector3& position, const Vector3& cameraPos) {
    Vector3 forward = (cameraPos - position).normalized();
    Vector3 right = Vector3::up().cross(forward).normalized();
    Vector3 up = forward.cross(right);
    
    Matrix4 billboard;
    billboard.setColumn(0, Vector4(right, 0));
    billboard.setColumn(1, Vector4(up, 0));
    billboard.setColumn(2, Vector4(forward, 0));
    billboard.setColumn(3, Vector4(position, 1));
    
    return billboard;
}
```

## Key Takeaways

1. **Matrix multiplication** represents transformation composition
2. **Order is critical**: AB ≠ BA
3. **Matrix stacks** manage hierarchical transformations
4. **Scene graphs** use parent-child matrix relationships
5. **Skeletal animation** relies on matrix hierarchies
6. **Optimization** opportunities exist for special cases
7. **SIMD instructions** can accelerate multiplication
8. **Right-to-left** reading: C = A * B * v means apply v, then B, then A

## Exercises

1. Implement a complete scene graph with transform propagation
2. Create a spring-based smooth camera follow system
3. Write a skeletal animation player using matrix composition
4. Optimize matrix multiplication for your specific use case
5. Build a matrix debugging/visualization tool

---

**Lesson 08 Complete** - Next: Lesson 09 - Coordinate Systems and Spaces
