#include "files.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include "math.hpp"
#include "grid.hpp"
#include "particle.hpp"

/*
Creamos una funcion para leer el header del fichero (las ppm y n particulas). Esta info sera devuelta al main
para el posterior tratamiento de los 2 valores devueltos, contenidos en el struct (asi garanizamos el retorno de
2 tipos de datos diferentes)
*/
struct Datos_cabecera read_header_file (std::string fileName){
  std::ifstream archivo(fileName, std::ios::binary);
  float ppm;
  int np;
  archivo.read(reinterpret_cast<char*>(&ppm), sizeof(float));
  archivo.read(reinterpret_cast<char*>(&np), sizeof(np));
  std::cout << "Number of particles: " << np << std::endl;
  std::cout << "Particles per meter: " << ppm << std::endl;
  struct Datos_cabecera resultado;
  resultado.np = np;
  resultado.ppm = static_cast<double>(ppm);
  archivo.close();
  return resultado;
}

int Read_particles(std:: string filename, std:: array<double, 3> tamanio_bloque, Grid &malla) {
    //Lectura
    std::ifstream archivo(filename, std::ios::binary);
    int id = 0;
    archivo.seekg(8, std::ios_base::beg); //offset para leer el fichero desde las particulas
    while (!archivo.eof()) {
        float px, py, pz, hvx, hvy, hvz, vx, vy, vz;
        archivo.read(reinterpret_cast<char *>(&px), sizeof(float));
        archivo.read(reinterpret_cast<char *>(&py), sizeof(float));
        archivo.read(reinterpret_cast<char *>(&pz), sizeof(float));
        std:: vector<double> pos = {static_cast<double>(px), static_cast<double>(py), static_cast<double>(pz)};
        archivo.read(reinterpret_cast<char *>(&hvx), sizeof(float));
        archivo.read(reinterpret_cast<char *>(&hvy), sizeof(float));
        archivo.read(reinterpret_cast<char *>(&hvz), sizeof(float));
        std:: vector<double> gradient = {static_cast<double>(hvx), static_cast<double>(hvy), static_cast<double>(hvz)};
        archivo.read(reinterpret_cast<char *>(&vx), sizeof(float));
        archivo.read(reinterpret_cast<char *>(&vy), sizeof(float));
        archivo.read(reinterpret_cast<char *>(&vz), sizeof(float));
        std:: vector<double> speed = {static_cast<double>(vx), static_cast<double>(vy), static_cast<double>(vz)};
        std:: array<int, 3> pos_particle = posicion_particula(px, py, pz, tamanio_bloque);
        int identificador = malla.find_block(pos_particle[0], pos_particle[1], pos_particle[2]);
        Particle particula(id, pos, gradient, speed);
        malla.add_block_particle(identificador, particula);
        id ++;}
    return id-1;}