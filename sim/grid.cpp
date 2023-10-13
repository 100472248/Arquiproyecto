//
// Created by jaime-vaquero on 2/10/23.
//
#include "grid.hpp"
#include "block.hpp"
#include "particle.hpp"
#include "math.hpp"
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
  return px * (m_ny*m_nz) + py * (m_nz) + pz;
}

int Grid::find_block_2(int px, int py, int pz) {
    if (px < 0 || px >= m_nx) {
        return -1; }
    if (py < 0 || py >= m_ny) {
        return -1; }
    if (pz < 0 || pz >= m_nz) {
        return -1; }
    return px * (m_ny*m_nz) + py * (m_nz) + pz;
}

void Grid::add_block_particle(int i, Particle &particle){
    bloques[i].Add_particle(particle);
    std::array<int, 3> bloque = {bloques[i].get_i(), bloques[i].get_j(), bloques[i].get_k()};
    particle.Set_bloque(bloque);
}

std::vector<int> Grid::find_adjacent_blocks(int px, int py, int pz) {
    std::vector<int> adyacentes;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            for (int k = -1; k <= 1; k++) {
                int posicion = find_block_2(px + i, py + j, pz + k);
                if (posicion != -1) {
                    adyacentes.push_back(posicion);
                }
            }
        }
    }
    return adyacentes;
}

void Grid::reposition_particles() {
    for (int i = 0; i < static_cast<int>(bloques.size()); i++) {
        int particles = bloques[i].get_particles_length();
        for (int j = 0; j <particles; j++) {
            if (bloques[i].needs_reset(j, get_block_size())) {
                Particle particula = bloques[i].pop_particle(j);
                std::vector<double> position_particle = particula.get_position();
                std::array<int, 3> pos_particle = posicion_particula(position_particle[0], position_particle[1],
                                                                      position_particle[2], get_block_size());
                int identificador = find_block(pos_particle[0], pos_particle[1], pos_particle[2]);
                add_block_particle(identificador, particula);
            };
        }
    }
}

void Grid::calc_speedup() {

}

void Grid::simulation() {
    reposition_particles();

}



std::array<double, 3> Grid::get_block_size() {
    return m_block_size;
}

void Grid::set_block_size(std::array<double, 3> block_size) {
    m_block_size = block_size;

}


/*
bool Grid::is_adjacent(int b1_i, int b1_j, int b1_k, int b2_i, int b2_j, int b2_k) {
    return ((abs(b2_i - b1_i) == 1 || abs(b2_i - b1_i) == 0) && (abs(b2_j - b1_j) == 1 || abs(b2_j - b1_j) == 0)
    && (abs(b2_k - b1_k) == 1 || abs(b2_k - b1_k) == 0));
}
*/