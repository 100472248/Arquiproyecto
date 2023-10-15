#ifndef ARQUIPROYECTO_MATH_HPP
#define ARQUIPROYECTO_MATH_HPP
#include <array>
#include <numbers>
#include "particle.hpp"
double const R = 1.695;
int const RO = 1000;
double const PS = 3.0;
int const S_C = 30000;
double const D_V = 128.0;
double const MU = 0.4;
double const D_P = 0.0002;
double const DELTA_T = 0.003;
double const PI = std::numbers::pi;
double longitud_suavizado(double ppm);
std::array<int,3> calc_n_blocks(double ppm);
std::array<double, 3>calc_size_blocks(std::array<int,3> medidas_grid);
std::array<int, 3>posicion_particula(double px, double py, double pz, std::array<double,3> size_block);
double calc_masa (double ppm);
double increase_density(double ppm, std::vector<double> pi, std::vector<double> pj);
double transform_density(double ppm, double Ri);
std::array<double, 3> increase_accerelation(double ppm, Particle pi, Particle pj) ;

#endif //ARQUIPROYECTO_MATH_HPP

