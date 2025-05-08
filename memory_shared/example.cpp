#include <iostream>
#include "shared_memory.hpp"

int main() {
    SharedMemory mem;

    // Escribir y leer
    mem.write_mem(0, 1234);
    mem.write_mem(4, 5678);
    mem.write_mem(8, 9999);

    std::cout << "mem[0] = " << mem.read_mem(0) << std::endl;
    std::cout << "mem[4] = " << mem.read_mem(4) << std::endl;
    std::cout << "mem[8] = " << mem.read_mem(8) << std::endl;

    // Prueba de dirección inválida
    try {
        mem.write_mem(3, 42); // Dirección desalineada
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}


