#pragma once
#include <iostream>
#include <cstdint>
#include <limits>
#include "Sandpile.h"

struct Arguments {
    char* inputFile{};
    char* outputPath{};
    uint64_t maxIter = std::numeric_limits<uint64_t>::max();
    uint64_t freq = 0;

    static Arguments Parse(char* argv[], int argc);
    static void PrintArguments(Arguments arguments);
    static Sandpile SetSandpiles(Arguments arguments);
};