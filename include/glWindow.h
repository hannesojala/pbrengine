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
        Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
        m_pWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        if (!m_pWindow) 
            std::cerr << "Could not create window!" << SDL_GetError() << "\n";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        m_context = SDL_GL_CreateContext(m_pWindow);
        SDL_GL_MakeCurrent(m_pWindow, m_context);
        if (!m_context) 
            std::cerr << "Context could not be created: " << SDL_GetError() << "\n";
        if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress))
            std::cerr << "Failed to initialize GLAD: " << SDL_GetError() << "\n";
        int x, y;
        getSize(x, y);
        glViewport(0, 0, x, y);
    }
    ~glWindow() {
        SDL_GL_DeleteContext(m_context);
        SDL_DestroyWindow(m_pWindow);
        SDL_Quit();
    }
private:
    SDL_Window* m_pWindow;
    SDL_GLContext m_context;
    SDL_Event m_event;
public:
    void swap() {
        SDL_GL_SwapWindow(m_pWindow);
    }
    void getSize(int& width, int& height) {
        SDL_GetWindowSize(m_pWindow, &width, &height);
    }
    int pollEvents() {
        return SDL_PollEvent(&m_event);
    }
    Uint32 getEventType() {
        return m_event.type;
    }
    Uint8 getWindowEvent() {
        return m_event.window.event;
    }
    float getAspect() {
        int width, height;
        SDL_GetWindowSize(m_pWindow, &width, &height);
        return float(width)/float(height);
    }
};