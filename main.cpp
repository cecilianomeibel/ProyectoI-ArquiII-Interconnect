#include <iostream>
#include <cstdint>
#include "interconnect/interconnect.hpp"
#include "PE.cpp"
#include "interprete.hpp"

int main(int argc, char const *argv[])
{
    int count = 16;

    instruction* program = interpretate("code_test.txt");
    Procesador pe;
    Interconnect interconnect;
    interconnect.WRITE_MEM(1,1,1,1);
    interconnect.WRITE_MEM(2,2,2,2);
    interconnect.WRITE_MEM(1,1,1,3);
    interconnect.WRITE_MEM(1,1,1,4);

    for (int i = 0; i < count; ++i) {
        pe.agregarInstruccion(program[i]);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    //Mover los calendarizadores a publicos en interconnect.cpp para pruebas.
    /*for(int i=0; i < 4; i++){
        std::cout << "Mensaje FIFO: " << std::dec << (int)interconnect.fifo.getMsg().qos << std::endl;
    }*/
    return 0;
}
