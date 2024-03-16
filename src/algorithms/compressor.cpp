#pragma once

#include <string>

#include "../config/config.hpp"

class Compressor {
   public:
    std::string algorithm_name;
    std::string compressor_ext;
    virtual void compress(const std::string input_file_path, const std::string output_file_path) = 0;
    virtual void decompress(const std::string input_file_path, const std::string output_file_path) = 0;

    Compressor(const std::string name, const std::string ext) {
        algorithm_name = name;
        compressor_ext = ext;
    }
};
