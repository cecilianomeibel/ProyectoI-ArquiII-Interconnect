#ifndef INSTRUCTIONMEMORY_H
#define INSTRUCTIONMEMORY_H

#include "interprete.cpp"
#include <vector>

class InstructionMemory {
private:
    std::vector<instruction> instrucciones;
    uint8_t peId; // ID del PE (igual al src)

public:
    InstructionMemory(uint8_t id, const std::vector<instruction>& inst);
    const instruction& fetch(size_t pc) const;
    size_t size() const;
    uint8_t getId() const { return peId; }
    void print() const;
};

#endif