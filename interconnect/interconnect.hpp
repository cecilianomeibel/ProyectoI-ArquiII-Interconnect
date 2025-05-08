#ifndef INTERCONNECT_HPP
#define INTERCONNECT_HPP
#include <cstdint>
#include "FIFO.cpp"
#include "Priority.cpp"
#include "msg.hpp"

class Interconnect {
    public:
        Interconnect();
        void WRITE_MEM(uint8_t SRC, uint32_t NUM_OF_CACHE_LINES, uint32_t START_CACHE_LINE, uint8_t QoS);
        void READ_MEM(uint8_t SRC, uint64_t ADDR, uint32_t SIZE, uint8_t QoS);
        void BROADCAST_INVALIDATE(uint8_t SRC, uint32_t CACHE_LINE, uint8_t QoS);
        void INV_ACK(uint8_t SRC, uint8_t QoS);
        void INV_COMPLETE(uint8_t DEST, uint8_t QoS);
        void READ_RESP(uint8_t DEST, uint32_t DATA, uint8_t QoS);
        void WRITE_RESP(uint8_t DEST, uint8_t STATUS, uint8_t QoS);
        //testing
        

    private:
        Priority priority;
        FIFO fifo;
        int scheduler; //selector FIFO o PRIORITY
        bool clk; //por implementar
        void schedule(msg message);

};


#endif