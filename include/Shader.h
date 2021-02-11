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

GLuint compileSource(std::string src, GLenum type) {
    auto shader = glCreateShader(type);
    auto src_c_str = src.c_str();
    glShaderSource(shader, 1, &src_c_str, NULL);
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

GLuint create_shader(const std::vector<ShaderSrcInfo>& sources) {
    auto name = glCreateProgram();

    std::vector<GLuint> compiled_shaders;

    for (ShaderSrcInfo shader_info : sources) {
        std::ifstream ifs(shader_info.path);
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        compiled_shaders.push_back(compileSource(buffer.str(), shader_info.type));
    }

    for (GLuint shader : compiled_shaders) glAttachShader(name, shader);

    glLinkProgram(name);

    int success;
    char log_buff[512];
    glGetProgramiv(name, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(name, 512, NULL, log_buff);
        std::cerr << "Error linking shader program:\n" << log_buff << "\n";
    }

    for (GLuint shader : compiled_shaders) glDeleteShader(shader);

    return name;
}

bool checkLocErr(GLint location, const std::string& name) {
    static bool SUPPRESS_LOC_ERRS = false;
    if (location >= 0 || SUPPRESS_LOC_ERRS) return false;
    std::cerr << "Shader error: Could not obtain location of uniform: \"" << name << "\".\n";
    return (SUPPRESS_LOC_ERRS = true);
}

void setUniform(GLuint program, std::string uniform, glm::vec4 value) {
    GLint location = glGetUniformLocation(program, uniform.c_str());
    checkLocErr(location, uniform);
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void setUniform(GLuint program, std::string uniform, glm::mat4 value) {
    GLint location = glGetUniformLocation(program, uniform.c_str());
    checkLocErr(location, uniform);
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
}

void setUniform(GLuint program, std::string uniform, GLuint value) {
    GLint location = glGetUniformLocation(program, uniform.c_str());
    checkLocErr(location, uniform);
    glUniform1i(location, value);
}