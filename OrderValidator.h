#pragma once
#include "DependencyProvider.h"

class OrderValidator {
private:
    const std::unordered_map<int, std::unordered_set<int>>& dependencies;
public:
    OrderValidator(const std::unordered_map<int, std::unordered_set<int>>& deps) : dependencies(deps) {}

    bool isOrderCorrect(const std::vector<std::string>& filenames) const {
        std::unordered_map<std::string, int> filenameToIndex;
        for (size_t i = 0; i < filenames.size(); ++i) {
            filenameToIndex[filenames[i]] = i;
        }

        for (const auto& dependency : dependencies) {
            int fileIndex = dependency.first;
            const auto& dependentFiles = dependency.second;
            for (int dependentFileIndex : dependentFiles) {
                if (filenameToIndex.find(filenames[fileIndex]) == filenameToIndex.end() ||
                    filenameToIndex.find(filenames[dependentFileIndex]) == filenameToIndex.end()) {
                    // One of the files involved in the dependency is missing in the list
                    return false;
                }

                if (filenameToIndex[filenames[fileIndex]] > filenameToIndex[filenames[dependentFileIndex]]) {
                    // Dependency violation: fileIndex should come before dependentFileIndex
                    return false;
                }
            }
        }

        return true;
    }
};