#include <iostream>
#include <cstdint>
#include "interconnect/interconnect.hpp"
#include "PE.cpp"
#include "interprete.hpp"
#include "clk/clk.hpp"

int main()
{
    int size;
    instruction* program = interpretate("code_test.txt", &size);
    Procesador pe0;
    Procesador pe1;
    Procesador pe2;
    Procesador pe3;
    Procesador pe4;
    Procesador pe5;
    Procesador pe6;
    Procesador pe7;
    for (int i = 0; i < size; ++i) {
        switch (program[i].src){
        case 0:
            pe0.agregarInstruccion(program[i]);
            break;
        case 1:
            pe1.agregarInstruccion(program[i]);
            break;
        case 2:
            pe2.agregarInstruccion(program[i]);
            break;
        case 3:
            pe3.agregarInstruccion(program[i]);
            break;
        case 4:
            pe4.agregarInstruccion(program[i]);
            break;
        case 5:
            pe5.agregarInstruccion(program[i]);
            break;
        case 6:
            pe6.agregarInstruccion(program[i]);
            break;
        case 7:
            pe7.agregarInstruccion(program[i]);
            break;
        
        default:
            break;
        }
    }

    std::thread clk(run_clk);
    clk.join();

    for(int i=0; i<5; i++){

        std::cout << "PE src: " << (int)Interconnect::getInstance().fifo.getMsg().src << std::endl;
    }
    return 0;
}
