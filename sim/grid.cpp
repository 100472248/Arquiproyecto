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

Grid::Grid(int nx, int ny, int nz) : m_nx(nx), m_ny(ny), m_nz(nz) {



  Generate_blocks(nx, ny, nz);
}

void Grid::Generate_blocks(int nx, int ny, int nz) {
  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++ ) {
      for (int k = 0; k < nz; k++) {
        Block const bloque(i,j,k);
        bloques.push_back(bloque);
      }
    }
  }
}

int Grid::find_block(int px, int py, int pz) const {
  // Encuentra si dadas unas constantes existe un bloque
  if (px < 0) { px = 0; }
  else if (px >= m_nx) { px = m_nx-1; }
  if (py < 0) { py = 0; }
  else if (py >= m_ny) { py = m_ny-1; }
  if (pz < 0) { pz = 0; }
  else if (pz >= m_nz) { pz = m_nz-1; }
  return px * (m_ny*m_nz) + py * (m_nz) + pz;
}

int Grid::find_block_2(int px, int py, int pz) const {
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
    std::array<int, 3> const bloque = {bloques[i].get_i(), bloques[i].get_j(), bloques[i].get_k()};
    particle.Set_bloque(bloque);
}

std::vector<int> Grid::find_adjacent_blocks(int px, int py, int pz) const {
    std::vector<int> adyacentes;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            for (int k = -1; k <= 1; k++) {
                // std::cout << "Coordenadas a buscar" << px+i << " " << py+j << " " << pz+k << std::endl;
                int const posicion = find_block_2(px + i, py + j, pz + k);
                // std::cout << "Posicion encontrada: " << posicion << std::endl;
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
        int const particles = bloques[i].get_particles_length();
        for (int j = 0; j <particles; j++) {
            if (bloques[i].needs_reset(j, get_block_size())) {
                Particle particula = bloques[i].pop_particle(j);
                std::vector<double> position_particle = particula.get_position();
                std::array<int, 3> pos_particle = posicion_particula(position_particle[0], position_particle[1],
                                                                      position_particle[2], get_block_size());
                int const identificador = find_block(pos_particle[0], pos_particle[1], pos_particle[2]);
                add_block_particle(identificador, particula);
            }
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
        // std::cout << "Soy el bloque: " << i << " " << "mis coordenadas son: " << bloques[i].get_i() << " " << bloques[i].get_j() << " " << bloques[i].get_k() << std::endl;
        std::vector<int> const contiguos = find_adjacent_blocks(bloques[i].get_i(), bloques[i].get_j(),
                                                          bloques[i].get_k());
        int const longitud_particulas_bloque = bloques[i].get_particles_length();
        // std::cout << "Soy el bloque: " << i << " " << "mi check es: " << bloques[i].get_checked() << std::endl;
        //std::cout << "Tengo un total de: " << longitud_particulas_bloque << " particulas" << std::endl;
        //std::cout << "Las posiciones de mis bloques contiguos son: " << std::endl;
        //for (int posicion_contiguo2: contiguos) {
            //std::cout << "posicion_contiguo: " << posicion_contiguo2 << std::endl;
        //}
        for (int const posicion_contiguo: contiguos) {
            //std::cout << "Soy el contiguo que tiene posicion: " << posicion_contiguo << " " << "mi check es: " << bloques[posicion_contiguo].get_checked() << std::endl;
            if (bloques[posicion_contiguo].get_checked()) {
                continue;
            }
            int const longitud_particulas_contiguo = bloques[posicion_contiguo].get_particles_length();
            //std::cout << "Tengo un total de: " << longitud_particulas_contiguo << " particulas" << std::endl;
            for (int b = 0; b < longitud_particulas_bloque; b++) {
                std::vector<double> const posicion_particula_bloque = bloques[i].get_particle_position(b);
                //std::cout << "Soy la particula: " << b << " del bloque base" << std::endl;
                for (int c = 0; c < longitud_particulas_contiguo; c++) {
                    //std::cout << "Soy la particula: " << c << " del bloque contiguo" << std::endl;
                    if (posicion_contiguo == i && c <= b ) {
                        //std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
                        continue;}
                    std::vector<double> const posicion_particula_contiguo = bloques[posicion_contiguo].get_particle_position(c);
                    double const aumento = increase_density(ppm, posicion_particula_bloque, posicion_particula_contiguo);
                    bloques[i].update_particle_density(aumento, b);
                    bloques[posicion_contiguo].update_particle_density(aumento, c);
                }
            }
        }
        bloques[i].set_checked(true);
    }
    uncheck();
}

void Grid::calc_density_2(double ppm) {
    for (int i = 0; i < static_cast<int>(bloques.size()); i++) {
        int const longitud_particulas_bloque = bloques[i].get_particles_length();
        for (int b = 0; b < longitud_particulas_bloque; b++) {
            double const density = bloques[i].get_particle_density(b);
            //std::cout << "densidad: " << density << std::endl;
            double const transformacion_bloque = transform_density(ppm, density);
            //std::cout << "transformacion: " << transformacion_bloque << std::endl;
            bloques[i].set_particle_density(transformacion_bloque, b);
        }
    }
}
void Grid::calc_acceleration(double ppm) {
    for (int i = 0; i < static_cast<int>(bloques.size()); i++) {
        std::vector<int> const contiguos = find_adjacent_blocks(bloques[i].get_i(), bloques[i].get_j(),
                                                          bloques[i].get_k());
        for (int const posicion_contiguo: contiguos) {
            if (bloques[posicion_contiguo].get_checked()) {
                continue;
            }
            int const longitud_particulas_bloque = bloques[i].get_particles_length();
            int const longitud_particulas_contiguo = bloques[posicion_contiguo].get_particles_length();
            for (int b = 0; b < longitud_particulas_bloque; b++) {
                std::vector<double> const posicion_particula_bloque = bloques[i].get_particle_position(b);
                for (int c = 0; c < longitud_particulas_contiguo; c++) {
                    if (posicion_contiguo == i && c <= b) {continue;}
                    std::vector<double> const posicion_particula_contiguo = bloques[posicion_contiguo].get_particle_position(c);
                    // estas particulas son copias, no se estan actualizando
                    Particle const particula_bloque = bloques[i].get_particle(b);
                    Particle const particula_contiguo = bloques[posicion_contiguo].get_particle(c);
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
    // std:: cout << "Hola" << '\n';
    for (int i = 0; i < iteraciones; i++) {
        uncheck();
        reposition_particles(); // 4.3.1
        initialize_acc_dens(); // 4.3.1.1
        // std:: cout << "Hola" << i << '\n';
        calc_density(ppm); // 4.3.1.2 - 4.3.1.3
        calc_density_2(ppm);
        print_particles();
        calc_acceleration(ppm); // 4.3.1.4
    }
}

std::array<double, 3> Grid::get_grid_size() const{
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
            Particle const particula  =bloques[i].get_particle(j);
            particulas_reordenadas.push_back(particula);
        }
    }
    for (int k = 0; k < static_cast<int>(particulas_reordenadas.size()); k++) {
        for (int var_l = 0; var_l < (static_cast<int>(particulas_reordenadas.size()) - k - 1); var_l++) {
            Particle const particula = particulas_reordenadas[var_l];
            Particle const siguiente = particulas_reordenadas[var_l + 1];
            if (particula.get_id() > siguiente.get_id()) {
                Particle const temp = particulas_reordenadas[var_l];
                particulas_reordenadas[var_l] = particulas_reordenadas[var_l + 1];
                particulas_reordenadas[var_l + 1] = temp;
            }
        }
    }
    return particulas_reordenadas;
}

void Grid::print_particles() {
    std::vector<Particle> const particulas = reordenar_particulas();
    int contador = 0;
    for (Particle particula: particulas) {
        if (contador > MIL) {break;}
        std::array<double, 3>  acceleration = particula.get_acceleration();
        std::vector<double> position = particula.get_position();
        std::cout << "Particle ID: " << particula.get_id() << '\n';
        std::cout << "Particle position: " << position[0] << " " << position[1] << " " << position[2] << '\n';
        std::cout << "Particle acceleration: " << acceleration[0] << " " << acceleration[1] << " " << acceleration[2] << '\n';
        std::cout << "Particle density: " << particula.get_density() << '\n';
        contador++;
    }

}




/*
bool Grid::is_adjacent(int b1_i, int b1_j, int b1_k, int b2_i, int b2_j, int b2_k) {
    return ((abs(b2_i - b1_i) == 1 || abs(b2_i - b1_i) == 0) && (abs(b2_j - b1_j) == 1 || abs(b2_j - b1_j) == 0)
    && (abs(b2_k - b1_k) == 1 || abs(b2_k - b1_k) == 0));
}
*/