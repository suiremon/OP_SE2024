#include <cstring>
#include <charconv>
#include "../lib/Parser.h"
#include "../lib/Bmp.h"

#include <iostream>

char* convertToBmp(unsigned int number) {
    unsigned int length = 0;
    unsigned int temp = number;
    while (temp != 0) {
        temp /= 10;
        length++;
    }
    unsigned int bufferSize = length + 5;
    char* result = new char[bufferSize];
    for (unsigned int i = 0; i < bufferSize - 5; i++) {
        result[bufferSize - 6 - i] = '0' + (number % 10);
        number /= 10;
    }
    result[bufferSize - 5] = '.';
    result[bufferSize - 4] = 'b';
    result[bufferSize - 3] = 'm';
    result[bufferSize - 2] = 'p';
    result[bufferSize - 1] = '\0';
    std::cout << result << std::endl;
    return result;
}

int main(int argc, char *argv[]) {
    Arguments arguments = Arguments::Parse(argv, argc);
    Arguments::PrintArguments(arguments);
    Sandpile sandpile = Arguments::SetSandpiles(arguments);
    unsigned int iter = 0;
    if (arguments.maxIter == std::numeric_limits<uint64_t>::max() && arguments.freq == 0) {
        sandpile.ToppleAll();
        Image image(sandpile.m_grid.m_y, sandpile.m_grid.m_x);
        ++iter;
        char* result = convertToBmp(iter);
        char tmp[strlen(arguments.outputPath)];
        strcpy(tmp, arguments.outputPath);
        strcat(tmp, result);
        image.Export(tmp, sandpile);
    } else {
        bool isStable = false;
        for (int i = 0; i < arguments.maxIter && !isStable; ++i) {
            isStable = sandpile.ToppleSome();
            if (i % arguments.freq == 0) {
                ++iter;
                Image image(sandpile.m_grid.m_y, sandpile.m_grid.m_x);
                char* result = convertToBmp(iter);
                char tmp[strlen(arguments.outputPath)];
                strcpy(tmp, arguments.outputPath);
                strcat(tmp, result);
                image.Export(tmp, sandpile);
            }
        }
    }
    Image image(sandpile.m_grid.m_y, sandpile.m_grid.m_x);
    image.Export("result.bmp", sandpile);
    std::cout << "Done!" << '\n';
    return 0;
}