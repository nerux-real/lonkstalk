#pragma once
#include <string>

struct Settings {
    int volume=50;
    int offset=0;
    int resWidth=1920;
    int resHeight=1080;
    int fpsLock=0;
    int bgAlpha=64;

    void load(const std::string &path);
    void save(const std::string &path);
};