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
  std::cout << "Número de partículas: " << np << std::endl;
  std::cout << "PPM: " << ppm << std::endl;
  struct Datos_cabecera resultado;
  std::cout << "Okey Lets go header!" << std::endl;
  resultado.np = np;
  resultado.ppm = static_cast<double>(ppm);
  archivo.close();
  std::cout << "Okey Lets go header 1!" << std::endl;
  return resultado;
}

int Read_particles(std:: string filename, std:: array<double, 3> tamanio_bloque, Grid &malla) {
    //Lectura
    std::cout << "Okey Lets go!" << std::endl;
    std::ifstream archivo(filename, std::ios::binary);
    int id = 0;
    archivo.seekg(8, std::ios_base::beg); //offset para leer el fichero desde las particulas
    std::cout << "Okey Lets go 1!" << std::endl;
    while (!archivo.eof()) {
      std::cout << "Okey Lets go 2!" << std::endl;
        float px, py, pz, hvx, hvy, hvz, vx, vy, vz;
        archivo.read(reinterpret_cast<char *>(&px), sizeof(float));
        archivo.read(reinterpret_cast<char *>(&py), sizeof(float));
        archivo.read(reinterpret_cast<char *>(&pz), sizeof(float));
        std:: vector<double> pos = {static_cast<double>(px), static_cast<double>(py), static_cast<double>(pz)};
        std::cout << "Okey Lets go 3!" << std::endl;
        archivo.read(reinterpret_cast<char *>(&hvx), sizeof(float));
        archivo.read(reinterpret_cast<char *>(&hvy), sizeof(float));
        archivo.read(reinterpret_cast<char *>(&hvz), sizeof(float));
        std:: vector<double> gradient = {static_cast<double>(hvx), static_cast<double>(hvy), static_cast<double>(hvz)};
        std::cout << "Okey Lets go 4!" << std::endl;
        archivo.read(reinterpret_cast<char *>(&vx), sizeof(float));
        archivo.read(reinterpret_cast<char *>(&vy), sizeof(float));
        archivo.read(reinterpret_cast<char *>(&vz), sizeof(float));
        std:: vector<double> speed = {static_cast<double>(vx), static_cast<double>(vy), static_cast<double>(vz)};
        std::cout << "Okey Lets go 5!" << std::endl;
        std::cout<< "ID Particula: " << id << "," << px << "," << py << "," << pz << "," << hvx << "," << hvy << "," << hvz << "," << vz << std::endl;
        std:: array<int, 3> pos_particle = posicion_particula(px, py, pz, tamanio_bloque);
        std::cout << "Okey Lets go 6!" << std::endl;
        int identificador = malla.find_block(pos_particle[0], pos_particle[1], pos_particle[2]);
        std::cout << "Este es el indentificador:" <<identificador << std::endl;
        std::cout << "Okey Lets go 7!" << std::endl;
        Particle particula(id, pos, gradient, speed);
        std::cout << "Okey Lets go 8!" << std::endl;
        malla.add_block_particle(identificador, particula);
        std::cout << "Okey Lets go 9!" << std::endl;
        id ++;}
    std::cout << "Okey Lets go out 10 extrremo!" << std::endl;
    return id;}