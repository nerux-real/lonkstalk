#pragma once

#include <SDL2/SDL.h>
#include "Window.h"
#include "Beatmap.h"
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <unordered_map>
#include <random>
#include "DebugOverlay.h"
#include <filesystem>
#include <vector>
#include "ParticleSystem.h"
#include "Settings.h"

namespace fs = std::filesystem;

struct SongEntry {
    std::string lkPath;
    std::string name;
    std::string author;
    int bpm;
    std::vector<std::string> difficulties;
};

enum class GameState {
    MainMenu,
    SettingsMenu,
    SongSelectMenu,
    Countdown,
    Gameplay,
    Results
};

class Game {
public:
    bool init();
    void run();
    void shutdown();
    float getSongTimeMs();
    void handleInput(SDL_Keycode key, float songTime);
    void handleKeyUp(SDL_Keycode key, float songTime);

    void updateJudgmentTexture();
    void updateComboTexture();
    void updatePointsTexture();
    void renderLoadingScreen();

    void startGame(const std::string &path, const std::string &difficulty);
    void endMap();

    void updateSongSelectMenu(float deltaMs);
    void renderSongSelectMenu();
    void renderSongSelectLeft();
    void renderSongSelectRight();
    void handleSongSelectInput(SDL_Keycode key);
    void updateCountdown(float deltaMs);
    void renderCountdown();
    void updateGameplay(float deltaMs);
    void renderGameplay();
    void updateResults(float deltaMs);
    void renderResults();

    void scanBeatmaps();
private:
    DebugOverlay m_debug;

    bool m_loading=true;
    Window m_window;
    bool m_running=false;
    SDL_Event m_event;
    Uint64 m_lastTick = 0l;
    Beatmap m_beatmap;
    Mix_Music *m_music=nullptr;
    std::string m_judgmentText="";
    SDL_Color m_judgementColor={255,255,255,255};
    float m_judgmentTimer=0.0f;
    float m_visualTime=0.0f;
    float m_syncTimer=0.0f;

    TTF_Font *m_fontLarge=nullptr;
    TTF_Font *m_fontMedium=nullptr;
    TTF_Font *m_fontSmall=nullptr;
    TTF_Font *m_fontSmallest=nullptr;

    int m_combo=0;
    int m_points=0;
    float m_displayPoints=0;
    std::unordered_map<char, SDL_Texture*> m_letterTextures;

    SDL_Texture* m_judgmentTexture = nullptr;
    SDL_Texture* m_comboTexture = nullptr;
    SDL_Texture* m_scoreTexture = nullptr;
    SDL_Texture* m_bgTexture = nullptr;

    int m_judgmentW, m_judgmentH;
    int m_comboW, m_comboH;
    int m_scoreW, m_scoreH;
    bool m_changed = false;

    int m_minColor=0;
    int m_maxColor=255;
    std::mt19937 m_gen{std::random_device{}()};

    GameState m_state=GameState::SongSelectMenu;
    std::vector<SongEntry> m_songList;
    int m_selectedSong=0;
    int m_selectedDifficulty=0;
    ParticleSystem m_particles;

    float m_countdownTimer=0.0f;
    int m_countdownValue=3;

    Settings m_settings;
};