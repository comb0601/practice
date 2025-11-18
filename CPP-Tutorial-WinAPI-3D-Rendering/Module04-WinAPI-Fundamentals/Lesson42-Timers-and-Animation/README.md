# Lesson 42: Timers and Animation

## Introduction
Learn to create smooth animations and time-based operations using Windows timers.

**Topics Covered:**
- SetTimer for periodic events
- WM_TIMER message handling
- Simple animation loops
- Smooth movement and interpolation
- High-resolution timing (QueryPerformanceCounter)
- Frame rate calculation and limiting
- Sprite animation
- Game-style update loops

**Time Required:** 4-5 hours

## Basic Timer

### Creating Timer
```cpp
// Create timer with 50ms interval (20 FPS)
UINT_PTR timerID = SetTimer(hwnd, 1, 50, NULL);
```

### Handling WM_TIMER
```cpp
case WM_TIMER: {
    if (wParam == 1) {
        // Update animation
        position.x += velocity.x;
        position.y += velocity.y;
        
        // Redraw
        InvalidateRect(hwnd, NULL, FALSE);
    }
    return 0;
}
```

### Destroying Timer
```cpp
case WM_DESTROY:
    KillTimer(hwnd, 1);
    PostQuitMessage(0);
    return 0;
```

## Simple Animation
```cpp
static int xPos = 0;
static int direction = 1;

case WM_TIMER: {
    xPos += direction * 5;
    if (xPos > 500 || xPos < 0) direction *= -1;
    
    InvalidateRect(hwnd, NULL, FALSE);
    return 0;
}

case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    
    Ellipse(hdc, xPos, 100, xPos + 50, 150);
    
    EndPaint(hwnd, &ps);
    return 0;
}
```

## High-Resolution Timing
```cpp
LARGE_INTEGER frequency, start, end;
QueryPerformanceFrequency(&frequency);
QueryPerformanceCounter(&start);

// ... do work ...

QueryPerformanceCounter(&end);
double elapsedSeconds = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
```

## Frame Rate Calculation
```cpp
static DWORD lastTime = 0;
static int frameCount = 0;
static float fps = 0.0f;

case WM_TIMER: {
    frameCount++;
    DWORD currentTime = GetTickCount();
    
    if (currentTime - lastTime >= 1000) {
        fps = frameCount * 1000.0f / (currentTime - lastTime);
        frameCount = 0;
        lastTime = currentTime;
    }
    
    // Display FPS
    InvalidateRect(hwnd, NULL, FALSE);
    return 0;
}
```

## Smooth Movement (Interpolation)
```cpp
float Lerp(float start, float end, float t) {
    return start + (end - start) * t;
}

// In animation update
float t = 0.0f;  // 0 to 1
float currentX = Lerp(startX, endX, t);
t += 0.01f;  // Increment each frame
```

## See Code Examples
8 complete timer and animation demonstrations in Code/ folder.
