#pragma once

#include <SDL2/SDL.h>
#include <iostream>

class Window {
public:
    bool init(const char *title, int width, int height, int fpsLock);
    void destroy();

    SDL_Window *getWindow() const;
    SDL_Renderer *getRenderer() const;
private:
    SDL_Window *m_window=nullptr;
    SDL_Renderer *m_renderer=nullptr;
};