#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../array_reverse.hpp"

// ============================================================
// Sequential container (vector) tests - no key preservation needed
// ============================================================

TEST(test_array_reverse_vector_basic) {
    vector<int> data = {1, 2, 3, 4, 5};
    auto result = array_reverse(data);
    
    assert(result.size() == 5 && "Result size should be 5");
    assert(result[0] == 5 && "First element should be 5");
    assert(result[1] == 4 && "Second element should be 4");
    assert(result[2] == 3 && "Third element should be 3");
    assert(result[3] == 2 && "Fourth element should be 2");
    assert(result[4] == 1 && "Fifth element should be 1");
}

TEST(test_array_reverse_vector_empty) {
    vector<int> data;
    auto result = array_reverse(data);
    
    assert(result.empty() && "Result should be empty for empty input");
    assert(result.size() == 0 && "Result size should be 0");
}

TEST(test_array_reverse_vector_single_element) {
    vector<string> data = {"only"};
    auto result = array_reverse(data);
    
    assert(result.size() == 1 && "Result size should be 1");
    assert(result[0] == "only" && "Single element should remain the same");
}

TEST(test_array_reverse_vector_two_elements) {
    vector<int> data = {10, 20};
    auto result = array_reverse(data);
    
    assert(result.size() == 2 && "Result size should be 2");
    assert(result[0] == 20 && "First element should be 20");
    assert(result[1] == 10 && "Second element should be 10");
}

TEST(test_array_reverse_vector_strings) {
    vector<string> data = {"apple", "banana", "cherry"};
    auto result = array_reverse(data);
    
    assert(result.size() == 3 && "Result size should be 3");
    assert(result[0] == "cherry" && "First element should be cherry");
    assert(result[1] == "banana" && "Second element should be banana");
    assert(result[2] == "apple" && "Third element should be apple");
}

TEST(test_array_reverse_vector_preserves_values_not_original) {
    vector<double> data = {1.5, 2.5, 3.5};
    auto result = array_reverse(data);
    
    assert(result.size() == 3 && "Result size should be 3");
    assert(result[0] == 3.5 && "First element should be 3.5");
    assert(result[1] == 2.5 && "Second element should be 2.5");
    assert(result[2] == 1.5 && "Third element should be 1.5");
}

// ============================================================
// Integer-keyed map tests - preserve_keys matters
// ============================================================

TEST(test_array_reverse_int_map_no_preserve) {
    map<int, string> data = {{0, "a"}, {1, "b"}, {2, "c"}};
    auto result = array_reverse(data, false);
    
    assert(result.size() == 3 && "Result size should be 3");
    // Values reversed: c, b, a with keys renumbered from 0
    assert(result[0] == "c" && "Key 0 should map to 'c' (reversed first)");
    assert(result[1] == "b" && "Key 1 should map to 'b'");
    assert(result[2] == "a" && "Key 2 should map to 'a' (reversed last)");
}

TEST(test_array_reverse_int_map_preserve_keys_true) {
    map<int, string> data = {{0, "a"}, {1, "b"}, {2, "c"}};
    auto result = array_reverse(data, true);
    
    assert(result.size() == 3 && "Result size should be 3");
    // Values reversed but keys preserved: key 2->c, key 1->b, key 0->a
    assert(result[0] == "a" && "Key 0 should still map to 'a' (key preserved)");
    assert(result[1] == "b" && "Key 1 should still map to 'b'");
    assert(result[2] == "c" && "Key 2 should still map to 'c'");
}

TEST(test_array_reverse_int_map_non_sequential_keys_no_preserve) {
    map<int, string> data = {{10, "a"}, {20, "b"}, {30, "c"}};
    auto result = array_reverse(data, false);
    
    assert(result.size() == 3 && "Result size should be 3");
    // Values reversed: c, b, a with keys renumbered from 0
    assert(result[0] == "c" && "Key 0 should map to 'c' (reversed first)");
    assert(result[1] == "b" && "Key 1 should map to 'b'");
    assert(result[2] == "a" && "Key 2 should map to 'a' (reversed last)");
}

TEST(test_array_reverse_int_map_non_sequential_keys_preserve) {
    map<int, string> data = {{10, "a"}, {20, "b"}, {30, "c"}};
    auto result = array_reverse(data, true);
    
    assert(result.size() == 3 && "Result size should be 3");
    // Values reversed but original keys preserved: key 30->c, key 20->b, key 10->a
    assert(result[10] == "a" && "Key 10 should map to 'a' (key preserved)");
    assert(result[20] == "b" && "Key 20 should map to 'b'");
    assert(result[30] == "c" && "Key 30 should map to 'c'");
}

TEST(test_array_reverse_int_map_empty) {
    map<int, string> data;
    auto result = array_reverse(data, false);
    
    assert(result.empty() && "Result should be empty for empty input");
    assert(result.size() == 0 && "Result size should be 0");
}

TEST(test_array_reverse_int_map_single_element) {
    map<int, string> data = {{5, "only"}};
    auto result = array_reverse(data, false);
    
    assert(result.size() == 1 && "Result size should be 1");
    if (!result.empty()) {
        // Key preserved when single element (key type is int but only one entry)
        auto it = result.begin();
        assert(it->second == "only" && "Value should be 'only'");
    }
}

// ============================================================
// String-keyed map tests - keys always preserved regardless of flag
// ============================================================

TEST(test_array_reverse_string_map_no_preserve_flag_ignored) {
    map<string, int> data = {{"apple", 1}, {"banana", 2}, {"cherry", 3}};
    auto result = array_reverse(data, false);
    
    assert(result.size() == 3 && "Result size should be 3");
    // String keys always preserved regardless of preserve_keys flag
    assert(result["apple"] == 1 && "Key 'apple' should map to 1");
    assert(result["banana"] == 2 && "Key 'banana' should map to 2");
    assert(result["cherry"] == 3 && "Key 'cherry' should map to 3");
}

TEST(test_array_reverse_string_map_preserve_flag_true) {
    map<string, int> data = {{"apple", 1}, {"banana", 2}, {"cherry", 3}};
    auto result = array_reverse(data, true);
    
    assert(result.size() == 3 && "Result size should be 3");
    // String keys always preserved
    assert(result["apple"] == 1 && "Key 'apple' should map to 1");
    assert(result["banana"] == 2 && "Key 'banana' should map to 2");
    assert(result["cherry"] == 3 && "Key 'cherry' should map to 3");
}

TEST(test_array_reverse_string_map_empty) {
    map<string, int> data;
    auto result = array_reverse(data);
    
    assert(result.empty() && "Result should be empty for empty input");
    assert(result.size() == 0 && "Result size should be 0");
}

TEST(test_array_reverse_string_map_single_element) {
    map<string, int> data = {{"key", 42}};
    auto result = array_reverse(data);
    
    assert(result.size() == 1 && "Result size should be 1");
    assert(result["key"] == 42 && "Key 'key' should map to 42");
}

// ============================================================
// unordered_map tests (integer-keyed)
// ============================================================

TEST(test_array_reverse_unordered_map_no_preserve) {
    unordered_map<int, string> data = {{0, "a"}, {1, "b"}, {2, "c"}};
    auto result = array_reverse(data, false);
    
    assert(result.size() == 3 && "Result size should be 3");
    // Values reversed: c, b, a with keys renumbered from 0 (map output is sorted by key)
    assert(result[0] == "c" && "Key 0 should map to 'c' (reversed first)");
    assert(result[1] == "b" && "Key 1 should map to 'b'");
    assert(result[2] == "a" && "Key 2 should map to 'a' (reversed last)");
}

TEST(test_array_reverse_unordered_map_preserve_keys) {
    unordered_map<int, string> data = {{0, "a"}, {1, "b"}, {2, "c"}};
    auto result = array_reverse(data, true);
    
    assert(result.size() == 3 && "Result size should be 3");
    // Values reversed but keys preserved (map output is sorted by key)
    assert(result[0] == "a" && "Key 0 should map to 'a' (key preserved)");
    assert(result[1] == "b" && "Key 1 should map to 'b'");
    assert(result[2] == "c" && "Key 2 should map to 'c'");
}

// ============================================================
// Original container is not modified
// ============================================================

TEST(test_array_reverse_does_not_modify_input_vector) {
    vector<int> data = {1, 2, 3};
    auto original_data = data;
    
    [[maybe_unused]] auto result = array_reverse(data);
    
    assert(data == original_data && "Original vector should not be modified");
}

TEST(test_array_reverse_does_not_modify_input_map) {
    map<int, string> data = {{0, "a"}, {1, "b"}};
    auto original_a = data[0];
    auto original_b = data[1];
    
    [[maybe_unused]] auto result = array_reverse(data);
    
    assert(data[0] == original_a && "Original map value for key 0 should not change");
    assert(data[1] == original_b && "Original map value for key 1 should not change");
}

#endif
