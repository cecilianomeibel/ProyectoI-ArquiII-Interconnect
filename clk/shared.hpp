#ifndef SHARED_HPP
#define SHARED_HPP

#include <atomic>
#include <mutex>
#include <condition_variable>

extern std::atomic<int> clock_signal;
extern std::mutex mtx;
extern std::condition_variable cv;
extern bool ready;
extern bool finish;

#endif