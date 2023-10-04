#ifndef ARQUIPROYECTO_PROGARGS_HPP
#define ARQUIPROYECTO_PROGARGS_HPP

#include <vector>
#include <string>
#include <fstream>
int analizeArgs(int numArgumentos, std::vector<std::string> argumentos);
bool check_int(std::string argument);
bool open_file(std::string fileName);

#endif