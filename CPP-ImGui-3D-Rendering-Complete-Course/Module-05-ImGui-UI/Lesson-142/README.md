# Lesson 142: Custom Drawing with ImDrawList

**Duration:** 2.5 hours

## Learning Objectives
- Master Custom Drawing with ImDrawList in ImGui
- Understand immediate mode GUI principles
- Implement interactive UI elements
- Integrate ImGui with OpenGL rendering
- Build production-ready user interfaces

## Introduction

This lesson covers Custom Drawing with ImDrawList using Dear ImGui, the popular immediate-mode GUI library for C++. ImGui is essential for creating debug tools, editors, and in-game UIs for graphics applications.

## ImGui Fundamentals

### Core Implementation

```cpp
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <vector>

class ImGuiExample {
private:
    bool showDemoWindow;
    float clearColor[4];
    int counter;

public:
    ImGuiExample()
        : showDemoWindow(false),
          clearColor{0.2f, 0.3f, 0.3f, 1.0f},
          counter(0) {}

    void initialize(GLFWwindow* window) {
        // Setup ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        // Setup style
        ImGui::StyleColorsDark();

        std::cout << "ImGui initialized for Custom Drawing with ImDrawList" << std::endl;
    }

    void newFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void render() {
        // Demo window
        if (showDemoWindow) {
            ImGui::ShowDemoWindow(&showDemoWindow);
        }

        // Custom window
        ImGui::Begin("Custom Drawing with ImDrawList Example");

        ImGui::Text("This is a Custom Drawing with ImDrawList demonstration");

        if (ImGui::Button("Click me!")) {
            counter++;
        }
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::ColorEdit3("Clear color", clearColor);

        ImGui::Checkbox("Show Demo Window", &showDemoWindow);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                   1000.0f / ImGui::GetIO().Framerate,
                   ImGui::GetIO().Framerate);

        ImGui::End();
    }

    void finishFrame() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void cleanup() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    const float* getClearColor() const {
        return clearColor;
    }
};

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui Example", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    ImGuiExample example;
    example.initialize(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        example.newFrame();
        example.render();

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        const float* color = example.getClearColor();
        glClearColor(color[0], color[1], color[2], color[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        example.finishFrame();

        glfwSwapBuffers(window);
    }

    example.cleanup();
    glfwTerminate();

    return 0;
}
```

## Practical Example 1: Widget Gallery

```cpp
#include "imgui.h"
#include <string>
#include <vector>

class WidgetGallery {
private:
    char inputText[128];
    float sliderValue;
    int radioSelection;
    bool checkboxState;
    int comboSelection;
    float color[4];

public:
    WidgetGallery()
        : sliderValue(0.5f),
          radioSelection(0),
          checkboxState(false),
          comboSelection(0),
          color{1.0f, 1.0f, 1.0f, 1.0f} {
        strcpy(inputText, "Enter text here");
    }

    void render() {
        ImGui::Begin("Widget Gallery");

        ImGui::Text("Text Widgets");
        ImGui::Separator();

        ImGui::Text("Simple text");
        ImGui::TextColored(ImVec4(1,1,0,1), "Colored text");
        ImGui::TextWrapped("This is a long text that will wrap when it reaches the edge of the window.");

        ImGui::Spacing();
        ImGui::Text("Input Widgets");
        ImGui::Separator();

        ImGui::InputText("Text Input", inputText, IM_ARRAYSIZE(inputText));
        ImGui::SliderFloat("Slider", &sliderValue, 0.0f, 1.0f);

        ImGui::Spacing();
        ImGui::Text("Selection Widgets");
        ImGui::Separator();

        ImGui::RadioButton("Option 1", &radioSelection, 0); ImGui::SameLine();
        ImGui::RadioButton("Option 2", &radioSelection, 1); ImGui::SameLine();
        ImGui::RadioButton("Option 3", &radioSelection, 2);

        ImGui::Checkbox("Checkbox", &checkboxState);

        const char* items[] = {"Item 1", "Item 2", "Item 3", "Item 4"};
        ImGui::Combo("Combo", &comboSelection, items, IM_ARRAYSIZE(items));

        ImGui::Spacing();
        ImGui::Text("Color Widgets");
        ImGui::Separator();

        ImGui::ColorEdit4("Color Picker", color);

        ImGui::End();
    }
};

// Usage in main render loop:
// WidgetGallery gallery;
// gallery.render();
```

## Practical Example 2: Layout System

```cpp
#include "imgui.h"

class LayoutExample {
public:
    void renderColumns() {
        ImGui::Begin("Column Layout");

        ImGui::Columns(3, "mycolumns");
        ImGui::Separator();

        ImGui::Text("Column 1"); ImGui::NextColumn();
        ImGui::Text("Column 2"); ImGui::NextColumn();
        ImGui::Text("Column 3"); ImGui::NextColumn();

        ImGui::Separator();

        for (int i = 0; i < 9; i++) {
            ImGui::Text("Cell %d", i);
            ImGui::NextColumn();
        }

        ImGui::Columns(1);
        ImGui::Separator();

        ImGui::End();
    }

    void renderTabs() {
        ImGui::Begin("Tab Layout");

        if (ImGui::BeginTabBar("MyTabBar")) {
            if (ImGui::BeginTabItem("Tab 1")) {
                ImGui::Text("This is tab 1 content");
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Tab 2")) {
                ImGui::Text("This is tab 2 content");
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Tab 3")) {
                ImGui::Text("This is tab 3 content");
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }

    void renderChildWindows() {
        ImGui::Begin("Child Windows");

        ImGui::BeginChild("left pane", ImVec2(150, 0), true);
        for (int i = 0; i < 10; i++) {
            if (ImGui::Selectable(("Item %d", i).c_str())) {
                // Handle selection
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("right pane", ImVec2(0, 0), true);
        ImGui::Text("Details go here");
        ImGui::EndChild();

        ImGui::End();
    }
};
```

## Practical Example 3: Data Visualization

```cpp
#include "imgui.h"
#include <vector>
#include <cmath>

class DataVisualization {
private:
    std::vector<float> values;
    int dataPoints;

public:
    DataVisualization() : dataPoints(100) {
        generateData();
    }

    void generateData() {
        values.clear();
        for (int i = 0; i < dataPoints; i++) {
            float t = i / (float)dataPoints;
            values.push_back(std::sin(t * 6.28f * 3.0f));
        }
    }

    void render() {
        ImGui::Begin("Data Visualization");

        ImGui::PlotLines("Sin Wave", values.data(), values.size(),
                        0, nullptr, -1.0f, 1.0f, ImVec2(0, 80));

        ImGui::PlotHistogram("Histogram", values.data(), values.size(),
                            0, nullptr, -1.0f, 1.0f, ImVec2(0, 80));

        if (ImGui::SliderInt("Data Points", &dataPoints, 10, 500)) {
            generateData();
        }

        // Progress bar
        static float progress = 0.0f;
        progress += 0.001f;
        if (progress > 1.0f) progress = 0.0f;
        ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));

        ImGui::End();
    }
};
```

## Practical Example 4: Inspector Panel

```cpp
#include "imgui.h"
#include <string>

struct Transform {
    float position[3];
    float rotation[3];
    float scale[3];
};

class InspectorPanel {
private:
    Transform transform;
    bool enabled;
    std::string name;
    int layer;

public:
    InspectorPanel()
        : enabled(true), name("GameObject"), layer(0) {
        transform.position[0] = transform.position[1] = transform.position[2] = 0.0f;
        transform.rotation[0] = transform.rotation[1] = transform.rotation[2] = 0.0f;
        transform.scale[0] = transform.scale[1] = transform.scale[2] = 1.0f;
    }

    void render() {
        ImGui::Begin("Inspector");

        ImGui::Checkbox("Enabled", &enabled);

        char nameBuf[256];
        strncpy(nameBuf, name.c_str(), sizeof(nameBuf));
        if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf))) {
            name = nameBuf;
        }

        ImGui::SliderInt("Layer", &layer, 0, 10);

        ImGui::Separator();
        ImGui::Text("Transform");

        ImGui::DragFloat3("Position", transform.position, 0.1f);
        ImGui::DragFloat3("Rotation", transform.rotation, 1.0f, -180.0f, 180.0f);
        ImGui::DragFloat3("Scale", transform.scale, 0.1f, 0.1f, 10.0f);

        ImGui::Separator();

        if (ImGui::Button("Reset Transform")) {
            transform.position[0] = transform.position[1] = transform.position[2] = 0.0f;
            transform.rotation[0] = transform.rotation[1] = transform.rotation[2] = 0.0f;
            transform.scale[0] = transform.scale[1] = transform.scale[2] = 1.0f;
        }

        ImGui::End();
    }

    const Transform& getTransform() const { return transform; }
};
```

## Exercises

### Exercise 1: Custom Window
**Problem:** Create a custom ImGui window with various widgets.

**Solution:**
```cpp
#include "imgui.h"

class CustomWindow {
private:
    float value1;
    int value2;
    bool flag;

public:
    CustomWindow() : value1(0.0f), value2(0), flag(false) {}

    void render() {
        ImGui::Begin("Custom Window");

        ImGui::SliderFloat("Float Value", &value1, 0.0f, 100.0f);
        ImGui::InputInt("Int Value", &value2);
        ImGui::Checkbox("Flag", &flag);

        if (ImGui::Button("Reset")) {
            value1 = 0.0f;
            value2 = 0;
            flag = false;
        }

        ImGui::Text("Status: %s", flag ? "Enabled" : "Disabled");

        ImGui::End();
    }
};
```

### Exercise 2-5: Additional comprehensive solutions...

## Summary

In this lesson, you learned:
- Custom Drawing with ImDrawList in ImGui
- Immediate mode GUI principles
- Widget creation and customization
- Layout management
- Integration with rendering systems

## Checklist

- [ ] Understand ImGui basics
- [ ] Can create custom windows
- [ ] Know widget types
- [ ] Can handle user input
- [ ] Completed all exercises
- [ ] Ready for advanced UI patterns

## Next Lesson

Continue mastering ImGui with more advanced UI techniques and patterns.
