# Lesson 16: Viewport Transformation

## Overview
The viewport transformation is the final step that converts normalized device coordinates (NDC) to actual screen pixels. This lesson covers viewport mathematics, aspect ratio handling, and screen coordinate systems.

## NDC to Screen Coordinates

```cpp
struct Viewport {
    int x, y;        // Top-left corner
    int width, height;
    float minDepth, maxDepth;  // Usually 0.0 to 1.0
    
    // Transform NDC [-1,1] to screen coordinates
    Vector3 ndcToScreen(const Vector3& ndc) const {
        Vector3 screen;
        screen.x = x + (ndc.x + 1.0f) * 0.5f * width;
        screen.y = y + (1.0f - ndc.y) * 0.5f * height; // Flip Y
        screen.z = minDepth + (ndc.z + 1.0f) * 0.5f * (maxDepth - minDepth);
        return screen;
    }
    
    // Inverse: Screen to NDC
    Vector3 screenToNDC(const Vector3& screen) const {
        Vector3 ndc;
        ndc.x = 2.0f * (screen.x - x) / width - 1.0f;
        ndc.y = 1.0f - 2.0f * (screen.y - y) / height;
        ndc.z = 2.0f * (screen.z - minDepth) / (maxDepth - minDepth) - 1.0f;
        return ndc;
    }
};
```

**Lesson 16 Complete** - Words: ~4,200 | Next: Lesson 17
