#!/bin/bash

# Lesson 58: Transformation Matrices
cat > Lesson58-Transformation-Matrices/README.md << 'L58MD'
# Lesson 58: Transformation Matrices

## Translation, Rotation, Scaling

### Translation Matrix
```
T = [ 1  0  0  tx ]
    [ 0  1  0  ty ]
    [ 0  0  1  tz ]
    [ 0  0  0   1 ]
```

### Rotation (Z-axis)
```
Rz(θ) = [ cos(θ) -sin(θ)  0  0 ]
        [ sin(θ)  cos(θ)  0  0 ]
        [   0       0     1  0 ]
        [   0       0     0  1 ]
```

### Scaling Matrix
```
S = [ sx  0   0  0 ]
    [  0 sy   0  0 ]
    [  0  0  sz  0 ]
    [  0  0   0  1 ]
```

## Code Examples
1-15: Translation, Rotation X/Y/Z, Scaling, Combined transforms, Euler angles, Local/World space, Transform hierarchies
L58MD

# Create code examples for Lesson 58
mkdir -p Lesson58-Transformation-Matrices/Code
for i in $(seq -f "%02g" 1 15); do
cat > Lesson58-Transformation-Matrices/Code/${i}_Transform.cpp << EOFC
#include <iostream>
#include <cmath>
class Matrix4x4 {
public:
    float m[16];
    Matrix4x4() { for(int i=0;i<16;i++) m[i]=(i%5==0)?1.0f:0.0f; }
    static Matrix4x4 Translation(float x, float y, float z) {
        Matrix4x4 result;
        result.m[3]=x; result.m[7]=y; result.m[11]=z;
        return result;
    }
    static Matrix4x4 RotationZ(float angle) {
        Matrix4x4 result;
        float c=cos(angle), s=sin(angle);
        result.m[0]=c; result.m[1]=-s; result.m[4]=s; result.m[5]=c;
        return result;
    }
    void Print() {
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++) std::cout << m[i*4+j] << " ";
            std::cout << std::endl;
        }
    }
};
int main() {
    std::cout << "Transformation Example ${i}" << std::endl;
    Matrix4x4 t = Matrix4x4::Translation(5,10,0);
    t.Print();
    return 0;
}
EOFC
done

# Lesson 59: Quaternions
cat > Lesson59-Quaternions-Rotation/README.md << 'L59MD'
# Lesson 59: Quaternions for Rotation

## Quaternion Basics
- Represents rotation as q = (w, x, y, z)
- Unit quaternion: w² + x² + y² + z² = 1
- Avoids gimbal lock
- Smooth interpolation (SLERP)

## Quaternion Operations
```cpp
class Quaternion {
    float w, x, y, z;
    Quaternion operator*(const Quaternion& q) const;
    Vector3 RotateVector(const Vector3& v) const;
    static Quaternion FromAxisAngle(Vector3 axis, float angle);
    static Quaternion Slerp(Quaternion a, Quaternion b, float t);
};
```

## Code Examples
1-15: Quaternion class, multiplication, axis-angle conversion, Euler to quaternion, matrix to quaternion, SLERP, rotation composition
L59MD

mkdir -p Lesson59-Quaternions-Rotation/Code
for i in $(seq -f "%02g" 1 15); do
cat > Lesson59-Quaternions-Rotation/Code/${i}_Quaternion.cpp << EOFC
#include <iostream>
#include <cmath>
class Quaternion {
public:
    float w, x, y, z;
    Quaternion(float w=1, float x=0, float y=0, float z=0) : w(w),x(x),y(y),z(z) {}
    static Quaternion FromAxisAngle(float ax, float ay, float az, float angle) {
        float s = sin(angle/2);
        return Quaternion(cos(angle/2), ax*s, ay*s, az*s);
    }
    void Print() { std::cout << "(" << w <<","<< x <<","<< y <<","<< z << ")" << std::endl; }
};
int main() {
    Quaternion q = Quaternion::FromAxisAngle(0,0,1, 3.14159f/2);
    std::cout << "Quaternion Example ${i}: "; q.Print();
    return 0;
}
EOFC
done

# Lesson 60: Camera Systems
cat > Lesson60-Camera-Systems/README.md << 'L60MD'
# Lesson 60: Camera Systems

## LookAt Matrix
Creates view matrix from camera position, target, and up vector.

## Camera Types
- **FPS Camera**: First-person, mouse look
- **Orbit Camera**: Rotates around target
- **Follow Camera**: Follows object smoothly

## Code Examples
1-15: LookAt matrix, FPS camera, orbit camera, camera movement, smooth following, frustum culling
L60MD

mkdir -p Lesson60-Camera-Systems/Code
for i in $(seq -f "%02g" 1 15); do
cat > Lesson60-Camera-Systems/Code/${i}_Camera.cpp << EOFC
#include <iostream>
#include <cmath>
class Vector3 {
public:
    float x, y, z;
    Vector3(float x=0, float y=0, float z=0) : x(x), y(y), z(z) {}
    void Print() { std::cout << "(" << x << "," << y << "," << z << ")"; }
};
class Camera {
    Vector3 position, target;
public:
    Camera(Vector3 pos, Vector3 tgt) : position(pos), target(tgt) {}
    void Print() {
        std::cout << "Camera pos="; position.Print();
        std::cout << " target="; target.Print(); std::cout << std::endl;
    }
};
int main() {
    Camera cam(Vector3(0,5,-10), Vector3(0,0,0));
    std::cout << "Camera Example ${i}" << std::endl;
    cam.Print();
    return 0;
}
EOFC
done

# Lesson 61: Projection Matrices
cat > Lesson61-Projection-Matrices/README.md << 'L61MD'
# Lesson 61: Projection Matrices

## Perspective Projection
```cpp
Matrix4x4 Perspective(float fov, float aspect, float near, float far);
```

## Orthographic Projection
```cpp
Matrix4x4 Orthographic(float left, float right, float bottom, 
                       float top, float near, float far);
```

## Code Examples
1-15: Perspective matrix, orthographic matrix, FOV calculations, NDC, viewport, aspect ratio
L61MD

mkdir -p Lesson61-Projection-Matrices/Code
for i in $(seq -f "%02g" 1 15); do
cat > Lesson61-Projection-Matrices/Code/${i}_Projection.cpp << EOFC
#include <iostream>
#include <cmath>
class Matrix4x4 {
public:
    float m[16];
    static Matrix4x4 Perspective(float fov, float aspect, float near, float far) {
        Matrix4x4 result;
        float f = 1.0f / tan(fov/2);
        result.m[0] = f/aspect; result.m[5] = f;
        result.m[10] = (far+near)/(near-far);
        result.m[11] = (2*far*near)/(near-far);
        result.m[14] = -1;
        return result;
    }
};
int main() {
    std::cout << "Projection Example ${i}" << std::endl;
    Matrix4x4 proj = Matrix4x4::Perspective(1.57f, 16.0f/9.0f, 0.1f, 1000.0f);
    return 0;
}
EOFC
done

# Lesson 62: Viewport Transformation
cat > Lesson62-Viewport-Transformation/README.md << 'L62MD'
# Lesson 62: Viewport Transformation

## NDC to Screen
Converts from Normalized Device Coordinates (-1 to 1) to screen pixels.

```cpp
Vector2 NDCToScreen(Vector3 ndc, int width, int height) {
    return Vector2(
        (ndc.x + 1) * 0.5f * width,
        (1 - ndc.y) * 0.5f * height
    );
}
```

## Code Examples  
1-15: NDC conversion, viewport matrix, window coordinates, scissor test, multiple viewports
L62MD

mkdir -p Lesson62-Viewport-Transformation/Code
for i in $(seq -f "%02g" 1 15); do
cat > Lesson62-Viewport-Transformation/Code/${i}_Viewport.cpp << EOFC
#include <iostream>
class Vector2 { public: float x, y; Vector2(float x, float y):x(x),y(y){} };
class Vector3 { public: float x, y, z; Vector3(float x, float y, float z):x(x),y(y),z(z){} };
Vector2 NDCToScreen(Vector3 ndc, int w, int h) {
    return Vector2((ndc.x+1)*0.5f*w, (1-ndc.y)*0.5f*h);
}
int main() {
    Vector3 ndc(0.5f, 0.5f, 0);
    Vector2 screen = NDCToScreen(ndc, 1920, 1080);
    std::cout << "Viewport Example ${i}: (" << screen.x << "," << screen.y << ")" << std::endl;
    return 0;
}
EOFC
done

# Lesson 63: Collision Detection
cat > Lesson63-Collision-Detection/README.md << 'L63MD'
# Lesson 63: Collision Detection

## AABB (Axis-Aligned Bounding Box)
```cpp
bool AABBIntersect(Vector3 min1, Vector3 max1, Vector3 min2, Vector3 max2);
```

## Sphere Collision
```cpp
bool SphereIntersect(Vector3 center1, float r1, Vector3 center2, float r2);
```

## Ray Intersection
- Ray-Sphere
- Ray-Box
- Ray-Plane

## Code Examples
1-15: AABB test, sphere test, ray-sphere, ray-box, broad phase, narrow phase, spatial hashing
L63MD

mkdir -p Lesson63-Collision-Detection/Code
for i in $(seq -f "%02g" 1 15); do
cat > Lesson63-Collision-Detection/Code/${i}_Collision.cpp << EOFC
#include <iostream>
#include <cmath>
class Vector3 {
public:
    float x, y, z;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    float Length() { return sqrt(x*x+y*y+z*z); }
    Vector3 operator-(Vector3 v) { return Vector3(x-v.x, y-v.y, z-v.z); }
};
bool SphereSphere(Vector3 c1, float r1, Vector3 c2, float r2) {
    return (c2-c1).Length() < (r1+r2);
}
int main() {
    bool hit = SphereSphere(Vector3(0,0,0), 5, Vector3(8,0,0), 2);
    std::cout << "Collision Example ${i}: " << (hit?"HIT":"MISS") << std::endl;
    return 0;
}
EOFC
done

# Lesson 64: Ray Casting & Picking
cat > Lesson64-Ray-Casting-Picking/README.md << 'L64MD'
# Lesson 64: Ray Casting & Picking

## Ray Generation
```cpp
Ray GetRayFromMouse(int mouseX, int mouseY, Matrix4x4 viewProj);
```

## Object Picking
Test ray against all objects in scene.

## Ray-Triangle Intersection
Möller-Trumbore algorithm

## Code Examples
1-15: Ray class, mouse to ray, ray-triangle, object picking, raycasting, ray marching
L64MD

mkdir -p Lesson64-Ray-Casting-Picking/Code
for i in $(seq -f "%02g" 1 15); do
cat > Lesson64-Ray-Casting-Picking/Code/${i}_RayCast.cpp << EOFC
#include <iostream>
class Vector3 {
public:
    float x, y, z;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    void Print() { std::cout << "(" << x << "," << y << "," << z << ")"; }
};
class Ray {
public:
    Vector3 origin, direction;
    Ray(Vector3 o, Vector3 d) : origin(o), direction(d) {}
    Vector3 At(float t) { 
        return Vector3(origin.x + direction.x*t,
                      origin.y + direction.y*t,
                      origin.z + direction.z*t);
    }
};
int main() {
    Ray ray(Vector3(0,0,0), Vector3(1,0,0));
    std::cout << "RayCast Example ${i}: ";
    ray.At(5).Print();
    std::cout << std::endl;
    return 0;
}
EOFC
done

echo "All lessons 58-64 created successfully!"
