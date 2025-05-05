#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include "Cache.cpp"
#include "PE.cpp"

int main() {
    instruction* instrucciones = interpretate("code_test.txt");
    if (!instrucciones) return 1;

    Procesador cpu;

    for (int i = 0; instrucciones[i].opcode != -1; ++i) {
        cpu.ejecutarInstruccion(instrucciones[i]);
    }

    return 0;
}