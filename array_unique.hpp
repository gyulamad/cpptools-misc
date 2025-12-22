#pragma once

#include <unordered_set>
#include <vector>

using namespace std;

template <typename T>
vector<T> array_unique(const vector<T>& vec) {
    // Use an unordered_set to filter out duplicates
    unordered_set<T> seen;
    vector<T> result;

    // Reserve space for the result vector to avoid repeated reallocations
    result.reserve(vec.size());

    // Iterate through the input vector
    for (const auto& element : vec)
        // Insert the element into the set (if it's not already present)
        if (seen.insert(element).second)
            // If the element was inserted (i.e., it's unique), add it to the result
            result.push_back(element);

    return result;
}
