#include "Window.h"
#include <SDL2/SDL.h>

bool Window::init(const char *title, int width, int height, int fpsLock){
    Uint32 flags = fpsLock==0 ? SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC : SDL_RENDERER_ACCELERATED;
    m_window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN
    );
    if(!m_window) { SDL_DestroyWindow(m_window); SDL_Quit(); return false; }
    m_renderer = SDL_CreateRenderer(m_window, -1, flags);
    if(!m_renderer) { destroy(); return false; }
    return true;
}

void Window::destroy(){
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

SDL_Window *Window::getWindow() const{
    return m_window;
}
SDL_Renderer *Window::getRenderer() const{
    return m_renderer;
}