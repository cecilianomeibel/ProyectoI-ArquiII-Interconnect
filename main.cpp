#include <iostream>
#include <cstdint>
#include "interconnect/FIFO.cpp"
#include "interconnect/Priority.cpp"

int main(int argc, char const *argv[])
{
    FIFO fifo;
    Priority priority;
    for (uint32_t i = 0; i < 5; i++)
    {
        fifo.addMsg({1, i});
        priority.addMsg({1, i});
    }

    for (int i = 0; i < 5; i++){
        std::cout << "Mensaje FIFO: " << fifo.getMsg().QoS << ", ";
    }

    std::cout << std::endl;
    for (int i = 0; i < 5; i++){
        std::cout << "Mensaje Priority: " << priority.getMsg().QoS << ", ";
    }
    std::cout << std::endl;

    return 0;
}
