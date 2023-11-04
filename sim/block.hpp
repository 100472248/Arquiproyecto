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
    bool Exists_block(int px, int py, int pz) const;
    int get_i() const;
    int get_j() const;
    int get_k() const;
    int get_particles_length();
    Particle get_particle(int pos);
    bool needs_reset(int index, std::array<double, 3> m_block_size);
    void initialize_acc_dens_block(std::array<double, 3> g);
    std::vector<double> get_particle_position(int pos);
    void update_particle_density(double aumento, int pos);
    void set_particle_density(double density, int pos);
    void set_particle_position(std::vector<double> position, int pos);
    void set_particle_speed(std::vector<double> speed, int pos);
    void set_particle_gradient(std::vector<double> gradient, int pos);
    double get_particle_density(int pos);
    std::vector<double> get_particle_gradient(int pos);
    std::vector<double> get_particle_speed(int pos);
    std::array<double, 3> get_particle_acc(int pos);
    void update_particle_acceleration(std::array<double, 3> aumento, int pos);
    void pop_particle(int index);
    void set_checked(bool valor);
    bool get_checked() const;
    void particle_collisions(int pos, int tipo, int coordenada);
    int get_particle_id(int pos);
};

#endif //ARQUIPROYECTO_BLOCK_HPP