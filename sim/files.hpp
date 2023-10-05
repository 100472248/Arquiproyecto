#ifndef ARQUIPROYECTO_FILES_HPP
#define ARQUIPROYECTO_FILES_HPP
#include <string>
struct Datos_cabecera {
    int np;
    double ppm;
};
struct Datos_cabecera read_header_file(std::string fileName);
#endif