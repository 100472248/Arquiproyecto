#ifndef ARQUIPROYECTO_GRID_HPP
#define ARQUIPROYECTO_GRID_HPP

#include <vector>
#include "block.hpp"
#include "particle.hpp"
#include <iostream>
#include <array>

std::array<double,3> const BMAX = {0.065, 0.1, 0.065};
std::array<double,3> const BMIN = {-0.065, -0.08, -0.065};
std::array<double, 3>const G = {0, -9.8, 0};

class Grid {
  private:
    int m_nx;
    int m_ny;
    int m_nz;
    std::array<double, 3> m_block_size;
    std::vector<Block>bloques;
  public:
    Grid(int nx, int ny, int nz);
    void set_block_size(std::array<double, 3> block_size);
    std::array<double, 3> get_block_size();
    void Generate_blocks(int nx, int ny, int nz);
    int find_block(int px, int py, int pz);
    void add_block_particle(int i, Particle &particle);
    int find_block_2(int px, int py, int pz);
    void initialize_acc_dens();
    void reposition_particles();
    void calc_density(double ppm);
    void calc_acceleration(double ppm);
    void simulation(int iteraciones, double ppm);
    std::vector<int> find_adjacent_blocks (int px, int py, int pz);
    void uncheck();
    std::array<double, 3> get_grid_size();
    std::vector<Particle> reordenar_particulas();
    void print_particles();

};

#endif //ARQUIPROYECTO_GRID_HPP