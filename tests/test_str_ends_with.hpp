#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../str_ends_with.hpp"

TEST(test_str_ends_with_empty_string) {
    string str = "";
    string suffix = "";
    assert(str_ends_with(str, suffix) == true && "Empty string with empty suffix");
}

TEST(test_str_ends_with_empty_suffix) {
    string str = "hello world";
    string suffix = "";
    assert(str_ends_with(str, suffix) == true && "Non-empty string with empty suffix");
}

TEST(test_str_ends_with_exact_match) {
    string str = "hello world";
    string suffix = "hello world";
    assert(str_ends_with(str, suffix) == true && "Exact match");
}

TEST(test_str_ends_with_partial_match) {
    string str = "hello world";
    string suffix = "world";
    assert(str_ends_with(str, suffix) == true && "Partial match at end");
}

TEST(test_str_ends_with_no_match) {
    string str = "hello world";
    string suffix = "hello";
    assert(str_ends_with(str, suffix) == false && "No match");
}

TEST(test_str_ends_with_different_case) {
    string str = "Hello World";
    string suffix = "world";
    assert(str_ends_with(str, suffix) == false && "Different case");
}

TEST(test_str_ends_with_longer_suffix) {
    string str = "hello";
    string suffix = "hello world";
    assert(str_ends_with(str, suffix) == false && "Longer suffix than string");
}

TEST(test_str_ends_with_special_chars) {
    string str = "test$@#";
    string suffix = "$@#";
    assert(str_ends_with(str, suffix) == true && "Special characters");
}

#endif
