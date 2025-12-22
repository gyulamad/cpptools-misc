#pragma once

#include "../TEST.hpp"
#include "../str_cut_begin.hpp"

#ifdef TEST


TEST(test_str_cut_begin_when_string_is_short) {
    string input = "hello";
    string expected = "hello";
    string actual = str_cut_begin(input, 10);
    assert(actual == expected && "test_str_cut_begin_when_string_is_short failed");
}

TEST(test_str_cut_begin_when_string_is_exact_length) {
    string input = "abcdefghij";
    string expected = "abcdefghij";
    string actual = str_cut_begin(input, 10);
    assert(actual == expected && "test_str_cut_begin_when_string_is_exact_length failed");
}

TEST(test_str_cut_begin_when_string_needs_truncation) {
    string input = "abcdefghijklmnopqrstuvwxyz";
    string expected = "...opqrstuvwxyz";
    string actual = str_cut_begin(input, 15);
    assert(actual == expected && "test_str_cut_begin_when_string_needs_truncation failed");
}

TEST(test_str_cut_begin_when_custom_prepend_is_used) {
    string input = "abcdefghijklmnopqrstuvwxyz";
    string expected = "!!nopqrstuvwxyz";
    string actual = str_cut_begin(input, 15, "!!");
    assert(actual == expected && "test_str_cut_begin_when_custom_prepend_is_used failed");
}

TEST(test_str_cut_begin_when_string_is_empty) {
    string input = "";
    string expected = "";
    string actual = str_cut_begin(input, 10);
    assert(actual == expected && "test_str_cut_begin_when_string_is_empty failed");
}

TEST(test_str_cut_begin_when_maxch_is_less_than_prepend_length) {
    string input = "abcdefghijklmnopqrstuvwxyz";
    string expected = "...";
    string actual = str_cut_begin(input, 2);
    assert(actual == expected && "test_str_cut_begin_when_maxch_is_less_than_prepend_length failed");
}

#endif
