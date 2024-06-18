#include <iostream>
#include <fstream>
#include <cstdint>
#include <limits>
#include <cstring>

bool IsNumber(const char* str) {
    for (; *(str + 1) != '\0'; ++str) {
        if (!std::isdigit(*str)) {
            return false;
        }
        ++str;
    }
    return true;
}

char GetDelimiter(char* del) {
    if (strlen(del) == 0) {
        std::cerr << "Error: delimiter is not specified.";
    } else if (strlen(del) == 1) {
        return del[0];
    } else if (del[0] == '\\') {
        switch (del[1]) {
            case 'n':
                return '\n';
            case 't':
                return '\t';
            case 'a':
                return '\a';
            case 'b':
                return '\b';
            case 'v':
                return '\v';
            case 'f':
                return '\f';
            case 'r':
                return '\r';
            case '0':
                return '\0';
            case '\\':
                return '\\';
        }
    }
    if (strlen(del) > 1) {
        std::cerr << "Error: delimiter must be a single char.";
    }
    exit(EXIT_FAILURE);
}

const char* kLinesArg = "--lines=";
const char* kDelimiterArg = "--delimiter=";
const char* delimiter = "\\n";

struct Arguments {
    char* filename;
    char char_delimiter = '\n';
    bool tail = false;
    uint64_t lines = std::numeric_limits<uint64_t>::max();
};

void ArgumentError() {
    std::cerr << "Error: incorrect argument.";
    exit(EXIT_FAILURE);
}

Arguments ParseArguments(char* argv[], int argc) {
    if (argc < 2) {
        ArgumentError();
    }

    Arguments arguments;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            if (i + 1 >= argc || !IsNumber(argv[i + 1])) {
                ArgumentError();
            }
            arguments.lines = atoll(argv[i + 1]);
        } else if (strncmp(argv[i], kLinesArg, strlen((kLinesArg))) == 0) {
            if (!IsNumber(argv[i] + strlen(kLinesArg))) {
                std::cout << 1;
                ArgumentError();
            }
            arguments.lines = atoll(argv[i] + strlen(kLinesArg));
        } else if (strcmp(argv[i], "-d") == 0) {
            if (i + 1 >= argc) {
                ArgumentError();
            }
            delimiter = argv[i + 1];
            arguments.char_delimiter = GetDelimiter(argv[i + 1]);
        } else if (strncmp(argv[i], kDelimiterArg, strlen(kDelimiterArg)) == 0) {
            delimiter = argv[i] + strlen(kDelimiterArg);
            arguments.char_delimiter = GetDelimiter(argv[i] + strlen(kDelimiterArg));
        } else if ((strcmp(argv[i], "-t") == 0) || (strcmp(argv[i], "--tail") == 0)) {
            arguments.tail = true;
        } else {
            if ((strcmp(argv[i - 1], "-l") != 0) && (strcmp(argv[i - 1], "-d")) != 0) {
                arguments.filename = argv[i];
            }
        }
    }
    return arguments;
}

void CheckFile(Arguments& arguments) {
    std::ifstream file(arguments.filename, std::ios::binary | std::ios::in);
    if (!file.is_open()) {
        ArgumentError();
    }
}

uint64_t GetFileLength(const Arguments& arguments) {
    uint64_t file_length;
    std::ifstream file(arguments.filename, std::ios::binary | std::ios::in);
    file.seekg(0, std::ifstream::end);
    file_length = file.tellg();
    file.seekg(0, std::ifstream::beg);
    return file_length;
}

void PrintFromBeginning(const Arguments& arguments, uint64_t file_length) {
    std::ifstream file(arguments.filename, std::ios::binary | std::ios::in);
    uint64_t delimiter_counter = 0;
    char tmp;
    for (uint64_t i = 0; i < file_length; i++) {
        file.get(tmp);
        if (tmp == arguments.char_delimiter) {
            ++delimiter_counter;
            if (delimiter_counter >= arguments.lines) {
                std::cout << tmp;
                if (arguments.char_delimiter != '\n') {
                    std::cout << '\n';
                }
                break;
            }
        }
        std::cout << tmp;
    }
}

void PrintFromEnd(const Arguments& arguments, uint64_t file_length) {
    char tmp;
    uint64_t tail_length = file_length;
    uint64_t delimiter_counter = 0;
    std::ifstream file(arguments.filename, std::ios::binary | std::ios::in);
    while (tail_length-- &&
           delimiter_counter <= arguments.lines) { //looking for the beginning to output from the end
        file.seekg(tail_length, std::ifstream::beg);
        if (file.get() == arguments.char_delimiter) {
            ++delimiter_counter;
            if (delimiter_counter == arguments.lines) {
                break;
            }
        }
    }
    std::cout << "\n";
    tail_length++;
    file.seekg(tail_length, std::ifstream::beg);
    for (uint64_t i = tail_length; i < file_length; i++) {
        file.get(tmp);
        std::cout << tmp;
    }
    std::cout << "\n";
}

void ShowArguments(const Arguments& arguments, char* argv[]) {
    std::cout << "=================\nPARAMETERS" << std::endl
              << "Filename: " << arguments.filename << std::endl
              << "Number of lines: " << arguments.lines << std::endl
              << "Delimiter: " << delimiter << std::endl
              << "Output: " << (arguments.tail ? "from the end" : "from the beginning") << std::endl
              << "=================\n" << std::endl;
}

int main(int argc, char* argv[]) {
    auto arguments = ParseArguments(argv, argc);
    CheckFile(arguments);
    uint64_t file_length = GetFileLength(arguments);
    if (arguments.lines == -1) {
        arguments.lines = file_length;
    }

    ShowArguments(arguments, argv);


    if (!arguments.tail) {
        PrintFromBeginning(arguments, file_length);
    } else {
        PrintFromEnd(arguments, file_length);
    }
    std::cout << "\n";
    return EXIT_SUCCESS;
}
