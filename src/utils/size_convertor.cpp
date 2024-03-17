#pragma once

#include <sys/stat.h> /* for stat() function */

#include <iostream>
#include <sstream>
#include <string>

class SizeConvertor {
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

    static std::string convertSize(size_t size) {
        static const char *SIZES[] = {"B", "KB", "MB", "GB"};
        int div = 0;
        size_t rem = 0;

        while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
            rem = (size % 1024);
            div++;
            size /= 1024;
        }

        double size_d = (float)size + (float)rem / 1024.0;
        std::string result =
            convertToString(roundOff(size_d)) + " " + SIZES[div];
        return result;
    }

    static int fileSizeFromStat(const char *path) {
        struct stat results;

        if (stat(path, &results) == 0) {
            return results.st_size;
        } else {
            return -1;
        }
    }

   public:
    static std::string getFileSize(std::string path) {
        size_t size = fileSizeFromStat((const char *)path.c_str());
        return convertSize(size);
    }
};