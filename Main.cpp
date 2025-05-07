#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include "Cache.cpp"
#include "PE.cpp"
#include "interprete.cpp"


 int main() {
    
    instruction* program = interpretate("code_test.txt");
    int count = 16;
    Procesador cpu;

    for (int i = 0; i < count; ++i) {
        cpu.ejecutarInstruccion(program[i]);
    }

    return 0;
}

