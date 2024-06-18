#include "Parser.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <filesystem>

const char* kInputArg = "--input";
const char* kOutputArg = "--output";
const char* kMaxIterArg = "--max-iter";
const char* kFreqArg = "--freq";

void ArgumentError() {
    std::cerr << "Error: incorrect argument.";
    exit(EXIT_FAILURE);
}

bool IsNumber(const std::string_view& str) {
    for (char i: str) {
        if (!isdigit(i)) {
            return false;
        }
    }
    return true;
}

bool CheckFile(char* filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::in);
    if (!file.is_open()) {
        return false;
    }
    return true;
}

bool CheckDir(const char* dirname) {
    return std::filesystem::exists(dirname);
}

Arguments Arguments::Parse(char* argv[], int argc) {
    if (argc < 7) {
        ArgumentError();
    }

    Arguments arguments;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], kInputArg) == 0) {
            if (i + 1 >= argc || !CheckFile(argv[i + 1])) {
                ArgumentError();
            }
            arguments.inputFile = argv[i + 1];
        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], kOutputArg) == 0) {
            if (i + 1 >= argc || !CheckDir(argv[i + 1])) {
                ArgumentError();
            }
            arguments.outputPath = argv[i + 1];
        } else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], kMaxIterArg) == 0) {
            std::cout << argv[i+1] << '\n';
            if (i + 1 >= argc || !IsNumber(argv[i+1])) {
                ArgumentError();
            }
            arguments.maxIter = atoll(argv[i + 1]);
        } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], kFreqArg) == 0) {
            if (i + 1 >= argc || !IsNumber(argv[i + 1])) {
                ArgumentError();
            }
            arguments.freq = atoll(argv[i + 1]);
        }
    }
    return arguments;
}

void Arguments::PrintArguments(Arguments arguments) {
    std::cout << "=================\nPARAMETERS" << std::endl
              << "Input file: " << arguments.inputFile << std::endl
              << "Output path: " << arguments.outputPath << std::endl
              << "Max iterations: " << arguments.maxIter << std::endl
              << "Frequency: " << arguments.freq << std::endl
              << "=================\n" << std::endl;
}

void Split(const char tmp[100], int16_t& x, int16_t& y, uint64_t& value) {
    int i = 0;
    int count = 0;
    int64_t buff = 0;
    int16_t tmp_x = 0;
    int16_t tmp_y = 0;
    while (tmp[i] != '\0') {
        if (tmp[i] != '\t') {
            if (count == 0) {
                tmp_x *= 10;
                tmp_x += (tmp[i] - '0');
            } else if (count == 1) {
                tmp_y *= 10;
                tmp_y += (tmp[i] - '0');
            } else if (count == 2) {
                buff *= 10;
                buff += (tmp[i] - '0');
            }
        } else {
            if (count == 0) {
                x = tmp_x;
            } else if (count == 1) {
                y = tmp_y;
            } else if (count == 2) {
                value = buff;
            }
            count++;
        }
        ++i;
    }
    value = buff;
}

Sandpile Arguments::SetSandpiles(Arguments arguments) {
    uint64_t params[3];
    int16_t x = 0;
    int16_t y = 0;
    uint64_t value = 0;
    char tmp[100];
    int16_t min_x = 0;
    int16_t min_y = 0;
    int16_t size_x = 10;
    int16_t size_y = 10;
    int8_t border = 1;
    std::ifstream file(arguments.inputFile);
    while (file.getline(tmp, 100, '\n')) {
        Split(tmp, x, y, value);
        min_x = std::min(min_x, x);
        min_y = std::min(min_y, y);
        size_x = std::max(size_x, x);
        size_y = std::max(size_y, y);
    }
    min_x = abs(min_x);
    min_y = abs(min_y);
    file.clear();
    file.seekg(std::ifstream::beg);
    Grid grid(size_x + border, size_y + border);
    Sandpile sandpile(0, 0, 0, grid);
    while (file.getline(tmp, 100, '\n')) {
        Split(tmp, x, y, value);
        sandpile.SetGrains(x + min_x, y + min_y, value);
    }
    return sandpile;
}