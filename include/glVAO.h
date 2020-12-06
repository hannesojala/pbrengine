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
    glVAO(const std::vector<Attribute>& attributes, glShader program) 
    {
        glGenVertexArrays(1, &m_ID);
        if (!m_ID)
            std::cerr << "VAO Error.\n";
        glBindVertexArray(m_ID);
        unsigned int stride = 0;
        unsigned int offset = 0;
        for (unsigned int i = 0; i < attributes.size(); i++) {
            stride += attributes[i].size;
        }
        for (unsigned int i = 0; i < attributes.size(); i++) {
            GLint attribute = glGetAttribLocation(program.ID(), attributes[i].name.c_str());
            if (attribute < 0) std::cerr << "Attribute error: " << std::hex << glGetError() << "\n";
            glVertexAttribPointer(attribute, attributes[i].size, GL_FLOAT, false,
                stride * sizeof(GLfloat), (void*) (offset * sizeof(GLfloat)));
            glEnableVertexAttribArray(attribute);
            offset += attributes[i].size;
        }
    }
    ~glVAO() {
        glDeleteVertexArrays(1, &m_ID);
    }
private:
    GLuint m_ID;
public:
    void bind() {
        glBindVertexArray(m_ID);
    }
    void unbind() {
        glBindVertexArray(0);
    }
};