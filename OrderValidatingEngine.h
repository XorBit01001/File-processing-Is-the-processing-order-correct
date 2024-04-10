#pragma once
#include "OrderValidator.h" // Include header file for OrderValidator class

class OrderValidatingEngine {
public:
    // Function to execute the order validation process
    void execute(std::istream& input, std::ostream& output) {
        // Read directory path from input
        std::string directoryPath;
        input >> directoryPath;

        // Create FilenameMatcher object for matching filenames with numbers
        FilenameMatcher matcher(directoryPath);

        // Create DependencyProvider object for providing dependencies between files
        DependencyProvider dependencyProvider(directoryPath, matcher);

        // Obtain dependencies between file numbers
        auto dependencies = dependencyProvider.provideDependencies();

        // Read filenames from input
        std::vector<std::string> filenames(matcher.GetFilesCountInDirectory());
        for(int i = 0; i < matcher.GetFilesCountInDirectory(); ++i){
            input >> filenames[i];
        }

        // Create OrderValidator object for validating the order of filenames
        OrderValidator validator(dependencies);

        // Check if the order of filenames is correct
        if (validator.isOrderCorrect(filenames)) {
            output << "File processing order is correct." << std::endl;
        } else {
            output << "Incorrect file processing order." << std::endl;
        }
    }
};
