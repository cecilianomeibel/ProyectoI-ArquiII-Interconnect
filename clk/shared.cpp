#include "shared.hpp"

std::atomic<int> clock_signal(0);
std::mutex mtx;
std::condition_variable cv;
bool ready = false;
bool finish = false;