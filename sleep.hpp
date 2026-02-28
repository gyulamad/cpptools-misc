#pragma once

#include <thread>
#include <chrono>

using namespace std;

// Sleep for 's' seconds
inline void sleep_s(int s) {
    this_thread::sleep_for(chrono::seconds(s));
}

// Sleep for 'ms' milliseconds
inline void sleep_ms(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}