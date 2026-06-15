#include <iostream>
#include "../../deps/miniz.h"
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

int main(){
    std::string current_path = fs::current_path().string();
    std::string path, extractpath=current_path+"/beatmaps/";
    if(!fs::exists(extractpath)){
        fs::create_directory(extractpath);
    }
    std::cout<<"Enter beatmap full folder path (ex. /home/nerux/firstbeatmap): ";
    getline(std::cin, path);
    
    std::string name=path.substr(path.find_last_of("/\\")+1)+".lk";
    extractpath+=name;

    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));

    if(!mz_zip_writer_init_file(&zip_archive, extractpath.c_str(), 0)){
        std::cout << "Failed to init lk writer" << std::endl;
        return 1;
    }

    for(auto &entry : fs::directory_iterator(path)){
        std::string filename=entry.path().filename().string();
        std::string fullpath=entry.path().string();
        mz_zip_writer_add_file(&zip_archive, filename.c_str(), fullpath.c_str(), nullptr, 0, MZ_BEST_COMPRESSION);
    }

    if(!mz_zip_writer_finalize_archive(&zip_archive)){
        std::cout<<"Failed to finalize lk file!"<<std::endl;
    }

    mz_zip_writer_end(&zip_archive);
    std::cout<<"Beatmap saved in: "<<extractpath<<std::endl;
    return 0;
}