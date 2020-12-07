#pragma once

#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <glShader.h>

struct Attribute {
    std::string name;
    unsigned int size;
};

class glVAO {
public:
    glVAO() = default;
    glVAO(const std::vector<Attribute>& attributes, const glShader& program) 
    {
        GLuint ID;
        glGenVertexArrays(1, &ID);
        gl_name = glName(ID);
        if (!gl_name.get())
            std::cerr << "VAO Error.\n";
        glBindVertexArray(gl_name.get());
        unsigned int stride = 0;
        unsigned int offset = 0;
        for (unsigned int i = 0; i < attributes.size(); i++) {
            stride += attributes[i].size;
        }
        for (Attribute attrib : attributes) {
            GLint attribute = glGetAttribLocation(program.getID(), attrib.name.c_str());
            if (attribute < 0) std::cerr << "Attribute " << attrib.name << " error: " << std::hex << glGetError() << "\n";
            glVertexAttribPointer(attribute, attrib.size, GL_FLOAT, false,
                stride * sizeof(GLfloat), (void*) (offset * sizeof(GLfloat)));
            glEnableVertexAttribArray(attribute);
            offset += attrib.size;
        }
    }

    // Default move ctor and operator
    glVAO(glVAO &&other) = default;
    glVAO &operator=(glVAO &&other) = default;

    ~glVAO() {
        GLuint ID = gl_name.get();
        glDeleteVertexArrays(1, &ID);
    }
private:
    glName gl_name;
public:
    void bind() {
        glBindVertexArray(gl_name.get());
    }
    void unbind() {
        glBindVertexArray(0);
    }
};