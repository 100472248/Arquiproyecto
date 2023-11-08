//
// Created by jaime-vaquero on 2/10/23.
//
#include "grid.hpp"
#include "block.hpp"
#include "particle.hpp"
#include "math.hpp"
#include <algorithm>

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
    std::array<int, 3>  const bloque = {bloques[i].get_i(), bloques[i].get_j(), bloques[i].get_k()};
    //std::cout << "Bloque: " << bloque[0] << " " << bloque[1] << " " << bloque[2] << std::endl;
    particle.Set_bloque(bloque);
    bloques[i].Add_particle(particle);
    //std::cout << "Bloque set: " << bloque[0] << " " << bloque[1] << " " << bloque[2] << std::endl;
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
    for (auto & bloque : bloques) {
        std::vector<int> borrar;
        int const particles = bloque.get_particles_length();
        // std::cout << "Soy el bloque " << i << "tengo estas particulas: " << particles << std::endl;
        for (int j = 0; j <particles; j++) {
            if (bloque.needs_reset(j, get_block_size())) {
                Particle particula = bloque.get_particle(j);
                borrar.insert(borrar.begin(), j);
                std::vector<double> position_particle = particula.get_position();
                std::array<int, 3> pos_particle = posicion_particula(position_particle[0], position_particle[1],
                                                                      position_particle[2], get_block_size());
                int const identificador = find_block(pos_particle[0], pos_particle[1], pos_particle[2]);
                add_block_particle(identificador, particula);
            }
        }
        for (int const posicion: borrar) {
            bloque.pop_particle(posicion);
        }
    }
}

void Grid::initialize_acc_dens() {
    for (auto & bloque : bloques) {
        bloque.initialize_acc_dens_block(G);
    }
}

void Grid::calc_density(double ppm) {
    for (int i = 0; i < static_cast<int>(bloques.size()); i++) {
        std::vector<int> const contiguos = find_adjacent_blocks(bloques[i].get_i(), bloques[i].get_j(),
                                                          bloques[i].get_k());
        int const longitud_particulas_bloque = bloques[i].get_particles_length();
        for (int const posicion_contiguo: contiguos) {
            if (bloques[posicion_contiguo].get_checked()) {
                continue;}
            int const longitud_particulas_contiguo = bloques[posicion_contiguo].get_particles_length();
            for (int loop_var1 = 0; loop_var1 < longitud_particulas_bloque; loop_var1++) {
                std::vector<double> const posicion_particula_bloque = bloques[i].get_particle_position(loop_var1);
                for (int loop_var2 = 0; loop_var2 < longitud_particulas_contiguo; loop_var2++) {
                    if (posicion_contiguo == i && loop_var2 <= loop_var1 ) {
                        continue;}
                    std::vector<double> const posicion_particula_contiguo = bloques[posicion_contiguo].get_particle_position(loop_var2);
                    double const aumento = increase_density(ppm, posicion_particula_bloque, posicion_particula_contiguo);
                    bloques[i].update_particle_density(aumento, loop_var1);
                    bloques[posicion_contiguo].update_particle_density(aumento, loop_var2);}
            }
        }
        bloques[i].set_checked(true);}
    uncheck();
}

void Grid::calc_density_2(double ppm) {
    for (auto & bloque : bloques) {
        int const longitud_particulas_bloque = bloque.get_particles_length();
        for (int loop_var = 0; loop_var < longitud_particulas_bloque; loop_var++) {
            double const density = bloque.get_particle_density(loop_var);
            //std::cout << "densidad: " << density << std::endl;
            double const transformacion_bloque = transform_density(ppm, density);
            //std::cout << "transformacion: " << transformacion_bloque << std::endl;
            bloque.set_particle_density(transformacion_bloque, loop_var);
        }
    }
}

void Grid::calc_acceleration(double ppm) {
    for (int i = 0; i < static_cast<int>(bloques.size()); i++) {
        std::vector<int> const contiguos = find_adjacent_blocks(bloques[i].get_i(), bloques[i].get_j(),
                                                          bloques[i].get_k());
        int const longitud_particulas_bloque = bloques[i].get_particles_length();
        for (int const posicion_contiguo: contiguos) {
            if (bloques[posicion_contiguo].get_checked()) {
                continue;}
            int const longitud_particulas_contiguo = bloques[posicion_contiguo].get_particles_length();
            for (int loop_var1 = 0; loop_var1 < longitud_particulas_bloque; loop_var1++) {
                std::vector<double> const posicion_particula_bloque = bloques[i].get_particle_position(loop_var1);
                for (int loop_var2 = 0; loop_var2 < longitud_particulas_contiguo; loop_var2++) {
                    if (posicion_contiguo == i && loop_var2 <= loop_var1) {continue;}
                    std::vector<double> const posicion_particula_contiguo = bloques[posicion_contiguo].get_particle_position(loop_var2);
                    // estas particulas son copias, no se estan actualizando
                    Particle const particula_bloque = bloques[i].get_particle(loop_var1);
                    Particle const particula_contiguo = bloques[posicion_contiguo].get_particle(loop_var2);
                    std::array<double, 3> aumento = increase_accerelation(ppm, particula_bloque, particula_contiguo);
                    bloques[i].update_particle_acceleration(aumento, loop_var1);
                    aumento = {-aumento[0], -aumento[1], -aumento[2]};
                    bloques[posicion_contiguo].update_particle_acceleration(aumento, loop_var2);}
            }
        }
        bloques[i].set_checked(true);}
    uncheck();}

void Grid::particles_collisions() {
    collisions_x();
    collisions_y();
    collisions_z();
}

void Grid::collisions_z() {
    for (int i = 0; i <= 1; i++) {
        std::vector<int> const border_positions = get_border_z(i);
        for (int const posicion: border_positions) {
            int const longitud_particulas_bloque = bloques[posicion].get_particles_length();
            for (int loop_var = 0; loop_var < longitud_particulas_bloque; loop_var++) {
                double const new_p = new_position(bloques[posicion].get_particle_position(loop_var)[2],
                                     bloques[posicion].get_particle_gradient(loop_var)[2]);
                double const aumento = update_acceleration(new_p, bloques[posicion].get_particle_speed(loop_var)[2],
                                              i, 2);
                bloques[posicion].update_particle_acceleration({0, 0, aumento}, loop_var);
            }
        }
    }
}

void Grid::collisions_y() {
    for (int i = 0; i <= 1; i++) {
        std::vector<int> const border_positions = get_border_y(i);
        for (int const posicion: border_positions) {
            int const longitud_particulas_bloque = bloques[posicion].get_particles_length();
            for (int loop_var = 0; loop_var < longitud_particulas_bloque; loop_var++) {
                double const new_p = new_position(bloques[posicion].get_particle_position(loop_var)[1],
                                     bloques[posicion].get_particle_gradient(loop_var)[1]);
                double const aumento = update_acceleration(new_p, bloques[posicion].get_particle_speed(loop_var)[1],
                                              i, 1);
                bloques[posicion].update_particle_acceleration({0, aumento, 0}, loop_var);
            }
        }
    }
}

void Grid::collisions_x() {
    for (int i = 0; i <= 1; i++) {
        std::vector<int> const border_positions = get_border_x(i);
        for (int const posicion: border_positions) {
            int const longitud_particulas_bloque = bloques[posicion].get_particles_length();
            for (int loop_var = 0; loop_var < longitud_particulas_bloque; loop_var++) {
                double const new_p = new_position(bloques[posicion].get_particle_position(loop_var)[0],
                                     bloques[posicion].get_particle_gradient(loop_var)[0]);
                double const aumento = update_acceleration(new_p, bloques[posicion].get_particle_speed(loop_var)[0],
                                              i, 0);
                // std::cout << "aumento: " << aumento << std::endl;
                bloques[posicion].update_particle_acceleration({aumento, 0, 0}, loop_var);
            }
        }
    }
}

void Grid::particle_movements() {
    for (auto & bloque : bloques) {
        int const longitud_particulas_bloque = bloque.get_particles_length();
        for (int var_loop = 0; var_loop < longitud_particulas_bloque; var_loop++) {
            std::vector<double> const gradient = bloque.get_particle_gradient(var_loop);
            std::vector<double> const position = bloque.get_particle_position(var_loop);
            std::array<double, 3> const acceleration = bloque.get_particle_acc(var_loop);
            std::vector<double> const new_position = uptdade_position(position, gradient, acceleration);
            std::vector<double> const new_speed = uptdade_velocity(gradient, acceleration);
            std::vector<double> const new_gradient = uptdade_gradient(gradient, acceleration);
            bloque.set_particle_position(new_position, var_loop);
            bloque.set_particle_speed(new_speed, var_loop);
            bloque.set_particle_gradient(new_gradient, var_loop);
        }
    }
}

void Grid::border_collisions() {
    for (int i = 0; i <= 1; i++) {
        std::vector<int> const border_positions = get_border_x(i);
        for (int const posicion: border_positions) {
            int const longitud_particulas_bloque = bloques[posicion].get_particles_length();
            for (int var_loop = 0; var_loop < longitud_particulas_bloque; var_loop++) {
                bloques[posicion].particle_collisions(var_loop, i, 0);}
        }
    }
    for (int i = 0; i <= 1; i++) {
        std::vector<int> const border_positions = get_border_y(i);
        for (int const posicion: border_positions) {
            int const longitud_particulas_bloque = bloques[posicion].get_particles_length();
            for (int var_loop = 0; var_loop < longitud_particulas_bloque; var_loop++) {
                bloques[posicion].particle_collisions(var_loop, i, 1);}
        }
    }
    for (int i = 0; i <= 1; i++) {
        std::vector<int> const border_positions = get_border_z(i);
        for (int const posicion: border_positions) {
            int const longitud_particulas_bloque = bloques[posicion].get_particles_length();
            for (int var_loop = 0; var_loop < longitud_particulas_bloque; var_loop++) {
                bloques[posicion].particle_collisions(var_loop, i, 2);}
        }
    }
}

void Grid::simulation(int iteraciones, double ppm) {
    // std:: cout << "Hola" << '\n';
    for (int i = 0; i < iteraciones; i++) {
        uncheck();
        //std::cout << "UNCHECK OK" << std::endl;
        reposition_particles(); // 4.3.1
        //std::cout << "REPOS OK" << std::endl;
        initialize_acc_dens(); // 4.3.1.1
        //std::cout << "INIC OK" << std::endl;
        // std:: cout << "Hola" << i << '\n';
        calc_density(ppm); // 4.3.1.2 - 4.3.1.3
        //std::cout << "DENS OK" << std::endl;
        calc_density_2(ppm);
        //std::cout << "DENS 2 OK" << std::endl;
        calc_acceleration(ppm); // 4.3.1.4
        //std::cout << "ACC OK" << std::endl;
        particles_collisions();
        //std::cout << "COLLISIONS OK" << std::endl;
        particle_movements();
        //std::cout << "MOVEMENTS OK" << std::endl;
        //std::cout << "i = " << i << std::endl;
        border_collisions();
    }
    //print_particles();
}

std::array<double, 3> Grid::get_block_size() {
    return m_block_size;
}

void Grid::set_block_size(std::array<double, 3> block_size) {
    m_block_size = block_size;

}

void Grid::uncheck() {
    for (auto & bloque : bloques) {
        bloque.set_checked(false);
    }
}

std::vector<Particle> Grid::reordenar_particulas() {
    std::vector<Particle> particulas_reordenadas;
    for (auto & bloque : bloques) {
        for (int j = 0; j < bloque.get_particles_length(); j++) {
            Particle const particula  =bloque.get_particle(j);
            particulas_reordenadas.push_back(particula);
        }
    }
    std::sort(particulas_reordenadas.begin(), particulas_reordenadas.end(),
              [](const Particle& a, const Particle& b) {
                return a.get_id() < b.get_id();
              });
    return particulas_reordenadas;
}

void Grid::print_particles() {
    std::vector<Particle> const particulas = reordenar_particulas();
    int contador = 0;
    for (Particle particula: particulas) {
        if (contador >= 4800) {break;}
        std::array<double, 3>  acceleration = particula.get_acceleration();
        std::vector<double> position = particula.get_position();
        std::cout << "Particle ID: " << particula.get_id() << '\n';
        std::cout << "Particle position: " << position[0] << " " << position[1] << " " << position[2] << '\n';
        std::cout << "Particle acceleration: " << acceleration[0] << " " << acceleration[1] << " " << acceleration[2] << '\n';
        std::cout << "Particle density: " << particula.get_density() << '\n';
        contador++;
    }
}

std::vector<int> Grid::get_border_x(int tipo) const {
    if (tipo != 0 && tipo != 1) {
        return {-1};
    }
    std::vector<int> posiciones;
    for (int i = 0; i < m_ny; i++) {
        for (int j = 0; j < m_nz; j++) {
            if (tipo == 0) {posiciones.push_back(find_block_2(0, i, j));}
            else {posiciones.push_back(find_block_2(m_nx-1, i, j ));}
        }
    }
    return posiciones;
}

std::vector<int> Grid::get_border_y(int tipo) const {
    if (tipo != 0 && tipo != 1) {
        return {-1};
    }
    std::vector<int> posiciones;
    for (int i = 0; i < m_nx; i++) {
        for (int j = 0; j < m_nz; j++) {
            if (tipo == 0) {posiciones.push_back(find_block_2(i, 0, j));}
            else {posiciones.push_back(find_block_2(i, m_ny-1, j ));}
        }
    }
    return posiciones;
}

std::vector<int> Grid::get_border_z(int tipo) const {
    if (tipo != 0 && tipo != 1) {
        return {-1};
    }
    std::vector<int> posiciones;
    for (int i = 0; i < m_nx; i++) {
        for (int j = 0; j < m_ny; j++) {
            if (tipo == 0) {posiciones.push_back(find_block_2(i, j, 0));}
            else {posiciones.push_back(find_block_2(i, j, m_nz-1 ));}
        }
    }
    return posiciones;
}