#pragma once

#include "../TEST.hpp"
#include "../compare_diff_vectors.hpp"

#ifdef TEST


TEST(test_compare_diff_vectors_identical) {
    vector<str_diff_t> actual = {
        {{1, 2}, {"added_line"}, {"removed_line"}},
        {{3, 4}, {"another_added"}, {"another_removed"}}
    };
    vector<str_diff_t> expected = {
        {{1, 2}, {"added_line"}, {"removed_line"}},
        {{3, 4}, {"another_added"}, {"another_removed"}}
    };
    bool result = compare_diff_vectors(actual, expected);
    assert(result == true && "Identical diff vectors should return true");
}

TEST(test_compare_diff_vectors_different_sizes) {
    vector<str_diff_t> actual = {
        {{1, 2}, {"added_line"}, {"removed_line"}}
    };
    vector<str_diff_t> expected = {
        {{1, 2}, {"added_line"}, {"removed_line"}},
        {{3, 4}, {"another_added"}, {"another_removed"}}
    };
    bool result = compare_diff_vectors(actual, expected);
    assert(result == false && "Vectors with different sizes should return false");
}

TEST(test_compare_diff_vectors_different_bounds) {
    vector<str_diff_t> actual = {
        {{1, 2}, {"added_line"}, {"removed_line"}}
    };
    vector<str_diff_t> expected = {
        {{2, 3}, {"added_line"}, {"removed_line"}}
    };
    bool result = compare_diff_vectors(actual, expected);
    assert(result == false && "Vectors with different bounds should return false");
}

TEST(test_compare_diff_vectors_different_added) {
    vector<str_diff_t> actual = {
        {{1, 2}, {"added_line"}, {"removed_line"}}
    };
    vector<str_diff_t> expected = {
        {{1, 2}, {"different_added"}, {"removed_line"}}
    };
    bool result = compare_diff_vectors(actual, expected);
    assert(result == false && "Vectors with different added lines should return false");
}

TEST(test_compare_diff_vectors_different_removed) {
    vector<str_diff_t> actual = {
        {{1, 2}, {"added_line"}, {"removed_line"}}
    };
    vector<str_diff_t> expected = {
        {{1, 2}, {"added_line"}, {"different_removed"}}
    };
    bool result = compare_diff_vectors(actual, expected);
    assert(result == false && "Vectors with different removed lines should return false");
}

TEST(test_compare_diff_vectors_empty) {
    vector<str_diff_t> actual = {};
    vector<str_diff_t> expected = {};
    bool result = compare_diff_vectors(actual, expected);
    assert(result == true && "Empty diff vectors should return true");
}

#endif