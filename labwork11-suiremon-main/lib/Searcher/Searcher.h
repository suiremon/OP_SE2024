#include <string>
#include <cstring>
#include <iostream>
#include <utility>
#include <vector>
#include <stack>
#include <string>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <map>

static std::vector<std::string> keys;

class Config {
private:
    std::vector<std::string> RPN;
public:
    Config() = default;
    Config(int, char**);

    std::vector<std::string> infixToRPN(const std::vector<std::string>&);
    std::vector<std::string> getRPN();
    static std::string checkKey(const std::string& key);
};

class Searcher {
private:
    std::vector<std::pair<std::string, size_t>> evalRPN(const std::vector<std::string>&);
    std::string getKey(const std::string&);
    void Parse(const std::string&, const std::string&);
    double countIDF();
    void countBM25();

    size_t fileSize;

public:
    std::unordered_map<std::string, std::vector<std::pair<std::string, size_t>>> files;
    std::vector<std::pair<std::string, size_t>> RPN;
    Searcher(int argc, char* argv[]) {
        Config conf(argc, argv);
        std::ifstream file("data.txt");
        if (!file.is_open()) {
            std::cerr << "Data file does not exist." << std::endl;
            exit(EXIT_FAILURE);
        }
        file.seekg(-1, std::ios_base::end);
        while (file.peek() != '\n') {
            file.seekg(-1, std::ios_base::cur);
        }
        file.seekg(1, std::ios_base::cur);
        std::string tmp;
        getline(file, tmp);
        fileSize = std::stoll(tmp);
        file.seekg(std::ios_base::beg);
        std::string words;
        while (getline(file, words)) {
            std::string key = getKey(words);
            std::string checker = Config::checkKey(key);
            if (!checker.empty()) {
                Parse(checker, words);
            }
        }
        RPN = evalRPN(conf.getRPN());
    };

    void PrintResult();

    std::map<std::string, double> scores;
};