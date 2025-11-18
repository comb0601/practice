# Lesson 21: OpenGL Setup with GLFW

## Overview
Complete OpenGL initialization with GLFW and GLAD. This is the foundation for all OpenGL lessons.

## Features
- GLFW window creation
- OpenGL 3.3 Core Profile context
- GLAD function loading
- Basic render loop
- Input handling
- FPS counter

## Building
```bash
cmake --build . --target Lesson21_OpenGLSetup
./bin/Lessons21-40/Lesson21_OpenGLSetup
```

## Controls
- **ESC**: Exit application

## Expected Output
- Window with teal background (RGB: 0.2, 0.3, 0.3)
- Console showing FPS every second
- OpenGL information in console

## Dependencies
- GLFW 3.3+
- GLAD (OpenGL 3.3)
- CMake 3.15+
