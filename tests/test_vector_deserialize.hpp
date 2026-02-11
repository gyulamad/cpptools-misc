#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../vector_deserialize.hpp"

TEST(test_vector_deserialize_empty) {
    vector<char> serialized;
    size_t nxt = 0;
    bool threw = false;
    try {
        vector<char> result = vector_deserialize<char>(serialized, nxt);
    } catch (exception& e) {
        threw = true;
    }
    assert(threw && "Empty serialized should throw");
}

TEST(test_vector_deserialize_single_element) {
    vector<char> serialized;
    // Size: 0x00000001 (1 element)
    serialized.push_back(0x00);
    serialized.push_back(0x00);
    serialized.push_back(0x00);
    serialized.push_back(0x01);
    // Element: 'A' (1 byte)
    serialized.push_back('A');
    
    size_t nxt = 0;
    vector<char> result = vector_deserialize<char>(serialized, nxt);
    assert(result.size() == 1 && "Single element vector");
    assert(result[0] == 'A' && "First element value");
}

TEST(test_vector_deserialize_multiple_elements) {
    vector<char> serialized;
    // Size: 0x00000002 (2 elements)
    serialized.push_back(0x00);
    serialized.push_back(0x00);
    serialized.push_back(0x00);
    serialized.push_back(0x02);
    // Elements: 'a', 'b' (1 byte each)
    serialized.push_back('a');
    serialized.push_back('b');
    
    size_t nxt = 0;
    vector<char> result = vector_deserialize<char>(serialized, nxt);
    assert(result.size() == 2 && "Two element vector");
    assert(result[0] == 'a' && "First element value");
    assert(result[1] == 'b' && "Second element value");
}

#endif