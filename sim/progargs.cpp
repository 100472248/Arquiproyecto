

#include "progargs.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
bool check_int(std::string argument) {
    if ((std::isdigit(argument[0]) != 0) || argument[0]=='-'){
        for (int i = 1; i < argument.length(); i++) {
            if (!std::isdigit(argument[i])) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool open_file(const std::string& fileName){
    std::ifstream fichero(fileName, std::ios::binary);
    if (!fichero) {
        return false;
    }
    fichero.close();
    return true;
}
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
    return 0;
}



