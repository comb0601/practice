# Module 5: ImGui UI Development
**25 Lessons | ~50 Hours**

## Module Description

This module teaches you to create professional tool interfaces and debugging UIs using Dear ImGui, the industry-standard immediate-mode GUI library used in game engines, graphics applications, and development tools.

## What You'll Build

- **Debug overlays** for real-time performance monitoring
- **Scene editors** with hierarchies and property panels
- **Complete 3D engine editor** with docking and multiple viewports
- **Custom widgets** and visualization tools
- **Professional-looking tool interfaces**

## Prerequisites

- C++ fundamentals (Module 1)
- Basic OpenGL knowledge (Module 4, Lessons 91-95)
- Understanding of rendering loops

## Module Structure

### Part 15: ImGui Fundamentals (Lessons 131-140)

**Lesson 131: Introduction to ImGui** ✓ COMPLETE
- Immediate mode vs retained mode GUI
- ImGui philosophy and use cases
- When to use ImGui
- Architecture overview

**Lesson 132: ImGui Setup and Integration with OpenGL** ✓ COMPLETE
- Download and integrate ImGui
- CMake configuration
- OpenGL + GLFW setup
- Complete working example

**Lesson 133: ImGui Context and Main Loop**
- Understanding ImGui context
- Frame lifecycle
- Multiple contexts
- Best practices

**Lesson 134: Windows and Basic Layouts**
- Creating windows
- Window flags and properties
- Positioning and sizing
- Layout basics

**Lesson 135: Widgets - Buttons, Text, Labels**
- Button variants
- Text display and formatting
- Labels and tooltips
- Separators and spacing

**Lesson 136: Input Widgets**
- Text input fields
- Sliders and drag controls
- Numeric inputs
- Input validation

**Lesson 137: Selection Widgets**
- Combo boxes (dropdowns)
- List boxes
- Radio buttons
- Checkboxes

**Lesson 138: Color Pickers and Editors**
- Color edit widgets
- Color picker types
- HSV vs RGB
- Alpha channel handling

**Lesson 139: Trees and Collapsing Headers**
- Tree nodes and hierarchies
- Collapsing headers
- Tree flags and styling
- Building scene hierarchies

**Lesson 140: Project - Simple ImGui Application**
- Complete application structure
- Multiple window management
- State management
- User preferences

### Part 16: Advanced ImGui (Lessons 141-150)

**Lesson 141: Tables and Columns**
- Modern table API
- Column setup and flags
- Sorting and filtering
- Cell customization

**Lesson 142: Tabs and Tab Bars**
- Creating tab bars
- Tab ordering and reordering
- Tab closing behavior
- Context menus

**Lesson 143: Menus and Menu Bars**
- Main menu bars
- Popup menus
- Context menus
- Menu item shortcuts

**Lesson 144: Popups and Modals**
- Modal dialogs
- Popup windows
- Context popups
- Confirmation dialogs

**Lesson 145: Custom Rendering in ImGui**
- Draw lists
- Custom shapes and primitives
- Image rendering
- Custom visualizations

**Lesson 146: ImGui Styling and Themes**
- Style customization
- Color schemes
- Font configuration
- Professional themes

**Lesson 147: Custom Widgets**
- Creating reusable widgets
- Widget ID system
- Input handling
- Widget state

**Lesson 148: Multiple Viewports**
- Platform windows
- Viewport configuration
- Multi-monitor support
- Window dragging

**Lesson 149: Docking System**
- Docking spaces
- Dockable windows
- Persistent layouts
- Editor-style interfaces

**Lesson 150: Project - Multi-Window ImGui Application**
- Complete editor interface
- Docking layout
- Tool panels
- Workflow integration

### Part 17: ImGui for Tools and Debugging (Lessons 151-155)

**Lesson 151: Real-Time Performance Monitoring**
- FPS counters and graphs
- Memory usage displays
- Performance profilers
- Real-time plotting

**Lesson 152: Debugging Visualizations**
- Debug overlays
- Visual debugging tools
- Log viewers
- Inspector panels

**Lesson 153: ImGui for Scene Editing**
- Object hierarchies
- Transform gizmos integration
- Property editors
- Asset browsers

**Lesson 154: Serialization and Settings**
- Saving window layouts
- User preferences
- INI file handling
- Custom serialization

**Lesson 155: Project - Complete 3D Engine Editor with ImGui**
- Full-featured editor
- Scene hierarchy
- Property inspector
- Asset management
- Real-time preview
- Tool integration

## Key Technologies

- **Dear ImGui** 1.90+
- **OpenGL 3.3+** for rendering
- **GLFW** for windowing
- **STB Image** for textures (ImGui uses this internally)

## Learning Outcomes

By completing this module, you will be able to:

✓ Integrate ImGui into any OpenGL application
✓ Create professional debugging interfaces
✓ Build complete editor tools for 3D engines
✓ Customize ImGui appearance and behavior
✓ Create custom widgets and visualizations
✓ Implement docking and multi-viewport interfaces
✓ Optimize ImGui performance
✓ Follow industry-standard tool development practices

## Real-World Applications

### Game Development
- **Level editors** - Position objects, edit properties
- **Debug consoles** - Execute commands, view logs
- **Performance profilers** - Monitor frame time, draw calls
- **Asset browsers** - Preview and select resources

### Graphics Programming
- **Shader editors** - Live shader editing and previewing
- **Material editors** - Configure rendering properties
- **Post-processing tools** - Adjust effect parameters
- **Camera debuggers** - Visualize frustums and matrices

### Tools Development
- **Data visualization** - Plot graphs, display statistics
- **Configuration tools** - Edit settings, export files
- **Testing utilities** - Automated testing interfaces
- **Debugging aids** - Inspect internal state

## Industry Usage

ImGui is used in:
- **Game engines**: Custom tool development
- **Graphics debuggers**: RenderDoc, NSight
- **3D applications**: Blender (some tools), custom engines
- **Simulation software**: Real-time parameter tuning
- **Research projects**: Quick prototyping interfaces

## Resources Provided

Each lesson includes:
- **Detailed explanations** of ImGui concepts
- **Complete working code examples**
- **Visual diagrams** and illustrations
- **Practical exercises** with solutions
- **Performance tips** and best practices
- **Real-world project examples**

## Prerequisites Checklist

Before starting this module, ensure you can:
- [ ] Write C++ programs confidently
- [ ] Understand pointers and references
- [ ] Create OpenGL contexts and render basic shapes
- [ ] Use GLFW for window management
- [ ] Compile multi-file C++ projects with CMake

## Recommended Study Path

1. **Complete Lessons 131-135** - Get basics working
2. **Build simple tools** - Practice with small projects
3. **Complete Lessons 136-140** - Learn all widget types
4. **Build a debug overlay** - Practical application
5. **Complete Lessons 141-149** - Master advanced features
6. **Final project (Lesson 155)** - Build complete editor

## Time Investment

- **Minimum**: 30 hours (core lessons only)
- **Recommended**: 50 hours (all lessons + projects)
- **Mastery**: 70+ hours (including custom projects)

## Projects You'll Build

1. **Lesson 140**: Simple tool application
2. **Lesson 150**: Multi-window dockable editor
3. **Lesson 155**: Complete 3D engine editor with:
   - Scene hierarchy browser
   - Object inspector panel
   - Asset browser
   - Real-time 3D preview
   - Toolbar and menu system
   - Customizable layout

## Next Steps

After completing this module:
- Apply ImGui to your own projects
- Create custom development tools
- Build editor interfaces for engines
- Continue to Module 6 for advanced rendering
- Combine with Module 7 for performance optimization

---

**Start Learning**: [Lesson 131 - Introduction to ImGui](Lesson-131-ImGui-Introduction/README.md)

**Get Started Now!** ImGui is one of the most practical skills for game and graphics developers. The sooner you learn it, the sooner you can build amazing tools!
