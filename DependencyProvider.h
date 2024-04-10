#pragma once
#include "FilenameMatcher.h"

class DependencyProvider {
private:
    const std::string directoryPath;
    const FilenameMatcher& filenameMatcher;
    std::unordered_map<int, std::unordered_set<int>> dependencies;

    // Function to process each file recursively and find dependencies
    void processFile(const std::string& filename, std::unordered_set<std::string>& visited) {
        // Check if the file has already been visited to avoid infinite recursion
        if (visited.find(filename) != visited.end()) {
          return;
          // std::cout << std::endl;
          //   throw std::runtime_error("Circular include detected; file processing is terminated.");
        }

        // Add the file to visited set
        visited.insert(filename);

        // Get the index of the current filename
        int currentIndex = filenameMatcher.GetFilenameToNumber(filename);

        // Open the file for reading
        std::ifstream file(directoryPath + "/" + filename);
        if (!file.is_open()) {
            throw std::runtime_error("The file " + filename + " was not found; file processing is terminated.");
        }

        // Read the file line by line
        std::string line;
        while (std::getline(file, line)) {
            // Check if the line contains an include directive
            if (line.find("#include") != std::string::npos) {
                // Extract the included filename
                std::size_t start = line.find_first_of('"');
                std::size_t end = line.find_last_of('"');
                if (start != std::string::npos && end != std::string::npos && start < end) {
                    std::string includedFilename = line.substr(start + 1, end - start - 1);
                    // Get the index of the included filename
                    int includedIndex = filenameMatcher.GetFilenameToNumber(includedFilename);
                    // Add dependency
                    dependencies[currentIndex].insert(includedIndex);
                    // Process the included file recursively
                    processFile(includedFilename, visited);
                }
            }
        }

        // Close the file
        file.close();
    }

public:
    DependencyProvider(const std::string& path, const FilenameMatcher& matcher) : directoryPath(path), filenameMatcher(matcher) {}

    // Provide dependencies between file numbers
    std::unordered_map<int, std::unordered_set<int>> provideDependencies() {
        // Initialize visited set
        std::unordered_set<std::string> visited;

        // Open directory
        DIR* dir = opendir(directoryPath.c_str());
        if (dir == nullptr) {
            throw std::runtime_error("Error opening directory");
        }

        // Read directory entries
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_REG) { // Regular file
                std::string filename(entry->d_name);
                if (visited.find(filename) == visited.end()){
                    processFile(filename, visited);
                }
            }
        }

        // Close directory
        closedir(dir);

        return dependencies;
    }
};
