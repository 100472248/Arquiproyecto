//
// Created by jaime-vaquero on 2/10/23.
//
#include "block.hpp"
#include "particle.hpp"
#include "files.hpp"
#include "math.hpp"

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

int Block::get_i(){
    return m_i;
}

int Block::get_j(){
    return m_i;
}

int Block::get_k(){
    return m_i;
}

int Block::get_particles_length() {
    return static_cast<int>(particles.size());
}

bool Block::needs_reset(int index, std::array<double, 3> block_size) {
    std::array<int, 3> bloque = particles[index].get_bloque();
    std::vector<double> posicion = particles[index].get_position();
    std:: array<int, 3> pos_particle = posicion_particula(posicion[0], posicion[1], posicion[2], block_size);
    if (bloque[0] != pos_particle[0] || bloque[1] != pos_particle[1] || bloque[2] != pos_particle[2]) {
        return true;
    }
    return false;

}

Particle Block::pop_particle(int index) {
    Particle particle =  particles[index];
    particles.erase(particles.begin() + index);
    return particle;
}



