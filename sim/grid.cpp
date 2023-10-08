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
  for (int i = 0; i <= nx; i++) {
    for (int j = 0; j <= ny; j++ ) {
      for (int k = 0; k <= nz; k++) {
        Block bloque(i,j,k);
        bloques.push_back(bloque);
      }
    }
  }
}

int Grid::find_block(int px, int py, int pz) {
  // Encuentra si dadas unas constantes existe un bloque
  if (px < 0) { px = 0; }
  else if (px > m_nx) { px = m_nx; }
  if (py < 0) { py = 0; }
  else if (py > m_ny) { py = m_ny; }
  if (pz < 0) { pz = 0; }
  else if (pz > m_nz) { pz = m_nz; }
  for (int i = 0; i < static_cast<int>(bloques.size()); i++) {
    Block bloque = bloques[i];
    std::cout << "i en funcion: " << i << std::endl;
    if (bloque.Exists_block(px, py, pz)) { return i; }
  }
  std::cout << "Hola Ivo: " << static_cast<int>(bloques.size()) << std::endl;
  std::cout << "mn_x: " << m_nx << " mn_y: " << m_ny << " mn_z: " << m_nz << std::endl;
  std::cout << "px: " << px << "py: " << py << "pz: " << pz << std::endl;
  return -1;
}


void Grid:: add_block_particle(int i, Particle &particle){
    std::cout << "Okey Lets go add block !" << std::endl;
    bloques[i].Add_particle(particle);
    std::cout << "Okey Lets go add block, sale?!" << std::endl;
}