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

class Shader {
public:
    Shader() = default;
    Shader(const std::vector<ShaderSrcInfo> & sources) : gl_name(glCreateProgram())
    {
        std::vector<GLuint> compiledShaders;

        for (ShaderSrcInfo shaderInfo : sources) {
            auto src = openSource(shaderInfo.path);
            auto compiledSrc = compileSource(src, shaderInfo.type);
            compiledShaders.push_back(compiledSrc);
        }

        for (GLuint shader : compiledShaders) glAttachShader(gl_name, shader);

        glLinkProgram(gl_name);

        int success;
        char log_buff[512];
        glGetProgramiv(gl_name, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(gl_name, 512, NULL, log_buff);
            std::cerr << "Error linking shader program:\n" << log_buff << "\n";
        }

        for (GLuint shader : compiledShaders) glDeleteShader(shader);
    }

private:
    std::string openSource(std::string path) {
        std::ifstream ifs(path);
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        return buffer.str();
    }

    GLuint compileSource(std::string src, GLenum type) {
        auto shader = glCreateShader(type);
        auto c_src = src.c_str();
        glShaderSource(shader, 1, &c_src, NULL);
        glCompileShader(shader);
        int success;
        char log_buff[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader, 512, NULL, log_buff);
            std::cerr << "Error compiling shader source:\n" << src << "\n" << log_buff << "\n";
        }
        return shader;
    }

    GLuint gl_name;
public:
    GLuint getID() const { return gl_name; }

    void use() { glUseProgram(gl_name); }

    bool LocErr(GLint location, const std::string& name) {
        if (location >= 0) return false;
        std::cerr << "Shader error: Could not obtain location of uniform: \"" << name << "\".\n";
        return true;
    }

    void setUniform(std::string uniform, glm::vec4 value) {
        GLint location = glGetUniformLocation(gl_name, uniform.c_str());
        if (LocErr(location, uniform)) std::cerr << "Shader error: Could not obtain location of uniform: \"" << uniform << "\".\n";
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void setUniform(std::string uniform, glm::mat4 value) {
        GLint location = glGetUniformLocation(gl_name, uniform.c_str());
        if (LocErr(location, uniform)) std::cerr << "Shader error: Could not obtain location of uniform: \"" << uniform << "\".\n";
        glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
    }

    void setUniform(std::string uniform, GLuint value) {
        GLint location = glGetUniformLocation(gl_name, uniform.c_str());
        if (LocErr(location, uniform)) std::cerr << "Shader error: Could not obtain location of uniform: \"" << uniform << "\".\n";
        glUniform1i(location, value);
    }
};