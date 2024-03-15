#pragma once

#include <string>

#include "../config/config.hpp"

class Compressor {
   public:
    virtual void compress(const std::string input_file_path, const std::string output_file_path) = 0;
    virtual void decompress(const std::string input_file_path, const std::string output_file_path) = 0;
};
