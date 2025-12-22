#pragma once

#include "../TEST.hpp"
#include "../execute.hpp"

#ifdef TEST


TEST(test_execute_successful_output) {
    string result = execute("echo Hello");
    // On some systems, echo appends a newline
    assert((result == "Hello\n" || result == "Hello") && "execute should capture echo output");
}

TEST(test_execute_empty_output) {
    string result = execute("true"); // POSIX command that does nothing
    assert(result.empty() && "execute should return empty string for no output");
}

TEST(test_execute_multiline_output) {
    // Use a portable command; 'ls -d .' on Unix, 'dir' on Windows might need adjustment
    string result = execute("printf 'Line1\nLine2\n'");
    assert(result == "Line1\nLine2\n" && "execute should concatenate multi-line output");
}

TEST(test_execute_successful_output_with_string) {
    string result = execute(string("echo Hello"));
    // On some systems, echo appends a newline
    assert((result == "Hello\n" || result == "Hello") && "execute should capture echo output");
}

TEST(test_execute_empty_output_with_string) {
    string result = execute(string("true")); // POSIX command that does nothing
    assert(result.empty() && "execute should return empty string for no output");
}

TEST(test_execute_multiline_output_with_string) {
    // Use a portable command; 'ls -d .' on Unix, 'dir' on Windows might need adjustment
    string result = execute(string("printf 'Line1\nLine2\n'"));
    assert(result == "Line1\nLine2\n" && "execute should concatenate multi-line output");
}

#endif