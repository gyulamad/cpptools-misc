#pragma once

#include "../TEST.hpp"
#include "../array_key_exists.hpp"

#ifdef TEST

// Test cases for array_key_exists with map
TEST(test_array_key_exists_map_exists) {
    map<string, int> m = {{"one", 1}, {"two", 2}};
    bool actual = array_key_exists("one", m);
    assert(actual == true && "Key 'one' should exist in map");
}

TEST(test_array_key_exists_map_not_exists) {
    map<string, int> m = {{"one", 1}, {"two", 2}};
    bool actual = array_key_exists("three", m);
    assert(actual == false && "Key 'three' should not exist in map");
}

TEST(test_array_key_exists_map_empty) {
    map<string, int> m;
    bool actual = array_key_exists("one", m);
    assert(actual == false && "Key 'one' should not exist in empty map");
}

// Test cases for array_key_exists with unordered_map
TEST(test_array_key_exists_unordered_map_exists) {
    unordered_map<string, int> m = {{"one", 1}, {"two", 2}};
    bool actual = array_key_exists("two", m);
    assert(actual == true && "Key 'two' should exist in unordered_map");
}

TEST(test_array_key_exists_unordered_map_not_exists) {
    unordered_map<string, int> m = {{"one", 1}, {"two", 2}};
    bool actual = array_key_exists("three", m);
    assert(actual == false && "Key 'three' should not exist in unordered_map");
}

TEST(test_array_key_exists_unordered_map_empty) {
    unordered_map<string, int> m;
    bool actual = array_key_exists("one", m);
    assert(actual == false && "Key 'one' should not exist in empty unordered_map");
}

#endif
