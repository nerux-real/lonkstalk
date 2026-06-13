#pragma once

#include <string>
#include <vector>
#include "Note.h"
#include <map>

class Beatmap {
public:
    std::string name;
    std::string author;
    std::string song_filename;
    std::string bg_filename;
    std::string bg_path;
    std::string song_path;
    std::string song_original_name;
    int bpm;
    std::vector<Note> notes;
    std::vector<std::string> difficulties;
    void load(const char *folder); //legacy loader from folder
    void loadFromLk(const char *lkPath, const std::string &difficulty);
    void loadMetaFromLk(const char *lkPath);
    void unloadMap();
};