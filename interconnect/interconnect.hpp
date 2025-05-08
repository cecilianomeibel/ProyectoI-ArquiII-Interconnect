#ifndef INTERCONNECT_HPP
#define INTERCONNECT_HPP
#include <cstdint>
#include "FIFO.cpp"
#include "Priority.cpp"
#include "msg.hpp"

class Interconnect {
    public:
        static Interconnect& getInstance();
        void write_mem(uint8_t SRC, uint64_t ADDR, uint32_t NUM_OF_CACHE_LINES, uint32_t START_CACHE_LINE, uint8_t QoS);
        void read_mem(uint8_t SRC, uint64_t ADDR, uint32_t SIZE, uint8_t QoS);
        void broadcast_invalidate(uint8_t SRC, uint32_t CACHE_LINE, uint8_t QoS);
        void inv_ack(uint8_t SRC, uint8_t QoS);
        void inv_complete(uint8_t DEST, uint8_t QoS);
        void read_resp(uint8_t DEST, uint32_t DATA, uint8_t QoS);
        void write_resp(uint8_t DEST, uint8_t STATUS, uint8_t QoS);
        //testing
        Priority priority;
        FIFO fifo;
        
        
    private:
        Interconnect();
        int scheduler; //selector FIFO o PRIORITY
        bool clk; //por implementar
        void schedule(msg message);

};


#endif