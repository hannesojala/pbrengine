#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glName.h>

struct ShaderSrcInfo {
    std::string path;
    GLenum type;
};

class glShader {
public:
    glShader() = default;
    glShader(const std::vector<ShaderSrcInfo> & sources) : gl_name(glName(glCreateProgram()))
    {
        std::vector<GLuint> compiledShaders;

        for (ShaderSrcInfo shaderInfo : sources) {
            std::string src = openSource(shaderInfo.path);
            GLuint compiledSrc = compileSource(src, shaderInfo.type);
            compiledShaders.push_back(compiledSrc);
        }

        for (GLuint shader : compiledShaders) {
            glAttachShader(gl_name.get(), shader);
        }

        glLinkProgram(gl_name.get());

        int success;
        char infoLog[512];
        glGetProgramiv(gl_name.get(), GL_LINK_STATUS, &success);

        if(!success) {
            glGetProgramInfoLog(gl_name.get(), 512, NULL, infoLog);
            std::cerr << "Error linking shader program:\n" << infoLog << "\n";
        }

        for (GLuint shader : compiledShaders) {
            glDeleteShader(shader);
        }
    }

    ~glShader() {
        glDeleteProgram(gl_name.get());
    }

    // Default move ctor and operator
    glShader(glShader &&other) = default;
    glShader &operator=(glShader &&other) = default;

private:
    std::string openSource(std::string path) {
        std::ifstream ifs(path);
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        return buffer.str();
    }

    GLuint compileSource(std::string src, GLenum type) {
        GLuint shader = glCreateShader(type);
        const GLchar* c_src = src.c_str();
        glShaderSource(shader, 1, &c_src, NULL);
        glCompileShader(shader);
        int success;
        char infoLogVert[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLogVert);
            std::cerr << "Error compiling shader source:\n" << src << "\n" << infoLogVert << "\n";
        }
        return shader;
    }

    glName gl_name;
public:
    GLuint getID() const {
        return gl_name.get();
    }

    void setUniform(std::string uniform, glm::vec4 value) {
        GLint location = glGetUniformLocation(gl_name.get(), uniform.c_str());
        if (location < 0) {
            std::cerr << "Shader error: Could not obtain location of uniform: \"" << uniform << "\".\n";
            return;
        }
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void setUniform(std::string uniform, glm::mat4 value) {
        GLint location = glGetUniformLocation(gl_name.get(), uniform.c_str());
        if (location < 0) {
            std::cerr << "Shader error: Could not obtain location of uniform: \"" << uniform << "\".\n";
            return;
        }
        glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
    }

    void setUniform(std::string uniform, GLuint value) {
        GLint location = glGetUniformLocation(gl_name.get(), uniform.c_str());
        if (location < 0) {
            std::cerr << "Shader error: Could not obtain location of uniform: \"" << uniform << "\".\n";
            return;
        }
        glUniform1i(location, value);
    }

    void use() {
        glUseProgram(gl_name.get());
    }
};