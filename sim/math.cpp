#include <cmath>
#include <string>
#include "math.hpp"
#include "files.hpp"
#include "grid.hpp"
/*Calculamos la longitud de suavizado. Notese que, h al ser una variable global declarada en el archivo header,
mantendra su valor para siempre a lo largo del programa una vez esta funcion sea declarada (se presume que en principio,
solo una vez)*/
void calc_longitud_suavizado (double ppm){
  h = R/ppm; ///Recordemos que R ya es una constante global conocida en el header
}
/*Esta funcion sera devuelta al main como un array fijo de 3 posiciones para poder invocar al constructor del grid*/
std::array<int,3> calc_n_blocks(double ppm){
  double numerador, denominador;
  calc_longitud_suavizado(ppm); ///Como se anticipo previamente, h ya se le ha puesto valor aqui
  int n_x = std::floor((BMAX[0]-BMIN[0])/h);
  int n_y = std::floor((BMAX[1]-BMIN[1])/h);
  int n_z = std::floor((BMAX[2]-BMIN[2])/h);
  std::array<int,3> resultado = {n_x,n_y,n_z};
  return resultado;
}
