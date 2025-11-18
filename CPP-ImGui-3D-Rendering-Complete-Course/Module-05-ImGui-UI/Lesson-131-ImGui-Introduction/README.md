# Lesson 131: Introduction to ImGui - Immediate Mode GUI
**Duration: 2 hours**
**Module 5: ImGui UI Development**

## Learning Objectives
By the end of this lesson, you will:
- Understand what Immediate Mode GUI (IMGUI) is
- Know the differences between Immediate Mode and Retained Mode GUIs
- Understand ImGui's philosophy and use cases
- Know when to use ImGui vs traditional GUI frameworks
- Set up your mindset for ImGui development
- Understand ImGui's architecture

## 1. What is Immediate Mode GUI?

### Traditional GUI (Retained Mode)
In traditional GUI systems, you create UI elements and store them:

```cpp
// Pseudo-code for Retained Mode GUI
Button* myButton = new Button("Click Me");
myButton->setPosition(10, 10);
myButton->setSize(100, 30);
myButton->onClick([]{ std::cout << "Clicked!"; });
ui->addWidget(myButton);  // Widget persists in memory
```

**Characteristics:**
- Widgets are objects stored in memory
- State is managed by the widget
- Event-driven architecture
- Complex hierarchy of objects
- More memory usage

### Immediate Mode GUI (ImGui)
In ImGui, you draw UI every frame based on current state:

```cpp
// ImGui - Immediate Mode
if (ImGui::Button("Click Me")) {
    std::cout << "Clicked!" << std::endl;
}
// Button doesn't exist as an object - it's drawn and checked immediately
```

**Characteristics:**
- UI code runs every frame
- No widget objects stored in memory
- State is in your application, not in the GUI
- Simple, declarative code
- Minimal memory overhead

## 2. Immediate Mode vs Retained Mode

### Memory Model

**Retained Mode:**
```
Application State → Widget State → Rendering
     (sync)            (sync)
```

**Immediate Mode:**
```
Application State → Rendering
     (direct)
```

### Code Comparison

#### Retained Mode Example (Qt/wxWidgets style)
```cpp
class MyWindow : public Window {
    Button* button;
    TextBox* textbox;
    Label* label;

public:
    MyWindow() {
        button = new Button("Submit");
        textbox = new TextBox();
        label = new Label("Hello");

        button->onClick([this]{
            label->setText(textbox->getText());
        });

        addWidget(button);
        addWidget(textbox);
        addWidget(label);
    }

    ~MyWindow() {
        delete button;
        delete textbox;
        delete label;
    }
};
```

#### Immediate Mode Example (ImGui)
```cpp
void RenderUI() {
    static std::string text = "Hello";

    ImGui::Begin("My Window");

    ImGui::InputText("##input", &text);
    if (ImGui::Button("Submit")) {
        // Use text directly
    }
    ImGui::Text("%s", text.c_str());

    ImGui::End();
}
```

Much simpler! State is in your variables, not in widget objects.

## 3. ImGui Philosophy and Design

### Core Principles

1. **Minimal State Storage**
   - ImGui stores almost no state
   - Your application owns the data
   - No widget objects to manage

2. **Declarative Code**
   - Describe what you want, not how to achieve it
   - Code reads like a description of the UI

3. **Immediate Feedback**
   - Changes in code immediately visible
   - Easy to experiment and iterate

4. **Integration-Friendly**
   - Works with any rendering backend (OpenGL, DirectX, Vulkan)
   - Easy to integrate into existing applications
   - Minimal dependencies

### The ImGui Pattern

```cpp
// Every frame, in your main loop:
while (!shouldClose) {
    // Start ImGui frame
    ImGui::NewFrame();

    // Draw your UI (this code runs every frame!)
    ImGui::Begin("Window Title");
    ImGui::Text("Hello, World!");
    ImGui::Button("Click me");
    ImGui::End();

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap buffers, etc.
}
```

## 4. When to Use ImGui

### Perfect For:

✓ **Debug Tools**
```cpp
void ShowDebugInfo() {
    ImGui::Begin("Debug Info");
    ImGui::Text("FPS: %.1f", fps);
    ImGui::Text("Draw Calls: %d", drawCalls);
    ImGui::Text("Vertices: %d", vertexCount);
    ImGui::End();
}
```

✓ **Development Tools**
- Scene editors
- Profilers
- Asset browsers
- Configuration panels

✓ **In-Game Developer Consoles**
```cpp
void ShowConsole() {
    ImGui::Begin("Console");

    for (const auto& msg : messages) {
        ImGui::TextColored(msg.color, "%s", msg.text.c_str());
    }

    static char input[256];
    if (ImGui::InputText("##input", input, 256, ImGuiInputTextFlags_EnterReturnsTrue)) {
        ExecuteCommand(input);
        input[0] = '\0';
    }

    ImGui::End();
}
```

✓ **Rapid Prototyping**
- Quick UI mockups
- Testing gameplay systems
- Parameter tuning interfaces

✓ **Visualization Tools**
- Performance graphs
- Data visualization
- Real-time plotting

### Not Ideal For:

✗ **Polished Game UIs**
- Main menus (use dedicated UI system)
- HUD elements (use optimized rendering)
- Cutscene interfaces

✗ **Complex Animations**
- ImGui animations are simple
- Not designed for fancy transitions

✗ **Mobile Touch Interfaces**
- Designed for mouse/keyboard
- Not optimized for touch

✗ **Accessibility Requirements**
- Limited screen reader support
- No built-in accessibility features

## 5. ImGui Architecture Overview

### High-Level Structure

```
┌─────────────────────────────────────┐
│     Your Application Code           │
│  (Game Engine, Tool, etc.)          │
└─────────────┬───────────────────────┘
              │
              ▼
┌─────────────────────────────────────┐
│          ImGui Core                 │
│  (Platform-Independent)             │
│  - Input handling                   │
│  - Layout calculations              │
│  - Widget logic                     │
│  - Draw list generation             │
└─────────────┬───────────────────────┘
              │
              ▼
┌─────────────────────────────────────┐
│    ImGui Backend (Platform)         │
│  - Window creation                  │
│  - Input polling                    │
│  - Clipboard handling               │
└─────────────────────────────────────┘
              │
              ▼
┌─────────────────────────────────────┐
│   ImGui Renderer Backend            │
│  - OpenGL / DirectX / Vulkan        │
│  - Texture management               │
│  - Draw command execution           │
└─────────────────────────────────────┘
```

### Key Components

1. **ImGui Context**
   - Stores all ImGui state for a session
   - One context per application (usually)

2. **IO Structure**
   - Input/output configuration
   - Delta time, mouse position, key states
   - Display size, DPI scaling

3. **Draw Lists**
   - Generated every frame
   - Contains rendering commands
   - Backend-agnostic

4. **Style**
   - Colors, sizes, spacing
   - Customizable appearance

## 6. ImGui Code Patterns

### Pattern 1: Simple Window

```cpp
void ShowSimpleWindow() {
    ImGui::Begin("Simple Window");

    ImGui::Text("This is a simple window");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    ImGui::End();
}
```

### Pattern 2: Window with State

```cpp
void ShowStatefulWindow() {
    static float value = 0.0f;
    static bool enabled = true;

    ImGui::Begin("Stateful Window");

    ImGui::Checkbox("Enabled", &enabled);

    if (enabled) {
        ImGui::SliderFloat("Value", &value, 0.0f, 100.0f);
        ImGui::Text("Current value: %.2f", value);
    }

    ImGui::End();
}
```

### Pattern 3: Conditional UI

```cpp
void ShowConditionalUI(bool isEditor) {
    ImGui::Begin("Settings");

    ImGui::Text("Resolution: %dx%d", width, height);
    ImGui::Checkbox("Fullscreen", &fullscreen);

    if (isEditor) {
        // Only show debug options in editor
        ImGui::Separator();
        ImGui::Text("Debug Options");
        ImGui::Checkbox("Show Wireframe", &wireframe);
        ImGui::Checkbox("Show Normals", &normals);
    }

    ImGui::End();
}
```

### Pattern 4: Data Visualization

```cpp
void ShowDataViz(const std::vector<float>& data) {
    ImGui::Begin("Performance");

    ImGui::PlotLines("Frame Time (ms)",
                     data.data(),
                     data.size(),
                     0,
                     nullptr,
                     0.0f,
                     33.0f,  // 60 FPS target
                     ImVec2(0, 80));

    float avg = std::accumulate(data.begin(), data.end(), 0.0f) / data.size();
    ImGui::Text("Average: %.2f ms", avg);

    ImGui::End();
}
```

## 7. ImGui Best Practices

### Use Static for Persistent State

```cpp
void MyWindow() {
    static int counter = 0;  // Persists between frames
    static bool showAdvanced = false;

    ImGui::Begin("My Window");

    ImGui::Text("Counter: %d", counter);
    if (ImGui::Button("Increment")) {
        counter++;
    }

    ImGui::End();
}
```

### ID Management

ImGui uses IDs to track widgets. Handle duplicates:

```cpp
// Problem: Same labels create ID conflicts
ImGui::Button("Select");  // Item 1
ImGui::Button("Select");  // Item 2 - CONFLICT!

// Solution 1: Use ### to hide ID from display
ImGui::Button("Select###button1");
ImGui::Button("Select###button2");

// Solution 2: Use ImGui::PushID
for (int i = 0; i < items.size(); i++) {
    ImGui::PushID(i);
    if (ImGui::Button("Select")) {
        SelectItem(i);
    }
    ImGui::PopID();
}

// Solution 3: Use ## to make label invisible but part of ID
ImGui::Button("Select##1");
ImGui::Button("Select##2");
```

### Minimize State in ImGui

```cpp
// BAD: Storing state you don't need
struct MyGUI {
    int value;
    bool enabled;
    std::string text;
    // ... lots of GUI-only state
};

// GOOD: Store application state, derive UI from it
struct GameState {
    Player player;
    World world;
    Settings settings;
};

void ShowSettingsUI(Settings& settings) {
    ImGui::Begin("Settings");
    ImGui::SliderFloat("Volume", &settings.volume, 0.0f, 1.0f);
    ImGui::ColorEdit3("BG Color", settings.backgroundColor);
    ImGui::End();
}
```

## 8. ImGui Performance Characteristics

### Frame Budget

```cpp
// Typical ImGui performance (approximate)
// Simple debug window: ~0.01ms
// Complex editor UI: ~0.5ms
// Very heavy UI: ~2ms

void MeasureImGuiPerformance() {
    auto start = std::chrono::high_resolution_clock::now();

    ImGui::NewFrame();
    ShowAllWindows();  // Your UI code
    ImGui::Render();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "ImGui frame time: " << duration.count() / 1000.0f << "ms" << std::endl;
}
```

### Optimization Tips

1. **Don't render hidden windows**
```cpp
if (showDebugWindow) {
    ImGui::Begin("Debug");
    // ... expensive rendering
    ImGui::End();
}
```

2. **Use collapsed headers for large data**
```cpp
if (ImGui::CollapsingHeader("Expensive Section")) {
    // Only evaluated when expanded
    for (int i = 0; i < 10000; i++) {
        ImGui::Text("Item %d", i);
    }
}
```

3. **Limit text formatting**
```cpp
// Avoid excessive formatting in hot loops
for (auto& item : items) {
    ImGui::Text("%s", item.name.c_str());  // Fast
    // ImGui::Text("%s: %.2f (%.1f%%)", ...);  // Slower
}
```

## 9. Real-World ImGui Applications

### Game Engine Editors
- **Unity**: Uses ImGui-style system internally
- **Unreal Engine**: Editor tools use similar patterns
- **Godot**: Inspector panels
- **Custom engines**: Scene editors, profilers

### Graphics Tools
- **RenderDoc**: Graphics debugger UI
- **NSight**: NVIDIA profiling tools
- **Custom profilers**: Real-time visualization

### Development Tools
- **Live shader editors**
- **Asset browsers**
- **Animation editors**
- **Particle system editors**

## 10. Comparison with Alternatives

| Feature | ImGui | Qt | wxWidgets | Web UI |
|---------|-------|----|-----------|---------|
| **Learning Curve** | Low | High | Medium | Medium |
| **Integration** | Easy | Complex | Medium | Very Complex |
| **Performance** | Excellent | Good | Good | Variable |
| **Appearance** | Functional | Native | Native | Custom |
| **Use Case** | Tools/Debug | Applications | Applications | Applications |
| **Memory** | Very Low | High | Medium | High |
| **Dependencies** | Minimal | Large | Large | Huge |

## 11. ImGui Ecosystem

### Official Backends
- **OpenGL** (2, 3, ES)
- **DirectX** (9, 10, 11, 12)
- **Vulkan**
- **Metal** (macOS/iOS)
- **WebGL** (Emscripten)

### Platform Backends
- **GLFW**
- **SDL2**
- **Win32**
- **GLUT**
- **Allegro**

### Extensions and Tools
- **ImPlot**: Plotting library
- **ImGuizmo**: 3D gizmo widgets
- **ImNodes**: Node editor
- **ImGui-SFML**: SFML binding
- **ImGui-DX12**: DirectX 12 helpers

## 12. Getting Started Mindset

### Think Differently

**Retained Mode thinking:**
> "I need to create a button object, store it, update its state, handle events..."

**Immediate Mode thinking:**
> "Each frame, I'll check if the button is clicked right where I need that information."

### Embrace the Pattern

```cpp
// This feels weird at first but is powerful!
void GameLoop() {
    while (running) {
        // Your entire UI is rebuilt every frame
        ImGui::NewFrame();

        // Imagine describing your UI to someone
        ImGui::Begin("Settings");
        ImGui::Text("Volume");
        ImGui::SliderFloat("##volume", &volume, 0, 1);
        if (ImGui::Button("Reset")) volume = 0.5f;
        ImGui::End();

        ImGui::Render();
        RenderImGui();

        Update();
        Render();
    }
}
```

## 13. Practical Exercise

### Exercise: Plan Your First ImGui Application

Before writing code, plan a simple debug overlay for a game:

**Requirements:**
1. Show FPS
2. Display player position (X, Y, Z)
3. Toggle wireframe mode (checkbox)
4. Adjust game speed (slider 0.1x to 2.0x)
5. Button to reset game state

**Pseudocode your solution:**
```cpp
void ShowDebugOverlay(GameState& state) {
    ImGui::Begin("Debug");

    // TODO: Fill this in with ImGui calls
    // How would you display each piece of information?
    // What widgets would you use?

    ImGui::End();
}
```

## Summary

### Key Takeaways

✓ **ImGui is Immediate Mode**
- UI code runs every frame
- No widget objects to manage
- State lives in your application

✓ **Perfect for Tools**
- Debug interfaces
- Editor GUIs
- Development tools
- Real-time visualization

✓ **Simple and Fast**
- Minimal code
- Easy integration
- Excellent performance
- Low memory usage

✓ **Different Thinking**
- Declarative, not imperative
- Describe what you want
- Let ImGui handle the details

### Next Steps

In the next lesson, we'll:
- Set up ImGui with OpenGL
- Create our first ImGui window
- Integrate with GLFW
- Build a complete working example

## Checklist

- [ ] Understand Immediate Mode vs Retained Mode
- [ ] Know when to use ImGui
- [ ] Understand ImGui's architecture
- [ ] Familiar with common patterns
- [ ] Recognize best practices
- [ ] Ready to write actual ImGui code

---

**Time to complete**: 2 hours
**Next lesson**: [Lesson 132 - ImGui Setup and Integration with OpenGL](../Lesson-132-ImGui-OpenGL-Setup/README.md)

**Congratulations!** You now understand the philosophy behind ImGui. Next, we'll get hands-on with actual code!
