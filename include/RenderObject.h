#pragma once

#include <glBuffer.h>
#include <glVAO.h>

class RenderObject {
public:
    RenderObject(glShader& program, glBuffer& VBO, glBuffer& EBO, glVAO& VAO) :
        shaderProgram(program), vertexBuffer(VBO), elementBuffer(EBO), vertexArray(VAO)
    {}

    glShader& getShader() {
        return shaderProgram;
    }

    glBuffer& getVBO() {
        return vertexBuffer;
    }

    glBuffer& getEBO() {
        return elementBuffer;
    }

    glVAO& getVAO() {
        return vertexArray;
    }

    glm::mat4 modelMat() {
        return glm::mat4(1.f);
    }
private:
    // VBO/EBO and VAO could be owned by the RO, but shader probably not.
    glShader& shaderProgram;
    glBuffer& vertexBuffer;
    glBuffer& elementBuffer;
    glVAO& vertexArray;
    //glm::vec3 m_position = glm::vec3(0.f, 0.f, 0.f);
    //glm::vec3 m_scale = glm::vec3(0.f, 0.f, 0.f);
    //glm::vec3 m_orientation = glm::vec3(0.f, 0.f, 0.f);
};