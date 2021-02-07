#pragma once

#include <glad/glad.h>
#include <utility>

class GLName {
public:
    GLName() = default;

    GLName(GLuint ID) : ID(ID) {}

    // Copy Ctor and Copy assignment = delete
    GLName(const GLName& other) = delete;
    GLName& operator=(const GLName& other) = delete;

    // Move Ctor and Copy assignment
    GLName(GLName&& other) noexcept : 
        ID(std::exchange(other.ID, 0)) {}

    GLName& operator=(GLName&& other) noexcept {
        std::swap(ID, other.ID);
        return *this;
    }

    GLuint get() const {
        return ID;
    }
private:
    GLuint ID = 0;
};  