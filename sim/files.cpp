#include "files.hpp"

#include <cmath>
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
struct Datos_cabecera read_header_file (const std::string& fileName){
  std::ifstream archivo(fileName, std::ios::binary);
  float ppm = 0.0;
  int num_np = 0;
  archivo.read(reinterpret_cast<char*>(&ppm), sizeof(float));
  archivo.read(reinterpret_cast<char*>(&num_np), sizeof(num_np));
  std::cout << "Number of particles: " << num_np << '\n';
  std::cout << "Particles per meter: " << ppm << '\n';
  struct Datos_cabecera resultado{};
  resultado.np = num_np;
  resultado.ppm = static_cast<double>(ppm);
  archivo.close();
  return resultado;
}

std::vector<double> read_particle(std::ifstream &archivo, float &p_x, float &p_y, float &p_z);

std::vector<double> readGradient(std::ifstream &archivo);

std::vector<double> readSpeed(std::ifstream &archivo);

int Read_particles(std:: string filename, Grid &malla, double ppm) {
    double masa = calc_masa(ppm);
    //Lectura
    std::ifstream archivo(filename, std::ios::binary);
    int num_id = 0;
    archivo.seekg(OCHO, std::ios_base::beg); //offset para leer el fichero desde las particulas
    while (!archivo.eof()) {
        std::vector<double> pos = read_particle(archivo);
        // std::cout << "posicion_bloque: " << pos[0] << " " << pos[1] << " " << pos[2] << std::endl;
        std::vector<double> gradient = readGradient(archivo);
        std::vector<double> speed = readSpeed(archivo);
        std:: array<int, 3> pos_particle = posicion_particula(pos[0], pos[1], pos[2], malla.get_block_size());
        int identificador = malla.find_block(pos_particle[0], pos_particle[1], pos_particle[2]);
        Particle particula(num_id, pos, gradient, speed);
        particula.Set_mass(masa);
        malla.add_block_particle(identificador, particula);
        num_id ++;}
    return num_id - 1;}

std::vector<double> readSpeed(std::ifstream &archivo) {
    float v_x;
    float v_y;
    float v_z;
    archivo.read(reinterpret_cast<char *>(&v_x), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&v_y), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&v_z), sizeof(float));
    std:: vector<double> speed = {static_cast<double>(v_x), static_cast<double>(v_y), static_cast<double>(v_z)};
    return speed;
}

std::vector<double> readGradient(std::ifstream &archivo) {
    float hvx;
    float hvy;
    float hvz;
    archivo.read(reinterpret_cast<char *>(&hvx), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&hvy), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&hvz), sizeof(float));
    std:: vector<double> gradient = {static_cast<double>(hvx), static_cast<double>(hvy), static_cast<double>(hvz)};
    return gradient;
}

std::vector<double> read_particle(std::ifstream &archivo) {
    float p_x;
    float p_y;
    float p_z;
    archivo.read(reinterpret_cast<char *>(&p_x), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&p_y), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&p_z), sizeof(float));
    std:: vector <double> pos= {static_cast<double>(p_x), static_cast<double>(p_y), static_cast<double>(p_z)};
    return pos;
}
