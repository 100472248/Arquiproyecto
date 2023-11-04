#include "files.hpp"
#include <cmath>
#include <fstream>
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

int Read_particles(const std:: string& filename, Grid &malla, double ppm) {
    double const masa = calc_masa(ppm);
    //Lectura
    std::ifstream archivo(filename, std::ios::binary);
    int num_id = 0;
    archivo.seekg(OCHO, std::ios_base::beg); //offset para leer el fichero desde las particulas
    while (!archivo.eof()) {
        std::vector<double> pos = read_particle(archivo);
        // std::cout << "posicion_bloque: " << pos[0] << " " << pos[1] << " " << pos[2] << std::endl;
        std::vector<double> const gradient = readGradient(archivo);
        std::vector<double> const speed = readSpeed(archivo);
        std:: array<int, 3> pos_particle = posicion_particula(pos[0], pos[1], pos[2], malla.get_block_size());
        int const identificador = malla.find_block(pos_particle[0], pos_particle[1], pos_particle[2]);
        Particle particula(num_id, pos, gradient, speed);
        //std::cout << pos[0] << " " << pos[1] << " " << pos[2] << std::endl;
        particula.Set_mass(masa);
        malla.add_block_particle(identificador, particula);
        num_id ++;}
    return num_id - 1;}

std::vector<double> readSpeed(std::ifstream &archivo) {
    float v_x = NAN;
    float v_y = NAN;
    float v_z = NAN;
    archivo.read(reinterpret_cast<char *>(&v_x), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&v_y), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&v_z), sizeof(float));
    std:: vector<double> speed = {static_cast<double>(v_x), static_cast<double>(v_y), static_cast<double>(v_z)};
    return speed;
}

std::vector<double> readGradient(std::ifstream &archivo) {
    float hvx = NAN;
    float hvy = NAN;
    float hvz = NAN;
    archivo.read(reinterpret_cast<char *>(&hvx), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&hvy), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&hvz), sizeof(float));
    std:: vector<double> gradient = {static_cast<double>(hvx), static_cast<double>(hvy), static_cast<double>(hvz)};
    return gradient;
}

std::vector<double> read_particle(std::ifstream &archivo) {
    float p_x = NAN;
    float p_y = NAN;
    float p_z = NAN;
    archivo.read(reinterpret_cast<char *>(&p_x), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&p_y), sizeof(float));
    archivo.read(reinterpret_cast<char *>(&p_z), sizeof(float));
    std:: vector <double> pos= {static_cast<double>(p_x), static_cast<double>(p_y), static_cast<double>(p_z)};
    return pos;
}

void Write_particles(std::string& archivo, Grid &malla) {
    std::vector<Particle> const particulas = malla.reordenar_particulas();
    int contador = 0;
    std::ofstream outputFile(archivo);  // Abre el archivo de salida
    if (outputFile.is_open()) {
        for (Particle particula : particulas) {
          if (contador >= 4800) { break; }
          std::array<double, 3> acceleration = particula.get_acceleration();
          std::vector<double> position = particula.get_position();
          std::vector<double> gradient = particula.get_gradient();
          std::vector<double> speed = particula.get_speed();
          outputFile << "Particle ID: " << particula.get_id() << '\n';
          outputFile << "Particle position: " << position[0] << " " << position[1] << " " << position[2] << '\n';
          outputFile << "Particle speed: " << speed[0] << " " << speed[1] << " " << speed[2] << '\n';
          outputFile << "Particle gradient: " << gradient[0] << " " << gradient[1] << " " << gradient[2] << '\n';
          outputFile << "Particle acceleration: " << acceleration[0] << " " << acceleration[1] << " " << acceleration[2] << '\n';
          outputFile << "Particle density: " << particula.get_density() << '\n';
          contador++;
        }
        outputFile.close();  // Cierra el archivo de salida
    }
}


