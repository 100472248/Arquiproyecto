#include <iostream>
#include <vector>
#include <string>
#include <array>
#include "../sim/progargs.hpp"
#include "../sim/math.hpp"
#include "../sim/grid.hpp"
#include "../sim/files.hpp"
int main(int argc, char* argv[]) {
  std::vector<std::string> argumentos;
  for (int i = 1; i < argc; i++) argumentos.push_back(argv[i]); ///Nos olvidamos del problema de punteros de argv pasandolo todo a un vector
  if (int retorno = analizeArgs(argc - 1, argumentos); retorno == 0) std::cout << "Bien!"; ///Mensaje de depuracion
  else return retorno; ///Devolver codigo de error
  Datos_cabecera datos_grid = read_header_file(argumentos[1]); ///Struct de los datos de la cabecera. Estos seran usados para calculos
  std::array<int,3> bloques;
  bloques = calc_n_blocks(datos_grid.ppm); ///Calculamos ppm de ese struct de datos de cabecera y almacenamos esos datos en el array bloques
  Grid grid(bloques[0],bloques[1],bloques[2]); ///Con el array bloques creamos el objeto 'grid'
  return 0;
}