#pragma once

#include <vector>

using namespace std;

// Template function to compare two vectors for equality
template<typename T>
bool vector_equal(const vector<T>& a, const vector<T>& b) {
    // First, check if the sizes of the vectors are equal
    if (a.size() != b.size()) return false;

    // Then, compare each element of the vectors
    for (size_t i = 0; i < a.size(); ++i)
        if (a[i] != b[i]) return false;

    // If all elements are equal, return true
    return true;
}

