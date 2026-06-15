#include "Window.h"
#include <SDL2/SDL.h>


bool Window::init(const char *title, int width, int height, int fpsLock, std::string videoMode="windowed"){
    Uint32 flags = fpsLock==0 ? SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC : SDL_RENDERER_ACCELERATED;
    m_window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );
    if(!m_window) { SDL_Quit(); return false; }

    if(videoMode == "fullscreen"){
        if(SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0){
            std::cerr << "Failed to set fullscreen mode: "
                    << SDL_GetError() << "\n";
            destroy();
            return false;
        }
    }
    else if(videoMode == "borderless"){
        if(SDL_SetWindowFullscreen(m_window, 0) != 0){
            std::cerr << "Failed to leave fullscreen mode: "
                    << SDL_GetError() << "\n";
            destroy();
            return false;
        }

        SDL_SetWindowBordered(m_window, SDL_FALSE);

        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);
        SDL_SetWindowSize(m_window, dm.w, dm.h);
        SDL_SetWindowPosition(m_window, 0, 0);
    }
    else { // windowed
        if(SDL_SetWindowFullscreen(m_window, 0) != 0){
            std::cerr << "Failed to leave fullscreen mode: "
                    << SDL_GetError() << "\n";
            destroy();
            return false;
        }

        SDL_SetWindowBordered(m_window, SDL_TRUE);
        SDL_SetWindowSize(m_window, width, height);
        SDL_SetWindowPosition(
            m_window,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED
        );
    }

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