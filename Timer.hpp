#pragma once

#include "datetime_defs.hpp"
#include "Stopper.hpp"
#include <thread>
#include <functional>

class Timer {
public:

    Timer(function<void()> cb, time_ms ms, bool running = true, time_ms slpms = 40): 
        cb(cb), 
        ms(ms),
        running(running), 
        paused(!running),        
        slpms(slpms),
        t([this]() {
            while (this->running) {
                this_thread::sleep_for(chrono::milliseconds(this->slpms));
                if (!this->paused) {
                    // lock_guard<mutex> lock(m);
                    check(this->cb, this->ms);
                }
            }
        })
    {}

    virtual ~Timer() {
        stop();
        if (t.joinable()) t.join();
    }

    void start() { paused = false; running = true; }
    void stop() { paused = true; running = false; }
    void pause() { paused = true; }
    void resume() { paused = false; }
    bool isRunning() const { return running && !paused; }

    void check(function<void()> cb, time_ms ms) {
        // If no callback provided, nothing to do
        if (cb == nullptr) return;
        
        // If stopper is not running, start it for the first time
        if (!stopper.isRunning()) {
            stopper.start();
            return; // Don't call callback on first check
        }
        
        // Check if enough time has elapsed
        double elapsedMs = stopper.getElapsedMs();
        if (elapsedMs >= ms) {
            lock_guard<mutex> lock(m);

            // Call the callback
            cb();
            
            // Reset the stopper to start timing for next interval
            stopper.reset();
            // stopper.start();
        }
    }
private:
    Stopper stopper;
    function<void()> cb; 
    time_ms ms;
    atomic<bool> running;
    atomic<bool> paused;
    time_ms slpms;
    thread t;
    mutex m;
};
