#include <cmath>
#include "math.hpp"

#include "grid.hpp"
#include "particle.hpp"
/*Calculamos la longitud de suavizado. Notese que, h al ser una variable global declarada en el archivo header,
mantendra su valor para siempre a lo largo del programa una vez esta funcion sea declarada (se presume que en principio,
solo una vez)*/
double longitud_suavizado (double ppm){
 ///Recordemos que R ya es una constante global conocida en el header
  // std::cout << "Smoothing length: " << h << std::endl;
  return R_H/ppm;}

/*Esta funcion sera devuelta al main como un array fijo de 3 posiciones para poder invocar al constructor del grid*/
std::array<int,3> calc_n_blocks(double ppm){
  double const var_h = longitud_suavizado(ppm); ///Como se anticipo previamente, h ya se le ha puesto valor aqu
  int const n_x = std::floor((BMAX[0]-BMIN[0])/var_h);
  int const n_y = std::floor((BMAX[1]-BMIN[1])/var_h);
  int const n_z = std::floor((BMAX[2]-BMIN[2])/var_h);
  std::array<int,3> resultado = {n_x,n_y,n_z};
  // std::cout << "Grid size: " << n_x << " x " << n_y << " x " << n_z << std::endl;
  // std::cout << "Number of blocks: " << n_x * n_y * n_z << std::endl;
  return resultado;
}

std::array<double,3> calc_size_blocks(std::array<int,3> medidas_grid){
    double const s_x = (BMAX[0] - BMIN[0]) / medidas_grid[0];
    double const s_y = (BMAX[1] - BMIN[1]) / medidas_grid[1];
    double const s_z = (BMAX[2] - BMIN[2]) / medidas_grid[2];
    std::array<double,3> resultado = {s_x, s_y, s_z};
    // std::cout << "Block size: " << s_x << " x " << s_y << " x " << s_z << std::endl;
    return resultado;
}

std::array<int, 3>posicion_particula(double px, double py, double pz, std::array<double,3> size_block){
    int const new_x = std::floor((px - BMIN[0]) / size_block[0]);
    int const new_y = std::floor((py - BMIN[1]) / size_block[1]);
    int const new_z = std::floor((pz - BMIN[2]) / size_block[2]);
    std::array<int,3> resultado = {new_x, new_y, new_z};
    return resultado;
}

double calc_masa (double ppm){
    return R_0 / pow(ppm, 3);}

double increase_density(double ppm, std::vector <double> pi, std::vector <double> pj) {
    /*Incremento de las densidades*/
    double const h_2 = pow(longitud_suavizado(ppm), 2);
    double const resta_x = pi[0] - pj[0];
    double const resta_y = pi[1] - pj[1];
    double const resta_z = pi[2] - pj[2];
    double const v_abs = pow(resta_x, 2) + pow(resta_y, 2) + pow(resta_z, 2);

    double aumento = 0;
    if (h_2 > v_abs) {
        aumento = pow((h_2 - v_abs), 3);
    }
    return aumento;
}
double transform_density(double ppm, double Ri){
    /*Transformación de las densidad Ri*/
    double const var_h = longitud_suavizado(ppm);
    double const masa = calc_masa(ppm);
    double const h_6 = pow(var_h, POT6);
    double const h_9 = pow(var_h, POT9);
    Ri = (Ri + h_6)*(TOFIVE*masa)/(SFOUR * N_PI * h_9);
    return Ri;
}

std::array<double, 3> increase_accerelation(double ppm, Particle pi, Particle pj) {
    double const var_h = longitud_suavizado(ppm);
    double const masa = pi.get_mass();
    std::vector<double>posi = pi.get_position();
    std::vector<double>posj = pj.get_position();
    std::vector<double>v_i = pi.get_speed();
    std::vector<double>v_j = pj.get_speed();
    double const h_2 = pow(var_h, 2);
    double const resta_x = posi[0] - posj[0];
    double const resta_y = posi[1] - posj[1];
    double const resta_z = posi[2] - posj[2];
    double const v_abs = pow(resta_x, 2) + pow(resta_y, 2) + pow(resta_z, 2);
    std::array<double, 3>  aumento = {0,0,0};
    if (h_2 > v_abs) {
        double const d_i = pi.get_density();
        double const d_j = pj.get_density();
        double const maximo = sqrt(std::max(v_abs, pow(TEN, -DOCE)));
        double const h_6 = pow(var_h, POT6);
        aumento[0] = ((posi[0]-posj[0]) * (FTEEN/(N_PI * h_6)) * OPFIVE * masa * PS * (pow(var_h - maximo, 2) / maximo) * (d_i + d_j - 2 * R_0) + (v_j[0]- v_i[0]) * (FFIVE / (N_PI * h_6)) * masa * MU) / (d_i * d_j);
        aumento[1] = ((posi[1]-posj[1]) * (FTEEN/(N_PI * h_6)) * OPFIVE * masa * PS * (pow(var_h - maximo, 2) / maximo) * (d_i + d_j - 2 * R_0) + (v_j[1] - v_i[1]) * (FFIVE / (N_PI * h_6)) * masa * MU) / (d_i * d_j);
        aumento[2] = ((posi[2]-posj[2]) * (FTEEN/(N_PI * h_6)) * OPFIVE * masa * PS * (pow(var_h - maximo, 2) / maximo) * (d_i + d_j - 2 * R_0) + (v_j[2] - v_i[2]) * (FFIVE / (N_PI * h_6)) * masa * MU) / (d_i * d_j);}
    return aumento;}


std::array<double, 3> new_position_x_y_z(std::vector<double> position_part, std::vector<double> speed_gradient_part){
    double const new_x = position_part[0] + speed_gradient_part[0] * DELTA_T;
    double const new_y = position_part[1] + speed_gradient_part[1] * DELTA_T;
    double const new_z = position_part[2] + speed_gradient_part[2] * DELTA_T;
    std::array<double, 3> new_position_x_y_z = {new_x, new_y, new_z};
    return new_position_x_y_z;
}

void update_acceleration(std::array<double, 3> new_positions, Particle &particle, Grid &grid) {
    std::vector<double> const position_part       = particle.get_position();
    std::array<int, 3> block_part           = particle.get_bloque();
    std::vector<double> speed_part          = particle.get_speed();
    std::array<double, 3> acceleration_part = particle.get_acceleration();
    std::array<double, 3> grid_size         = grid.get_grid_size();
    double delta = NAN;

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


void update_all (Particle &particula){
    std::vector<double> position_part = particula.get_position();
    std::vector<double> gradient_part = particula.get_gradient();
    std::vector<double> speed_part = particula.get_speed();
    std::array<double, 3> acceleration_part = particula.get_acceleration();
    for (int i = 0; i < 3; i ++){
        position_part[i] = position_part[i] +  gradient_part[i] * DELTA_T + acceleration_part[i] * pow(DELTA_T, 2);
        speed_part[i] = gradient_part[i] + (acceleration_part[i] * DELTA_T)/2;
        gradient_part[i] = gradient_part[i] + acceleration_part[i] * DELTA_T;
    }
    particula.set_position(position_part);
    particula.set_gradient(gradient_part);
    particula.set_speed(speed_part);
}

void colision_walls (Particle &particula, Grid &grid){
    std::vector<double> position_part = particula.get_position();
    std::vector<double> gradient_part = particula.get_gradient();
    std::vector<double> speed_part = particula.get_speed();
    std::array<int, 3> block_part  = particula.get_bloque();
    std::array<double, 3> grid_size  = grid.get_grid_size();
    std:: vector<double> var_d = {0, 0, 0};
    for (int it = 0; it < 3; it++) {
        if (block_part[it] == 0) {
            var_d[it] = position_part[it] - BMIN[it];}
        else if (block_part[it] == grid_size[it] - 1) {
            var_d[it] = BMAX[it] - position_part[it];}
        if (var_d[it] < 0) {
            if (block_part[it] == 0) {
                position_part[it] = BMIN[it] - var_d[it];}
            else if (block_part[it] == grid_size[it] - 1) {
                position_part[it] = BMAX[it] + var_d[it];}
            speed_part[it] = -speed_part[it];
            gradient_part[it] = -gradient_part[it];}}
    particula.set_position(position_part);
    particula.set_gradient(gradient_part);
    particula.set_speed(speed_part);}
