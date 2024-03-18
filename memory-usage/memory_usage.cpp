#include <sys/resource.h>
#include <sys/stat.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>

#include "../src/algorithms/compressor.cpp"
#include "../src/algorithms/lz77.cpp"
#include "../src/algorithms/lz78.cpp"
#include "../src/algorithms/lzw.cpp"
#include "../src/algorithms/run_length_encoding.cpp"
#include "./lib/cmdparser.cpp"

void configure_parser(cli::Parser& parser) {
    parser.set_required<std::string>("algo", "algorithm", "algorithm to be used for compression or decompression");
    parser.set_required<std::string>("i", "input", "path to the file that has to be compressed");
    parser.set_optional<std::string>("o", "output", "", "path where the output file is stored");
    parser.set_optional<std::string>("m", "mode", "compress",
                                     "whether to run in compression mode or decompression mode");
}

void generate_input_output_path(std::string& input_file_path, std::string& output_file_path, cli::Parser& parser) {
    std::string algo = parser.get<std::string>("algo");
    input_file_path = parser.get<std::string>("i");

    std::filesystem::path ip(input_file_path);
    std::string output_file_name = ip.filename().replace_extension().replace_extension(algo).c_str();

    std::filesystem::path op = ip.replace_filename(ip.filename().replace_extension(algo));

    if (parser.doesArgumentExist("o", "output")) {
        output_file_path = parser.get<std::string>("o");
        op = std::filesystem::path(output_file_path);

        struct stat s;
        if (stat(op.c_str(), &s) == 0) {
            if (s.st_mode & S_IFDIR) {
                op.concat("/");
            } else if (s.st_mode & S_IFREG) {
                // it's a file
            }
        }

        if (!op.has_filename()) {
            op.replace_filename(output_file_name);
        }
        output_file_path = op.c_str();
    } else {
        output_file_path = ip.replace_filename(output_file_name).c_str();
    }
}

int main(int argc, char* argv[]) {
    cli::Parser parser(argc, argv);
    configure_parser(parser);
    parser.run_and_exit_if_error();
    struct rusage base_usage, usage, destroyed_usage;

    std::string input_file_path, output_file_path;
    std::string mode = parser.get<std::string>("m");
    std::string algo = parser.get<std::string>("algo");
    generate_input_output_path(input_file_path, output_file_path, parser);

    Compressor* comp;
    getrusage(RUSAGE_SELF, &base_usage);

    if (strcmp(algo.c_str(), "rle") == 0) {
        comp = new RunLengthEncodingCompressor();
    } else if (strcmp(algo.c_str(), "lzw") == 0) {
        comp = new LZWCompressor();
    } else if (strcmp(algo.c_str(), "lz77") == 0) {
        comp = new LZ77Compressor();
    } else if (strcmp(algo.c_str(), "lz78") == 0) {
        comp = new LZ78Compressor();
    } else {
        printf("given algorithm not supported\n");
        printf("supported algorithms are [rle|lzw|lz77|lz78]\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(mode.c_str(), "compress") == 0) {
        comp->compress(input_file_path, output_file_path);
    } else if (strcmp(mode.c_str(), "decompress") == 0) {
        comp->decompress(input_file_path, output_file_path);
    } else {
        printf("mode not supported\n");
        printf("supported modes are [compress|decompress]\n");
        exit(EXIT_FAILURE);
    }

    getrusage(RUSAGE_SELF, &usage);
    // std::cout << "rss before usage " << base_usage.ru_maxrss << " KB\n";
    // std::cout << "rss after usage " << usage.ru_maxrss << " KB\n";
    std::cout << "Algorithm Name: " << comp->algorithm_name
              << "\tMemory: " << (usage.ru_maxrss - base_usage.ru_maxrss) / (double)1024 << " MB\n";

    return 0;
}