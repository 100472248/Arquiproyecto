//
// Created by jaime-vaquero on 2/10/23.
//
#include "grid.hpp"
#include "block.hpp"
#include "particle.hpp"
#include <vector>
#include <iostream>
#include <array>

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

int Grid::find_block(int px, int py, int pz) {
  // Encuentra si dadas unas constantes existe un bloque
  if (px < 0) { px = 0; }
  else if (px >= m_nx) { px = m_nx-1; }
  if (py < 0) { py = 0; }
  else if (py >= m_ny) { py = m_ny-1; }
  if (pz < 0) { pz = 0; }
  else if (pz >= m_nz) { pz = m_nz-1; }
  for (int i = 0; i < static_cast<int>(bloques.size()); i++) {
    Block bloque = bloques[i];
    if (bloque.Exists_block(px, py, pz)) {
        return i; }
  }
    return -1;
}


void Grid::add_block_particle(int i, Particle &particle){
    bloques[i].Add_particle(particle);
}

std::vector<Block> Grid::find_adjacent_blocks(int i, int j, int k) {
    std::vector<Block> adjacents;
    for (int it = 0; it < static_cast<int>(bloques.size()); it++){
      Block bloque = bloques[it];
      int b_i, b_j, b_k;
      b_i = bloque.get_i();
      b_j = bloque.get_j();
      b_k = bloque.get_k();
      if ((abs(b_i - i) == 1 || abs(b_i) == 0) && (abs(b_j - j) == 1 || abs(b_j) == 0) && (abs(b_k - k) == 1 || abs(b_k) == 0)){
        adjacents.push_back(bloque);
      }
    }
    return adjacents;
}