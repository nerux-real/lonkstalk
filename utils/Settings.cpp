#include "Settings.h"
#include <fstream>
#include <string>
#include <iostream>

void Settings::load(const std::string &path) {
    bool changed = false;

    bool hasVolume = false;
    bool hasOffset = false;
    bool hasResWidth = false;
    bool hasResHeight = false;
    bool hasVideoMode = false;
    bool hasFpsLock = false;
    bool hasBgAlpha = false;
    bool hasSelectedSkin = false;
    bool hasDebug = false;

    std::ifstream fin(path);
    if(!fin.is_open()){
        std::cerr << "Could not open settings file: " << path << "\n";
        save(path);
        return;
    }

    std::string line;
    while(getline(fin, line)){
        size_t equal = line.find('=');
        if(equal == std::string::npos) continue;

        std::string key = line.substr(0, equal);
        std::string value = line.substr(equal + 1);

        if(key=="volume") { volume = std::stoi(value); hasVolume = true; }
        else if(key=="offset") { offset = std::stoi(value); hasOffset = true; }
        else if(key=="resWidth") { resWidth = std::stoi(value); hasResWidth = true; }
        else if(key=="resHeight") { resHeight = std::stoi(value); hasResHeight = true; }
        else if(key=="videoMode") { videoMode = value; hasVideoMode = true; }
        else if(key=="fpsLock") { fpsLock = std::stoi(value); hasFpsLock = true; }
        else if(key=="bgAlpha") { bgAlpha = std::stoi(value); hasBgAlpha = true; }
        else if(key=="selectedSkin") { selectedSkin = value; hasSelectedSkin = true; }
        else if(key=="debug") { debug = (value == "true"); hasDebug = true; }
    }

    if(!hasVolume) changed = true;
    if(!hasOffset) changed = true;
    if(!hasResWidth) changed = true;
    if(!hasResHeight) changed = true;
    if(!hasVideoMode) changed = true;
    if(!hasFpsLock) changed = true;
    if(!hasBgAlpha) changed = true;
    if(!hasSelectedSkin) changed = true;
    if(!hasDebug) changed = true;

    if(changed) {
        std::cout << "Settings file missing fields — rewriting with defaults\n";
        save(path);
    }
}


void Settings::save(const std::string &path) {
    std::ofstream fout(path);
    if(!fout.is_open()){
        std::cerr<<"Could not open settings file for writing: "<<path<<std::endl;
        return;
    }
    fout<<"volume="<<volume<<"\n";
    fout<<"offset="<<offset<<"\n";
    fout<<"resWidth="<<resWidth<<"\n";
    fout<<"resHeight="<<resHeight<<"\n";
    fout<<"videoMode="<<videoMode<<"\n";
    fout<<"fpsLock="<<fpsLock<<"\n";
    fout<<"bgAlpha="<<bgAlpha<<"\n";
    fout<<"selectedSkin="<<selectedSkin<<"\n";
    fout<<"debug="<<(debug ? "true" : "false")<<"\n";
}