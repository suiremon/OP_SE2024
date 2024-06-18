#pragma once
#include <iostream>
#include <cstdint>
#include <valarray>

struct Grid {
    uint16_t m_x{};
    uint16_t m_y{};
    uint64_t* grid;
    Grid(uint16_t size_x, uint16_t size_y) : m_x(size_x), m_y(size_y) {
        grid = new uint64_t[size_x * size_y]{0};
    }
    uint64_t& at(uint16_t x, uint16_t y) const {
        return grid[x * m_y + y];
    }
    uint64_t operator[](uint16_t ind) const {
        return grid[ind];
    }
    static void ExpandGrid(Grid& g, uint16_t expand_size, int8_t type);
};

struct Sandpile {
    Grid m_grid;
    uint64_t m_height{};
    uint16_t m_x = m_grid.m_x;
    uint16_t m_y = m_grid.m_y;
    Sandpile(uint16_t x, uint16_t y, uint64_t height, Grid& grid) : m_height(height), m_grid(grid) {
        m_grid.at(x, y) = height;
    };
    uint64_t& at(uint16_t x, uint16_t y) {
        return m_grid.at(x, y);
    }
    uint64_t operator[](uint16_t ind) const {
        return m_grid[ind];
    }
    void SetGrains(uint16_t x, uint16_t y, uint64_t height) const;
    void ToppleOne(uint16_t x, uint16_t y, bool fl);
    bool ToppleSome();
    void ToppleAll() ;
    void Print() const;
};