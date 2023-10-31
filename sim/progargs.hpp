#ifndef ARQUIPROYECTO_PROGARGS_HPP
#define ARQUIPROYECTO_PROGARGS_HPP

#include <vector>
#include <string>
#include <fstream>
int const FIVE = 5;
int analizeArgs(int numArgumentos, std::vector<std::string> argumentos);
bool check_int(std::string argument);
bool open_file(const std::string& fileName);
int check_np(int np, int leidas);

#endif