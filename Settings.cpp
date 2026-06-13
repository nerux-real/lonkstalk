#include "Settings.h"
#include <fstream>
#include <string>
#include <iostream>

void Settings::load(const std::string &path) {
    std::ifstream fin(path);
    if(!fin.is_open()){
        std::cerr<<"Could not open settings file: "<<path<<std::endl;
        save(path);
        return;
    }
    std::string line;
    while(getline(fin, line)){
        size_t equal = line.find('=');
        if(equal == std::string::npos) continue;
        std::string key = line.substr(0, equal);
        std::string value = line.substr(equal+1);
        if(key=="volume") volume=std::stoi(value);
        else if(key=="offset") offset=std::stoi(value);
        else if(key=="resWidth") resWidth=std::stoi(value);
        else if(key=="resHeight") resHeight=std::stoi(value);
        else if(key=="fpsLock") fpsLock=std::stoi(value);
        else if(key=="bgAlpha") bgAlpha=std::stoi(value);
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
    fout<<"fpsLock="<<fpsLock<<"\n";
    fout<<"bgAlpha="<<bgAlpha<<"\n";
}