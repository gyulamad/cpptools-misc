#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../str_deserialize.hpp"

TEST(test_str_deserialize_empty) {
    vector<char> serialized;
    size_t nxt = 0;
    bool threw = false;
    try {
        str_deserialize(serialized, nxt);
    } catch (exception& e) {
        threw = true;
    }
    assert(threw && "Empty serialized should throw");
}

TEST(test_str_deserialize_single_char) {
    vector<char> serialized;
    // Size: 0x00000001 (1 character)
    serialized.push_back(0x00);
    serialized.push_back(0x00);
    serialized.push_back(0x00);
    serialized.push_back(0x01);
    // Character: 'a'
    serialized.push_back('a');
    
    size_t nxt = 0;
    string result = str_deserialize(serialized, nxt);
    assert(result == "a" && "Single character");
}

TEST(test_str_deserialize_multiple_chars) {
    vector<char> serialized;
    // Size: 0x00000003 (3 characters)
    serialized.push_back(0x00);
    serialized.push_back(0x00);
    serialized.push_back(0x00);
    serialized.push_back(0x03);
    // Characters: 'abc'
    serialized.push_back('a');
    serialized.push_back('b');
    serialized.push_back('c');
    
    size_t nxt = 0;
    string result = str_deserialize(serialized, nxt);
    assert(result == "abc" && "Multiple characters");
}

#endif