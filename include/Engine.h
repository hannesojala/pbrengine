#pragma once

#include <glWindow.h>
#include <glBuffer.h>
#include <glVAO.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/ext/quaternion_float.hpp>

// Spaceship style relative camera
class Camera {
public:
    Camera(){}
    Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up, float fov):
        position(position), forward(forward), up(up),
        right(glm::cross(up, forward)), fov(fov){}
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

class RenderObject {
public:
    RenderObject(glShader& program, glBuffer& VBO, glVAO& VAO) :
        shaderProgram(program), vertexBuffer(VBO), vertexArray(VAO)
    {}
    glShader& getShader() {
        return shaderProgram;
    }
    glBuffer& getVBO() {
        return vertexBuffer;
    }
    glVAO& getVAO() {
        return vertexArray;
    }
    glm::mat4 modelMat() {
        return glm::mat4(1.f);
    }
private:
    glShader& shaderProgram;
    glBuffer& vertexBuffer;
    glVAO& vertexArray;
    //glm::vec3 m_position = glm::vec3(0.f, 0.f, 0.f);
    //glm::vec3 m_scale = glm::vec3(0.f, 0.f, 0.f);
    //glm::vec3 m_orientation = glm::vec3(0.f, 0.f, 0.f); // use quat instead?
};

GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0,     1.0, 0.0, 0.0, 1.0,
     0.5f, -0.5f, 0.0f, 1.0,     0.0, 1.0, 0.0, 1.0,
     0.0f,  0.5f, 0.0f, 1.0,     0.0, 0.0, 1.0, 1.0
};

auto shaders = std::vector<ShaderSrcInfo>{
    {"vert.glsl", GL_VERTEX_SHADER},
    {"frag.glsl", GL_FRAGMENT_SHADER}
};

auto attributes = std::vector<Attribute>{
    {"a_position", 4},
    {"a_color", 4}
};

class Engine {
public:
    Engine(int width, int height) :
        window(glWindow(width, height, "REEEEEEEEEEEEEEEEEEEe")),
        program(glShader(shaders)),
        VBO(glBuffer(vertices, sizeof(vertices), GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW)),
        VAO(glVAO(attributes, program)),
        camera(Camera()),
        running(true)
    {
        toRender.push_back(
            RenderObject(
                program,
                VBO,
                VAO
            )
        );
    }
    ~Engine() 
    {}

    void startFrame() {
        time_prev = time_curr;
        time_curr = SDL_GetPerformanceCounter();
        dt_seconds = (double) (time_curr - time_prev) / (double) SDL_GetPerformanceFrequency();
    }

    void events() {
        while(window.pollEvents()) {
            switch(window.getEventType()) {
            case SDL_QUIT :
                running = false;
                break;
            case SDL_WINDOWEVENT :
                switch(window.getWindowEvent()) {
                case SDL_WINDOWEVENT_CLOSE :
                    running = false;
                    break;
                case SDL_WINDOWEVENT_SIZE_CHANGED :
                    int x, y;
                    window.getSize(x, y);
                    glViewport(0, 0, x, y);
                    break;
                }
                break;
            }
        }
    }

    void update() {
        // Access keys with keystate[SDL_SCANCODE(key)]
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if(keystate[SDL_SCANCODE_W]) camera.position += float(dt_seconds) * 1.F * camera.forward;
        if(keystate[SDL_SCANCODE_A]) camera.position -= float(dt_seconds) * glm::normalize(glm::cross(camera.forward, camera.up));
        if(keystate[SDL_SCANCODE_S]) camera.position -= float(dt_seconds) * 1.F * camera.forward;
        if(keystate[SDL_SCANCODE_D]) camera.position += float(dt_seconds) * glm::normalize(glm::cross(camera.forward, camera.up));
        if(keystate[SDL_SCANCODE_UP])    camera.pitchView(float(dt_seconds) * 1.f);
        if(keystate[SDL_SCANCODE_DOWN])  camera.pitchView(float(dt_seconds) *-1.F);
        if(keystate[SDL_SCANCODE_LEFT])  camera.yawView(float(dt_seconds) * 1.F);
        if(keystate[SDL_SCANCODE_RIGHT]) camera.yawView(float(dt_seconds) * -1.F);
        if(keystate[SDL_SCANCODE_Q])     camera.rollView(float(dt_seconds) * 1.F);
        if(keystate[SDL_SCANCODE_E])     camera.rollView(float(dt_seconds) * -1.F);
    }

    void render() {

        glClearColor(1.0, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        for (RenderObject ob : toRender) {
            ob.getShader().use();
            glm::mat4 proj = glm::perspective(camera.fov, window.getAspect(), 0.03125f, 64.f);
            glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.forward, camera.up);
            ob.getShader().setUniform("u_mvp", proj * view * ob.modelMat());
            ob.getVAO().bind();
            glDrawArrays(GL_TRIANGLES, 0, ob.getVBO().getSize());
        }

        window.swap();
    }
private:
    glWindow window;
    glShader program;
    glBuffer VBO;
    glVAO VAO;
    std::vector<RenderObject> toRender;
    Camera camera;

     // Time variables
    Uint64 time_init;
    Uint64 time_prev;
    Uint64 time_curr;
    double dt_seconds;
public:
    bool running;
};