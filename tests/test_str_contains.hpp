#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../str_contains.hpp"

TEST(test_str_contains_empty_string) {
    string str = "";
    string substring = "";
    assert(str_contains(str, substring) == true && "Empty substring in empty string");
}

TEST(test_str_contains_empty_substring) {
    string str = "hello";
    string substring = "";
    assert(str_contains(str, substring) == true && "Empty substring in non-empty string");
}

TEST(test_str_contains_exact_match) {
    string str = "hello world";
    string substring = "hello world";
    assert(str_contains(str, substring) == true && "Exact match");
}

TEST(test_str_contains_substring_at_start) {
    string str = "hello world";
    string substring = "hello";
    assert(str_contains(str, substring) == true && "Substring at start");
}

TEST(test_str_contains_substring_at_end) {
    string str = "hello world";
    string substring = "world";
    assert(str_contains(str, substring) == true && "Substring at end");
}

TEST(test_str_contains_substring_in_middle) {
    string str = "hello world";
    string substring = "lo wo";
    assert(str_contains(str, substring) == true && "Substring in middle");
}

TEST(test_str_contains_no_match) {
    string str = "hello world";
    string substring = "xyz";
    assert(str_contains(str, substring) == false && "No match");
}

TEST(test_str_contains_case_sensitive) {
    string str = "Hello World";
    string substring = "hello";
    assert(str_contains(str, substring) == false && "Case sensitive");
}

TEST(test_str_contains_special_chars) {
    string str = "$@#test$@#";
    string substring = "$@#";
    assert(str_contains(str, substring) == true && "Special characters");
}

#endif