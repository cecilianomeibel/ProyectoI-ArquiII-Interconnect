#include "clk.hpp"

void run_clk(){
    const int delay = 1000000;
    int counter = 0;

    while(!finish){
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