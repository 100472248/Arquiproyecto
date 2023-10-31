//
// Created by jaime-vaquero on 30/10/23.
//

#include <cmath>
#include "new_math.hpp"
#include "grid.hpp"
#include "particle.hpp"

Math::Math(double ppm) {
    h =R_H/ppm;
    m = R_0 / pow(ppm, 3);

};
/*Esta funcion sera devuelta al main como un array fijo de 3 posiciones para poder invocar al constructor del grid*/
std::array<int,3> Math:: calc_n_blocks(){
    int n_x = std::floor((BMAX[0]-BMIN[0])/h);
    int n_y = std::floor((BMAX[1]-BMIN[1])/h);
    int n_z = std::floor((BMAX[2]-BMIN[2])/h);
    std::array<int,3> resultado = {n_x,n_y,n_z};
    // std::cout << "Grid size: " << n_x << " x " << n_y << " x " << n_z << std::endl;
    // std::cout << "Number of blocks: " << n_x * n_y * n_z << std::endl;
    return resultado;
}

std::array<double,3> Math:: calc_size_blocks(std::array<int,3> medidas_grid){
    double s_x = (BMAX[0] - BMIN[0]) / medidas_grid[0];
    double s_y = (BMAX[1] - BMIN[1]) / medidas_grid[1];
    double s_z = (BMAX[2] - BMIN[2]) / medidas_grid[2];
    std::array<double,3> resultado = {s_x, s_y, s_z};
    // std::cout << "Block size: " << s_x << " x " << s_y << " x " << s_z << std::endl;
    return resultado;
}

std::array<int, 3> Math:: posicion_particula(double px, double py, double pz, std::array<double,3> size_block){
    int new_x = std::floor((px - BMIN[0]) / size_block[0]);
    int new_y = std::floor((py - BMIN[1]) / size_block[1]);
    int new_z = std::floor((pz - BMIN[2]) / size_block[2]);
    std::array<int,3> resultado = {new_x, new_y, new_z};
    return resultado;
}

double Math:: increase_density(std::array <double, 3> pi, std::array <double, 3> pj){
    /*Incremento de las densidades*/
    double resta_x = pi[0] - pj[0];
    double resta_y = pi[1] - pj[1];
    double resta_z = pi[2] - pj[2];
    double v_abs = sqrt(pow(resta_x, 2) + pow(resta_y, 2) + pow(resta_z, 2));
    double aumento = 0;
    double h_2 = pow(h, 2);
    if (h_2 > v_abs) {
        aumento = pow(fabs(h_2 - v_abs), 3);
    }
    return aumento;
}
double Math:: transform_density(double Ri){
    /*Transformación de las densidad Ri*/
   double h_6 = pow(h, POT6);
    double h_9 = pow(h, POT9);
    Ri = (Ri + h_6)*(TOFIVE*m)/(SFOUR * N_PI * h_9);
    return Ri;
}

std::array<double, 3> Math:: increase_accerelation(Particle pi, Particle pj) {
    double masa = pi.get_mass();
    std::vector<double>posi = pi.get_position();
    std::vector<double>posj = pj.get_position();
    std::vector<double>v_i = pi.get_speed();
    std::vector<double>v_j = pj.get_speed();
    double h_2 = pow(h, 2);
    double resta_x = posi[0] - posj[0];
    double resta_y = posi[1] - posj[1];
    double resta_z = posi[2] - posj[2];
    double v_abs = sqrt(pow(resta_x, 2) + pow(resta_y, 2) + pow(resta_z, 2));
    std::array<double, 3>  aumento = {0,0,0};
    if (h_2 > v_abs) {
        double di = pi.get_density();
        double dj = pj.get_density();
        double maximo = sqrt(std::max(v_abs, pow(TEN, -DOCE)));
        double h_6 = pow(h, POT6);
        aumento[0] = ((posi[0]-posj[0]) * (FTEEN/(N_PI * h_6)) * OPFIVE * masa * PS * (pow(h - maximo, 2) / maximo) * (di + dj - 2 * R_0) + (v_i[0] + v_j[0]) * (FFIVE / (N_PI * h_6)) * masa * MU) / (di + dj);
        aumento[1] = ((posi[1]-posj[1]) * (FTEEN/(N_PI * h_6)) * OPFIVE * masa * PS * (pow(h - maximo, 2) / maximo) * (di + dj - 2 * R_0) + (v_i[1] + v_j[1]) * (FFIVE / (N_PI * h_6)) * masa * MU) / (di + dj);
        aumento[2] = ((posi[2]-posj[2]) * (FTEEN/(N_PI * h_6)) * OPFIVE * masa * PS * (pow(h - maximo, 2) / maximo) * (di + dj - 2 * R_0) + (v_i[2] + v_j[2]) * (FFIVE / (N_PI * h_6)) * masa * MU) / (di + dj);}
    return aumento;}


std::array<double, 3> Math:: new_position_x_y_z(std::vector<double> position_part, std::vector<double> speed_gradient_part){
    double x = position_part[0]+ speed_gradient_part[0] * DELTA_T;
    double y = position_part[1]+ speed_gradient_part[1] * DELTA_T;
    double z = position_part[2]+ speed_gradient_part[2] * DELTA_T;
    std::array<double, 3> new_position_x_y_z = {x, y, z};
    return new_position_x_y_z;
}

void Math:: update_acceleration(std::array<double, 3> new_positions, Particle &particle, Grid &grid) {
    std::vector<double> position_part       = particle.get_position();
    std::array<int, 3> block_part           = particle.get_bloque();
    std::vector<double> speed_part          = particle.get_speed();
    std::array<double, 3> acceleration_part = particle.get_acceleration();
    std::array<double, 3> grid_size         = grid.get_grid_size();
    double delta                            = 0;

    for (int it = 0; it < 3; it++) {
        delta = 0;
        if (block_part[it] == 0) {
            delta = D_P - (new_positions[it] - BMIN[it]);
        } else if (block_part[it] == grid_size[it] - 1) {
            delta = D_P - (BMAX[it] - new_positions[it]);
        }
        if (delta > pow(TEN, -TEN)) {
            if (block_part[it] == 0) {
                acceleration_part[it] = acceleration_part[it] + (S_C * delta - D_V * speed_part[it]);
            } else if (block_part[it] == grid_size[it] - 1) {
                acceleration_part[it] = acceleration_part[it] - (S_C * delta + D_V * speed_part[it]);
            }
        }
    }
    particle.Set_acceleration(acceleration_part);
}
