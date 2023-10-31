#include <iostream>
#include <vector>
#include <string>
#include <array>
#include "../sim/progargs.hpp"
#include "../sim/math.hpp"
#include "../sim/grid.hpp"
#include "../sim/files.hpp"
#include <chrono>

int main(int argc, char* argv[]) {
    auto start_time = std::chrono::high_resolution_clock::now();
  std::vector<std::string> argumentos;
  for (int i = 1; i < argc; i++) { argumentos.emplace_back(argv[i]); ///Nos olvidamos del problema de punteros de argv pasandolo
}
  int retorno = analizeArgs(argc - 1, argumentos);
  if (retorno != 0) {
      return retorno; } ///Devolver codigo de error
  Datos_cabecera datos_grid = read_header_file(argumentos[1]); ///Struct de los datos de la cabecera. Estos seran usados para calculos
  std::array<int,3> bloques = calc_n_blocks(datos_grid.ppm); ///Calculamos ppm de ese struct de datos de cabecera y almacenamos esos datos en el array bloques
  Grid grid(bloques[0],bloques[1],bloques[2]); ///Con el array bloques creamos el objeto 'grid'
  std:: array<double, 3> medidas_bloque = calc_size_blocks(bloques);
  grid.set_block_size(medidas_bloque);
  int num_id = Read_particles(argumentos[1], grid, datos_grid.ppm);
  retorno = check_np(datos_grid.np, num_id);
  if (retorno != 0) {
      return retorno;}
  grid.simulation(stoi(argumentos[0]), datos_grid.ppm);
  auto end_time = std::chrono::high_resolution_clock::now();
  // Calcula la duración de la ejecución
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
  // Imprime el tiempo de ejecución en milisegundos
  std::cout << "Tiempo de ejecución: " << duration.count() << " ms" << '\n';
  return 0;
}