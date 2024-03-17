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
#include "./src/lib/variadic_table.hpp"
#include "./src/profilers/profiler.cpp"
#include "./src/utils/file_size_util.cpp"
#include "./src/utils/setup_utils.cpp"
#include "./src/utils/tokenizer.cpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "USE: <executable_name> <filename1>.<extension> <filename2>.<extension>..." << std::endl;
        std::cerr << "NOTE: files must reside inside ./original directory" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<std::string> test_files;
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

    VariadicTable<std::string, double, double, double> average(
        {"Algorithm Name", "Compression Ratio", "Compression Speed (Mbps)", "Decompression Speed (Mbps)"});

    mm.event("Algorithms Execution Started");
    for (Compressor* com : comps) {
        double avg_comp_ratio = 0, avg_comp_speed = 0, avg_decomp_speed = 0;
        std::cout << com->algorithm_name << "\n";
        VariadicTable<std::string, double, double, double, double, double, double> table(
            {"File", "Compression Time (Sec)", "Compression Ratio", "Compression Speed (Mbps)", "Space Saving (%)",
             "Decompression Time (Sec)", "Decompression Speed (Mbps)"});

        int files_tested_on = 0;
        for (std::string test_file : test_files) {
            std::vector<std::string> tokens = Tokenizer::getTokens(test_file, ".");

            // std::cout << ">> executing " << com->algorithm_name << " algorithm on " << test_file << "\n";

            std::string input_file_path = "./original/" + test_file;
            std::string compressed_file_path = "./compressed/" + tokens[0] + "_compressed." + com->compressor_ext;
            std::string decompressed_file_path = "./decompressed/" + tokens[0] + "_decompressed." + tokens[1];

            if (!SetupUtils::exists(input_file_path)) {
                std::cerr << "no such file exists: [" << input_file_path << "]\n";
                std::cerr << "cannot compress non existent file, ignoring\n";
                continue;
            }

            mm.event("started " + com->algorithm_name + " compression");

            clock_t comp_start = clock();
            com->compress(input_file_path, compressed_file_path);
            clock_t comp_end = clock();

            mm.event("started " + com->algorithm_name + " decompression");

            clock_t decomp_start = clock();
            com->decompress(compressed_file_path, decompressed_file_path);
            clock_t decomp_end = clock();

            clock_t comp_time = (comp_end - comp_start);
            clock_t decomp_time = decomp_end - decomp_start;

            double compression_ratio = Profiler::calculate_compression_ratio(input_file_path, compressed_file_path);
            double comp_speed = Profiler::calculate_processing_speed(comp_time, input_file_path);
            double decomp_speed = Profiler::calculate_processing_speed(decomp_time, decompressed_file_path);

            table.addRow(                                                            //
                test_file + "(" + FileSizeUtil::getFileSize(input_file_path) + ")",  //
                ((double)comp_time / CLOCKS_PER_SEC),                                //
                compression_ratio,                                                   //
                comp_speed,                                                          //
                Profiler::calculate_space_saving(compression_ratio),                 //
                ((double)decomp_time / CLOCKS_PER_SEC),                              //
                decomp_speed                                                         //
            );

            files_tested_on++;
            avg_comp_speed += comp_speed;
            avg_decomp_speed += decomp_speed;
            avg_comp_ratio += compression_ratio;
        }

        if (files_tested_on > 0) {
            average.addRow(com->algorithm_name, avg_comp_ratio / files_tested_on, avg_comp_speed / files_tested_on,
                           avg_decomp_speed / files_tested_on);
        }
        table.print(std::cout);
    }
    std::cout << "\ncomparison between different algorithms: \n";
    average.print(std::cout);
    mm.event("Algorithms Execution Finished");
}