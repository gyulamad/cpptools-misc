#pragma once

#include "../TEST.hpp"
#include "../array_search.hpp"

#ifdef TEST

TEST(test_array_search_vector_found) {
    vector<string> data = {"apple", "banana", "cherry"};
    ssize_t index = array_search("banana", data);
    assert(index == 1 && "Index should be 1 for 'banana'");
}

TEST(test_array_search_vector_not_found) {
    vector<int> data = {10, 20, 30};
    ssize_t index = array_search(40, data);
    assert(index == -1 && "Index should be -1 for missing value");
}

TEST(test_array_search_map_found) {
    map<int, string> data = {{1, "one"}, {2, "two"}, {3, "three"}};
    auto result = array_search("two", data);
    assert(result.has_value() && "Result should have value");
    assert(result.value() == 2 && "Key should be 2");
}

TEST(test_array_search_map_not_found) {
    map<string, int> data = {{"a", 1}, {"b", 2}};
    auto result = array_search(3, data);
    assert(!result.has_value() && "Result should be nullopt");
}

TEST(test_array_search_unordered_map) {
    unordered_map<string, double> data = {{"pi", 3.14}, {"e", 2.71}};
    auto result = array_search(3.14, data);
    assert(result.has_value() && "Result should have value");
    assert(result.value() == "pi" && "Key should be 'pi'");
}

TEST(test_array_search_empty_vector) {
    vector<int> data;
    ssize_t index = array_search(1, data);
    assert(index == -1 && "Index should be -1 for empty vector");
}

TEST(test_array_search_empty_map) {
    map<int, int> data;
    auto result = array_search(1, data);
    assert(!result.has_value() && "Result should be nullopt for empty map");
}

#endif