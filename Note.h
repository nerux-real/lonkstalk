#pragma once
#include <SDL2/SDL.h>

enum class NoteType {
    Tap,
    Slider
};

enum class NoteState {
    Waiting,
    Active,
    Holding,
    Hit,
    Missed
};

struct Note {
    NoteType type = NoteType::Tap;
    float timestampMs;
    char key;
    int gridCol;
    int gridRow;
    float endTimestampMs;
    int endGridCol;
    int endGridRow;
    NoteState state = NoteState::Waiting;
    float fadeTimer=0.0f;
    float nextTickTime=0.0f;

    Uint8 r, g, b;
    bool hasColor=false;
};