#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <vector>
#include <cstdint>
#include <mutex>


class SharedMemory{
private:
    static const size_t MEM_SIZE = 4096; //Tamano solicitado, size_t le entra 32/64 bits 
    static const size_t BYTES_PER_WORD = 4;//Alineacion xd 
    std::vector<uint32_t> memory; //Memoria, es un vector literal 
    //std::mutex mutex_mem;// Mutex para hacerla compartida 
public: 
    SharedMemory() : memory(MEM_SIZE,0) {} // Aca se define la memoria compartida


    uint32_t read_mem(size_t addr){//sacar data por addr
        if(addr % BYTES_PER_WORD != 0 || addr >= MEM_SIZE * BYTES_PER_WORD)
            throw std::out_of_range("Direccion desalineada o fuera de rango");
        return memory[addr / BYTES_PER_WORD]; 

    }
    void write_mem(size_t addr, uint32_t data){//meter data por addr
        if( addr % BYTES_PER_WORD != 0 || addr >=  MEM_SIZE*BYTES_PER_WORD)
            throw std::out_of_range("Direccion desalineada o fuera de rango");
        memory[addr/ BYTES_PER_WORD] = data;
    }
};


#endif 