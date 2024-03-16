#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include "./src/algorithms/lz77.cpp"
#include "./src/algorithms/lz78.cpp"
#include "./src/algorithms/lzw.cpp"
#include "./src/algorithms/run_length_encoding.cpp"
#include "./src/lib/mem_monitor.hpp"
#include "./src/lib/tabulate.hpp"
#include "./src/profilers/profiler.cpp"
#include "./src/utils/setup_utils.cpp"
#include "./src/utils/tokenizer.cpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "USE: <executable_name> <filename1>.<extension> "
                     "<filename2>.<extension>..."
                  << std::endl;
        std::cerr << "NOTE: files must reside inside ./original directory"
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<std::string> test_files = {
        "test1.txt", "test2.txt", "test3.txt", "test4.txt", "test5.txt"};
    for (int i = 1; i < argc; i++) {
        test_files.push_back(argv[i]);
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

    mem_monitor mm("mem-mon-out.csv", std::chrono::milliseconds(1));

    mm.event("Algorithms Execution Started");
    for (Compressor* com : comps) {
        std::cout << com->algorithm_name << "\n";
        tabulate::Table table;
        table.add_row({"Filename", "Compression Time(Seconds)",
                       "Compression Ratio", "Space Saving(%)",
                       "Decompression Time(Seconds)"});
        for (std::string test_file : test_files) {
            std::vector<std::string> tokens =
                Tokenizer::getTokens(test_file, ".");

            std::cout << ">> Using " << com->algorithm_name << " Algorithm For "
                      << test_file << "\n";
            std::string input_file_path = "./original/" + test_file;
            std::string compressed_file_path = "./compressed/" + tokens[0] +
                                               "_compressed." +
                                               com->compressor_ext;
            std::string decompressed_file_path =
                "./decompressed/" + tokens[0] + "_decompressed." + tokens[1];

            mm.event("started " + com->algorithm_name + " compression");
            clock_t comp_start = clock();
            com->compress(input_file_path, compressed_file_path);
            clock_t comp_end = clock();

            double compression_ratio = Profiler::calculate_compression_ratio(
                input_file_path, compressed_file_path);

            // printf("Compression Time: %f [Seconds]\n", ((double)(comp_end -
            // comp_start) / CLOCKS_PER_SEC)); printf("Compression Ratio: %lf
            // \n", compression_ratio); printf("Space Saving: %.2lf%%\n",
            // Profiler::calculate_space_saving(compression_ratio));

            mm.event("started " + com->algorithm_name + " decompression");
            clock_t decomp_start = clock();
            com->decompress(compressed_file_path, decompressed_file_path);
            clock_t decomp_end = clock();

            // printf("Deompression Time: %f [Seconds]\n\n",
            //        ((double)(decomp_end - decomp_start) / CLOCKS_PER_SEC));

            table.add_row(
                tabulate::RowStream{}
                << test_file
                << ((double)(comp_end - comp_start) / CLOCKS_PER_SEC)
                << compression_ratio
                << Profiler::calculate_space_saving(compression_ratio)
                << ((double)(decomp_end - decomp_start) / CLOCKS_PER_SEC));
        }

        std::cout << table << std::endl;
    }
    mm.event("Algorithms Execution Finished");
}