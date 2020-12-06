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
        glGenVertexArrays(1, &ID);
        if (!ID)
            std::cerr << "VAO Error.\n";
        glBindVertexArray(ID);
        unsigned int stride = 0;
        unsigned int offset = 0;
        for (unsigned int i = 0; i < attributes.size(); i++) {
            stride += attributes[i].size;
        }
        for (unsigned int i = 0; i < attributes.size(); i++) {
            GLint attribute = glGetAttribLocation(program.getID(), attributes[i].name.c_str());
            if (attribute < 0) std::cerr << "Attribute error: " << std::hex << glGetError() << "\n";
            glVertexAttribPointer(attribute, attributes[i].size, GL_FLOAT, false,
                stride * sizeof(GLfloat), (void*) (offset * sizeof(GLfloat)));
            glEnableVertexAttribArray(attribute);
            offset += attributes[i].size;
        }
    }
    ~glVAO() {
        glDeleteVertexArrays(1, &ID);
    }
private:
    GLuint ID;
public:
    void bind() {
        glBindVertexArray(ID);
    }
    void unbind() {
        glBindVertexArray(0);
    }
};