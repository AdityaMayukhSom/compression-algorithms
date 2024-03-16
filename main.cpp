#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include "./src/algorithms/lz77.cpp"
#include "./src/algorithms/lz78.cpp"
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

    std::vector<Compressor*> comps = {
        new RunLengthEncodingCompressor(),  //
        new LZWCompressor(),                //
        new LZ77Compressor(),               //
        new LZ78Compressor()                //
    };

    for (Compressor* com : comps) {
        std::cout << ">> Using " << com->algorithm_name << " Algorithm"
                  << "\n\n";
        std::string input_file_path = "./original/" + default_test_file_name;
        std::string compressed_file_path = "./compressed/" + tokens[0] + "_compressed." + com->compressor_ext;
        std::string decompressed_file_path = "./decompressed/" + tokens[0] + "_decompressed." + tokens[1];

        clock_t comp_start = clock();
        com->compress(input_file_path, compressed_file_path);
        clock_t comp_end = clock();

        printf("Compression Time: %f [Seconds]\n\n", ((double)(comp_end - comp_start) / CLOCKS_PER_SEC));

        clock_t decomp_start = clock();
        com->decompress(compressed_file_path, decompressed_file_path);
        clock_t decomp_end = clock();

        printf("Deompression Time: %f [Seconds]\n\n", ((double)(decomp_end - decomp_start) / CLOCKS_PER_SEC));
    }
}