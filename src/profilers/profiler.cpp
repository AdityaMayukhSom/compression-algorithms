#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
class Profiler {
   public:
    static double calculate_space_saving(double compression_ratio) {
        if (compression_ratio < 0) {
            return -1;
        }
        if (compression_ratio == 0) {
            return 100;
        }
        return (1 - (1 / compression_ratio)) * 100;
    }

    static double calculate_compression_ratio(const std::string original_file_path,
                                              const std::string compressed_file_path) {
        std::ifstream original_file(original_file_path, std::ios::binary);
        if (!original_file.is_open()) {
            std::cerr << "Profiler error: Unable to open original file: " << original_file_path << std::endl;
            return -1;
        }

        std::ifstream compressed_file(compressed_file_path, std::ios::binary);
        if (!compressed_file.is_open()) {
            std::cerr << "Profiler rror: Unable to open compressed file:" << compressed_file_path << std::endl;
            return -1;
        }
        original_file.seekg(0, std::ios::end);
        compressed_file.seekg(0, std::ios::end);

        long original_file_size = original_file.tellg();
        long compressed_file_size = compressed_file.tellg();

        if (compressed_file_size == 0) {
            return -1;
        }

        double compression_ratio = original_file_size / (double)compressed_file_size;

        original_file.close();
        compressed_file.close();
        return compression_ratio;
    }

    static double calculate_processing_speed(const clock_t start, const clock_t end, const std::string filename,
                                             const std::string unit = "MB") {
        std::ifstream f(filename, std::ios::binary);

        if (!f.is_open()) {
            std::cerr << "Error: Unable to open file." << std::endl;
            return -1;
        }

        f.seekg(0, std::ios::end);

        long file_size = f.tellg();
        double f_size = 0.0;
        if (unit == "MB") {
            f_size = file_size / ((double)1024 * 1024);  // converting into mb
        } else if (unit == "KB") {
            f_size = file_size / ((double)1024);
        }

        double speed = f_size / ((double)(end - start) / CLOCKS_PER_SEC);
        return speed;
    }

    static double calculate_processing_speed(const clock_t time_taken, const std::string filename,
                                             const std::string unit = "MB") {
        std::ifstream f(filename, std::ios::binary);

        if (!f.is_open()) {
            std::cerr << "Error: Unable to open file." << std::endl;
            return -1;
        }

        f.seekg(0, std::ios::end);

        long file_size = f.tellg();
        double f_size = 0.0;
        if (unit == "MB") {
            f_size = file_size / ((double)1024 * 1024);  // converting into mb
        } else if (unit == "KB") {
            f_size = file_size / ((double)1024);
        }
        double speed = f_size / ((double)(time_taken) / CLOCKS_PER_SEC);
        return speed;
    }
};