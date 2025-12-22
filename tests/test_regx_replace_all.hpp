#pragma once

#include "../TEST.hpp"
#include "../regx_replace_all.hpp"

#ifdef TEST


TEST(test_regx_replace_all) {
    string input = "apple apple apple";
    string pattern = "apple";
    string replace = "fruit";
    string expected = "fruit fruit fruit";
    string actual = regx_replace_all(pattern, input, replace);
    assert(actual == expected && "test_regx_replace_all failed");
}

// Test cases for regx_replace_all
TEST(test_regx_replace_all_multiple_matches) {
    string input = "apple banana apple";
    string pattern = "apple";
    string replace = "fruit";
    string expected = "fruit banana fruit";
    string actual = regx_replace_all(pattern, input, replace);
    assert(actual == expected && "test_regx_replace_all_multiple_matches failed");
}

TEST(test_regx_replace_all_no_match) {
    string input = "hello world";
    string pattern = "foo";
    string replace = "bar";
    string expected = "hello world";
    string actual = regx_replace_all(pattern, input, replace);
    assert(actual == expected && "test_regx_replace_all_no_match failed");
}

TEST(test_regx_replace_all_empty_input) {
    string input = "";
    string pattern = "foo";
    string replace = "bar";
    string expected = "";
    string actual = regx_replace_all(pattern, input, replace);
    assert(actual == expected && "test_regx_replace_all_empty_input failed");
}

TEST(test_regx_replace_all_empty_pattern) {
    bool thrown = false;
    try {
        string input = "hello world";
        string pattern = "";
        string replace = "bar";
        regx_replace_all(pattern, input, replace);
    } catch (...) {
        thrown = true;
    }
    assert(thrown && "test_regx_replace_all_empty_pattern failed");
}

TEST(test_regx_replace_all_regex_special_characters) {
    string input = "123-456-7890";
    string pattern = "\\d{3}-\\d{3}-\\d{4}";
    string replace = "[phone]";
    string expected = "[phone]";
    string actual = regx_replace_all(pattern, input, replace);
    assert(actual == expected && "test_regx_replace_all_regex_special_characters failed");
}

#endif
