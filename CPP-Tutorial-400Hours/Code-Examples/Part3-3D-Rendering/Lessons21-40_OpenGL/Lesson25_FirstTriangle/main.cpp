/*
 * Lesson 25: Drawing Your First Triangle
 *
 * This lesson demonstrates:
 * - Vertex Buffer Objects (VBO)
 * - Vertex Array Objects (VAO)
 * - Shader compilation and linking
 * - Complete rendering pipeline
 * - Colored triangle rendering
 *
 * Dependencies: GLFW, GLAD
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
std::string loadShaderSource(const char* filename);
GLuint compileShader(GLenum type, const char* source);
GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath);

int main() {
    std::cout << "==========================================" << std::endl;
    std::cout << "  Lesson 25: Drawing Your First Triangle" << std::endl;
    std::cout << "==========================================" << std::endl;

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lesson 25: First Triangle", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "\n[1] Setting up vertex data..." << std::endl;

    // Vertex data (position + color)
    float vertices[] = {
        // positions         // colors
         0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // top (red)
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left (green)
         0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // bottom right (blue)
    };

    // Generate and bind VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    std::cout << "    ✓ Created VAO (ID: " << VAO << ")" << std::endl;
    std::cout << "    ✓ Created VBO (ID: " << VBO << ")" << std::endl;
    std::cout << "    ✓ Uploaded " << sizeof(vertices) << " bytes of vertex data" << std::endl;

    std::cout << "\n[2] Loading and compiling shaders..." << std::endl;

    // Load shader source code
    std::string vertexCode = loadShaderSource("vertex.glsl");
    std::string fragmentCode = loadShaderSource("fragment.glsl");

    if (vertexCode.empty() || fragmentCode.empty()) {
        std::cerr << "Failed to load shader files" << std::endl;
        std::cout << "Using fallback inline shaders..." << std::endl;

        // Fallback shaders
        vertexCode = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aColor;
            out vec3 vertexColor;
            void main() {
                gl_Position = vec4(aPos, 1.0);
                vertexColor = aColor;
            }
        )";

        fragmentCode = R"(
            #version 330 core
            in vec3 vertexColor;
            out vec4 FragColor;
            void main() {
                FragColor = vec4(vertexColor, 1.0);
            }
        )";
    }

    // Compile shaders
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

    // Create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check linking
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR: Shader program linking failed\n" << infoLog << std::endl;
    } else {
        std::cout << "    ✓ Shader program linked successfully (ID: " << shaderProgram << ")" << std::endl;
    }

    // Clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    std::cout << "\n[3] Entering render loop..." << std::endl;
    std::cout << "    Press ESC to exit\n" << std::endl;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Input
        process_input(window);

        // Clear
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap and poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    std::cout << "\n[4] Cleaning up resources..." << std::endl;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    std::cout << "\n==========================================" << std::endl;
    std::cout << "  Lesson Complete!" << std::endl;
    std::cout << "==========================================" << std::endl;

    return 0;
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

std::string loadShaderSource(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::string shaderType = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        std::cerr << "ERROR: " << shaderType << " shader compilation failed\n" << infoLog << std::endl;
    } else {
        std::string shaderType = (type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
        std::cout << "    ✓ " << shaderType << " shader compiled (ID: " << shader << ")" << std::endl;
    }

    return shader;
}
