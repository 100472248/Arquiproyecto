#include "files.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/*
Creamos una funcion para leer el header del fichero (las ppm y n particulas). Esta info sera devuelta al main
para el posterior tratamiento de los 2 valores devueltos, contenidos en el struct (asi garanizamos el retorno de
2 tipos de datos diferentes)
*/
struct Datos_cabecera read_header_file (std::string fileName){
  std::ifstream archivo(fileName, std::ios::binary);
  double ppm;
  int np;
  archivo.read(reinterpret_cast<char*>(&ppm), sizeof(float));
  archivo.read(reinterpret_cast<char*>(&np), sizeof(np));
  std::cout << "Número de partículas: " << np << std::endl;
  std::cout << "PPM: " << ppm << std::endl;
  struct Datos_cabecera resultado;
  resultado.np = np;
  resultado.ppm = ppm;
  archivo.close();
  return resultado;
}