#include <iostream>
#include <vector>
#include "PE.cpp"

class InstructionMemory {
private:
    std::vector<instruction> instrucciones;

public:
    InstructionMemory(instruction* program, size_t count) {
        instrucciones.assign(program, program + count);
    }

    const instruction& fetch(size_t pc) const {
        if (pc < instrucciones.size())
            return instrucciones[pc];
        static instruction nop = {}; // Instrucción vacía o NOP
        return nop;
    }

    size_t size() const {
        return instrucciones.size();
    }
};

