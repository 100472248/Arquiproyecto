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
}

#endif //ARQUIPROYECTO_BLOCK_HPP