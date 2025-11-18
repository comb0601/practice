# Lesson 45: Advanced GDI

## Introduction
Explore advanced GDI techniques for sophisticated graphics.

**Topics Covered:**
- Paths (BeginPath, EndPath, StrokePath, FillPath)
- Regions for complex clipping
- Transformations (rotation, scaling, shearing)
- Alpha blending
- Gradient fills
- Antialiased drawing
- Bezier curves
- Complex clipping regions

**Time Required:** 4-5 hours

## Paths
```cpp
BeginPath(hdc);
MoveToEx(hdc, 100, 100, NULL);
LineTo(hdc, 200, 200);
LineTo(hdc, 300, 100);
CloseFigure(hdc);
EndPath(hdc);

// Stroke or fill the path
StrokePath(hdc);  // Or FillPath(hdc)
```

## Regions
```cpp
HRGN hRgn1 = CreateRectRgn(10, 10, 100, 100);
HRGN hRgn2 = CreateEllipticRgn(50, 50, 150, 150);
HRGN hRgnCombined = CreateRectRgn(0, 0, 1, 1);
CombineRgn(hRgnCombined, hRgn1, hRgn2, RGN_OR);

SelectClipRgn(hdc, hRgnCombined);
// Drawing is now clipped to combined region
```

## Transformations
```cpp
XFORM xform;
xform.eM11 = cos(angle);
xform.eM12 = sin(angle);
xform.eM21 = -sin(angle);
xform.eM22 = cos(angle);
xform.eDx = centerX;
xform.eDy = centerY;

SetGraphicsMode(hdc, GM_ADVANCED);
SetWorldTransform(hdc, &xform);
// Drawing is now rotated
```

## Alpha Blending
```cpp
BLENDFUNCTION blend = {AC_SRC_OVER, 0, 128, 0};  // 50% alpha
AlphaBlend(hdc, x, y, width, height,
           hdcSrc, 0, 0, width, height, blend);
```

## See Code Examples
8 complete advanced GDI demonstrations in Code/ folder.
