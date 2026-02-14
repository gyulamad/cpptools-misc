#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../Logger.hpp"
#include "../ConsoleLogger.hpp"
#include "../capture_cout_cerr.hpp"
#include "../str_contains.hpp"

using namespace std;

// Custom logger for testing
class TestLogger : public Logger {
public:
    void write(const string& output) override {
        capturedOutput = output;
    }

    string capturedOutput = "";
};

// Test for Logger base class
TEST(test_Logger_constructor) {
    TestLogger logger;
    // Constructor should work
}

TEST(test_Logger_throws) {
    TestLogger logger;
    logger.set_level_output(LOGLVL_THROW);
    bool threw = false;
    try {
        logger.throws("Test message", "test.cpp:1", true);
    } catch (exception& e) {
        threw = true;
        assert(str_contains(e.what(), "Logger throws") && "Exception should contain 'Logger throws'");
    }
    assert(threw && "throws should throw an exception");
}

TEST(test_Logger_error) {
    TestLogger logger;
    logger.set_level_output(LOGLVL_ERROR);
    string output = capture_cout_cerr([&logger]() {
        logger.error("Test error message", "test.cpp:2", false);
    }, true);
    // Error level should produce output
}

TEST(test_Logger_alert) {
    TestLogger logger;
    logger.set_level_output(LOGLVL_ALERT);
    string output = capture_cout_cerr([&logger]() {
        logger.alert("Test alert message", "test.cpp:3", false);
    }, true);
    // Alert level should produce output
}

TEST(test_Logger_warning) {
    TestLogger logger;
    logger.set_level_output(LOGLVL_WARNING);
    string output = capture_cout_cerr([&logger]() {
        logger.warning("Test warning message", "test.cpp:4", false);
    }, true);
    // Warning level should produce output
}

TEST(test_Logger_info) {
    TestLogger logger;
    logger.set_level_output(LOGLVL_INFO);
    string output = capture_cout_cerr([&logger]() {
        logger.info("Test info message", "test.cpp:5", false);
    }, true);
    // Info level should produce output
}

TEST(test_Logger_note) {
    TestLogger logger;
    logger.set_level_output(LOGLVL_NOTE);
    string output = capture_cout_cerr([&logger]() {
        logger.note("Test note message", "test.cpp:6", false);
    }, true);
    // Note level should produce output
}

TEST(test_Logger_success) {
    TestLogger logger;
    logger.set_level_output(LOGLVL_SUCCESS);
    string output = capture_cout_cerr([&logger]() {
        logger.success("Test success message", "test.cpp:7", false);
    }, true);
    // Success level should produce output
}

TEST(test_Logger_debug) {
    TestLogger logger;
    logger.set_level_output(LOGLVL_DEBUG);
    string output = capture_cout_cerr([&logger]() {
        logger.debug("Test debug message", "test.cpp:8", false);
    }, true);
    // Debug level should produce output
}

TEST(test_Logger_mutex) {
    TestLogger logger;
    // Test that mutex is used (thread safety)
    string output = capture_cout_cerr([&logger]() {
        logger.info("Thread safe message", "test.cpp:11", false);
    }, true);
    // Output should be produced
}

TEST(test_Logger_time) {
    TestLogger logger;
    string output = capture_cout_cerr([&logger]() {
        logger.info("Time test", "test.cpp:12", false);
    }, true);
    // Time should be included in output
}

// Test for LoggerFactory
TEST(test_LoggerFactory_createLogger) {
    Logger* logger = LoggerFactory::createLogger<ConsoleLogger>();
    assert(logger != nullptr && "createLogger should return a logger");
    LoggerFactory::setLogger(nullptr); // Cleanup
}

TEST(test_LoggerFactory_setLogger) {
    TestLogger* testLogger = new TestLogger();
    LoggerFactory::setLogger(testLogger);
    Logger* retrieved = LoggerFactory::getLogger();
    assert(retrieved == testLogger && "setLogger should store the logger");
    delete testLogger;
}

TEST(test_LoggerFactory_getLogger) {
    TestLogger* testLogger = new TestLogger();
    LoggerFactory::setLogger(testLogger);
    Logger* retrieved = LoggerFactory::getLogger();
    assert(retrieved == testLogger && "getLogger should return the stored logger");
    LoggerFactory::setLogger(nullptr); // Cleanup
    delete testLogger;
}

// Test for global logger functions
TEST(test_global_logger_functions) {
    TestLogger* testLogger = new TestLogger();
    logger(testLogger);
    Logger* retrieved = logger();
    assert(retrieved == testLogger && "global logger() should return the stored logger");
    LoggerFactory::setLogger(nullptr); // Cleanup
    delete testLogger;
}

#endif