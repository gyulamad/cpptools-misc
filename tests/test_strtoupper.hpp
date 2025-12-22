#pragma once

#include "../TEST.hpp"
#include "../strtoupper.hpp"

#ifdef TEST


TEST(test_strtoupper_basic_conversion) {
    string input = "hello world";
    string expected = "HELLO WORLD";
    string actual = strtoupper(input);
    assert(actual == expected && "test_strtoupper_basic_conversion failed");
}

TEST(test_strtoupper_mixed_case) {
    string input = "HeLLo WoRLd";
    string expected = "HELLO WORLD";
    string actual = strtoupper(input);
    assert(actual == expected && "test_strtoupper_mixed_case failed");
}

TEST(test_strtoupper_already_uppercase) {
    string input = "HELLO WORLD";
    string expected = "HELLO WORLD";
    string actual = strtoupper(input);
    assert(actual == expected && "test_strtoupper_already_uppercase failed");
}

TEST(test_strtoupper_empty_string) {
    string input = "";
    string expected = "";
    string actual = strtoupper(input);
    assert(actual == expected && "test_strtoupper_empty_string failed");
}

TEST(test_strtoupper_with_numbers_and_symbols) {
    string input = "123 abc^!%&#$@";
    string expected = "123 ABC^!%&#$@";
    string actual = strtoupper(input);
    assert(actual == expected && "test_strtoupper_with_numbers_and_symbols failed");
}

#endif
