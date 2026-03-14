#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../Timer.hpp"

#include <atomic>
#include <chrono>
#include <thread>

TEST(test_Timer_constructor_starts_running_by_default) {
    atomic<int> callCount{0};
    atomic<bool> callbackCalled{false};
    
    // Timer with 50ms interval, should start running by default
    Timer timer([&callCount, &callbackCalled]() {
        callCount++;
        callbackCalled = true;
    }, 50, true, 10);  // 50ms interval, running=true, 10ms sleep
    
    // Wait for at least one callback
    int maxWait = 0;
    while (!callbackCalled && maxWait < 1000) {
        this_thread::sleep_for(chrono::milliseconds(10));
        maxWait += 10;
    }
    
    assert(callbackCalled && "Callback should have been called");
    assert(callCount >= 1 && "Call count should be at least 1");
}

TEST(test_Timer_constructor_not_running) {
    atomic<int> callCount{0};
    
    // Timer with 50ms interval, not running
    Timer timer([&callCount]() {
        callCount++;
    }, 50, false, 10);  // 50ms interval, running=false
    
    // Wait a bit
    this_thread::sleep_for(chrono::milliseconds(100));
    
    // Callback should not have been called since timer is paused
    assert(callCount == 0 && "Callback should not have been called when timer starts paused");
}

TEST(test_Timer_pause_stops_callback) {
    atomic<int> callCount{0};
    atomic<bool> firstCall{false};
    
    Timer timer([&callCount, &firstCall]() {
        callCount++;
        firstCall = true;
    }, 30, true, 10);  // 30ms interval
    
    // Wait for first callback
    int maxWait = 0;
    while (!firstCall && maxWait < 500) {
        this_thread::sleep_for(chrono::milliseconds(5));
        maxWait += 5;
    }
    
    assert(firstCall && "First callback should have been called");
    
    int countAfterFirst = callCount;
    
    // Pause the timer
    timer.pause();
    
    // Wait some more
    this_thread::sleep_for(chrono::milliseconds(100));
    
    // Count should not have increased significantly after pause
    assert(callCount <= countAfterFirst + 1 && "Callback should not be called after pause");
}

TEST(test_Timer_resume_continues_callback) {
    atomic<int> callCount{0};
    atomic<bool> firstCall{false};
    
    Timer timer([&callCount, &firstCall]() {
        callCount++;
        firstCall = true;
    }, 30, true, 10);  // 30ms interval
    
    // Wait for first callback
    int maxWait = 0;
    while (!firstCall && maxWait < 500) {
        this_thread::sleep_for(chrono::milliseconds(5));
        maxWait += 5;
    }
    
    timer.pause();
    int countAfterPause = callCount;
    
    // Wait to ensure no more callbacks
    this_thread::sleep_for(chrono::milliseconds(50));
    
    // Resume
    timer.resume();
    
    // Wait for next callback
    atomic<bool> secondCall{false};
    maxWait = 0;
    while (!secondCall && maxWait < 500) {
        this_thread::sleep_for(chrono::milliseconds(5));
        if (callCount > countAfterPause) {
            secondCall = true;
        }
        maxWait += 5;
    }
    
    assert(callCount > countAfterPause && "Callback should be called after resume");
}

TEST(test_Timer_stop_halts_timer) {
    atomic<int> callCount{0};
    atomic<bool> firstCall{false};
    
    Timer timer([&callCount, &firstCall]() {
        callCount++;
        firstCall = true;
    }, 30, true, 10);  // 30ms interval
    
    // Wait for first callback
    int maxWait = 0;
    while (!firstCall && maxWait < 500) {
        this_thread::sleep_for(chrono::milliseconds(5));
        maxWait += 5;
    }
    
    int countAfterFirst = callCount;
    
    // Stop the timer
    timer.stop();
    
    // Wait some more
    this_thread::sleep_for(chrono::milliseconds(100));
    
    // Count should not have increased after stop
    assert(callCount == countAfterFirst && "Callback should not be called after stop");
}

TEST(test_Timer_isRunning_returns_correct_state) {
    atomic<int> callCount{0};
    
    // Timer starts paused
    Timer timer([&callCount]() {
        callCount++;
    }, 100, false, 10);
    
    assert(!timer.isRunning() && "Timer should not be running when constructed with running=false");
    
    timer.start();
    assert(timer.isRunning() && "Timer should be running after start()");
    
    timer.pause();
    assert(!timer.isRunning() && "Timer should not be running after pause()");
    
    timer.resume();
    assert(timer.isRunning() && "Timer should be running after resume()");
    
    timer.stop();
    assert(!timer.isRunning() && "Timer should not be running after stop()");
}

TEST(test_Timer_multiple_intervals) {
    atomic<int> callCount{0};
    atomic<bool> callbackCalled{false};
    
    // Timer with 100ms interval
    Timer timer([&callCount, &callbackCalled]() {
        callCount++;
        callbackCalled = true;
    }, 100, true, 20);  // 100ms interval, 20ms sleep
    
    // Wait for first callback
    int maxWait = 0;
    while (!callbackCalled && maxWait < 1000) {
        this_thread::sleep_for(chrono::milliseconds(10));
        maxWait += 10;
    }
    
    assert(callbackCalled && "Callback should have been called");
    
    // Wait for potentially more callbacks
    this_thread::sleep_for(chrono::milliseconds(250));
    
    // Should have been called multiple times
    assert(callCount >= 2 && "Callback should have been called at least twice");
}

TEST(test_Timer_short_interval) {
    atomic<int> callCount{0};
    atomic<bool> callbackCalled{false};
    
    // Timer with very short interval (20ms)
    Timer timer([&callCount, &callbackCalled]() {
        callCount++;
        callbackCalled = true;
    }, 20, true, 5);  // 20ms interval, 5ms sleep
    
    // Wait for callback
    int maxWait = 0;
    while (!callbackCalled && maxWait < 500) {
        this_thread::sleep_for(chrono::milliseconds(5));
        maxWait += 5;
    }
    
    assert(callbackCalled && "Callback should have been called with short interval");
}

TEST(test_Timer_start_restarts_stopped_timer) {
    atomic<int> callCount{0};
    atomic<bool> callbackCalled{false};
    
    Timer timer([&callCount, &callbackCalled]() {
        callCount++;
        callbackCalled = true;
    }, 30, true, 10);
    
    // Wait for first callback
    int maxWait = 0;
    while (!callbackCalled && maxWait < 500) {
        this_thread::sleep_for(chrono::milliseconds(5));
        maxWait += 5;
    }
    
    timer.stop();
    int countAfterStop = callCount;
    
    // Reset flag
    callbackCalled = false;
    
    // Restart
    timer.start();
    
    // Wait for next callback
    maxWait = 0;
    while (!callbackCalled && maxWait < 500) {
        this_thread::sleep_for(chrono::milliseconds(5));
        maxWait += 5;
    }
    
    assert(callCount > countAfterStop && "Callback should be called after restart");
}

#endif
