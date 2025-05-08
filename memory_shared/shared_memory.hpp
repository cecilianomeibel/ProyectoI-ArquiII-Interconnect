#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <vector>
#include <cstdint>
#include <iostream>
#include <mutex>

/**
 * @class SharedMemory
 * @brief Simula un bloque de memoria compartida con acceso alineado a palabra (4 bytes).
 * 
 * Esta clase proporciona una interfaz para lectura y escritura de palabras
 * (uint32_t) en una memoria simulada de tamaño fijo. La memoria se alinea
 * automáticamente a múltiplos de 4 bytes y lanza excepciones si la dirección es inválida.
 */
class SharedMemory{
private:
    static const size_t MEM_SIZE = 4096; //Tamano solicitado, size_t le entra 32/64 bits 
    static const size_t BYTES_PER_WORD = 4;//Alineacion xd 
    std::vector<uint32_t> memory; //Memoria, es un vector literal 
    //std::mutex mutex_mem;// Mutex para hacerla compartida 
    SharedMemory() : memory(MEM_SIZE,0) {} // Aca se define la memoria compartida
public: 
    
    static SharedMemory& getInstance(){
        static SharedMemory sharedMemory;
        return sharedMemory; 
    };
    /**
     * @brief Lee una palabra de memoria en una dirección dada.
     * 
     * @param addr Dirección de la cual se desea leer (debe estar alineada a 4 bytes).
     * @return Valor de 32 bits almacenado en la dirección especificada.
     * 
     * @throws std::out_of_range Si la dirección está desalineada o fuera del rango válido.
     */
    uint32_t read(size_t addr){//sacar data por addr
        if(addr % BYTES_PER_WORD != 0 || addr >= MEM_SIZE * BYTES_PER_WORD)
            throw std::out_of_range("Direccion desalineada o fuera de rango");
        return memory[addr / BYTES_PER_WORD]; 

    }
    /**
     * @brief Escribe una palabra de 32 bits en una dirección de memoria.
     * 
     * @param addr Dirección en la que se desea escribir (debe estar alineada a 4 bytes).
     * @param data Valor de 32 bits a escribir en la memoria.
     * 
     * @throws std::out_of_range Si la dirección está desalineada o fuera del rango válido.
     */
    void write(size_t addr, uint32_t data){//meter data por addr
        if( addr % BYTES_PER_WORD != 0 || addr >=  MEM_SIZE*BYTES_PER_WORD)
            throw std::out_of_range("Direccion desalineada o fuera de rango");
        memory[addr/ BYTES_PER_WORD] = data;
        std::cout << "Escritura de datos: " << memory[addr/BYTES_PER_WORD] << std::endl;
    }
};


#endif 