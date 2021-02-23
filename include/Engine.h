#pragma once

#include <Window.h>
#include <Texture.h>
#include <Camera.h>
#include <Scene.h>
#include <Debug.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/* TODO: Figure out why black screen rarely */

/* Shaders to be compiled into program */
auto shaders = std::vector<ShaderSrcInfo>{
    {"vert.glsl", GL_VERTEX_SHADER},
    {"frag.glsl", GL_FRAGMENT_SHADER}
};

/* Runs main loop */
class Engine {
public:
    Engine(int width, int height) :
        window(Window(width, height, "PBR Engine")),
        camera(Camera()),
        running(true) 
    {
        /* Enable OpenGL capabilities */
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        
        /* -d flag */
        if (IN_DEBUG_MODE) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(debug_message_callback, nullptr);
            std::cout << "Using: " << glGetString(GL_RENDERER) << "\n";
        }

        program = create_shader(shaders);
        model = import_model("Helm");
        camera.position -= 16.0f * camera.forward;
    }

    ~Engine() {   
        /* Delete shader and all meshes in model */
        glDeleteProgram(program);
        /* !!! Note/Warning: does not account for child models !!! */
        for (auto mesh : model.meshes) {
            glDeleteTextures(1, &(mesh.material.albedo));
            glDeleteTextures(1, &(mesh.material.metal_rough));
            glDeleteTextures(1, &(mesh.material.normal));
            glDeleteBuffers(1, &(mesh.vbo));
            glDeleteBuffers(1, &(mesh.ibo));
            glDeleteVertexArrays(1, &(mesh.vao));
        }
    }

    /* Get frame time and frequency */
    void startFrame() {
        time_prev = time_curr;
        time_curr = SDL_GetPerformanceCounter();
        dt_seconds = (double) (time_curr - time_prev) / (double) SDL_GetPerformanceFrequency();
    }

    /* Process SDL event queue */
    void events() {
        while(window.pollEvents()) {
            switch(window.getEventType()) {
            case SDL_KEYDOWN :
                switch(window.event.key.keysym.sym) {
                case SDLK_p :
                    std::cout << "FPS: " << 1.0/dt_seconds << "\n";
                    break;
                case SDLK_v :
                    window.setSwap(!window.getSwap());
                    break;
                case SDLK_F11 :
                    window.toggleFullscreen();
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
                /* Resize viewport with window */
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

    /* Update state with delta time */
    void update() {
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if(keystate[SDL_SCANCODE_W])        camera.position += float(dt_seconds) * 5.0f * camera.forward;
        if(keystate[SDL_SCANCODE_A])        camera.position -= float(dt_seconds) * 5.0f * camera.right;
        if(keystate[SDL_SCANCODE_S])        camera.position -= float(dt_seconds) * 5.0f * camera.forward;
        if(keystate[SDL_SCANCODE_D])        camera.position += float(dt_seconds) * 5.0f * camera.right;
        if(keystate[SDL_SCANCODE_UP])       camera.pitchView(float(dt_seconds) * 2.5f);
        if(keystate[SDL_SCANCODE_DOWN])     camera.pitchView(float(dt_seconds) *-2.5f);
        if(keystate[SDL_SCANCODE_LEFT])     camera.yawView(float(dt_seconds)   * 2.5f);
        if(keystate[SDL_SCANCODE_RIGHT])    camera.yawView(float(dt_seconds)   *-2.5f);
        if(keystate[SDL_SCANCODE_Q])        camera.rollView(float(dt_seconds)  * 2.5f);
        if(keystate[SDL_SCANCODE_E])        camera.rollView(float(dt_seconds)  *-2.5f);
    }

    /* Recursively render model and its children using provided projection and view */
    void renderModel(Model model, glm::mat4 proj_view) {
        /* Set shader uniforms */
        setUniform(program, "u_mvp", proj_view * glm::mat4(1.f));
        setUniform(program, "u_model", glm::mat4(1.f));
        setUniform(program, "u_view_pos", glm::vec4(camera.position, 1.0f));

        /* Draw model meshes */
        for (Mesh mesh : model.meshes) {
            /* Bind textures and set sampler uniforms */
            glBindTextureUnit(0, mesh.material.albedo);
            setUniform(program, "u_diffuse_map", 0);

            glBindTextureUnit(1, mesh.material.metal_rough);
            setUniform(program, "u_mtl_rgh_map", 1);

            glBindTextureUnit(2, mesh.material.normal);
            setUniform(program, "u_normal_map",  2);
            
            /* Draw indexed */
            glBindVertexArray(mesh.vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
            glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, 0);
        }
        /* Recurse */
        for (Model child : model.children) renderModel(child, proj_view); 
    }

    /* Clears and renders model using program */
    void render() {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        auto proj = glm::perspective(camera.fov, window.getAspect(), 0.03125f, 64.f);
        auto view = glm::lookAt(camera.position, camera.position + camera.forward, camera.up);
        renderModel(model, proj * view);

        window.swap();
    }
private:
    Window window;
    GLuint program;
    Camera camera;

    Model model;

    Uint64 time_init  = 0,
           time_prev  = 0,
           time_curr  = 0;
    double dt_seconds = 0;

public:
    bool running;
};