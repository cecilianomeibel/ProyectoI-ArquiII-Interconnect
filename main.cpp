#include <iostream>
#include <cstdint>
#include "interconnect/interconnect.hpp"
#include "clk/clk.cpp"

int main(int argc, char const *argv[])
{
    Interconnect interconnect;
    interconnect.WRITE_MEM(1,1,1,1);
    interconnect.WRITE_MEM(2,2,2,2);
    interconnect.WRITE_MEM(1,1,1,3);
    interconnect.WRITE_MEM(1,1,1,4);

    //Mover los calendarizadores a publicos en interconnect.cpp para pruebas.
    /*for(int i=0; i < 4; i++){
        std::cout << "Mensaje FIFO: " << std::dec << (int)interconnect.fifo.getMsg().qos << std::endl;
    }*/
    return 0;
}
