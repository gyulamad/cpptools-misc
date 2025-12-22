#pragma once

#include <vector>

using namespace std;

template<typename T>
vector<T> array_slice(const vector<T>& original, size_t offset, size_t length) {
    if (offset >= original.size()) return {}; // Offset out of bounds -> empty result

    size_t end = offset + length;
    if (end > original.size()) end = original.size(); // Clamp to vector size

    return vector<T>(original.begin() + offset, original.begin() + end);
}

template<typename T>
vector<T> array_slice(const vector<T>& original, size_t offset) {
    return array_slice(original, offset, original.size() - offset);
}
