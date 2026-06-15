#include "Hash.h"
#include "picosha2.h"
#include <fstream>

std::string sha256File(const std::string& path){
    std::ifstream file(path, std::ios::binary);
    if(!file) return "";

    std::vector<unsigned char> hash(picosha2::k_digest_size);
    picosha2::hash256(file, hash.begin(), hash.end());

    return picosha2::bytes_to_hex_string(hash.begin(), hash.end());
}