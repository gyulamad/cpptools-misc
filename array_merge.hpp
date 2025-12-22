#pragma once

#include "foreach.hpp"
#include "in_array.hpp"

using namespace std;

// Function to merge two vectors
template <typename T>
vector<T> array_merge(const vector<T>& vec1, const vector<T>& vec2) {
    vector<T> results = vec1;
    foreach (vec2, [&](const T& item) { if (!in_array(item, results)) results.push_back(item); });
    return results;
}
