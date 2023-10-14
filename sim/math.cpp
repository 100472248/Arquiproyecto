#include <cmath>
#include <string>
#include "math.hpp"
#include "files.hpp"
#include "grid.hpp"
#include <numbers>
#include "particle.hpp"
/*Calculamos la longitud de suavizado. Notese que, h al ser una variable global declarada en el archivo header,
mantendra su valor para siempre a lo largo del programa una vez esta funcion sea declarada (se presume que en principio,
solo una vez)*/
double longitud_suavizado (double ppm){
  double h = R/ppm; ///Recordemos que R ya es una constante global conocida en el header
  std::cout << "Smoothing length: " << h << std::endl;
  return h;
}
/*Esta funcion sera devuelta al main como un array fijo de 3 posiciones para poder invocar al constructor del grid*/
std::array<int,3> calc_n_blocks(double ppm){
  double h = longitud_suavizado(ppm); ///Como se anticipo previamente, h ya se le ha puesto valor aqu
  int n_x = std::floor((BMAX[0]-BMIN[0])/h);
  int n_y = std::floor((BMAX[1]-BMIN[1])/h);
  int n_z = std::floor((BMAX[2]-BMIN[2])/h);
  std::array<int,3> resultado = {n_x,n_y,n_z};
  std::cout << "Grid size: " << n_x << " x " << n_y << " x " << n_z << std::endl;
  std::cout << "Number of blocks: " << n_x * n_y * n_z << std::endl;
  return resultado;
}

std::array<double,3> calc_size_blocks(std::array<int,3> medidas_grid){
    double sx = (BMAX[0]-BMIN[0])/medidas_grid[0];
    double sy = (BMAX[1]-BMIN[1])/medidas_grid[1];
    double sz = (BMAX[2]-BMIN[2])/medidas_grid[2];
    std::array<double,3> resultado = {sx, sy, sz};
    std::cout << "Block size: " << sx << " x " << sy << " x " << sz << std::endl;
    return resultado;
}

std::array<int, 3>posicion_particula(double px, double py, double pz, std::array<double,3> size_block){
    int x = std::floor((px-BMIN[0])/size_block[0]);
    int y = std::floor((py-BMIN[1])/size_block[1]);
    int z = std::floor((pz-BMIN[2])/size_block[2]);
    std::array<int,3> resultado = {x, y, z};
    return resultado;
}

double calc_masa (double ppm){
    double masa = RO / pow(ppm,3);
    std::cout << "Particle mass: "<< masa << std::endl;
    return masa;
}

double increase_density(double h, std::array <double, 3> pi, std::array <double, 3> pj) {
    /*Incremento de las densidades*/
    double h2 = pow(h, 2);
    double resta_x = pi[0] - pj[0];
    double resta_y = pi[1] - pj[1];
    double resta_z = pi[2] - pj[2];
    double v_abs = sqrt(pow(resta_x, 2) + pow(resta_y, 2) + pow(resta_z, 2));
    double aumento = 0;
    if (h2 > v_abs) {
        aumento = pow(fabs(h2 - v_abs), 3);
    }
    return aumento;
}
double transform_density(double h, double m, double Ri){
    /*Transformación de las densidad Ri*/
    double h6 = pow(h, 6);
    double h9 = pow(h, 9);
    Ri = (Ri + h6)*(315*m)/(64*PI*h9);
    return Ri;
}

std::array<double, 3> increase_accerelation(std::array <double, 2> h_y_m , Particle pi, Particle pj,
                                            std::array <double, 2> densidades){
    double h = h_y_m[0];
    std::vector<double>posi = pi.get_position();
    std::vector<double>posj = pj.get_position();
    std::vector<double>vi = pi.get_speed();
    std::vector<double>vj = pj.get_speed();
    double h2 = pow(h, 2);
    double resta_x = posi[0] - posj[0];
    double resta_y = posi[1] - posj[1];
    double resta_z = posi[2] - posj[2];
    double v_abs = sqrt(pow(resta_x, 2) + pow(resta_y, 2) + pow(resta_z, 2));
    std::array<double, 3>  aumento = {0,0,0};
    if (h2 > v_abs) {
        double m = h_y_m[1];
        double di = densidades[0];
        double dj = densidades[1];
        double maximo = sqrt(std::max(v_abs, pow(10, -12)));
        double h6 = pow(h, 6);
        aumento[0] = ((posi[0]-posj[0])*(15/(PI*h6))*1.5*m*PS*(pow(h-maximo, 2)/maximo)*(di+dj-2*RO) + (vi[0] + vj[0])*(45/(PI*h6))*m*MU)/(di + dj);
        aumento[1] = ((posi[1]-posj[1])*(15/(PI*h6))*1.5*m*PS*(pow(h-maximo, 2)/maximo)*(di+dj-2*RO) + (vi[1] + vj[1])*(45/(PI*h6))*m*MU)/(di + dj);
        aumento[2] = ((posi[2]-posj[2])*(15/(PI*h6))*1.5*m*PS*(pow(h-maximo, 2)/maximo)*(di+dj-2*RO) + (vi[2] + vj[2])*(45/(PI*h6))*m*MU)/(di + dj);}
    return aumento;}