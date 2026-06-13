#include "DebugOverlay.h"

void DebugOverlay::init(TTF_Font *font, SDL_Renderer *renderer){
    m_font = font;
    m_renderer = renderer;
}

void DebugOverlay::update(float deltaMs){
    for(auto &msg : m_messages) msg.timeLeft-=deltaMs;
    m_messages.erase(
        std::remove_if(m_messages.begin(), m_messages.end(),
        [](const DebugMessage &m){ return m.timeLeft<=0; }),
        m_messages.end()
    );
}

int DebugOverlay::overflow(int c){
    if(c=='\n'){
        if(!m_line.empty()) m_messages.push_back({m_line, 5000.0f});
        m_line.clear();
    } else {
        m_line += (char)c;
    }
    return c;
}

void DebugOverlay::render(SDL_Renderer *renderer){
    if(!m_font || m_messages.empty()) return;
    int y=10;
    for(auto &msg : m_messages){
        Uint8 alpha = (msg.timeLeft<1000.0f)
            ? (Uint8)((msg.timeLeft/1000.0f)*255)
            : 255;
        SDL_Color color = {255, 255, 255, alpha};
        SDL_Surface *surface = TTF_RenderText_Solid(m_font, msg.text.c_str(), color);
        if(!surface) continue;
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_SetTextureAlphaMod(texture, alpha);
        int w,h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        SDL_Rect dst = {10, y, w, h};
        SDL_RenderCopy(renderer, texture, nullptr, &dst);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        y+=h+4;
    }
}

void DebugOverlay::hook(){
    m_oldCout=std::cout.rdbuf(this);
    m_oldCerr=std::cerr.rdbuf(this);
}