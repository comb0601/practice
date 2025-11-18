# Lesson 22: OpenGL Context and Window Creation (GLFW)

## Table of Contents
1. [Understanding OpenGL Contexts](#understanding-opengl-contexts)
2. [Introduction to GLFW](#introduction-to-glfw)
3. [GLFW Initialization and Configuration](#glfw-initialization-and-configuration)
4. [Window Creation and Management](#window-creation-and-management)
5. [Window Hints and Attributes](#window-hints-and-attributes)
6. [Input Handling](#input-handling)
7. [Event Processing](#event-processing)
8. [Multiple Windows and Contexts](#multiple-windows-and-contexts)
9. [Complete Window Manager Implementation](#complete-window-manager-implementation)
10. [Advanced Topics](#advanced-topics)

## Understanding OpenGL Contexts

### What is an OpenGL Context?

An OpenGL context is a state machine that stores all the OpenGL state information. It contains:
- Current shader programs
- Texture bindings
- Buffer bindings
- Rendering states (blending, depth testing, etc.)
- Viewport and framebuffer configurations

Think of a context as a "rendering environment" where all OpenGL commands execute.

### Why Do We Need Contexts?

**Thread Safety**: Each thread can have its own context, preventing conflicts.

**Resource Management**: Contexts manage GPU resources like textures and buffers.

**Window Binding**: Each window needs a context to render OpenGL content.

**State Isolation**: Different contexts maintain separate OpenGL states.

### Context Types

**Core Profile**: Modern OpenGL only, deprecated functions removed.
```cpp
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
```

**Compatibility Profile**: Includes legacy OpenGL functions.
```cpp
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
```

**Forward Compatible**: Removes deprecated features.
```cpp
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
```

## Introduction to GLFW

### What is GLFW?

GLFW (Graphics Library Framework) is a lightweight, multi-platform library for:
- Creating windows with OpenGL contexts
- Managing input from keyboard, mouse, and joysticks
- Handling window events (resize, close, focus, etc.)
- Managing multiple monitors
- High-resolution timer access

### GLFW vs Other Libraries

**GLFW vs SDL2**:
- GLFW: Focused on OpenGL, lightweight, simple API
- SDL2: General multimedia library, includes audio, networking

**GLFW vs GLUT**:
- GLFW: Modern, actively maintained, flexible
- GLUT: Legacy, deprecated, limited functionality

**GLFW Advantages**:
- Cross-platform (Windows, Linux, macOS)
- Simple and clean API
- Well-documented
- Active development and community support
- No external dependencies

## GLFW Initialization and Configuration

### Basic Initialization

```cpp
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    // Initialize GLFW library
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set error callback before initialization
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
    });

    // Your code here...

    // Cleanup
    glfwTerminate();
    return 0;
}
```

### Error Handling

```cpp
// Error callback function
void glfwErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error [" << error << "]: " << description << std::endl;
}

int main() {
    // Set error callback BEFORE glfwInit()
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Continue...
}
```

### Version Information

```cpp
int major, minor, revision;
glfwGetVersion(&major, &minor, &revision);
std::cout << "GLFW Version: " << major << "." << minor << "." << revision << std::endl;

const char* versionString = glfwGetVersionString();
std::cout << "GLFW Version String: " << versionString << std::endl;
```

## Window Creation and Management

### Creating a Basic Window

```cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Configure OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    GLFWwindow* window = glfwCreateWindow(
        800,                    // Width
        600,                    // Height
        "My OpenGL Window",     // Title
        nullptr,                // Monitor (nullptr = windowed mode)
        nullptr                 // Share (nullptr = don't share resources)
    );

    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the OpenGL context current
    glfwMakeContextCurrent(window);

    // Load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Rendering...
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
```

### Fullscreen Windows

```cpp
// Get primary monitor
GLFWmonitor* monitor = glfwGetPrimaryMonitor();

// Get video mode
const GLFWvidmode* mode = glfwGetVideoMode(monitor);

// Create fullscreen window
GLFWwindow* window = glfwCreateWindow(
    mode->width,
    mode->height,
    "Fullscreen Window",
    monitor,     // Pass monitor for fullscreen
    nullptr
);
```

### Borderless Fullscreen Window

```cpp
GLFWmonitor* monitor = glfwGetPrimaryMonitor();
const GLFWvidmode* mode = glfwGetVideoMode(monitor);

glfwWindowHint(GLFW_RED_BITS, mode->redBits);
glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

GLFWwindow* window = glfwCreateWindow(
    mode->width,
    mode->height,
    "Borderless Fullscreen",
    monitor,
    nullptr
);
```

### Window Properties

```cpp
// Get window size
int width, height;
glfwGetWindowSize(window, &width, &height);
std::cout << "Window size: " << width << "x" << height << std::endl;

// Get framebuffer size (actual pixel count)
int fbWidth, fbHeight;
glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
std::cout << "Framebuffer size: " << fbWidth << "x" << fbHeight << std::endl;

// Set window position
glfwSetWindowPos(window, 100, 100);

// Set window size
glfwSetWindowSize(window, 1024, 768);

// Set window title
glfwSetWindowTitle(window, "New Title");

// Hide/Show window
glfwHideWindow(window);
glfwShowWindow(window);

// Iconify (minimize) window
glfwIconifyWindow(window);

// Restore window
glfwRestoreWindow(window);

// Maximize window
glfwMaximizeWindow(window);
```

## Window Hints and Attributes

### Common Window Hints

```cpp
// Must be set BEFORE glfwCreateWindow()

// OpenGL version
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

// OpenGL profile
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

// Resizable window
glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

// Visible window
glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

// Decorated (has title bar and borders)
glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

// Focused on creation
glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

// Always on top
glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);

// Maximized on creation
glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);

// Multi-sampling (anti-aliasing)
glfwWindowHint(GLFW_SAMPLES, 4);

// sRGB framebuffer
glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

// Double buffering
glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
```

### Framebuffer Hints

```cpp
// Color bits
glfwWindowHint(GLFW_RED_BITS, 8);
glfwWindowHint(GLFW_GREEN_BITS, 8);
glfwWindowHint(GLFW_BLUE_BITS, 8);
glfwWindowHint(GLFW_ALPHA_BITS, 8);

// Depth buffer bits
glfwWindowHint(GLFW_DEPTH_BITS, 24);

// Stencil buffer bits
glfwWindowHint(GLFW_STENCIL_BITS, 8);
```

## Input Handling

### Keyboard Input

```cpp
// Polling method
void processInput(GLFWwindow* window) {
    // Escape key closes window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // WASD movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        std::cout << "W pressed - Move forward" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        std::cout << "S pressed - Move backward" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        std::cout << "A pressed - Move left" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        std::cout << "D pressed - Move right" << std::endl;
    }
}

// Call in main loop
while (!glfwWindowShouldClose(window)) {
    processInput(window);
    // ...
}
```

### Keyboard Callback

```cpp
// Key callback for discrete key events
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        std::cout << "Key pressed: " << key << std::endl;
    } else if (action == GLFW_RELEASE) {
        std::cout << "Key released: " << key << std::endl;
    } else if (action == GLFW_REPEAT) {
        std::cout << "Key repeated: " << key << std::endl;
    }

    // Check modifiers
    if (mods & GLFW_MOD_SHIFT) {
        std::cout << "Shift modifier active" << std::endl;
    }
    if (mods & GLFW_MOD_CONTROL) {
        std::cout << "Control modifier active" << std::endl;
    }
    if (mods & GLFW_MOD_ALT) {
        std::cout << "Alt modifier active" << std::endl;
    }
}

// Register callback
glfwSetKeyCallback(window, keyCallback);
```

### Mouse Input

```cpp
// Mouse position
double xpos, ypos;
glfwGetCursorPos(window, &xpos, &ypos);
std::cout << "Mouse position: (" << xpos << ", " << ypos << ")" << std::endl;

// Mouse button state
int leftButton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
int rightButton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
int middleButton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);

if (leftButton == GLFW_PRESS) {
    std::cout << "Left mouse button pressed" << std::endl;
}
```

### Mouse Callbacks

```cpp
// Mouse position callback
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    std::cout << "Mouse moved to: (" << xpos << ", " << ypos << ")" << std::endl;
}

// Mouse button callback
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout << "Left click at: (" << xpos << ", " << ypos << ")" << std::endl;
    }
}

// Mouse scroll callback
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    std::cout << "Scroll: (" << xoffset << ", " << yoffset << ")" << std::endl;
}

// Register callbacks
glfwSetCursorPosCallback(window, cursorPositionCallback);
glfwSetMouseButtonCallback(window, mouseButtonCallback);
glfwSetScrollCallback(window, scrollCallback);
```

### Cursor Modes

```cpp
// Normal cursor
glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

// Hidden cursor (but still works)
glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

// Disabled cursor (for FPS camera)
glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
```

## Event Processing

### Event Processing Methods

```cpp
// Poll events (non-blocking)
glfwPollEvents();  // Processes events immediately and returns

// Wait for events (blocking)
glfwWaitEvents();  // Blocks until at least one event is available

// Wait with timeout
glfwWaitEventsTimeout(0.5);  // Wait up to 0.5 seconds

// Post empty event (wake up waiting thread)
glfwPostEmptyEvent();
```

### Window Callbacks

```cpp
// Window resize callback
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    std::cout << "Framebuffer resized to: " << width << "x" << height << std::endl;
    glViewport(0, 0, width, height);
}

// Window close callback
void windowCloseCallback(GLFWwindow* window) {
    std::cout << "Window about to close" << std::endl;
    // Return without setting glfwSetWindowShouldClose to prevent closing
}

// Window focus callback
void windowFocusCallback(GLFWwindow* window, int focused) {
    if (focused) {
        std::cout << "Window gained focus" << std::endl;
    } else {
        std::cout << "Window lost focus" << std::endl;
    }
}

// Window iconify callback
void windowIconifyCallback(GLFWwindow* window, int iconified) {
    if (iconified) {
        std::cout << "Window minimized" << std::endl;
    } else {
        std::cout << "Window restored" << std::endl;
    }
}

// Register callbacks
glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
glfwSetWindowCloseCallback(window, windowCloseCallback);
glfwSetWindowFocusCallback(window, windowFocusCallback);
glfwSetWindowIconifyCallback(window, windowIconifyCallback);
```

## Multiple Windows and Contexts

### Creating Multiple Windows

```cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create first window
    GLFWwindow* window1 = glfwCreateWindow(400, 300, "Window 1", nullptr, nullptr);
    if (!window1) {
        glfwTerminate();
        return -1;
    }

    // Create second window (shares resources with window1)
    GLFWwindow* window2 = glfwCreateWindow(400, 300, "Window 2", nullptr, window1);
    if (!window2) {
        glfwDestroyWindow(window1);
        glfwTerminate();
        return -1;
    }

    // Position windows side by side
    glfwSetWindowPos(window1, 100, 100);
    glfwSetWindowPos(window2, 520, 100);

    // Initialize OpenGL for window1
    glfwMakeContextCurrent(window1);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(window1);
        glfwDestroyWindow(window2);
        glfwTerminate();
        return -1;
    }

    // Main loop
    while (!glfwWindowShouldClose(window1) && !glfwWindowShouldClose(window2)) {
        // Render window 1
        glfwMakeContextCurrent(window1);
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window1);

        // Render window 2
        glfwMakeContextCurrent(window2);
        glClearColor(0.4f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window2);

        glfwPollEvents();
    }

    glfwDestroyWindow(window1);
    glfwDestroyWindow(window2);
    glfwTerminate();

    return 0;
}
```

## Complete Window Manager Implementation

```cpp
// WindowManager.h
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <functional>
#include <iostream>

class WindowManager {
public:
    struct WindowConfig {
        int width = 800;
        int height = 600;
        std::string title = "OpenGL Window";
        bool fullscreen = false;
        bool resizable = true;
        bool vsync = true;
        int samples = 4;  // MSAA samples
    };

    WindowManager() : window(nullptr) {}

    ~WindowManager() {
        cleanup();
    }

    bool initialize(const WindowConfig& config = WindowConfig()) {
        this->config = config;

        // Initialize GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }

        // Set error callback
        glfwSetErrorCallback([](int error, const char* description) {
            std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
        });

        // Configure GLFW
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, config.resizable ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_SAMPLES, config.samples);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // Create window
        GLFWmonitor* monitor = config.fullscreen ? glfwGetPrimaryMonitor() : nullptr;
        window = glfwCreateWindow(
            config.width,
            config.height,
            config.title.c_str(),
            monitor,
            nullptr
        );

        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }

        // Make context current
        glfwMakeContextCurrent(window);

        // Set VSync
        glfwSwapInterval(config.vsync ? 1 : 0);

        // Load OpenGL functions
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            glfwDestroyWindow(window);
            glfwTerminate();
            return false;
        }

        // Set up callbacks
        setupCallbacks();

        // Print OpenGL info
        printOpenGLInfo();

        // Set initial viewport
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        return true;
    }

    void setKeyCallback(std::function<void(int, int, int, int)> callback) {
        keyCallback = callback;
    }

    void setMouseButtonCallback(std::function<void(int, int, int)> callback) {
        mouseButtonCallback = callback;
    }

    void setCursorPosCallback(std::function<void(double, double)> callback) {
        cursorPosCallback = callback;
    }

    void setScrollCallback(std::function<void(double, double)> callback) {
        scrollCallback = callback;
    }

    void setResizeCallback(std::function<void(int, int)> callback) {
        resizeCallback = callback;
    }

    bool shouldClose() const {
        return glfwWindowShouldClose(window);
    }

    void setShouldClose(bool value) {
        glfwSetWindowShouldClose(window, value);
    }

    void swapBuffers() {
        glfwSwapBuffers(window);
    }

    void pollEvents() {
        glfwPollEvents();
    }

    void waitEvents() {
        glfwWaitEvents();
    }

    GLFWwindow* getWindow() const {
        return window;
    }

    void getWindowSize(int& width, int& height) const {
        glfwGetWindowSize(window, &width, &height);
    }

    void getFramebufferSize(int& width, int& height) const {
        glfwGetFramebufferSize(window, &width, &height);
    }

    void setTitle(const std::string& title) {
        glfwSetWindowTitle(window, title.c_str());
    }

    void setVSync(bool enabled) {
        glfwSwapInterval(enabled ? 1 : 0);
        config.vsync = enabled;
    }

    void setCursorMode(int mode) {
        glfwSetInputMode(window, GLFW_CURSOR, mode);
    }

    double getTime() const {
        return glfwGetTime();
    }

private:
    GLFWwindow* window;
    WindowConfig config;

    std::function<void(int, int, int, int)> keyCallback;
    std::function<void(int, int, int)> mouseButtonCallback;
    std::function<void(double, double)> cursorPosCallback;
    std::function<void(double, double)> scrollCallback;
    std::function<void(int, int)> resizeCallback;

    void setupCallbacks() {
        // Store this pointer in window user pointer
        glfwSetWindowUserPointer(window, this);

        // Framebuffer size callback
        glfwSetFramebufferSizeCallback(window,
            [](GLFWwindow* win, int width, int height) {
                auto* manager = static_cast<WindowManager*>(glfwGetWindowUserPointer(win));
                glViewport(0, 0, width, height);
                if (manager->resizeCallback) {
                    manager->resizeCallback(width, height);
                }
            }
        );

        // Key callback
        glfwSetKeyCallback(window,
            [](GLFWwindow* win, int key, int scancode, int action, int mods) {
                auto* manager = static_cast<WindowManager*>(glfwGetWindowUserPointer(win));
                if (manager->keyCallback) {
                    manager->keyCallback(key, scancode, action, mods);
                }
            }
        );

        // Mouse button callback
        glfwSetMouseButtonCallback(window,
            [](GLFWwindow* win, int button, int action, int mods) {
                auto* manager = static_cast<WindowManager*>(glfwGetWindowUserPointer(win));
                if (manager->mouseButtonCallback) {
                    manager->mouseButtonCallback(button, action, mods);
                }
            }
        );

        // Cursor position callback
        glfwSetCursorPosCallback(window,
            [](GLFWwindow* win, double xpos, double ypos) {
                auto* manager = static_cast<WindowManager*>(glfwGetWindowUserPointer(win));
                if (manager->cursorPosCallback) {
                    manager->cursorPosCallback(xpos, ypos);
                }
            }
        );

        // Scroll callback
        glfwSetScrollCallback(window,
            [](GLFWwindow* win, double xoffset, double yoffset) {
                auto* manager = static_cast<WindowManager*>(glfwGetWindowUserPointer(win));
                if (manager->scrollCallback) {
                    manager->scrollCallback(xoffset, yoffset);
                }
            }
        );
    }

    void printOpenGLInfo() {
        std::cout << "OpenGL Information:" << std::endl;
        std::cout << "  Vendor: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "  Renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "  Version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "  GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    }

    void cleanup() {
        if (window) {
            glfwDestroyWindow(window);
            window = nullptr;
        }
        glfwTerminate();
    }
};

// Example usage: main.cpp
int main() {
    WindowManager windowManager;

    // Configure window
    WindowManager::WindowConfig config;
    config.width = 1280;
    config.height = 720;
    config.title = "Advanced Window Manager";
    config.vsync = true;
    config.samples = 4;

    if (!windowManager.initialize(config)) {
        return -1;
    }

    // Set up input callbacks
    windowManager.setKeyCallback([&](int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            windowManager.setShouldClose(true);
        }
        if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
            std::cout << "F11 pressed - Toggle fullscreen" << std::endl;
        }
    });

    windowManager.setMouseButtonCallback([](int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            std::cout << "Left mouse button clicked" << std::endl;
        }
    });

    windowManager.setScrollCallback([](double xoffset, double yoffset) {
        std::cout << "Scroll: " << yoffset << std::endl;
    });

    windowManager.setResizeCallback([](int width, int height) {
        std::cout << "Window resized to: " << width << "x" << height << std::endl;
    });

    // Main render loop
    double lastTime = windowManager.getTime();
    int frameCount = 0;

    while (!windowManager.shouldClose()) {
        // Calculate FPS
        double currentTime = windowManager.getTime();
        frameCount++;
        if (currentTime - lastTime >= 1.0) {
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            lastTime = currentTime;
        }

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Your rendering code here...

        windowManager.swapBuffers();
        windowManager.pollEvents();
    }

    return 0;
}
```

## Advanced Topics

### Monitor Management

```cpp
// Get all monitors
int monitorCount;
GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

std::cout << "Available monitors: " << monitorCount << std::endl;

for (int i = 0; i < monitorCount; i++) {
    const char* name = glfwGetMonitorName(monitors[i]);
    std::cout << "  Monitor " << i << ": " << name << std::endl;

    // Get video modes
    int modeCount;
    const GLFWvidmode* modes = glfwGetVideoModes(monitors[i], &modeCount);

    std::cout << "    Available video modes:" << std::endl;
    for (int j = 0; j < modeCount; j++) {
        std::cout << "      " << modes[j].width << "x" << modes[j].height
                  << " @ " << modes[j].refreshRate << "Hz" << std::endl;
    }
}
```

### High DPI Support

```cpp
// Get window content scale (for high DPI displays)
float xscale, yscale;
glfwGetWindowContentScale(window, &xscale, &yscale);

std::cout << "Content scale: " << xscale << "x" << yscale << std::endl;

// Adjust UI based on scale
float uiScale = xscale;  // Use xscale for UI elements
```

### Clipboard Support

```cpp
// Set clipboard text
glfwSetClipboardString(window, "Hello, clipboard!");

// Get clipboard text
const char* text = glfwGetClipboardString(window);
if (text) {
    std::cout << "Clipboard: " << text << std::endl;
}
```

### Window Icon

```cpp
#include <GLFW/glfw3.h>

// Load image data (you'll need an image loading library like stb_image)
// Example with raw data:
GLFWimage icon;
icon.width = 16;
icon.height = 16;
// icon.pixels = /* your pixel data (RGBA) */;

glfwSetWindowIcon(window, 1, &icon);
```

### Performance Timing

```cpp
// High-resolution timer
double startTime = glfwGetTime();

// ... do some work ...

double endTime = glfwGetTime();
double elapsed = endTime - startTime;
std::cout << "Operation took: " << elapsed * 1000.0 << " ms" << std::endl;

// Frame timing
double lastFrame = 0.0;
while (!glfwWindowShouldClose(window)) {
    double currentFrame = glfwGetTime();
    double deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    std::cout << "Delta time: " << deltaTime << " ("
              << 1.0 / deltaTime << " FPS)" << std::endl;

    // Render...
    glfwSwapBuffers(window);
    glfwPollEvents();
}
```

## Summary

In this lesson, you learned:

1. **OpenGL Contexts**: What they are, why we need them, and different context types
2. **GLFW Basics**: Initialization, error handling, and version information
3. **Window Creation**: Creating windows, fullscreen modes, and window properties
4. **Window Configuration**: Using hints to configure OpenGL version, framebuffer properties, and window behavior
5. **Input Handling**: Polling and callback methods for keyboard, mouse, and scroll input
6. **Event Processing**: Different event processing methods and window callbacks
7. **Multiple Windows**: Creating and managing multiple windows with shared contexts
8. **Window Manager**: A complete reusable window manager class with callbacks
9. **Advanced Features**: Monitor management, high DPI support, clipboard, and performance timing

### What's Next?

In **Lesson 23**, we'll explore:
- The complete OpenGL graphics pipeline
- Vertex processing stages
- Fragment processing stages
- Fixed-function stages
- How data flows through the pipeline
- Pipeline performance optimization

### Practice Exercises

1. **Multiple Windows**: Create a program with 3 windows, each with a different background color
2. **Input Logger**: Create a program that logs all keyboard and mouse events to a file
3. **FPS Counter**: Implement a smooth FPS counter with averaging
4. **Fullscreen Toggle**: Add F11 key to toggle between windowed and fullscreen mode
5. **Custom Cursor**: Implement a custom cursor using a texture

### Additional Resources

- **GLFW Documentation**: https://www.glfw.org/documentation.html
- **GLFW Examples**: https://github.com/glfw/glfw/tree/master/examples
- **OpenGL Context Tutorial**: https://www.khronos.org/opengl/wiki/OpenGL_Context
- **LearnOpenGL - Creating a Window**: https://learnopengl.com/Getting-started/Creating-a-window

Congratulations! You now have a solid understanding of OpenGL context creation and window management using GLFW. You're ready to dive into the OpenGL rendering pipeline!
