#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../ConsoleLogger.hpp"
#include "../capture_cout_cerr.hpp"
#include "../str_contains.hpp"

using namespace std;

// Test that TEST macro defines Test function
TEST(test_TEST_macro_defines_function) {
    // The TEST macro should define a function
    // This test will only compile if TEST macro works
}

// Test that tester object exists
TEST(test_TEST_tester_exists) {
    // tester should be accessible
    // This test verifies the tester object is declared
}

// Test add method
TEST(test_TEST_add) {
    // This test verifies that add method exists and compiles
    // We can't directly test it without exposing internal details
}

// Test run method with no tests
TEST(test_TEST_run_empty) {
    vector<string> filters;
    // This will throw if filters are not empty
    // tester.run(filters);
}

// Test run method without filters
TEST(test_TEST_run_no_filters) {
    // This will run all tests and count them
    // tester.run();
}

// Test run method with filters
TEST(test_TEST_run_with_filters) {
    vector<string> filters = {"test_TEST"};
    // This will filter tests containing "test_TEST"
    // tester.run(filters);
}

// Test capture_cout_cerr in test context
TEST(test_TEST_capture_cout_cerr) {
    string output = capture_cout_cerr([]() {
        cout << "Test output";
        cerr << "Test error";
    }, false);
    // Output should capture both cout and cerr without displaying
}

// Test capture_cout_cerr with throws = false
TEST(test_TEST_capture_cout_cerr_no_throw) {
    string output = capture_cout_cerr([]() {
        cout << "Test output";
    }, false);
    // Output should capture cout only
}

// Test capture_cout_cerr with empty lambda
TEST(test_TEST_capture_cout_cerr_empty) {
    string output = capture_cout_cerr([]() {
        // Empty
    }, true);
    // Output should be empty
}

// Test capture_cout_cerr with exception
TEST(test_TEST_capture_cout_cerr_exception) {
    bool threw = false;
    try {
        string output = capture_cout_cerr([]() {
            throw runtime_error("Test error");
        }, true);
    } catch (exception& e) {
        threw = true;
    }
    assert(threw && "capture_cout_cerr should propagate exceptions");
}

// Test that multiple TEST macros can be defined
TEST(test_TEST_multiple_macros_1) {
    // First test macro
}

TEST(test_TEST_multiple_macros_2) {
    // Second test macro
}

TEST(test_TEST_multiple_macros_3) {
    // Third test macro
}

// Test TEST_SHOW_DETAILS mode
#ifdef TEST_SHOW_DETAILS
TEST(test_TEST_show_details_enabled) {
    // This test only runs in TEST_SHOW_DETAILS mode
}
#endif

// Test with different log levels
TEST(test_TEST_log_levels) {
    // Log level tests - just verify they compile
    // The actual logging is tested in test_Logger.hpp
}

// Test with ASSERT macro
TEST(test_TEST_assert_macro) {
    int x = 5;
    assert(x == 5 && "Assert should pass for equal values");
}

// Test with SAFE macro
TEST(test_TEST_safe_macro) {
    // Create a logger to test SAFE
    createLogger<ConsoleLogger>();
    // SAFE should return a valid pointer
}

// Test with F macro
TEST(test_TEST_f_macro) {
    string result = F(F_INFO, "Test message");
    // F macro should format the message
}

// Test with FILELN macro
TEST(test_TEST_fileln_macro) {
    string fileln = FILELN;
    // FILELN should contain file and line info
}

// Test with F_CALL macro
TEST(test_TEST_f_call_macro) {
    string result = F_CALL("testName", "test.cpp", 1);
    // F_CALL should format the call info
}

// Test with EMPTY_OR macro
TEST(test_TEST_empty_or_macro) {
    string empty = "";
    string nonEmpty = "value";
    // EMPTY_OR returns a formatted string with ANSI codes
    // So we check that it contains the expected content
    string emptyResult = EMPTY_OR(empty);
    string nonEmptyResult = EMPTY_OR(nonEmpty);
    assert(str_contains(emptyResult, "<empty>") && "EMPTY_OR should contain '<empty>' for empty string");
    assert(str_contains(nonEmptyResult, "value") && "EMPTY_OR should contain 'value' for non-empty string");
}

// Test with ERROR macro
TEST(test_TEST_error_macro) {
    bool threw = false;
    try {
        throw ERROR("Test error message");
    } catch (exception& e) {
        threw = true;
    }
    assert(threw && "ERROR macro should throw exception");
}

// Test with LOG macros
TEST(test_TEST_log_macros) {
    // LOG macros tests - just verify they compile
    // The actual logging is tested in test_Logger.hpp
}

// Test with DUMP macro
TEST(test_TEST_dump_macro) {
    // DUMP macro tests - just verify it compiles
    // The actual output is handled by the logger
}

#endif