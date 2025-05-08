#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "Cache.cpp"
#include "instruction.hpp"
#include "clk/shared.hpp"
#include "interconnect/interconnect.hpp"


/**
 * @file Procesador.cpp
 * @brief Implementación de un procesador simulado (PE) que ejecuta instrucciones en base a un archivo interpretado.
 *        Cada procesador contiene su propio hilo de ejecución, caché y tiempos simulados para eventos como cache hit, miss,
 *        transferencias de memoria, broadcasts, invalidaciones, y escritura/lectura en memoria.
 *
 * Instrucciones válidas son:
 *   - WRITE_MEM src, addr, num_lines, start_line, qos
 *   - READ_MEM src, addr, size, qos
 *   - BROADCAST_INVALIDATE src, cache_line, qos
 *   - INV_ACK src, qos
 *   - INV_COMPLETE dest, qos  (no implementada en este archivo)
 *   - READ_RESP dest, data, qos
 *   - WRITE_RESP dest, status, qos
 *
 */

class Procesador {
private:
    uint64_t tiempoActual;  // tiempo simulado (en ciclos)
    uint64_t latenciaCacheHit = 3;
    uint64_t latenciaCacheMiss = 6;
    uint64_t ciclosTransferenciaPorByte = 5;
    uint64_t ciclosBroadcast = 15;
    uint64_t ciclosInvAck = 10;  
    uint64_t ciclosWriteMem = 50;
    uint64_t pc;                           // Contador de programa
    
    Cache cache;  
    std::thread hilo;          // El hilo que ejecutará el procesador
    std::atomic<bool> activo;  // Para controlar el ciclo de vida del hilo
    
    std::queue<instruction> colaInstrucciones;
    
    void run() {
        while (activo) {
            instruction instr;
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this](){ return ready; });
                if(colaInstrucciones.empty()){
                    activo = false;
                    //finish = true; 
                    break;
                }
                ready = false;
                
                if (!activo) break;
                
                instr = colaInstrucciones.front();
                colaInstrucciones.pop();
            }
            
            ejecutarInstruccion(instr);
        }
    }
    

public:
    Procesador() : tiempoActual(0), activo(true), pc(0) {
        hilo = std::thread(&Procesador::run, this);
    }



   /**
   *@brief Agrega una instrucción a la cola del procesador.
   * @param instr: Instrucción a ejecutar */

    void agregarInstruccion(const instruction& instr) {
        colaInstrucciones.push(instr);
    }


    /**
    * @brief Ejecuta la instrucción recibida según su opcode.
    * @param instr: Instrucción a ejecutar*/

    void ejecutarInstruccion(const instruction& instr) {
        switch (instr.opcode) {
            case 0: // WRITE_MEM
                WRITE_MEM(instr.src, instr.addr, instr.num_of_cache_lines, instr.start_cache_line, instr.qos);
                break;
            case 1: // READ_MEM
                READ_MEM(instr.src, instr.addr, instr.size, instr.qos); 
                break;
            case 2: // BROADCAST_INVALIDATE 
                BROADCAST_INVALIDATE(instr.src, instr.cache_line, instr.qos); 
                break;
            case 3:  // INV_ACK 
                INV_ACK(instr.src, instr.qos);
                break;
            case 4: // INV_COMPLETE 
                //INV_COMPLETE(instr.dest, instr.qos);
                break;
            case 5: // READ_RESP 
                READ_RESP(instr.dest,instr.data, instr.qos);
                break;
            case 6: // WRITE_RESP
                WRITE_RESP(instr.dest, instr.status, instr.qos);
                break;
            default:
                std::cerr << "[ERROR] Opcode no reconocido: " << instr.opcode << "\n";
                break;
        }
    }


    /**
    * @brief Simula el acceso a la memoria con latencias de HIT o MISS.
    * @param direccion: Dirección de memoria a acceder.
    * @param tam: Tamaño de los datos a acceder en bytes.
    * @param tipo: Tipo de operación ("READ", "WRITE"....).*/

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


    /**
    * @brief Ejecuta una espera simulada basada en el reloj de simulación.
    * @param time: Número de ciclos de reloj que debe esperar*/

    void exec_waiting(uint64_t time){
        int count = 0;
        while(count != time){
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this](){ return ready; });  
            
            if(clock_signal.load() == 1){
                count++;
            }
            ready = false;
        }
    }


    //-----------------
    //FUNCION WRITE_MEM   
    //-----------------

    /**
    * @brief Maneja una instrucción WRITE_MEM, simulando el acceso al caché y escritura a memoria.
    * @param src: Identificador del PE origen.
    * @param addr: Dirección de memoria donde se escribe.
    * @param numOfCacheLines: Cantidad de líneas de caché a escribir.
    * @param startCacheLine: Linea de cache en la que se inicia a escribir
    * @param qos: Prioridad*/

    void WRITE_MEM(uint8_t src, uint64_t addr, uint32_t numOfCacheLines, uint32_t startCacheLine, uint8_t qos) {
        Interconnect::getInstance().write_mem(src, addr, numOfCacheLines, startCacheLine, qos);
        // Se asume que datos a escribir provienen del caché del PE fuente
        std::cout << "[WRITE_MEM] PE #" << (int)src
                  << " escribiendo en dir 0x" << std::hex << addr
                  << " con QoS: " << std::dec <<  (int)qos
                  << " utilizando " << numOfCacheLines
                  << " líneas de caché, comenzando desde la línea "
                  << startCacheLine << "\n";

        for (uint32_t i = 0; i < numOfCacheLines; ++i) {
            bool hit = cache.access(addr + i * cache.getBlockSize());  // Asumiendo que cada línea de caché tiene un bloque
            if (hit) {
                tiempoActual += latenciaCacheHit;
                std::cout << "[WRITE_MEM] HIT en dir 0x" << std::hex << addr + i * cache.getBlockSize()   
                          << " => Tiempo: " << std::dec << tiempoActual << " ciclos\n";
                exec_waiting(latenciaCacheHit);
                
            } else {
                tiempoActual += latenciaCacheMiss;
                tiempoActual += ciclosTransferenciaPorByte * cache.getBlockSize();
                std::cout << "[WRITE_MEM] MISS en dir 0x" << std::hex << addr + i * cache.getBlockSize()
                          << " => Tiempo: " << std::dec << tiempoActual << " ciclos\n";
                exec_waiting(latenciaCacheMiss + ciclosTransferenciaPorByte * cache.getBlockSize());
            }
        }

        // Agregar el tiempo de escritura en memoria
        tiempoActual += ciclosWriteMem;
        exec_waiting(ciclosWriteMem);
        std::cout << "[WRITE_MEM] Finalizó la escritura en memoria. Tiempo total: " << tiempoActual << " ciclos\n";
    }


    //-----------------
    //FUNCION READ_MEM
    //-----------------

    /**
    * @brief Maneja una instrucción READ_MEM, simulando lectura desde memoria con caché.
    * @param src: Identificador del PE origen.
    * @param addr: Dirección de memoria a leer.
    * @param size: Tamaño de los datos a leer (en bytes).
    * @param qos: Prioridad
    */

    void READ_MEM(uint8_t src, uint64_t addr, uint32_t size, uint8_t qos) {
        Interconnect::getInstance().read_mem(src, addr, size, qos);
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
                exec_waiting(latenciaCacheHit);
            } else {
                // Si es un MISS, suma la latencia del MISS y el tiempo de transferencia de datos
                tiempoActual += latenciaCacheMiss;
                tiempoActual += ciclosTransferenciaPorByte * blockSize;
                std::cout << "[READ_MEM] MISS en dir 0x" << std::hex << addr + i * blockSize
                          << " => Tiempo: " << std::dec << tiempoActual << " ciclos\n";
                exec_waiting(latenciaCacheMiss);
            }
        }

        std::cout << "[READ_MEM] Datos leídos y almacenados en la caché del PE #" << (int)src << "\n";
    }


    //----------------------------
    //FUNCION BROADCAST_INVALIDATE
    //----------------------------

    /**
    * @brief Envía un broadcast para invalidar una línea de caché en todos los procesadores.
    * @param src: Identificador del PE origen.
    * @param cache_line: Línea de caché a invalidar.
    * @param qos: Prioridad.*/

    void BROADCAST_INVALIDATE(uint8_t src, uint32_t cache_line, uint8_t qos) {
        Interconnect::getInstance().broadcast_invalidate(src, cache_line, qos);
        std::cout << "[BROADCAST_INVALIDATE] PE #" << (int)src 
        << " invalida la línea de caché: " << cache_line
        << " en todos los PEs con QoS: " << (int)qos 
        << " => Tiempo inicial: " << tiempoActual << " ciclos\n";
        
        tiempoActual += ciclosBroadcast; // Tiempo para enviar el broadcast a todos los PEs
        exec_waiting(ciclosBroadcast);
        std::cout << "[BROADCAST_INVALIDATE] Invalidando línea " << cache_line  //Simular la invalidación de la línea de caché específica
        << " en todos los PEs. Tiempo: " << tiempoActual << " ciclos\n";
        std::cout << "[BROADCAST_INVALIDATE] Invalidación completada. Tiempo total: " 
        << tiempoActual << " ciclos\n";
    }
    

    //---------------
    //FUNCION INV_ACK
    //---------------
    
    /**
    * @brief Procesa una respuesta de invalidación (ACK) desde otro procesador.
    * @param src: Identificador del PE que responde.
    * @param qos: Prioridad.*/

    void INV_ACK(uint8_t src, uint8_t qos) {
        Interconnect::getInstance().inv_ack(src, qos);
        std::cout << "Respuesta de invalidación recibida desde PE #" << (int)src
        << " con QoS: " << (int)qos << " Tiempo: " << tiempoActual << " ciclos\n";
        tiempoActual += ciclosInvAck;   //añadir aqui *numPEs
        exec_waiting(ciclosInvAck);
    }


    //--------------------
    //FUNCION READ_RESP
    //--------------------

    /**
    * @brief Simula la recepción de datos desde memoria hacia la caché del PE.
    * @param dest: Identificador del PE destino.
    * @param data: Dirección de los datos que se almacenan.
    * @param qos: Prioridad.*/

    void READ_RESP(uint8_t dest, uint32_t data, uint8_t qos) {
        std::cout << "[READ_RESP] Recibiendo datos desde memoria para PE #" << (int)dest
        << " desde dirección 0x" << std::hex << data
        << " con QoS: " << (int)qos << "\n";

        // Simula que estos datos (1 bloque) llegan desde memoria y se almacenan en caché
        uint32_t blockSize = cache.getBlockSize();
        tiempoActual += ciclosTransferenciaPorByte * blockSize;
        exec_waiting(ciclosTransferenciaPorByte * blockSize);
        cache.access(data);  // Asume que se ha accedido/cargado el bloque
        std::cout << "[READ_RESP] Datos almacenados en la caché. Tiempo total: "
        << tiempoActual << " ciclos\n";
    }

     //---------------------
    //FUNCION WRITE_RESP
   //---------------------
   /**
   * @brief Procesa la respuesta de una operación de escritura.
   * @param dest: PE destino.
   * @param status: Estado de la operación (OK, NOT_OK).
   * @param qos: Prioridad */

    void WRITE_RESP(uint8_t dest, uint8_t status, uint8_t qos) {
        std::string estado = (status == 0x1) ? "OK" : "NOT_OK";
        std::cout << "[WRITE_RESP] Respuesta a escritura recibida por PE #" << (int)dest
        << " => estado: " << estado
        << " (0x" << std::dec << (int)status << "), QoS: " << (int)qos
        << " => Tiempo: " << tiempoActual << " ciclos\n";
        tiempoActual += 5; // 5 ciclos por procesar la respuesta
        exec_waiting(5);
    }


    /**
    * @brief Obtiene el tiempo simulado actual del procesador.
    * @return Tiempo actual en ciclos.*/

    uint64_t getTiempoActual() const {
        return tiempoActual;
    }

    /**
    * @brief Imprime el estado de la caché del procesador*/

    void printCache() const {
        cache.printCache();
    }

    /**
    * @brief Reinicia el tiempo simulado a cero*/

    void resetTiempo() {
        tiempoActual = 0;
    }

    /**
    * @brief Invalida una línea de caché dada su dirección.
    * @param address: Dirección de memoria asociada a la línea a invalidar*/

    void invalidateLine(uint64_t address) {
        cache.invalidateLineByAddress(address);
    }


    /**
    * @brief Detiene el hilo de ejecución del procesador*/

    void detener() {
        activo = false;
        cv.notify_all(); 
        if (hilo.joinable()) {
            hilo.join();
        }
    }


    /**
    * @brief Destructor del procesador. Asegura que el hilo se detiene limpiamente.*/

    ~Procesador() {
        detener();
    }  
};
  