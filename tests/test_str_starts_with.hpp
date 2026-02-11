#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../str_starts_with.hpp"

TEST(test_str_starts_with_empty_string) {
    string str = "";
    string prefix = "";
    assert(str_starts_with(str, prefix) == true && "Empty string with empty prefix");
}

TEST(test_str_starts_with_empty_prefix) {
    string str = "hello world";
    string prefix = "";
    assert(str_starts_with(str, prefix) == true && "Non-empty string with empty prefix");
}

TEST(test_str_starts_with_exact_match) {
    string str = "hello world";
    string prefix = "hello world";
    assert(str_starts_with(str, prefix) == true && "Exact match");
}

TEST(test_str_starts_with_partial_match) {
    string str = "hello world";
    string prefix = "hello";
    assert(str_starts_with(str, prefix) == true && "Partial match at start");
}

TEST(test_str_starts_with_no_match) {
    string str = "hello world";
    string prefix = "world";
    assert(str_starts_with(str, prefix) == false && "No match");
}

TEST(test_str_starts_with_different_case) {
    string str = "Hello World";
    string prefix = "hello";
    assert(str_starts_with(str, prefix) == false && "Different case");
}

TEST(test_str_starts_with_longer_prefix) {
    string str = "hello";
    string prefix = "hello world";
    assert(str_starts_with(str, prefix) == false && "Longer prefix than string");
}

TEST(test_str_starts_with_special_chars) {
    string str = "$@#test";
    string prefix = "$@#";
    assert(str_starts_with(str, prefix) == true && "Special characters");
}

#endif
