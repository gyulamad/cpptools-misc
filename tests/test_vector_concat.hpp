#pragma once

#include "../TEST.hpp"
#include "../vector_concat.hpp"

#ifdef TEST


struct test_vector_concat_TestData {
    vector<int> a;
    vector<int> b;
    vector<int> expected;
};

TEST(test_vector_concat_non_empty_vectors) {
    test_vector_concat_TestData data = {{4, 2, 6}, {9, 3, 2}, {4, 2, 6, 9, 3, 2}};
    vector<int> result = vector_concat(data.a, data.b);
    assert(result.size() == data.expected.size() && "Result size should match expected");
    for (size_t i = 0; i < result.size(); i++)
        assert(result[i] == data.expected[i] && "Result elements should match expected");
}

TEST(test_vector_concat_empty_first_vector) {
    test_vector_concat_TestData data = {{}, {9, 3, 2}, {9, 3, 2}};
    vector<int> result = vector_concat(data.a, data.b);
    assert(result.size() == data.expected.size() && "Result size should match expected");
    for (size_t i = 0; i < result.size(); i++)
        assert(result[i] == data.expected[i] && "Result elements should match expected");
}

TEST(test_vector_concat_empty_second_vector) {
    test_vector_concat_TestData data = {{4, 2, 6}, {}, {4, 2, 6}};
    vector<int> result = vector_concat(data.a, data.b);
    assert(result.size() == data.expected.size() && "Result size should match expected");
    for (size_t i = 0; i < result.size(); i++)
        assert(result[i] == data.expected[i] && "Result elements should match expected");
}

TEST(test_vector_concat_both_empty_vectors) {
    test_vector_concat_TestData data = {{}, {}, {}};
    vector<int> result = vector_concat(data.a, data.b);
    assert(result.empty() && "Result should be empty");
}

TEST(test_vector_concat_duplicate_elements) {
    test_vector_concat_TestData data = {{2, 2}, {2, 2}, {2, 2, 2, 2}};
    vector<int> result = vector_concat(data.a, data.b);
    assert(result.size() == data.expected.size() && "Result size should match expected");
    for (size_t i = 0; i < result.size(); i++)
        assert(result[i] == data.expected[i] && "Result elements should match expected");
}

#endif