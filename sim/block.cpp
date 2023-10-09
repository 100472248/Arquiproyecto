//
// Created by jaime-vaquero on 2/10/23.
//
#include "block.hpp"
#include "particle.hpp"
#include "files.hpp"

Block::Block(int i, int j, int k) {
  m_i = i;
  m_j = j;
  m_k = k;
}

void Block::Add_particle(Particle &particle) {
  particles.push_back(particle);
}

bool Block:: Exists_block(int px, int py, int pz){
    if (m_i == px && m_j == py && m_k == pz){
        return true;
    }
    return false;
}



