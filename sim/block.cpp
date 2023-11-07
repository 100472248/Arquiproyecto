//
// Created by jaime-vaquero on 2/10/23.
//
#include "block.hpp"

#include <utility>
#include "particle.hpp"
#include "math.hpp"

Block::Block(int i, int j, int k) : m_i(i), m_j(j), m_k(k) {
}

void Block::Add_particle(Particle &particle) {
  particles.push_back(particle);
}

bool Block:: Exists_block(int px, int py, int pz) const{
    return m_i == px && m_j == py && m_k == pz;
}

int Block::get_i() const{
    return m_i;
}

int Block::get_j() const{
    return m_j;
}

int Block::get_k() const{
    return m_k;
}

int Block::get_particles_length() {
    return static_cast<int>(particles.size());
}

bool Block::needs_reset(int index, std::array<double, 3> block_size) {
    //std::cout << "index " << index << std::endl;
    //std::cout << "id " << particles[index].get_id() << std::endl;
    std::array<int, 3> bloque = particles[index].get_bloque();
    //std::cout << "Bloque antiguo: " << bloque[0] << " " << bloque[1] << " " << bloque[2] << std::endl;
    std::vector<double> posicion = particles[index].get_position();
    //std::cout << "Posicion: " << posicion[0] << " " << posicion[1] << " " << posicion[2] << std::endl;
    std::array<int, 3> pos_particle = posicion_particula(posicion[0], posicion[1], posicion[2], block_size);
    //std::cout << "Bloque nuevo: " << pos_particle[0] << " " << pos_particle[1] << " " << pos_particle[2] << std::endl;
    return bloque[0] != pos_particle[0] || bloque[1] != pos_particle[1] || bloque[2] != pos_particle[2];
}

void Block::pop_particle(int index) {
    particles.erase(particles.begin() + index);
}

void Block::initialize_acc_dens_block(std::array<double, 3> g) {
    for (int i = 0; i < static_cast<int>(particles.size()); i++) {
        particles[i].Set_acceleration(g);
        particles[i].Set_density(0);
    }

}

std::vector<double> Block::get_particle_position(int pos) {
    return particles[pos].get_position();
}

void Block::update_particle_density(double aumento, int pos) {
    particles[pos].update_density(aumento);
}

void Block::set_particle_density(double density, int pos) {
    particles[pos].Set_density(density);
}

void Block::set_checked(bool valor) {
    checked = valor;

}

bool Block::get_checked() const {
    return checked;
}

Particle Block::get_particle(int pos) {
    Particle particula = particles[pos];
    return particula;
}

void Block::update_particle_acceleration(std::array<double, 3> aumento, int pos) {
    particles[pos].update_acceleration(aumento);

}

double Block::get_particle_density(int pos) {
    return particles[pos].get_density();
}

std::vector<double> Block::get_particle_gradient(int pos) {
    return particles[pos].get_gradient();
}

std::vector<double> Block::get_particle_speed(int pos) {
    return particles[pos].get_speed();
}

std::array<double, 3> Block::get_particle_acc(int pos) {
    return particles[pos].get_acceleration();
}

void Block::set_particle_position(std::vector<double> position, int pos) {
    particles[pos].set_position(std::move(position));
}

void Block::set_particle_speed(std::vector<double> speed, int pos) {
    particles[pos].set_speed(std::move(speed));
}

void Block::set_particle_gradient(std::vector<double> gradient, int pos) {
    particles[pos].set_gradient(std::move(gradient));
}

void Block::particle_collisions(int pos, int tipo, int coordenada) {
    if (coordenada == 0) {
        if (tipo == 0) {
            collisions_x(particles[pos], tipo);
        }
        else if (tipo == 1) {
            collisions_x(particles[pos], tipo);
        }
    }
    else if (coordenada == 1) {
        if (tipo == 0) {
            collisions_y(particles[pos], tipo);
        }
        else if (tipo == 1) {
            collisions_y(particles[pos], tipo);
        }
    }
    else if (coordenada == 2) {
        if (tipo == 0) {
            collisions_z(particles[pos], tipo);
        }
        else if (tipo == 1) {
            collisions_z(particles[pos], tipo);
        }
    }
}

int Block::get_particle_id(int pos) {
    return particles[pos].get_id();
}
