#pragma once

#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>

class FileSizeUtil {
    static std::string convertToString(double num) {
        std::ostringstream convert;
        convert << num;
        return convert.str();
    }

    static double roundOff(double n) {
        double d = n * 100.0;
        int i = d + 0.5;
        d = (float)i / 100.0;
        return d;
    }

    static unsigned long fileSizeFromStat(const char *path) {
        std::uintmax_t size{};
        std::filesystem::path p(path);
        size = std::filesystem::file_size(p);
        return size;
    }

   public:
    static std::string getFileSize(std::string path) {
        try {
            size_t size = fileSizeFromStat((const char *)path.c_str());

            static const char *SIZES[] = {"B", "KB", "MB", "GB"};
            int div = 0;
            size_t rem = 0;

            while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
                rem = (size % 1024);
                div++;
                size /= 1024;
            }

            double size_d = (float)size + (float)rem / 1024.0;
            std::string result = convertToString(roundOff(size_d)) + " " + SIZES[div];
            return result;
        } catch (std::filesystem::filesystem_error &e) {
            std::cout << e.what() << "\n";
            return "Size Unavailable";
        }
    }
};