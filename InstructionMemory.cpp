#include "InstructionMemory.h"
#include <iostream>

InstructionMemory::InstructionMemory(uint8_t id, const std::vector<instruction>& inst) 
    : peId(id), instrucciones(inst) {}

const instruction& InstructionMemory::fetch(size_t pc) const {
    if (pc < instrucciones.size())
        return instrucciones[pc];
    static instruction nop = {.opcode = -1};
    return nop;
}

size_t InstructionMemory::size() const {
    return instrucciones.size();
}

void InstructionMemory::print() const {
    std::cout << "Memoria de Instrucciones del PE #" << (int)peId << ":\n";
    for (size_t i = 0; i < instrucciones.size(); i++) {
        std::cout << "  [" << i << "] Opcode: " << instrucciones[i].opcode 
                  << ", Addr: 0x" << std::hex << instrucciones[i].addr << std::dec << "\n";
    }
}
