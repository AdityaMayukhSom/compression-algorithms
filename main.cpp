#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "./src/algorithms/lz77.cpp"
#include "./src/algorithms/lzw.cpp"
#include "./src/algorithms/run_length_encoding.cpp"
#include "./src/utils/setup_utils.cpp"
#include "./src/utils/tokenizer.cpp"

int main(int argc, char* argv[]) {
    std::string default_test_file_name = "test.txt";
    if (argc == 2) {
        default_test_file_name = argv[1];
    }

    std::vector<std::string> tokens = Tokenizer::getTokens(default_test_file_name, ".");

    if (tokens.size() != 2) {
        std::cerr << "USE: <executable_name> <filename>.<extension>" << std::endl;
        exit(EXIT_FAILURE);
    }

    SetupUtils::createDirectory("original");
    SetupUtils::createDirectory("compressed");
    SetupUtils::createDirectory("decompressed");

    std::string input_file_path = "./original/" + default_test_file_name;
    std::string compressed_file_path = "./compressed/" + tokens[0] + "_compressed.dev";
    std::string decompressed_file_path = "./decompressed/" + tokens[0] + "_decompressed." + tokens[1];

    Compressor* rlec = new RunLengthEncodingCompressor();
    Compressor* lzwc = new LZWCompressor();
    Compressor* lz77c = new LZ77Compressor();

    Compressor* com = lz77c;

    com->compress(input_file_path, compressed_file_path);
    com->decompress(compressed_file_path, decompressed_file_path);
}