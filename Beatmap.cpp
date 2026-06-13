#include "Beatmap.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "miniz.h"

void Beatmap::load(const char *folder){
    std::string path=std::string(folder)+"/metadata.txt";
    std::ifstream fin(path);
    if(!fin.is_open()){
        std::cerr<<"Could not open "<<path<<std::endl;
        return;
    }

    std::string line;
    while(getline(fin, line)){
        size_t colon = line.find(':');
        if(colon==std::string::npos) continue;
        std::string key = line.substr(0, colon);
        std::string value = line.substr(colon+2);

        if(key=="name") name=value;
        else if(key=="author") author=value;
        else if(key=="song") song_path=std::string(folder)+"/"+value;
        else if(key=="song_original_name") song_original_name=value;
        else if(key=="bpm") bpm=std::stoi(value);
    }
    fin.close();

    path=std::string(folder)+"/map.txt";
    fin.open(path);
    if(!fin.is_open()){
        std::cerr<<"Could not open "<<path<<std::endl;
        return;
    }
    while(getline(fin, line)){
        if(line.empty()) continue;
        std::istringstream iss(line);
        Note note;
        iss>>note.timestampMs>>note.key>>note.gridCol>>note.gridRow;
        notes.push_back(note);
    }
    std::cout << "Loaded: " << name << " by " << author << " BPM:" << bpm << "\n";
    std::cout << "Notes: " << notes.size() << "\n";
}

void Beatmap::loadFromLk(const char *lkPath, const std::string &difficulty){
    mz_zip_archive zip = {};
    if(!mz_zip_reader_init_file(&zip, lkPath, 0)){
        std::cerr<<"Failed to open: "<<lkPath<<std::endl;
        return;
    }

    size_t size=0;
    std::string filename = "map_"+difficulty+".txt";
    void *data = mz_zip_reader_extract_file_to_heap(&zip, filename.c_str(), &size, 0);
    if(!data){
        std::cerr<<"CORRUPTED .LK BEATMAP FILE IN: "<<lkPath<<std::endl;
        mz_zip_reader_end(&zip);
        return;
    }

    //song extraction
    size_t songSize = 0;
    void* songData = mz_zip_reader_extract_file_to_heap(&zip, song_filename.c_str(), &songSize, 0);
    if(!songData){
        std::cerr << "No song file in " << lkPath << "\n";
        mz_zip_reader_end(&zip);
        return;
    }
    song_path="/tmp/beatmap.mp3";
    FILE *f=fopen(song_path.c_str(), "wb");
    fwrite(songData, 1, songSize, f);
    fclose(f);
    mz_free(songData);
    ///////

    ///bg extraction (if exists)
    size_t bgSize=0;
    void* bgData = mz_zip_reader_extract_file_to_heap(&zip, bg_filename.c_str(), &bgSize, 0);
    if(bgData){
        std::string bgPath="/tmp/beatmap_bg.png";
        FILE *bgf=fopen(bgPath.c_str(), "wb");
        fwrite(bgData, 1, bgSize, bgf);
        fclose(bgf);
        mz_free(bgData);
        bg_path=bgPath;
    }
    ////

    std::string content((char*)data, size);
    mz_free(data);
    mz_zip_reader_end(&zip);

    std::istringstream ss(content);
    std::string line;
    while(getline(ss, line)){
        if(line.empty()) continue;
        
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        while(iss >> token) tokens.push_back(token);
        
        Note note;
        if(tokens.size() == 4){
            note.type = NoteType::Tap;
            note.timestampMs = std::stof(tokens[0]);
            note.key = tokens[1][0];
            note.gridCol = std::stoi(tokens[2]);
            note.gridRow = std::stoi(tokens[3]);
        } else if(tokens.size() == 7){
            note.type = NoteType::Slider;
            note.timestampMs = std::stof(tokens[0]);
            note.endTimestampMs = std::stof(tokens[1]);
            note.key = tokens[2][0];
            note.gridCol = std::stoi(tokens[3]);
            note.gridRow = std::stoi(tokens[4]);
            note.endGridCol = std::stoi(tokens[5]);
            note.endGridRow = std::stoi(tokens[6]);
        } else {
            std::cerr << "Unknown note format: " << line << "\n";
            continue;
        }
        notes.push_back(note);
    }
    std::cout << "Loaded: " << name << " by " << author << " BPM:" << bpm << "\n";
    std::cout << "Notes: " << notes.size() << "\n";
}

void Beatmap::loadMetaFromLk(const char *lkPath){
    mz_zip_archive zip = {};
    if(!mz_zip_reader_init_file(&zip, lkPath, 0)){
        std::cerr<<"Failed to open: "<<lkPath<<std::endl;
        return;
    }

    size_t size=0;
    void *data = mz_zip_reader_extract_file_to_heap(&zip, "metadata.txt", &size, 0);
    if(!data){
        std::cerr<<"CORRUPTED .LK BEATMAP FILE IN: "<<lkPath<<std::endl;
        mz_zip_reader_end(&zip);
        return;
    }

    std::string content((char*)data, size);
    mz_free(data);
    mz_zip_reader_end(&zip);

    std::istringstream ss(content);
    std::string line;
    while(getline(ss, line)){
        size_t colon = line.find(':');
        if(colon==std::string::npos) continue;
        std::string key = line.substr(0, colon);
        std::string value = line.substr(colon+2);

        if(key=="name") name=value;
        else if(key=="author") author=value;
        else if(key=="song") song_filename=value;
        else if(key=="bg") bg_filename=value;
        else if(key=="song_original_name") song_original_name=value;
        else if(key=="bpm") bpm=std::stoi(value);
        else if(key=="difficulties"){
            std::stringstream ds(value);
            std::string diff;
            while(getline(ds, diff, ',')) difficulties.push_back(diff);
        }
    }
}

void Beatmap::unloadMap(){
    song_original_name.clear();
    song_filename.clear();
    name.clear();
    author.clear();
    notes.clear();
    song_path.clear();
    difficulties.clear();
    bg_filename.clear();
    bg_path.clear();
}