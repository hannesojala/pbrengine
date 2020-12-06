#pragma once

#include <glWindow.h>
#include <glBuffer.h>
#include <glVAO.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/ext/quaternion_float.hpp>

class RenderObject {
public:
    RenderObject(glShader& shader, glBuffer& VBO, glVAO& VAO) :
        m_shader(shader), m_VBO(VBO), m_VAO(VAO)
    {}
    glShader& getShader() {
        return m_shader;
    }
    glBuffer& getVBO() {
        return m_VBO;
    }
    glVAO& getVAO() {
        return m_VAO;
    }
    glm::mat4 modelMat() {
        return glm::mat4(1.f);
    }
private:
    glShader& m_shader;
    glBuffer& m_VBO;
    glVAO& m_VAO;
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
        if(keystate[SDL_SCANCODE_W]) cameraPos += float(dt_seconds) * 1.F * cameraFwd;
        if(keystate[SDL_SCANCODE_A]) cameraPos -= float(dt_seconds) * glm::normalize(glm::cross(cameraFwd, cameraUp));
        if(keystate[SDL_SCANCODE_S]) cameraPos -= float(dt_seconds) * 1.F * cameraFwd;
        if(keystate[SDL_SCANCODE_D]) cameraPos += float(dt_seconds) * glm::normalize(glm::cross(cameraFwd, cameraUp));
        if(keystate[SDL_SCANCODE_UP])    cameraPitch(float(dt_seconds) * 1.f);
        if(keystate[SDL_SCANCODE_DOWN])  cameraPitch(float(dt_seconds) *-1.F);
        if(keystate[SDL_SCANCODE_LEFT])  cameraYaw(float(dt_seconds) * 1.F);
        if(keystate[SDL_SCANCODE_RIGHT]) cameraYaw(float(dt_seconds) * -1.F);
        if(keystate[SDL_SCANCODE_Q])     cameraRoll(float(dt_seconds) * 1.F);
        if(keystate[SDL_SCANCODE_E])     cameraRoll(float(dt_seconds) * -1.F);
    }

    void render() {

        glClearColor(1.0, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        for (RenderObject ob : toRender) {
            ob.getShader().use();
            glm::mat4 proj = glm::perspective(glm::radians(45.f), window.getAspect(), 0.03125f, 16.f);
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFwd, cameraUp);
            ob.getShader().setUniform("u_mvp", proj * view * ob.modelMat());
            ob.getVAO().bind();
            glDrawArrays(GL_TRIANGLES, 0, ob.getVBO().size());
        }

        window.swap();
    }
private:
    glWindow window;
    glShader program;
    glBuffer VBO;
    glVAO VAO;
    std::vector<RenderObject> toRender;

    // Camera
    glm::vec3 cameraPos     = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraFwd     = glm::vec3(0.0f, 0.0f,-1.0f);
    glm::vec3 cameraRight   = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp      = glm::vec3(0.0f, 1.0f, 0.0f);
    void cameraPitch(float angle) {
        cameraFwd = glm::normalize((cameraFwd * cos(angle)) + (cameraUp * sin(angle)));
        cameraUp = glm::normalize(glm::cross(cameraRight, cameraFwd));
    }
    void cameraYaw(float angle) {
        cameraRight = glm::normalize((cameraRight * cos(angle)) + (cameraFwd * sin(angle)));
        cameraFwd = glm::normalize(glm::cross(cameraUp, cameraRight));
    }
    void cameraRoll(float angle) {
        cameraRight = glm::normalize((cameraRight * cos(angle)) + (cameraUp * sin(angle)));
        cameraUp = glm::normalize(glm::cross(cameraRight, cameraFwd));
    }

     // Time variables
    Uint64 time_init;
    Uint64 time_prev;
    Uint64 time_curr;
    double dt_seconds;
public:
    bool running;
};