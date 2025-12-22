#pragma once

#include "../TEST.hpp"
#include "../regx_replace.hpp"

#ifdef TEST


// Test cases for regx_replace
TEST(test_regx_replace_single_match) {
    string input = "hello world";
    string pattern = "world";
    string replace = "universe";
    string expected = "hello universe";
    string actual = regx_replace(pattern, input, replace);
    assert(actual == expected && "test_regx_replace_single_match failed");
}

TEST(test_regx_replace_no_match) {
    string input = "hello world";
    string pattern = "foo";
    string replace = "bar";
    string expected = "hello world";
    string actual = regx_replace(pattern, input, replace);
    assert(actual == expected && "test_regx_replace_no_match failed");
}

TEST(test_regx_replace_first_occurrence_only) {
    string input = "apple apple apple";
    string pattern = "apple";
    string replace = "fruit";
    string expected = "fruit apple apple";
    string actual = regx_replace(pattern, input, replace);
    assert(actual == expected && "test_regx_replace_first_occurrence_only failed");
}

TEST(test_regx_replace_empty_input) {
    string input = "";
    string pattern = "foo";
    string replace = "bar";
    string expected = "";
    string actual = regx_replace(pattern, input, replace);
    assert(actual == expected && "test_regx_replace_empty_input failed");
}

TEST(test_regx_replace_empty_pattern) {
    bool thrown = false;
    try {
        string input = "hello world";
        string pattern = "";
        string replace = "bar";
        regx_replace(pattern, input, replace);
    } catch (...) {
        thrown = true;
    }
    assert(thrown && "test_regx_replace_empty_pattern failed");
}

#endif
