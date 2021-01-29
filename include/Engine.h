#pragma once

#include <glWindow.h>
#include <glVAO.h>
#include <Shapes.h>
#include <glTexture.h>
#include <Camera.h>
#include <RenderObject.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float MOVESPEED = 5.0;
const float CAMERASPEED = 2.5;

auto shaders = std::vector<ShaderSrcInfo>{
    {"vert.glsl", GL_VERTEX_SHADER},
    {"frag.glsl", GL_FRAGMENT_SHADER}
};

auto attributes = std::vector<Attribute>{
    {"a_position", 3},
    {"a_uvCoords", 2}
};

// not mine
void debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
	auto const src_str = [source]() {
		switch (source)
		{
		case GL_DEBUG_SOURCE_API: return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER: return "OTHER";
		}
        return "This shouldn't happen!";
	}();

	auto const type_str = [type]() {
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR: return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER: return "MARKER";
		case GL_DEBUG_TYPE_OTHER: return "OTHER";
		}
        return "This shouldn't happen!";
	}();

	auto const severity_str = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW: return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
		}
        return "This shouldn't happen!";
	}();

	std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}

class Engine {
public:
    Engine(int width, int height) :
        window(glWindow(width, height, "PBR Engine")),
        camera(Camera()),
        running(true)
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        
        if (IN_DEBUG_MODE) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(debug_message_callback, nullptr);
        }

        std::cout << "Using: " << glGetString(GL_RENDERER) << "\n";
        
        program = glShader(shaders);
        texture = texFromImg(TEXTURE_FILENAME);
        VBO = glBuffer(CubeFlat::vertices, sizeof(CubeFlat::vertices), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
        EBO = glBuffer(CubeFlat::indices, sizeof(CubeFlat::indices), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
        VAO = glVAO(attributes, program);
        toRender.push_back(
            RenderObject(
                program,
                VBO,
                EBO,
                VAO
            )
        );
    }
    ~Engine() 
    {
        glDeleteTextures(1, &texture);
    }

    void startFrame() {
        time_prev = time_curr;
        time_curr = SDL_GetPerformanceCounter();
        dt_seconds = (double) (time_curr - time_prev) / (double) SDL_GetPerformanceFrequency();
    }

    void events() {
        while(window.pollEvents()) {
            switch(window.getEventType()) {
            case SDL_KEYDOWN :
                switch(window.event.key.keysym.sym) {
                    case SDLK_p :
                        std::cout << "FPS: " << 1/dt_seconds << "\n";
                        break;
                    case SDLK_v :
                        window.setSwap(!window.getSwap());
                        break;
                }
                break;
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
        if(keystate[SDL_SCANCODE_W])        camera.position += float(dt_seconds) * MOVESPEED * camera.forward;
        if(keystate[SDL_SCANCODE_A])        camera.position -= float(dt_seconds) * MOVESPEED * camera.right;
        if(keystate[SDL_SCANCODE_S])        camera.position -= float(dt_seconds) * MOVESPEED * camera.forward;
        if(keystate[SDL_SCANCODE_D])        camera.position += float(dt_seconds) * MOVESPEED * camera.right;
        if(keystate[SDL_SCANCODE_UP])       camera.pitchView(float(dt_seconds) * CAMERASPEED);
        if(keystate[SDL_SCANCODE_DOWN])     camera.pitchView(float(dt_seconds) *-CAMERASPEED);
        if(keystate[SDL_SCANCODE_LEFT])     camera.yawView(float(dt_seconds)   * CAMERASPEED);
        if(keystate[SDL_SCANCODE_RIGHT])    camera.yawView(float(dt_seconds)   *-CAMERASPEED);
        if(keystate[SDL_SCANCODE_Q])        camera.rollView(float(dt_seconds)  * CAMERASPEED);
        if(keystate[SDL_SCANCODE_E])        camera.rollView(float(dt_seconds)  *-CAMERASPEED);
    }

    void render() {

        glClearColor(1.0, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (RenderObject ob : toRender) {
            ob.getShader().use();
            glm::mat4 proj = glm::perspective(camera.fov, window.getAspect(), 0.03125f, 64.f);
            glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.forward, camera.up);
            ob.getShader().setUniform("u_mvp", proj * view * ob.modelMat());

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            ob.getShader().setUniform("u_texture", 0);

            ob.getVAO().bind();
            ob.getEBO().bind();
            glDrawElements(GL_TRIANGLES, ob.getEBO().getSize(), GL_UNSIGNED_INT, 0);
        }

        window.swap();
    }
private:
    glWindow window;
    // This should be in some data struct with render objects grabbing references to elements
    glShader program;
    // These should be owned by the render object, maybe not so in direct state access
    glBuffer VBO;
    glBuffer EBO;
    glVAO VAO;
    GLuint texture;
    // rethonk
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