#include <iostream>
#include "SearchEngine.h"

size_t fileCount = 0;

void SearchEngine::saveToFile(std::unordered_map<std::string, std::unordered_map<std::filesystem::path, size_t>>& index) {
    std::ofstream data("data.txt", std::ios_base::app);
    for (const auto& pair : index) {
        data << pair.first << ": ";
        for (const auto& path : pair.second) {
            data << path.first << '|' << path.second << '|';
        }
        data << std::endl;
    }
    data << fileCount;
    index.clear();
}

void SearchEngine::indexFiles(const std::filesystem::path& directory, std::unordered_map<std::string, std::unordered_map<std::filesystem::path, size_t>>& index) {

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            ++fileCount;
            std::string content;
            std::ifstream file(entry.path());
            std::string line;
            while (getline(file, line)) {
                std::stringstream ss(line);
                std::string word;
                while (ss >> word) {
                    ++index[word][entry.path()];
                }
            }
        } else if (entry.is_directory()) {
            indexFiles(entry.path(), index);
        }
        if (index.size() == std::numeric_limits<size_t>::max() / sizeof(std::filesystem::path)) {
            saveToFile(index);
        };
    }
    saveToFile(index);
}

SearchEngine::SearchEngine(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <directory>" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::filesystem::path directory(argv[1]);
    if (!std::filesystem::exists(directory) ||!std::filesystem::is_directory(directory)) {
        std::cerr << "Directory does not exist: " << directory << std::endl;
        exit(EXIT_FAILURE);
    }

    std::unordered_map<std::string, std::unordered_map<std::filesystem::path, size_t>> index;
    SearchEngine::indexFiles(directory, index);
}
