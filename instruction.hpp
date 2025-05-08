#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP
#include <cstdint>

struct instruction {
    int opcode;// 0: WRITE_MEM, 1: READ_MEM, etc.
    uint8_t src = 0xFF; // num del PE
    uint64_t addr = 0;
    uint32_t size = 0;
    uint32_t num_of_cache_lines = 0;
    uint32_t start_cache_line = 0;
    uint32_t cache_line = 0;
    uint8_t dest = 0xFF; // para INV_COMPLETE y RESP
    uint32_t data; // opcional
    uint8_t status = 0xFF; // OK o NOT_OK
    uint8_t qos = 0xFF;
    // prioridad
};

#endif