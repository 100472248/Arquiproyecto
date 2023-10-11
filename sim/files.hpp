#ifndef ARQUIPROYECTO_FILES_HPP
#define ARQUIPROYECTO_FILES_HPP
#include <string>
#include "grid.hpp"
#include "particle.hpp"
struct Datos_cabecera {
    int np;
    double ppm;
};
struct Datos_cabecera read_header_file(std::string fileName);

int Read_particles(std:: string filename, std:: array<double, 3> tamanio_bloque, Grid &malla, double ppm);
#endif