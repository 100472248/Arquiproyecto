//
// Created by jaime-vaquero on 2/10/23.
//
#include "grid.hpp"
#include "block.hpp"
#include <vector>
#include <iostream>

Grid::Grid(int nx, int ny, int nz) {
  m_nx = nx;
  m_ny = ny;
  m_nz = nz;
  Generate_blocks(nx, ny, nz);
}

void Grid::Generate_blocks(int nx, int ny, int nz) {
  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++ ) {
      for (int k = 0; k < nz; k++) {
        Block bloque(i,j,k);
        bloques.push_back(bloque);
      }
    }
  }
}

Block Grid:: Find_block(int px, int py, int pz) {
    // Encuentra si dadas unas constantes existe un bloque
    if (BMIN[0] <= px && px <= BMAX[0]) {
        if (BMIN[1] <= py && py <= BMAX[1]) {
            if (BMIN[2] <= pz && pz <= BMAX[2]) {
                for (int i = 0; i < isdigit(bloques.size()); i++) {
                    Block bloque = bloques[i];
                    if (bloque.Exists_block(px, py, pz)) {
                        return bloque;
                    }
                }

            }
        }
    }
    std:: cerr << "Error, cifras incorrectas.";}
