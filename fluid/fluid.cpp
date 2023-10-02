#include <iostream>
#include <vector>
#include <string>
#include "../sim/progargs.hpp"
int main(int argc, char* argv[]) {
    std::vector<std::string> argumentos;

    // Recorremos argv y añadimos cada argumento al vector
    for (int i = 1; i < argc; i++) {
        argumentos.push_back(argv[i]);
    }

    for (int i=0; i<argumentos.size(); i++){
        std::cout<<argumentos[i]<<",";
    }
    if (!analizeArgs(argc-1,argumentos)) std::cout<<"Bien!"<<std::endl;
    // El código restante de tu programa aquí
    return 0;
}