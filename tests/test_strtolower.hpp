#pragma once

#include "../TEST.hpp"
#include "../strtolower.hpp"

#ifdef TEST



TEST(test_strtolower_basic_conversion) {
    string input = "HELLO WORLD";
    string expected = "hello world";
    string actual = strtolower(input);
    assert(actual == expected && "test_strtolower_basic_conversion failed");
}

TEST(test_strtolower_mixed_case) {
    string input = "HeLLo WoRLd";
    string expected = "hello world";
    string actual = strtolower(input);
    assert(actual == expected && "test_strtolower_mixed_case failed");
}

TEST(test_strtolower_already_lowercase) {
    string input = "hello world";
    string expected = "hello world";
    string actual = strtolower(input);
    assert(actual == expected && "test_strtolower_already_lowercase failed");
}

TEST(test_strtolower_empty_string) {
    string input = "";
    string expected = "";
    string actual = strtolower(input);
    assert(actual == expected && "test_strtolower_empty_string failed");
}

TEST(test_strtolower_with_numbers_and_symbols) {
    string input = "123 ABC^!%&#$@";
    string expected = "123 abc^!%&#$@";
    string actual = strtolower(input);
    assert(actual == expected && "test_strtolower_with_numbers_and_symbols failed");
}

#endif
