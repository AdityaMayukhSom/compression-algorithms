#pragma once
#include <cstdint>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "./compressor.cpp"

class LZ77Compressor : public Compressor {
   private:
    int look_ahead_buffer = 4096;
    int search_buffer = 512;

    typedef struct Triplet {
       public:
        char codeword;
        uint16_t offset;
        uint16_t length_of_match;
    } __attribute__((packed)) Triplet;

    void compress_buffer(std::string input, std::vector<Triplet>& data, int& triplet_length) {
        data.clear();
        triplet_length = 0;

        int inputLength = input.length();
        int position = 0;
        while (position < inputLength) {
            Triplet triplet;
            triplet.offset = 0;
            triplet.length_of_match = 0;
            triplet.codeword = input[position];

            // Calculate the maximum offset based on the search buffer
            int max_offset = (position - search_buffer) < 0 ? position : search_buffer;

            // Calculate the maximum search length based on the look-ahead buffer
            int max_search_length =
                (position + look_ahead_buffer) > inputLength ? inputLength - position : look_ahead_buffer;

            // Search for the longest match in the search buffer
            for (int offset = 1; offset <= max_offset; offset++) {
                int len = 0;
                while (len < max_search_length && input[position - offset + len] == input[position + len]) {
                    len++;
                }

                // Update the  Triplet if a longer match is found
                if (len > triplet.length_of_match) {
                    triplet.offset = offset;
                    triplet.length_of_match = len;
                    triplet.codeword = input[position + len];
                }
            }
            data.push_back(triplet);

            triplet_length++;
            position += triplet.length_of_match + 1;
        }
    }

    std::string decompress_buffer(std::vector<Triplet>& data, int& triplets_length) {
        std::string tmp = "";
        int pos = 0;
        for (int i = 0; i < triplets_length; i++) {
            if (data[i].offset != 0) {
                int start = pos - data[i].offset;
                int len = data[i].length_of_match;
                // Copy the matched substring
                while (len > 0) {
                    tmp += tmp[start];
                    start++;
                    len--;
                    pos++;
                }
            }
            if (data[i].codeword != '\0') {
                tmp += data[i].codeword;
            };
            pos++;
        }
        return tmp;
    }

   public:
    void compress(const std::string input_file_path, const std::string output_file_path) {
        std::ifstream is(input_file_path, std::ios::binary);

        std::ofstream os(output_file_path, std::ios::binary);

        if (!is.is_open()) {
            std::cerr << "Error: Unable to open input file." << std::endl;
            return;
        }
        if (!os.is_open()) {
            std::cerr << "Error: Unable to open output file." << std::endl;
            return;
        }

        // move the cursor to the beginning of the file
        is.seekg(0, std::ios::beg);
        os.seekp(0, std::ios::beg);

        // Stores a chunk of data that is to be compressed.
        std::unique_ptr<char[]> buffer = std::make_unique<char[]>(BUFFER_SIZE);

        uint8_t zero = 0, one = 1;

        std::vector<Triplet> triplet_data;
        int triplet_length;
        // std::cout << "* Lempel Ziv Welsh Encoding: Starting File Compression" << std::endl;
        while (!is.eof()) {
            std::memset(buffer.get(), 0, BUFFER_SIZE);
            // read into buffer upto the buffer limit or eof whichever occurs first
            is.read(reinterpret_cast<char*>(buffer.get()), BUFFER_SIZE);
            compress_buffer(buffer.get(), triplet_data, triplet_length);

            int bytes_to_read = 0;
            os.write(reinterpret_cast<const char*>(&bytes_to_read), sizeof(bytes_to_read));

            for (int i = 0; i < triplet_length; i++) {
                if (triplet_data[i].length_of_match == 0 && triplet_data[i].offset == 0) {
                    os.write(reinterpret_cast<const char*>(&zero), sizeof(zero));
                    os.write(reinterpret_cast<const char*>(&triplet_data[i].codeword),
                             sizeof(triplet_data[i].codeword));
                    bytes_to_read += sizeof(Triplet::codeword) + 1;
                } else {
                    os.write(reinterpret_cast<const char*>(&one), sizeof(one));
                    os.write(reinterpret_cast<const char*>(&triplet_data[i].offset), sizeof(triplet_data[i].offset));
                    os.write(reinterpret_cast<const char*>(&triplet_data[i].length_of_match),
                             sizeof(triplet_data[i].length_of_match));
                    os.write(reinterpret_cast<const char*>(&triplet_data[i].codeword),
                             sizeof(triplet_data[i].codeword));
                    bytes_to_read += sizeof(Triplet) + 1;
                }
            }
            long pos = os.tellp();
            os.seekp(pos - bytes_to_read - sizeof(bytes_to_read));
            os.write(reinterpret_cast<const char*>(&bytes_to_read), sizeof(bytes_to_read));
            os.seekp(0, std::ios::end);
            pos = os.tellp();
        }
        // std::cout << "* Lempel Ziv Welsh Encoding: File Compression Ended" << std::endl;

        is.close();
        os.close();
    }

    void decompress(const std::string input_file_path, const std::string output_file_path) {
        std::ifstream is(input_file_path, std::ios::binary);
        std::ofstream os(output_file_path, std::ios::binary);

        if (!is.is_open()) {
            std::cerr << "Error: Unable to open input file." << std::endl;
            return;
        }

        if (!os.is_open()) {
            std::cerr << "Error: Unable to open output file." << std::endl;
            return;
        }

        // std::cout << "* LZ77 Encoding: Starting File Decompression" << std::endl;
        // move the cursor to the beginning of the file
        is.seekg(0, std::ios::beg);
        os.seekp(0, std::ios::beg);

        // Stores a chunk of data that is to be compressed.

        std::vector<Triplet> triplet_data;
        int triplet_length;

        int bytes_to_read;

        while (!is.eof()) {
            // is >> bytes_to_read;
            // read into buffer upto the buffer limit or eof whichever occurs first
            is.read(reinterpret_cast<char*>(&bytes_to_read), sizeof(bytes_to_read));
            if (is.eof()) {
                break;
            }
            std::unique_ptr<char[]> buffer = std::make_unique<char[]>(bytes_to_read);
            is.read(reinterpret_cast<char*>(buffer.get()), bytes_to_read);

            triplet_data.clear();
            triplet_length = 0;
            uint8_t is_triplet;
            for (int i = 0; i < bytes_to_read;) {
                triplet_length++;
                is_triplet = buffer[i++];
                Triplet triplet;
                if (is_triplet == 0) {
                    triplet.offset = 0;
                    triplet.length_of_match = 0;
                    triplet.codeword = buffer[i];
                    i += sizeof(Triplet::codeword);
                } else {
                    triplet.offset = buffer[i];
                    i += sizeof(Triplet::offset);
                    triplet.length_of_match = buffer[i];
                    i += sizeof(Triplet::length_of_match);
                    triplet.codeword = buffer[i];
                    i += sizeof(Triplet::codeword);
                }
                triplet_data.push_back(triplet);
            }
            std::string res = decompress_buffer(triplet_data, triplet_length);
            os << res;

            // for (Triplet t : triplet_data) {
            //     std::cout << t.offset << " " << t.length_of_match << " " << t.codeword << std::endl;
            // }
        }

        // std::cout << "* LZ77 Encoding: File Decompression Ended" << std::endl;

        is.close();
        os.close();
    }

    LZ77Compressor() : Compressor("LZ77", "lz77") {}
};