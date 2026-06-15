#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <SDL2/SDL_mixer.h>

struct Skin {
    std::string name;
    std::string author;
    std::string path;

    SDL_Texture* noteTexture  = nullptr;
    TTF_Font* ll_fontGame = nullptr;
    TTF_Font* l_fontGame = nullptr;
    TTF_Font* m_fontGame = nullptr;
    TTF_Font* s_fontGame = nullptr;
    TTF_Font* l_fontUI = nullptr;
    TTF_Font* m_fontUI = nullptr;
    TTF_Font* s_fontUI = nullptr;
    Mix_Chunk* hitsound1 = nullptr;

    void load(SDL_Renderer* renderer);
    void unload();
};

class Skins {
public:
    void scanSkins(const std::string& folder);
    void loadSkin(int index, SDL_Renderer* renderer);
    Skin& getActive();

    std::vector<Skin> m_skins;
    int m_activeSkin = 0;
};