#ifndef INTERPRETE_H
#define INTERPRETE_H

#include <vector>
#include <map>
#include <cstdint>
#include <cstddef>

struct instruction {
    int opcode;
    uint8_t src;
    uint64_t addr;
    uint32_t size;
    uint32_t num_of_cache_lines;
    uint32_t start_cache_line;
    uint32_t cache_line;
    uint8_t dest;
    uint32_t data;
    uint8_t status;
    uint8_t qos;
};

// Declaraciones de funciones
int analicer_opcode(char *text);
void parse_instruction(char* line, instruction* inst);
std::map<uint8_t, std::vector<instruction>> agruparPorPE(instruction* programa, size_t totalInstrucciones);
instruction* interpretate(const char* path);

#endif