#pragma once

#include "../TEST.hpp"
#include "../str_cut_end.hpp"

#ifdef TEST

// Test cases for str_cut_end
TEST(test_str_cut_end_when_string_is_short) {
    string input = "hello";
    string expected = "hello";
    string actual = str_cut_end(input, 10);
    assert(actual == expected && "test_str_cut_end_when_string_is_short failed");
}

TEST(test_str_cut_end_when_string_is_exact_length) {
    string input = "abcdefghij";
    string expected = "abcdefghij";
    string actual = str_cut_end(input, 10);
    assert(actual == expected && "test_str_cut_end_when_string_is_exact_length failed");
}

TEST(test_str_cut_end_when_string_needs_truncation) {
    string input = "abcdefghijklmnopqrstuvwxyz";
    string expected = "abcdefghijkl...";
    string actual = str_cut_end(input, 15);
    assert(actual == expected && "test_str_cut_end_when_string_needs_truncation failed");
}

TEST(test_str_cut_end_when_custom_append_is_used) {
    string input = "abcdefghijklmnopqrstuvwxyz";
    string expected = "abcdefghijklm!!";
    string actual = str_cut_end(input, 15, "!!");
    assert(actual == expected && "test_str_cut_end_when_custom_append_is_used failed");
}

TEST(test_str_cut_end_when_string_is_empty) {
    string input = "";
    string expected = "";
    string actual = str_cut_end(input, 10);
    assert(actual == expected && "test_str_cut_end_when_string_is_empty failed");
}

TEST(test_str_cut_end_when_maxch_is_less_than_append_length) {
    string input = "abcdefghijklmnopqrstuvwxyz";
    string expected = "...";
    string actual = str_cut_end(input, 2);
    assert(actual == expected && "test_str_cut_end_when_maxch_is_less_than_append_length failed");
}

#endif
