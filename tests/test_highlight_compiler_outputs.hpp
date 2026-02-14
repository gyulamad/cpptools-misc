#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../highlight_compiler_outputs.hpp"
#include "../F.hpp"

TEST(test_highlight_compiler_outputs_basic) {
    string output = "error: Something went wrong\nnote: This is a note\nwarning: Be careful";
    string result = highlight_compiler_outputs(output);
    
    // Verify that the output contains the formatted text
    assert(str_contains(result, F_ERROR) && "Output should contain error format");
    assert(str_contains(result, F_NOTE) && "Output should contain note format");
    assert(str_contains(result, F_WARNING) && "Output should contain warning format");
}

TEST(test_highlight_compiler_outputs_empty) {
    string output = "";
    string result = highlight_compiler_outputs(output);
    assert(result.empty() && "Empty input should return empty output");
}

TEST(test_highlight_compiler_outputs_no_matches) {
    string output = "This is a normal message";
    string result = highlight_compiler_outputs(output);
    assert(result == output && "Output should be unchanged when no matches found");
}

#endif