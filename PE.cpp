#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include "Cache.cpp"

class Procesador {
private:
    uint64_t tiempoActual;  // tiempo simulado (en ciclos)
    uint64_t latenciaCacheHit = 3;
    uint64_t latenciaCacheMiss = 6;
    uint64_t ciclosTransferenciaPorByte = 5;
    uint64_t ciclosBroadcast = 50;
    uint64_t ciclosInvAck = 10;  
    uint64_t ciclosWriteMem = 50;

    Cache cache;  

    std::thread hilo;          // El hilo que ejecutará el procesador
    std::atomic<bool> activo;  // Para controlar el ciclo de vida del hilo

    void run() {
        // Ejecución de las instrucciones
        while (activo) {
            // Agregar la lógica para ejecutar instrucciones
        }
    }

public:
    Procesador() : tiempoActual(0), activo(true) {
        // Crea el hilo que ejecuta el método `run`
        hilo = std::thread(&Procesador::run, this);
    }

    // Método que ejecuta una instrucción
    void ejecutarInstruccion(const instruction& instr) {
        switch (instr.opcode) {
            case 0: // WRITE_MEM
                WRITE_MEM(instr.src, instr.addr, instr.num_of_cache_lines, instr.start_cache_line, instr.qos);
                break;
            case 1: // READ_MEM
                READ_MEM(instr.src, instr.addr, instr.size, instr.qos); 
                break;
            case 2: // BROADCAST_INVALIDATE 
                BROADCAST_INVALIDATE(instr.src, instr.num_of_cache_lines, instr.qos); 
                break;
            case 3:  // INV_ACK 
                INV_ACK(instr.src, instr.qos);
                break;
            case 4: // INV_COMPLETE 
                INV_COMPLETE(instr.dest, instr.qos);
                break;
            case 5: // READ_RESP 
                READ_RESP(instr.dest,instr.data, instr);
                break;
            case 6: // WRITE_RESP
                WRITE_RESP(instr.dest, instr.status, instr.qos);
                break;
            default:
                std::cerr << "[ERROR] Opcode no reconocido: " << instr.opcode << "\n";
                break;
        }
    }

    void manejarAccesoMemoria(uint64_t direccion, uint32_t tam, const std::string& tipo) {
        bool hit = cache.access(direccion);
        if (hit) {
            tiempoActual += latenciaCacheHit;
            std::cout << "[" << tipo << "] HIT en dir 0x" << std::hex << direccion
                      << " => Tiempo: " << std::dec << tiempoActual << " ciclos\n";
        } else {
            tiempoActual += latenciaCacheMiss;
            tiempoActual += tam * ciclosTransferenciaPorByte;
            std::cout << "[" << tipo << "] MISS en dir 0x" << std::hex << direccion
                      << ", tam: " << std::dec << tam
                      << " => Tiempo: " << tiempoActual << " ciclos\n";
        }
    } 


    //-----------------
    //FUNCION WRITE_MEM
    //-----------------

    void WRITE_MEM(uint8_t src, uint64_t addr, uint32_t numOfCacheLines, uint32_t startCacheLine, uint8_t qos) {
        // Se asume que datos a escribir provienen del caché del PE fuente
        std::cout << "[WRITE_MEM] PE #" << (int)src
                  << " escribiendo en dir 0x" << std::hex << addr
                  << " con QoS: " << (int)qos
                  << " utilizando " << numOfCacheLines
                  << " líneas de caché, comenzando desde la línea "
                  << startCacheLine << "\n";

        for (uint32_t i = 0; i < numOfCacheLines; ++i) {
            bool hit = cache.access(addr + i * cache.getBlockSize());  // Asumiendo que cada línea de caché tiene un bloque
            if (hit) {
                tiempoActual += latenciaCacheHit;
                std::cout << "[WRITE_MEM] HIT en dir 0x" << std::hex << addr + i * cache.getBlockSize()   
                          << " => Tiempo: " << std::dec << tiempoActual << " ciclos\n";
            } else {
                tiempoActual += latenciaCacheMiss;
                tiempoActual += ciclosTransferenciaPorByte * cache.getBlockSize();
                std::cout << "[WRITE_MEM] MISS en dir 0x" << std::hex << addr + i * cache.getBlockSize()
                          << " => Tiempo: " << std::dec << tiempoActual << " ciclos\n";
            }
        }

        // Agregar el tiempo de escritura en memoria
        tiempoActual += ciclosWriteMem;
        std::cout << "[WRITE_MEM] Finalizó la escritura en memoria. Tiempo total: " << tiempoActual << " ciclos\n";
    }


    //-----------------
    //FUNCION READ_MEM
    //-----------------

    void READ_MEM(uint8_t src, uint64_t addr, uint32_t size, uint8_t qos) {
        std::cout << "[READ_MEM] PE #" << (int)src
                  << " leyendo en dir 0x" << std::hex << addr
                  << " de tamaño " << size << " bytes con QoS: "
                  << (int)qos << "\n";

        uint32_t blocksToRead = (size + cache.getBlockSize() - 1) / cache.getBlockSize();  // Número de bloques a leer

        for (uint32_t i = 0; i < blocksToRead; ++i) {
            uint32_t blockSize = cache.getBlockSize();       // Obtener tamaño del bloque de caché
            bool hit = cache.access(addr + i * blockSize);  // Acceder a la caché con la dirección ajustada por el bloque

            if (hit) {
                // Si hay un HIT en la caché, suma la latencia correspondiente
                tiempoActual += latenciaCacheHit;
                std::cout << "[READ_MEM] HIT en dir 0x" << std::hex << addr + i * blockSize   
                          << " => Tiempo: " << std::dec << tiempoActual << " ciclos\n";
            } else {
                // Si es un MISS, suma la latencia del MISS y el tiempo de transferencia de datos
                tiempoActual += latenciaCacheMiss;
                tiempoActual += ciclosTransferenciaPorByte * blockSize;
                std::cout << "[READ_MEM] MISS en dir 0x" << std::hex << addr + i * blockSize
                          << " => Tiempo: " << std::dec << tiempoActual << " ciclos\n";
            }
        }

        std::cout << "[READ_MEM] Datos leídos y almacenados en la caché del PE #" << (int)src << "\n";
    }

  
    //---------------
    //FUNCION INV_ACK
    //---------------

    void INV_ACK(uint8_t src, uint8_t qos) {
        tiempoActual += ciclosInvAck;
        std::cout << "Respuesta de invalidación recibida desde PE #" << (int)src
                  << " con QoS: " << (int)qos
                  << " => Tiempo: " << tiempoActual << " ciclos\n";
    }



    uint64_t getTiempoActual() const {
        return tiempoActual;
    }

    void printCache() const {
        cache.printCache();
    }

    void resetTiempo() {
        tiempoActual = 0;
    }

    // Detener el procesador y unir el hilo al final
    void detener() {
        activo = false;
        if (hilo.joinable()) {
            hilo.join();
        }
    }

    //Cerrar el hilo
    ~Procesador() {
        detener();
    }
};



