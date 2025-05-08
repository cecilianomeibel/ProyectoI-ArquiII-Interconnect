#include <iostream>
#include <vector>
#include <thread>
#include <atomic>

class Cache {
private:
    int cacheSize;                  // Tamaño total de cache 128*16 = 2048 bytes
    int blockSize;                  // Bloque de cache 16 bytes 
    std::vector<int> cacheLines;    // vector que representa las lineas de cache (128)

public:
    Cache() : cacheSize(2048), blockSize(16) {
        cacheLines.resize(128, -1);  // -1 indica que están vacías
    }

    // Método para obtener el tamaño del bloque de caché
    int getBlockSize() const {
        return blockSize;
    }

    bool access(int address) {
        int blockIndex = (address / blockSize) % cacheLines.size();
        if (cacheLines[blockIndex] == address) {
            return true;  // Hit
        } else {
            cacheLines[blockIndex] = address;  // Miss
            return false;
        }
    }

    // Ver contenido actual de cada línea de caché
    void printCache() const {
        for (int i = 0; i < cacheLines.size(); ++i) {
            std::cout << "Cache[" << i << "] = " << cacheLines[i] << std::endl;
        }
    }

    void invalidateLineByAddress(int address) {
    int blockIndex = (address / blockSize) % cacheLines.size();
    if (cacheLines[blockIndex] == address) {
        cacheLines[blockIndex] = -1;  // Invalidar
        std::cout << "[CACHE] Línea " << blockIndex
                  << " invalidada para dirección 0x" << std::hex << address << std::dec << "\n";
    }
}


};
