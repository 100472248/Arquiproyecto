#include <cmath>
#include <string>
#include "math.hpp"
#include "files.hpp"
#include "grid.hpp"
/*Calculamos la longitud de suavizado. Notese que, h al ser una variable global declarada en el archivo header,
mantendra su valor para siempre a lo largo del programa una vez esta funcion sea declarada (se presume que en principio,
solo una vez)*/
double longitud_suavizado (double ppm){
  double h = R/ppm; ///Recordemos que R ya es una constante global conocida en el header
  return h;
}
/*Esta funcion sera devuelta al main como un array fijo de 3 posiciones para poder invocar al constructor del grid*/
std::array<int,3> calc_n_blocks(double ppm){
  double h = longitud_suavizado(ppm); ///Como se anticipo previamente, h ya se le ha puesto valor aqui

  int n_x = std::floor((BMAX[0]-BMIN[0])/h);
  int n_y = std::floor((BMAX[1]-BMIN[1])/h);
  int n_z = std::floor((BMAX[2]-BMIN[2])/h);
  std::cout << "h: " << h << " ppm: " << ppm << " BMAX: " << BMAX[0] << " BMIN: " << BMIN[0] <<" nx: "<< n_x<<std::endl;
  std::array<int,3> resultado = {n_x,n_y,n_z};
  return resultado;
}

std::array<double,3> calc_size_blocks(std::array<int,3> medidas_grid){
    std::cout << " Medidas Grid: " << medidas_grid[0] << medidas_grid[1] << medidas_grid[2] << std::endl;
    double sx = (BMAX[0]-BMIN[0])/medidas_grid[0];
    double sy = (BMAX[1]-BMIN[1])/medidas_grid[1];
    double sz = (BMAX[2]-BMIN[2])/medidas_grid[2];
    std::array<double,3> resultado = {sx, sy, sz};
    return resultado;
}

std::array<int, 3>posicion_particula(double px, double py, double pz, std::array<double,3> size_block){
    std::cout << "px: " << px << "py: " << py << "pz: " << pz << " Size block: " << size_block[0] << size_block[1] << size_block[2] << std::endl;
    int x = std::floor((px-BMIN[0])/size_block[0]);
    int y = std::floor((py-BMIN[1])/size_block[1]);
    int z = std::floor((pz-BMIN[2])/size_block[2]);
    std::array<int,3> resultado = {x, y, z};
    return resultado;
}