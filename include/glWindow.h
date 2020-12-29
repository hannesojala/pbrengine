#pragma once

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <vector>

#include <glShader.h>

class glWindow {
public:
    glWindow(int width, int height, std::string title) {
        SDL_Init(SDL_INIT_VIDEO);
        Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
        pWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
        if (!pWindow) 
            std::cerr << "Could not create window!" << SDL_GetError() << "\n";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        glContext = SDL_GL_CreateContext(pWindow);
        SDL_GL_MakeCurrent(pWindow, glContext);
        if (!glContext) 
            std::cerr << "Context could not be created: " << SDL_GetError() << "\n";
        if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress))
            std::cerr << "Failed to initialize GLAD: " << SDL_GetError() << "\n";
        glViewport(0, 0, width, height);
    }
    ~glWindow() {
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
    }
private:
    SDL_Window* pWindow;
    SDL_GLContext glContext;
public:
    SDL_Event event;
    void swap() {
        SDL_GL_SwapWindow(pWindow);
    }
    void setSwap(int mode) {
        SDL_GL_SetSwapInterval(mode);
    }
    int getSwap() {
        return SDL_GL_GetSwapInterval();
    }
    void getSize(int& width, int& height) {
        SDL_GetWindowSize(pWindow, &width, &height);
    }
    int pollEvents() {
        return SDL_PollEvent(&event);
    }
    Uint32 getEventType() {
        return event.type;
    }
    Uint8 getWindowEvent() {
        return event.window.event;
    }
    float getAspect() {
        int width, height;
        SDL_GetWindowSize(pWindow, &width, &height);
        return float(width)/float(height);
    }
};