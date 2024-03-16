#include <sys/types.h>

#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

#include "./compressor.cpp"

class RunLengthEncodingCompressor : public Compressor {
    /**
     * @param[out] compressed_buffer Compressed buffer will be stored in a (count, value) format. Note that all the
     * contents previously stored will be cleared.
     */
    void compress_buffer(const uint8_t* buffer, const uint& buffer_size, std::vector<uint8_t>& compressed_buffer) {
        // we make the size of the compressed buffer zero, it does not deallocate the memory
        // hence it is faster than creating a new vector each time which does heap allocation
        compressed_buffer.clear();

        uint8_t repr = buffer[0];
        uint8_t count = 0;

        for (int idx = 0; idx < buffer_size; ++idx) {
            int curr = buffer[idx];
            if (curr == repr) {
                // if count is equal to repr, then there is a match
                // hence increase the count by one
                ++count;

                // count can at max store what can be stored inside an eight
                // bit container, i.e. 2^8 -1 that is UINT8_MAX
                //
                // now the count will overflow in the next iteration, hence we
                // need to store the contents inside the compressed buffer and
                // resets the counter to zero
                if (count == UINT8_MAX) {
                    compressed_buffer.insert(compressed_buffer.cend(), {count, repr});
                    count = 0;
                }
            } else {
                // there is a mismatch, hence we push the old value into the compressed buffer
                compressed_buffer.insert(compressed_buffer.cend(), {count, repr});
                // we update the representative value by the mismatched new value
                repr = curr;
                // we already have one instance of mismatched value, set count to one
                count = 1;
            }
        }

        // insert the last value and its count into the compressed_buffer
        compressed_buffer.insert(compressed_buffer.cend(), {count, repr});
    }

    void decompress_buffer(const uint8_t* buffer, const uint& buffer_size, std::vector<uint8_t>& decompressed_buffer) {
        decompressed_buffer.clear();

        for (int idx = 0; idx < buffer_size; idx += 2) {
            decompressed_buffer.insert(decompressed_buffer.cend(), buffer[idx], buffer[idx + 1]);
        }
    }

   public:
    void compress(const std::string input_file_path, const std::string output_file_path) {
        // open the input file to be read in binary mode
        std::ifstream is(input_file_path, std::ios::binary);

        // open the output file to be written in binary mode
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
        std::unique_ptr<uint8_t[]> buffer = std::make_unique<uint8_t[]>(BUFFER_SIZE);

        /// Stores number of bytes read from the input stream.
        std::streamsize bytes_read;

        /// Stores the compressed buffer in a count, value format.
        std::vector<uint8_t> compressed_buffer;

        // Used to output compressed buffer to the output file
        std::ostream_iterator<uint8_t> output_iterator = std::ostream_iterator<uint8_t>(os);

        std::cout << "* Run length Encoding: Starting File Compression" << std::endl;

        while (!is.eof()) {
            // read into buffer upto the buffer limit or eof whichever occurs first
            is.read(reinterpret_cast<char*>(buffer.get()), BUFFER_SIZE);

            bytes_read = is.gcount();

            compress_buffer(buffer.get(), bytes_read, compressed_buffer);

            // prints the compressed buffer inside output iterator
            std::copy(compressed_buffer.begin(), compressed_buffer.end(), output_iterator);
        }

        std::cout << "* Run length Encoding: File Compression Ended" << std::endl;

        is.close();
        os.close();
    }

    void decompress(const std::string input_file_path, const std::string output_file_path) {
        // note that output file if compressed using rle, will have even number of bytes
        // hence our buffer size must be a multiple of two in order for it to always take
        // the number of repeatitions and encoded character together

        // open the input file to be read in binary mode
        std::ifstream is(input_file_path, std::ios::binary);

        // open the output file to be written in binary mode
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

        std::ostream_iterator<char> output_iterator = std::ostream_iterator<char>(os);

        // Stores a chunk of data that is to be compressed.
        std::unique_ptr<uint8_t[]> buffer = std::make_unique<uint8_t[]>(BUFFER_SIZE);

        /// Stores number of bytes read from the input stream.
        std::streamsize bytes_read;

        /// Stores the compressed buffer in a count, value format.
        std::vector<uint8_t> decompressed_buffer;

        std::cout << "* Run length Encoding: Starting File Decompression" << std::endl;

        while (!is.eof()) {
            // read into buffer upto the buffer limit or eof whichever occurs first
            is.read(reinterpret_cast<char*>(buffer.get()), BUFFER_SIZE);

            bytes_read = is.gcount();

            decompress_buffer(buffer.get(), bytes_read, decompressed_buffer);

            // prints the compressed buffer inside output iterator
            std::copy(decompressed_buffer.begin(), decompressed_buffer.end(), output_iterator);
        }

        std::cout << "* Run length Encoding: File Decompression Ended" << std::endl;

        is.close();
        os.close();
    }

    RunLengthEncodingCompressor() : Compressor("RLE", "rle") {}
};