#pragma once

#include <vector>
#include <algorithm>
#include <string>

using namespace std;

// Remove all occurrences of elem and return a new vector (non-modifying)
template <typename T>
[[nodiscard]] vector<T> array_remove(const vector<T>& vec, const T& elem) {
    vector<T> result;
    result.reserve(vec.size()); // avoid repeated reallocations

    for (const auto& item : vec)
        if (item != elem)
            result.push_back(item);

    return result;
}

// Convenience overload for string literals when the vector contains strings
template <typename T>
[[nodiscard]] vector<T> array_remove(const vector<T>& vec, const char* elem) {
    return array_remove(vec, T(elem));
}