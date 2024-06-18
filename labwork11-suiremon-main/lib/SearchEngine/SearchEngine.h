#include <filesystem>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <set>

class SearchEngine {
    std::filesystem::path dataPath;
public:
    static void indexFiles(const std::filesystem::path&, std::unordered_map<std::string, std::unordered_map<std::filesystem::path, size_t>>&);
    static void saveToFile(std::unordered_map<std::string, std::unordered_map<std::filesystem::path, size_t>>& index);
    explicit SearchEngine(int, char**);
};