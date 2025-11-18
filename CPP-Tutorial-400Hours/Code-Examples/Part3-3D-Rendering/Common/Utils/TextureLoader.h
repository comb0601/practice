#pragma once
#include <string>
#include <iostream>

#ifdef _WIN32
    #include <glad/glad.h>
#else
    #include <GL/gl.h>
#endif

// Define STB_IMAGE_IMPLEMENTATION in one .cpp file before including this
#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_STATIC
    #ifndef STB_IMAGE_H
        // Forward declare if stb_image.h is available
        extern "C" {
            unsigned char *stbi_load(char const *filename, int *x, int *y, int *channels_in_file, int desired_channels);
            void stbi_image_free(void *retval_from_stbi_load);
            void stbi_set_flip_vertically_on_load(int flag_true_if_should_flip);
        }
    #endif
#endif

namespace Utils {

class TextureLoader {
public:
    struct TextureData {
        unsigned char* data;
        int width;
        int height;
        int channels;

        TextureData() : data(nullptr), width(0), height(0), channels(0) {}
        ~TextureData() {
            if (data) {
                stbi_image_free(data);
            }
        }

        // Prevent copying
        TextureData(const TextureData&) = delete;
        TextureData& operator=(const TextureData&) = delete;
    };

    // Load texture from file
    static GLuint LoadTexture(const std::string& filepath, bool flipVertically = true) {
        stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

        int width, height, channels;
        unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

        if (!data) {
            std::cerr << "Failed to load texture: " << filepath << std::endl;
            return 0;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Determine format
        GLenum format = GL_RGB;
        if (channels == 1) format = GL_RED;
        else if (channels == 3) format = GL_RGB;
        else if (channels == 4) format = GL_RGBA;

        // Upload texture data
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);

        std::cout << "Loaded texture: " << filepath << " (" << width << "x" << height << ", " << channels << " channels)" << std::endl;

        return texture;
    }

    // Load texture with custom parameters
    static GLuint LoadTextureCustom(const std::string& filepath,
                                   GLint wrapS, GLint wrapT,
                                   GLint minFilter, GLint magFilter,
                                   bool flipVertically = true) {
        stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

        int width, height, channels;
        unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

        if (!data) {
            std::cerr << "Failed to load texture: " << filepath << std::endl;
            return 0;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Set custom parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

        // Determine format
        GLenum format = GL_RGB;
        if (channels == 1) format = GL_RED;
        else if (channels == 3) format = GL_RGB;
        else if (channels == 4) format = GL_RGBA;

        // Upload texture data
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        // Generate mipmaps if using mipmap filter
        if (minFilter == GL_LINEAR_MIPMAP_LINEAR || minFilter == GL_LINEAR_MIPMAP_NEAREST ||
            minFilter == GL_NEAREST_MIPMAP_LINEAR || minFilter == GL_NEAREST_MIPMAP_NEAREST) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        stbi_image_free(data);

        return texture;
    }

    // Load cubemap texture
    static GLuint LoadCubemap(const std::vector<std::string>& faces) {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, channels;
        for (unsigned int i = 0; i < faces.size(); i++) {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
            if (data) {
                GLenum format = GL_RGB;
                if (channels == 1) format = GL_RED;
                else if (channels == 3) format = GL_RGB;
                else if (channels == 4) format = GL_RGBA;

                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                           0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            } else {
                std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
                return 0;
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }

    // Create empty texture for render-to-texture
    static GLuint CreateEmptyTexture(int width, int height, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE) {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, type, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        return texture;
    }
};

} // namespace Utils
