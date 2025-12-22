#pragma once

#include <vector>

using namespace std;

template<typename T>
vector<T> vector_concat(const vector<T>& a, const vector<T>& b) {
    vector<T> result;
    result.reserve(a.size() + b.size()); // Pre-allocate capacity for efficiency
    result.insert(result.end(), a.begin(), a.end()); // Append all elements from a
    result.insert(result.end(), b.begin(), b.end()); // Append all elements from b
    return result;
}
