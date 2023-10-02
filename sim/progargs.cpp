#include <iostream>
#include "progargs.hpp"
#include <fstream>
int analizeArgs(int numArgumentos, std::vector argumentos){
    if (numArgumentos!=3){
        std::cerr<<"Error: Invalid number of arguments: " << numArgumentos << std:endl;
        return -1;
    }
    if (sizeof(argumentos[0])!=4){
        std::cerr<<"Error: time steps must be numeric."<< std:endl;
        return -2;
    }
    if (sizeof(argumentos[0])< 0){
        std::cerr<<"Error: Invalid number of time steps."<< std:endl;
        return -2;
    }
    if (!open_file(argumentos[1])){
        std::cerr<<"Error: Cannot open "<<argumentos[1]<<" for reading"<<std::endl;
        return -3;
    }
    if (!open_file(argumentos[1])){
        std::cerr<<"Error: Cannot open "<<argumentos[1]<<" for reading"<<std::endl;
        return -4;
    }
}

int open_file(std::string fileName){
    std::fstream fstream(nombre_archivo, std::ios::binary);
    if (!fstream) {
        return -1;
    }
    fstream.close();
    return 0;
}



