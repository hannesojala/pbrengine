#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

// Spaceship style relative camera
class Camera {
public:
    Camera() = default;

    Camera(vec3 position, vec3 forward, vec3 up, float fov):
        position(position), 
        forward(forward), 
        up(up),
        right(cross(up, forward)), 
        fov(fov){}

    vec3 position = vec3(0.0f, 0.0f, 0.0f);
    vec3 forward  = vec3(0.0f, 0.0f,-1.0f);
    vec3 up       = vec3(0.0f, 1.0f, 0.0f);
    vec3 right    = vec3(1.0f, 0.0f, 0.0f);

    float fov = radians(90.f);

    void pitchView(float radians) {
        forward = normalize((forward * cos(radians)) + (up * sin(radians)));
        up = normalize(cross(right, forward));
    }

    void yawView(float radians) {
        right = normalize((right * cos(radians)) + (forward * sin(radians)));
        forward = normalize(cross(up, right));
    }
    
    void rollView(float radians) {
        right = normalize((right * cos(radians)) + (up * sin(radians)));
        up = normalize(cross(right, forward));
    }
};
