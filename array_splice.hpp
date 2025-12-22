#pragma once

#include <vector>
#include <limits>

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

