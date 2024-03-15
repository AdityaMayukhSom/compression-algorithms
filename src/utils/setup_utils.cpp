#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#endif

#include <iostream>
#include <string>

class SetupUtils {
   public:
    static bool createDirectory(const std::string& path) {
#ifdef _WIN32
        if (CreateDirectoryA(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
            std::cout << "Directory created.\n";
            return true;
        }
#else

        if (mkdir(path.c_str(), 0777) == 0) {
            std::cout << "Directory created.\n";
            return true;
        }
#endif
        else {
            //     std::cerr << "Failed to create directory!\n";
            return false;
        }
    }
};