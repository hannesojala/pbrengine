#pragma once

#include <glad/glad.h>
#include <utility>

class glName {
public:
    glName() = default;

    glName(GLuint ID) : ID(ID) {}

    // Copy Ctor and Copy assignment = delete
    glName(const glName& other) = delete;
    glName& operator=(const glName& other) = delete;

    // Move Ctor and Copy assignment
    glName(glName&& other) noexcept : 
        ID(std::exchange(other.ID, 0)) {}

    glName& operator=(glName&& other) noexcept {
        std::swap(ID, other.ID);
        return *this;
    }

    GLuint get() const {
        return ID;
    }
private:
    GLuint ID = 0;
};  