#ifndef ARQUIPROYECTO_MATH_HPP
#define ARQUIPROYECTO_MATH_HPP
#include <array>
double const R = 1.695;
int const RO = 1000;
double const PS = 3.0;
int const S_C = 30000;
double const D_V = 128.0;
double const MU = 0.4;
double const D_P = 0.0002;
double const DELTA_T = 0.003;
double h;

std::array<int,3> calc_n_blocks(double ppm);
#endif //ARQUIPROYECTO_MATH_HPP

