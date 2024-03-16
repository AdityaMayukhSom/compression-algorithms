#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "./compressor.cpp"

class LZWCompressor : public Compressor {
   public:
    void compress(const std::string input_file_path, const std::string output_file_path) {
        std::ifstream input_file(input_file_path, std::ios::binary);

        std::ofstream output_file(output_file_path, std::ios::binary);

        if (!input_file.is_open()) {
            std::cerr << "Error: Unable to open input file." << std::endl;
            return;
        }
        if (!output_file.is_open()) {
            std::cerr << "Error: Unable to open output file." << std::endl;
            return;
        }

        std::unordered_map<std::string, uint32_t> dictionary;

        // Initialize the dictionary with single-character strings
        for (int i = 0; i <= 255; ++i) {
            dictionary[std::string(1, static_cast<char>(i))] = i;
        }

        char input_char;
        int next_code = 256;
        std::string current, test;

        std::vector<uint32_t> compressed_data;

        // std::cout << "* Lempel Ziv Welsh Encoding: Starting File Compression" << std::endl;

        while (input_file.get(input_char)) {
            test = current + input_char;
            if (dictionary.find(test) != dictionary.end()) {
                // Input symbol found in dictionary, add it to current
                current.push_back(input_char);
            } else {
                // Output the code for current (if it exists)
                compressed_data.push_back(dictionary[current]);

                // Add input_symbol to the dictionary
                dictionary[test] = next_code++;
                current = std::string(1, input_char);
            }
        }

        // Output the code for the last current
        if (!current.empty()) {
            compressed_data.push_back(dictionary[current]);
        }

        // std::cout << "* Lempel Ziv Welsh Encoding: File ompression Ended" << std::endl;

        uint64_t size = compressed_data.size();

        output_file.write(reinterpret_cast<char*>(compressed_data.data()), size * sizeof(uint32_t));

        input_file.close();
        output_file.close();
    }

    void decompress(const std::string input_file_path, const std::string output_file_path) {
        std::ifstream input_file(input_file_path, std::ios::binary);
        std::ofstream output_file(output_file_path, std::ios::binary);

        if (!input_file.is_open()) {
            std::cerr << "Error: Unable to open input file." << std::endl;
            return;
        }

        if (!output_file.is_open()) {
            std::cerr << "Error: Unable to open output file." << std::endl;
            return;
        }

        std::unordered_map<uint32_t, std::string> dictionary;

        // generate the initial dictionary with the available words
        for (int i = 0; i < 256; ++i) {
            dictionary[i] = std::string(1, static_cast<char>(i));
        }

        // Stores the compressed data from the file, need to do buffering later
        std::vector<uint32_t> compressed_data;

        uint32_t code;
        while (input_file.read(reinterpret_cast<char*>(&code), sizeof(uint32_t))) {
            compressed_data.push_back(code);
        }

        uint32_t dict_size = 256;
        std::string characters = dictionary[compressed_data[0]];
        std::string result = characters;
        std::string entry;
        // std::cout << "* Lempel Ziv Welsh Encoding: Starting File Decompression" << std::endl;

        for (size_t i = 1; i < compressed_data.size(); ++i) {
            uint32_t code = compressed_data[i];
            if (dictionary.find(code) != dictionary.end()) {
                entry = dictionary[code];
            } else {
                entry = characters + characters[0];
            }
            result += entry;
            dictionary[dict_size++] = characters + entry[0];
            characters = entry;
        }
        // std::cout << "* Lempel Ziv Welsh Encoding: File Decompression Ended" << std::endl;
        output_file << result;

        input_file.close();
        output_file.close();
    }

    LZWCompressor() : Compressor("LZW", "lzw") {}
};
