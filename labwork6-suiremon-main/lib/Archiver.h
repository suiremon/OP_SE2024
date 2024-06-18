#include "Encoder.h"

unsigned char controlBitsBase = 3;
unsigned char kHammingBits = 8;
unsigned char kInformationBits = 4;

unsigned char kSizeHeaderSize = CHAR_BIT;
unsigned char kNameHeaderSize = 25;

void GetFileSize(std::ifstream& file, size_t& filesize) {
    char tmp;
    while (file.get(tmp)) {
        ++filesize;
    }
    file.clear();
    file.seekg(0, std::ios::beg);
}

void fillSizeHeader(unsigned char* sizeHeader, size_t& filesize) {
    for (size_t i = 0; i < kSizeHeaderSize; ++i) {
        sizeHeader[i] = (filesize >> ((kSizeHeaderSize - i - 1) * CHAR_BIT));
    }
}

void GetCodeFromByte(const unsigned char& byte, std::vector<bool>& code) {
    for (int i = 0; i < CHAR_BIT; ++i) {
        code[CHAR_BIT - i - 1] = (byte >> 1) & 1;
    }
}

void GetByteFromCode(unsigned char& byte, const std::vector<bool>& code) {
    for (int i = 0; i < CHAR_BIT; ++i) {
        byte += static_cast<unsigned char>(code[i] << (CHAR_BIT - i - 1));
    }
}

void AddEncodedByte(unsigned char byte, std::ofstream& arch) {
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
            if ((i & (i+1)) == 0) {
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
        arch << byte;
    }
}

unsigned char GetDecodedByte(const unsigned char& firstByte, const unsigned char& secondByte) {
    unsigned char result = 0;
    std::vector<bool> fst(CHAR_BIT);
    std::vector<bool> scnd(CHAR_BIT);
    std::vector<bool> resultCode(CHAR_BIT);
    for (size_t j = 0; j < CHAR_BIT; j++) {
        fst[CHAR_BIT - j - 1] = (firstByte >> j) & 1;
    }
    for (size_t j = 0; j < CHAR_BIT; j++) {
        scnd[CHAR_BIT - j - 1] = (secondByte >> j) & 1;
    }

    bool isFirstPartDecoded = FixErrors(fst);
    bool isSecondPartDecoded = FixErrors(scnd);

    if (!isFirstPartDecoded || !isSecondPartDecoded) {
        exit(1);
    }

    resultCode[0] = fst[2];
    resultCode[1] = fst[4];
    resultCode[2] = fst[5];
    resultCode[3] = fst[6];

    resultCode[4] = scnd[2];
    resultCode[5] = scnd[4];
    resultCode[6] = scnd[5];
    resultCode[7] = scnd[6];

    for (size_t j = 0; j < CHAR_BIT; j++) {
        if (resultCode[j]) {
            result += 1 << (CHAR_BIT - j - 1);
        }
    }

    return result;
}

void AddSizeHeader(std::ofstream& arch, size_t bitsize) {
    unsigned char sizeHeader[kSizeHeaderSize];
    fillSizeHeader(sizeHeader, bitsize);
    for (uint8_t k = 0; k < kSizeHeaderSize; k++) {
        AddEncodedByte(sizeHeader[k], arch);
    }
}

size_t GetSizeHeader(unsigned char byte, std::ifstream& arch) {
    size_t size = 0;
    unsigned char encoded[kSizeHeaderSize * 2];
    std::vector<unsigned char> decoded(kSizeHeaderSize);
    encoded[0] = byte;

    for (size_t i = 1; i < kSizeHeaderSize * 2; ++i) {
        arch >> byte;
        encoded[i] = byte;
    }
    for (size_t i = 0; i < kSizeHeaderSize; i++) {
        std::cout << "SIZE" << '\n';
        decoded[i] = GetDecodedByte(encoded[i * 2], encoded[i * 2 + 1]);
    }
    for (size_t i = 0; i < kSizeHeaderSize; i++) {
        size += (decoded[i] << ((kSizeHeaderSize - i - 1) * CHAR_BIT));
    }
    return size;
}

void AddNameHeader(std::ofstream& arch, const std::string& filename) {
    unsigned char name[kNameHeaderSize];
    for (size_t i = 0; i < kNameHeaderSize; i++) {
        if (i < filename.size()) {
            name[i] = filename[i];
        } else {
            name[i] = '\0';
        }
    }
    for (uint8_t k = 0; k < kNameHeaderSize; k++) {
        AddEncodedByte(name[k], arch);
    }
}

std::string GetNameHeader(std::ifstream& arch) {
    unsigned char byte;
    unsigned char encoded[kNameHeaderSize * 2];
    unsigned char decoded[kNameHeaderSize];
    for (size_t i = 0; i < kNameHeaderSize * 2; i++) {
        arch >> byte;
        encoded[i] = byte;
    }
    for (size_t i = 0; i < kNameHeaderSize; i++) {
        std::cout << "NAME" << '\n';
        decoded[i] = GetDecodedByte(encoded[i * 2], encoded[i * 2 + 1]);
    }
    std::string filename(reinterpret_cast<const char* const>(decoded));
    return filename;
}

void AddBitsizeHeader(std::ofstream& arch, unsigned char controlBitsCount = 3) {
    AddEncodedByte(controlBitsCount, arch);
}

unsigned char GetBitsizeHeader(std::ifstream& arch) {
    unsigned char firstByte;
    arch >> firstByte;

    unsigned char secondByte;
    arch >> secondByte;

    unsigned char result = GetDecodedByte(firstByte, secondByte);
    std:: cout << result << '\n';
    return result;
}

void AddFileInfo(std::ofstream& arch, std::ifstream& inputFile) {
    size_t filesize;
    GetFileSize(inputFile, filesize);
    for (size_t i = 0; i < filesize; ++i) {
        AddByte(inputFile.get(), arch);
    }
}

char* toChars(const std::string& old) {
    char result[old.length()];
    for (size_t i = 0; i < old.length(); ++i) {
        result[i] = old[i];
    }
    return result;
}

class Archive {
public:
    std::string archname;
    std::vector<std::string> filenames;
    void AddFile(const std::string& filename, unsigned char controlBitsCount = 3) const {
        std::ifstream file;
        size_t filesize;
        file.open(filename, std::ios::binary);
        GetFileSize(file, filesize);

        std::ofstream arch;
        arch.open(archname, std::ios::app | std::ios::binary);

        AddSizeHeader(arch, filesize);
        AddNameHeader(arch, filename);
        AddBitsizeHeader(arch, controlBitsCount);
        AddFileInfo(arch, file);

        file.close();
        arch.close();
    };
    void GetList(std::vector<std::string>& list) const {
        std::ifstream arch(archname, std::ios::in | std::ios::binary);
        size_t filesize;
        unsigned char byte;
        std::string filename;
        unsigned char bitsize;
        unsigned char fileBytes;

        while (arch >> byte) {
            filesize = GetSizeHeader(byte, arch);
            filename = GetNameHeader(arch);
            bitsize = GetBitsizeHeader(arch);
            fileBytes = filesize * (CHAR_BIT / kInformationBits + CHAR_BIT % (kInformationBits != 0));

            list.push_back(filename);
            std::cout << filename << '\n';

            arch.seekg(fileBytes, std::ios::cur);
        }
    }
    void ExtractFile(std::string& filename, std::string path) {
        std::ifstream arch(archname, std::ios::in | std::ios::binary);
        std::ofstream file(path + '\\' + filename, std::ios::binary);
        size_t filesize;
        unsigned char byte;
        std::string currentFilename;
        unsigned char bitsize;
        unsigned char fileBytes;

        while (arch >> byte) {
            filesize = GetSizeHeader(byte, arch);
            currentFilename = GetNameHeader(arch);
            bitsize = GetBitsizeHeader(arch);
            fileBytes = filesize * (CHAR_BIT / kInformationBits + CHAR_BIT % (kInformationBits != 0));

            if (currentFilename == filename) {
                for (size_t i = 0; i < fileBytes; i+=2) {
                    unsigned char firstByte;
                    arch >> firstByte;
                    unsigned char secondByte;
                    arch >> secondByte;
                    unsigned char result = GetDecodedByte(firstByte, secondByte);
                    file << result;
                }
            } else {
                arch.seekg(fileBytes, std::ios::cur);
            }
        }
    }
    void DeleteFile(std::string& filename) {
        Archive resultArch;
        resultArch.archname = "amogus.haf";

        std::vector<std::string> archFiles;
        GetList(archFiles);

        for (auto & i : archFiles) {
            if (i != filename) {
                ExtractFile(i, std::filesystem::temp_directory_path().string());
                resultArch.AddFile(std::filesystem::temp_directory_path().string() + i);
            }
        }
        remove(toChars(archname));
        rename("amogus.haf", toChars(archname));
    }
};

std::string GetName(const std::string& filename) {
    size_t index = filename.length();
    for (size_t i = 0; i < filename.size(); i++) {
        if (filename[i] == '.') {
            index = i;
            break;
        }
    }
    return filename.substr(0, index);
}

void Create(std::string& archname, std::vector<std::string>& filenames, unsigned char controlBitCount = 3) {
    Archive archive;
    archive.archname = GetName(archname) + ".haf";
    std::ofstream arch(archive.archname);
    for (auto& file: filenames) {
        archive.AddFile(file);
    }
};
void List(std::string& archname) {
    Archive archive;
    archive.archname = GetName(archname) + ".haf";
    std::vector<std::string> list;
    archive.GetList(list);
};
void Extract(std::string& archname, std::vector<std::string>& filenames) {
    Archive archive;
    archive.archname = GetName(archname) + ".haf";
    std::filesystem::create_directory(GetName(archname));
    for (auto& i : filenames) {
        archive.ExtractFile(i, GetName(archname));
    }
};
void Append(std::string& archname, std::vector<std::string>& filenames) {
    Archive archive;
    archive.archname = GetName(archname) + ".haf";
    for (auto& i : filenames) {
        archive.AddFile(i);
    }
};
void Delete(std::string& archname, std::vector<std::string>& filenames) {
    Archive archive;
    archive.archname = GetName(archname) + ".haf";
    for (auto& i : filenames) {
        archive.DeleteFile(i);
    }
};
void Concat(std::string& fstArchname, std::string& scndArchname) {
    Archive fstArchive;
    fstArchive.archname = GetName(fstArchname) + ".haf";
    Archive scndArchive;
    scndArchive.archname = GetName(scndArchname) + ".haf";
    std::vector<std::string> scndArchiveFilenames;
    scndArchive.GetList(scndArchiveFilenames);
    for (auto& i: scndArchiveFilenames) {
        fstArchive.AddFile(i);
    }
};