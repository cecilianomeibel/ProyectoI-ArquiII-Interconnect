#ifndef INTERPRETE_HPP
#define INTERPRETE_HPP

#include <cstdio> 
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cstddef>
#include "instruction.hpp"

// Declaraciones de funciones
int analicer_opcode(char *text);
void parse_instruction(char* line, instruction* inst);
instruction* interpretate(const char* path, int* size);

#endif