#pragma once
#include "Sandpile.h"

struct Color {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char reserved;
};

struct Image {
    unsigned int m_width;
    unsigned int m_height;
    Color* m_colors;
    Image(unsigned int  width, unsigned int  height) : m_width(width), m_height(height) {    };
    ~Image() = default;

    void Export(const char* path, Sandpile sandpile) const;

};

