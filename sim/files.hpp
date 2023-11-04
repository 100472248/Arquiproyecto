#ifndef ARQUIPROYECTO_FILES_HPP
#define ARQUIPROYECTO_FILES_HPP
#include <string>
#include "grid.hpp"
#include "particle.hpp"
int const OCHO = 8;
struct Datos_cabecera {
    int np;
    double ppm;
};
struct Datos_cabecera read_header_file(const std::string& fileName);
int Read_particles(const std:: string& filename, Grid &malla, double ppm);
std::vector<double> read_particle(std::ifstream &archivo);
std::vector<double> readGradient(std::ifstream &archivo);
std::vector<double> readSpeed(std::ifstream &archivo);
void Write_particles(std::string& archivo, Grid &malla);
#endif