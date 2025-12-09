#pragma once

#include <vector>
#include <string>
#include <iostream>

using namespace std;

// Main array_splice implementation
template<typename T>
vector<T> array_splice(
    vector<T>& array,
    int offset,
    int length = numeric_limits<int>::max(),
    const vector<T>& replacement = {}
) {
    vector<T> removed;
    
    // Handle negative offset (from the end)
    if (offset < 0) {
        offset = (int)array.size() + offset;
        if (offset < 0) offset = 0;
    }
    
    // If offset is beyond array size, set to array size
    if (offset > (int)array.size()) {
        offset = (int)array.size();
    }
    
    // Calculate actual length to remove
    int actual_length;
    if (length >= 0) {
        // Positive length
        actual_length = min(length, (int)array.size() - offset);
    } else {
        // Negative length - stop that many elements from the end
        int stop_from_end = -length;
        actual_length = max(0, (int)array.size() - offset - stop_from_end);
    }
    
    // Extract removed elements
    if (actual_length > 0 && offset < (int)array.size()) {
        auto start_it = array.begin() + offset;
        auto end_it = start_it + actual_length;
        removed.assign(start_it, end_it);
        
        // Remove elements from original array
        array.erase(start_it, end_it);
    }
    
    // Insert replacement elements
    if (!replacement.empty()) {
        array.insert(array.begin() + offset, replacement.begin(), replacement.end());
    }
    
    return removed;
}


#ifdef TEST
#include "str_contains.hpp"
#include "vector_equal.hpp"

// Test struct for complex test data (when needed)
struct test_array_splice_TestData {
    vector<string> original;
    int offset;
    int length = numeric_limits<int>::max();
    vector<string> replacement;
    vector<string> expected_result;
    vector<string> expected_removed;
};


TEST(test_array_splice_basic_removal) {
    vector<string> fruits = {"apple", "banana", "cherry", "date"};
    auto removed = array_splice(fruits, 1, 1);
    
    assert(vector_equal(fruits, {"apple", "cherry", "date"}) && 
           "Should remove 'banana' from position 1");
    assert(vector_equal(removed, {"banana"}) && 
           "Should return removed element 'banana'");
}

TEST(test_array_splice_remove_multiple_elements) {
    vector<int> numbers = {1, 2, 3, 4, 5, 6};
    auto removed = array_splice(numbers, 2, 3);
    
    assert(vector_equal(numbers, {1, 2, 6}) && 
           "Should remove elements 3,4,5 from position 2");
    assert(vector_equal(removed, {3, 4, 5}) && 
           "Should return removed elements 3,4,5");
}

TEST(test_array_splice_negative_offset_removes_from_end) {
    vector<string> fruits = {"apple", "banana", "cherry", "date"};
    auto removed = array_splice(fruits, -2, 1);
    
    assert(vector_equal(fruits, {"apple", "banana", "date"}) && 
           "Should remove 'cherry' using negative offset -2");
    assert(vector_equal(removed, {"cherry"}) && 
           "Should return removed element 'cherry'");
}

TEST(test_array_splice_negative_length_stops_from_end) {
    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8};
    auto removed = array_splice(numbers, 1, -2);
    
    // Should remove from index 1 to 4 (6-2=4 elements from start)
    assert(vector_equal(numbers, {1, 7, 8}) && 
           "Should remove elements 2,3,4,5,6");
    assert(removed.size() == 5 && removed[0] == 2 && removed[4] == 6 && 
           "Should return removed elements 2 through 6");
}

TEST(test_array_splice_no_length_removes_to_end) {
    vector<string> fruits = {"apple", "banana", "cherry", "date", "elderberry"};
    auto removed = array_splice(fruits, 2);
    
    assert(vector_equal(fruits, {"apple", "banana"}) && 
           "Should remove from index 2 to end");
    assert(vector_equal(removed, {"cherry", "date", "elderberry"}) && 
           "Should return all removed elements");
}

TEST(test_array_splice_zero_length_inserts_without_removal) {
    vector<string> fruits = {"apple", "banana", "cherry"};
    auto removed = array_splice(fruits, 1, 0, {"orange", "grape"});
    
    assert(vector_equal(fruits, {"apple", "orange", "grape", "banana", "cherry"}) && 
           "Should insert at position 1 without removing");
    assert(removed.empty() && "Should return empty vector when no removal");
}

TEST(test_array_splice_replace_elements) {
    vector<string> fruits = {"apple", "banana", "cherry", "date"};
    auto removed = array_splice(fruits, 1, 2, {"orange", "grape", "kiwi"});
    
    assert(vector_equal(fruits, {"apple", "orange", "grape", "kiwi", "date"}) && 
           "Should replace 2 elements with 3 new ones");
    assert(vector_equal(removed, {"banana", "cherry"}) && 
           "Should return replaced elements");
}

TEST(test_array_splice_single_element_replacement) {
    vector<int> numbers = {1, 2, 3, 4};
    auto removed = array_splice(numbers, 1, 1, { 99 }); // Single int replacement
    
    assert(vector_equal(numbers, {1, 99, 3, 4}) && 
           "Should replace element at position 1 with 99");
    assert(vector_equal(removed, {2}) && 
           "Should return replaced element 2");
}

TEST(test_array_splice_initializer_list_replacement) {
    vector<int> numbers = {1, 2, 3, 4};
    auto removed = array_splice(numbers, 2, 1, {10, 11, 12});
    
    assert(vector_equal(numbers, {1, 2, 10, 11, 12, 4}) && 
           "Should replace one element with three");
    assert(vector_equal(removed, {3}) && 
           "Should return replaced element 3");
}

TEST(test_array_splice_offset_beyond_size_appends_replacement) {
    vector<string> fruits = {"apple", "banana"};
    auto removed = array_splice(fruits, 5, 1, {"cherry"});
    
    assert(vector_equal(fruits, {"apple", "banana", "cherry"}) && 
           "Should append 'cherry' when offset beyond size");
    assert(removed.empty() && "Should return empty when nothing to remove");
}

TEST(test_array_splice_negative_offset_beyond_bounds) {
    vector<string> fruits = {"apple", "banana"};
    auto removed = array_splice(fruits, -10, 1);
    
    assert(vector_equal(fruits, {"banana"}) && 
           "Should remove from start when offset beyond negative bounds");
    assert(vector_equal(removed, {"apple"}) && 
           "Should return removed element 'apple'");
}

TEST(test_array_splice_length_exceeds_available_elements) {
    vector<int> numbers = {1, 2, 3};
    auto removed = array_splice(numbers, 1, 10);
    
    assert(vector_equal(numbers, {1}) && 
           "Should remove all available elements from position 1");
    assert(vector_equal(removed, {2, 3}) && 
           "Should return all removed elements");
}

TEST(test_array_splice_empty_array_returns_empty) {
    vector<string> empty_vec;
    auto removed = array_splice(empty_vec, 0, 1, {"test"});
    
    assert(vector_equal(empty_vec, {"test"}) && 
           "Should insert 'test' into empty array");
    assert(removed.empty() && "Should return empty vector");
}

TEST(test_array_splice_with_test_struct_example) {
    // Example using test struct for complex test case
    test_array_splice_TestData data = {
        {"a", "b", "c", "d", "e", "f"}, // original
        1,                               // offset
        -2,                              // length (negative)
        {"x", "y", "z"},                 // replacement
        {"a", "x", "y", "z", "e", "f"}, // expected_result
        {"b", "c", "d"}                  // expected_removed
    };
    
    vector<string> array = data.original;
    auto removed = array_splice(array, data.offset, data.length, data.replacement);
    
    assert(vector_equal(array, data.expected_result) && 
           "Complex test with negative length and replacement");
    assert(vector_equal(removed, data.expected_removed) && 
           "Should return correct removed elements");
}

TEST(test_array_splice_int_types) {
    vector<int> numbers = {10, 20, 30, 40, 50};
    auto removed = array_splice(numbers, 1, 2, {25, 35});
    
    assert(vector_equal(numbers, {10, 25, 35, 40, 50}) && 
           "Should work with int types");
    assert(vector_equal(removed, {20, 30}) && 
           "Should return removed ints");
}

TEST(test_array_splice_double_types) {
    vector<double> values = {1.1, 2.2, 3.3, 4.4};
    auto removed = array_splice(values, 0, 2, {0.5, 1.5});
    
    assert(values.size() == 4 && values[0] == 0.5 && values[3] == 4.4 && 
           "Should work with double types");
    assert(removed.size() == 2 && removed[0] == 1.1 && removed[1] == 2.2 && 
           "Should return removed doubles");
}

TEST(test_array_splice_edge_case_zero_length_with_replacement_at_end) {
    vector<string> fruits = {"apple", "banana"};
    auto removed = array_splice(fruits, 2, 0, {"cherry"});
    
    assert(vector_equal(fruits, {"apple", "banana", "cherry"}) && 
           "Should append when offset equals size and length is 0");
    assert(removed.empty() && "Should return empty vector");
}

#endif
