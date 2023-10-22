//
// Created by jaime-vaquero on 2/10/23.
//

#ifndef ARQUIPROYECTO_BLOCK_HPP
#define ARQUIPROYECTO_BLOCK_HPP

#include <vector>
#include <iostream>
#include "particle.hpp"

class Block {
  private:
    bool checked;
    int m_i;
    int m_j;
    int m_k;
    std::vector<Particle> particles;
  public:
    Block(int i, int j, int k);
    void Add_particle(Particle &particle);
    bool Exists_block(int px, int py, int pz);
    int get_i();
    int get_j();
    int get_k();
    int get_particles_length();
    Particle get_particle(int pos);
    bool needs_reset(int index, std::array<double, 3> m_block_size);
    void initialize_acc_dens_block(std::array<double, 3> g);
    std::vector<double> get_particle_position(int pos);
    double update_particle_density(double aumento, int pos);
    void set_particle_density(double density, int pos);
    double get_particle_density(int pos);
    void update_particle_acceleration(std::array<double, 3> aumento, int pos);
    Particle pop_particle(int index);
    void set_checked(bool valor);
    bool get_checked();
};

#endif //ARQUIPROYECTO_BLOCK_HPP