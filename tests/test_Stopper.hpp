#pragma once

#include "../TEST.hpp"
#include "../Stopper.hpp"

#ifdef TEST


#include "../str_contains.hpp"
#include <thread>

class StopperForTest: public Stopper {
public:
    using Stopper::Stopper;
    virtual ~StopperForTest() {}

    void setElapsedMsForTesting(double ms) {
        if (started) {
            throw ERROR("Cannot set elapsed for testing while running");
        }
        elapsed = static_cast<long long>(ms * 1'000'000.0); // ms to nanoseconds
    }
};

TEST(test_Stopper_start_stop_basic) {
    StopperForTest stopper(false);
    stopper.start();
    this_thread::sleep_for(chrono::milliseconds(100));
    double elapsed = stopper.stop();
    assert(elapsed >= 90 && elapsed <= 150 && "Elapsed time should be approximately 100ms");
    assert(!stopper.isRunning() && "Stopper should not be running after stop");
}

TEST(test_Stopper_pause_resume) {
    StopperForTest stopper(false);
    stopper.start();
    this_thread::sleep_for(chrono::milliseconds(50));
    stopper.pause();
    double elapsedPaused = stopper.getElapsedMs();
    assert(elapsedPaused >= 45 && elapsedPaused <= 75 && "Paused elapsed time should be approximately 50ms");
    stopper.resume();
    this_thread::sleep_for(chrono::milliseconds(50));
    double elapsed = stopper.stop();
    assert(elapsed >= 90 && elapsed <= 150 && "Total elapsed time should be approximately 100ms");
}

TEST(test_Stopper_getElapsedMs_running) {
    StopperForTest stopper(false);
    stopper.start();
    this_thread::sleep_for(chrono::milliseconds(100));
    double elapsed = stopper.getElapsedMs();
    assert(elapsed >= 90 && elapsed <= 150 && "Running elapsed time should be approximately 100ms");
    stopper.stop();
}

TEST(test_Stopper_getElapsedMs_paused) {
    StopperForTest stopper(false);
    stopper.start();
    this_thread::sleep_for(chrono::milliseconds(50));
    stopper.pause();
    double elapsed = stopper.getElapsedMs();
    assert(elapsed >= 45 && elapsed <= 75 && "Paused elapsed time should be approximately 50ms");
    stopper.resume();
    stopper.stop();
}

TEST(test_Stopper_reset) {
    StopperForTest stopper(false);
    stopper.start();
    this_thread::sleep_for(chrono::milliseconds(50));
    stopper.pause();
    stopper.reset();
    double elapsed = stopper.getElapsedMs();
    assert(elapsed < 0.01 && "Elapsed time should be almost 0 after reset");
    assert(stopper.isRunning() && "Stopper should be running after reset");
}

TEST(test_Stopper_start_already_running) {
    StopperForTest stopper(false);
    stopper.start();
    bool thrown = false;
    try {
        stopper.start();
    } catch (exception &e) {
        thrown = true;
        string what = e.what();
        assert(str_contains(what, "stopper is already started") && "Exception message should indicate already running");
    }
    assert(thrown && "Start while running should throw");
    stopper.stop();
}

TEST(test_Stopper_pause_not_running) {
    StopperForTest stopper(false);
    bool thrown = false;
    try {
        stopper.pause();
    } catch (exception &e) {
        thrown = true;
        string what = e.what();
        assert(str_contains(what, "stopper is not started") && "Exception message should indicate not running");
    }
    assert(thrown && "Pause while not running should throw");
}

TEST(test_Stopper_stop_not_running) {
    StopperForTest stopper(false);
    bool thrown = false;
    try {
        stopper.stop();
    } catch (exception &e) {
        thrown = true;
        string what = e.what();
        assert(str_contains(what, "stopper is not started") && "Exception message should indicate not running");
    }
    assert(thrown && "Stop while not running should throw");
}

TEST(test_Stopper_resume_already_running) {
    StopperForTest stopper(false);
    stopper.start();
    bool thrown = false;
    try {
        stopper.resume();
    } catch (exception &e) {
        thrown = true;
        string what = e.what();
        assert(str_contains(what, "stopper is not paused") && "Exception message should indicate can not resume");
    }
    assert(thrown && "Resume while running should throw");
    stopper.stop();
}

TEST(test_Stopper_immediate_stop) {
    StopperForTest stopper(false);
    stopper.start();
    double elapsed = stopper.stop();
    assert(elapsed >= 0 && elapsed <= 10 && "Immediate stop should have near-zero elapsed time");
    assert(!stopper.isRunning() && "Stopper should not be running after stop");
}

TEST(test_Stopper_toString_zero) {
    StopperForTest s(false);
    s.setElapsedMsForTesting(0.0);
    string result = s.toString();
    assert(result == "0 milliseconds");
}

TEST(test_Stopper_toString_1ms) {
    StopperForTest s(false);
    s.setElapsedMsForTesting(1.0);
    string result = s.toString();
    assert(result == "1 millisecond");
}

TEST(test_Stopper_toString_2ms) {
    StopperForTest s(false);
    s.setElapsedMsForTesting(2.0);
    string result = s.toString();
    assert(result == "2 milliseconds");
}

TEST(test_Stopper_toString_1000ms) {
    StopperForTest s(false);
    s.setElapsedMsForTesting(1000.0);
    string result = s.toString();
    assert(result == "1 second");
}

TEST(test_Stopper_toString_1001ms) {
    StopperForTest s(false);
    s.setElapsedMsForTesting(1001.0);
    string result = s.toString();
    assert(result == "1 second, 1 millisecond");
}

TEST(test_Stopper_toString_60000ms) {
    StopperForTest s(false);
    s.setElapsedMsForTesting(60000.0);
    string result = s.toString();
    assert(result == "1 minute");
}

TEST(test_Stopper_toString_3600000ms) {
    StopperForTest s(false);
    s.setElapsedMsForTesting(3600000.0);
    string result = s.toString();
    assert(result == "1 hour");
}

TEST(test_Stopper_toString_86400000ms) {
    StopperForTest s(false);
    s.setElapsedMsForTesting(86400000.0);
    string result = s.toString();
    assert(result == "1 day");
}

TEST(test_Stopper_toString_604800000ms) {
    StopperForTest s(false);
    s.setElapsedMsForTesting(604800000.0);
    string result = s.toString();
    assert(result == "1 week");
}

TEST(test_Stopper_toString_2592000000ms) {
    StopperForTest s(false);
    s.setElapsedMsForTesting(2592000000.0);
    string result = s.toString();
    assert(result == "1 month");
}

TEST(test_Stopper_toString_31536000000ms) {
    StopperForTest s(false);
    s.setElapsedMsForTesting(31536000000.0);
    string result = s.toString();
    assert(result == "1 year");
}

TEST(test_Stopper_toString_combination) {
    StopperForTest s(false);
    // 1 year, 1 month, 1 week, 1 day, 1 hour, 1 minute, 1 second, 1 millisecond
    s.setElapsedMsForTesting(31536000000.0 + 2592000000.0 + 604800000.0 + 86400000.0 + 3600000.0 + 60000.0 + 1000.0 + 1.0);
    string result = s.toString();
    assert(result == "1 year, 1 month, 1 week, 1 day, 1 hour, 1 minute, 1 second, 1 millisecond");
}

TEST(test_Stopper_toString_rounding) {
    StopperForTest s(false);
    s.setElapsedMsForTesting(1.4);
    assert(s.toString() == "1 millisecond");
    s.setElapsedMsForTesting(1.5);
    assert(s.toString() == "2 milliseconds");
}

#endif

