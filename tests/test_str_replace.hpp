#pragma once

#include "../TEST.hpp"
#include "../str_replace.hpp"

#ifdef TEST


// Test cases for str_replace
TEST(test_str_replace_single_replacement) {
    string input = "hello world";
    string expected = "hi world";
    string actual = str_replace("hello", "hi", input);
    assert(actual == expected && "test_str_replace_single_replacement failed");
}

TEST(test_str_replace_multiple_occurrences) {
    string input = "apple banana apple";
    string expected = "fruit banana fruit";
    string actual = str_replace("apple", "fruit", input);
    assert(actual == expected && "test_str_replace_multiple_occurrences failed");
}

TEST(test_str_replace_no_match) {
    string input = "hello world";
    string expected = "hello world";
    string actual = str_replace("foo", "bar", input);
    assert(actual == expected && "test_str_replace_no_match failed");
}

TEST(test_str_replace_empty_to_multiple_occurrences) {
    string input = "apple banana apple";
    string expected = " banana ";
    string actual = str_replace("apple", "", input);
    assert(actual == expected && "test_str_replace_empty_to_multiple_occurrences failed");
}

TEST(test_str_replace_empty_to_no_match) {
    string input = "hello world";
    string expected = "hello world";
    string actual = str_replace("foo", "", input);
    assert(actual == expected && "test_str_replace_empty_to_no_match failed");
}

TEST(test_str_replace_non_empty_to) {
    string input = "hello world";
    string expected = "hello universe";
    string actual = str_replace("world", "universe", input);
    assert(actual == expected && "test_str_replace_non_empty_to failed");
}

TEST(test_str_replace_empty_input) {
    string input = "";
    string expected = "";
    string actual = str_replace("foo", "bar", input);
    assert(actual == expected && "test_str_replace_empty_input failed");
}

TEST(test_str_replace_empty_to) {
    string input = "hello world";
    string expected = "hello ";
    string actual = str_replace("world", "", input);
    assert(actual == expected && "test_str_replace_empty_to failed");
}

TEST(test_str_replace_empty_from) {
    bool thrown = false;
    try {
        string input = "hello world";
        string actual = str_replace("", "bar", input);
    } catch (const exception&) {
        thrown = true;
    }
    assert(thrown && "test_str_replace_empty_from failed");
}

TEST(test_str_replace_map_multiple_replacements) {
    string input = "hello world, hello universe";
    map<string, string> replacements = {{"hello", "hi"}, {"world", "earth"}};
    string expected = "hi earth, hi universe";
    string actual = str_replace(replacements, input);
    assert(actual == expected && "test_str_replace_map_multiple_replacements failed");
}

TEST(test_str_replace_map_overlapping_keys) {
    string input = "abcde";
    map<string, string> replacements = {{"abc", "123"}, {"cde", "456"}};
    string expected = "123de";
    string actual = str_replace(replacements, input);
    assert(actual == expected && "test_str_replace_map_overlapping_keys failed");
}

TEST(test_str_replace_map_empty_input) {
    string input = "";
    map<string, string> replacements = {{"hello", "hi"}, {"world", "earth"}};
    string expected = "";
    string actual = str_replace(replacements, input);
    assert(actual == expected && "test_str_replace_map_empty_input failed");
}

#endif
