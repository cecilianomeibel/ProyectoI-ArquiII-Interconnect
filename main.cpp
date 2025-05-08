#include <iostream>
#include <cstdint>
#include "interconnect/interconnect.hpp"
#include "PE.cpp"
#include "interprete.hpp"
#include "clk/clk.hpp"

int main(int argc, char const *argv[])
{
    int count = 15;

    instruction* program = interpretate("code_test.txt");
    Procesador pe;

    for (int i = 0; i < count; ++i) {
        pe.agregarInstruccion(program[i]);
    }

    std::thread clk(run_clk);
    clk.join();

    for(int i=0; i<5; i++){

        std::cout << "PE src: " << (int)Interconnect::getInstance().fifo.getMsg().src << std::endl;
    }
    return 0;
}
