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

void Grid::initialize_acc_dens() {
    for (int i = 0; i < static_cast<int>(bloques.size()); i++) {
        bloques[i].initialize_acc_dens_block(G);
    }
}

void Grid::calc_density(double ppm) {
    for (int i = 0; i < static_cast<int>(bloques.size()); i++) {
        // std::cout << "iteracion: " << i << std::endl;
        // std::cout << "CHECK: " << bloques[i].get_checked() << std::endl;
        std::vector<int> contiguos = find_adjacent_blocks(bloques[i].get_i(), bloques[i].get_j(),
                                                          bloques[i].get_k());
        for (int posicion_contiguo: contiguos) {
            if (bloques[posicion_contiguo].get_checked()) {
                continue;
            }
            // std::cout << "2 " << std::endl;
            int longitud_particulas_bloque = bloques[i].get_particles_length();
            int longitud_particulas_contiguo = bloques[posicion_contiguo].get_particles_length();
            for (int b = 0; b < longitud_particulas_bloque; b++) {
                // std::cout << "3 " << std::endl;
                std::vector<double> posicion_particula_bloque = bloques[i].get_particle_position(b);
                for (int c = 0; c < longitud_particulas_contiguo; c++) {
                    if (posicion_contiguo == i && b == c) {continue;}
                    std::vector<double> posicion_particula_contiguo = bloques[posicion_contiguo].get_particle_position(c);
                    double aumento = increase_density(ppm, posicion_particula_bloque, posicion_particula_contiguo);
                    double densidad_bloque = bloques[i].update_particle_density(aumento, b);
                    double densidad_contiguo = bloques[posicion_contiguo].update_particle_density(aumento, c);
                    double transformacion_bloque = transform_density(ppm, densidad_bloque);
                    double transformacion_contiguo = transform_density(ppm, densidad_contiguo);
                    // std::cout << "transformacion_bloque: " << transformacion_bloque << std::endl;
                    bloques[i].set_particle_density(transformacion_bloque, b);
                    //std::cout << "densidad: " << bloques[i].get_particle_density(b) << std::endl;
                    bloques[posicion_contiguo].set_particle_density(transformacion_contiguo, c);
                }
            }
        }
        bloques[i].set_checked(true);
    }
    uncheck();

}

void Grid::calc_acceleration(double ppm) {
    for (int i = 0; i < static_cast<int>(bloques.size()); i++) {
        std::vector<int> contiguos = find_adjacent_blocks(bloques[i].get_i(), bloques[i].get_j(),
                                                          bloques[i].get_k());
        for (int posicion_contiguo: contiguos) {
            if (bloques[posicion_contiguo].get_checked()) {
                continue;
            }
            int longitud_particulas_bloque = bloques[i].get_particles_length();
            int longitud_particulas_contiguo = bloques[posicion_contiguo].get_particles_length();
            for (int b = 0; b < longitud_particulas_bloque; b++) {
                std::vector<double> posicion_particula_bloque = bloques[i].get_particle_position(b);
                for (int c = 0; c < longitud_particulas_contiguo; c++) {
                    if (posicion_contiguo == i && b == c) {continue;}
                    std::vector<double> posicion_particula_contiguo = bloques[posicion_contiguo].get_particle_position(c);
                    // estas particulas son copias, no se estan actualizando
                    Particle particula_bloque = bloques[i].get_particle(b);
                    Particle particula_contiguo = bloques[posicion_contiguo].get_particle(c);
                    std::array<double, 3> aumento = increase_accerelation(ppm, particula_bloque, particula_contiguo);
                    bloques[i].update_particle_acceleration(aumento, b);
                    aumento = {-aumento[0], -aumento[1], -aumento[2]};
                    bloques[posicion_contiguo].update_particle_acceleration(aumento, c);
                }
            }
        }
        bloques[i].set_checked(true);
    }
    uncheck();
}

void Grid::simulation(int iteraciones, double ppm) {
    for (int i = 0; i < iteraciones; i++) {
        uncheck();
        reposition_particles(); // 4.3.1
        if (i == 0) {
            initialize_acc_dens(); // 4.3.1.1
        }
        calc_density(ppm); // 4.3.1.2 - 4.3.1.3
        //print_particles();
        calc_acceleration(ppm); // 4.3.1.4
    }
}

std::array<double, 3> Grid::get_grid_size(){
    std::array<double, 3> grid_size;
    grid_size[0] = m_nx;
    grid_size[1] = m_ny;
    grid_size[2] = m_nz;
    return grid_size;
}

std::array<double, 3> Grid::get_block_size() {
    return m_block_size;
}

void Grid::set_block_size(std::array<double, 3> block_size) {
    m_block_size = block_size;

}

void Grid::uncheck() {
    for (int i  = 0; i < static_cast<int>(bloques.size()); i++) {
        bloques[i].set_checked(false);
    }
}

std::vector<Particle> Grid::reordenar_particulas() {
    std::vector<Particle> particulas_reordenadas;
    for (int i = 0; i < static_cast<int>(bloques.size()); i++) {
        for (int j = 0; j < bloques[i].get_particles_length(); j++) {
            Particle particula  =bloques[i].get_particle(j);
            particulas_reordenadas.push_back(particula);
        }
    }
    for (int k = 0; k < static_cast<int>(particulas_reordenadas.size()); k++) {
        for (int l = 0; l < (static_cast<int>(particulas_reordenadas.size()) - k -1); l++) {
            Particle particula = particulas_reordenadas[l];
            Particle siguiente = particulas_reordenadas[l+1];
            if (particula.get_id() > siguiente.get_id()) {
                Particle temp = particulas_reordenadas[l];
                particulas_reordenadas[l] = particulas_reordenadas[l+1];
                particulas_reordenadas[l+1] = temp;
            }
        }
    }
    return particulas_reordenadas;
}

void Grid::print_particles() {
    std::vector<Particle> particulas = reordenar_particulas();
    for (Particle particula: particulas) {
        std::array<double, 3>  acceleration = particula.get_acceleration();
        std::cout << "Particle ID: " << particula.get_id() << std::endl;
        std::cout << "Particle acceleration: " << acceleration[0] << " " << acceleration[1] << " " << acceleration[2] << std::endl;
        std::cout << "Particle density: " << particula.get_density() << std::endl;
    }

}




/*
bool Grid::is_adjacent(int b1_i, int b1_j, int b1_k, int b2_i, int b2_j, int b2_k) {
    return ((abs(b2_i - b1_i) == 1 || abs(b2_i - b1_i) == 0) && (abs(b2_j - b1_j) == 1 || abs(b2_j - b1_j) == 0)
    && (abs(b2_k - b1_k) == 1 || abs(b2_k - b1_k) == 0));
}
*/