#pragma once

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

struct DebugMessage {
    std::string text;
    float timeLeft=5000.0f;
};

class DebugOverlay : public std::streambuf {
public:
    void init(TTF_Font *font, SDL_Renderer *renderer);
    void update(float deltaMs);
    void render(SDL_Renderer *renderer);
    void hook();
    void unHook();
protected:
    int overflow(int c) override;
private:
    std::string m_line;
    std::vector<DebugMessage> m_messages;
    TTF_Font *m_font=nullptr;
    SDL_Renderer *m_renderer=nullptr;

    std::streambuf *m_oldCout=nullptr;
    std::streambuf *m_oldCerr=nullptr;
};