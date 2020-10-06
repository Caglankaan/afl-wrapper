#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

class Timer {
    bool clear = false;
    std::mutex mtx; 
    public:
        int elapsed_time{};
        template<typename Callback>
        void setInterval(Callback function, int interval);
        void stop();

};

template<typename Callback>
void Timer::setInterval(Callback function, int interval) {
    this->clear = false;
    std::thread t([=]() {
        while(true) {
            mtx.lock();
            if(this->clear) return;
            mtx.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            mtx.lock();
            if(this->clear) return;
            mtx.unlock();
            function();
        }
    });
    t.detach();
}


void Timer::stop() {
    mtx.lock();
    this->clear = true;
    mtx.unlock();
}