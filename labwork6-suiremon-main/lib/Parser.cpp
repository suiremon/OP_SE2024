#include "Parser.h"
#include <cstring>
#include <iostream>

const char* kFilenameArg = "--file";

bool IsNumber(const char* str) {
    for (; *(str + 1) != '\0'; ++str) {
        if (!std::isdigit(*str)) {
            return false;
        }
        ++str;
    }
    return true;
}

void Arguments::Parse(Arguments& arguments, int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        if ((strcmp(argv[i], "-c") == 0) || (strcmp(argv[i], "--create") == 0)) {
            arguments.operation = "create";
        } else if ((strcmp(argv[i], "-l") == 0) || (strcmp(argv[i], "--list") == 0)) {
            arguments.operation = "list";
        } else if ((strcmp(argv[i], "-x") == 0) || (strcmp(argv[i], "--extract") == 0)) {
            arguments.operation = "extract";
        } else if ((strcmp(argv[i], "-a") == 0) || (strcmp(argv[i], "--append") == 0)) {
            arguments.operation = "append";
        } else if ((strcmp(argv[i], "-d") == 0) || (strcmp(argv[i], "--delete") == 0)) {
            arguments.operation = "delete";
        } else if ((strcmp(argv[i], "-A") == 0) || (strcmp(argv[i], "--concatenate") == 0)) {
            arguments.operation = "concatenate";
        } else if ((strcmp(argv[i], "-f") == 0) || (strcmp(argv[i], "--file") == 0)) {
            arguments.archName = argv[i + 1];
            std::cout << arguments.archName << '\n';
        } else {
            if (IsNumber(argv[i])) {
                arguments.controlBits = atoi(argv[i]);
                if (arguments.controlBits != 7) {
                    std::cerr << "Incorrect control bits amount." << '\n';
                    exit(EXIT_FAILURE);
                }
            } else {
                if (strcmp(argv[i] + strlen(argv[i]) - 4, ".haf") == 0) {
                    arguments.secondArchName = argv[i];
                } else {
                    arguments.filenames.emplace_back(argv[i]);
                }
            }
        }
    }
}
