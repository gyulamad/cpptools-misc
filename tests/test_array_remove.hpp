#pragma once

#include "../TEST.hpp"
#include "../array_remove.hpp"

#ifdef TEST

#include "../str_contains.hpp"  // Only when testing
#include "../array_remove.hpp"  // Include the real implementation
#include <vector>
#include <string>

using namespace std;

// Test data struct for complex case
struct test_array_remove_multiple_occurrences_TestData {
    vector<string> input;
    string to_remove;
    vector<string> expected;
};

TEST(test_array_remove_basic_removal) {
    vector<string> arr = {"apple", "mango", "banana"};
    vector<string> result = array_remove(arr, "mango");

    assert(result.size() == 2 && "Should have 2 elements after removal");
    assert(result[0] == "apple" && "First element should be apple");
    assert(result[1] == "banana" && "Second element should be banana");
}

TEST(test_array_remove_multiple_occurrences) {
    test_array_remove_multiple_occurrences_TestData data = {
        {"apple", "mango", "banana", "mango", "apple"},
        "mango",
        {"apple", "banana", "apple"}
    };

    vector<string> result = array_remove(data.input, data.to_remove);

    assert(result == data.expected && "All occurrences of 'mango' should be removed");
}

TEST(test_array_remove_element_not_present) {
    vector<string> arr = {"apple", "banana", "orange"};
    vector<string> result = array_remove(arr, "mango");

    assert(result == arr && "Vector should remain unchanged when element is not present");
}

TEST(test_array_remove_empty_vector) {
    vector<string> arr = {};
    vector<string> result = array_remove(arr, "anything");

    assert(result.empty() && "Removing from empty vector should return empty vector");
}

TEST(test_array_remove_single_element_match) {
    vector<string> arr = {"solo"};
    vector<string> result = array_remove(arr, "solo");

    assert(result.empty() && "Removing the only element should result in empty vector");
}

TEST(test_array_remove_single_element_no_match) {
    vector<string> arr = {"solo"};
    vector<string> result = array_remove(arr, "other");

    assert(result.size() == 1 && "Vector should remain unchanged");
    assert(result[0] == "solo" && "The single element should still be present");
}

TEST(test_array_remove_with_string_literal) {
    vector<string> arr = {"apple", "mango", "banana"};
    vector<string> result = array_remove(arr, "mango");  // Uses const char* overload

    assert(result.size() == 2 && "Should work with const char* overload");
    assert(result[0] == "apple" && "First remaining element should be apple");
    assert(result[1] == "banana" && "Second remaining element should be banana");
}

TEST(test_array_remove_original_vector_unchanged) {
    vector<string> original = {"apple", "mango", "banana"};
    vector<string> copy = original;

    vector<string> result = array_remove(original, "mango");

    assert(original == copy && "Original vector must not be modified");
}

#endif // TEST