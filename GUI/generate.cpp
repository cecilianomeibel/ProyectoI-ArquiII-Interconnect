#include <fstream>
#include <nlohmann/json.hpp>
//sudo apt install nlohmann-json3-dev     //Antes de ejecutar


using json = nlohmann::json;

void export_metrics_to_json(const std::vector<int>& bandwidth,
                            const std::vector<int>& traffic,
                            const std::vector<int>& latency) {
    json j;

    for (int i = 0; i < 8; ++i) {
        j["PE" + std::to_string(i)] = {
            {"bandwidth", bandwidth[i]},
            {"traffic", traffic[i]},
            {"latency", latency[i]}
        };
    }

    std::ofstream file("metrics1.json");
    file << j.dump(4); // indentado para facilitar lectura
}

//Ejemplo de como usar 
int main(){


    std::vector<int> bandwidth = {120, 98, 110, 130, 100, 105, 115, 125};
    std::vector<int> traffic = {3000, 2800, 3100, 3300, 2900, 3000, 3200, 3400};
    std::vector<int> latency = {12, 14, 10, 11, 13, 12, 11, 9};

    export_metrics_to_json(bandwidth, traffic, latency);



    return 0;
}