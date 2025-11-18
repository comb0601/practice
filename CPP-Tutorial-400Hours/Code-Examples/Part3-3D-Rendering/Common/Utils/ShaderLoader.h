#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#ifdef _WIN32
    #include <glad/glad.h>
#else
    #include <GL/gl.h>
#endif

namespace Utils {

class ShaderLoader {
public:
    // Load shader from file
    static std::string LoadShaderSource(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Failed to open shader file: " << filepath << std::endl;
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    // Compile shader
    static GLuint CompileShader(GLenum type, const std::string& source) {
        GLuint shader = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        // Check for compilation errors
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[1024];
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::string shaderType = (type == GL_VERTEX_SHADER) ? "VERTEX" :
                                    (type == GL_FRAGMENT_SHADER) ? "FRAGMENT" :
                                    (type == GL_GEOMETRY_SHADER) ? "GEOMETRY" : "UNKNOWN";
            std::cerr << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog << std::endl;
            return 0;
        }

        return shader;
    }

    // Create shader program from source code
    static GLuint CreateProgram(const std::string& vertexSource, const std::string& fragmentSource) {
        GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
        GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

        if (vertexShader == 0 || fragmentShader == 0) {
            if (vertexShader) glDeleteShader(vertexShader);
            if (fragmentShader) glDeleteShader(fragmentShader);
            return 0;
        }

        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        // Check for linking errors
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[1024];
            glGetProgramInfoLog(program, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            glDeleteProgram(program);
            return 0;
        }

        // Clean up shaders (they're linked into the program now)
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    }

    // Create shader program from files
    static GLuint CreateProgramFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
        std::string vertexSource = LoadShaderSource(vertexPath);
        std::string fragmentSource = LoadShaderSource(fragmentPath);

        if (vertexSource.empty() || fragmentSource.empty()) {
            std::cerr << "Failed to load shader files" << std::endl;
            return 0;
        }

        return CreateProgram(vertexSource, fragmentSource);
    }

    // Create program with geometry shader
    static GLuint CreateProgramWithGeometry(const std::string& vertexSource,
                                           const std::string& fragmentSource,
                                           const std::string& geometrySource) {
        GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
        GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
        GLuint geometryShader = CompileShader(GL_GEOMETRY_SHADER, geometrySource);

        if (vertexShader == 0 || fragmentShader == 0 || geometryShader == 0) {
            if (vertexShader) glDeleteShader(vertexShader);
            if (fragmentShader) glDeleteShader(fragmentShader);
            if (geometryShader) glDeleteShader(geometryShader);
            return 0;
        }

        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glAttachShader(program, geometryShader);
        glLinkProgram(program);

        // Check for linking errors
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[1024];
            glGetProgramInfoLog(program, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteShader(geometryShader);

        return success ? program : 0;
    }

    // Validate program
    static bool ValidateProgram(GLuint program) {
        glValidateProgram(program);
        GLint success;
        glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
        if (!success) {
            GLchar infoLog[1024];
            glGetProgramInfoLog(program, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM::VALIDATION_FAILED\n" << infoLog << std::endl;
            return false;
        }
        return true;
    }
};

} // namespace Utils
