#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Spaceship style relative camera
class Camera {
public:
    Camera(){}

    Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up, float fov):
        position(position), 
        forward(forward), 
        up(up),
        right(glm::cross(up, forward)), 
        fov(fov){}

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 forward  = glm::vec3(0.0f, 0.0f,-1.0f);
    glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right    = glm::vec3(1.0f, 0.0f, 0.0f);

    float fov = glm::radians(90.f);

    void pitchView(float radians) {
        forward = glm::normalize((forward * cos(radians)) + (up * sin(radians)));
        up = glm::normalize(glm::cross(right, forward));
    }

    void yawView(float radians) {
        right = glm::normalize((right * cos(radians)) + (forward * sin(radians)));
        forward = glm::normalize(glm::cross(up, right));
    }
    
    void rollView(float radians) {
        right = glm::normalize((right * cos(radians)) + (up * sin(radians)));
        up = glm::normalize(glm::cross(right, forward));
    }
};
