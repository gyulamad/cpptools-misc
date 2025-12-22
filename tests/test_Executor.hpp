#pragma once

#include "../TEST.hpp"
#include "../Executor.hpp"

#ifdef TEST


#include "../capture_cout.hpp"

// Test struct for complex test cases
struct test_Executor_TestData {
    string command;
    int expected_status;
    string expected_output_contains;
    string expected_error_contains;
};

TEST(test_Executor_successful_command_with_output_capture) {
    string output, error;
    int status = Executor::execute("echo 'Hello World'", &output, &error);
    
    assert(status == 0 && "Successful command should return 0");
    assert(output == "Hello World\n" && "Should capture stdout");
    assert(error.empty() && "Successful command should have empty stderr");
}

TEST(test_Executor_failing_command_with_error_capture) {
    string output, error;
    int status = Executor::execute("ls /nonexistent_directory_xyz", &output, &error, false);
    
    assert(status != 0 && "Failing command should return non-zero");
    assert(output.empty() && "Failing command should have empty stdout");
    assert(!error.empty() && "Should capture stderr");
    assert(str_contains(error, "No such file or directory") && "Error should contain expected message");
}

TEST(test_Executor_display_output_when_nullptr) {
    // This test visually verifies output is displayed when pointers are nullptr
    string output = capture_cout([]() {
        int status = Executor::execute("echo 'This should appear on console'", nullptr, nullptr);
        assert(status == 0 && "Should return success");
    });
    assert(str_contains(output, "This should appear on console") && "Output should appear");
    
    bool t = false;
    try {
        string out, err;
        Executor::execute("ls /nonexistent_directory_xyz", &out, &err, true);
    } catch (exception &e) {
        t = true;
        string errs = e.what();
        assert(str_contains(errs, "No such file or directory") && "Error massage should contains error infos");
    }
    assert(t && "Should throw");
}

TEST(test_Executor_mixed_output_and_error_capture) {
    string output, error;
    int status = Executor::execute("echo 'Output'; echo 'Error' >&2", &output, &error);
    
    assert(status == 0 && "Command should succeed");
    assert(output == "Output\n" && "Should capture stdout");
    assert(error == "Error\n" && "Should capture stderr");
}

TEST(test_Executor_empty_command) {
    test_Executor_TestData data = {
        "",  // Empty command
        0,   // Expected status (shell typically succeeds for empty command)
        "",  // Expected output
        ""   // Expected error
    };
    
    string output, error;
    int status = Executor::execute(data.command, &output, &error);
    
    assert(status == data.expected_status && "Empty command should return expected status");
    assert(output == data.expected_output_contains && "Empty command should produce no output");
    assert(error == data.expected_error_contains && "Empty command should produce no error");
}

TEST(test_Executor_invalid_command) {
    test_Executor_TestData data = {
        "this_command_does_not_exist_xyz123",
        127,  // Command not found typically returns 127
        "",    // No stdout expected
        "not found"  // Error should contain this
    };
    
    string output, error;
    int status = Executor::execute(data.command, &output, &error, false);
    
    assert(status == data.expected_status && "Invalid command should return expected status");
    assert(output == data.expected_output_contains && "Invalid command should produce no output");
    assert(str_contains(error, data.expected_error_contains) && 
        "Invalid command error should contain expected message");
}

TEST(test_Executor_command_with_spaces) {
    string output;
    int status = Executor::execute("echo \"Hello  World\"", &output, nullptr);
    
    assert(status == 0 && "Command with spaces should succeed");
    assert(output == "Hello  World\n" && "Should preserve spaces in output");
}

TEST(test_Executor_pipe_command) {
    string output;
    int status = Executor::execute("echo 'pipe test' | wc -c", &output, nullptr);
    
    assert(status == 0 && "Piped command should succeed");
    assert(str_contains(output, "10") && "Should get correct character count");
}


#endif
