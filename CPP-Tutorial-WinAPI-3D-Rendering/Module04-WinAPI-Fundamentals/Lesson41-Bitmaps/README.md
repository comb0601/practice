# Lesson 41: Bitmaps

## Introduction
Learn to load, display, manipulate, and save bitmap images in Windows applications.

**Topics Covered:**
- Loading bitmap resources
- BitBlt for copying bitmaps
- StretchBlt for scaling
- Transparent blitting
- Device-independent bitmaps (DIB)
- BITMAP structure
- Creating bitmaps at runtime
- Saving bitmaps to files

**Time Required:** 4-5 hours

## Loading Bitmaps

### From Resources
```cpp
HBITMAP hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));

// Or LoadImage for more control
HBITMAP hBitmap = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP1),
                                      IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
```

### From File
```cpp
HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, L"image.bmp",
                                      IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
```

## Displaying Bitmaps with BitBlt

```cpp
case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    
    // Create compatible DC and select bitmap
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);
    
    // Copy bitmap to window
    BitBlt(hdc, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);
    
    // Cleanup
    SelectObject(hdcMem, hOldBitmap);
    DeleteDC(hdcMem);
    EndPaint(hwnd, &ps);
    return 0;
}
```

## Scaling with StretchBlt
```cpp
StretchBlt(hdc, 0, 0, newWidth, newHeight,
           hdcMem, 0, 0, originalWidth, originalHeight, SRCCOPY);
```

## Transparent Blitting
```cpp
// Using TransparentBlt (requires msimg32.lib)
TransparentBlt(hdc, x, y, width, height,
               hdcMem, 0, 0, width, height,
               RGB(255, 0, 255));  // Magenta = transparent
```

## Getting Bitmap Info
```cpp
BITMAP bm;
GetObject(hBitmap, sizeof(BITMAP), &bm);

// bm.bmWidth - Width in pixels
// bm.bmHeight - Height in pixels
// bm.bmBitsPixel - Bits per pixel
```

## Creating Bitmaps
```cpp
// Create compatible bitmap
HDC hdc = GetDC(hwnd);
HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);

// Draw on bitmap
HDC hdcMem = CreateCompatibleDC(hdc);
HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);
// Drawing operations on hdcMem
SelectObject(hdcMem, hOldBitmap);
DeleteDC(hdcMem);
ReleaseDC(hwnd, hdc);
```

## See Code Examples
8 complete bitmap demonstrations in Code/ folder.
