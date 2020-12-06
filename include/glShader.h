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

struct ShaderSrcInfo {
    std::string path;
    GLenum type;
};

class glShader {
public:
    glShader() : m_ID(0) {}
    glShader(std::vector<ShaderSrcInfo> sources) {
        std::vector<GLuint> compiledShaders;
        for (ShaderSrcInfo shaderInfo : sources) {
            std::string src = openSource(shaderInfo.path);
            GLuint compiledSrc = compileSource(src, shaderInfo.type);
            compiledShaders.push_back(compiledSrc);
        }
        m_ID = glCreateProgram();
        for (GLuint shader : compiledShaders) {
            glAttachShader(m_ID, shader);
        }
        glLinkProgram(m_ID);
        int success;
        char infoLog[512];
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
            std::cerr << "Error linking shader program:\n" << infoLog << "\n";
        }
        for (GLuint shader : compiledShaders) {
            glDeleteShader(shader);
        }
    }
    ~glShader() {
        // fix
    }
    void use() {
        glUseProgram(m_ID);
    }
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

    GLuint m_ID = 0;
public:
    void DeleteProgram() {
        glDeleteProgram(m_ID);
        m_ID = 0;
    }
    GLuint ID() {
        return m_ID;
    }
    void setUniform(std::string name, glm::vec4 value) {
        GLint location = glGetUniformLocation(m_ID, name.c_str());
        if (location < 0) {
            std::cerr << "Shader error: Could not obtain location of uniform: \"" << name << "\".\n";
            return;
        }
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }
    void setUniform(std::string name, glm::mat4 value) {
        GLint location = glGetUniformLocation(m_ID, name.c_str());
        if (location < 0) {
            std::cerr << "Shader error: Could not obtain location of uniform: \"" << name << "\".\n";
            return;
        }
        glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
    }
};