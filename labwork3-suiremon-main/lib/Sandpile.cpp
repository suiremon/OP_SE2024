#include "Sandpile.h"

void TopExpand(Grid& g, uint16_t new_m_x, uint64_t new_grid[]) {
    for (uint16_t y = 0; y < g.m_y; y++) {
        for (uint16_t x = 1; x < new_m_x; x++) {
            new_grid[x * g.m_y + y] = g.at(x - 1, y);
        }
    }
}

void BotExpand(Grid& g, uint64_t new_grid[]) {
    for (uint16_t y = 0; y < g.m_y; y++) {
        for (uint16_t x = 0; x < g.m_x; x++) {
            new_grid[x * g.m_y + y] = g.at(x, y);
        }
    }
}

void RightExpand(Grid& g, uint16_t new_m_y, uint64_t new_grid[]) {
    for (uint16_t y = 1; y < new_m_y; y++) {
        for (uint16_t x = 0; x < g.m_x; x++) {
            new_grid[x * new_m_y + y] = g.at(x, y - 1);
        }
    }
}

void LeftExpand(Grid& g, uint16_t new_m_y, uint64_t new_grid[]) {
    for (uint16_t y = 0; y < g.m_y; y++) {
        for (uint16_t x = 0; x < g.m_x; x++) {
            new_grid[x * new_m_y + y] = g.at(x, y);
        }
    }
}

void Grid::ExpandGrid(Grid& g, uint16_t expand_size, int8_t type) {
    if (type == 1 || type == 2) {
        uint16_t new_m_x = g.m_x + expand_size;
        auto *new_grid = new uint64_t[new_m_x * g.m_y]{0};
        if (type == 1) {
            TopExpand(g, new_m_x, new_grid);
        } else {
            BotExpand(g, new_grid);
        }
        delete[] g.grid;
        g.grid = new_grid;
        g.m_x = new_m_x;
    } else if (type == 3 || type == 4) {
        uint16_t new_m_y = g.m_y + expand_size;
        auto *new_grid = new uint64_t[g.m_x * new_m_y]{0};
        if (type == 3) {
            RightExpand(g, new_m_y, new_grid);
        } else {
            LeftExpand(g, new_m_y, new_grid);
        }
        delete[] g.grid;
        g.grid = new_grid;
        g.m_y = new_m_y;
    } else if (type == 5) {
        uint16_t new_m_y = g.m_y + expand_size;
        uint16_t new_m_x = g.m_x + expand_size;
        auto *new_grid = new uint64_t[new_m_x * new_m_y]{0};
        TopExpand(g, new_m_x, new_grid);
        delete[] g.grid;
        g.grid = new_grid;
        g.m_x = new_m_x;
        g.m_y = new_m_y;
    }
}

void Sandpile::ToppleOne(uint16_t x, uint16_t y, bool fl) {
    if (m_grid.at(x, y) > 3) {
        int shiftX = 0;
        int shiftY = 0;
        if (x == 0 && y == 0) { //right top corner
            shiftX = 1;
            shiftY = 1;
            Grid::ExpandGrid(m_grid, 1, 1);
            Grid::ExpandGrid(m_grid, 1, 3);
        } else if (x == 0 && y == m_grid.m_y - 1) { //left top corner
            shiftX = 1;
            Grid::ExpandGrid(m_grid, 1, 1);
            Grid::ExpandGrid(m_grid, 1, 4);
        } else if (x == m_grid.m_x - 1 && y == 0) { //right bot corner
            shiftY = 1;
            Grid::ExpandGrid(m_grid, 1, 2);
            Grid::ExpandGrid(m_grid, 1, 3);
        } else if (x == m_grid.m_x - 1 && y == m_grid.m_y - 1) { //left bot corner
            Grid::ExpandGrid(m_grid, 1, 2);
            Grid::ExpandGrid(m_grid, 1, 4);
        } else if (x == 0) { //top
            shiftX = 1;
            Grid::ExpandGrid(m_grid, 1, 1);
        } else if (x == m_grid.m_x - 1) { //bot
            Grid::ExpandGrid(m_grid, 1, 2);
        } else if (y == 0 && x > 0 && x < m_grid.m_x) { //right
            shiftY = 1;
            Grid::ExpandGrid(m_grid, 1, 3);
        } else if (y == m_grid.m_y - 1) { //left
            Grid::ExpandGrid(m_grid, 1, 4);
        }
        uint64_t tmp;
        if (!fl) {
            tmp = m_grid.at(x + shiftX, y + shiftY) / 4;
            m_grid.at(x + shiftX, y + shiftY) %= 4;
        } else {
            tmp = 1;
            m_grid.at(x + shiftX, y + shiftY) -= 4;
        }
        m_grid.at(x + shiftX + 1, y + shiftY) += tmp;
        m_grid.at(x + shiftX - 1, y + shiftY) += tmp;
        m_grid.at(x + shiftX, y + 1 + shiftY) += tmp;
        m_grid.at(x + shiftX, y - 1 + shiftY) += tmp;
    }
}

bool Sandpile::ToppleSome() {
    bool iterTopple[m_grid.m_x * m_grid.m_y];
    bool isStable = true;
    for (int x = 0; x < m_grid.m_x; ++x) {
        for (int y = 0; y < m_grid.m_y; ++y) {
            iterTopple[x * m_grid.m_y + y] = (m_grid.at(x, y) > 3);
        }
    }
    for (int x = 0; x < m_grid.m_x; ++x) {
        for (int y = 0; y < m_grid.m_y; ++y) {
            if (iterTopple[x * m_grid.m_y + y]) {
                ToppleOne(x, y, true);
                isStable = false;
            }
        }
    }
    return isStable;
}

void Sandpile::ToppleAll() {
    bool isStable = true;
    bool iterTopple[m_grid.m_x * m_grid.m_y];
    while (isStable) {
        isStable = false;
        for (int x = 0; x < m_grid.m_x; ++x) {
            for (int y = 0; y < m_grid.m_y; ++y) {
                iterTopple[x * m_grid.m_y + y] = (m_grid.at(x, y) > 3);
            }
        }
        for (int x = 0; x < m_grid.m_x; ++x) {
            for (int y = 0; y < m_grid.m_y; ++y) {
                if (iterTopple[x * m_grid.m_y + y]) {
                    ToppleOne(x, y, false);
                    isStable = true;
                }
            }
        }
    }
}

void Sandpile::SetGrains(uint16_t x, uint16_t y, uint64_t height) const {
    m_grid.at(x, y) += height;
}

void Sandpile::Print() const {
    for (int i = 0; i < m_grid.m_x; ++i) {
        for (int j = 0; j < m_grid.m_y; ++j) {
            std::cout << m_grid.at(i, j) << " ";
        }
        std::cout << "\n";
    }
}
