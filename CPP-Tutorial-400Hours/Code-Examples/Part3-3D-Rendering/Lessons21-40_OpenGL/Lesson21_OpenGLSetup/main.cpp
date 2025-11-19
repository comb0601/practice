/*
 * Lesson 21: Complete OpenGL Setup with GLFW
 *
 * This lesson demonstrates:
 * - GLFW window creation
 * - OpenGL context initialization
 * - GLAD loading
 * - Basic render loop
 * - Input handling
 *
 * Dependencies: GLFW, GLAD
 */

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

int main() {
    std::cout << "==========================================" << std::endl;
    std::cout << "  Lesson 21: OpenGL Setup with GLFW" << std::endl;
    std::cout << "==========================================" << std::endl;

    // Initialize GLFW
    std::cout << "\n[1] Initializing GLFW..." << std::endl;
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW
    std::cout << "[2] Configuring OpenGL context..." << std::endl;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    std::cout << "[3] Creating GLFW window..." << std::endl;
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lesson 21: OpenGL Setup", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load OpenGL function pointers with GLAD
    std::cout << "[4] Loading OpenGL functions with GLAD..." << std::endl;
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Print OpenGL information
    std::cout << "\n========== OpenGL Information ==========" << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "========================================\n" << std::endl;

    // Configure OpenGL state
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    std::cout << "[5] Entering render loop..." << std::endl;
    std::cout << "    Press ESC to exit\n" << std::endl;

    // Render loop
    int frameCount = 0;
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        // Input
        process_input(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();

        // FPS counter (every second)
        frameCount++;
        double currentTime = glfwGetTime();
        if (currentTime - lastTime >= 1.0) {
            std::cout << "FPS: " << frameCount << " | Time: " << currentTime << "s" << std::endl;
            frameCount = 0;
            lastTime = currentTime;
        }
    }

    std::cout << "\n[6] Cleaning up..." << std::endl;
    glfwTerminate();

    std::cout << "\n==========================================" << std::endl;
    std::cout << "  Lesson Complete!" << std::endl;
    std::cout << "==========================================" << std::endl;

    return 0;
}

// Process input
void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

// Framebuffer size callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    std::cout << "Window resized to: " << width << "x" << height << std::endl;
}
