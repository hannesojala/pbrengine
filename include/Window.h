#pragma once

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <vector>

#include <Shader.h>

class Window {
public:
    // memory leak note: seems that simply SDL_Init() followed by SDL_Quit() causes a memory leak of 80kb!
    Window(int width, int height, std::string title) {
        SDL_Init(SDL_INIT_VIDEO);
        auto windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

        p_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
        if (!p_window) std::cerr << "Could not create window!" << SDL_GetError() << "\n";

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        gl_context = SDL_GL_CreateContext(p_window);
        SDL_GL_MakeCurrent(p_window, gl_context);
        if (!gl_context) std::cerr << "Context could not be created: " << SDL_GetError() << "\n";
        if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) std::cerr << "Failed to initialize GLAD: " << SDL_GetError() << "\n";
        
        glViewport(0, 0, width, height);
    }
    ~Window() {
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(p_window);
        SDL_Quit();
    }

    void toggleFullscreen() {
        if (SDL_GetWindowFlags(p_window) & SDL_WINDOW_FULLSCREEN_DESKTOP) SDL_SetWindowFullscreen(p_window, 0);
        else SDL_SetWindowFullscreen(p_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    void swap() 
        { SDL_GL_SwapWindow(p_window); }

    void setSwap(int mode) 
        { SDL_GL_SetSwapInterval(mode); }

    int getSwap() 
        { return SDL_GL_GetSwapInterval(); }

    void getSize(int& width, int& height) 
        { SDL_GetWindowSize(p_window, &width, &height); }

    int pollEvents() 
        { return SDL_PollEvent(&event); }

    Uint32 getEventType() 
        { return event.type; }

    Uint8 getWindowEvent() 
        { return event.window.event; }

    float getAspect() {
        int width, height;
        SDL_GetWindowSize(p_window, &width, &height);
        return float(width)/float(height);
    }

    SDL_Event event;
    
private:
    SDL_Window* p_window;
    SDL_GLContext gl_context;
};