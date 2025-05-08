#include "shared.hpp"
#include <thread>

//para que funcione se tiene que implementar un while con cv.wait() en la ejecucion de inst PE
//e incrementar un contador cada cv.wait()
void run_clk(){
    const int delay = 1000000000;
    int counter = 0;

    while(true){
        counter++;
        if(counter >= delay){
            clock_signal = 1 - clock_signal;
            {
                std::lock_guard<std::mutex> lock(mtx);
                ready = true;
            }
            cv.notify_all();

            counter = 0;
        }
    }
}