#pragma once

#include <SDL2/SDL.h>
#include "Window.h"
#include "../gameplay/Beatmap.h"
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <unordered_map>
#include <random>
#include "../utils/DebugOverlay.h"
#include <filesystem>
#include <vector>
#include "../gameplay/ParticleSystem.h"
#include "../utils/Settings.h"
#include "../gameplay/Skins.h"
// #include "../utils/DiscordRPC.h"
#include "../utils/Hash.h"
#include "../utils/Database.h"

namespace fs = std::filesystem;

struct Layout {
    int cellSize;
    int cols;
    int rows;
    int gridOffsetX;
    int gridOffsetY;
    int panelSplit;
};

struct SongEntry {
    std::string lkPath;
    std::string name;
    std::string author;
    std::string bgPath;
    int bpm;
    std::vector<std::string> difficulties;
};

struct HitError {
    float offset;
    float lifetime;
    bool excellent;
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
    void computeLayout();
    void run();
    void shutdown();
    float getSongTimeMs();
    void handleInput(SDL_Keycode key, float songTime);
    void handleKeyUp(SDL_Keycode key, float songTime);

    void updateJudgmentTexture();
    void updateComboTexture();
    void updatePointsTexture();
    void renderLoadingScreen();
    void updateAccuracyTexture();
    float getAccuracy();
    void renderHPBar();
    void renderHitErrorBar();
    void renderComboMilestone();
    void renderHpPulseEffect();
    void renderTimerDisplay();
    void renderFPSCounter();
    void renderCanSkipIndicator(float songTime);

    void backToSongSelect();

    void toggleDebug();

    std::string getGrade();

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
    Layout m_layout;

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

    float m_previewTimer=0.0f;
    float m_previewFade=0.0f;
    float m_previewPlayTime = 0.0f;
    bool m_previewPlaying=false;
    int m_lastSelectedSong=-1;
    Mix_Music* m_previewMusic=nullptr;
    int m_combo=0;
    int m_maxCombo=0;
    int m_points=0;
    float m_displayPoints=0;
    float m_milestoneTimer=0.0f;
    int m_milestoneCombo=0;
    float m_hpPulseTimer=0.0f;
    float m_mapDuration=0.0f;
    float m_countdownTotalTime=0.0f;
    std::unordered_map<char, SDL_Texture*> m_letterTextures;

    float m_fpsTimer=0.0f;
    int m_fps=0;
    int m_fpsCount=0;
    float m_inputLatency=0.0f;

    SDL_Texture* m_judgmentTexture = nullptr;
    SDL_Texture* m_comboTexture = nullptr;
    SDL_Texture* m_scoreTexture = nullptr;
    SDL_Texture* m_bgTexture = nullptr;
    SDL_Texture* m_accuracyTexture = nullptr;

    int m_judgmentW, m_judgmentH;
    int m_comboW, m_comboH;
    int m_scoreW, m_scoreH;
    int m_accuracyW, m_accuracyH;
    int m_totalNotes=0;
    int m_excellentHits=0;
    int m_goodHits=0;
    int m_misses=0;
    float m_hp=100.0f;
    float m_maxHp=100.0f;
    bool m_changed = false;
    bool m_failed=false;

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
    Skins m_skins;

    std::vector<HitError> m_hitErrors;
    //DiscordRPC m_discordRPC;

    SDL_Texture* m_menuBgTexture = nullptr;
    int m_lastBgSong = -1;

    bool m_canSkip=false;
    float m_skipTargetTime=0.0f;

    Database m_database;
    bool m_saveScore=true;

    SDL_Color m_bgDominantColor={255,255,255,255};
    SDL_Color m_bgDominantColorInverted={0,0,0,255};

    //gamemodes
    bool m_noFail=false;
};