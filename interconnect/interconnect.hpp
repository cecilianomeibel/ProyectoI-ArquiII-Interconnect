#ifndef INTERCONNECT_HPP
#define INTERCONNECT_HPP
#include <cstdint>
#include <iostream>
#include "FIFO.cpp"
#include "Priority.cpp"
#include "msg.hpp"
#include "../memory_shared/shared_memory.hpp"

/**
 * @class Interconnect
 * @brief Modela un medio de comunicacion entre procesadores.
 * 
 * Esta clase proporciona una interfaz para enviar y recibir mensajes
 * que provienen de los procesadores que componen el sistema.
 */
class Interconnect {
    public:
        static Interconnect& getInstance();
        /**
         * @brief Mensajes que los procesadores pueden enviar.
         */
        void write_mem(uint8_t SRC, uint64_t ADDR, uint32_t NUM_OF_CACHE_LINES, uint32_t START_CACHE_LINE, uint8_t QoS);
        void read_mem(uint8_t SRC, uint64_t ADDR, uint32_t SIZE, uint8_t QoS);
        void broadcast_invalidate(uint8_t SRC, uint32_t CACHE_LINE, uint8_t QoS);
        void inv_ack(uint8_t SRC, uint8_t QoS);
        void inv_complete(uint8_t DEST, uint8_t QoS);
        void read_resp(uint8_t DEST, uint32_t DATA, uint8_t QoS);
        void write_resp(uint8_t DEST, uint8_t STATUS, uint8_t QoS);
        void set_scheduler(int sched);
        Priority priority;
        FIFO fifo;
        
        
    private:
        Interconnect();
        int scheduler; //selector FIFO o PRIORITY
        int counter;
        void schedule(msg message);

};


#endif