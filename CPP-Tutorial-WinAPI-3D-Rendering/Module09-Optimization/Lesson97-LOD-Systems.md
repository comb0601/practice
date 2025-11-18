# Lesson 97: Level of Detail (LOD) Systems
**Estimated Time: 3-4 hours**

## Table of Contents
1. [Introduction to LOD](#introduction)
2. [Distance-Based LOD](#distance-based-lod)
3. [LOD Generation Techniques](#lod-generation)
4. [Smooth LOD Transitions](#lod-transitions)
5. [Occlusion Culling](#occlusion-culling)
6. [Frustum Culling](#frustum-culling)
7. [LOD Management System](#lod-management)
8. [Complete Examples](#complete-examples)
9. [Exercises](#exercises)

## Introduction to LOD

**Level of Detail (LOD)**: Render simpler versions of objects based on distance or importance.

### Why LOD?

```
Close object (50m):   10,000 triangles - Visible detail
Far object (500m):    100 triangles - Detail not visible anyway

Without LOD: 10,000 triangles (wasted)
With LOD:    100 triangles (10ms GPU time saved!)
```

### LOD Benefits

- **Reduces polygon count** (less GPU work)
- **Improves framerate**
- **Allows larger scenes**
- **Better cache efficiency**

## Distance-Based LOD

### Basic Concept

```
LOD 0 (highest): 0-100m    10,000 triangles
LOD 1 (medium):  100-300m  2,500 triangles
LOD 2 (low):     300-500m  625 triangles
LOD 3 (lowest):  500m+     156 triangles
```

### Implementation

```cpp
struct LODLevel {
    Mesh* mesh;
    float maxDistance;  // Switch to next LOD beyond this
    int triangleCount;
};

class LODMesh {
private:
    std::vector<LODLevel> levels;

public:
    void AddLevel(Mesh* mesh, float maxDistance) {
        levels.push_back({ mesh, maxDistance, mesh->GetTriangleCount() });
    }

    Mesh* GetLOD(float distanceToCamera) {
        for (const LODLevel& level : levels) {
            if (distanceToCamera <= level.maxDistance) {
                return level.mesh;
            }
        }
        return levels.back().mesh;  // Use lowest LOD
    }

    int GetActiveLODIndex(float distance) {
        for (size_t i = 0; i < levels.size(); ++i) {
            if (distance <= levels[i].maxDistance) {
                return i;
            }
        }
        return levels.size() - 1;
    }
};
```

### Usage

```cpp
void RenderObject(GameObject* obj, const Vector3& cameraPos) {
    float distance = Distance(obj->position, cameraPos);

    Mesh* lodMesh = obj->lodMesh->GetLOD(distance);

    Draw(lodMesh, obj->transform);
}
```

## LOD Generation Techniques

### 1. Manual LOD Creation

Artist creates each LOD level:
- LOD 0: Full detail model
- LOD 1: 50% polygons
- LOD 2: 25% polygons
- LOD 3: 10% polygons

**Pros:** Best quality, artist control
**Cons:** Time-consuming, expensive

### 2. Automatic Mesh Simplification

**Edge Collapse Algorithm:**

```cpp
Mesh* SimplifyMesh(Mesh* original, float targetRatio) {
    // targetRatio: 0.5 = half the triangles

    while (mesh->GetTriangleCount() > targetCount) {
        // Find edge with lowest "collapse cost"
        Edge* edge = FindCheapestEdge(mesh);

        // Collapse edge (merge two vertices into one)
        CollapseEdge(mesh, edge);

        // Update affected triangles
        UpdateAdjacent(mesh, edge);
    }

    return mesh;
}
```

**Cost Calculation:**
- Distance from edge to original surface
- Triangle shape quality
- UV seam preservation
- Material boundary preservation

### 3. Quadric Error Metrics

**Garland-Heckbert Algorithm:**

```cpp
float CalculateCollapseError(Edge* edge) {
    // Measure how much geometry changes
    Quadric Q = edge->v1->quadric + edge->v2->quadric;

    Vector3 optimalPos = Q.OptimalPosition();
    float error = Q.Evaluate(optimalPos);

    return error;
}
```

**Better quality** than simple edge collapse.

### 4. Clustering/Aggregation

For very far distances, replace many objects with one simple mesh:

```
1000 grass blades (near) → 1 grass patch quad (far)
100 trees (near)         → 1 forest billboard (far)
```

## Smooth LOD Transitions

### The Popping Problem

```
Frame 299: LOD 0 (10,000 tris) ────┐
Frame 300: LOD 1 (2,500 tris)  ────┘  ← Visual "pop"!
```

### Solutions

**1. Discrete LOD (Simple)**
```cpp
int lodIndex = GetLODIndex(distance);
DrawMesh(lods[lodIndex]);
// Fast but visible popping
```

**2. Geomorphing (Smooth)**
```cpp
int lodIndex = GetLODIndex(distance);
int nextLODIndex = lodIndex + 1;

float transitionStart = lods[lodIndex].maxDistance * 0.9f;
float transitionEnd = lods[lodIndex].maxDistance;

float blend = (distance - transitionStart) / (transitionEnd - transitionStart);
blend = Clamp(blend, 0.0f, 1.0f);

// Vertex shader morphs between LOD levels
Draw(lods[lodIndex], lods[nextLODIndex], blend);
```

**Vertex Shader:**
```hlsl
VS_OUTPUT main(VS_INPUT input, VS_INPUT nextLOD) {
    float3 pos = lerp(input.position, nextLOD.position, lodBlend);
    // ... transform pos
}
```

**3. Alpha Fading**
```cpp
// Fade out current LOD
Draw(currentLOD, alpha = 1.0f - blend);

// Fade in next LOD
Draw(nextLOD, alpha = blend);
```

**4. Dithered Transitions**
```cpp
// Pixel shader uses dithering pattern
if (dither[pixelPos % 4] > blend) {
    discard;  // Use current LOD pixel
} else {
    // Use next LOD pixel
}
```

## Occlusion Culling

### What is Occlusion Culling?

Don't render objects hidden behind other objects:

```
     Camera
        |
        |  Building (blocks view)
        |    |
        |    |  Trees (hidden)
        v    v  vvv

Only render building, not trees!
```

### Hardware Occlusion Queries

**DirectX 11:**

```cpp
void SetupOcclusionQuery(ID3D11Device* device) {
    D3D11_QUERY_DESC queryDesc;
    queryDesc.Query = D3D11_QUERY_OCCLUSION;
    queryDesc.MiscFlags = 0;
    device->CreateQuery(&queryDesc, &occlusionQuery);
}

bool IsObjectVisible(GameObject* obj, ID3D11DeviceContext* context) {
    // Render bounding box with occlusion query
    context->Begin(occlusionQuery);
    DrawBoundingBox(obj->bounds);
    context->End(occlusionQuery);

    // Get result (may stall pipeline!)
    UINT64 visiblePixels;
    while (context->GetData(occlusionQuery, &visiblePixels,
           sizeof(UINT64), 0) == S_FALSE) {
        // Wait
    }

    return visiblePixels > 0;
}
```

### Hierarchical Z-Buffer (HZB)

**Concept:**
```
Full resolution depth: 1024×1024
Level 1 (max):         512×512   (max of 2×2 blocks)
Level 2 (max):         256×256   (max of 2×2 blocks)
...
Level 10 (max):        1×1
```

**Test object:**
```cpp
bool IsVisibleHZB(BoundingBox box, Texture2D hzbuffer) {
    // Project box to screen space
    Rect screenRect = ProjectToScreen(box);

    // Select HZB mip level
    int mipLevel = GetMipLevelForSize(screenRect);

    // Sample HZB
    float occluderDepth = hzbuffer.Sample(mipLevel, screenRect.center);

    // Compare depths
    return box.minDepth < occluderDepth;
}
```

**Benefits:**
- Fast (GPU-based)
- No CPU stalls
- Works for any object size

## Frustum Culling

### View Frustum

Camera view is a truncated pyramid:

```
         Near plane
        .-´  `-.
       /        \
      /          \
     /            \
    /     Far      \
   ´-plane----------´

Objects outside frustum = not visible!
```

### Frustum Representation

```cpp
struct Frustum {
    Plane planes[6];  // Near, Far, Left, Right, Top, Bottom
};

Frustum ExtractFrustum(const Matrix& viewProjection) {
    Frustum frustum;

    // Extract planes from view-projection matrix
    // Left plane: row3 + row0
    frustum.planes[0] = NormalizePlane(
        viewProjection.row3 + viewProjection.row0);

    // Right plane: row3 - row0
    frustum.planes[1] = NormalizePlane(
        viewProjection.row3 - viewProjection.row0);

    // Top plane: row3 - row1
    frustum.planes[2] = NormalizePlane(
        viewProjection.row3 - viewProjection.row1);

    // Bottom plane: row3 + row1
    frustum.planes[3] = NormalizePlane(
        viewProjection.row3 + viewProjection.row1);

    // Near plane: row3 + row2
    frustum.planes[4] = NormalizePlane(
        viewProjection.row3 + viewProjection.row2);

    // Far plane: row3 - row2
    frustum.planes[5] = NormalizePlane(
        viewProjection.row3 - viewProjection.row2);

    return frustum;
}
```

### Bounding Volume Tests

**Sphere Test (Fast):**
```cpp
bool IsInFrustum(const Sphere& sphere, const Frustum& frustum) {
    for (const Plane& plane : frustum.planes) {
        float distance = Dot(plane.normal, sphere.center) + plane.distance;
        if (distance < -sphere.radius) {
            return false;  // Completely outside
        }
    }
    return true;
}
```

**AABB Test (Accurate):**
```cpp
bool IsInFrustum(const AABB& box, const Frustum& frustum) {
    for (const Plane& plane : frustum.planes) {
        // Find point on box closest to plane
        Vector3 positiveVertex = box.min;
        if (plane.normal.x >= 0) positiveVertex.x = box.max.x;
        if (plane.normal.y >= 0) positiveVertex.y = box.max.y;
        if (plane.normal.z >= 0) positiveVertex.z = box.max.z;

        // Test if closest point is inside
        if (Dot(plane.normal, positiveVertex) + plane.distance < 0) {
            return false;
        }
    }
    return true;
}
```

## LOD Management System

### Complete LOD Manager

```cpp
class LODManager {
private:
    std::vector<LODObject*> objects;
    Frustum cameraFrustum;
    Vector3 cameraPosition;

public:
    void Update(const Camera& camera) {
        cameraPosition = camera.GetPosition();
        cameraFrustum = ExtractFrustum(camera.GetViewProjection());
    }

    void Render() {
        // Frustum culling and LOD selection
        for (LODObject* obj : objects) {
            // Frustum cull
            if (!IsInFrustum(obj->bounds, cameraFrustum)) {
                continue;
            }

            // Calculate distance
            float distance = Distance(obj->position, cameraPosition);

            // Select LOD
            Mesh* lodMesh = obj->SelectLOD(distance);

            // Render
            Draw(lodMesh, obj->transform);
        }
    }

    void AddObject(LODObject* obj) {
        objects.push_back(obj);
    }

    void GetStatistics(LODStats& stats) {
        stats.totalObjects = objects.size();
        stats.renderedObjects = 0;
        stats.culledObjects = 0;
        stats.trianglesRendered = 0;

        for (LODObject* obj : objects) {
            if (IsInFrustum(obj->bounds, cameraFrustum)) {
                stats.renderedObjects++;
                float dist = Distance(obj->position, cameraPosition);
                Mesh* lod = obj->SelectLOD(dist);
                stats.trianglesRendered += lod->GetTriangleCount();
            } else {
                stats.culledObjects++;
            }
        }
    }
};
```

## Performance Impact

### Example Scene (10,000 trees)

```
No LOD:
  - 10,000 objects × 10,000 tris = 100M triangles
  - GPU time: 50ms
  - FPS: 20

With LOD:
  - 1,000 close × 10,000 tris = 10M triangles
  - 5,000 medium × 2,500 tris = 12.5M triangles
  - 4,000 far × 625 tris = 2.5M triangles
  - Total: 25M triangles
  - GPU time: 12.5ms
  - FPS: 80

Speedup: 4x!
```

## Summary

**Key Techniques:**
1. **Distance-based LOD** - Simple and effective
2. **Frustum culling** - Don't render off-screen objects
3. **Occlusion culling** - Don't render hidden objects
4. **Smooth transitions** - Avoid popping
5. **Automatic simplification** - Generate LODs programmatically

**Best Practices:**
- 3-4 LOD levels sufficient
- Transition zones: 10-20% of distance range
- Combine LOD with instancing
- Profile to find optimal distances

---

**Congratulations on completing Lesson 97!**
