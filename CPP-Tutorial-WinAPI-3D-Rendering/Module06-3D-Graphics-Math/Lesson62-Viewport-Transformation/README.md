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
