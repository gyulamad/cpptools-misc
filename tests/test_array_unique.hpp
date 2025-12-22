#pragma once

#include "../TEST.hpp"
#include "../array_unique.hpp"

#ifdef TEST


TEST(test_array_unique_basic) {
    vector<int> vec = {1, 2, 2, 3, 4, 4, 4, 5};
    vector<int> result = array_unique(vec);
    vector<int> expected = {1, 2, 3, 4, 5};
    assert(result == expected && "Basic uniqueness");
}

TEST(test_array_unique_empty_vector) {
    vector<int> vec;
    vector<int> result = array_unique(vec);
    assert(result.empty() && "Empty vector");
}

TEST(test_array_unique_all_duplicates) {
    vector<int> vec = {1, 1, 1, 1, 1};
    vector<int> result = array_unique(vec);
    vector<int> expected = {1};
    assert(result == expected && "All duplicates");
}

TEST(test_array_unique_strings) {
    vector<string> vec = {"hello", "world", "hello", "cpp", "world"};
    vector<string> result = array_unique(vec);
    vector<string> expected = {"hello", "world", "cpp"};
    assert(result == expected && "Strings with duplicates");
}

TEST(test_array_unique_large_vector) {
    vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5};
    vector<int> result = array_unique(vec);
    vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    assert(result == expected && "Large vector with duplicates");
}

#endif
