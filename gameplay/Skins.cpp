#include "Skins.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <SDL2/SDL_image.h>

namespace fs = std::filesystem;

void Skin::load(SDL_Renderer* renderer){

}

void Skin::unload(){
    if(noteTexture) SDL_DestroyTexture(noteTexture);
    if(l_fontGame) TTF_CloseFont(l_fontGame);
    if(m_fontGame) TTF_CloseFont(m_fontGame);
    if(s_fontGame) TTF_CloseFont(s_fontGame);
    if(l_fontUI) TTF_CloseFont(l_fontUI);
    if(m_fontUI) TTF_CloseFont(m_fontUI);
    if(s_fontUI) TTF_CloseFont(s_fontUI);

    noteTexture = nullptr;
    l_fontGame = m_fontGame = s_fontGame = nullptr;
    l_fontUI = m_fontUI = s_fontUI = nullptr;
}

void Skins::loadSkin(int index, SDL_Renderer* renderer){
    Skin& skin = m_skins[index];

    std::ifstream fin(skin.path + "/metadata.txt");
    if(!fin.is_open()){
        std::cerr << "Could not open skin metadata: " << skin.path << "\n";
        return;
    }
    std::string line;
    while(getline(fin, line)){
        size_t colon = line.find(':');
        if(colon == std::string::npos) continue;
        std::string key   = line.substr(0, colon);
        std::string value = line.substr(colon + 2);
        if(key == "name")   skin.name   = value;
        if(key == "author") skin.author = value;
    }
    fin.close();

    SDL_Surface* s = IMG_Load((skin.path + "/note.png").c_str());
    std::cout << "Loading: " << (skin.path + "/note.png") << "\n";
    if(s){
        std::cout << "Surface: " << s->w << "x" << s->h << "\n";
        skin.noteTexture = SDL_CreateTextureFromSurface(renderer, s);
        SDL_SetTextureBlendMode(skin.noteTexture, SDL_BLENDMODE_BLEND);

        Uint32 format;
        int access, w, h;
        SDL_QueryTexture(skin.noteTexture, &format, &access, &w, &h);

        SDL_FreeSurface(s);
    } else {
        std::cerr << "IMG_Load failed: " << IMG_GetError() << "\n";
    }

    skin.ll_fontGame = TTF_OpenFont((skin.path + "/gameinterface.ttf").c_str(), 64);
    skin.l_fontGame = TTF_OpenFont((skin.path + "/gameinterface.ttf").c_str(), 48);
    skin.m_fontGame = TTF_OpenFont((skin.path + "/gameinterface.ttf").c_str(), 32);
    skin.s_fontGame = TTF_OpenFont((skin.path + "/gameinterface.ttf").c_str(), 24);
    skin.l_fontUI   = TTF_OpenFont((skin.path + "/uiinterface.ttf").c_str(), 48);
    skin.m_fontUI   = TTF_OpenFont((skin.path + "/uiinterface.ttf").c_str(), 32);
    skin.s_fontUI   = TTF_OpenFont((skin.path + "/uiinterface.ttf").c_str(), 24);

    skin.hitsound1 = Mix_LoadWAV((skin.path + "/hitsound1.wav").c_str());

    if(!skin.l_fontGame) std::cerr << "Could not load gameinterface.ttf\n";
    if(!skin.l_fontUI)   std::cerr << "Could not load uiinterface.ttf\n";
    if(!skin.hitsound1)  std::cerr << "Could not load hitsound1.wav\n";

    m_activeSkin = index;
    std::cout << "Loaded skin: " << skin.name << "\n";
}

void Skins::scanSkins(const std::string &folder){
    m_skins.clear();
    if(!fs::exists(folder)){
        std::cerr << "Skins folder not found: " << folder << "\n";
        return;
    }
    for(const auto& entry : fs::directory_iterator(folder)){
        if(entry.is_directory()){
            Skin skin;
            skin.name = entry.path().filename().string();
            skin.path = entry.path().string();
            m_skins.push_back(skin);
        }
    }
}

Skin& Skins::getActive(){
    return m_skins[m_activeSkin];
}