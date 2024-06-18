#pragma once
#include <cinttypes>
#include <iostream>
#include <cstdint>


struct int2023_t {
    const static int BYTE_SIZE = 8;
    const static int SIZE = 253;
    const static int INDEX_MAX = 252;
    const static int HALF_BASE = 128;
    uint8_t data[SIZE] = {0};

    int2023_t() = default;
    int2023_t(int64_t value) {
        if (value < 0) {
            value *= -1;
            data[SIZE - 1] = data[SIZE - 1] + HALF_BASE;
        }
        for (int i = 0; i < SIZE - 1; i++) {
            data[i] += value & 0xFF;
            value >>= BYTE_SIZE;
        }
    }
};

static_assert(sizeof(int2023_t) <= 253, "Size of int2023_t must be no higher than 253 bytes");

int2023_t from_int(int32_t i);

int2023_t from_string(const char* buff);

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator+=(int2023_t& lhs, const int2023_t& rhs);

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator-=(int2023_t& lhs, const int2023_t& rhs);

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator*=(int2023_t& lhs, const int2023_t& rhs);

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator/=(int2023_t& lhs, const int2023_t& rhs);

bool operator>(const int2023_t& lhs, const int2023_t& rhs);

bool operator<(const int2023_t& lhs, const int2023_t& rhs);

bool operator==(const int2023_t& lhs, const int2023_t& rhs);

bool operator!=(const int2023_t& lhs, const int2023_t& rhs);

bool operator>=(const int2023_t& lhs, const int2023_t& rhs);

bool operator<=(const int2023_t& lhs, const int2023_t& rhs);

std::ostream& operator<<(std::ostream& stream, const int2023_t& value);
