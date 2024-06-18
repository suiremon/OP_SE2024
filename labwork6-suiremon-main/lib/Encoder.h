#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <filesystem>

const uint16_t kBits[] = {0, 4};

void Encode(std::vector<bool> &code) {
    for (int i = 1; i < code.size() - 1; ++i) {
        if ((i & (i - 1)) == 0) {
            int tmp = 0;
            for (int j = i; j < code.size() - 1; j += 2 * i) {
                for (int u = (j == i); u < i && (u + j) < code.size() - 1; ++u) {
                    if (code[u + j]) tmp ^= 1;
                }
            }
            if ((tmp == 0 && code[i]) || (tmp == 1 && !code[i])) {
                code[i] = tmp;
            }
        }
    }

    bool lastControl = false;
    for (size_t i = 0; i < code.size() - 1; i++) {
        lastControl = (lastControl != code[i]);
    }
    code[code.size() - 1] = lastControl;
}

bool FixErrors(std::vector<bool> &code) {
    size_t errBit = 0;
    for (int i = 1; i < code.size() - 1; ++i) {
        if ((i & (i - 1)) == 0) {
            int tmp = 0;
            for (int j = i; j < code.size() - 1; j += 2 * i) {
                for (int u = (j == i); u < i && (u + j) < code.size() - 1; ++u) {
                    if (code[u + j]) tmp ^= 1;
                }
            }
            if ((tmp == 0 && code[i]) || (tmp == 1 && !code[i])) {
                errBit += i;
            }
        }
    }
    if (errBit == 0) {
        return true;
    }
    bool lastControl = false;
    for (auto &&bit: code) {
        lastControl = (lastControl != bit);
    }
    if (lastControl) {
        code[errBit - 1] = !code[errBit - 1];
        return true;
    } else {
        return false;
    }
}

std::vector<bool> Decode(const std::vector<bool>& code) {
    std::vector<bool> result;
    for (int i = 0; i < CHAR_BIT; ++i) {
        if (((i + 1) & i) != 0) {
            result.push_back(code[i]);
        }
    }
    return result;
}

class File {
public:
    std::string filename;
    size_t filesize = 0;

    explicit File(std::string name) : filename(std::move(name)) {
        std::ifstream file;
        file.open(filename);
        char tmp;
        while (file.get(tmp)) {
            ++filesize;
        }
        file.clear();
        file.seekg(0, std::ios::beg);
    }
};

void AddByte(unsigned char byte, std::ofstream &out) {
    std::vector<bool> bits(CHAR_BIT);
    size_t control;
    std::vector<bool> code;
    for (size_t j = 0; j < CHAR_BIT; j++) {
        bits[CHAR_BIT - j - 1] = (byte >> j) & 1;
    }

    for (uint16_t count: kBits) {
        control = 1;
        byte = 0;
        for (size_t i = 0; i < CHAR_BIT; ++i) {
            if (i > control) {
                control *= 2;
                code.push_back(false);
            } else {
                code.push_back(bits[count]);
                ++count;
            }
        }
        Encode(code);
        for (int i = 0; i < CHAR_BIT; ++i) {
            if (code[i]) {
                byte += static_cast<unsigned char>(1 << (CHAR_BIT - i - 1));
            }
        }
        out << byte;
    }
}

class Encoder {
    static void EncodeFile(const File &file, std::ofstream &out) {
        unsigned char current;
        std::ifstream inputFile;
        inputFile.open(file.filename, std::ios::binary);

        for (size_t i = 0; i < file.filesize; ++i) {
            current = inputFile.get();
            AddByte(current, out);
        }
    };
};