#include <iostream>
#include <string>
#include <fstream>
#include <memory.h>
#include "Bmp.h"

#pragma pack(push, 1)

struct BITMAPFILEHEADER {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

struct BITMAPINFOHEADER {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

#pragma pack(pop)

Color palette[5] = {
        {255, 255, 255, 0},
        {0,   255, 0,   0},
        {0,   255, 255, 0},
        {255, 0,   255, 0},
        {0,   0,   0,   0},
};

void Image::Export(const char* name, Sandpile sandpile) const {
    std::ofstream file(name, std::ios::binary);

    unsigned int rowSize = m_width / 2 + m_width % 2;
    unsigned int paddingPerRow = (4 - rowSize % 4) % 4;
    unsigned int totalDataSize = (rowSize + paddingPerRow) * m_height;

    BITMAPFILEHEADER fileHeader = {0};
    BITMAPINFOHEADER infoHeader = {0};

    fileHeader.bfType = 0x4D42;
    fileHeader.bfSize = totalDataSize + sizeof(palette) + sizeof(infoHeader) + sizeof(fileHeader);
    fileHeader.bfOffBits = sizeof(palette) + sizeof(infoHeader) + sizeof(fileHeader);

    infoHeader.biSize = sizeof(infoHeader);
    infoHeader.biWidth = m_width;
    infoHeader.biHeight = m_height;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 4;
    infoHeader.biCompression = 0;
    infoHeader.biSizeImage = totalDataSize;
    infoHeader.biXPelsPerMeter = 0;
    infoHeader.biYPelsPerMeter = 0;
    infoHeader.biClrUsed = 5;
    infoHeader.biClrImportant = 5;

    file.write((char *) &fileHeader, sizeof(fileHeader));
    file.write((char *) &infoHeader, sizeof(infoHeader));
    file.write((char *) palette, sizeof(palette));

    auto* pixelData = new unsigned char[totalDataSize];
    unsigned char* currentPixel = pixelData;
    for (int i = m_height - 1; i >= 0; --i) {
        for (int j = 0; j < m_width; j += 2) {
            uint64_t first_pixel = sandpile.at(i, j);
            uint64_t second_pixel = 4;
            if (j + 1 < m_width) {
                second_pixel = std::min((long long) sandpile.at(i, j + 1), 4ll);
            }
            *currentPixel = static_cast<uint8_t>(first_pixel << 4 | second_pixel);
            ++currentPixel;
        }
        currentPixel += paddingPerRow;
    }

    file.write((char*) (pixelData), totalDataSize);
    delete[] pixelData;
}