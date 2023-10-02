//
// Created by jaime-vaquero on 2/10/23.
//

#ifndef ARQUIPROYECTO_GRID_HPP
#define ARQUIPROYECTO_GRID_HPP

#include <vector>
#include "block.hpp"
#include <iostream>

class Grid {
private:
    int m_nx;
    int m_ny;
    int m_nz;
    std::vector<Block>bloques;
public:
    Grid(int nx, int ny, int nz);
    void Generate_blocks(int nx, int ny, int nz);
};

/*
 *  std::vector<float> b_max{0.065, 0.1, 0.065};
    std::vector<float> b_min{-0.065, -0.08, -0.065};
 */

#endif //ARQUIPROYECTO_GRID_HPP
