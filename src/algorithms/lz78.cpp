#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "./compressor.cpp"

class LZ78Compressor : public Compressor {
   public:
    void compress(const std::string input_file_path, const std::string output_file_path) {
        std::ifstream input_file(input_file_path, std::ios::binary);
        if (!input_file.is_open()) {
            std::cerr << "LZ78 error: unable to open compression input file " << input_file_path << "\n";
            return;
        }

        std::ofstream output_file(output_file_path, std::ios::binary);
        if (!output_file.is_open()) {
            std::cerr << "LZ78 error: unable to open compression output file " << output_file_path << "\n";
            return;
        }

        std::unordered_map<std::string, uint32_t> dictionary;

        // Initialize the dictionary with an empty string
        dictionary[""] = 0;

        std::string current;
        char input_char;

        // std::cout << "* LZ78 Encoding: Starting File Compression" << std::endl;

        while (input_file.get(input_char)) {
            current.push_back(input_char);
            if (dictionary.find(current) == dictionary.end()) {
                // Output the index of the prefix of current in the dictionary
                output_file.write(reinterpret_cast<char*>(&dictionary[current.substr(0, current.size() - 1)]),
                                  sizeof(uint32_t));

                // Add current to the dictionary
                dictionary[current] = dictionary.size();

                // Output the character following the prefix
                output_file.put(input_char);

                // Reset current
                current.clear();
            }
        }

        // Output the index of the last prefix and character
        if (!current.empty()) {
            output_file.write(reinterpret_cast<char*>(&dictionary[current.substr(0, current.size() - 1)]),
                              sizeof(uint32_t));
            output_file.put(current.back());
        }

        // std::cout << "* LZ78 Encoding: File Compression Ended" << std::endl;

        input_file.close();
        output_file.close();
    }

    void decompress(const std::string input_file_path, const std::string output_file_path) {
        std::ifstream input_file(input_file_path, std::ios::binary);
        if (!input_file.is_open()) {
            std::cerr << "LZ78 error: unable to open decompression input file " << input_file_path << "\n";
            return;
        }

        std::ofstream output_file(output_file_path, std::ios::binary);
        if (!output_file.is_open()) {
            std::cerr << "LZ78 error: unable to open decompression output file " << output_file_path << "\n";
            return;
        }

        std::vector<std::string> dictionary;
        dictionary.push_back("");  // Initialize the dictionary with an empty string at index 0

        uint32_t index;
        char next_char;
        std::string current_str;

        // std::cout << "* LZ78 Decoding: Starting File Decompression" << std::endl;

        while (input_file.read(reinterpret_cast<char*>(&index), sizeof(uint32_t)) && input_file.get(next_char)) {
            if (index == dictionary.size()) {
                // If the index equals the current dictionary size, it means
                // we're adding a new entry
                current_str = dictionary[index - 1] + next_char;
                output_file << current_str;
                dictionary.push_back(current_str);  // Add the new entry to the dictionary
            } else {
                // If the index is in the dictionary, output the corresponding
                // string
                output_file << dictionary[index] << next_char;
                current_str = dictionary[index] + next_char;
                dictionary.push_back(current_str);  // Add the new entry to the dictionary
            }
        }

        // std::cout << "* LZ78 Decoding: File Decompression Ended" << std::endl;

        input_file.close();
        output_file.close();
    }

    LZ78Compressor() : Compressor("LZ78", "lz78") {}
};