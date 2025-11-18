# Lesson 132: ImGui Setup and Integration with OpenGL
**Duration: 2.5 hours**
**Module 5: ImGui UI Development**

## Learning Objectives
By the end of this lesson, you will:
- Download and integrate ImGui into your project
- Set up ImGui with OpenGL 3.3+
- Configure GLFW for ImGui input handling
- Create your first ImGui window
- Understand ImGui's build system integration
- Create a complete working ImGui + OpenGL application

## 1. Prerequisites

### Required Libraries
- **GLFW**: Window and input handling
- **GLAD** (or GLEW): OpenGL function loader
- **ImGui**: The GUI library (we'll download this)
- **GLM**: Math library (optional but recommended)

### Project Structure
```
MyImGuiProject/
├── src/
│   ├── main.cpp
│   ├── imgui/               # ImGui source files
│   │   ├── imgui.cpp
│   │   ├── imgui.h
│   │   ├── imgui_draw.cpp
│   │   ├── imgui_widgets.cpp
│   │   ├── imgui_tables.cpp
│   │   ├── imgui_demo.cpp
│   │   ├── imgui_impl_glfw.cpp
│   │   ├── imgui_impl_glfw.h
│   │   ├── imgui_impl_opengl3.cpp
│   │   └── imgui_impl_opengl3.h
│   └── glad/                # GLAD files
│       ├── glad.c
│       └── glad.h
├── include/
├── CMakeLists.txt
└── README.md
```

## 2. Downloading ImGui

### From GitHub

```bash
# Clone ImGui repository
git clone https://github.com/ocornut/imgui.git

# Or download specific release
wget https://github.com/ocornut/imgui/archive/refs/tags/v1.90.0.zip
unzip v1.90.0.zip
```

### Files You Need

**Core ImGui files** (required):
- `imgui.cpp`
- `imgui.h`
- `imgui_draw.cpp`
- `imgui_widgets.cpp`
- `imgui_tables.cpp`
- `imgui_internal.h`
- `imstb_rectpack.h`
- `imstb_textedit.h`
- `imstb_truetype.h`
- `imconfig.h`

**Backend files** (from `backends/` folder):
- `imgui_impl_glfw.cpp`
- `imgui_impl_glfw.h`
- `imgui_impl_opengl3.cpp`
- `imgui_impl_opengl3.h`

**Optional** (but useful):
- `imgui_demo.cpp` - Full demo showing all widgets

## 3. CMakeLists.txt Configuration

### Complete CMake Setup

```cmake
cmake_minimum_required(VERSION 3.10)
project(ImGuiOpenGLApp)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# ImGui source files
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/imgui_impl_opengl3.cpp
)

# GLAD source
set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/glad)
set(GLAD_SOURCES ${GLAD_DIR}/glad.c)

# Executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    ${IMGUI_SOURCES}
    ${GLAD_SOURCES}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${IMGUI_DIR}
    ${GLAD_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    glfw
    ${CMAKE_DL_LIBS}  # For glad
)

# Copy shaders and resources (if needed)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
```

### Building the Project

```bash
# Create build directory
mkdir build && cd build

# Generate build files
cmake ..

# Build
cmake --build .

# Or use make directly
make

# Run
./ImGuiOpenGLApp
```

## 4. Complete Working Example

### main.cpp - Full Application

```cpp
// main.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

// Callback for framebuffer resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Error callback
void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main() {
    // Set GLFW error callback
    glfwSetErrorCallback(glfw_error_callback);

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui + OpenGL Example", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1); // Enable vsync

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();  // Alternative style

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Application state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll events
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()!)
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, ImGui!");  // Create a window

            ImGui::Text("This is some useful text.");  // Display text
            ImGui::Checkbox("Demo Window", &show_demo_window);  // Checkbox
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);  // Slider
            ImGui::ColorEdit3("clear color", (float*)&clear_color);  // Color picker

            if (ImGui::Button("Button"))  // Button
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                       1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window
        if (show_another_window) {
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w,
                     clear_color.y * clear_color.w,
                     clear_color.z * clear_color.w,
                     clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render ImGui draw data
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
```

## 5. Understanding the Code

### Initialization Sequence

```cpp
// 1. Initialize GLFW
glfwInit();
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

// 2. Create window
GLFWwindow* window = glfwCreateWindow(1280, 720, "Title", NULL, NULL);
glfwMakeContextCurrent(window);

// 3. Load OpenGL functions
gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

// 4. Initialize ImGui
IMGUI_CHECKVERSION();
ImGui::CreateContext();
ImGuiIO& io = ImGui::GetIO();

// 5. Setup ImGui style
ImGui::StyleColorsDark();

// 6. Setup backends
ImGui_ImplGlfw_InitForOpenGL(window, true);
ImGui_ImplOpenGL3_Init("#version 330");
```

### Main Loop Structure

```cpp
while (!glfwWindowShouldClose(window)) {
    // 1. Poll input events
    glfwPollEvents();

    // 2. Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 3. YOUR IMGUI CODE HERE
    ImGui::Begin("My Window");
    ImGui::Text("Hello!");
    ImGui::End();

    // 4. Render ImGui
    ImGui::Render();

    // 5. Clear screen
    glClear(GL_COLOR_BUFFER_BIT);

    // 6. Render ImGui draw data
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // 7. Swap buffers
    glfwSwapBuffers(window);
}
```

### Cleanup Sequence

```cpp
// Reverse order of initialization
ImGui_ImplOpenGL3_Shutdown();
ImGui_ImplGlfw_Shutdown();
ImGui::DestroyContext();

glfwDestroyWindow(window);
glfwTerminate();
```

## 6. ImGuiIO Configuration

### Common IO Settings

```cpp
ImGuiIO& io = ImGui::GetIO();

// Enable keyboard navigation
io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

// Enable gamepad navigation
io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

// Enable docking (multi-window docking)
io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

// Enable multi-viewport (windows can move outside main window)
io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

// Adjust mouse sensitivity
io.MouseDoubleClickTime = 0.30f;  // seconds
io.MouseDoubleClickMaxDist = 6.0f;  // pixels

// Font size
io.FontGlobalScale = 1.0f;  // Scale all fonts

// Custom font (optional)
io.Fonts->AddFontFromFileTTF("path/to/font.ttf", 16.0f);
```

## 7. ImGui Styling

### Using Built-in Styles

```cpp
// Dark style (default)
ImGui::StyleColorsDark();

// Light style
ImGui::StyleColorsLight();

// Classic style
ImGui::StyleColorsClassic();
```

### Custom Styling

```cpp
ImGuiStyle& style = ImGui::GetStyle();

// Adjust rounding
style.WindowRounding = 5.0f;
style.FrameRounding = 3.0f;
style.ScrollbarRounding = 2.0f;

// Adjust spacing
style.WindowPadding = ImVec2(10, 10);
style.FramePadding = ImVec2(5, 3);
style.ItemSpacing = ImVec2(8, 4);

// Adjust sizes
style.ScrollbarSize = 15.0f;
style.GrabMinSize = 10.0f;

// Custom colors
style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
style.Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.3f, 0.8f, 1.0f);
style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.4f, 0.9f, 1.0f);
style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1f, 0.2f, 0.7f, 1.0f);
```

## 8. Integrating with Existing OpenGL Application

### Adding ImGui to Existing Code

```cpp
class Application {
private:
    GLFWwindow* window;
    // Your existing OpenGL stuff
    unsigned int VAO, VBO, shaderProgram;

public:
    void init() {
        // Your existing OpenGL initialization
        initWindow();
        initOpenGL();

        // Add ImGui initialization
        initImGui();
    }

    void initImGui() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void run() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            // Start ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Your ImGui windows
            renderUI();

            // Your existing rendering
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderScene();  // Your 3D scene

            // Render ImGui on top
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }
    }

    void renderUI() {
        ImGui::Begin("Scene Controls");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        // More controls...
        ImGui::End();
    }

    void renderScene() {
        // Your existing 3D rendering code
    }

    void cleanup() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        // Your existing cleanup
        cleanupOpenGL();
        glfwTerminate();
    }
};
```

## 9. Common Issues and Solutions

### Issue 1: ImGui Not Rendering

**Symptoms**: Window appears but no ImGui visible

**Solution**: Check this sequence:
```cpp
// Make sure you have:
ImGui_ImplOpenGL3_NewFrame();
ImGui_ImplGlfw_NewFrame();
ImGui::NewFrame();

// ... your UI code ...

ImGui::Render();
ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
```

### Issue 2: Input Not Working

**Symptoms**: Can't interact with ImGui widgets

**Solution**: Make sure callbacks are installed:
```cpp
// Pass 'true' to install callbacks
ImGui_ImplGlfw_InitForOpenGL(window, true);

// Or install manually:
ImGui_ImplGlfw_InitForOpenGL(window, false);
glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
glfwSetScrollCallback(window, ImGui_ImplGlfw_ScrollCallback);
glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
```

### Issue 3: Blank/Corrupted Rendering

**Symptoms**: ImGui renders but looks wrong

**Solution**: Ensure OpenGL state is clean:
```cpp
// Before rendering ImGui
glDisable(GL_DEPTH_TEST);
glDisable(GL_CULL_FACE);
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
```

### Issue 4: Linking Errors

**Symptoms**: Undefined references during linking

**Solution**: Make sure all ImGui source files are compiled:
```cmake
set(IMGUI_SOURCES
    imgui.cpp
    imgui_draw.cpp
    imgui_widgets.cpp
    imgui_tables.cpp
    imgui_impl_glfw.cpp
    imgui_impl_opengl3.cpp
    # Don't forget backend implementations!
)
```

## 10. Minimal Working Example

For quick testing, here's the absolute minimum:

```cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello!");
        ImGui::Text("Welcome to ImGui!");
        ImGui::End();

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
```

## 11. Exercises

### Exercise 1: Basic Setup
- Set up a new project with ImGui
- Display "Hello, ImGui!" in a window
- Show FPS counter

### Exercise 2: Styling
- Try all three built-in styles
- Create a custom color scheme
- Adjust window rounding and padding

### Exercise 3: Multiple Windows
- Create 3 different ImGui windows
- Make them toggleable with checkboxes
- Position them at different screen locations

### Exercise 4: Integration
- Add ImGui to an existing OpenGL project
- Create a debug overlay
- Display rendering statistics

## Summary

### Key Points

✓ **ImGui Integration is Simple**
- Add source files to project
- Initialize backends
- Render every frame

✓ **Main Loop Pattern**
```cpp
NewFrame() → UI Code → Render() → RenderDrawData()
```

✓ **Two Backends Needed**
- Platform: GLFW (input/window)
- Renderer: OpenGL3 (drawing)

✓ **Common Pitfalls**
- Forgetting to call NewFrame()
- Missing Render() before RenderDrawData()
- Not including all source files

## Checklist

- [ ] Successfully built ImGui project
- [ ] Can create windows and show text
- [ ] Understand initialization sequence
- [ ] Understand main loop structure
- [ ] Can customize styling
- [ ] Know how to integrate with existing code
- [ ] Completed all exercises

---

**Time to complete**: 2.5 hours
**Previous lesson**: [Lesson 131 - Introduction to ImGui](../Lesson-131-ImGui-Introduction/README.md)
**Next lesson**: [Lesson 133 - ImGui Context and Main Loop](../Lesson-133-ImGui-Context-MainLoop/README.md)

**Congratulations!** You now have a working ImGui + OpenGL application!
