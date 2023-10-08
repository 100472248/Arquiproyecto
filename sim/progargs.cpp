#include "progargs.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>


/*En esta funcion, se comprueba el primer caracter: si es negativo o digito,
se sigue comparando, teniendo 2 bifurcaciones:
        a: es digito {
                a.1. en algun momento no es digito: return false
                a.2  se completa la iteracion: return true
                }
        b: no es digito: return false
*/
bool check_int(std::string argument) {
  if (std::isdigit(argument[0]) || argument[0]=='-'){
    int index = 0;
    for (char c: argument) {
      if (index > 0) {
          if (!std::isdigit(c)) return false;
        }
      index ++;
    }
    return true;
  }
  return false;
}
/*Funcion solo para abrir el fichero y comprobar que todo esta perfecto*/
bool open_file(std::string fileName){
  std::ifstream fichero(fileName, std::ios::binary);
  if (!fichero) {
    return false;
  }
  fichero.close();
  return true;
}
/*Trata de argumentos*/
int analizeArgs(int numArgumentos, std::vector<std::string> argumentos){
  if (numArgumentos!=3){
    std::cerr<<"Error: Invalid number of arguments: " << numArgumentos << std::endl;
    return -1;
  }
  if (!check_int(argumentos[0])){
    std::cerr<<"Error: time steps must be numeric."<< std::endl;
    return -1;
  }
  int iteraciones = std::stoi(argumentos[0]);
  if (iteraciones <= 0){
    std::cerr<<"Error: Invalid number of time steps."<< std::endl;
    return -2;
  }
  if (!open_file(argumentos[1])){
    std::cerr<<"Error: Cannot open "<<argumentos[1]<<" for reading"<<std::endl;
    return -3;
  }
  if (!open_file(argumentos[2])){
    std::cerr<<"Error: Cannot open "<<argumentos[1]<<" for writing"<<std::endl;
    return -4;
  }
  return 0; ///Todo ha ido bien
}

