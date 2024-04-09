#pragma once
#include "includes.h"
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <string>
#include <cstring>
#include <dirent.h>

class FilenameMatcher {
private:
    std::unordered_map<std::string, int> filenameToNumber;
    std::vector<std::string> numberToFilename;

public:
    // Constructor to initialize the FilenameMatcher object
    FilenameMatcher(const std::string& directoryPath) {
        DIR* dir = opendir(directoryPath.c_str());
        if (dir == nullptr) {
            throw std::runtime_error("Error opening directory");
        }

        dirent* entry;
        int index = 0;
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_REG) { // Regular file
                std::string filename(entry->d_name);
                filenameToNumber[filename] = index;
                numberToFilename.push_back(filename);
                ++index;
            }
        }
        closedir(dir);

        if (index == 0) {
            throw std::runtime_error("No files found in the directory");
        }
    }

    // Function to get the number corresponding to a filename
    int GetFilenameToNumber(const std::string& filename) const {
        if (filenameToNumber.find(filename) != filenameToNumber.end()) {
            return filenameToNumber.at(filename);
        }
        throw std::out_of_range("Filename not found in the directory");
    }

    // Function to get the filename corresponding to a number
    std::string GetNumberToFilename(int number) const {
        if (number >= 0 && number < numberToFilename.size()) {
            return numberToFilename[number];
        }
        throw std::out_of_range("Number out of range");
    }
};
