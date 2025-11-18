# Lesson 46: Double Buffering

## Introduction
Eliminate flicker with double buffering techniques.

**Topics Covered:**
- Basic double buffering concept
- Creating memory DCs
- Flicker-free rendering
- Handling WM_ERASEBKGND
- Triple buffering
- Dirty rectangle optimization
- Buffer resizing
- Performance optimization

**Time Required:** 4-5 hours

## Basic Double Buffering
```cpp
case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    
    RECT rc;
    GetClientRect(hwnd, &rc);
    
    // Create memory DC and bitmap
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);
    
    // Draw to memory DC
    FillRect(hdcMem, &rc, (HBRUSH)(COLOR_WINDOW+1));
    // ... more drawing ...
    
    // Copy to screen (single operation = no flicker)
    BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, SRCCOPY);
    
    // Cleanup
    SelectObject(hdcMem, hbmOld);
    DeleteObject(hbmMem);
    DeleteDC(hdcMem);
    EndPaint(hwnd, &ps);
    return 0;
}

case WM_ERASEBKGND:
    return 1;  // Don't erase, we'll paint everything
```

## Persistent Buffer
```cpp
static HDC hdcBuffer = NULL;
static HBITMAP hbmBuffer = NULL;

case WM_SIZE: {
    // Recreate buffer on resize
    if (hdcBuffer) {
        SelectObject(hdcBuffer, hbmOldBuffer);
        DeleteObject(hbmBuffer);
        DeleteDC(hdcBuffer);
    }
    
    HDC hdc = GetDC(hwnd);
    hdcBuffer = CreateCompatibleDC(hdc);
    hbmBuffer = CreateCompatibleBitmap(hdc, LOWORD(lParam), HIWORD(lParam));
    hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);
    ReleaseDC(hwnd, hdc);
    return 0;
}
```

## See Code Examples
8 complete double buffering demonstrations in Code/ folder.
