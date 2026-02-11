#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../vector_serialize.hpp"

TEST(test_vector_serialize_empty) {
    vector<int> vec;
    vector<char> result = vector_serialize<char>(vec);
    assert(result.size() == 4 && "Empty vector should serialize size only");
}

TEST(test_vector_serialize_single_element) {
    vector<int> vec = {42};
    vector<char> result = vector_serialize<char>(vec);
    assert(result.size() == 5 && "Single element should be 4 bytes size + 1 element");
}

TEST(test_vector_serialize_multiple_elements) {
    vector<int> vec = {1, 2, 3};
    vector<char> result = vector_serialize<char>(vec);
    assert(result.size() == 7 && "Three elements should be 4 bytes size + 3 elements");
}

#endif